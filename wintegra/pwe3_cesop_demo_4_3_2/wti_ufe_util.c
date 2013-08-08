/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/****************************************************************************
 *
 * Example: wti_ufe_util.c
 * UFE demo utility functions for PWE3 CES application.
 *  Demonstrates the use of UFE API.
 *
 ****************************************************************************/
#include <string.h>

#ifdef __WT_UFE3__
#include "wti_tmx336_util.h"
#else
#error "__WT_UFE__ Undefined"
#endif

#include "wpx_tdm_comet.h"

#include "wufe_types.h"
#include "wufe_errors.h"
#include "wti_ufe_util.h"

#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)

/* calculated for 100Mhz/100 PPM DCO ref clock */
static  WUFE_clk_rec_drc_divisor_list drc =
{
   {
      { 0x1f/*nominal_int*/, 0x6227caf1 /*nominal_rem*/ },/*T1*/
      { 0x17/*nominal_int*/, 0x6a000000/*nominal_rem*/ },/*E1*/

      /* In DS3 the DCO output is set to DS3/4 , the UFE divide the (DS3/4)/(1398) = 8KHz            */
      /* The 8KHz clock is feed to the jitter attenuator (FX-700) that genarate recovered DS3 clock  */
      /* The DS3 clock is feed to the temux as recovered clock and also to ufe that divide           */
      /* the clock by 4 and inject the recovered DS3/4 clock to the TS logic                         */

      { 0x03/*nominal_int*/, 0x787dfc56/*nominal_rem*/ },/*DS3/4*/

      /* In E3 the DCO output is set to E3/4 , the UFE divide the (E3/4)/(1074) = 8KHz               */
      /* The 8KHz clock is feed to the jitter attenuator (FX-700) that genarate recovered E3 clock   */
      /* The E3 clock is feed to the temux as recovered clock and also to ufe that divide            */
      /* the clock by 4 and inject the recovered E3/4 clock to the TS logic                          */

      { 0x04/*nominal_int*/, 0xd1c206ac/*nominal_rem*/ } /*E3/4 */
   }
};

/* calculated for 100Mhz/100 PPM extended DCO ref clock (extended differential) */
static  WUFE_clk_rec_drc_divisor_list ext_drc =
{
   {
      { 0x1f/*nominal_int*/, 0x6227caf1 /*nominal_rem*/ },/*T1*/
      { 0x17/*nominal_int*/, 0x6a000000/*nominal_rem*/ },/*E1*/

      /* In DS3 the DCO output is set to DS3/4 , the UFE divide the (DS3/4)/(1398) = 8KHz            */
      /* The 8KHz clock is feed to the jitter attenuator (FX-700) that genarate recovered DS3 clock  */
      /* The DS3 clock is feed to the temux as recovered clock and also to ufe that divide           */
      /* the clock by 4 and inject the recovered DS3/4 clock to the TS logic                         */

      { 0x03/*nominal_int*/, 0x787dfc56/*nominal_rem*/ },/*DS3/4*/

      /* In E3 the DCO output is set to E3/4 , the UFE divide the (E3/4)/(1074) = 8KHz               */
      /* The 8KHz clock is feed to the jitter attenuator (FX-700) that genarate recovered E3 clock   */
      /* The E3 clock is feed to the temux as recovered clock and also to ufe that divide            */
      /* the clock by 4 and inject the recovered E3/4 clock to the TS logic                          */

      { 0x04/*nominal_int*/, 0xd1c206ac/*nominal_rem*/ } /*E3/4 */
   }
};

#endif

WP_U8 pri_descr[4][4] =
{
   {0,1,2,3},
   {0,1,2,3},
   {0,0,1,1},
   {2,2,3,3}
};

static void WT_UfeTerminateOnError(WP_handle handle, WP_CHAR *s,WP_U32 id,WP_U32 LineNum)
{
   if(handle != WUFE_OK)
   {
      printf("Test Abort %s %d %s 0x%x Line:%d\n", s,id,
             WUFE_error_name[(handle&0x03ffffff)], handle, LineNum);

      exit(1);

   }
#if DBG_MODE > 0
   else
   {
      printf("Test info %s %d 0x%x Line:%d\n", s,id, handle, LineNum);
   }
#endif
}

void WT_UfeTdmCometOverUfeInit(WT_ufe *ufe, WT_ufe_line_octal_params *line_octal_params)
{
   WP_status status;
   WP_U32 tdm_commet_protocol=0, i;
   WP_U32 tdm_commet_cpld=0;

#if WTI_DEBUG_LEVEL > 0
   printf("Initializing Comet - UFE3\n");
#endif

   switch (line_octal_params->transfer_type)
   {
      case WUFE_CAD_TYPE_E1:
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
         tdm_commet_cpld = WPU_INIT_COMET_CPLD_CR_E1;
#else
         tdm_commet_cpld = WPU_INIT_COMET_CPLD_E1;
#endif
         if (line_octal_params->framed==WUFE_FRAMED)
         {
            if (line_octal_params->cas_enable == WUFE_CAS_ENABLE)  /* CAS mode */
            {

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
               tdm_commet_protocol = WPU_MODE_TDM_E1_CR_CAS_MASTER_PWE3_CES_CR;
#elif WTI_CESOP_REGRESSION_TEST
               tdm_commet_protocol = WPU_MODE_TDM_TRANS_E1_CAS_SLAVE_PWE3_CES_CR;
#else
               tdm_commet_protocol = WPU_MODE_TDM_TRANS_E1_CAS_MASTER_PWE3_CES_CR;
#endif
            }
            else

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
               tdm_commet_protocol = WPU_MODE_TDM_E1_CR_MASTER_PWE3_CES_CR;
#elif WTI_CESOP_REGRESSION_TEST
               tdm_commet_protocol = WPU_MODE_TDM_E1_SLAVE_PWE3_CES_CR;
#else
               tdm_commet_protocol = WPU_MODE_TDM_E1_MASTER_PWE3_CES_CR;
#endif
         }
         else
#if WTI_CESOP_REGRESSION_TEST
            tdm_commet_protocol = WPU_MODE_TDM_E1_SLAVE_UNFRAMED_PWE3_CES_CR;
#else
            tdm_commet_protocol = WPU_MODE_TDM_E1_MASTER_UNFRAMED_PWE3_CES_CR;
#endif
         break;
      case WUFE_CAD_TYPE_T1:
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
         tdm_commet_cpld = WPU_INIT_COMET_CPLD_CR_T1;
#else
         tdm_commet_cpld = WPU_INIT_COMET_CPLD_T1;
#endif
         if (line_octal_params->framed==WUFE_FRAMED)
         {
            if (line_octal_params->cas_enable == WUFE_CAS_ENABLE)  /* CAS mode */
            {

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
               tdm_commet_protocol = WPU_MODE_TDM_T1_CR_CAS_MASTER_PWE3_CES_CR;
#elif WTI_CESOP_REGRESSION_TEST
               tdm_commet_protocol = WPU_MODE_TDM_TRANS_T1_CAS_SLAVE_PWE3_CES_CR;
#else
               tdm_commet_protocol = WPU_MODE_TDM_TRANS_T1_CAS_MASTER_PWE3_CES_CR;
#endif
            }
            else
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
               tdm_commet_protocol = WPU_MODE_TDM_T1_CR_MASTER_PWE3_CES_CR;
#elif WTI_CESOP_REGRESSION_TEST
               tdm_commet_protocol = WPU_MODE_TDM_T1_SLAVE_PWE3_CES_CR;
#else
               tdm_commet_protocol = WPU_MODE_TDM_T1_MASTER_PWE3_CES_CR;
#endif
         }
         else
#if WTI_CESOP_REGRESSION_TEST
            tdm_commet_protocol = WPU_MODE_TDM_T1_SLAVE_UNFRAMED_PWE3_CES_CR;
#else
            tdm_commet_protocol = WPU_MODE_TDM_T1_MASTER_UNFRAMED_PWE3_CES_CR;
#endif
         break;
      default:
         printf("Error: invlalid tdm_commet_protocol\n");
         exit(1);
   }

#ifdef WP_HW_WINPATH3
   status = WPU_TdmCometCardInit(WP_UFE_UPI3_TDM1, tdm_commet_cpld);
   if (status != WP_OK)
   {
      printf("WPU_TdmCometCardInit failed: %s\n", WP_error_name[WP_ERROR(status)]);
      exit(1);
   }
   /*initializes the TDM ports all ports instaead of  WP_UFE_UPI2_TDM1+line_octal_params->line_id*/
   for( i=0; i<8; i++)
   {
      status = WPU_TdmCometCardInit(WP_UFE_UPI3_TDM1 + i, tdm_commet_protocol);
      if (status != WP_OK)
      {
         printf("WPU_TdmCometCardInit failed: %s\n", WP_error_name[WP_ERROR(status)]);
         exit(1);
      }
   }
#else
   status = WPU_TdmCometCardInit(WP_UFE_UPI2_TDM1, tdm_commet_cpld);
   if (status != WP_OK)
   {
      printf("WPU_TdmCometCardInit failed: %s\n", WP_error_name[WP_ERROR(status)]);
      exit(1);
   }
   /*initializes the TDM ports all ports instaead of  WP_UFE_UPI2_TDM1+line_octal_params->line_id*/
   for( i=0; i<8; i++)
   {
      status = WPU_TdmCometCardInit(WP_UFE_UPI2_TDM1 + i, tdm_commet_protocol);
      if (status != WP_OK)
      {
         printf("WPU_TdmCometCardInit failed: %s\n", WP_error_name[WP_ERROR(status)]);
         exit(1);
      }
   }
#endif
}

/**********************************************************************************
 **********************************************************************************
 ************                       SYSTEM                    *********************
 **********************************************************************************
 *********************************************************************************/
void WT_UfeSbiGlobalConfig(WT_ufe *ufe, WUFE_system *sys_cfg)
{
   int i;

   sys_cfg->line_interface  = WUFE_INTERFACE_SBI;
#ifndef __WT_UFE3__
   sys_cfg->sbi.in_clock = WUFE_SBI_CLK_19_MHZ;
   sys_cfg->sbi.stm_mode = WUFE_SBI_STM_0;
#else
   sys_cfg->sbi.in_clock = WUFE_SBI_CLK_77_MHZ;
   sys_cfg->sbi.stm_mode = WUFE_SBI_STM_INVALID;
#endif
   sys_cfg->sbi.rx_parity = WUFE_SBI_PARITY_ODD;
   sys_cfg->sbi.tx_parity = WUFE_SBI_PARITY_ODD;
   sys_cfg->sbi.add_c1fp  = WUFE_SBI_C1FP_ALIGN_4;
   sys_cfg->sbi.drop_c1fp = WUFE_SBI_C1FP_ALIGN_4;

   if (ufe->operation_mode == WT_OP_MODE_LOOPBACK_LINE)
      sys_cfg->sbi.loopback = WUFE_SBI_LOOPBACK_LINE;
   else if (ufe->operation_mode == WT_OP_MODE_LOOPBACK_SYSTEM)
      sys_cfg->sbi.loopback = WUFE_SBI_LOOPBACK_SYSTEM;
   else
      sys_cfg->sbi.loopback = WUFE_SBI_NORMAL;

   sys_cfg->sbi.bus_master = WUFE_SBI_BUS_MASTER;

   for (i=0; i<N_MAX_UFE_SPE_INDEX; i++)
      sys_cfg->sbi.transfer_type_spe[i] = ufe->sbi_spe_line_type[i];

   sys_cfg->sbi.max_clock_translators = ufe->max_sbi_clock_translators;
#if WTI_EXT_DIFF_MODE_ACTIVE
   sys_cfg->sbi.max_ext_diff_clock_translators = ufe->max_sbi_ext_diff_clock_translators;
#else
   sys_cfg->sbi.max_ext_diff_clock_translators = 0;
#endif
}

void WT_UfeSystemConfig(WT_ufe *ufe, WUFE_system *sys_cfg)
{
   int i, j;

   memset(sys_cfg, 0, sizeof(WUFE_system));

   if (ufe->fpga_mode == WT_SBI)
   {
      WT_UfeSbiGlobalConfig(ufe, sys_cfg);
   }
   else if (ufe->fpga_mode == WT_OCTAL)
   {
      sys_cfg->line_interface = WUFE_INTERFACE_CAD_E1_T1;
   }
   else if (ufe->fpga_mode == WT_HMVIP)
   {
      sys_cfg->line_interface = WUFE_INTERFACE_CAD_HMVIP;
   }
   else
   {
      WUFE_status ufe_status = 0x1;
      WT_UfeTerminateOnError(ufe_status, "HW not HMVIP not SBI or OCTAL", ufe->fpga_mode, __LINE__);
   }

   sys_cfg->max_line_number = ufe->max_lines;
   sys_cfg->max_phy_number = ufe->max_phys;

#if WTI_8K_CH_SETUP
   /* Allocate up to 4 buffers to each phy TX fifo. */
   sys_cfg->fifo_manager.tx_fbp_size = sys_cfg->max_phy_number * 4;

   sys_cfg->fifo_manager.rx_fbp_size =
      WT_SELECT_MIN((N_MAX_UFE_ZBT_BUFFERS-sys_cfg->fifo_manager.tx_fbp_size-1),
                    sys_cfg->max_phy_number*0x3);
#elif WTI_CAD_0_3_SETUP
   /* Allocate up to 4 buffers to each phy TX fifo. */
   sys_cfg->fifo_manager.tx_fbp_size = sys_cfg->max_phy_number * 4;

   sys_cfg->fifo_manager.rx_fbp_size =
      WT_SELECT_MIN((N_MAX_UFE_ZBT_BUFFERS-sys_cfg->fifo_manager.tx_fbp_size-1),
                    sys_cfg->max_phy_number*0x4);
#else
   /* Allocate up to 4 buffers to each phy TX fifo. */
   sys_cfg->fifo_manager.tx_fbp_size = sys_cfg->max_phy_number * 4;

   sys_cfg->fifo_manager.rx_fbp_size =
      WT_SELECT_MIN((N_MAX_UFE_ZBT_BUFFERS-sys_cfg->fifo_manager.tx_fbp_size-1),
                    sys_cfg->max_phy_number*0xb);
#endif

   sys_cfg->idle_pattern.tx_idle_byte_data_pattern=0xff;
   sys_cfg->idle_pattern.tx_idle_nibble_cas_pattern=0x4;

   sys_cfg->emphy.rx_parity_mode  = WUFE_RX_PARITY_ODD;
   sys_cfg->emphy.tx_parity_mode  = WUFE_TX_PARITY_ODD;
   sys_cfg->emphy.tx_parity_check = WUFE_TX_PARITY_IGNORE;

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   if (ufe->fpga_mode == WT_OCTAL)
   {
      /* for CAD regular non-CR ports */
      sys_cfg->emphy.rx_pri_table[0][0] = 0;
      sys_cfg->emphy.rx_pri_table[0][1] = 0;
      sys_cfg->emphy.rx_pri_table[0][2] = 1;
      sys_cfg->emphy.rx_pri_table[0][3] = 1;

      sys_cfg->emphy.rx_pri_table[1][0] = 0;
      sys_cfg->emphy.rx_pri_table[1][1] = 0;
      sys_cfg->emphy.rx_pri_table[1][2] = 1;
      sys_cfg->emphy.rx_pri_table[1][3] = 1;

      /* for CAD CR E1/T1 ports */
      sys_cfg->emphy.rx_pri_table[2][0] = 2;
      sys_cfg->emphy.rx_pri_table[2][1] = 2;
      sys_cfg->emphy.rx_pri_table[2][2] = 3;
      sys_cfg->emphy.rx_pri_table[2][3] = 3;

      /* for CAD CR DS3/E3 ports */
      sys_cfg->emphy.rx_pri_table[3][0] = 2;
      sys_cfg->emphy.rx_pri_table[3][1] = 2;
      sys_cfg->emphy.rx_pri_table[3][2] = 3;
      sys_cfg->emphy.rx_pri_table[3][3] = 3;

      /* for CAD regular non-CR ports */
      sys_cfg->emphy.tx_pri_table[0][0] = 0;
      sys_cfg->emphy.tx_pri_table[0][1] = 0;
      sys_cfg->emphy.tx_pri_table[0][2] = 1;
      sys_cfg->emphy.tx_pri_table[0][3] = 1;

      sys_cfg->emphy.tx_pri_table[1][0] = 0;
      sys_cfg->emphy.tx_pri_table[1][1] = 0;
      sys_cfg->emphy.tx_pri_table[1][2] = 1;
      sys_cfg->emphy.tx_pri_table[1][3] = 1;

      /* for CAD CR E1/T1 ports */
      sys_cfg->emphy.tx_pri_table[2][0] = 2;
      sys_cfg->emphy.tx_pri_table[2][1] = 2;
      sys_cfg->emphy.tx_pri_table[2][2] = 3;
      sys_cfg->emphy.tx_pri_table[2][3] = 3;

      /* for CAD CR DS3/E3 ports */
      sys_cfg->emphy.tx_pri_table[3][0] = 2;
      sys_cfg->emphy.tx_pri_table[3][1] = 2;
      sys_cfg->emphy.tx_pri_table[3][2] = 3;
      sys_cfg->emphy.tx_pri_table[3][3] = 3;

      sys_cfg->emphy.tx_inframe_pri_table[0][0] = 0;
      sys_cfg->emphy.tx_inframe_pri_table[0][1] = 0;
      sys_cfg->emphy.tx_inframe_pri_table[0][2] = 1;
      sys_cfg->emphy.tx_inframe_pri_table[0][3] = 1;

      sys_cfg->emphy.tx_inframe_pri_table[1][0] = 0;
      sys_cfg->emphy.tx_inframe_pri_table[1][1] = 0;
      sys_cfg->emphy.tx_inframe_pri_table[1][2] = 1;
      sys_cfg->emphy.tx_inframe_pri_table[1][3] = 1;

      sys_cfg->emphy.tx_inframe_pri_table[2][0] = 2;
      sys_cfg->emphy.tx_inframe_pri_table[2][1] = 2;
      sys_cfg->emphy.tx_inframe_pri_table[2][2] = 3;
      sys_cfg->emphy.tx_inframe_pri_table[2][3] = 3;

      sys_cfg->emphy.tx_inframe_pri_table[3][0] = 2;
      sys_cfg->emphy.tx_inframe_pri_table[3][1] = 2;
      sys_cfg->emphy.tx_inframe_pri_table[3][2] = 3;
      sys_cfg->emphy.tx_inframe_pri_table[3][3] = 3;
   }
   else
   {
      for (i=0;i<4;i++)
         for (j=0;j<4;j++)
         {
            sys_cfg->emphy.rx_pri_table[i][j] = j;
         }
      for (i=0;i<4;i++)
      {
         sys_cfg->emphy.tx_pri_table[i][0] = 0;
         for (j=1;j<4;j++)
         {
            sys_cfg->emphy.tx_pri_table[i][j] = j-1;
         }
      }
      for (i=0;i<4;i++)
         for (j=0;j<4;j++)
         {
            sys_cfg->emphy.tx_inframe_pri_table[i][j] = j;
         }
   }

#else
   for (i=0;i<4;i++)
      for (j=0;j<4;j++)
      {
         sys_cfg->emphy.rx_pri_table[i][j] = j;
      }
   for (i=0;i<4;i++)
   {
      sys_cfg->emphy.tx_pri_table[i][0] = 0;
      for (j=1;j<4;j++)
      {
         sys_cfg->emphy.tx_pri_table[i][j] = j-1;
      }
   }
   for (i=0;i<4;i++)
      for (j=0;j<4;j++)
      {
         sys_cfg->emphy.tx_inframe_pri_table[i][j] = j;
      }

#endif
   sys_cfg->system_clock_rec_enable = WUFE_SYSTEM_CLOCK_REC_DISABLE;
   sys_cfg->clock_rec_sys_params = 0;
}

#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE|| WTI_CESOP_RX_TIMING_ENABLE)
static void WT_UfeClockRecoveryConfig(WT_ufe *ufe,
                                      WUFE_clock_rec_system *clock_rec_sys_cfg,
                                      WP_U8 clock_rec_method)
{
   WUFE_status ufe_status;
   WUFE_clk_rec_info_hw info_hw;

   memset(&info_hw, 0, sizeof(WUFE_clk_rec_info_hw));

   ufe_status = WUFE_ClockRecInfo(ufe->ufe_id, 0, WUFE_CLOCK_REC_INFO_HW, &info_hw);
   WT_UfeTerminateOnError(ufe_status, "WUFE_ClockRecInfo" ,0,__LINE__);

   if (ufe->fpga_mode == WT_OCTAL)
   {
#if (!(WTI_CLOCK_REC_ADA_BW_COM_MODE))
      clock_rec_sys_cfg->max_rx_clk_rec_if = info_hw.max_rx_clk_rec_if;
      clock_rec_sys_cfg->max_tx_clk_rec_if = info_hw.max_tx_clk_rec_if;
#if WTI_EXT_DIFF_MODE_ACTIVE
      clock_rec_sys_cfg->max_ext_diff_rx_clk_rec_if = info_hw.max_ext_diff_rx_clk_rec_if;
      clock_rec_sys_cfg->max_ext_diff_tx_clk_rec_if = info_hw.max_ext_diff_tx_clk_rec_if;
#else
      clock_rec_sys_cfg->max_ext_diff_rx_clk_rec_if = 0;
      clock_rec_sys_cfg->max_ext_diff_tx_clk_rec_if = 0;
#endif
#else
      clock_rec_sys_cfg->max_rx_clk_rec_if = 0;
      clock_rec_sys_cfg->max_tx_clk_rec_if = N_ACTIVE_UFE_OCT_LINES;
      clock_rec_sys_cfg->max_ext_diff_rx_clk_rec_if = 0;
      clock_rec_sys_cfg->max_ext_diff_tx_clk_rec_if = 0;
#endif
   }
   else
   {
      /* Max rx clock recovery interfaces <= number of clock recovery interfaces in ufe */
      clock_rec_sys_cfg->max_rx_clk_rec_if = info_hw.max_rx_clk_rec_if;/* max 32 */
      clock_rec_sys_cfg->max_tx_clk_rec_if = info_hw.max_tx_clk_rec_if;/* max 32 */
#if WTI_EXT_DIFF_MODE_ACTIVE
      clock_rec_sys_cfg->max_ext_diff_rx_clk_rec_if = info_hw.max_ext_diff_rx_clk_rec_if;
      clock_rec_sys_cfg->max_ext_diff_tx_clk_rec_if = info_hw.max_ext_diff_tx_clk_rec_if;
#else
      clock_rec_sys_cfg->max_ext_diff_rx_clk_rec_if = 0;
      clock_rec_sys_cfg->max_ext_diff_tx_clk_rec_if = 0;
#endif
   }

   if (clock_rec_method == WUFE_CLOCK_REC_METHOD_ADAPTIVE)
      clock_rec_sys_cfg->method = WUFE_CLOCK_REC_METHOD_ADAPTIVE;
   else
      clock_rec_sys_cfg->method = WUFE_CLOCK_REC_METHOD_DIFFERENTIAL;
   clock_rec_sys_cfg->sync_mode = WUFE_CLOCK_REC_SYNC_NEG;
   clock_rec_sys_cfg->active_mode = WUFE_CLOCK_REC_ACTIVE_HIGH;
   clock_rec_sys_cfg->drc = &drc;
   clock_rec_sys_cfg->ext_drc = &ext_drc;
}
#endif

void  WT_UfeSystemCreate(WT_ufe *ufe)
{
   WUFE_system sys_cfg;
   WUFE_status ufe_status;
#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE|| WTI_CESOP_RX_TIMING_ENABLE)
   WUFE_clock_rec_system clock_rec_sys_cfg;
#endif
   WUFE_init_config config;

   memset(&config, 0, sizeof(WUFE_init_config));
   config.ufe_id = ufe->ufe_id;

   if(ufe->upi_id == WP_PORT_UPI2)
      config.attributes |= WUFE_ATTRIB_CONNECT_OFFSET_F(1);

   /* Initialize the UFE*/
   ufe_status = WUFE_UfeInit(&config);
   WT_UfeTerminateOnError(ufe_status,"WUFE_UfeInit",0,__LINE__);

   /************************************
    * Configure the UFE system.
    ************************************/

   WT_UfeSystemConfig(ufe, &sys_cfg);

   /* initialize clock recovery global parameters */
#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)
   if (ufe->system_clock_rec_enable == WUFE_SYSTEM_CLOCK_REC_ENABLE)
   {
      WT_UfeClockRecoveryConfig(ufe, &clock_rec_sys_cfg, ufe->clock_rec_method);
      sys_cfg.system_clock_rec_enable = WUFE_SYSTEM_CLOCK_REC_ENABLE;
      sys_cfg.clock_rec_sys_params = &clock_rec_sys_cfg;
      if (ufe->fpga_mode == WT_SBI)
      {
#if WTI_EXT_DIFF_MODE_ACTIVE
         ufe->max_sbi_clock_translators = global_active_cr_async_mode;
         ufe->max_sbi_ext_diff_clock_translators = global_active_cr_extdiff_mode;
         sys_cfg.sbi.max_clock_translators = ufe->max_sbi_clock_translators;
         sys_cfg.sbi.max_ext_diff_clock_translators = ufe->max_sbi_ext_diff_clock_translators;
#else
         ufe->max_sbi_clock_translators = global_active_cr_async_mode;
         ufe->max_sbi_ext_diff_clock_translators = 0;
         sys_cfg.sbi.max_clock_translators = ufe->max_sbi_clock_translators;
         sys_cfg.sbi.max_ext_diff_clock_translators = ufe->max_sbi_ext_diff_clock_translators;
#endif
      }
   }
#endif

   ufe_status = WUFE_SystemConfig(ufe->ufe_id, &sys_cfg);
   WT_UfeTerminateOnError(ufe_status, "WUFE_SystemConfig  ",0,__LINE__);
}

void  WT_UfeSystemEnable(WT_ufe *ufe)
{
   WUFE_status ufe_status;
   WP_U32 property, i;

   /* Enable UFE system */
   property = WUFE_SYS_EMPHY;
   if (ufe->fpga_mode == WT_SBI)
   {
      property |= WUFE_SYS_SBI_FULL;

      if (ufe->sbi_spe_line_type[0] == WUFE_SBI_TYPE_IDLE)
         property &= ~(WUFE_SYS_SBI_RX_1 | WUFE_SYS_SBI_TX_1);
      if (ufe->sbi_spe_line_type[1] == WUFE_SBI_TYPE_IDLE)
         property &= ~(WUFE_SYS_SBI_RX_2 | WUFE_SYS_SBI_TX_2);
      if (ufe->sbi_spe_line_type[2] == WUFE_SBI_TYPE_IDLE)
         property &= ~(WUFE_SYS_SBI_RX_3 | WUFE_SYS_SBI_TX_3);
#ifdef __WT_UFE3__
      if (ufe->sbi_spe_line_type[3] == WUFE_SBI_TYPE_IDLE)
         property &= ~(WUFE_SYS_SBI_RX_4 | WUFE_SYS_SBI_TX_4);
      if (ufe->sbi_spe_line_type[4] == WUFE_SBI_TYPE_IDLE)
         property &= ~(WUFE_SYS_SBI_RX_5 | WUFE_SYS_SBI_TX_5);
      if (ufe->sbi_spe_line_type[5] == WUFE_SBI_TYPE_IDLE)
         property &= ~(WUFE_SYS_SBI_RX_6 | WUFE_SYS_SBI_TX_6);
      if (ufe->sbi_spe_line_type[6] == WUFE_SBI_TYPE_IDLE)
         property &= ~(WUFE_SYS_SBI_RX_7 | WUFE_SYS_SBI_TX_7);
      if (ufe->sbi_spe_line_type[7] == WUFE_SBI_TYPE_IDLE)
         property &= ~(WUFE_SYS_SBI_RX_8 | WUFE_SYS_SBI_TX_8);
      if (ufe->sbi_spe_line_type[8] == WUFE_SBI_TYPE_IDLE)
         property &= ~(WUFE_SYS_SBI_RX_9 | WUFE_SYS_SBI_TX_9);
      if (ufe->sbi_spe_line_type[9] == WUFE_SBI_TYPE_IDLE)
         property &= ~(WUFE_SYS_SBI_RX_10 | WUFE_SYS_SBI_TX_10);
      if (ufe->sbi_spe_line_type[10] == WUFE_SBI_TYPE_IDLE)
         property &= ~(WUFE_SYS_SBI_RX_11 | WUFE_SYS_SBI_TX_11);
      if (ufe->sbi_spe_line_type[11] == WUFE_SBI_TYPE_IDLE)
         property &= ~(WUFE_SYS_SBI_RX_12 | WUFE_SYS_SBI_TX_12);
#endif
   }
   ufe_status = WUFE_SystemEnable(ufe->ufe_id, property, WUFE_FULL_DUPLEX);
   WT_UfeTerminateOnError(ufe_status, "WUFE_SystemEnable ",0,__LINE__);

   for (i=0; i<MAX_UFE_LINES_USED; i++)
      ufe->line_handle[i] = WT_UFE_HANDLE_INVALID;

   for (i=0; i<N_MAX_EMPHY_PHYS; i++)
      ufe->phy_handle[i] = WT_UFE_HANDLE_INVALID;

}

/**********************************************************************************
 **********************************************************************************
 ************                          LINE                   *********************
 **********************************************************************************
 *********************************************************************************/
void WT_UfeLineCreate(WT_ufe *ufe, WP_U32 index, void *config, WP_U8 enable)
{
   if (ufe->fpga_mode == WT_OCTAL)
      WT_UfeLineOctalCreate(ufe, index, (WT_ufe_line_octal_params*)config, enable);
   else if (ufe->fpga_mode == WT_SBI)
      WT_UfeLineSbiCreate(ufe, index, (WUFE_line_sbi*)config, enable);
   else
   {
      printf("The UFE FPGA mode is not supported %d\n", ufe->fpga_mode);
      exit(1);
   }
}

void WT_UfeLineDelete(WT_ufe *ufe, WP_U32 index)
{
   WUFE_status ufe_status;

   if(ufe->line_handle[index] == WT_UFE_HANDLE_INVALID)
      return;

   ufe_status = WUFE_LineDisable(ufe->line_handle[index], WUFE_FULL_DUPLEX);
   WT_UfeTerminateOnError(ufe_status, "WUFE_LineDisable", index, __LINE__);

   ufe_status = WUFE_LineDelete(ufe->line_handle[index]);
   WT_UfeTerminateOnError(ufe_status, "WUFE_LineDelete", index, __LINE__);

   ufe->line_handle[index] = WT_UFE_HANDLE_INVALID;
}

void WT_UfeClkRecRxCgenModify(WT_ufe *ufe, WP_U32 new_line_index)
{
   WUFE_status ufe_status;

   if(ufe->line_handle[new_line_index] == WT_UFE_HANDLE_INVALID)
      return;

   ufe_status = WUFE_ClockRecRxCgenModify(ufe->line_handle[new_line_index]);
   WT_UfeTerminateOnError(ufe_status, "WUFE_ClockRecRxCgenModify", new_line_index, __LINE__);

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

#if WTI_CAD_SYSTEM_LOOPBACK
   line_cfg->cad_loopback = WUFE_CAD_LOOPBACK_SYSTEM;
#endif

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
   line_cfg->clock_rec_mode = line_octal_params->clock_rec_mode;
   line_cfg->rx_clk_rec_if = line_octal_params->rx_clk_rec_if;
   line_cfg->ext_mode_active = line_octal_params->ext_mode_active;
   line_cfg->cad_loopback = WUFE_CAD_NORMAL;/*WUFE_CAD_NORMAL or WUFE_CAD_LOOPBACK_SYSTEM or WUFE_CAD_LOOPBACK_LINE*/
}

void  WT_UfeLineOctalCreate(WT_ufe *ufe,
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
}

/**********************************************************************************
 **********************************************************************************
 ************                    Line SBI                     *********************
 **********************************************************************************
 *********************************************************************************/
void WT_UfeLineSbiCreate(WT_ufe *ufe,
                         WP_U32 line_index,
                         WUFE_line_sbi *line_cfg,
                         WP_U8 enable)
{
   WUFE_status ufe_status;

   if (ufe->fpga_mode != WT_SBI)
   {
      printf("The UFE FPGA mode is not SBI. Can't create octal line\n");
      exit(1);
   }
   line_cfg->struct_id = WUFE_STRUCT_LINE_SBI;

   ufe_status = WUFE_LineCreate(&ufe->line_handle[line_index],
                                ufe->ufe_id,
                                WUFE_STRUCT_LINE_SBI,
                                line_cfg);

   WT_UfeTerminateOnError(ufe_status, "SBI WUFE_LineCreate", line_index, __LINE__);
   if (enable)
   {
      ufe_status = WUFE_LineEnable(ufe->line_handle[line_index], WUFE_FULL_DUPLEX);
      WT_UfeTerminateOnError(ufe_status, "SBI WUFE_LineEnable", line_index, __LINE__);
   }
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
                                 WP_U8 *slots_arr,
                                 WP_U32 spe,
                                 WP_U8 rx_clk_rec_mode)
{
   WUFE_SlotGroup slot_group;

   /************************************
    * Configure the UFE phy .
    ************************************/
   memset(phy_cfg, 0, sizeof(WUFE_phy));

   phy_cfg->struct_id = WUFE_STRUCT_PHY_TRANS;

   phy_cfg->rx_fifo_param.fifo_max_size  = 0xa;
   phy_cfg->tx_fifo_param.fifo_max_size  = 0x2;
   if (ufe->fpga_mode == WT_SBI &&
       (ufe->sbi_spe_line_type[spe] == WUFE_SBI_TYPE_DS3 ||
        ufe->sbi_spe_line_type[spe] == WUFE_SBI_TYPE_E3))
   {
      phy_cfg->tx_fifo_param.transmit_th    = 0x4;
      phy_cfg->tx_fifo_param.fifo_max_size  = 0xa;
      phy_cfg->tx_fifo_param.fast_mode = WUFE_PHY_FIFO_MODE_FAST;
   }
   else
   {
      phy_cfg->tx_fifo_param.transmit_th    = 0x1;
      phy_cfg->tx_fifo_param.fast_mode = WUFE_PHY_FIFO_MODE_SLOW;
   }

   phy_cfg->tx_fifo_param.wait_type      = WUFE_PHY_TX_WAIT_TYPE_A;
   phy_cfg->rx_fifo_param.priority_table = 0;
   phy_cfg->tx_fifo_param.priority_table = 0;

   /* configure the slots */
   memset (&slot_group, 0, sizeof(WUFE_SlotGroup));
   slot_group.n_slots = n_slots;
   memcpy(slot_group.slot_arr, slots_arr, n_slots*sizeof(WP_U8));

   memcpy(&phy_cfg->rx_slot_group,&slot_group,sizeof(slot_group));
   memcpy(&phy_cfg->tx_slot_group,&slot_group,sizeof(slot_group));

   phy_cfg->type.trans.rx_data_unit_size = data_unit_size;
   phy_cfg->type.trans.tx_underrun_interrupt_mode = WUFE_UFE_PHY_INT_ENABLE;
   if(rx_clk_rec_mode)
      phy_cfg->type.trans.rx_clk_rec_mode =WUFE_ENABLE;
   else
      phy_cfg->type.trans.rx_clk_rec_mode =WUFE_DISABLE;
}

void WT_UfePhyTransCreate(WT_ufe *ufe,
                          WP_U32 line_index,
                          WP_U32 phy_index,
                          WP_U32 n_slots,
                          WP_U8 *slots_arr,
                          WP_U32 ufe2_dataunit_size,
                          WP_U8 rx_clk_rec_mode)
{
   WUFE_phy phy_cfg;
   WUFE_status ufe_status;

   if (ufe->fpga_mode == WT_SBI)
   {
      WP_U32 spe_id;

      spe_id = line_index / N_UFE_SBI_SPE_LINE;

      WT_UfePhyTransConfig(ufe, &phy_cfg, ufe2_dataunit_size, n_slots,
                           slots_arr, spe_id, rx_clk_rec_mode);
   }
   else
   {
      WT_UfePhyTransConfig(ufe, &phy_cfg, ufe2_dataunit_size, n_slots,
                           slots_arr, 0, rx_clk_rec_mode);
   }

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   if (ufe->fpga_mode == WT_OCTAL)
   {
      phy_cfg.rx_fifo_param.priority_table = 2;
      phy_cfg.tx_fifo_param.priority_table = 2;
   }
#endif

   if (ufe->fpga_mode == WT_HMVIP)
   {
      phy_cfg.rx_slot_group.hmvip_id = phy_index % 4;
      phy_cfg.tx_slot_group.hmvip_id = phy_index % 4;
   }

   ufe_status = WUFE_PhyCreate(&ufe->phy_handle[phy_index],
                               ufe->line_handle[line_index],
                               &phy_cfg,
                               WUFE_FULL_DUPLEX);
   WT_UfeTerminateOnError(ufe_status, "WUFE_PhyCreate", phy_index, __LINE__);
}

void WT_UfePhyEnable(WT_ufe *ufe, WP_U32 phy_index)
{
   WUFE_status ufe_status;

   ufe_status = WUFE_PhyEnable(ufe->phy_handle[phy_index], WUFE_FULL_DUPLEX);
   WT_UfeTerminateOnError(ufe_status, "WUFE_PhyEnable", phy_index, __LINE__);
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

      WUFE_PhyDisplay(ufe->phy_handle[0]);
      WT_UfeTerminateOnError(ufe_status,"WUFE_PhyDelete not complete", timeout ,__LINE__);
   }

   ufe->phy_handle[phy_index] = WT_UFE_HANDLE_INVALID;

}

void WT_UfePhyRxClkRecReconfigure(WT_ufe *ufe, WP_U32 phy_index, WP_U32 enable)
{
   WP_handle phy_handle = ufe->phy_handle[phy_index];
   WUFE_phy usr_phy_cfg;
   WUFE_status ufe_status=WUFE_OK;
   volatile WP_U32 timeout;

   if(phy_handle == WT_UFE_HANDLE_INVALID)
      return;

   memset(&usr_phy_cfg, 0xff, sizeof(WUFE_phy));
   if (enable == 1)
      usr_phy_cfg.type.trans.rx_clk_rec_mode = WUFE_ENABLE;
   else
      usr_phy_cfg.type.trans.rx_clk_rec_mode = WUFE_DISABLE;

   timeout = 0;
   do
   {
      ufe_status = WUFE_PhyReconfigure(phy_handle, &usr_phy_cfg);
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
      WT_UfeTerminateOnError(ufe_status,"WUFE_PhyReconfigure not complete", timeout ,__LINE__);

      WUFE_PhyDisplay(ufe->phy_handle[0]);
      WT_UfeTerminateOnError(ufe_status,"WUFE_PhyReconfigure not complete", timeout ,__LINE__);
   }

}

static void WT_UfeAnalizeCoreEvents(WP_U32 coreid, WUFE_events_core *core_events)
{
   WP_U16 event = core_events->event_reg;

   printf("core %d event register 0x%04x\n", coreid, event);
   if (event & WUFE_EVENT_CORE)
   {
      if (event & WUFE_EVENT_CORE_INGRESS_FBP_OV)
         printf("WUFE_EVENT_CORE_INGRESS_FBP_OV\n");
      if (event & WUFE_EVENT_CORE_EGRESS_FBP_OV)
         printf("WUFE_EVENT_CORE_EGRESS_FBP_OV\n");
      if (event & WUFE_EVENT_CORE_ATM_SYNC_INDICATOR)
         printf("WUFE_EVENT_CORE_ATM_SYNC_INDICATOR\n");
      if (event & WUFE_EVENT_CORE_INGRESS_PHY_CR_UNDERRUN)
      {
         printf("WUFE_EVENT_CORE_INGRESS_PHY_CR_UNDERRUN\n");
         printf("rx_phy_cr_underrun_handle = 0x%x\n",
                core_events->rx_phy_underrun_handle);
      }
      if (event & WUFE_EVENT_CORE_EGRESS_PHY_UNDERRUN)
      {
         printf("WUFE_EVENT_CORE_EGRESS_PHY_UNDERRUN\n");
         printf("tx_phy_underrun_handle = 0x%x\n",
                core_events->tx_phy_underrun_handle);
      }
   }
}

static void WT_UfeAnalizeEmphyEvents(WUFE_events_emphy *emphy_events)
{
   WP_U16 event = emphy_events->event_reg;

   printf("emphy event register 0x%04x\n", event);
   if (event & WUFE_EVENT_EMPHY)
   {
      if (event & WUFE_EVENT_EMPHY_INGRESS_UN)
         printf("WUFE_EVENT_EMPHY_INGRESS_UN\n");
      if (event & WUFE_EVENT_EMPHY_INGRESS_BUS)
         printf("WUFE_EVENT_EMPHY_INGRESS_BUS\n");
      if (event & WUFE_EVENT_EMPHY_INGRESS_IF)
         printf("WUFE_EVENT_EMPHY_INGRESS_IF\n");
#ifndef __WT_UFE3__
      if (event & WUFE_EVENT_EMPHY_EGRESS_PHY_STUCK)
      {
         printf("WUFE_EVENT_EMPHY_EGRESS_PHY_STUCK\n");
         printf("phy_stuck_handle = 0x%x\n",
                emphy_events->phy_stuck_handle);
      }
#endif
      if (event & WUFE_EVENT_EMPHY_EGRESS_PHY_NOT_VALID)
      {
         printf("WUFE_EVENT_EMPHY_EGRESS_PHY_NOT_VALID\n");
         printf("phy_not_valid_handle = 0x%x\n",
                emphy_events->phy_not_valid_handle);
      }
      if (event & WUFE_EVENT_EMPHY_EGRESS_OV)
         printf("WUFE_EVENT_EMPHY_EGRESS_OV\n");
      if (event & WUFE_EVENT_EMPHY_EGRESS_BUS)
         printf("WUFE_EVENT_EMPHY_EGRESS_BUS\n");
      if (event & WUFE_EVENT_EMPHY_EGRESS_IF )
         printf("WUFE_EVENT_EMPHY_EGRESS_IF\n");
      if (event &WUFE_EVENT_EMPHY_EGRESS_PARITY )
         printf("WUFE_EVENT_EMPHY_EGRESS_PARITY\n");
   }
}

static void WT_UfeAnalizeLiuEvents(WT_ufe *ufe)
{
   WUFE_events* ufe_events = &ufe->ufe_events;
   WUFE_events_liu *liu_events = &ufe_events->liu_events;
   WP_U16 event = liu_events->event_reg;

   if (event & WUFE_EVENT_LIU)
   {
      printf("Raised Global events: \n");

      if (event &WUFE_EVENT_LIU_INGRESS )
         printf("WUFE_EVENT_LIU_INGRESS\n");
      if (event & WUFE_EVENT_LIU_EGRESS)
         printf("WUFE_EVENT_LIU_EGRESS\n");
   }

   if (ufe->fpga_mode == WT_SBI)
   {
      event = liu_events->sbi_events->event_reg;

      if(event)
      {
         printf("Raised SBI events: \n");
         if (event & WUFE_EVENT_SBI_V5_SYNC_LOSS_CORE0)
            printf("WUFE_EVENT_SBI_V5_SYNC_LOSS_CORE0\n");
         if (event & WUFE_EVENT_SBI_RX_SYNC_LOSS_CORE0)
            printf("WUFE_EVENT_SBI_RX_SYNC_LOSS_CORE0\n");
         if (event & WUFE_EVENT_SBI_TX_AC1FP_SYNC_LOSS)
            printf("WUFE_EVENT_SBI_TX_AC1FP_SYNC_LOSS\n");
         if (event & WUFE_EVENT_SBI_PARITY_ERROR_CORE0)
            printf("WUFE_EVENT_SBI_PARITY_ERROR_CORE0\n");
#ifdef __WT_UFE3__
         if (event & WUFE_EVENT_SBI_V5_SYNC_LOSS_CORE1)
            printf("WUFE_EVENT_SBI_V5_SYNC_LOSS_CORE1\n");
         if (event & WUFE_EVENT_SBI_RX_SYNC_LOSS_CORE1)
            printf("WUFE_EVENT_SBI_RX_SYNC_LOSS_CORE1\n");
         if (event & WUFE_EVENT_SBI_PARITY_ERROR_CORE1)
            printf("WUFE_EVENT_SBI_PARITY_ERROR_CORE1\n");
         if (event & WUFE_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE0)
            printf("WUFE_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE0\n");
         if (event & WUFE_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE1)
            printf("WUFE_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE1\n");
         if (event & WUFE_EVENT_SBI_RX_ALM_INDICATOR_CORE0)
            printf("WUFE_EVENT_SBI_RX_ALM_INDICATOR_CORE0\n");
         if (event & WUFE_EVENT_SBI_RX_ALM_INDICATOR_CORE1)
            printf("WUFE_EVENT_SBI_RX_ALM_INDICATOR_CORE1\n");
         if (event & WUFE_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE0)
            printf("WUFE_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE0\n");
         if (event & WUFE_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE1)
            printf("WUFE_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE1\n");
         if (event & WUFE_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE0)
            printf("WUFE_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE0\n");
         if (event & WUFE_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE1)
            printf("WUFE_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE1\n");
#endif
      }

   }
   else
   {
      int i;

      for (i=0; i<WUFE_CAD_EVENTS; i++)
      {
         if (liu_events->cad_events[i].rx_event_reg)
         {
            event = liu_events->cad_events[i].rx_event_reg;
            printf("Raised CAD RX events: \n");
            if (event & WUFE_EVENT_CAD_RX_OVERRUN)
               printf("CAD %d: WUFE_EVENT_CAD_RX_OVERRUN\n", i);
            if (event & WUFE_EVENT_CAD_RX_SYNC_LOSS)
               printf("CAD %d: WUFE_EVENT_CAD_RX_SYNC_LOSS\n", i);
         }
         if (liu_events->cad_events[i].tx_event_reg)
         {
            event = liu_events->cad_events[i].tx_event_reg;

            printf("Raised CADX  events: \n");
            if (event & WUFE_EVENT_CAD_TX_UNDERRUN)
               printf("CAD %d: WUFE_EVENT_CAD_TX_UNDERRUN\n", i);
            if (event & WUFE_EVENT_CAD_TX_SYNC_LOSS)
               printf("CAD %d: WUFE_EVENT_CAD_TX_SYNC_LOSS\n", i);
         }
      }
   }
}

static void WT_UfeAnalizeCrEvents(WT_ufe *ufe)
{
   WUFE_events* ufe_events = &ufe->ufe_events;
   WUFE_events_clock_recovery *cr_events = &ufe_events->cr_events;
   WP_U16 event;

   if (ufe->fpga_mode == WT_SBI)
   {
      event = cr_events->event_reg;

      if(event & WUFE_EVENT_CR_CT_COUNTER_OVERFLOW)
      {
         int i;

         printf("Raised CR events:\n");

         printf("WUFE_EVENT_CR_CT_COUNTER_OVERFLOW\n");

         for (i = 0; i < ufe->max_sbi_clock_translators; i++)
         {
            event = cr_events->ct_events[i];
            if (event)
               printf("Clock Translator Event for CT ID: %d.\n", i);
         }
      }
   }
}

static void WT_UfeAnalizeEvents(WT_ufe *ufe)
{
   WP_U32 coreid;
   WUFE_events *ufe_events = &ufe->ufe_events;

   for(coreid=0; coreid<N_MAX_UFE_CORES; coreid++)
   {
      WT_UfeAnalizeCoreEvents(coreid, &(ufe_events->core_events[coreid]));
      WT_UfeAnalizeEmphyEvents(&(ufe_events->emphy_events[coreid]));
   }

   WT_UfeAnalizeLiuEvents(ufe);
   WT_UfeAnalizeCrEvents(ufe);

}

WP_U32 WT_UfeAnalyzeEvents4Automation(WT_ufe *ufe)
{
   WP_U32 coreid;
   WUFE_events *ufe_events = &ufe->ufe_events;

   for (coreid=0; coreid<N_MAX_UFE_CORES; coreid++)
   {
      if ( (ufe_events->core_events[coreid].event_reg) & WUFE_EVENT_CORE) return WT_FAIL;

      if ( (ufe_events->emphy_events[coreid].event_reg) & WUFE_EVENT_EMPHY) return WT_FAIL;

   }
   if ( (ufe_events->liu_events.event_reg) & WUFE_EVENT_LIU ) return WT_FAIL;

   if ( ufe->fpga_mode == WT_SBI )
   {
      if( (ufe_events->liu_events.sbi_events->event_reg) ) return WT_FAIL;

      if( (ufe_events->cr_events.event_reg) & WUFE_EVENT_CR_CT_COUNTER_OVERFLOW ) return WT_FAIL;
   }
   else
   {
      int i;

      for (i=0; i<WUFE_CAD_EVENTS; i++)
      {
         if ( (ufe_events->liu_events.cad_events[i].rx_event_reg) || (ufe_events->liu_events.cad_events[i].tx_event_reg) ) return 1;
      }
   }
   return WT_PASS;
}

void WT_UfeReadEvents(WT_ufe *ufe)
{
   WUFE_status status;
   WUFE_events* ufe_events = &ufe->ufe_events;

   memset(ufe_events, 0, sizeof(WUFE_events));

   status = WUFE_UfeReadEvents(ufe->ufe_id, ufe_events);
   WT_UfeTerminateOnError(status, "WUFE_UfeReadEvents  fail ", 0, __LINE__);

   WT_UfeAnalizeEvents(ufe);

}

WP_U32 WT_UfeReadEvents4Automation(WT_ufe *ufe)
{
   WUFE_status status;
   WUFE_events* ufe_events = &ufe->ufe_events;
   WP_U32 result;

   memset(ufe_events, 0, sizeof(WUFE_events));

   status = WUFE_UfeReadEvents(ufe->ufe_id, ufe_events);
   WT_UfeTerminateOnError(status, "WUFE_UfeReadEvents  fail ", 0, __LINE__);

   WT_UfeAnalizeEvents(ufe);

   result = WT_UfeAnalyzeEvents4Automation(ufe);

   return result;
}

void  WT_UfeRelease(WT_ufe *ufe)
{
   WUFE_status ufe_status;

   ufe_status = WUFE_UfeRelease(ufe->ufe_id);
   WT_UfeTerminateOnError( ufe_status, "WUFE_UfeRelease (from WT_UfeRelease)",0,__LINE__);
}

void WT_UfeSystemSbiModify(WT_ufe *ufe, WUFE_sbi_global_reconfig *new_config)
{
   WUFE_status ufe_status;

   ufe_status = WUFE_SystemReconfigure(ufe->ufe_id, WUFE_SYSTEM_RECONFIG_SBI, new_config);
   WT_UfeTerminateOnError( ufe_status, "WUFE_Ufe2SystemReconfigure" ,0,__LINE__);
}

void WT_UfeLineSbiModify(WT_ufe *ufe, int line_index, WUFE_line_sbi_reconfig *config)
{
   WUFE_status ufe_status;

   ufe_status = WUFE_LineReconfigure(ufe->line_handle[line_index],
                                     WUFE_STRUCT_LINE_SBI,
                                     config);
   WT_UfeTerminateOnError(ufe_status, "WUFE_LineReconfigure" ,line_index,__LINE__);
}

void WT_UfeClockRecMasterSet(WUFE_handle ufe_phy_handle,
                             WP_U8 tx_data_unit_size)
{
   WUFE_status ufe_status;

   ufe_status = WUFE_ClockRecMasterSet(ufe_phy_handle, tx_data_unit_size);
   if (ufe_status != WUFE_OK && ufe_status != WUFE_ERR_CLOCK_REC_INVALID_TS_WIDTH)
   {
      WT_UfeTerminateOnError(ufe_status, "WT_UfeClockRecMasterSet" ,0,__LINE__);
   }
   else
   {
      if(ufe_status == WUFE_ERR_CLOCK_REC_INVALID_TS_WIDTH)
      {
         printf("**********************************************************\n");
         printf("**** WARNING:WUFE_ERR_CLOCK_REC_INVALID_TS_WIDTH !!!! ****\n");
         printf("**********************************************************\n");

      }

   }

}

void WT_UfeClockRecInfoGet(WP_U32 wufe_id,
                           WUFE_handle ufe_phy_handle,
                           WUFE_clock_rec_info_type info_type,
                           void* clk_rec_info)
{
   WUFE_status ufe_status;

   ufe_status = WUFE_ClockRecInfo(wufe_id,
                                  ufe_phy_handle,
                                  info_type,
                                  clk_rec_info);
   WT_UfeTerminateOnError(ufe_status, "WT_UfeClockRecInfoGet" ,0,__LINE__);
}

void WT_UfeClockRecIndirectCgenRead(WP_U16 entry)
{
   WP_U32 wufe_id = 0;
   WP_U16 return_val = 0;

   return_val = WUFE_ClockRecCgenRamEntryRead(wufe_id, entry);
   if (return_val == 0xFFFF)
   {
      printf("Warning! WUFE_ClockRecCgenRamEntryRead is not valid in UFE3 mode!\n");
   }
}

void WT_UfeClockRecIndirectPortRamRead(WP_U16 clk_rec_if, WP_U16 entry)
{
   WP_U32 wufe_id = 0;
   WP_U16 return_val = 0;

   return_val = WUFE_ClockRecRxPortRamEntryRead(wufe_id, clk_rec_if, entry);
   if (return_val == 0xFFFF)
   {
      printf("Warning! WUFE_ClockRecRxPortRamEntryRead is not valid in UFE3 mode!\n");
   }
}

void WT_UfeClockRecIndirectCtRead(WP_U16 entry)
{
   WP_U32 wufe_id = 0;
   WP_U16 return_val = 0;

   return_val = WUFE_ClockRecCtRamEntryRead(wufe_id, entry);
   if (return_val == 0xFFFF)
   {
      printf("Warning! WT_UfeClockRecIndirectCtRead is not valid in UFE3 mode!\n");
   }
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
