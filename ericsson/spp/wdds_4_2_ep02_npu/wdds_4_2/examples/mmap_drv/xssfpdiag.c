//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// xssfpdiag.c - SFP diagnostics
// Copyright (C) 2003-2012 Xalyo Systems  
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This example is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//
// The full GNU Lesser General Public License is included in this distribution 
// in the file called LGPL_LICENSE.
//
// Contact Information:
//     Andre Wiesel <awiesel@xalyo.com>
//     Xalyo Systems, Riant-Coteau 7, 1196 Gland, Switzerland
//
//----------------------------------------------------------------------------
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include <string.h>
#include <sys/mman.h>
#include <xs_ioctl.h>

typedef enum {
  SFP_IDENTIFIER = 0,
  SFP_EXT_IDENTIFIER,
  SFP_CONNECTOR,
  SFP_TRANSCEIVER,
  SFP_ENCODING,
  SFP_BR,
  SFP_LENGTH,
  SFP_VENDOR_NAME,
  SFP_VENDOR_OUI,
  SFP_VENDOR_PN,
  SFP_VENDOR_REV,
  SFP_OPTIONS,
  SFP_BR_MAX,
  SFP_BR_MIN,
  SFP_VENDOR_SN,
  SFP_DATE_CODE
} XS_SFP_FIELD;

#define XS_SFP_DIAG   0x00020000

#ifndef O_NOFOLLOW
#define O_NOFOLLOW     00400000        /* don't follow links */
#endif

static int fd;

static char sfpString[50][64] = 
    {
	"Unknown",
	"Reserved",
	"Vendor Specific",

	// Identifiers, offset = 3
	"Unknown",
	"GBIC",
	"Soldered",
	"SFP",

	// Ext Identifiers, offset = 7
	"Serial ID",
    
	// Connector, offset = 8
	"Unknown",
	"SC",
	"Fiber Channel Style 1",
	"Fiber Channel Style 2",
	"BNC/TNC",
	"Fiber Channel Coaxial",
	"FIber Jack",
	"LC",
	"MT-RJ",
	"MU",
	"SG",
	"Optical Pigtail",
	"HSSDC II",
	"Copper Pigtail",

	// Transceiver, offset = 22
	"OC 48, long reach",
	"OC 48, intermediate reach",
	"OC 48, short reach",
	"OC 12, single mode long reach",
	"OC 12, single mode inter. reach",
	"OC 12, multimode short reach",
	"OC 3, single mode long reach",
	"OC 3, single mode inter. reach",
	"OC 3, multimode short reach",
	"1000BASE-T",
	"1000BASE-CX",
	"1000BASE-LX",
	"1000BASE-SX",
	// FC transceiver types ignored
    
	// Encoding, offset = 35
	"Unspecified",
	"8B10B",
	"4B5B",
	"NRZ",
	"Manchester"

	// User offset = 40
    
    };

static char *string;

static int xs_sfp_present      (int dev, int port);
static int xs_sfp_byte_value(unsigned int dev, unsigned int port, unsigned int reg, unsigned int *c);
static int xs_sfp_diag_byte_value(unsigned int dev, unsigned int port, unsigned int reg, unsigned int *c);

// Warning: Non-reentrant call
static char * xs_sfp_field_value  (int devNo, int port, XS_SFP_FIELD field) {
    unsigned int val=0;

    if(!xs_sfp_present(devNo, port)) {
	return NULL;
    }

    switch(field) {
    case SFP_IDENTIFIER :
	xs_sfp_byte_value(devNo, port, 0, &val);
	if(val < 4) {
	    string = sfpString[val + 3];
	}
	if((val > 3) && (val < 0x80)) {
	    string = sfpString[1];
	}
	if(val > 0x80) {
	    string = sfpString[2];
	}
	break;

    case SFP_EXT_IDENTIFIER :
	xs_sfp_byte_value(devNo, port, 1, &val);
	if(val == 4) {
	    string = sfpString[7];
	} else {
	    string = sfpString[2];
	}
	break;

    case SFP_CONNECTOR :
	xs_sfp_byte_value(devNo, port, 2, &val);
	if(val < 0xc) {
	    string = sfpString[val + 8];
	}
	if((val > 0xb) && (val < 0x10)) {
	    string = sfpString[1];
	}
	if((val > 0x1f) && (val < 0x22)) {
	    string = sfpString[val + 8 - 19];
	}
	if(val > 0x80) {
	    string = sfpString[2];
	}
	break;

    case SFP_TRANSCEIVER :
    
	// Check byte 4
	xs_sfp_byte_value(devNo, port, 4, &val);
	if(val & 0x04) {
	    string = sfpString[22];
	}
	if(val & 0x02) {
	    string = sfpString[23];
	}
	if(val & 0x01) {
	    string = sfpString[24];
	}
    
	// Check byte 5
	xs_sfp_byte_value(devNo, port, 5, &val);

	if(val & 0x40) {
	    string = sfpString[25];
	}
	if(val & 0x20) {
	    string = sfpString[26];
	}
	if(val & 0x10) {
	    string = sfpString[27];
	}
	if(val & 0x04) {
	    string = sfpString[28];
	}
	if(val & 0x02) {
	    string = sfpString[29];
	}
	if(val & 0x01) {
	    string = sfpString[30];
	}
    
	// Check byte 6
	xs_sfp_byte_value(devNo, port, 6, &val);
	if(val & 0x08) {
	    string = sfpString[31];
	}
	if(val & 0x04) {
	    string = sfpString[32];
	}
	if(val & 0x02) {
	    string = sfpString[33];
	}
	if(val & 0x01) {
	    string = sfpString[34];
	}
	break;

    case SFP_ENCODING :
	xs_sfp_byte_value(devNo, port, 11, &val);

	if(val < 0x5) {
	    string = sfpString[val + 35];
	}
	if(val > 0x04) {
	    string = sfpString[1];
	}
	break;

    case SFP_BR :
	string = &sfpString[40][0];
	bzero(string, 64);
	xs_sfp_byte_value(devNo, port, 12, &val);
	sprintf(string, "%d", val*100);
	break;

    case SFP_LENGTH :
	string = &sfpString[40][0];
	bzero(string, 64);

	xs_sfp_byte_value(devNo, port, 14, &val);
	if(val != 0) {
	    sprintf(string, "%d", val * 1000);
	    break;
	}
	xs_sfp_byte_value(devNo, port, 15, &val);
	if(val != 0) {
	    sprintf(string, "%d", val * 100);
	    break;
	}
	xs_sfp_byte_value(devNo, port, 16, &val);
	if(val != 0) {
	    sprintf(string, "%d", val * 10);
	    break;
	}
	xs_sfp_byte_value(devNo, port, 17, &val);
	if(val != 0) {
	    sprintf(string, "%d", val * 10);
	    break;
	}
	xs_sfp_byte_value(devNo, port, 18, &val);
	if(val != 0) {
	    sprintf(string, "%d", val);
	    break;
	}
    
	break;

    case SFP_VENDOR_NAME :
	{
	    int i;

	    string = sfpString[40];
	    bzero(string, 64);
    
	    for(i=0; i<16; i++) {
		xs_sfp_byte_value(devNo, port, 20 + i, &val);
		string[i] = val;
	    }
	}
	break;

    case SFP_VENDOR_OUI :
	{
	    unsigned int val1=0, val2=0, val3=0;

	    xs_sfp_byte_value(devNo, port, 37, &val1);
	    xs_sfp_byte_value(devNo, port, 38, &val2);
	    xs_sfp_byte_value(devNo, port, 39, &val3);
	    sprintf(string, "%02x%02x%02x", val1, val2, val3);
	}
	break;

    case SFP_VENDOR_PN :
	{
	    int i;

	    string = &sfpString[40][0];
	    bzero(string, 64);

	    for(i=0; i<16; i++) {
		xs_sfp_byte_value(devNo, port, 40 + i, &val);
		string[i] = val;
	    }
	}
	break;

    case SFP_VENDOR_REV :
	{
	    int i;

	    string = &sfpString[40][0];
	    bzero(string, 64);

	    for(i=0; i<4; i++) {
		xs_sfp_byte_value(devNo, port, 56 + i, &val);
		string[i] = val;
	    }
	}
	break;

    case SFP_OPTIONS :
	string = &sfpString[40][0];
	bzero(string, 64);

	xs_sfp_byte_value(devNo, port, 65, &val);

	string[0] = 0;

	if(val & 0x20) {
	    strcat(string, " + RATE_SELECT"); 
	}    
	if(val & 0x10) {
	    strcat(string, " + TX_DISABLE"); 
	}    
	if(val & 0x08) {
	    strcat(string, " + TX_FAULT"); 
	}    
	if(val & 0x04) {
	    strcat(string, " + SD"); 
	}    
	if(val & 0x02) {
	    strcat(string, " + LOS"); 
	}
	break;

    case SFP_BR_MAX :
	xs_sfp_byte_value(devNo, port, 66, &val);
	sprintf(string, "%d", val);
	break;

    case SFP_BR_MIN :
	xs_sfp_byte_value(devNo, port, 67, &val);
	sprintf(string, "%d", val);
	break;

    case SFP_VENDOR_SN :
	{
	    int i;

	    string = &sfpString[40][0];
	    bzero(string, 64);

	    for(i=0; i<16; i++) {
		xs_sfp_byte_value(devNo, port, 68 + i, &val);
		string[i] = val;
	    }
	}
	break;

    case SFP_DATE_CODE :
	string = &sfpString[40][0];
	bzero(string, 64);

	xs_sfp_byte_value(devNo, port, 88, &val);
	string[0] = val;
	xs_sfp_byte_value(devNo, port, 89, &val);
	string[1] = val;
	string[2] = '/';
	xs_sfp_byte_value(devNo, port, 86, &val);
	string[3] = val;
	xs_sfp_byte_value(devNo, port, 87, &val);
	string[4] = val;
	string[5] = '/';
	string[6] = '2';
	string[7] = '0';
	xs_sfp_byte_value(devNo, port, 84, &val);
	string[8] = val;
	xs_sfp_byte_value(devNo, port, 85, &val);
	string[9] = val;
	string[10] = '/';
	string[11] = '(';
	xs_sfp_byte_value(devNo, port, 90, &val);
	string[12] = val;
	xs_sfp_byte_value(devNo, port, 91, &val);
	string[13] = val;
	string[14] = ')';

	break;
    
    default : 
	return NULL;
    }
    return string;
}

typedef struct pci_dev_t {
    unsigned int   irq;                  /* IRQ number */
    unsigned long  base_addr[6];         /* Base addresses */
    unsigned long  size[6];              /* Region sizes */
    unsigned long  rom_base_addr;        /* Expansion ROM base address */
    unsigned long  rom_size;             /* Expansion ROM size */
} pci_dev_t;

typedef struct pci_map_t {
    unsigned char *fpga;
    unsigned char *wpath;
    unsigned char *param;
    unsigned char *packet;
    unsigned char *intl;
    unsigned char *appl;
    unsigned int  fpga_size;
    unsigned int  wpath_size;
    unsigned int  param_size;
    unsigned int  packet_size;
    unsigned int  intl_size;
    unsigned int  appl_size;
} pci_map_t;

static unsigned int inited = 0;
static pci_dev_t    pci_dev;
static pci_map_t    map;

static pci_dev_t *pci_init(unsigned int vdid, int card_no)
{
    FILE *f;    
    char buf[512];   
    unsigned int dev_no, dfn, vend;
	
    dev_no = 0;
    
    if((f=fopen("/proc/bus/pci/devices", "r")) == NULL) {
	return NULL;
    }
    
    while (fgets(buf, sizeof(buf)-1, f)) {
	sscanf(buf,
	       "%x %x %x %lx %lx %lx %lx %lx %lx %lx %lx %lx %lx %lx %lx %lx %lx",
	       &dfn,
	       &vend,
	       &pci_dev.irq,
	       &pci_dev.base_addr[0],
	       &pci_dev.base_addr[1],
	       &pci_dev.base_addr[2],
	       &pci_dev.base_addr[3],
	       &pci_dev.base_addr[4],
	       &pci_dev.base_addr[5],
	       &pci_dev.rom_base_addr,
	       &pci_dev.size[0],
	       &pci_dev.size[1],
	       &pci_dev.size[2],
	       &pci_dev.size[3],
	       &pci_dev.size[4],
	       &pci_dev.size[5],
	       &pci_dev.rom_size);
	
	if(vend == vdid) {
	    if(dev_no++ == card_no) {
		break;
	    }
	}
    }
    
    if(vend != vdid) {
	return NULL;
    }
    
    return &pci_dev;
}

static pci_map_t *wp3_map_get(unsigned int dev)
{
    pci_dev_t    *pci_dev;
    int          mem_fd;

    if(inited == 0) {
	pci_dev = pci_init(0x19800003, dev);

	if(pci_dev == NULL) {
	    printf("wp3_init: Could not find device %08x\n", 0x19800003);
	    return NULL;
	}
	
	if((mem_fd=open("/dev/mem", O_RDWR)) < 0) {
	    perror("open");
	    return NULL;
	}

	if((map.intl  = mmap(NULL,
			     pci_dev->size[0], 
			     PROT_READ | PROT_WRITE, 
			     MAP_SHARED, 
			     mem_fd, 
			     pci_dev->base_addr[0] & 0xfffffff0)) == (void *)-1) { 
	    perror("mmap");
	    return NULL; 
	}
	map.intl_size = pci_dev->size[0];

	if((map.fpga  = mmap(NULL,
			     pci_dev->size[1], 
			     PROT_READ | PROT_WRITE, 
			     MAP_SHARED, 
			     mem_fd, 
			     pci_dev->base_addr[1] & 0xfffffff0)) == (void *)-1) { 
	    perror("mmap");
	    return NULL; 
	}
	map.fpga_size = pci_dev->size[1];
	
	if((map.appl  = mmap(NULL,
			     pci_dev->size[2], 
			     PROT_READ | PROT_WRITE, 
			     MAP_SHARED, 
			     mem_fd, 
			     pci_dev->base_addr[2] & 0xfffffff0)) == (void *)-1) { 
	    perror("mmap");
	    return NULL; 
	}
	map.appl_size = pci_dev->size[2];
	
	if((map.wpath  = mmap(NULL,
			      pci_dev->size[3], 
			      PROT_READ | PROT_WRITE, 
			      MAP_SHARED, 
			      mem_fd, 
			      pci_dev->base_addr[3] & 0xfffffff0)) == (void *)-1) { 
	    perror("mmap");
	    return NULL; 
	}
	map.wpath_size = pci_dev->size[3];
	
	if((map.param  = mmap(NULL,
			      pci_dev->size[4], 
			      PROT_READ | PROT_WRITE, 
			      MAP_SHARED, 
			      mem_fd, 
			      pci_dev->base_addr[4] & 0xfffffff0)) == (void *)-1) { 
	    perror("mmap");
	    return NULL; 
	}
	map.param_size = pci_dev->size[4];

	if((map.packet  = mmap(NULL,
			       pci_dev->size[5],
			       PROT_READ | PROT_WRITE, 
			       MAP_SHARED, 
			       mem_fd, 
			       pci_dev->base_addr[5] & 0xfffffff0)) == (void *)-1) { 
	    perror("mmap");
	    return NULL; 
	}
	map.packet_size = pci_dev->size[5];

	inited = 1;
    }
    
    return &map;
}

static unsigned int xsByteSwap16(unsigned short data)
{
    unsigned short x;
    
    x = data;
    
    return (((x & 0x00ff) <<  8) |
            ((x & 0xff00) >>  8));
}

static int ufe4_get_16(unsigned int dev, unsigned short *pval, unsigned int addr)
{
    pci_map_t *map;
    
    map = wp3_map_get(dev);
    if(map == NULL) {
	return -1;
    }

    *pval = xsByteSwap16(*(volatile unsigned short *)(map->fpga + addr + 0x300000));

    return 0;
}

static int ufe4_put_16(unsigned int dev, unsigned short val, unsigned int addr)
{
    pci_map_t *map;
    
    map = wp3_map_get(dev);
    if(map == NULL) {
	return -1;
    }

    *(volatile unsigned short *)(map->fpga + addr + 0x300000) = xsByteSwap16(val);

    return 0;
}

static void i2c_wait(unsigned int dev) {
    int            timeout = 10000;
    unsigned short sval;

    do {
	ufe4_get_16(dev, &sval, 0x30086);
	usleep(100);
    } while((sval & 1) && --timeout);

    if(timeout == 0) {
	printf("i2c_wait: timeout\n");
    }

    //usleep(1000);

    return;
}

static void sfp_wclk(unsigned int dev, unsigned int port, unsigned int val) 
{                  
    unsigned short sval;
 
    i2c_wait(dev);                                        
    ufe4_get_16(dev, &sval, 0x30080);     
    //printf("1 CLK = %8x\n",  sval);

    i2c_wait(dev);                      
    if(val & 1) {                                         
	ufe4_put_16(dev, sval |  (1 << port), 0x30080);	  
	//printf("2 CLK = %8x\n",  sval |  (1 << port));
    } else {                                              
	ufe4_put_16(dev, sval & ~(1 << port), 0x30080);   
	//printf("2 CLK = %8x\n",  sval &  ~(1 << port));
    }                                                     
}

static void sfp_wsda(unsigned int dev, unsigned int port, unsigned int val) 
{
    unsigned short sval;

    i2c_wait(dev);                                        
    ufe4_get_16(dev, &sval, 0x30082);         
    //printf("1 SDA = %8x\n",  sval);

    i2c_wait(dev);                   
    if(val & 1) {                                         
	ufe4_put_16(dev, sval |  (1 << port), 0x30082);   
	//printf("2 SDA = %8x\n",  sval |  (1 << port));
    } else {                                             
	ufe4_put_16(dev, sval & ~(1 << port), 0x30082);   
	//printf("2 SDA = %8x\n",  sval &  ~(1 << port));
    }                                                     
}

static unsigned char sfp_rsda(unsigned int dev, unsigned int port) 
{
    unsigned short sval;

    i2c_wait(dev);      
    ufe4_get_16(dev, &sval, 0x30084);

    return (unsigned char)((sval >> port) & 1);
}

static int xs_sfp_byte_value(unsigned int dev, unsigned int port, unsigned int reg, unsigned int *c)
{
    unsigned int   tout;
    unsigned short bit;
    unsigned char  devAdrs = 0xa0;
                                        
    i2c_wait(dev);     
    ufe4_put_16(dev, 0xffff, 0x30080);	
    i2c_wait(dev);      
    ufe4_put_16(dev, 0xffff, 0x30082);	

    /* CLK and DATA initialized to low */
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, 0);

    /* Start */
    sfp_wsda(dev, port, 1);
    sfp_wclk(dev, port, 1);
    sfp_wsda(dev, port, 0);
    sfp_wclk(dev, port, 0);
  
    /* Device address */
    sfp_wsda(dev, port, devAdrs >> 7);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 6);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 5);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 4);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 3);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 2);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 1);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
  
    /* Write instruction */
    sfp_wsda(dev, port, 0);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
  
    /* Wait acknowledge */
    sfp_wsda(dev, port, 1); // to tristate

    tout = 1000;
    do {
	bit = sfp_rsda(dev, port);
	if(tout-- == 0) {
	    printf("sfp_i2c_read: timeout 1\n");
	    return 0;
	}
	usleep(1000);
    } while (bit);

    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
  
    /* Put address */
    sfp_wsda(dev, port, reg >> 7);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, reg >> 6);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, reg >> 5);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, reg >> 4);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, reg >> 3);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, reg >> 2);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, reg >> 1);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, reg >> 0);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
  
    /* Wait acknowledge */
    sfp_wsda(dev, port, 1); // to tristate

    tout = 1000;
    do {
	bit = sfp_rsda(dev, port);
	if(tout-- == 0) {
	    printf("sfp_i2c_read: timeout 2\n");
	    return 0;
	}
	usleep(1000);
    } while (bit);

    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
  
    /* Start */
    sfp_wsda(dev, port, 1);
    sfp_wclk(dev, port, 1);
    sfp_wsda(dev, port, 0);
    sfp_wclk(dev, port, 0);
  
    /* Device address */
    sfp_wsda(dev, port, devAdrs >> 7);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 6);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 5);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 4);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 3);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 2);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 1);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
  
    /* Read instruction */
    sfp_wsda(dev, port, 1);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
  
    /* Wait acknowledge */
    sfp_wsda(dev, port, 1);// to tristate

    tout = 1000;
    do {
	bit = sfp_rsda(dev, port);
	if(tout-- == 0) {
	    printf("sfp_i2c_read: timeout 3\n");
	    return 0;
	}
	usleep(1000);
    } while (bit);

    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);

    *c = 0;

    sfp_wclk(dev, port, 1);
    bit = sfp_rsda(dev, port);
    *c |= bit << 7; //printf("--- %x\n", *c);
    sfp_wclk(dev, port, 0);

    sfp_wclk(dev, port, 1);
    bit = sfp_rsda(dev, port);
    *c |= bit << 6; //printf("--- %x\n", *c);
    sfp_wclk(dev, port, 0);

    sfp_wclk(dev, port, 1);
    bit = sfp_rsda(dev, port);
    *c |= bit << 5; //printf("--- %x\n", *c);
    sfp_wclk(dev, port, 0);

    sfp_wclk(dev, port, 1);
    bit = sfp_rsda(dev, port);
    *c |= bit << 4; //printf("--- %x\n", *c);
    sfp_wclk(dev, port, 0);

    sfp_wclk(dev, port, 1);
    bit = sfp_rsda(dev, port);
    *c |= bit << 3; //printf("--- %x\n", *c);
    sfp_wclk(dev, port, 0);

    sfp_wclk(dev, port, 1);
    bit = sfp_rsda(dev, port);
    *c |= bit << 2; //printf("--- %x\n", *c);
    sfp_wclk(dev, port, 0);

    sfp_wclk(dev, port, 1);
    bit = sfp_rsda(dev, port);
    *c |= bit << 1; //printf("--- %x\n", *c);
    sfp_wclk(dev, port, 0);

    sfp_wclk(dev, port, 1);
    bit = sfp_rsda(dev, port);
    *c |= bit << 0; //printf("--- %x\n", *c);
    sfp_wclk(dev, port, 0);
  
    /* No ack */
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
  
    /* Stop */
    sfp_wsda(dev, port, 0);
    sfp_wclk(dev, port, 0);
    sfp_wclk(dev, port, 1);
    sfp_wsda(dev, port, 1);
    sfp_wclk(dev, port, 0);
 
    return 0;
}


static int xs_sfp_diag_byte_value(unsigned int dev, unsigned int port, unsigned int reg, unsigned int *c)
{
    unsigned int   tout;
    unsigned short bit;
    unsigned char  devAdrs = 0xa2;
                                        
    i2c_wait(dev);     
    ufe4_put_16(dev, 0xffff, 0x30080);	
    i2c_wait(dev);      
    ufe4_put_16(dev, 0xffff, 0x30082);	

    /* CLK and DATA initialized to low */
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, 0);

    /* Start */
    sfp_wsda(dev, port, 1);
    sfp_wclk(dev, port, 1);
    sfp_wsda(dev, port, 0);
    sfp_wclk(dev, port, 0);
  
    /* Device address */
    sfp_wsda(dev, port, devAdrs >> 7);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 6);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 5);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 4);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 3);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 2);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 1);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
  
    /* Write instruction */
    sfp_wsda(dev, port, 0);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
  
    /* Wait acknowledge */
    sfp_wsda(dev, port, 1); // to tristate

    tout = 1000;
    do {
	bit = sfp_rsda(dev, port);
	if(tout-- == 0) {
	    printf("sfp_i2c_read: timeout 1\n");
	    return 0;
	}
	usleep(1000);
    } while (bit);

    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
  
    /* Put address */
    sfp_wsda(dev, port, reg >> 7);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, reg >> 6);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, reg >> 5);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, reg >> 4);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, reg >> 3);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, reg >> 2);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, reg >> 1);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, reg >> 0);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
  
    /* Wait acknowledge */
    sfp_wsda(dev, port, 1); // to tristate

    tout = 1000;
    do {
	bit = sfp_rsda(dev, port);
	if(tout-- == 0) {
	    printf("sfp_i2c_read: timeout 2\n");
	    return 0;
	}
	usleep(1000);
    } while (bit);

    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
  
    /* Start */
    sfp_wsda(dev, port, 1);
    sfp_wclk(dev, port, 1);
    sfp_wsda(dev, port, 0);
    sfp_wclk(dev, port, 0);
  
    /* Device address */
    sfp_wsda(dev, port, devAdrs >> 7);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 6);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 5);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 4);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 3);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 2);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
    sfp_wsda(dev, port, devAdrs >> 1);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
  
    /* Read instruction */
    sfp_wsda(dev, port, 1);
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
  
    /* Wait acknowledge */
    sfp_wsda(dev, port, 1);// to tristate

    tout = 1000;
    do {
	bit = sfp_rsda(dev, port);
	if(tout-- == 0) {
	    printf("sfp_i2c_read: timeout 3\n");
	    return 0;
	}
	usleep(1000);
    } while (bit);

    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);

    *c = 0;

    sfp_wclk(dev, port, 1);
    bit = sfp_rsda(dev, port);
    *c |= bit << 7; //printf("--- %x\n", *c);
    sfp_wclk(dev, port, 0);

    sfp_wclk(dev, port, 1);
    bit = sfp_rsda(dev, port);
    *c |= bit << 6; //printf("--- %x\n", *c);
    sfp_wclk(dev, port, 0);

    sfp_wclk(dev, port, 1);
    bit = sfp_rsda(dev, port);
    *c |= bit << 5; //printf("--- %x\n", *c);
    sfp_wclk(dev, port, 0);

    sfp_wclk(dev, port, 1);
    bit = sfp_rsda(dev, port);
    *c |= bit << 4; //printf("--- %x\n", *c);
    sfp_wclk(dev, port, 0);

    sfp_wclk(dev, port, 1);
    bit = sfp_rsda(dev, port);
    *c |= bit << 3; //printf("--- %x\n", *c);
    sfp_wclk(dev, port, 0);

    sfp_wclk(dev, port, 1);
    bit = sfp_rsda(dev, port);
    *c |= bit << 2; //printf("--- %x\n", *c);
    sfp_wclk(dev, port, 0);

    sfp_wclk(dev, port, 1);
    bit = sfp_rsda(dev, port);
    *c |= bit << 1; //printf("--- %x\n", *c);
    sfp_wclk(dev, port, 0);

    sfp_wclk(dev, port, 1);
    bit = sfp_rsda(dev, port);
    *c |= bit << 0; //printf("--- %x\n", *c);
    sfp_wclk(dev, port, 0);
  
    /* No ack */
    sfp_wclk(dev, port, 1);
    sfp_wclk(dev, port, 0);
  
    /* Stop */
    sfp_wsda(dev, port, 0);
    sfp_wclk(dev, port, 0);
    sfp_wclk(dev, port, 1);
    sfp_wsda(dev, port, 1);
    sfp_wclk(dev, port, 0);
 
    return 0;
}

static int xs_sfp_present      (int dev, int port) {
    unsigned int   present, absent;
    unsigned short val;

    ufe4_get_16(dev, &val, 0x300a0);
    	    
    switch(port) {
    case 0 :
	{
	    absent = val & 0x0001;
	}
	break;
    case 1 :
	{
	    absent = val & 0x0002;
	}
	break;
    case 2 :
	{
	    absent = val & 0x0004;
	}
	break;
    case 3 :
	{
	    absent = val & 0x0008;
	}
	break;
    default :
	return -1;
    }

    if(absent) present = 0; else present = 1;

    return present;
}

// -------------------------------------------------------------------
//
// xssfpdiag: Get SFP diagnostics
//
int
xssfpdiag(int itf, int port)
{    
    unsigned int val, temp, powerType, sign;
    unsigned int ts=1, to=0, ext;

    fd = open("/dev/wp_bank24", O_RDWR | O_NOFOLLOW, 0644);
    if(fd == -1) {
	perror("open");
	exit(1);
    }

    if(xs_sfp_present(itf, port)) {
	printf("Present: YES\n");
    } else {
	printf("Present: NO\n");
	return 0;
    }

    printf("\n");
    printf("Base ID Fields:\n");
    printf("---------------\n");
    printf("Identifier     : %s\n",      xs_sfp_field_value(itf, port, SFP_IDENTIFIER));
    printf("Ext. Identifier: %s\n",      xs_sfp_field_value(itf, port, SFP_EXT_IDENTIFIER));
    printf("Connector      : %s\n",      xs_sfp_field_value(itf, port, SFP_CONNECTOR));
    printf("Transceiver    : %s\n",      xs_sfp_field_value(itf, port, SFP_TRANSCEIVER));
    printf("Encoding       : %s\n",      xs_sfp_field_value(itf, port, SFP_ENCODING));
    printf("Bit Rate       : %s mbps\n", xs_sfp_field_value(itf, port, SFP_BR));
    printf("Length         : %s\n",      xs_sfp_field_value(itf, port, SFP_LENGTH));
    printf("Vendor Name    : %s\n",      xs_sfp_field_value(itf, port, SFP_VENDOR_NAME));
    printf("Vendor OUI     : %s\n",      xs_sfp_field_value(itf, port, SFP_VENDOR_OUI));
    printf("Vendor PN      : %s\n",      xs_sfp_field_value(itf, port, SFP_VENDOR_PN));
    printf("Vendor Revision: %s\n",      xs_sfp_field_value(itf, port, SFP_VENDOR_REV));
    printf("Options        : %s\n",      xs_sfp_field_value(itf, port, SFP_OPTIONS));
    printf("\n");
    printf("Extended ID Fields:\n");
    printf("-------------------\n");
    printf("BR, max        :+%s %%\n",   xs_sfp_field_value(itf, port, SFP_BR_MAX));
    printf("BR, min        :-%s %%\n",   xs_sfp_field_value(itf, port, SFP_BR_MIN));
    printf("Vendor SN      : %s\n",      xs_sfp_field_value(itf, port, SFP_VENDOR_SN));
    printf("Date Code      : %s\n",      xs_sfp_field_value(itf, port, SFP_DATE_CODE));
    
    // Find out diagnostic type
    xs_sfp_byte_value(itf, port, 92, &val);
    if(val & 0x40) {
	printf("Diagnostics    : Capable\n");
    } else {
	printf("Diagnostics    : Unsupported\n");
	return 0;
    }
    if(val & 0x08) {
	powerType = 1; // Average
    } else {
	powerType = 0; // OMA
    }
    if((val & 0x20) != 0x20) {
	ext = 1;
    	//printf("External calibration, not supported\n");
    	//return 0;
    } else {
	ext = 0;
    }

    if(val & 0x04) {
	printf("Address change required, not supported\n");
	return 0;
    }
    printf("\n");

    printf("Digital Diagnostics Fields:\n");
    printf("---------------------------\n");

    xs_sfp_byte_value(itf, port, 93, &val);
    xs_sfp_byte_value(itf, port, 94, &val);

    xs_sfp_diag_byte_value(itf, port, 96, &val);
    temp = val << 8;
    xs_sfp_diag_byte_value(itf, port, 97, &val);
    temp |= val;
    sign = temp & 0x8000;
    temp &= 0x7fff;
    temp /= 256;
    
    if(ext) {
	// External calibration
	
	xs_sfp_diag_byte_value(itf, port, 84, &val);
	ts = val << 8;
	xs_sfp_diag_byte_value(itf, port, 85, &val);
	ts |= val << 8;
	ts /= 256;

	temp *= ts;

	xs_sfp_diag_byte_value(itf, port, 86, &val);
	to = val << 8;
	xs_sfp_diag_byte_value(itf, port, 87, &val);
	to |= val << 8;
	if(to & 0x8000) {
	    to /= 256;
	    temp -= to;
	} else {
	    to /= 256;
	    temp -= to;
	}
    }
    
    if(sign) {
	printf("Temperature    : -%d oC\n", temp);
    } else {
	printf("Temperature    : %d oC\n", temp);
    }

    // Voltage
    xs_sfp_diag_byte_value(itf, port, 98, &val);
    temp = val << 8;
    xs_sfp_diag_byte_value(itf, port, 99, &val);
    temp |= val;
    printf("Voltage        : %.2f V\n", (float)temp/10000); // unit = 100 uV

    // Laser Bias
    xs_sfp_diag_byte_value(itf, port, 100, &val);
    temp = val << 8;
    xs_sfp_diag_byte_value(itf, port, 101, &val);
    temp |= val;
    printf("Laser Bias     : %d mA\n", 2*temp/1000); // unit = 2 uA

    // Output Power
    xs_sfp_diag_byte_value(itf, port, 102, &val);
    temp = val << 8;
    xs_sfp_diag_byte_value(itf, port, 103, &val);
    temp |= val;
    if(powerType == 1) {
	printf("Output Power   : %.1f dBm\n", 
	       log10f((float)temp/10000)*10); // unit = 0.1 uW
    } else {
	printf("Output Power   : OMA type, unsupported\n");
    }

    // Rx Power
    xs_sfp_diag_byte_value(itf, port, 104, &val);
    temp = val << 8;
    xs_sfp_diag_byte_value(itf, port, 105, &val);
    temp |= val;
    if(powerType == 1) {
	printf("Rx Power       : %.1f dBm\n", 
	       log10f((float)temp/10000)*10); // unit = 0.1 uW
    } else {
	printf("Output Power   : OMA type, unsupported\n");
    }
    
    close(fd);

    return 0;
}

// -------------------------------------------------------------------
//
// main: Main
//
#ifndef __VxWorks
int main(int argc, char *argv[]) 
{    
    int itf, port;

    // check arguments
    if(argc < 3) {
        printf("usage : %s itf port\n", argv[0]);
        exit(1);
    }

    itf  = atoi(argv[1]);
    port = atoi(argv[2]);
    
    exit(xssfpdiag(itf, port));
}
#endif
