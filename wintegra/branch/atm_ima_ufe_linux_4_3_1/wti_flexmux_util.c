/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/****************************************************************************
 *
 * Example: wti_flexmux_util.c
 * Demonstrates the use of Omiino Flexmux API.
 *
 ****************************************************************************/

#include <string.h>
#include "wp_wddi.h"

#ifndef WTI_FLEXMUX_UTIL_H
#include "wti_flexmux_util.h"
#endif
#ifndef _WPL_PLATFORM_H__
#include <wpl_platform.h>
#endif
#ifndef _SW_SEMAPHORE_H__
#include <wpl_sw_semaphore.h>
#endif

#ifdef __WT_UFE4_REVB__
#define WTI_UFE412_REVA 0
#else
#define WTI_UFE412_REVA 1
#endif

#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING
int ho_transfer_type_in_use;
int lo_transfer_type_in_use;
WP_U8 build_personality_in_use;
#endif

#define WTI_NUM_OF_WORDS_TO_READ 64

struct WTI_DIAG_PEEK_INFO{
      U8 iDevice;
      U32 Address;
      U32 Value;
      U8 Valid;

} DIAG_PEEK_INFO;
extern void AIS_insertion_enable                           (char *StrPrm);
void WTI_UfeFramerTerminateOnError(WP_handle handle, WP_CHAR *s,WP_U32 id,WP_U32 LineNum)
{
   if(handle != WUFE_OK)
   {
      printf("Test Abort %s %d %s 0x%x Line:%d\n", s,id,
             WUFE_error_name[(handle&0x03ffffff)], handle, LineNum);

      exit(1);
   }
#if WTI_DEBUG_LEVEL
   else
   {
      printf("Handle,Status returned from %s is %d, id is %d, Line %d\n", s, handle, id, LineNum);
   }
#endif
}

void WTI_UFE_FRAMER_PEEK_RESPONSE_CALLBACK(U8 iDevice, U32 Address, U32 Value)
{
   DIAG_PEEK_INFO.iDevice=iDevice;
   DIAG_PEEK_INFO.Address=Address;
   DIAG_PEEK_INFO.Value=Value;
   DIAG_PEEK_INFO.Valid=1;
#if 1
   printf("Peek number %d: Value is 0x%x\n",Address,Value);
#endif

   return;
}

void WTI_FlexmuxTerminateOnError(WP_CHAR * pFatalErrorMessage, WP_U8 error_code)
{
   printf("Test Abort: %s Line:%d Error code %d\n", pFatalErrorMessage, __LINE__, (WP_U32) error_code);
   exit(-1);
}

void WTI_FlexmuxCheckStatus(WP_CHAR * Message, WP_U8 status, WP_U32 line)
{
   extern char *OMIINO_FRAMER_ErrorCodeToTxt(WP_U32 ErrorCode);
   if (status)
   {
      printf("Test Abort: function %s in Line:%d returned Error code %s\n",
             Message, line, OMIINO_FRAMER_ErrorCodeToTxt((WP_U32) status));
//      exit(-1);
   }
   else
   {
#if WTI_FLEXMUX_DBG_MODE
      printf("Status returned from %s is WPX_UFE_FRAMER_OK\n", Message);
      return;
#endif
   }
}

/* Interrupt/lock coordination */
WP_U32 framer_lock;   /* coordinate between interrupt handler and mailbox server thread  */
WP_U32 server_lock;   /* coordinate between mailbox server thread and task server thread */

#define WTI_FRAMER_LOCK_KEY   WPL_LOCK_KEY_CREATE(WPL_SW_LOCK, WPL_PRIVATE_LOCK, 0, 0x37901)
#define WTI_SERVER_LOCK_KEY   WPL_LOCK_KEY_CREATE(WPL_SW_LOCK, WPL_PRIVATE_LOCK, 0, 0x37902)


#if (USE_POLLING == 0)
void WTI_flexmux_take_framer_lock(void)
{
   int rc;

   while (1)
   {
      rc = wpv_swsem_acquire((int *)(&framer_lock), NO_WAIT);
      if (rc == WPL_OK)
         /* Got the lock */
         return;
      /* Force a noncached reference */
      WPX_Ufe412CpldInterruptMaskGet(0);
   }
}

void WTI_flexmux_release_framer_lock(void)
{
   wpv_swsem_release((int *)(&framer_lock));
}

void WTI_flexmux_take_server_lock(void)
{
   int rc;

   while (1)
   {
      rc = wpv_swsem_acquire((int *)(&server_lock), NO_WAIT);
      if (rc == WPL_OK)
         /* Got the lock */
         return;
      /* Force a noncached reference */
      WPX_Ufe412CpldInterruptMaskGet(0);
   }
}

void WTI_flexmux_release_server_lock(void)
{
   wpv_swsem_release((int *)(&server_lock));
}

void initialize_locks(void)
{
   WPL_LockKeyInit(WTI_FRAMER_LOCK_KEY, &framer_lock);
   WPL_LockKeyInit(WTI_SERVER_LOCK_KEY, &server_lock);
}

#else

void initialize_locks(void) {}
void WTI_flexmux_take_framer_lock(void) {}
void WTI_flexmux_release_framer_lock(void) {}
void WTI_flexmux_take_server_lock(void) {}
void WTI_flexmux_release_server_lock(void) {}

#endif


WP_S32 WTI_FlexmuxDeviceInit(WP_U32 upi_index,
                             WP_U8 flexmux_id,
                             WTI_flexmux_physical_conf flexmux_hw_config,
                             WP_U8 flexmux_mode,
                             WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4,
                             WP_U8 max_line_ports,
                             WP_U8 line_increment,
                             WP_U8 wti_flexmux_build_personality,
                             WTI_flexmux_line_port_mode line_port_mode,
                             WTI_Flexmux_global_setup *flexmux_cfg);

WP_S32 WTI_FlexmuxClearChannelConfigureHighOrder(
												WP_U8 flexmux_id,
												WP_U8 flexmux_mode,
												WTI_flexmux_line_port_mode line_port_mode,
												WP_U8 line_increment,
												WP_U8 max_line_ports
												);



void WTI_allocate_pm_storage_areas(void);
WP_THREAD_ID mailbox_thread_id = 0;
WP_THREAD_ID alarm_and_pm_thread_id = 0;

WP_S32 WTI_FlexmuxInit(WP_U32 upi_index,
                       WP_U8 flexmux_id,
                       WTI_flexmux_physical_conf flexmux_hw_config,
                       WP_U8 flexmux_mode,
                       WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4,
                       WTI_Flexmux_global_setup *flexmux_cfg)
{
   WUFE_status status;
   WP_U8 max_line_ports=0, line_increment=0;
   WP_U8 wti_flexmux_build_personality=0;
   WTI_flexmux_line_port_mode line_port_mode=0;
#if WTI_DUAL_EMPHY
   WTI_flexmux_physical_conf second_flexmux_hw_config = 0;
#endif  

   if (flexmux_id > WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES)
      WTI_FlexmuxTerminateOnError("flexmux_id is out of bounds !", 0);

   if (flexmux_hw_config != WT_FLEXMUX_UFE_412_1_PORT_OC12 &&
       flexmux_hw_config != WT_FLEXMUX_UFE_412_2_PORTS_OC3 &&
       flexmux_hw_config != WT_FLEXMUX_UFE_412_4_PORTS_OC3 &&
       flexmux_hw_config != WT_FLEXMUX_UFE_448_4_PORTS_OC12 &&
       flexmux_hw_config != WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION &&
       flexmux_hw_config != WT_FLEXMUX_UFE_448_4_PORTS_OC12_CARD_PROTECTION_MASTER &&
       flexmux_hw_config != WT_FLEXMUX_UFE_448_4_PORTS_OC12_CARD_PROTECTION_SLAVE &&
       flexmux_hw_config != WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER &&
       flexmux_hw_config != WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE &&
       flexmux_hw_config != WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE &&
       flexmux_hw_config != WT_FLEXMUX_UFE_412_4_PORTS_OC3_CLEAR_CHANNEL &&
       flexmux_hw_config != WT_FLEXMUX_UFE_412_1_PORT_OC12_CLEAR_CHANNEL 

#if WTI_DUAL_EMPHY
       &&  flexmux_hw_config != WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_UPI1_MASTER_UPI3_SLAVE
#endif
      )
      WTI_FlexmuxTerminateOnError("Invalid flexmux_hw_config !", 0);

   if (flexmux_mode != WPX_UFE_FRAMER_DEVICE_MODE_SONET &&
       flexmux_mode != WPX_UFE_FRAMER_DEVICE_MODE_SDH)
      WTI_FlexmuxTerminateOnError("Invalid flexmux_mode !", 0);

   WTI_allocate_pm_storage_areas();

   if ( flexmux_hw_config == WT_FLEXMUX_UFE_412_1_PORT_OC12 ||
		flexmux_hw_config== WT_FLEXMUX_UFE_412_1_PORT_OC12_CLEAR_CHANNEL)
   {
      if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
      {
         line_port_mode = WT_FLEXMUX_LINE_PORT_OC12;
      }
      else if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
      {
         line_port_mode = WT_FLEXMUX_LINE_PORT_STM4;
      }
      max_line_ports = 1;
      line_increment = 1;
      wti_flexmux_build_personality = WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12;
#if WTI_DUAL_EMPHY
      second_flexmux_hw_config = flexmux_hw_config;
#endif  
   }
   else if (flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3 ||
            flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER ||
            flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE ||
            flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3_CLEAR_CHANNEL)
   {
      if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
      {
         line_port_mode = WT_FLEXMUX_LINE_PORT_OC3;
      }
      else if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
      {
         line_port_mode = WT_FLEXMUX_LINE_PORT_STM1;
      }
      max_line_ports = 4;
      line_increment = 1;
      wti_flexmux_build_personality = WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3;
#if WTI_DUAL_EMPHY
      second_flexmux_hw_config = flexmux_hw_config;
#endif      
   }
#if WTI_DUAL_EMPHY
   else if (flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_UPI1_MASTER_UPI3_SLAVE)
   {
      if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
      {
         line_port_mode = WT_FLEXMUX_LINE_PORT_OC3;
      }
      else if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
      {
         line_port_mode = WT_FLEXMUX_LINE_PORT_STM1;
      }
      max_line_ports = 4;
      line_increment = 1;
      wti_flexmux_build_personality = WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3;
      flexmux_hw_config = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER;
      second_flexmux_hw_config = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE;
   }
#endif
   else if (flexmux_hw_config == WT_FLEXMUX_UFE_412_2_PORTS_OC3)
   {
      if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
      {
         line_port_mode = WT_FLEXMUX_LINE_PORT_OC3;
      }
      else if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
      {
         line_port_mode = WT_FLEXMUX_LINE_PORT_STM1;
      }
      max_line_ports = 2;
      line_increment = 1;
      wti_flexmux_build_personality = WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3;
#if WTI_DUAL_EMPHY
      second_flexmux_hw_config = flexmux_hw_config;
#endif  
   }
   else if (flexmux_hw_config == WT_FLEXMUX_UFE_448_4_PORTS_OC12                         ||
            flexmux_hw_config == WT_FLEXMUX_UFE_448_4_PORTS_OC12_CARD_PROTECTION_MASTER  ||
            flexmux_hw_config == WT_FLEXMUX_UFE_448_4_PORTS_OC12_CARD_PROTECTION_SLAVE )
   {
      if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
      {
         line_port_mode = WT_FLEXMUX_LINE_PORT_OC12;
      }
      else if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
      {
         line_port_mode = WT_FLEXMUX_LINE_PORT_STM4;
      }
      max_line_ports = 4;
      line_increment = 1;
      wti_flexmux_build_personality = WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12;
   }
   else if (flexmux_hw_config == WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION)
   {
      if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
      {
         line_port_mode = WT_FLEXMUX_LINE_PORT_OC3;
      }
      else if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
      {
         line_port_mode = WT_FLEXMUX_LINE_PORT_STM1;
      }
      max_line_ports = 8;
      line_increment = 1;
      wti_flexmux_build_personality = WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3;
   }
   else
   {
      WTI_FlexmuxTerminateOnError("Invalid flexmux_hw_config !", 0);
   }

#if WTI_DUAL_EMPHY
   if (flexmux_id == WTI_FLEXMUX_SECOND_FLEXMUX_ID)
      WTI_FlexmuxTerminateOnError("Invalid flexmux_id for DUAL EMPHY !", 0);
#endif
   
   initialize_locks();

   memset(flexmux_cfg, 0, sizeof(WTI_Flexmux_global_setup));

   /* set flexmux ID */
   flexmux_cfg->flexmux_id = flexmux_id;

#if WTI_DUAL_EMPHY
   /* set second flexmux ID */
   flexmux_cfg->second_flexmux_id = WTI_FLEXMUX_SECOND_FLEXMUX_ID;
#endif

   /* Mask all framer interrupts in the CPLD so that northbound messaging is disabled */
   WPX_Ufe412CpldInterruptMaskSet(0, WPX_FPGA_INTR_ALL_MASKED);

   /*register the peek response callback function to the frmr driver*/
   WPX_FRMR_RegisterPeekCallback(WTI_UFE_FRAMER_PEEK_RESPONSE_CALLBACK); 

   /* register the framer lock take function */
   WPX_FRMR_RegisterFramerLockCallback(WTI_flexmux_take_framer_lock);

   /* Initialize the Flexmux devices */
   status = WPX_UFE_FRAMER_FlexmuxInit(flexmux_cfg->flexmux_id,
                                       upi_index);
   WTI_FlexmuxCheckStatus("WPX_UFE_FRAMER_FlexmuxInit", status, __LINE__);

#if defined(WP_BOARD_WDS3)
   {
      /* Workaround for lost packets due to transceiver */
      WP_U32 j;

#if defined(__WT_UFE412__)
      for (j = 0x88000028; j <= 0x88000e28; j += 0x200)
         WPX_FRMR_DEV_DIAG_Poke(0, 0, j, 0x00000001);
#elif defined(__WT_UFE448__)
      for (j = 0x88000028; j <= 0x88000e28; j += 0x200)
         WPX_FRMR_DEV_DIAG_Poke(0, 0, j, 0x00000000);
#endif
   }
#endif

   /* service the mailboxes in a separate thread */
   {
      WP_U32 thread_params[2*WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES];

      thread_params[0] = flexmux_cfg->flexmux_id;
      thread_params[1] = upi_index;
#if WTI_DUAL_EMPHY
      thread_params[2] = flexmux_cfg->second_flexmux_id;
      thread_params[3] = WTI_FLEXMUX_SECOND_UPI_INDEX;
#endif

      if (mailbox_thread_id == 0)
         WPL_ThreadInit(&mailbox_thread_id,
                     (void *(*)(void *)) WPX_UFE_FRAMER_FlexmuxServiceMailbox,
                     (void *) thread_params);
   }

#if WTI_FLEXMUX_ALARM_AND_PM_THREAD

   /* service the alarms and PM callbacks in a separate thread */
   {
      WP_U32 thread_params[2] = { 0, 1 };
      void process_framer_task_list(void);

      if (alarm_and_pm_thread_id == 0)
         WPL_ThreadInit(&alarm_and_pm_thread_id,
                        (void *(*)(void *)) process_framer_task_list,
                        (void *) thread_params);
   }

#endif

   /* set the Flexmux device build personality */
   status = WPX_UFE_FRAMER_FlexmuxSetBuildPersonality(flexmux_cfg->flexmux_id,
                                                      wti_flexmux_build_personality,
                                                      upi_index);
   WTI_UfeFramerTerminateOnError(status, "first UFE WPX_UFE_FRAMER_FlexmuxSetBuildPersonality", 0, __LINE__);
   
#if WTI_DUAL_EMPHY
   /* set the Flexmux device build personality */
   status = WPX_UFE_FRAMER_FlexmuxSetBuildPersonality(flexmux_cfg->second_flexmux_id,
                                                      wti_flexmux_build_personality,
                                                      WTI_FLEXMUX_SECOND_UPI_INDEX);
   WTI_UfeFramerTerminateOnError(status, "second UFE WPX_UFE_FRAMER_FlexmuxSetBuildPersonality", 0, __LINE__);
#endif
#if WTI_FLEXMUX_ENABLE_ALARMS
   printf("AIS_insertion_enable\n");   
   AIS_insertion_enable("0 0");
#endif

#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING
   build_personality_in_use = wti_flexmux_build_personality;
#endif

   WTI_FlexmuxDeviceInit(upi_index,
                         flexmux_id,
                         flexmux_hw_config,
                         flexmux_mode,
                         vc_3_4,
                         max_line_ports,
                         line_increment,
                         wti_flexmux_build_personality,
                         line_port_mode,
                         flexmux_cfg);

#if WTI_DUAL_EMPHY

   WP_Delay(50000);

   WTI_FlexmuxDeviceInit(WTI_FLEXMUX_SECOND_UPI_INDEX,
                         flexmux_cfg->second_flexmux_id,
                         second_flexmux_hw_config,
                         flexmux_mode,
                         vc_3_4,
                         max_line_ports,
                         line_increment,
                         wti_flexmux_build_personality,
                         line_port_mode,
                         flexmux_cfg);

#endif

   return 0;
}

WP_S32 WTI_FlexmuxDeviceInit(WP_U32 upi_index,
                             WP_U8 flexmux_id,
                             WTI_flexmux_physical_conf flexmux_hw_config,
                             WP_U8 flexmux_mode,
                             WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4,
                             WP_U8 max_line_ports,
                             WP_U8 line_increment,
                             WP_U8 wti_flexmux_build_personality,
                             WTI_flexmux_line_port_mode line_port_mode,
                             WTI_Flexmux_global_setup *flexmux_cfg)
{
   WUFE_status status;

#ifdef __WT_UFE448__
WPX_FRMR_DEMO_ONLY_Configure_GTP_ClockSource(flexmux_id);
#else
#if WTI_UFE412_REVA
   /* set the gpt source clock required with wds-ufe4-s6 board*/
   WPX_FRMR_DEMO_ONLY_Configure_GTP_ClockSource(flexmux_id);
#endif

#if WTI_FRAMER_SCRIPT
 return 0;
#endif
#endif

   /* set device mode (SONET\SDH) */
   status = WPX_FRMR_DEV_SetMode(0, flexmux_id, flexmux_mode);
   WTI_FlexmuxCheckStatus("WPX_UFE_FRAMER_CONFIGURATION_DEVICE_SetMode", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
      U8 pDeviceProtectionMode;
      U8 pCardProtectionOperatingMode;
#endif
   /* Set line protection according to the build personality in use*/
   if(flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER ||
      flexmux_hw_config == WT_FLEXMUX_UFE_448_4_PORTS_OC12_CARD_PROTECTION_MASTER){
      status = WPX_FRMR_SetDevProtectMode( 0, flexmux_id, WPX_UFE_DEVICE_PROTECTION_MODE_CARD);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SetDevProtectMode", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
      WPX_FRMR_GetDevProtectMode (flexmux_id,&pDeviceProtectionMode);
      if(pDeviceProtectionMode != WPX_UFE_DEVICE_PROTECTION_MODE_CARD)
         WTI_FlexmuxTerminateOnError("WPX_FRMR_GetDevProtectMode != WPX_UFE_DEVICE_PROTECTION_MODE_CARD\n", 0);
#endif
      status = WPX_FRMR_SetDevCardProtectOperatingMode(0,flexmux_id,WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_MASTER);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SetDevCardProtectOperatingMode", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
      WPX_FRMR_GetDevCardProtectOperatingMode(flexmux_id, &pCardProtectionOperatingMode);
      if(pCardProtectionOperatingMode!=WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_MASTER)
         WTI_FlexmuxTerminateOnError("WPX_FRMR_GetDevCardProtectOperatingMode != WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_MASTER\n", 0);
#endif
   }
   else if(flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE ||
           flexmux_hw_config == WT_FLEXMUX_UFE_448_4_PORTS_OC12_CARD_PROTECTION_SLAVE){
      status = WPX_FRMR_SetDevProtectMode( 0, flexmux_id, WPX_UFE_DEVICE_PROTECTION_MODE_CARD);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SetDevProtectMode", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
      WPX_FRMR_GetDevProtectMode (flexmux_id,&pDeviceProtectionMode);
      if(pDeviceProtectionMode != WPX_UFE_DEVICE_PROTECTION_MODE_CARD)
         WTI_FlexmuxTerminateOnError("WPX_FRMR_GetDevProtectMode != WPX_UFE_DEVICE_PROTECTION_MODE_CARD\n", 0);
#endif
      status = WPX_FRMR_SetDevCardProtectOperatingMode(0,flexmux_id,WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_SLAVE);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SetDevCardProtectOperatingMode", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
      WPX_FRMR_GetDevCardProtectOperatingMode(flexmux_id, &pCardProtectionOperatingMode);
      if(pCardProtectionOperatingMode!=WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_SLAVE)
         WTI_FlexmuxTerminateOnError("WPX_FRMR_GetDevCardProtectOperatingMode != WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_SLAVE\n", 0);
#endif

   }
   else{
      status = WPX_FRMR_SetDevProtectMode( 0, flexmux_id, WPX_UFE_DEVICE_PROTECTION_MODE_LINE);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SetDevProtectMode", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
      WPX_FRMR_GetDevProtectMode (flexmux_id,&pDeviceProtectionMode);
      if(pDeviceProtectionMode != WPX_UFE_DEVICE_PROTECTION_MODE_LINE)
         WTI_FlexmuxTerminateOnError("WPX_FRMR_GetDevProtectMode != WPX_UFE_DEVICE_PROTECTION_MODE_LINE\n", 0);
#endif
   }
#if WTI_FLEXMUX_DBG_MODE
   U8 pDeviceMode;
   status = WPX_FRMR_DEV_GetMode(flexmux_id,&pDeviceMode );
   WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
   switch(flexmux_mode)
   {
      case WPX_UFE_FRAMER_DEVICE_MODE_SDH:
         if(pDeviceMode != WPX_UFE_FRAMER_DEVICE_MODE_SDH)
         {
            WTI_FlexmuxTerminateOnError("WPX_FRMR_DEV_GetMode != WPX_UFE_DEVICE_MODE_SDH\n", 0);
         }
         break;
      case WPX_UFE_FRAMER_DEVICE_MODE_SONET:
         if(pDeviceMode != WPX_UFE_FRAMER_DEVICE_MODE_SONET)
         {
            WTI_FlexmuxTerminateOnError("WPX_FRMR_DEV_GetMode != WPX_UFE_DEVICE_MODE_SONET\n", 0);
         }
         break;
   }
#endif
   
   /* set flexmux physical configuration */
   flexmux_cfg->flexmux_hw_config = flexmux_hw_config;

   /* set flexmux build personality */
   flexmux_cfg->flexmux_build_personality = wti_flexmux_build_personality;

   /* save device parameters in registry */
   flexmux_reg[flexmux_id].is_initialized = WP_TRUE;
   flexmux_reg[flexmux_id].flexmux_hw_config = flexmux_hw_config;
   flexmux_reg[flexmux_id].flexmux_build_personality = wti_flexmux_build_personality;
   flexmux_reg[flexmux_id].flexmux_mode = flexmux_mode;
   flexmux_reg[flexmux_id].max_line_ports = max_line_ports;
   flexmux_reg[flexmux_id].vc_3_4 = vc_3_4;
   if(	flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3_CLEAR_CHANNEL ||
		flexmux_hw_config == WT_FLEXMUX_UFE_412_1_PORT_OC12_CLEAR_CHANNEL)
	   WTI_FlexmuxClearChannelConfigureHighOrder(flexmux_id,flexmux_mode,line_port_mode,line_increment,max_line_ports);
   else if (wti_flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12 ||
       wti_flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3      ||
       wti_flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12 ||
       wti_flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3      ||
       wti_flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3)
   {
      WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE line_end_point = {0};

#if WTI_ADM_MODE
/* in CR when using the ADM for testing */
      U8 TX_C2=0x1, EX_C2=0x1;
      WP_CHAR *TX_J1="0000000000000000", *EX_J1="0000000000000000",*pTX_TraceString="WG RS-TRACE",*pEX_TraceString="WG RS-TRACE";
#else /*  WTI_ADM_MODE*/
      U8 TX_C2=0x2, EX_C2=0x2;
      WP_CHAR *TX_J1="WG HP-TRACE", *EX_J1="WG HP-TRACE",*pTX_TraceString="WG RS-TRACE",*pEX_TraceString="WG RS-TRACE";
#endif/* WTI_ADM_MODE */

      if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
      {
         WP_U32 sts12, sts3, sts1;
         line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS1_SPE;

         if(line_port_mode == WT_FLEXMUX_LINE_PORT_OC3)
         {
            sts12 = 0;
            line_end_point.u.SONET.sts12 = sts12;
            for (sts3=0; sts3<max_line_ports; sts3+=line_increment)
            {
               WTI_FlexmuxLinePortConfig(flexmux_id,
                                         sts3,
                                         line_port_mode);
               WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode(0, flexmux_id, sts3, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
               status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetTX(0, flexmux_id, sts3, pTX_TraceString);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetTX", status, __LINE__);

               status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetEX(0, flexmux_id, sts3, pEX_TraceString);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetEX", status, __LINE__);

               line_end_point.u.SONET.sts3 = sts3;
               for(sts1=0; sts1<3; ++sts1)
               {
                  line_end_point.u.SONET.sts1 = sts1;
                  status = WPX_FRMR_SONET_SDH_CreateFacility(0, flexmux_id, &line_end_point);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC3/4)", status, __LINE__);

                  /* set the transmitted and expected C2 values */
                  status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX(0, flexmux_id, &line_end_point, TX_C2);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                  U8 pTX_C2;
                  status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pTX_C2 );
                  WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
                  if(pTX_C2!=TX_C2)
                  {
                     WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != TX_C2\n", 0);
                  }
#endif
                  status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX(0, flexmux_id, &line_end_point, EX_C2);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                  U8 pEX_C2;
                  status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pEX_C2 );
                  WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
                  if(pTX_C2!=TX_C2)
                  {
                     WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != EX_C2\n", 0);
                  }
#endif


                  WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode(0, flexmux_id, &line_end_point, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
                  status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX(0, flexmux_id, &line_end_point, TX_J1);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                  WP_CHAR pTX_J1[65];
                  status =WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX(flexmux_id, &line_end_point, pTX_J1);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
                  if (strncmp(pTX_J1, TX_J1, 11))
                  {
                     printf("pTX_J1 %s\nTX_J1 %s\n", pTX_J1, TX_J1);
                     WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX != TX_J1\n", 0);
                  }
#endif

                  status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX(0, flexmux_id, &line_end_point, EX_J1);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                  WP_CHAR pEX_J1[65];
                  status = WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX(flexmux_id, &line_end_point, pEX_J1);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
                  if (strncmp(pEX_J1, EX_J1, 11))
                  {
                     printf("pEX_J1 %s\nEX_J1 %s\n", pEX_J1, EX_J1);
                     WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX != EX_J1\n", 0);
                  }
#endif
               }

            }
         }
         else if(line_port_mode == WT_FLEXMUX_LINE_PORT_OC12)
         {
            for (sts12=0; sts12<max_line_ports; sts12+=line_increment)
            {
               line_end_point.u.SONET.sts12 = sts12;
               WTI_FlexmuxLinePortConfig(flexmux_id,
                                         sts12,
                                         line_port_mode);
               WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode(0, flexmux_id, sts12, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
               status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetTX(0, flexmux_id, sts12, pTX_TraceString);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetTX", status, __LINE__);

               status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetEX(0, flexmux_id, sts12, pEX_TraceString);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetEX", status, __LINE__);

               for (sts3=0; sts3<4; ++sts3)
               {
                  line_end_point.u.SONET.sts3 = sts3;
                  for (sts1=0; sts1<3; ++sts1)
                  {
                     line_end_point.u.SONET.sts1 = sts1;
                     status = WPX_FRMR_SONET_SDH_CreateFacility(0, flexmux_id, &line_end_point);
                     WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC2/4)", status, __LINE__);

                     /* set the transmitted and expected C2 values */
                     status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX(0, flexmux_id, &line_end_point, TX_C2);
                     WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
                     U8 pTX_C2;
                     status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pTX_C2 );
                     WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
                     if(pTX_C2!=TX_C2)
                     {
                        WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != TX_C2\n", 0);
                     }
#endif
                     status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX(0, flexmux_id, &line_end_point, EX_C2);
                     WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                     U8 pEX_C2;
                     status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pEX_C2 );
                     WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
                     if(pTX_C2!=TX_C2)
                     {
                        WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != EX_C2\n", 0);
                     }
#endif
                     WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode(0, flexmux_id, &line_end_point, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
                     status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX(0, flexmux_id, &line_end_point, TX_J1);
                     WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                     WP_CHAR pTX_J1[65];
                     status = WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX(flexmux_id, &line_end_point, pTX_J1);
                     WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
                     if (strncmp(pTX_J1, TX_J1, 11))
                     {
                        printf("pTX_J1 %s\nTX_J1 %s\n", pTX_J1, TX_J1);
                        WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX != TX_J1\n", 0);
                     }
#endif

                     status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX(0, flexmux_id, &line_end_point, EX_J1);
                     WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                     WP_CHAR pEX_J1[65];
                     status = WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX(flexmux_id, &line_end_point, pEX_J1);
                     WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
                     if (strncmp(pEX_J1, EX_J1, 11))
                     {
                        printf("pEX_J1 %s\nEX_J1 %s\n", pEX_J1, EX_J1);
                        WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX != EX_J1\n", 0);
                     }
#endif
                  }

               }
            }
         }
      }
      else if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
      {
         WP_U32 stm4, stm1, stm0, num_for_vc3_or_vc4;

         line_end_point.TransferType = vc_3_4;

         /* for VC3 we need to configure 3 AU-3 containers
            and for VC4 we need to configure only 1 AU-4 container  */
         if (vc_3_4 == WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3)
         {
            num_for_vc3_or_vc4 = 3;
         }
         else
         {
            num_for_vc3_or_vc4 = 1;
         }

         if (line_port_mode == WT_FLEXMUX_LINE_PORT_STM1)
         {
             for (stm1=0; stm1<max_line_ports; stm1+=line_increment)
             {
                line_end_point.u.SDH.stm4 = 2 * (stm1/4); /*multiplication by 2 is for 8 plus 8 build - 
                                                            this is done in order to let the 252 upper lines
                                                            to be mapped to the second core */
                line_end_point.u.SDH.stm1 = (stm1%4);
                WTI_FlexmuxLinePortConfig(flexmux_id,
                                          stm1,
                                          line_port_mode);
                WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode(0, flexmux_id, stm1, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
                status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetTX(0, flexmux_id, stm1, pTX_TraceString);
                WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetTX", status, __LINE__);

                status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetEX(0, flexmux_id, stm1, pEX_TraceString);
                WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetEX", status, __LINE__);

                status = WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetExcessiveTh(510 + stm1, 0, stm1,
                                                                          WPX_UFE_FRAMER_THRESHOLD_10E4);
                WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetExcessiveTh", status, __LINE__);

                status = WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetSignalDegradeTh(410 + stm1, 0, stm1,
                                                                          WPX_UFE_FRAMER_THRESHOLD_10E5);
                WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetSignalDegradeTh", status, __LINE__);

                status = WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetExcessiveTh(540 + stm1, 0, stm1,
                                                                          WPX_UFE_FRAMER_THRESHOLD_10E5);
                WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetExcessiveTh", status, __LINE__);

                status = WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetSigDegradeTh(440 + stm1, 0, stm1,
                                                                          WPX_UFE_FRAMER_THRESHOLD_10E6);
                WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetSigDegradeTh", status, __LINE__);

                for(stm0=0; stm0<num_for_vc3_or_vc4; ++stm0)
                {
                   line_end_point.u.SDH.stm0 = stm0;
#if WTI_FLEXMUX_DBG_MODE
                   printf("line_end_point.u.SDH.stm4 %d\n", line_end_point.u.SDH.stm4);
                   printf("line_end_point.u.SDH.stm1 %d\n", line_end_point.u.SDH.stm1);
                   printf("line_end_point.u.SDH.stm0 %d\n", line_end_point.u.SDH.stm0);
                   
#endif
                   status = WPX_FRMR_SONET_SDH_CreateFacility(0, flexmux_id, &line_end_point);
                   WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC3/4)", status, __LINE__);

                   /* set the transmitted and expected C2 values */
                   status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX(0, flexmux_id, &line_end_point, TX_C2);
                   WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                   U8 pTX_C2;
                   status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pTX_C2 );
                   WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
                   if(pTX_C2!=TX_C2)
                   {
                      WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != TX_C2\n", 0);
                   }
#endif
                   status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX(0, flexmux_id, &line_end_point, EX_C2);
                   WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                   U8 pEX_C2;
                   status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pEX_C2 );
                   WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
                   if(pTX_C2!=TX_C2)
                   {
                      WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != EX_C2\n", 0);
                   }
#endif
                   WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode(0, flexmux_id, &line_end_point, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
                   status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX(0, flexmux_id, &line_end_point, TX_J1);
                   WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                   WP_CHAR pTX_J1[65];
                   status = WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX(flexmux_id, &line_end_point, pTX_J1);
                   WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX", status, __LINE__);
                   if (strncmp(pTX_J1, TX_J1, 11))
                   {
                      printf("pTX_J1 %s\nTX_J1 %s\n", pTX_J1, TX_J1);
                      WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX != TX_J1\n", 0);
                   }
#endif

                   status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX(0, flexmux_id, &line_end_point, EX_J1);
                   WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                   WP_CHAR pEX_J1[65];
                   status = WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX(flexmux_id, &line_end_point, pEX_J1);
                   WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
                   if (strncmp(pTX_J1, TX_J1, 11))
                   {
                      printf("pEX_J1 %s\nEX_J1 %s\n", pEX_J1, EX_J1);
                      WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX != EX_J1\n", 0);
                   }
#endif
                  status = WPX_FRMR_SONET_SDH_HO_PATH_B3_SetExcessiveTh(0,flexmux_id, &line_end_point,
                                                                        WPX_UFE_FRAMER_THRESHOLD_10E4);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_B3_SetExcessiveTh", status, __LINE__);

                  status = WPX_FRMR_SONET_SDH_HO_PATH_B3_SetSigDegradeTh(0, flexmux_id, &line_end_point,
                                                                        WPX_UFE_FRAMER_THRESHOLD_10E6);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_B3_SetSigDegradeTh", status, __LINE__);
                }
             }
         }
         else if (line_port_mode == WT_FLEXMUX_LINE_PORT_STM4)
         {
            for (stm4=0; stm4<max_line_ports; stm4+=line_increment)
            {
               line_end_point.u.SDH.stm4 = stm4;
               WTI_FlexmuxLinePortConfig(flexmux_id,
                                         stm4,
                                         line_port_mode);
               WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode(0, flexmux_id, stm4, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
               status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetTX(0, flexmux_id, stm4, pTX_TraceString);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetTX", status, __LINE__);

               status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetEX(0, flexmux_id, stm4, pEX_TraceString);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetEX", status, __LINE__);

                status = WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetExcessiveTh(510 + stm1, flexmux_id, stm4,
                                                                          WPX_UFE_FRAMER_THRESHOLD_10E4);
                WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetExcessiveTh", status, __LINE__);

                status = WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetSignalDegradeTh(410 + stm1, flexmux_id, stm4,
                                                                          WPX_UFE_FRAMER_THRESHOLD_10E5);
                WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetSignalDegradeTh", status, __LINE__);

                status = WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetExcessiveTh(540 + stm1, flexmux_id, stm4,
                                                                          WPX_UFE_FRAMER_THRESHOLD_10E5);
                WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetExcessiveTh", status, __LINE__);

                status = WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetSigDegradeTh(440 + stm1, flexmux_id, stm4,
                                                                          WPX_UFE_FRAMER_THRESHOLD_10E6);
                WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetSigDegradeTh", status, __LINE__);

               for (stm1=0; stm1<4; ++stm1)
               {
                  line_end_point.u.SDH.stm1 = stm1;
                  for(stm0=0; stm0<num_for_vc3_or_vc4; ++stm0)
                  {
                     line_end_point.u.SDH.stm0 = stm0;
                     status = WPX_FRMR_SONET_SDH_CreateFacility(0, flexmux_id, &line_end_point);
                     WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC3/4)", status, __LINE__);

                     /* set the transmitted and expected C2 values */
                     status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX(0, flexmux_id, &line_end_point, TX_C2);
                     WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                     U8 pTX_C2;
                     status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pTX_C2 );
                     WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
                     if(pTX_C2!=TX_C2)
                     {
                        WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != TX_C2\n", 0);
                     }
#endif
                     status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX(0, flexmux_id, &line_end_point, EX_C2);
                     WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                     U8 pEX_C2;
                     status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pEX_C2 );
                     WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
                     if(pTX_C2!=TX_C2)
                     {
                        WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != EX_C2\n", 0);
                     }
#endif
                     WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode(0, flexmux_id, &line_end_point, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
                     status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX(0, flexmux_id, &line_end_point, TX_J1);
                     WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                     WP_CHAR pTX_J1[65];
                     status = WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX(flexmux_id, &line_end_point, pTX_J1);
                     WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
                     if (strncmp(pTX_J1, TX_J1, 11))
                     {
                        printf("pTX_J1 %s\nTX_J1 %s\n", pTX_J1, TX_J1);
                        WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX != TX_J1\n", 0);
                     }
#endif

                     status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX(0, flexmux_id, &line_end_point, EX_J1);
                     WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                     WP_CHAR pEX_J1[65];
                     status = WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX(flexmux_id, &line_end_point, pEX_J1);
                     WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
                     if (strncmp(pEX_J1, EX_J1, 11))
                     {
                        printf("pEX_J1 %s\nEX_J1 %s\n", pEX_J1, EX_J1);
                        WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX != EX_J1\n", 0);
                     }
#endif
                     status = WPX_FRMR_SONET_SDH_HO_PATH_B3_SetExcessiveTh(700, 0, &line_end_point,
                                                                           WPX_UFE_FRAMER_THRESHOLD_10E4);
                     WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_B3_SetExcessiveTh", status, __LINE__);

                     status = WPX_FRMR_SONET_SDH_HO_PATH_B3_SetSigDegradeTh(701, 0, &line_end_point,
                                                                            WPX_UFE_FRAMER_THRESHOLD_10E6);
                     WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_B3_SetSigDegradeTh", status, __LINE__);
                  }
               }
            }
         }
      }
   }
   else
   {
      WTI_FlexmuxTerminateOnError("Unsupported Build personality", 0);
   }

   /* Set which SONET\SDH port Rx clock drives the RECV_LINE_CLK (which drive the External PLL diff 0/1 output) */
   /* WPX_FRMR_DEV_DIAG_DRV_RecClkSet(U32 TransactionIdentifier, U8 iDevice, U8 OutputSelector, U8 iLinePort)   */

   status = WPX_FRMR_DEV_DIAG_DRV_RecClkSet(0, 0, 0 ,0);
   WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_DIAG_DRV_RecClkSet", status, __LINE__);

   status = WPX_FRMR_DEV_DIAG_DRV_RecClkSet(0, 0, 1 ,0);
   WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_DIAG_DRV_RecClkSet", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 piLinePort;
      status = WPX_FRMR_DEV_DIAG_DRV_RecClkGet(flexmux_id,0,&piLinePort );
      WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
      if(piLinePort!=0)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_DEV_DIAG_DRV_RecClkGet != 0\n", 0);
      }
#endif

   return 0;
}

WP_S32 WTI_FlexmuxClearChannelConfigureHighOrder(
												WP_U8 flexmux_id,
												WP_U8 flexmux_mode,
												WTI_flexmux_line_port_mode line_port_mode,
												WP_U8 line_increment,
												WP_U8 max_line_ports
												)
{
	WUFE_status status;
	WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE line_end_point = {0};
#if WTI_ADM_MODE
/* in CR when using the ADM for testing */
      U8 TX_C2=0x1, EX_C2=0x1;
      WP_CHAR *TX_J1="0000000000000000", *EX_J1="0000000000000000",*pTX_TraceString="WG RS-TRACE",*pEX_TraceString="WG RS-TRACE";
#else /*  WTI_ADM_MODE*/
      U8 TX_C2=0x2, EX_C2=0x2;
      WP_CHAR *TX_J1="WG HP-TRACE", *EX_J1="WG HP-TRACE",*pTX_TraceString="WG RS-TRACE",*pEX_TraceString="WG RS-TRACE";
#endif/* WTI_ADM_MODE */
	if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
	{
		WP_U32 sts12, sts3;
		if(line_port_mode == WT_FLEXMUX_LINE_PORT_OC3)
		{
			line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS3;
			sts12 = 0;
			line_end_point.u.SONET.sts12 = sts12;
			for (sts3=0; sts3<max_line_ports; sts3+=line_increment)
			{
				WTI_FlexmuxLinePortConfig(0,
						sts3,
						line_port_mode);
				WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode(0, flexmux_id, sts3, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
				status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetTX(0, flexmux_id, sts3, pTX_TraceString);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetTX", status, __LINE__);

				status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetEX(0, flexmux_id, sts3, pEX_TraceString);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetEX", status, __LINE__);

				line_end_point.u.SONET.sts3 = sts3;
				line_end_point.u.SONET.sts1 = 0;
				status = WPX_FRMR_SONET_SDH_CreateFacility(0, flexmux_id, &line_end_point);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC3/4)", status, __LINE__);

				/* set the transmitted and expected C2 values */
				status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX(0, flexmux_id, &line_end_point, TX_C2);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
				U8 pTX_C2;
				status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pTX_C2 );
				WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
				if(pTX_C2!=TX_C2)
				{
					WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != TX_C2\n", 0);
				}
#endif
				status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX(0, flexmux_id, &line_end_point, EX_C2);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
				U8 pEX_C2;
				status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pEX_C2 );
				WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
				if(pTX_C2!=TX_C2)
				{
					WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != EX_C2\n", 0);
				}
#endif
				WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode(0, flexmux_id, &line_end_point, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
				status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX(0, flexmux_id, &line_end_point, TX_J1);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
				WP_CHAR pTX_J1[65];
				status =WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX(flexmux_id, &line_end_point, pTX_J1);
				WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
				if (strncmp(pTX_J1, TX_J1, 11))
				{
					printf("pTX_J1 %s\nTX_J1 %s\n", pTX_J1, TX_J1);
					WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX != TX_J1\n", 0);
				}
#endif

				status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX(0, flexmux_id, &line_end_point, EX_J1);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
				WP_CHAR pEX_J1[65];
				status = WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX(flexmux_id, &line_end_point, pEX_J1);
				WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
				if (strncmp(pEX_J1, EX_J1, 11))
				{
					printf("pEX_J1 %s\nEX_J1 %s\n", pEX_J1, EX_J1);
					WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX != EX_J1\n", 0);
				}
#endif
			}
		}//end of oc3
		else if(line_port_mode == WT_FLEXMUX_LINE_PORT_OC12)
		{
			line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS12;
			for (sts12=0; sts12<max_line_ports; sts12+=line_increment)
			{
				line_end_point.u.SONET.sts12 = sts12;
				WTI_FlexmuxLinePortConfig(0,
						sts12,
						line_port_mode);
				WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode(0, flexmux_id, sts12, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
				status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetTX(0, flexmux_id, sts12, pTX_TraceString);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetTX", status, __LINE__);

				status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetEX(0, flexmux_id, sts12, pEX_TraceString);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetEX", status, __LINE__);
				line_end_point.u.SONET.sts3 = 0;
				line_end_point.u.SONET.sts1 = 0;
				status = WPX_FRMR_SONET_SDH_CreateFacility(0, flexmux_id, &line_end_point);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC2/4)", status, __LINE__);

				/* set the transmitted and expected C2 values */
				status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX(0, flexmux_id, &line_end_point, TX_C2);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
				U8 pTX_C2;
				status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pTX_C2 );
				WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
				if (pTX_C2!=TX_C2)
				{
					WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != TX_C2\n", 0);
				}
#endif
				status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX(0, flexmux_id, &line_end_point, EX_C2);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
				U8 pEX_C2;
				status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pEX_C2 );
				WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
				if (pTX_C2!=TX_C2)
				{
					WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != EX_C2\n", 0);
				}
#endif
				WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode(0, flexmux_id, &line_end_point, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
				status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX(0, flexmux_id, &line_end_point, TX_J1);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
				WP_CHAR pTX_J1[65];
				status = WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX(flexmux_id, &line_end_point, pTX_J1);
				WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
				if (strncmp(pTX_J1, TX_J1, 11))
				{
					printf("pTX_J1 %s\nTX_J1 %s\n", pTX_J1, TX_J1);
					WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX != TX_J1\n", 0);
				}
#endif
				status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX(0, flexmux_id, &line_end_point, EX_J1);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
				WP_CHAR pEX_J1[65];
				status = WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX(flexmux_id, &line_end_point, pEX_J1);
				WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
				if (strncmp(pEX_J1, EX_J1, 11))
				{
					printf("pEX_J1 %s\nEX_J1 %s\n", pEX_J1, EX_J1);
					WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX != EX_J1\n", 0);
				}
#endif
			}
		}
	}
	else if(flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
	{
		WP_U32 stm4, stm1;
		/* for VC3 we need to configure 3 AU-3 containers
           and for VC4 we need to configure only 1 AU-4 container  */
		if (line_port_mode == WT_FLEXMUX_LINE_PORT_STM1)
		{
			stm4 = 0;
			line_end_point.u.SDH.stm4 = stm4;
			line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
			for (stm1=0; stm1<max_line_ports; stm1+=line_increment)
			{
				line_end_point.u.SDH.stm1 = stm1;
				WTI_FlexmuxLinePortConfig(0,stm1,line_port_mode);
				WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode(0, flexmux_id, stm1, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
				status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetTX(0, flexmux_id, stm1, pTX_TraceString);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetTX", status, __LINE__);

				status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetEX(0, flexmux_id, stm1, pEX_TraceString);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetEX", status, __LINE__);

				status = WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetExcessiveTh(510 + stm1, 0, stm1,
						WPX_UFE_FRAMER_THRESHOLD_10E4);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetExcessiveTh", status, __LINE__);

				status = WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetSignalDegradeTh(410 + stm1, 0, stm1,
						WPX_UFE_FRAMER_THRESHOLD_10E5);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetSignalDegradeTh", status, __LINE__);

				status = WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetExcessiveTh(540 + stm1, 0, stm1,
						WPX_UFE_FRAMER_THRESHOLD_10E5);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetExcessiveTh", status, __LINE__);

				status = WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetSigDegradeTh(440 + stm1, 0, stm1,
						WPX_UFE_FRAMER_THRESHOLD_10E6);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetSigDegradeTh", status, __LINE__);

				line_end_point.u.SDH.stm0 = 0;
				status = WPX_FRMR_SONET_SDH_CreateFacility(0, flexmux_id, &line_end_point);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC3/4)", status, __LINE__);

				/* set the transmitted and expected C2 values */
				status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX(0, flexmux_id, &line_end_point, TX_C2);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
				U8 pTX_C2;
				status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pTX_C2 );
				WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
				if(pTX_C2!=TX_C2)
				{
					WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != TX_C2\n", 0);
				}
#endif
				status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX(0, flexmux_id, &line_end_point, EX_C2);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
				U8 pEX_C2;
				status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pEX_C2 );
				WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
				if(pTX_C2!=TX_C2)
				{
					WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != EX_C2\n", 0);
				}
#endif
				WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode(0, flexmux_id, &line_end_point, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
				status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX(0, flexmux_id, &line_end_point, TX_J1);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
				WP_CHAR pTX_J1[65];
				status = WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX(flexmux_id, &line_end_point, pTX_J1);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX", status, __LINE__);
				if (strncmp(pTX_J1, TX_J1, 11))
				{
					printf("pTX_J1 %s\nTX_J1 %s\n", pTX_J1, TX_J1);
					WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX != TX_J1\n", 0);
				}
#endif
				status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX(0, flexmux_id, &line_end_point, EX_J1);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
				WP_CHAR pEX_J1[65];
				status = WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX(flexmux_id, &line_end_point, pEX_J1);
				WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
				if (strncmp(pTX_J1, TX_J1, 11))
				{
					printf("pEX_J1 %s\nEX_J1 %s\n", pEX_J1, EX_J1);
					WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX != EX_J1\n", 0);
				}
#endif
				status = WPX_FRMR_SONET_SDH_HO_PATH_B3_SetExcessiveTh(0,flexmux_id, &line_end_point,
						WPX_UFE_FRAMER_THRESHOLD_10E4);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_B3_SetExcessiveTh", status, __LINE__);

				status = WPX_FRMR_SONET_SDH_HO_PATH_B3_SetSigDegradeTh(0, flexmux_id, &line_end_point,
						WPX_UFE_FRAMER_THRESHOLD_10E6);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_B3_SetSigDegradeTh", status, __LINE__);
			}
		}
		else if (line_port_mode == WT_FLEXMUX_LINE_PORT_STM4)
		{
			for (stm4=0; stm4<max_line_ports; stm4+=line_increment)
			{
				stm1=0;
				line_end_point.u.SDH.stm4 = stm4;
				line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4_4C;
				WTI_FlexmuxLinePortConfig(0,
						stm4,
						line_port_mode);
				WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode(0, flexmux_id, stm4, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
				status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetTX(0, flexmux_id, stm4, pTX_TraceString);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetTX", status, __LINE__);

				status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetEX(0, flexmux_id, stm4, pEX_TraceString);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetEX", status, __LINE__);

				status = WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetExcessiveTh(510 + stm1, flexmux_id, stm4,
						WPX_UFE_FRAMER_THRESHOLD_10E4);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetExcessiveTh", status, __LINE__);

				status = WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetSignalDegradeTh(410 + stm1, flexmux_id, stm4,
						WPX_UFE_FRAMER_THRESHOLD_10E5);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetSignalDegradeTh", status, __LINE__);

				status = WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetExcessiveTh(540 + stm1, flexmux_id, stm4,
						WPX_UFE_FRAMER_THRESHOLD_10E5);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetExcessiveTh", status, __LINE__);

				status = WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetSigDegradeTh(440 + stm1, flexmux_id, stm4,
						WPX_UFE_FRAMER_THRESHOLD_10E6);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetSigDegradeTh", status, __LINE__);
				line_end_point.u.SDH.stm1 = 0;
				line_end_point.u.SDH.stm0 = 0;
				status = WPX_FRMR_SONET_SDH_CreateFacility(0, flexmux_id, &line_end_point);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC3/4)", status, __LINE__);

				/* set the transmitted and expected C2 values */
				status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX(0, flexmux_id, &line_end_point, TX_C2);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
				U8 pTX_C2;
				status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pTX_C2 );
				WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
				if(pTX_C2!=TX_C2)
				{
					WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != TX_C2\n", 0);
				}
#endif
				status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX(0, flexmux_id, &line_end_point, EX_C2);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
				U8 pEX_C2;
				status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pEX_C2 );
				WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
				if(pTX_C2!=TX_C2)
				{
					WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != EX_C2\n", 0);
				}
#endif
				WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode(0, flexmux_id, &line_end_point, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
				status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX(0, flexmux_id, &line_end_point, TX_J1);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
				WP_CHAR pTX_J1[65];
				status = WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX(flexmux_id, &line_end_point, pTX_J1);
				WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
				if (strncmp(pTX_J1, TX_J1, 11))
				{
					printf("pTX_J1 %s\nTX_J1 %s\n", pTX_J1, TX_J1);
					WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX != TX_J1\n", 0);
				}
#endif

				status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX(0, flexmux_id, &line_end_point, EX_J1);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
				WP_CHAR pEX_J1[65];
				status = WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX(flexmux_id, &line_end_point, pEX_J1);
				WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
				if (strncmp(pEX_J1, EX_J1, 11))
				{
					printf("pEX_J1 %s\nEX_J1 %s\n", pEX_J1, EX_J1);
					WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX != EX_J1\n", 0);
				}
#endif
				status = WPX_FRMR_SONET_SDH_HO_PATH_B3_SetExcessiveTh(700, 0, &line_end_point,
						WPX_UFE_FRAMER_THRESHOLD_10E4);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_B3_SetExcessiveTh", status, __LINE__);

				status = WPX_FRMR_SONET_SDH_HO_PATH_B3_SetSigDegradeTh(701, 0, &line_end_point,
						WPX_UFE_FRAMER_THRESHOLD_10E6);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_B3_SetSigDegradeTh", status, __LINE__);
			}
		}
	}
	return 0;
}


// #if WTI_DUAL_EMPHY
// #if (WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES == 1)
// #error "Error - Dual EMPHY support is not enabled in UFE framer\n"
// #endif
// #endif

WP_S32 WTI_FlexmuxLinePortConfig(WP_U8 flexmux_id,
                                 WP_U8 line_port_id,
                                 WTI_flexmux_line_port_mode line_port_cfg)
{
   WP_U8 status =0;

   if (flexmux_id > WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES)
      WTI_FlexmuxTerminateOnError("flexmux_id is out of bounds !", 0);

   if (flexmux_reg[flexmux_id].is_initialized != WP_TRUE)
      WTI_FlexmuxTerminateOnError("Flexmux device is not initialized!", 0);

   if (line_port_id >= WTI_MAX_LINE_PORTS_PER_FLEXMUX_DEVICE)
      WTI_FlexmuxTerminateOnError("Invalid line_port_id!", 0);

   if ((flexmux_reg[flexmux_id].flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET) &&
       (line_port_cfg != WT_FLEXMUX_LINE_PORT_OC3 && line_port_cfg != WT_FLEXMUX_LINE_PORT_OC12))
      WTI_FlexmuxTerminateOnError("Invalid line_port_cfg!", 0);

   if ((flexmux_reg[flexmux_id].flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH) &&
       (line_port_cfg != WT_FLEXMUX_LINE_PORT_STM1 && line_port_cfg != WT_FLEXMUX_LINE_PORT_STM4))
      WTI_FlexmuxTerminateOnError("Invalid line_port_cfg!", 0);

#undef WTI_FLEXMUX_DBG_MODE
#define WTI_FLEXMUX_DBG_MODE 1
   if (line_port_cfg == WT_FLEXMUX_LINE_PORT_OC3)
   {
      status = WPX_FRMR_SONET_SDH_PORT_SetRate(0, flexmux_id, line_port_id, WPX_UFE_FRAMER_LINE_PORT_RATE_OC3);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_SetRate", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
      {
         U8 pPortRate;
         status =WPX_FRMR_SONET_SDH_PORT_GetRate(flexmux_id,line_port_id,&pPortRate );
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_GetRate", status, __LINE__);
         if(pPortRate!=WPX_UFE_FRAMER_LINE_PORT_RATE_OC3)
         {
            WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_PORT_GetRate != WPX_UFE_FRAMER_LINE_PORT_RATE_STS3C\n", 0);
         }
      }
#endif
   }
   else if (line_port_cfg == WT_FLEXMUX_LINE_PORT_OC12)
   {
      status = WPX_FRMR_SONET_SDH_PORT_SetRate(0, flexmux_id, line_port_id, WPX_UFE_FRAMER_LINE_PORT_RATE_OC12);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_SetRate", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
      U8 pPortRate;
      status =WPX_FRMR_SONET_SDH_PORT_GetRate(flexmux_id,line_port_id,&pPortRate );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_GetRate", status, __LINE__);
      if(pPortRate!=WPX_UFE_FRAMER_LINE_PORT_RATE_OC12)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_PORT_GetRate != WPX_UFE_FRAMER_LINE_PORT_RATE_STS12C\n", 0);
      }
#endif

   }
   else if (line_port_cfg == WT_FLEXMUX_LINE_PORT_STM1)
   {
      status = WPX_FRMR_SONET_SDH_PORT_SetRate(0, flexmux_id, line_port_id, WPX_UFE_FRAMER_LINE_PORT_RATE_STM1);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_SetRate", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
      U8 pPortRate;
      status =WPX_FRMR_SONET_SDH_PORT_GetRate(flexmux_id,line_port_id,&pPortRate );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_GetRate", status, __LINE__);
      if(pPortRate!=WPX_UFE_FRAMER_LINE_PORT_RATE_STM1)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_PORT_GetRate != WPX_UFE_FRAMER_LINE_PORT_RATE_STM1\n", 0);
      }
#endif
   }
   else if (line_port_cfg == WT_FLEXMUX_LINE_PORT_STM4)
   {
      status = WPX_FRMR_SONET_SDH_PORT_SetRate(0, flexmux_id, line_port_id, WPX_UFE_FRAMER_LINE_PORT_RATE_STM4);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_SetRate", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
      U8 pPortRate;
      status =WPX_FRMR_SONET_SDH_PORT_GetRate(flexmux_id,line_port_id,&pPortRate );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_GetRate", status, __LINE__);
      if(pPortRate!=WPX_UFE_FRAMER_LINE_PORT_RATE_STM4)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_PORT_GetRate != WPX_UFE_FRAMER_LINE_PORT_RATE_STM4\n", 0);
      }
#endif
   }
#undef WTI_FLEXMUX_DBG_MODE
#define WTI_FLEXMUX_DBG_MODE 0

   /* save line port rate in the registry */
   flexmux_reg[flexmux_id].line_ports_reg[line_port_id].line_port_mode = line_port_cfg;

   return 0;
}

WP_S32 WTI_FlexmuxDccEnable(WP_U8 flexmux_id,
                            WP_U8 line_port_id,
                            WP_U32 dcc_mode)
{
   WP_U8 status =0;

   /* This function Enables a DCC channel on a port */
   if (flexmux_id > WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES)
      WTI_FlexmuxTerminateOnError("flexmux_id is out of bounds !", 0);

   /* Enable the DCC*/
   status = WPX_FRMR_SONET_SDH_PORT_DCC_Enable(0 ,flexmux_id, line_port_id, dcc_mode);
   WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_DCC_Enable", status, __LINE__);

   return 0;
}

WP_S32 WTI_FlexmuxDccDisable(WP_U8 flexmux_id,
                             WP_U8 line_port_id,
                             WP_U32 dcc_mode)
{
   WP_U8 status =0;

   /* This function Disables a DCC channel on a port */
   if (flexmux_id > WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES)
      WTI_FlexmuxTerminateOnError("flexmux_id is out of bounds !", 0);

   /* Disable the DCC*/
   status = WPX_FRMR_SONET_SDH_PORT_DCC_Disable(0 ,flexmux_id, line_port_id, dcc_mode);
   WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_DCC_Disable", status, __LINE__);

   return 0;
}

WP_S32 WTI_FlexmuxConnectionCreate(WP_U8 flexmux_id,
                                   WP_U8 line_port_id,
                                   WP_U32 client_port_id,
                                   WTI_flexmux_connection_type type,
                                   void *ufe_line_params)
{
   /* This function creates the client and line port facilities and connects them */
   WP_U8 status =0,tx_clk_rec_enable=0;
   WP_U32 line_timing=0;
   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE line_end_point;

   if (flexmux_id > WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES)
      WTI_FlexmuxTerminateOnError("flexmux_id is out of bounds !", 0);

   /* create client port facility */
   if (type == WT_FLEXMUX_CONNECTION_TYPE_E1_UNFRAMED)
   {
      status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_E1);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);

      status = WPX_FRMR_SOCKET_PDH_SetFraming(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_UNFRAMED);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
      U8 pFraming;
      status =WPX_FRMR_SOCKET_PDH_GetFraming(flexmux_id,client_port_id,&pFraming );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetFraming", status, __LINE__);
      if(pFraming!=WPX_UFE_FRAMER_CLIENT_UNFRAMED)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_UNFRAMED\n", 0);
      }
#endif
      status = WPX_FRMR_SOCKET_PDH_SetMapping(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetMapping", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
      U8 pMapping;
      status =WPX_FRMR_SOCKET_PDH_GetMapping(flexmux_id,client_port_id,&pMapping );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetMapping", status, __LINE__);
      if(pMapping!=WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC\n", 0);
      }
#endif

   }
   else if (type == WT_FLEXMUX_CONNECTION_TYPE_E1_FRAMED)
   {
      status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_E1);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);

      status = WPX_FRMR_SOCKET_PDH_SetFraming(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_PCM31CRC);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 pFraming;
      status =WPX_FRMR_SOCKET_PDH_GetFraming(flexmux_id,client_port_id,&pFraming );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetFraming", status, __LINE__);
      if(pFraming!=WPX_UFE_FRAMER_CLIENT_PCM31CRC)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_PCM31CRC\n", 0);
      }
#endif

      status = WPX_FRMR_SOCKET_PDH_SetMapping(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetMapping", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 pMapping;
      status =WPX_FRMR_SOCKET_PDH_GetMapping(flexmux_id,client_port_id,&pMapping );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetMapping", status, __LINE__);
      if(pMapping!=WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC\n", 0);
      }
#endif

      status = WPX_FRMR_SOCKET_PDH_DisableCAS(0, flexmux_id, client_port_id);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DisableCAS", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 pCAS_IsEnabled;
      status =WPX_FRMR_SOCKET_PDH_Get_CAS_State(flexmux_id,client_port_id,&pCAS_IsEnabled );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_Get_CAS_State", status, __LINE__);
      if(pCAS_IsEnabled!=0)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_Get_CAS_State != 0\n", 0);
      }
#endif

   }
   else if (type == WT_FLEXMUX_CONNECTION_TYPE_E1_FRAMED_CAS)
   {
      status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_E1);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);

      status = WPX_FRMR_SOCKET_PDH_SetFraming(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_PCM31CRC);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 pFraming;
      status =WPX_FRMR_SOCKET_PDH_GetFraming(flexmux_id,client_port_id,&pFraming );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetFraming", status, __LINE__);
      if(pFraming!=WPX_UFE_FRAMER_CLIENT_PCM31CRC)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_PCM31CRC\n", 0);
      }
#endif

      status = WPX_FRMR_SOCKET_PDH_SetMapping(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetMapping", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 pMapping;
      status =WPX_FRMR_SOCKET_PDH_GetMapping(flexmux_id,client_port_id,&pMapping );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetMapping", status, __LINE__);
      if(pMapping!=WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC\n", 0);
      }
#endif


      status = WPX_FRMR_SOCKET_PDH_EnableCAS(0, flexmux_id, client_port_id);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DisableCAS", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 pCAS_IsEnabled;
      status =WPX_FRMR_SOCKET_PDH_Get_CAS_State(flexmux_id,client_port_id,&pCAS_IsEnabled );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_Get_CAS_State", status, __LINE__);
      if(pCAS_IsEnabled!=1)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_Get_CAS_State != 1\n", 0);
      }
#endif

   }
   else if (type == WT_FLEXMUX_CONNECTION_TYPE_T1_UNFRAMED)
   {
      status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_T1);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);

      status = WPX_FRMR_SOCKET_PDH_SetFraming(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_UNFRAMED);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 pFraming;
      status =WPX_FRMR_SOCKET_PDH_GetFraming(flexmux_id,client_port_id,&pFraming );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetFraming", status, __LINE__);
      if(pFraming!=WPX_UFE_FRAMER_CLIENT_UNFRAMED)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_UNFRAMED\n", 0);
      }
#endif

      status = WPX_FRMR_SOCKET_PDH_SetMapping(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetMapping", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 pMapping;
      status =WPX_FRMR_SOCKET_PDH_GetMapping(flexmux_id,client_port_id,&pMapping );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetMapping", status, __LINE__);
      if(pMapping!=WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC\n", 0);
      }
#endif

   }
   else if (type == WT_FLEXMUX_CONNECTION_TYPE_T1_FRAMED)
   {
      status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_T1);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);

      status = WPX_FRMR_SOCKET_PDH_SetFraming(0, flexmux_id, client_port_id,WPX_UFE_FRAMER_CLIENT_FRAMING_ESF/*WPX_UFE_FRAMER_CLIENT_FRAMING_SF_D4*/);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 pFraming;
      status =WPX_FRMR_SOCKET_PDH_GetFraming(flexmux_id,client_port_id,&pFraming );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetFraming", status, __LINE__);
      if(pFraming!=WPX_UFE_FRAMER_CLIENT_FRAMING_ESF)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_FRAMING_ESF\n", 0);
      }
#endif

      status = WPX_FRMR_SOCKET_PDH_SetMapping(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetMapping", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 pMapping;
      status =WPX_FRMR_SOCKET_PDH_GetMapping(flexmux_id,client_port_id,&pMapping );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetMapping", status, __LINE__);
      if(pMapping!=WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC\n", 0);
      }
#endif

      status = WPX_FRMR_SOCKET_PDH_DisableCAS(0, flexmux_id, client_port_id);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DisableCAS", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 pCAS_IsEnabled;
      status =WPX_FRMR_SOCKET_PDH_Get_CAS_State(flexmux_id,client_port_id,&pCAS_IsEnabled );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_Get_CAS_State", status, __LINE__);
      if(pCAS_IsEnabled!=0)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_Get_CAS_State != 0\n", 0);
      }
#endif
   }
   else if (type == WT_FLEXMUX_CONNECTION_TYPE_T1_FRAMED_CAS)
   {
      status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_T1);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);

      status = WPX_FRMR_SOCKET_PDH_SetFraming(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_FRAMING_ESF);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 pFraming;
      status =WPX_FRMR_SOCKET_PDH_GetFraming(flexmux_id,client_port_id,&pFraming );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetFraming", status, __LINE__);
      if(pFraming!=WPX_UFE_FRAMER_CLIENT_FRAMING_ESF)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_FRAMING_ESF\n", 0);
      }
#endif

      status = WPX_FRMR_SOCKET_PDH_SetMapping(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetMapping", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 pMapping;
      status =WPX_FRMR_SOCKET_PDH_GetMapping(flexmux_id,client_port_id,&pMapping );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetMapping", status, __LINE__);
      if(pMapping!=WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC\n", 0);
      }
#endif

      status = WPX_FRMR_SOCKET_PDH_EnableCAS(0, flexmux_id, client_port_id);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_EnableCAS", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 pCAS_IsEnabled;
      status =WPX_FRMR_SOCKET_PDH_Get_CAS_State(flexmux_id,client_port_id,&pCAS_IsEnabled );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_Get_CAS_State", status, __LINE__);
      if(pCAS_IsEnabled!=1)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_Get_CAS_State != 1\n", 0);
      }
#endif
   }
   else if(type == WT_FLEXMUX_CONNECTION_TYPE_STS3)
   {
	   WT_ufe_line_sonet_params *sonet_line_params = (WT_ufe_line_sonet_params*) ufe_line_params;
	   line_end_point.u.SONET.sts12 = sonet_line_params->sts12;
	   line_end_point.u.SONET.sts3 = sonet_line_params->sts3;
	   line_end_point.u.SONET.sts1 = sonet_line_params->sts1;
	   line_end_point.u.SONET.vt_group = sonet_line_params->vt_group;
	   line_end_point.u.SONET.vt = sonet_line_params->vt;
	   line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS3;
	   status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_C4);
	   WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);
	   status = WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate(0, flexmux_id, &line_end_point, client_port_id);
	   WTI_FlexmuxCheckStatus("WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate", status, __LINE__);
	   return 0;
//	   status = WPX_FRMR_SOCKET_PDH_SetFraming(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_UNFRAMED);
//	   WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);
//#if WTI_FLEXMUX_DBG_MODE
//	   U8 pFraming;
//	   status =WPX_FRMR_SOCKET_PDH_GetFraming(flexmux_id,client_port_id,&pFraming );
//	   WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetFraming", status, __LINE__);
//	   if(pFraming!=WPX_UFE_FRAMER_CLIENT_UNFRAMED)
//	   {
//		   WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_UNFRAMED\n", 0);
//	   }
//#endif
//	   status = WPX_FRMR_SOCKET_PDH_SetMapping(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
//	   WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetMapping", status, __LINE__);
//#if WTI_FLEXMUX_DBG_MODE
//	   U8 pMapping;
//	   status =WPX_FRMR_SOCKET_PDH_GetMapping(flexmux_id,client_port_id,&pMapping );
//	   WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetMapping", status, __LINE__);
//	   if(pMapping!=WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC)
//	   {
//		   WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC\n", 0);
//	   }
//#endif
   }
   else if(type == WT_FLEXMUX_CONNECTION_TYPE_STS12)
   {
	   WT_ufe_line_sonet_params *sonet_line_params = (WT_ufe_line_sonet_params*) ufe_line_params;
	   line_end_point.u.SONET.sts12 = sonet_line_params->sts12;
	   line_end_point.u.SONET.sts3 = sonet_line_params->sts3;
	   line_end_point.u.SONET.sts1 = sonet_line_params->sts1;
	   line_end_point.u.SONET.vt_group = sonet_line_params->vt_group;
	   line_end_point.u.SONET.vt = sonet_line_params->vt;
	   line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS12;
	   status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_C4_4C);
	   WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);
	   status = WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate(0, flexmux_id, &line_end_point, client_port_id);
	   WTI_FlexmuxCheckStatus("WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate", status, __LINE__);
	   return 0;
   }
   else if(type == WT_FLEXMUX_CONNECTION_TYPE_VC4)
   {
	   WT_ufe_line_sdh_params *sdh_line_params = (WT_ufe_line_sdh_params*) ufe_line_params;
	   line_end_point.u.SDH.stm4 = sdh_line_params->stm4;
	   line_end_point.u.SDH.stm1 = sdh_line_params->stm1;
	   line_end_point.u.SDH.stm0 = sdh_line_params->stm0;
	   line_end_point.u.SDH.tug2 = sdh_line_params->tug2;
	   line_end_point.u.SDH.tu = sdh_line_params->tu;
	   line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
	   status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_C4);
	   WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);
	   status = WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate(0, flexmux_id, &line_end_point, client_port_id);
	   WTI_FlexmuxCheckStatus("WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate", status, __LINE__);
	   return 0;
   }
   else if(type == WT_FLEXMUX_CONNECTION_TYPE_VC4_4C)
   {
	   WT_ufe_line_sdh_params *sdh_line_params = (WT_ufe_line_sdh_params*) ufe_line_params;
	   line_end_point.u.SDH.stm4 = sdh_line_params->stm4;
	   line_end_point.u.SDH.stm1 = sdh_line_params->stm1;
	   line_end_point.u.SDH.stm0 = sdh_line_params->stm0;
	   line_end_point.u.SDH.tug2 = sdh_line_params->tug2;
	   line_end_point.u.SDH.tu = sdh_line_params->tu;
	   line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4_4C;
	   status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_C4_4C);
	   WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);
	   status = WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate(0, flexmux_id, &line_end_point, client_port_id);
	   WTI_FlexmuxCheckStatus("WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate", status, __LINE__);
	   return 0;
   }
   else if (type == WT_FLEXMUX_CONNECTION_TYPE_E3_UNFRAMED)
   {
      status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_E3);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);

      status = WPX_FRMR_SOCKET_PDH_SetFraming(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_UNFRAMED);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 pFraming;
      status =WPX_FRMR_SOCKET_PDH_GetFraming(flexmux_id,client_port_id,&pFraming );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetFraming", status, __LINE__);
      if(pFraming!=WPX_UFE_FRAMER_CLIENT_UNFRAMED)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_UNFRAMED\n", 0);
      }
#endif
   }
   else if (type == WT_FLEXMUX_CONNECTION_TYPE_E3_FRAMED_G751)
   {
      status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_E3);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);

      status = WPX_FRMR_SOCKET_PDH_SetFraming(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_FRAMING_E3_G751);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 pFraming;
      status =WPX_FRMR_SOCKET_PDH_GetFraming(flexmux_id,client_port_id,&pFraming );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetFraming", status, __LINE__);
      if(pFraming!=WPX_UFE_FRAMER_CLIENT_FRAMING_G751)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_FRAMING_G751\n", 0);
      }
#endif
   }
   else if (type == WT_FLEXMUX_CONNECTION_TYPE_E3_FRAMED_G751_ATM)
   {
      status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_E3);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);

      status = WPX_FRMR_SOCKET_PDH_SetFraming(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_FRAMING_E3_G751_ATM);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 pFraming;
      status =WPX_FRMR_SOCKET_PDH_GetFraming(flexmux_id,client_port_id,&pFraming );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetFraming", status, __LINE__);
      if(pFraming!=WPX_UFE_FRAMER_CLIENT_FRAMING_G751_ATM)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_FRAMING_G751_ATM\n", 0);
      }
#endif
   }
   else if (type == WT_FLEXMUX_CONNECTION_TYPE_DS3_UNFRAMED)
   {
     
      status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_T3);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);

      status = WPX_FRMR_SOCKET_PDH_SetFraming(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_UNFRAMED);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 pFraming;
      status =WPX_FRMR_SOCKET_PDH_GetFraming(flexmux_id,client_port_id,&pFraming );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetFraming", status, __LINE__);
      if(pFraming!=WPX_UFE_FRAMER_CLIENT_UNFRAMED)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_UNFRAMED\n", 0);
      }
#endif
   }
   else if (type == WT_FLEXMUX_CONNECTION_TYPE_DS3_FRAMED_M23)
   {
      status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_T3);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);

      status = WPX_FRMR_SOCKET_PDH_SetFraming(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_M23);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 pFraming;
      status =WPX_FRMR_SOCKET_PDH_GetFraming(flexmux_id,client_port_id,&pFraming );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetFraming", status, __LINE__);
      if(pFraming!=WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_M23)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_M23\n", 0);
      }
#endif
   }
   else if (type == WT_FLEXMUX_CONNECTION_TYPE_DS3_FRAMED_CPARITY)
   {
      status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_T3);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);

      status = WPX_FRMR_SOCKET_PDH_SetFraming(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_CPARITY);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
      U8 pFraming;
      status =WPX_FRMR_SOCKET_PDH_GetFraming(flexmux_id,client_port_id,&pFraming );
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetFraming", status, __LINE__);
      if(pFraming!=WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_CPARITY)
      {
         WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_CPARITY\n", 0);
      }
#endif
   }
   else
   {
      WTI_FlexmuxTerminateOnError("Connection type is not supported !", 0);
   }

   /* configure the line end point in SONET or SDH mode */
   if (flexmux_reg[flexmux_id].flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
   {
      WT_ufe_line_sonet_params *sonet_line_params = (WT_ufe_line_sonet_params*) ufe_line_params;
      U8 TX_V5_SignalLabel=0x2, EX_V5_SignalLabel=0x2;
      WP_CHAR *TX_J2_TraceString = "WG LP-TRACE",*EX_J2_TraceString = "WG LP-TRACE";

      line_end_point.u.SONET.sts12 = sonet_line_params->sts12;
      line_end_point.u.SONET.sts3 = sonet_line_params->sts3;
      line_end_point.u.SONET.sts1 = sonet_line_params->sts1;
      line_end_point.u.SONET.vt_group = sonet_line_params->vt_group;
      line_end_point.u.SONET.vt = sonet_line_params->vt;

#if WTI_FLEXMUX_DBG_MODE
      printf("connecting client_port_id %d to line_port_id %d\n",client_port_id,line_port_id);
      printf("sts12 %d sts3 %d sts1 %d vt_group %d vt %d\n", sonet_line_params->sts12,
                                                             sonet_line_params->sts3,
                                                             sonet_line_params->sts1,
                                                             sonet_line_params->vt_group,
                                                             sonet_line_params->vt);
#endif

      if (sonet_line_params->transfer_type == WUFE_SONET_TYPE_E1)
         line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT2;
      else if (sonet_line_params->transfer_type == WUFE_SONET_TYPE_T1)
         line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT1_5;
      else if (sonet_line_params->transfer_type == WUFE_SONET_TYPE_E3 ||
               sonet_line_params->transfer_type == WUFE_SONET_TYPE_DS3)
         line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS1_SPE;
      else
         WTI_FlexmuxTerminateOnError("Invalid line transfer type", 0); 


      if (sonet_line_params->transfer_type == WUFE_SONET_TYPE_E1 ||
          sonet_line_params->transfer_type == WUFE_SONET_TYPE_T1)
      {
         status = WPX_FRMR_SONET_SDH_CreateFacility(0, flexmux_id, &line_end_point);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (Vt1.5 or Vt2)", status, __LINE__);

         /* set the transmitted and expected V5 values */
         status = WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetTX(0, flexmux_id, &line_end_point, TX_V5_SignalLabel);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetTX", status, __LINE__);
         
#if WTI_FLEXMUX_DBG_MODE
         U8 pTX_V5_SignalLabel;
         status =WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetTX(flexmux_id,&line_end_point,&pTX_V5_SignalLabel );
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetTX", status, __LINE__);
         if(pTX_V5_SignalLabel!=TX_V5_SignalLabel)
         {
            WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetTX != TX_V5_SignalLabel\n", 0);
         }
#endif
         
         status = WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetEX(0, flexmux_id, &line_end_point, EX_V5_SignalLabel);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetEX", status, __LINE__);
         
#if WTI_FLEXMUX_DBG_MODE
         U8 pEX_V5_SignalLabel;
         status =WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetEX(flexmux_id,&line_end_point,&pEX_V5_SignalLabel );
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetEX", status, __LINE__);
         if(pEX_V5_SignalLabel!=EX_V5_SignalLabel)
         {
            WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetEX != EX_V5_SignalLabel\n", 0);
         }
#endif
         
         
         status = WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX(0, flexmux_id, &line_end_point, TX_J2_TraceString);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX", status, __LINE__);
         
#if WTI_FLEXMUX_DBG_MODE
         WP_CHAR *pTX_J2_TraceString;
         status =WPX_FRMR_SONET_SDH_LO_PATH_J2_GetTX(flexmux_id,&line_end_point,pTX_J2_TraceString );
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX", status, __LINE__);
         if(*pTX_J2_TraceString!=*TX_J2_TraceString)
         {
            WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_LO_PATH_J2_GetTX != TX_J2_TraceString\n", 0);
         }
#endif
         
         status = WPX_FRMR_SONET_SDH_LO_PATH_J2_SetEX(0, flexmux_id, &line_end_point, EX_J2_TraceString);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetEX", status, __LINE__);
         
#if WTI_FLEXMUX_DBG_MODE
         WP_CHAR *pEX_J2_TraceString;
         status =WPX_FRMR_SONET_SDH_LO_PATH_J2_GetEX(flexmux_id,&line_end_point,pEX_J2_TraceString );
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_J2_GetEX", status, __LINE__);
         if(*pEX_J2_TraceString!=*EX_J2_TraceString)
         {
            WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_LO_PATH_J2_GetEX != EX_J2_TraceString\n", 0);
         }
#endif
      }
      
      if (sonet_line_params->clock_rec_line_params != NULL)
      {
         tx_clk_rec_enable = sonet_line_params->clock_rec_line_params->tx_clk_rec_enable;
      }
   }
   else if (flexmux_reg[flexmux_id].flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
   {
      WT_ufe_line_sdh_params *sdh_line_params = (WT_ufe_line_sdh_params*) ufe_line_params;
      U8 TX_V5_SignalLabel=0x2, EX_V5_SignalLabel=0x2;
      WP_CHAR *TX_J2_TraceString = "WG LP-TRACE",*EX_J2_TraceString = "WG LP-TRACE";

      /* configure the facility */
      line_end_point.u.SDH.stm4 = sdh_line_params->stm4;
      line_end_point.u.SDH.stm1 = sdh_line_params->stm1;
      line_end_point.u.SDH.stm0 = sdh_line_params->stm0;
      line_end_point.u.SDH.tug2 = sdh_line_params->tug2;
      line_end_point.u.SDH.tu = sdh_line_params->tu;

#if WTI_FLEXMUX_DBG_MODE
      printf("connecting client_port_id %d to line_port_id %d\n",client_port_id,line_port_id);
      printf("stm4 %d stm1 %d stm0 %d tug2 %d tu %d\n", sdh_line_params->stm4,sdh_line_params->stm1,sdh_line_params->stm0,sdh_line_params->tug2,sdh_line_params->tu);
#endif
      if (sdh_line_params->transfer_type == WUFE_SDH_TYPE_E1)
      {
         line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC12;
      }

      else if (sdh_line_params->transfer_type == WUFE_SDH_TYPE_T1)
      {
         line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC11;
      }
      else if (sdh_line_params->transfer_type == WUFE_SDH_TYPE_E3 ||
               sdh_line_params->transfer_type == WUFE_SDH_TYPE_DS3)
      {
         if (flexmux_reg[flexmux_id].vc_3_4 == WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3)
         {
            line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         }
         else /*AU4 - need to configure TU3 facility*/
         {
            line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_TU3;
            /* create the line port VC3 facility */
            status = WPX_FRMR_SONET_SDH_CreateFacility(0, flexmux_id, &line_end_point);
            WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC3)", status, __LINE__);

            line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
            status = WPX_FRMR_SONET_SDH_HO_PATH_SS_BITS_Set_Tx(0, flexmux_id, &line_end_point, 0x2);
            WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC3)", status, __LINE__);

            line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_TU3;
         }
      }
      else
      {         
         WTI_FlexmuxTerminateOnError("Invalid line transfer type", 0);
      }
      
#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING
      lo_transfer_type_in_use = line_end_point.TransferType;
#endif

      if (sdh_line_params->clock_rec_line_params != NULL)
      {
         tx_clk_rec_enable = sdh_line_params->clock_rec_line_params->tx_clk_rec_enable;
      }

      if (sdh_line_params->transfer_type == WUFE_SDH_TYPE_E1 ||
          sdh_line_params->transfer_type == WUFE_SDH_TYPE_T1)
      {
         /* create the line port VC11\VC12 facility */
         status = WPX_FRMR_SONET_SDH_CreateFacility(0, flexmux_id, &line_end_point);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC11 or VC12)", status, __LINE__);
        
         /* set the transmitted and expected V5 values */
         status = WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetTX(0, flexmux_id, &line_end_point, TX_V5_SignalLabel);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetTX", status, __LINE__);
         
#if WTI_FLEXMUX_DBG_MODE
         U8 pTX_V5_SignalLabel;
         status =WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetTX(flexmux_id,&line_end_point,&pTX_V5_SignalLabel );
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetTX", status, __LINE__);
         if(pTX_V5_SignalLabel!=TX_V5_SignalLabel)
         {
            WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetTX != TX_V5_SignalLabel\n", 0);
         }
#endif
         
         status = WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetEX(0, flexmux_id, &line_end_point, EX_V5_SignalLabel);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetEX", status, __LINE__);
         
#if WTI_FLEXMUX_DBG_MODE
         U8 pEX_V5_SignalLabel;
         status =WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetEX(flexmux_id,&line_end_point,&pEX_V5_SignalLabel );
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetEX", status, __LINE__);
         if(pEX_V5_SignalLabel!=EX_V5_SignalLabel)
         {
            WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetEX != EX_V5_SignalLabel\n", 0);
         }
#endif
         status = WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX(0, flexmux_id, &line_end_point, TX_J2_TraceString);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX", status, __LINE__);
         
#if WTI_FLEXMUX_DBG_MODE
         WP_CHAR *pTX_J2_TraceString;
         status =WPX_FRMR_SONET_SDH_LO_PATH_J2_GetTX(flexmux_id,&line_end_point,pTX_J2_TraceString );
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX", status, __LINE__);
         if(*pTX_J2_TraceString!=*TX_J2_TraceString)
         {
            WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_LO_PATH_J2_GetTX != TX_J2_TraceString\n", 0);
         }
#endif
         
         status = WPX_FRMR_SONET_SDH_LO_PATH_J2_SetEX(0, flexmux_id, &line_end_point, EX_J2_TraceString);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetEX", status, __LINE__);
         
#if WTI_FLEXMUX_DBG_MODE
         WP_CHAR *pEX_J2_TraceString;
         status =WPX_FRMR_SONET_SDH_LO_PATH_J2_GetEX(flexmux_id,&line_end_point,pEX_J2_TraceString );
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_J2_GetEX", status, __LINE__);
         if(*pEX_J2_TraceString!=*EX_J2_TraceString)
         {
            WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_LO_PATH_J2_GetEX != EX_J2_TraceString\n", 0);
         }
#endif
      }
   }
   
   /* create the connection between the client port and the line port */
   status = WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate(0, flexmux_id, &line_end_point, client_port_id);
   WTI_FlexmuxCheckStatus("WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate", status, __LINE__);
   
   /* Set the line timing mode */
   /* The PDH timing mode can be one of the follwoing :
      WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH  - PDH looptiming
      WPX_UFE_FRAMER_TRANSMIT_TIMING_INPUT_REFERENCE_CLOCK - Common transmit (CT) clock
      WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING - System timing from the SONET/SDH Tx clock
      WPX_UFE_FRAMER_TRANSMIT_TIMING_WINPATH - ACR/DCR - Winpath control the PDH timing
   */
   if (tx_clk_rec_enable)
   {

#if(WTI_CESOP_REGRESSION_TEST)
      /* When regression mode is set the framer is configured to SONET timing in order to make sure the PDH Tx clock stable */
      /* All other configuration in the UFE and WP are set to work with CR                                                  */
      line_timing = WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING;
#if WTI_FLEXMUX_DBG_MODE
      printf("WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING\n");
#endif
#else /* NOT WTI_CESOP_REGRESSION_TEST */
      /* Framer uses justification commands from Winpath via UFE */
      line_timing = WPX_UFE_FRAMER_TRANSMIT_TIMING_WINPATH;
#endif

#if WTI_FLEXMUX_DBG_MODE
      printf("WPX_UFE_FRAMER_TRANSMIT_TIMING_WINPATH\n");
#endif
   }
   else
   {
      /* No clock recovery */
#if(WTI_CESOP_REGRESSION_TEST)
      /* Working in OC12/48 loopback the timing should be CT or SDH/SONET clock */
      line_timing = WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING;
#if WTI_FLEXMUX_DBG_MODE
      printf("WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING\n");
#endif

#else /* NOT WTI_CESOP_REGRESSION_TEST */

#if WTI_FLEXMUX_LINE_TIMING_CT_MODE_ENABLE
      /* CR is not enabled and working with Common Transmit Clock */
      line_timing = WPX_UFE_FRAMER_TRANSMIT_TIMING_INPUT_REFERENCE_CLOCK;
#else /* NOT WTI_FLEXMUX_LINE_TIMING_CT_MODE_ENABLE */
      /* CR is not enabled and working with TDM external tester - use PDH loop timing */
      line_timing =  WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH;    /* Loop timing */
#endif /* WTI_FLEXMUX_LINE_TIMING_CT_MODE_ENABLE */

#if WTI_FLEXMUX_DBG_MODE
      printf("WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH\n");
#endif
#endif /* WTI_CESOP_REGRESSION_TEST */
   }




   /* Clock rec Rx timestamps */
   if (tx_clk_rec_enable)
   {
      status = WPX_FRMR_SOCKET_PDH_EnableTimeStamp(0, flexmux_id, client_port_id,WTI_FLEXMUX_CLOCK_REC_GAIN_SLOW);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_EnableTimeStamp", status, __LINE__);
   }

   /* Set the PDH Tx line timing  */
   status = WPX_FRMR_SOCKET_PDH_SetClkRec(0, flexmux_id, client_port_id, line_timing);
   WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetClkRec", status, __LINE__);


   /* Set line or system loopback */
   status = WTI_FlexmuxEnableLoopback(flexmux_id, line_port_id, client_port_id, WT_FLEXMUX_NONE_LOOPBACK);
   WTI_FlexmuxCheckStatus("WTI_FlexmuxEnableLoopback", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
   status = WTI_FlexmuxGetLoopbackState(flexmux_id, line_port_id, client_port_id, WT_FLEXMUX_NONE_LOOPBACK);
   WTI_FlexmuxCheckStatus("WTI_FlexmuxGetLoopbackState", status, __LINE__);
#endif

   return 0;
}



WP_S32 WTI_FlexmuxPDHTxTimingSet(WP_U8 flexmux_id,
                                 WP_U32 client_port_id,
                                 WP_U32 line_timing)
   
   
{
   WP_U8 status =0;
   if(!((line_timing == WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH) ||
        (line_timing == WPX_UFE_FRAMER_TRANSMIT_TIMING_INPUT_REFERENCE_CLOCK)||
        (line_timing == WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING)||
        (line_timing == WPX_UFE_FRAMER_TRANSMIT_TIMING_WINPATH)))

   {
      WTI_FlexmuxTerminateOnError("Invalid line timing ", 0);
   }
   

   /* Set the PDH Tx line timing  */
   status = WPX_FRMR_SOCKET_PDH_SetClkRec(0, flexmux_id, client_port_id, line_timing);
   WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetClkRec", status, __LINE__);
   
   return 0;
}



WP_S32 WTI_FlexmuxConnectionDelete(WP_U8 flexmux_id,
                                   WP_U32 client_port_id,
                                   void *ufe_line_params)
{
/* This function deletes an existing connection and the associated client and line port facilities */
   WP_U8 status =0;
   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE line_end_point = {0};

   if (flexmux_id > WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES)
      WTI_FlexmuxTerminateOnError("flexmux_id is out of bounds !", 0);

   /* configure the line end point in SONET or SDH mode */
   if (flexmux_reg[flexmux_id].flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
   {
      WT_ufe_line_sonet_params *sonet_line_params = (WT_ufe_line_sonet_params*) ufe_line_params;

      if (sonet_line_params->transfer_type == WUFE_SONET_TYPE_E1)
         line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT2;
      else if (sonet_line_params->transfer_type == WUFE_SONET_TYPE_T1)
         line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT1_5;
      else if (sonet_line_params->transfer_type == WUFE_SONET_TYPE_E3 ||
               sonet_line_params->transfer_type == WUFE_SONET_TYPE_DS3)
         line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS1_SPE;
      else
         WTI_FlexmuxTerminateOnError("Invalid line transfer type", 0);

      line_end_point.u.SONET.sts12 = sonet_line_params->sts12;
      line_end_point.u.SONET.sts3 = sonet_line_params->sts3;
      line_end_point.u.SONET.sts1 = sonet_line_params->sts1;
      line_end_point.u.SONET.vt_group = sonet_line_params->vt_group;
      line_end_point.u.SONET.vt = sonet_line_params->vt;
   }
   else if (flexmux_reg[flexmux_id].flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
   {
      WT_ufe_line_sdh_params *sdh_line_params = (WT_ufe_line_sdh_params*) ufe_line_params;

      if (sdh_line_params->transfer_type == WUFE_SDH_TYPE_E1)
         line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC12;
      else if (sdh_line_params->transfer_type == WUFE_SDH_TYPE_T1)
         line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC11;
      else if (sdh_line_params->transfer_type == WUFE_SDH_TYPE_E3 ||
               sdh_line_params->transfer_type == WUFE_SDH_TYPE_DS3)
      {
         if (flexmux_reg[flexmux_id].vc_3_4 == WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3)
         {
            line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         }
         else /*AU4 */
         {
            line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_TU3;
         }
      }
      else
      {         
         WTI_FlexmuxTerminateOnError("Invalid line transfer type", 0);
      }
      
      line_end_point.u.SDH.stm4 = sdh_line_params->stm4;
      line_end_point.u.SDH.stm1 = sdh_line_params->stm1;
      line_end_point.u.SDH.stm0 = sdh_line_params->stm0;
      line_end_point.u.SDH.tug2 = sdh_line_params->tug2;
      line_end_point.u.SDH.tu = sdh_line_params->tu;
   }

   /* delete the connection between the client port and the line port */
   status = WPX_FRMR_CONNECTIONS_AddDropLineToSocketDelete(0, flexmux_id, &line_end_point, client_port_id);
   WTI_FlexmuxCheckStatus("WPX_FRMR_CONNECTIONS_AddDropLineToSocketDelete", status, __LINE__);

   /* delete the line port facility */
   status = WPX_FRMR_SONET_SDH_DeleteFacility(0, flexmux_id, &line_end_point);
   WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_DeleteFacility", status, __LINE__);

   /* delete the client port facility */
   status = WPX_FRMR_SOCKET_PDH_DeleteFacility(0, flexmux_id, client_port_id);
   WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DeleteFacility", status, __LINE__);

   return 0;
}

WP_S32 WTI_FlexmuxClientPortIndexGet(WP_U32 ufe_line_index, WUFE_line_transf_type transfer_type)
{
   switch (transfer_type)
   {
      case (WUFE_SONET_TYPE_E1):
      case (WUFE_SDH_TYPE_E1):
         {
            WP_U32 spe_num = (ufe_line_index / 28);
            WP_U32 tug2_num = ((ufe_line_index % 28) / 3);
            WP_U32 offset_within_tug2 = ((ufe_line_index % 28) % 3);

            /* In E1 mode skip indices 3,7,11.. due to TUG restriction */
            return ((spe_num*28) + (tug2_num*4) + offset_within_tug2);
         }
      case (WUFE_SONET_TYPE_T1):
      case (WUFE_SDH_TYPE_T1):
      case (WUFE_SONET_TYPE_E3):
      case (WUFE_SDH_TYPE_E3):
      case (WUFE_SONET_TYPE_DS3):
      case (WUFE_SDH_TYPE_DS3):
      case (WUFE_SONET_TYPE_STS3):
      case (WUFE_SONET_TYPE_STS12):
      case (WUFE_SDH_TYPE_VC4):
      case (WUFE_SDH_TYPE_VC44C):
         {
            return ufe_line_index;
         }
      default:
         printf("Unsupported transfer_type %d\n", transfer_type);
         exit(-1);
   }
   return 0;
}

WP_S32 WTI_FlexmuxLinePortIndexGet(WP_U32 ufe_line_index, WP_U32 flexmux_id)
{
   WP_U32 result = -1;
   if (flexmux_reg[flexmux_id].flexmux_build_personality != WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12 &&
       flexmux_reg[flexmux_id].flexmux_build_personality != WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3      &&
       flexmux_reg[flexmux_id].flexmux_build_personality != WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12 &&
       flexmux_reg[flexmux_id].flexmux_build_personality != WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3      &&
       flexmux_reg[flexmux_id].flexmux_build_personality != WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3)
      WTI_FlexmuxTerminateOnError("Invalid flexmux_build_personality !", 0);

   if (flexmux_reg[flexmux_id].flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12)
   {
      result = (ufe_line_index / 336);
      if (result > 3)
      {
         result = -1;
      }
      else
      {
         result = result * 2;
      }
   }
   else if (flexmux_reg[flexmux_id].flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3)
   {
      result = (ufe_line_index / 84);

      if (!(((result <= 3 ) && (result >=0 )) || ((result >= 8)&& (result <= 11))))
      {
         result = -1;
      }
      else
      {
          if (result >= 8)
              result = result - 4;

      }
   }
   else if ((flexmux_reg[flexmux_id].flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12) ||
            (flexmux_reg[flexmux_id].flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3)      ||
            (flexmux_reg[flexmux_id].flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3))
   {
      /* In case a single OC12 port is used, then line port index is always 0*/
      if (flexmux_reg[flexmux_id].flexmux_hw_config == WT_FLEXMUX_UFE_412_1_PORT_OC12 ||
    		  flexmux_reg[flexmux_id].flexmux_hw_config ==  WT_FLEXMUX_UFE_412_1_PORT_OC12_CLEAR_CHANNEL)
      {
         result = 0;
      }
      /* In case 4 OC3 ports are used, then line port index changes between 0-3*/
      else if ((flexmux_reg[flexmux_id].flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3) ||
               (flexmux_reg[flexmux_id].flexmux_hw_config == WT_FLEXMUX_UFE_412_2_PORTS_OC3)||
               flexmux_reg[flexmux_id].flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER ||
               flexmux_reg[flexmux_id].flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE  ||
			   flexmux_reg[flexmux_id].flexmux_hw_config ==  WT_FLEXMUX_UFE_412_4_PORTS_OC3_CLEAR_CHANNEL
#if WTI_DUAL_EMPHY
               ||
               flexmux_reg[flexmux_id].flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_UPI1_MASTER_UPI3_SLAVE
#endif
         )
      {
         result = (ufe_line_index / 84);
         if (result > 3)
         {
            result = -1;
         }
      }
   }

   return result;
}

void WTI_FlexmuxDisplayVersionAndConfig(WP_U32 flexmux_id)
{
   WP_U8 status;
   WP_U8 build[128];

   status = WPX_FRMR_DEV_GetBuildPersonality(flexmux_id, build);
   WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetBuildPersonality", status, __LINE__);
   printf("Flexmux Build Personality: %d\n", (WP_U32) build);
}

WP_U8 WTI_FlexmuxEnableLoopback(U8 flexmux_id, WP_U32 line_port_id, WP_U32 client_port_id, WTI_flexmux_loopback_mode loopback_type)
{
   WP_U8 status = WPX_UFE_FRAMER_OK;

   switch (loopback_type)
   {
      case WT_FLEXMUX_NONE_LOOPBACK:
         break;
      case WT_FLEXMUX_SHALLOW_LINE_LOOPBACK:
      {
         status = (WP_U8) WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableShallowLine(0, flexmux_id, line_port_id);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableShallowLine", status, __LINE__);

         break;
      }
      case WT_FLEXMUX_DEEP_LINE_LOOPBACK:
      {
         status = (WP_U8) WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableDeepLine(0, flexmux_id, client_port_id);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableDeepLine", status, __LINE__);

         break;
      }
      case WT_FLEXMUX_SHALLOW_SYSTEM_LOOPBACK:
      {
         status = (WP_U8) WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableShallowSystem(0, flexmux_id, client_port_id);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableShallowSystem", status, __LINE__);

         break;
      }
      case WT_FLEXMUX_DEEP_SYSTEM_LOOPBACK:
      {
         status = (WP_U8) WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableDeepSystem(0, flexmux_id, line_port_id);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableDeepSystem", status, __LINE__);

         break;
      }
      default:
      {
         printf("Unsupported loopback_type %d\n", loopback_type);
         exit(-1);
      }
   }

   return status;
}

WP_U8 WTI_FlexmuxDisableLoopback(U8 flexmux_id, WP_U32 line_port_id, WP_U32 client_port_id, WTI_flexmux_loopback_mode loopback_type)
{
   WP_U8 status = WPX_UFE_FRAMER_OK;

   switch (loopback_type)
   {
      case WT_FLEXMUX_NONE_LOOPBACK:
         break;
      case WT_FLEXMUX_SHALLOW_LINE_LOOPBACK:
      {
         status = (WP_U8) WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableShallowLine(0, flexmux_id, line_port_id);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableShallowLine", status, __LINE__);

         break;
      }
      case WT_FLEXMUX_DEEP_LINE_LOOPBACK:
      {
         status = (WP_U8) WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableDeepLine(0, flexmux_id, client_port_id);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableDeepLine", status, __LINE__);

         break;
      }
      case WT_FLEXMUX_SHALLOW_SYSTEM_LOOPBACK:
      {
         status = (WP_U8) WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableShallowSystem(0,flexmux_id, client_port_id);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableShallowSystem", status, __LINE__);

         break;
      }
      case WT_FLEXMUX_DEEP_SYSTEM_LOOPBACK:
      {
         status = (WP_U8) WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableDeepSystem(0, flexmux_id, line_port_id);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableDeepSystem", status, __LINE__);

         break;
      }
      default:
      {
         printf("Unsupported loopback_type %d\n", loopback_type);
         exit(-1);
      }
   }
   return status;
}

WP_U8 WTI_FlexmuxGetLoopbackState(U8 flexmux_id, WP_U32 line_port_id, WP_U32 client_port_id, WTI_flexmux_loopback_mode loopback_type)
{
   WP_U8 status = WPX_UFE_FRAMER_OK, loopback_enabled = 0;

   switch (loopback_type)
   {
      case WT_FLEXMUX_NONE_LOOPBACK:
         break;
      case WT_FLEXMUX_SHALLOW_LINE_LOOPBACK:
      {
         status = (WP_U8) WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkGetShallowLineState(0, line_port_id, &loopback_enabled );
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkGetShallowLineState", status, __LINE__);
         if (loopback_enabled)
         {
            printf("SHALLOW_LINE");
         }

         break;
      }
      case WT_FLEXMUX_DEEP_LINE_LOOPBACK:
      {
         status = (WP_U8) WPX_FRMR_SOCKET_PDH_DIAG_LpbkGetDeepLineState(0,client_port_id, &loopback_enabled);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_LpbkGetDeepLineState", status, __LINE__);
         if (loopback_enabled)
         {
            printf("DEEP_LINE");
         }

         break;
      }
      case WT_FLEXMUX_SHALLOW_SYSTEM_LOOPBACK:
      {
         status = (WP_U8) WPX_FRMR_SOCKET_PDH_DIAG_LpbkGetShallowSystemState(0,client_port_id,&loopback_enabled);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_LpbkGetShallowSystemState", status, __LINE__);
         if (loopback_enabled)
         {
            printf("SHALLOW_SYSTEM");
         }

         break;
      }
      case WT_FLEXMUX_DEEP_SYSTEM_LOOPBACK:
      {
         status = (WP_U8) WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkGetDeepSystemState(0, line_port_id, &loopback_enabled);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkGetDeepSystemState", status, __LINE__);
         if (loopback_enabled)
         {
            printf("DEEP_SYSTEM");
         }

         break;
      }
      default:
      {
         printf("Unsupported loopback_type %d\n", loopback_type);
         exit(-1);
      }
   }
   if (loopback_enabled)
   {
      printf(" Loopback is enabled\n");
   }
   else
   {
      printf(" Loopback is not enabled\n");
   }

   return status;
}

WP_U8 WTI_FlexmuxCheckPrbsResult(U8 flexmux_id, WP_U32 client_port_id, WUFE_un_framed framing_mode)
{
   WP_U8 prbs_state = WPX_UFE_FRAMER_PRBS_STATE_IDLE;
   WP_U8 ufe_framer_status;
   WP_U8 status = WPX_UFE_FRAMER_OK;

   /* enable the Egress PRBS generator for the client port*/
   status = WPX_FRMR_SOCKET_PDH_DIAG_EgressGenPrbs_Set(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_PRBS_23);
   WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_EgressGenPrbs_Set", status, __LINE__);


   if(framing_mode == WUFE_UNFRAMED)
   {
      /* enable the Egress PRBS detector for the client port*/
      status = WPX_FRMR_SOCKET_PDH_DIAG_EgressTermPrbsSet(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_PRBS_23);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_EgressTermPrbsSet", status, __LINE__);
   }
   else
   {
      /* enable the Ingress PRBS detector for the client port*/
      status = WPX_FRMR_SOCKET_PDH_DIAG_IngressTermPrbsSet(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_PRBS_23);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_IngressTermPrbsSet", status, __LINE__);
   }

   WPL_Delay(2000000);

   if(framing_mode == WUFE_UNFRAMED)
   {
      ufe_framer_status = WPX_FRMR_STATUS_SOCKET_PDH_DIAG_EgressPrbsGetState(flexmux_id, client_port_id , &prbs_state);
      WTI_FlexmuxCheckStatus("WPX_FRMR_STATUS_SOCKET_PDH_DIAG_EgressPrbsGetState", ufe_framer_status, __LINE__);
      prbs_result[client_port_id] = prbs_state;
   }
   else
   {
      ufe_framer_status = WPX_FRMR_STATUS_SOCKET_PDH_DIAG_IngressPrbsGetState(flexmux_id , client_port_id , &prbs_state);
      WTI_FlexmuxCheckStatus("WPX_FRMR_STATUS_SOCKET_PDH_DIAG_IngressPrbsGetState", ufe_framer_status, __LINE__);
      prbs_result[client_port_id] = prbs_state;
   }


   /* disable the Egress PRBS generator for the client port*/
   status = WPX_FRMR_SOCKET_PDH_DIAG_EgressGenPrbs_Set(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_PRBS_DISABLED);
   WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_EgressGenPrbs_Set", status, __LINE__);

   if(framing_mode == WUFE_UNFRAMED)
   {
      /* disable the Egress PRBS detector for the client port*/
      status = WPX_FRMR_SOCKET_PDH_DIAG_EgressTermPrbsSet(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_PRBS_DISABLED);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_EgressTermPrbsSet", status, __LINE__);
   }
   else
   {
     /* disable the Ingress PRBS detector for the client port*/
      status = WPX_FRMR_SOCKET_PDH_DIAG_IngressTermPrbsSet(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_PRBS_DISABLED);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_IngressTermPrbsSet", status, __LINE__);
   }


   if (prbs_result[client_port_id] == WPX_UFE_FRAMER_PRBS_STATE_CLEAR)
   {
      return 0;
   }
   else
   {
      switch (prbs_result[client_port_id])
      {
         case WPX_UFE_FRAMER_PRBS_STATE_IDLE: printf("PRBS is disabled for client_port_id %d of flexmux_id %d\n",client_port_id, flexmux_id);
            break;
         case WPX_UFE_FRAMER_PRBS_STATE_ERRORS: printf("PRBS data check failed for client_port_id %d of flexmux_id %d\n",client_port_id, flexmux_id);
            break;
         case WPX_UFE_FRAMER_PRBS_STATE_INACTIVE: printf("PRBS is inactive for client_port_id %d of flexmux_id %d\n",client_port_id, flexmux_id);
            break;
      }
      return 1;
   }
}

void WTI_FlexmuxForceA(WP_U32 flexmux_id, WP_U32 line_port_id)
{
   WP_U8 status = WPX_UFE_FRAMER_OK;
#if DEBUG
   WP_U8 force_side = 2;
   WP_time before ,after;
   WP_U32 sec, usec;

   WPL_TimestampGet(&before);
#endif
   status = WPX_FRMR_SONET_SDH_PORT_ForceA(0, flexmux_id, line_port_id);
   WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_ForceA", status, __LINE__);
#if DEBUG
   status =WPX_FRMR_SONET_SDH_PORT_GetForceState(0, line_port_id, &force_side);
   WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_GetForceState", status, __LINE__);
   WPL_TimestampGet(&after);
   sec = (after.sec)-(before.sec);
   usec = (after.usec)-(before.usec);
   if (usec<0)
   {
      sec = sec - 1;
      usec = usec + 1000000;
   }
   printf("In line %d forcing port A took %d.%06d seconds\n",line_port_id,sec,usec);
   if (status == WPX_UFE_FRAMER_OK)
   {
      if (force_side == WPX_UFE_FRAMER_PROTECTION_FORCE_A)
      {
         printf("From ForceA: In line %d traffic is received from port A\n",line_port_id);
      }
      else if (force_side == WPX_UFE_FRAMER_PROTECTION_FORCE_B)
      {
         printf("From ForceA: In line %d traffic is received from port B\n",line_port_id);
      }
   }
#endif
}

void WTI_FlexmuxForceB(WP_U32 flexmux_id, WP_U32 line_port_id)
{
   WP_U8 status = WPX_UFE_FRAMER_OK;
#if DEBUG
   WP_U8 force_side = 2;
   WP_time before ,after;
   WP_U32 sec, usec;

   WPL_TimestampGet(&before);
#endif
   status = WPX_FRMR_SONET_SDH_PORT_ForceB(0, flexmux_id, line_port_id);
   WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_ForceB", status, __LINE__);
#if DEBUG
   status =WPX_FRMR_SONET_SDH_PORT_GetForceState(0, line_port_id, &force_side);
   WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_GetForceState", status, __LINE__);
   WPL_TimestampGet(&after);
   sec = (after.sec)-(before.sec);
   usec = (after.usec)-(before.usec);
   if (usec<0)
   {
      sec = sec - 1;
      usec = usec + 1000000;
   }
   printf("In line %d forcing port B took %d.%06d seconds\n",line_port_id,sec,usec);
   if (status == WPX_UFE_FRAMER_OK)
   {
      if (force_side == WPX_UFE_FRAMER_PROTECTION_FORCE_A)
      {
         printf("From ForceB: In line %d traffic is received from port A\n",line_port_id);
      }
      else if (force_side == WPX_UFE_FRAMER_PROTECTION_FORCE_B)
      {
         printf("From ForceB: In line %d traffic is received from port B\n",line_port_id);
      }
   }
#endif
}

void WTI_Flexmux_ForceStateGet(WP_U32 iLinePort)
{
   WP_U8 status, force_side;

   status = WPX_FRMR_SONET_SDH_PORT_GetForceState(0, iLinePort, &force_side);
   WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_GetForceState", status, __LINE__);

   switch (force_side)
   {
      case WPX_UFE_FRAMER_PROTECTION_FORCE_A:
         printf("Active port for line #%u is working port!!!\n", iLinePort);
         break;
      case WPX_UFE_FRAMER_PROTECTION_FORCE_B:
         printf("Active port for line #%u is protected port!!!\n", iLinePort);
         break;
      default:
         break;
         
   }
   
}

void WTI_Flexmux_Poke(U8 flexmux_id, U32 Address, U32 Value)
{
   WP_U8 Result;
   
   Result = WPX_FRMR_DEV_DIAG_Poke(0, flexmux_id, Address, Value);
   if (WPX_UFE_FRAMER_OK !=Result)
   {
      printf("Poke was not accopmlished successfully\n");
   }
   else
   {
      printf("write data %#x to address %#x\n",Value,Address);
      
   }
   
   return;
   
   
}

void WTI_Flexmux_Peek(U8 flexmux_id, U32 Address)
{
   WP_U8 Result;
   WP_U32 iii=0;
   
   
   /*Clear the DIAG_PEEK_INFO to be filled by the framer callback function*/
   memset(&DIAG_PEEK_INFO,0,sizeof(struct WTI_DIAG_PEEK_INFO));

   Result = WPX_FRMR_DEV_DIAG_Peek(flexmux_id,  Address); 
   if (WPX_UFE_FRAMER_OK !=Result)
   {
      printf("Peek was not accopmlished successfully - can't read the relevant address\n");
      return;
      
   }

   /*Loop for 2 seconds*/
   while (DIAG_PEEK_INFO.Valid!=1)
   {
      iii++;

      WPL_Delay(1);
      if(iii>3000000)
      {
         printf("Error: Peek function timeout\n");
         return;
         
      }

   }
     
   if (1 != DIAG_PEEK_INFO.Valid)
   {
      printf("can't read address %#x\n",Address);
      return;
   }
   
  
   printf("%#x : %08x\n",DIAG_PEEK_INFO.Address,DIAG_PEEK_INFO.Value);
   return;
   
   
}

void WTI_Flexmux_Port_SetRate(U8 flexmux_id,U8 line_port_id, U8 rate)
{
  WP_U8 status = WPX_UFE_FRAMER_OK;

  status = WPX_FRMR_SONET_SDH_PORT_SetRate(0, flexmux_id, line_port_id, rate);
  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_SetRate", status, __LINE__);

  if (WPX_UFE_FRAMER_LINE_PORT_RATE_OFF != rate)
  {     
     status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode(0, flexmux_id, line_port_id, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
     WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode", status, __LINE__);

     status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetTX(0, flexmux_id, line_port_id, "WG RS-TRACE");
     WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetTX", status, __LINE__);
     
     status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetEX(0, flexmux_id, line_port_id, "WG RS-TRACE");
     WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetEX", status, __LINE__);
  }
  

}

void WTI_Flexmux_AddressBasePeek(U8 flexmux_id, U32 StartAddress)
{
   WP_U8 Result;
   WP_U32 iii=0;
   WP_U32 addr;

   /*Loop for reading WTI_NUM_OF_WORDS_TO_READ words  */
   for (addr=StartAddress;addr<StartAddress+(WTI_NUM_OF_WORDS_TO_READ*4);addr=addr+4)
   {
      
      /*Clear the DIAG_PEEK_INFO to be filled by the framer callback function*/
      memset(&DIAG_PEEK_INFO,0,sizeof(struct WTI_DIAG_PEEK_INFO));

      /*read the address  */
      Result = WPX_FRMR_DEV_DIAG_Peek(flexmux_id, addr); 
      if (WPX_UFE_FRAMER_OK !=Result)
      {
         printf("Peek was not accopmlished successfully - can't read the relevant address\n");
         return;
      
      }

      /*Loop for 2 seconds*/
      while (1!=DIAG_PEEK_INFO.Valid)
      {
         iii++;

         WPL_Delay(1);
         if(iii>2000000)
         {
            printf("Error: Peek function timeout\n");
            return;
            

         }

      }
     
      /*check the returned data validity */
      if (1 != DIAG_PEEK_INFO.Valid)
      {
         printf("can't read address %#x\n",addr);
         return;
      }
   
      /*print data */
      if (addr%0x10 ==0)
         printf("\n%#x: ",addr);
      
      printf("%08x  ",DIAG_PEEK_INFO.Value);
   }
   printf("\n");
   return;   
   
}


#if 1
void WT_BulkFlexmuxSDH_HO_FacitilyDelete(WT_ufe *ufe, 
                                         WP_U8 flexmux_mode, 
                                         WTI_flexmux_line_port_mode line_port_mode, 
                                         WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4)
{
   WP_U32 stm4, stm1, stm0, num_for_vc3_or_vc4;
   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE line_end_point = {0};
   WUFE_status status;

   printf("WT_BulkFlexmuxSDH_HO_FacitilyDelete\n");
   printf("vc_3_4 %d\n", vc_3_4);
   
   if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
   {
      WP_U32 sts12, sts3, sts1;

      if(line_port_mode == WT_FLEXMUX_LINE_PORT_OC3)
      {
         sts12 = 0;
         line_end_point.u.SONET.sts12 = sts12;
         for (sts3=0; sts3<4; sts3++)
         {
            line_end_point.u.SONET.sts3 = sts3;
            for(sts1=0; sts1<3; ++sts1)
            {
               line_end_point.u.SONET.sts1 = sts1;
               status = WPX_FRMR_SONET_SDH_DeleteFacility(0, 0, &line_end_point);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_DeleteeFacility (VC3/4)", status, __LINE__);
            }
         }
      }
      else if(line_port_mode == WT_FLEXMUX_LINE_PORT_OC12)
      {
         for (sts12=0; sts12<1; sts12++)
         {
            line_end_point.u.SONET.sts12 = sts12;
            for (sts3=0; sts3<4; ++sts3)
            {
               line_end_point.u.SONET.sts3 = sts3;
               for (sts1=0; sts1<3; ++sts1)
               {
                  line_end_point.u.SONET.sts1 = sts1;
                  status = WPX_FRMR_SONET_SDH_DeleteFacility(0, 0, &line_end_point);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_DeleteeFacility (VC3/4)", status, __LINE__);
               }
            }
         }
      }
   }
   else if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
   {
      line_end_point.TransferType = vc_3_4;

      /* for VC3 we need to configure 3 AU-3 containers
         and for VC4 we need to configure only 1 AU-4 container  */
      num_for_vc3_or_vc4 = (vc_3_4 == WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3) ? 3:1;
      printf("num_for_vc3_or_vc4 %d\n", num_for_vc3_or_vc4);
      
         
//       if (vc_3_4 == WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3)
//       {
//          num_for_vc3_or_vc4 = 3;
//       }
//       else
//       {
//          num_for_vc3_or_vc4 = 1;
//       }

      if (line_port_mode == WT_FLEXMUX_LINE_PORT_STM1)
      {
         stm4 = 0;
         line_end_point.u.SDH.stm4 = stm4;
         for (stm1=0; stm1<4; stm1++)
         {
            line_end_point.u.SDH.stm1 = stm1;
            for(stm0=0; stm0<num_for_vc3_or_vc4; ++stm0)
            {
               line_end_point.u.SDH.stm0 = stm0;
               status = WPX_FRMR_SONET_SDH_DeleteFacility(0, 0, &line_end_point);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_DeleteFacility (VC3/4)", status, __LINE__);
            }
         }
      }
      else if (line_port_mode == WT_FLEXMUX_LINE_PORT_STM4)
      {
         for (stm4=0; stm4<1; stm4++)
         {
            line_end_point.u.SDH.stm4 = stm4;
            for (stm1=0; stm1<4; ++stm1)
            {
               line_end_point.u.SDH.stm1 = stm1;
               for(stm0=0; stm0<num_for_vc3_or_vc4; ++stm0)
               {
                  line_end_point.u.SDH.stm0 = stm0;
                  printf("stm4 %d, stm1 %d, stm0 %d, TransferType %d\n", line_end_point.u.SDH.stm4, line_end_point.u.SDH.stm1, line_end_point.u.SDH.stm0, line_end_point.TransferType);
                  
                  status = WPX_FRMR_SONET_SDH_DeleteFacility(0, 0, &line_end_point);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_DeleteeFacility (VC3/4)", status, __LINE__);

               }
            }
         }
      }
   }
}

void WT_BulkFlexmuxSDH_HO_FacitilyCreate(WT_ufe *ufe, 
                                         WP_U8 flexmux_mode, 
                                         WTI_flexmux_line_port_mode line_port_mode, 
                                         WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4)
{
   WP_U32 stm4, stm1, stm0, num_for_vc3_or_vc4;
   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE line_end_point = {0};
   WUFE_status status;

   printf("WT_BulkFlexmuxSDH_HO_FacitilyCreate\n");
   
#if WTI_ADM_MODE
/* in CR when using the ADM for testing */
   U8 TX_C2=0x1, EX_C2=0x1;
   WP_CHAR *TX_J1="0000000000000000", *EX_J1="0000000000000000",*pTX_TraceString="WG RS-TRACE",*pEX_TraceString="WG RS-TRACE";
#else /*  WTI_ADM_MODE*/
   U8 TX_C2=0x2, EX_C2=0x2;
   WP_CHAR *TX_J1="WG HP-TRACE", *EX_J1="WG HP-TRACE" /*,*pTX_TraceString="WG RS-TRACE",*pEX_TraceString="WG RS-TRACE"*/;
#endif/* WTI_ADM_MODE */

   
   if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
   {
      WP_U32 sts12, sts3, sts1;
      line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS1_SPE;

      if(line_port_mode == WT_FLEXMUX_LINE_PORT_OC3)
      {
         sts12 = 0;
         line_end_point.u.SONET.sts12 = sts12;
         for (sts3=0; sts3<4; sts3+=1)
         {
            line_end_point.u.SONET.sts3 = sts3;
            for(sts1=0; sts1<3; ++sts1)
            {
               line_end_point.u.SONET.sts1 = sts1;
               status = WPX_FRMR_SONET_SDH_CreateFacility(0, 0, &line_end_point);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC3/4)", status, __LINE__);

               /* set the transmitted and expected C2 values */
               status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX(0, 0, &line_end_point, TX_C2);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX", status, __LINE__);

               status =WPX_FRMR_SONET_SDH_HO_PATH_SetRdiMode(0, 0, &line_end_point, 0);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_SetRdiMode", status, __LINE__);
               status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX(0, 0, &line_end_point, EX_C2);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX", status, __LINE__);



               WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode(0, 0, &line_end_point, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
               status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX(0, 0, &line_end_point, TX_J1);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX", status, __LINE__);


               status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX(0, 0, &line_end_point, EX_J1);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX", status, __LINE__);

            }

         }
      }
      else if(line_port_mode == WT_FLEXMUX_LINE_PORT_OC12)
      {
         for (sts12=0; sts12<1; sts12+=1)
         {
            line_end_point.u.SONET.sts12 = sts12;
            for (sts3=0; sts3<4; ++sts3)
            {
               line_end_point.u.SONET.sts3 = sts3;
               for (sts1=0; sts1<3; ++sts1)
               {
                  line_end_point.u.SONET.sts1 = sts1;
                  status = WPX_FRMR_SONET_SDH_CreateFacility(0, 0, &line_end_point);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC2/4)", status, __LINE__);

                  /* set the transmitted and expected C2 values */
                  status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX(0, 0, &line_end_point, TX_C2);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX", status, __LINE__);


                  status =WPX_FRMR_SONET_SDH_HO_PATH_SetRdiMode(0, 0, &line_end_point, 0);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_SetRdiMode", status, __LINE__);
                  status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX(0, 0, &line_end_point, EX_C2);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX", status, __LINE__);


                  WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode(0, 0, &line_end_point, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
                  status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX(0, 0, &line_end_point, TX_J1);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX", status, __LINE__);

                  status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX(0, 0, &line_end_point, EX_J1);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX", status, __LINE__);

               }

            }
         }
      }
   }
   else if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
   {
      line_end_point.TransferType = vc_3_4;

      /* for VC3 we need to configure 3 AU-3 containers
         and for VC4 we need to configure only 1 AU-4 container  */
      if (vc_3_4 == WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3)
      {
         num_for_vc3_or_vc4 = 3;
      }
      else
      {
         num_for_vc3_or_vc4 = 1;
      }

      if (line_port_mode == WT_FLEXMUX_LINE_PORT_STM1)
      {
         stm4 = 0;
         line_end_point.u.SDH.stm4 = stm4;
         for (stm1=0; stm1<4; stm1+=1)
         {
            line_end_point.u.SDH.stm1 = stm1;
            for(stm0=0; stm0<num_for_vc3_or_vc4; ++stm0)
            {
               line_end_point.u.SDH.stm0 = stm0;
               printf("WPX_FRMR_SONET_SDH_CreateFacility(HO):line_end_point.TransferType %d\n", line_end_point.TransferType);
               status = WPX_FRMR_SONET_SDH_CreateFacility(0, 0, &line_end_point);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC3/4)", status, __LINE__);

               /* set the transmitted and expected C2 values */
               status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX(0, 0, &line_end_point, TX_C2);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX", status, __LINE__);

               status =WPX_FRMR_SONET_SDH_HO_PATH_SetRdiMode(0, 0, &line_end_point, 1);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_SetRdiMode", status, __LINE__);
               status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX(0, 0, &line_end_point, EX_C2);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX", status, __LINE__);

               WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode(0, 0, &line_end_point, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
               status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX(0, 0, &line_end_point, TX_J1);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX", status, __LINE__);

               status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX(0, 0, &line_end_point, EX_J1);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX", status, __LINE__);

               status = WPX_FRMR_SONET_SDH_HO_PATH_B3_SetExcessiveTh(0,0, &line_end_point,
                                                                     WPX_UFE_FRAMER_THRESHOLD_10E4);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_B3_SetExcessiveTh", status, __LINE__);

               status = WPX_FRMR_SONET_SDH_HO_PATH_B3_SetSigDegradeTh(0, 0, &line_end_point,
                                                                      WPX_UFE_FRAMER_THRESHOLD_10E6);
               WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_B3_SetSigDegradeTh", status, __LINE__);
            }
         }
      }
      else if (line_port_mode == WT_FLEXMUX_LINE_PORT_STM4)
      {
         for (stm4=0; stm4<1; stm4+=1)
         {
            line_end_point.u.SDH.stm4 = stm4;
            for (stm1=0; stm1<4; ++stm1)
            {
               line_end_point.u.SDH.stm1 = stm1;
               for(stm0=0; stm0<num_for_vc3_or_vc4; ++stm0)
               {
                  line_end_point.u.SDH.stm0 = stm0;
                  status = WPX_FRMR_SONET_SDH_CreateFacility(0, 0, &line_end_point);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC3/4)", status, __LINE__);

                  /* set the transmitted and expected C2 values */
                  status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX(0, 0, &line_end_point, TX_C2);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX", status, __LINE__);

//                   status =WPX_FRMR_SONET_SDH_HO_PATH_SetRdiMode(0, 0, &line_end_point, 0);
//                   WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_SetRdiMode", status, __LINE__);
                  status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX(0, 0, &line_end_point, EX_C2);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX", status, __LINE__);

                  WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode(0, 0, &line_end_point, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
                  status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX(0, 0, &line_end_point, TX_J1);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX", status, __LINE__);


                  status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX(0, 0, &line_end_point, EX_J1);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX", status, __LINE__);

                  status = WPX_FRMR_SONET_SDH_HO_PATH_B3_SetExcessiveTh(700, 0, &line_end_point,
                                                                        WPX_UFE_FRAMER_THRESHOLD_10E4);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_B3_SetExcessiveTh", status, __LINE__);

                  status = WPX_FRMR_SONET_SDH_HO_PATH_B3_SetSigDegradeTh(701, 0, &line_end_point,
                                                                         WPX_UFE_FRAMER_THRESHOLD_10E6);
                  WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_B3_SetSigDegradeTh", status, __LINE__);
               }
            }
         }
      }
      else
      {
         printf("Error - false port mode\n");
         exit(1); 
      }
   }   
   else
   {
      printf("Error - false flexmux mode\n");
      exit(1);
      
   }
   
}

/*
 *
 * You should these two function below only when you start with T1's first, in case you want to pass from T1 to E1 and vice versa.
 * This is order to be compatible to the indices of the test registry holding the framer sockets information.
 *
 */
void WT_BulkFlexmuxConnectionDelete(WT_ufe *ufe, WP_U32 max_pw, WP_U32 is_e1, WP_U32 delete_lo)
{
   WP_U32 iSocketClient=0, pw_index;
   WT_ufe_line_sonet_params *sonet_line_params;
   WT_ufe_line_sdh_params *sdh_line_params;
   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE line_end_point;
   WP_U8 status;
   
   printf("WT_BulkFlexmuxConnectionDelete\n");

   /* The skip rule for E1's is compatible to the standard and not to the test. Meaning, the skip is on the forth socket client and not 8 socket clients out of 28 */
   for(pw_index=0;pw_index<max_pw;pw_index++)
   {
      WPL_Delay(50000);
      //printf("pw_index %d\n", pw_index);
      
//       if (is_e1 && (pw_index % 28) > 20)
//          continue;
      
      if (ufe->fpga_mode == WT_SONET)
      {
         sonet_line_params = &ufe->line_params[pw_index].framer_line_params.line_sonet_params;
         line_end_point.u.SONET.sts12 = sonet_line_params->sts12;
         line_end_point.u.SONET.sts3 = sonet_line_params->sts3;
         line_end_point.u.SONET.sts1 = sonet_line_params->sts1;
         line_end_point.u.SONET.vt_group = sonet_line_params->vt_group;
         line_end_point.u.SONET.vt = sonet_line_params->vt;

         if (sonet_line_params->transfer_type == WUFE_SONET_TYPE_E1)
            line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT2;
         else if (sonet_line_params->transfer_type == WUFE_SONET_TYPE_T1)
            line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT1_5;
         else
            WTI_FlexmuxTerminateOnError("Invalid line transfer type", 0);

         iSocketClient = WTI_FlexmuxClientPortIndexGet(pw_index, sonet_line_params->transfer_type);
#if WTI_FLEXMUX_DBG_MODE
         printf("deleting iSocketClient %d\n", iSocketClient);
         printf("sts12 %d sts3 %d sts1 %d vt_group %d vt %d\n", sonet_line_params->sts12,
                sonet_line_params->sts3,
                sonet_line_params->sts1,
                sonet_line_params->vt_group,
                sonet_line_params->vt);
#endif
      }
      else if (ufe->fpga_mode == WT_SDH)
      {
//         printf("SDH\n");
         
         sdh_line_params = &ufe->line_params[pw_index].framer_line_params.line_sdh_params;
         
         /* create the VC3/4 facility */
         line_end_point.u.SDH.stm4 = sdh_line_params->stm4;
         line_end_point.u.SDH.stm1 = sdh_line_params->stm1;
         line_end_point.u.SDH.stm0 = sdh_line_params->stm0;
         line_end_point.u.SDH.tug2 = sdh_line_params->tug2;
         line_end_point.u.SDH.tu = sdh_line_params->tu;

         /* Skip rule for E1's */
         if(1 == is_e1)
         {
            if (3 == line_end_point.u.SDH.tu)
            {            
               continue;            
            }         
         }
         if (1 == is_e1)
         {
            line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC12;
         }

         else
         {
            line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC11;
         }


//         iSocketClient = WTI_FlexmuxClientPortIndexGet(pw_index, sdh_line_params->transfer_type);
         /* Since the pw_index is actually the line index is actually implies the socket client id */
         iSocketClient = pw_index;
         
#if WTI_FLEXMUX_DBG_MODE
         printf("deleting  iSocketClient %d\n", iSocketClient);
         printf("stm4 %d stm1 %d stm0 %d tug2 %d tu %d\n", sdh_line_params->stm4, sdh_line_params->stm1, sdh_line_params->stm0, sdh_line_params->tug2, sdh_line_params->tu);
#endif
      }
      else
      {
         printf("The UFE FPGA mode is not supported %d\n", ufe->fpga_mode);
         exit(1);
      }
      
      status = WPX_FRMR_CONNECTIONS_AddDropLineToSocketDelete(0, 0, &line_end_point, iSocketClient);
      WTI_FlexmuxCheckStatus("WPX_FRMR_CONNECTIONS_AddDropLineToSocketDelete", status, __LINE__); 

       /* delete the client port facility */
       status = WPX_FRMR_SOCKET_PDH_DeleteFacility(0, 0, iSocketClient);
       WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DeleteFacility", status, __LINE__);

        /* delete the SDH VC11/VC12 facility */
       if (1 == delete_lo)
       {    
//         printf("stm4 %d, stm1 %d, stm0 %d, tug2 %d, tu %d\n", line_end_point.u.SDH.stm4, line_end_point.u.SDH.stm1, line_end_point.u.SDH.stm0, line_end_point.u.SDH.tug2, line_end_point.u.SDH.tu);
          status = WPX_FRMR_SONET_SDH_DeleteFacility(0, 0, &line_end_point);
          WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_DeleteFacility (VC11/12)", status, __LINE__);
       }       
   }
}
void WT_BulkFlexmuxConnectionCreate(WT_ufe *ufe, WP_U32 max_pw, WP_U32 is_e1, WP_U32 create_lo)
{
   WP_U32 iSocketClient=0, pw_index;
   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE line_end_point;
   WT_ufe_line_sonet_params *sonet_line_params=NULL;
   WT_ufe_line_sdh_params *sdh_line_params=NULL;   
   WP_U8 status;

   printf("WT_BulkFlexmuxConnectionCreate\n");
   
   for(pw_index=0;pw_index<max_pw;pw_index++)
   {
      WPL_Delay(50000);
      //printf("pw_index %d\n", pw_index);
      
//       if (is_e1 && (pw_index % 28) > 20)
//          continue;

      if (ufe->fpga_mode == WT_SONET)
      {
         sonet_line_params = &ufe->line_params[pw_index].framer_line_params.line_sonet_params;
         line_end_point.u.SONET.sts12 = sonet_line_params->sts12;
         line_end_point.u.SONET.sts3 = sonet_line_params->sts3;
         line_end_point.u.SONET.sts1 = sonet_line_params->sts1;
         line_end_point.u.SONET.vt_group = sonet_line_params->vt_group;
         line_end_point.u.SONET.vt = sonet_line_params->vt;
         


         if (1 == is_e1)
         {            
            line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT2;
            /* Update the registry */
            sonet_line_params->transfer_type = WUFE_SONET_TYPE_E1;            
         }         
         else if (sonet_line_params->transfer_type == WUFE_SONET_TYPE_T1)
         {            
            line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT1_5;
            /* Update the registry */
            sonet_line_params->transfer_type = WUFE_SONET_TYPE_T1;            
         }
         

          iSocketClient = WTI_FlexmuxClientPortIndexGet(pw_index, sonet_line_params->transfer_type);
#if WTI_FLEXMUX_DBG_MODE
         printf("connecting   iSocketClient %d\n", iSocketClient);
         printf("sts12 %d sts3 %d sts1 %d vt_group %d vt %d\n", sonet_line_params->sts12,
                sonet_line_params->sts3,
                sonet_line_params->sts1,
                sonet_line_params->vt_group,
                sonet_line_params->vt);
#endif      
      }
      else if (ufe->fpga_mode == WT_SDH)
      {
         //printf("WT_SDH\n");
         
         sdh_line_params = &ufe->line_params[pw_index].framer_line_params.line_sdh_params;
         
         /* create the VC3/4 facility */
         line_end_point.u.SDH.stm4 = sdh_line_params->stm4;
         line_end_point.u.SDH.stm1 = sdh_line_params->stm1;
         line_end_point.u.SDH.stm0 = sdh_line_params->stm0;
         line_end_point.u.SDH.tug2 = sdh_line_params->tug2;
         line_end_point.u.SDH.tu = sdh_line_params->tu;

         /* Skip rule for E1's */
         if(1 == is_e1)
         {
            if (3 == line_end_point.u.SDH.tu)
            {            
               continue;            
            }         
         }
         if (1 == is_e1)
         {
            //printf("WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC12\n");
            line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC12;
            /* Update the registry */
            sdh_line_params->transfer_type = WUFE_SDH_TYPE_E1;            
         }
         else 
         {
            //printf("WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC11\n");
            
            line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC11;
            /* Update the registry */
            sdh_line_params->transfer_type = WUFE_SDH_TYPE_T1;
         }
         /* Since the pw_index is actually the line index is actually implies the socket client id */
         iSocketClient = pw_index;
         
#if WTI_FLEXMUX_DBG_MODE
         printf("connecting iSocketClient %d\n", iSocketClient);
         printf("stm4 %d stm1 %d stm0 %d tug2 %d tu %d\n", sdh_line_params->stm4, sdh_line_params->stm1, sdh_line_params->stm0, sdh_line_params->tug2, sdh_line_params->tu);
#endif
      }
      else
      {
         printf("The UFE FPGA mode is not supported %d\n", ufe->fpga_mode);
         exit(1);
      }
      if (1 == create_lo)
      {         
//         printf("stm4 %d, stm1 %d, stm0 %d, tug2 %d, tu %d\n", line_end_point.u.SDH.stm4, line_end_point.u.SDH.stm1, line_end_point.u.SDH.stm0, line_end_point.u.SDH.tug2, line_end_point.u.SDH.tu);
         status = WPX_FRMR_SONET_SDH_CreateFacility(0, 0, &line_end_point);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC11/12)", status, __LINE__);


      }
      
      if (sdh_line_params->transfer_type == WUFE_SDH_TYPE_E1)
      {
         status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, 0,iSocketClient, WPX_UFE_FRAMER_CLIENT_E1);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);
      
         status = WPX_FRMR_SOCKET_PDH_SetFraming(0, 0, iSocketClient , WPX_UFE_FRAMER_CLIENT_PCM31CRC);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);

         status = WPX_FRMR_SOCKET_PDH_SetMapping(0,0, iSocketClient, WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetMapping", status, __LINE__);
      }
      else
      {
         status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, 0,iSocketClient, WPX_UFE_FRAMER_CLIENT_T1);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);
      
         status = WPX_FRMR_SOCKET_PDH_SetFraming(0, 0, iSocketClient ,WPX_UFE_FRAMER_CLIENT_FRAMING_ESF);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);

         status = WPX_FRMR_SOCKET_PDH_SetMapping(0,0, iSocketClient, WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
         WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetMapping", status, __LINE__); 
      }

      status = WPX_FRMR_SOCKET_PDH_DisableCAS(0, 0, iSocketClient);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DisableCAS", status, __LINE__);

      /* create the connection between the client port and the line port */
      status = WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate(0, 0, &line_end_point, iSocketClient);
      WTI_FlexmuxCheckStatus("WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate", status, __LINE__);

      /* set the transmitted and expected V5 values */
      status = WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetTX(0, 0,&line_end_point, 0x02);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetTX", status, __LINE__);
      
      status = WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetEX(0,0, &line_end_point, 0x02);
      WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetEX", status, __LINE__);
      
      /* set the transmitted and expected J2 values */
      status = WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX(0,0, &line_end_point, "WG LP-TRACE");
      WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX", status, __LINE__);
      
      status = WPX_FRMR_SONET_SDH_LO_PATH_J2_SetEX(0, 0, &line_end_point, "WG LP-TRACE");
      WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetEX", status, __LINE__); 
   }
   
}

#endif /* if 1*/

void WTI_FlexmuxPDHOutputClockSet(WP_U8 output_select, WP_U32 line_index,WUFE_line_transf_type transfer_type,WP_U32 clock_rate)
{
   WP_U32 client_port_id=0;
   WP_U8 status = WPX_UFE_FRAMER_OK;

   client_port_id = WTI_FlexmuxClientPortIndexGet(line_index,transfer_type);
   /* Set the Recover PDH clock line (Rx) to RECV_PDH_LINE_CLK_0   - the call must be after the SocketClientPDH was created */
   /* WPX_FRMR_SOCKET_PDH_DIAG_DRV_RecClkSet(U32 TransactionIdentifier,U8 iDevice,U8 OutputSelector,U32 iSocketClientPDH,U32 ClockRate)*/
   status = WPX_FRMR_SOCKET_PDH_DIAG_DRV_RecClkSet(0,0,output_select,client_port_id,clock_rate); /* or _1544 */
   WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_DRV_RecClkSet", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
   printf("Setting PDH Rx clock %d to line %d at %dKhz\n",output_select,client_port_id,clock_rate);
#endif
}

void WTI_Flexmux_Squelch_RecClkSet(WP_U8 flexmux_id, WP_U8 output_selector, WP_U8 squelch_mode)
{
   WP_U32 status = WPX_UFE_FRAMER_OK;
#if WTI_FLEXMUX_DBG_MODE
   printf("output_selector %d, squelch_mode %d\n", output_selector, squelch_mode);
#endif   
   status = WPX_FRMR_DEV_Squelch_RecClkSet(0, flexmux_id, output_selector, squelch_mode);
   WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_Squelch_RecClkSet", status, __LINE__);
   
}

#if WTI_COLLECT_TRACKING_DATA

typedef struct {
   WP_U32 type;
   WP_U32 timestamp;
} PATS_RECORD;

#if 0
#if defined(__linux__)
PATS_RECORD *save_area = (PATS_RECORD *) 0x1c249000;
#else
PATS_RECORD *save_area = (PATS_RECORD *) 0xbc249000;
#endif
#else
PATS_RECORD save_area[0x10000/sizeof(PATS_RECORD)];
#endif

static int fence = 0;
static int max_items = 0x10000/sizeof(PATS_RECORD);

void record_action(WP_U32 type, WP_U32 timestamp)
{
   PATS_RECORD *item;

   if (fence >= max_items - 1)
      return;

   item = &save_area[fence++];
   item->type = type;
   item->timestamp = timestamp;
}


WP_CHAR *type_name[] = {
                          "SERVER ACTIVE       ", //   0
                          "ANC PORT ALARM      ", //   1
                          "ANC PATH ALARM      ", //   2
                          "ANC DC PORT ALARM   ", //   3
                          "ANC SC PORT ALARM   ", //   4
                          "ANC PORT PM         ", //   5
                          "ANC HO PATH PM      ", //   6
                          "ANC LO PATH PM      ", //   7
                          "ANC PDH PM          ", //   8
                          "ANC DC PORT PM      ", //   9
                          "ANC K1/K2 SIGNAL    ", //  10
                          "ANC S1 SIGNAL       ", //  11
                          "ANC DIAGNOSTIC TABLE", //  12
                          "ENTER INT           ", //  13
                          "EXIT INT            ", //  14
                          "REINSTATE           ", //  15
                          "PENDING INTERRUPTS  ", //  16
                          "PENDING AT REINSTATE", //  17
                          "AVAIL               ", //  18
                          "AVAIL               ", //  19
                          "SVC PATH ALARM      ", //  20
                          "SVC PORT ALARM      ", //  21
                          "ENTER LOS SVC       ", //  22
                          "EXIT LOS SVC        ", //  23
                          "SVC K1/K2           ", //  24
                          "SVC S1              ", //  25
                          "CB PATH ALARM       ", //  26
                          "CB LOS ALARM        ", //  27
                          "CB PORT ALARM       ", //  28
                          "CB K1/K2            ", //  29
                          "CB S1               ", //  30
                          "CB PDH              ", //  31
                          "SVC PM PORT         ", //  32
                          "SVC PM HO PATH      ", //  33
                          "SVC PM LO PATH      ", //  34
                          "SVC PM PDH          ", //  35
                          "CB PM PORT          ", //  36
                          "CB PM HO PATH       ", //  37
                          "CB PM LO PATH       ", //  38
                          "CB PM PDH           ", //  39
                          "LOS REINSTATE       ", //  40
                          "SVC LOS REINSTATE   ", //  41
                          "AVAIL               ", //  42
                          "IS_CORE/FRAMER      ", //  43
                          "IS SFP              ", //  44
                          "IS LOST LOCK        ", //  45
                          "IS EXT PLL          ", //  46
                          "LOCK AVAIL          ", //  47
                          "LOCK ACQUIRE        ", //  48
                          "LOCK TAKEN          ", //  49
                          "MB LOCK TAKE        ", //  50
                          "MB LOCK ACQUIRE     ", //  51
                          "DEVICE MB SERVICED  "  //  52
                       };

static WP_U32 last_record_index = sizeof(type_name) / sizeof(type_name[0]);

void WTI_DisplayRecords(void)
{
   WP_U32 j;
   PATS_RECORD *item;

   for (item = save_area, j = 0; j < fence; ++j, ++item)
      if (item->type < last_record_index)
         printf("%s 0x%x\n", type_name[item->type], item->timestamp);
      else
         printf("UNRECOGNIZED:  %d, 0x%x\n", item->type, item->timestamp);

   printf("%d items\n", fence);
   fence = 0;
}

void WTI_InitializeDisplayRecords(void)
{
   fence = 0;
}

void WTI_APS_Time_Measure(void)
{
   WP_U32 j, exit_los_service_cnt, max_exit_los_service, time_usec;
   PATS_RECORD *item, *first_los_service, *enter_int, *last_exit_los_service;   

   /* find first ENTER LOS SVC */
   item = save_area; 
   j = 0;
   
   while(22 != item->type)
   {      
      item++;
      if (fence <= j)
      {
         printf("Error - Exceeds array boundaries!!!\n");
         return;         
      }
      j++;
      
   }
   
   first_los_service = item;
   
   /* Find last ENTER INT before the first ENTER LOS SVC */
   item = first_los_service; 

   while(13 != item->type)
   {      
      item--;
      j--;      
   }
   
   enter_int = item;

   /* Find the 4th EXIT LOS SVC */
   item = first_los_service; 
   exit_los_service_cnt = 0;

   switch(flexmux_reg[0].flexmux_build_personality)
   {
      case WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12:
         max_exit_los_service = 1;   
         break;         
      case WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3:
         max_exit_los_service = 4;   
         break;
      case WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3:
         max_exit_los_service = 2;
         break;
      default:
         max_exit_los_service = 0;
         printf("Error - Invalid build personality!!!\n");
         return;
         
   }
   printf("max_exit_los_service %d\n", max_exit_los_service);

   while(max_exit_los_service > exit_los_service_cnt)
   {      
      item++;
      j++;  

      if (23 == item->type)
         exit_los_service_cnt++;
      
    
   }
   
   last_exit_los_service = item;
   
   time_usec = WP_TimeDelta(last_exit_los_service->timestamp, enter_int->timestamp);
   
   printf("enter_int %#x, last_exit_los_service %#x delta %#x, time_delta %d[uSec]\n", enter_int->timestamp, last_exit_los_service->timestamp, (last_exit_los_service->timestamp - enter_int->timestamp), time_usec);
   
   
   if (50000 > time_usec)
      printf("Test Passed: APS in time\n");
   else
      printf("Test Failed: APS exceeded time\n");

//    /* Reset the save area*/
//    fence = 0;
}

#endif
