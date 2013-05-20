//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// xsgetrevision.c - Get WinPath3 revision
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
    int fd;
    XS_REV_DESC desc;
    unsigned int part, rev, subrev;

    fd = open("/dev/wp_bank24", O_RDWR | O_NOFOLLOW, 0644);
    if(fd == -1) {
	perror("open");
	exit(1);
    }

    if(ioctl(fd, XS_IOCTL_REV_GET, &desc) < 0) {
	printf("Could not get revision\n");
	perror("ioctl");
	close(fd);
	exit(1);
    }
    
    part   = (desc.Fuse0 >> 19) & 0xff;
    rev    = (desc.Fuse0 >> 27) & 0x0f;
    subrev = ((desc.Fuse0 >> 31) & 0x01) | ((desc.Fuse1 & 0x07) << 1);

    printf("\nFuse0 : %08x\nFuse1 : %08x\nPart  : %08x\nRev   : %08x\nSubRev: %08x\n", 
	   desc.Fuse0, desc.Fuse1, part, rev, subrev);

    printf("\n\nDevice: ");

    if(part == 3) {
	printf("WinPath3 ");
    } else {
	printf("Unknown WinPath ");
    }

    if(rev == 0) {
	printf("RevA%d\n", subrev);
    } else if(rev == 1) {
	printf("RevB%d\n", subrev);
    } else {
	printf("Unknown revision\n");
    }
    
    printf("\n");

    close(fd);

    return rev;
}
