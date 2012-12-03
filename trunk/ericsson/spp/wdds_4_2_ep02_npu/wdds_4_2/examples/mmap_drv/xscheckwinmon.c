//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// xscheckwinmon.c - Check WinMon revision
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
#include <xs_ioctl.h>

#define WDS_WINPATH_NO_MAX   4
#define MAX_SIU_NO           5
#define MAX_WP_BANKS         7

typedef struct  {
    unsigned int wp_bus;
    unsigned int wp_bus_active;
    unsigned int wp_phys_bank_base_addr;
    unsigned int wp_phys_bank_size;
    unsigned int wp_application_bank_base_addr;
    unsigned int wp_application_bank_size;
} wds_winpath_mem_bank_info_t;


typedef struct {
    unsigned int wp_id;
    unsigned int wp_clock_freq_siu[MAX_SIU_NO];
    unsigned int wp_clock_freq_mips;
    unsigned int wds_winpath_no_of_banks;
    wds_winpath_mem_bank_info_t wp_bank_info[MAX_WP_BANKS];      
} wds_winpath_info_t;

typedef struct  { 
    unsigned int wds_winpath_no;            // Number of WinPath devices on board
    unsigned int wds_clock_freq_external;   // Board external clock ferquency
    wds_winpath_info_t wds_winpath_info[WDS_WINPATH_NO_MAX];      
    unsigned int wds_chip;
    unsigned int wds_winmon_ver;
} wds_board_info_t;

#ifndef O_NOFOLLOW
#define O_NOFOLLOW     00400000        /* don't follow links */
#endif

int main (int argc, char *argv[])
{
    wds_board_info_t desc;
    int              fd, status = 0;
    unsigned int     revision;
    char             date[16];
    unsigned int     data[3];

    fd = open("/dev/wp_bank24", O_RDWR | O_NOFOLLOW, 0644);
    if(fd == -1) {
	perror("open");
	exit(1);
    }
	    
    if(ioctl(fd, MMAP_IOCTL_READ_BOARD_GEN_CONF, &desc) != 0) {
	close(fd);
	printf("Could not get WinMon revision\n");
	exit(1);
    }
	    
    data[0] = 0x0002fe34;
    data[1] = 0x0002fe38;
    data[2] = 0x0002fe3c;

    if(ioctl(fd, XS_IOCTL_READ_WORD, &data[0]) != 0) {
	close(fd);
	printf("Could not get WinMon revision date\n");
	exit(1);
    }
	    
    if(ioctl(fd, XS_IOCTL_READ_WORD, &data[1]) != 0) {
	close(fd);
	printf("Could not get WinMon revision date\n");
	exit(1);
    }
	    
    if(ioctl(fd, XS_IOCTL_READ_WORD, &data[2]) != 0) {
	close(fd);
	printf("Could not get WinMon revision date\n");
	exit(1);
    }
    
    close(fd);

    //printf("data = %x %x %x\n", data[0], data[1], data[2]);

    date[ 0] = (data[0] >> 24) & 0xff;
    date[ 1] = (data[0] >> 16) & 0xff;
    date[ 2] = (data[0] >>  8) & 0xff;
    date[ 3] = (data[0] >>  0) & 0xff;
    date[ 4] = (data[1] >> 24) & 0xff;
    date[ 5] = (data[1] >> 16) & 0xff;
    date[ 6] = (data[1] >>  8) & 0xff;
    date[ 7] = (data[1] >>  0) & 0xff;
    date[ 8] = (data[2] >> 24) & 0xff;
    date[ 9] = (data[2] >> 16) & 0xff;
    date[10] = (data[2] >>  8) & 0xff;
    date[11] = (data[2] >>  0) & 0xff;
    date[12] = 0;

    revision = desc.wds_winmon_ver;

    if(revision != 0x04020400) {
	printf("WinMon version should be %x and is %x\n", 0x04020400, revision);
	status = -1;
    }

    if(strcmp(date, "May  4 2012")) {
	printf("WinMon version date should be %s and is %s\n", "May  4 2012", date);
	status = -1;
    }

    printf("WinMon revision: %08x %s\n", revision, date);

    return status;
}
