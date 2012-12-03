//----------------------------------------------------------------------------
// xsconsole.c - PCI console
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
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <signal.h> 
#include <ctype.h>
#include <sys/select.h>
#include <termios.h>
#include <time.h>
#include <sys/time.h>
//#include <asm/fcntl.h>
#include <xs_ioctl.h>

#define STDIN  0
#define STDOUT 1

int main(int argc, char *argv[])
{
  struct timeval timeout;
  fd_set         rset;
  int            fd, size;
  int            first=1;
  struct termios tty;
  char *filename = "/dev/wp_bank24";

  XS_CONSOLE_BUF_DESC consoleBufDesc;

  timeout.tv_sec  = 0;
  timeout.tv_usec = 50000;

  // Set STDIN
  if(tcgetattr(STDIN_FILENO, &tty) != 0)
    {
      perror("tcgetattr");
      exit(1);
    }
  tty.c_lflag &= ~(ICANON | ISIG | ECHO | ECHONL); // RAW
  if(tcsetattr(STDIN_FILENO, TCSANOW, &tty) != 0)
    {
      perror("tcsetattr");
      exit(1);
    }

  // Set STDOUT
  if(tcgetattr(STDOUT_FILENO, &tty) != 0)
    {
      perror("tcgetattr");
      exit(1);
    }
  tty.c_lflag |= ICRNL; // CRMOD
  tty.c_lflag &= ~(ISIG | ICANON); // RAW
  if(tcsetattr(STDOUT_FILENO, TCSANOW, &tty) != 0)
    {
      perror("tcsetattr");
      exit(1);
    }

  fd = open(filename, O_RDWR/* | O_NOFOLLOW*/, 0644);

  if(fd == -1)
    {
      perror("open");
      goto restore_tty;
      exit(1);
    }

  FD_ZERO(&rset);

  // Loop
  while (1)
  {    
    FD_SET(STDIN,  &rset);

    if(select (STDIN + 1, &rset, NULL, NULL, &timeout) < 0)
      {
	perror("select");
        goto restore_tty;
	exit (-1);
      }

    if (FD_ISSET(STDIN, &rset))
      { 
	if(first)
	  {
	    first = 0;
	  }
	else 
	  {
	    size = read (STDIN, consoleBufDesc.buf, PCI_CONSOLE_SIZE);

	    if (consoleBufDesc.buf[0] == '~')
	      {
		printf("\n");
		break;
	      }
	    if (consoleBufDesc.buf[size-1] == '\n')
	      {
		consoleBufDesc.buf[size-1] ='\r';
	      }

	    consoleBufDesc.bufLen = size;

	    if(ioctl(fd, XS_IOCTL_CONSOLE_WRITE, &consoleBufDesc) == -1)
	      {
		perror("ioctl");
		close(fd);
		goto restore_tty;
	      }
	  }
      }
    
    bzero(consoleBufDesc.buf, PCI_CONSOLE_SIZE);

    if(ioctl(fd, XS_IOCTL_CONSOLE_READ, &consoleBufDesc) == -1)
      {
	perror("ioctl");
        goto restore_tty;
	close(fd);
	exit(1);
      }

    if (consoleBufDesc.bufLen)
      {
	if(consoleBufDesc.bufLen < 0)
	  {
	    if(consoleBufDesc.bufLen == -1) {
	      printf("\n\n\n\t\t MIPS rebooted \n\n\n");
	    } else if(consoleBufDesc.bufLen == -2) {
	      printf("\n\n\n\t\t UART console \n\n\n");
	    } else {
	      printf("\n\n\n\t\t Console protocol error \n\n\n");
	    }
	  }
	else
	  {
	    //int i;
	    //for(i=0; i<consoleBufDesc.bufLen; i++) {
	    //  if(consoleBufDesc.buf[i] == '\r')
	    //	consoleBufDesc.buf[i] = '\n';
	    //}
	    write(STDOUT, consoleBufDesc.buf, consoleBufDesc.bufLen);
	  }
      }
  }

 restore_tty:
  // Set STDIN
  if(tcgetattr(STDIN_FILENO, &tty) != 0)
    {
      perror("tcgetattr");
      exit(1);
    }
  tty.c_lflag |= ICANON | ISIG | ECHO | ECHONL;
  if(tcsetattr(STDIN_FILENO, TCSANOW, &tty) != 0)
    {
      perror("tcsetattr");
      exit(1);
    }

  // Set STDOUT
  if(tcgetattr(STDOUT_FILENO, &tty) != 0)
    {
      perror("tcgetattr");
      exit(1);
    }
  tty.c_lflag &= ~ICRNL; // CRMOD
  tty.c_lflag |= ISIG | ICANON; // RAW
  if(tcsetattr(STDOUT_FILENO, TCSANOW, &tty) != 0)
    {
      perror("tcsetattr");
      exit(1);
    }

  close(fd);
  
  exit(0);
}
