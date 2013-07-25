/*
 * Copyright (C) 2000-2010 Wintegra Inc.  All rights reserved.
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
 * Setting up the clock on the MIPS boards.
 */

#include <linux/init.h>
#include <linux/timer.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/time.h>
#include <asm/wintegra/winpath.h>

extern signed int getMipsSystemClock(void);

unsigned long cpu_khz;

static int mips_cpu_timer_irq;
static int mips_cpu_perf_irq;
extern int cp0_perfcount_irq;

#ifdef CONFIG_WINTEGRA_WDS3
static void scroll_display_message(unsigned long data);
static DEFINE_TIMER(mips_scroll_timer, scroll_display_message, HZ, 0);

static void scroll_display_message(unsigned long data)
{
  static unsigned int __iomem *display = (unsigned int __iomem *)WDS3_LED_REG;
  static unsigned int val = 0xf;
  static int left = 1;

  if (left)
  {
    val = (val << 1);
    if (val == 0x0f00) left = 0;
  }
  else
  {
    val = (val >> 1) ;
    if (val == 0xf) left = 1;
  }

  __raw_writel((~((val&0xf0)<<5))|0x6100, display);
  mod_timer(&mips_scroll_timer, jiffies + HZ);
}
#endif

static void __init plat_perf_setup(void)
{
  if (cp0_perfcount_irq >= 0) {
    mips_cpu_perf_irq = MIPS_CPU_IRQ_BASE+cp0_perfcount_irq;
#ifdef CONFIG_SMP
    set_irq_handler(mips_cpu_perf_irq, handle_percpu_irq);
#endif
  }
}

unsigned int __cpuinit get_c0_compare_int(void)
{
  mips_cpu_timer_irq = MIPS_CPU_IRQ_BASE+cp0_compare_irq;
  return mips_cpu_timer_irq;
}

void __init plat_time_init(void)
{
	unsigned int est_freq;

        est_freq = getMipsSystemClock();
        mips_hpt_frequency = est_freq / 2;

	printk("CPU frequency %d.%02d MHz\n", est_freq/1000000,
	       (est_freq%1000000)*100/1000000);

        cpu_khz = est_freq / 1000;
#if 0
#ifdef CONFIG_WINTEGRA_WDS3
	del_timer_sync(&mips_scroll_timer);
	mod_timer(&mips_scroll_timer, jiffies + 1);
#endif
#endif /*if 0 */
	plat_perf_setup();
}
