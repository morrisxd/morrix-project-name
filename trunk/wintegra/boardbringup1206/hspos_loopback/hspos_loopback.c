/*************************************************************
 * (C) Copyright 2001-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/
/*****************************************************************************
 *
 * Unique Test Number:
 *
 * Test File:
 *
 * Test Requirement addressed:
 *
 * Test Description:
 * -----------------
 *
 *
 * Test Traffic Setup:
 * ------------------
 *    None
 *
 * Test Output:
 * ------------
 *   Printed on screen
 *
 ****************************************************************************/

#ifdef USE_SOCKET
#ifdef WINSIM
#include "sockint.h"
#elif  VERILOG
#include "../../target/verilog_winpath3/include/sockint.h"
#endif
#endif

#include "api/wp_wddi.h"
#include "api/wp_host_io.h"

#ifndef _WT_UTIL
#define _WT_UTIL 1


#ifndef WT_CONFIG_H
#define WT_CONFIG_H

#if 0
#pragma commit_check OK 8
#endif
#define _WT_MODULE_TEST_COMMENT_ENABLE_ 0
#define _WT_MODULE_TEST_ENABLE_ 1

#endif /* WT_CONFIG_H */
#include "wp_wddi.h"
#include "wpx_app_data.h"
#include "wpx_board_if.h"
#define STRINGIFY(x) STRINGIFY_(x)
#define STRINGIFY_(x) #x
/* Functions provided by wt_util.c.  */

void WT_Identify(void);
void WT_TranslateAsciiToHex(WP_CHAR *Hex,WP_CHAR *Ascii,WP_U32 length);
void WT_PassAndTerminate(void);
void WT_FailAndTerminate(void);
void WT_PassAndContinue(void);
void WT_FailAndContinue(void);
void WT_TerminateOnError(WP_handle handle, WP_CHAR *s);
WP_U32 WT_TdmBoardFrequency(void);
#define WT_MAX(a,b) ((a) < (b) ? (a) : (b))

#define WTI_MAX(a,b) ((a) > (b) ? (a) : (b))
#define WTI_MIN(a,b) ((a) < (b) ? (a) : (b))

#define WPI_TARGET_MIPS_WEB_VXWORKS      1
#define WPI_TARGET_MIPS_WEB_WINMON       2
#define WPI_TARGET_MIPS_WINMON           2
#define WPI_TARGET_MIPS_WINMON_WINPATH3  2
#define WPI_TARGET_MIPS_WEB_WINMON_GDB   3
#define WPI_TARGET_PPC_WEB_VXWORKS       4
#define WPI_TARGET_PPC_WEB_WINMON        5
#define WPI_TARGET_PPC_WINMON            5
#define WPI_TARGET_VERILOG               6
#define WPI_TARGET_WINSIM                7
#define WPI_TARGET_MIPS_WEB_VXWORKS_DUAL 8
#define WPI_TARGET_MIPS_WEB_WINMON_DUAL  9
#define WPI_TARGET_PPC_WEB_VXWORKS_DUAL  10
#define WPI_TARGET_MIPS_WEB_LINUX        11
#define WPI_TARGET_MIPS_WEB_KMODULE      12
#define WPI_TARGET_MIPS_WEB2_WINMON      13
#define WPI_TARGET_MIPS_WEB2_VXWORKS     14
#define WPI_TARGET_MIPS_LINUX            15
#define WPI_TARGET_PPC_WEB2_VXWORKS      16
#define WPI_TARGET_PPC_LINUX             17
#define WPI_TARGET_MIPS_WINMON_AUTO_MEMMAP  18
#define WPI_TARGET_MIPS_LINUX_AUTO_MEMMAP   19

#define WT_LOG(x,y)

/* protottype for RTDI GPIO configuration */
WP_status WT_GpioCommit(void);
WP_status WT_ItdmGpioCommit(void);

/* prototype for checking port existence */
WP_boolean WTI_CheckForPortPresent(WP_U32 portId);

#define PRINT_STAT_FIELD( field) \
        printf( "%08X%08X", ( sizeof(field)==sizeof(WP_U64))?(WP_U32)(field>>32):0, (WP_U32)field)

/* BRG of each UPI */
#ifdef WP_HW_WINPATH1
#define WT_BRG_UPI1 WP_BRG1
#define WT_BRG_UPI2 WP_BRG3
#endif
#ifdef WP_HW_WINPATH2
#define WT_BRG_UPI1 WP_BRG3
#define WT_BRG_UPI2 WP_BRG1
#endif
#ifdef WP_HW_WINPATH3
#define WT_BRG_UPI1 WP_BRG3
#define WT_BRG_UPI2 WP_BRG1
#endif

#endif
#include "wp_sim.h"

#include "wpi_veneer.h"
#include "wpx_app_data.h"
/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 * (C) Copyright Wintegra 2001-2008.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 *****************************************************************************/


/*****************************************************************************
 *
 * Module:
 * Purpose:  This function is used by some interworking examples.
 *           It should not be replicated.
 *           Its use is not required by WDDI.

 * Change History:
 * +------------+--------------------+----------------------------------------
 * | Date       | By                 | Description
 * +------------+--------------------+----------------------------------------
 * |            |                    |
 * +------------+--------------------+----------------------------------------
 * |            |                    |
 * +------------+--------------------+----------------------------------------
 *
 *****************************************************************************/
#ifndef _APP_CONVERT_H
#define _APP_CONVERT_H

#include "wp_wddi.h"

void App_TranslateAsciiToHex(WP_CHAR *Hex,WP_CHAR *Ascii,WP_U32 length)
{
    WP_S32 i,first=0,second=0;

   for (i=0;i<length;i++)
     {

          if ( Ascii[2*i] >= '0' &&  Ascii[2*i] <= '9')
            first =  Ascii[2*i] - '0';

          if ( Ascii[2*i] >= 'a' &&  Ascii[2*i] <= 'f')
             first =  Ascii[2*i] - 'a' + 10;

          if ( Ascii[2*i] >= 'A' &&  Ascii[2*i] <= 'F')
             first =  Ascii[2*i] - 'A' + 10;

          if ( Ascii[2*i+1] >= '0' &&  Ascii[2*i+1] <= '9')
            second =  Ascii[2*i+1] - '0';
          if ( Ascii[2*i+1] >= 'a' &&  Ascii[2*i+1] <= 'f')
            second =  Ascii[2*i+1] - 'a' + 10;
          if ( Ascii[2*i+1] >= 'A' &&  Ascii[2*i+1] <= 'F')
            second =  Ascii[2*i+1] - 'A' + 10;

          Hex[i]=(WP_CHAR )(first * 16 + second);
     }
    return;
}


void App_TranslateHexToAscii(WP_U8 *Hex,WP_CHAR *Ascii,WP_U32 length)
{
   WP_S32 i;
   WP_U8 first=0,second=0;
   
   for (i=0;i<length;i++)
   {
      first = Hex[i]%16;
      second = Hex[i]>>4;
      
      if ( first <= 9)
      {
         Ascii[2*i +1]= first + '0';
      }
      else
      {
         Ascii[2*i +1] = first + 'a' -10;
      }
      
      if ( second <= 9)
      {
         Ascii[2*i]= second + '0';
      }
      else
      {
         Ascii[2*i] = second + 'a' -10;
      }
   }
   /* Terminate with null */
   Ascii[2 * i] = 0;
   return;
}

#endif

#include "wpx_oc12_pmc5357.h"
#include "wpx_gei_phy.h"
#include "wpx_pin_mux_selector.h"
#include "wpui_dump.h"

#ifndef WT_FMU_TEST_UTIL_H
/*****************************************************************************
 * (C) Copyright 2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wt_fmu_test_util.h
 *
 * Purpose: FMU tests utility functions
 *
 ****************************************************************************/
#ifndef WT_FMU_TEST_UTIL_H
#define WT_FMU_TEST_UTIL_H

#ifdef __linux__
#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define TESTSTRING(x) TESTSTRING_(x)
#define TESTSTRING_(x) #x

#define WT_DEBUG_COMMON_PRINTS   0x1
#define WT_DEBUG_DISPLAY_STATS   0x2
#define WT_DEBUG_DISPLAY_TRAFFIC 0x4

#define WT_LOOPBACK_INTERNAL   0
#define WT_LOOPBACK_SERDES     1
#define WT_LOOPBACK_PHY_CLOSE  2
#define WT_LOOPBACK_PHY_MIDDLE 3 
#define WT_LOOPBACK_PHY_FAR    4
#define WT_LOOPBACK_EXTERNAL   5
#define WT_LOOPBACK_OFF        6

typedef enum
{
   WT_ENET,
   WT_HSPOS,
   WT_POS
} WT_port_type;

typedef enum 
{
   IW_SYS_TAG = 0,
   FLOW_AGG_TAG,
   BPORT_TAG,
   IW_TX_CH_TAG,
   IW_RX_CH_TAG,
   TX_CHANNEL_POS_TAG = 100,
   RX_CHANNEL_POS_TAG = 200,
   TX_CHANNEL_ENET_TAG = 400,
   RX_CHANNEL_ENET_TAG = 800
}WT_test_tags;

typedef enum
{
   WT_L1 = 0,
   WT_L2,
   WT_L3
} WT_shaping_level;

typedef enum
{
   WT_STRICT = 0,
   WT_WFQ,
   WT_WRR,
   WT_CIR_EIR,
   WT_CIR_EIR_STRICT,
   WT_1STRICT_7CIR_EIR,
   WT_2STRICT_6CIR_EIR,
   WT_1STRICT_7WRR,
   WT_2STRICT_6WRR
} WT_shaping_type;

#define KILO 1000
#define MEGA 1000000
#define GIGA 1000000000LL

#define MAX_L1_GROUPS 304
#define MAX_L2_GROUPS 2048
#define MAX_L3_GROUPS 16384

#define MAX_L1_BLOCKS   MAX_L1_GROUPS
#define MAX_L2_BLOCKS   MAX_L2_GROUPS

#define MAX_PORTS       16 //referring to ENET ports
#define MAX_DEVICES     MAX_L1_GROUPS
#define MAX_TX_CHANNELS MAX_L3_GROUPS
#define MAX_RX_CHANNELS MAX_DEVICES

#ifdef USE_SOCKET
#define WT_RECEIVE_LOOPS 500
#define WT_RECEIVE_DELAY 2000000
#else
#define WT_RECEIVE_LOOPS 100
#define WT_RECEIVE_DELAY 2000
#endif

typedef struct
{
      WP_U32    name;
      WP_handle handle;
} WT_port;

typedef struct
{
      WP_handle handle;
      WP_handle rx_channel;
      WP_U8 tx_shaping_type;
} WT_tx_channel;

typedef struct
{
      WT_port   ports[MAX_PORTS];
      WP_handle devices[MAX_DEVICES];
      WP_handle l1_blocks[MAX_L1_BLOCKS];
      WP_handle l1_groups[MAX_L1_GROUPS];
      WP_handle l2_blocks[MAX_L2_BLOCKS];
      WP_handle l2_groups[MAX_L2_GROUPS];
      WP_handle rx_channels[MAX_RX_CHANNELS];
      WT_tx_channel tx_channels[MAX_TX_CHANNELS];
} WT_test_system;

typedef enum
{
   WT_CONNECT_INDEXED = 0x10000,
   WT_CONNECT_GROUPED = 0x20000,
   WT_CONNECT_SINGLE  = 0x40000,
   WT_CONNECT_MASK    = 0xffff0000,
} WT_fmu_connect_mode;

typedef struct
{
      WP_U32 parent_offset;
      WP_U32 shaping_type;
      void *shaping_params;
} WT_fmu_group_configuration;

typedef struct
{
      WP_U32 num_groups;
      WP_boolean identical;
      WT_fmu_group_configuration group[MAX_L1_GROUPS];
} WT_fmu_l1_configuration;

typedef struct
{
      WP_U32 num_groups;
      WP_U32 connect_mode;
      WP_boolean identical;
      WT_fmu_group_configuration group[MAX_L2_GROUPS];
} WT_fmu_l2_configuration;

typedef struct
{
      WP_U32 num_groups;
      WP_U32 connect_mode;
      WP_boolean identical;
      WT_fmu_group_configuration group[MAX_L3_GROUPS];
} WT_fmu_l3_configuration;

typedef struct
{
      WP_handle device_handle;
      WP_U64    device_bitrate;
      WT_fmu_l1_configuration l1;
      WT_fmu_l2_configuration l2;
      WT_fmu_l3_configuration l3;
} WT_fmu_device_configuration;

typedef union StatField
{
      WP_U64 field;
      struct {
            WP_U32 high;
            WP_U32 low;
      } part;
} WPT_StatField;

typedef struct
{
      unsigned Vers                : 4  ;/* Version number.*/
      unsigned IHL                 : 4  ;/* ethernet Header Length*/
      unsigned TOS                 : 8  ;/* Type of Service.*/
      unsigned TotalLength         : 16 ;/* Total Length is the length of the datagram.*/
      unsigned Identification      : 16 ;/* identifying fragments of a datagram.*/
      unsigned Flags               : 3  ;/* Control Flags (Bit0:res,Bit1:1 = Don't Fragment,Bit2:0 = Last Fragment)*/
      unsigned FragmentOffset      : 13 ;/* Indicates where in the datagram this fragment belongs */
      unsigned TimeToLive          : 8  ;/* Time to Live */
      unsigned Protocol            : 8  ;/* Indicates the next level protocol used*/
      unsigned HeaderChecksum      : 16 ;/* checksum on the header only*/
      unsigned SourceAddress       : 32 ;/* Source address*/
      unsigned DestinationAddress  : 32 ;/* Destination address*/
} S_IPv4Header;

/* shaping structures */
WT_fmu_device_configuration fast_fmu_cfg[] =
{
   {
      /* device handle  */ 0,
      /* device_bitrate */ WP_TX_BIT_RATE_UNLIMITED,
      /* L1 config */
      {
         /* num_groups    */ 1,
         /* identical     */ 1,
         /* group         */ {{0,WP_FMU_SHAPING_TYPE_STRICT,NULL}},
      },
      /* L2 config */
      {
         /* num_groups    */ 1,
         /* connect_mode  */ WT_CONNECT_SINGLE,
         /* identical     */ 1,
         /* group         */ {{0,WP_FMU_SHAPING_TYPE_STRICT,NULL}},
      },
      /* L3 config */
      {
         /* num_groups    */ 1,
         /* connect_mode  */ WT_CONNECT_SINGLE,
         /* identical     */ 1,
         /* group         */ {{0,WP_FMU_SHAPING_TYPE_STRICT,NULL}},
      },
   },
};

#define WT_WFQ_WEIGHT_UNIT 4096
WP_fmu_shaping_wfq wfq_shaping_params[] =
{
   {
      /* weight */ WT_WFQ_WEIGHT_UNIT,
      /* flags  */ 0
   }
};

#define WT_WRR_WEIGHT_UNIT 128
WP_fmu_shaping_wrr wrr_shaping_params[] =
{
   {
      /* weight */ WT_WRR_WEIGHT_UNIT,
      /* flags  */ 0
   }
};

#define WT_DEFAULT_CBS (10*1536*8)
#define WT_DEFAULT_EBS (10*1536*8)
WP_fmu_shaping_cir_eir cir_eir_shaping_params[] =
{
   {
      /* cir   */ 900000000,
      /* cbs   */ WT_DEFAULT_CBS,
      /* eir   */ 100000000,
      /* ebs   */ WT_DEFAULT_EBS,
      /* flags */ 0
   }
};

WP_fmu_shaping_strict_and_shaped strict_and_shaped_shaping_params[] =
{
   {
      /* shaping_type  */ 0,
      /* shaping_param */ NULL,
      /* flags         */ 0
   }
};

WP_shaping_group_type_packet l1_group_config[] =
{
   {
         /* group_level */     WP_L1_GROUP,
         /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
         /* shaping_params */  NULL,
         /* num_fifos */       WP_NUM_FIFOS_8,
         /* block_handle */    0,
         /* block_level */     0,
         /* group_mode */      0
   }
};

WP_shaping_group_type_packet l2_group_config[] =
{
   {
         /* group_level */     WP_L2_GROUP,
         /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
         /* shaping_params */  NULL,
         /* num_fifos */       WP_NUM_FIFOS_8,
         /* block_handle */    0,
         /* block_level */     0,
         /* group_mode */      WP_MODE_HW
   }
};

WP_tx_binding_bytes tx_binding_bytes[] =
{
   {
      /* maxt_bytes;*/0x7ffff,
   }
};

WP_tx_binding tx_binding[] =
{
   {
      /* wred_mode;*/ 0,
      /* dci_mode; */ 1,
      /* maxt;     */ 255,
   }
};

/* Terminal functions */
#define STR_MENU_PROMPT    ">"
#define WTE_TTY_LOAD_TIME 1000000
#define WTE_TTY_FIRST_CHAR 1
#define WTE_TTY_FIRST_LINE 1
#define WTE_TTY_COLUMN_WIDTH 32
#define WTE_TTY_COLUMN(n) ((n-1)*WTE_TTY_COLUMN_WIDTH+WTE_TTY_FIRST_CHAR)

#define WEB_UART_LINE_STATUS_REG_OFFSET 0xc51c
#define WEB_UART_STATUS_RXFE            0x80
#define WEB_UART_STATUS_TEMT            0x40
#define WEB_UART_STATUS_THRE            0x20
#define WEB_UART_STATUS_BI              0x10
#define WEB_UART_STATUS_FE              0x08
#define WEB_UART_STATUS_PE              0x04
#define WEB_UART_STATUS_OE              0x02
#define WEB_UART_STATUS_DR              0x01
void Vt100Bright(void);
void Vt100Reverse(void);
void Vt100Color(WP_S32 foreground, WP_S32 background);
void Vt100Normal(void);
void Vt100GotoXy(WP_S32 x, WP_S32 y);
void Vt100EraseEOL(void);
void Vt100ClearScreen(void);
void Vt100CrsrInvisible(void);
void Vt100CrsrVisible(void);
void Vt100CrsrUp(WP_U32 up);
void Vt100CrsrDown(WP_U32 down);
void Vt100EraseDown(void);
WP_U8 F_SampleSerial(void);

/* Test functions */
WP_status logCallBack(WP_S32 level, const WP_CHAR *message);

WP_status WTE_DebugCallback(const WP_CHAR *file, const WP_CHAR *function,
                            WP_S32 line, WP_S32 error_id);
void WTE_Terminate(void);

void terminate_on_error(WP_handle handle, WP_CHAR *s,WP_U32 line);

void terminate_on_no_error(WP_handle handle, WP_CHAR *s,WP_U32 line);

WP_U16 ChecksumCalc(S_IPv4Header *IPHeader);

WP_U32 F_InsertHeader(WP_U32 system_type, WP_CHAR* tos);

void F_IwDataUnitsSetupIPv4(WP_U32 system_type, WP_CHAR* tos, WP_U32 data_type);

WP_status WTE_HostSendIPv4( WP_handle ch_handle,
                            WP_U32 system_type,
                            WP_CHAR* tos);
WP_status WTE_HostSendDirectIPv4( WP_handle handle,
                                  WP_U32 system_type,
                                  WP_CHAR* tos);

void WTE_ResetHandles(void);

WP_status WTE_HostReceiveIPv4(WP_handle ch_handle);

WP_boolean WTE_CheckData(void);

WP_status WTE_RunChannelTraffic(WP_handle tx_channel,
                                WP_handle rx_channel,
                                WP_boolean check_data);

WP_status WTE_RunTraffic(WP_U32 num_channels);

void WTE_DeviceChannelsCreate(WP_U32 device_index,
                              WP_U32 *l1_offset,
                              WP_U32 *l2_offset,
                              WP_U32 *l3_offset,
                              WP_handle qnode,
                              WT_fmu_device_configuration *config,
                              WP_ch_enet *ch_config_enet);

void WTE_DevicesEnable(void);

void WTE_DevicesDisable(void);

void WTE_GroupsEnable(void);

void WTE_GroupsDisable(void);

void WTE_GroupsDelete(void);

void WTE_ChannelsEnable(void);

void WTE_ChannelsDisable(void);

void WTE_ChannelsDelete(void);

void WTE_TxBindingCreate(void);

void WTE_AssignShapingType(WP_U32 level,
                           WP_U32 index,
                           WP_U8 *shaping_type,
                           void **shaping_params,
                           WP_boolean *is_strict);

void WTE_Pause(void);

void WTE_DisplayEnetDevicesStats(void);

void WTE_DisplayXgiDevicesStats(void);

void WTE_CheckEnetDeviceStats(WP_handle device_handle, WP_U32 num_channels);

int WTE_CheckHsposDeviceStats(WP_handle device_handle);

WP_U32 WTE_DisplayDeviceStats(WP_handle device_handle, WP_device device, WP_CHAR* type);

WP_U32 WTE_DisplayIwPortStats( WP_handle port_handle, WP_CHAR* type);

void WTE_DisplayFlowStats(WP_handle agg, WP_CHAR* type);

void WTE_DisplayGenericFlowStats(WP_handle agg, WP_CHAR* type);

void WTE_DisplayPceGlobalStats(void);

void WTE_DisplayPceFilterStats(WP_handle filter);

WP_U32 WTE_DisplayAAL5RxChannelStats(WP_handle ch_handle, WP_CHAR *type);

WP_U32 WTE_DisplayAAL5TxChannelStats(WP_handle ch_handle, WP_CHAR *type, WP_U8 display);

#endif
#endif

/*************************************************************
 * (C) Copyright 2001-2003, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

/*****************************************************************************
 *
 * Module:
 * Purpose:
 *
 * Change History:
 * +------------+--------------------+----------------------------------------
 * | Date       | By                 | Description
 * +------------+--------------------+----------------------------------------
 * |            |                    |
 * +------------+--------------------+----------------------------------------
 * |            |                    |
 * +------------+--------------------+----------------------------------------
 *
 *****************************************************************************/

#ifndef _WT_DEBUG_MENU_H_
#define _WT_DEBUG_MENU_H_

#define USE_MULTI_THREADING_FINE_GRAIN_LOCKING_WDDI 1
#define BUFFER_SIZE 50
#define NO_OF_THREADS WINFARM_NUMBER*16

/*Menu Sizes*/
/*Sizes that differ between Winpaths*/
#ifdef WP_HW_WINPATH3 /*Winpath 3*/
  #define WU_ALL_MENU_SIZE 26
  #define PERF_MENU_SIZE 6
#else /*Winpath 2*/
  #define WU_ALL_MENU_SIZE 24
  #define PERF_MENU_SIZE 5
#endif

/*Sizes that differ between Linux and Winmon*/ 
#ifdef linux
  #define FATAL_MENU_SIZE 18
  #define TRACE_MENU_SIZE 7
 #else
  #define FATAL_MENU_SIZE 16
  #define TRACE_MENU_SIZE 6
 #endif


#ifdef WP_HW_WINPATH3
 #ifdef WT_HW_WINPATH3_SL
   #define WINFARM_NUMBER 2
 #else
   #define WINFARM_NUMBER 4
 #endif
#else
  #define WINFARM_NUMBER 2
#endif

#ifdef MENU_DEBUG_MODE
 #define STATUS TRUE
#else
 #define STATUS FALSE
#endif




void CLI_OrderRamDisplay(void);
void CLI_Display_System(void);
void menu_terminate_on_error(WP_handle handle, WP_CHAR *s,WP_U32 line);
void CLI_WinUtilBusErrCheck(WP_CHAR *StrPrm);
void CLI_WinUtilWingineBusUtilization(WP_CHAR *StrPrm);
void CLI_WinUtilThreadUtilization(WP_CHAR *StrPrm);
void CLI_WinUtilThreadArbiterDump(WP_CHAR *StrPrm);
void CLI_WinUtilSerialArbiterDump (WP_CHAR *StrPrm);
void CLI_WinUtilTraceStart (WP_CHAR *StrPrm);
void CLI_WinUtilTraceDump (WP_CHAR *StrPrm);
void CLI_DebugAreDump     (WP_CHAR *StrPrm);
void CLI_DebugVerionInfo  (WP_CHAR *StrPrm);
WP_status WTI_DebugAlloc(const WP_CHAR *file, WP_U32 line, WP_U32 bus,WP_U32 size, WP_U32 alignment, const WP_CHAR *purpose);
void CLI_WinUtilBmap  (WP_CHAR *StrPrm);
void CLI_WinUtilCoreDump  (WP_CHAR *StrPrm);
void CLI_WinUtilDmap  (WP_CHAR *StrPrm);
void CLI_WinUtilFmum (WP_CHAR *StrPrm);
void CLI_WinUtilFreq (WP_CHAR *StrPrm);
void CLI_WinUtilTraceEnable_DPS (WP_CHAR *StrPrm);
void CLI_WinUtilTraceEnable_Serial (WP_CHAR *StrPrm);
void CLI_WinUtilMemoryMap (WP_CHAR *StrPrm);
void CLI_WinUtilMemoryCheck (WP_CHAR *StrPrm);
void CLI_WinUtilMemoryDisplay (WP_CHAR *StrPrm);
void CLI_WinUtilMemoryFill (WP_CHAR *StrPrm);
void CLI_WinUtilMemoryModify (WP_CHAR *StrPrm);
void CLI_WinUtilMemoryModifyOffsetBased (WP_CHAR *StrPrm);
void CLI_WinUtilSA_TRSLimits (WP_CHAR *StrPrm);
void CLI_WinUtilTA_Bases (WP_CHAR *StrPrm);
void CLI_WinUtilDisplayTA_Table (WP_CHAR *StrPrm);
void CLI_WinUtilOrderMonitorDisplay (WP_CHAR *StrPrm);
void CLI_WinUtilTAOrderRegisters (WP_CHAR *StrPrm);
void CLI_WinUtilCurrentTRSStatus (WP_CHAR *StrPrm);
void CLI_WinUtilTraceStart (WP_CHAR *StrPrm);
void CLI_WinUtilTraceDump (WP_CHAR *StrPrm);
void CLI_WinUtilDisplayAllTraceSerials(void);
void CLI_WinUtilTRS_SerialAllocation (WP_CHAR *StrPrm);
void CLI_WDDI_Allocation(void);
void CLI_WDDI_AllocationEND(void);
void CLI_WinUtilDebugVersionInfo(WP_CHAR *StrPrm);
void CLI_WinUtilWingineUtilization (WP_CHAR *StrPrm);
void CLI_WinUtilFifoCheck (WP_CHAR *StrPrm);
void CLI_WinUtilFreeTxt (WP_CHAR *StrPrm);
void CLI_WinUtilCall(WP_CHAR* strParam, WP_CHAR *StrPrm);
void CLI_WinUtilTrsDump  (WP_CHAR *StrPrm);
void CLI_WinUtilHelp(void);
void CLI_WinUtilTraceOptionsStatus(void);
void CLI_SYSCMD_GetTRS(WP_CHAR *StrPrm);
void CLI_SYSCMD_SetTRS(WP_CHAR *StrPrm);
void CLI_SYSCMD_SmartDropConfig (WP_CHAR *StrPrm);
void CLI_SYSCMD_SetCFUTX( WP_CHAR *StrPrm);
void CLI_SYSCMD_PrintTRSserials(void);
void CLI_DisplayDTCR(void);
void CLI_DisplayTA_PCs(void);
void CLI_DisplaySA_PCT_Table(void);
void CLI_DisplayFifo_StartEnd(WP_CHAR *StrPrm);
void CLI_DisplayFifo_corruption(WP_CHAR *StrPrm);
void CLI_SetFifo_base(WP_CHAR *StrPrm);
void CLI_SetFifo_offset(WP_CHAR *StrPrm);
void CLI_DisplayTRS_AllInfo(void);
void CLI_DPS_RAM_ECCcheck(void);
void CLI_Bus_ECCcheck(void);
void CLI_WF_ReadDPSRAMPCs(void);
void CLI_DisplayFifo_CorruptedNumbers(void);
void CLI_All_VersionBuildTime(void);
WP_status WT_WddiLog(WP_S32 level, const WP_CHAR *string);
void F_menu_TermOnError (WP_handle handle, WP_CHAR *s, WP_U32 LineNum);
void CLI_SetLogLevel(WP_CHAR *StrPrm);
void CLI_APILevel_StartLog(void);
void CLI_APILevel_EndLog(void);
void CLI_InternalLevel_StartLog(void);
void CLI_InternalLevel_EndLog(void);
void CLI_WPILevel_StartLog(void);
void CLI_WPILevel_EndLog(void);
void CLI_FineGrainLocking(void);
void CLI_WDDI_VersionBuildTime(void);
void CLI_FatalError_DisplayAll(WP_CHAR *StrPrm);
void CLI_ShowAllDisplayCmds(void);
void CLI_DisplayCmd (WP_CHAR *StrPrm);
void CLI_PrintMemoryAvailable (WP_CHAR *StrPrm);
void CLI_WP_QNodeStatus(WP_CHAR *StrPrm);
void CLI_ListQnodes(void);
void CLI_TRS_Timeout(void);
void WT_ExampleDebugAreaCallback(WP_U32 wpid, WP_U32 size_allocated, WP_U32 *host_ptr);
void CLI_DebugAreDumpPartial (WP_CHAR *StrPrm);
void CLI_DebugAreDumpFull (WP_CHAR *StrPrm);
#endif

/************************************************************************/
/*                   AUTOMATION INTERFACE                               */
/************************************************************************/

#ifndef _WT_NEW_AUTOMATION_
#define _WT_NEW_AUTOMATION_

/******************************* AUTOMATION *************************/

/* Defined modes of operation */
#define MANUAL                  0
#define AUTO                    1

/* Types of menu systems */
#define REGULAR                 0
#define SPECIAL                 1
#define TTL                     2

/* Automation constants */
#define INST_NUM                256
#define MAX_TTL_FILE_SIZE       10000
#define MAX_CONTROL_STRING_SIZE 1536
#define QUEUESIZE               3
#define SERVER_PORT             9333
#define MSGMAXSIZE              2048
#define WS_PACKET_LENGTH        2049
#define HOST_NAME_LEN           50
#define INVALID_SOCKET          -1
#define SOCKET_ERROR            -1

#define MAP_GS_IC_SYIR          (WEB_WINPATH_BASE+0x10c74)
#define MAP_GS_IC_SEIR          (WEB_WINPATH_BASE+0x10c70)
#define MAP_GS_IC_SEMR0         (WEB_WINPATH_BASE+0x10c60)
#define MAP_GS_IC_SEMR1         (WEB_WINPATH_BASE+0x10c64)
#define MAP_GS_IC_SYMR0         (WEB_WINPATH_BASE+0x10c68)
#define MAP_GS_IC_SYMR1         (WEB_WINPATH_BASE+0x10c6c)

#define MAP_GS_IC_INTMODE       (WEB_WINPATH_BASE+0x10c78)

#ifdef WP_HW_WINPATH1
#define ASIX_ISR                0xbf202818
#define WEB_CPLD_LED_1          0xbf100010
#elif defined WP_HW_WINPATH2
#define WEB_CPLD_LED_1          (WEB_CPLD_SECONDARY_BASE + 0x40)


#endif
/******************************* MENU API *************************/

#ifdef OK
#undef OK
#endif

#define OK 0
#ifndef ERROR
#define ERROR -1
#endif

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#define TRUE   1
#define FALSE  0
#define TRUE_M 3
#define FALSE_M 2
#define HELP 5

#define STR_STAR           "*************************************************************************"
#define STR_HOT_KEYS       "  Hot keys: !-Main Menu ^-Upper Menu =-Display Cur Menu  d-Debug Menu!    "
#define STR_ENTER_COMMAND  "# Enter your command:"
#define STR_MENU_ERR       "Err> Wrong menu command"

/* Definitions of menus structs */
typedef void (*Y_MnuLeafP) (char *);

typedef enum
{
   K_Menu,
   K_Leaf
} E_MenuNodeType;

typedef struct
{
   E_MenuNodeType    Type;
   char              Visible;
   char              *Desc;
   union
   {
      void           *Menu;
      Y_MnuLeafP     Leaf;
   }  Node;
}  Y_MenuEntry;

typedef void (*sed_int_handler_t)(void);
typedef struct netif my_netif;

extern /*const*/ Y_MenuEntry V_MainMenu[];

extern char *STR_MENU_HDR;
extern void sed_int_handler(void);
extern void sedif_input(my_netif *netif);
extern void tcpdump_init(void);
extern sed_int_handler_t sed_register_handler(sed_int_handler_t fnc);
extern void asixif_int_handler(void);
extern unsigned int imr_shadow;

/*****************************************************************************/
/*****************************************************************************/
/*                         MENU API FUNCTIONS                                */
/*****************************************************************************/
/*****************************************************************************/

void F_MenuEngine     (char * InputBuf);
int  F_EvaluateHotKey (char * InputBuf);
void F_EvaluateCommand(char * InputBuf);
void F_SetMainMenu    (void);
void F_SetDebugMenu   (void);
void F_SetUpperMenu   (void);
void F_SetMenu	      (Y_MenuEntry *Menu);
void F_DisplayMenu    (void);
void F_DisplayHeader  (void);

/*****************************************************************************/
/*****************************************************************************/
/*                         AUTOMATION FUNCTIONS                              */
/*****************************************************************************/
/*****************************************************************************/

/* Gets a string either from the stdin (manual mode) or from the automation
   global array (automation mode) */
WP_CHAR *WT_Gets( char *str);

/* Parses data on the control packet */
void EvaluateControlString(WP_U8 *InputBuf,WP_U32 demo_mode);

/* Fills automation global array of strings */
WP_U32 FillAutomationArray(WP_CHAR *InputBuf, WP_CHAR *params_string,WP_U32 demo_mode);

/* Processes incoming automation packets*/
void WT_processPacketCommand(void);

/* Translates data from ascii to hexadecimal */
void TranslateAsciiToHex(WP_U8 *Hex,WP_CHAR *Ascii,WP_U32 length);

/* Translates data from hexadecimal to ascii*/
void TranslateHexToAscii(WP_CHAR *Hex,WP_CHAR *Ascii,WP_U32 length);

#endif /* _WT_AUTOMATION_ */


#define LOOPBACK 1
#define WT_DEBUG 1

WP_CHAR appname[] = "HSPOS hier shaping FMU";
WP_CHAR *STR_MENU_HDR = {"   HSPOS Test"};

extern Y_MenuEntry V_Menu_Debug[];
extern Y_MenuEntry V_MainMenu[];

void menu_interface(void);

#ifdef USE_SOCKET
#define WT_RECEIVE_LOOPS 500
#define WT_RECEIVE_DELAY 2000000
#else
#define WT_RECEIVE_LOOPS 100
#define WT_RECEIVE_DELAY 2000
#endif

#define RE_ENABLE_DEVICES 1

#define NUM_OF_UPI   2

#define CHANNELS_PER_DEVICE 1
#define FRAMES_PER_CHANNEL 1

#define MAX_CHANNELS 300
#define POOL_LIMIT 8

#define MAX_DATA_SIZE 1530
#define DATA_LENGTH   92
#define DATA_TYPE     WP_DATA_PPP_HDLC

/* define trials */
#ifdef WT_TRIAL_MASK
#undef WT_TRIAL_MASK
#endif

#define WT_TRIAL_MASK 0x01

#if LOOPBACK
struct trial {
      char *name;
      WP_U32 flowmode;
      WP_U8 tqmode;
      WP_U8 phymode;
      WP_U32 num_of_phys;
      WP_U32 used_phys[288];
}trials[] = {
#if WT_TRIAL_MASK & (1 << 0)
   {
      "1 HSPOS single phy, hier shaping HW channels  UPI1-3", /* trial name */
      WP_UPI_FMU_HIERARCHICAL_SHAPING_MODE, /* flowmode */
      WP_TQ_FAST_MODE,                      /* tqmode */
      WP_UPI_SINGLE,                         /* single/multi phy */
      1,                                    /* number of UPI phys */
      {0},                   /* used phys */
   },
#endif
   {NULL}
};

#else
struct trial {
      char *name;
      WP_U32 flowmode;
      WP_U8 tqmode;
      WP_U8 phymode;
      WP_U32 num_of_phys;
      WP_U32 used_phys[288];
} trials[] = {
#if WT_TRIAL_MASK & (1 << 0)
   {
      "1 HSPOS multiphy, hier shaping HW channels  UPI1-3", /* trial name */
      WP_UPI_FMU_HIERARCHICAL_SHAPING_MODE, /* flowmode */
      WP_TQ_FAST_MODE,                      /* tqmode */
      WP_UPI_MULTI,                         /* single/multi phy */
      6,                                    /* number of UPI phys */
      {0,1,2,64,128,169},                   /* used phys */
   },
#endif
   {NULL}
};


#endif
extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];

WP_int_queue_table int_queue_tables = {{{24, 1}, {10, 1}, {10, 1}, {10, 1}}};

WP_iw_global  max_iw[] =
{
   {
      /* max_iw_aggregations */ 0,
      /* max_r_iw_systems;*/ 0,
      /* max_directmap_enet_systems;*/ 0,
      /* max_b_iw_systems;*/ 0,
      /* max_m_iw_systems;*/ 0,
      /* max_vlan_priority_maps; */ 0,
      /* iw_bkgnd_fifo_size */0,
      /* cong_pt       */NULL,
      /* iw_host_limits */ {64,0,0,2,0},
      /* mpls_config*/ {0,0},
      /* iw_modes;*/
      {  /*policer_mode;*/                      WP_IW_POLICER_DISABLE,
         /*statistics_bundle_stat_mode;*/       WP_IW_STATISTICS_BUNDLE_STAT_DISABLE,
         /*l2_prefix_ext_mode;*/                WP_IW_L2_HEADER_EXTENSION_DISABLE,
         /*enhanced_flow_stat_mode;*/           WP_IW_ENHANCED_FLOW_STAT_DISABLE,
         /*flow_stat_mode;*/                    WP_IW_FLOW_STAT_DISABLE,
         /*fr_tx_flow_stat_mode */              WP_IW_FR_TX_FLOW_STAT_DISABLE,
         /*mfc_alloc_mode; */                   WP_IW_MFC_ALLOC_DISABLE,
         /*Learning_queue_mode */               WP_IW_LEARNING_DISABLE,
         /*port_filtering_mode */               WP_IW_PORT_FILTERING_DISABLE,
         /* lpm_alloc_mode */                   WP_IW_LPM_ALLOC_DISABLE,
         /* enhanced_mc_member_stat_mode */     0,
         /* routing_alloc_modes */              WP_DISABLE
      },

      /* max_ch_agg_stat;*/0,
      /* max_fr_s_iw_systems;*/ 0,
      /*max_iw_ports;*/ MAX_CHANNELS,
      /* max_iw_mc_groups;*/0,
      /* max_nat_systems;*/0,
      /*max_iw_second_aggregations;*/0, /* secondary flow aggregations */
      /*iw_comp_limits;*/0,
      /* max_iw_mc_members;*/0,
      /* max_iw_programmable_filters;*/0,
      /* max_iw_policers;*/0
   }
};

WP_context context[] =
{
   {5, /* qnodes */
    /* the first four assignments are mandatory bus assignments */
    {
       {/* WP_BUS_PARAM    */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* WP_BUS_PACKET   */ WP_BUS_PACKET, APP_BANK_PACKET},
       {/* WP_BUS_INTERNAL */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* WP_BUS_HOST     */ WP_BUS_HOST, APP_BANK_HOST},
       {/* WP_BUS_IRQ      */ WP_BUS_HOST, APP_BANK_HOST},
       {/* WP_BUS_TMSTMP   */ WP_BUS_HOST, APP_BANK_HOST},
       {/* UNASSIGNED      */ WP_BUS_NONE, 0},
       {/* UNASSIGNED      */ WP_BUS_NONE, 0},
       {/* ASU_TPT         */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* ASU CW          */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* ASU TX QUEUE    */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* STATISTICS      */ WP_BUS_PARAM  , APP_BANK_PARAM},
       {/* ADDRESS LOOKUP  */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* ATM CPT         */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* ENET HDLC CPT   */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* IW FBP PARAM    */ WP_BUS_PARAM, APP_BANK_PARAM},
#ifdef WP_BOARD_WDS3_SL
       {/* IW FBP DATA     */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* EXTERNAL FIFO   */ WP_BUS_PARAM, APP_BANK_PARAM},
#else
       {/* IW FBP DATA     */ WP_BUS_PACKET, APP_BANK_PACKET},
       {/* EXTERNAL FIFO   */ WP_BUS_PACKET, APP_BANK_PACKET},
#endif
       {/* MAC LOOKUP      */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* IMA SMALL       */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* IMA LARGE       */ WP_BUS_PARAM, APP_BANK_PARAM},
#ifdef WP_BOARD_WDS3_SL
       {/* LPM             */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* MFC RULES       */ WP_BUS_PARAM, APP_BANK_PARAM},
#else
       {/* LPM             */ WP_BUS_PACKET, APP_BANK_PACKET},
       {/* MFC RULES       */ WP_BUS_PACKET, APP_BANK_PACKET},
#endif
       {/* FIWT, QAT       */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* MFCPT, UPPPT, DS*/ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* STATISTICS2      */ WP_BUS_HOST, APP_BANK_HOST},
       {/* STATISTICS3      */ WP_BUS_HOST, APP_BANK_HOST},
       {/* UNASSIGNED      */ WP_BUS_NONE, 0},
       {/* PKT_GLOBAL_TCPT */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* UNASSIGNED      */ WP_BUS_NONE, 0},
       {/* MANDATORY       */ WP_BUS_NONE, 0},
       {/* MANDATORY       */ WP_BUS_NONE, 0},
       /* ************* VIRTUAL BUS ASSIGNMENT ***************/
       {/* MCH_DATA        */ WP_BUS_PACKET, APP_BANK_PACKET}
       ,
       {/* WP_VB_OAM_FM_QUERY*/ WP_BUS_NONE, 0},
       {/* WP_VB_HASH_ON_HEAP*/ WP_BUS_NONE, 0},
#ifdef WP_BOARD_WDS3_SL
       {/* WP_VB_MFC_INTERNAL*/ WP_BUS_PARAM, APP_BANK_PARAM}
#else
       {/* WP_VB_MFC_INTERNAL*/ WP_BUS_PACKET, APP_BANK_PACKET}
#endif
    },
    {
       dps_ProgramImage, dps_ProgramImage
    },
    {
       dps_PC_Table_Init, dps_PC_Table_Init
    },
    &int_queue_tables, /* Interrupt queue tables */
    POOL_LIMIT,        /* Mixumum number of pools */
    NULL,              /* ATM global parameters */
    max_iw             /* Interworking global parameters */
   }
};

/* Configure Utopia port */
WP_port_pkt_limits pkt_limits = {
   /* max_tx_channels */ MAX_CHANNELS,
   /* max_rx_channels */ MAX_CHANNELS,
   /* pq_block_size   */ 0,
   /* n_pq_blocks     */ 0,
   /* emphy_devices   */
};
WP_port_upi_packet_ext pkt_ext = {
   /* packet_characteristics */
   /*WP_UPI_POS_CRC_ENABLE,*/ 0,
   /* slow_phy_tx_bitrates */ 0,
   /* packet_limits */ &pkt_limits
};

WP_port_upi upi_s[] = {{
   /* upimode */ WP_UPI_POS_HIGHSPEED,
   /* loopback */ 0,
   /* tx_characteristics */ 0,
   /* rx_characteristics */ 0,
   /* atm_ext */ NULL,
   /* packet_ext */ &pkt_ext,
   /* iw_ext */ NULL,
   /* emphy_ext */ 0
}};

/* Configuration for the HSPOS Device */
WP_device_upi_hspos upi_hspos_device[] =
{
   {
      /* tx_bit_rate;*/ 622000000,
      /* tx_statmode;*/ WP_PKT_STAT_ENABLE,
      /* tx_intmode;*/ WP_PKTCH_INT_DISABLE,
      /* tx_slave_addr;*/ 0,
      /* tx_queuedepth[8];*/ {0},
      /* rx_bit_rate;*/ 622000000,
      /* rx_statmode;*/ WP_PKT_STAT_ENABLE,
      /* rx_intmode;*/ WP_PKTCH_INT_DISABLE,
      /* rx_queuedepth;*/ 2000,
      /* rx_slave_addr;*/ 0,
   }
};

WP_pool_buffer_data buffer_data_2048[] = {
   {/* n_buffers */ 400,
    /* offset */ 64,
    /* size */ 1984,
    /* pad */ 0,
#ifdef WP_BOARD_WDS3_SL
    /* bus */ WP_BUS_PARAM,
    /* bank */ APP_BANK_PARAM }
#else
    /* bus */ WP_BUS_PACKET,
    /* bank */ APP_BANK_PACKET }
#endif
};

WP_pool_buffer_data buffer_data_256[] = {
   {/* n_buffers */ 4096,
    /* offset */ 0,
    /* size */ 256,
    /* pad */ 0,
#ifdef WP_BOARD_WDS3_SL
    /* bus */ WP_BUS_PARAM,
    /* bank */ APP_BANK_PARAM}
#else
    /* bus */ WP_BUS_PACKET,
    /* bank */ APP_BANK_PACKET}
#endif
};

WP_pool_buffer_data buffer_data_144[] =
{
   {
      /* n_buffers */ 50,
      /* offset */ 0,
      /* size */ 144,
      /* pad */ 0,
#ifdef WP_BOARD_WDS3_SL
      /* bus */ WP_BUS_PARAM,
      /* bank */ APP_BANK_PARAM
#else
      /* bus */ WP_BUS_PACKET,
      /* bank */ APP_BANK_PACKET
#endif
   }
};

WP_pool_ring_data ring_data[] =
{
   {
      /* n_rings */ 2048,
      /* ring_length */ 10,
      /* bus */ WP_BUS_PARAM,
      /* bank */ APP_BANK_PARAM
   }
};

WP_pool_ring_data ring_data_host[] = {
   {/* n_rings */ 4096,
    /* ring_length */ 10,
    /* bus */ WP_BUS_PARAM,
    /* bank */ APP_BANK_PARAM}
};

WP_qnode_hostq qn_host[] =
{
   {
      /* pool_buffer */ 0,
      /* pool_ring */ 0,
      /* interruptqueue */ WP_IRQT0,
   }
};

WP_qnode_iwq qn_iw[]=
{
   {
      /* interruptqueue */ 0,
      /* num_buffers */ 400,
      /* adjunct_pool */ 0
   }
};

/* HDLC channel configuration */
WP_ch_hdlc ch_config_hdlc[] =
{
   {
      /* intmode */ WP_PKTCH_INT_DISABLE,
      /* iwmmode */  WP_PKTCH_IWM_DISABLE,
      /* testmode */ WP_PKTCH_TEST_DISABLE,
      /* tx_pqblock */ 0,
      /* tx_pqlevel */ 0,
      /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
      /* tx_shaping_params */ NULL,
      /* rx_maxsdu */ 1536,
      /* tx_cwid */   WP_CW_ID_B,
      /* tx_tq */     0
   }
};

WP_tx_binding_bytes tx_binding_pos[1]=
{
   {
      /* maxt;*/     (1<<27)-1
   }
};

static void WPE_GroupsCreate(struct trial *t);
static void WPE_CreateChannels(struct trial *t);
static void WPE_EnableObjects(struct trial *t);
static void WPE_DisableObjects(struct trial *t);
static void WPE_CheckStatistics(void);
static void WPE_SendTraffic(void);
static void WPE_ReceiveTraffic(void);
void WT_Trial(struct trial *t);

WT_test_system test_system;

WP_handle pool_256,pool_2048,qnh,qniw;
WP_handle pool_ring_host;
WP_handle h_iw_port_host;

WP_U32 num_fast_devices;
WP_U32 num_fast_l1_groups;
WP_U32 num_fast_l2_groups;
WP_U32 num_fast_channels;

WP_U32 entry, ii;
WP_U32 i,j,k,frame;
WP_status status;

WP_data_unit generator_data_unit;
WP_data_segment km_tx_buf;
WP_U32 sent_data_size;
WP_CHAR send_buffer[MAX_DATA_SIZE];
WP_CHAR receive_buffer[MAX_DATA_SIZE];

/**********************************************************/
WP_handle tx_pos_handle[4][288][8],rx_pos_handle[4][288];
WP_handle rx_host_handle,tx_host_handle;

WP_handle upidev[4][288],host_device,host_port;

WP_stats_upi_hspos hspos_stats = {0};
/**********************************************************/

/*****************************************************************************
 * WT_Trial
 *
 * Perform one complete configuration.
 ****************************************************************************/
void WT_Trial (struct trial *t)
{

   num_fast_devices = NUM_OF_UPI * t->num_of_phys;

   WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_LOG,logCallBack);
   WP_ControlRegister(WP_DEBUG_CALLBACK_FILE_LINE_ERROR,WTE_DebugCallback);

   status = WP_DriverInit();
   terminate_on_error(status, "WP_DriverInit()",__LINE__);

#ifndef USE_SOCKET
   WPX_BoardConfigure(0, WPX_CONFIGURE_DEFAULT_CHECKIN);
#endif

   status = WP_SysInit(WP_WINPATH(0), context);
   terminate_on_error(status, "WP_SysInit()",__LINE__);

   /*Creates buffer pools*/
   pool_256 = WP_PoolCreate(WP_WINPATH(0), WP_pool_buffer, buffer_data_256);
   terminate_on_error(pool_256, "WP_PoolCreate() pool_256",__LINE__);

   pool_2048 = WP_PoolCreate(WP_WINPATH(0), WP_pool_iwbuffer, buffer_data_2048);
   terminate_on_error(pool_2048, "WP_PoolCreate() pool_2048",__LINE__);

   /* Create the Ring pool(s) */
   pool_ring_host = WP_PoolCreate(WP_WINPATH(0), WP_pool_ring, ring_data_host);
   terminate_on_error(pool_ring_host, "WP_PoolCreate() pool_ring_host",__LINE__);

   qn_host->pool_buffer = pool_256;
   qn_host->pool_ring = pool_ring_host;
   qnh = WP_QNodeCreate(0,WP_QNODE_HOSTQ,qn_host);
   terminate_on_error(qnh, "WP_QNodeCreate() qnh",__LINE__);

   /* Create an interworking queue node */
   qn_iw->adjunct_pool = pool_2048;
#ifdef WP_HW_WINPATH2
   qniw = WP_QNodeCreate(0, \
           WP_QNODE_IWQ | WP_QNODE_OPT_DEDICATED_RX_HWQ | WP_QNODE_OPT_FMU, qn_iw);
#else
   qniw = WP_QNodeCreate(0, WP_QNODE_IWQ | WP_QNODE_OPT_FMU, qn_iw);
#endif
   terminate_on_error(qniw, "WP_QNodeCreate() qniw",__LINE__);

/************** create ports & devices ****************/

   /* Create POS Port */
   upi_s[0].tx_characteristics =
      t->flowmode |
      t->phymode |
      t->tqmode |
      WP_UPI_MASTER |
      WP_UPI_PRT_ODD |
      WP_UPI_ROBIN |
      WP_TX_PRIORITY_8 |
      WP_UPI_LEVEL3 |
      WP_UPI_HANDSHAKE_BYTE |
      WP_UPI_BUS_WIDTH_32;

   upi_s[0].rx_characteristics =
      t->phymode |
      WP_UPI_PRT_ODD |
      WP_UPI_ROBIN |
      WP_UPI_PRTCHK_ON |
      WP_UPI_LEVEL3 |
      WP_UPI_HANDSHAKE_BYTE |
      WP_UPI_BUS_WIDTH_32;

   if(t->phymode == WP_UPI_MULTI)
   {
      upi_s[0].rx_characteristics |= WP_UPI_MASTER;
      upi_s[0].loopback = WP_UPI_NORMAL;
   }
   else
   {
      upi_s[0].rx_characteristics |= WP_UPI_SLAVE;
      upi_s[0].loopback = WP_UPI_LOOPBACK;
   }

   for (i=0; i < NUM_OF_UPI; i++)
   {
      /* Create UPI port */
      test_system.ports[i].handle = WP_PortCreate(WP_WINPATH(0), WP_PORT_UPI1+i, upi_s);
      terminate_on_error(test_system.ports[i].handle, "WP_PortCreate()",__LINE__);

      for (j=0; j < t->num_of_phys; j++)
      {
         /* Create HSPOS Device */
         upidev[i][j] = WP_DeviceCreate(test_system.ports[i].handle, \
                                        WP_PHY(t->used_phys[j]), \
                                        WP_DEVICE_HSPOS, \
                                        &upi_hspos_device[0]);
         terminate_on_error(upidev[i][j], "WP_DeviceCreate()",__LINE__);
      }
   }

/***************************************************************************/

   status = WP_SysCommit();
   terminate_on_error(status, "WP_SysCommit()",__LINE__);
   WT_Identify();
   WPE_GroupsCreate(t);
   WPE_CreateChannels(t);
   WPE_EnableObjects(t);
 #if RE_ENABLE_DEVICES
   WPU_DUMP("\nRe-enabling channels...\n");
   WPE_DisableObjects(t);
   WPE_CreateChannels(t);
   WPE_EnableObjects(t);
#endif

   menu_interface();

   WTE_Terminate();
   return;
}

WP_S32 main(WP_S32 argc, WP_CHAR **argv)
{
   struct trial *t;
#ifdef USE_SOCKET
   InitSocketInterface();
#endif
   WPU_DUMP("Beginning Trials\n");
   for (t = trials; t->name; t++) {
      WT_Trial (t);
   }
#ifdef USE_SOCKET
   CleanUpSocket(5);
#endif
   return 0;
}

static void WPE_GroupsCreate(struct trial *t)
{
   /* create shaping groups and blocks */
   for (i=0; i < NUM_OF_UPI; i++)
   {
      for (j=0; j < t->num_of_phys; j++)
      {
         WP_U32 l1_index = i*t->num_of_phys + j;
         WP_boolean is_strict;

         WTE_AssignShapingType(WT_L1,WT_CIR_EIR,
                               &l1_group_config->tx_shaping_type,
                               &l1_group_config->tx_shaping_params,
                               &is_strict);

         test_system.l1_groups[l1_index] = WP_ShapingGroupCreate(upidev[i][j],
                                                                 WP_SHAPING_GROUP_TYPE_PACKET,
                                                                 l1_group_config);
         terminate_on_error(test_system.l1_groups[l1_index] ,\
                            "WP_ShapingGroupCreate l1 group",__LINE__);

         test_system.l2_blocks[l1_index] = WP_ShapingBlockCreate( WP_SYSHANDLE(WP_WINPATH(0)),
                                                                  WP_L2_GROUP,
                                                                  (CHANNELS_PER_DEVICE+7)/8,
                                                                  WP_NUM_FIFOS_8);
         terminate_on_error(test_system.l2_blocks[l1_index] , "WP_ShapingBlockCreate",__LINE__);

         l2_group_config->block_level = 0;
         l2_group_config->block_handle = test_system.l2_blocks[l1_index];
         test_system.l2_groups[l1_index] = WP_ShapingGroupCreate(test_system.l1_groups[l1_index],
                                                                 WP_SHAPING_GROUP_TYPE_PACKET,
                                                                 l2_group_config);
         terminate_on_error(test_system.l2_groups[l1_index] ,\
                           "WP_ShapingGroupCreate l2 group",__LINE__);
      }
   }
}

static void WPE_CreateChannels(struct trial *t)
{
   /* create HDLC channels */
   for (i=0; i < NUM_OF_UPI; i++)
   {
      for (j=0; j < t->num_of_phys; j++)
      {
         for(k = 0; k < CHANNELS_PER_DEVICE; k++)
         {
            WP_U32 l1_index = i*t->num_of_phys + j;

            /* create HDLC channels*/
            ch_config_hdlc[0].iwmode = WP_PKTCH_IWM_ENABLE;
            ch_config_hdlc[0].tx_tq = k;

#if WT_DEBUG
            WPU_DUMP("%s:%d Tx ch #%d:%d:%d\n",__FILE__,__LINE__,i,j,k);
#endif
            tx_pos_handle[i][j][k] = WP_ChannelCreate(TX_CHANNEL_POS_TAG+j,
                                                   test_system.l2_groups[l1_index],
                                                   qniw,
                                                   WP_CH_TX,
                                                   WP_HDLC,
                                                   &ch_config_hdlc[0]);
            terminate_on_error(tx_pos_handle[i][j][k], "WP_ChannelCreate() pos TX",__LINE__);

            /* bind Tx channels */
            status =  WP_IwTxBindingCreate(tx_pos_handle[i][j][k],\
                                           WP_IW_TX_BINDING_BYTES, &tx_binding_pos[0]);
            terminate_on_error(status, "WP_IwTxBindingCreate() pos TX",__LINE__);
         }

         ch_config_hdlc[0].iwmode = WP_PKTCH_IWM_DISABLE;

#if WT_DEBUG
            WPU_DUMP("%s:%d Tx ch #%d:%d\n",__FILE__,__LINE__,i,j);
#endif
         rx_pos_handle[i][j] = WP_ChannelCreate(RX_CHANNEL_POS_TAG+j,
                                                upidev[i][j],
                                                qniw,
                                                WP_CH_RX,
                                                WP_HDLC,
                                                &ch_config_hdlc[0]);
         terminate_on_error(rx_pos_handle[i][j], "WP_ChannelCreate() pos RX",__LINE__);
      }
   }
}

static void WPE_EnableObjects(struct trial *t)
{
   /* enable shaping groups */
   WTE_GroupsEnable();

   /* enable ports, devices and channels */
   for (i=0; i < NUM_OF_UPI; i++)
   {
      status = WP_PortEnable(test_system.ports[i].handle, WP_DIRECTION_DUPLEX);
      terminate_on_error(status, "WP_PortEnable()",__LINE__);

      for (j=0; j < t->num_of_phys; j++)
      {
         status = WP_DeviceEnable(upidev[i][j], WP_DIRECTION_DUPLEX);
         terminate_on_error(status, "WP_DeviceEnable()",__LINE__);

         for(k = 0; k < CHANNELS_PER_DEVICE; k++)
         {
            status = WP_ChannelEnable(tx_pos_handle[i][j][k]);
            terminate_on_error(status, "WP_ChannelEnable() pos TX",__LINE__);

         }
      }
   }
}

static void WPE_DisableObjects(struct trial *t)
{
   /* disable ports, devices and channels */
   for (i=0; i < NUM_OF_UPI; i++)
   {
      for (j=0; j < t->num_of_phys; j++)
      {
         for(k = 0; k < CHANNELS_PER_DEVICE; k++)
         {
            status = WP_ChannelDisable(tx_pos_handle[i][j][k]);
            terminate_on_error(status, "WP_ChannelDisable() pos TX",__LINE__);

            status = WP_ChannelDelete(tx_pos_handle[i][j][k]);
            terminate_on_error(status, "WP_ChannelDelete() pos TX",__LINE__);
         }

         status = WP_ChannelDelete(rx_pos_handle[i][j]);
         terminate_on_error(status, "WP_ChannelDelete() pos RX",__LINE__);

         status = WP_DeviceDisable(upidev[i][j], WP_DIRECTION_DUPLEX);
         terminate_on_error(status, "WP_DeviceDisable()",__LINE__);
      }
      status = WP_PortDisable(test_system.ports[i].handle, WP_DIRECTION_DUPLEX);
      terminate_on_error(status, "WP_PortDisable()",__LINE__);
   }

   /* disable shaping groups */
   WTE_GroupsDisable();
}

static void WPE_CheckStatistics(void)
{
   struct trial *t = trials;

   for (i=0; i < NUM_OF_UPI; i++)
   {
      for (j=0; j < t->num_of_phys; j++)
      {
        WTE_DisplayDeviceStats(upidev[i][j], WP_DEVICE_HSPOS,"UPI HSPOS");
      }
   }
}

static void WPE_SendTraffic(void)
{
   struct trial *t = trials;
   for (i=0; i < NUM_OF_UPI; i++)
   {
      for (j=0; j < t->num_of_phys; j++)
      {
         for(k = 0; k < CHANNELS_PER_DEVICE; k++)
         {
            for(frame = 0; frame < FRAMES_PER_CHANNEL; frame++)
            {
               /* Send a packet to the UPI port */
               WPU_DUMP("Sending via UPI%d Phy %d\n",i,t->used_phys[j]);
               WTE_HostSendIPv4( tx_pos_handle[i][j][k],
                               WP_IW_VLAN_AWARE_BRIDGING_MODE,
                               "05");
               WPU_DUMP("\n");

            }
         }
      }
   }
}

static void WPE_ReceiveTraffic()
{
   struct trial *t = trials;
   for (i=0; i < NUM_OF_UPI; i++)
   {
      for (j=0; j < t->num_of_phys; j++)
      {
         for(k = 0; k < CHANNELS_PER_DEVICE; k++)
         {
            for(frame = 0; frame < FRAMES_PER_CHANNEL; frame++)
            {
               WPU_DUMP("Listening on UPI%d Phy %d\n",i,t->used_phys[j]);
               WTE_HostReceiveIPv4(rx_pos_handle[i][j]);
               WPU_DUMP("\n");
            }
         }
      }
   }
}


void CLI_ProgramQuit(void)
{
  WP_status status;
  status = WP_DriverRelease();
  return;
}

Y_MenuEntry V_MainMenu[] =
{
  {5     , TRUE, "UPI AAL5 Test",       {V_MainMenu}},
  {K_Leaf, TRUE, "Send Packet HSPOS",     {WPE_SendTraffic}},
  {K_Leaf, TRUE, "Receive Packet HSPOS",  {WPE_ReceiveTraffic}},
  {K_Leaf, TRUE, "HSPOS Statistics",    {WPE_CheckStatistics}},
  {K_Menu, TRUE, "Utilities and Debug", {V_Menu_Debug}},
  {K_Leaf, TRUE, "Quit the program",    {CLI_ProgramQuit}}
};

void menu_interface()
{
   static char InputBuf[512];
   F_SetMainMenu();
   F_DisplayMenu();
   while(TRUE)
   {
     WPU_DUMP("%s",STR_ENTER_COMMAND);
     memset(InputBuf,0,512);
     gets(InputBuf);
     WPU_DUMP("\n");
     if(strcmp(InputBuf,"cont") == 0)
        break;
     F_MenuEngine(InputBuf);
   }
}

/*****************************************************************************
 * (C) Copyright 2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wt_fmu_test_util.c
 *
 * Purpose: FMU tests utility functions
 *
 ****************************************************************************/

#ifndef WT_FMU_TEST_UTIL_H
/*****************************************************************************
 * (C) Copyright 2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wt_fmu_test_util.h
 *
 * Purpose: FMU tests utility functions
 *
 ****************************************************************************/
#ifndef WT_FMU_TEST_UTIL_H
#define WT_FMU_TEST_UTIL_H

#ifdef __linux__
#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define TESTSTRING(x) TESTSTRING_(x)
#define TESTSTRING_(x) #x

#define WT_DEBUG_COMMON_PRINTS   0x1
#define WT_DEBUG_DISPLAY_STATS   0x2
#define WT_DEBUG_DISPLAY_TRAFFIC 0x4

#define WT_LOOPBACK_INTERNAL   0
#define WT_LOOPBACK_SERDES     1
#define WT_LOOPBACK_PHY_CLOSE  2
#define WT_LOOPBACK_PHY_MIDDLE 3 
#define WT_LOOPBACK_PHY_FAR    4
#define WT_LOOPBACK_EXTERNAL   5
#define WT_LOOPBACK_OFF        6

typedef enum
{
   WT_ENET,
   WT_HSPOS,
   WT_POS
} WT_port_type;

typedef enum 
{
   IW_SYS_TAG = 0,
   FLOW_AGG_TAG,
   BPORT_TAG,
   IW_TX_CH_TAG,
   IW_RX_CH_TAG,
   TX_CHANNEL_POS_TAG = 100,
   RX_CHANNEL_POS_TAG = 200,
   TX_CHANNEL_ENET_TAG = 400,
   RX_CHANNEL_ENET_TAG = 800
}WT_test_tags;

typedef enum
{
   WT_L1 = 0,
   WT_L2,
   WT_L3
} WT_shaping_level;

typedef enum
{
   WT_STRICT = 0,
   WT_WFQ,
   WT_WRR,
   WT_CIR_EIR,
   WT_CIR_EIR_STRICT,
   WT_1STRICT_7CIR_EIR,
   WT_2STRICT_6CIR_EIR,
   WT_1STRICT_7WRR,
   WT_2STRICT_6WRR
} WT_shaping_type;

#define KILO 1000
#define MEGA 1000000
#define GIGA 1000000000LL

#define MAX_L1_GROUPS 304
#define MAX_L2_GROUPS 2048
#define MAX_L3_GROUPS 16384

#define MAX_L1_BLOCKS   MAX_L1_GROUPS
#define MAX_L2_BLOCKS   MAX_L2_GROUPS

#define MAX_PORTS       16 //referring to ENET ports
#define MAX_DEVICES     MAX_L1_GROUPS
#define MAX_TX_CHANNELS MAX_L3_GROUPS
#define MAX_RX_CHANNELS MAX_DEVICES

#ifdef USE_SOCKET
#define WT_RECEIVE_LOOPS 500
#define WT_RECEIVE_DELAY 2000000
#else
#define WT_RECEIVE_LOOPS 100
#define WT_RECEIVE_DELAY 2000
#endif

typedef struct
{
      WP_U32    name;
      WP_handle handle;
} WT_port;

typedef struct
{
      WP_handle handle;
      WP_handle rx_channel;
      WP_U8 tx_shaping_type;
} WT_tx_channel;

typedef struct
{
      WT_port   ports[MAX_PORTS];
      WP_handle devices[MAX_DEVICES];
      WP_handle l1_blocks[MAX_L1_BLOCKS];
      WP_handle l1_groups[MAX_L1_GROUPS];
      WP_handle l2_blocks[MAX_L2_BLOCKS];
      WP_handle l2_groups[MAX_L2_GROUPS];
      WP_handle rx_channels[MAX_RX_CHANNELS];
      WT_tx_channel tx_channels[MAX_TX_CHANNELS];
} WT_test_system;

typedef enum
{
   WT_CONNECT_INDEXED = 0x10000,
   WT_CONNECT_GROUPED = 0x20000,
   WT_CONNECT_SINGLE  = 0x40000,
   WT_CONNECT_MASK    = 0xffff0000,
} WT_fmu_connect_mode;

typedef struct
{
      WP_U32 parent_offset;
      WP_U32 shaping_type;
      void *shaping_params;
} WT_fmu_group_configuration;

typedef struct
{
      WP_U32 num_groups;
      WP_boolean identical;
      WT_fmu_group_configuration group[MAX_L1_GROUPS];
} WT_fmu_l1_configuration;

typedef struct
{
      WP_U32 num_groups;
      WP_U32 connect_mode;
      WP_boolean identical;
      WT_fmu_group_configuration group[MAX_L2_GROUPS];
} WT_fmu_l2_configuration;

typedef struct
{
      WP_U32 num_groups;
      WP_U32 connect_mode;
      WP_boolean identical;
      WT_fmu_group_configuration group[MAX_L3_GROUPS];
} WT_fmu_l3_configuration;

typedef struct
{
      WP_handle device_handle;
      WP_U64    device_bitrate;
      WT_fmu_l1_configuration l1;
      WT_fmu_l2_configuration l2;
      WT_fmu_l3_configuration l3;
} WT_fmu_device_configuration;

typedef union StatField
{
      WP_U64 field;
      struct {
            WP_U32 high;
            WP_U32 low;
      } part;
} WPT_StatField;

typedef struct
{
      unsigned Vers                : 4  ;/* Version number.*/
      unsigned IHL                 : 4  ;/* ethernet Header Length*/
      unsigned TOS                 : 8  ;/* Type of Service.*/
      unsigned TotalLength         : 16 ;/* Total Length is the length of the datagram.*/
      unsigned Identification      : 16 ;/* identifying fragments of a datagram.*/
      unsigned Flags               : 3  ;/* Control Flags (Bit0:res,Bit1:1 = Don't Fragment,Bit2:0 = Last Fragment)*/
      unsigned FragmentOffset      : 13 ;/* Indicates where in the datagram this fragment belongs */
      unsigned TimeToLive          : 8  ;/* Time to Live */
      unsigned Protocol            : 8  ;/* Indicates the next level protocol used*/
      unsigned HeaderChecksum      : 16 ;/* checksum on the header only*/
      unsigned SourceAddress       : 32 ;/* Source address*/
      unsigned DestinationAddress  : 32 ;/* Destination address*/
} S_IPv4Header;

/* shaping structures */
WT_fmu_device_configuration fast_fmu_cfg[] =
{
   {
      /* device handle  */ 0,
      /* device_bitrate */ WP_TX_BIT_RATE_UNLIMITED,
      /* L1 config */
      {
         /* num_groups    */ 1,
         /* identical     */ 1,
         /* group         */ {{0,WP_FMU_SHAPING_TYPE_STRICT,NULL}},
      },
      /* L2 config */
      {
         /* num_groups    */ 1,
         /* connect_mode  */ WT_CONNECT_SINGLE,
         /* identical     */ 1,
         /* group         */ {{0,WP_FMU_SHAPING_TYPE_STRICT,NULL}},
      },
      /* L3 config */
      {
         /* num_groups    */ 1,
         /* connect_mode  */ WT_CONNECT_SINGLE,
         /* identical     */ 1,
         /* group         */ {{0,WP_FMU_SHAPING_TYPE_STRICT,NULL}},
      },
   },
};

#define WT_WFQ_WEIGHT_UNIT 4096
WP_fmu_shaping_wfq wfq_shaping_params[] =
{
   {
      /* weight */ WT_WFQ_WEIGHT_UNIT,
      /* flags  */ 0
   }
};

#define WT_WRR_WEIGHT_UNIT 128
WP_fmu_shaping_wrr wrr_shaping_params[] =
{
   {
      /* weight */ WT_WRR_WEIGHT_UNIT,
      /* flags  */ 0
   }
};

#define WT_DEFAULT_CBS (10*1536*8)
#define WT_DEFAULT_EBS (10*1536*8)
WP_fmu_shaping_cir_eir cir_eir_shaping_params[] =
{
   {
      /* cir   */ 900000000,
      /* cbs   */ WT_DEFAULT_CBS,
      /* eir   */ 100000000,
      /* ebs   */ WT_DEFAULT_EBS,
      /* flags */ 0
   }
};

WP_fmu_shaping_strict_and_shaped strict_and_shaped_shaping_params[] =
{
   {
      /* shaping_type  */ 0,
      /* shaping_param */ NULL,
      /* flags         */ 0
   }
};

WP_shaping_group_type_packet l1_group_config[] =
{
   {
         /* group_level */     WP_L1_GROUP,
         /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
         /* shaping_params */  NULL,
         /* num_fifos */       WP_NUM_FIFOS_8,
         /* block_handle */    0,
         /* block_level */     0,
         /* group_mode */      0
   }
};

WP_shaping_group_type_packet l2_group_config[] =
{
   {
         /* group_level */     WP_L2_GROUP,
         /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
         /* shaping_params */  NULL,
         /* num_fifos */       WP_NUM_FIFOS_8,
         /* block_handle */    0,
         /* block_level */     0,
         /* group_mode */      WP_MODE_HW
   }
};

WP_tx_binding_bytes tx_binding_bytes[] =
{
   {
      /* maxt_bytes;*/0x7ffff,
   }
};

WP_tx_binding tx_binding[] =
{
   {
      /* wred_mode;*/ 0,
      /* dci_mode; */ 1,
      /* maxt;     */ 255,
   }
};

/* Terminal functions */
#define STR_MENU_PROMPT    ">"
#define WTE_TTY_LOAD_TIME 1000000
#define WTE_TTY_FIRST_CHAR 1
#define WTE_TTY_FIRST_LINE 1
#define WTE_TTY_COLUMN_WIDTH 32
#define WTE_TTY_COLUMN(n) ((n-1)*WTE_TTY_COLUMN_WIDTH+WTE_TTY_FIRST_CHAR)

#define WEB_UART_LINE_STATUS_REG_OFFSET 0xc51c
#define WEB_UART_STATUS_RXFE            0x80
#define WEB_UART_STATUS_TEMT            0x40
#define WEB_UART_STATUS_THRE            0x20
#define WEB_UART_STATUS_BI              0x10
#define WEB_UART_STATUS_FE              0x08
#define WEB_UART_STATUS_PE              0x04
#define WEB_UART_STATUS_OE              0x02
#define WEB_UART_STATUS_DR              0x01
void Vt100Bright(void);
void Vt100Reverse(void);
void Vt100Color(WP_S32 foreground, WP_S32 background);
void Vt100Normal(void);
void Vt100GotoXy(WP_S32 x, WP_S32 y);
void Vt100EraseEOL(void);
void Vt100ClearScreen(void);
void Vt100CrsrInvisible(void);
void Vt100CrsrVisible(void);
void Vt100CrsrUp(WP_U32 up);
void Vt100CrsrDown(WP_U32 down);
void Vt100EraseDown(void);
WP_U8 F_SampleSerial(void);

/* Test functions */
WP_status logCallBack(WP_S32 level, const WP_CHAR *message);

WP_status WTE_DebugCallback(const WP_CHAR *file, const WP_CHAR *function,
                            WP_S32 line, WP_S32 error_id);
void WTE_Terminate(void);

void terminate_on_error(WP_handle handle, WP_CHAR *s,WP_U32 line);

void terminate_on_no_error(WP_handle handle, WP_CHAR *s,WP_U32 line);

WP_U16 ChecksumCalc(S_IPv4Header *IPHeader);

WP_U32 F_InsertHeader(WP_U32 system_type, WP_CHAR* tos);

void F_IwDataUnitsSetupIPv4(WP_U32 system_type, WP_CHAR* tos, WP_U32 data_type);

WP_status WTE_HostSendIPv4( WP_handle ch_handle,
                            WP_U32 system_type,
                            WP_CHAR* tos);
WP_status WTE_HostSendDirectIPv4( WP_handle handle,
                                  WP_U32 system_type,
                                  WP_CHAR* tos);

void WTE_ResetHandles(void);

WP_status WTE_HostReceiveIPv4(WP_handle ch_handle);

WP_boolean WTE_CheckData(void);

WP_status WTE_RunChannelTraffic(WP_handle tx_channel,
                                WP_handle rx_channel,
                                WP_boolean check_data);

WP_status WTE_RunTraffic(WP_U32 num_channels);

void WTE_DeviceChannelsCreate(WP_U32 device_index,
                              WP_U32 *l1_offset,
                              WP_U32 *l2_offset,
                              WP_U32 *l3_offset,
                              WP_handle qnode,
                              WT_fmu_device_configuration *config,
                              WP_ch_enet *ch_config_enet);

void WTE_DevicesEnable(void);

void WTE_DevicesDisable(void);

void WTE_GroupsEnable(void);

void WTE_GroupsDisable(void);

void WTE_GroupsDelete(void);

void WTE_ChannelsEnable(void);

void WTE_ChannelsDisable(void);

void WTE_ChannelsDelete(void);

void WTE_TxBindingCreate(void);

void WTE_AssignShapingType(WP_U32 level,
                           WP_U32 index,
                           WP_U8 *shaping_type,
                           void **shaping_params,
                           WP_boolean *is_strict);

void WTE_Pause(void);

void WTE_DisplayEnetDevicesStats(void);

void WTE_DisplayXgiDevicesStats(void);

void WTE_CheckEnetDeviceStats(WP_handle device_handle, WP_U32 num_channels);

int WTE_CheckHsposDeviceStats(WP_handle device_handle);

WP_U32 WTE_DisplayDeviceStats(WP_handle device_handle, WP_device device, WP_CHAR* type);

WP_U32 WTE_DisplayIwPortStats( WP_handle port_handle, WP_CHAR* type);

void WTE_DisplayFlowStats(WP_handle agg, WP_CHAR* type);

void WTE_DisplayGenericFlowStats(WP_handle agg, WP_CHAR* type);

void WTE_DisplayPceGlobalStats(void);

void WTE_DisplayPceFilterStats(WP_handle filter);

WP_U32 WTE_DisplayAAL5RxChannelStats(WP_handle ch_handle, WP_CHAR *type);

WP_U32 WTE_DisplayAAL5TxChannelStats(WP_handle ch_handle, WP_CHAR *type, WP_U8 display);

#endif
#endif

WP_U8 log_debug = 1;
WP_status logCallBack(WP_S32 level, const WP_CHAR *message)
{
   if (log_debug || level == 0)
      printf("%s\n",message);
   return WP_OK;
}

WP_status WTE_DebugCallback(const WP_CHAR *file, const WP_CHAR *function,
                            WP_S32 line, WP_S32 error_id)
{
   if (error_id)
      printf("\n\nfile = %s, line = %d\nfunction = %s\n",file,line,function);
   return WP_OK;
}

void WTE_Terminate(void)
{
   WP_DriverRelease();

#if USE_SOCKET
   CleanUpSocket(5);
#endif

   exit(1);
}

void terminate_on_error(WP_handle handle, WP_CHAR *s,WP_U32 line)
{
   if (WP_ERROR_P(handle)) {
      printf("%s: Terminal error (handle 0x%x) %s\n", appname, handle, s);
      printf("%s: Error type - %s\n", appname, WP_error_name[WP_ERROR(handle)]);
      printf("line = %d\n",line);
      printf("Test Failed\n");
      WTE_Terminate();
   }
#if WT_DEBUG
   else
   {
      printf("%s - 0x%08x\n",s,handle);
   }
#endif
}

void terminate_on_no_error(WP_handle handle, WP_CHAR *s,WP_U32 line)
{
   if (strcmp(WP_error_name[WP_ERROR(handle)], s)==0)
      printf("Received expected error: %s\n",s);
   else
   {
      printf("Expected error: %s\n",s);
      terminate_on_error(handle,"Returned handle",line);
      printf("Test Failed\n");
      WTE_Terminate();
   }
}

/***************************************************************
 *                  PACKET SENDING
 *
 ***************************************************************/

WP_U16 ChecksumCalc(S_IPv4Header *IPHeader)
{
   unsigned int body,carry,i,tmp,tmpLsb,tmpMsb,headerLengh;
   unsigned short *ptr;
   unsigned short chksum;

   body=0xffff;
   chksum=(unsigned short)(~body);
   body=0;
   chksum=(unsigned short)(~body);
   ptr = (unsigned short*)IPHeader;
   body=0;
   carry=0;
   headerLengh = IPHeader->IHL *2;

   for (i=0; i<headerLengh ;i++) /*The IPv4 header is divided to 10 chunks of 2B */
   {
      if (i!=5)      /*The 6th chunk which contains the checksum bytes is skipped*/
      {
         tmp = *ptr;
         tmpMsb=tmp & 0xffff0000 ;
         tmpLsb=tmp & 0x0000ffff;

         /* add the MSB         */

         carry =(body + tmpMsb )>>16;
         body = ((body + tmpMsb)&0x0000ffff)+carry;

         /* add the LSB*/
         carry =(body + tmpLsb )>>16;
         body = ((body + tmpLsb)&0x0000ffff)+carry;
      }

      ptr ++;
   }
   chksum=(unsigned short)(~body);
   IPHeader->HeaderChecksum = chksum;

   return(chksum);
}

WP_U32 F_InsertHeader(WP_U32 system_type, WP_CHAR* tos)
{
   WP_CHAR ascii_buffer[MAX_DATA_SIZE] = "";
   WP_U32 header_offset = 0;
   WP_U16 checksum = 0;
#if 0
    switch (system_type)
    {
       case WP_IW_ROUTING_MODE:
          strcpy (ascii_buffer,"");
          ipv4_header_offset = 0;
          break;
       case WP_IW_VLAN_AWARE_BRIDGING_MODE:
          strcpy (ascii_buffer,"0000000b0000");/* mac da */
          strcat (ascii_buffer,"0000000a0000");/* mac sa */
          strcat (ascii_buffer,"8100");/* VLAN type */
          strcat (ascii_buffer,"0015");/* VLAN */
          strcat (ascii_buffer,"0800");
          ipv4_header_offset = 18;
          break;
       default:
          break;
    }
#endif

   strcat (ascii_buffer,"0a0b0c0d0000"); /* Mac DA */
   strcat (ascii_buffer,"010203040506"); /* Mac SA */

   strcat (ascii_buffer,"45");
   strcat (ascii_buffer,tos);/* TOS */
   strcat (ascii_buffer,"0020");
   strcat (ascii_buffer,"001ba57d");
   strcat (ascii_buffer,"07110000"); /* Protocol */
   strcat (ascii_buffer,"11112222"); /* IP SA */
   strcat (ascii_buffer,"33334444"); /* IP DA */
   strcat (ascii_buffer,"00080008");
   strcat (ascii_buffer,"001b77c0");
   strcat (ascii_buffer,"00a4c5ff001b77c4\
3a528a3f001b77c8eeffeeff0000000000000000000000000000000000000000");

   App_TranslateAsciiToHex(send_buffer,ascii_buffer,DATA_LENGTH);

   checksum = ChecksumCalc((S_IPv4Header *)&send_buffer[header_offset]);

   header_offset += 68;
   return header_offset;
}

WP_U32 data_unit_index = 0;
void F_IwDataUnitsSetupIPv4(WP_U32 system_type, WP_CHAR* tos, WP_U32 data_type)
{
   WP_U8 *km_data_ptr = NULL;
   WP_U32 ipv4_header_offset = 0;
   WP_U32 ii,jj;

   /* Host Send  */
   km_data_ptr = WP_PoolAlloc(pool_2048);
   if(km_data_ptr == NULL) WTE_Terminate();

   km_tx_buf.data                 = km_data_ptr;
   km_tx_buf.pool_handle          = pool_2048;
   generator_data_unit.segment    = (&km_tx_buf);
   generator_data_unit.type       = data_type;
   generator_data_unit.data_size  = DATA_LENGTH;
   generator_data_unit.n_active   = 1;
   km_tx_buf.data_size            = generator_data_unit.data_size;
   km_tx_buf.next                 = NULL;
   km_tx_buf.displacement         = 0;
   generator_data_unit.n_segments = 1;
   generator_data_unit.control    = WP_HT_IW_B_MODE_FIELD(WP_HT_IW_B_NORMAL_MODE)|
                                    WP_HT_IW_CRC_FIELD(0) |
                                    WP_HT_IW_FSP_VLAN_BPORT_FIELD(0);

   sent_data_size = generator_data_unit.data_size;

   /* create the header */
   ipv4_header_offset += F_InsertHeader(system_type,tos);

   /* create the payload */
   for(ii=ipv4_header_offset, jj=0; ii<generator_data_unit.data_size; ii++, jj++)
      send_buffer[ii] = (data_unit_index+1+jj)%256;

   data_unit_index++;

   WP_MEM_BYTES_FILL(km_data_ptr, 0, buffer_data_144->size);
   WP_MEM_BYTES_SET(km_data_ptr,send_buffer,DATA_LENGTH);
}

WP_status WTE_HostSendIPv4( WP_handle ch_handle,
                            WP_U32 system_type,
                            WP_CHAR* tos)
{
   WP_status status = WP_OK;
   WP_data_segment *curr_buff_ptr;

   F_IwDataUnitsSetupIPv4(system_type,tos,DATA_TYPE);

   curr_buff_ptr = generator_data_unit.segment;

#if WT_DEBUG
   printf("sent %d segments, %d long each\n",\
          generator_data_unit.n_active, curr_buff_ptr->data_size);
#if (WT_DEBUG & WT_DEBUG_DISPLAY_TRAFFIC)
   for(ii=0; ii < curr_buff_ptr->data_size; ii++)
   {
      if((ii)%16 == 0) printf("\n");
      printf("%2.2x",(WP_U8)send_buffer[ii] );
   }
   printf("\n");
#endif
#endif

   status = WP_HostSend( ch_handle, &generator_data_unit);
   terminate_on_error( status, "WP_HostSend()", __LINE__);

   generator_data_unit.n_active = 0;
   return status;
}

WP_status WTE_HostSendDirectIPv4( WP_handle handle,
                                  WP_U32 system_type,
                                  WP_CHAR* tos)
{
   WP_status status = WP_OK;
   WP_data_segment *curr_buff_ptr;

   F_IwDataUnitsSetupIPv4(system_type,tos,WP_DATA_IW);

   curr_buff_ptr = generator_data_unit.segment;

#if WT_DEBUG
   printf("sent %d segments, %d long each\n",\
          generator_data_unit.n_active, curr_buff_ptr->data_size);
#if (WT_DEBUG & WT_DEBUG_DISPLAY_TRAFFIC)
   for(ii=0; ii < curr_buff_ptr->data_size; ii++)
   {
      if((ii)%16 == 0) printf("\n");
      printf("%2.2x",(WP_U8)send_buffer[ii] );
   }
   printf("\n");
#endif
#endif

   status = WP_HostSendDirect( handle, 0, qniw, &generator_data_unit);
   terminate_on_error( status, "WP_HostSendDirect()", __LINE__);

   generator_data_unit.n_active = 0;

   return status;
}

#define NUM_OF_BUFS 2
WP_data_segment bufs[NUM_OF_BUFS];

void WTE_ResetHandles(void)
{
   WP_U32 i;

   for(i=0; i < NUM_OF_BUFS; i++)
   {
      bufs[i].next = &bufs[i+1];
      bufs[i].displacement = 0;
   }
   bufs[NUM_OF_BUFS -1].next = NULL;
}

WP_status WTE_HostReceiveIPv4(WP_handle ch_handle)
{
   WP_status status;
   WP_data_segment *curr_buff_ptr;
   WP_data_unit data_unit;
   WP_iw_data_return extended_info;

   WTE_ResetHandles();
   data_unit.segment = &bufs[0];
   data_unit.n_segments = NUM_OF_BUFS;
   data_unit.n_active = NUM_OF_BUFS;
   data_unit.type = DATA_TYPE;
   data_unit.data_return = &extended_info;

   for (ii = 0; ii < WT_RECEIVE_LOOPS; ++ii)
   {
      status = WP_HostReceive (ch_handle, &data_unit);
      if (WP_ERROR(status) == WP_ERR_HST_NO_DATA_TO_GET) /* didn't get yet? */
      {
         WP_Delay(WT_RECEIVE_DELAY);
      }
      else if(status != WP_OK)
         break;
      else
      {
         curr_buff_ptr = data_unit.segment;
         WP_MEM_BYTES_GET((WP_U8 *)receive_buffer,\
                          curr_buff_ptr->data,curr_buff_ptr->data_size);

#if WT_DEBUG
         printf("recieved %d segments, %d long each\n",\
                 data_unit.n_active, curr_buff_ptr->data_size);
#if (WT_DEBUG & WT_DEBUG_DISPLAY_TRAFFIC)
         for(ii=0; ii < curr_buff_ptr->data_size; ii++)
         {
            if((ii)%16 == 0) printf("\n");
            printf("%2.2x", (WP_U8)receive_buffer[ii]);
         }
         printf("\n\n");
#endif
#endif

         WP_PoolFree(bufs[0].pool_handle, bufs[0].data);
         break;
      }
   }
   return status;
}

WP_boolean WTE_CheckData(void)
{
   WP_boolean result = WP_TRUE;

   for(ii = 0; ii<sent_data_size; ii++)
   {
      if(send_buffer[ii] != receive_buffer[ii])
      {
#if WT_DEBUG
         printf("ERROR!!! Different data on byte #%d: sent value 0x%x received value 0x%x\n",
                ii,send_buffer[ii],receive_buffer[ii]);
#endif
         result = WP_FALSE;
         break;
      }
   }
   if(result == WP_FALSE)
   {
      printf("ERROR!!! different data\n");
      WTE_DisplayEnetDevicesStats();
      printf("Test Failed\n");
      WTE_Terminate();
   }

   return result;
}

WP_status WTE_RunChannelTraffic(WP_handle tx_channel,
                                WP_handle rx_channel,
                                WP_boolean check_data)
{
   WP_status status = WP_OK;
   WP_U32 frame;

   for(frame=0;frame < FRAMES_PER_CHANNEL;frame++)
   {
#if WT_DEBUG
      printf("Sending to ch 0x%08x [frame #%d]\n", tx_channel, frame);
#endif
      status = WTE_HostSendIPv4( tx_channel, 0, "05");
      if(status != WP_OK)
         break;

      status = WTE_HostReceiveIPv4(rx_channel);
      if(status != WP_OK)
         break;

      if(check_data)
         WTE_CheckData();
   }
   return status;
}

WP_status WTE_RunTraffic(WP_U32 num_channels)
{
   WP_status status = WP_OK;

   printf("\n----------------- Run Traffic -----------------\n");

   for(entry=0;entry < num_channels;entry++)
   {
      status = WTE_RunChannelTraffic(test_system.tx_channels[entry].handle,
                                     test_system.tx_channels[entry].rx_channel, WP_TRUE);

      if(status != WP_OK)
         break;
   }
   return status;
}

/***************************************************************
 *                  HANDLING WDDI OBJECTS
 *
 ***************************************************************/

void WTE_DeviceChannelsCreate(WP_U32 device_index,
                              WP_U32 *l1_offset,
                              WP_U32 *l2_offset,
                              WP_U32 *l3_offset,
                              WP_handle qnode,
                              WT_fmu_device_configuration *config,
                              WP_ch_enet *ch_config_enet)
{
   WP_U32 l1_index,l2_index,l3_index;
   WP_U32 l3_count=0,last_l2=0xffffffff;
   WP_CHAR str[100];

   config->device_handle = test_system.devices[device_index];

   /* FMU L1 */
   test_system.l1_blocks[device_index] = WP_ShapingBlockCreate(WP_SYSHANDLE(WP_WINPATH(0)),
                                                               WP_L1_GROUP,
                                                               config->l1.num_groups,
                                                               WP_NUM_FIFOS_8);
   sprintf(str, "WP_ShapingBlockCreate L1 #%d",device_index);
   terminate_on_error(test_system.l1_blocks[device_index], str,__LINE__);

   for(l1_index=0;l1_index < config->l1.num_groups;l1_index++)
   {
      l1_group_config->block_level = l1_index;
      l1_group_config->block_handle = test_system.l1_blocks[device_index];

      if(!config->l1.identical)
      {
         l1_group_config->tx_shaping_type = config->l1.group[l1_index].shaping_type;
         l1_group_config->tx_shaping_params = config->l1.group[l1_index].shaping_params;
      }
      else
      {
         l1_group_config->tx_shaping_type = config->l1.group[0].shaping_type;
         l1_group_config->tx_shaping_params = config->l1.group[0].shaping_params;
      }

      test_system.l1_groups[l1_index + *l1_offset] = \
                                              WP_ShapingGroupCreate(config->device_handle,
                                                                    WP_SHAPING_GROUP_TYPE_PACKET,
                                                                    l1_group_config);
      sprintf(str, "WP_ShapingGroupCreate L1 #%d",l1_index + *l1_offset);
      terminate_on_error(test_system.l1_groups[l1_index + *l1_offset], str,__LINE__);
   }

   /* FMU L2 */
   test_system.l2_blocks[device_index] = WP_ShapingBlockCreate(WP_SYSHANDLE(WP_WINPATH(0)),
                                                               WP_L2_GROUP,
                                                               config->l2.num_groups,
                                                               WP_NUM_FIFOS_8);
   sprintf(str, "WP_ShapingBlockCreate L2 #%d",device_index);
   terminate_on_error(test_system.l2_blocks[device_index], str,__LINE__);

   for(l2_index=0;l2_index < config->l2.num_groups;l2_index++)
   {
      switch(config->l2.connect_mode & WT_CONNECT_MASK)
      {
         case WT_CONNECT_INDEXED:
            l1_index = config->l2.group[l2_index].parent_offset;
            break;
         case WT_CONNECT_GROUPED:
            l1_index = l2_index / (config->l2.connect_mode & ~WT_CONNECT_MASK);
            break;
         case WT_CONNECT_SINGLE:
            l1_index = config->l2.connect_mode & ~WT_CONNECT_MASK;
            break;
         default:
            printf("WT ERROR in %s:%d\n",__FILE__,__LINE__);
            WTE_Terminate();
            break;
      }

      l2_group_config->block_level = l2_index;
      l2_group_config->block_handle = test_system.l2_blocks[device_index];

      if(!config->l2.identical)
      {
         l2_group_config->tx_shaping_type = config->l2.group[l2_index].shaping_type;
         l2_group_config->tx_shaping_params = config->l2.group[l2_index].shaping_params;
      }
      else
      {
         l2_group_config->tx_shaping_type = config->l2.group[0].shaping_type;
         l2_group_config->tx_shaping_params = config->l2.group[0].shaping_params;
      }

      test_system.l2_groups[l2_index + *l2_offset] =\
                       WP_ShapingGroupCreate(test_system.l1_groups[l1_index + *l1_offset],
                                             WP_SHAPING_GROUP_TYPE_PACKET,
                                             l2_group_config);
      sprintf(str, "WP_ShapingGroupCreate L2 #%d",l2_index + *l2_offset);
      terminate_on_error(test_system.l2_groups[l2_index + *l2_offset], str,__LINE__);
   }

   /* Create Rx channel */
   test_system.rx_channels[device_index] =\
                           WP_ChannelCreate(0,config->device_handle,
                           qnode, WP_CH_RX, WP_ENET ,&ch_config_enet[0]);
   sprintf(str, "WP_ChannelCreate ENET RX #%d",device_index);
   terminate_on_error(test_system.rx_channels[device_index], str,__LINE__);

   /* Create Tx channels (L3) */
   for(l3_index=0;l3_index < config->l3.num_groups;l3_index++)
   {
      switch(config->l3.connect_mode & WT_CONNECT_MASK)
      {
         case WT_CONNECT_INDEXED:
            l2_index = config->l3.group[l3_index].parent_offset;
            if(l2_index != last_l2) /* new l2 */
            {
               last_l2 = l2_index;
               l3_count = 0;
            }
            else                    /* same l2 */
            {
               l3_count++;
            }
            ch_config_enet[0].tx_tq = l3_count;
            break;
         case WT_CONNECT_GROUPED:
            l2_index = l3_index / (config->l3.connect_mode & ~WT_CONNECT_MASK);
            ch_config_enet[0].tx_tq = l3_index % (config->l3.connect_mode & ~WT_CONNECT_MASK);
            break;
         case WT_CONNECT_SINGLE:
            l2_index = config->l3.connect_mode & ~WT_CONNECT_MASK;
            ch_config_enet[0].tx_tq = l3_index%8;
            break;
         default:
            printf("WT ERROR in %s:%d\n",__FILE__,__LINE__);
            WTE_Terminate();
            break;
      }

      if(!config->l3.identical)
      {
         ch_config_enet->tx_shaping_type = config->l3.group[l3_index].shaping_type;
         ch_config_enet->tx_shaping_params = config->l3.group[l3_index].shaping_params;
      }
      else
      {
         ch_config_enet->tx_shaping_type = config->l3.group[0].shaping_type;
         ch_config_enet->tx_shaping_params = config->l3.group[0].shaping_params;
      }

      test_system.tx_channels[l3_index + *l3_offset].handle = \
          WP_ChannelCreate(0,test_system.l2_groups[l2_index + *l2_offset],
                           qnode,WP_CH_TX, WP_ENET ,&ch_config_enet[0]);
      sprintf(str, "WP_ChannelCreate ENET TX #%d",l3_index + *l3_offset);
      terminate_on_error(test_system.tx_channels[l3_index + *l3_offset].handle, str,__LINE__);

      test_system.tx_channels[l3_index].rx_channel = test_system.rx_channels[device_index];
      test_system.tx_channels[l3_index].tx_shaping_type = ch_config_enet->tx_shaping_type;
   }

   *l1_offset += config->l1.num_groups;
   *l2_offset += config->l2.num_groups;
   *l3_offset += config->l3.num_groups;
}

void WTE_DevicesEnable(void)
{
    /* Enable the Hspi Port */
   for(entry = 0; entry < MAX_PORTS; entry ++)
   {
      if(test_system.ports[entry].handle)
      {
         status = WP_PortEnable(test_system.ports[entry].handle, WP_DIRECTION_DUPLEX);
         terminate_on_error(status, "WP_PortEnable ENET",__LINE__);
      }
   }

   for(entry = 0; entry < MAX_DEVICES; entry ++)
   {
      if(test_system.devices[entry])
      {
         status = WP_DeviceEnable(test_system.devices[entry], WP_DIRECTION_DUPLEX);
         terminate_on_error(status, "WP_DeviceEnable() ENET",__LINE__);
      }
   }
}

void WTE_DevicesDisable(void)
{
   for(entry = 0; entry < MAX_DEVICES; entry ++)
   {
      if(test_system.devices[entry])
      {
         status = WP_DeviceDisable(test_system.devices[entry], WP_DIRECTION_DUPLEX);
         terminate_on_error(status, "WP_DeviceDisable() ENET",__LINE__);
      }
   }

   for(entry = 0; entry < MAX_PORTS; entry ++)
   {
      if(test_system.ports[entry].handle)
      {
         status = WP_PortDisable(test_system.ports[entry].handle, WP_DIRECTION_DUPLEX);
         terminate_on_error(status, "WP_PortDisable ENET",__LINE__);
      }
   }
}

void WTE_GroupsEnable(void)
{
   for(entry = 0; entry < MAX_L1_GROUPS; entry ++)
   {
      if(test_system.l1_groups[entry])
      {
         status = WP_ShapingGroupEnable(test_system.l1_groups[entry]);
         terminate_on_error(status, "WP_ShapingGroupEnable L1",__LINE__);
      }
   }

   for(entry = 0; entry < MAX_L2_GROUPS; entry ++)
   {
      if(test_system.l2_groups[entry])
      {
         status = WP_ShapingGroupEnable(test_system.l2_groups[entry]);
         terminate_on_error(status, "WP_ShapingGroupEnable L2",__LINE__);
      }
   }
}

void WTE_GroupsDisable(void)
{
   for(entry = 0; entry < MAX_L1_GROUPS; entry ++)
   {
      if(test_system.l1_groups[entry])
      {
         status = WP_ShapingGroupDisable(test_system.l1_groups[entry]);
         terminate_on_error(status, "WP_ShapingGroupDisable L1",__LINE__);
      }
   }

   for(entry = 0; entry < MAX_L2_GROUPS; entry ++)
   {
      if(test_system.l2_groups[entry])
      {
         status = WP_ShapingGroupDisable(test_system.l2_groups[entry]);
         terminate_on_error(status, "WP_ShapingGroupDisable L2",__LINE__);
      }
   }
}

void WTE_GroupsDelete(void)
{
   for(entry = 0; entry < MAX_L2_GROUPS; entry ++)
   {
      if(test_system.l2_groups[entry])
      {
         status = WP_ShapingGroupDelete(test_system.l2_groups[entry]);
         terminate_on_error(status, "WP_ShapingGroupDelete L2",__LINE__);

         test_system.l2_groups[entry] = 0;
      }
   }

   for(entry = 0; entry < MAX_L2_BLOCKS; entry ++)
   {
      if(test_system.l2_blocks[entry])
      {
         status = WP_ShapingBlockDelete(test_system.l2_blocks[entry]);
         terminate_on_error(status, "WP_ShapingBlockDelete L2",__LINE__);

         test_system.l2_blocks[entry] = 0;
      }
   }

   for(entry = 0; entry < MAX_L1_GROUPS; entry ++)
   {
      if(test_system.l1_groups[entry])
      {
         status = WP_ShapingGroupDelete(test_system.l1_groups[entry]);
         terminate_on_error(status, "WP_ShapingGroupDelete L1",__LINE__);

         test_system.l1_groups[entry] = 0;
      }
   }

   for(entry = 0; entry < MAX_L1_BLOCKS; entry ++)
   {
      if(test_system.l1_blocks[entry])
      {
         status = WP_ShapingBlockDelete(test_system.l1_blocks[entry]);
         terminate_on_error(status, "WP_ShapingBlockDelete L1",__LINE__);

         test_system.l1_blocks[entry] = 0;
      }
   }
}

void WTE_ChannelsEnable(void)
{
   for(entry=0; entry < MAX_TX_CHANNELS; entry++)
   {
      if(test_system.tx_channels[entry].handle)
      {
         status = WP_ChannelEnable(test_system.tx_channels[entry].handle);
         if( WP_ERROR( status) == WP_ERR_CH_ALREADY_ENABLED)
            printf( "Warning: WP_ChannelEnable WP_ERR_CH_ALREADY_ENABLED line:%d\n", __LINE__);
         else
            terminate_on_error(status, "WP_ChannelEnable() Tx",__LINE__);
      }
   }
   for(entry=0; entry < MAX_RX_CHANNELS; entry++)
   {
      if(test_system.rx_channels[entry])
      {
         status = WP_ChannelEnable(test_system.rx_channels[entry]);
         if( WP_ERROR( status) == WP_ERR_CH_ALREADY_ENABLED ||
             WP_ERROR( status) == WP_ERR_PKT_RX_IMPLICITLY_ENABLED)
            printf( "Warning: WP_ChannelEnable WP_ERR_CH_ALREADY_ENABLED line:%d\n", __LINE__);
         else
            terminate_on_error(status, "WP_ChannelEnable() Rx",__LINE__);
      }
   }
}

void WTE_ChannelsDisable(void)
{
   for(entry=0; entry < MAX_TX_CHANNELS; entry++)
   {
      if(test_system.tx_channels[entry].handle)
      {
         status = WP_ChannelDisable(test_system.tx_channels[entry].handle);
         if( WP_ERROR( status) == WP_ERR_CH_ALREADY_DISABLED)
            printf("Warning: WP_ChannelDisable WP_ERR_CH_ALREADY_DISABLED line:%d\n", __LINE__);
         else
            terminate_on_error(status, "WP_ChannelDisable() ENET Tx",__LINE__);
      }
   }
   for(entry=0; entry < MAX_RX_CHANNELS; entry++)
   {
      if(test_system.rx_channels[entry])
      {
         status = WP_ChannelDisable(test_system.rx_channels[entry]);
         if( WP_ERROR( status) == WP_ERR_CH_ALREADY_DISABLED ||
             WP_ERROR( status) == WP_ERR_PKT_RX_DISABLE_NOT_ALLOWED)
            printf("Warning: WP_ChannelDisable WP_ERR_CH_ALREADY_DISABLED line:%d\n", __LINE__);
         else
            terminate_on_error(status, "WP_ChannelDisable() ENET Rx",__LINE__);
      }
   }
}

void WTE_ChannelsDelete(void)
{
   for(entry=0; entry < MAX_TX_CHANNELS; entry++)
   {
      if(test_system.tx_channels[entry].handle)
      {
         status = WP_ChannelDelete(test_system.tx_channels[entry].handle);
         terminate_on_error(status, "WP_ChannelDelete ENET Tx",__LINE__);

         test_system.tx_channels[entry].handle = 0;
      }
   }
   for(entry=0; entry < MAX_RX_CHANNELS; entry++)
   {
      if(test_system.rx_channels[entry])
      {
         status = WP_ChannelDelete(test_system.rx_channels[entry]);
         terminate_on_error(status, "WP_ChannelDelete ENET Rx",__LINE__);

         test_system.rx_channels[entry] = 0;
      }
   }
}

void WTE_TxBindingCreate(void)
{
   /* Bind ENET tx channels */
   for(entry = 0; entry < MAX_TX_CHANNELS; entry++)
   {
      if(test_system.tx_channels[entry].handle)
      {
         status =  WP_IwTxBindingCreate(test_system.tx_channels[entry].handle,\
                                WP_IW_TX_BINDING_BYTES, &tx_binding_bytes[0]);
         terminate_on_error(status, "WP_IwTxBindingCreate() ENET Tx",__LINE__);
      }
   }
}

void WTE_AssignShapingType(WP_U32 level,
                           WP_U32 index,
                           WP_U8 *shaping_type,
                           void **shaping_params,
                           WP_boolean *is_strict)
{
   WP_U32 num_of_types = 5;


   if(level==WT_L3)
      num_of_types = 9;

   *is_strict = WP_FALSE;

   switch(index % num_of_types)
   {
      case WT_STRICT:
      {
#if WT_DEBUG
         printf("Selected STRICT shaping\n");
#endif
         *shaping_type = WP_FMU_SHAPING_TYPE_STRICT;
         *shaping_params = NULL;
         *is_strict = WP_TRUE;
         break;
      }
      case WT_WFQ:
      {
#if WT_DEBUG
         printf("Selected WFQ shaping\n");
#endif
         *shaping_type = WP_FMU_SHAPING_TYPE_WFQ;
         *shaping_params = &wfq_shaping_params[0];
         break;
      }
      case WT_WRR:
      {
         if(level!=WT_L3)
         {
#if WT_DEBUG
            printf("Selected WRR shaping\n");
#endif
            *shaping_type = WP_FMU_SHAPING_TYPE_WRR;
            *shaping_params = &wrr_shaping_params[0];
            break;
         }
         else
         {
#if WT_DEBUG
            printf("Selected WFQ shaping\n");
#endif
            *shaping_type = WP_FMU_SHAPING_TYPE_WFQ;
            *shaping_params = &wfq_shaping_params[0];
            break;
         }
      }
      case WT_CIR_EIR:
      {
#if WT_DEBUG
         printf("Selected CIR EIR RR shaping\n");
#endif
         *shaping_type = WP_FMU_SHAPING_TYPE_CIR_EIR;
         *shaping_params = &cir_eir_shaping_params[0];
         cir_eir_shaping_params[0].flags = WP_FMU_SHAPING_CIR_EIR_SELECT_MODE_RR;
         break;
      }
      case WT_CIR_EIR_STRICT:
      {
#if WT_DEBUG
         printf("Selected CIR EIR STRICT shaping\n");
#endif
         *shaping_type = WP_FMU_SHAPING_TYPE_CIR_EIR;
         *shaping_params = &cir_eir_shaping_params[0];
         cir_eir_shaping_params[0].flags = WP_FMU_SHAPING_CIR_EIR_SELECT_MODE_STRICT;
         *is_strict = WP_TRUE;
         break;
      }
      case WT_1STRICT_7CIR_EIR:
      {
#if WT_DEBUG
         printf("Selected STRICT AND SHAPED shaping (fix min max)\n");
#endif
         *shaping_type = WP_FMU_SHAPING_TYPE_STRICT_AND_SHAPED;
         *shaping_params = &strict_and_shaped_shaping_params[0];
         strict_and_shaped_shaping_params[0].shaping_type = WP_FMU_SHAPING_TYPE_CIR_EIR;
         strict_and_shaped_shaping_params[0].shaping_param = &cir_eir_shaping_params[0];
         strict_and_shaped_shaping_params[0].flags = WP_FMU_SHAPING_STRICT_LEVELS_1;
         break;
      }
      case WT_2STRICT_6CIR_EIR:
      {
#if WT_DEBUG
         printf("Selected STRICT AND SHAPED shaping (fix fix min max)\n");
#endif
         *shaping_type = WP_FMU_SHAPING_TYPE_STRICT_AND_SHAPED;
         *shaping_params = &strict_and_shaped_shaping_params[0];
         strict_and_shaped_shaping_params[0].shaping_type = WP_FMU_SHAPING_TYPE_CIR_EIR;
         strict_and_shaped_shaping_params[0].shaping_param = &cir_eir_shaping_params[0];
         strict_and_shaped_shaping_params[0].flags = WP_FMU_SHAPING_STRICT_LEVELS_2;
         break;
      }
      case WT_1STRICT_7WRR:
      {
#if WT_DEBUG
         printf("Selected STRICT AND SHAPED shaping (fix WFQ)\n");
#endif
         *shaping_type = WP_FMU_SHAPING_TYPE_STRICT_AND_SHAPED;
         *shaping_params = &strict_and_shaped_shaping_params[0];
         strict_and_shaped_shaping_params[0].shaping_type = WP_FMU_SHAPING_TYPE_WFQ;
         strict_and_shaped_shaping_params[0].shaping_param = &wfq_shaping_params[0];
         strict_and_shaped_shaping_params[0].flags = WP_FMU_SHAPING_STRICT_LEVELS_1;
         break;
      }
      case WT_2STRICT_6WRR:
      {
#if WT_DEBUG
         printf("Selected STRICT AND SHAPED shaping (fix fix WFQ)\n");
#endif
         *shaping_type = WP_FMU_SHAPING_TYPE_STRICT_AND_SHAPED;
         *shaping_params = &strict_and_shaped_shaping_params[0];
         strict_and_shaped_shaping_params[0].shaping_type = WP_FMU_SHAPING_TYPE_WFQ;
         strict_and_shaped_shaping_params[0].shaping_param = &wfq_shaping_params[0];
         strict_and_shaped_shaping_params[0].flags = WP_FMU_SHAPING_STRICT_LEVELS_2;
         break;
      }
      default:
         printf("Assert %s:%d\n",__FILE__,__LINE__); while(1) {};
   }
}

void WTE_Pause(void)
{
   WP_U32 t1=0, t2=0;

   while (1)
   {
      t1 = WP_TimeRead();

      if (F_SampleSerial())
      {
         break;
      }
      while (WP_TimeDelta(t2,t1) < WTE_TTY_LOAD_TIME)
      {
         t2 = WP_TimeRead();
      }
   }
}

void WTE_DisplayEnetDevicesStats(void)
{
   for(entry = 0; entry < MAX_DEVICES; entry ++)
   {
      if(test_system.devices[entry])
         WTE_DisplayDeviceStats(test_system.devices[entry],WP_DEVICE_ENET,"ENET");
   }
}

void WTE_DisplayXgiDevicesStats(void)
{
   for(entry = 0; entry < MAX_DEVICES; entry ++)
   {
      if(test_system.devices[entry])
         WTE_DisplayDeviceStats(test_system.devices[entry],WP_DEVICE_ENET,"XGI");
   }
}

void WTE_CheckEnetDeviceStats(WP_handle device_handle, WP_U32 num_channels)
{
   WP_stats_enet stats_enet = {0};
   WP_status status = WP_OK;
   WP_U32 expected_bytes_tx, expected_bytes_rx, expected_du;
   WP_U32 stats_value;

   expected_du = num_channels * FRAMES_PER_CHANNEL;
   expected_bytes_tx = expected_bytes_rx = expected_du * DATA_LENGTH;
   expected_bytes_rx += expected_du * 4; /*add 4 bytes of CRC*/

   memset( &stats_enet, 0, sizeof(WP_stats_enet) );
   if((status = WP_DeviceStatistics(device_handle, &stats_enet)))
   {
#if WT_DEBUG
      printf("GetStatistics error\n");
#endif
      goto L_StatsError;
   }

   stats_value = stats_enet.serial_dps_if_stats.rx_frames & 0xffffffff;
   if(stats_value != expected_du)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! rx_packets = 0x%x should be 0x%x\n",
             stats_value, expected_du);
#endif
      goto L_StatsError;
   }

   stats_value = stats_enet.serial_dps_if_stats.tx_frames & 0xffffffff;
   if(stats_value != expected_du)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! tx_packets = 0x%x should be 0x%x\n",
             stats_value, expected_du);
#endif
      goto L_StatsError;
   }

   stats_value = stats_enet.rx_bytes & 0xffffffff;
   if(stats_value != expected_bytes_rx)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! rx_bytes = 0x%x should be 0x%x\n",
             stats_value, expected_bytes_rx);
#endif
      goto L_StatsError;
   }

   stats_value = stats_enet.tx_bytes & 0xffffffff;
   if(stats_value != expected_bytes_tx)
   {
      printf("expected_bytes_tx= 0x%x\n", expected_bytes_tx);
#if WT_DEBUG
      printf("Statistics ERROR!!! tx_bytes = 0x%x should be 0x%x\n",
             stats_value, expected_bytes_tx);
#endif
      goto L_StatsError;
   }

   if(stats_enet.rx_err_fcs != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! rx_err_fcs = 0x%x\n",
             ((WP_U32)(stats_enet.rx_err_fcs & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.rx_err_alignment != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! rx_err_alignment = 0x%x\n",
             ((WP_U32)(stats_enet.rx_err_alignment & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.rx_err_length != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! rx_err_length = 0x%x\n",
             ((WP_U32)(stats_enet.rx_err_length & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.rx_err_code != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! rx_err_code = 0x%x\n",
             ((WP_U32)(stats_enet.rx_err_code & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.rx_false_carrier != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! rx_false_carrier = 0x%x\n",
             ((WP_U32)(stats_enet.rx_false_carrier & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.rx_undersize != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! rx_undersize = 0x%x\n",
             ((WP_U32)(stats_enet.rx_undersize & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.rx_fragments != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! rx_fragments = 0x%x\n",
             ((WP_U32)(stats_enet.rx_fragments & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.rx_jabber != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! rx_jabber = 0x%x\n",
             ((WP_U32)(stats_enet.rx_jabber & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.rx_dropped != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! rx_dropped = 0x%x\n",
             ((WP_U32)(stats_enet.rx_dropped & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.tx_defer != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! tx_defer = 0x%x\n",
             ((WP_U32)(stats_enet.tx_defer & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.tx_excess_defer != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! tx_excess_defer = 0x%x\n",
             ((WP_U32)(stats_enet.tx_excess_defer & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.tx_single_collision != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! tx_single_collision = 0x%x\n",
             ((WP_U32)(stats_enet.tx_single_collision & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.tx_multi_collision != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! tx_multi_collision = 0x%x\n",
             ((WP_U32)(stats_enet.tx_multi_collision & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.tx_late_collision != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! tx_late_collision = 0x%x\n",
             ((WP_U32)(stats_enet.tx_late_collision & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.tx_excess_collision != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! tx_excess_collision = 0x%x\n",
             ((WP_U32)(stats_enet.tx_excess_collision & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.tx_no_collision != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! tx_no_collision = 0x%x\n",
             ((WP_U32)(stats_enet.tx_no_collision & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.tx_mac_pause_honored != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! tx_mac_pause_honored = 0x%x\n",
             ((WP_U32)(stats_enet.tx_mac_pause_honored & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.tx_dropped != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! tx_dropped = 0x%x\n",
             ((WP_U32)(stats_enet.tx_dropped & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.tx_jabber != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! tx_jabber = 0x%x\n",
             ((WP_U32)(stats_enet.tx_jabber & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.tx_err_fcs != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! tx_err_fcs = 0x%x\n",
             ((WP_U32)(stats_enet.tx_err_fcs & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.tx_control != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! tx_control = 0x%x\n",
             ((WP_U32)(stats_enet.tx_control & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.tx_undersize != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! tx_undersize = 0x%x\n",
             ((WP_U32)(stats_enet.tx_undersize & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   if(stats_enet.tx_fragments != 0)
   {
#if WT_DEBUG
      printf("Statistics ERROR!!! tx_fragments = 0x%x\n",
             ((WP_U32)(stats_enet.tx_fragments & 0xffffffff)));
#endif
      goto L_StatsError;
   }

   return;

  L_StatsError:
      printf("Test Failed\n");
      WTE_Terminate();
}

int WTE_CheckHsposDeviceStats(WP_handle device_handle)
{
   WP_status status;
   WP_U32 temp1, temp2;
   WP_stats_upi_hspos hspos_stats = {0};
   int stats_failed = 0;

   memset( &hspos_stats, 0, sizeof(WP_stats_upi_hspos) );
   status = WP_DeviceStatistics(device_handle, &hspos_stats);
   terminate_on_error(status, "WP_DeviceStatistics()", __LINE__);

   if (hspos_stats.rx_host_frames != FRAMES_PER_CHANNEL)
   {
      temp1 = hspos_stats.rx_host_frames;
      temp2 = hspos_stats.rx_host_frames >> 32;
      printf("Test Abort. rx_host_frames statistics failed:\n");
      printf("Test Abort. expected: %.8x, received: %.8x%.8x\n", FRAMES_PER_CHANNEL, temp2,temp1);
      stats_failed++;
   }
   if (hspos_stats.rx_iw_frames > 0)
   {
      temp1 = hspos_stats.rx_iw_frames;
      temp2 = hspos_stats.rx_iw_frames >> 32;
      printf("Test Abort. rx_iw_frames statistics failed:\n");
      printf("Test Abort. expected: %.8x, received: %.8x%.8x\n", 0, temp2,temp1);
      stats_failed++;
   }
   if (hspos_stats.rx_err_host_full > 0)
   {
      temp1 = hspos_stats.rx_err_host_full;
      temp2 = hspos_stats.rx_err_host_full >> 32;
      printf("Test Abort. rx_err_host_full statistics failed:\n");
      printf("Test Abort. expected: %.8x, received: %.8x%.8x\n", 0, temp2,temp1);
      stats_failed++;
   }
   if (hspos_stats.rx_err_fbp_underrun > 0)
   {
      temp1 = hspos_stats.rx_err_fbp_underrun;
      temp2 = hspos_stats.rx_err_fbp_underrun >> 32;
      printf("Test Abort. rx_err_fbp_underrun statistics failed: %.8x%.8x\n", temp2,temp1);
      stats_failed++;
   }
   if (hspos_stats.rx_err_mru > 0)
   {
      temp1 = hspos_stats.rx_err_mru;
      temp2 = hspos_stats.rx_err_mru >> 32;
      printf("Test Abort. rx_err_mru statistics failed: %.8x%.8x\n", temp2,temp1);
      stats_failed++;
   }
   if (hspos_stats.rx_err_sdu > 0)
   {
      temp1 = hspos_stats.rx_err_sdu;
      temp2 = hspos_stats.rx_err_sdu >> 32;
      printf("Test Abort. rx_err_sdu statistics failed: %.8x%.8x\n", temp2,temp1);
      stats_failed++;
   }
   if (hspos_stats.rx_err_sm > 0)
   {
      temp1 = hspos_stats.rx_err_sm;
      temp2 = hspos_stats.rx_err_sm >> 32;
      printf("Test Abort. rx_err_sm statistics failed: %.8x%.8x\n", temp2,temp1);
      stats_failed++;
   }
   if (hspos_stats.rx_err_parity > 0)
   {
      temp1 = hspos_stats.rx_err_parity;
      temp2 = hspos_stats.rx_err_parity >> 32;
      printf("Test Abort. rx_err_parity statistics failed: %.8x%.8x\n", temp2,temp1);
      stats_failed++;
   }
   if (hspos_stats.rx_err_phy > 0)
   {
      temp1 = hspos_stats.rx_err_phy;
      temp2 = hspos_stats.rx_err_phy >> 32;
      printf("Test Abort. rx_err_phy statistics failed: %.8x%.8x\n", temp2,temp1);
      stats_failed++;
   }
   if (hspos_stats.tx_frames != FRAMES_PER_CHANNEL)
   {
      temp1 = hspos_stats.tx_frames;
      temp2 = hspos_stats.tx_frames >> 32;
      printf("Test Abort. tx_frames statistics failed: %.8x%.8x\n", temp2,temp1);
      stats_failed++;
   }

   return stats_failed;
}

/* Displays UPI or ENET device statistics */
WP_U32 WTE_DisplayDeviceStats(WP_handle device_handle, WP_device device, WP_CHAR* type)
{
   WP_status status;
   WP_U32 temp1, temp2;
   WP_stats_enet stats_enet = {0};
/*   WP_stats_xgi xgi_stats = {0};*/
   WP_stats_upi_hspos hspos_stats = {0};
   WP_stats_upi_hdlc stats_upi_pos = {0};
   WP_stats_upi_atm   stats_upi   = {0};

   printf ("\n%s Device statistics of device %d (%x)\n",
           type,(device_handle & 0x000000ff),device_handle);
   printf ("============================================\n");
   switch (device)
   {
      case WP_DEVICE_ENET:
         memset( &stats_enet, 0, sizeof(WP_stats_enet) );
         if( (status = WP_DeviceStatistics(device_handle, &stats_enet)) )
         {
            printf("GetStatistics error\n");
            return 0;
         }
         printf("Hardware statistics:\n");
         temp1 = stats_enet.rx_packets;
         temp2 = stats_enet.rx_packets >> 32;
         printf("rx_packets:           %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.tx_packets;
         temp2 = stats_enet.tx_packets >> 32;
         printf("tx_packets:           %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.txrx_frames_64;
         temp2 = stats_enet.txrx_frames_64 >> 32;
         printf("txrx_frames_64:       %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.txrx_frames_127;
         temp2 = stats_enet.txrx_frames_127 >> 32;
         printf("txrx_frames_127:      %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.txrx_frames_255;
         temp2 = stats_enet.txrx_frames_255 >> 32;
         printf("txrx_frames_255:      %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.txrx_frames_511;
         temp2 = stats_enet.txrx_frames_511 >> 32;
         printf("txrx_frames_511:      %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.txrx_frames_1023;
         temp2 = stats_enet.txrx_frames_1023 >> 32;
         printf("txrx_frames_1023:     %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.txrx_frames_1518;
         temp2 = stats_enet.txrx_frames_1518 >> 32;
         printf("txrx_frames_1518:     %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.txrx_frames_1522;
         temp2 = stats_enet.txrx_frames_1522 >> 32;
         printf("txrx_frames_1522:     %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.rx_bytes;
         temp2 = stats_enet.rx_bytes >> 32;
         printf("rx_bytes:             %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.rx_err_fcs;
         temp2 = stats_enet.rx_err_fcs >> 32;
         printf("rx_err_fcs:           %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.rx_multicast;
         temp2 = stats_enet.rx_multicast >> 32;
         printf("rx_multicast:         %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.rx_broadcast;
         temp2 = stats_enet.rx_broadcast >> 32;
         printf("rx_broadcast:         %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.rx_mac_control;
         temp2 = stats_enet.rx_mac_control >> 32;
         printf("rx_mac_control:       %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.rx_mac_pause;
         temp2 = stats_enet.rx_mac_pause >> 32;
         printf("rx_mac_pause:         %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.rx_mac_unknown;
         temp2 = stats_enet.rx_mac_unknown >> 32;
         printf("rx_mac_unknown:       %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.rx_err_alignment;
         temp2 = stats_enet.rx_err_alignment >> 32;
         printf("rx_err_alignment:     %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.rx_err_length;
         temp2 = stats_enet.rx_err_length >> 32;
         printf("rx_err_length:        %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.rx_err_code;
         temp2 = stats_enet.rx_err_code >> 32;
         printf("rx_err_code:          %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.rx_false_carrier;
         temp2 = stats_enet.rx_false_carrier >> 32;
         printf("rx_false_carrier:     %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.rx_undersize;
         temp2 = stats_enet.rx_undersize >> 32;
         printf("rx_undersize:         %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.rx_oversize;
         temp2 = stats_enet.rx_oversize >> 32;
         printf("rx_oversize:          %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.rx_fragments;
         temp2 = stats_enet.rx_fragments >> 32;
         printf("rx_fragments:         %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.rx_jabber;
         temp2 = stats_enet.rx_jabber >> 32;
         printf("rx_jabber:            %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.rx_dropped;
         temp2 = stats_enet.rx_dropped >> 32;
         printf("rx_dropped:           %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.tx_bytes;
         temp2 = stats_enet.tx_bytes >> 32;
         printf("tx_bytes:             %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.tx_multicast;
         temp2 = stats_enet.tx_multicast >> 32;
         printf("tx_multicast:         %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.tx_broadcast;
         temp2 = stats_enet.tx_broadcast >> 32;
         printf("tx_broadcast:         %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.tx_mac_pause;
         temp2 = stats_enet.tx_mac_pause >> 32;
         printf("tx_mac_pause:         %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.tx_defer;
         temp2 = stats_enet.tx_defer >> 32;
         printf("tx_defer:             %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.tx_excess_defer;
         temp2 = stats_enet.tx_excess_defer >> 32;
         printf("tx_excess_defer:      %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.tx_single_collision;
         temp2 = stats_enet.tx_single_collision >> 32;
         printf("tx_single_collision:  %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.tx_multi_collision;
         temp2 = stats_enet.tx_multi_collision >> 32;
         printf("tx_multi_collision:   %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.tx_late_collision;
         temp2 = stats_enet.tx_late_collision >> 32;
         printf("tx_late_collision:    %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.tx_excess_collision;
         temp2 = stats_enet.tx_excess_collision >> 32;
         printf("tx_excess_collision:  %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.tx_no_collision;
         temp2 = stats_enet.tx_no_collision >> 32;
         printf("tx_no_collision:      %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.tx_mac_pause_honored;
         temp2 = stats_enet.tx_mac_pause_honored >> 32;
         printf("tx_mac_pause_honored: %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.tx_dropped;
         temp2 = stats_enet.tx_dropped >> 32;
         printf("tx_dropped:           %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.tx_jabber;
         temp2 = stats_enet.tx_jabber >> 32;
         printf("tx_jabber:            %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.tx_err_fcs;
         temp2 = stats_enet.tx_err_fcs >> 32;
         printf("tx_err_fcs:           %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.tx_control;
         temp2 = stats_enet.tx_control >> 32;
         printf("tx_control:           %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.tx_oversize;
         temp2 = stats_enet.tx_oversize >> 32;
         printf("tx_oversize:          %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.tx_undersize;
         temp2 = stats_enet.tx_undersize >> 32;
         printf("tx_undersize:         %.8x%.8x\n", temp2,temp1);

         temp1 = stats_enet.tx_fragments;
         temp2 = stats_enet.tx_fragments >> 32;
         printf("tx_fragments:         %.8x%.8x\n", temp2,temp1);

         printf("DPS statistics:\n");
         temp1 = stats_enet.rx_err_overrun;
         temp2 = stats_enet.rx_err_overrun >> 32;
         printf("rx_err_overrun:       %.8x%.8x\n", temp2,temp1);
         temp1 = stats_enet.rx_err_sdu;
         temp2 = stats_enet.rx_err_sdu >> 32;
         printf("rx_err_maxsdu:        %.8x%.8x\n", temp2,temp1);
         temp1 = stats_enet.rx_err_nonvalid_mac;
         temp2 = stats_enet.rx_err_nonvalid_mac >> 32;
         printf("rx_err_mac_address:   %.8x%.8x\n", temp2,temp1);
         temp1 = stats_enet.rx_err_mru;
         temp2 = stats_enet.rx_err_mru >> 32;
         printf("rx_err_mru:           %.8x%.8x\n", temp2,temp1);
         temp1 = stats_enet.tx_err_underrun;
         temp2 = stats_enet.tx_err_underrun >> 32;
         printf("tx_err_underrun:      %.8x%.8x\n", temp2,temp1);
         temp1 = stats_enet.rx_err_host_full;
         temp2 = stats_enet.rx_err_host_full >> 32;
         printf("rx_err_host_full:     %.8x%.8x\n", temp2,temp1);
         temp1 = stats_enet.rx_err_fbp_underrun;
         temp2 = stats_enet.rx_err_fbp_underrun >> 32;
         printf("rx_err_fbp_underrun:  %.8x%.8x\n", temp2,temp1);
         temp1 = stats_enet.rx_iw_frames;
         temp2 = stats_enet.rx_iw_frames >> 32;
         printf("rx_iw_frames:         %.8x%.8x\n", temp2,temp1);
         temp1 = stats_enet.tx_frames;
         temp2 = stats_enet.tx_frames >> 32;
         printf("tx_good_frames:       %.8x%.8x\n", temp2,temp1);
         temp1 = stats_enet.rx_host_frames;
         temp2 = stats_enet.rx_host_frames >> 32;
         printf("rx_host_frames:       %.8x%.8x\n", temp2,temp1);
         printf("\n");

         printf("Serial DPS IF statistics:\n");
         temp1 = stats_enet.serial_dps_if_stats.tx_frames;
         temp2 = stats_enet.serial_dps_if_stats.tx_frames >> 32;
         printf("Serial DPS IF tx frames             %.8x%.8x\n", temp2,temp1);
         temp1 = stats_enet.serial_dps_if_stats.tx_bytes;
         temp2 = stats_enet.serial_dps_if_stats.tx_bytes >> 32;
         printf("Serial DPS IF tx bytes              %.8x%.8x\n", temp2,temp1);
         temp1 = stats_enet.serial_dps_if_stats.rx_frames;
         temp2 = stats_enet.serial_dps_if_stats.rx_frames >> 32;
         printf("Serial DPS IF rx frames             %.8x%.8x\n", temp2,temp1);
         temp1 = stats_enet.serial_dps_if_stats.rx_bytes;
         temp2 = stats_enet.serial_dps_if_stats.rx_bytes >> 32;
         printf("Serial DPS IF rx bytes              %.8x%.8x\n", temp2,temp1);
         temp1 = stats_enet.serial_dps_if_stats.rx_pce_denied_frames;
         temp2 = stats_enet.serial_dps_if_stats.rx_pce_denied_frames >> 32;
         printf("Serial DPS IF rx pce denied frames  %.8x%.8x\n", temp2,temp1);
         temp1 = stats_enet.serial_dps_if_stats.rx_dropped_frames;
         temp2 = stats_enet.serial_dps_if_stats.rx_dropped_frames >> 32;
         printf("Serial DPS IF rx dropped frames     %.8x%.8x\n", temp2,temp1);
         printf("\n");

         break;
#if 0
       case WP_DEVICE_XGI:
          memset( &xgi_stats, 0, sizeof(WP_stats_xgi) );
          if((status = WP_DeviceStatistics(device_handle, &xgi_stats)))
          {
             printf("GetStatistics error\n");
             return;
          }
          printf("Hardware statistics:\n");
          temp1 = xgi_stats.rx_packets;
          temp2 = xgi_stats.rx_packets >> 32;
          printf("rx_packets:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.tx_packets;
          temp2 = xgi_stats.tx_packets >> 32;
          printf("tx_packets:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.rx_frames_64;
          temp2 = xgi_stats.rx_frames_64 >> 32;
          printf("rx_frames_64:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.tx_frames_64;
          temp2 = xgi_stats.tx_frames_64 >> 32;
          printf("tx_frames_64:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.rx_frames_65_127;
          temp2 = xgi_stats.rx_frames_65_127 >> 32;
          printf("rx_frames_65_127:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.tx_frames_65_127;
          temp2 = xgi_stats.tx_frames_65_127 >> 32;
          printf("tx_frames_65_127:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.rx_frames_128_255;
          temp2 = xgi_stats.rx_frames_128_255 >> 32;
          printf("rx_frames_128_255:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.tx_frames_128_255;
          temp2 = xgi_stats.tx_frames_128_255 >> 32;
          printf("tx_frames_128_255:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.rx_frames_256_511;
          temp2 = xgi_stats.rx_frames_256_511 >> 32;
          printf("rx_frames_256_511:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.tx_frames_256_511;
          temp2 = xgi_stats.tx_frames_256_511 >> 32;
          printf("tx_frames_256_511:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.rx_frames_512_1023;
          temp2 = xgi_stats.rx_frames_512_1023 >> 32;
          printf("rx_frames_512_1023:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.tx_frames_512_1023;
          temp2 = xgi_stats.tx_frames_512_1023 >> 32;
          printf("tx_frames_512_1023:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.rx_frames_1024_1518;
          temp2 = xgi_stats.rx_frames_1024_1518 >> 32;
          printf("rx_frames_1024_1518:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.tx_frames_1024_1518;
          temp2 = xgi_stats.tx_frames_1024_1518 >> 32;
          printf("tx_frames_1024_1518:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.rx_frames_1519_max;
          temp2 = xgi_stats.rx_frames_1519_max >> 32;
          printf("rx_frames_1519_max:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.tx_frames_1519_max;
          temp2 = xgi_stats.tx_frames_1519_max >> 32;
          printf("tx_frames_1519_max:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.rx_bytes;
          temp2 = xgi_stats.rx_bytes >> 32;
          printf("rx_bytes:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.rx_err_fcs;
          temp2 = xgi_stats.rx_err_fcs >> 32;
          printf("rx_err_fcs:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.rx_multicast;
          temp2 = xgi_stats.rx_multicast >> 32;
          printf("rx_multicast:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.rx_broadcast;
          temp2 = xgi_stats.rx_broadcast >> 32;
          printf("rx_broadcast:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.rx_mac_pause;
          temp2 = xgi_stats.rx_mac_pause >> 32;
          printf("rx_mac_pause:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.rx_err_length;
          temp2 = xgi_stats.rx_err_length >> 32;
          printf("rx_err_length:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.rx_err_code;
          temp2 = xgi_stats.rx_err_code >> 32;
          printf("rx_err_code:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.rx_undersize;
          temp2 = xgi_stats.rx_undersize >> 32;
          printf("rx_undersize:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.rx_oversize;
          temp2 = xgi_stats.rx_oversize >> 32;
          printf("rx_oversize:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.rx_jabber;
          temp2 = xgi_stats.rx_jabber >> 32;
          printf("rx_jabber:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.tx_bytes;
          temp2 = xgi_stats.tx_bytes >> 32;
          printf("tx_bytes:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.tx_multicast;
          temp2 = xgi_stats.tx_multicast >> 32;
          printf("tx_multicast:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.tx_broadcast;
          temp2 = xgi_stats.tx_broadcast >> 32;
          printf("tx_broadcast:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.tx_mac_pause;
          temp2 = xgi_stats.tx_mac_pause >> 32;
          printf("tx_mac_pause:           %.8x%.8x\n", temp2,temp1);

          temp1 = xgi_stats.tx_err_packets;
          temp2 = xgi_stats.tx_err_packets >> 32;
          printf("tx_err_packets:           %.8x%.8x\n", temp2,temp1);

          printf("DPS statistics:\n");
          temp1 = xgi_stats.rx_err_overrun;
          temp2 = xgi_stats.rx_err_overrun >> 32;
          printf("rx_err_overrun:       %.8x%.8x\n", temp2,temp1);
          temp1 = xgi_stats.rx_err_sdu;
          temp2 = xgi_stats.rx_err_sdu >> 32;
          printf("rx_err_maxsdu:        %.8x%.8x\n", temp2,temp1);
          temp1 = xgi_stats.rx_err_nonvalid_mac;
          temp2 = xgi_stats.rx_err_nonvalid_mac >> 32;
          printf("rx_err_mac_address:   %.8x%.8x\n", temp2,temp1);
          temp1 = xgi_stats.rx_err_mru;
          temp2 = xgi_stats.rx_err_mru >> 32;
          printf("rx_err_mru:           %.8x%.8x\n", temp2,temp1);
          temp1 = xgi_stats.tx_err_underrun;
          temp2 = xgi_stats.tx_err_underrun >> 32;
          printf("tx_err_underrun:      %.8x%.8x\n", temp2,temp1);
          temp1 = xgi_stats.rx_err_host_full;
          temp2 = xgi_stats.rx_err_host_full >> 32;
          printf("rx_err_host_full:     %.8x%.8x\n", temp2,temp1);
          temp1 = xgi_stats.rx_err_fbp_underrun;
          temp2 = xgi_stats.rx_err_fbp_underrun >> 32;
          printf("rx_err_fbp_underrun:  %.8x%.8x\n", temp2,temp1);
          temp1 = xgi_stats.rx_iw_frames;
          temp2 = xgi_stats.rx_iw_frames >> 32;
          printf("rx_iw_frames:         %.8x%.8x\n", temp2,temp1);
          temp1 = xgi_stats.tx_frames;
          temp2 = xgi_stats.tx_frames >> 32;
          printf("tx_good_frames:       %.8x%.8x\n", temp2,temp1);
          printf("\n");
          temp1 = xgi_stats.rx_host_frames;
          temp2 = xgi_stats.rx_host_frames >> 32;
          printf("rx_host_frames:       %.8x%.8x\n", temp2,temp1);
          printf("\n");

          printf("Serial DPS IF statistics:\n");
          temp1 = xgi_stats.serial_dps_if_stats.tx_frames;
          temp2 = xgi_stats.serial_dps_if_stats.tx_frames >> 32;
          printf("Serial DPS IF tx frames             %.8x%.8x\n", temp2,temp1);
          temp1 = xgi_stats.serial_dps_if_stats.tx_bytes;
          temp2 = xgi_stats.serial_dps_if_stats.tx_bytes >> 32;
          printf("Serial DPS IF tx bytes              %.8x%.8x\n", temp2,temp1);
          temp1 = xgi_stats.serial_dps_if_stats.rx_frames;
          temp2 = xgi_stats.serial_dps_if_stats.rx_frames >> 32;
          printf("Serial DPS IF rx frames             %.8x%.8x\n", temp2,temp1);
          temp1 = xgi_stats.serial_dps_if_stats.rx_bytes;
          temp2 = xgi_stats.serial_dps_if_stats.rx_bytes >> 32;
          printf("Serial DPS IF rx bytes              %.8x%.8x\n", temp2,temp1);
          temp1 = xgi_stats.serial_dps_if_stats.rx_pce_denied_frames;
          temp2 = xgi_stats.serial_dps_if_stats.rx_pce_denied_frames >> 32;
          printf("Serial DPS IF rx pce denied frames  %.8x%.8x\n", temp2,temp1);
          temp1 = xgi_stats.serial_dps_if_stats.rx_dropped_frames;
          temp2 = xgi_stats.serial_dps_if_stats.rx_dropped_frames >> 32;
          printf("Serial DPS IF rx dropped frames %.8x%.8x\n", temp2,temp1);
          printf("\n");
#endif
      case WP_DEVICE_HSPOS:
         memset( &hspos_stats, 0, sizeof(WP_stats_upi_hspos) );
         if( (status = WP_DeviceStatistics(device_handle, &hspos_stats)) )
         {
            printf("GetStatistics error\n");
            return 0;
         }
         printf("\nStatistics for HSPOS device:\n");
         temp1 = hspos_stats.rx_host_frames;
         temp2 = hspos_stats.rx_host_frames >> 32;
         printf("rx_host_frames:           %.8x%.8x\n", temp2,temp1);
         temp1 = hspos_stats.rx_iw_frames;
         temp2 = hspos_stats.rx_iw_frames >> 32;
         printf("rx_iw_frames:           %.8x%.8x\n", temp2,temp1);
         temp1 = hspos_stats.rx_err_host_full;
         temp2 = hspos_stats.rx_err_host_full >> 32;
         printf("rx_err_host_full:           %.8x%.8x\n", temp2,temp1);
         temp1 = hspos_stats.rx_err_fbp_underrun;
         temp2 = hspos_stats.rx_err_fbp_underrun >> 32;
         printf("rx_err_fbp_underrun:  %.8x%.8x\n", temp2,temp1);
         temp1 = hspos_stats.rx_err_mru;
         temp2 = hspos_stats.rx_err_mru >> 32;
         printf("rx_err_mru:  %.8x%.8x\n", temp2,temp1);
         temp1 = hspos_stats.rx_err_sdu;
         temp2 = hspos_stats.rx_err_sdu >> 32;
         printf("rx_err_sdu:  %.8x%.8x\n", temp2,temp1);
         temp1 = hspos_stats.rx_err_sm;
         temp2 = hspos_stats.rx_err_sm >> 32;
         printf("rx_err_sm:  %.8x%.8x\n", temp2,temp1);
         temp1 = hspos_stats.rx_err_parity;
         temp2 = hspos_stats.rx_err_parity >> 32;
         printf("rx_err_parity:  %.8x%.8x\n", temp2,temp1);
         temp1 = hspos_stats.rx_err_phy;
         temp2 = hspos_stats.rx_err_phy >> 32;
         printf("rx_err_phy:  %.8x%.8x\n", temp2,temp1);
         temp1 = hspos_stats.tx_frames;
         temp2 = hspos_stats.tx_frames >> 32;
         printf("tx_frames:  %.8x%.8x\n", temp2,temp1);
         printf("\n");

         printf("Serial DPS IF statistics:\n");
         temp1 = hspos_stats.serial_dps_if_stats.tx_frames;
         temp2 = hspos_stats.serial_dps_if_stats.tx_frames >> 32;
         printf("Serial DPS IF tx frames             %.8x%.8x\n", temp2,temp1);
         temp1 = hspos_stats.serial_dps_if_stats.tx_bytes;
         temp2 = hspos_stats.serial_dps_if_stats.tx_bytes >> 32;
         printf("Serial DPS IF tx bytes              %.8x%.8x\n", temp2,temp1);
         temp1 = hspos_stats.serial_dps_if_stats.rx_frames;
         temp2 = hspos_stats.serial_dps_if_stats.rx_frames >> 32;
         printf("Serial DPS IF rx frames             %.8x%.8x\n", temp2,temp1);
         temp1 = hspos_stats.serial_dps_if_stats.rx_bytes;
         temp2 = hspos_stats.serial_dps_if_stats.rx_bytes >> 32;
         printf("Serial DPS IF rx bytes              %.8x%.8x\n", temp2,temp1);
         temp1 = hspos_stats.serial_dps_if_stats.rx_pce_denied_frames;
         temp2 = hspos_stats.serial_dps_if_stats.rx_pce_denied_frames >> 32;
         printf("Serial DPS IF rx pce denied frames  %.8x%.8x\n", temp2,temp1);
         temp1 = hspos_stats.serial_dps_if_stats.rx_dropped_frames;
         temp2 = hspos_stats.serial_dps_if_stats.rx_dropped_frames >> 32;
         printf("Serial DPS IF rx dropped frames     %.8x%.8x\n", temp2,temp1);
         printf("\n");

         break;

      case WP_DEVICE_HDLC:
         memset( &stats_upi_pos, 0, sizeof(WP_stats_upi_hdlc) );
         if( (status = WP_DeviceStatistics(device_handle, &stats_upi_pos)) )
         {
            printf("GetStatistics error\n");
            return 0;
         }
         printf("\nStatistics for HDLC device:\n");

         temp1 = stats_upi_pos.rx_frames;
         temp2 = stats_upi_pos.rx_frames >> 32;
         printf("rx_frames:                   %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi_pos.tx_frames;
         temp2 = stats_upi_pos.tx_frames >> 32;
         printf("tx_frames:                   %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi_pos.rx_err_crc;
         temp2 = stats_upi_pos.rx_err_crc >> 32;
         printf("rx_err_crc:                  %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi_pos.rx_err_phy;
         temp2 = stats_upi_pos.rx_err_phy >> 32;
         printf("rx_err_phy:                  %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi_pos.rx_err_addr_mismatch;
         temp2 = stats_upi_pos.rx_err_addr_mismatch >> 32;
         printf("rx_err_addr_mismatch:        %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi_pos.rx_err_buffer_overrun;
         temp2 = stats_upi_pos.rx_err_buffer_overrun >> 32;
         printf("rx_err_buffer_overrun:       %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi_pos.rx_err_overrun;
         temp2 = stats_upi_pos.rx_err_overrun >> 32;
         printf("rx_err_overrun:              %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi_pos.rx_err_sop_eop;
         temp2 = stats_upi_pos.rx_err_sop_eop >> 32;
         printf("rx_err_sop_eop:              %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi_pos.rx_err_parity;
         temp2 = stats_upi_pos.rx_err_parity >> 32;
         printf("rx_err_parity:               %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi_pos.rx_err_iw_buffer_underrun;
         temp2 = stats_upi_pos.rx_err_iw_buffer_underrun >> 32;
         printf("rx_err_iw_buffer_underrun:   %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi_pos.rx_err_iw_mru;
         temp2 = stats_upi_pos.rx_err_iw_mru >> 32;
         printf("rx_err_iw_mru:               %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi_pos.rx_err_maxsdu;
         temp2 = stats_upi_pos.rx_err_maxsdu >> 32;
         printf("rx_err_maxsdu:               %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi_pos.rx_iw_frames;
         temp2 = stats_upi_pos.rx_iw_frames >> 32;
         printf("rx_iw_frames:                %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi_pos.rx_bytes;
         temp2 = stats_upi_pos.rx_bytes >> 32;
         printf("rx_bytes:                    %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi_pos.rx_err_bytes;
         temp2 = stats_upi_pos.rx_err_bytes >> 32;
         printf("rx_err_bytes:                %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi_pos.tx_bytes;
         temp2 = stats_upi_pos.tx_bytes >> 32;
         printf("tx_bytes:                    %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi_pos.rx_err_abort;
         temp2 = stats_upi_pos.rx_err_abort >> 32;
         printf("rx_err_abort:                %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi_pos.rx_err_iw_l2_parse;
         temp2 = stats_upi_pos.rx_err_iw_l2_parse >> 32;
         printf("rx_err_iw_l2_parse:          %.8x%.8x\n", temp2,temp1);
         printf("\n");

         printf("Serial DPS IF statistics:\n");
         temp1 = stats_upi_pos.serial_dps_if_stats.tx_frames;
         temp2 = stats_upi_pos.serial_dps_if_stats.tx_frames >> 32;
         printf("Serial DPS IF tx frames             %.8x%.8x\n", temp2,temp1);
         temp1 = stats_upi_pos.serial_dps_if_stats.tx_bytes;
         temp2 = stats_upi_pos.serial_dps_if_stats.tx_bytes >> 32;
         printf("Serial DPS IF tx bytes              %.8x%.8x\n", temp2,temp1);
         temp1 = stats_upi_pos.serial_dps_if_stats.rx_frames;
         temp2 = stats_upi_pos.serial_dps_if_stats.rx_frames >> 32;
         printf("Serial DPS IF rx frames             %.8x%.8x\n", temp2,temp1);
         temp1 = stats_upi_pos.serial_dps_if_stats.rx_bytes;
         temp2 = stats_upi_pos.serial_dps_if_stats.rx_bytes >> 32;
         printf("Serial DPS IF rx bytes              %.8x%.8x\n", temp2,temp1);
         temp1 = stats_upi_pos.serial_dps_if_stats.rx_pce_denied_frames;
         temp2 = stats_upi_pos.serial_dps_if_stats.rx_pce_denied_frames >> 32;
         printf("Serial DPS IF rx pce denied frames  %.8x%.8x\n", temp2,temp1);
         temp1 = stats_upi_pos.serial_dps_if_stats.rx_dropped_frames;
         temp2 = stats_upi_pos.serial_dps_if_stats.rx_dropped_frames >> 32;
         printf("Serial DPS IF rx dropped frames     %.8x%.8x\n", temp2,temp1);
         printf("\n");

         break;

      case WP_DEVICE_ATM:
         memset( &stats_upi, 0, sizeof(WP_stats_upi_atm) );
         if( (status = WP_DeviceStatistics(device_handle, &stats_upi)) )
         {
            printf("GetStatistics error\n");
            return 0;
         }
         printf("\nStatistics for ATM device:\n");

         temp1 = stats_upi.rx_err_slc;
         temp2 = stats_upi.rx_err_slc >> 32;
         printf("rx_err_slc:           %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.rx_err_parity;
         temp2 = stats_upi.rx_err_parity >> 32;
         printf("rx_err_parity:        %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.rx_err_addr_mismatch;
         temp2 = stats_upi.rx_err_addr_mismatch >> 32;
         printf("rx_err_addr_mismatch: %.8x%.8x\n", temp2,temp1);

         printf("EXT STATS\n");
         temp1 = stats_upi.ext_stats.rx_cells;
         temp2 = stats_upi.ext_stats.rx_cells >> 32;
         printf("rx_cells:             %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.ext_stats.rx_last_unknown_addr;
         temp2 = stats_upi.ext_stats.rx_last_unknown_addr >> 32;
         printf("rx_last_unknown_addr: %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.ext_stats.rx_clp1_cells;
         temp2 = stats_upi.ext_stats.rx_clp1_cells >> 32;
         printf("rx_clp1_cells:        %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.ext_stats.rx_gfc_cells;
         temp2 = stats_upi.ext_stats.rx_gfc_cells >> 32;
         printf("rx_gfc_cells:         %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.ext_stats.tx_cells;
         temp2 = stats_upi.ext_stats.tx_cells >> 32;
         printf("tx_cells:             %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.ext_stats.tx_clp1_cells;
         temp2 = stats_upi.ext_stats.tx_clp1_cells >> 32;
         printf("tx_clp1_cells:        %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.ext_stats.rx_oam_cells;
         temp2 = stats_upi.ext_stats.rx_oam_cells >> 32;
         printf("rx_oam_cells:         %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.ext_stats.rx_rm_cells;
         temp2 = stats_upi.ext_stats.rx_rm_cells >> 32;
         printf("rx_rm_cells:          %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.ext_stats.rx_efci_cells;
         temp2 = stats_upi.ext_stats.rx_efci_cells >> 32;
         printf("rx_efci_cells:        %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.ext_stats.rx_crc_err_cells;
         temp2 = stats_upi.ext_stats.rx_crc_err_cells >> 32;
         printf("rx_crc_err_cells:     %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.ext_stats.rx_unsupported_cells;
         temp2 = stats_upi.ext_stats.rx_unsupported_cells >> 32;
         printf("rx_unsupported_cells: %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.ext_stats.tx_oam_cells;
         temp2 = stats_upi.ext_stats.tx_oam_cells >> 32;
         printf("tx_oam_cells:         %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.ext_stats.tx_rm_cells;
         temp2 = stats_upi.ext_stats.tx_rm_cells >> 32;
         printf("tx_rm_cells:          %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.ext_stats.tx_clp0_cells;
         temp2 = stats_upi.ext_stats.tx_clp0_cells >> 32;
         printf("tx_clp0_cells:        %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.ext_stats.tx_efci_cells;
         temp2 = stats_upi.ext_stats.tx_efci_cells >> 32;
         printf("tx_efci_cells:        %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.ext_stats.tx_idle_cells;
         temp2 = stats_upi.ext_stats.tx_idle_cells >> 32;
         printf("tx_idle_cells:        %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.ext_stats.rx_clp0_cells;
         temp2 = stats_upi.ext_stats.rx_clp0_cells >> 32;
         printf("rx_clp0_cells:        %.8x%.8x\n", temp2,temp1);

         printf("EMPHY STATS\n");
         temp1 = stats_upi.emphy_stats.rx_err_emphy_overrun;
         temp2 = stats_upi.emphy_stats.rx_err_emphy_overrun>> 32;
         printf("rx_err_emphy_overrun: %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.emphy_stats.rx_err_hec;
         temp2 = stats_upi.emphy_stats.rx_err_hec >> 32;
         printf("rx_err_hec:           %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.emphy_stats.rx_out_of_sync;
         temp2 = stats_upi.emphy_stats.rx_out_of_sync >> 32;
         printf("rx_out_of_sync:       %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.emphy_stats.rx_enter_sync;
         temp2 = stats_upi.emphy_stats.rx_enter_sync >> 32;
         printf("rx_enter_sync:       %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.emphy_stats.rx_err_emphy;
         temp2 = stats_upi.emphy_stats.rx_err_emphy >> 32;
         printf("rx_err_emphy:       %.8x%.8x\n", temp2,temp1);

         temp1 = stats_upi.emphy_stats.rx_sync_state;
         temp2 = 0;
         printf("rx_sync_state:       %.8x%.8x\n", temp2,temp1);

         printf("\n");

         break;

      default:
         printf("Bad device type!!\n");
         break;
   }
   return 0;
}

/* Displays Bridge Port Statistics.*/
WP_U32 WTE_DisplayIwPortStats( WP_handle port_handle, WP_CHAR* type)
{
   WP_bport_stats bport_stats;
   WP_U32 i, rx_packets = 0;
   WP_status status;

   memset( &bport_stats, 0, sizeof(WP_bport_stats) );
   status = WP_IwBridgingStatistics( port_handle, &bport_stats );
   terminate_on_error(status, "WP_DeviceStatistics()", __LINE__);

   printf ("\n%s Bridge port statistics of port %d (%x)\n",
           type,(port_handle & 0x000000ff),port_handle);
   printf ("============================================\n");
   printf ("Rx Valid Packets:               %08x%08x\n",
           ((WPT_StatField)bport_stats.rx_valid_packets).part.high,
           ((WPT_StatField)bport_stats.rx_valid_packets).part.low);
   rx_packets = ((WPT_StatField)bport_stats.rx_valid_packets).part.low;

   printf ("Discard VLAN acceptable filter: %08x%08x\n",
           ((WPT_StatField)bport_stats.discard_vlan_acceptable_filter).part.high,
           ((WPT_StatField)bport_stats.discard_vlan_acceptable_filter).part.low);
   printf ("Discard ingress filter:         %08x%08x\n",
           ((WPT_StatField)bport_stats.discard_ingress_filter).part.high,
           ((WPT_StatField)bport_stats.discard_ingress_filter).part.low);
   printf ("Discard bridge classifier:      %08x%08x\n",
           ((WPT_StatField)bport_stats.discard_bridge_classifier).part.high,
           ((WPT_StatField)bport_stats.discard_bridge_classifier).part.low);
   printf ("Discard unknown MAC SA:         %08x%08x\n",
           ((WPT_StatField)bport_stats.discard_unk_macsa).part.high,
           ((WPT_StatField)bport_stats.discard_unk_macsa).part.low);
   printf ("Deny MAC SA:                    %08x%08x\n",
           ((WPT_StatField)bport_stats.deny_mac_sa).part.high,
           ((WPT_StatField)bport_stats.deny_mac_sa).part.low);
   printf ("Deny MAC DA:                    %08x%08x\n",
           ((WPT_StatField)bport_stats.deny_mac_da).part.high,
           ((WPT_StatField)bport_stats.deny_mac_da).part.low);
   printf ("Rx BC valid packets:            %08x%08x\n",
           ((WPT_StatField)bport_stats.rx_bc_valid_packets).part.high,
           ((WPT_StatField)bport_stats.rx_bc_valid_packets).part.low);
   printf ("Rx MC valid packets:            %08x%08x\n",
           ((WPT_StatField)bport_stats.rx_mc_valid_packets).part.high,
           ((WPT_StatField)bport_stats.rx_mc_valid_packets).part.low);
   printf ("Forwarded UC packets:           %08x%08x\n",
           ((WPT_StatField)bport_stats.forwarded_uc_packets).part.high,
           ((WPT_StatField)bport_stats.forwarded_uc_packets).part.low);
   printf ("Forwarded BC packets:           %08x%08x\n",
           ((WPT_StatField)bport_stats.forwarded_bc_packets).part.high,
           ((WPT_StatField)bport_stats.forwarded_bc_packets).part.low);
   printf ("Forwarded MC packets:           %08x%08x\n",
           ((WPT_StatField)bport_stats.forwarded_mc_packets).part.high,
           ((WPT_StatField)bport_stats.forwarded_mc_packets).part.low);

   for (i=0;i<5;i++)
   {
      printf ("DFC Filter %d Discard:           %08x%08x\n",i,
              ((WPT_StatField)bport_stats.discard_dfc_filter[i]).part.high,
              ((WPT_StatField)bport_stats.discard_dfc_filter[i]).part.low);
   }
   for(i=0; i<10; i++)
   {
      printf ("PCE Filter %d denied packets:    %08x%08x\n", i,
              ((WPT_StatField)bport_stats.discard_dfc_filter[i]).part.high,
              ((WPT_StatField)bport_stats.discard_dfc_filter[i]).part.low);
   }

   printf("\n");
   return rx_packets;
}

void WTE_DisplayFlowStats(WP_handle agg, WP_CHAR* type)
{
   WP_iw_flow_stats iw_flow_stats;
   WP_status status;

   memset(&iw_flow_stats,0,sizeof(iw_flow_stats));
   status = WP_IwFlowStatistics( agg, WP_IW_FLOW_STAT, &iw_flow_stats);
   terminate_on_error(status , "WP_IwFlowStatistics",__LINE__);

   printf ("\n%s Flow Aggregation (%x) statistics \n", type, agg);
   printf ("=================================================\n");

   printf ("Forward Packet:          %08x%08x\n",
           ((WPT_StatField)iw_flow_stats.forward_packet).part.high,
           ((WPT_StatField)iw_flow_stats.forward_packet).part.low);

   printf( "FBP Drop Packets:        %08x%08x\n",
           ((WPT_StatField)iw_flow_stats.fbp_drop_packets).part.high,
           ((WPT_StatField)iw_flow_stats.fbp_drop_packets).part.low);

   printf( "MTU Drop Packets:        %08x%08x\n",
           ((WPT_StatField)iw_flow_stats.mtu_drop_packets).part.high,
           ((WPT_StatField)iw_flow_stats.mtu_drop_packets).part.low);

   printf( "TTL Drop Packets:        %08x%08x\n",
           ((WPT_StatField)iw_flow_stats.ttl_drop_packets).part.high,
           ((WPT_StatField)iw_flow_stats.ttl_drop_packets).part.low);

   printf( "TX Queue Drop Packets:   %08x%08x\n",
           ((WPT_StatField)iw_flow_stats.tx_queue_drop_packets).part.high,
           ((WPT_StatField)iw_flow_stats.tx_queue_drop_packets).part.low);

   printf( "MPLS Drop:               %08x%08x\n",
           ((WPT_StatField)iw_flow_stats.mpls_drop).part.high,
           ((WPT_StatField)iw_flow_stats.mpls_drop).part.low);

   printf( "Denied Packets:          %08x%08x\n",
           ((WPT_StatField)iw_flow_stats.denied_packets).part.high,
           ((WPT_StatField)iw_flow_stats.denied_packets).part.low);

   printf( "Group filtered packets:  %08x%08x\n",
           ((WPT_StatField)iw_flow_stats.group_filtered_packets).part.high,
           ((WPT_StatField)iw_flow_stats.group_filtered_packets).part.low);

   printf( "forwarded_bytes:         %08x%08x\n",
           ((WPT_StatField)iw_flow_stats.forwarded_bytes).part.high,
           ((WPT_StatField)iw_flow_stats.forwarded_bytes).part.low);

   printf( "GTP bad headers:         %08x%08x\n",
           ((WPT_StatField)iw_flow_stats.gtp_bad_headers).part.high,
           ((WPT_StatField)iw_flow_stats.gtp_bad_headers).part.low);

   printf( "Policer Non Conforming:  %08x%08x\n",
           ((WPT_StatField)iw_flow_stats.policer_non_conforming_packets).part.high,
           ((WPT_StatField)iw_flow_stats.policer_non_conforming_packets).part.low);
}

/* Displays Generic Flow Aggregations Statistics. */
void WTE_DisplayGenericFlowStats(WP_handle agg, WP_CHAR *type)
{
   WP_iw_generic_flow_stats iw_flow_stats;

   memset(&iw_flow_stats,0,sizeof(iw_flow_stats));
   WP_IwFlowStatistics(agg, WP_IW_FLOW_STAT_GENERIC, &iw_flow_stats);

   printf("\n%s Generic Flow Aggregation (%x) statistics \n", type, agg);
   printf("==========================================================\n");

   printf("Forward Packet:         %08x%08x\n",
          ((WPT_StatField)iw_flow_stats.forward_packet).part.high,
          ((WPT_StatField)iw_flow_stats.forward_packet).part.low);

   printf("Forwarded Bytes:        %08x%08x\n",
          ((WPT_StatField)iw_flow_stats.forwarded_bytes).part.high,
          ((WPT_StatField)iw_flow_stats.forwarded_bytes).part.low);

   printf("MTU Drop Packets:       %08x%08x\n",
          ((WPT_StatField)iw_flow_stats.mtu_drop_packets).part.high,
          ((WPT_StatField)iw_flow_stats.mtu_drop_packets).part.low);

   printf("TX Queue Drop Packets:  %08x%08x\n",
          ((WPT_StatField)iw_flow_stats.tx_queue_drop_packets).part.high,
          ((WPT_StatField)iw_flow_stats.tx_queue_drop_packets).part.low);

   printf("Denied Packets:         %08x%08x\n",
          ((WPT_StatField)iw_flow_stats.denied_packets).part.high,
          ((WPT_StatField)iw_flow_stats.denied_packets).part.low);

   printf("Policer Non Conforming: %08x%08x\n",
          ((WPT_StatField)iw_flow_stats.policer_non_conforming_packets).part.high,
          ((WPT_StatField)iw_flow_stats.policer_non_conforming_packets).part.low);
}
#if 0
 void WTE_DisplayFlowStats(WP_handle agg, WP_CHAR *type)
 {
    WP_iw_flow_stats iw_flow_stats;

    memset(&iw_flow_stats,0,sizeof(iw_flow_stats));
    WP_IwFlowStatistics(agg, WP_IW_FLOW_STAT, &iw_flow_stats);

    printf("\n%s Flow Aggregation (%x) statistics \n", type, agg);
    printf("==========================================================\n");

    printf("Forward Packet:         %08x%08x\n",
           ((WPT_StatField)iw_flow_stats.forward_packet).part.high,
           ((WPT_StatField)iw_flow_stats.forward_packet).part.low);

    printf("Forwarded Bytes:        %08x%08x\n",
           ((WPT_StatField)iw_flow_stats.forwarded_bytes).part.high,
           ((WPT_StatField)iw_flow_stats.forwarded_bytes).part.low);

    printf("MTU Drop Packets:       %08x%08x\n",
           ((WPT_StatField)iw_flow_stats.mtu_drop_packets).part.high,
           ((WPT_StatField)iw_flow_stats.mtu_drop_packets).part.low);

    printf("TX Queue Drop Packets:  %08x%08x\n",
           ((WPT_StatField)iw_flow_stats.tx_queue_drop_packets).part.high,
           ((WPT_StatField)iw_flow_stats.tx_queue_drop_packets).part.low);

    printf("Denied Packets:         %08x%08x\n",
           ((WPT_StatField)iw_flow_stats.denied_packets).part.high,
           ((WPT_StatField)iw_flow_stats.denied_packets).part.low);

    printf("Policer Non Conforming: %08x%08x\n",
           ((WPT_StatField)iw_flow_stats.policer_non_conforming_packets).part.high,
           ((WPT_StatField)iw_flow_stats.policer_non_conforming_packets).part.low);
 }
#endif
void WTE_DisplayPceGlobalStats(void)
{
   WP_pce_global_stat pce_global_stats;
   WP_U32 temp1, temp2;

   memset(&pce_global_stats,0,sizeof(pce_global_stats));
   WP_PceGlobalStatistics(WP_WINPATH(0), &pce_global_stats);

   printf("\n\nPCE Global Statistics\n");

   temp1 = pce_global_stats.parser_header_access_errors;
   temp2 = pce_global_stats.parser_header_access_errors >> 32;
   printf("parser_header_access_errors %08X %08X\n", temp2, temp1);

   temp1 = pce_global_stats.parser_timeout_err;
   temp2 = pce_global_stats.parser_timeout_err >> 32;
   printf("parser_timeout_err          %08X %08X\n", temp2, temp1);

   temp1 = pce_global_stats.classifier_timeout_err;
   temp2 = pce_global_stats.classifier_timeout_err >> 32;
   printf("classifier_timeout_err      %08X %08X\n", temp2, temp1);

   temp1 = pce_global_stats.parser_accept;
   temp2 = pce_global_stats.parser_accept >> 32;
   printf("parser_accept               %08X %08X\n", temp2, temp1);
   printf("\n");
}

void WTE_DisplayPceFilterStats(WP_handle filter)
{
   WP_pce_filter_stat pce_filter_stats[256];
   WP_U32 temp1, temp2;
   WP_U32 index = filter&0xff;

   WP_PceFilterStatistics (filter, &(pce_filter_stats[index]));

   printf("PCE Filter %d Statistics\n", index);

   temp1 = pce_filter_stats[index].denied_pkts;
   temp2 = pce_filter_stats[index].denied_pkts >> 32;
   printf("denied_pkts            %08X %08X\n", temp2, temp1);

   temp1 = pce_filter_stats[index].matched_pkts;
   temp2 = pce_filter_stats[index].matched_pkts >> 32;
   printf("matched_pkts           %08X %08X\n", temp2, temp1);

   temp1 = pce_filter_stats[index].no_matched_pkts;
   temp2 = pce_filter_stats[index].no_matched_pkts >> 32;
   printf("no_matched_pkts        %08X %08X\n", temp2, temp1);
   printf("\n");
}

WP_U32 WTE_DisplayAAL5RxChannelStats(WP_handle ch_handle, WP_CHAR *type)
{
   WP_stats_aal5_rx aal5_rx;
   WP_status status = WP_OK;
   WP_U32 rx_frames = 0;

   memset( &aal5_rx, 0, sizeof(WP_stats_aal5_rx) );

   status = WP_ChannelStatistics( ch_handle, &aal5_rx );

   printf ("\n%s RX channel %d (0x%08x)\n",
           type,(ch_handle & 0x000000ff),ch_handle);
   printf ("=================================================\n");

   printf ("RX Frames:          %08x%08x\n",
           ((WPT_StatField)aal5_rx.rx_frames).part.high,
           ((WPT_StatField)aal5_rx.rx_frames).part.low);

   printf ("Aborted Frames:     %08x%08x\n",
           ((WPT_StatField)aal5_rx.aborted_frames).part.high,
           ((WPT_StatField)aal5_rx.aborted_frames).part.low);

   printf ("CRC Error:          %08x%08x\n",
           ((WPT_StatField)aal5_rx.err_crc).part.high,
           ((WPT_StatField)aal5_rx.err_crc).part.low);

   printf ("Size Error:         %08x%08x\n",
           ((WPT_StatField)aal5_rx.err_size).part.high,
           ((WPT_StatField)aal5_rx.err_size).part.low);

   printf ("Underlow Error:     %08x%08x\n",
           ((WPT_StatField)aal5_rx.err_underflow).part.high,
           ((WPT_StatField)aal5_rx.err_underflow).part.low);

   printf ("MaxSDU Error:       %08x%08x\n",
           ((WPT_StatField)aal5_rx.err_maxsdu).part.high,
           ((WPT_StatField)aal5_rx.err_maxsdu).part.low);

   printf ("IW Underrun Error:  %08x%08x\n",
           ((WPT_StatField)aal5_rx.err_iw_underrun).part.high,
           ((WPT_StatField)aal5_rx.err_iw_underrun).part.low);

   printf ("IW Max MRU Error:   %08x%08x\n",
           ((WPT_StatField)aal5_rx.err_iw_underrun).part.high,
           ((WPT_StatField)aal5_rx.err_iw_underrun).part.low);

   printf ("CPI Error:          %08x%08x\n",
           ((WPT_StatField)aal5_rx.err_cpi).part.high,
           ((WPT_StatField)aal5_rx.err_cpi).part.low);

   printf ("CLP Frames:         %08x%08x\n",
           ((WPT_StatField)aal5_rx.clp_frames).part.high,
           ((WPT_StatField)aal5_rx.clp_frames).part.low);

   printf ("L3 IW Frames:       %08x%08x\n",
           ((WPT_StatField)aal5_rx.l3_iw_frames).part.high,
           ((WPT_StatField)aal5_rx.l3_iw_frames).part.low);

   printf ("Ras Timeout Error:  %08x%08x\n",
           ((WPT_StatField)aal5_rx.err_ras_timeout).part.high,
           ((WPT_StatField)aal5_rx.err_ras_timeout).part.low);

   printf ("RX CLP0 Cells:      %08x%08x\n",
           ((WPT_StatField)aal5_rx.rx_clp0_cells).part.high,
           ((WPT_StatField)aal5_rx.rx_clp0_cells).part.low);

   printf ("RX CLP1 Cells:      %08x%08x\n",
           ((WPT_StatField)aal5_rx.rx_clp1_cells).part.high,
           ((WPT_StatField)aal5_rx.rx_clp1_cells).part.low);

   printf("\n");

   return rx_frames;
}

WP_U32 WTE_DisplayAAL5TxChannelStats(WP_handle ch_handle, WP_CHAR *type, WP_U8 display)
{
   WP_stats_aal5_tx aal5_tx;
   WP_status status = WP_OK;
   WP_U32 tx_frames = 0;

   memset( &aal5_tx, 0, sizeof(WP_stats_aal5_tx) );

   status = WP_ChannelStatistics( ch_handle, &aal5_tx );
   if (display)
   {
      printf ("\n%s TX channel %d (0x%08x)\n",
              type,(ch_handle & 0x000000ff),ch_handle);
      printf ("=================================================\n");

      printf ("TX Frames:          %08x%08x\n",
              ((WPT_StatField)aal5_tx.tx_frames).part.high,
              ((WPT_StatField)aal5_tx.tx_frames).part.low);
      tx_frames = ((WPT_StatField)aal5_tx.tx_frames).part.low;

      printf ("TX CLP0 Cells:      %08x%08x\n",
              ((WPT_StatField)aal5_tx.tx_clp0_cells).part.high,
              ((WPT_StatField)aal5_tx.tx_clp0_cells).part.low);

      printf ("TX CLP1 Cells:      %08x%08x\n",
              ((WPT_StatField)aal5_tx.tx_clp1_cells).part.high,
              ((WPT_StatField)aal5_tx.tx_clp1_cells).part.low);

      printf("\n");
   }
   else
   {
      tx_frames = (WP_U32)aal5_tx.tx_frames;
   }

   return tx_frames;
}

#define FOREGROUND_COLOR_BASE 40
#define BACKGROUND_COLOR_BASE 30

/***************************************************************
 * Func name  : Vt100Bright
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void Vt100Bright(void)
{
   printf("\033[1m");
} /* Vt100Bright */

/***************************************************************
 * Func name  : Vt100Reverse
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void Vt100Reverse(void)
{
   printf("\033[7m");
} /* Vt100Reverse */

/***************************************************************
 * Func name  : Vt100Color
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void Vt100Color(WP_S32 foreground, WP_S32 background)
{
    printf(
        "\033[%d;%dm",
        FOREGROUND_COLOR_BASE + foreground,
        BACKGROUND_COLOR_BASE + background);
} /* Vt100Color */

/***************************************************************
 * Func name  : Vt100Normal
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void Vt100Normal(void)
{
    printf("\033[0m");
} /* Vt100Normal */

/***************************************************************
 * Func name  : Vt100GotoXy
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void Vt100GotoXy(WP_S32 x, WP_S32 y)
{
    printf("\033[%d;%dH",y, x);
} /* Vt100GotoXy */

/***************************************************************
 * Func name  : Vt100EraseEOL
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void Vt100EraseEOL(void)
{
    printf("\033[K");
} /* Vt100EraseEOL */

/***************************************************************
 * Func name  : Vt100ClearScreen
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void Vt100ClearScreen(void)
{
    printf("\033[2J");
} /* Vt100ClearScreen */

/***************************************************************
 * Func name  : Vt100CrsrInvisible
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void Vt100CrsrInvisible(void)
{
   printf("\033[?25l");
}

/***************************************************************
 * Func name  : Vt100CrsrVisible
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void Vt100CrsrVisible(void)
{
   printf("\033[?25h");
}

/***************************************************************
 * Func name  : Vt100CrsrUp
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void Vt100CrsrUp(WP_U32 up)
{
   printf("\033[%dA",up);
}

/***************************************************************
 * Func name  : Vt100CrsrDown
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void Vt100CrsrDown(WP_U32 down)
{
   printf("\033[%dB",down);
}

/***************************************************************
 * Func name  : Vt100EraseDown
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void Vt100EraseDown(void)
{
    printf("\033[J");
}
/***************************************************************/

WP_U8 F_SampleSerial(void)
{
   WP_U32 uart_line_status_reg;
   WP_U32 data;

    /* sample serial port */
   uart_line_status_reg = WPX_AppWpIntRegBaseGet(0) + WEB_UART_LINE_STATUS_REG_OFFSET;

   data = *(WP_U32 volatile *)uart_line_status_reg;
    if (data & (WEB_UART_STATUS_RXFE |
                WEB_UART_STATUS_OE |
                WEB_UART_STATUS_DR))
       return 1;
    else
       return 0;
}



