/*************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "wp_wddi.h"
#include "wp_host_io.h"

#include "wpx_tdm_comet.h"

#include "wp_sim.h"
#include "wpx_app_data.h"

#include "wpx_oc3_pmc5351.h"
#include "wpx_oc12_pmc5357.h"
#ifndef _WT_IMA_DEBUG_H_
#define _WT_IMA_DEBUG_H_
typedef enum{
  ICP_ATM_HEADER          =      1,
  ICP_OAM_LABEL           =      5,
  ICP_CELL_LINK_ID        =      6,
  ICP_FRAME_SEQUENCE_NO   =      7,
  ICP_CELL_OFFSET         =      8,
  ICP_LINK_INDICATION     =      9,
  ICP_STATUS_INDICATION   =      10,
  ICP_IMA_ID              =      11,
  ICP_GROUP_STATUS        =      12,
  ICP_TRANSMIT_TIMING     =      13,
  ICP_TX_TEST_CONTROL     =      14,
  ICP_TX_TEST_PATTERN     =      15,
  ICP_RX_TEST_PATTERN     =      16,
  ICP_LINK_INFO           =      17,
  ICP_END_TO_END_CHANNEL  =      50,
  ICP_CRC_ERROR_CONTROL   =      51
}ICP_cell_format;

typedef enum{
  ICP_DECODE_HEADER   = 0,
  ICP_DECODE_OAM_LABEL,
  ICP_DECODE_CELL_LINK_ID,
  ICP_DECODE_FRAME_SEQUENCE_NUMBER,
  ICP_DECODE_ICP_CELL_OFFSET,
  ICP_DECODE_LINK_STUFF_INDICATION,
  ICP_DECODE_STATUS_CHANGE_INDICATION,
  ICP_DECODE_IMA_ID,
  ICP_DECODE_GROUP_STATUS_AND_CONTROL,
  ICP_DECODE_TRANSMIT_TIMING_INFO,
  ICP_DECODE_TX_TEST_CONTROL,
  ICP_DECODE_TX_TEST_PATTERN,
  ICP_DECODE_RX_TEST_PATTERN,
  ICP_DECODE_LINK_INFO,
  ICP_DECODE_END_TO_END_CHANNEL,
  ICP_DECODE_CRC_ERROR_CONTROL,
  ICP_DECODE_ALL
 }ICP_decode_commands;

void CLI_IMA_DecodeAlarmState(WP_U16 state);
void CLI_IMA_DecodeAlarmType(WP_U16 type);
void CLI_IMA_DecodeOAM(WP_U16 oam_label);
void CLI_IMA_DecodeSymmetry(WP_U16 symmetry);
void CLI_IMA_DecodeGroupControl(WP_U16 group_control);
void CLI_IMA_DecodeFrameLength(WP_U16 frame_size);
void CLI_IMA_DecodeClockMode(WP_U16 clock_mode);
#endif
/* Specify which test features are selected.  */

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

void print_icp(WP_ima_icp_cell *icp)
{
   WP_U32 *ptr_word;
   WP_U32 i;
   ptr_word = (WP_U32 *)icp;
   printf("\n");
   for(i=0;i < 13;  i++)
   {
     if(i && ((i%4) == 0)) printf("\n");
     printf("%8.8x ",*ptr_word++);
   }
   printf("\n");
}


void tdm_stats_print(WP_stats_tdi_atm *tdi_atm_stats)
{
   printf("\nRx Err Hec => %#8.8x %#8.8x\n",
          (WP_U32) (tdi_atm_stats->rx_err_hec >> 32),
          (WP_U32) (tdi_atm_stats->rx_err_hec & 0xffffffff));
   printf("Rx Addr MM => %#8.8x %#8.8x\n",
          (WP_U32) (tdi_atm_stats->rx_err_addr_mismatch >> 32),
          (WP_U32) (tdi_atm_stats->rx_err_addr_mismatch & 0xffffffff));
   printf("Rx Overrun => %#8.8x %#8.8x\n",
          (WP_U32) (tdi_atm_stats->rx_err_overrun >> 32),
          (WP_U32) (tdi_atm_stats->rx_err_overrun & 0xffffffff));
   printf("Rx OOS     => %#8.8x %#8.8x\n",
          (WP_U32) (tdi_atm_stats->rx_out_of_sync >> 32),
          (WP_U32) (tdi_atm_stats->rx_out_of_sync & 0xffffffff));
   printf("Rx Enter Sync => %#8.8x %#8.8x\n",
          (WP_U32) (tdi_atm_stats->rx_enter_sync >> 32),
          (WP_U32) (tdi_atm_stats->rx_enter_sync & 0xffffffff));
   printf("Rx Cells => %#8.8x %#8.8x\n",
          (WP_U32) (tdi_atm_stats->ext_stats.rx_cells >> 32),
          (WP_U32) (tdi_atm_stats->ext_stats.rx_cells & 0xffffffff));
}


void print_link_statistics(WP_ima_link_statistics * link_statistics)
{
   printf("NE SES           => %2.2d\n", link_statistics->rx_ne_ses);
   printf("FE SES           => %2.2d\n", link_statistics->rx_fe_ses);
   printf("NE UAS           => %2.2d\n", link_statistics->rx_ne_uas);
   printf("FE UAS           => %2.2d\n", link_statistics->rx_fe_uas);
   printf("NE Rx UUS        => %2.2d\n", link_statistics->rx_ne_uus);
   printf("FE Tx UUS        => %2.2d\n", link_statistics->tx_fe_uus);
   printf("NE Tx UUS        => %2.2d\n", link_statistics->tx_ne_uus);
   printf("FE Rx UUS        => %2.2d\n", link_statistics->rx_fe_uus);
   printf("NE Rx Failure    => %2.2d\n", link_statistics->rx_ne_failure);
   printf("FE Rx Failure    => %2.2d\n", link_statistics->rx_fe_failure);
   printf("FE Tx Failure    => %2.2d\n", link_statistics->tx_fe_failure);
   printf("Rx ICP Cells     => %2.2d\n", (link_statistics->rx_icp_cells -
                                          link_statistics->rx_stuff));
   printf("Rx Err ICP Cells => %2.2d\n", link_statistics->rx_err_icp_cells);
   printf("Rx LODS          => %2.2d\n", link_statistics->rx_lods);
   printf("Rx OIF           => %2.2d\n", link_statistics->rx_oif);
   printf("Rx ICP Violation => %2.2d\n", link_statistics->rx_icp_violations);
   printf("Rx Stuff Cells   => %2.2d\n", link_statistics->rx_stuff);
   printf("Rx FE Defects    => %2.2d\n", link_statistics->rx_fe_defects);
   printf("Tx ICP Cells     => %2.2d\n", link_statistics->tx_icp_cells);
   printf("Tx Stuff Cells   => %2.2d\n", link_statistics->tx_stuff);

}




void CLI_IMA_DecodeOAM(WP_U16 oam_label)
{
  switch(oam_label){
  case 1:
      printf("IMA version 1.0\n");
      break;
  case 3:
      printf("IMA version 1.1\n");
      break;
  default:
      printf("Invalid version\n");
  }
}

void CLI_IMA_DecodeSymmetry(WP_U16 symmetry)
{
  switch(symmetry){
  case WP_IMA_SYM_CFG_SYM_OP:
    printf("Symmetrical configuration and operation\n");
    break;
  case WP_IMA_SYM_CFG_ASYM_OP:
    printf("Symmetrical configuration and asymmetrical operation\n");
    break;
  case WP_IMA_ASYM_CFG_ASYM_OP:
    printf("Asymmetrical configuration and asymmetrical operation\n");
    break;
  default:
    printf("Unrecognized\n");
    break;
  }
}

void CLI_IMA_DecodeClockMode(WP_U16 clock_mode)
{
  switch(clock_mode)
  {
    case WP_IMA_TX_CLOCK_ITC:
         printf("ITC mode\n");
         break;
    case WP_IMA_TX_CLOCK_CTC:
         printf("CTC mode\n");
         break;
     default:
         printf("Unrecognized\n");
         break;
  }
}

void CLI_IMA_DecodeAlarmState(WP_U16 state)
{
 switch(state){
  case WP_IMA_GROUP_NO_ALARM:
        printf("No Alarm\n");
        break;
  case WP_IMA_GROUP_FAILURE:
        printf("Group Failure\n");
        break;
  case WP_IMA_GROUP_ALARM:
        printf("Group Alarm\n");
        break;
  default:
        printf("Unrecognized\n");
        break;
  }
}

void CLI_IMA_DecodeAlarmType(WP_U16 type)
{
 switch(type){
  case WP_IMA_GROUP_ALARM_START_UP:
       printf("Start Up\n");
       break;
  case WP_IMA_GROUP_ALARM_CLOCK_MISMATCH:
       printf("Clock Mismatch\n");
       break;
  case WP_IMA_GROUP_ALARM_CONFIG_ABORTED:
       printf("Config Aborted\n");
       break;
  case WP_IMA_GROUP_ALARM_INSUFF_LINKS:
       printf("Insufficient Links\n");
       break;
  case WP_IMA_GROUP_ALARM_BLOCKED:
       printf("Alarm Blocked\n");
       break;
  case WP_IMA_GROUP_ALARM_VERSION_MISMATCH:
       printf("Alarm version mismatch\n");
       break;
  default:
       printf("Unrecognized\n");
  }
}

void CLI_IMA_DecodeGroupControl(WP_U16 group_control)
{
 switch(group_control){

  case WP_IMA_FE_GSM_START_UP:
        printf("Start Up\n");
        break;
  case WP_IMA_FE_GSM_START_UP_ACK:
        printf("Start Up ACK\n");
        break;
  case WP_IMA_FE_GSM_CONFIG_ABORTED_M:
        printf("Config Aborted -unsupported M\n");
        break;
  case WP_IMA_FE_GSM_CONFIG_ABORTED_SYM:
        printf("Config Aborted -Incompatable Group Symmetry\n");
        break;
  case WP_IMA_FE_GSM_CONFIG_ABORTED_VER:
        printf("Config Aborted -Unsupported IMA Version\n");
        break;
  case WP_IMA_FE_GSM_CONFIG_ABORTED_RES1:
  case WP_IMA_FE_GSM_CONFIG_ABORTED_RES2:
        printf("Config Aborted -Reserved reasons\n");
        break;
  case WP_IMA_FE_GSM_CONFIG_ABORTED_OTH:
        printf("Config Aborted -other\n");
        break;
  case WP_IMA_FE_GSM_INSUFFICIENT:
        printf("Insufficient Links\n");
        break;
  case WP_IMA_FE_GSM_BLOCKED:
        printf("Blocked\n");
        break;
  case WP_IMA_FE_GSM_OPERATIONAL:
        printf("Operational\n");
        break;
  case WP_IMA_FE_GSM_RESERVED_1:
  case WP_IMA_FE_GSM_RESERVED_2:
  case WP_IMA_FE_GSM_RESERVED_3:
  case WP_IMA_FE_GSM_RESERVED_4:
  case WP_IMA_FE_GSM_RESERVED_5:
        printf("Reserved for future use\n");
        break;
  case WP_IMA_NO_ICP_RECEIVED:
        printf("No ICP Received\n");
        break;
  default:
        printf("Unrecognized\n");
        break;
  }
}

void CLI_IMA_DecodeFrameLength(WP_U16 frame_size)
{
  switch(frame_size){
  case WP_IMA_FRAME_SIZE_32:
    printf("M = 32\n");
    break;
  case WP_IMA_FRAME_SIZE_64:
    printf("M = 64\n");
    break;
  case WP_IMA_FRAME_SIZE_128:
    printf("M = 128\n");
    break;
  case WP_IMA_FRAME_SIZE_256:
    printf("M = 256\n");
    break;
  default:
    printf("Unrecognized\n");
    break;
  }
}

void CLI_IMA_DecodeLinkState(WP_U16 state)
{
  switch(state){
  case WP_IMA_LSM_NOT_IN_GROUP:
   printf("Not in Group  ");
   break;
  case WP_IMA_LSM_UNUSABLE:
   printf("Unusable -reason unknown  ");
   break;
  case WP_IMA_LSM_FAULT:
   printf("Unusable -fault (vendor specific)  ");
   break;
  case WP_IMA_LSM_MIS_CONNECTED:
   printf("Unusable -misconnected  ");
   break;
  case WP_IMA_LSM_INHIBITED:
   printf("Unusable -inhibited (vendor specific)  ");
   break;
  case WP_IMA_LSM_FAILED:
   printf("Unusable -failed (not currently defined)  ");
   break;
  case WP_IMA_LSM_USABLE:
   printf("Usable  ");
   break;
  case WP_IMA_LSM_ACTIVE:
   printf("Active  ");
   break;
  default:
   printf("Error Reading Link Information  ");
   break;
  }
}

/******************** ICP functions ***************************/

WP_U8 * ICP_Point2Byte(WP_ima_icp_cell *icp_ptr, WP_U8 byte_num)
{
  int i;
  WP_U8 * bytePtr;
  bytePtr = (WP_U8 *)icp_ptr;
  for(i = 1; i < byte_num; i++)
  {
    bytePtr++;
  }
  return bytePtr;
}

void ICP_CheckHeader(WP_ima_icp_cell *icp_ptr)
{
  WP_U8   *ptr_byte;
  WP_U32 header;
  ptr_byte = ICP_Point2Byte(icp_ptr, ICP_ATM_HEADER);
  header = *(WP_U32*)ptr_byte;
  if(header != 0x0000000b)
  {
    printf("Invalid ATM cell Header Value\n");
    printf("Expected 0x0000000b, Given 0x%08x\n",header);
  } else
     printf("Valid ATM Cell Header Value\n");
}


void ICP_CheckOamLabel(WP_ima_icp_cell *icp_ptr)
{
  WP_U8   *ptr_byte;
  WP_U8   label;
  ptr_byte = ICP_Point2Byte(icp_ptr, ICP_OAM_LABEL);
  label = *ptr_byte;
  CLI_IMA_DecodeOAM(label);
 return;
}


void ICP_CheckIDs(WP_ima_icp_cell *icp_ptr)
{
  WP_U8   *ptr_byte;
  WP_U8   ID;
  ptr_byte = ICP_Point2Byte(icp_ptr, ICP_CELL_LINK_ID);
  ID = *ptr_byte;
  if(ID & 0x80)
   printf("IMA OAM Cell Type: ICP cell\n");
  else
   printf("IMA OAM Cell Type: non ICP cell\n");

  printf("Link ID: 0x%02x\n",(ID & 0x1F));

  return;
}

void ICP_PrintSequenceNo(WP_ima_icp_cell *icp_ptr)
{
  WP_U8   *ptr_byte;
  WP_U8   sequence;
  ptr_byte = ICP_Point2Byte(icp_ptr, ICP_FRAME_SEQUENCE_NO);
  sequence = *ptr_byte;
  printf("IMA Frame Sequence Number: %d\n",sequence);
}

void ICP_PrintCellOffset(WP_ima_icp_cell *icp_ptr)
{
  WP_U8   *ptr_byte;
  WP_U8   offset;
  ptr_byte = ICP_Point2Byte(icp_ptr, ICP_CELL_OFFSET);
  offset = *ptr_byte;
  printf("Position within IMA frame: byte %d\n",offset);
}

void ICP_LinkStuffIndication(WP_ima_icp_cell *icp_ptr)
{
  WP_U8   *ptr_byte;
  WP_U8   stuff;
  ptr_byte = ICP_Point2Byte(icp_ptr, ICP_LINK_INDICATION);
  stuff = *ptr_byte;
  if(stuff & 0xF8)
  {
    printf("Error, bits 7-3 not cleared\n");
    return;
  }
  switch(stuff){
  case 0x7:
     printf("No imminent stuff event\n");
     break;
  case 0x1:
     printf("Stuff event at the next ICP cell location\n");
     break;
  case 0x0:
     printf("1 of 2 ICP cells comprising the stuff event\n");
     break;
  default:
     printf("Stuff event in %d ICP cell locations\n",stuff);
     break;
  }
  return;
}

void ICP_StatusChangeIndication(WP_ima_icp_cell *icp_ptr)
{
  WP_U8   *ptr_byte;
  WP_U8   changes;
  ptr_byte = ICP_Point2Byte(icp_ptr, ICP_STATUS_INDICATION);
  changes = *ptr_byte;
  printf("SCCI: 0x%08x\n",changes);
}

void ICP_PrintImaID(WP_ima_icp_cell *icp_ptr)
{
  WP_U8   *ptr_byte;
  WP_U8   ID;
  ptr_byte = ICP_Point2Byte(icp_ptr, ICP_IMA_ID);
  ID = *ptr_byte;
  printf("IMA_ID: %02x\n",ID);
}

void ICP_GroupStatusControl(WP_ima_icp_cell *icp_ptr)
{
  WP_U8   *ptr_byte;
  WP_U8   status;
  ptr_byte = ICP_Point2Byte(icp_ptr, ICP_GROUP_STATUS);

  status = *ptr_byte;
  printf("Group State: ");
  CLI_IMA_DecodeGroupControl((status&0xF0) >> 4);

  printf("Group Symmetry Mode: ");
  CLI_IMA_DecodeSymmetry((status&0x0A) >> 2);

  printf("IMA frame length: ");
  CLI_IMA_DecodeFrameLength(status&0x03);
}

void ICP_TransmitTimingInfo(WP_ima_icp_cell * icp_ptr)
{
  WP_U8   *ptr_byte;
  WP_U8   timing;
  ptr_byte = ICP_Point2Byte(icp_ptr, ICP_TRANSMIT_TIMING);
  timing = *ptr_byte;

  printf("Transmit Clock Mode: ");
  CLI_IMA_DecodeClockMode((timing&0x20) >> 5);
  printf("Timing reference Tx LID: %d\n",timing&0x1F);
}

void ICP_TxTestControl(WP_ima_icp_cell * icp_ptr)
{
  WP_U8   *ptr_byte;
  WP_U8   control;
  ptr_byte = ICP_Point2Byte(icp_ptr, ICP_TX_TEST_CONTROL);
  control = *ptr_byte;

  if(control&0x20)
   printf("Tx Testing is active\n");
  else
   printf("Tx Testing is inactive\n");

  printf("Test link Tx LID: %d\n",control&0x1F);
}

void ICP_TxTestPattern(WP_ima_icp_cell * icp_ptr)
{
  WP_U8   *ptr_byte;
  WP_U8   pattern;
  ptr_byte = ICP_Point2Byte(icp_ptr, ICP_TX_TEST_PATTERN);
  pattern = *ptr_byte;

  printf("Tx Test Pattern: %d\n",pattern);
}

void ICP_RxTestPattern(WP_ima_icp_cell * icp_ptr)
{
  WP_U8   *ptr_byte;
  WP_U8   pattern;
  ptr_byte = ICP_Point2Byte(icp_ptr, ICP_RX_TEST_PATTERN);
  pattern = *ptr_byte;

  printf("Rx Test Pattern: %d\n",pattern);
}


void ICP_LinkInfo(WP_ima_icp_cell * icp_ptr, WP_U8 link_no)
{
  WP_U8   *ptr_byte;
  WP_U8   info;
  WP_S32  i;
  ptr_byte = ICP_Point2Byte(icp_ptr, ICP_LINK_INFO);
  for(i = 0; i < link_no; i++)
    ptr_byte++;
  info = *ptr_byte;

  if((info & 0xFC) == 0)  /*If Tx Rx both not in group, dont report*/
    return;

  printf("Link %02d Transmit state:  ",link_no);
  CLI_IMA_DecodeLinkState((info&0xE0) >> 5);

  printf("Receive state:  ");
  CLI_IMA_DecodeLinkState((info&0x1A) >> 2);
  printf("Rx Defect Indicators: ");

 switch((info&0x03)){
  case 0:
   printf("No defect\n");
   break;
  case 1:
   printf("Physical Link Defect\n");
   break;
  case 2:
   printf("LIF\n");
   break;
  case 3:
   printf("LODS\n");
   break;
  default:
   printf("Error Reading Link Information\n");
   break;
  }

}

void ICP_End2EndChannel(WP_ima_icp_cell * icp_ptr)
{
  WP_U8   *ptr_byte;
  WP_U8   channel;
  ptr_byte = ICP_Point2Byte(icp_ptr, ICP_END_TO_END_CHANNEL);
  channel = *ptr_byte;

  if(channel)
    printf("End to End channel: %d\n",channel);
  else
    printf("End to End channel: unused\n");
}

void ICP_CRCErrorControl(WP_ima_icp_cell * icp_ptr)
{

  WP_U8   *ptr_byte;
  ptr_byte = ICP_Point2Byte(icp_ptr, ICP_CRC_ERROR_CONTROL);
  printf("CRC Error Control Bytes: %02x ",*ptr_byte);
  printf("%02x\n",*(++ptr_byte));
}

void ICP_Decode(WP_ima_icp_cell * icp_ptr, WP_U32 cmds, WP_U32 extra_options)
{
  WP_S32 i;

  switch(cmds){
  case ICP_DECODE_HEADER:
         ICP_CheckHeader(icp_ptr);
         break;
  case ICP_DECODE_OAM_LABEL:
         ICP_CheckOamLabel(icp_ptr);
         break;
  case  ICP_DECODE_CELL_LINK_ID:
         ICP_CheckIDs(icp_ptr);
         break;
  case  ICP_DECODE_FRAME_SEQUENCE_NUMBER:
         ICP_PrintSequenceNo(icp_ptr);
         break;
  case  ICP_DECODE_ICP_CELL_OFFSET:
         ICP_PrintCellOffset(icp_ptr);
         break;
  case  ICP_DECODE_LINK_STUFF_INDICATION:
         ICP_LinkStuffIndication(icp_ptr);
         break;
  case  ICP_DECODE_STATUS_CHANGE_INDICATION:
         ICP_StatusChangeIndication(icp_ptr);
         break;
  case  ICP_DECODE_IMA_ID:
         ICP_PrintImaID(icp_ptr);
         break;
  case  ICP_DECODE_GROUP_STATUS_AND_CONTROL:
         ICP_GroupStatusControl(icp_ptr);
         break;
  case  ICP_DECODE_TRANSMIT_TIMING_INFO:
         ICP_TransmitTimingInfo(icp_ptr);
         break;
  case  ICP_DECODE_TX_TEST_CONTROL:
         ICP_TxTestControl(icp_ptr);
         break;
  case  ICP_DECODE_TX_TEST_PATTERN:
         ICP_TxTestPattern(icp_ptr);
         break;
  case  ICP_DECODE_RX_TEST_PATTERN:
         ICP_RxTestPattern(icp_ptr);
         break;
  case  ICP_DECODE_LINK_INFO:
         ICP_LinkInfo(icp_ptr, extra_options);
         break;
  case  ICP_DECODE_END_TO_END_CHANNEL:
         ICP_End2EndChannel(icp_ptr);
         break;
  case  ICP_DECODE_CRC_ERROR_CONTROL:
         ICP_CRCErrorControl(icp_ptr);
         break;
  case  ICP_DECODE_ALL:
         ICP_CheckHeader(icp_ptr);
         ICP_CheckOamLabel(icp_ptr);
         ICP_CheckIDs(icp_ptr);
         ICP_PrintSequenceNo(icp_ptr);
         ICP_PrintCellOffset(icp_ptr);
         ICP_LinkStuffIndication(icp_ptr);
         ICP_StatusChangeIndication(icp_ptr);
         ICP_PrintImaID(icp_ptr);
         ICP_GroupStatusControl(icp_ptr);
         ICP_TransmitTimingInfo(icp_ptr);
         ICP_TxTestControl(icp_ptr);
         ICP_TxTestPattern(icp_ptr);
         ICP_RxTestPattern(icp_ptr);
         for(i = 0; i < 32; i++)
           ICP_LinkInfo(icp_ptr, i);
         ICP_End2EndChannel(icp_ptr);
         ICP_CRCErrorControl(icp_ptr);
         break;
  default:
         printf("Invalid command\n");

  }
}

/******************** ICP functions ***************************/
