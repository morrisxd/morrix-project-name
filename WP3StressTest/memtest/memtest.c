/**********************************************************************
 *
 * Filename:    memtest.c
 * 
 * Description: General-purpose memory testing functions.
 *
 * Notes:       This software can be easily ported to systems with
 *              different data bus widths by redefining 'datum'.
 *
 * 
 * Copyright (c) 1998 by Michael Barr.  This software is placed into
 * the public domain and may be used for any purpose.  However, this
 * notice must not be changed or removed and no warranty is either
 * expressed or implied by its publication or distribution.
 **********************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "generic.h"
#include "memtest.h"
#define HANDE_SHAKE_PATTERN 0xCAFECAFE

/***********************************************************************
 ***********************************************************************/
#define DMA_CMD_COMPARE     0x80
#define DMA_CMD_CountEn     0x40
#define DMA_CMD_CRC_Source  0x20
#define DMA_CMD_Atomic      0x10
#define DMA_CMD_SrcBusType  0x0c
#define DMA_CMD_DestBusType 0x03

#define PRINT_WIDTH         8
#define DISPLAY_UPDATE_DELAY 0x100000


static int RandomCmdSize[] = {  0x08, 0x40, 0x07, 0x41, 0x20, 0x19, 
			 	0x40, 0x08, 0x50, 0x23, 0x08, 0x19,
				0x23, 0x18, 0x45, 0x14, 0x13, 0x39,
				0x10, 0x06, 0x46, 0x33, 0x28, 0x29,
				0x26, 0x27, 0x63, 0x23, 0x72, 0x15,
				0x26, 0x53, 0x08, 0x40, 0x07, 0x41, 
				0x20, 0x19, 0x40, 0x08, 0x50, 0x23, 
				0x08, 0x19, 0x23, 0x18, 0x45, 0x14, 
				0x13, 0x39, 0x10, 0x06, 0x46, 0x33, 
				0x28, 0x29, 0x26, 0x27, 0x63, 0x23, 
				0x72, 0x15, 0x26, 0x53 };



struct S_DmaConf
{
      datum 	     NextPointer   ;
      datum          DestAddr      ;
      datum          SrcAddr       ;
      unsigned short TotalTranSize ;
      unsigned char  DmaCmdSize    ;  
      unsigned char  DmaCmdControl ;  
      unsigned int   RepeatCount   ;
      unsigned int   DejaCount     ;
      unsigned int   TRSMask       ; 
      datum          ErrAddr       ;     
      datum          HandShakeAddr ;
};

struct S_TestResult
{
      unsigned int ErrorCode;
      unsigned int TRSMask;
      datum        SrcAddr;
      datum        DestAddr;
};

#ifndef _STAND_ALONE_

#include "winpath.h"
#include "sysio.h"
#include "common.h"

extern unsigned int active_winfarms;
extern unsigned int siu_ProgramImage;
extern unsigned short siu_PC_Table_Init;

#else

#include "siu_image.c"
/* Global variable to hold the active Wingines */
unsigned char WinPath_Wingines;
/* the derivative string returned to shell_info */
unsigned char bond_id[40];
/* number of winfarms active*/
unsigned int active_winfarms = 0;


/*Location of internal registers */
volatile datum IntRegBaseAddr = 0;

/*Accessor function for the internal registers address*/
unsigned long GET_REG_BASE(){ return IntRegBaseAddr;}


/*----------------------sysio.c-begin--------------------------------*/
int SysIO_CheckForCtrlC()
{
  unsigned int status;
  status = *(volatile unsigned int *)MAP_UART_LSR;   /* Read UART line status register */
  if (status & (WM_UART_STATUS_RXFE | WM_UART_STATUS_OE))
     return UART_ERROR;

  if (status & WM_UART_STATUS_DR)
     return ((*(volatile unsigned int *)MAP_UART_RBR)==CTRL_C);
  
  return FALSE;
}

/*----------------------sysio.c-end--------------------------------*/

/*----------------------winpath.c-begin--------------------------------*/
void WinPath_DpsSpawnInit()
{
   /* set the accurate threads (TRS) per Winfarm */ 
   volatile wm_tgu *tgu = WM_TGU(MAP_CHCIIMT,0);
   volatile wm_aa  *aa  = WM_AA(MAP_CHCIIMT);
   unsigned int i;
   /*
   * Initialize TGUS
   */

   active_winfarms = 0;
#ifdef VERBOSE_MODE
   printf("Initializing TGUs... \n");
#endif
   for (i=0; i< NUM_OF_TGU; i++)
   {
      //  printf("Initializing STLs... ");
      volatile unsigned long* tmp_stl = WM_TGU_STL_ADDR(i);
#ifdef VERBOSE_MODE
      printf("Initializing STLs... stl%d at %x \n", i, tmp_stl );
#endif
      *tmp_stl = 0;
      *(tmp_stl+1) = 0;

      tgu[i].tcr1 = 0;
      tgu[i].tcr2 = 0;
      tgu[i].tmr = i; 
#ifdef VERBOSE_MODE
      printf("tgu[%d] | TCR1 at %x, TCR2 at %x TMR at %x\n", i, &(tgu[i].tcr1), &(tgu[i].tcr2), &(tgu[i].tmr) );
#endif
   }

   /*
   * Use polling address 0 for TGU0
   */

   for (i=0; i< NUM_OF_TGU; i++)
   {
      volatile unsigned long* tgu_stl = WM_TGU_STL_ADDR(i);
      volatile unsigned long* tgu_stc = WM_TGU_STC_ADDR(i);

      if(WINFARM_0_IS_ACTIVE)
      {
         //printf("tgu_%d_stl_H = %x\n", i, &(*(tgu_stl+1)) );
         *(tgu_stl+1) = 0x0000ffff;//Only the lower 16 TRSs are available at Winfarm 0;
         *tgu_stc = 0xf;
         active_winfarms = 1;
         //printf("tgu_%d_stc   = %x val = %x\n", i, tgu_stc, *tgu_stc );
      }   

      if(WINFARM_1_IS_ACTIVE)
      {
         //printf("tgu_%d_stl_H = %x\n", i, &(*(tgu_stl+1)) );
         *(tgu_stl+1) |= 0xffff0000;//Only the lower 16 TRSs are available at Winfarm 0;
         *tgu_stc += 0x10;         //Only 16 TRSs can be run by the TGU (0..15)
         active_winfarms += 1;
         //printf("tgu_%d_stc   = %x val = %x\n", i, tgu_stc, *tgu_stc );
      }

      if(WINFARM_2_IS_ACTIVE)
      {
         //printf("tgu_%d_stl_L = %x\n", i, &(*tgu_stl) );
         *tgu_stl = 0x0000ffff;//Only the lower 16 TRSs are available at Winfarm 0;
         *tgu_stc += 0x10;         //Only 16 TRSs can be run by the TGU (0..15)
         active_winfarms += 1;
         //printf("tgu_%d_stc   = %x val = %x\n", i, tgu_stc, *tgu_stc );

      }
      if(WINFARM_3_IS_ACTIVE)
      {
         //printf("tgu_%d_stl_L = %x\n", i, &(*tgu_stl) );
         *tgu_stl |= 0xffff0000;//Only the lower 16 TRSs are available at Winfarm 0;
         *tgu_stc += 0x10;      //Only 16 TRSs can be run by the TGU (0..15)
         active_winfarms += 1;
         //printf("tgu_%d_stc   = %x val = %x\n", i, tgu_stc, *tgu_stc );
      }
   }
#ifdef VERBOSE_MODE
   printf("Enable GMR...\n");
   printf( "AA_GMR at %x\n", &(aa->gmr) );
#endif
   aa->gmr = 0xF3000000;
}

void WinPath_DpsSpawnTerminate()
{
   volatile wm_aa  *aa = WM_AA(MAP_CHCIIMT);
   volatile wm_tgu *tgu = WM_TGU(MAP_CHCIIMT,0);
   unsigned int i;
#ifdef VERBOSE_MODE
   printf("Disabling TGUs... ");
#endif
   for (i=0; i<NUM_OF_TGU; i++)
   {
      //  printf("Initializing STLs... ");
      volatile unsigned long* tmp_stl = WM_TGU_STL_ADDR(i);
      tgu[i].tcr1 = 0;
      //	tgu[i].tcr2 = 0;
      tgu[i].tmr = 0x3f; 

      *tmp_stl = 0;
      *(tmp_stl+1) = 0;

   }
#ifdef VERBOSE_MODE
   printf("Disabling GMR...\n");
#endif
   aa->gmr = 0;
}

unsigned int WinPath_DpsTaskSpawn(unsigned int Param1)
{
  volatile wm_tgu *tgu = WM_TGU(MAP_CHCIIMT,0);
  //volatile wm_ta  *ta = WM_TA(MAP_CHCIIMT);
  unsigned int TimeOut = 0x5000;
  tgu->tmr = 0x80000000; 
  while ((tgu->tcr1 & 0x80000000) && TimeOut>0)  
			   /* wait until previous command completes */ 
    TimeOut--;
  if (TimeOut==0)
  {
    printf("DPS task spawn failed\n");   
    return 1;
  }

  tgu->tcr2 = Param1;
  tgu->tcr1 = 0x80000000;
  return 0;
}

void WinPath_LoadDps(unsigned int DpsId, unsigned int *dps_image, unsigned int wfs )
{
   unsigned int DpsIndex = 0;

   unsigned int SegmentProgramLength = dps_image[DpsIndex+1];
   unsigned int SegmentIndex;
   static unsigned int CurrentDPS = UNDEFINED_DPS;
#ifdef VERBOSE_MODE
   printf("Loading DPS...\n");
#endif
   if (DpsId != CurrentDPS || DpsId == USER_DEFINED_DPS)
   {
      while (SegmentProgramLength)
      {
         /* Set start address */
         if (wfs & LOAD_WF1 )
            WM_REG_SET ( MAP_WF0_PRADD, dps_image[DpsIndex]);
         if (wfs & LOAD_WF2 )
            WM_REG_SET (MAP_WF1_PRADD, dps_image[DpsIndex] );
         if (wfs & LOAD_WF3 )
            WM_REG_SET ( MAP_WF2_PRADD, dps_image[DpsIndex]);
         if (wfs & LOAD_WF4 )
            WM_REG_SET (MAP_WF3_PRADD, dps_image[DpsIndex]);

         DpsIndex+=2;     /* Skip to data start */
         for (SegmentIndex = 0; SegmentIndex < SegmentProgramLength; SegmentIndex++)
         {
            if (wfs & LOAD_WF1 )
               WM_REG_SET (MAP_WF0_PRDATA, dps_image[DpsIndex]);
            if (wfs & LOAD_WF2 )
               WM_REG_SET ( MAP_WF1_PRDATA, dps_image[DpsIndex] );
            if (wfs & LOAD_WF3 )
               WM_REG_SET ( MAP_WF2_PRDATA, dps_image[DpsIndex] );
            if (wfs & LOAD_WF4 )
               WM_REG_SET ( MAP_WF3_PRDATA, dps_image[DpsIndex]);
            DpsIndex++;
         } 
         SegmentProgramLength = dps_image[DpsIndex+1];       
      }
   }
}


/* 
 * Load PC table.
#define LOAD_WF1 1
#define LOAD_WF2 2
#define LOAD_WF3 4
#define LOAD_WF4 8
#define LOAD_ALL_WF (LOAD_WF1|LOAD_WF2|LOAD_WF3|LOAD_WF4)

 *
 */
void WinPath_InitPCT(unsigned short *pct, unsigned int wfs )
{
   unsigned int i = 0;
   wm_aa *aa = WM_AA(MAP_CHCIIMT);
   unsigned short *ptr = (unsigned short *)&(aa->pct);
#ifdef VERBOSE_MODE
   printf("Initializing PCT... \n");
#endif
   while( pct[i] != 0xffff )
   {
      *(ptr+i)   = pct[i];
      *(ptr+i+1) = pct[i];
      *(ptr+i+2) = pct[i];
      *(ptr+i+3) = pct[i];
      i+=1; /*changed from i+= 4, increments pointer past the array limit -> bug?*/
   }
   //*(ptr+i) = 0xffff;
}

#endif /*_STAND_ALONE_*/

/*----------------------winpath.c-end----------------------------------*/

/**********************************************************************
 *
 * Function:    memTestDataBus()
 *
 * Description: Test the data bus wiring in a memory region by
 *              performing a walking 1's test at a fixed address
 *              within that region.  The address (and hence the
 *              memory region) is selected by the caller.
 *
 * Notes:       
 *
 * Returns:     0 if the test succeeds.  
 *              A non-zero result is the first pattern that failed.
 *
 **********************************************************************/
datum memTestDataBus(volatile datum * address)
{
   datum pattern;

   /*
    * Perform a walking 1's test at the given address.
    */
   for (pattern = 1; pattern != 0; pattern <<= 1)
   {
      /*
       * Write the test pattern.
       */
      *address = pattern;

      /*
       * Read it back (immediately is okay for this test).
       */
      if (*address != pattern) 
      {
         //SysIO_Display(0xe0);
         return(pattern);
      }
   }

   return (0);

}   /* memTestDataBus() */

/**********************************************************************
 *
 * Function:    memTestAddressBus()
 *
 * Description: Test the address bus wiring in a memory region by
 *              performing a walking 1's test on the relevant bits
 *              of the address and checking for aliasing. This test
 *              will find single-bit address failures such as stuck
 *              -high, stuck-low, and shorted pins.  The base address
 *              and size of the region are selected by the caller.
 *
 * Notes:       For best results, the selected base address should
 *              have enough LSB 0's to guarantee single address bit
 *              changes.  For example, to test a 64-Kbyte region, 
 *              select a base address on a 64-Kbyte boundary.  Also, 
 *              select the region size as a power-of-two--if at all 
 *              possible.
 *
 * Returns:     NULL if the test succeeds.  
 *              A non-zero result is the first address at which an
 *              aliasing problem was uncovered.  By examining the
 *              contents of memory, it may be possible to gather
 *              additional information about the problem.
 *
 **********************************************************************/
datum * memTestAddressBus(volatile datum * baseAddress, unsigned long nBytes)
{
   unsigned long addressMask = (nBytes/sizeof(datum) - 1);
   unsigned long offset;
   unsigned long testOffset;

   datum pattern     = (datum) 0xAAAAAAAA;
   datum antipattern = (datum) 0x55555555;

   /*
    * Write the default pattern at each of the power-of-two offsets.
    */
   for (offset = 1; (offset & addressMask) != 0; offset <<= 1)
   {
      baseAddress[offset] = pattern;
   }

   /* 
    * Check for address bits stuck high.
    */
   testOffset = 0;
   baseAddress[testOffset] = antipattern;

   for (offset = 1; (offset & addressMask) != 0; offset <<= 1)
   {
      if (baseAddress[offset] != pattern)
      {
         //SysIO_Display(0xe1);
         return ((datum *) &baseAddress[offset]);
      }
   }

   baseAddress[testOffset] = pattern;

   /*
    * Check for address bits stuck low or shorted.
    */
   for (testOffset = 1; (testOffset & addressMask) != 0; testOffset <<= 1)
   {
      baseAddress[testOffset] = antipattern;

      if (baseAddress[0] != pattern)
      {
         //SysIO_Display(0xe2);
         return ((datum *) &baseAddress[testOffset]);
      }

      for (offset = 1; (offset & addressMask) != 0; offset <<= 1)
      {
         if ((baseAddress[offset] != pattern) && (offset != testOffset))
         {
            //SysIO_Display(0xe3);
            return ((datum *) &baseAddress[testOffset]);
         }
      }

      baseAddress[testOffset] = pattern;
   }

   return (NULL);

}   /* memTestAddressBus() */

/**********************************************************************
 *
 * Function:    memTestDevice()
 *
 * Description: Test the integrity of a physical memory device by
 *              performing an increment/decrement test over the
 *              entire region.  In the process every storage bit 
 *              in the device is tested as a zero and a one.  The
 *              base address and the size of the region are
 *              selected by the caller.
 *
 * Notes:       
 *
 * Returns:     NULL if the test succeeds.
 *
 *              A non-zero result is the first address at which an
 *              incorrect value was read back.  By examining the
 *              contents of memory, it may be possible to gather
 *              additional information about the problem.
 *
 **********************************************************************/
datum * memTestDevice(volatile datum * baseAddress, unsigned long nBytes)	
{
   unsigned long offset;
   unsigned long nWords = nBytes / sizeof(datum);
   char backspace[16];
   int i;

   datum pattern;
   datum antipattern;

   for (i=0; i<7; i++)
      backspace[i]=8;
   backspace[7]=0;
   /*
    * Fill memory with a known pattern.
    */
    
   for (pattern = 1, offset = 0; offset < nWords; pattern++, offset++)
   {
      baseAddress[offset] = pattern;
      if ((offset & 0xffff)==0)
         printf("%6dK%s",offset>>8,backspace);
      if (SysIO_CheckForCtrlC())
         return NULL;
   }


   /*
    * Check each location and invert it for the second pass.
    */
   for (pattern = 1, offset = 0; offset < nWords; pattern++, offset++)
   {
      if (baseAddress[offset] != pattern)
      {
         //SysIO_Display(0xe4);
         //return ((datum *) &baseAddress[offset]);
      }

      antipattern = ~pattern;
      baseAddress[offset] = antipattern;
      if ((offset & 0xffff)==0)
         printf("%6dK%s",offset>>8,backspace);
      if (SysIO_CheckForCtrlC())
         return NULL;
   }

   /*
    * Check each location for the inverted pattern and zero it.
    */
   for (pattern = 1, offset = 0; offset < nWords; pattern++, offset++)
   {
      antipattern = ~pattern;
      if (baseAddress[offset] != antipattern)
      {
         //SysIO_Display(0xe5);
         return ((datum *) &baseAddress[offset]);
      }

      if ((offset & 0xffff)==0)
         printf("%6dK%s",offset>>8,backspace);
      if (SysIO_CheckForCtrlC())
         return NULL;
   }

   return (NULL);

}   /* memTestDevice() */

void TestMemory(volatile datum * baseAddress, unsigned long nBytes, char *msg)
{
   datum *rc;
   printf("%s data bus test:\t ",msg);
   printf(memTestDataBus(baseAddress)?"FAILED\n" : "PASSED\n");
   printf("%s addr bus test:\t ",msg);
   rc = memTestAddressBus(baseAddress,nBytes);
   if (rc == NULL)
      printf("PASSED\n");
   else
      printf("FAILED in address 0x%08x\n",rc);

   printf("%s device test:\t ",msg);
   rc = memTestDevice(baseAddress,nBytes);
   if (rc == NULL)
      printf("PASSED   \n\n");
   else
      printf("FAILED in address 0x%08x\n\n",rc);
}

/************************************************************************
 *
 * Function:  siuMemoryTest 
 *
 * Purpose:   Stress test the SIU memory interface 
 *
 * Description: This function spawns 16*Winfarm# DPS threads that stress tests the
 * 		provided memory bus.
 * 		Each thread copies data from source address to destination
 * 		address and verifies corrent values.
 * 		This test destroys the origianl memory contents.
 *
 * Inputs:      Phys_srcAddress  Physical source base address
 *		Virt_srcAddress  Virtual source base address
 *		Phys_intSpace	 Physical internal base address
 *		Virt_intSpace	 Virtual internal base address
 *		siuBus		 Bus to test
 * 		nReps		 Test repetitions
 *		nWinFarms	 Winfarms to test
 *		memSize		 Memory size to test
 * 		msg		 Test identification message
 *
 * Outputs:		none.
 *
 * Return Value:
 *
 **************************************************************************/

#define COUNT_LOOP 0x500000

void siuMemoryTest(datum Phys_srcAddress, datum Virt_srcAddress, datum Phys_intSpace, datum Virt_intSpace, int siuBus, int nReps, int nWinFarms, unsigned long memSize, char *msg)
{
   struct S_DmaConf *virtConf; 
   struct S_DmaConf *physConf; 
   struct S_TestResult *res;
   volatile datum * baseAddress;
   int i,offset;
   int TestRunning;
   int NumLines;
   unsigned int loadwfs = 0;
   unsigned int no_of_threads;
   unsigned int counter;
   datum *uncached_HandShakeAddr;
   unsigned int thread_address_inc;
   unsigned long total_trans_size;
   unsigned long testRuns = 0;
#ifdef FULL_SCAN
   unsigned long maxRuns;
   int linecount;
#endif
   /*Error if max # of WF exceeded*/
   if(nWinFarms > active_winfarms)
   {	
	printf("ERROR: Number of WinFarms specified exceeds Max WF #\n");
	printf("Winfarms available: %d, Winfarms specified: %d \n",active_winfarms, nWinFarms);
	return ;
   }
   /*Special case of 0 to designate debugging case with Single WF with Single thread*/
   else if(nWinFarms == 0)
   	{
	  printf("Special case activation, activating a single thread\n");
	  no_of_threads = 1;
	  loadwfs = 1;
	}
	/*Normal case, load WF#1 through #N*/
   	else{
		no_of_threads = nWinFarms*16;
		for(i=0, loadwfs = 0; i < nWinFarms; i++)
	   	{
		   loadwfs = loadwfs + (1 << i);
		}
	    }

   thread_address_inc = ((2*memSize ) / (2*no_of_threads+ 1)) & 0xFFFFFFF8;
#ifdef VERBOSE_MODE
   printf("Thread_address_inc: 0x%08X\n",thread_address_inc);
#endif
   /* 
   * If first call, (siu test DPS not yet loaded), load DPS 
   */
   WinPath_LoadDps(SIU_MEMORY_TEST_DPS,&siu_ProgramImage[0],loadwfs);
   WinPath_InitPCT(&siu_PC_Table_Init[0],loadwfs);
   /* 
   * Initialize DMA descriptos (one per thread).
   */
   physConf = (struct S_DmaConf *)(Phys_intSpace + DMA_DESCRIPTORS_OFFSET_WP3);
   virtConf = (struct S_DmaConf *)(Virt_intSpace + DMA_DESCRIPTORS_OFFSET_WP3);
#ifdef FULL_SCAN
   maxRuns = (thread_address_inc/0x200);
   printf("Running Full System Memory Test: \n");
   printf("Total runs required: %d\n\n",maxRuns);
   for(testRuns = 0;testRuns < maxRuns;testRuns++)
 {
#endif
   for (i=0; i<no_of_threads; i++)
   {
      virtConf[i].NextPointer   = (datum)&(physConf[i]);
      virtConf[i].DestAddr      = Phys_srcAddress+ SRC_TO_DEST_OFFSET+(thread_address_inc*i)+(testRuns*0x200);
      virtConf[i].SrcAddr       = Phys_srcAddress+(thread_address_inc*i)+(testRuns*0x200);
      virtConf[i].TotalTranSize = i<2?0x40 : 256;
      virtConf[i].DmaCmdSize    = RandomCmdSize[i];
      if ((i%3) == 0) /* third of the tests should be atomic */
         virtConf[i].DmaCmdControl = DMA_CMD_Atomic | DMA_CMD_CountEn |
         (siuBus<<2)| siuBus;
      else
         virtConf[i].DmaCmdControl = DMA_CMD_COMPARE | DMA_CMD_CountEn |
         (siuBus<<2)| siuBus;
      virtConf[i].RepeatCount   = nReps;
      virtConf[i].DejaCount     = 0;
      virtConf[i].TRSMask       = (i<32?(1<<i): 1);

      virtConf[i].ErrAddr       = Phys_intSpace + TEST_RESULT_OFFSET_WP3; 
      virtConf[i].HandShakeAddr = (datum)&(physConf[i].HandShakeAddr);
   }
#ifdef VERBOSE_MODE
//testRuns always zero outside of FULL_SCAN mode
   printf("Configurations: \n");
   printf("Physical base source address: 0x%08X\n",virtConf[0].SrcAddr);
   printf("Virtual  base source address: 0x%08X\n", Virt_srcAddress+ SRC_TO_DEST_OFFSET+(testRuns*0x200));
   printf("Physical base dest address  : 0x%08X\n",virtConf[0].DestAddr);
   printf("Virtual  base dest address  : 0x%08X\n\n", Virt_srcAddress+(testRuns*0x200));
   printf("Physical end source address : 0x%08X\n",virtConf[i-1].SrcAddr);
   printf("Virtual  end source address : 0x%08X\n",Phys_srcAddress+ (thread_address_inc*(i-1))+(testRuns*0x200));
   printf("Physical end dest address   : 0x%08X\n",virtConf[i-1].DestAddr);
   printf("Virtual  end dest address   : 0x%08X\n\n",Phys_srcAddress+ SRC_TO_DEST_OFFSET+(thread_address_inc*(i-1))+(testRuns*0x200));
#endif
#ifdef FULL_SCAN
   printf("Completed Runs: %d\n",testRuns+1);
#endif
   /*
   * Zero result status and addresses 
   */
   res = (struct S_TestResult *)(Virt_intSpace + TEST_RESULT_OFFSET_WP3);
   res->ErrorCode=0;
   res->TRSMask=0;
   res->SrcAddr=0;
   res->DestAddr=0;
   /*
    * Clear destination addresses
    */
#ifdef VERBOSE_MODE
   printf("Clearing dest addrs..."); 
#endif
   for (i=0; i < no_of_threads; i++)
   {
      total_trans_size = (virtConf[i].TotalTranSize/sizeof(datum));
      
      baseAddress = (volatile datum *)(Virt_srcAddress + SRC_TO_DEST_OFFSET + (thread_address_inc*i));

      for( offset = 0; offset < total_trans_size; )
      {
	baseAddress[offset++] = 0;
      }
    }
#ifdef VERBOSE_MODE
   printf("Completed!..."); 
#endif
  /*
   * Initialize Source memory addresses.
   */
   

#ifdef VERBOSE_MODE
   printf("Initialising src addrs..."); 
#endif
   for (i=0; i < no_of_threads; i++)
   {
      total_trans_size = (virtConf[i].TotalTranSize/sizeof(datum));     
      baseAddress = (volatile datum *)(Virt_srcAddress + (thread_address_inc*i) + testRuns*0x200); 
     //testRuns always zero outside of FULL_SCAN mode
      for( offset = 0; offset < total_trans_size; )
      {

         switch( i%5 )
         {
         case 0: //init worst case 32 bit port size:
            if( offset < (total_trans_size/2) )
            {
               baseAddress[offset++] = 0x00000000;
               baseAddress[offset++] = 0xffffffff;
            }
            else
            {
               baseAddress[offset++] = 0xffffffff;
               baseAddress[offset++] = 0x00000000;
            }
            break;
         case 1: //init worst case 16 bit port size:
            if( offset < (total_trans_size/2) )
            {
               baseAddress[offset++] = 0x0000ffff;
               baseAddress[offset++] = 0x0000ffff;
            }
            else
            {
               baseAddress[offset++] = 0xffff0000;
               baseAddress[offset++] = 0xffff0000;
            }
            break;
         case 2: //init worst case  8 bit port size:
            if( offset < (total_trans_size/2) )
            {
               baseAddress[offset++] = 0x00ff00ff;
               baseAddress[offset++] = 0x00ff00ff;
            }
            else
            {
               baseAddress[offset++] = 0xff00ff00;
               baseAddress[offset++] = 0xff00ff00;
            }
            break;
         case 3: //init walking ones and walking zeros:
            {
               unsigned long pattern;
               int bit = ((offset/2) %32);
               pattern = (1 << bit);
               baseAddress[offset++] =   pattern;
               baseAddress[offset++] = ~(pattern);
            }
            break;
         case 4: //init random data:
         default:
            baseAddress[offset++] = ((0x9fa523d7 >> (offset%32)) | (0x95 << (offset%32)));
            baseAddress[offset++] = ((0x9fa523d7 >> (offset%32)) | (0x95 << (offset%32)));
            break;
         }
      }
   }
#ifdef VERBOSE_MODE
   printf("Completed!...\n"); 
#endif
   /*
   * Spawn threads 
   */
   printf("\n");
   for (i=0; i<no_of_threads; i++)
   {
      counter = 0;
      uncached_HandShakeAddr = (datum*)(Virt_intSpace - Phys_intSpace + virtConf[i].HandShakeAddr);
      WinPath_DpsTaskSpawn(virtConf[i].NextPointer);
      #ifdef VERBOSE_MODE
      printf("\033[1A\r");
      printf("DPS Task Spawn: Threads Spawned[%d]\n",i+1);
      #endif
      while((  ( * ((volatile datum*) uncached_HandShakeAddr)) != HANDE_SHAKE_PATTERN) && (counter < COUNT_LOOP))
         counter++;

      if(counter == COUNT_LOOP)
      {
         printf("\nERROR: Waited 0x%08x busy wait loops without getting ack from the Wingines for WinPath_DpsTaskSpawn\n",counter);
         return;
      }

   }


   /*****************************************************/
   /* the test is now running                           */
   /* let's check if the test passes                    */
   /*****************************************************/
   printf("\n%s stress test 0x%08x repetitions:\n",msg,nReps);
   TestRunning=1;

   while(TestRunning)

   {
      int tmp=0;

      TestRunning=0;
      for (i=0; i<no_of_threads; i++)
      {
         if ((i % PRINT_WIDTH) == 0)
            printf("[TRS %2d-%2d]: ",i,((i+PRINT_WIDTH)<no_of_threads)?(i+PRINT_WIDTH-1):(no_of_threads-1));
         if (virtConf[i].DejaCount<virtConf[i].RepeatCount)
         {
            TestRunning=1;
            if (res->TRSMask & (1<<i))
               printf("%s  FAILED  %s ",COLOR_FG_RED,COLOR_NORMAL);
            else
               printf("0x%08x ",virtConf[i].DejaCount);
         }
         else 
         {
            if (res->TRSMask&(1<<i))
               printf("%s  FAILED  %s ",COLOR_FG_RED,COLOR_NORMAL);
            else
               printf("%s  PASSED  %s ",COLOR_FG_GREEN,COLOR_NORMAL);
         }
         if ((i%PRINT_WIDTH)==(PRINT_WIDTH-1) && (i != no_of_threads-1))
            printf("\n");
      }
      NumLines = ((no_of_threads + PRINT_WIDTH - 1) / PRINT_WIDTH) - 1;
      /* Move cursor up */
      if (TestRunning)
      {
         for (i=0; i<NumLines; i++)
            printf("\033[1A\r");
      }
      else
         printf("\n");

      while  (tmp++<DISPLAY_UPDATE_DELAY)
         if (SysIO_CheckForCtrlC())
         {
            /* If cntl-c detected, set all counters to maximum value
            * to force termination 
            */
            for (i=0; i<no_of_threads; i++)
               virtConf[i].RepeatCount = virtConf[i].DejaCount;
#ifdef FULL_SCAN
	    testRuns = maxRuns;
#endif
         }
   }

   if (res->ErrorCode)
   {
      printf("%sTest Failed%s: %d errors found. First error at address 0x%08x\n\n",
         COLOR_FG_RED,COLOR_NORMAL,res->ErrorCode,res->SrcAddr);
      return ;
   }

   /*
   * Checks if the correct values have been copied through the stress test.
   */
   for (i=0; i<no_of_threads; i++)
   {
      unsigned long error_offset = 0xffffffff;
      /* no need to check the atomic threads */
      if ((i%3) == 0)
         continue;
   
      total_trans_size = (virtConf[i].TotalTranSize/sizeof(datum));
      baseAddress = (volatile datum *)(Virt_srcAddress + SRC_TO_DEST_OFFSET + (thread_address_inc*i)+ testRuns*0x200);
     //testRuns always zero outside of FULL_SCAN mode
      /*
      * Checks the value of the destivation address.
      */
      for( offset = 0; offset < total_trans_size; )
      {
         switch( i%5 )
         {

         case 0: //init worst case 32 bit port size:
            if( offset < (total_trans_size/2) )
            {
               if( baseAddress[offset++] != 0x00000000 )
                  error_offset = offset-1;
               if( baseAddress[offset++] != 0xffffffff )
                  error_offset = offset-1;
            }
            else
            {
               if( baseAddress[offset++] != 0xffffffff)
                  error_offset = offset-1;
               if( baseAddress[offset++] != 0x00000000 )
                  error_offset = offset-1;
            }
            break;
         case 1: //init worst case 16 bit port size:
            if( offset < (total_trans_size/2) )
            {
               if( baseAddress[offset++] != 0x0000ffff )
                  error_offset = offset-1;
               if( baseAddress[offset++] != 0x0000ffff )
                  error_offset = offset-1;
            }
            else
            {
               if( baseAddress[offset++] != 0xffff0000 )
                  error_offset = offset-1;
               if( baseAddress[offset++] != 0xffff0000 )
                  error_offset = offset-1;
            }
            break;
         case 2: //init worst case  8 bit port size:
            if( offset < (total_trans_size/2) )
            {
               if( baseAddress[offset++] != 0x00ff00ff )
                  error_offset = offset-1;
               if( baseAddress[offset++] != 0x00ff00ff )
                  error_offset = offset-1;
            }
            else
            {
               if( baseAddress[offset++] != 0xff00ff00 )
                  error_offset = offset-1;
               if( baseAddress[offset++] != 0xff00ff00 )
                  error_offset = offset-1;
            }
            break;
         case 3: //init worst case  8 bit port size:
            {
               unsigned long pattern;
               int bit = ((offset/2) %32);
               pattern = (1 << bit);
               if( baseAddress[offset++] !=   pattern )
                  error_offset = offset-1;
               if( baseAddress[offset++] != ~(pattern) )
                  error_offset = offset-1;
            }
            break;

         case 4: //init random data:
         default:
            if( baseAddress[offset++] != ((0x9fa523d7 >> (offset%32)) | (0x95 << (offset%32))) )
               error_offset = offset-1;
            if( baseAddress[offset++] != ((0x9fa523d7 >> (offset%32)) | (0x95 << (offset%32))) )
               error_offset = offset-1;
            break;
         }
         if( error_offset != 0xffffffff )
            break;
      }

      if( error_offset != 0xffffffff )
      {
         printf("%sTest Failed%s: value at address 0x%08x is not as expected\n\n",
            COLOR_FG_RED,COLOR_NORMAL,(datum)&baseAddress[error_offset]);
         return;
      }
   }
#ifdef FULL_SCAN
   //Clear the screen for next iteration of full system test
   printf("*****************************************************************\n");
   if(testRuns+1 < maxRuns)
   { 
#ifdef VERBOSE_MODE
     for(linecount = 0; linecount < (NumLines+ 18); linecount++)
#else
     for(linecount = 0; linecount < (NumLines + 6); linecount++)
#endif
       printf("\033[1A\r");
   }
 }
   printf("\n\n");
#endif
   return;
}

/************************************************************************
 *
 * Function:  Mem_Test 
 *
 * Purpose:   Runs siuMemoryTest or TestMemory based
 *
 * Description: 
 *
 * Inputs:    test_select: 1: processor based test, 0 = DPS based test
	      Phys_srcAddress: Physical base address of the memory region tested
	      Virt_srcAddress: Virtual base address of the memory region tested
	      Phys_intSpace: Physical base address of the internal space
	      Virt_intSpace: Virtual base address of the internal space
	      Virt_intReg: Virtual base address of the internal registers
	      siuBus: 0 = Parameter bus, 1 = Packet bus, 3 = Host bus
	      nReps: Number of Test Repetitions
	      nWinFarms: Number of WinFarms to run. Note: 0 = Run 1 thread with 1 repetition
	      memSize: Total size of the memory bank being tested
	      msg: debug message

	      Processor Based test only requires test_select=1, Virt_srcAddress, nReps, memSize and msg.
 *
 * Outputs: none
 *
 * Return Value: none
 *
 **************************************************************************/

void Mem_Test(int test_select, datum Phys_srcAddress, datum Virt_srcAddress, datum Phys_intSpace, datum Virt_intSpace, datum Virt_intReg, int siuBus, int nReps, int nWinFarms, unsigned long memSize, char *msg){	
	#ifdef _STAND_ALONE_	
	IntRegBaseAddr = Virt_intReg;
	#endif
	switch(test_select){
	case 0: WinPath_DpsSpawnInit();
		siuMemoryTest(Phys_srcAddress, Virt_srcAddress, Phys_intSpace, Virt_intSpace, siuBus, nReps, nWinFarms, memSize, msg);
		WinPath_DpsSpawnTerminate();
		break;
	case 1: TestMemory(Virt_srcAddress,memSize,msg);
		break; 
	default: printf("ERROR: invalid test selection");
		return;
	}
}
