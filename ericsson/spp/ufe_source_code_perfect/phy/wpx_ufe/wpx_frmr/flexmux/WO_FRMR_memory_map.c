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


volatile int * OMIINO_FRAMER_GivenMailboxBaseGetAnyMailbox(WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE * pMailboxMemoryMap, U8 iMailbox)
{
    return (volatile int *)pMailboxMemoryMap->Address[iMailbox].RAM;
}


void OMIINO_FRAMER_BindMemoryMap(OMIINO_FRAMER_RAM_TYPE *pRAM,
                                 WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE (*pMailboxMemoryMap)[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES])
{
   U32 iDevice=0;
   U32 iMailbox;
   
   for(iDevice=0; iDevice<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; iDevice++)
   {            
      for(iMailbox=0;iMailbox<WPX_UFE_FRAMER_UNI_DIRECTIONAL_MAX_COMMS_CHANNELS;iMailbox++)
      {
          pRAM->Device[iDevice].MemoryMap.Address[iMailbox].CTRL=(*pMailboxMemoryMap)[iDevice].Address[iMailbox].CTRL;
          pRAM->Device[iDevice].MemoryMap.Address[iMailbox].ENABLE=(*pMailboxMemoryMap)[iDevice].Address[iMailbox].ENABLE;
          pRAM->Device[iDevice].MemoryMap.Address[iMailbox].PEND=(*pMailboxMemoryMap)[iDevice].Address[iMailbox].PEND;
          pRAM->Device[iDevice].MemoryMap.Address[iMailbox].RAM=(*pMailboxMemoryMap)[iDevice].Address[iMailbox].RAM;
      }
   }
}



