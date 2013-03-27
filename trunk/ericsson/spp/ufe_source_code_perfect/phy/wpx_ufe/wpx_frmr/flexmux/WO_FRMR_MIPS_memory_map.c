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

#include "WO_FRMR_private.h"
#include "WO_FRMR_MIPS_private.h"


void WPX_UFE_FRAMER_605_Initialize_MemoryMap(WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE (*pMailboxMemoryMap)[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES])
{
    pMailboxMemoryMap[0]->Address[0].CTRL=(volatile unsigned int *)0x80000000;
    pMailboxMemoryMap[0]->Address[1].CTRL=(volatile unsigned int *)0x80000008;
    pMailboxMemoryMap[0]->Address[2].CTRL=(volatile unsigned int *)0x80000010;
    pMailboxMemoryMap[0]->Address[3].CTRL=(volatile unsigned int *)0x80000018;
    pMailboxMemoryMap[0]->Address[4].CTRL=(volatile unsigned int *)0x80000020;
    pMailboxMemoryMap[0]->Address[5].CTRL=(volatile unsigned int *)0x80000028;
    pMailboxMemoryMap[0]->Address[6].CTRL=(volatile unsigned int *)0x80000030;
    pMailboxMemoryMap[0]->Address[7].CTRL=(volatile unsigned int *)0x80000038;

    pMailboxMemoryMap[0]->Address[0].PEND=(volatile unsigned int *)0x80000040;       
    pMailboxMemoryMap[0]->Address[1].PEND=(volatile unsigned int *)0x80000048;       
    pMailboxMemoryMap[0]->Address[2].PEND=(volatile unsigned int *)0x80000050;       
    pMailboxMemoryMap[0]->Address[3].PEND=(volatile unsigned int *)0x80000058;       
    pMailboxMemoryMap[0]->Address[4].PEND=(volatile unsigned int *)0x80000060;       
    pMailboxMemoryMap[0]->Address[5].PEND=(volatile unsigned int *)0x80000068;       
    pMailboxMemoryMap[0]->Address[6].PEND=(volatile unsigned int *)0x80000070;       
    pMailboxMemoryMap[0]->Address[7].PEND=(volatile unsigned int *)0x80000078;       

    pMailboxMemoryMap[0]->Address[0].ENABLE=(volatile unsigned int *)0x80000080;     
    pMailboxMemoryMap[0]->Address[1].ENABLE=(volatile unsigned int *)0x80000088;     
    pMailboxMemoryMap[0]->Address[2].ENABLE=(volatile unsigned int *)0x80000090;     
    pMailboxMemoryMap[0]->Address[3].ENABLE=(volatile unsigned int *)0x80000098;     
    pMailboxMemoryMap[0]->Address[4].ENABLE=(volatile unsigned int *)0x800000A0;     
    pMailboxMemoryMap[0]->Address[5].ENABLE=(volatile unsigned int *)0x800000A8;     
    pMailboxMemoryMap[0]->Address[6].ENABLE=(volatile unsigned int *)0x800000B0;     
    pMailboxMemoryMap[0]->Address[7].ENABLE=(volatile unsigned int *)0x800000B8;     

    pMailboxMemoryMap[0]->Address[0].DDR=(volatile unsigned int *)0x800000C0;
    pMailboxMemoryMap[0]->Address[1].DDR=(volatile unsigned int *)0x800000C8;
    pMailboxMemoryMap[0]->Address[2].DDR=(volatile unsigned int *)0x800000D0;
    pMailboxMemoryMap[0]->Address[3].DDR=(volatile unsigned int *)0x800000D8;
    pMailboxMemoryMap[0]->Address[4].DDR=(volatile unsigned int *)0x800000E0;
    pMailboxMemoryMap[0]->Address[5].DDR=(volatile unsigned int *)0x800000E8;
    pMailboxMemoryMap[0]->Address[6].DDR=(volatile unsigned int *)0x800000F0;
    pMailboxMemoryMap[0]->Address[7].DDR=(volatile unsigned int *)0x800000F8;

    pMailboxMemoryMap[0]->Address[0].RAM=(volatile unsigned int *)0x80000800;
    pMailboxMemoryMap[0]->Address[1].RAM=(volatile unsigned int *)0x80000884;
    pMailboxMemoryMap[0]->Address[2].RAM=(volatile unsigned int *)0x80000908;
    pMailboxMemoryMap[0]->Address[3].RAM=(volatile unsigned int *)0x8000098C;
    pMailboxMemoryMap[0]->Address[4].RAM=(volatile unsigned int *)0x80000A10;
    pMailboxMemoryMap[0]->Address[5].RAM=(volatile unsigned int *)0x80000A94;
    pMailboxMemoryMap[0]->Address[6].RAM=(volatile unsigned int *)0x80000B18;
    pMailboxMemoryMap[0]->Address[7].RAM=(volatile unsigned int *)0x80000B9C;

}


void WPX_UFE_FRAMER_MIPS_Initialize_MemoryMap(WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE (*pMailboxMemoryMap)[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES])
{
	U32 flexmux_base = 0xBF240000;   

	pMailboxMemoryMap[0]->Address[0].CTRL=(volatile unsigned int *)		 (flexmux_base + 0x0);
    pMailboxMemoryMap[0]->Address[1].CTRL=(volatile unsigned int *)		 (flexmux_base + 0x8);
    pMailboxMemoryMap[0]->Address[2].CTRL=(volatile unsigned int *)		 (flexmux_base + 0x10);
    pMailboxMemoryMap[0]->Address[3].CTRL=(volatile unsigned int *)		 (flexmux_base + 0x18);
    pMailboxMemoryMap[0]->Address[4].CTRL=(volatile unsigned int *)		 (flexmux_base + 0x20);
    pMailboxMemoryMap[0]->Address[5].CTRL=(volatile unsigned int *)		 (flexmux_base + 0x28);
    pMailboxMemoryMap[0]->Address[6].CTRL=(volatile unsigned int *)		 (flexmux_base + 0x30);
    pMailboxMemoryMap[0]->Address[7].CTRL=(volatile unsigned int *)		 (flexmux_base + 0x38);
																		
    pMailboxMemoryMap[0]->Address[0].PEND=(volatile unsigned int *)		 (flexmux_base + 0x40);        
    pMailboxMemoryMap[0]->Address[1].PEND=(volatile unsigned int *)		 (flexmux_base + 0x48);        
    pMailboxMemoryMap[0]->Address[2].PEND=(volatile unsigned int *)		 (flexmux_base + 0x50);        
    pMailboxMemoryMap[0]->Address[3].PEND=(volatile unsigned int *)		 (flexmux_base + 0x58);        
    pMailboxMemoryMap[0]->Address[4].PEND=(volatile unsigned int *)		 (flexmux_base + 0x60);        
    pMailboxMemoryMap[0]->Address[5].PEND=(volatile unsigned int *)		 (flexmux_base + 0x68);        
    pMailboxMemoryMap[0]->Address[6].PEND=(volatile unsigned int *)		 (flexmux_base + 0x70);        
    pMailboxMemoryMap[0]->Address[7].PEND=(volatile unsigned int *)		 (flexmux_base + 0x78);        
																		
    pMailboxMemoryMap[0]->Address[0].ENABLE=(volatile unsigned int *)	(flexmux_base + 0x80);    
    pMailboxMemoryMap[0]->Address[1].ENABLE=(volatile unsigned int *)	(flexmux_base + 0x88);    
    pMailboxMemoryMap[0]->Address[2].ENABLE=(volatile unsigned int *)	(flexmux_base + 0x90);    
    pMailboxMemoryMap[0]->Address[3].ENABLE=(volatile unsigned int *)	(flexmux_base + 0x98);    
    pMailboxMemoryMap[0]->Address[4].ENABLE=(volatile unsigned int *)	(flexmux_base + 0xA0);    
    pMailboxMemoryMap[0]->Address[5].ENABLE=(volatile unsigned int *)	(flexmux_base + 0xA8);    
    pMailboxMemoryMap[0]->Address[6].ENABLE=(volatile unsigned int *)	(flexmux_base + 0xB0);    
    pMailboxMemoryMap[0]->Address[7].ENABLE=(volatile unsigned int *)	(flexmux_base + 0xB8);    
																		
    pMailboxMemoryMap[0]->Address[0].DDR=(volatile unsigned int *)		(flexmux_base + 0xC0);
    pMailboxMemoryMap[0]->Address[1].DDR=(volatile unsigned int *)		(flexmux_base + 0xC8);
    pMailboxMemoryMap[0]->Address[2].DDR=(volatile unsigned int *)		(flexmux_base + 0xD0);
    pMailboxMemoryMap[0]->Address[3].DDR=(volatile unsigned int *)		(flexmux_base + 0xD8);
    pMailboxMemoryMap[0]->Address[4].DDR=(volatile unsigned int *)		(flexmux_base + 0xE0);
    pMailboxMemoryMap[0]->Address[5].DDR=(volatile unsigned int *)		(flexmux_base + 0xE8);
    pMailboxMemoryMap[0]->Address[6].DDR=(volatile unsigned int *)		(flexmux_base + 0xF0);
    pMailboxMemoryMap[0]->Address[7].DDR=(volatile unsigned int *)		(flexmux_base + 0xF8);
																		
    pMailboxMemoryMap[0]->Address[0].RAM=(volatile unsigned int *)		(flexmux_base + 0x800);
    pMailboxMemoryMap[0]->Address[1].RAM=(volatile unsigned int *)		(flexmux_base + 0x884);
    pMailboxMemoryMap[0]->Address[2].RAM=(volatile unsigned int *)		(flexmux_base + 0x908);
    pMailboxMemoryMap[0]->Address[3].RAM=(volatile unsigned int *)		(flexmux_base + 0x98C);
    pMailboxMemoryMap[0]->Address[4].RAM=(volatile unsigned int *)		(flexmux_base + 0xA10);
    pMailboxMemoryMap[0]->Address[5].RAM=(volatile unsigned int *)		(flexmux_base + 0xA94);
    pMailboxMemoryMap[0]->Address[6].RAM=(volatile unsigned int *)		(flexmux_base + 0xB18);
    pMailboxMemoryMap[0]->Address[7].RAM=(volatile unsigned int *)		(flexmux_base + 0xB9C);
}

