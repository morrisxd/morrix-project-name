/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wti_cesop_defines.h
 *
 * Purpose: provide cesop definitions.
 *
 ****************************************************************************/

#ifndef WTI_CESOP_DEFINES_H
#define WTI_CESOP_DEFINES_H

 /* iw system type :        */
/* WTI_IW_SYSTEM_ROUTING when DPS package support routing (msa_pwe3)        */
/* WTI_IW_SYSTEM_BRIDGE  when DPS package support bridging (msa_pwe3_bridge)*/
#if WTI_CESOP_MEF8_ENCAP
#define WTI_IW_SYSTEM_MODE    WTI_IW_SYSTEM_BRIDGE  /* must be WTI_IW_SYSTEM_BRIDGE */
#elif WTI_TEST_RAISECOM_DCR
#define WTI_IW_SYSTEM_MODE    WTI_IW_SYSTEM_BRIDGE  /* must be WTI_IW_SYSTEM_BRIDGE */
#else
#define WTI_IW_SYSTEM_MODE    WTI_IW_SYSTEM_ROUTING  /* WTI_IW_SYSTEM_ROUTING or WTI_IW_SYSTEM_BRIDGE */
#endif

/********************************** Clock recovery define *************************************/
#define WTI_CLOCK_REC_DCO_PPM_OFFSET               100       /* maximum allow ppm offset from nominal service clock frequency */
#define WTI_CLOCK_REC_NOMINAL_FREQ_E1              2048000   /* Nominal frequency for E1  */
#define WTI_CLOCK_REC_NOMINAL_FREQ_T1              1544000   /* Nominal frequency for T1  */
#define WTI_CLOCK_REC_NOMINAL_FREQ_E3              34368000  /* Nominal frequency for E3  */
#define WTI_CLOCK_REC_NOMINAL_FREQ_DS3             44736000  /* Nominal frequency for DS3 */

#define WTI_TIMESTAMP_PRESCALER_ENABLE              0 /* When this bit is set the TS period (and prescalar) can be change by setting WTI_TS_PRESCALER */
#define WTI_TS_DIVISOR_PRESCALER                    2 /* Divisor range 1 to 32768  i.e. DIVISOR = 2  internal clock = 200M -> 2/200M = 10 nsec*/

#define UDP_DES_PORT_DFC_RULE                       0 /* When this bit is set the DFC rule include UDP   */
                                                      /* destnation  port. this mode is used to work with*/
                                                      /* number of PW and nistnet (Need to respond to ARP)*/
#define UDP_DES_PORT_PCE_RULE                       0 /* When this bit is set the PCE rule include UDP   */
                                                      /* destnation  port. this mode is used to work with*/
                                                      /* number of PW and nistnet (Need to respond to ARP)*/

#define WTI_MAX_ABS_PW                              WTI_MAX_PW


/* CR TDI WP2 address */
#if WTI_CLOCK_REC_TDI_LINES_4_7
#define WTI_TDI_RX_TS_BASE_ADDRESS                  0x1DC00000 /* 0xbdaxxx is for UPI1 */
#define WTI_TDI_TX_TS_BASE_ADDRESS                  0x1DC00800 /* 0xbdcxxx is for UPI2 */
#define WTI_TDI_RX_TS_INTERRUPT_ADDRESS             0x1DC00400
#define WTI_TDI_TX_TS_INTERRUPT_ADDRESS             0x1DC00C00
#else
#define WTI_TDI_RX_TS_BASE_ADDRESS                  0x1DA00000 /* 0xbdaxxx is for UPI1 */
#define WTI_TDI_TX_TS_BASE_ADDRESS                  0x1DA00800 /* 0xbdcxxx is for UPI2 */
#define WTI_TDI_RX_TS_INTERRUPT_ADDRESS             0x1DA00400
#define WTI_TDI_TX_TS_INTERRUPT_ADDRESS             0x1DA00C00
#endif /* WTI_CLOCK_REC_TDI_LINES_4_7 */

#if TWO_BOARDS_SIMULATION

#if WTI_CESOP_TDI
#ifdef WP_HW_WINPATH2
#define WT_CLOCK_REC_REG_DCO_REF_CLOCK              300000000
#else  /* WP_HW_WINPATH3  (not WP_HW_WINPATH2) */
#define WT_CLOCK_REC_REG_DCO_REF_CLOCK              125000000
#endif /* WP_HW_WINPATH2  (not WP_HW_WINPATH2) */
#else   /* not WTI_CESOP_TDI  */
#if defined __WT_UFE412__ || defined __WT_UFE448__
#define WT_CLOCK_REC_REG_DCO_REF_CLOCK              125000000
#else   /* not __WT_UFE412__  */
#define WT_CLOCK_REC_REG_DCO_REF_CLOCK              100000000
#endif
#endif /* not WTI_CESOP_TDI */

#ifdef WTI_BOARD_1
#define WT_CLOCK_REC_DIFF_REF_CLOCK                 125000000 /* 100 MHz local differential reference clock */
#define WT_CLOCK_REC_REMOTE_DIFF_REF_CLOCK          WT_CLOCK_REC_DIFF_REF_CLOCK /* remote differential reference clock */
#else
#define WT_CLOCK_REC_DIFF_REF_CLOCK                 125000000 /* 100 MHz local differential reference clock */
#define WT_CLOCK_REC_REMOTE_DIFF_REF_CLOCK          WT_CLOCK_REC_DIFF_REF_CLOCK /* remote differential reference clock */
#endif

#else /* one board in used */

#ifdef WP_HW_WINPATH2
/* WP_HW_WINPATH2 */
#define WT_CLOCK_REC_DIFF_REF_CLOCK                 100000000 /* 100 MHz local differential reference clock */
#define WT_CLOCK_REC_REMOTE_DIFF_REF_CLOCK          WT_CLOCK_REC_DIFF_REF_CLOCK /* remote differential reference clock */

#if WTI_CESOP_TDI
#define WT_CLOCK_REC_REG_DCO_REF_CLOCK              300000000 /* 300 MHz DCO reference clock for
                                                                 regular differential operation mode
                                                                 or adaptive operation mode.*/
#else /* Not WTI_CESOP_TDI*/
#define WT_CLOCK_REC_REG_DCO_REF_CLOCK              100000000 /* 100 MHz DCO reference clock for
                                                                 regular differential operation mode
                                                                 or adaptive operation mode.
                                                                 This field is synthesis depended. */
#endif /* Not WTI_CESOP_TDI*/

#else /* WP_HW_WINPATH3 */

#if WTI_CESOP_TDI
#define WT_CLOCK_REC_REG_DCO_REF_CLOCK              100000000 /* 125MHz DCO reference clock for
                                                                 regular differential operation mode
                                                                 or adaptive operation mode.*/
#if WTI_CI_TESTING_MODE_TDI
#define WT_CLOCK_REC_DIFF_REF_CLOCK                 100000000 /* in CI mode - 100MHz is created from PTPclock */
#else
#define WT_CLOCK_REC_DIFF_REF_CLOCK                 125000000 /* WP3 - 125 MHz local differential reference clock */
#endif

#define WT_CLOCK_REC_REMOTE_DIFF_REF_CLOCK          WT_CLOCK_REC_DIFF_REF_CLOCK /* remote differential reference clock */
#else /* not WTI_CESOP_TDI*/
#if defined __WT_UFE412__ || defined __WT_UFE448__

#if WTI_CI_TESTING_MODE_UFE4 
#define WT_CLOCK_REC_DIFF_REF_CLOCK                 100000000 /* in CI mode - 100MHz is created from PTPclock */
#else
#define WT_CLOCK_REC_DIFF_REF_CLOCK                125000000  /*  WP3 - 125 MHz local differential reference clock*/
#endif

#define WT_CLOCK_REC_REMOTE_DIFF_REF_CLOCK          WT_CLOCK_REC_DIFF_REF_CLOCK /* remote differential reference clock */
#if WTI_DUAL_EMPHY
#define WT_CLOCK_REC_REG_DCO_REF_CLOCK              WP_DISABLE
#else
#define WT_CLOCK_REC_REG_DCO_REF_CLOCK              WP_DISABLE
#endif
#else  /* UFE3 */
#define WT_CLOCK_REC_REG_DCO_REF_CLOCK              100000000 /* 100MHz DCO reference clock for
                                                                 regular differential operation mode
                                                                 or adaptive operation mode.
                                                                 This field is synthesis depended. */
#define WT_CLOCK_REC_DIFF_REF_CLOCK                 100000000 /* WP3 - 100MHz local differential reference clock */
#define WT_CLOCK_REC_REMOTE_DIFF_REF_CLOCK          WT_CLOCK_REC_DIFF_REF_CLOCK /* remote differential reference clock */
#endif
#endif /* not WTI_CESOP_TDI*/
#endif /* WP_HW_WINPATH3 */
#endif /* one board in used */




#define WT_CLOCK_REC_EXT_DIFF_DCO_REF_CLOCK         100000000 /* 100 MHz DCO reference clock for
                                                                 extended differential operation mode.
                                                                 This field is synthesis depended.
                                                                 DCO_REF_CLOCK/DIFF_REF_CLOCK must be
                                                                 a power of 2. */
#define WT_CLOCK_REC_ACTIVE_ADAP_CLOCK_REC          1         /* indicates line/PW works with CR in
                                                                 adaptive or regular differntial mode*/
#define WT_CLOCK_REC_ACTIVE_EXT_DIFF_CLOCK_REC      2         /* indicates line/PW works with CR in
                                                                 extended differential mode. */



/***************************** End of Clock recovery define ***********************************/

/********************************** SyncE defines  *************************************/

#define WT_SYNC_ENET_CLOCK_MASTER              WP_SYNC_ENET_CLOCK_MASTER0 /* SyncE clock output on Sync_eth_Clk_1 */

/********************************** End OF SyncE defines *************************************/
/* DFC configuration */
#define WTI_DFC_CLASSIFIER                          1
#if WTI_DFC_CLASSIFIER
#if WTI_8K_CH_SETUP
#define WTI_IW_RULE_MAX_LENGTH                      128
#else
#define WTI_IW_RULE_MAX_LENGTH                      256
#endif
#endif

#define MY_PRINTF printf
#define WPE_TEST_ADD_ERROR(error_str) \
        do{ \
            printf("TEST FAILURE in file %s,  function %s,  line: %d \n    %s\n", \
                  __FILE__, __FUNCTION__, __LINE__, error_str); \
        } while(0)

#define WT_PASS 0
#define WT_FAIL 1

#endif /*WTI_CESOP_DEFINES_ */


