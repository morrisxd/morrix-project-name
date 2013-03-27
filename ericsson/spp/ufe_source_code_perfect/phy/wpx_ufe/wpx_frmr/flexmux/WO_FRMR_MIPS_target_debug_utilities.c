/*--------------------------------------------------------------------------*/
/*                                                                          */
/*        Copyright (c) 2010  Omiino Ltd                                    */
/*                                                                          */
/*        All rights reserved.                                              */
/*        This code is provided under license and or Non-disclosure         */
/*        Agreement and must be used solely for the purpose for which it    */
/*        was provided. It must not be passed to any third party without    */
/*        the written permission of Omiino Ltd.                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#include <stdio.h>

#include "WO_FRMR_private.h"
#include "WO_FRMR_MIPS_private.h"



extern unsigned int WPL_Delay(unsigned int interval);


void WPX_UFE_FRAMER_MIPS_Delay(unsigned int AnyTimeInterval)
{
	WPL_Delay(AnyTimeInterval);
}



void WPX_UFE_FRAMER_MIPS_DisplayChipSelect_Serial_1(void)
{
	volatile U32 Register1Value;
	volatile U32 Register2Value;
	volatile unsigned short ShortRegisterValue;


	printf("[OMIINO] [->] WPX_UFE_FRAMER_MIPS_DisplayChipSelect_Serial_1\n");

#ifndef XALYO_FRAMER_ACCESS
	Register1Value     = *(volatile U32 *)0xBE010820;
	Register2Value     = *(volatile U32 *)0xBE010824;
	ShortRegisterValue = *(volatile U16 *)0xBE010890;
#else
        {
            volatile U32 *ptr;
            volatile U16 *sptr;

            ptr = (volatile U32 *)0xBE010820;
            WP_MEM_GET(Register1Value, ptr[0]);
            ptr = (volatile U32 *)0xBE010824;
            WP_MEM_GET(Register2Value, ptr[0]);
            sptr = (volatile U16 *)0xBE010890;
            WP_MEM_SHORT_GET(ShortRegisterValue, sptr[0]);
        }
#endif

	printf("[OMIINO] [  ] Chip Select for Serial1. Address=0xBE010820 Value=%08X\n", Register1Value    );
	printf("[OMIINO] [  ] Chip Select for Serial1. Address=0xBE010824 Value=%08X\n", Register2Value    );
	printf("[OMIINO] [  ] Third Magic Number.      Address=0xBE010890 Value=%04X\n", ShortRegisterValue);

	printf("[OMIINO] [  ] Writing Third Magic Number: Address=0xBE010890 Value=0x0046\n");
#ifndef XALYO_FRAMER_ACCESS
	*(volatile U16 *)0xBE010890=0x0046;
#else
        {
            volatile U16 *sptr;

            sptr = (volatile U16 *)0xBE010890;
            WP_MEM_SHORT_SET(sptr[0], 0x0046);
        }
#endif
	WPX_UFE_FRAMER_MIPS_Delay(1000);
	printf("[OMIINO] [  ] Third Magic Number. Address=0xBE010890 Value=%04X\n", ShortRegisterValue);

	printf("[OMIINO] [<-] WPX_UFE_FRAMER_MIPS_DisplayChipSelect_Serial_1\n");

}
