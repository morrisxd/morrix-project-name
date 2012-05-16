/**********************************************************************
 *
 * Filename:    memtest.h
 * 
 * Description: Memory-testing module API.
 *
 * Notes:       The memory tests can be easily ported to systems with
 *              different data bus widths by redefining 'datum' type.
 *
 * 
 * Copyright (c) 2000 by Michael Barr.  This software is placed into
 * the public domain and may be used for any purpose.  However, this
 * notice must not be changed or removed and no warranty is either
 * expressed or implied by its publication or distribution.
 **********************************************************************/

#ifndef _memtest_h
#define _memtest_h

#include "memmap.h"

/*
 * Define NULL pointer value.
 */
#ifndef NULL
#define NULL  (void *) 0
#endif

/*
 * Set the data bus width.
 */
typedef unsigned long datum;


#define SRC_TO_DEST_OFFSET 0x100
#ifdef WDS_SL
#define DMA_DESCRIPTORS_OFFSET_WP3 0x10000
#define TEST_RESULT_OFFSET_WP3 0x15000
#else
#define DMA_DESCRIPTORS_OFFSET_WP3 0x100000
#define TEST_RESULT_OFFSET_WP3 0x150000
#endif

#ifndef _STAND_ALONE_

#include "winpath.h"
#include "sysio.h"
#include "common.h"

#else
unsigned long GET_REG_BASE(void);

#ifdef  MAP_CHCIIMT
#undef  MAP_CHCIIMT
#endif 
#ifndef MAP_CHCIIMT
#define MAP_CHCIIMT GET_REG_BASE()
#endif /*MAP_CHCIIMT*/

#define SIU_BUS_PARAM    0
#define SIU_BUS_PACKET   1
#define SIU_BUS_INTERNAL 2
#define SIU_BUS_HOST_APP 3

#define PROC_TEST 1
#define STRESS_TEST 0

/*------------------------------sysio.h-begin-------------------------------*/
#define CTRL_C         ('C'-0x40)
#define CTRL_P          ('P'-0x40)
#define FALSE          0
#define UART_ERROR     -1
/*------------------------------sysio.h-end---------------------------------*/



/*------------------------------winpath.h-------------------------------------*/
/*****************************************************************************
 * (C) Copyright 2000-2010, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 * Updated for WP3 Tsahi 30-SEP-09
 ****************************************************************************/
#define WM_REG_SET(reg,value) *(volatile unsigned int *)(reg) = (value)
#define WM_REG_GET(value,reg) (value) = *(volatile unsigned int *)(reg)

#define FUNCTION_DISABLE_REG (*((volatile unsigned int*)MAP_GI_DEVFN))
//Wingines on Winfarm #0
#define WINFARM_0_IS_ACTIVE	1 //todo - read function disable to see if this is active
#define WINFARM_1_IS_ACTIVE	1 //todo - read function disable to see if this is active
#define WINFARM_2_IS_ACTIVE	1 //todo - read function disable to see if this is active
#define WINFARM_3_IS_ACTIVE	1 //todo - read function disable to see if this is active

#define LOAD_WF1 1
#define LOAD_WF2 2
#define LOAD_WF3 4
#define LOAD_WF4 8
#define LOAD_ALL_WF (LOAD_WF1|LOAD_WF2|LOAD_WF3|LOAD_WF4)


/************************************************************************
 *  UART status bits 
 ************************************************************************/
#define WM_UART_STATUS_RXFE          0x80
#define WM_UART_STATUS_TEMT          0x40
#define WM_UART_STATUS_THRE          0x20
#define WM_UART_STATUS_BI            0x10
#define WM_UART_STATUS_FE            0x08
#define WM_UART_STATUS_PE            0x04
#define WM_UART_STATUS_OE            0x02
#define WM_UART_STATUS_DR            0x01

/* ------------------------ AA ------------------------ */
#define WM_AA_BASE  ((MAP_AA_OFFSET) - (MAP_CHCIIMT))
#define WM_AA(base) ((wm_aa *) ((base) + WM_AA_BASE))
#define WM_AA_STL_SIZE 100
#define WM_AA_PCT_SIZE 100
#define WM_AA_STC_SIZE 100
#define WM_AA_CNT_SIZE 100

#define WM_AA_STL_RES  ((0x0400-(WM_AA_STL_SIZE*4))/4)
#define WM_AA_PCT_RES  ((0x0400-(WM_AA_PCT_SIZE*2))/4)
#define WM_AA_STC_RES  ((0x0400-(WM_AA_STC_SIZE*4))/4)
#define WM_AA_CNT_RES  ((0x0100-(WM_AA_CNT_SIZE))/4)
typedef struct wm_aa
{
      unsigned int   stl[WM_AA_STL_SIZE - 2];
      unsigned int   reserved_1[WM_AA_STL_RES+2];
      unsigned short pct[WM_AA_PCT_SIZE];
      unsigned int   reserved_2[WM_AA_PCT_RES];
      unsigned int   stc[WM_AA_STL_SIZE - 2];
      unsigned int   reserved_3[WM_AA_STC_RES+2];
      unsigned char  trs_count[WM_AA_CNT_SIZE];
      unsigned int   reserved_4[WM_AA_CNT_RES];
      unsigned int   gmr;
      unsigned int   wfcnts;
} wm_aa;

/* ------------------------ TA ----------------------- */
#define WM_TA_BASE ((MAP_TA_OFFSET) - (MAP_CHCIIMT))
#define WM_TA(base) ((volatile wm_ta *) ((base) + WM_TA_BASE))

#define WM_TA_BASE_REGS    512
#define WM_TA_ORDER_REGS   128
#define WM_TA_DEV_MAP_REGS 32
#define WM_TA_DEV_MAP_RES  (((0x1b00-0x1a00)-(WM_TA_DEV_MAP_REGS*4))/4)
#define WM_TA_TRS_SETS     64
#define WM_TA_PC_RES       (((0x1f80-0x1d00)-(WM_TA_TRS_SETS*4))/4)


typedef struct wm_ta {
      unsigned int          b[WM_TA_BASE_REGS];
      volatile unsigned int order[WM_TA_ORDER_REGS];
      unsigned int          dev_map[WM_TA_DEV_MAP_REGS];
      unsigned int          res1[WM_TA_DEV_MAP_RES];
      volatile unsigned int trs_stat[WM_TA_TRS_SETS];
      volatile unsigned int ta_cam[WM_TA_TRS_SETS];
      volatile unsigned int ta_pc[WM_TA_TRS_SETS];
      unsigned int          res2[WM_TA_PC_RES];
      unsigned int          ts_prescalar;
      volatile unsigned int ts_timer[2];
} wm_ta;

/* ------------------------ TGU ----------------------- */
#define NUM_OF_TGU 16
#define WM_TGU_DELTA  MAP_TGU0_SIZE
#define WM_TGU_BASE   (MAP_TGU0_OFFSET-MAP_CHCIIMT)
#define WM_TGU_SECTION_OFFSET(t) (WM_TGU_BASE + WM_TGU_DELTA * (t))
#define WM_TGU(base, t) ((volatile wm_tgu *) ((base) + WM_TGU_SECTION_OFFSET(t)))

#define WM_TGU_STL_BASE (MAP_AA_STL+0x180)
#define WM_TGU_STC_BASE (MAP_AA_STC+0xc0)
#define WM_TGU_STL_ADDR(t) ((volatile unsigned long*)(WM_TGU_STL_BASE+(8*t)))
#define WM_TGU_STC_ADDR(t) ((volatile unsigned long*)(WM_TGU_STC_BASE+(4*t)))


typedef struct {
      unsigned int tcr2;
      unsigned int tcr1;
      unsigned int tmr;
      unsigned int cnt;
      unsigned int twr;
      unsigned int tcr3;
      unsigned int reserved[2];
} wm_tgu;

/* ------------------------ WMM ----------------------- */
#define WM_WMM_DELTA (MAP_WMM0_SIZE)
#define WM_WMM_BASE  ((MAP_WMM0_OFFSET)-(MAP_CHCIIMT))
#define WM_WMM_SECTION_OFFSET(busno) (WM_WMM_BASE + WM_WMM_DELTA * (busno))
#define WM_WMM(base, busno) ((wm_wmm *) ((base) + WM_WMM_SECTION_OFFSET(busno)))

#define WM_NUM_OF_WM_FIFOS 48

typedef struct wm_wmm {
      unsigned int fifo_start[WM_NUM_OF_WM_FIFOS];
      unsigned int pad_0[16];
      unsigned int fifo_end[WM_NUM_OF_WM_FIFOS];
      unsigned int pad_1[16];
      unsigned int fifo_counter[WM_NUM_OF_WM_FIFOS];
      unsigned int pad_2[16];
      unsigned int fifo_max[WM_NUM_OF_WM_FIFOS];
      unsigned int pad_3[16];
      unsigned int iq_start[4];
      unsigned int iq_temp[4];
      unsigned int iq_end[4];
      unsigned int lar;
      unsigned int utar;
      unsigned int pad_7[2];
      unsigned int fer[2];
      unsigned int ffr[2];
      unsigned int hcr_reg;
      unsigned int hcr_addr;
      unsigned int hcr_cmd;
} wm_wmm;


/* --todo update for wp3 according to devfn relevant definitions for WinPath_Wingines variable */
#define WP_WG0  0x01
#define WP_WG1  0x02
#define WP_WG2  0x04
#define WP_WG3  0x08
#define WP_WG4  0x10
#define WP_WG5  0x20
#define WP_WG6  0x40
#define WP_WG7  0x80
#define WP_WG8  0x100
#define WP_WG9  0x200
#define WP_WG10 0x400
#define WP_WG11 0x800

/* mask for 2nd winfarm - relevant to WinPath_Wingines variable */
#define WP_2ND_WINFARM_MASK (WP_WG3 | WP_WG4  | WP_WG5)
#define WP_3RD_WINFARM_MASK (WP_WG6 | WP_WG7  | WP_WG8)
#define WP_4TH_WINFARM_MASK (WP_WG9 | WP_WG10 | WP_WG11)
/* 
 * DPS packages 
 */
#define UNDEFINED_DPS	    0
#define DEFAULT_DPS	    1
#define USER_DEFINED_DPS    2
#define SIU_MEMORY_TEST_DPS 3
#define KEY_CAM_CLEAR_DPS   5

/*****************************************************************************

 *****************************************************************************/
void    WinPath_DpsSpawnInit(void);
void    WinPath_DpsSpawnTerminate(void);
unsigned int  WinPath_DpsTaskSpawn(unsigned int Param1);
void WinPath_LoadDps(unsigned int DpsId, unsigned int *dps_image, unsigned int wfs );
void WinPath_InitPCT(unsigned short *pct, unsigned int wfs);
/*------------------------------------------winpath.h-end-----------------------*/


int SysIO_CheckForCtrlC(void);
#endif /*stand alone*/


/*
 * Function prototypes.
 */
datum   memTestDataBus(volatile datum * address);
datum * memTestAddressBus(volatile datum * baseAddress, unsigned long nBytes);
datum * memTestDevice(volatile datum * baseAddress, unsigned long nBytes);

void TestMemory(volatile datum * baseAddress, unsigned long nBytes, char *msg);
void siuMemoryTest(datum Phys_srcAddress,
	  	   datum Virt_srcAddress,
		   datum Phys_intSpace, 
		   datum Virt_intSpace,
	           int siuBus,
	           int nReps,
		   int nWinFarms,
		   unsigned long memSize,
	           char *msg);

void Mem_Test(int test_select, 
	     datum Phys_srcAddress, 
	     datum Virt_srcAddress, 
	     datum Phys_intSpace, 
	     datum Virt_intSpace, 
	     datum Virt_intReg, 
	     int siuBus, 
	     int nReps, 
	     int nWinFarms, 
	     unsigned long memSize, 
	     char *msg);

#endif /* _memtest_h */
