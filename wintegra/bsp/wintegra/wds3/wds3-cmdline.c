/*
 * Wintegra Inc, Copyright (C) 2000-2010 All rights reserved.
 *
 * This program is free software; you can distribute it and/or modify it
 * under the terms of the GNU General Public License (Version 2) as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * Kernel command line creation using the prom monitor argc/argv.
 */
#include <linux/init.h>
#include <linux/string.h>
#include <linux/kernel.h>

#include <asm/bootinfo.h>
#include "asm/wintegra/winmon.h"

extern int prom_argc;
extern char **_prom_argv;

#define ARG_IP_STR      "ip="
#define ARG_NFSROOT_STR "nfsroot="
#define ARG_RAMROOT_STR "root="

char * __init prom_getcmdline(void)
{
	return &(arcs_cmdline[0]);
}


void  __init prom_init_cmdline(void)
{
	char *cp;
	int actr;

        int    f_ip = 0;
        int    f_nfsroot = 0;
        int    f_ramroot = 0;
        char * env_var = NULL;

	actr = 1; /* Always ignore argv[0] */

	cp = &(arcs_cmdline[0]);
	while(actr < prom_argc) 
        {
          strcpy(cp, _prom_argv[actr]);                             
          if (strncmp(cp, ARG_IP_STR, strlen(ARG_IP_STR)) == 0)
            f_ip = 1;
          else if (strncmp(cp, ARG_NFSROOT_STR, strlen(ARG_NFSROOT_STR)) == 0)
            f_nfsroot = 1;
          else if (strncmp(cp, ARG_RAMROOT_STR, strlen(ARG_RAMROOT_STR)) == 0)
            f_ramroot = 1;

	  cp += strlen(_prom_argv[actr]);
	  *cp++ = ' ';
	  actr++;
	}

        if (!f_ip)
        {
          env_var = (char *)WinMon_getenv("BOARD_IP_ADDRESS");
          if (!env_var)
            printk("\nWARNING: WEB IP Address not specified\n");
          else
          {
            strcpy(cp, ARG_IP_STR);
            strcpy(cp+strlen(ARG_IP_STR), env_var);
            cp += strlen(ARG_IP_STR) + strlen(env_var);
            env_var = WinMon_getenv("BOARD_GATEWAY");
            if(env_var)
            {
              *cp++=':';*cp++=':';
              strcpy(cp,env_var);
              cp += strlen(env_var);
              *cp++=':';
              env_var = WinMon_getenv("BOARD_SUBNET_MASK");
              if (!env_var)
                printk("\nWARNING: WEB SUBNET mask not specified\n");
              else
              {
                strcpy(cp,env_var);
                cp += strlen(env_var);
              }
            }

            *cp++ = ' ';
            prom_argc++;
            actr++;
          }
        }

#ifdef CONFIG_IP_PNP_BOOTP
        if (!f_nfsroot && !f_ramroot)
        {
          env_var = WinMon_getenv("NFS_ROOT");
          if (!env_var)
            printk("\nWARNING: NFS root not specified\n");
          else
          {
            strcpy(cp, ARG_NFSROOT_STR);
            strcat(cp, env_var);
            cp += strlen(ARG_NFSROOT_STR) + strlen(env_var);
            *cp++ = ' ';
            prom_argc++;
            actr++;

            f_nfsroot = 1;
          }
        }
#endif /* CONFIG_IP_PNP_BOOTP */

#ifdef CONFIG_BLK_DEV_INITRD
        if (!f_nfsroot && !f_ramroot)
        {
          env_var = "/dev/ram";
          strcpy(cp, ARG_RAMROOT_STR);
          strcat(cp, env_var);
          cp += strlen(ARG_RAMROOT_STR) + strlen(env_var);
          *cp++ = ' ';
          prom_argc++;
          actr++;

          f_ramroot = 1;
        }
#endif /* CONFIG_BLK_DEV_INITRD */

	if (cp != &(arcs_cmdline[0])) {
		/* get rid of trailing space */
		--cp;
		*cp = '\0';
	}
}
