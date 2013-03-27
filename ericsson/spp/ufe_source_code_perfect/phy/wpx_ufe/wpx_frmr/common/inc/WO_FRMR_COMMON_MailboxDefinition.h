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

#ifndef _MAILBOX_DEFINITION_PUBLIC_
#define _MAILBOX_DEFINITION_PUBLIC_


#ifdef __cplusplus
extern "C"
{
#endif

#define WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION           (0)
#define WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_SIGNALLING              (1)
#define WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY                 (2)
#define WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_STATUS                  (3)
#define WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_PERFORMANCE_MONITORING  (4)
#define WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_SIGNALLING              (5)
#define WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_OMNISPY                 (6)
#define WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_AUXILIARY_OMNISPY       (7)
#define WPX_UFE_FRAMER_UNI_DIRECTIONAL_MAX_COMMS_CHANNELS                               (8)

#define WPX_UFE_FRAMER_BI_DIRECTIONAL_COMMS_CHANNEL_UART								(WPX_UFE_FRAMER_UNI_DIRECTIONAL_MAX_COMMS_CHANNELS)


typedef struct WPX_UFE_FRAMER_WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_ELEMENT_TYPE
{
    volatile unsigned int *                     RAM;
    volatile unsigned int *                     CTRL;
    volatile unsigned int *                     PEND;
    volatile unsigned int *                     DDR;
    volatile unsigned int *                     ENABLE;

} WPX_UFE_FRAMER_WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_ELEMENT_TYPE;



typedef struct WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE
{
    WPX_UFE_FRAMER_WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_ELEMENT_TYPE             Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_MAX_COMMS_CHANNELS];

} WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE;





#ifdef __cplusplus
}
#endif

#endif /* Include this file only once */

