//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// xsprogflash.c - Update WinMon in Flash
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
#include <strings.h>
#include <xs_ioctl.h>

#ifndef O_NOFOLLOW
#define O_NOFOLLOW     00400000        /* don't follow links */
#endif

int main (int argc, char *argv[])
{
    XS_FILE_DESC  desc;
    FILE          *fp;
    unsigned char *buf, *buf_orig;
    char          *filename;
    unsigned int  flen;
    int           fd;

    if(argc < 2) {
	printf("Usage: %s filename\n", argv[0]);
	exit(1);
    }

    filename = argv[1];


    /* Open file */
    if((fp=fopen(filename, "r")) == NULL) {
        printf("Couldn't open %s\n", filename);
        return -1;
    }

    /* Get file length */
    fseek(fp, 0, SEEK_END);
    flen = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    /* Allocate buffers to read file */
    buf = (unsigned char *)malloc(flen);
    if(buf==NULL) {
        printf("Couldn't allocate memory to read file 1\n");
        return -1;
    }

    buf_orig = (unsigned char *)malloc(flen);
    if(buf_orig==NULL) {
        printf("Couldn't allocate memory to read file 2\n");
	free(buf);
        return -1;
    }

    /* Read file */
    if(fread(buf, flen, 1, fp) != 1) {
        printf("Couldn't read file\n");
	free(buf);
	free(buf_orig);
        return -1;
    }

    bcopy(buf, buf_orig, flen);

    /* Close file */
    fclose(fp);

    fd = open("/dev/wp_bank24", O_RDWR | O_NOFOLLOW, 0644);
    if(fd == -1) {
	perror("open");
	free(buf);
	free(buf_orig);
	exit(1);
    }

    desc.len = flen;
    desc.buf = buf;

    if(ioctl(fd, XS_IOCTL_FLASH_PROG, &desc) != 0) {
	close(fd);
	perror("ioctl");
	printf("Couldn't flash WinMon version\n");
	free(buf);
	free(buf_orig);
	exit(1);
    }
    
    if(bcmp(buf, buf_orig, flen) == 0) {
	printf("Flash successfully programmed\n");
    } else {
	printf("ERROR: Flash successfully failed !!!!!!!!!!!!\n");
    }

    close(fd);
    free(buf);
    free(buf_orig);

    return 0;
}
