//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// xsenv.c - Displays WinMon Environment 
// Copyright (C) 2003-2011 Xalyo Systems  
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
#include <xs_ioctl.h>
#include <string.h>
#include <netinet/ether.h>

#ifndef O_NOFOLLOW
#define O_NOFOLLOW     00400000        /* don't follow links */
#endif

int main (int argc, char *argv[])
{
    int               fd;
    XS_FILE_DESC      desc;
    unsigned char     buf[2048], offset;
    char              *str, *ptr, *saveptr;
    struct ether_addr *addr;

    fd = open("/dev/wp_bank24", O_RDWR | O_NOFOLLOW, 0644);
    if(fd == -1) {
	perror("open");
	exit(1);
    }
	
    desc.buf = buf;

    if(ioctl(fd, XS_IOCTL_ENV_GET, &desc) != 0) {
	close(fd);
	printf("XS_IOCTL_ENV_GET failed\n");
	perror("ioctl");
	exit(1);
    }

    close(fd);

    // Print all environment variables
    printf("%s\n", buf);

    // Get Serial and MACs
    ptr = (char *)buf;
    while((str = strtok_r(ptr, "\n", &saveptr)) != NULL) {
	if(!strncmp(str, "BOARD_MAC_ETH0", strlen("BOARD_MAC_ETH0"))) {
	    offset = strlen("BOARD_MAC_ETH0") + 1;
	    addr = ether_aton(&str[offset]);
	    printf("MAC for port 0 is %02x:%02x:%02x:%02x:%02x:%02x\n", 
		   addr->ether_addr_octet[0],
		   addr->ether_addr_octet[1],
		   addr->ether_addr_octet[2],
		   addr->ether_addr_octet[3],
		   addr->ether_addr_octet[4],
		   addr->ether_addr_octet[5]);
	}
	if(!strncmp(str, "BOARD_MAC_ETH1", strlen("BOARD_MAC_ETH1"))) {
	    offset = strlen("BOARD_MAC_ETH1") + 1;
	    addr = ether_aton(&str[offset]);
	    printf("MAC for port 1 is %02x:%02x:%02x:%02x:%02x:%02x\n", 
		   addr->ether_addr_octet[0],
		   addr->ether_addr_octet[1],
		   addr->ether_addr_octet[2],
		   addr->ether_addr_octet[3],
		   addr->ether_addr_octet[4],
		   addr->ether_addr_octet[5]);
	}
	if(!strncmp(str, "BOARD_MAC_ETH2", strlen("BOARD_MAC_ETH2"))) {
	    offset = strlen("BOARD_MAC_ETH2") + 1;
	    addr = ether_aton(&str[offset]);
	    printf("MAC for port 2 is %02x:%02x:%02x:%02x:%02x:%02x\n", 
		   addr->ether_addr_octet[0],
		   addr->ether_addr_octet[1],
		   addr->ether_addr_octet[2],
		   addr->ether_addr_octet[3],
		   addr->ether_addr_octet[4],
		   addr->ether_addr_octet[5]);
	}

	if(!strncmp(str, "BOARD_SERIAL", strlen("BOARD_SERIAL"))) {
	    offset = strlen("BOARD_SERIAL") + 1;
	    printf("Serial is %d\n", atoi(&str[offset]));
	}

	ptr = NULL;
    }

    return 0;
}
