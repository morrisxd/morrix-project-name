#ifndef  _OMIINO_FRAMER_LOW_LEVEL_COMMS_API_
#define  _OMIINO_FRAMER_LOW_LEVEL_COMMS_API_


/*
	About: Copyright

	Copyright (c) 2010  Omiino Ltd

	All rights reserved.  

	This code is provided under license and or Non-disclosure     
	Agreement and must be used solely for the purpose for which it
	was provided. 

	It must not be passed to any third party without
	the written permission of Omiino Ltd.   

	Visit <http://www.omiino.com> or send messages to support@omiino.com.


*/




#ifdef __cplusplus
extern "C"
{
#endif

    
#include "WO_FRMR_API_public.h"



void OMIINO_LLC_BindMailboxCopyErrorHandler(void (*AnyHandler)(char *));


void OMIINO_LLC_API_Transmit(volatile WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE *pMemoryMap, U8 iCommsChannel, U32 * pMessage, U8 Length);
U8   OMIINO_LLC_API_TryReceive(volatile WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE *pMemoryMap, U8 iCommsChannel, U32 * pMessage, U8 * pLength);


void OMIINO_LLC_Direction_Set(volatile WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE *pMemoryMap, U8 iCommsChannel, U8 DirectionIsNorthbound);
U8   OMIINO_LLC_Direction_IsNorthbound(volatile WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE *pMemoryMap, U8 iCommsChannel);
U8   OMIINO_LLC_Direction_IsSouthbound(volatile WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE *pMemoryMap, U8 iCommsChannel);

void OMIINO_LLC_Register_IER_Enable(volatile WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE *pMemoryMap, U8 iCommsChannel);
void OMIINO_LLC_Register_IER_DisableAll(volatile WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE *pMemoryMap);

U8   OMIINO_LLC_Register_IPR_IsRequestingService(volatile WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE *pMemoryMap, U8 iCommsChannel);
void OMIINO_LLC_Register_IPR_NorthBoundAcknowledgeMessageTransfer(volatile WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE *pMemoryMap, U8 iCommsChannel);

void OMIINO_LLC_Register_CTRL_ToggleOwnership(volatile WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE *pMemoryMap, U8 iCommsChannel);

U8   OMIINO_LLC_Register_CTRL_Southbound_MailboxAvailable(volatile WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE *pMemoryMap, U8 iCommsChannel);
U8   OMIINO_LLC_Register_CTRL_Northbound_MailboxHasMessage(volatile WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE *pMemoryMap, U8 iCommsChannel);

U8   OMIINO_LLC_Register_CTRL_Northbound_MailboxAvailable(volatile WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE *pMemoryMap, U8 iCommsChannel);
U8   OMIINO_LLC_Register_CTRL_Southbound_MailboxHasMessage(volatile WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE *pMemoryMap, U8 iCommsChannel);

void OMIINO_LLC_Register_Initialize(volatile WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE *pMemoryMap);

#ifdef __cplusplus
}
#endif


#endif /* _OMIINO_FRAMER_LOW_LEVEL_COMMS_API_ */

