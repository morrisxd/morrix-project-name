/*
 * Copyright (C) 2000-2010 Wintegra Inc.  All rights reserved.
 *
 * ########################################################################
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * ########################################################################
 *
 * Reset the MIPS boards.
 *
 */
#include <linux/pm.h>

#include <asm/io.h>
#include <asm/reboot.h>
#include <asm/wintegra/wds.h>

static void mips_machine_restart(char *command);
static void mips_machine_halt(void);

static void mips_machine_restart(char *command)
{
#if 0
	unsigned int __iomem *softres_reg = (unsigned int *)WDS_CPLD_RESET;
	__raw_writel(0x01, softres_reg);
#endif
}

static void mips_machine_halt(void)
{
  mips_machine_restart("");
}


void mips_reboot_setup(void)
{
	_machine_restart = mips_machine_restart;
	_machine_halt = mips_machine_halt;
	pm_power_off = mips_machine_halt;
}
