/*
 * Copyright (C) 2000-2010 Wintegra Inc.
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
 */
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/kernel_stat.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>

#include <asm/traps.h>
#include <asm/irq_cpu.h>
#include <asm/irq_regs.h>
#include <asm/mipsregs.h>

#include <asm/wintegra/winpath.h>
#include <asm/wintegra/winmon.h>
#include <asm/wintegra/wds.h>


#define DEBUG 0
#if DEBUG
# define DBGP(x...) printk("\n");printk(x);printk("\n");
#else
# define DBGP(x...)
#endif

#define ALLOW_RECORD      0
#define ALLOW_RECORD_WRAP 0
#if ALLOW_RECORD
static unsigned int f_record     = 0;
static unsigned int *history      = (unsigned int *) 0xbec00000;
static unsigned int history_size = 0x00010000;
static int           first_time   = 1;
void record_enable(void)  { f_record = 1; }
void record_disable(void) { f_record = 0; }

static void init_record(void)
{
   volatile unsigned int * fence = history;
   *fence++ = 0;
   *fence++ = 0;
   *fence++ = 0;
   *fence++ = 0;
   first_time = 0;
   *history = (unsigned int) fence;
   *fence = 0xdeadbeef;
   record_enable();
}

void record(unsigned int id1, unsigned int irq, unsigned int val1, unsigned int val2)
{
   volatile unsigned int * fence = NULL;

   if (first_time)
      init_record();

   if (!f_record)
      return;

   fence = *(unsigned int **) history;

   *fence++ = id1;
   *fence++ = irq;
   *fence++ = val1;
   *fence++ = val2;
   if ((unsigned int) fence >= ((unsigned int) history + history_size))
   {
      if (ALLOW_RECORD_WRAP)
      {
         fence = history + 2;
         *fence = *fence + 1;
         fence = history + 4;
      }
      else
         record_disable();
   }
   *history = (unsigned int) fence;
   *fence = 0xdeadbeef;
}
#else /* ALLOW_RECORD */
#define record(id1, irq, val1, val2)
#endif /* ALLOW_RECORD */

#define WINPATH_REG_READ(x,result) ((result) = *(volatile unsigned int *)(x))
#define WINPATH_REG_WRITE(x,data) (*((volatile unsigned int *)(x)) = (data))

static unsigned int WP_core_id =0 ;
static unsigned long winpathEnabledLO = 0;
static unsigned long winpathEnabledHI = 0;

unsigned long levelDisabledLO = 0;
unsigned long levelDisabledHI = 0;

static unsigned long winpathEnabledLO_ep_wp[WDS_WINPATH_NO_MAX] = {0};
static unsigned long winpathEnabledHI_ep_wp[WDS_WINPATH_NO_MAX] = {0};

static void __init winpath3_pic_init (void);
void winpath3_pic_enable(unsigned int irq);
static void winpath3_pic_disable(unsigned int irq);
static void winpath3_pic_ack(unsigned int irq);
static inline int winpath3_get_irq(struct pt_regs *regs);

static inline int clz(unsigned long x)
{
   __asm__(
      "	.set	push					\n"
      "	.set	mips32					\n"
      "	clz	%0, %1					\n"
      "	.set	pop					\n"
      : "=r" (x)
      : "r" (x));

   return x;
}

/*
 * Version of ffs that only looks at bits 12..15.
 */
static inline unsigned int irq_ffs(unsigned int pending)
{
   return -clz(pending) + 31 - CAUSEB_IP;
}

/*
 * IRQs on the WinPath3 device look basically (barring software IRQs which we
 * don't use at all and all external interrupt sources are combined together
 * on hardware interrupt 0 (MIPS IRQ 2)) like:
 *
 *	MIPS IRQ	Source
 *      --------        ------
 *             0	Software (ignored)
 *             1        Software (ignored)
 *             2        Combined hardware interrupt (hw0)
 *             3        Hardware (ignored)
 *             4        Hardware (ignored)
 *             5        Hardware (ignored)
 *             6        Hardware (ignored)
 *             7        R4k timer (what we use)
 *
 * We handle the IRQ according to _our_ priority which is:
 *
 * Highest ----     R4k Timer
 * Lowest  ----     Combined hardware interrupt
 *
 * then we just return, if multiple IRQs are pending then we will just take
 * another exception, big deal.
 */

asmlinkage void plat_irq_dispatch(void)
{
   unsigned int cause = read_c0_cause();
   unsigned int pending = cause & read_c0_status() & ST0_IM;
   struct pt_regs *regs = get_irq_regs();
   int irq  = irq_ffs(pending);

   if (irq==MIPS_CPU_IRQ_WINPATH)
   {    
      irq = winpath3_get_irq(regs);
      if(irq == IV_NULL_VEC)
      {
         spurious_interrupt();
         return;
      }
      record(0x66,irq+WINPATH_IRQ_BASE,0,jiffies);
      //setEventMask(irq, regs->regs[WDS_IRQ_MIPS_REG]);
      do_IRQ(irq+WINPATH_IRQ_BASE);
   }
   else if (irq>=0 && irq<WINPATH_IRQ_BASE)
      do_IRQ(MIPS_CPU_IRQ_BASE+irq);
   else
      spurious_interrupt();
}

#ifdef CONFIG_MIPS_MT_SMP

#define MIPS_CPU_IPI_RESCHED_IRQ 0	/* SW int 0 for resched */
#define MIPS_CPU_IPI_CALL_IRQ 1		/* SW int 1 for resched */

static int cpu_ipi_resched_irq, cpu_ipi_call_irq;

static void ipi_resched_dispatch(void)
{
   do_IRQ(MIPS_CPU_IRQ_BASE + MIPS_CPU_IPI_RESCHED_IRQ);
}

static void ipi_call_dispatch(void)
{
   do_IRQ(MIPS_CPU_IRQ_BASE + MIPS_CPU_IPI_CALL_IRQ);
}

static irqreturn_t ipi_resched_interrupt(int irq, void *dev_id)
{
   return IRQ_HANDLED;
}

static irqreturn_t ipi_call_interrupt(int irq, void *dev_id)
{
   smp_call_function_interrupt();

   return IRQ_HANDLED;
}

static struct irqaction irq_resched = {
   .handler	= ipi_resched_interrupt,
   .flags		= IRQF_DISABLED|IRQF_PERCPU,
   .name		= "IPI_resched"
};

static struct irqaction irq_call = {
   .handler	= ipi_call_interrupt,
   .flags		= IRQF_DISABLED|IRQF_PERCPU,
   .name		= "IPI_call"
};
#endif


static struct irqaction irq_WinPath = {
   .handler	= no_action,
   .flags		= IRQF_DISABLED,   /* Keep irq disabled during handler */
   .name		= "WinPath interrupt controller"
};

static struct irq_chip winpath3_pic = {
   .name           = "WinPath-PIC",
   .mask           = winpath3_pic_disable,
   .disable        = winpath3_pic_disable,
   .unmask         = winpath3_pic_enable,
   .mask_ack       = winpath3_pic_ack,
#ifdef CONFIG_MIPS_MT_SMTC_IRQAFF
   .set_affinity   = plat_set_irq_affinity,
#endif /* CONFIG_MIPS_MT_SMTC_IRQAFF */
};

/*******************************************************************************
 *
 * winpath_pic_init - initialize the interrupt controller
 *
 * This routine will initialize the interrupt controller device, disabling all
 * interrupt sources.  It will also connect the device driver specific routines
 * into the architecture level hooks.  If the BSP needs to create a wrapper
 * pointer to the wrapper routine after calling this routine.
 *
 * RETURNS: N/A.
 */

static void __init winpath3_pic_init (void)
{
   int i;
  
   WP_core_id = WinMon_get_core_id();

   /* Mask all interrupts - a 1 allows the interrupt to be seen; 0 hides it */
   if(minimon_desc.dual_core_enabled == 0)
   {
      WDS3_WINPATH_RIF_WRITE(0x00000000, MAP_IC_MASK_MIPS_0_L);
      WDS3_WINPATH_RIF_WRITE(0x00000000, MAP_IC_MASK_MIPS_0_H);
      WDS3_WINPATH_RIF_WRITE(0x00000000, MAP_IC_MASK_MIPS_1_L);
      WDS3_WINPATH_RIF_WRITE(0x00000000, MAP_IC_MASK_MIPS_1_H);
   }
   else /* Don't override INT setup of the other core */
   {
      if(WP_core_id == WINPATH3_CORE0)
      {
         WDS3_WINPATH_RIF_WRITE(0x00000000, MAP_IC_MASK_MIPS_0_L);
         WDS3_WINPATH_RIF_WRITE(0x00000000, MAP_IC_MASK_MIPS_0_H);         
      }
      else
      {
         WDS3_WINPATH_RIF_WRITE(0x00000000, MAP_IC_MASK_MIPS_1_L);
         WDS3_WINPATH_RIF_WRITE(0x00000000, MAP_IC_MASK_MIPS_1_H);
      }
   }

   /* Clear any pending interrupt flags */
   WDS3_WINPATH_RIF_WRITE(0xffffffff, MAP_IC_INTERUPT_L );
   WDS3_WINPATH_RIF_WRITE(0xffffffff, MAP_IC_INTERUPT_H );

   /* Config the PIC */
#ifdef CONFIG_WINTEGRA_UFE4_FRAMER_INTERRUPTS
   WDS3_WINPATH_RIF_WRITE(0x00008688, MAP_IC_INTMODE);
#else
   WDS3_WINPATH_RIF_WRITE(0x00000688, MAP_IC_INTMODE);
#endif
  
   for (i = WINPATH_IRQ_BASE; i < (WINPATH_IRQ_BASE+IV_BERR4X_VEC+1); i++)
      set_irq_chip_and_handler(i, &winpath3_pic, handle_simple_irq);

   for (i = WINPATH_IRQ_BASE+IV_BERR4X_VEC+1; i < (WINPATH_IRQ_BASE+IV_NULL_VEC); i++)
      set_irq_chip_and_handler(i, &winpath3_pic, handle_level_irq);
}

#define REGSIZE (8*(sizeof(unsigned int)))

static unsigned int getEventMask(unsigned int wirq)
{
   unsigned int mask = 0;
   unsigned int hi = (wirq>=REGSIZE);

   /* return if irq is not in range */
   if (wirq >= IV_WINPATH_BASE && wirq <= IV_WINPATH_MAX)
   {
      if (wirq == IV_SERIAL_QS)
         mask = WINPATH_SER_SERVICE_MASK_HI;
      else if (wirq == IV_SERIAL_QO)
         mask = WINPATH_SER_OVERFLOW_MASK_HI;
#if CONFIG_WINTEGRA_WINK_MODULE
      else if (wirq == IV_WINK_QS)
         mask = WINPATH_SER_WINK_SERVICE_MASK_HI;
      else if (wirq == IV_WINK_QO)
         mask = WINPATH_SER_WINK_OVERFLOW_MASK_HI;
#endif
      else if (hi)
         mask = 1 << (wirq - REGSIZE);
      else
         mask = 1 << wirq;
   }
   return mask;
}

/*******************************************************************************
 *
 * winpath_pic_enable - enable a single WinPath device interrupts
 *
 */
void  winpath3_pic_enable (unsigned int irq)
{
   unsigned int wirq = irq - WINPATH_IRQ_BASE;
   unsigned int mask = getEventMask(wirq);
   unsigned int hi = (wirq>=REGSIZE);
   unsigned int p_l = (WP_core_id == WINPATH3_CORE0 ? MAP_IC_MASK_MIPS_0_L :
                       MAP_IC_MASK_MIPS_1_L );
   unsigned int p_h = (WP_core_id == WINPATH3_CORE0 ? MAP_IC_MASK_MIPS_0_H :
                       MAP_IC_MASK_MIPS_1_H );
  
   if (hi)
   {
      if(levelDisabledHI & mask)
      {
         return;    
      }
     
      winpathEnabledHI |= mask;
      WDS3_WINPATH_RIF_WRITE(winpathEnabledHI & INT_HI_REG_MASK, p_h);      
   }
   else
   {       
      if(levelDisabledLO & mask)
      {
         return;    
      } 

      winpathEnabledLO |= mask;
      WDS3_WINPATH_RIF_WRITE(winpathEnabledLO & INT_LO_REG_MASK, p_l);
   }

   if ((WDS3_GET_NUM_OF_WP() > 1) && IRQ_ENABLE_EP_WP(wirq))
   {
      unsigned int wpid;
      
      for(wpid = 1; wpid < WDS3_GET_NUM_OF_WP(); wpid++)
      {
         if (hi)
         {
            winpathEnabledHI_ep_wp[wpid] |= mask;
            WDS3_WINPATH_RIF_WRITE_EP_WP(wpid, winpathEnabledHI_ep_wp[wpid] & INT_HI_REG_MASK, MAP_IC_MASK_EXTERNAL_H);
         }
         else
         {
            winpathEnabledLO_ep_wp[wpid] |= mask;
            WDS3_WINPATH_RIF_WRITE_EP_WP(wpid, winpathEnabledLO_ep_wp[wpid] & INT_LO_REG_MASK, MAP_IC_MASK_EXTERNAL_L);
         }
      }
   }


#if 0
      printk("WinPath enable irq: %d hi:0x%08x lo:0x%08x\n",irq, (unsigned int)winpathEnabledHI, (unsigned int)winpathEnabledLO);
#endif
}

/*******************************************************************************
 *
 * winpath_pic_disable - disable a single WinPath device interrupt
 * NEEDS UPDATE FOR WP3
 *
 */

void winpath3_pic_disable (unsigned int irq)
{
   unsigned int wirq = irq - WINPATH_IRQ_BASE;
   unsigned int mask = getEventMask(wirq);
   unsigned int hi = (wirq>=REGSIZE);
   unsigned int p_l = (WP_core_id == WINPATH3_CORE0 ? MAP_IC_MASK_MIPS_0_L :
                       MAP_IC_MASK_MIPS_1_L );
   unsigned int p_h = (WP_core_id == WINPATH3_CORE0 ? MAP_IC_MASK_MIPS_0_H :
                       MAP_IC_MASK_MIPS_1_H );

   winpath3_pic_ack(irq);                     /* Acknowledge in case already pending */

   if (hi)
   {
      winpathEnabledHI &= ~mask;
      WDS3_WINPATH_RIF_WRITE(winpathEnabledHI & INT_HI_REG_MASK, p_h);
   }
   else
   {
      winpathEnabledLO &= ~mask;
      WDS3_WINPATH_RIF_WRITE(winpathEnabledLO & INT_LO_REG_MASK, p_l);
   }

   if ((WDS3_GET_NUM_OF_WP() > 1) && IRQ_ENABLE_EP_WP(wirq)) 
   {
      unsigned int wpid;
      for(wpid = 1; wpid < WDS3_GET_NUM_OF_WP(); wpid++)
      {
         if (hi)
         {
            winpathEnabledHI_ep_wp[wpid] &= ~mask;
            WDS3_WINPATH_RIF_WRITE_EP_WP(wpid, winpathEnabledHI_ep_wp[wpid] & INT_HI_REG_MASK, p_h);
         }
         else
         {
            winpathEnabledLO_ep_wp[wpid] &= ~mask;
            WDS3_WINPATH_RIF_WRITE_EP_WP(wpid, winpathEnabledLO_ep_wp[wpid] & INT_LO_REG_MASK, p_l);
         }
      }
   }
#if 0
      printk("WinPath disable irq: %d hi:0x%08x lo:0x%08x\n",irq, (unsigned int)winpathEnabledHI, (unsigned int)winpathEnabledLO);
#endif
}

/*******************************************************************************
 *
 * winpath3_pic_ack - acknowledge the current interrupt.
 *
 *    Do not reenable the interrupting device but allow subsequent interrupt
 *    requests to be noted.
 *
 *    This functionality is specifically for the WinComm devices.
 *
 */

static void winpath3_pic_ack(unsigned int irq)
{
   /* acknowledge the current interrupt cycle by "clearing" the current
    *  interrupt by writing a "1" to the bit(s) to be reset.
    */

   unsigned int wirq = irq - WINPATH_IRQ_BASE;
   unsigned int ack = getEventMask(wirq);
   unsigned int hi = (wirq>=REGSIZE);

   if (hi)
      WDS3_WINPATH_RIF_WRITE(ack, MAP_IC_INTERUPT_H);
   else
      WDS3_WINPATH_RIF_WRITE(ack, MAP_IC_INTERUPT_L);
}


static inline int winpath3_get_irq(struct pt_regs *regs)
{  
   unsigned int pending_hi, pending_lo;
   unsigned int irq;

   unsigned int p = (WP_core_id == WINPATH3_CORE0 ?  MAP_IC_FIRST_MIPS_0 :
                     MAP_IC_FIRST_MIPS_1 );
   unsigned int mask = 0;  

   irq = WDS3_WINPATH_RIF_READ(p);   
   pending_hi = WDS3_WINPATH_RIF_READ(MAP_IC_INTERUPT_H);
   pending_lo = WDS3_WINPATH_RIF_READ(MAP_IC_INTERUPT_L);


   if(levelDisabledLO)
   {
      if(WP_core_id == WINPATH3_CORE0)
      {
         mask = WDS3_WINPATH_RIF_READ(MAP_IC_MASK_MIPS_0_L);
      }
      else
      {
         mask = WDS3_WINPATH_RIF_READ(MAP_IC_MASK_MIPS_1_L);
      } 

      if(mask & levelDisabledLO)
      {
         unsigned int shift = 0; 
         unsigned int tmp = mask & levelDisabledLO;         
         
         while((shift < 32) && (tmp))
         {
            if(tmp & (1<<shift))
            {
               levelDisabledLO  &= (~((1<<shift)));
               enable_irq(shift + WINPATH_IRQ_BASE);               
               tmp  &= (~((1<<shift)));
            }
            shift++;            
         }
      }
   }
   if(levelDisabledHI)
   {
      if(WP_core_id == WINPATH3_CORE0)
      {
         mask = WDS3_WINPATH_RIF_READ(MAP_IC_MASK_MIPS_0_H);
      }
      else
      {
         mask = WDS3_WINPATH_RIF_READ(MAP_IC_MASK_MIPS_1_H);
      } 
      if(mask & levelDisabledHI)
      {
         unsigned int shift = 0; 
         unsigned int tmp = mask & levelDisabledHI;   
         
         while((shift < 32) && (tmp))
         {
            if(tmp & (1<<shift))
            {
               levelDisabledHI  &= (~((1<<shift)));
               enable_irq(shift + 32 + WINPATH_IRQ_BASE);               
               tmp  &= (~((1<<shift)));
            }
            shift++;            
         }
      }              
   }

   pending_hi &= winpathEnabledHI; 
   pending_lo &= winpathEnabledLO; 

   if (pending_hi & WINPATH_SER_SERVICE_MASK_HI) 
   {
      regs->regs[WDS_IRQ_MIPS_REG] = pending_hi & WINPATH_SER_SERVICE_MASK_HI;
      return IV_SERIAL_QS;
   }
   else if (pending_hi & WINPATH_SER_OVERFLOW_MASK_HI) 
   {
      regs->regs[WDS_IRQ_MIPS_REG] = pending_hi & WINPATH_SER_OVERFLOW_MASK_HI;
      return IV_SERIAL_QO;
   }
#if CONFIG_WINTEGRA_WINK_MODULE
   else if (pending_hi & WINPATH_SER_WINK_SERVICE_MASK_HI)
   {
      regs->regs[WDS_IRQ_MIPS_REG] = pending_hi & WINPATH_SER_WINK_SERVICE_MASK_HI;
      return IV_WINK_QS;
   }
   else if (pending_hi & WINPATH_SER_WINK_OVERFLOW_MASK_HI)
   {
      regs->regs[WDS_IRQ_MIPS_REG] = pending_hi & WINPATH_SER_WINK_OVERFLOW_MASK_HI;
      return IV_WINK_QO;
   }
#endif
   else if (pending_hi || pending_lo)
      return irq;    /* There is an issue here if the first set bit in the pending reg is not enabled.
                        We will then ignore this bit, but still return this irq number.
                        I am assuming this will not happen.....
                     */
   else
      /* If no interrupt is pending, return IV_NULL_VEC */
      return IV_NULL_VEC;
}

#if defined(CONFIG_MIPS_MT_SMP)
void __init arch_init_ipiirq(int irq, struct irqaction *action)
{
   setup_irq(irq, action);
   set_irq_handler(irq, handle_percpu_irq);
}
#endif

void __init arch_init_irq(void)
{
   winpath3_pic_init();
   mips_cpu_irq_init();
   setup_irq(MIPS_CPU_IRQ_BASE+MIPS_CPU_IRQ_WINPATH, &irq_WinPath);
#ifdef CONFIG_MIPS_MT_SMTC
   arch_init_ipiirq(cpu_ipi_resched_irq, &irq_resched);
   arch_init_ipiirq(cpu_ipi_call_irq, &irq_call);
#endif
}

void wds3_be_init(void)
{
}

int wds3_be_handler(struct pt_regs *regs, int is_fixup)
{
   int retval = is_fixup ? MIPS_BE_FIXUP : MIPS_BE_FATAL;
   return retval;
}
