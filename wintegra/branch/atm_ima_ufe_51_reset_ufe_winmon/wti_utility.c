
enum
{
      WTI_BOARD_CONFIG_NA = 0,
         WTI_DEFAULT_PIN_CONFIG,
	    WTI_PTP_TWO_BOARDS_CONFIG,
	       WTI_SYNC_ETH_CONFIG
};

#define WT_CLOCK_REC_DIFF_REF_CLOCK                 125000000
WP_U32 global_clock_rec_diff_ref_clock = WT_CLOCK_REC_DIFF_REF_CLOCK;


void WTI_ZarlinkPllSetup (WPX_pll_cfg * pllConfigParam)
{
   // pllConfigParam->wpid = the_system->wpid;
   pllConfigParam->wpid = 0;

   switch (global_clock_rec_diff_ref_clock)
   {
   case 77760000:
      pllConfigParam->apll_rate = wpx_f_77_76;
      pllConfigParam->clock_out = wpx_apll_clk0;
      break;
   case 38880000:
      pllConfigParam->apll_rate = wpx_f_38_88;
      pllConfigParam->clock_out = wpx_apll_clk0;
      break;
   case 19440000:
      pllConfigParam->apll_rate = wpx_f_19_44;
      pllConfigParam->clock_out = wpx_apll_clk0;
      break;
   case 9720000:
      pllConfigParam->apll_rate = wpx_f_9_72;
      pllConfigParam->clock_out = wpx_apll_clk0;
      break;
   case 51840000:
      pllConfigParam->apll_rate = wpx_f_51_84;
      pllConfigParam->clock_out = wpx_apll_clk0;
      break;
   case 25920000:
      pllConfigParam->apll_rate = wpx_f_25_92;
      pllConfigParam->clock_out = wpx_apll_clk0;
      break;
   case 12960000:
      pllConfigParam->apll_rate = wpx_f_12_96;
      pllConfigParam->clock_out = wpx_apll_clk0;
      break;
   case 6480000:
      pllConfigParam->apll_rate = wpx_f_6_48;
      pllConfigParam->clock_out = wpx_apll_clk0;
      break;
   case 125000000:
      pllConfigParam->apll_rate = wpx_f_125;
      pllConfigParam->clock_out = wpx_apll_clk0;
      break;
   case 6250000:
      pllConfigParam->apll_rate = wpx_f_62_5;
      pllConfigParam->clock_out = wpx_apll_clk0;
      break;
   case 50000000:
      pllConfigParam->apll_rate = wpx_f_50;
      pllConfigParam->clock_out = wpx_apll_clk0;
      break;
   case 25000000:
      pllConfigParam->apll_rate = wpx_f_25;
      pllConfigParam->clock_out = wpx_apll_clk0;
      break;
   case 12500000:
      pllConfigParam->apll_rate = wpx_f_12_5;
      pllConfigParam->clock_out = wpx_apll_clk0;
      break;
   default:
      pllConfigParam->clock_rate = global_clock_rec_diff_ref_clock / 1000;
      pllConfigParam->clock_out = wpx_p0_clk0;
   }
   printf ("********************************************\n");
   printf ("SMA1 reference clock set to %d\n",
           global_clock_rec_diff_ref_clock);
   printf ("********************************************\n\n");
}

/*****************************************************************************
 * Function name: WTI_BoardPinConfig
  * Description  : configure On WDS3 clocks mux using the on-board FPGA
   * Input  params: configuration, board_id;
    * Output params:
     * Return val   : none
      *****************************************************************************/
void WTI_BoardPinConfig (WP_U32 configuration, WP_U32 board_id,
                         WP_U32 pll_clock_out)
{
#if defined(WP_HW_WINPATH3) || defined(WP_HW_WINPATH4)
   WP_status status;
   wpx_wp_cr_pin_io wp_cr_pin_io = { 0 };

   if (configuration == WTI_DEFAULT_PIN_CONFIG)
   {
      /* Adaptive clock through synt clock,
         differential clock through SMA6, 1 PTP domain, in and out,
         SMA1 extract TCXO as output clock. */
      wp_cr_pin_io.brg_b = WPX_FPGA_NA;   //WPX_FPGA_BRG_B_IN_SMA5;
      wp_cr_pin_io.brg_c = WPX_FPGA_BRG_C_IN_SYNT_CLK;
      wp_cr_pin_io.brg_d = WPX_FPGA_BRG_D_IN_SMA6;
      wp_cr_pin_io.clk_rec_fclk1 = WPX_FPGA_CR_FCLK1_OUT_SMA5;
      wp_cr_pin_io.clk_rec_fclk2 = WPX_FPGA_CR_FCLK2_OUT_SMA4;
      wp_cr_pin_io.clk_rec_pps1_output = WPX_FPGA_NA;
      wp_cr_pin_io.clk_rec_pps1_input = WPX_FPGA_CR_PPS1_IN_SMA3;
      wp_cr_pin_io.clk_rec_pps2_output = WPX_FPGA_CR_PPS2_OUT_SMA2;
      wp_cr_pin_io.clk_rec_pps2_input = WPX_FPGA_NA;
      wp_cr_pin_io.sync_eth_clk0_output = WPX_FPGA_NA;
      wp_cr_pin_io.sync_eth_clk1_output = WPX_FPGA_NA;
#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)
      if (wpx_apll_clk0 == pll_clock_out)
      {
         wp_cr_pin_io.sma1_out = WPX_FPGA_SMA1_OUT_APLL0_CLK;
      }
      else
      {
         wp_cr_pin_io.sma1_out = WPX_FPGA_SMA1_OUT_P0_CLK0;
      }
#else
      wp_cr_pin_io.sma1_out = WPX_FPGA_SMA1_OUT_APLL0_CLK;
#endif
      wp_cr_pin_io.serdes_x_select = WPX_FPGA_SERDES_CLK_APLL0;   /* The serdes output is taken from the external APLL output */
      wp_cr_pin_io.serdes1_ref_clk = WPX_FPGA_SERDES_CLK_125;  /* relevant in case serdes_x_select = WPX_FPGA_SERDES_SELECT_CLK */
      wp_cr_pin_io.serdes0_ref_clk = WPX_FPGA_SERDES_CLK_125;  /* relevant in case serdes_x_select = WPX_FPGA_SERDES_SELECT_CLK */

   }
   else if (configuration == WTI_PTP_TWO_BOARDS_CONFIG)
   {
      if (board_id)
      {
         /* BOARD #1: all inputs */
         wp_cr_pin_io.brg_b = WPX_FPGA_BRG_B_IN_SMA5;
         wp_cr_pin_io.brg_c = WPX_FPGA_NA;
         wp_cr_pin_io.brg_d = WPX_FPGA_BRG_D_IN_SMA4;
         wp_cr_pin_io.clk_rec_fclk1 = WPX_FPGA_NA;
         wp_cr_pin_io.clk_rec_fclk2 = WPX_FPGA_NA;
         wp_cr_pin_io.clk_rec_pps1_output = WPX_FPGA_NA;
         wp_cr_pin_io.clk_rec_pps1_input = WPX_FPGA_CR_PPS1_IN_SMA3;
         wp_cr_pin_io.clk_rec_pps2_output = WPX_FPGA_NA;
         wp_cr_pin_io.clk_rec_pps2_input = WPX_FPGA_CR_PPS2_IN_SMA2;
         wp_cr_pin_io.sync_eth_clk0_output = WPX_FPGA_NA;
         wp_cr_pin_io.sync_eth_clk1_output = WPX_FPGA_NA;
         wp_cr_pin_io.sma1_out = WPX_FPGA_SMA1_OUT_TCXO;
         wp_cr_pin_io.serdes_x_select =
            WPX_FPGA_SERDES_CLK_APLL0
            /* The serdes output is taken from the external APLL output */
            ;
         wp_cr_pin_io.serdes1_ref_clk = WPX_FPGA_SERDES_CLK_125;  /* relevant in case serdes_x_select = WPX_FPGA_SERDES_SELECT_CLK */
         wp_cr_pin_io.serdes0_ref_clk = WPX_FPGA_SERDES_CLK_125;  /* relevant in case serdes_x_select = WPX_FPGA_SERDES_SELECT_CLK */
      }
      else
      {
         /* BOARD #2: all outputs */
         wp_cr_pin_io.brg_b = WPX_FPGA_BRG_B_IN_SMA5;
         wp_cr_pin_io.brg_c = WPX_FPGA_NA;
         wp_cr_pin_io.brg_d = WPX_FPGA_NA;
         wp_cr_pin_io.clk_rec_fclk1 = WPX_FPGA_CR_FCLK1_OUT_SMA5;
         wp_cr_pin_io.clk_rec_fclk2 = WPX_FPGA_CR_FCLK2_OUT_SMA4;
         wp_cr_pin_io.clk_rec_pps1_output = WPX_FPGA_CR_PPS1_OUT_SMA3;
         wp_cr_pin_io.clk_rec_pps1_input = WPX_FPGA_NA;
         wp_cr_pin_io.clk_rec_pps2_output = WPX_FPGA_CR_PPS2_OUT_SMA2;
         wp_cr_pin_io.clk_rec_pps2_input = WPX_FPGA_NA;
         wp_cr_pin_io.sync_eth_clk0_output = WPX_FPGA_NA;
         wp_cr_pin_io.sync_eth_clk1_output = WPX_FPGA_NA;
         wp_cr_pin_io.sma1_out = WPX_FPGA_SMA1_OUT_TCXO;
         wp_cr_pin_io.serdes_x_select =
            WPX_FPGA_SERDES_CLK_APLL0
            /* The serdes output is taken from the external APLL output */
            ;
         wp_cr_pin_io.serdes1_ref_clk = WPX_FPGA_SERDES_CLK_125;  /* relevant in case serdes_x_select = WPX_FPGA_SERDES_SELECT_CLK */
         wp_cr_pin_io.serdes0_ref_clk = WPX_FPGA_SERDES_CLK_125;  /* relevant in case serdes_x_select = WPX_FPGA_SERDES_SELECT_CLK */
      }
   }
   else if (configuration == WTI_SYNC_ETH_CONFIG)
   {
      wp_cr_pin_io.brg_b = WPX_FPGA_BRG_B_IN_SMA5;
      wp_cr_pin_io.brg_c = WPX_FPGA_BRG_C_IN_SYNT_CLK;
      wp_cr_pin_io.brg_d = WPX_FPGA_BRG_D_IN_SMA6;
      wp_cr_pin_io.clk_rec_fclk1 = WPX_FPGA_NA;
      wp_cr_pin_io.clk_rec_fclk2 = WPX_FPGA_CR_FCLK2_OUT_SMA4;
      wp_cr_pin_io.clk_rec_pps1_output = WPX_FPGA_NA;
      wp_cr_pin_io.clk_rec_pps1_input = WPX_FPGA_CR_PPS1_IN_SMA3;
      wp_cr_pin_io.clk_rec_pps2_output = WPX_FPGA_NA;
      wp_cr_pin_io.clk_rec_pps2_input = WPX_FPGA_NA;
      wp_cr_pin_io.sma1_out = WPX_FPGA_SMA1_OUT_APLL0_CLK;
      if (board_id)
      {
         wp_cr_pin_io.sync_eth_clk0_output = WPX_FPGA_NA;
         wp_cr_pin_io.sync_eth_clk1_output = WPX_FPGA_SYNC_ETH1_OUT_SMA2;
      }
      else
      {
         wp_cr_pin_io.sync_eth_clk0_output = WPX_FPGA_SYNC_ETH0_OUT_SMA2;
         wp_cr_pin_io.sync_eth_clk1_output = WPX_FPGA_NA;
      }
      wp_cr_pin_io.serdes_x_select =
         WPX_FPGA_SERDES_CLK_APLL0
         /* The serdes output is taken from the external APLL output */ ;
      wp_cr_pin_io.serdes1_ref_clk = WPX_FPGA_SERDES_CLK_125;  /* relevant in case serdes_x_select = WPX_FPGA_SERDES_SELECT_CLK */
      wp_cr_pin_io.serdes0_ref_clk = WPX_FPGA_SERDES_CLK_125;  /* relevant in case serdes_x_select = WPX_FPGA_SERDES_SELECT_CLK */
   }
   else
   {
      printf ("WTI_BoardPinConfig: No confoguration selected\n");
      // WTI_Reboot ();
      WPX_Reboot ();
   }

   wp_cr_pin_io.sync_eth_clk0_output = WPX_FPGA_NA;   /* out through ext. pll. sources muxes */
   wp_cr_pin_io.sync_eth_clk1_output = WPX_FPGA_NA;   /* out through ext. pll. sources muxes */
   wp_cr_pin_io.pll_ref_clk1_sel = WPX_FPGA_PLL_REFCLK1_CLK_REC_FCLK_1;
   wp_cr_pin_io.pll_ref_clk2_sel = WPX_FPGA_PLL_REFCLK2_CLK_REC_FCLK_2;
   /* Set PLL source (entire system clock inc DCO source) to synt clock (board's TCXO/miniOCXO) */
   wp_cr_pin_io.osci_sel = WPX_FPGA_OSCI_SOURCE_SYNT_CLK;   /*  or WPX_FPGA_OSCI_SOURCE_SMA7 */

   status = WPX_CrBoardPinConfig (0, wp_cr_pin_io);
   // status = WPX_CrBoardPinConfig (the_system->wpid, wp_cr_pin_io);
   WT_UfeTerminateOnError(status, "WUFE_SystemConfig",0, __LINE__);
   // WTI_TerminateOnError (status, "WPX_CrBoardPinConfig()", __LINE__);
#endif /* WP_HW_WINPATH3 */
}
