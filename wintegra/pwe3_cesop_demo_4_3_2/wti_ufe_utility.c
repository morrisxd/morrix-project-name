/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/****************************************************************************
 *
 * Example: wti_ufe_utility.c
 * UFE demo utility functions for PWE3 CES application.
 * Demonstrates the use of UFE API.
 *
 ****************************************************************************/
#include <string.h>

#include "wufe_types.h"    /* All .H files located at: /projects/winpath/wddi/phy/wpx_ufe/api */
#include "wufe_errors.h"
#ifndef WTI_UFE_UTILITY_H
#include "wti_ufe_utility.h"
#endif

#ifdef WT_UFE_FRAMER
#ifndef WTI_FLEXMUX_UTIL_H
#include "wti_flexmux_util.c"
#endif
#include "wti_flexmux_alarms_and_pm.c"
#endif

#define USE_UFE 1

WUFE_events events_connector_0;
WP_boolean event_contents_0 = WP_FALSE;

WUFE_events events_connector_2;
WP_boolean event_contents_2 = WP_FALSE;

static void WT_UfeTerminateOnError(WP_handle handle, WP_CHAR *s,WP_U32 id,WP_U32 LineNum)
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

/**********************************************************************************
 **********************************************************************************
 ************                       SYSTEM                    *********************
 **********************************************************************************
 *********************************************************************************/

void WT_UfeSystemConfig(WT_ufe *ufe, WUFE_system *sys_cfg)
{
   memset(sys_cfg, 0, sizeof(WUFE_system));

   if (ufe->fpga_mode == WT_SONET)
   {
      sys_cfg->line_interface = WUFE_INTERFACE_SONET;
   }
   else if (ufe->fpga_mode == WT_SDH)
   {
      sys_cfg->line_interface = WUFE_INTERFACE_SDH;
   }
   else if (ufe->fpga_mode == WT_OCTAL)
   {
      sys_cfg->line_interface = WUFE_INTERFACE_CAD_E1_T1;
   }
   else
   {
      WUFE_status ufe_status = 0x1;
      WT_UfeTerminateOnError(ufe_status, "HW not SONET or SDH", ufe->fpga_mode, __LINE__);
   }

   sys_cfg->max_line_number = ufe->max_lines;
   sys_cfg->max_phy_number = ufe->max_phys;

   sys_cfg->idle_pattern.tx_idle_byte_data_pattern=0x81;
   sys_cfg->idle_pattern.tx_idle_nibble_cas_pattern=0x4;

   sys_cfg->emphy.rx_parity_mode  = WUFE_RX_PARITY_EVEN;
   sys_cfg->emphy.tx_parity_mode  = WUFE_TX_PARITY_EVEN;
   sys_cfg->emphy.tx_parity_check = WUFE_TX_PARITY_IGNORE;

   sys_cfg->tdm_parity.rx_parity_mode  = WUFE_RX_PARITY_EVEN;
   sys_cfg->tdm_parity.tx_parity_mode  = WUFE_TX_PARITY_EVEN;

   sys_cfg->fifo_manager.loopback = WUFE_FM_LOOPBACK_DISABLE;
   sys_cfg->max_buff_num = WUFE_MAX_BUF_SIZE_64;

   /* If set, enable CR at system level */
   sys_cfg->system_clock_rec_enable = ufe->system_clock_rec_enable;
   sys_cfg->clk_rec_system_params = ufe->clk_rec_system_params;
}

void WT_UfeInitializeFirmware(WP_U32 ufe_id)
{
   WUFE_init_config config;
   WUFE_status ufe_status;

   memset(&config, 0, sizeof(WUFE_init_config));

#if WTI_DUAL_EMPHY

   if (ufe_id != 0)
   {
      config.attributes |= WUFE_ATTRIB_CONNECT_OFFSET_F(WTI_SECOND_EMPHY_PORT);
   }

   /* change UFE_ID to 0 in order to retrieve correct base address (WDS only) */
   config.ufe_id = 0;

#else

   config.ufe_id = ufe_id;

   config.attributes |= WUFE_ATTRIB_CONNECT_OFFSET_F(WTI_EMPHY_PORT);
#endif

   ufe_status = WUFE_UfeInitializeFirmware(&config);
   WT_UfeTerminateOnError(ufe_status, "WUFE_UfeInitializeFirmware  ",0,__LINE__);
}


#ifdef WT_UFE_FRAMER

WP_U8 last_interrupt_mask;
// extern WP_U32 WPI_TimeRead(WP_U32 *result);
extern WP_status WPI_TimeRead(WP_U32 *result, WP_U32 wpid);
WP_U32 timeResult = 0;

void WT_ReinstateInterruptMask(WP_U32 wpid)
{
#if WTI_COLLECT_TIMING_INFORMATION
   WP_U8 pending;
   extern WP_U8 WPX_Ufe412CpldInterruptSourceGet(WP_U32 wpid);
   WPI_TimeRead(&timeResult, 0);
   record_action(15, timeResult);
   pending = WPX_Ufe412CpldInterruptSourceGet(0);
   record_action(17, pending);
#endif

   /* Reset the CPLD interrupt mask */
   WPX_Ufe412CpldInterruptMaskSet(wpid, last_interrupt_mask);
}


#if WTI_DUAL_EMPHY
#define CONNECTOR_0_UFE_ID    WTI_FLEXMUX_PRIMARY_FLEXMUX_ID
#define CONNECTOR_0_DEVICE_ID WTI_FLEXMUX_PRIMARY_DEVICE_ID
#define CONNECTOR_2_UFE_ID    WTI_FLEXMUX_SECOND_FLEXMUX_ID
#define CONNECTOR_2_DEVICE_ID WTI_FLEXMUX_SECOND_DEVICE_ID
#else
#define CONNECTOR_0_UFE_ID    WTI_FLEXMUX_PRIMARY_FLEXMUX_ID
#define CONNECTOR_0_DEVICE_ID WTI_FLEXMUX_PRIMARY_DEVICE_ID
#define CONNECTOR_2_UFE_ID    WTI_FLEXMUX_PRIMARY_FLEXMUX_ID
#define CONNECTOR_2_DEVICE_ID WTI_FLEXMUX_PRIMARY_DEVICE_ID
#endif

void WT_Eint3Interrupt(WP_U32 wpid, WP_U32 signal_info)
{
   WP_boolean is_framer_int_0 = WP_FALSE, is_core_int_0 = WP_FALSE;
   WP_boolean is_framer_int_2 = WP_FALSE, is_core_int_2 = WP_FALSE;
   WUFE_events active_events;
   WP_U8 pending;
   extern void WPX_UFE_FRAMER_ISR(WP_U8 iDevice);
   extern WP_U32 WPL_IC_IntmodeGet(WP_U32 wpid);

#if WTI_COLLECT_TIMING_INFORMATION
   WPI_TimeRead(&timeResult, 0);
   record_action(13, timeResult);
#endif

   /* Recover the value of the CPLD interrupt mask at the time of the interrupt */
#if !defined(__linux__)
   last_interrupt_mask = WPX_Ufe412CpldInterruptMaskGet(0);
   WPX_Ufe412CpldInterruptMaskSet(0, WPX_FPGA_INTR_ALL_MASKED);
#else
   last_interrupt_mask = signal_info & 0xffff;
#endif

   pending = WPX_Ufe412CpldInterruptSourceGet(wpid);

#if WTI_COLLECT_TIMING_INFORMATION
   record_action(16, pending);
#endif

   if ((pending & WPX_FPGA_INTR_SERIAL_1_CONNECTOR) == 0)
   {
      is_framer_int_0 = WUFE_UfeFramerHwInterruptGet(CONNECTOR_0_UFE_ID);
      is_core_int_0 = WUFE_UfeCoreHwInterruptGet(CONNECTOR_0_UFE_ID);
   }

   if ((pending & WPX_FPGA_INTR_SERIAL_3_CONNECTOR) == 0)
   {
      is_framer_int_2 = WUFE_UfeFramerHwInterruptGet(CONNECTOR_2_UFE_ID);
      is_core_int_2 = WUFE_UfeCoreHwInterruptGet(CONNECTOR_2_UFE_ID);
   }

#if WTI_COLLECT_TIMING_INFORMATION

   if ((pending & WPX_FPGA_INTR_SERIAL_1_CONNECTOR) == 0)
   {
      if (is_framer_int_0 || is_core_int_0)
         record_action(43, (0 << 24) + (is_core_int_0 << 8) + is_framer_int_0);
      else
      {
         record_action(46, WUFE_UfeExtPllHwInterruptGet(CONNECTOR_0_UFE_ID));
         record_action(45, WUFE_UfeLockLostHwInterruptGet(CONNECTOR_0_UFE_ID));
         record_action(44, WUFE_UfeSfpHwInterruptGet(CONNECTOR_0_UFE_ID));
      }
   }

   if ((pending & WPX_FPGA_INTR_SERIAL_3_CONNECTOR) == 0)
   {
      if (is_framer_int_2 || is_core_int_2)
         record_action(43, (2 << 24) + (is_core_int_2 << 8) + is_framer_int_2);
      else
      {
         record_action(46, (2 << 24) + WUFE_UfeExtPllHwInterruptGet(CONNECTOR_2_UFE_ID));
         record_action(45, (2 << 24) + WUFE_UfeLockLostHwInterruptGet(CONNECTOR_2_UFE_ID));
         record_action(44, (2 << 24) + WUFE_UfeSfpHwInterruptGet(CONNECTOR_2_UFE_ID));
      }
   }

#endif

   if (is_framer_int_0)
      WPX_UFE_FRAMER_ISR(CONNECTOR_0_DEVICE_ID);

   if (is_core_int_0)
   {
      memset(&active_events, 0, sizeof(WUFE_events));

      WUFE_UfeReadEvents(CONNECTOR_0_UFE_ID, &active_events);

#if CHECK_CORE_HW_INTERRUPT_ASSERTED

      if (event_contents_0 == WP_FALSE)
      {
         memcpy(&events_connector_0, &active_events, sizeof(WUFE_events));
         event_contents_0 = WP_TRUE;
      }

#endif

   }

   if (is_framer_int_2)
      WPX_UFE_FRAMER_ISR(CONNECTOR_2_DEVICE_ID);

   if (is_core_int_2)
   {
      memset(&active_events, 0, sizeof(WUFE_events));

      WUFE_UfeReadEvents(CONNECTOR_2_UFE_ID, &active_events);

#if CHECK_CORE_HW_INTERRUPT_ASSERTED

      if (event_contents_2 == WP_FALSE)
      {
         memcpy(&events_connector_2, &active_events, sizeof(WUFE_events));
         event_contents_2 = WP_TRUE;
      }

#endif

   }

#if WTI_COLLECT_TIMING_INFORMATION
   WPI_TimeRead(&timeResult, 0);
   record_action(14, timeResult);
#endif

   if (is_framer_int_0 || is_framer_int_2)
   {
      WTI_flexmux_release_framer_lock();

#if WTI_COLLECT_TRACE_INFORMATION
      record_action(40, WPI_TimeRead());
#endif
   }

   WT_ReinstateInterruptMask(wpid);
}
#endif


void WUFE_BSSetExtPllFreeRunModeOffset(WP_U32 ufe_id,WP_S32 clock_offset)
{   
      WUFEI_BoardWriteZarlinkPll1FreeRunFreqOffset(ufe_id,clock_offset);
}

void  WT_UfeSystemCreate(WT_ufe *ufe)
{
   WUFE_system sys_cfg;
   WUFE_status ufe_status;
   WUFE_init_config config;
#if !defined(__linux__)
   extern void WPL_InterruptConfigureEint3(WP_U32 wpid);
#endif

   memset(&config, 0, sizeof(WUFE_init_config));
   config.ufe_id = ufe->ufe_id;

   if(ufe->upi_id == WP_PORT_UPI2)
      config.attributes |= WUFE_ATTRIB_CONNECT_OFFSET_F(1);
   if(ufe->upi_id == WP_PORT_UPI3)
      config.attributes |= WUFE_ATTRIB_CONNECT_OFFSET_F(2);

   /* Initialize the UFE*/
   ufe_status = WUFE_UfeInit(&config);
   WT_UfeTerminateOnError(ufe_status,"WUFE_UfeInit",0,__LINE__);
#if defined __WT_UFE412__ || defined __WT_UFE448__
// #if WTI_CESOP_REGRESSION_TEST
#if MORRIS_ENABLE_FREERUN_MODE
// #error ________FREERUN_MODE_IS_ON_________
   /* In regression mode. UFE4 is master on the sonet clock so setting zarlink to freerun */
   printf("\nzarlink set to free-run mode\n");
   WUFE_BSSetExtPllFreeRunMode(ufe->ufe_id);
   if (g_isForceOffset)
   {
      WUFE_BSSetExtPllFreeRunModeOffset(the_system->ufe.ufe_id,0x00700000);
   }


#endif
#endif

#ifdef __WT_UFE412__
   ufe_status = WUFE_DeviceTypeSet(ufe->ufe_id, WUFE_DEVICE_TYPE_UFE412_T4);
   WT_UfeTerminateOnError(ufe_status, "WUFE_DeviceTypeSet  ",0,__LINE__);
#else
#ifdef __WT_UFE448__
   ufe_status = WUFE_DeviceTypeSet(ufe->ufe_id, WUFE_DEVICE_TYPE_UFE448_T48);
   WT_UfeTerminateOnError(ufe_status, "WUFE_DeviceTypeSet  ",0,__LINE__);
#endif
#endif
   /************************************
    * Configure the UFE system.
    ************************************/
#if USE_UFE
   WT_UfeSystemConfig(ufe, &sys_cfg);

   ufe_status = WUFE_SystemConfig(ufe->ufe_id, &sys_cfg);
   WT_UfeTerminateOnError(ufe_status, "WUFE_SystemConfig  ",0,__LINE__);
#endif

#ifdef WT_UFE_FRAMER

#if !defined(__linux__)

   WPL_IntDisable(WP_WINPATH(0), WPL_IntGlobalState);
   WPL_InterruptConfigureEint3(WP_WINPATH(0));
   WPL_IntEnable(WP_WINPATH(0), WPL_IntGlobalState);

#endif

   /* Connect handler for UFE4 interrupts and supply signal handler */
   WPL_IntConnect(WP_WINPATH(0), WPL_Eint3Ufe4, 0, WT_Eint3Interrupt);
   WPL_IntEnable(WP_WINPATH(0), WPL_Eint3Ufe4);

   /* Select EINT3 for interrupts */
   WPX_Ufe412CpldInterruptSelect(0, WPX_INTERRUPT_EINT3);

#endif
}

void  WT_UfeSystemEnable(WT_ufe *ufe)
{
   WUFE_status ufe_status;
   WP_U32 property, i;

   /* Enable UFE system */
   property = WUFE_SYS_EMPHY;

#if USE_UFE
   ufe_status = WUFE_SystemEnable(ufe->ufe_id, property, WUFE_FULL_DUPLEX);
   WT_UfeTerminateOnError(ufe_status, "WUFE_SystemEnable ",0,__LINE__);
#endif

   for (i=0; i<MAX_UFE_LINES_USED; i++)
      ufe->line_handle[i] = WT_UFE_HANDLE_INVALID;

   for (i=0; i<N_MAX_EMPHY_PHYS; i++)
      ufe->phy_handle[i] = WT_UFE_HANDLE_INVALID;

#if USE_UFE
   ufe_status = WUFE_UfeFramerHwInterruptEnable(ufe->ufe_id);
   WT_UfeTerminateOnError(ufe_status, "WUFE_UfeFramerHwInterruptEnable",0,__LINE__);

   ufe_status = WUFE_UfeCoreHwInterruptEnable(ufe->ufe_id);
   WT_UfeTerminateOnError(ufe_status, "WUFE_UfeCoreHwInterruptEnable",0,__LINE__);

   ufe_status = WUFE_SystemInterruptEnable(ufe->ufe_id);
   WT_UfeTerminateOnError(ufe_status, "WUFE_SystemInterruptEnable ",0,__LINE__);

   ufe_status = WUFE_UfeCoreHwInterruptDisable(ufe->ufe_id);
   WT_UfeTerminateOnError(ufe_status, "WUFE_UfeCoreHwInterruptDisable",0,__LINE__);
#endif
}

void  WT_UfeSystemRxTrafficMask(WT_ufe *ufe, WP_U8 aug1, WP_U8 enable)
{
   WUFE_status ufe_status;
   
   ufe_status = WUFE_SystemRxTrafficMask(ufe->ufe_id, aug1, enable);
   WT_UfeTerminateOnError(ufe_status, "WUFE_SystemRxTrafficMask", 0,__LINE__);
}

/**********************************************************************************
 **********************************************************************************
 ************                          LINE                   *********************
 **********************************************************************************
 *********************************************************************************/
void WT_UfeLineCreate(WT_ufe *ufe, WP_U32 index, void *config, WP_U8 enable)
{
   if (ufe->fpga_mode == WT_SONET)
   {
      /* create the UFE line */
      WT_UfeLineSonetCreate(ufe, index, (WT_ufe_line_sonet_params*) config, enable);

#ifdef WT_UFE_FRAMER
      {
         WT_ufe_line_sonet_params *line_params = (WT_ufe_line_sonet_params *) config;
         int client_port, line_port;
         WTI_flexmux_connection_type con_type =0;

         client_port = WTI_FlexmuxClientPortIndexGet(index, line_params->transfer_type);
         line_port = WTI_FlexmuxLinePortIndexGet(index, ufe->ufe_id);
         if (line_port == -1)
         {
            printf("Illegal line port index %d\n",index);
            exit(1);
         }

         switch (line_params->transfer_type)
         {
            case (WUFE_SONET_TYPE_E1):
               {
                  con_type = (line_params->framed == WUFE_FRAMED) ? WT_FLEXMUX_CONNECTION_TYPE_E1_FRAMED : WT_FLEXMUX_CONNECTION_TYPE_E1_UNFRAMED;
                  break;
               }
            case (WUFE_SONET_TYPE_T1):
                  con_type = (line_params->framed == WUFE_FRAMED) ? WT_FLEXMUX_CONNECTION_TYPE_T1_FRAMED : WT_FLEXMUX_CONNECTION_TYPE_T1_UNFRAMED;
                  break;
            case (WUFE_SONET_TYPE_STS3):
               con_type = WT_FLEXMUX_CONNECTION_TYPE_STS3;
               break;
            case (WUFE_SONET_TYPE_STS12):
               con_type = WT_FLEXMUX_CONNECTION_TYPE_STS12;
               break;
            case (WUFE_SONET_TYPE_E3):
            case (WUFE_SONET_TYPE_DS3):
               con_type = line_params->ds3_e3_framing_type;
               break;
            case WUFE_SDH_TYPE_VC44C:
               con_type = WT_FLEXMUX_CONNECTION_TYPE_VC4_4C;               
               break;               
               case WUFE_SDH_TYPE_VC4:
               con_type = WT_FLEXMUX_CONNECTION_TYPE_VC4;               
               break; 
            default:
               printf("Unsupported transfer_type %d\n", line_params->transfer_type);
               exit(1);
         }

         /* save the UFE line configuration structure */
         memcpy(&ufe->line_params[index].framer_line_params.line_sonet_params, line_params, sizeof(WT_ufe_line_sonet_params));
         ufe->line_params[index].transfer_type = line_params->transfer_type;

#if !WTI_FRAMER_SCRIPT
      /* create connection on the Flexmux framer */
         WTI_FlexmuxConnectionCreate(ufe->ufe_id,
                                     line_port,
                                     client_port,
                                     con_type,
                                     config);
#endif
      }
#endif
   }
   else if (ufe->fpga_mode == WT_SDH)
   {
      /* create the UFE line */
      WT_UfeLineSdhCreate(ufe, index, (WT_ufe_line_sdh_params*) config, enable);
#ifdef WT_UFE_FRAMER
#if !WTI_FRAMER_SCRIPT
      {
         WT_ufe_line_sdh_params *line_params = (WT_ufe_line_sdh_params *) config;
         int client_port, line_port;
         WTI_flexmux_connection_type con_type =0;

         client_port = WTI_FlexmuxClientPortIndexGet(index, line_params->transfer_type);
         line_port = WTI_FlexmuxLinePortIndexGet(index, ufe->ufe_id);
         if (line_port == -1)
         {
            printf("Illegal line port index %d\n",index);
            exit(1);
         }

         switch (line_params->transfer_type)
         {
            case (WUFE_SDH_TYPE_E1):
               {
                  con_type = (line_params->framed == WUFE_FRAMED) ? WT_FLEXMUX_CONNECTION_TYPE_E1_FRAMED : WT_FLEXMUX_CONNECTION_TYPE_E1_UNFRAMED;
                  break;
               }
            case (WUFE_SDH_TYPE_T1):
               {
                  con_type = (line_params->framed == WUFE_FRAMED) ? WT_FLEXMUX_CONNECTION_TYPE_T1_FRAMED : WT_FLEXMUX_CONNECTION_TYPE_T1_UNFRAMED;
                  break;
               }
            case (WUFE_SDH_TYPE_VC4):
               con_type = WT_FLEXMUX_CONNECTION_TYPE_VC4;
               printf("in WUFE_SDH_TYPE_VC4 line\n");
               
               break;
            case (WUFE_SDH_TYPE_VC44C):
               con_type = WT_FLEXMUX_CONNECTION_TYPE_VC4_4C;
               break;
            case (WUFE_SDH_TYPE_E3):
            case (WUFE_SDH_TYPE_DS3):
               con_type = line_params->ds3_e3_framing_type;
               break;
            default:
               printf("Unsupported transfer_type %d\n", line_params->transfer_type);
               exit(1);
         }
         
         /* save the UFE line configuration structure */
         memcpy(&ufe->line_params[index].framer_line_params.line_sdh_params, line_params, sizeof(WT_ufe_line_sdh_params));
         /* create connection on the Flexmux framer */

         WTI_FlexmuxConnectionCreate(ufe->ufe_id,
                                     line_port,
                                     client_port,
                                     con_type,
                                     config);
      }
#endif
#endif
   }
   else if (ufe->fpga_mode == WT_OCTAL)
      WT_UfeLineOctalCreate(ufe, index, (WT_ufe_line_octal_params*) config, enable);
   else
   {
      printf("The UFE FPGA mode is not supported %d\n", ufe->fpga_mode);
      exit(1);
   }
}

void WT_UfeClockRecLineConfigure(WUFE_line_clock_rec_params *line_cr_params, WP_U32 tx_cr_enable, WP_U32 physical_clock_enable, WP_U32 physical_clock_id)
{
   line_cr_params->tx_clk_rec_enable = tx_cr_enable;
   line_cr_params->clk_rec_physical_clock_enable = physical_clock_enable;
   line_cr_params->clk_rec_physical_clock_id = physical_clock_id;
}

void WT_UfeLineConfigureAndCreateT1(WT_ufe *ufe, WP_U32 i, WP_U16 transfer_type, WP_U16 framing_mode, WP_U8 cas_enable, WP_U8 enable, WUFE_line_clock_rec_params *line_cr_params)
{
   if (ufe->fpga_mode == WT_SONET)
   {
      WT_ufe_line_sonet_params line_params;

      memset(&line_params, 0, sizeof(WT_ufe_line_sonet_params));

      line_params.sts12 = (i / 336);
      line_params.sts3 = (i % 336) / 84;
      line_params.sts1 = (i % 84) / 28;
      line_params.vt_group = (i % 28) / 4;
      line_params.vt = (i % 4);
      line_params.transfer_type = transfer_type;
      line_params.framed = framing_mode;
      line_params.cas_enable = cas_enable;
      if (line_cr_params != NULL)
         line_params.clock_rec_line_params = line_cr_params;

      WT_UfeLineCreate(ufe, i, &line_params, enable);
   }
   else if (ufe->fpga_mode == WT_SDH)
   {
      WT_ufe_line_sdh_params line_params;

      memset(&line_params, 0, sizeof(WT_ufe_line_sdh_params));

      line_params.stm4 = (i / 336);
      line_params.stm1 = (i % 336) / 84;
      line_params.stm0 = (i % 84) / 28;
      line_params.tug2 = (i % 28) / 4;
      line_params.tu = (i % 4);
      line_params.transfer_type = transfer_type;
      line_params.framed = framing_mode;
      line_params.cas_enable = cas_enable;
      if (line_cr_params != NULL)
         line_params.clock_rec_line_params = line_cr_params;

      WT_UfeLineCreate(ufe, i, &line_params, enable);
   }
   else
   {/*CAD*/
      WT_ufe_line_octal_params line_params;

      memset(&line_params, 0, sizeof(WT_ufe_line_octal_params));

      line_params.line_id = i;
      line_params.transfer_type = transfer_type;
      line_params.framed = framing_mode;
      line_params.cas_enable = cas_enable;
      if (line_cr_params != NULL)
         line_params.clock_rec_line_params = line_cr_params;

      WT_UfeLineCreate(ufe, i, &line_params, enable);
   }
}

void WT_UfeLineConfigureAndCreateE3DS3(WT_ufe *ufe, WP_U32 i, WP_U16 transfer_type, WP_U16 framing_mode, WP_U8 ds3_e3_framing_type, WP_U8 cas_enable, WP_U8 enable, WUFE_line_clock_rec_params *line_cr_params)
{
   if (ufe->fpga_mode == WT_SONET)
   {
      WT_ufe_line_sonet_params line_params;

      memset(&line_params, 0, sizeof(WT_ufe_line_sonet_params));

      line_params.sts12 = (i / 336);
      line_params.sts3 = (i % 336) / 84;
      line_params.sts1 = (i % 84) /28;
      line_params.vt_group = 0;
      line_params.vt = 0;
      line_params.transfer_type = transfer_type;
      line_params.framed = framing_mode;
      line_params.cas_enable = cas_enable;
      line_params.ds3_e3_framing_type = ds3_e3_framing_type;

      if (line_cr_params != NULL)
         line_params.clock_rec_line_params = line_cr_params;

      WT_UfeLineCreate(ufe, i, &line_params, enable);
   }
   else if (ufe->fpga_mode == WT_SDH)
   {
      WT_ufe_line_sdh_params line_params;

      memset(&line_params, 0, sizeof(WT_ufe_line_sdh_params));

      line_params.stm4 = (i / 336);
      line_params.stm1 = (i % 336) / 84;
      line_params.stm0 = (i % 84) /28;
      line_params.tug2 = 0;
      line_params.tu = 0;
      line_params.transfer_type = transfer_type;
      line_params.framed = framing_mode;
      line_params.cas_enable = cas_enable;
      line_params.ds3_e3_framing_type = ds3_e3_framing_type;

      if (line_cr_params != NULL)
         line_params.clock_rec_line_params = line_cr_params;


      WT_UfeLineCreate(ufe, i, &line_params, enable);
   }
}

void WT_UfeLineConfigureAndCreateClearChannel(WT_ufe *ufe, WP_U32 i, WP_U16 transfer_type, WP_U16 framing_mode, WP_U8 cas_enable, WP_U8 enable, WUFE_line_clock_rec_params *line_cr_params)
{
   if (ufe->fpga_mode == WT_SONET)
   {
      WT_ufe_line_sonet_params line_params;

      memset(&line_params, 0, sizeof(WT_ufe_line_sonet_params));

      line_params.sts12 = i / 336;
      line_params.sts3 = (i % 336) / 84;
      line_params.sts1 = 0;
      line_params.vt_group = 0;
      line_params.vt = 0;
      line_params.transfer_type = transfer_type;
      line_params.framed = framing_mode;
      line_params.cas_enable = WUFE_CAS_DISABLE;
      if (line_cr_params != NULL)
         line_params.clock_rec_line_params = line_cr_params;

      WT_UfeLineCreate(ufe, i, &line_params, enable);
   }
   else if (ufe->fpga_mode == WT_SDH)
   {
      WT_ufe_line_sdh_params line_params;

      memset(&line_params, 0, sizeof(WT_ufe_line_sdh_params));

      line_params.stm4 = i / 336;
      line_params.stm1 = (i % 336) / 84;
      line_params.stm0 = 0;
      line_params.tug2 = 0;
      line_params.tu = 0;
      line_params.transfer_type = transfer_type;
      line_params.framed = framing_mode;
      line_params.cas_enable = WUFE_CAS_DISABLE;
      if (line_cr_params != NULL)
         line_params.clock_rec_line_params = line_cr_params;

      WT_UfeLineCreate(ufe, i, &line_params, enable);
   }
}

void WT_UfeLineConfigureAndCreateE1(WT_ufe *ufe, WP_U32 i, WP_U16 transfer_type, WP_U16 framing_mode, WP_U8 cas_enable, WP_U8 enable, WUFE_line_clock_rec_params *line_cr_params)
{
   if (ufe->fpga_mode == WT_SONET)
   {
      WT_ufe_line_sonet_params line_params;

      memset(&line_params, 0, sizeof(WT_ufe_line_sonet_params));

      line_params.sts12 = i / 336;
      line_params.sts3 = (i % 336) / 84;
      line_params.sts1 = (i % 84) / 28;
      line_params.vt_group = (i % 28) / 3;
      line_params.vt = ((i % 28) % 3);

      line_params.transfer_type = transfer_type;
      line_params.framed = framing_mode;
      line_params.cas_enable = cas_enable;
      if (line_cr_params != NULL)
         line_params.clock_rec_line_params = line_cr_params;

      WT_UfeLineCreate(ufe, i, &line_params, enable);
   }
   else if (ufe->fpga_mode == WT_SDH)
   {
      WT_ufe_line_sdh_params line_params;

      memset(&line_params, 0, sizeof(WT_ufe_line_sdh_params));

      line_params.stm4 = (i / 336);
      line_params.stm1 = (i % 336) / 84;
      line_params.stm0 = (i % 84) / 28;
      line_params.tug2 = (i % 28) / 3;
      line_params.tu = ((i % 28) % 3);
      line_params.transfer_type = transfer_type;
      line_params.framed = framing_mode;
      line_params.cas_enable = cas_enable;
      if (line_cr_params != NULL)
         line_params.clock_rec_line_params = line_cr_params;

      WT_UfeLineCreate(ufe, i, &line_params, enable);
   }
   else
   {/*CAD*/
      WT_ufe_line_octal_params line_params;

      memset(&line_params, 0, sizeof(WT_ufe_line_octal_params));

      line_params.line_id = i;
      line_params.transfer_type = transfer_type;
      line_params.framed = framing_mode;
      line_params.cas_enable = cas_enable;
      if (line_cr_params != NULL)
         line_params.clock_rec_line_params = line_cr_params;

      WT_UfeLineCreate(ufe, i, &line_params, enable);
   }
}

void WT_UfeLineConfigureAndCreateE1T1(WT_ufe *ufe, WP_U32 i, WP_U16 transfer_type, WP_U16 framing_mode, WP_U8 cas_enable, WP_U8 enable, WUFE_line_clock_rec_params *line_cr_params)
{
   static WP_U32 index = 0;
   if (ufe->fpga_mode == WT_SONET)
   {
      WT_ufe_line_sonet_params line_params;

      memset(&line_params, 0, sizeof(WT_ufe_line_sonet_params));

      line_params.sts12 = index / 336;
      line_params.sts3 = (index % 336) / 84;
      line_params.sts1 = (index % 84) / 28;
      line_params.vt_group = (index % 28) / 4;
      line_params.vt = (index % 4);
      line_params.transfer_type = transfer_type;
      line_params.framed = framing_mode;
      line_params.cas_enable = cas_enable;
      if (line_cr_params != NULL)
         line_params.clock_rec_line_params = line_cr_params;
      index++;
      if(index%4 == 3){
         if(WUFE_SONET_TYPE_E1 == transfer_type)
            index++;
      }
      WT_UfeLineCreate(ufe, i, &line_params, enable);
   }
   else if (ufe->fpga_mode == WT_SDH)
   {
      WT_ufe_line_sdh_params line_params;

      memset(&line_params, 0, sizeof(WT_ufe_line_sdh_params));

      line_params.stm4 = (index / 336);
      line_params.stm1 = (index % 336) / 84;
      line_params.stm0 = (index % 84) / 28;
      line_params.tug2 = (index % 28) / 4;
      line_params.tu = (index % 4);
      line_params.transfer_type = transfer_type;
      line_params.framed = framing_mode;
      line_params.cas_enable = cas_enable;
      if (line_cr_params != NULL)
         line_params.clock_rec_line_params = line_cr_params;
      index++;
      if(index%4 == 3){
         if(WUFE_SDH_TYPE_E1 == transfer_type)
            index++;
      }
      WT_UfeLineCreate(ufe, i, &line_params, enable);
   }
   else
   {/*CAD*/
      WT_ufe_line_octal_params line_params;

      memset(&line_params, 0, sizeof(WT_ufe_line_octal_params));

      line_params.line_id = i;
      line_params.transfer_type = transfer_type;
      line_params.framed = framing_mode;
      line_params.cas_enable = cas_enable;
      if (line_cr_params != NULL)
         line_params.clock_rec_line_params = line_cr_params;

      WT_UfeLineCreate(ufe, i, &line_params, enable);
   }

}


void WT_UfeLineDelete(WT_ufe *ufe, WP_U32 index)
{
   WUFE_status ufe_status;
#ifdef WT_UFE_FRAMER
   int client_port;
#endif

   if(ufe->line_handle[index] == WT_UFE_HANDLE_INVALID)
      return;

   ufe_status = WUFE_LineDisable(ufe->line_handle[index], WUFE_FULL_DUPLEX);
   WT_UfeTerminateOnError(ufe_status, "WUFE_LineDisable", index, __LINE__);

   ufe_status = WUFE_LineDelete(ufe->line_handle[index]);
   WT_UfeTerminateOnError(ufe_status, "WUFE_LineDelete", index, __LINE__);

   ufe->line_handle[index] = WT_UFE_HANDLE_INVALID;


#ifdef WT_UFE_FRAMER
   /* delete connection on the Flexmux framer */
   if (ufe->fpga_mode == WT_SONET)
   {
      client_port = WTI_FlexmuxClientPortIndexGet(index, ufe->line_params[index].framer_line_params.line_sonet_params.transfer_type);
      WTI_FlexmuxConnectionDelete(ufe->ufe_id,
                                  client_port,
                                  &ufe->line_params[index].framer_line_params.line_sonet_params);
   }
   else if (ufe->fpga_mode == WT_SDH)
   {
      client_port = WTI_FlexmuxClientPortIndexGet(index, ufe->line_params[index].framer_line_params.line_sdh_params.transfer_type);
      WTI_FlexmuxConnectionDelete(ufe->ufe_id,
                                  client_port,
                                  &ufe->line_params[index].framer_line_params.line_sdh_params);
   }
   else
   {
      printf("The UFE FPGA mode is not supported %d\n", ufe->fpga_mode);
      exit(1);
   }

#endif
}

void WT_UfeLineProtectionSwitch(WP_U32 flexmux_id, WP_U32 line_port_id, WP_U32 protected_line_active)
{
   /* This function supports only 2xOC3+protection mode. When additional modes will be
      available, this function needs to be updated. */
#ifdef WT_UFE_FRAMER
   if (protected_line_active)
      WTI_FlexmuxForceB(flexmux_id, line_port_id);
   else
      WTI_FlexmuxForceA(flexmux_id, line_port_id);
#endif /* WT_UFE_FRAMER */
}

/**********************************************************************************
 **********************************************************************************
 ************                    Line Octal                   *********************
 **********************************************************************************
 *********************************************************************************/
static void WT_UfeLineOctalConfig(WT_ufe_line_octal_params *line_octal_params,
                                   WUFE_line_cad *line_cfg)
{
   memset(line_cfg, 0, sizeof(WUFE_line_cad));

   line_cfg->struct_id = WUFE_STRUCT_LINE_CAD;
   line_cfg->cad_id  = line_octal_params->line_id;

   line_cfg->transfer_type = line_octal_params->transfer_type;
   line_cfg->framed = line_octal_params->framed;
   line_cfg->cas_enable = line_octal_params->cas_enable;

   if (line_cfg->cas_enable == WUFE_CAS_ENABLE)
      line_cfg->multi_frame = WUFE_MULTI_FRAME;
   else
      line_cfg->multi_frame = WUFE_SINGLE_FRAME;
   line_cfg->multi_frame_sync_level = WUFE_MULTI_F_SYNC_HIGH_START_OF_FRAME;

   if ((line_cfg->transfer_type == WUFE_CAD_TYPE_T1 &&
        line_cfg->framed == WUFE_FRAMED) ||
       (line_cfg->transfer_type == WUFE_CAD_TYPE_HMVIP_4_T1))
      line_cfg->multi_frame_num= WUFE_MULTI_FRAME_24;
   else
      line_cfg->multi_frame_num= WUFE_MULTI_FRAME_16;

   line_cfg->cas_alignment_location = WUFE_CAS_LOWER_NIBBLE;
//   line_cfg->clock_rec_mode = line_octal_params->clock_rec_mode;
//   line_cfg->rx_clk_rec_if = line_octal_params->rx_clk_rec_if;
//   line_cfg->ext_mode_active = line_octal_params->ext_mode_active;
   line_cfg->cad_loopback = WUFE_CAD_NORMAL;/*WUFE_CAD_NORMAL or WUFE_CAD_LOOPBACK_SYSTEM or WUFE_CAD_LOOPBACK_LINE*/


   /* Copy clock recovery line parameters */
   if (line_octal_params->clock_rec_line_params != NULL)
   {
      WUFE_line_clock_rec_params *line_cr_params = line_octal_params->clock_rec_line_params;
      if (line_cr_params->tx_clk_rec_enable !=0 && line_cr_params->tx_clk_rec_enable != 1)
      {
         printf("Invalid TX Clock Recovery mode\n");
         exit(1);
      }
      if (line_cr_params->clk_rec_physical_clock_enable != 0 && line_cr_params->clk_rec_physical_clock_enable != 1)
      {
         printf("Invalid Physical clock mode\n");
         exit(1);
      }
      if (line_cr_params->clk_rec_physical_clock_id < 0 || line_cr_params->clk_rec_physical_clock_id > 1)
      {
         printf("Invalid Physical clock ID, must be '0' or '1'\n");
         exit(1);
      }
      line_cfg->clock_rec_line_params = line_cr_params;
   }
}

void WT_UfeLineOctalCreate(WT_ufe *ufe,
                           WP_U32 line_index,
                           WT_ufe_line_octal_params *line_octal_params,
                           WP_U8 enable)
{
   WUFE_status ufe_status;
   WUFE_line_cad line_cfg;

   if (ufe->fpga_mode != WT_OCTAL)
   {
      printf("The UFE FPGA mode is not OCTAL. Can't create octal line\n");
      exit(1);
   }

   /* Set the line configuration parameters */
   WT_UfeLineOctalConfig(line_octal_params, &line_cfg);

#if USE_UFE
   /* Create the line */
   ufe_status = WUFE_LineCreate(&ufe->line_handle[line_index],
                                ufe->ufe_id,
                                WUFE_STRUCT_LINE_CAD,
                                &line_cfg);
   WT_UfeTerminateOnError(ufe_status, "Octal WUFE_LineCreate", line_index ,__LINE__);

   /* Enable the line */
   if (enable)
   {
      ufe_status = WUFE_LineEnable(ufe->line_handle[line_index], WUFE_FULL_DUPLEX);
      WT_UfeTerminateOnError(ufe_status, "WUFE_LineEnable ", line_index, __LINE__);
   }
#endif
}

/**********************************************************************************
 **********************************************************************************
 ************                    Line Sonet                   *********************
 **********************************************************************************
 *********************************************************************************/
static void WT_UfeLineSonetConfig(WT_ufe_line_sonet_params *line_sonet_params,
                                  WUFE_line_sonet *line_cfg)
{
   memset(line_cfg, 0, sizeof(WUFE_line_sonet));

   line_cfg->struct_id = WUFE_STRUCT_LINE_SONET;

   if (line_sonet_params->sts12 > 3)
   {
      printf("Invalid STS12 parameter, use 0-3 only \n");
      exit(1);
   }
   else line_cfg->sts12 = line_sonet_params->sts12;

   if (line_sonet_params->sts3 > 3)
   {
      printf("Invalid STS3 parameter, use 0-3 only \n");
      exit(1);
   }
   else line_cfg->sts3 = line_sonet_params->sts3;

   if (line_sonet_params->sts1 > 2)
   {
      printf("Invalid STS1 parameter, use 0-2 only \n");
      exit(1);
   }
   else line_cfg->sts1 = line_sonet_params->sts1;

   if (line_sonet_params->vt_group > 6)
   {
      printf("Invalid Vt_Group parameter,use 0-6 only \n");
      exit(1);
   }
   else line_cfg->vt_group = line_sonet_params->vt_group;

   if (line_sonet_params->vt > 3)
   {
      printf("Invalid Vt_Group parameter,use 0-6 only \n");
      exit(1);
   }
   else if ((line_sonet_params->vt == 3)&&
            ((line_sonet_params->transfer_type == WUFE_SONET_TYPE_E1) ||
             (line_sonet_params->transfer_type == WUFE_SONET_TYPE_VT2)))
   {
      printf("Invalid Vt_Group parameter, slot 3 not valid for E1 or VT2\n");
      exit(1);
   }
   else line_cfg->vt = line_sonet_params->vt;

   line_cfg->transfer_type = line_sonet_params->transfer_type;
   line_cfg->framed = line_sonet_params->framed;
   line_cfg->cas_enable = line_sonet_params->cas_enable;

   /* Copy clock recovery line parameters */
   if (line_sonet_params->clock_rec_line_params != NULL)
   {
      WUFE_line_clock_rec_params *line_cr_params = line_sonet_params->clock_rec_line_params;
      if (line_cr_params->tx_clk_rec_enable !=0 && line_cr_params->tx_clk_rec_enable != 1)
      {
         printf("Invalid TX Clock Recovery mode\n");
         exit(1);
      }
      if (line_cr_params->clk_rec_physical_clock_enable != 0 && line_cr_params->clk_rec_physical_clock_enable != 1)
      {
         printf("Invalid Physical clock mode\n");
         exit(1);
      }
      if (line_cr_params->clk_rec_physical_clock_id < 0 || line_cr_params->clk_rec_physical_clock_id > 1)
      {
         printf("Invalid Physical clock ID, must be '0' or '1'\n");
         exit(1);
      }
      line_cfg->clock_rec_line_params = line_cr_params;
   }
}

static void WT_UfeLineSdhConfig(WT_ufe_line_sdh_params *line_sdh_params,
                                  WUFE_line_sdh *line_cfg)
{
   memset(line_cfg, 0, sizeof(WUFE_line_sdh));

   line_cfg->struct_id = WUFE_STRUCT_LINE_SDH;
   if (line_sdh_params->stm4 > 3)
   {
      printf("Invalid stm4 parameter, use 0-3 only \n");
      exit(1);
   }
   else line_cfg->stm4 = line_sdh_params->stm4;

   if (line_sdh_params->stm1 > 3)
   {
      printf("Invalid stm1 parameter, use 0-3 only \n");
      exit(1);
   }
   else line_cfg->stm1 = line_sdh_params->stm1;

   if (line_sdh_params->stm0 > 2)
   {
      printf("Invalid stm0 parameter, use 0-2 only \n");
      exit(1);
   }
   else line_cfg->stm0 = line_sdh_params->stm0;

   if (line_sdh_params->tug2 > 6)
   {
      printf("Invalid tug2 parameter,use 0-6 only \n");
      exit(1);
   }
   else line_cfg->tug2 = line_sdh_params->tug2;

   if (line_sdh_params->tu > 3)
   {
      printf("Invalid tu parameter,use 0-3 only \n");
      exit(1);
   }
   else if ((line_sdh_params->tu == 3) &&
            ((line_sdh_params->transfer_type == WUFE_SDH_TYPE_E1) ||
             (line_sdh_params->transfer_type == WUFE_SDH_TYPE_VC12)))
   {
      printf("Invalid tu parameter, slot 3 not valid for E1 or VC12\n");
      exit(1);
   }
   else
      line_cfg->tu = line_sdh_params->tu;

   line_cfg->transfer_type = line_sdh_params->transfer_type;
   line_cfg->framed = line_sdh_params->framed;
   line_cfg->cas_enable = line_sdh_params->cas_enable;

   /* Copy clock recovery line parameters */
   if (line_sdh_params->clock_rec_line_params != NULL)
   {
      WUFE_line_clock_rec_params *line_cr_params = line_sdh_params->clock_rec_line_params;
      if (line_cr_params->tx_clk_rec_enable !=0 && line_cr_params->tx_clk_rec_enable != 1)
      {
         printf("Invalid TX Clock Recovery mode\n");
         exit(1);
      }
      if (line_cr_params->clk_rec_physical_clock_enable != 0 && line_cr_params->clk_rec_physical_clock_enable != 1)
      {
         printf("Invalid Physical clock mode\n");
         exit(1);
      }
      if (line_cr_params->clk_rec_physical_clock_id < 0 || line_cr_params->clk_rec_physical_clock_id > 1)
      {
         printf("Invalid Physical clock ID, must be '0' or '1'\n");
         exit(1);
      }
      line_cfg->clock_rec_line_params = line_cr_params;
   }
}

void  WT_UfeLineSonetCreate(WT_ufe *ufe,
                             WP_U32 line_index,
                             WT_ufe_line_sonet_params *line_sonet_params,
                             WP_U8 enable)
{
   WUFE_status ufe_status;
   WUFE_line_sonet line_cfg;

   if (ufe->fpga_mode != WT_SONET)
   {
      printf("The UFE FPGA mode is not SONET. Can't create sonet line\n");
      exit(1);
   }

   /* Set the line configuration parameters */
   WT_UfeLineSonetConfig(line_sonet_params, &line_cfg);

#if USE_UFE
   /* Create the line */
   ufe_status = WUFE_LineCreate(&ufe->line_handle[line_index],
                                ufe->ufe_id,
                                WUFE_STRUCT_LINE_SONET,
                                &line_cfg);
   WT_UfeTerminateOnError(ufe_status, "Sonet WUFE_LineCreate", line_index ,__LINE__);

   /* Enable the line */
   if (enable)
   {
      ufe_status = WUFE_LineEnable(ufe->line_handle[line_index], WUFE_FULL_DUPLEX);
      WT_UfeTerminateOnError(ufe_status, "WUFE_LineEnable ", line_index, __LINE__);
   }
#endif
}
void WT_UfeLineSdhCreate(WT_ufe *ufe,
                         WP_U32 line_index,
                         WT_ufe_line_sdh_params *line_sdh_params,
                         WP_U8 enable)
{
   WUFE_status ufe_status;
   WUFE_line_sdh line_cfg;

   if (ufe->fpga_mode != WT_SDH)
   {
      printf("The UFE FPGA mode is not SDH. Can't create sdh line\n");
      exit(1);
   }

   /* Set the line configuration parameters */
   WT_UfeLineSdhConfig(line_sdh_params, &line_cfg);

#if USE_UFE
   ufe_status = WUFE_LineCreate(&ufe->line_handle[line_index],
                                ufe->ufe_id,
                                WUFE_STRUCT_LINE_SDH,
                                &line_cfg);
   WT_UfeTerminateOnError(ufe_status, "SDH WUFE_LineCreate", line_index, __LINE__);

   if (enable)
   {
      ufe_status = WUFE_LineEnable(ufe->line_handle[line_index], WUFE_FULL_DUPLEX);
      WT_UfeTerminateOnError(ufe_status, "SDH WUFE_LineEnable", line_index, __LINE__);
   }
#endif
}


void WT_UfeLineDisable(WT_ufe *ufe, WP_U32 line_index)
{
   WUFE_status ufe_status;

   ufe_status = WUFE_LineDisable(ufe->line_handle[line_index],WUFE_FULL_DUPLEX);
   WT_UfeTerminateOnError(ufe_status, "WUFE_LineDisable ", line_index, __LINE__);
}

void  WT_UfeLineEnable(WT_ufe *ufe, WP_U32 line_index)
{
   WUFE_status ufe_status;

   ufe_status = WUFE_LineEnable(ufe->line_handle[line_index],WUFE_FULL_DUPLEX);
   WT_UfeTerminateOnError(ufe_status, "WUFE_LineEnable ", line_index, __LINE__);
}


/**********************************************************************************
 **********************************************************************************
 ************                    PHYS                         *********************
 **********************************************************************************/
static void WT_UfePhyTransConfig(WT_ufe *ufe,
                                 WUFE_phy *phy_cfg,
                                 WP_U32 data_unit_size,
                                 WP_U32 n_slots,
                                 WP_U8 *slots_arr)
{
   WUFE_SlotGroup slot_group;

   /************************************
    * Configure the UFE phy .
    ************************************/
   memset(phy_cfg, 0, sizeof(WUFE_phy));

   phy_cfg->struct_id = WUFE_STRUCT_PHY_TRANS;
   phy_cfg->tx_fifo_param.transmit_th = 0x1;
   phy_cfg->tx_fifo_param.fast_mode = WUFE_PHY_FIFO_MODE_SLOW;
   phy_cfg->tx_fifo_param.wait_type = WUFE_PHY_TX_WAIT_TYPE_A;

   /* configure the slots */
   memset (&slot_group, 0, sizeof(WUFE_SlotGroup));
   slot_group.n_slots = n_slots;
   memcpy(slot_group.slot_arr, slots_arr, n_slots*sizeof(WP_U8));

   memcpy(&phy_cfg->rx_slot_group,&slot_group,sizeof(slot_group));
   memcpy(&phy_cfg->tx_slot_group,&slot_group,sizeof(slot_group));

   phy_cfg->type.trans.rx_data_unit_size = data_unit_size;
}

void WT_UfePhyTransCreate(WT_ufe *ufe,
                          WP_U32 line_index,
                          WP_U32 phy_index,
                          WP_U32 n_slots,
                          WP_U8 *slots_arr,
                          WP_U32 ufe_dataunit_size,
                          WP_U8 rx_clk_rec_mode)
{
   WUFE_phy phy_cfg;
   WUFE_status ufe_status;

   WT_UfePhyTransConfig(ufe, &phy_cfg, ufe_dataunit_size, n_slots, slots_arr);

#if USE_UFE
   ufe_status = WUFE_PhyCreate(&ufe->phy_handle[phy_index],
                               ufe->line_handle[line_index],
                               &phy_cfg,
                               WUFE_FULL_DUPLEX);
   WT_UfeTerminateOnError(ufe_status, "WUFE_PhyCreate", phy_index, __LINE__);
#endif
}

void WT_UfePhyEnable(WT_ufe *ufe, WP_U32 phy_index)
{
   WUFE_status ufe_status;

#if USE_UFE
   ufe_status = WUFE_PhyEnable(ufe->phy_handle[phy_index], WUFE_FULL_DUPLEX);
   WT_UfeTerminateOnError(ufe_status, "WUFE_PhyEnable", phy_index, __LINE__);
#endif
}

void WT_UfePhyDisplay(WT_ufe *ufe, WP_U32 phy_index)
{
   WUFE_status ufe_status;

#if USE_UFE
   ufe_status = WUFE_PhyDisplay(ufe->phy_handle[phy_index]);
   WT_UfeTerminateOnError(ufe_status, "WUFE_PhyDisable", phy_index, __LINE__);
#endif
}

void WT_UfePhyDisable(WT_ufe *ufe, WP_U32 phy_index)
{
   WUFE_status ufe_status;

   if(ufe->phy_handle[phy_index] == WT_UFE_HANDLE_INVALID)
      return;

   ufe_status = WUFE_PhyDisable(ufe->phy_handle[phy_index], WUFE_FULL_DUPLEX);
   WT_UfeTerminateOnError(ufe_status, "WUFE_PhyDisable", phy_index, __LINE__);

}

void WT_UfePhyDelete(WT_ufe *ufe, WP_U32 phy_index)
{
   WP_handle phy_handle = ufe->phy_handle[phy_index];
   WUFE_status ufe_status=WUFE_OK;
   volatile WP_U32 timeout;

   if(phy_handle == WT_UFE_HANDLE_INVALID)
      return;

   timeout = 0;
   do
   {
      ufe_status = WUFE_PhyDelete(phy_handle);
      if (ufe_status != WUFE_OK && ufe_status != WUFE_ERR_PHY_FIFO_NOT_EMPTY)
         break;

      if (++timeout > 100000)
      {
         printf("---------------- Timeout -------------\n");
         break;
      }

      WP_Delay(10);
   }
   while (ufe_status != WUFE_OK);

   if(ufe_status != WUFE_OK)
   {
      WUFE_PhyDisplay(phy_handle);
      WT_UfeTerminateOnError(ufe_status,"WUFE_PhyDelete not complete", timeout ,__LINE__);
   }

   ufe->phy_handle[phy_index] = WT_UFE_HANDLE_INVALID;
}


static void WT_UfeAnalyzeEmphyEvents(WUFE_events_emphy *emphy_events, WP_U32 coreid)
{
   WP_U16 event = emphy_events->event_reg;

   printf("Emphy event register for core id %d: 0x%04x\n", coreid, event);
   if (event & WUFE_EVENT_EMPHY)
   {
      if (event & WUFE_EVENT_EMPHY_INGRESS_BUS)
         printf("WUFE_EVENT_EMPHY_INGRESS_BUS\n");
      if (event & WUFE_EVENT_EMPHY_INGRESS_MAX_BUFF_SIZE)
         printf("WUFE_EVENT_EMPHY_INGRESS_MAX_BUFF_SIZE\n");
      if (event & WUFE_EVENT_EMPHY_INGRESS_IF)
         printf("WUFE_EVENT_EMPHY_INGRESS_IF\n");
      if (event & WUFE_EVENT_EMPHY_EGRESS_PHY_NOT_VALID)
      {
         printf("WUFE_EVENT_EMPHY_EGRESS_PHY_NOT_VALID\n");
         printf("phy_not_valid_handle = 0x%x\n",
                emphy_events->phy_not_valid_handle);
      }
      if (event & WUFE_EVENT_EMPHY_EGRESS_BUS)
         printf("WUFE_EVENT_EMPHY_EGRESS_BUS\n");
      if (event & WUFE_EVENT_EMPHY_EGRESS_IF )
         printf("WUFE_EVENT_EMPHY_EGRESS_IF\n");
      if (event &WUFE_EVENT_EMPHY_EGRESS_PARITY )
         printf("WUFE_EVENT_EMPHY_EGRESS_PARITY\n");
   }
}

static void WT_UfeAnalyzeTdmEvents(WUFE_events_tdm *tdm_events)
{
   WP_U16 event = tdm_events->event_reg;

   printf("TDM event register 0x%04x\n", event);
   if (event & WUFE_EVENT_TDM)
   {
      if (event & WUFE_EVENT_TDM_RX_PARITY)
         printf("WUFE_EVENT_TDM_RX_PARITY\n");
      if (event & WUFE_EVENT_TDM_TX_PARITY)
         printf("WUFE_EVENT_TDM_TX_PARITY\n");
   }
}

static void WT_UfeAnalyzeFmEvents(WUFE_events_fm *fm_events)
{
   WP_U16 event = fm_events->event_reg;

   printf("FM event register 0x%04x\n", event);
   if (event & WUFE_EVENT_FM)
   {
      if (event & WUFE_EVENT_FM_RX_DDR_OVERRUN)
         printf("WUFE_EVENT_FM_RX_DDR_OVERRUN\n");
      if (event & WUFE_EVENT_FM_RX_BURST_OVERRUN)
         printf("WUFE_EVENT_FM_RX_BURST_OVERRUN\n");
      if (event & WUFE_EVENT_FM_TX_DDR_UNDERRUN)
         printf("WUFE_EVENT_FM_TX_DDR_UNDERRUN\n");
      if (event & WUFE_EVENT_FM_TX_BURST_UNDERRUN)
         printf("WUFE_EVENT_FM_TX_BURST_UNDERRUN\n");
      if (event & WUFE_EVENT_FM_RX_ECC_ONE_BIT)
         printf("WUFE_EVENT_FM_RX_ECC_ONE_BIT\n");
      if (event & WUFE_EVENT_FM_TX_ECC_ONE_BIT)
         printf("WUFE_EVENT_FM_TX_ECC_ONE_BIT\n");
      if (event & WUFE_EVENT_FM_TX_READ_FIFO_OVERRUN)
         printf("WUFE_EVENT_FM_TX_READ_FIFO_OVERRUN\n");
      if (event & WUFE_EVENT_FM_FATAL_ERROR)
         printf("WUFE_EVENT_FM_FATAL_ERROR\n");
   }
}

static void WT_UfeAnalyzeMachineEvents(WUFE_events_machine *machine_events)
{
   WP_U16 event = machine_events->rx_event_reg;

   printf("Machine event Rx register 0x%04x\n", event);
   if (event & WUFE_EVENT_MACHINE_RX)
   {
       if (event & WUFE_EVENT_MACHINE_CES_RX_LOSS)
          printf("WUFE_EVENT_MACHINE_CES_RX_LOSS\n");
       if (event & WUFE_EVENT_MACHINE_CES_RX_MF_LOSS)
          printf("WUFE_EVENT_MACHINE_CES_RX_MF_LOSS\n");
       if (event & WUFE_EVENT_MACHINE_ATM_RX_LCD)
          printf("WUFE_EVENT_MACHINE_ATM_RX_LCD\n");
       if (event & WUFE_EVENT_MACHINE_HDLC_RX_ABORT)
          printf("WUFE_EVENT_MACHINE_HDLC_RX_ABORT\n");
       if (event & WUFE_EVENT_MACHINE_HDLC_RX_CRC_ERR)
          printf("WUFE_EVENT_MACHINE_HDLC_RX_CRC_ERR\n");
       if (event & WUFE_EVENT_MACHINE_HDLC_RX_NON_OCTET)
          printf("WUFE_EVENT_MACHINE_HDLC_RX_NON_OCTET\n");
       if (event & WUFE_EVENT_MACHINE_POS_RX_ABORT)
          printf("WUFE_EVENT_MACHINE_POS_RX_ABORT\n");
       if (event & WUFE_EVENT_MACHINE_POS_CRC_ERR)
          printf("WUFE_EVENT_MACHINE_POS_CRC_ERR\n");
       if (event & WUFE_EVENT_MACHINE_RX_OVERRUN_BURST)
          printf("WUFE_EVENT_MACHINE_RX_OVERRUN_BURST\n");
       if (event & WUFE_EVENT_MACHINE_RX_OVERRUN_DDR)
          printf("WUFE_EVENT_MACHINE_RX_OVERRUN_DDR\n");
   }

   event = machine_events->tx_event_reg;

   printf("Machine event Tx register 0x%04x\n", event);
   if (event & WUFE_EVENT_MACHINE_TX)
   {
       if (event & WUFE_EVENT_MACHINE_CES_TX_LOSS)
          printf("WUFE_EVENT_MACHINE_CES_TX_LOSS\n");
       if (event & WUFE_EVENT_MACHINE_TX_UNDERRUN_BURST)
          printf("WUFE_EVENT_MACHINE_TX_UNDERRUN_BURST\n");
       if (event & WUFE_EVENT_MACHINE_TX_UNDERRUN_DDR)
          printf("WUFE_EVENT_MACHINE_TX_UNDERRUN_DDR\n");
   }
}

static void WT_UfeAnalyzeClockRecEvents(WUFE_events_clock_recovery *cr_events)
{
   WP_U16 event = cr_events->event_reg;
   WP_U16 stf_loss_line_id = cr_events->stuffing_loss_indication_line_id;

   printf("Clock Recovery event register 0x%04x\n", event);
   if (event & WUFE_EVENT_CLOCK_REC)
   {
      if (event & WUFE_EVENT_CLOCK_REC_XO_RATIO_LOSS_1)
         printf("WUFE_EVENT_CLOCK_REC_XO_RATIO_LOSS_1\n");
      if (event & WUFE_EVENT_CLOCK_REC_XO_RATIO_LOSS)
         printf("WUFE_EVENT_CLOCK_REC_XO_RATIO_LOSS\n");
      if (event & WUFE_EVENT_CLOCK_REC_DIFF_RATIO_LOSS_1)
         printf("WUFE_EVENT_CLOCK_REC_DIFF_RATIO_LOSS_1\n");
      if (event & WUFE_EVENT_CLOCK_REC_DIFF_RATIO_LOSS)
         printf("WUFE_EVENT_CLOCK_REC_DIFF_RATIO_LOSS\n");
      if (event & WUFE_EVENT_CLOCK_REC_STUFF_LOSS)
      {
         printf("WUFE_EVENT_CLOCK_REC_STUFF_LOSS\n");
         printf("Stuffing loss on line ID: %d\n", stf_loss_line_id);
      }
   }
}

static void WT_UfeAnalyzeEvents(WT_ufe *ufe)
{
   WP_U32 coreid;
   WUFE_events *ufe_events = &ufe->ufe_events;

   for(coreid=0; coreid<N_MAX_UFE_CORES; coreid++)
   {
      WT_UfeAnalyzeEmphyEvents(&(ufe_events->emphy_events[coreid]), coreid);
   }

   WT_UfeAnalyzeTdmEvents(&(ufe_events->tdm_events));
   WT_UfeAnalyzeFmEvents(&(ufe_events->fm_events));
   WT_UfeAnalyzeMachineEvents(&(ufe_events->machine_events));
   WT_UfeAnalyzeClockRecEvents(&(ufe_events->cr_events));
}

WP_U32 WT_UfeAnalyzeEvents4Automation(WT_ufe *ufe)
{
#if 0
   WP_U32 coreid;
   WUFE_events *ufe_events = &ufe->ufe_events;

   for (coreid = 0; coreid < N_MAX_UFE_CORES; coreid++)
   {
      if ((ufe_events->emphy_events[coreid].event_reg) & WUFE_EVENT_EMPHY)
         return WT_FAIL;
   }

   if ((ufe_events->tdm_events.event_reg) & WUFE_EVENT_TDM)
      return WT_FAIL;

   if ((ufe_events->fm_events.event_reg) & WUFE_EVENT_FM)
      return WT_FAIL;

   if ((ufe_events->machine_events.rx_event_reg) & WUFE_EVENT_MACHINE_RX)
      return WT_FAIL;

   if ((ufe_events->machine_events.tx_event_reg) & WUFE_EVENT_MACHINE_TX)
      return WT_FAIL;

   if ((ufe_events->cr_events.event_reg) & WUFE_EVENT_CLOCK_REC)
      return WT_FAIL;
#endif

   return WT_PASS;
}

void WT_UfeReadEvents(WT_ufe *ufe)
{
   WUFE_status status;
   WUFE_events* ufe_events = &ufe->ufe_events;

   memset(ufe_events, 0, sizeof(WUFE_events));

   status = WUFE_UfeReadEvents(ufe->ufe_id, ufe_events);
   WT_UfeTerminateOnError(status, "WUFE_UfeReadEvents  fail ", 0, __LINE__);

   WT_UfeAnalyzeEvents(ufe);
}

WP_U32 WT_UfeReadEvents4Automation(WT_ufe *ufe)
{
   WUFE_status status;
   WUFE_events* ufe_events = &ufe->ufe_events;
   WP_U32 result;

   memset(ufe_events, 0, sizeof(WUFE_events));

   status = WUFE_UfeReadEvents(ufe->ufe_id, ufe_events);
   WT_UfeTerminateOnError(status, "WUFE_UfeReadEvents  fail ", 0, __LINE__);

   WT_UfeAnalyzeEvents(ufe);

   result = WT_UfeAnalyzeEvents4Automation(ufe);

   return result;
}

#if CHECK_CORE_HW_INTERRUPT_ASSERTED
static void WTI_DisplayDeviceUfeEvents(WUFE_events *ufe_events)
{
   int coreid;

   for (coreid = 0; coreid < N_MAX_UFE_CORES; ++coreid)
      WT_UfeAnalyzeEmphyEvents(&(ufe_events->emphy_events[coreid]), coreid);

   WT_UfeAnalyzeTdmEvents(&(ufe_events->tdm_events));
   WT_UfeAnalyzeFmEvents(&(ufe_events->fm_events));
   WT_UfeAnalyzeMachineEvents(&(ufe_events->machine_events));
   WT_UfeAnalyzeClockRecEvents(&(ufe_events->cr_events));
}
#endif

void CLI_F_DisplayUfeEvents(char *StrPrm)
{
#if CHECK_CORE_HW_INTERRUPT_ASSERTED

   if (event_contents_0)
   {
      printf("Events for UFE on SERIAL 1\n");
      WTI_DisplayDeviceUfeEvents(&events_connector_0);

      event_contents_0 = WP_FALSE;
   }

   if (event_contents_2)
   {
      printf("Events for UFE on SERIAL 3\n");
      WTI_DisplayDeviceUfeEvents(&events_connector_2);

      event_contents_2 = WP_FALSE;
   }

#else
   printf("Ufe Events not collected\n");
#endif
}

void WT_UfeSystemIdleModify(WT_ufe *ufe, WUFE_idle_pattern *new_config)
{
   WUFE_status ufe_status;

   ufe_status = WUFE_SystemReconfigure(ufe->ufe_id, WUFE_SYSTEM_RECONFIG_IDLE_PATTERN, new_config);
   WT_UfeTerminateOnError( ufe_status, "WUFE_UfeSystemReconfigure" ,0,__LINE__);
}

void WT_UfeLineSdhModify(WT_ufe *ufe, WP_U32 line_index, WUFE_line_sdh_reconfig *config)
{
   WUFE_status ufe_status;

   ufe_status = WUFE_LineReconfigure(ufe->line_handle[line_index],
                                     WUFE_STRUCT_LINE_SDH,
                                     config);

   WT_UfeTerminateOnError(ufe_status, "WUFE_LineReconfigure" ,line_index,__LINE__);
}

void WT_UfeLineSonetModify(WT_ufe *ufe, WP_U32 line_index, WUFE_line_sonet_reconfig *config)
{
   WUFE_status ufe_status;



   ufe_status = WUFE_LineReconfigure(ufe->line_handle[line_index],
                                     WUFE_STRUCT_LINE_SONET,
                                     config);


   WT_UfeTerminateOnError(ufe_status, "WUFE_LineReconfigure" ,line_index,__LINE__);
}

void WT_UfePhySlotsModify(WT_ufe *ufe, WP_U32 phy_index, WUFE_phy *new_phy_cfg)
{
   WUFE_status ufe_status;

   ufe_status = WUFE_PhyReconfigure(ufe->phy_handle[phy_index],
                                    new_phy_cfg);
   WT_UfeTerminateOnError(ufe_status, "WUFE_PhyReconfigure" ,phy_index,__LINE__);
}

void WT_UfeLineSocketLpbkSetup(WT_ufe *ufe, int line_index, WUFE_line_loopback_type lpbk_type)
{
   WUFE_status ufe_status;

   ufe_status = WUFE_LineLoopbackConfigure(ufe->line_handle[line_index],lpbk_type);
   WT_UfeTerminateOnError(ufe_status, "WUFE_LineLoopbackConfigure" ,0,__LINE__);
}

void WT_UfeLinePdhLoopbackSetup(WT_ufe *ufe, int line_index, WUFE_line_pdh_loopback_type lpbk_type)
{
   WUFE_status ufe_status;

   ufe_status = WUFE_LinePdhLoopbackConfigure(ufe->line_handle[line_index],lpbk_type);
   WT_UfeTerminateOnError(ufe_status, "WUFE_LinePdhLoopbackConfigure" ,0,__LINE__);
}

void  WT_UfeRelease(WT_ufe *ufe)
{
   WUFE_status ufe_status;

   ufe_status = WUFE_UfeRelease(ufe->ufe_id);
   WT_UfeTerminateOnError( ufe_status, "WUFE_UfeRelease (from WT_UfeRelease)",0,__LINE__);
}

/* --------------------------------------------------------- */
/* ------------------   Vt100 Functions   ------------------ */
/* --------------------------------------------------------- */

void WT_Vt100CrsrRight(WP_U32 right)
{
   printf("\033[%dC",right);
}

void WT_Vt100CrsrLeft(WP_U32 left)
{
   printf("\033[%dD",left);
}


/*****************************************************************************
 * Function name: F_SampleSerial
 * Description  :
 * Input  params:
 * Output params:
 * Return val   :
 *****************************************************************************/
WP_U8 WT_SampleSerial(void)
{
#ifndef __PPC__

#ifndef MAP_UA_LSR
#if __WT_UFE3__
#define MAP_UA_RBR 0xbd030f00
#define MAP_UA_LSR 0xbd030f1c
#endif
#endif /*MAP_UA_LSR*/
#ifndef WEB_UART_STATUS_RXFE
#define WEB_UART_STATUS_RXFE            0x80
#define WEB_UART_STATUS_TEMT            0x40
#define WEB_UART_STATUS_THRE            0x20
#define WEB_UART_STATUS_BI              0x10
#define WEB_UART_STATUS_FE              0x08
#define WEB_UART_STATUS_PE              0x04
#define WEB_UART_STATUS_OE              0x02
#define WEB_UART_STATUS_DR              0x01
#endif /*WEB_UART_STATUS_RXFE*/

   WP_handle status;
   /* sample serial port */
   status = *(WP_U32 volatile *)MAP_UA_LSR;   /* Read UART line status register */
   if (status & (WEB_UART_STATUS_RXFE | WEB_UART_STATUS_OE | WEB_UART_STATUS_DR))
   {
      return 1;
   }
   else
   {
      return 0;
   }
#else
   return 0;
#endif
} /* F_SampleSerial */


/* ---------------------------------------------------------------------- */
/* ----------                                             --------------- */
/* ----------     End of Temperature Sensor Functions     --------------- */
/* ----------                                             --------------- */
/* ---------------------------------------------------------------------- */

/*********************************************
 ***        IO functions                 ***
 *********************************************/
void WT_UfeGpioOutputClockSet(WT_ufe *ufe, WP_U32 gpio_number,
                              WP_U8 gpio_clock_type, WP_U8 gpio_ref_clk_out)
{
   WUFE_status ufe_status = WUFE_OK;

   ufe_status = WUFE_GpioOutputClockSet(ufe->ufe_id, gpio_number, gpio_clock_type, gpio_ref_clk_out);
   WT_UfeTerminateOnError(ufe_status, "WT_UfeGpioOutputClockSet" ,0,__LINE__);
}


void WT_UfeGpioOutputClockRead(WT_ufe *ufe, WP_U16 *gpio_register)
{
   WUFE_status ufe_status = WUFE_OK;

   ufe_status = WUFE_GpioOutputClockRead(ufe->ufe_id, gpio_register);
   WT_UfeTerminateOnError(ufe_status, "WT_UfeGpioOutputClockRead" ,0,__LINE__);
}


#ifdef WT_UFE_FRAMER
void WT_UFEPDHOutputClockSet(WT_ufe *ufe,WP_U8 output_select, WP_U32 line_index,WP_U32 ClockRate)
{
   WTI_FlexmuxPDHOutputClockSet(output_select,line_index,ufe->line_params[line_index].transfer_type,ClockRate);
}

WP_U32 CLI_F_UfeFramerPrbsCheck(char *StrPrm)
{
   WP_U32 res, cmd;
   WP_CHAR temp_string[WTI_MAX_STRING_SIZE], *byte;
   WP_U32 lines[MAX_UFE_LINES_USED];
   WP_U32 line_index, client_port;
   WP_U8 prbs_errors = 0;
   WP_U8 prbs_tmp = 0;
   WP_U32 i,j;

   res = sscanf(StrPrm, "%d %s", &cmd, temp_string);

   if (res == 1)
   {
      printf("Checking UFE Framer PRBS (only on active lines)...\n");
      for (line_index = 0; line_index < MAX_UFE_LINES_USED; line_index ++)
      {
         if(! (the_system->ufe.line_handle[line_index] == WT_UFE_HANDLE_INVALID) )
         {
#if WTI_DUAL_EMPHY
        	 if(line_index >= WTI_FIRST_UFE_NUM_LINES)
        	 {
        		 client_port = WTI_FlexmuxClientPortIndexGet(line_index, the_system->second_ufe.line_params->transfer_type);
        	 }
        	 else
        	 {
        		 client_port = WTI_FlexmuxClientPortIndexGet(line_index, the_system->ufe.line_params->transfer_type);
        	 }
#else
        	 client_port = WTI_FlexmuxClientPortIndexGet(line_index, the_system->ufe.line_params->transfer_type);
#endif
        	 prbs_tmp = WTI_FlexmuxCheckPrbsResult(0,client_port,the_system->ufe.line_params[line_index].framed);
        	 prbs_errors |= prbs_tmp;
        	 if(prbs_tmp)
        		 printf("PRBS check failed at line %d. Line:%d\n",line_index, __LINE__);
         }
      }
   }
   else
   {
      i = 0;

      if (strstr(temp_string, ","))
      {
         byte = strtok(temp_string, ",");
         while (byte)
         {
            lines[i++] = atoi(byte);
            byte = strtok(NULL, ",");
         }
         if (i > MAX_UFE_LINES_USED)
         {
            WTI_TerminatePrintError("CLI_F_UfeFramerPrbsCheck", __LINE__);
            return WT_FAIL;
         }
      }
      else if (strstr(temp_string, "-"))
      {
         byte = strtok(temp_string, "-");
         if (byte)
         {
            lines[0] = atoi(byte);
            byte = strtok(NULL, "-");
            j = atoi(byte);
            for (i = 1; i <= j-lines[0]; i++)
            {
               lines[i] = lines[i - 1] + 1;
            }
         }
      }
      else /* Single line index */
      {
            i = 1;
            lines[0] = atoi(temp_string);
      }

      for (j = 0; j < i; j ++)
      {
         line_index = lines[j];
         if ( line_index >= MAX_UFE_LINES_USED || line_index < 0 )
         {
            printf("\nCLI_F_UfeFramerPrbsCheck(). Line index %d is invalid. Line:%d\n",line_index, __LINE__);
            return WT_FAIL;
         }
      }

      printf("Checking UFE Framer PRBS (only on active lines)...\n");
      for (j = 0; j < i; j ++)
      {
    	  line_index = lines[j];


#if WTI_DUAL_EMPHY
    	  if(!(the_system->ufe.line_handle[line_index] == WT_UFE_HANDLE_INVALID))
    	  {
    		  printf("Check PRBS on first UFE at line %d\n",line_index);
    		  client_port = WTI_FlexmuxClientPortIndexGet(line_index, the_system->ufe.line_params->transfer_type);
    		  prbs_tmp = WTI_FlexmuxCheckPrbsResult(0,client_port,the_system->ufe.line_params[line_index].framed);
    		  prbs_errors |= prbs_tmp;
    		  if(prbs_tmp)
    			  printf("PRBS check failed at line %d on first UFE. Line:%d\n",line_index, __LINE__);
    	  }
    	  else
    		  printf("\nLine %d is inactive on first UFE. Line:%d\n",line_index, __LINE__);

   	  if(!(the_system->second_ufe.line_handle[line_index] == WT_UFE_HANDLE_INVALID))
   	  {
   		  printf("Check PRBS on second UFE at line %d\n",line_index);
   		  client_port = WTI_FlexmuxClientPortIndexGet(line_index, the_system->second_ufe.line_params->transfer_type);
   		  prbs_tmp = WTI_FlexmuxCheckPrbsResult(1,client_port,the_system->second_ufe.line_params[line_index].framed);
   		  prbs_errors |= prbs_tmp;
   		  if(prbs_tmp)
   			  printf("PRBS check failed at line %d on second UFE. Line:%d\n",line_index, __LINE__);
   	  }
   	  else
   		  printf("\nLine %d is inactive on second UFE. Line:%d\n",line_index, __LINE__);
#elif defined __WT_UFE448__

          if ((WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION == WTI_FLEXMUX_PHYSICAL_CONF) ||
              (WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION == physical_configuration))
          {
             if ((336 <= line_index) && (672 > line_index))
                line_index += 336;
          }
          

          WP_U32 ufe_line_index = line_index;
          
          WP_U32 spe_num = (ufe_line_index / 28);
          WP_U32 tug2_num = ((ufe_line_index % 28) / 3);
          WP_U32 offset_within_tug2 = ((ufe_line_index % 28) % 3);          

          line_index = ((spe_num*28) + (tug2_num*3) + offset_within_tug2);
          
    	  if(!(the_system->ufe.line_handle[line_index] == WT_UFE_HANDLE_INVALID))
    	  {
    		  printf("Check PRBS at line %d\n",line_index);
    		  client_port = WTI_FlexmuxClientPortIndexGet(line_index, the_system->ufe.line_params->transfer_type);
                  printf("client_port %u\n", client_port);
    		  prbs_tmp = WTI_FlexmuxCheckPrbsResult(0,client_port,the_system->ufe.line_params[line_index].framed);
    		  prbs_errors |= prbs_tmp;
    		  if(prbs_tmp)
    			  printf("PRBS check failed at line %d. Line:%d\n",line_index, __LINE__);
    	  }
    	  else
    		  printf("\nLine %d is inactive. Line:%d\n",line_index, __LINE__);
#else
    	  if(!(the_system->ufe.line_handle[line_index] == WT_UFE_HANDLE_INVALID))
    	  {
    		  printf("Check PRBS at line %d\n",line_index);
    		  client_port = WTI_FlexmuxClientPortIndexGet(line_index, the_system->ufe.line_params->transfer_type);
    		  prbs_tmp = WTI_FlexmuxCheckPrbsResult(0,client_port,the_system->ufe.line_params[line_index].framed);
    		  prbs_errors |= prbs_tmp;
    		  if(prbs_tmp)
    			  printf("PRBS check failed at line %d. Line:%d\n",line_index, __LINE__);
    	  }
    	  else
    		  printf("\nLine %d is inactive. Line:%d\n",line_index, __LINE__);
#endif
      }
      }

   if (prbs_errors == 0)
   {
      printf("PRBS check Passed\n");
      return WT_PASS;
   }
   else
   {
      printf("PRBS check failed\n");
      return WT_FAIL;
   }
}

#endif /* WT_UFE_FRAMER */


/**********************************************************************************
 **********************************************************************************
 ************             Clock Recovery Functions            *********************
 **********************************************************************************
 *********************************************************************************/
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE

WP_U32 WT_ClockRecInterfaceId(WP_U32 line_index)
{
	WUFE_clk_rec_info_line clk_rec_info_line;

	memset(&clk_rec_info_line, 0, sizeof(WUFE_clk_rec_info_line));

#if WTI_DUAL_EMPHY
	if(line_index >= WTI_FIRST_UFE_NUM_LINES)
	{
		WT_UfeClockRecInfoGet(the_system->second_ufe.ufe_id,
							  the_system->second_ufe.line_handle[line_index - WTI_FIRST_UFE_NUM_LINES],
							  WUFE_CLOCK_REC_INFO_LINE,
							  &clk_rec_info_line);
	}
	else
	{
		WT_UfeClockRecInfoGet(the_system->ufe.ufe_id,
							  the_system->ufe.line_handle[line_index],
							  WUFE_CLOCK_REC_INFO_LINE,
							  &clk_rec_info_line);
	}
#else
	WT_UfeClockRecInfoGet(the_system->ufe.ufe_id,
						  the_system->ufe.line_handle[line_index],
						  WUFE_CLOCK_REC_INFO_LINE,
						  &clk_rec_info_line);
#endif

return (clk_rec_info_line.clock_rec_if_id);
}

void WT_UfeClockRecInfoGet(WP_U32 wufe_id,
                           WUFE_handle ufe_line_handle,
                           WUFE_clock_rec_info_type info_type,
                           void *clk_rec_info)
{
   WUFE_status ufe_status = WUFE_OK;

   ufe_status = WUFE_ClockRecInfo(wufe_id,
                                  ufe_line_handle,
                                  info_type,
                                  clk_rec_info);
   WT_UfeTerminateOnError(ufe_status, "WT_UfeClockRecInfoGet" ,0,__LINE__);
}

/* Function for internal testing and debug */
/* This function sets or clears the TS stub bit in the CR mode register.
   It is used mainly for testing and debug and required a dedicated synthesis. This function
   has no real purpose in a working system and should not be called. */
void WT_UfeClockRecTimestampStubSet(WP_U32 enable)
{
   WUFE_status ufe_status = WUFE_OK;

   ufe_status = WUFE_ClockRecTimestampStubSet(the_system->ufe.ufe_id, enable);
   WT_UfeTerminateOnError(ufe_status, "WT_UfeClockRecTimestampStubSet" ,0,__LINE__);

}
/* end of function for internal testing and debug */

#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
