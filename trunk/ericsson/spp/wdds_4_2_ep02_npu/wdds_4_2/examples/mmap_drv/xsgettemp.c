//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// xsgettemp.c - Get temperature
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
#include <math.h>
#include <xs_ioctl.h>

#ifndef O_NOFOLLOW
#define O_NOFOLLOW     00400000        /* don't follow links */
#endif

int main (int argc, char *argv[])
{
    XS_THERMO_DESC desc;
    int fd, sign;

    fd = open("/dev/wp_bank24", O_RDWR | O_NOFOLLOW, 0644);
    if(fd == -1) {
	perror("open");
	exit(1);
    }
	    
    desc.devNo    = 0;
    desc.thermNo  = 0;
    desc.reg      = 0;

    if(ioctl(fd, XS_IOCTL_THERMO_READ, &desc) != 0) {
	close(fd);
	printf("No Thermometer could be read\n");
	exit(1);
    }
    
    sign = desc.value & 0x8000;
    desc.value &= 0x7fff;

    if(desc.reg == 1) {
	printf("Config register = %02x\n", desc.value);
    } else {
	if(sign) {
	    printf("Temperature = -%d.%02d oC\n", desc.value >> 8, (100*(desc.value & 0xff))/256);
	} else {
	    printf("Temperature =  %d.%02d oC\n", desc.value >> 8, (100*(desc.value & 0xff))/256);
	}
    }

    close(fd);

    return 0;
}
