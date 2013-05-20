/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra, Inc.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/
/*****************************************************************************
 *
 * Module: $RCSfile: wpv_sockint.c,v $
 * Purpose:
 *      This file implements the driver/simulator interface using
 *  sockets.
 *
 * Module Originator: Arnon Mordoh (Login Name: 'arnonm')
 * Creation Date: Oct 23, 2000
 *
 * Change History:
 * +------------+--------------------+----------------------------------------
 * | Date       | By                 | Description
 * +------------+--------------------+----------------------------------------
 * |            |                    |
 * +------------+--------------------+----------------------------------------
 * |            |                    |
 * +------------+--------------------+----------------------------------------
 *
 ****************************************************************************/
#include <wpl_platform.h>

#undef int
#undef long
#undef char
#undef short
#undef signed
#undef unsigned
#undef float
#undef double

#define WPI_SOCKET_DEBUG_LEVEL WPI_DEBUG_LEVEL_5

#define SOCKET_DATA(label,address,pointer,size) do { int sdi; \
 unsigned char *p = (unsigned char *) pointer; \
WPLI_LOG(("%20s %#8.8x ", label, address)); \
for (sdi = 0; sdi < size; sdi++) \
{WPLI_LOG(("%2.2x", *p)); \
p++; } \
WPLI_LOG(("\n")); } while (0)

int winpath_simulator_port = 0;

#if defined ( __CYGWIN32__ ) || defined ( __CYGWIN__ ) || defined(_MSC_VER)
#ifndef WIN32
#define WIN32
#endif
#endif

#if defined(WIN32) || defined(__linux__)
#ifndef OS_LITTLE_ENDIAN
#define OS_LITTLE_ENDIAN
#endif
#endif




#ifdef WIN32
#include <winsock.h>
/* I had to make a change in the winsock.h file for cygwin 1.3.1
 * removed if ifdef that defines the fd_set type
 * Arnon 15/5/2001
 */
#else
#include <sys/poll.h>
#endif

#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#ifdef __cplusplus
extern "C"
{
   int close(int fildes);
}
#endif

#ifdef WIN32
#define LastError WSAGetLastError()
#define WOULDBLOCK  WSAEWOULDBLOCK
#define INPROGRESS  WSAEINPROGRESS
#define CONNREFUSED WSAECONNREFUSED
#include <sys/unistd.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR   -1
#define LastError errno
#define WOULDBLOCK EWOULDBLOCK
#define INPROGRESS EINPROGRESS
#define CONNREFUSED ECONNREFUSED

typedef struct hostent *PHOSTENT;
typedef struct in_addr IN_ADDR;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr *PSOCKADDR;

typedef int SOCKET;
#endif

int CycleCount;

#include "sockint.h"

#define CONNECTION_TIMEOUT 0x72000000
#define CONNECTION_RETRIES 5
#define HOST_NAME_LEN  50
#define MESSAGE_LENGTH 256

#define NO_CONNECTION          0
#define WAITING_FOR_CONNECTION 1
#define CONNECTED              2

#ifdef PLI_INTERFACE
#define printf io_printf
#define TraceError io_printf( \
"Sockint error %d: %s\n",LastError,ErrnoImage(LastError))
#else
#define TraceError \
WPLI_LOG(("Sockint error %d: %s\n",LastError,ErrnoImage(LastError)))
#endif

static int DbgTrace = 0;
static SOCKET CSsoc;


        /*
         * Message between client and server format
         *  (Message is always in big endian):
         *   [0]  opcode:   1  - read   2 - write 3 - ack
         *   [1]  length  0 - 119
         *   [2]  reserved
         *   [3]  reserved
         *   [4-7] Start address
         *   [8...] data
         */
static unsigned char Message[MESSAGE_LENGTH];

#define MSG_OPCODE  0
#define MSG_LENGTH  1
#define MSG_ADDRESS 4
#define MSG_DATA    8

#define MSG_OP_READ     1
#define MSG_OP_WRITE    2
#define MSG_OP_ACK      3
#define MSG_OP_BYE      4
#define MSG_OP_SHUT     5
#define MSG_OP_CMD_FILE 6

#define MSG_CMD_SIZE 1

#define MSG_MAX_SIZE MESSAGE_LENGTH-8

char *ErrnoImage(int ErrCode);
void SimReadBlock(unsigned int,
                  unsigned char *,
                  unsigned char,
                  int);
void SimWriteBlock(unsigned int,
                   unsigned char *,
                   unsigned char,
                   int);
int WaitForAck(void);

#ifdef OS_LITTLE_ENDIAN
#define MK_LONG(x) SwapLong(x)
#define MK_SHORT(x) SwapHalf(x)
#else
#define MK_LONG(x) x
#define MK_SHORT(x) x
#endif

#ifdef OS_LITTLE_ENDIAN
int SwapLong(int x)
{
   int result = 0;
   int i;

   for (i = 0; i < sizeof(x); i++)
   {
      result <<= 8;
      result |= (x & 0xff);
      x >>= 8;
   }
   return result;
}

short SwapHalf(short x)
{
   short result = 0;
   int i;

   for (i = 0; i < sizeof(x); i++)
   {
      result <<= 8;
      result |= (x & 0xff);
      x >>= 8;
   }
   return result;
}
#endif

static void ExitSocketError( int stat )
{
   close( CSsoc );
   exit( stat );
}




char *LocalIPAddress(void)
{
   char HostName[HOST_NAME_LEN];
   PHOSTENT phostent;

   if (gethostname(HostName, HOST_NAME_LEN) != 0)
      TraceError;
   else
   {
      if ((phostent = gethostbyname(HostName)) == NULL)
         TraceError;
      else
      {
         IN_ADDR in;

         memcpy(&in, phostent->h_addr, 4);

         if (DbgTrace)
         {
            WPLI_LOG(( "Host %s has IP address ", phostent->h_name));
            WPLI_LOG(( "%s\n", inet_ntoa(in)));
         }
         return (char *) inet_ntoa(in);
      }
   }
   return NULL;
}

static int WPI_ReadWriteTest(void)
{
   int st;
   unsigned int address = 0;
   unsigned int pvalue;

   Message[MSG_OPCODE] = MSG_OP_READ;
   Message[MSG_LENGTH] = 4;
   *((unsigned int *) &Message[MSG_ADDRESS]) = MK_LONG(address);

   st = send(CSsoc, Message, MESSAGE_LENGTH, 0);

   if( st < 0 && LastError == ECONNREFUSED )
      return -1;

   if (st != MESSAGE_LENGTH)
   {
      TraceError;
      return -1;
   }

   if ((st = WaitForAck()) != SOCKINT_OK)
   {
      WPLI_LOG(( "Error acknowledging socket\n"));
      return -1;
   }

   pvalue = MK_LONG(*(unsigned int *) &Message[MSG_DATA]);
   SOCKET_DATA("wpi read long", address, &Message[MSG_DATA], 4);
   return 0;
}




int InitSocketInterface()
{
   int f_times_to_try  = 100;
   int f_reconnect;
   int ret_val = SOCKINT_OK;
   SOCKADDR_IN addr;

   sleep(1);

   do
   {


      char * winpath_simulator_ip;

      int st;
      unsigned long ConnectionTimeOut;

#ifdef WIN32
      unsigned long argp = 0;
      WORD wVersionRequested;
      WSADATA wsaData;
#endif

      f_reconnect = 0;
      if (getenv("WINPATH_SIMULATOR_PORT") != NULL)
         winpath_simulator_port = atoi(getenv("WINPATH_SIMULATOR_PORT"));
      else
         winpath_simulator_port = WINPATH_DEFAULT_SIMULATOR_PORT;

      if (getenv("WINPATH_SIMULATOR_IP") != NULL)
         winpath_simulator_ip = getenv("WINPATH_SIMULATOR_IP");
      else
         winpath_simulator_ip = LocalIPAddress();


         /*
         * Create sockets.
      */
#ifdef WIN32
      wVersionRequested = MAKEWORD(1, 1);
      st = WSAStartup(wVersionRequested, &wsaData);
      if (st != 0)
      {
         WPLI_LOG(( "Could not find winsock library\n"));
         return SOCKINT_ERROR;
      }
#endif

      CSsoc = socket(PF_INET, SOCK_STREAM, 0);
      if (CSsoc == INVALID_SOCKET)
      {
         TraceError;
         return (SOCKINT_ERROR);
      }

      /*
      * Set socket to be an unblocking socket
      */
#ifdef WIN32
      ioctlsocket(CSsoc, FIONBIO, &argp);
#else
      fcntl(CSsoc, F_SETFL, O_NONBLOCK);
#endif

      /*
      * Bind socket to any available network interface on
      * current host.
      */
      addr.sin_family = AF_INET;
      addr.sin_port = 0;

      addr.sin_addr.s_addr = htonl(INADDR_ANY);

      st = bind(CSsoc, (PSOCKADDR) &addr, sizeof(addr));
      if (st == SOCKET_ERROR)
      {
         TraceError;
         return (SOCKINT_ERROR);
      }

      /* CLIENT configuration */

      addr.sin_family = AF_INET;
      addr.sin_port = htons(winpath_simulator_port);
      addr.sin_addr.s_addr = inet_addr( winpath_simulator_ip );

      /* addr.sin_addr.s_addr = inet_addr("192.168.150.63"); */
      /* For now, we assume
      the client and server
      run on the same machine */

      ConnectionTimeOut = CONNECTION_TIMEOUT;



      if (0 == connect(CSsoc, (PSOCKADDR) &addr, sizeof(addr)))
         ret_val = SOCKINT_OK;
      else
      {
         int err = LastError;

         switch( err )
         {

         case CONNREFUSED:
            {

               if (0 == --ConnectionTimeOut)
               {
                  close(CSsoc);
                  WPLI_LOG(("Sockint: Can not locate a WinPath Simulator Server\n"));
                  return SOCKINT_TIMEOUT;
               }
               sleep(1);
            }
            break;

         case INPROGRESS:
         case WOULDBLOCK:
            {
               int results = 0;
               fd_set wsock;
               struct timeval tv;


               /*check socket for writing then check it for reading*/
               FD_ZERO(&wsock);
               FD_SET(CSsoc, &wsock);
               tv.tv_sec  = 1;
               tv.tv_usec = 0;

               results  = select(CSsoc + 1, 0, &wsock, 0, &tv);


               switch (results)
               {
               case 0:


                  WPLI_LOG(("Sockint: Timed out waiting for connection.\n"));
                  ret_val = SOCKINT_TIMEOUT;
                  break;

               case 1:
               case 2:


                  /*try a read test -  if it fails give it one more try*/
                  if( WPI_ReadWriteTest() < 0 )
                  {
                     f_reconnect = 1;
                     f_times_to_try--;

                     if( close(CSsoc) < 0 )
                     {
                        WPLI_LOG(( "Sockint: Close failed\n"));
                        ExitSocketError(1);
                     }
                     sleep(1);
                     continue;
                  }


                  WPLI_LOG(( "Sockint: Read passed\n"));


                  WPLI_LOG(( "Sockint: Connected\n"));
                  return SOCKINT_OK;
                  break;

               default:

                  close(CSsoc);
                  WPLI_LOG(("Sockint: select() failed: %s\n", strerror(err)));
                  return SOCKINT_ERROR;
               }
            }
            break;

         default:
            {
               close(CSsoc);
               WPLI_LOG(("Sockint: connect() failed: %s\n", strerror(err)));
               return SOCKINT_ERROR;
            }
            break;
         }

      }

   }while( f_times_to_try-- > 0 && f_reconnect );

   if (DbgTrace)
      WPLI_LOG(("Sockint: client connection established\n"));

   return ret_val;
}


int CleanUpSocket(int perm)
{
   int st;

   Message[MSG_OPCODE] = perm ? MSG_OP_SHUT : MSG_OP_BYE;
   Message[MSG_LENGTH] = 0;
   st = send(CSsoc, Message, MESSAGE_LENGTH, 0);
   st = WaitForAck();

   st = close(CSsoc);
   return st ? SOCKINT_ERROR : SOCKINT_OK;
}


int WaitForAck(void)
{
   int ConnectionTimeOut = CONNECTION_TIMEOUT;
   int st = recv(CSsoc, Message, MESSAGE_LENGTH, MSG_PEEK);

   while (st == INVALID_SOCKET)
   {
      if (LastError == WOULDBLOCK)
      {
         ConnectionTimeOut--;
         if (ConnectionTimeOut == 0)
            return SOCKINT_TIMEOUT;
      }
      else
      {
         TraceError;
         return SOCKINT_ERROR;
      }
      st = recv(CSsoc, Message, MESSAGE_LENGTH, MSG_PEEK);
   }

   if (Message[MSG_OPCODE] == MSG_OP_ACK)
   {
      st = recv(CSsoc, Message, MESSAGE_LENGTH, 0);
      CycleCount = *(unsigned int *) &Message[MSG_ADDRESS];
      if (DbgTrace)
         WPLI_LOG(( "recv ACK\n"));
      return SOCKINT_OK;
   }
   else
   {
      TraceError;
      return SOCKINT_ERROR;
   }
}

int WriteByte(unsigned int address,
              unsigned int value)
{
   int st;

   Message[MSG_OPCODE] = MSG_OP_WRITE;
   Message[MSG_LENGTH] = 1;
   *((unsigned int *) &Message[MSG_ADDRESS]) = MK_LONG(address);
   Message[MSG_DATA] = (unsigned char) value;
   SOCKET_DATA("write byte", address, &Message[MSG_DATA], 1);
   st = send(CSsoc, Message, MESSAGE_LENGTH, 0);
   if (DbgTrace)
      WPLI_LOG(( "send WriteByte\n"));
   if (st != MESSAGE_LENGTH)
   {
      WPLI_LOG(( "Error writing to socket\n"));
      TraceError;
      return SOCKINT_ERROR;
   }
   return WaitForAck();
}

int WriteShort(unsigned int address,
               unsigned int value)
{
   int st;

   Message[MSG_OPCODE] = MSG_OP_WRITE;
   Message[MSG_LENGTH] = 2;
   *((unsigned int *) &Message[MSG_ADDRESS]) = MK_LONG(address);
   *((unsigned short *) &Message[MSG_DATA]) =
      MK_SHORT((unsigned short) value);
   SOCKET_DATA("write short", address, &Message[MSG_DATA], 2);
   st = send(CSsoc, Message, MESSAGE_LENGTH, 0);
   if (st != MESSAGE_LENGTH)
   {
      WPLI_LOG(( "Error writing to socket\n"));
      TraceError;
      return SOCKINT_ERROR;
   }
   return WaitForAck();
}

void WPI_WriteShort(unsigned int address,
                    unsigned int value)
{
   int st;

   Message[MSG_OPCODE] = MSG_OP_WRITE;
   Message[MSG_LENGTH] = 2;
   *((unsigned int *) &Message[MSG_ADDRESS]) = MK_LONG(address);
   *((unsigned short *) &Message[MSG_DATA]) =
      MK_SHORT((unsigned short) value);
   SOCKET_DATA("wpi write short", address, &Message[MSG_DATA], 2);
   st = send(CSsoc, Message, MESSAGE_LENGTH, 0);
   if (st != MESSAGE_LENGTH)
   {
      WPLI_LOG(( "Error writing to socket\n"));
      TraceError;
      ExitSocketError(1);
   }
   WaitForAck();
}

int WriteLong(unsigned int address,
              unsigned int value)
{
   int st;

   Message[MSG_OPCODE] = MSG_OP_WRITE;
   Message[MSG_LENGTH] = 4;
   *((unsigned int *) &Message[MSG_ADDRESS]) = MK_LONG(address);
   *((unsigned int *) &Message[MSG_DATA]) = MK_LONG(value);
   SOCKET_DATA("write long", address, &Message[MSG_DATA], 4);
   st = send(CSsoc, Message, MESSAGE_LENGTH, 0);
   if (st != MESSAGE_LENGTH)
   {
      WPLI_LOG(( "Error writing to socket\n"));
      TraceError;
      return SOCKINT_ERROR;
   }
   return WaitForAck();
}

void WPI_WriteLong(unsigned int address,
                   unsigned int value)
{
   int st;

   Message[MSG_OPCODE] = MSG_OP_WRITE;
   Message[MSG_LENGTH] = 4;
   *((unsigned int *) &Message[MSG_ADDRESS]) = MK_LONG(address);
   *((unsigned int *) &Message[MSG_DATA]) = MK_LONG(value);
   SOCKET_DATA("wpi write long", address, &Message[MSG_DATA], 4);
   st = send(CSsoc, Message, MESSAGE_LENGTH, 0);
   if (st != MESSAGE_LENGTH)
   {
      WPLI_LOG(( "Error writing to socket\n"));
      TraceError;
      ExitSocketError(1);
   }
   WaitForAck();
}

int WriteBlockGeneral(unsigned int address,
                      unsigned char *pval,
                      int size,
                      int DoSwap)
{
   int st;

#ifdef OS_LITTLE_ENDIAN
   int i;
#endif

   while (size > 0)
   {
      int psize = size > MSG_MAX_SIZE ? MSG_MAX_SIZE : size;

      Message[MSG_OPCODE] = MSG_OP_WRITE;
      Message[MSG_LENGTH] = psize;
      *((unsigned int *) &Message[MSG_ADDRESS]) = MK_LONG(address);

#ifdef OS_LITTLE_ENDIAN
      if (DoSwap)
         for (i = 0; i < psize; i += 4)
            *((unsigned int *) &Message[MSG_DATA + i]) =
               MK_LONG(*(unsigned int *) (pval + i));
      else
#endif
         memcpy(&Message[MSG_DATA], pval, psize);
      SOCKET_DATA("write block", address, &Message[MSG_DATA], psize);
      st = send(CSsoc, Message, MESSAGE_LENGTH, 0);
      if (st != MESSAGE_LENGTH)
      {
         WPLI_LOG(( "Error writing to socket\n"));
         TraceError;
         return SOCKINT_ERROR;
      }
      if ((st = WaitForAck()) != SOCKINT_OK)
         return st;

      size -= MSG_MAX_SIZE;
      pval += MSG_MAX_SIZE;
      address += MSG_MAX_SIZE;
   }
   return SOCKINT_OK;
}

int WriteBlock(unsigned int address,
               unsigned char *pval,
               int size)
{
   return WriteBlockGeneral(address, pval, size, 0);
}

int WriteBlockSwap(unsigned int address,
                   unsigned char *pval,
                   int size)
{
   if ((size % 4) != 0)
   {
      WPLI_LOG(("Error in WriteBlockSwap: size has to be a multiple of 4\n"));
      return SOCKINT_ERROR;
   }
   else if ((((unsigned int) pval) % 4) != 0)
   {
      WPLI_LOG(("Error in WriteBlockSwap: data pointer has to be 4-byte aligned\n"));
      return SOCKINT_ERROR;
   }
   else
      return WriteBlockGeneral(address, pval, size, 1);
}


int WriteCmdFile( char* file_buf, unsigned long file_size )
{
   int byte_cnt;
   int st;
   int orig_file_size = file_size;
   file_size = MK_LONG(file_size);

   /* chage socket to blocking socket: */
#ifdef WIN32
   {
      unsigned long argp = 0;
      ioctlsocket(CSsoc,FIONBIO, &argp);
   }
#else
   {
      unsigned long result = fcntl(CSsoc,F_GETFL, NULL );
      result &= ~(O_NONBLOCK);
      fcntl( CSsoc,F_SETFL, result );
   }
#endif





   Message[MSG_OPCODE] = MSG_OP_CMD_FILE;
   memcpy( Message + MSG_CMD_SIZE, &file_size, sizeof(unsigned long) );
   st = send( CSsoc, (char *)Message, MESSAGE_LENGTH, 0 );

   if (st != MESSAGE_LENGTH)
   {
      TraceError;
      return SOCKINT_ERROR;
   }
   if( (st = WaitForAck()) != SOCKINT_OK )
      return st;
   /* now send the entire cmd file: */
   for( byte_cnt = 0; byte_cnt < orig_file_size ; byte_cnt+= MESSAGE_LENGTH )
   {
      st = send( CSsoc, (char *)file_buf+byte_cnt, MESSAGE_LENGTH, 0 );
      if (st != MESSAGE_LENGTH)
      {
         TraceError;
         return SOCKINT_ERROR;
      }
      if( (st = WaitForAck()) != SOCKINT_OK )
         return st;

   }

   /* change the socket to non_blocking socket: */
#ifdef WIN32
   {
    unsigned long argp = 1;
    ioctlsocket(CSsoc,FIONBIO, &argp);
   }
#else
   {
      fcntl( CSsoc,F_SETFL, O_NONBLOCK );
   }
#endif

   if( (st = WaitForAck()) != SOCKINT_OK )
      return st;
   return SOCKINT_OK;
}


int ReadByte(unsigned int address,
             unsigned int *pvalue)
{
   int st;

   Message[MSG_OPCODE] = MSG_OP_READ;
   Message[MSG_LENGTH] = 1;
   *((unsigned int *) &Message[MSG_ADDRESS]) = MK_LONG(address);

   st = send(CSsoc, Message, MESSAGE_LENGTH, 0);
   if (DbgTrace)
      WPLI_LOG(( "send ReadByte\n"));
   if (st != MESSAGE_LENGTH)
   {
      WPLI_LOG(( "Error writing to socket\n"));
      TraceError;
      return SOCKINT_ERROR;
   }

   if ((st = WaitForAck()) != SOCKINT_OK)
      return st;

   if (DbgTrace)
      WPLI_LOG(( "recv acknoledge\n"));
   *pvalue = (unsigned int) Message[MSG_DATA];
   SOCKET_DATA("read byte", address, &Message[MSG_DATA], 1);
   return SOCKINT_OK;
}

void WPI_ReadByte(unsigned int address,
                  unsigned char *pvalue)
{
   int st;

   Message[MSG_OPCODE] = MSG_OP_READ;
   Message[MSG_LENGTH] = 1;
   *((unsigned int *) &Message[MSG_ADDRESS]) = MK_LONG(address);

   st = send(CSsoc, Message, MESSAGE_LENGTH, 0);
   if (st != MESSAGE_LENGTH)
   {
      WPLI_LOG(( "Error writing to socket\n"));
      TraceError;
      ExitSocketError(1);
   }

   if ((st = WaitForAck()) != SOCKINT_OK)
   {
      WPLI_LOG(( "Error acknowledging socket\n"));
      ExitSocketError(1);
   }

   *pvalue = Message[MSG_DATA];
   SOCKET_DATA("wpi read byte", address, &Message[MSG_DATA], 1);
   return;
}

int ReadShort(unsigned int address,
              unsigned int *pvalue)
{
   int st;

   Message[MSG_OPCODE] = MSG_OP_READ;
   Message[MSG_LENGTH] = 2;
   *((unsigned int *) &Message[MSG_ADDRESS]) = MK_LONG(address);

   st = send(CSsoc, Message, MESSAGE_LENGTH, 0);
   if (st != MESSAGE_LENGTH)
   {
      WPLI_LOG(( "Error writing to socket\n"));
      TraceError;
      return SOCKINT_ERROR;
   }

   if ((st = WaitForAck()) != SOCKINT_OK)
      return st;

   *pvalue =
      (unsigned int) MK_SHORT((*(unsigned short *) &Message[MSG_DATA]));
   SOCKET_DATA("read short", address, &Message[MSG_DATA], 2);
   return SOCKINT_OK;
}

void WPI_ReadShort(unsigned int address,
                   unsigned short *pvalue)
{
   int st;

   Message[MSG_OPCODE] = MSG_OP_READ;
   Message[MSG_LENGTH] = 2;
   *((unsigned int *) &Message[MSG_ADDRESS]) = MK_LONG(address);

   st = send(CSsoc, Message, MESSAGE_LENGTH, 0);
   if (st != MESSAGE_LENGTH)
   {
      WPLI_LOG(( "Error writing to socket\n"));
      TraceError;
      ExitSocketError(1);
   }

   if ((st = WaitForAck()) != SOCKINT_OK)
   {
      WPLI_LOG(( "Error acknowledging socket\n"));
      ExitSocketError(1);
   }

   *pvalue = (unsigned short) MK_SHORT((*(unsigned short *)
                                        &Message[MSG_DATA]));
   SOCKET_DATA("wpi read short", address, &Message[MSG_DATA], 2);
   return;
}

int ReadLong(unsigned int address,
             unsigned int *pvalue)
{
   int st;

   Message[MSG_OPCODE] = MSG_OP_READ;
   Message[MSG_LENGTH] = 4;
   *((unsigned int *) &Message[MSG_ADDRESS]) = MK_LONG(address);

   st = send(CSsoc, Message, MESSAGE_LENGTH, 0);
   if (st != MESSAGE_LENGTH)
   {
      WPLI_LOG(( "Error writing to socket\n"));
      TraceError;
      return SOCKINT_ERROR;
   }

   if ((st = WaitForAck()) != SOCKINT_OK)
      return st;

   *pvalue = MK_LONG(*(unsigned int *) &Message[MSG_DATA]);
   SOCKET_DATA("read long", address, &Message[MSG_DATA], 4);
   return SOCKINT_OK;
}

void WPI_ReadLong(unsigned int address,
                  unsigned int *pvalue)
{
   int st;


   Message[MSG_OPCODE] = MSG_OP_READ;
   Message[MSG_LENGTH] = 4;
   *((unsigned int *) &Message[MSG_ADDRESS]) = MK_LONG(address);


   st = send(CSsoc, Message, MESSAGE_LENGTH, 0);

   if (st != MESSAGE_LENGTH)
   {
      WPLI_LOG(( "Error writing to socket\n"));
      TraceError;
      ExitSocketError(1);
   }

   if ((st = WaitForAck()) != SOCKINT_OK)
   {
      WPLI_LOG(( "Error acknowledging socket\n"));
      ExitSocketError(1);
   }

   *pvalue = MK_LONG(*(unsigned int *) &Message[MSG_DATA]);
   SOCKET_DATA("wpi read long", address, &Message[MSG_DATA], 4);
}




int ReadBlockGeneral(unsigned int address,
                     unsigned char *pval,
                     int size,
                     int DoSwap)
{
   int st;

#ifdef OS_LITTLE_ENDIAN
   int i;
#endif
   while (size > 0)
   {
      int psize = size > MSG_MAX_SIZE ? MSG_MAX_SIZE : size;

      Message[MSG_OPCODE] = MSG_OP_READ;
      Message[MSG_LENGTH] = psize;
      *((unsigned int *) &Message[MSG_ADDRESS]) = MK_LONG(address);
      st = send(CSsoc, Message, MESSAGE_LENGTH, 0);
      if (st != MESSAGE_LENGTH)
      {
         WPLI_LOG(( "Error writing to socket\n"));
         TraceError;
         return SOCKINT_ERROR;
      }
      if ((st = WaitForAck()) != SOCKINT_OK)
         return st;

      SOCKET_DATA("read block", address, &Message[MSG_DATA], psize);
#ifdef OS_LITTLE_ENDIAN
      if (DoSwap)
         for (i = 0; i < size; i += 4)
            *(unsigned int *) (pval + i) =
               MK_LONG(*((unsigned int *) &Message[MSG_DATA + i]));
      else
#endif
         memcpy(pval, &Message[MSG_DATA], psize);
      size -= MSG_MAX_SIZE;
      pval += MSG_MAX_SIZE;
      address += MSG_MAX_SIZE;
   }
   return SOCKINT_OK;
}

int ReadBlock(unsigned int address,
              unsigned char *pval,
              int size)
{
   return ReadBlockGeneral(address, pval, size, 0);
}

int ReadBlockSwap(unsigned int address,
                  unsigned char *pval,
                  int size)
{
   if ((size % 4) != 0)
   {
      WPLI_LOG(("Error in ReadBlockSwap: size has to be a multiple of 4\n"));
      return SOCKINT_ERROR;
   }
   else if ((((unsigned int) pval) % 4) != 0)
   {
      WPLI_LOG(("Error in ReadBlockSwap: data pointer has to be 4-byte aligned\n"));
      return SOCKINT_ERROR;
   }
   else
      return ReadBlockGeneral(address, pval, size, 1);
}


char *ErrnoImage(int ErrCode)
{
   switch (ErrCode)
   {
#ifndef WIN32
      case ENOTSOCK:
         return "Socket operation on non-socket";
      case EDESTADDRREQ:
         return "Destination address required";
      case EMSGSIZE:
         return "Message too long";
      case EPROTOTYPE:
         return "Protocol wrong type for socket";
      case ENOPROTOOPT:
         return "Protocol not available";
      case EPROTONOSUPPORT:
         return "Protocol not supported";
      case ESOCKTNOSUPPORT:
         return "Socket type not supported";
      case EOPNOTSUPP:
         return "Operation not supported on socket";
      case EPFNOSUPPORT:
         return "Protocol family not supported";
      case EAFNOSUPPORT:
         return "Address family not supported by " "protocol family";
      case EADDRINUSE:
         return "Address already in use";
      case EADDRNOTAVAIL:
         return "Can't assign requested address";
      case ENETDOWN:
         return "Network is down";
      case ENETUNREACH:
         return "Network is unreachable";
      case ENETRESET:
         return "Network dropped connection because " "of reset";
      case ECONNABORTED:
         return "Software caused connection abort";
      case ECONNRESET:
         return "Connection reset by peer";
      case ENOBUFS:
         return "No buffer space available";
      case EISCONN:
         return "Socket is already connected";
      case ENOTCONN:
         return "Socket is not connected";
      case ESHUTDOWN:
         return "Can't send after socket shutdown";
      case ETOOMANYREFS:
         return "Too many references: can't splice";
      case ETIMEDOUT:
         return "Connection timed out";
      case ECONNREFUSED:
         return "Connection refused";
      case EHOSTDOWN:
         return "Host is down";
      case EHOSTUNREACH:
         return "No route to host";
      case EALREADY:
         return "operation already in progress";
      case EINPROGRESS:
         return "operation now in progress";
#else
      case WSAENOTSOCK:
         return "Socket operation on non-socket";
      case WSAEDESTADDRREQ:
         return "Destination address required";
      case WSAEMSGSIZE:
         return "Message too long";
      case WSAEPROTOTYPE:
         return "Protocol wrong type for socket";
      case WSAENOPROTOOPT:
         return "Protocol not available";
      case WSAEPROTONOSUPPORT:
         return "Protocol not supported";
      case WSAESOCKTNOSUPPORT:
         return "Socket type not supported";
      case WSAEOPNOTSUPP:
         return "Operation not supported on socket";
      case WSAEPFNOSUPPORT:
         return "Protocol family not supported";
      case WSAEAFNOSUPPORT:
         return "Address family not supported by " "protocol family";
      case WSAEADDRINUSE:
         return "Address already in use";
      case WSAEADDRNOTAVAIL:
         return "Can't assign requested address";
      case WSAENETDOWN:
         return "Network is down";
      case WSAENETUNREACH:
         return "Network is unreachable";
      case WSAENETRESET:
         return "Network dropped connection because " "of reset";
      case WSAECONNABORTED:
         return "Software caused connection abort";
      case WSAECONNRESET:
         return "Connection reset by peer";
      case WSAENOBUFS:
         return "No buffer space available";
      case WSAEISCONN:
         return "Socket is already connected";
      case WSAENOTCONN:
         return "Socket is not connected";
      case WSAESHUTDOWN:
         return "Can't send after socket shutdown";
      case WSAETOOMANYREFS:
         return "Too many references: can't splice";
      case WSAETIMEDOUT:
         return "Connection timed out";
      case WSAECONNREFUSED:
         return "Connection refused";
      case WSAEHOSTDOWN:
         return "Host is down";
      case WSAEHOSTUNREACH:
         return "No route to host";
      case WSAEALREADY:
         return "operation already in progress";
      case WSAEINPROGRESS:
         return "operation now in progress";
#endif
      default:
         return "undefined error";
   }
}

/*****************************************************************************
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 ****************************************************************************/
