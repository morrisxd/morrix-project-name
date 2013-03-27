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




void WPX_UFE_FRAMER_MIPS_LLC_Register_InitializeNullMessage(volatile WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE *pMemoryMap, U8 iCommsChannel)
{
    int i;
    volatile unsigned int * pRAM=pMemoryMap->Address[iCommsChannel].RAM;

#ifndef XALYO_FRAMER_ACCESS
    *(pRAM++)=(unsigned int)OMIINO_MESSAGE_CATALOGUE_NULL_MESSAGE;
    *(pRAM++)=0x00000000;   /* TID          */
    *(pRAM++)=0x00000000;   /* Length       */
    *(pRAM++)=0x00000000;   /* Attributes   */

    for(i=4;i<WPX_UFE_FRAMER_MAX_WORDS_IN_MESSAGE;i++)
    {
        *(pRAM++)=0xDEAD0000|(U32)iCommsChannel;
    }
#else
    WP_MEM_SET(pRAM[0], (unsigned int) OMIINO_MESSAGE_CATALOGUE_NULL_MESSAGE);
    pRAM++;
    WP_MEM_SET(pRAM[0], 0);
    pRAM++;
    WP_MEM_SET(pRAM[0], 0);
    pRAM++;
    WP_MEM_SET(pRAM[0], 0);
    pRAM++;

    for(i=4;i<WPX_UFE_FRAMER_MAX_WORDS_IN_MESSAGE;i++)
    {
        WP_MEM_SET(pRAM[0], 0xDEAD0000|(U32)iCommsChannel);
        pRAM++;
    }
#endif
}

void WPX_UFE_FRAMER_MIPS_Enable_MailboxCommunicationChannels(void)
{
#ifdef XALYO_FRAMER_ACCESS 
    volatile unsigned int *ptr;
#endif

    if(0x00000000== *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION].CTRL)
    {
        WPX_UFE_FRAMER_MIPS_LLC_Register_InitializeNullMessage(&OMIINO_RAM.Device[0].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION);
#ifndef XALYO_FRAMER_ACCESS
        *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION].CTRL=0x00000001;
#else
        ptr = OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION].CTRL;
        WP_MEM_SET(ptr[0], 1);
#endif
    }

    if(0x00000000== *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_SIGNALLING].CTRL)
    {
        WPX_UFE_FRAMER_MIPS_LLC_Register_InitializeNullMessage(&OMIINO_RAM.Device[0].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_SIGNALLING);
#ifndef XALYO_FRAMER_ACCESS
        *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_SIGNALLING].CTRL=0x00000001;
#else
        ptr = OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_SIGNALLING].CTRL;
        WP_MEM_SET(ptr[0], 1);
#endif
    }

    if(0x00000000== *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY].CTRL)
    {
        WPX_UFE_FRAMER_MIPS_LLC_Register_InitializeNullMessage(&OMIINO_RAM.Device[0].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY);
#ifndef XALYO_FRAMER_ACCESS
        *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY].CTRL=0x00000001;
#else
        ptr = OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY].CTRL;
        WP_MEM_SET(ptr[0], 1);
#endif
    }
    

    if(0x00000000 == *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_STATUS].CTRL)
    {
#ifndef XALYO_FRAMER_ACCESS
        *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_STATUS].CTRL=0x00000001; /* TODO Device hard coded */
#else
        ptr = OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_STATUS].CTRL;
        WP_MEM_SET(ptr[0], 1);
#endif
    }

    if(0x00000000 == *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_PERFORMANCE_MONITORING].CTRL)
    {
#ifndef XALYO_FRAMER_ACCESS
        *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_PERFORMANCE_MONITORING].CTRL=0x00000001; /* TODO Device hard coded */
#else
        ptr = OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_PERFORMANCE_MONITORING].CTRL;
        WP_MEM_SET(ptr[0], 1);
#endif
    }

    if(0x00000000 == *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_SIGNALLING].CTRL)
    {
#ifndef XALYO_FRAMER_ACCESS
        *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_SIGNALLING].CTRL=0x00000001; /* TODO Device hard coded */
#else
        ptr = OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_SIGNALLING].CTRL;
        WP_MEM_SET(ptr[0], 1);
#endif
    }
    
    if(0x00000000 == *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_OMNISPY].CTRL)
    {
#ifndef XALYO_FRAMER_ACCESS
        *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_OMNISPY].CTRL=0x00000001; /* TODO Device hard coded */
#else
        ptr = OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_OMNISPY].CTRL;
        WP_MEM_SET(ptr[0], 1);
#endif
    }
    
    if(0x00000000 == *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_AUXILIARY_OMNISPY].CTRL)
    {
#ifndef XALYO_FRAMER_ACCESS
        *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_AUXILIARY_OMNISPY].CTRL=0x00000001; /* TODO Device hard coded */
#else
        ptr = OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_AUXILIARY_OMNISPY].CTRL;
        WP_MEM_SET(ptr[0], 1);
#endif
    }
    
#ifndef XALYO_FRAMER_ACCESS
    *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_STATUS].ENABLE=0x00000001;
    *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_PERFORMANCE_MONITORING].ENABLE=0x00000001;
    *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_SIGNALLING].ENABLE=0x00000001;
    *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_OMNISPY].ENABLE=0x00000001;
    *OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_AUXILIARY_OMNISPY].ENABLE=0x00000001;
#else
        ptr = OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_STATUS].ENABLE;
        WP_MEM_SET(ptr[0], 1);
        ptr = OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_PERFORMANCE_MONITORING].ENABLE;
        WP_MEM_SET(ptr[0], 1);
        ptr = OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_SIGNALLING].ENABLE;
        WP_MEM_SET(ptr[0], 1);
        ptr = OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_OMNISPY].ENABLE;
        WP_MEM_SET(ptr[0], 1);
        ptr = OMIINO_RAM.Device[0].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_AUXILIARY_OMNISPY].ENABLE;
        WP_MEM_SET(ptr[0], 1);

#endif
}





void WPX_UFE_FRAMER_MIPS_ServiceNorthboundLowLevelComms(void)
{
	WPX_FRMR_ServiceNorthboundLowLevelComms(0);
}



