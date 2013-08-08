/*****************************************************************************
 *
 *   (C) Copyright Wintegra 2000-2004.  All rights reserved.
 *
 *   This program is derived from free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version
 *   2 of the License, or (at your option) any later version.
 *
 *  THIS SOFTWARE IS PROVIDED BY WINTEGRA ``AS IS'' AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED.  IN NO EVENT SHALL WINTEGRA BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 *  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *  SUCH DAMAGE.
 *
 *
 ****************************************************************************/

/*****************************************************************************
 *
 * File: ./src/tun/wls_tun.c
 *
 * Purpose: Implement interface between WDDI and Linux tun driver.
 *
 * Functions contained in this file:
 *
 * +---------------------------+----------------------------------------------
 * |  Function name            | Description
 * +---------------------------+----------------------------------------------
 * |
 * |  API Functions
 * |  -------------------------+----------------------------------------------
 * |  (None)
 * |  -------------------------+----------------------------------------------
 * |
 * |  Internal WLS Functions
 * |  -------------------------+----------------------------------------------
 * |  (None)
 * |  -------------------------+----------------------------------------------
 * |
 * |  Internal WLS Functions (static)
 * |  -------------------------+----------------------------------------------
 * |  (None)
 * +---------------------------+----------------------------------------------
 *
 ****************************************************************************/

#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/if_ether.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <errno.h>

#include "wp_wddi.h"

WP_handle rx_chan;
WP_handle tx_handle;
WP_handle tx_pool;
WP_handle qnode_handle;

#define MAX_T 500
#define IW_MTU 1536

typedef struct
{
      WP_U8      my_mac[6];
      WP_U32     ip;
      WP_U32     gw;
} WSI_tun_shadow;

WSI_tun_shadow tun_shadow;
int tun_fd;

void  WSI_TunShadowMyAddrSet(WP_U8 * mac,
                             WP_U32 ip,
                             WP_U32 gw);

void tun_rx_event(WP_tag tag, WP_U32 data, WP_U32 info);

int get_hwaddr(WP_CHAR *interface, WP_U8 * buf)
{
   int sd;

   struct ifreq req;
   sd = socket(AF_INET,SOCK_DGRAM,0);
   if (sd == -1)
   {
      perror("socket()");
      return -1;
   }
   sprintf(req.ifr_name,"%s",interface);
   if (ioctl(sd,SIOCGIFHWADDR,&req) == -1)  {
      perror("ioctl():SIOCGIFHWADDR");
      close(sd);
      return -1;
   }

   memcpy(buf, req.ifr_hwaddr.sa_data, 6);

   close(sd);

   return 0;
}

/*****************************************************************************
 * Function name: low_level_init
 * Description  : Initialize the TUN driver to WDDI interface..
 * Input  params: gw - default gateway address.
 *                ip - interface ip address.
 * Output params: none.
 * Return val   : none.
 *****************************************************************************/
static void low_level_init(char *gw, char *ip)
{
   WP_U8 my_mac[6];

   char buf[100];
   struct ifreq ifr;
   int err;

   /* init the TUN interface */
   tun_fd = open("/dev/net/tun",O_RDWR);
   if (tun_fd == -1)
   {
      printf("fail to open tun device\n");
      exit(1);
   }

   memset(&ifr, 0, sizeof(ifr));

   ifr.ifr_flags = IFF_TAP | IFF_NO_PI;

   if( (err = ioctl(tun_fd, TUNSETIFF, (void *) &ifr)) < 0 )
   {
      printf("Cannot allocate TUN device\n");
      close(tun_fd);
      exit(1);
   }

   if (get_hwaddr(ifr.ifr_name, my_mac) == 1)
   {
      printf("Cannot extract hw addr\n");
      close(tun_fd);
      exit(1);
   }

   snprintf(buf,sizeof(buf),"ifconfig %s inet %s %s",
            ifr.ifr_name, gw, ip);
   system(buf);

   /* Save global params */
   {
      WP_U32 gw_val,ip_val;
      WP_U32 a,b,c,d;
      WP_U8 *ip_ptr = (WP_U8 *)&ip_val;
      WP_U8 *gw_ptr = (WP_U8 *)&gw_val;

      sscanf(((const char *)ip),"%d.%d.%d.%d", &a, &b, &c, &d);
      ip_ptr[0] = a;
      ip_ptr[1] = b;
      ip_ptr[2] = c;
      ip_ptr[3] = d;

      sscanf(((const char *)gw),"%d.%d.%d.%d", &a, &b, &c, &d);
      gw_ptr[0] = a;
      gw_ptr[1] = b;
      gw_ptr[2] = c;
      gw_ptr[3] = d;

      WSI_TunShadowMyAddrSet(my_mac, ip_val, gw_val);
   }
}

/*****************************************************************************
 * Function name: low_level_output_joker
 * Description  : send packet from TUN to WDDI (all exsiting interfaces).
 * Input  params: pointer to buffer, size.
 * Output params: none.
 * Return val   : 0 on success.
 *****************************************************************************/
static int low_level_output_joker(WP_U8 * buf, WP_U16 size)
{
   WP_data_unit     data_unit;
   WP_data_segment  segment;
   WP_U16           len;
   /* WP_U16 tmp=0; */
   WP_status status;

   len = (size < 64) ? 64 : size;

   memset(&data_unit, 0, sizeof(WP_data_unit));

   /* prepare the data unit */
   data_unit.segment   = &segment;
   data_unit.n_segments= 1;
   data_unit.n_active  = 1;
   data_unit.data_size = len;
   data_unit.type      = WP_DATA_IW;
   data_unit.control   = 0;

   memset(&segment, 0, sizeof(WP_data_segment));

   /* prepare data_unit for transmit */
   segment.next = NULL;
   segment.data_size   = len;
   segment.pool_handle = tx_pool;
   segment.data = buf;
   memcpy(segment.data,buf,len);

   /* printf("sending DATA UNIT: ");
   while (tmp < len)
   {
      printf("%x",segment.data[tmp++]);
   }
   printf("\n"); */

   /* send the packet */
   status = WP_HostSendDirect(tx_handle,
                              0,
                              qnode_handle,
                              &data_unit);

   if (status != WP_OK)
   {
      printf("Host send failed!\n");
      printf("%s\n",WP_error_name[WP_ERROR(status)]);

      return -1;
   }

   return 0;
}

/*****************************************************************************
 * Function name: low_level_output
 * Description  : send packet from TUN to WDDI.
 * Input  params: none.
 * Output params: none.
 * Return val   : none.
 *****************************************************************************/
static int low_level_output(void)
{
   WP_U16 len;
   WP_U8 *buffer;
   /* printf("low_level_output\n"); */

   /* copy data to data unit */
   buffer = WP_PoolAlloc(tx_pool);
   if (buffer == NULL)
   {
      printf("unable to allocate buffer!\n");
      return -1;
   }

   /* read data from tun */
   len = read(tun_fd,buffer,IW_MTU);

   /* send the packet */
   return low_level_output_joker(buffer,len);
}

/*****************************************************************************
 * Function name: low_level_input
 * Description  : send packet from WDDI to TUN.
 * Input  params: none.
 * Output params: none.
 * Return val   : none.
 *****************************************************************************/
static int low_level_input(void)
{
   WP_U16 len;
   /* tmp=0; */
   WP_data_unit     data_unit;
   WP_data_segment  segment;
   WP_status status;
   ssize_t ret_val;
   WP_U32           counter;

   /* printf("low_level_input\n"); */

   /* receive the data_unit */
   data_unit.segment = &segment;
   segment.next = NULL;
   segment.displacement = 0;
   data_unit.n_segments = 1;
   data_unit.type = WP_DATA_IW;

   for (counter = 0; counter < (MAX_T/2) ; counter++)
   {
      status = WP_HostReceive(rx_chan,&data_unit);

      if (WP_ERROR(status) == WP_ERR_HST_NO_DATA_TO_GET)
      {
         break;
      }

      if (WP_ERROR_P(status))
      {
         printf("Rx Err!!!\n");

         return -1;
      }

      /* get the length from the data_unit */
      len = data_unit.data_size; /* CRC */

      /* send the buffer to the tun driver */
      ret_val = write(tun_fd,segment.data,len);
      if(ret_val == -1)
      {
         /* free the data_unit buffer */
         status = WP_PoolFree(segment.pool_handle,
                              segment.data);
         printf("cannot enqueue to driver\n");
         break;
      }
/*
      else
      {
         printf("data enqueued successfuly to driver\n");
         printf("len %d enqueued DATA UNIT: ", (WP_U32) len);
         tmp=0;
         while (tmp < len)
         {
            printf("%x",segment.data[tmp++]);
         }
         printf("\n");
      }
*/
      /* free the data_unit buffer */
      status = WP_PoolFree(segment.pool_handle,
                           segment.data);
   }

   return 0;
}

/*****************************************************************************
 * Function name: tun_tread
 * Description  : TUN service tread main function.
 * Input  params: none.
 * Output params: none.
 * Return val   : none.
 *****************************************************************************/
static void * tun_tread(void)
{
   fd_set fdset;
   int ret;
   void * ptr=NULL;
   
   while(1)
   {
      FD_ZERO(&fdset);
      FD_SET(tun_fd,&fdset);

      /* wait for a packet to arrive */
      ret = select(tun_fd+1,&fdset,NULL,NULL,NULL);
      if (ret == 1)
         low_level_output();
      else
      {
         if (errno != EINTR)
         {
            printf("select error\n");
            printf("errno %x\n\n", (WP_U32)errno);
         }
      }
   }

   return ptr;
}

/*****************************************************************************
 * Function name: tun_rx_event
 * Description  : WDDI receive call back.
 * Input  params: none.
 * Output params: none.
 * Return val   : none.
 *****************************************************************************/
void tun_rx_event(WP_tag tag, WP_U32 data, WP_U32 info)
{
   /* call to internal handler */
   low_level_input();
}

/*****************************************************************************
 * Function name: tun_init
 * Description  : TUN interface init function
 * Input  params: gw - default gateway address.
 *                ip - interface ip address.
 * Output params: none.
 * Return val   : none.
 *****************************************************************************/
void tun_init(char *gw, char * ip)
{
   low_level_init(gw,ip);
}

/* Set system addresses */
void  WSI_TunShadowMyAddrSet(WP_U8 * mac,
                             WP_U32 ip,
                             WP_U32 gw)
{
   memcpy(tun_shadow.my_mac, mac,6);
   tun_shadow.ip = ip;
   tun_shadow.gw = gw;
}

/* Get system mac */
void  WSI_TunShadowMyMacGet(WP_U8 * mac)
{
  memcpy(mac,tun_shadow.my_mac,6);
}

/* Get system ip */
WP_U32  WSI_TunShadowMyIpGet(void)
{
   return tun_shadow.ip;
}

/* Get system gw */
WP_U32   WSI_TunShadowMyGwGet(void)
{
   return tun_shadow.gw;
}
