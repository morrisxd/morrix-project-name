/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

#include "WO_FRMR_private.h"

#ifndef WUFE_UFE_H
#include "wufe_ufe.h"
#endif

#ifdef WUFE_UFE4
#ifndef WPX_UFE4_DATA_H
#include "wpx_ufe4_data.h"
#endif
#endif

#include "wp_wddi.h"

#ifndef _WPL_PLATFORM_H__
#include <wpl_platform.h>
#endif

#ifndef WUFEI_BOARD_SUPPORT_INT_H
#include "wufei_board_support_int.h"
#endif

#ifndef WUFEI_BOARD_DATA_H
#include "wufei_board_data.h"
#endif

#ifndef WPL_MEM_ACCESS_H
#include "wpl_mem_access.h"
#endif

#define WTI_COLLECT_TIMING_INFORMATION 0

#define WPX_DISPLAY_ENABLE    1
#define WPX_PRINT_ERR_ENABLE  1
#define WPX_PRINT_DBG_ENABLE  1

#define WPX_BOARD_SUPPORT_ENABLED 1

#define WPX_MAX_LINE_PORTS_PER_FLEXMUX_DEVICE 16

#define WPX_FLEXMUX_REG_UNLOCKED 0
#define WPX_FLEXMUX_REG_LOCKED   1

#define WPX_FLEXMUX_NOT_READY 0
#define WPX_FLEXMUX_READY 1
#define WPX_FLEXMUX_LOG_SIZE 10000
#define WPX_FLEXMUX_MAX_LOOP_COUNT 500000
#define WPX_FLEXMUX_LOCK_HANDLE 0xABCDABCD

#define WPL_FRAMER_LOCK_KEY WPL_LOCK_KEY_CREATE(WPL_HW_LOCK,            \
                                                WPL_PRIVATE_LOCK,       \
                                                WPL_TA_SEMAPHORE_RESERVED_FRAMER, 0)

/* Use private (FRAMER only) semaphore */
#define WPX_FLEXMUX_SEMAPHORE_KEY WPL_FRAMER_LOCK_KEY

#define WPX_FLEXMUX_LOCK(lock)   WPL_Lock(WPX_FLEXMUX_SEMAPHORE_KEY, lock)
#define WPX_FLEXMUX_UNLOCK(lock) WPL_Unlock(WPX_FLEXMUX_SEMAPHORE_KEY, lock)

#if WPX_DISPLAY_ENABLE
#define WPX_PRINT_DISP(y) \
do { sprintf y ; printf("%s",wpx_flexmux_log);} while(0)
#else
#define WPX_PRINT_DISP(y)
#endif

#if WPX_PRINT_ERR_ENABLE
#define WPX_PRINT_ERR(y) \
do { sprintf y ; printf("%s",wpx_flexmux_log);} while(0)
#else
#define WPX_PRINT_ERR(y)
#endif

#if WPX_PRINT_DBG_ENABLE
#define WPX_PRINT_DBG(y) \
do { sprintf y ; printf("%s",wpx_flexmux_log);} while(0)
#else
#define WPX_PRINT_DBG(y)
#endif

/*---------------------- GENERALIZED ALARM HANDLING ------------------------------------*/
#define ALARM_CALLBACK_SONET_SDH_PATH_INDEX        (0)
#define ALARM_CALLBACK_SONET_SDH_PORT_INDEX        (1)
#define ALARM_CALLBACK_DISCRETE_CLIENT_PORT_INDEX  (2)
#define ALARM_CALLBACK_SOCKET_PDH_INDEX            (3)
#define ALARM_CALLBACK_LAST_INDEX                  (4)

void *alarm_callbacks[ALARM_CALLBACK_LAST_INDEX];
/*---------------------- GENERALIZED ALARM HANDLING ------------------------------------*/

/*---------------------- GENERALIZED PERFORMANCE MONITORING HANDLING -------------------*/
#define PMA_CALLBACK_SONET_SDH_PORT_INDEX          (0)
#define PMA_CALLBACK_SONET_SDH_HO_PATH_INDEX       (1)
#define PMA_CALLBACK_SONET_SDH_LO_PATH_INDEX       (2)
#define PMA_CALLBACK_DISCRETE_CLIENT_INDEX         (3)
#define PMA_CALLBACK_SOCKET_PDH_INDEX              (4)
#define PMA_CALLBACK_LAST_INDEX                    (5)

void *performance_monitoring_announce_callbacks[PMA_CALLBACK_LAST_INDEX];
/*---------------------- GENERALIZED PERFORMANCE MONITORING HANDLING -------------------*/

/*---------------------- GENERALIZED SECTION SIGNALLING HANDLING -----------------------*/
#define SSA_CALLBACK_SONET_SDH_SECTION_K1K2_INDEX  (0)
#define SSA_CALLBACK_SONET_SDH_SECTION_S1_INDEX    (1)
#define SSA_CALLBACK_LAST_INDEX                    (2)

void *section_signalling_announce_callbacks[SSA_CALLBACK_LAST_INDEX];
/*---------------------- GENERALIZED SECTION SIGNALLING HANDLING -----------------------*/

void OMIINO_FRAMER_TransmitMessageTo_OMNISHELL(char * pAnyMessage)
{
   return;
}

void OMIINO_FRAMER_Binary_TransmitMessageTo_OMNISHELL(char * pAnyMessage, int Length)
{
   return;
}

#define PEEK_RESPONSE_CALLBACK_INDEX		       (0)
#define PEEK_RESPONSE_CALLBACK_LAST_INDEX              (1)

void *peek_response_callback_array[PEEK_RESPONSE_CALLBACK_LAST_INDEX];

void WPX_UFE_FRAMER_PEEK_RESPONSE_CALLBACK(U8 iDevice, U32 Address, U32 Value)
{
   WPX_UFE_FRAMER_PEEK_RESPONSE_ANNOUNCE_TYPE callback;
   callback = peek_response_callback_array[PEEK_RESPONSE_CALLBACK_INDEX];

   if (callback != NULL)
      (callback)(iDevice, Address, Value);
}

#define BLOCK_POKE_VERIFY_RESPONSE_CALLBACK_INDEX             (0)
#define BLOCK_POKE_VERIFY_RESPONSE_CALLBACK_LAST_INDEX        (1)

void *block_poke_verify_response_callback_array[BLOCK_POKE_VERIFY_RESPONSE_CALLBACK_LAST_INDEX];

void WPX_UFE_FRAMER_BLOCK_POKE_VERIFY_RESPONSE_CALLBACK(U8 iDevice, U32 VerifyResult)
{
   WPX_UFE_FRAMER_BLOCK_POKE_VERIFY_RESPONSE_ANNOUNCE_TYPE callback;

   callback = block_poke_verify_response_callback_array[BLOCK_POKE_VERIFY_RESPONSE_CALLBACK_INDEX];
   if (callback != NULL)
      (callback)( iDevice,VerifyResult);
}

#define ALARM_MAP_RESPONSE_CALLBACK_INDEX             (0)
#define ALARM_MAP_RESPONSE_CALLBACK_LAST_INDEX        (1)

void *alarm_map_response_callback_array[ALARM_MAP_RESPONSE_CALLBACK_LAST_INDEX];

#define SS_BITS_RESPONSE_CALLBACK_INDEX             (0)
#define SS_BITS_RESPONSE_CALLBACK_LAST_INDEX        (1)

void *ss_bits_response_callback_array[SS_BITS_RESPONSE_CALLBACK_LAST_INDEX];


void WPX_UFE_FRAMER_ALARM_MAP_RESPONSE_CALLBACK(U8 iDevice)
{
   WPX_UFE_FRAMER_ALARM_MAP_RESPONSE_ANNOUNCE_TYPE callback;

   callback = alarm_map_response_callback_array[ALARM_MAP_RESPONSE_CALLBACK_INDEX];
   if (callback != NULL)
      (callback)(iDevice);
}

void WPX_UFE_FRAMER_SS_BITS_RESPONSE_CALLBACK(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType,U8 Value)
{
   WPX_UFE_FRAMER_SS_BITS_ANNOUNCE_TYPE callback;

   callback = ss_bits_response_callback_array[SS_BITS_RESPONSE_CALLBACK_INDEX];
   if (callback != NULL)
      (callback)(iDevice,pCommon_SDH_SONET_LineEndpointType,Value);
}
#define FRAMER_LOCK_CALLBACK_INDEX             (0)
#define FRAMER_LOCK_CALLBACK_LAST_INDEX        (1)

void *framer_lock_callback_array[FRAMER_LOCK_CALLBACK_LAST_INDEX];

void WPX_UFE_FRAMER_LOCK_FUNCTION(void)
{
   WPX_UFE_FRAMER_LOCK_CALLBACK_TYPE callback;

   callback = framer_lock_callback_array[FRAMER_LOCK_CALLBACK_INDEX];
   if (callback != NULL)
      (callback)();
}

void (*EarlyFatalError)(int ErrorCode);

/* WDDI semaphore functions */
extern WP_U32 WPL_LockKeyInit(WP_U32 i_key, WP_U32 *i_lock);
extern WP_U32 WPL_Lock(WP_U32 i_key, WP_U32 *i_lock);
extern WP_U32 WPL_Unlock(WP_U32 i_key, WP_U32 *i_lock);

/* Flexmux base address */
extern wufei_mem_space_block ufe_mem_space_block[];
wpi_match_mask wpx_ufe_framer_save_mask[WUFE_MAX_N_UFE]={{0,0,0},{0,0,0},{0,0,0},{0,0,0}};

/* flexmux log */
static WP_CHAR wpx_flexmux_log[WPX_FLEXMUX_LOG_SIZE];

typedef struct UFE_BOARD_TYPE
{
    int                                                     device_is_ready[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES];
    WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE                  mailbox_memory[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES];
    WPX_UFE_FRAMER_ENVIRONMENT_TABLE_TYPE                   env_table;
    WPX_UFE_FRAMER_AUTONOMOUS_OUTPUT_TABLE_TYPE             autonomous_output_table;
    WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE performance_monitoring_device_table;
    WPX_UFE_FRAMER_DEVICE_LEVEL_DIAGNOSTIC_TABLE_TYPE       device_level_diagnostic_table;
    WPX_UFE_FRAMER_OMNISPY_TABLE_TYPE                       omnispy_table;
} UFE_BOARD_TYPE;

/* Flexmux device registry */
UFE_BOARD_TYPE My_UFE_Board;

U32 wpx_flexmux_lock = WPX_FLEXMUX_REG_UNLOCKED;

/* Place holder for PM tables */
WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE local_pm_device_table;

/* set match and mask registers in the WP in order to access the flexmux device */
/* This function is dangerous:  if other processes/threads are also modifying these registers, there is no
 * guarantee that the mask and match values remain in-sync (that is, the mask value and the match value could
 * belong to different pairs. The "check-then-install" part needs to be atomic. 
 */

void WPX_UFE_FRAMER_FlexmuxSetBank(U32 flexmux_id, wpi_match_mask* save, U32 connector_index)
{
#ifdef WPX_BOARD_SUPPORT_ENABLED

   U32 *if_addr;

   /* Save current match/mask, install UFE values. */
   wpx_ufe_framer_save_mask[flexmux_id].match_address = wpi_board_data.match_mask->ufe[connector_index].match_address;

   /* Get the virtual address of the match/mask register pair. */
   if_addr = (U32 *) WPL_RIF_VIRTUAL(0, wpi_board_data.match_mask->ufe[connector_index].match_address);

   /* Save the current match/mask values. */
   WP_MEM_GET(wpx_ufe_framer_save_mask[flexmux_id].match_value, if_addr[0]);
   WP_MEM_GET(wpx_ufe_framer_save_mask[flexmux_id].mask_value, if_addr[1]);

   /* Install the desired match/mask values. */
   if (wpi_board_data.match_mask->ufe[connector_index].mask_value != wpx_ufe_framer_save_mask[flexmux_id].mask_value ||
       wpi_board_data.match_mask->ufe[connector_index].match_value != wpx_ufe_framer_save_mask[flexmux_id].match_value)
   {
      WP_MEM_SET(if_addr[1], wpi_board_data.match_mask->ufe[connector_index].mask_value);
      WP_MEM_SET(if_addr[0], wpi_board_data.match_mask->ufe[connector_index].match_value);
   }
#endif
}

/* restore original values of match and mask registers in the WP */
/* This function is dangerous:  if other processes/threads are also modifying these registers, there is no
 * guarantee that the mask and match values remain in-sync (that is, the mask value and the match value could
 * belong to different pairs. The "check-then-install" part needs to be atomic. */

void WPX_UFE_FRAMER_FlexmuxRestoreBank(U32 flexmux_id, wpi_match_mask* save)
{
#ifdef WPX_BOARD_SUPPORT_ENABLED

   /* reload the previous match/mask, before UFE\FRAMER values. */
   U32 *if_addr;
   U32 current_mask, current_match;

   if (wpx_ufe_framer_save_mask[flexmux_id].match_address == 0xffffffff)
      return;

   /* Get the virtual address of the match/mask register pair. */
   if_addr = (U32 *) WPL_RIF_VIRTUAL(0, wpx_ufe_framer_save_mask[flexmux_id].match_address);

   /* Get the current values */
   WP_MEM_GET(current_match, if_addr[0]);
   WP_MEM_GET(current_mask, if_addr[1]);

   if (current_match != wpx_ufe_framer_save_mask[flexmux_id].match_value ||
       current_mask != wpx_ufe_framer_save_mask[flexmux_id].mask_value)
   {
      /* Install the desired match/mask values. */
      WP_MEM_SET(if_addr[1], wpx_ufe_framer_save_mask[flexmux_id].mask_value);
      WP_MEM_SET(if_addr[0], wpx_ufe_framer_save_mask[flexmux_id].match_value);
   }

#endif
}

/* get the base address of the Flexmux device */
static U32 WPX_UFE_FRAMER_FlexmuxBaseAddrGet(U32 flexmux_id, U32 connector_id)
{
   if (connector_id >= APP_UFE_MEM_SPACE_CONNECTORS)
   {
      WPX_PRINT_ERR((wpx_flexmux_log,
                     "WPX_UFE_FRAMER_FlexmuxBaseAddrGet: invalid connector_id %d, max=%d\n",
                     connector_id, APP_UFE_MEM_SPACE_CONNECTORS));
      return 0;
   }

   if (flexmux_id >= WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES)
   {
      WPX_PRINT_ERR((wpx_flexmux_log,
                     "WPX_UFE_FRAMER_FlexmuxBaseAddrGet: flexmux_id(%d) >= %d\n",
                     flexmux_id, WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES));
      return 0;
   }

   return WPL_PHY_VIRTUAL(0, ufe_mem_space_block[connector_id].ufe_bases[0].flexmux_base);
}

/* In case of fatal error log the info in the Flexmux logfile and send a message to Omnishell */
void WPX_UFE_FRAMER_FlexmuxTerminateOnError(char * pFatalErrorMessage)
{
   if(strstr(pFatalErrorMessage, "00000175")==NULL)
   {
      WPX_PRINT_ERR((wpx_flexmux_log,
                     "WPX_UFE_FRAMER_FlexmuxTerminateOnError: %s in line %d\n",
                     pFatalErrorMessage, __LINE__));
   }
}

/* Flexmux logging function */
void WPX_UFE_FRAMER_FlexmuxLog(char *pTraceMessage)
{
   WPX_PRINT_DISP((wpx_flexmux_log,
                   "WPX_UFE_FRAMER_FlexmuxLog: %s in line %d\n",
                   pTraceMessage, __LINE__));
}

/* Dummy function for creating the Flexmux semaphore (WDDI semaphore is already created in WP_SysCommit() */
void WPX_UFE_FRAMER_FlexmuxCreateLockHandle(U8 *pNewHandle, char *pHandleName)
{
   *pNewHandle = 1;
}

/* Lock Flexmux semaphore */
void WPX_UFE_FRAMER_FlexmuxLock(U8 AnyHandle)
{
   WPX_FLEXMUX_LOCK(&wpx_flexmux_lock);
}

/* Unlock Flexmux semaphore */
void WPX_UFE_FRAMER_FlexmuxUnlock(U8 AnyHandle)
{
   WPX_FLEXMUX_UNLOCK(&wpx_flexmux_lock);
}

/* Initialize Flexmux addresses */
void WPX_UFE_FRAMER_FlexmuxMailBoxInit(U32 upi_index,
                                       U8 flexmux_id,
                                       UFE_BOARD_TYPE *flexmux_cfg)
{
   U32 flexmux_base = WPX_UFE_FRAMER_FlexmuxBaseAddrGet(flexmux_id, upi_index);

   memset(&flexmux_cfg->mailbox_memory[flexmux_id], 0, sizeof(WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE));

   /* set Mailbox physical addresses */
   flexmux_cfg->mailbox_memory[flexmux_id].Address[0].CTRL=(volatile U32 *) (flexmux_base + 0x0);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[1].CTRL=(volatile U32 *) (flexmux_base + 0x8);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[2].CTRL=(volatile U32 *) (flexmux_base + 0x10);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[3].CTRL=(volatile U32 *) (flexmux_base + 0x18);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[4].CTRL=(volatile U32 *) (flexmux_base + 0x20);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[5].CTRL=(volatile U32 *) (flexmux_base + 0x28);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[6].CTRL=(volatile U32 *) (flexmux_base + 0x30);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[7].CTRL=(volatile U32 *) (flexmux_base + 0x38);

   flexmux_cfg->mailbox_memory[flexmux_id].Address[0].PEND=(volatile U32 *) (flexmux_base + 0x40);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[1].PEND=(volatile U32 *) (flexmux_base + 0x48);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[2].PEND=(volatile U32 *) (flexmux_base + 0x50);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[3].PEND=(volatile U32 *) (flexmux_base + 0x58);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[4].PEND=(volatile U32 *) (flexmux_base + 0x60);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[5].PEND=(volatile U32 *) (flexmux_base + 0x68);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[6].PEND=(volatile U32 *) (flexmux_base + 0x70);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[7].PEND=(volatile U32 *) (flexmux_base + 0x78);

   flexmux_cfg->mailbox_memory[flexmux_id].Address[0].ENABLE=(volatile U32 *) (flexmux_base + 0x80);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[1].ENABLE=(volatile U32 *) (flexmux_base + 0x88);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[2].ENABLE=(volatile U32 *) (flexmux_base + 0x90);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[3].ENABLE=(volatile U32 *) (flexmux_base + 0x98);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[4].ENABLE=(volatile U32 *) (flexmux_base + 0xA0);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[5].ENABLE=(volatile U32 *) (flexmux_base + 0xA8);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[6].ENABLE=(volatile U32 *) (flexmux_base + 0xB0);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[7].ENABLE=(volatile U32 *) (flexmux_base + 0xB8);

   flexmux_cfg->mailbox_memory[flexmux_id].Address[0].DDR=(volatile U32 *) (flexmux_base + 0xC0);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[1].DDR=(volatile U32 *) (flexmux_base + 0xC8);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[2].DDR=(volatile U32 *) (flexmux_base + 0xD0);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[3].DDR=(volatile U32 *) (flexmux_base + 0xD8);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[4].DDR=(volatile U32 *) (flexmux_base + 0xE0);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[5].DDR=(volatile U32 *) (flexmux_base + 0xE8);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[6].DDR=(volatile U32 *) (flexmux_base + 0xF0);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[7].DDR=(volatile U32 *) (flexmux_base + 0xF8);

   flexmux_cfg->mailbox_memory[flexmux_id].Address[0].RAM=(volatile U32 *) (flexmux_base + 0x800);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[1].RAM=(volatile U32 *) (flexmux_base + 0x884);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[2].RAM=(volatile U32 *) (flexmux_base + 0x908);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[3].RAM=(volatile U32 *) (flexmux_base + 0x98C);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[4].RAM=(volatile U32 *) (flexmux_base + 0xA10);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[5].RAM=(volatile U32 *) (flexmux_base + 0xA94);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[6].RAM=(volatile U32 *) (flexmux_base + 0xB18);
   flexmux_cfg->mailbox_memory[flexmux_id].Address[7].RAM=(volatile U32 *) (flexmux_base + 0xB9C);
}

#if WTI_COLLECT_TIMING_INFORMATION
extern void record_action(WP_U32 type, WP_U32 time);
extern WP_U32 WP_TimeRead(void);
#endif

/* Autonomous output table callbacks - Log all events raised from Flexmux firmware in the Flexmux log */
void WPX_UFE_FRAMER_Flexmux_AutonomousAlarmTable_SONET_SDH_PortAlarmAnnounce(U8 iDevice,
                                                                             U8 iLinePort,
                                                                             U8 AlarmCategory,
                                                                             U8 IsAsserted)
{
   WPX_UFE_FRAMER_SONET_SDH_PORT_ALARM_ANNOUNCE_TYPE callback;

#if WTI_FLEXMUX_DBG_MODE
   WPX_PRINT_DISP((wpx_flexmux_log,
                   "SONET_SDH_PortAlarm event occured!: iDevice %d, iLinePort %d, "
                   "AlarmCategory %d, IsAsserted %d\n",
                   (U32) iDevice, (U32)iLinePort, (U32)AlarmCategory, (U32)IsAsserted));
#endif

#if WTI_COLLECT_TIMING_INFORMATION
   record_action(1, WP_TimeRead());
#endif

   callback = alarm_callbacks[ALARM_CALLBACK_SONET_SDH_PORT_INDEX];

   if (callback != NULL)
      (callback)(iDevice, iLinePort, AlarmCategory, IsAsserted);
}

void WPX_UFE_FRAMER_Flexmux_AutonomousAlarmTable_SONET_SDH_PathAlarmAnnounce(
                  U8 iDevice,
                  WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pLineEndpoint,
                  U8 AlarmCategory,
                  U8 IsAsserted)
{
   WPX_UFE_FRAMER_SONET_SDH_PATH_ALARM_ANNOUNCE_TYPE callback;

#if WTI_FLEXMUX_DBG_MODE
   WPX_PRINT_DISP((wpx_flexmux_log,
                   "SONET_SDH_PathAlarm event occured!: iDevice %d, "
                   "WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pLineEndpoint %x, "
                   "AlarmCategory %d, IsAsserted %d\n",
                   (U32)iDevice, (U32) pLineEndpoint, (U32) AlarmCategory, (U32) IsAsserted));
#endif

#if WTI_COLLECT_TIMING_INFORMATION
   record_action(2, WP_TimeRead());
#endif

   callback = alarm_callbacks[ALARM_CALLBACK_SONET_SDH_PATH_INDEX];

   if (callback != NULL)
      (callback)(iDevice, pLineEndpoint, AlarmCategory, IsAsserted);
}

void WPX_UFE_FRAMER_Flexmux_AutonomousAlarmTable_DiscreteClientPortAlarmAnnounce(U8 iDevice,
                                                                                 U8 iDiscreteClientPort,
                                                                                 U8 AlarmCategory,
                                                                                 U8 IsAsserted)
{
   WPX_UFE_FRAMER_DISCRETE_CLIENT_PORT_ALARM_ANNOUNCE_TYPE callback;

#if WTI_FLEXMUX_DBG_MODE
   WPX_PRINT_DISP((wpx_flexmux_log,
                   "DiscreteClientPortAlarm event occured!: iDevice %d, iDiscreteClientPort %d, "
                   "AlarmCategory %d, IsAsserted %d\n",
                   (U32) iDevice, (U32)iDiscreteClientPort, (U32)AlarmCategory, (U32)IsAsserted));
#endif

#if WTI_COLLECT_TIMING_INFORMATION
   record_action(3, WP_TimeRead());
#endif

   callback = alarm_callbacks[ALARM_CALLBACK_DISCRETE_CLIENT_PORT_INDEX];

   if (callback != NULL)
      (callback)(iDevice, iDiscreteClientPort, AlarmCategory, IsAsserted);
}

void WPX_UFE_FRAMER_Flexmux_AutonomousAlarmTable_SocketClientSidePortAlarmAnnounce(
                  U8 iDevice,
                  U32 iSocketClientPDH,
                  U8 AlarmCategory,
                  U8 IsAsserted)
{
   WPX_UFE_FRAMER_SOCKET_CLIENT_PDH_ALARM_ANNOUNCE_TYPE callback;

#if WTI_FLEXMUX_DBG_MODE
   WPX_PRINT_DISP((wpx_flexmux_log,
                   "SocketClientSidePortAlarm event occured!: iDevice %d, iSocketClientPDH %d, "
                   "AlarmCategory %d, IsAsserted %d\n",
                   (U32) iDevice, iSocketClientPDH, (U32)AlarmCategory, (U32)IsAsserted));
#endif

#if WTI_COLLECT_TIMING_INFORMATION
   record_action(4, WP_TimeRead());
#endif

   callback = alarm_callbacks[ALARM_CALLBACK_SOCKET_PDH_INDEX];

   if (callback != NULL)
      (callback)(iDevice, iSocketClientPDH, AlarmCategory, IsAsserted);
}

void WPX_UFE_FRAMER_Flexmux_AutonomousPerformanceMonitoringTable_SONET_SDH_PortPerformanceMonitoringAnnounce(
                  U8 iDevice,
                  U8 iTableIndex,
                  U32 TimeStamp)
{
   WPX_UFE_FRAMER_SONET_SDH_PORT_PERFORMANCE_MONITORING_ANNOUNCE_TYPE callback;

#if WTI_FLEXMUX_DBG_MODE
   WPX_PRINT_DISP((wpx_flexmux_log,
                   "SONET_SDH_PortPerformanceMonitoring event occured!: iDevice %d, iTableIndex %d, TimeStamp %d\n",
                   (U32) iDevice, (U32)iTableIndex, TimeStamp));
#endif

#if WTI_COLLECT_TIMING_INFORMATION
   record_action(5, WP_TimeRead());
#endif

   callback = performance_monitoring_announce_callbacks[PMA_CALLBACK_SONET_SDH_PORT_INDEX];

   if (callback != NULL)
      (callback)(iDevice, iTableIndex, TimeStamp);
}

void WPX_UFE_FRAMER_Flexmux_AutonomousPerformanceMonitoringTable_SONET_SDH_HighOrderPathPerformanceMonitoringAnnounce(
                  U8 iDevice,
                  U8 iTableIndex,
                  U32 TimeStamp)
{
   WPX_UFE_FRAMER_SONET_SDH_HO_PATH_PERFORMANCE_MONITORING_ANNOUNCE_TYPE callback;

#if WTI_FLEXMUX_DBG_MODE
   WPX_PRINT_DISP((wpx_flexmux_log,
                   "SONET_SDH_HighOrderPathPerformanceMonitoring event occured!: "
                   "iDevice %d, iTableIndex %d, TimeStamp %d\n",
                   (U32) iDevice, (U32)iTableIndex, TimeStamp));
#endif

#if WTI_COLLECT_TIMING_INFORMATION
   record_action(6, WP_TimeRead());
#endif

   callback = performance_monitoring_announce_callbacks[PMA_CALLBACK_SONET_SDH_HO_PATH_INDEX];

   if (callback != NULL)
      (callback)(iDevice, iTableIndex, TimeStamp);
}

void WPX_UFE_FRAMER_Flexmux_AutonomousPerformanceMonitoringTable_SONET_SDH_LowOrderPathPerformanceMonitoringAnnounce(
                  U8 iDevice,
                  U8 iTableIndex,
                  U32 TimeStamp)
{
   WPX_UFE_FRAMER_SONET_SDH_LO_PATH_PERFORMANCE_MONITORING_ANNOUNCE_TYPE callback;

#if WTI_FLEXMUX_DBG_MODE
   WPX_PRINT_DISP((wpx_flexmux_log,
                   "SONET_SDH_LowOrderPathPerformanceMonitoring event occured!: "
                   "iDevice %d, iTableIndex %d, TimeStamp %d\n",
                   (U32) iDevice, (U32)iTableIndex, TimeStamp));
#endif

#if WTI_COLLECT_TIMING_INFORMATION
   record_action(7, WP_TimeRead());
#endif

   callback = performance_monitoring_announce_callbacks[PMA_CALLBACK_SONET_SDH_LO_PATH_INDEX];

   if (callback != NULL)
      (callback)(iDevice, iTableIndex, TimeStamp);
}

void WPX_UFE_FRAMER_Flexmux_AutonomousPerformanceMonitoringTable_PDH_PerformanceMonitoringAnnounce(
                  U8 iDevice,
                  U8 iTableIndex,
                  U32 TimeStamp)
{
   WPX_UFE_FRAMER_PDH_PERFORMANCE_MONITORING_ANNOUNCE_TYPE callback;

#if WTI_FLEXMUX_DBG_MODE
   WPX_PRINT_DISP((wpx_flexmux_log,
                   "PDH_PerformanceMonitoring event occured!: " "iDevice %d, iTableIndex %d, TimeStamp %d\n",
                   (U32) iDevice, (U32)iTableIndex, TimeStamp));
#endif

#if WTI_COLLECT_TIMING_INFORMATION
   record_action(8, WP_TimeRead());
#endif

   callback = performance_monitoring_announce_callbacks[PMA_CALLBACK_SOCKET_PDH_INDEX];

   if (callback != NULL)
      (callback)(iDevice, iTableIndex, TimeStamp);
}

void WPX_UFE_FRAMER_Flexmux_AutonomousPerformanceMonitoringTable_DiscreteClientPortPerformanceMonitoringAnnounce(
                  U8 iDevice,
                  U8 iTableIndex,
                  U32 TimeStamp)
{
   WPX_UFE_FRAMER_DISCRETE_CLIENT_PERFORMANCE_MONITORING_ANNOUNCE_TYPE callback;

#if WTI_FLEXMUX_DBG_MODE
   WPX_PRINT_DISP((wpx_flexmux_log,
                   "DiscreteClientPortPerformanceMonitoring event occured!: "
                   "iDevice %d, iTableIndex %d, TimeStamp %d\n",
                   (U32) iDevice, (U32)iTableIndex, TimeStamp));
#endif

#if WTI_COLLECT_TIMING_INFORMATION
   record_action(9, WP_TimeRead());
#endif

   callback = performance_monitoring_announce_callbacks[PMA_CALLBACK_DISCRETE_CLIENT_INDEX];

   if (callback != NULL)
      (callback)(iDevice, iTableIndex, TimeStamp);
}

void WPX_UFE_FRAMER_Flexmux_AutonomousSignallingTable_LineSideSectionK1K2Announce(U8 iDevice, U8 iLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE *pRX_K1K2)
{
   WPX_UFE_FRAMER_SONET_SDH_SECTION_K1K2_ANNOUNCE_TYPE callback;

#if WTI_FLEXMUX_DBG_MODE
   WPX_PRINT_DISP((wpx_flexmux_log,
                   "SignallingTable_LineSideSectionK1K2 event occured!: iDevice %d, iLinePort %d, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE *pRX_K1K2 %x\n",
                   (U32)iDevice, (U32)iLinePort, (U32)pRX_K1K2));
#endif

#if WTI_COLLECT_TIMING_INFORMATION
   record_action(10, WP_TimeRead());
#endif

   callback = section_signalling_announce_callbacks[SSA_CALLBACK_SONET_SDH_SECTION_K1K2_INDEX];

   if (callback != NULL)
      (callback)(iDevice, iLinePort, pRX_K1K2);
}

void WPX_UFE_FRAMER_Flexmux_AutonomousSignallingTable_LineSideSectionS1Announce(U8 iDevice, U8 iLinePort, U8 S1)
{
   WPX_UFE_FRAMER_SONET_SDH_SECTION_S1_ANNOUNCE_TYPE callback;

#if WTI_FLEXMUX_DBG_MODE
   WPX_PRINT_DISP((wpx_flexmux_log,
                   "SignallingTable_LineSideSectionS1 event occured!: iDevice %d, iLinePort %d, S1 %d\n",
                   (U32)iDevice, (U32)iLinePort, (U32)S1));
#endif

#if WTI_COLLECT_TIMING_INFORMATION
   record_action(11, WP_TimeRead());
#endif

   callback = section_signalling_announce_callbacks[SSA_CALLBACK_SONET_SDH_SECTION_S1_INDEX];

   if (callback != NULL)
      (callback)(iDevice, iLinePort, S1);
}

void WPX_UFE_FRAMER_FlexmuxDiagnosticTableDevice(U8 iDevice, U8 DiagnosticAlarmCategory, U8 IsAsserted)
{
#if WTI_COLLECT_TIMING_INFORMATION
   record_action(12, WP_TimeRead());
#endif

#if WTI_FLEXMUX_DBG_MODE
   WPX_PRINT_DISP((wpx_flexmux_log,
                   "DiagnosticTableDevice event occured!: iDevice %d, DiagnosticAlarmCategory %d, IsAsserted %d\n",
                   (U32)iDevice, (U32)DiagnosticAlarmCategory, (U32)IsAsserted));
#endif
}

void WPX_UFE_FRAMER_FlexmuxDeviceIsReady(U8 iDevice, unsigned char Reported_HardwareBuildVariant)
{
#if WTI_FLEXMUX_DBG_MODE
   WPX_PRINT_DISP((wpx_flexmux_log,
                   "DeviceReady event occured!: iDevice %d HardwareBuildVariant %d\n",
                   (U32)iDevice,
                   (U32)Reported_HardwareBuildVariant));
#endif

   /* set device ready in relevant Flexmux registry */
   My_UFE_Board.device_is_ready[iDevice] = WPX_FLEXMUX_READY;
}

void WPX_UFE_FRAMER_FlexmuxFirmwareRestart(U8 iDevice, unsigned char Reported_HardwareBuildVariant)
{
#if WTI_FLEXMUX_DBG_MODE
   WPX_PRINT_DISP((wpx_flexmux_log,
                   "Firmware Restart event occured!: iDevice %d HardwareBuildVariant %d\n",
                   (U32)iDevice,
                   (U32)Reported_HardwareBuildVariant));
#endif
}

void WPX_UFE_FRAMER_FlexmuxOmnispyNorthboundAnnounce(U8 iDevice, U32 * pMessage, U32 Length)
{
#if WTI_FLEXMUX_DBG_MODE
   WPX_PRINT_DISP((wpx_flexmux_log,
                   "OmnispyNorthbound event occured!: iDevice %d, pMessage %x, Length %d\n",
                   (U32)iDevice, (U32)pMessage, Length));
#endif
}

/* Init Flexmux autononmous output table with UFE callback functions (currently only log the events) */
void WPX_UFE_FRAMER_FlexmuxAutonomousOutputTableInit(UFE_BOARD_TYPE *flexmux_cfg)
{
   memset(&flexmux_cfg->autonomous_output_table, 0, sizeof(WPX_UFE_FRAMER_AUTONOMOUS_OUTPUT_TABLE_TYPE));
   memset(alarm_callbacks, 0, sizeof(alarm_callbacks));

   flexmux_cfg->autonomous_output_table.AutonomousAlarmTable.DiscreteClientPortAlarmAnnounce =
      WPX_UFE_FRAMER_Flexmux_AutonomousAlarmTable_DiscreteClientPortAlarmAnnounce;
   flexmux_cfg->autonomous_output_table.AutonomousAlarmTable.SocketClientSidePortAlarmAnnounce =
      WPX_UFE_FRAMER_Flexmux_AutonomousAlarmTable_SocketClientSidePortAlarmAnnounce;
   flexmux_cfg->autonomous_output_table.AutonomousAlarmTable.SONET_SDH_PathAlarmAnnounce =
      WPX_UFE_FRAMER_Flexmux_AutonomousAlarmTable_SONET_SDH_PathAlarmAnnounce;
   flexmux_cfg->autonomous_output_table.AutonomousAlarmTable.SONET_SDH_PortAlarmAnnounce =
      WPX_UFE_FRAMER_Flexmux_AutonomousAlarmTable_SONET_SDH_PortAlarmAnnounce;

   flexmux_cfg->autonomous_output_table.AutonomousPerformanceMonitoringTable.DiscreteClientPortPerformanceMonitoringAnnounce =
      WPX_UFE_FRAMER_Flexmux_AutonomousPerformanceMonitoringTable_DiscreteClientPortPerformanceMonitoringAnnounce;
   flexmux_cfg->autonomous_output_table.AutonomousPerformanceMonitoringTable.SONET_SDH_HighOrderPathPerformanceMonitoringAnnounce =
      WPX_UFE_FRAMER_Flexmux_AutonomousPerformanceMonitoringTable_SONET_SDH_HighOrderPathPerformanceMonitoringAnnounce;
   flexmux_cfg->autonomous_output_table.AutonomousPerformanceMonitoringTable.SONET_SDH_LowOrderPathPerformanceMonitoringAnnounce =
      WPX_UFE_FRAMER_Flexmux_AutonomousPerformanceMonitoringTable_SONET_SDH_LowOrderPathPerformanceMonitoringAnnounce;
   flexmux_cfg->autonomous_output_table.AutonomousPerformanceMonitoringTable.SONET_SDH_PortPerformanceMonitoringAnnounce =
      WPX_UFE_FRAMER_Flexmux_AutonomousPerformanceMonitoringTable_SONET_SDH_PortPerformanceMonitoringAnnounce;

   flexmux_cfg->autonomous_output_table.AutonomousSignallingTable.LineSideSectionK1K2Announce =
      WPX_UFE_FRAMER_Flexmux_AutonomousSignallingTable_LineSideSectionK1K2Announce;
   flexmux_cfg->autonomous_output_table.AutonomousSignallingTable.LineSideSectionS1Announce =
      WPX_UFE_FRAMER_Flexmux_AutonomousSignallingTable_LineSideSectionS1Announce;
   flexmux_cfg->autonomous_output_table.AutonomousPerformanceMonitoringTable.PDH_PerformanceMonitoringAnnounce =
      WPX_UFE_FRAMER_Flexmux_AutonomousPerformanceMonitoringTable_PDH_PerformanceMonitoringAnnounce;

}

/* Init Flexmux environment table: Bind in functions for creating, locking and unlocking the WDDI semaphore,
   Flexmux log functions and function for termination in case of fatal error */
void WPX_UFE_FRAMER_FlexmuxEnvironmentTableInit(UFE_BOARD_TYPE *flexmux_cfg)
{
   memset(&flexmux_cfg->env_table, 0, sizeof(WPX_UFE_FRAMER_ENVIRONMENT_TABLE_TYPE));

   /* Set environment callbacks */
   flexmux_cfg->env_table.EnterCriticalRegion = WPX_UFE_FRAMER_FlexmuxLock;
   flexmux_cfg->env_table.LeaveCriticalRegion = WPX_UFE_FRAMER_FlexmuxUnlock;
   flexmux_cfg->env_table.CriticalRegionCreateHandle = WPX_UFE_FRAMER_FlexmuxCreateLockHandle;
   flexmux_cfg->env_table.Trace = WPX_UFE_FRAMER_FlexmuxLog;
   flexmux_cfg->env_table.FatalErrorHandler = WPX_UFE_FRAMER_FlexmuxTerminateOnError;
}

/* Init Flexmux performance monitoring function - currently PM is not used */
void WPX_UFE_FRAMER_FlexmuxPmTableInit(UFE_BOARD_TYPE *flexmux_cfg)
{
   memcpy(&flexmux_cfg->performance_monitoring_device_table, &local_pm_device_table,
         sizeof(WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE));

   memset(&local_pm_device_table, 0, sizeof(WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE));
}

/* Init Flexmux device level diagnostic table with UFE callback */
void WPX_UFE_FRAMER_FlexmuxDeviceLevelDiagnosticTableInit(UFE_BOARD_TYPE *flexmux_cfg)
{
   memset(&flexmux_cfg->device_level_diagnostic_table, 0, sizeof(WPX_UFE_FRAMER_DEVICE_LEVEL_DIAGNOSTIC_TABLE_TYPE));

   /* Set device level diagnostic callbacks */
   flexmux_cfg->device_level_diagnostic_table.DeviceDiagnostic = WPX_UFE_FRAMER_FlexmuxDiagnosticTableDevice;
   flexmux_cfg->device_level_diagnostic_table.DeviceReady = WPX_UFE_FRAMER_FlexmuxDeviceIsReady;
   flexmux_cfg->device_level_diagnostic_table.FirmwareRestart=WPX_UFE_FRAMER_FlexmuxFirmwareRestart;

   /*Assign the peek response callback*/
   flexmux_cfg->device_level_diagnostic_table.PeekResponse = WPX_UFE_FRAMER_PEEK_RESPONSE_CALLBACK;

   flexmux_cfg->device_level_diagnostic_table.BlockPokeVerifyResponse = WPX_UFE_FRAMER_BLOCK_POKE_VERIFY_RESPONSE_CALLBACK;
   
   /*Assign the alarm map response callback*/
   flexmux_cfg->device_level_diagnostic_table.AlarmMapResponse = WPX_UFE_FRAMER_ALARM_MAP_RESPONSE_CALLBACK;

   /*Assign the S0S1 bit get Rx callback*/
   flexmux_cfg->device_level_diagnostic_table.SSBitsGetRxResponse = WPX_UFE_FRAMER_SS_BITS_RESPONSE_CALLBACK;
}

/* Init Omnispy table (logs Omnispy messages) */
void WPX_UFE_FRAMER_FlexmuxOmnispyTableInit(UFE_BOARD_TYPE *flexmux_cfg)
{
   memset(&flexmux_cfg->omnispy_table, 0, sizeof(WPX_UFE_FRAMER_OMNISPY_TABLE_TYPE));

   /* Set Omniispy callback */
   flexmux_cfg->omnispy_table.OmniSpyNorthBoundAnnounce = WPX_UFE_FRAMER_FlexmuxOmnispyNorthboundAnnounce;
}

void WPX_UFE_FRAMER_LLC_Register_InitializeNullMessage(U8 flexmux_id,
                                                       U8 flexmux_connector_id,
                                                       volatile WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE *pMemoryMap,
                                                       U8 iCommsChannel)
{
   int i;
   volatile unsigned int * pRAM=pMemoryMap->Address[iCommsChannel].RAM;
#ifndef XALYO_FRAMER_ACCESS
    *(pRAM++)=(unsigned int) OMIINO_MESSAGE_CATALOGUE_NULL_MESSAGE;
    *(pRAM++)=0x00000000;   /* TID          */
    *(pRAM++)=0x00000000;   /* Length       */
    *(pRAM++)=0x00000000;   /* Attributes   */

    for(i=4;i<WPX_UFE_FRAMER_MAX_WORDS_IN_MESSAGE;i++)
    {
        *(pRAM++)=0xDEAD0000|(U32)iCommsChannel;
    }
#else
   /* Set match and mask registers on the Winpath */
   WPX_UFE_FRAMER_FlexmuxSetBank(flexmux_id, &wpx_ufe_framer_save_mask[flexmux_id], flexmux_connector_id);
   
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
   /* Restore previous match and mask registers values */
   WPX_UFE_FRAMER_FlexmuxRestoreBank(flexmux_id, &wpx_ufe_framer_save_mask[flexmux_id]);
#endif
}

/* Enable the mailboxes communication channels (used for transferring information between the Flexmux device driver and the firmware */
void WPX_UFE_FRAMER_Enable_MailboxCommunicationChannels(U32 flexmux_id, U32 flexmux_connector_id)
{
#ifdef XALYO_FRAMER_ACCESS    
    volatile unsigned int *ptr;
#endif

    if(0x00000000== *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION].CTRL)
    {
       WPX_UFE_FRAMER_LLC_Register_InitializeNullMessage(flexmux_id, flexmux_connector_id, &OMIINO_RAM.Device[flexmux_id].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION);
#ifndef XALYO_FRAMER_ACCESS
        *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION].CTRL=0x00000001;
#else
       ptr = OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION].CTRL;
       WP_MEM_SET(ptr[0], 1);
#endif
    }

    if(0x00000000== *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_SIGNALLING].CTRL)
    {
        WPX_UFE_FRAMER_LLC_Register_InitializeNullMessage(flexmux_id, flexmux_connector_id, &OMIINO_RAM.Device[flexmux_id].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_SIGNALLING);
#ifndef XALYO_FRAMER_ACCESS
        *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_SIGNALLING].CTRL=0x00000001;
#else
        ptr = OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_SIGNALLING].CTRL;
        WP_MEM_SET(ptr[0], 1);
#endif
    }

    if(0x00000000== *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY].CTRL)
    {
        WPX_UFE_FRAMER_LLC_Register_InitializeNullMessage(flexmux_id, flexmux_connector_id, &OMIINO_RAM.Device[flexmux_id].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY);
#ifndef XALYO_FRAMER_ACCESS
        *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY].CTRL=0x00000001;
#else
        ptr = OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY].CTRL;
        WP_MEM_SET(ptr[0], 1);
#endif
    }

    if(0x00000000 == *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_STATUS].CTRL)
    {
#ifndef XALYO_FRAMER_ACCESS
        *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_STATUS].CTRL=0x00000001; /* TODO Device hard coded */
#else
        ptr = OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_STATUS].CTRL;
        WP_MEM_SET(ptr[0], 1);
#endif
    }

    if(0x00000000 == *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_PERFORMANCE_MONITORING].CTRL)
    {
#ifndef XALYO_FRAMER_ACCESS
        *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_PERFORMANCE_MONITORING].CTRL=0x00000001; /* TODO Device hard coded */
#else
        ptr = OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_PERFORMANCE_MONITORING].CTRL;
        WP_MEM_SET(ptr[0], 1);
#endif
    }

    if(0x00000000 == *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_SIGNALLING].CTRL)
    {
#ifndef XALYO_FRAMER_ACCESS
        *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_SIGNALLING].CTRL=0x00000001; /* TODO Device hard coded */
#else
        ptr = OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_SIGNALLING].CTRL;
        WP_MEM_SET(ptr[0], 1);
#endif
    }

    if(0x00000000 == *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_OMNISPY].CTRL)
    {
#ifndef XALYO_FRAMER_ACCESS
        *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_OMNISPY].CTRL=0x00000001; /* TODO Device hard coded */
#else
        ptr = OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_OMNISPY].CTRL;
        WP_MEM_SET(ptr[0], 1);
#endif
    }

    if(0x00000000 == *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_AUXILIARY_OMNISPY].CTRL)
    {
#ifndef XALYO_FRAMER_ACCESS
        *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_AUXILIARY_OMNISPY].CTRL=0x00000001; /* TODO Device hard coded */
#else
        ptr = OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_AUXILIARY_OMNISPY].CTRL;
        WP_MEM_SET(ptr[0], 1);
#endif
    }

#ifndef XALYO_FRAMER_ACCESS
    *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_STATUS].ENABLE=0x00000001;
    *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_PERFORMANCE_MONITORING].ENABLE=0x00000001;
    *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_SIGNALLING].ENABLE=0x00000001;
    *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_OMNISPY].ENABLE=0x00000001;
    *OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_AUXILIARY_OMNISPY].ENABLE=0x00000001;
#else
    ptr = OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_STATUS].ENABLE;
    WP_MEM_SET(ptr[0], 1);
    ptr = OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_PERFORMANCE_MONITORING].ENABLE;
    WP_MEM_SET(ptr[0], 1);
    ptr = OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_SIGNALLING].ENABLE;
    WP_MEM_SET(ptr[0], 1);
    ptr = OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_OMNISPY].ENABLE;
    WP_MEM_SET(ptr[0], 1);
    ptr = OMIINO_RAM.Device[flexmux_id].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_AUXILIARY_OMNISPY].ENABLE;
    WP_MEM_SET(ptr[0], 1);
#endif
}

/* enable communication between firmware and driver */
void WPX_UFE_FRAMER_InitializeInterrupts(U32 flexmux_id, U32 flexmux_connector_id)
{
   WPX_UFE_FRAMER_Enable_MailboxCommunicationChannels(flexmux_id, flexmux_connector_id);
}

/* This function checks whether a message had arrived from the Flexmux firmware and calls the
   relevant callback function */
int WPX_UFE_FRAMER_ServiceMailbox(U32 flexmux_id, U32 flexmux_connector_id)
{
   /* Set match and mask registers on the Winpath */
   WPX_UFE_FRAMER_FlexmuxSetBank(flexmux_id, &wpx_ufe_framer_save_mask[flexmux_id], flexmux_connector_id);

   /* Handle new message that arrived from firmware  */
   WPX_FRMR_ServiceNorthboundLowLevelComms(flexmux_id);

   /* Restore previous match and mask registers values */
   WPX_UFE_FRAMER_FlexmuxRestoreBank(flexmux_id, &wpx_ufe_framer_save_mask[flexmux_id]);

   return 1;
}

/*---------------------- GENERALIZED ALARM HANDLING ------------------------------------*/

void WPX_FRMR_RegisterSonetSdhPortAlarmCallback(void *fnc)
{
   alarm_callbacks[ALARM_CALLBACK_SONET_SDH_PORT_INDEX] = fnc;
}

void WPX_FRMR_RegisterSonetSdhPathAlarmCallback(void *fnc)
{
   alarm_callbacks[ALARM_CALLBACK_SONET_SDH_PATH_INDEX] = fnc;
}

void WPX_FRMR_RegisterSocketClientPdhAlarmCallback(void *fnc)
{
   alarm_callbacks[ALARM_CALLBACK_SOCKET_PDH_INDEX] = fnc;
}

void WPX_FRMR_RegisterDiscreteClientPortAlarmCallback(void *fnc)
{
   alarm_callbacks[ALARM_CALLBACK_DISCRETE_CLIENT_PORT_INDEX] = fnc;
}

/*---------------------- GENERALIZED ALARM HANDLING ------------------------------------*/

/*---------------------- GENERALIZED PERFORMANCE MONITORING HANDLING -------------------*/

void WPX_FRMR_PM_RegisterSonetSdhPortAnnounceCallback(void *fnc)
{
   performance_monitoring_announce_callbacks[PMA_CALLBACK_SONET_SDH_PORT_INDEX] = fnc;
}

void WPX_FRMR_PM_RegisterSonetSdhHOPathAnnounceCallback(void *fnc)
{
   performance_monitoring_announce_callbacks[PMA_CALLBACK_SONET_SDH_HO_PATH_INDEX] = fnc;
}

void WPX_FRMR_PM_RegisterSonetSdhLOPathAnnounceCallback(void *fnc)
{
   performance_monitoring_announce_callbacks[PMA_CALLBACK_SONET_SDH_LO_PATH_INDEX] = fnc;
}

void WPX_FRMR_PM_RegisterSocketClientPdhAnnounceCallback(void *fnc)
{
   performance_monitoring_announce_callbacks[PMA_CALLBACK_SOCKET_PDH_INDEX] = fnc;
}

void WPX_FRMR_RegisterDiscreteClientPortAnnounceCallback(void *fnc)
{
   performance_monitoring_announce_callbacks[PMA_CALLBACK_DISCRETE_CLIENT_INDEX] = fnc;
}

/*---------------------- GENERALIZED PERFORMANCE MONITORING HANDLING -------------------*/

/*---------------------- GENERALIZED SECTION SIGNALLING HANDLING -----------------------*/

void WPX_FRMR_RegisterSonetSdhSectionK1K2AnnounceCallback(void *fnc)
{
   section_signalling_announce_callbacks[SSA_CALLBACK_SONET_SDH_SECTION_K1K2_INDEX] = fnc;
}

void WPX_FRMR_RegisterSonetSdhSectionS1AnnounceCallback(void *fnc)
{
   section_signalling_announce_callbacks[SSA_CALLBACK_SONET_SDH_SECTION_S1_INDEX] = fnc;
}

/*---------------------- GENERALIZED SECTION SIGNALLING HANDLING -----------------------*/

/* This function initializes all the configuration structures required
   for proper initialization of the flexmux device */
void WPX_UFE_FRAMER_GlobalConfigurationInitialize(U32 first_connector_id)
{
   U32 second_connector_id=2; /* In DUAL EMPHY mode the second UFE is connected on SERIAL2 */
   U32 connector_id = first_connector_id;
   U32 flexmux_id;
   
   /* initialize the environment table */
   WPX_UFE_FRAMER_FlexmuxEnvironmentTableInit(&My_UFE_Board);

   /* initialize the autonomous output tables that contain alarm callbacks */
   WPX_UFE_FRAMER_FlexmuxAutonomousOutputTableInit(&My_UFE_Board);

   /* initialize the performance monitoring table */
   WPX_UFE_FRAMER_FlexmuxPmTableInit(&My_UFE_Board);

   /* initialize the device level diagnostic table */
   WPX_UFE_FRAMER_FlexmuxDeviceLevelDiagnosticTableInit(&My_UFE_Board);

   /* initialize the Omnispy table */
   WPX_UFE_FRAMER_FlexmuxOmnispyTableInit(&My_UFE_Board);

   /* Initialize the framer lock */
   WPL_LockKeyInit(WPX_FLEXMUX_SEMAPHORE_KEY, &wpx_flexmux_lock);

   /* WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES defines the number of flexmux 
      devices to be initialized. For DUAL EMPHY configuration this parameter should
      be set to 2 otherwise it should be set to 1 */
   for (flexmux_id=0; flexmux_id < WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; ++flexmux_id)
   {
      /* initialize the mailbox physical addresses */
      WPX_UFE_FRAMER_FlexmuxMailBoxInit(connector_id,
                                        flexmux_id,
                                        &My_UFE_Board);
         
      connector_id = second_connector_id;
   }
}

/*---------------------Peek Function Register----------------------------*/
void WPX_FRMR_RegisterPeekCallback(void *fnc)
{
   peek_response_callback_array[PEEK_RESPONSE_CALLBACK_INDEX] = fnc;
}

void WPX_FRMR_RegisterBlockPokeVerifyCallback(void *fnc)
{
   block_poke_verify_response_callback_array[BLOCK_POKE_VERIFY_RESPONSE_CALLBACK_INDEX] = fnc;
}

void WPX_FRMR_RegisterAlarmMapCallback(void *fnc)
{
   alarm_map_response_callback_array[ALARM_MAP_RESPONSE_CALLBACK_INDEX] = fnc;
}

void WPX_FRMR_RegisterFramerLockCallback(void *fnc)
{
   framer_lock_callback_array[FRAMER_LOCK_CALLBACK_INDEX] = fnc;
}

/* Displays the Flexmux global configuration and registry */
void WPX_UFE_FRAMER_FlexmuxDisplay(U32 flexmux_id,
                                   U32 connector_id)
{
   WPX_PRINT_DBG((wpx_flexmux_log, "flexmux_id                                          0x%x\n", flexmux_id));
   WPX_PRINT_DBG((wpx_flexmux_log, "connector_id                                        0x%x\n", connector_id));
}

void InitializationProcedure(void)
{
   WPX_PRINT_DBG((wpx_flexmux_log, "WPX_UFE_FRAMER_InitUfeFlexmuxFramer\n"));
}

/* Initialize the UFE Flexmux framer (To be called from framer application) */
U32 WPX_UFE_FRAMER_FlexmuxInit(U32 first_flexmux_id,
                               U32 first_flexmux_connector_id)
{
   U32 status, flexmux_id;
   U32 second_connector_id = 2; /* In DUAL EMPHY mode the second UFE is connected on SERIAL2 */
   U32 flexmux_connector_id = first_flexmux_connector_id;
   
   /* Log entry in Flexmux logfile */
   InitializationProcedure();

   for (flexmux_id = first_flexmux_id; flexmux_id < WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; ++flexmux_id)
   {
      /* set device not ready in registry */
      My_UFE_Board.device_is_ready[flexmux_id] = WPX_FLEXMUX_NOT_READY;
   }

   for (flexmux_id = first_flexmux_id; flexmux_id < WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; ++flexmux_id)
   {
      /* Set match and mask registers on the Winpath */
      WPX_UFE_FRAMER_FlexmuxSetBank(flexmux_id, &wpx_ufe_framer_save_mask[flexmux_id], flexmux_connector_id);

      /* Display global configuration parameters */
      WPX_UFE_FRAMER_FlexmuxDisplay(flexmux_id, flexmux_connector_id);

      if (flexmux_id == first_flexmux_id)
      {
         /* Initialize the global parameters of the Flexmux devices */
         WPX_UFE_FRAMER_GlobalConfigurationInitialize(first_flexmux_connector_id);

         /* Init the flexmux devices */
         WPX_UFE_FRAMER_Initialize(&My_UFE_Board.mailbox_memory,
                                   &My_UFE_Board.env_table,
                                   &My_UFE_Board.autonomous_output_table,
                                   &My_UFE_Board.performance_monitoring_device_table,
                                   &My_UFE_Board.device_level_diagnostic_table,
                                   &My_UFE_Board.omnispy_table,
                                   0);
      }

      /* Enable communication with firmware */
      status = WPX_FRMR_LowLevelCommsEnableAccess(flexmux_id);
      if (status != WPX_UFE_FRAMER_OK)
      {
         WPX_PRINT_ERR((wpx_flexmux_log,
                        "WUFE_ERR_FLEXMUX_FAILURE_COMMUNICATION_WITH_FRAMER: %s in line %d\n",
                        "WPX_UFE_FRAMER_FlexmuxInit", __LINE__));
         
         return WUFE_ERR_FLEXMUX_FAILURE_COMMUNICATION_WITH_FRAMER;
      }

      /* Init Flexmux events */
      WPX_UFE_FRAMER_InitializeInterrupts(flexmux_id, flexmux_connector_id);

      /* Restore previous match and mask registers values */
      WPX_UFE_FRAMER_FlexmuxRestoreBank(flexmux_id, &wpx_ufe_framer_save_mask[flexmux_id]);

      flexmux_connector_id = second_connector_id;      
   }

   return WUFE_OK;
}

U32 WPX_UFE_FRAMER_FlexmuxInit_TestMode(U32 first_flexmux_id,
                                        U32 first_flexmux_connector_id)
{
   U32 status, flexmux_id;
   U32 second_connector_id = 2; /* In DUAL EMPHY mode the second UFE is connected on SERIAL2 */
   U32 flexmux_connector_id = first_flexmux_connector_id;

   /* Log entry in Flexmux logfile */
   InitializationProcedure();

   for (flexmux_id = first_flexmux_id; flexmux_id < WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; ++flexmux_id)
   {
      /* Set match and mask registers on the Winpath */
      WPX_UFE_FRAMER_FlexmuxSetBank(flexmux_id, &wpx_ufe_framer_save_mask[flexmux_id], flexmux_connector_id);

      /* set device not ready in registry */
      My_UFE_Board.device_is_ready[flexmux_id] = WPX_FLEXMUX_NOT_READY;

      /* Display global configuaration parameters */
      WPX_UFE_FRAMER_FlexmuxDisplay(flexmux_id, flexmux_connector_id);

      if (flexmux_id == first_flexmux_id)
      {
         /* Initialize the global parameters of the Flexmux device */
         WPX_UFE_FRAMER_GlobalConfigurationInitialize(flexmux_connector_id);

         /* Init the flexmux device */
         WPX_UFE_FRAMER_Initialize(&My_UFE_Board.mailbox_memory,
                                   &My_UFE_Board.env_table,
                                   &My_UFE_Board.autonomous_output_table,
                                   &My_UFE_Board.performance_monitoring_device_table,
                                   &My_UFE_Board.device_level_diagnostic_table,
                                   &My_UFE_Board.omnispy_table,
                                   1);
      }
      
      /* Enable communication with firmware */
      status = WPX_FRMR_LowLevelCommsEnableAccess(flexmux_id);
      if (status != WPX_UFE_FRAMER_OK)
      {
         WPX_PRINT_ERR((wpx_flexmux_log,
                        "WUFE_ERR_FLEXMUX_FAILURE_COMMUNICATION_WITH_FRAMER: %s in line %d\n",
                        "WPX_UFE_FRAMER_FlexmuxInit_TestMode", __LINE__));

         return WUFE_ERR_FLEXMUX_FAILURE_COMMUNICATION_WITH_FRAMER;
      }

      /* Init Flexmux events */
      WPX_UFE_FRAMER_InitializeInterrupts(flexmux_id, flexmux_connector_id);

      /* Restore previous match and mask registers values */
      WPX_UFE_FRAMER_FlexmuxRestoreBank(flexmux_id, &wpx_ufe_framer_save_mask[flexmux_id]);

      flexmux_connector_id = second_connector_id;
   }

   return WUFE_OK;
}

/* Set the UFE Flexmux framer build personality (To be called from framer application) */
U32 WPX_UFE_FRAMER_FlexmuxSetBuildPersonality(U32 flexmux_id,
                                              U32 flexmux_hw_config,
                                              U32 flexmux_connector_id)
{
   U32 counter=0, status;

   /* Set match and mask registers on the Winpath */
   WPX_UFE_FRAMER_FlexmuxSetBank(flexmux_id, &wpx_ufe_framer_save_mask[flexmux_id], flexmux_connector_id);

   /* wait until Flexmux device is initialized */
   while ((counter < WPX_FLEXMUX_MAX_LOOP_COUNT) &&
          (My_UFE_Board.device_is_ready[flexmux_id] == WPX_FLEXMUX_NOT_READY))
   {
      /* Delay 10 microseconds */
      WPL_Delay(10);
      ++counter;
   }

   /* If 5 seconds passed and device is still not ready
      (device ready callback not dispatched by firmware) - return fatal error */
   if (counter == WPX_FLEXMUX_MAX_LOOP_COUNT)
   {
      WPX_PRINT_ERR((wpx_flexmux_log,
                     "WUFE_ERR_FLEXMUX_FAILURE_DEVICE_NOT_READY for flexmux_id %d: %s in line %d\n",
                     flexmux_id, "WPX_UFE_FRAMER_FlexmuxSetBuildPersonality", __LINE__));
      
      return WUFE_ERR_FLEXMUX_FAILURE_DEVICE_NOT_READY;
   }
   
   /* set device build personality */
   status = WPX_FRMR_DEV_SetBuildPersonality(flexmux_id, flexmux_hw_config);
   if (status != WPX_UFE_FRAMER_OK)
   {
      WPX_PRINT_ERR((wpx_flexmux_log,
                     "WUFE_ERR_FLEXMUX_FAILURE_BUILD_PERSONALITY for flexmux_id %d: %s in line %d\n",
                     flexmux_id, "WPX_UFE_FRAMER_FlexmuxSetBuildPersonality", __LINE__));

      return WUFE_ERR_FLEXMUX_FAILURE_BUILD_PERSONALITY;
   }

   /* Restore previous match and mask registers values */
   WPX_UFE_FRAMER_FlexmuxRestoreBank(flexmux_id, &wpx_ufe_framer_save_mask[flexmux_id]);

   return WUFE_OK;
}


void WPX_UFE_FRAMER_FlexmuxServiceMailbox(void *params)
{
   U32 flexmux_id[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES], flexmux_connector_id[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES];
   int ContinueProcessing = 1;
   U32 ii;

   /*
     WP_U32 ufe_current_status;
  
     if (WUFE_GetCurrentStatus(0, &ufe_current_status[flexmux_id]) != WUFE_OK)
     return;
  
     if (ufe_current_status[flexmux_id] == 0)
     return;
   */

   for (ii=0; ii <WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; ++ii)
   {
      flexmux_id[ii] = ((U32 *) params)[2*ii];
      flexmux_connector_id[ii] = ((U32 *) params)[(2*ii)+1];

      /* service the mail boxes */
      while (My_UFE_Board.device_is_ready[flexmux_id[ii]] == WPX_FLEXMUX_NOT_READY)
      {
         WPX_UFE_FRAMER_ServiceMailbox(flexmux_id[ii], flexmux_connector_id[ii]);
      }
   }

   /* service the mail boxes */
   while (ContinueProcessing)
   {
      ContinueProcessing = 0;

#if WTI_COLLECT_TIMING_INFORMATION
      record_action(50, WP_TimeRead());
#endif

      WPX_UFE_FRAMER_LOCK_FUNCTION();

#if WTI_COLLECT_TIMING_INFORMATION
      record_action(51, WP_TimeRead());
#endif

      for (ii=0; ii < WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; ++ii)
      {
         ContinueProcessing = WPX_UFE_FRAMER_ServiceMailbox(flexmux_id[ii], flexmux_connector_id[ii]);

#if WTI_COLLECT_TIMING_INFORMATION
         record_action(52, WP_TimeRead());
#endif
      }
   }
}

WUFE_status WPX_FRMR_PM_SonetSdhPortStorage(U32 flexmux_id, void *Table, U8 NumElements)
{
   if (((int) Table & 3) != 0)
      return WUFE_ERR_INVALID_CONFIG;

   local_pm_device_table.Device[flexmux_id].SONET_SDH_PerformanceMonitoringPortTable = Table;
   local_pm_device_table.Device[flexmux_id].MAX_SONET_SDH_PerformanceMonitoringPortTableElements = NumElements;

   return WUFE_OK;
}

WUFE_status WPX_FRMR_PM_SonetSdhHOPathStorage(U32 flexmux_id, void *Table, U8 NumElements)
{
   if (((int) Table & 3) != 0)
      return WUFE_ERR_INVALID_CONFIG;

   local_pm_device_table.Device[flexmux_id].SONET_SDH_PerformanceMonitoringHighOrderPathTable = Table;
   local_pm_device_table.Device[flexmux_id].MAX_SONET_SDH_PerformanceMonitoringHighOrderPathTableElements = NumElements;

   return WUFE_OK;
}

WUFE_status WPX_FRMR_PM_SonetSdhLOPathStorage(U32 flexmux_id, void *Table, U8 NumElements)
{
   if (((int) Table & 3) != 0)
      return WUFE_ERR_INVALID_CONFIG;

   local_pm_device_table.Device[flexmux_id].SONET_SDH_PerformanceMonitoringLowOrderPathTable = Table;
   local_pm_device_table.Device[flexmux_id].MAX_SONET_SDH_PerformanceMonitoringLowOrderPathTableElements = NumElements;

   return WUFE_OK;
}

WUFE_status WPX_FRMR_PM_PdhStorage(U32 flexmux_id, void *Table, U8 NumElements)
{
   if (((int) Table & 3) != 0)
      return WUFE_ERR_INVALID_CONFIG;

   local_pm_device_table.Device[flexmux_id].PDH_PerformanceMonitoringTable = Table;
   local_pm_device_table.Device[flexmux_id].MAX_PDH_PerformanceMonitoringTableElements = NumElements;

   return WUFE_OK;
}
