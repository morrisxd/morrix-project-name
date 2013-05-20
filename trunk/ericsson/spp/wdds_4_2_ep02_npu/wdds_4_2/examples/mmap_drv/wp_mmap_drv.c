#include <linux/stddef.h>
//#include <linux/autoconf.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18) // config.h removed
#include <linux/config.h>
#endif

#if defined(MODVERSIONS)
#include <linux/modversions.h>
#endif

#include <linux/module.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h>
#include <asm/byteorder.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <linux/pagemap.h>
#include <asm/bitops.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <linux/capability.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/list.h>
#include <asm/uaccess.h>
#include <linux/if_arp.h>
#include <linux/workqueue.h>
#include <linux/dma-mapping.h>
#include <linux/time.h>
#include <linux/miscdevice.h>
#include <linux/vmalloc.h>
#include <linux/mman.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/compat.h>
#include <asm/compat.h>
#include <winpath3.h>
#include <xs_ioctl.h>
#include <irq.h>

#define XS_VERSION "0.93"

#define WDS_WINPATH_NO_MAX   4
#define MAX_SIU_NO           5
#define MAX_WP_BANKS         7

#define WDS3_SIU_BUS_PARAM_NUM       1
#define WDS3_SIU_BUS_PACKET_NUM      2
#define WDS3_SIU_BUS_INTERNAL_NUM    3
#define WDS3_SIU_BUS_HOST_NUM        4
#define WDS3_SIU_BUS_WMM5_NUM        5
#define WDS3_SIU_BUS_RIF_NUM         6
#define WDS3_FPGA_NUM                7

#define WDS_IRQ_MIPS_REG   27

#define WDS3_EXTERNAL_CLOCK 20000000

#define THREAD_DEATH_STATUS   (PF_EXITING | PF_EXITPIDONE | PF_SIGNALED)

#define SIGNAL_RT_SER                35 /*SIGRTMIN+3*/
#define SIGNAL_RT_OV         (SIGNAL_RT_SER + 1)
#define SIGNAL_RT_BUSERR     (SIGNAL_RT_SER + 2)
#define SIGNAL_RT_SERDES             44
#define SIGNAL_RT_SGMII_AN           45
#define SIGNAL_RT_UFE4_FRAMER        46

#define XS_WRITE_32(adrs, val) xsPciWrite32((adrs),  (val))
#define XS_READ_32(adrs, val)  xsPciRead32 ((adrs), &(val))
#define XS_WRITE_16(adrs, val) xsPciWrite16((adrs),  (val))
#define XS_READ_16(adrs, val)  xsPciRead16 ((adrs), &(val))

#define STORE(base, val, adrs) XS_WRITE_32((unsigned int *)((long)(base) + (long)(adrs)), xsByteSwap(val));

#define GET(base, val, adrs) {                                             \
  unsigned int __GETtemp;                                                  \
  __GETtemp = 0;                                                           \
  XS_READ_32((unsigned int *)((long)(base) + (long)(adrs)), __GETtemp);    \
  (val) = xsByteSwap(__GETtemp);                                           \
}

// --------------------------------------------------------------------------
//   Driver identification
// --------------------------------------------------------------------------
MODULE_AUTHOR("Andre Wiesel <awiesel@xalyo.com>");
MODULE_DESCRIPTION("WinPath3 PCI mmap driver");
MODULE_LICENSE("GPL");
#ifdef LINUX26
MODULE_VERSION(XS_VERSION);
#endif

static char mmap_driver_name[]    = "wp_mmapdrv";
static char mmap_driver_string[]  = "WinPath3 PCI mmap Driver";
static char mmap_driver_version[] = XS_VERSION;
static char mmap_copyright[]      = "Copyright (c) 2003-2011 Xalyo Systems, Wintegra";

static int mmap_log_level = 0;
module_param(mmap_log_level, int, 0);

static int proc_created;
static long int_nbr[64];
static long sigsent_nbr[64];

static int  mmap_probe (struct pci_dev *pdev, const struct pci_device_id *ent);
static void mmap_remove(struct pci_dev *pdev);

static int mmapdrv_open(struct inode *inode, struct file *file);
static int mmapdrv_release(struct inode *inode, struct file *file);
static int mmapdrv_mmap(struct file *file, struct vm_area_struct *vma);

#ifdef HAVE_UNLOCKED_IOCTL
static long mmapdrv_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
#else
static int  mmapdrv_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg);
#endif
#ifdef HAVE_COMPAT_IOCTL
static long mmapdrv_compat_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
#endif

static irqreturn_t ser_qs_handler   (int irq, void * dev_id);
static irqreturn_t ser_qo_handler   (int irq, void * dev_id);
static irqreturn_t sys_err_handler  (int irq, void * dev_id);
static irqreturn_t int3_handler     (int irq, void *dev_id);
static irqreturn_t sgmii_an_handler (int irq, void *dev_id);
static irqreturn_t serdes_handler (int irq, void *dev_id);
static void        report_errors    (unsigned long syir);

/*Internal information */
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

static wds_board_info_t wds;

/* Config structure */
static struct {
    uint32_t suppress_dup_signals;
    uint32_t send_buserr_sig;
    uint32_t kernel_buserr_report;
} mmapdrv_dev_cfg = {
    1,    /* suppress duplicate signals -- default behavior */
    0,
    1
};
static struct task_struct *mmapdrv_dev = NULL;

#define PCI_CONSOLE_BUF_SIZE (10 * 1024)

#define PCI_CONSOLE_WRITE_STATUS_MASK    0x00000040
#define PCI_CONSOLE_READ_STATUS_MASK     0x00000080

typedef struct XS_HANDLE {
    struct pci_dev *pdev;
    unsigned int   *siu3Adrs;
    unsigned int   *siu3AdrsPhys;
    unsigned int   siu3AdrsSize;
    unsigned int   *fpgaAdrs;
    unsigned int   *fpgaAdrsPhys;
    unsigned int   fpgaAdrsSize;
    unsigned int   *wpathAdrs;
    unsigned int   *wpathAdrsPhys;
    unsigned int   wpathAdrsSize;
    unsigned int   *hostAdrs;
    unsigned int   *hostAdrsPhys;
    unsigned int   hostAdrsSize;
    unsigned int   *paramAdrs;
    unsigned int   *paramAdrsPhys;
    unsigned int   paramAdrsSize;
    unsigned int   *packetAdrs;
    unsigned int   *packetAdrsPhys;
    unsigned int   packetAdrsSize;
    spinlock_t     xsInterrupt_lock;
    int            consoleWrPtr;
    int            consoleRdPtr;
    int            consoleWrap;
    char           consoleBuf[PCI_CONSOLE_BUF_SIZE];
    spinlock_t     xsConsole_lock;
} XS_HANDLE;

static XS_HANDLE *gxsHandle;

static int i2c_read_seq  (XS_HANDLE *xsHandle, unsigned int addr, unsigned char *buf);
static int i2c_write_page(XS_HANDLE *xsHandle, unsigned int page, unsigned char *buf);

static unsigned int usage_count;

static unsigned long mmap_virt_to_phys(unsigned int virt) 
{
    unsigned long phys = 0;
    int mapped = 0;

    if((virt >= 0x00000000) && (virt < (0x00000000 + gxsHandle->hostAdrsSize))) {
	phys = (long)gxsHandle->hostAdrsPhys + virt - 0x00000000;
	mapped = 1;
    }

    if((virt >= 0x1e000000) && (virt < (0x1e000000 + gxsHandle->wpathAdrsSize))) {
	phys = (long)gxsHandle->wpathAdrsPhys + virt - 0x1e000000;
	mapped = 1;
    }

    if((virt >= 0x1f000000) && (virt < (0x1f000000 + gxsHandle->fpgaAdrsSize))) {
	phys = (long)gxsHandle->fpgaAdrsPhys + virt - 0x1f000000;  
	mapped = 1;
    }

    if((virt >= 0x1fc00000) && (virt < (0x1fc00000 + gxsHandle->siu3AdrsSize))) {
	phys = (long)gxsHandle->siu3AdrsPhys + virt - 0x1fc00000;
	mapped = 1;
    }

    if((virt >= 0x20000000) && (virt < (0x20000000 + gxsHandle->paramAdrsSize))) {
	phys = (long)gxsHandle->paramAdrsPhys + virt - 0x20000000;
	mapped = 1;
    }

    if((virt >= 0x30000000) && (virt < (0x30000000 + gxsHandle->packetAdrsSize))) {
	phys = (long)gxsHandle->packetAdrsPhys + virt - 0x30000000;
	mapped = 1;
    }

    if(mapped == 0) {
	printk("Could not find a physical address for virtual address %x!!!!\n", virt);
    }

    if(mmap_log_level > 0) {
	printk("wp_mmap_drv: mmap_virt_to_phys %x -> %lx\n", virt, phys);
    }

    return phys;
}

// PCI Device ID Table
static struct pci_device_id mmap_pci_tbl[] = {
    // { VID, DID, SVID, SDID, Class, Class Mask, private data (not used) }
    {0x1980, 0x0003, 0x4567, 0x0010, 0, 0, 0}, // XS-AMC-WP3C
    {0x1980, 0x0003, 0x4567, 0x0011, 0, 0, 0}, // XS-3200
    {0x1980, 0x0003, 0x4567, 0x0012, 0, 0, 0}, // XS-3100
    {0,}
};

MODULE_DEVICE_TABLE(pci, mmap_pci_tbl);
/* PCI driver */
static struct pci_driver mmap_pci_driver = {
    .name     = mmap_driver_name,
    .id_table = mmap_pci_tbl,
    .probe    = mmap_probe,
    .remove   = __devexit_p(mmap_remove)
};

/* the ordinary device operations */
static struct file_operations mmapdrv_fops =
    {
	owner:   THIS_MODULE,
#ifdef HAVE_UNLOCKED_IOCTL
	unlocked_ioctl:   mmapdrv_unlocked_ioctl,
#else
	ioctl:            mmapdrv_ioctl,
#endif
#ifdef HAVE_COMPAT_IOCTL
        .compat_ioctl = mmapdrv_compat_ioctl,
#endif
	mmap:    mmapdrv_mmap,
	open:    mmapdrv_open,
	release: mmapdrv_release,
    };

static int major; /* major number of device */
static unsigned char *RIF_BASE;

static unsigned int xsByteSwap(unsigned int data)
{
    unsigned int x;
    
    x = data;
    
    return (((x & 0x000000ff) << 24) |
            ((x & 0x0000ff00) <<  8) |
            ((x & 0x00ff0000) >>  8) |
            ((x & 0xff000000) >> 24));
}

static void xsPciWrite32(volatile unsigned int *adrs, volatile unsigned int val)
{
    *adrs = val;
    
    return;
}

static void xsPciRead32(volatile unsigned int *adrs, volatile unsigned int *val)
{
    *val = *adrs;
    
    return;
}

static void xsPciWrite16(volatile unsigned short *adrs, volatile unsigned short val)
{
    *adrs = val;
    
    return;
}

static void xsPciRead16(volatile unsigned short *adrs, volatile unsigned short *val)
{
    *val = *adrs;

    return;
}

static unsigned int WDS3_WINPATH_RIF_READ(unsigned int address) 
{
    unsigned int data;

    xsPciRead32((unsigned int *)((long)address + (long)gxsHandle->wpathAdrs), &data);

    return xsByteSwap(data);
}

static int xsPciMap  (struct pci_dev *pdev,
		      unsigned int   bar,
		      unsigned int   **cpuAdrs,
		      unsigned int   **physAdrs,
		      unsigned int   *size) 
{

    *physAdrs = (unsigned int *)(unsigned long)pci_resource_start(pdev, bar);
    *size     = (unsigned int  )pci_resource_len(pdev, bar);
    *cpuAdrs  = (unsigned int *)ioremap(pci_resource_start(pdev, bar), (unsigned long)*size);

    if(*cpuAdrs == NULL) {
	printk("Warning: Couldn't map BAR %d, try to increase vmalloc\n", bar);
	return -ENOMEM;
    }

    return 0;
}

static int xsPciMapShow(XS_HANDLE *xsHandle) {
    unsigned int pciSize;

    pciSize = xsHandle->siu3AdrsSize;
    printk("XS: SIU3 at ............................ %p (%p) (MEM - %3d %s)\n",
	   xsHandle->siu3Adrs,
	   xsHandle->siu3AdrsPhys,
	   pciSize>=(1024*1024)?pciSize/(1024*1024):pciSize/1024,
	   pciSize>=(1024*1024)?"MB":"KB");

    pciSize = xsHandle->fpgaAdrsSize;
    printk("XS: FPGA at ............................ %p (%p) (MEM - %3d %s)\n",
	   xsHandle->fpgaAdrs,
	   xsHandle->fpgaAdrsPhys,
	   pciSize>=(1024*1024)?pciSize/(1024*1024):pciSize/1024,
	   pciSize>=(1024*1024)?"MB":"KB");

    pciSize = xsHandle->hostAdrsSize;
    printk("XS: Host bus at ........................ %p (%p) (MEM - %3d %s)\n",
	   xsHandle->hostAdrs,
	   xsHandle->hostAdrsPhys,
	   pciSize>=(1024*1024)?pciSize/(1024*1024):pciSize/1024,
	   pciSize>=(1024*1024)?"MB":"KB");

    pciSize = xsHandle->wpathAdrsSize;
    printk("XS: WinPath at ......................... %p (%p) (MEM - %3d %s)\n",
	   xsHandle->wpathAdrs,
	   xsHandle->wpathAdrsPhys,
	   pciSize>=(1024*1024)?pciSize/(1024*1024):pciSize/1024,
	   pciSize>=(1024*1024)?"MB":"KB");

    pciSize = xsHandle->paramAdrsSize;    
    printk("XS: Parameter bus at ................... %p (%p) (MEM - %3d %s)\n",
	   xsHandle->paramAdrs,
	   xsHandle->paramAdrsPhys,
	   pciSize>=(1024*1024)?pciSize/(1024*1024):pciSize/1024,
	   pciSize>=(1024*1024)?"MB":"KB");

    pciSize = xsHandle->packetAdrsSize;
    printk("XS: Packet bus at ...................... %p (%p) (MEM - %3d %s)\n",
	   xsHandle->packetAdrs,
	   xsHandle->packetAdrsPhys,
	   pciSize>=(1024*1024)?pciSize/(1024*1024):pciSize/1024,
	   pciSize>=(1024*1024)?"MB":"KB");

    return 0;
}

static void consoleIntr(void *arg) {
    XS_HANDLE        *xsHandle;
    unsigned int     data;
    unsigned long    flags;

    xsHandle = (XS_HANDLE *)arg;

    spin_lock_irqsave(&xsHandle->xsConsole_lock, flags);

    XS_READ_32(xsHandle->fpgaAdrs + 0x46c/4, data);

    if(data == 0x12ab34cd) {
	xsHandle->consoleBuf[xsHandle->consoleWrPtr] = -1;
    } else if(data == 0x15263748) {
	xsHandle->consoleBuf[xsHandle->consoleWrPtr] = -2;
    } else {
	xsHandle->consoleBuf[xsHandle->consoleWrPtr] = (char)data;
    }

    xsHandle->consoleWrPtr = (xsHandle->consoleWrPtr + 1) % PCI_CONSOLE_BUF_SIZE;
    
    if(xsHandle->consoleWrPtr == xsHandle->consoleRdPtr) {
	xsHandle->consoleWrap = 1;
	xsHandle->consoleRdPtr = (xsHandle->consoleRdPtr + 1) % PCI_CONSOLE_BUF_SIZE;
    }

    spin_unlock_irqrestore(&xsHandle->xsConsole_lock, flags);

    // to say that the transmitter is empty
    XS_WRITE_32(xsHandle->fpgaAdrs + 0x458/4, 1);

    return;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
static irqreturn_t xsIntr(int irq, void *intno, struct pt_regs *regs)
#else
     static irqreturn_t xsIntr(int irq, void *intno)
#endif
{
    
    XS_HANDLE *xsHandle;
    unsigned long flags;
    unsigned int  ih, il, fpga_isr0, val, mh, ml;

    xsHandle = intno;

    spin_lock_irqsave(&xsHandle->xsInterrupt_lock, flags);

    // Read interrupt vector & clear
    GET(xsHandle->wpathAdrs, ih, MAP_IC_INTERUPT_H);
    if(ih) {
	STORE(xsHandle->wpathAdrs, ih, MAP_IC_INTERUPT_H);
    }
    GET(xsHandle->wpathAdrs, il, MAP_IC_INTERUPT_L);
    if(il) {
	STORE(xsHandle->wpathAdrs, il, MAP_IC_INTERUPT_L);
    }

    GET(xsHandle->wpathAdrs, mh, MAP_IC_MASK_EXTERNAL_H);
    GET(xsHandle->wpathAdrs, ml, MAP_IC_MASK_EXTERNAL_L);

    if((ih & 0x00f00000) & mh) {
	if(ih & 0x00800000) {
	    int_nbr[IV_WMMIQ3_VEC]++;
	}
	if(ih & 0x00400000) {
	    int_nbr[IV_WMMIQ2_VEC]++;
	}
	if(ih & 0x00200000) {
	    int_nbr[IV_WMMIQ1_VEC]++;
	}
	if(ih & 0x00100000) {
	    int_nbr[IV_WMMIQ0_VEC]++;
	}

	ser_qs_handler(ih, NULL);
    }
    
    if((ih & 0x0003c000) & mh) {
	// Mask the interrupt, will be unmasked by WDDI
	//GET(xsHandle->wpathAdrs, val, MAP_IC_MASK_EXTERNAL_H);
	//STORE(xsHandle->wpathAdrs, val & ~0x0003c000, MAP_IC_MASK_EXTERNAL_H);

	if(ih & 0x00020000) {
	    int_nbr[IV_WMMIQO3_VEC]++;
	}
	if(ih & 0x00010000) {
	    int_nbr[IV_WMMIQO2_VEC]++;
	}
	if(ih & 0x00008000) {
	    int_nbr[IV_WMMIQO1_VEC]++;
	}
	if(ih & 0x00004000) {
	    int_nbr[IV_WMMIQO0_VEC]++;
	}

	ser_qo_handler(ih, NULL);
    }
    
    if((il & 0x0000003f) & ml) {

	if(il & 0x00000020) {
	    int_nbr[IV_BERR4X_VEC]++;
	}
	if(il & 0x00000010) {
	    int_nbr[IV_BERR4D_VEC]++;
	}
	if(il & 0x00000008) {
	    int_nbr[IV_BERR3I_VEC]++;
	}
	if(il & 0x00000004) {
	    int_nbr[IV_BERR3_VEC]++;
	}
	if(il & 0x00000002) {
	    int_nbr[IV_BERR2_VEC]++;
	}
	if(il & 0x00000001) {
	    int_nbr[IV_BERR1_VEC]++;
	}

	sys_err_handler(il, NULL);
    }

    if(il & 0x20000000) {

	int_nbr[IV_EINT2_VEC]++;

	XS_READ_32(gxsHandle->fpgaAdrs + 0x410/4, fpga_isr0);

	if(fpga_isr0 & 0x80) {
	    consoleIntr(xsHandle);
	    XS_WRITE_32(gxsHandle->fpgaAdrs + 0x410/4, 0x80);
	}
    }

    if((ih & 0x00002000) & mh) {
	// Mask the interrupt, will be unmasked by WDDI
	GET(xsHandle->wpathAdrs, val, MAP_IC_MASK_EXTERNAL_H);
	STORE(xsHandle->wpathAdrs, val &~0x00002000, MAP_IC_MASK_EXTERNAL_H);

	int_nbr[IV_SERDES2_VEC]++;

	sgmii_an_handler(ih, NULL);
    }

    if((ih & 0x00001000) & mh) {
	// Mask the interrupt, will be unmasked by WDDI
	GET(xsHandle->wpathAdrs, val, MAP_IC_MASK_EXTERNAL_H);
	STORE(xsHandle->wpathAdrs, val &~0x00001000, MAP_IC_MASK_EXTERNAL_H);

	int_nbr[IV_SERDES1_VEC]++;

	serdes_handler(ih, NULL);
    }

    if((il & 0x40000000) & ml) {
	// Mask the interrupt, will be unmasked by WDDI
	GET(xsHandle->wpathAdrs, val, MAP_IC_MASK_EXTERNAL_L);
	STORE(xsHandle->wpathAdrs, val &~0x40000000, MAP_IC_MASK_EXTERNAL_L);

	int_nbr[IV_EINT3_VEC]++;

	int3_handler(il, NULL);
    }

    spin_unlock_irqrestore(&xsHandle->xsInterrupt_lock, flags);

    return IRQ_HANDLED;
}

static unsigned int getFreq( unsigned int pllcfg1 )
{
    unsigned int pllcfg_val = WDS3_WINPATH_RIF_READ(pllcfg1);
    unsigned int pfd        = (pllcfg_val & 0x1fff);
    unsigned int prd        = ((pllcfg_val>>16) & 0x3f);
    unsigned int freq       = (WDS3_EXTERNAL_CLOCK/2);

    freq /= (prd+1);
    freq *= (pfd+1);
    freq += 1000;   /* Fix round off error */

    return freq;
}

// PROC
int mmap_read_proc_interrupts(char  *page, 
			      char  **start, 
			      off_t offset, 
			      int   count, 
			      int   *eof, 
			      void  *data)
{
    XS_HANDLE  *xsHandle;
    int len;
    unsigned int  mh, ml;

    xsHandle = (XS_HANDLE  *)data;
    len = 0;
    
    GET(xsHandle->wpathAdrs, mh, MAP_IC_MASK_EXTERNAL_H);
    GET(xsHandle->wpathAdrs, ml, MAP_IC_MASK_EXTERNAL_L);

    len += sprintf(page+len, "\nInterrupt                 Nbr      Signal Sent Enabled\n");
    len+= sprintf(page+len, "WMMIQ0     : %16ld %16ld       %d\n", int_nbr[IV_WMMIQ0_VEC],  sigsent_nbr[IV_WMMIQ0_VEC],  (mh & (1<<(IV_WMMIQ0_VEC-32)))?1:0);
    len+= sprintf(page+len, "WMMIQ1     : %16ld %16ld       %d\n", int_nbr[IV_WMMIQ1_VEC],  sigsent_nbr[IV_WMMIQ1_VEC],  (mh & (1<<(IV_WMMIQ1_VEC-32)))?1:0);
    len+= sprintf(page+len, "WMMIQ2     : %16ld %16ld       %d\n", int_nbr[IV_WMMIQ2_VEC],  sigsent_nbr[IV_WMMIQ2_VEC],  (mh & (1<<(IV_WMMIQ1_VEC-32)))?1:0);
    len+= sprintf(page+len, "WMMIQ3     : %16ld %16ld       %d\n", int_nbr[IV_WMMIQ3_VEC],  sigsent_nbr[IV_WMMIQ3_VEC],  (mh & (1<<(IV_WMMIQ3_VEC-32)))?1:0);

    len+= sprintf(page+len, "WMMIQO0    : %16ld %16ld       %d\n", int_nbr[IV_WMMIQO0_VEC], sigsent_nbr[IV_WMMIQO0_VEC], (mh & (1<<(IV_WMMIQO0_VEC-32)))?1:0);
    len+= sprintf(page+len, "WMMIQO1    : %16ld %16ld       %d\n", int_nbr[IV_WMMIQO1_VEC], sigsent_nbr[IV_WMMIQO1_VEC], (mh & (1<<(IV_WMMIQO1_VEC-32)))?1:0);
    len+= sprintf(page+len, "WMMIQO2    : %16ld %16ld       %d\n", int_nbr[IV_WMMIQO2_VEC], sigsent_nbr[IV_WMMIQO2_VEC], (mh & (1<<(IV_WMMIQO2_VEC-32)))?1:0);
    len+= sprintf(page+len, "WMMIQO3    : %16ld %16ld       %d\n", int_nbr[IV_WMMIQO3_VEC], sigsent_nbr[IV_WMMIQO3_VEC], (mh & (1<<(IV_WMMIQO3_VEC-32)))?1:0);

    len+= sprintf(page+len, "SERDES     : %16ld %16ld       %d\n", int_nbr[IV_SERDES1_VEC], sigsent_nbr[IV_SERDES1_VEC], (mh & (1<<(IV_SERDES1_VEC-32)))?1:0);
    len+= sprintf(page+len, "AN         : %16ld %16ld       %d\n", int_nbr[IV_SERDES2_VEC], sigsent_nbr[IV_SERDES2_VEC], (mh & (1<<(IV_SERDES2_VEC-32)))?1:0);

    len+= sprintf(page+len, "EINT1      : %16ld %16ld       %d\n", int_nbr[IV_EINT1_VEC],   sigsent_nbr[IV_EINT1_VEC],   (ml & (1<<(IV_EINT1_VEC)))?1:0);
    len+= sprintf(page+len, "EINT2(cons): %16ld %16ld       %d\n", int_nbr[IV_EINT2_VEC],   sigsent_nbr[IV_EINT2_VEC],   (ml & (1<<(IV_EINT2_VEC)))?1:0);
    len+= sprintf(page+len, "EINT3(UFE4): %16ld %16ld       %d\n", int_nbr[IV_EINT3_VEC],   sigsent_nbr[IV_EINT3_VEC],   (ml & (1<<(IV_EINT3_VEC)))?1:0);
    len+= sprintf(page+len, "EINT4      : %16ld %16ld       %d\n", int_nbr[IV_EINT4_VEC],   sigsent_nbr[IV_EINT4_VEC],   (ml & (1<<(IV_EINT4_VEC)))?1:0);

    len+= sprintf(page+len, "BER_PARAM  : %16ld %16ld       %d\n", int_nbr[IV_BERR1_VEC],   sigsent_nbr[IV_BERR1_VEC],   (ml & (1<<(IV_BERR1_VEC)))?1:0);
    len+= sprintf(page+len, "BER_PACKT  : %16ld %16ld       %d\n", int_nbr[IV_BERR2_VEC],   sigsent_nbr[IV_BERR2_VEC],   (ml & (1<<(IV_BERR2_VEC)))?1:0);
    len+= sprintf(page+len, "BER_INTL   : %16ld %16ld       %d\n", int_nbr[IV_BERR3_VEC],   sigsent_nbr[IV_BERR3_VEC],   (ml & (1<<(IV_BERR3_VEC)))?1:0);
    len+= sprintf(page+len, "BER_RIF    : %16ld %16ld       %d\n", int_nbr[IV_BERR3I_VEC],  sigsent_nbr[IV_BERR3I_VEC],  (ml & (1<<(IV_BERR3I_VEC)))?1:0);
    len+= sprintf(page+len, "BER_APPL   : %16ld %16ld       %d\n", int_nbr[IV_BERR4D_VEC],  sigsent_nbr[IV_BERR4D_VEC],  (ml & (1<<(IV_BERR4D_VEC)))?1:0);
    len+= sprintf(page+len, "BER_ASRAM  : %16ld %16ld       %d\n", int_nbr[IV_BERR4X_VEC],  sigsent_nbr[IV_BERR4X_VEC],  (ml & (1<<(IV_BERR4X_VEC)))?1:0);
    
    *eof = 1;

    return len;
}

static int  mmap_probe (struct pci_dev *pdev, const struct pci_device_id *ent)
{
    XS_HANDLE  *xsHandle;
    unsigned int val;
    int          err;

    if(ent) {
	if(mmap_log_level > 0) {
	    printk( "mmap_probe: %04x:%04x %04x:%04x\n",
		    ent->vendor,
		    ent->device,
		    ent->subvendor,
		    ent->subdevice);
	}

	if((err = pci_enable_device(pdev))) {
	    printk("mmap_probe: Error while doing pci_enable_device()\n");
	    return err;
	}
	
	if((err = pci_set_dma_mask(pdev, DMA_BIT_MASK(32)))) {
	    printk("mmap_probe: Error while doing pci_set_dma_mask()\n");
	    pci_disable_device(pdev);
	    return err;
	}
	
	if((err = pci_request_regions(pdev, mmap_driver_name))) {
	    printk("mmap_probe: Error while doing pci_request_regions()\n");
	    pci_disable_device(pdev);
	    return err;
	}

	pci_set_master(pdev);

	xsHandle = (XS_HANDLE *)kmalloc(sizeof(XS_HANDLE), GFP_KERNEL);
	if(xsHandle == NULL) {
	    printk("xs_probe: Error while allocating xsHandle (%d bytes)\n", (int)sizeof(XS_HANDLE));
	    pci_release_regions(pdev);
	    pci_disable_device(pdev);
	    return -ENOMEM;
	}
    
	memset(xsHandle, 0, sizeof(XS_HANDLE));
	gxsHandle = xsHandle;
	
	pci_set_drvdata(pdev, xsHandle);
    
	xsHandle->pdev = pdev;
	
	// MAP all BARs
	xsPciMap(pdev, 0, &xsHandle->siu3Adrs,   &xsHandle->siu3AdrsPhys,   &xsHandle->siu3AdrsSize);
	xsPciMap(pdev, 1, &xsHandle->fpgaAdrs,   &xsHandle->fpgaAdrsPhys,   &xsHandle->fpgaAdrsSize);
	xsPciMap(pdev, 2, &xsHandle->hostAdrs,   &xsHandle->hostAdrsPhys,   &xsHandle->hostAdrsSize);
	xsPciMap(pdev, 3, &xsHandle->wpathAdrs,  &xsHandle->wpathAdrsPhys,  &xsHandle->wpathAdrsSize);
	xsPciMap(pdev, 4, &xsHandle->paramAdrs,  &xsHandle->paramAdrsPhys,  &xsHandle->paramAdrsSize);
	xsPciMap(pdev, 5, &xsHandle->packetAdrs, &xsHandle->packetAdrsPhys, &xsHandle->packetAdrsSize);
    } else {
	xsHandle = gxsHandle;
    }

    if(mmap_log_level > 0) {
	xsPciMapShow(xsHandle);
    }

    // Init SIU mapping
    
    // Clear BAR offsets
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114fc/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x11500/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x11504/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x11508/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x1150c/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x11510/4, xsByteSwap(0));
    
    // Disable all SIUs
    XS_WRITE_32(xsHandle->wpathAdrs + 0x11490/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x11498/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114A0/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114A8/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114B0/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114B8/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114C0/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114C8/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114D0/4, xsByteSwap(0));
    
    // Clear SIU offsets
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114d4/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114d8/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114dc/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114e0/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114e4/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114e8/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114ec/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114f0/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114d4/4, xsByteSwap(0));
    
    // Set all SIUs
    XS_WRITE_32(xsHandle->wpathAdrs + 0x11490/4, xsByteSwap(0x20000001));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x11498/4, xsByteSwap(0x30000001));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114A0/4, xsByteSwap(0x1fc00001));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114A8/4, xsByteSwap(0x1e000001));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114B0/4, xsByteSwap(0x00000001));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114B8/4, xsByteSwap(0x1f000001));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114C0/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114C8/4, xsByteSwap(0));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114D0/4, xsByteSwap(0));
    
    // Set BAR offsets (SIU - BAR -> BAR - (SIU - BAR) = SIU
    XS_WRITE_32(xsHandle->wpathAdrs + 0x115d4/4, xsByteSwap(0x10));
    XS_READ_32 (xsHandle->wpathAdrs + 0x115dc/4, val);
    //printk("BAR0 = %08x\n", xsByteSwap(val));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x114fc/4, xsByteSwap((xsByteSwap(val) & 0xfffffff0) - 0x1fc00000));
    
    XS_WRITE_32(xsHandle->wpathAdrs + 0x115d4/4, xsByteSwap(0x14));
    XS_READ_32 (xsHandle->wpathAdrs + 0x115dc/4, val);
    //printk("BAR1 = %08x\n", xsByteSwap(val));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x11500/4, xsByteSwap((xsByteSwap(val) & 0xfffffff0) - 0x1f000000));
    
    XS_WRITE_32(xsHandle->wpathAdrs + 0x115d4/4, xsByteSwap(0x18));
    XS_READ_32 (xsHandle->wpathAdrs + 0x115dc/4, val);
    //printk("BAR2 = %08x\n", xsByteSwap(val));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x11504/4, xsByteSwap((xsByteSwap(val) & 0xfffffff0) - 0x00000000));
    
    // CHECK IF WE CAN READ THE FPGA
    XS_READ_32 (xsHandle->fpgaAdrs + 0x2C/4, val);
    //printk("FPGA ID = %08x\n", val);
    
    XS_WRITE_32(xsHandle->wpathAdrs + 0x115d4/4, xsByteSwap(0x1C));
    XS_READ_32 (xsHandle->wpathAdrs + 0x115dc/4, val);
    //printk("BAR3 = %08x\n", xsByteSwap(val));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x11508/4, xsByteSwap((xsByteSwap(val) & 0xfffffff0) - 0x1e000000));
    
    XS_WRITE_32(xsHandle->wpathAdrs + 0x115d4/4, xsByteSwap(0x20));
    XS_READ_32 (xsHandle->wpathAdrs + 0x115dc/4, val);
    //printk("BAR4 = %08x\n", xsByteSwap(val));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x1150c/4, xsByteSwap((xsByteSwap(val) & 0xfffffff0) - 0x20000000));
    
    XS_WRITE_32(xsHandle->wpathAdrs + 0x115d4/4, xsByteSwap(0x24));
    XS_READ_32 (xsHandle->wpathAdrs + 0x115dc/4, val);
    //printk("BAR5 = %08x\n", xsByteSwap(val));
    XS_WRITE_32(xsHandle->wpathAdrs + 0x11510/4, xsByteSwap((xsByteSwap(val) & 0xfffffff0) - 0x30000000));

    // Enable PCIe interrupts

    // IC_MODE - level, INT1 is output, WMM1 is used (IRQ on PARAM)
    STORE(xsHandle->wpathAdrs, 0x0001f000, MAP_IC_INTMODE);
    
    // INT2 -> PCIe
    STORE(xsHandle->wpathAdrs, 0x20000000, MAP_IC_MASK_EXTERNAL_L);
    STORE(xsHandle->wpathAdrs, 0x00000000, MAP_IC_MASK_EXTERNAL_H);
    
    // Enable PCIe interrupts
    STORE(xsHandle->wpathAdrs, 0x80000000, MAP_PCIE_REG_OF_EP_SEND_INTERRUPT);
   
    // Restore I2C frequency
    STORE(xsHandle->wpathAdrs, 0x320, MAP_I2C_CLKDIV); // 250 KHz

    spin_lock_init(&xsHandle->xsInterrupt_lock);

    // Fill board info manually, no way to get this from WinMon...
    wds.wds_winpath_no                              = 1;
    wds.wds_winmon_ver                              = 0x04020400;
    wds.wds_clock_freq_external                     = WDS3_EXTERNAL_CLOCK;
    wds.wds_winpath_info[0].wp_id                   = 0;
    wds.wds_winpath_info[0].wp_clock_freq_siu[0]    = getFreq(MAP_CLK_SIU1_PLLCFG1);
    wds.wds_winpath_info[0].wp_clock_freq_siu[1]    = getFreq(MAP_CLK_SIU2_PLLCFG1);
    wds.wds_winpath_info[0].wp_clock_freq_siu[2]    = getFreq(MAP_CLK_INTERNAL_PLLCFG1);
    wds.wds_winpath_info[0].wp_clock_freq_siu[3]    = getFreq(MAP_CLK_INTERNAL_PLLCFG1)/2;
    wds.wds_winpath_info[0].wp_clock_freq_siu[4]    = getFreq(MAP_CLK_SIU4D_PLLCFG1);
    wds.wds_winpath_info[0].wp_clock_freq_mips      = getFreq(MAP_CLK_MIPS_PLLCFG1);

    wds.wds_winpath_info[0].wds_winpath_no_of_banks = 7;

    wds.wds_winpath_info[0].wp_bank_info[0].wp_bus                        = WDS3_SIU_BUS_PARAM_NUM;
    wds.wds_winpath_info[0].wp_bank_info[0].wp_bus_active                 = 1;
    wds.wds_winpath_info[0].wp_bank_info[0].wp_phys_bank_base_addr        = 0x20000000;
    wds.wds_winpath_info[0].wp_bank_info[0].wp_phys_bank_size             = xsHandle->paramAdrsSize;
    wds.wds_winpath_info[0].wp_bank_info[0].wp_application_bank_base_addr = 0x20000000;
    wds.wds_winpath_info[0].wp_bank_info[0].wp_application_bank_size      = xsHandle->paramAdrsSize;

    wds.wds_winpath_info[0].wp_bank_info[1].wp_bus                        = WDS3_SIU_BUS_PACKET_NUM;
    wds.wds_winpath_info[0].wp_bank_info[1].wp_bus_active                 = 1;
    wds.wds_winpath_info[0].wp_bank_info[1].wp_phys_bank_base_addr        = 0x30000000;
    wds.wds_winpath_info[0].wp_bank_info[1].wp_phys_bank_size             = xsHandle->packetAdrsSize;
    wds.wds_winpath_info[0].wp_bank_info[1].wp_application_bank_base_addr = 0x30000000;
    wds.wds_winpath_info[0].wp_bank_info[1].wp_application_bank_size      = xsHandle->packetAdrsSize;

    wds.wds_winpath_info[0].wp_bank_info[2].wp_bus                        = WDS3_SIU_BUS_INTERNAL_NUM;
    wds.wds_winpath_info[0].wp_bank_info[2].wp_bus_active                 = 1;
    wds.wds_winpath_info[0].wp_bank_info[2].wp_phys_bank_base_addr        = 0x1fc00000;
    wds.wds_winpath_info[0].wp_bank_info[2].wp_phys_bank_size             = xsHandle->siu3AdrsSize;
    wds.wds_winpath_info[0].wp_bank_info[2].wp_application_bank_base_addr = 0x1fc00000;
    wds.wds_winpath_info[0].wp_bank_info[2].wp_application_bank_size      = xsHandle->siu3AdrsSize;

    // I think no bus can be mapped in 0 for WDDI...
    wds.wds_winpath_info[0].wp_bank_info[3].wp_bus                        = WDS3_SIU_BUS_HOST_NUM;
    wds.wds_winpath_info[0].wp_bank_info[3].wp_bus_active                 = 1;
    wds.wds_winpath_info[0].wp_bank_info[3].wp_phys_bank_base_addr        = 0x01000000;//0x00000000;
    wds.wds_winpath_info[0].wp_bank_info[3].wp_phys_bank_size             = xsHandle->hostAdrsSize - 0x01000000;
    wds.wds_winpath_info[0].wp_bank_info[3].wp_application_bank_base_addr = 0x01000000;//0x00000000;
    wds.wds_winpath_info[0].wp_bank_info[3].wp_application_bank_size      = xsHandle->hostAdrsSize - 0x01000000;

    wds.wds_winpath_info[0].wp_bank_info[4].wp_bus                        = WDS3_SIU_BUS_WMM5_NUM;
    wds.wds_winpath_info[0].wp_bank_info[4].wp_bus_active                 = 1;
    wds.wds_winpath_info[0].wp_bank_info[4].wp_phys_bank_base_addr        = 0x40000000;
    wds.wds_winpath_info[0].wp_bank_info[4].wp_phys_bank_size             = 0x10000000;
    wds.wds_winpath_info[0].wp_bank_info[4].wp_application_bank_base_addr = 0x40000000;
    wds.wds_winpath_info[0].wp_bank_info[4].wp_application_bank_size      = 0x10000000;

    wds.wds_winpath_info[0].wp_bank_info[5].wp_bus                        = WDS3_SIU_BUS_RIF_NUM;
    wds.wds_winpath_info[0].wp_bank_info[5].wp_bus_active                 = 1;
    wds.wds_winpath_info[0].wp_bank_info[5].wp_phys_bank_base_addr        = 0x1e000000;
    wds.wds_winpath_info[0].wp_bank_info[5].wp_phys_bank_size             = xsHandle->wpathAdrsSize;
    wds.wds_winpath_info[0].wp_bank_info[5].wp_application_bank_base_addr = 0x1e000000;
    wds.wds_winpath_info[0].wp_bank_info[5].wp_application_bank_size      = xsHandle->wpathAdrsSize;

    wds.wds_winpath_info[0].wp_bank_info[6].wp_bus                        = WDS3_FPGA_NUM;
    wds.wds_winpath_info[0].wp_bank_info[6].wp_bus_active                 = 1;
    wds.wds_winpath_info[0].wp_bank_info[6].wp_phys_bank_base_addr        = 0x1f000000;
    wds.wds_winpath_info[0].wp_bank_info[6].wp_phys_bank_size             = xsHandle->fpgaAdrsSize;
    wds.wds_winpath_info[0].wp_bank_info[6].wp_application_bank_base_addr = 0x1f000000;
    wds.wds_winpath_info[0].wp_bank_info[6].wp_application_bank_size      = xsHandle->fpgaAdrsSize;

    if(mmap_log_level > 0) {
	int i;

	printk("\nWinpath info: wp id %d mips freq %d no. of banks %d\n",
	       wds.wds_winpath_info[0].wp_id,
	       wds.wds_winpath_info[0].wp_clock_freq_mips,
	       wds.wds_winpath_info[0].wds_winpath_no_of_banks);

	for(i=0; i<5; i++) {
	    printk("siu %d freq %d\n", i, wds.wds_winpath_info[0].wp_clock_freq_siu[i]);
	}

	for(i=0; i<7; i++) {
	    printk("\nBank info: bus %d phys_base 0x%08x phys_size 0x%08x app_base 0x%08x app_size 0x%08x\n",
		   wds.wds_winpath_info[0].wp_bank_info[i].wp_bus,
		   wds.wds_winpath_info[0].wp_bank_info[i].wp_phys_bank_base_addr,   
		   wds.wds_winpath_info[0].wp_bank_info[i].wp_phys_bank_size,   
		   wds.wds_winpath_info[0].wp_bank_info[i].wp_application_bank_base_addr,  
		   wds.wds_winpath_info[0].wp_bank_info[i].wp_application_bank_size);
	}
    }

    // Set clock for Ethernet
    XS_WRITE_32(xsHandle->wpathAdrs + 0xc200/4, xsByteSwap(0xC0000001));

    if(ent) {
	if(ent->subdevice == 0x10) {
	    unsigned short sval;
	    
	    // Deassert mezzanine reset and set leds to check
	    XS_READ_32 (xsHandle->fpgaAdrs + 0x1020/4, val);
	    XS_WRITE_32(xsHandle->fpgaAdrs + 0x1020/4, 0x00001529);
	    
	    // Assert TX_ENABLE for the SFPs
	    XS_READ_16((unsigned short *)(xsHandle->fpgaAdrs) + 0x00300000/2 + 4, sval);
	    XS_WRITE_16((unsigned short *)(xsHandle->fpgaAdrs) + 0x00300000/2 + 4, sval & ~0x1111);
	} 
    }

    // Init xsconsole
    spin_lock_init(&xsHandle->xsConsole_lock);
    xsHandle->consoleWrPtr = xsHandle->consoleRdPtr = xsHandle->consoleWrap = 0;

    // say to WinMon to print on PCI console
    XS_READ_32 (xsHandle->fpgaAdrs + 0x1020/4, val);
    XS_WRITE_32(xsHandle->fpgaAdrs + 0x1020/4, val | 0xc0000000);

    // Enable console interrupts
    XS_WRITE_32(xsHandle->fpgaAdrs + 0x418/4, 0x80);

    // Connect interrupt routine
    if(ent) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
	err = request_irq(xsHandle->pdev->irq, xsIntr, SA_SHIRQ,    "wp_mmap", xsHandle);
#else
	err = request_irq(xsHandle->pdev->irq, xsIntr, IRQF_SHARED, "wp_mmap", xsHandle);
#endif
	if(err) {
	    return err;
	}
    } else {
	enable_irq(xsHandle->pdev->irq);
    }

    usage_count = 0;

    // Clear proc stats
    {
	int i;

	for(i=0; i<64; i++) {
	    int_nbr[i] = sigsent_nbr[i] = 0;
	}
    }

    // Proc init
    if(proc_created == 0) {
	if(proc_mkdir("driver/wp3", NULL) == NULL) {
	    printk("Could not create proc directory /proc/driver/wp3\n");
	}
	
	if(create_proc_read_entry("driver/wp3/interrupts", 0, NULL, mmap_read_proc_interrupts, xsHandle) == NULL) {
	    printk("Could not create proc entry /proc/driver/wp3/interrupts\n");
	}
	
	proc_created = 1;	
    }

    //printk("wp_mmapdrv device initalized\n");

    return 0;
}

static void mmap_remove(struct pci_dev *pdev)
{
    XS_HANDLE *xsHandle;
    unsigned int val;

    xsHandle= pci_get_drvdata(pdev);

    free_irq(xsHandle->pdev->irq, xsHandle);

    // say to WinMon to stop printing on PCI console
    XS_READ_32 (xsHandle->fpgaAdrs + 0x1020/4, val);
    XS_WRITE_32(xsHandle->fpgaAdrs + 0x1020/4, val & ~0xc0000000);

    // Disable FPGA interrupts
    XS_WRITE_32(xsHandle->fpgaAdrs + 0x418/4, 0);

    iounmap(xsHandle->siu3Adrs);
    iounmap(xsHandle->packetAdrs);
    iounmap(xsHandle->paramAdrs);
    iounmap(xsHandle->wpathAdrs);
    iounmap(xsHandle->hostAdrs);
    iounmap(xsHandle->fpgaAdrs);

    kfree(xsHandle);

    pci_release_regions(pdev);
    pci_disable_device(pdev);

    // Proc
    if(proc_created) {
	remove_proc_entry("driver/wp3/interrupts", NULL);
	remove_proc_entry("driver/wp3", NULL);
    }

    return;
}

/* device open method */
static int mmapdrv_open(struct inode *inode, struct file *file)
{
    int mode = file->f_flags;
    unsigned int ml, mh;

    if (!try_module_get(THIS_MODULE))
        return -ESTALE;

    if(mmap_log_level > 0) {
	printk("mmapdrv_open current=%p, flags=%x, mmapdrv_dev=%p\n", 
	       current, mode & O_NOFOLLOW, mmapdrv_dev);
	printk("pid=%d, parent = %p, real_parent = %p\n", 
	       current->pid, current->parent, current->real_parent);
    }

    /*The O_NOFOLLOW flag should be used if the current process
      does not want to except interrupts*/
    if(!(mode & O_NOFOLLOW)) {
	if (!mmapdrv_dev) {
	    mmapdrv_dev = current;
	    
	    // Enable interrupts for serials and bus errors
	    
	    // INT2, BERR, QS and QO -> PCIe 
	    // UFE4 and AN are enabled in WDDI platform
	    STORE(gxsHandle->wpathAdrs, 0x2000003f, MAP_IC_MASK_EXTERNAL_L);
	    STORE(gxsHandle->wpathAdrs, 0x00f3c000, MAP_IC_MASK_EXTERNAL_H);
	    //STORE(gxsHandle->wpathAdrs, 0x0003c000, MAP_IC_MASK_EXTERNAL_H); // to test overruns

	    if(mmap_log_level > 0) {
		GET(gxsHandle->wpathAdrs, ml, MAP_IC_MASK_EXTERNAL_L);
		GET(gxsHandle->wpathAdrs, mh, MAP_IC_MASK_EXTERNAL_H);
		
		printk("Enabled interrupts ml=%08x/mh=%08x\n", ml, mh);
	    }
	}
    }
    
    usage_count++;

    return 0;
}
   
/* device close method */
static int mmapdrv_release(struct inode *inode, struct file *file)
{
    unsigned int ml, mh;

    if(mmap_log_level > 0) {
	printk("mmapdrv_release current=%p, mmapdrv_dev=%p\n", current, mmapdrv_dev);
	printk("pid=%d, parent = %p, real_parent = %p, parent pid=%d, leader pid=%d\n", 
	       current->pid, current->parent, current->real_parent, current->parent->pid,
	       current->group_leader->pid);
    }

    //if ((current == mmapdrv_dev) && mmapdrv_dev)
    if(mmapdrv_dev) {
	if(current->group_leader->pid == mmapdrv_dev->pid)
	    {
		// Disable interrupts for serials and bus errors
		
		// INT2 -> PCIe
		STORE(gxsHandle->wpathAdrs, 0x20000000, MAP_IC_MASK_EXTERNAL_L);
		STORE(gxsHandle->wpathAdrs, 0x00000000, MAP_IC_MASK_EXTERNAL_H);
		
		if(mmap_log_level > 0) {
		    GET(gxsHandle->wpathAdrs, ml, MAP_IC_MASK_EXTERNAL_L);
		    GET(gxsHandle->wpathAdrs, mh, MAP_IC_MASK_EXTERNAL_H);
		    
		    printk("Disabled interrupts ml=%08x/mh=%08x\n", ml, mh);
		}
		
		mmapdrv_dev = NULL;
	    }
    }
   
    module_put (THIS_MODULE);
   
    usage_count--;

    return(0);
}

/* device memory map method */
/* 2.4.x: this method is called from do_mmap_pgoff, from
   do_mmap, from the syscall. The caller of do_mmap grabs
   the mm semaphore. So we are protected from races here.
*/
static int mmapdrv_mmap(struct file *file, struct vm_area_struct *vma)
{
    unsigned long offset = vma->vm_pgoff<<PAGE_SHIFT;
    unsigned long size   = vma->vm_end - vma->vm_start;
    struct inode *inode  = file->f_dentry->d_inode;
    wds_winpath_mem_bank_info_t *bank = NULL;
    int minor      = MINOR(inode->i_rdev);
    int bank_index = minor;

    if((minor > MAX_WP_BANKS) || (minor<0 ))
	{
	    printk("invalid minor number\n");
	    return(-ENXIO);
	}

    bank = &(wds.wds_winpath_info[0].wp_bank_info[bank_index]);

    if(size > bank->wp_application_bank_size)
	{
	    printk("size too big\n");
	    return(-ENXIO);
	}

    if (offset & ~PAGE_MASK)
	{
	    printk("offset not aligned: %ld\n", offset);
	    return -ENXIO;
	}

    /* we only support shared mappings. Copy on write mappings are
       rejected here. A shared mapping that is writeable must have the
       shared flag set.
    */
    if ((vma->vm_flags & VM_WRITE) && !(vma->vm_flags & VM_SHARED))
	{
	    printk("writeable mappings must be shared, rejecting\n");
	    return(-EINVAL);
	}

    /* We want this page to be uncached and we do not want to have this
     * area swapped out, so we mark it as reserved */
    vma->vm_page_prot  = pgprot_noncached(vma->vm_page_prot);
    vma->vm_flags     |= (VM_RESERVED | VM_IO);

    if(mmap_log_level > 0) {
	printk("Maping 0x%08lx bytes from 0x%08lx to 0x%08lx\n", size, mmap_virt_to_phys(bank->wp_application_bank_base_addr), vma->vm_start);
    }

    if (remap_pfn_range(vma,
			vma->vm_start,
			mmap_virt_to_phys(bank->wp_application_bank_base_addr) >> PAGE_SHIFT,
			size,
			vma->vm_page_prot)) {
	printk("remap page range failed\n");
	return -ENXIO;
    }

    return(0);
}

static void xs_wait(unsigned int time)
{
    while(time--);   
    return;
}

#ifdef HAVE_COMPAT_IOCTL
static long mmapdrv_compat_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    if(cmd == XS_IOCTL_ENV_GET) cmd = XS_IOCTL_ENV_GET32;

    return mmapdrv_unlocked_ioctl(file, cmd, (unsigned long)compat_ptr(arg));
}
#endif

#ifdef HAVE_UNLOCKED_IOCTL
static long mmapdrv_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
#if 0
{} // emacs indent
#endif
#else
static int mmapdrv_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
#endif
{
    uint32_t value;

    //printk("mmapdrv_ioctl: got cmd %x\n", cmd);

    switch (cmd)
	{
	case MMAP_IOCTL_READ_BOARD_GEN_CONF:
	    { 
		if(mmap_log_level > 0) {
		    printk("mmapdrv_ioctl: MMAP_IOCTL_READ_BOARD_GEN_CONF\n");
		}

		if (copy_to_user((void *) arg, &wds, sizeof(wds_board_info_t)))
		    return -EFAULT;            
	    }
	    break;
	case MMAP_IOCTL_SUPPRESS_DUP_SIGNAL:
	    {
		if(mmap_log_level > 0) {
		    printk("mmapdrv_ioctl: MMAP_IOCTL_SUPPRESS_DUP_SIGNAL\n");
		}

		if (copy_from_user(&value, (void *)arg, sizeof(value)))
		    return -EFAULT;
		mmapdrv_dev_cfg.suppress_dup_signals = value ? 1 : 0;
	    }
	    break;
	case MMAP_IOCTL_ENABLE_BUS_ERR_SIG:
	    {
		if(mmap_log_level > 0) {
		    printk("mmapdrv_ioctl: MMAP_IOCTL_ENABLE_BUS_ERR_SIG\n");
		}

		if (copy_from_user(&value, (void *)arg, sizeof(value)))
		    return -EFAULT;
		mmapdrv_dev_cfg.send_buserr_sig = value ? 1 : 0;
	    }
	    break;
	case MMAP_IOCTL_KERNEL_BUS_ERR_REPORT:
	    {
		if(mmap_log_level > 0) {
		    printk("mmapdrv_ioctl: MMAP_IOCTL_KERNEL_BUS_ERR_REPORT \n");
		}

		if (copy_from_user(&value, (void *)arg, sizeof(value)))
		    return -EFAULT;
		mmapdrv_dev_cfg.kernel_buserr_report = value ? 1 : 0;

		if(mmap_log_level > 0) {
		    printk("mmapdrv_ioctl: MMAP_IOCTL_KERNEL_BUS_ERR_REPORT report = %d", value);
		}
	    }
	    break;
	case MMAP_IOCTL_REPLACE_BUS_ERR_KERNEL_REPORT_WITH_SIGNAL:
	    {
		if (copy_from_user(&value, (void *)arg, sizeof(value)))
		    return -EFAULT;
		mmapdrv_dev_cfg.send_buserr_sig = value ? 1 : 0;
		mmapdrv_dev_cfg.kernel_buserr_report = value ? 0 : 1;

		if(mmap_log_level > 0) {
		    printk("mmapdrv_ioctl: MMAP_IOCTL_REPLACE_BUS_ERR_KERNEL_REPORT_WITH_SIGNAL (report = %d)\n", value);
		}
	    }
	    break;


	case XS_IOCTL_CONSOLE_WRITE:
	    {
		XS_CONSOLE_BUF_DESC   *desc;
		unsigned int          status;
		unsigned long         tout;
		int                   i;
		
		desc = kmalloc(sizeof(XS_CONSOLE_BUF_DESC), GFP_KERNEL);
					
		if (copy_from_user(desc, (void *)arg, sizeof(XS_CONSOLE_BUF_DESC))) {
		    kfree(desc);
		    return -EFAULT;
		}

		for (i=0; i<desc->bufLen; i++) {
		    tout = jiffies + (200 * HZ) / 1000;
		    do {
			XS_READ_32(gxsHandle->fpgaAdrs + 0x410/4, status);

			status &= PCI_CONSOLE_WRITE_STATUS_MASK;
			
			if(jiffies > tout) {
			    kfree(desc);
			    return -EWOULDBLOCK;
			}
		    } while(status);
		    
		    XS_WRITE_32(gxsHandle->fpgaAdrs + 0x468/4, (int)(desc->buf[i]));
		}

		kfree(desc);
	    }
	    break;

	case XS_IOCTL_CONSOLE_READ:
	    {
		XS_CONSOLE_BUF_DESC *desc;
		int                 i;
		unsigned long       flags;
		
		i = 0;
		
		desc = kmalloc(sizeof(XS_CONSOLE_BUF_DESC), GFP_KERNEL);		

		spin_lock_irqsave(&gxsHandle->xsConsole_lock, flags);

		while(gxsHandle->consoleWrPtr != gxsHandle->consoleRdPtr) {
		    if(gxsHandle->consoleBuf[gxsHandle->consoleRdPtr] == (char)-1) {
			i = -1;
		    } else if(gxsHandle->consoleBuf[gxsHandle->consoleRdPtr] == (char)-2) {
			i = -2;
		    } else {
			desc->buf[i++] = gxsHandle->consoleBuf[gxsHandle->consoleRdPtr];
		    }
		    
		    gxsHandle->consoleRdPtr = (gxsHandle->consoleRdPtr + 1) % PCI_CONSOLE_BUF_SIZE;
		    
		    if((i == PCI_CONSOLE_SIZE) || (i < 0)) {
			break;
		    }
		}
		
		spin_unlock_irqrestore(&gxsHandle->xsConsole_lock, flags);
		
		desc->bufLen = i;

		if (copy_to_user((void *) arg, desc, sizeof(XS_CONSOLE_BUF_DESC))) {
		    kfree(desc);
		    return -EFAULT;   
		}

		kfree(desc);
	    }
	    break;

	case XS_IOCTL_THERMO_READ:
	    {
		XS_THERMO_DESC desc;
		unsigned int i, value, data, tout;

		if (copy_from_user(&desc, (void *)arg, sizeof(desc)))
		    return -EFAULT;

		/* CLK and DATA initialized to low */
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0);
		xs_wait(100000);

		/* Start */
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x4 | 0x2);
		xs_wait(100000);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x4 | 0x3);
		xs_wait(100000);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x4 | 0x1);
		xs_wait(100000);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x4 | 0x0);
		xs_wait(100000);

		/* Device address - Write*/
		data = 0x90;
		for(i=0; i<8; i++) {
		    XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, (((data >> (7-i)) & 1) << 1) | 0 | 4);
		    xs_wait(100000);
		    XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, (((data >> (7-i)) & 1) << 1) | 1 | 4);
		    xs_wait(100000);
		    XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, (((data >> (7-i)) & 1) << 1) | 0 | 4);
		    xs_wait(100000);
		}
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0);

		/* Wait acknowledge */
		tout = 1000;
		do {
		    XS_READ_32(gxsHandle->fpgaAdrs + 0x1028/4, data);
		    xs_wait(100000);
		    if(tout-- == 0) { printk("mmapdrv_ioctl:XS_IOCTL_THERMO_READ No ACK 1!!!\n"); return 0;}
		} while(data & 0x40000);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x1);
		xs_wait(100000);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x0);
		xs_wait(100000);
  
		/* Put address */
		data = 0x00;
		for(i=0; i<8; i++) {
		    XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, (((data >> (7-i)) & 1) << 1) | 0 | 4);
		    xs_wait(100000);
		    XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, (((data >> (7-i)) & 1) << 1) | 1 | 4);
		    xs_wait(100000);
		    XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, (((data >> (7-i)) & 1) << 1) | 0 | 4);
		    xs_wait(100000);
		}
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0);

		/* Wait acknowledge */
		tout = 1000;
		do {
		    XS_READ_32(gxsHandle->fpgaAdrs + 0x1028/4, data);
		    xs_wait(100000);
		    if(tout-- == 0) { printk("mmapdrv_ioctl:XS_IOCTL_THERMO_READ No ACK 2!!!\n"); return 0;}
		} while(data & 0x40000);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x1);
		xs_wait(100000);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x0);
		xs_wait(100000);

		/* Start */
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x2 | 4);
		xs_wait(100000);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x3 | 4);
		xs_wait(100000);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x1 | 4);
		xs_wait(100000);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x0 | 4);
		xs_wait(100000);

		/* Device address - Read*/
		data = 0x91;
		for(i=0; i<8; i++) {
		    XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, (((data >> (7-i)) & 1) << 1) | 0 | 4);
		    xs_wait(100000);
		    XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, (((data >> (7-i)) & 1) << 1) | 1 | 4);
		    xs_wait(100000);
		    XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, (((data >> (7-i)) & 1) << 1) | 0 | 4);
		    xs_wait(100000);
		}
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0);

		/* Wait acknowledge */
		tout = 1000;
		do {
		    XS_READ_32(gxsHandle->fpgaAdrs + 0x1028/4, data);
		    xs_wait(100000);
		    if(tout-- == 0) { printk("mmapdrv_ioctl:XS_IOCTL_THERMO_READ No ACK 3!!!\n"); return 0;}
		} while(data & 0x40000);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x1);
		xs_wait(100000);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x0);
		xs_wait(100000);

		/* Get MSB */
		value = 0;
		for(i=0; i<8; i++) {
		    XS_READ_32(gxsHandle->fpgaAdrs + 0x1028/4, data);
		    value |= ((data >> 18) & 1) << (15 - i);
		    xs_wait(100000);
		    XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x1);
		    xs_wait(100000);
		    XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x0);
		    xs_wait(100000);
		}

		/* Ack Master */
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x0 | 4);
		xs_wait(100000);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x1 | 4);
		xs_wait(100000);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x0 | 4);
		xs_wait(100000);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0);

		/* Get LSB */
		for(i=0; i<8; i++) {
		    XS_READ_32(gxsHandle->fpgaAdrs + 0x1028/4, data);
		    value |= ((data >> 18) & 1) << (7 - i);
		    xs_wait(100000);
		    XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x1);
		    xs_wait(100000);
		    XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x0);
		    xs_wait(100000);
		}

		/* No ack */
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x1);
		xs_wait(100000);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x0);
		xs_wait(100000);

		/* Stop */
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x0);
		xs_wait(100000);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x1);
		xs_wait(100000);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x3 | 4);
		xs_wait(100000);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1028/4, 0x2 | 4);
		xs_wait(100000);

		desc.value = value;

		if (copy_to_user((void *) arg, &desc, sizeof(desc)))
		    return -EFAULT;   
	    }
	    break;

	case XS_IOCTL_RESET :
	    {
		struct pci_dev *pdev;
		unsigned int pciReg[16];

		// Make sure we are the only one using the driver
		if(usage_count != 1) {
		    return -EBUSY;
		}

		pdev     = gxsHandle->pdev;
		
		disable_irq(gxsHandle->pdev->irq);
		    
		// Save PCI CSR
		pci_read_config_dword(pdev, 0x04, &pciReg[0]);
		pci_read_config_dword(pdev, 0x0C, &pciReg[1]);
		pci_read_config_dword(pdev, 0x10, &pciReg[2]);
		pci_read_config_dword(pdev, 0x14, &pciReg[3]);
		pci_read_config_dword(pdev, 0x18, &pciReg[4]);
		pci_read_config_dword(pdev, 0x1C, &pciReg[5]);
		pci_read_config_dword(pdev, 0x20, &pciReg[6]);
		pci_read_config_dword(pdev, 0x24, &pciReg[7]);
		pci_read_config_dword(pdev, 0x3C, &pciReg[8]);

#if 1
		set_current_state(TASK_UNINTERRUPTIBLE);
#endif
		// Reset
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1020/4, 0x10000);
				
		// Wait
#if 1
		msleep(5000);
#else
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(5*HZ); 
#endif		
		// Restore PCI registers
		pci_write_config_dword(pdev, 0x04, pciReg[0]);
		pci_write_config_dword(pdev, 0x0C, pciReg[1]);
		pci_write_config_dword(pdev, 0x10, pciReg[2]);
		pci_write_config_dword(pdev, 0x14, pciReg[3]);
		pci_write_config_dword(pdev, 0x18, pciReg[4]);
		pci_write_config_dword(pdev, 0x1C, pciReg[5]);
		pci_write_config_dword(pdev, 0x20, pciReg[6]);
		pci_write_config_dword(pdev, 0x24, pciReg[7]);
		pci_write_config_dword(pdev, 0x3C, pciReg[8]);
		
		mmap_probe(pdev, NULL);

		usage_count =1;

#if 1
		set_current_state(TASK_RUNNING);
#endif
	    }
	    break;

	case XS_IOCTL_FW_VERSION :
	    {
		unsigned int data;

		XS_READ_32(gxsHandle->fpgaAdrs + 0x1000/4, data);

		if (copy_to_user((void *) arg, &data, sizeof(unsigned int)))
		    return -EFAULT;   
	    }
	    break;

	case XS_IOCTL_I2C_PROG :
	    {
		XS_FILE_DESC  desc;
		unsigned char *wbuf, *rbuf;
		int           i, ret;
		unsigned int  data, pages;

		// Make sure we are the only one using the driver
		if(usage_count != 1) {
		    return -EBUSY;
		}

		if (copy_from_user(&desc, (void *)arg, sizeof(desc)))
		    return -EFAULT;

		// WinPath3 controller only supports up to 5 bytes (63) length including 
		// address. So we use half pages...
		
		pages = desc.len/32;
		if(desc.len % 32) {
		    pages++;
		}

		wbuf = kmalloc(pages*32, GFP_KERNEL);
		if(wbuf == NULL) {
		    return -ENOMEM;
		}

		rbuf = kmalloc(pages*32, GFP_KERNEL);
		if(rbuf == NULL) {
		    kfree(wbuf);
		    return -ENOMEM;
		}

		if (copy_from_user(wbuf, (void *)desc.buf, desc.len)) {
		    kfree(wbuf);
		    kfree(rbuf);
		    return -EFAULT;
		}

		// Unlock
		XS_READ_32(gxsHandle->fpgaAdrs + 0x1020/4, data);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1020/4, data | 0x2);

		// Init I2C controller
		STORE(gxsHandle->wpathAdrs, 0x1a,  MAP_I2C_CFG);
		STORE(gxsHandle->wpathAdrs, 2000, MAP_I2C_CLKDIV);  // 100 KHz
		STORE(gxsHandle->wpathAdrs, 0x50,  MAP_I2C_DEV_ADDR);

		// Write
		printk("Writing WinPath3 I2C:\n");

		for(i=0; i<pages; i++) {
		    ret = i2c_write_page(gxsHandle, i, &wbuf[32*i]);
		    if(ret != 0) {
			kfree(wbuf);
			kfree(rbuf);
			return -EIO;
		    }

		    if(pages > 10) {
			if(!(i%(pages/10))) {
			    printk("I2C Upgrade: %3d%% written\n", (100*i)/pages);
			    set_current_state(TASK_INTERRUPTIBLE); // to allow printk
			    schedule_timeout(100);
			} 
		    }
		}
		printk("I2C Upgrade: 100%% written\n\n");
		
		// Lock again
		XS_READ_32(gxsHandle->fpgaAdrs + 0x1020/4, data);
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x1020/4, data & ~0x2);

		printk("Reading WinPath3 I2C:\n");
		// Read back
		for(i=0; i<pages; i++) {
		    ret = i2c_read_seq(gxsHandle, i, &rbuf[32*i]);
		    if(ret != 0) {
			kfree(wbuf);
			kfree(rbuf);
			return -EIO;
		    }
		    if(pages > 10) {
			if(!(i%(pages/10))) {
			    printk("I2C Upgrade: %3d%% checked\n", (100*i)/pages);
			    set_current_state(TASK_INTERRUPTIBLE);
			    schedule_timeout(100);
			} 
		    }
		}

		for(i=0; i<desc.len; i++) {
		    if(rbuf[i] != wbuf[i]) {
			printk("### Error at %05x: written %02x read back %02x\n", 
			       i, wbuf[i], rbuf[i]);
			kfree(wbuf);
			kfree(rbuf);
			return -EIO;
		    }
		}
		printk("I2C Upgrade: 100%% checked\n");

		kfree(wbuf);
		kfree(rbuf);
	    }
	    break;

	case XS_IOCTL_I2C_READ :
	    {
		XS_FILE_DESC  desc;
		unsigned char *buf;
		int           i, ret;
		unsigned int  pages;

		// Make sure we are the only one using the driver
		if(usage_count != 1) {
		    return -EBUSY;
		}

		if (copy_from_user(&desc, (void *)arg, sizeof(desc)))
		    return -EFAULT;

		pages = desc.len/32;
		if(desc.len % 32) {
		    pages++;
		}

		buf = kmalloc(pages*32, GFP_KERNEL);
		if(buf == NULL) {
		    return -ENOMEM;
		}

   		STORE(gxsHandle->wpathAdrs, 0x1a,  MAP_I2C_CFG);
		STORE(gxsHandle->wpathAdrs, 2000, MAP_I2C_CLKDIV); // 100 KHz
		STORE(gxsHandle->wpathAdrs, 0x50,  MAP_I2C_DEV_ADDR);

		for(i=0; i<pages; i++) {
		    ret = i2c_read_seq(gxsHandle, i, &buf[32*i]);
		    if(ret != 0) {
			kfree(buf);
			return -EIO;
		    }
		}

		if (copy_to_user((void *) desc.buf, 
				 buf,
				 desc.len)) {
		    kfree(buf);
		    return -EFAULT;   
		}

		kfree(buf);
	    }
	    break;

	case XS_IOCTL_FLASH_PROG :
	    {
		XS_FILE_DESC  desc;
		unsigned char str[64];
		unsigned int  i, status;
		unsigned long tout;

		// Make sure we are the only one using the driver
		if(usage_count != 1) {
		    printk("XS_IOCTL_FLASH_PROG: Exit all other driver clients\n");
		    return -EBUSY;
		}

		if (copy_from_user(&desc, (void *)arg, sizeof(desc)))
		    return -EFAULT;

		// Copy buffer to WinPath memory
		if(copy_from_user(gxsHandle->paramAdrs + 0x1000000/4, 
				  desc.buf, 
				  desc.len) != 0) {
		    return -EFAULT;
		}
		

		// Send command to WinMon to flash WinMon
		sprintf(str, "\nxs_flash write 0 0x21000000 0x80000\n");
		for(i=0; i<strlen(str); i++) {
		    tout = jiffies + (200 * HZ) / 1000;
		    do {
			XS_READ_32(gxsHandle->fpgaAdrs + 0x410/4, status);
			
			status &= PCI_CONSOLE_WRITE_STATUS_MASK;
			
			if(jiffies > tout) {
			    return -EWOULDBLOCK;
			}
		    } while(status);
		    
		    XS_WRITE_32(gxsHandle->fpgaAdrs + 0x468/4, (int)str[i]);
		}

		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(HZ*3);

		// Send command to WinMon to read Flash
		sprintf(str, "\nxs_flash read 0 0x22000000 0x80000\n");
		for(i=0; i<strlen(str); i++) {
		    tout = jiffies + (200 * HZ) / 1000;
		    do {
			XS_READ_32(gxsHandle->fpgaAdrs + 0x410/4, status);
			
			status &= PCI_CONSOLE_WRITE_STATUS_MASK;
			
			if(jiffies > tout) {
			    return -EWOULDBLOCK;
			}
		    } while(status);
		    
		    XS_WRITE_32(gxsHandle->fpgaAdrs + 0x468/4, (int)str[i]);
		}
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(HZ*2);
		
		if (copy_to_user((void *) desc.buf, 
				 gxsHandle->paramAdrs + 0x2000000/4, 
				 desc.len)) {
		    return -EFAULT;   
		}
	    }
	    break;

	case XS_IOCTL_ENV_GET :
	case XS_IOCTL_ENV_GET32 :
	    {
		XS_FILE_DESC  desc;
		unsigned char str[64], *buf;
		unsigned int  i, status, cr = 0;
		unsigned long tout, flags;

		buf = kmalloc(2048, GFP_KERNEL);

		if (copy_from_user(&desc, (void *)arg, sizeof(desc))) {
		    kfree(buf);
		    return -EFAULT;
		}

		// Empty console
		XS_WRITE_32(gxsHandle->fpgaAdrs + 0x468/4, '\n');    
		i=0;
		while(gxsHandle->consoleWrPtr != gxsHandle->consoleRdPtr) {
		    gxsHandle->consoleRdPtr = (gxsHandle->consoleRdPtr + 1) % PCI_CONSOLE_BUF_SIZE;
		    if((i == PCI_CONSOLE_SIZE) || (i < 0)) {
			break;
		    }
		}

		// Send command to WinMon to get env
		sprintf(str, "env\n");
		for(i=0; i<strlen(str); i++) {
		    tout = jiffies + (200 * HZ) / 1000;
		    do {
			XS_READ_32(gxsHandle->fpgaAdrs + 0x410/4, status);
			
			status &= PCI_CONSOLE_WRITE_STATUS_MASK;
			
			if(jiffies > tout) {
			    kfree(buf);
			    return -EWOULDBLOCK;
			}
		    } while(status);
		    
		    XS_WRITE_32(gxsHandle->fpgaAdrs + 0x468/4, (int)str[i]);
		}

		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(HZ*1);

		spin_lock_irqsave(&gxsHandle->xsConsole_lock, flags);
		
		i=0;

		while(gxsHandle->consoleWrPtr != gxsHandle->consoleRdPtr) {
		    if(gxsHandle->consoleBuf[gxsHandle->consoleRdPtr] == (char)-1) {
			i = -1;
		    } else if(gxsHandle->consoleBuf[gxsHandle->consoleRdPtr] == (char)-2) {
			i = -2;
		    } else {
			if(cr == 2) {
			    buf[i++] = gxsHandle->consoleBuf[gxsHandle->consoleRdPtr];
			} else {
			    if(gxsHandle->consoleBuf[gxsHandle->consoleRdPtr] == '\n') {
				cr++;
			    }
			}
		    }
		    
		    gxsHandle->consoleRdPtr = (gxsHandle->consoleRdPtr + 1) % PCI_CONSOLE_BUF_SIZE;
		    
		    if((i == PCI_CONSOLE_SIZE) || (i < 0)) {
			break;
		    }
		}

		// Cut the trailing prompt
		if(i>=7)
		    buf[i-7] = 0;

		spin_unlock_irqrestore(&gxsHandle->xsConsole_lock, flags);
		
		if(cmd == XS_IOCTL_ENV_GET32) {
		    if (copy_to_user(compat_ptr((unsigned long)desc.buf), buf, 2048)) {
			kfree(buf);
			return -EFAULT;   
		    }
		} else {
		    if (copy_to_user(desc.buf, buf, 2048)) {
			kfree(buf);
			return -EFAULT;   
		    }
		}

		kfree(buf);
	    }
	    break;
	case XS_IOCTL_REV_GET :
	    {
		XS_REV_DESC desc;

		GET(gxsHandle->wpathAdrs, desc.Fuse0, MAP_FUSE_REGITER0);
		GET(gxsHandle->wpathAdrs, desc.Fuse1, MAP_FUSE_REGITER1);

		if (copy_to_user((void *) arg, &desc, sizeof(desc)))
		    return -EFAULT;   
	    }
	    break;

	case XS_IOCTL_READ_WORD :
	    {
		unsigned int ptr, data;

		if (copy_from_user(&ptr, (void *)arg, sizeof(unsigned int)))
		    return -EFAULT;

		//printk("XS_IOCTL_READ_WORD: address is %x\n", ptr);

		GET(gxsHandle->hostAdrs, data, ptr);

		//printk("XS_IOCTL_READ_WORD: data is %x\n", data);
		
		if (copy_to_user((void *)arg, &data, sizeof(unsigned int)))
		    return -EFAULT;   
	    }
	    break;

	default:
	    return -EINVAL;
	}
    
    return 0;
}

/* Suppress the signal only if explicitly told so AND a previously spawned signal is running */
#define sig_suppress(sig)       (mmapdrv_dev_cfg.suppress_dup_signals \
                                        && sigismember(&mmapdrv_dev->pending.signal, (sig)))
static irqreturn_t ser_qs_handler(int irq, void * dev_id)
{
    siginfo_t info;

    if(mmap_log_level > 0)
	printk("----------------> ser_qs_handler %x\n", irq);

    if (mmapdrv_dev && !sig_suppress(SIGNAL_RT_SER))
	{
	    if(mmapdrv_dev->flags & THREAD_DEATH_STATUS)
		return IRQ_HANDLED;

	    info.si_signo = SIGNAL_RT_SER;
	    info.si_pid   = mmapdrv_dev->pid;
	    info.si_int   = irq;
	    info.si_code  = SI_QUEUE;
	    info.si_errno = 0;

	    if(irq & 0x00800000) {
		sigsent_nbr[IV_WMMIQ3_VEC]++;
	    }
	    if(irq & 0x00400000) {
		sigsent_nbr[IV_WMMIQ2_VEC]++;
	    }
	    if(irq & 0x00200000) {
		sigsent_nbr[IV_WMMIQ1_VEC]++;
	    }
	    if(irq & 0x00100000) {
		sigsent_nbr[IV_WMMIQ0_VEC]++;
	    }

	    send_sig_info(SIGNAL_RT_SER, &info , mmapdrv_dev);
	}

    return IRQ_HANDLED;
}

static irqreturn_t ser_qo_handler(int irq, void * dev_id)
{
    siginfo_t info;
    unsigned int ml, mh;

    if(mmap_log_level > 0) {
	GET(gxsHandle->wpathAdrs, ml, MAP_IC_MASK_EXTERNAL_L);
	GET(gxsHandle->wpathAdrs, mh, MAP_IC_MASK_EXTERNAL_H);

	printk("----------------> ser_qo_handler ml=%08x/mh=%08x ... ", ml, mh);
    }

    if (mmapdrv_dev && !sig_suppress(SIGNAL_RT_OV)) {
	if(mmapdrv_dev->flags & THREAD_DEATH_STATUS) {
	    if(mmap_log_level > 0) {
		printk("death\n");
	    }
	    return IRQ_HANDLED;
	}

	info.si_signo = SIGNAL_RT_OV;
	info.si_pid   = mmapdrv_dev->pid;
	info.si_int   = irq;
	info.si_code  = SI_QUEUE;
	info.si_errno = 0;
	
	if(mmap_log_level > 0)
	    printk("sent irq %x\n", irq);
	
	if(irq & 0x00020000) {
	    sigsent_nbr[IV_WMMIQO3_VEC]++;
	}
	if(irq & 0x00010000) {
	    sigsent_nbr[IV_WMMIQO2_VEC]++;
	}
	if(irq & 0x00008000) {
	    sigsent_nbr[IV_WMMIQO1_VEC]++;
	}
	if(irq & 0x00004000) {
	    sigsent_nbr[IV_WMMIQO0_VEC]++;
	}

	send_sig_info(SIGNAL_RT_OV, &info , mmapdrv_dev);
    } else {
	if(mmap_log_level > 0)
	    printk("not sent\n");
    }
    
    return IRQ_HANDLED;
}

static irqreturn_t sys_err_handler(int irq, void *dev_id)
{
    siginfo_t info;

    //if(mmap_log_level > 0)
    printk("----------------> sys_err_handler %x, disabling all irqs\n", irq);

    // disable these irqs
    STORE(gxsHandle->wpathAdrs, 0x00000000, MAP_IC_MASK_EXTERNAL_L);
    STORE(gxsHandle->wpathAdrs, 0x00000000, MAP_IC_MASK_EXTERNAL_H);

    report_errors(irq);

    if (mmapdrv_dev && (mmapdrv_dev_cfg.send_buserr_sig == 1))
	{      
	    if(mmapdrv_dev->flags & THREAD_DEATH_STATUS)
		return IRQ_HANDLED;

	    info.si_signo = SIGNAL_RT_BUSERR;
	    info.si_pid   = mmapdrv_dev->pid;
	    info.si_int   = irq;
	    info.si_code  = SI_QUEUE;
	    info.si_errno = 0;

	    if(irq & 0x00000020) {
		sigsent_nbr[IV_BERR4X_VEC]++;
	    }
	    if(irq & 0x00000010) {
		sigsent_nbr[IV_BERR4D_VEC]++;
	    }
	    if(irq & 0x00000008) {
		sigsent_nbr[IV_BERR3I_VEC]++;
	    }
	    if(irq & 0x00000004) {
		sigsent_nbr[IV_BERR3_VEC]++;
	    }
	    if(irq & 0x00000002) {
		sigsent_nbr[IV_BERR2_VEC]++;
	    }
	    if(irq & 0x00000001) {
		sigsent_nbr[IV_BERR1_VEC]++;
	    }

	    send_sig_info(SIGNAL_RT_BUSERR, &info , mmapdrv_dev);      
	}
   
    return IRQ_HANDLED;
}

static irqreturn_t int3_handler(int irq, void *dev_id)
{
    siginfo_t info;
    unsigned int ml, mh;
    
    if(mmap_log_level > 0) {
	GET(gxsHandle->wpathAdrs, ml, MAP_IC_MASK_EXTERNAL_L);
	GET(gxsHandle->wpathAdrs, mh, MAP_IC_MASK_EXTERNAL_H);
	
	printk("----------------> int3_handler ml=%08x/mh=%08x ... ", ml, mh);
    }
    
    if (mmapdrv_dev && !sig_suppress(SIGNAL_RT_UFE4_FRAMER)) {
	if(mmapdrv_dev->flags & THREAD_DEATH_STATUS) {
	    if(mmap_log_level > 0) {
		printk("death\n");
	    }
	    return IRQ_HANDLED;
	}
	
	info.si_signo = SIGNAL_RT_UFE4_FRAMER;
	info.si_pid   = mmapdrv_dev->pid;
	info.si_int   = 0;
	info.si_code  = SI_QUEUE;
	info.si_errno = 0;
	
	if(mmap_log_level > 0)
	    printk("sent\n");
	
	sigsent_nbr[IV_EINT3_VEC]++;

	send_sig_info(SIGNAL_RT_UFE4_FRAMER, &info , mmapdrv_dev);
    } else {
	if(mmap_log_level > 0)
	    printk("not sent\n");
    }

    return IRQ_HANDLED;
}

static irqreturn_t sgmii_an_handler(int irq, void *dev_id)
{
    siginfo_t info;

    if(mmap_log_level > 0)
	printk("--------------------> sgmii_an_handler\n");

    if (mmapdrv_dev && !sig_suppress(SIGNAL_RT_SGMII_AN))
	{
	    if(mmapdrv_dev->flags & THREAD_DEATH_STATUS)
		return IRQ_HANDLED;

	    info.si_signo = SIGNAL_RT_SGMII_AN;
	    info.si_pid   = mmapdrv_dev->pid;
	    info.si_int   = 0;
	    info.si_code  = SI_QUEUE;
	    info.si_errno = 0;

	    //printk("SGMII signal sent\n");

	    sigsent_nbr[IV_SERDES2_VEC]++;

	    send_sig_info(SIGNAL_RT_SGMII_AN, &info , mmapdrv_dev);
	}   
    return IRQ_HANDLED;
}

static irqreturn_t serdes_handler(int irq, void *dev_id)
{
    siginfo_t info;

    if(mmap_log_level > 0)
	printk("--------------------> serdes_handler\n");

    if (mmapdrv_dev && !sig_suppress(SIGNAL_RT_SERDES))
	{
	    if(mmapdrv_dev->flags & THREAD_DEATH_STATUS)
		return IRQ_HANDLED;

	    info.si_signo = SIGNAL_RT_SERDES;
	    info.si_pid   = mmapdrv_dev->pid;
	    info.si_int   = 0;
	    info.si_code  = SI_QUEUE;
	    info.si_errno = 0;

	    //printk("SerDes signal sent\n");

	    sigsent_nbr[IV_SERDES1_VEC]++;

	    send_sig_info(SIGNAL_RT_SERDES, &info , mmapdrv_dev);
	}   
    return IRQ_HANDLED;
}

/*****************************************************************************
 * Function:    BusDumpDDR
 * Description:
 * Inputs:
 * Outputs:
 * Return Value:
 *****************************************************************************/
static void BusDumpDDR(unsigned int status, unsigned int addr, unsigned int extra)
{
    unsigned int master = (status>>23)&0x1f;
    unsigned int is_write = 0;

    printk("Status:0x%08x Addr:0x%08x\n",status,addr);
    if (status & 0xf0000000)
	{
	    printk("               *** Bus Error: ");

	    printk("%d-byte ",status&0x7ff);
	    if (status & 0x00400000) //bit 22 R/W
		printk("read ");
	    else
		{
		    printk("write ");
		    is_write = 1;
		}
	    printk("operation generated by ");
	    if (master == 1)
		{
		    unsigned int trsno = (status&0x003f0000)>>16; //bits 16-21- TRS
		    printk("WMM TRS: %d pc:0x",trsno);
		    if (is_write)
			printk("%04x ",extra & 0xffff);
		    else
			{
			    wm_ta *ta = (wm_ta *)(WINPATH_TA_BASE + RIF_BASE);
			    printk("%04x ",xsByteSwap(ta->ta_pc[trsno]));
			    printk("data: r%d ",(extra & 0x1fe00)>>9);
			    if (extra & 0x180)/*bits 7 and 8 specify a byte offset if any*/
				{
				    printk("%d bytes offset ", (extra & 0x180)>>7);
				}
			    printk("address: r%d", extra & 0x7f);/*bits 0-6 specify the address*/
			}
		}
	    else if (master == 2)
		printk("CORE 0");
	    else if (master == 3)
		printk("CORE 1");
	    else if (master == 4)
		printk("PCIe");
	    else if (master == 5)
		printk("SRIO 1");
	    else if (master == 6)
		printk("SRIO 2");
	    else if (master == 7)
		printk("FMU");
	    else
		printk("UNKNOWN MASTER\n");
	}
}

/*****************************************************************************
 * Function:    BusDumpInt
 * Description:
 * Inputs:
 * Outputs:
 * Return Value:
 *****************************************************************************/
static void BusDumpInt(unsigned int status, unsigned int addr, unsigned int extra)
{
    unsigned int master = (status>>23)&0x1f;
    unsigned int is_write = 0;

    printk("Status:0x%08x Addr:0x%08x\n",status,addr);
    if (status & 0xf0000000)
	{
	    printk("               *** Bus Error: ");

	    printk("%d-byte ",status&0x7ff);
	    if (status & 0x00400000) //bit 22 R/W
		printk("read ");
	    else
		{
		    printk("write ");
		    is_write = 1;
		}
	    printk("operation generated by ");
	    if (master == 1 || master == 2)
		{
		    unsigned int trsno = (status&0x003f0000)>>16; //bits 16-21- TRS
		    if(master == 1)
			printk("WMM 3 ");
		    else
			printk("WMM 5 ");
		    printk("TRS: %d pc:0x",trsno);
		    if (is_write)
			printk("%04x ",extra & 0xffff);
		    else
			{
			    wm_ta *ta = (wm_ta *)(WINPATH_TA_BASE + RIF_BASE);
			    printk("%04x ", xsByteSwap(ta->ta_pc[trsno]));
			    printk("data: r%d ",(extra & 0x1fe00)>>9);
			    if (extra & 0x180)/*bits 7 and 8 specify a byte offset if any*/
				{
				    printk("%d bytes offset ", (extra & 0x180)>>7);
				}
			    printk("address: r%d", extra & 0x7f);/*bits 0-6 specify the address*/
			}
		}
	    else if (master == 3)
		printk("CORE 0");
	    else if (master == 4)
		printk("CORE 1");
	    else if (master == 7)
		printk("PCIe");
	    else if (master == 5)
		printk("SRIO 1");
	    else if (master == 6)
		printk("SRIO 2");
	    else if (master == 8)
		printk("FMU");
	    else
		printk("UNKNOWN MASTER\n");
	}
}
/*****************************************************************************
 * Function:    BusDumpAsram
 * Description:
 * Inputs:
 * Outputs:
 * Return Value:
 *****************************************************************************/
static void BusDumpAsram(unsigned int status, unsigned int addr, unsigned int extra)
{
    unsigned int master = (status>>23)&0x7;
    unsigned int is_write = 0;

    printk("Status:0x%08x Addr:0x%08x\n",status,addr);
    if (status & 0xf0000000)
	{
	    printk("               *** Bus Error: ");

	    printk("%d-byte ",status&0x7ff);
	    if (status & 0x00400000) //bit 22 R/W
		printk("read ");
	    else
		{
		    printk("write ");
		    is_write = 1;
		}
	    printk("operation generated by ");
	    if (master == 1)
		{
		    unsigned int trsno = (status&0x003f0000)>>16; //bits 16-21- TRS
		    printk("WMM TRS: %d pc:0x",trsno);
		    if (is_write)
			printk("%04x ",extra & 0xffff);
		    else
			{
			    wm_ta *ta = (wm_ta *)(WINPATH_TA_BASE + RIF_BASE);
			    printk("%04x ", xsByteSwap(ta->ta_pc[trsno]));
			    printk("data: r%d ",(extra & 0x1fe00)>>9);
			    if (extra & 0x180)/*bits 7 and 8 specify a byte offset if any*/
				{
				    printk("%d bytes offset ", (extra & 0x180)>>7);
				}
			    printk("address: r%d", extra & 0x7f);/*bits 0-6 specify the address*/
			}
		}
	    else if (master == 2)
		printk("CORE 0");
	    else if (master == 3)
		printk("CORE 1");
	    else if (master == 4)
		printk("PCIe");
	    else if (master == 5)
		printk("SRIO 1");
	    else if (master == 6)
		printk("SRIO 2");
	    else if (master == 7)
		printk("FMU");
	    else
		printk("UNKNOWN MASTER\n");
	}
}

static void report_errors(unsigned long irq)
{
    if (mmapdrv_dev_cfg.kernel_buserr_report == 1)
	{
      
            if (irq & WINPATH_SYS_BUSERR_MASK)
		printk("\n\n**** ERROR **** BUS MONITOR TRIGGER\n\n");

            if (irq & (1<<IV_BERR1_VEC))
		{
		    printk("Parameter bus: ");
		    BusDumpDDR(WDS3_WINPATH_RIF_READ(WINPATH_PARAM_BIU_TESR),
			       WDS3_WINPATH_RIF_READ(WINPATH_PARAM_BIU_TEAR),
			       WDS3_WINPATH_RIF_READ(WINPATH_PARAM_BIU_HBMS1));
		}

            if (irq & (1<<IV_BERR2_VEC))
		{
		    printk("Packet bus: ");
		    BusDumpDDR(WDS3_WINPATH_RIF_READ(WINPATH_PACKET_BIU_TESR),
			       WDS3_WINPATH_RIF_READ(WINPATH_PACKET_BIU_TEAR),
			       WDS3_WINPATH_RIF_READ(WINPATH_PACKET_BIU_HBMS1));
		}

            if (irq & (1<<IV_BERR3_VEC))
		{
		    printk("Internal bus: ");
		    BusDumpInt(WDS3_WINPATH_RIF_READ(WINPATH_INTERNAL_BIU_TESR),
			       WDS3_WINPATH_RIF_READ(WINPATH_INTERNAL_BIU_TEAR),
			       WDS3_WINPATH_RIF_READ(WINPATH_INTERNAL_BIU_HBMS1));
		}

            if (irq & (1<<IV_BERR3I_VEC))
		{
		    printk("RIF: ");
		    BusDumpAsram(WDS3_WINPATH_RIF_READ(WINPATH_RIF_BIU_TESR),
				 WDS3_WINPATH_RIF_READ(WINPATH_RIF_BIU_TEAR),
				 WDS3_WINPATH_RIF_READ(WINPATH_RIF_BIU_HBMS1));
		}

            if (irq & (1<<IV_BERR4D_VEC))
		{
		    printk("Host bus: ");
		    BusDumpDDR(WDS3_WINPATH_RIF_READ(WINPATH_APP_BIU_TESR),
			       WDS3_WINPATH_RIF_READ(WINPATH_APP_BIU_TEAR),
			       WDS3_WINPATH_RIF_READ(WINPATH_APP_BIU_HBMS1));
		}

            if (irq & (1<<IV_BERR4X_VEC))
		{
		    printk("Host bus: ");
		    BusDumpAsram(WDS3_WINPATH_RIF_READ(WINPATH_HOST_BIU_TESR),
				 WDS3_WINPATH_RIF_READ(WINPATH_HOST_BIU_TEAR),
				 WDS3_WINPATH_RIF_READ(WINPATH_HOST_BIU_HBMS1));
		}

	}   
}

// Load the module
static int __init init_mmapdrv(void)
{
    int ret = 0;

    proc_created = 0;

    printk(KERN_INFO "%s - version %s\n",
	   mmap_driver_string, mmap_driver_version);
    
    printk(KERN_INFO "%s\n", mmap_copyright);
    
    if ((major = register_chrdev(0, "mmap_drv", &mmapdrv_fops))<0) {
	printk (KERN_ERR "xsDrv: can't get major %d\n", major);
	return (major);
    }
    
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
    ret = pci_module_init(&mmap_pci_driver);
#else
    ret = pci_register_driver(&mmap_pci_driver);
#endif            
    if(ret != 0) {
	printk("Module load failed, unregistering character device\n");
	unregister_chrdev(major, "mmap_drv");
	return ret;
    }

    RIF_BASE = (unsigned char *)gxsHandle->wpathAdrs;

    return(0);
}

// Remove the module 
static void __exit exit_mmapdrv(void)
{
    // Unregister the device
    pci_unregister_driver(&mmap_pci_driver);
    unregister_chrdev(major, "mmap_drv");

    return;
}

module_init(init_mmapdrv);
module_exit(exit_mmapdrv);

#define I2C_STATUS_TIMEOUT 0x10000

static int status_poll(XS_HANDLE *xsHandle, unsigned int expected_value, unsigned int index, unsigned int time_out) 
{
    unsigned int status;
    unsigned int masked_status, ackerr;
    unsigned int cnt = 0;

    do {
	GET(xsHandle->wpathAdrs, status, MAP_I2C_STATUS);
	ackerr = status & 0x8;
	masked_status = status & expected_value;
	cnt++;
    } while ((masked_status != expected_value) && (cnt != time_out));
    
    if(ackerr && ((status & 1) == 0)) {
	printk("!!! I2C ACK ERROR, Slave Not Responding !!! read %x, expected %x, index %x \n",
	       status, expected_value, index);
	
	// make sure we recover
	printk("Recovering ACK error\n");
	do {
	    STORE(xsHandle->wpathAdrs, 0, MAP_I2C_DATA_OUT);
	    GET(xsHandle->wpathAdrs, status, MAP_I2C_STATUS);
	} while ((status & 1) == 0);

	printk(" done\n");
	return 1;
    }

    if(cnt == time_out){
        printk("!!! I2C STATUS TIMEOUT !!! read %x, expected %x, index %x \n", 
		status, expected_value, index);

	return 1;
    }
    
    return 0;
}

static int i2c_write_page(XS_HANDLE *xsHandle, unsigned int page, unsigned char *buf)
{
    int i, rc;
    unsigned int addr_msb, addr_lsb;

    addr_msb = (page >> 3) & 0xff;
    addr_lsb = (page << 5) & 0xff;

    //printk("Writing to %02x %02x\n", addr_msb, addr_lsb);

    // Check I2C is idle
    rc = status_poll(xsHandle, 0x01, 0x0, I2C_STATUS_TIMEOUT);
    if(rc) {
	return -1;
    }

    set_current_state(TASK_UNINTERRUPTIBLE);

    STORE(xsHandle->wpathAdrs, 2+32-1, MAP_I2C_BYTCNT);
    STORE(xsHandle->wpathAdrs, addr_msb, MAP_I2C_DATA_OUT);
    STORE(xsHandle->wpathAdrs, 0x00, MAP_I2C_SDEN);
    rc = status_poll(xsHandle, 0x02, 0x1, I2C_STATUS_TIMEOUT);
    if(rc) { set_current_state(TASK_RUNNING); return -1; }
    STORE(xsHandle->wpathAdrs, addr_lsb, MAP_I2C_DATA_OUT);
    rc = status_poll(xsHandle, 0x02, 0x2, I2C_STATUS_TIMEOUT);
    if(rc) { set_current_state(TASK_RUNNING); return -1; }

    for(i=0; i<32; i++) {
	STORE(xsHandle->wpathAdrs, buf[i], MAP_I2C_DATA_OUT);
	rc = status_poll(xsHandle, 0x02, 0x3+i, I2C_STATUS_TIMEOUT);
	if(rc) { set_current_state(TASK_RUNNING); return -1; }
    }

    set_current_state(TASK_RUNNING);

    rc = status_poll(xsHandle, 0x01, 0xffffffff, I2C_STATUS_TIMEOUT);
    if(rc) return -1;

    msleep(5);

    return 0;
}

static int i2c_read_seq(XS_HANDLE *xsHandle, unsigned int page, unsigned char *buf)
{
    int i, rc;
    unsigned int addr_msb, addr_lsb;

    addr_msb = (page >> 3) & 0xff;
    addr_lsb = (page << 5) & 0xff;

    //printk("Reading from %02x %02x\n", addr_msb, addr_lsb);

    // Check I2C is idle
    rc = status_poll(xsHandle, 0x01, 0x0, I2C_STATUS_TIMEOUT);
    if(rc) {
	return -1;
    }

    set_current_state(TASK_UNINTERRUPTIBLE);

    STORE(xsHandle->wpathAdrs, 0x01, MAP_I2C_BYTCNT);
    STORE(xsHandle->wpathAdrs, addr_msb, MAP_I2C_DATA_OUT);
    STORE(xsHandle->wpathAdrs, 0x00, MAP_I2C_SDEN);
    rc = status_poll(xsHandle, 0x02, 0x1, I2C_STATUS_TIMEOUT);
    if(rc) { set_current_state(TASK_RUNNING); return -1; }
    STORE(xsHandle->wpathAdrs, addr_lsb, MAP_I2C_DATA_OUT);
    rc = status_poll(xsHandle, 0x03, 0x2, I2C_STATUS_TIMEOUT);
    if(rc) { set_current_state(TASK_RUNNING); return -1; }

    STORE(xsHandle->wpathAdrs, 32-1, MAP_I2C_BYTCNT);
    STORE(xsHandle->wpathAdrs, 0x01, MAP_I2C_SDEN);
    for(i=0; i<32; i++) {
	rc = status_poll(xsHandle, 0x04, 0x3+i, I2C_STATUS_TIMEOUT);
	if(rc) { set_current_state(TASK_RUNNING); return -1; }
	GET(xsHandle->wpathAdrs, buf[i], MAP_I2C_DATA_IN);
    }
    
    set_current_state(TASK_RUNNING);

    rc = status_poll(xsHandle, 0x01, 0xffffffff, I2C_STATUS_TIMEOUT);
    if(rc) return -1;

    return 0;
}
