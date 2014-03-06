/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************/
/*                       External Includes                                   */
/*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef __linux__
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>
#endif

/*****************************************************************************/
/*                       WDDI Include Section                                */
/*****************************************************************************/

#include "wp_wddi.h"
#include "wp_host_io.h"
#include "wpx_app_data.h"
#include "wpx_oc3_pmc5351.h"
#include "wpx_oc12_pmc5357.h"
#include "wpx_gei_phy.h"
#include "wpx_pin_mux_selector.h"

#include "wpu_bus_mon.h"

#include "winutil/include/wpu_sa_actual.h"
#include "winutil/include/wpu_wgu.h"
#include "winutil/include/wpu_ta_base.h"
#include "winutil/include/wpu_ta_error.h"
#include "winutil/include/wpu_ta_ord.h"
#include "winutil/include/wpu_ta_stat.h"
#include "winutil/include/wpu_sa_alloc.h"
#include "winutil/include/wpu_sa_limits.h"
#include "fiberhome.h"

#include "fiberhome.h"

//#undef WT_UFE_FRAMER

/*
 * Change here in manual compilation (37900b)
 */
#define WTI_CESOP_TDI                               1                 /* use of TDM I/F */
#warning TDIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
// #endif


#define WTI_CESOP_CLOCK_RECOVERY_ENABLE             0                 /* enable clock recovery module */
#define WTI_CESOP_RX_TIMING_ENABLE                  0                 /* enable Rx timing direction */
#define WTI_CESOP_REGRESSION_TEST                   0                 /* when '1' running in regression mode*/
#define A_AUTOMATION                                0                 /* Should set to 1 for automation */

/*
 * Used for automatic compilation of tests
 */
/* This is 37900b_UFE4_DCR and 37900b_UFE4_ACR correspondingly */
#if WTI_CI_TESTING_MODE_UFE4 
#define WTI_CESOP_TDI                               0                 /* use of TDM I/F */
#define WTI_CESOP_CLOCK_RECOVERY_ENABLE             1                 /* enable clock recovery module */
#define WTI_CESOP_RX_TIMING_ENABLE                  1                 /* enable Rx timing direction */
#define WTI_CESOP_REGRESSION_TEST                   1                 /* when '1' running in regression mode*/
#define A_AUTOMATION                                0                 /* Should set to 1 for automation */
/* This is 37900b_TDI_DCR and 37900b_TDI_ACR correspondingly */
#elif WTI_CI_TESTING_MODE_TDI
#define WTI_CESOP_TDI                               1                 /* use of TDM I/F */
#define WTI_CESOP_CLOCK_RECOVERY_ENABLE             1                 /* enable clock recovery module */
#define WTI_CESOP_RX_TIMING_ENABLE                  1                 /* enable Rx timing direction */
#define WTI_CESOP_REGRESSION_TEST                   1                 /* when '1' running in regression mode*/
#define A_AUTOMATION                                0                 /* Should set to 1 for automation */
#endif

/*****************************************************************************/
/*                       Internal Include                                    */
/*****************************************************************************/
#if WT_TRIAL_MASK & (1 << 10)
#define OTE_AUTOMATION
#endif

#if WTI_CESOP_TDI && (defined __WT_UFE448__ || defined __WT_UFE412__)
#error TDI and UFE4 cannot be compiled together
#endif

#define WTI_8K_CH_SETUP                             0             /* Should be set only for 8k ch test (UFE3)*/
#define WTI_CAD_0_3_SETUP                           0             /* Should be set only for CAD 0-3 test with Winpath2 and UFE3
                                                                     Note that the physical configuration requires UFE3 on UPI2 and Comet on TDI 1-8 */
#ifdef __WT_UFE4_DUAL_EMPHY__
#define WTI_DUAL_EMPHY                              1
#else
#define WTI_DUAL_EMPHY                              0
#endif
#ifdef __WT_UFE3__
#define WTI_CESOP_MPLS_IW                           0             /* enable MPLS (must be disabled in UFE3)  */
#else
#define WTI_CESOP_MPLS_IW                           1             /* enable MPLS (must be enabled in UFE4)  */
#endif

#if WTI_CI_TESTING_MODE_UFE4 
#define WTI_CESOP_MPLS_IW                           1             /* enable MPLS (must be enabled in UFE4)  */
#elif WTI_CI_TESTING_MODE_TDI
#define WTI_CESOP_MPLS_IW                           0             /* enable MPLS (must be enabled in UFE4)  */
#endif
#define WTI_CESOP_MEF8_ENCAP                        0             /* MEF8 encapsulation */
#if WTI_CESOP_MPLS_IW
#define WTI_CESOP_MPLS_OVER_ENET                    1
#define WTI_CESOP_TO_MPLS_FA_CASCADING              1             /* MPLS FA cascading enable - in this mode the TDM to PSN flow aggregation points at a layer2 router
                                                                     MPLS flow aggregation which adds the PSN tunnel MPLS label */
#define WTI_CESOP_NATIVE_MPLS_IW                    0             /* NATIVE POS MPLS on UPI2 (PPP header is not transmitted) */
#define WTI_MPLS_LOOPBACK                           0             /* Use loop back on UPI MPLS port (no need for OC card */
#endif /* WTI_CESOP_MPLS_IW */
#define WTI_DEBUG_LEVEL                             0             /* DEBUG LEVEL (0-lowest, 2-highest) */
#if WTI_8K_CH_SETUP
#define WTI_MAX_PW                                  8064          /* maximum number of PW's in PWE3 system */
#elif defined __WT_UFE412__
#if WTI_DUAL_EMPHY
#define WTI_MAX_PW                                  2688  /* 2688 *//* maximum number of PW's in a DUAL EMPHY PWE3 system (1344 PWs on each UFE device) */
#else
#define WTI_MAX_PW                                  1344          /* maximum number of PW's in PWE3 system */
#endif
#elif defined __WT_UFE448__
#define WTI_MAX_PW                                  2688          /* maximum number of PW's in PWE3 system */
#else //UFE3
#define WTI_MAX_PW                                  366          /* maximum number of PW's in PWE3 system */
#endif

#if (!WTI_CESOP_TDI)
#define WTI_MAX_NUM_EMPHY_PHYS                      WTI_MAX_PW    /* maximum number of emphy phys, relevant only if working with UFE */
#endif /* (!WTI_CESOP_TDI) */

#if WTI_8K_CH_SETUP/*UFE3 */
#define WTI_2_ENET_DEVICES                          1             /* two enet devices are created, pwe3 packets are received at one port and transmitted from the second port */
#elif defined __WT_UFE448__
#define WTI_XGI_MODE                                1
#define WTI_2_ENET_DEVICES                          0
#define WTI_4_ENET_DEVICES                          0
#elif defined __WT_UFE412__
#if WTI_DUAL_EMPHY
#define WTI_XGI_MODE                                1
#define WTI_2_ENET_DEVICES                          0  
#else  /* single emphy: */
#define WTI_XGI_MODE                                0
#define WTI_2_ENET_DEVICES                          0             /* two enet devices are created, pwe3 packets are received at one port and transmitted from the second port */
#endif
#endif

#if WTI_CESOP_MPLS_IW
#if WTI_CESOP_TO_MPLS_FA_CASCADING
#if WTI_XGI_MODE
#define WTI_MAX_NUM_OF_MPLS_FA                      1             /* maximum number of layer2 router MPLS flow aggregations */
#else
#define WTI_MAX_NUM_OF_MPLS_FA                      1             /* maximum number of layer2 router MPLS flow aggregations */
#endif
#else
#define WTI_MAX_NUM_OF_MPLS_FA                      1
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */
#define WTI_TDM2PSN_L2_MPLS_ROUTER_TAG              0xf0f00000 /* unique tag for each layer2 router MPLS flow aggregation (| FA INDEX) */
#define WTI_PSN2TDM_L2_MPLS_ROUTER_TAG              0xe0e00000 /* unique tag for each layer2 router MPLS flow aggregation (| FA INDEX) */

#endif  /* WTI_CESOP_MPLS_IW */

#define WTI_INFO_STRUCTURES_DEFINED 1

#if WTI_8K_CH_SETUP
#define WTI_FIRST_ENET_MAX_PW                       (WTI_MAX_PW / 2) /* defines how many PW's point to the first ENET port (in 8K configuration 4k goes to first Enet and 4k goes to second Enet*/
#else
#define WTI_FIRST_ENET_MAX_PW                       (WTI_MAX_PW / 2) /* defines how many PW's point to the first ENET port (all PWs go to the first Enet port) */
#endif

#define WTI_WPID                                    0             /* Winpath ID */

#define WTI_EMPHY_PORT                              WP_PORT_UPI1
#define SERIAL_X_CPLD_INTERRUPT_MASK   (~WPX_FPGA_INTR_SERIAL_1_CONNECTOR & 0x7f)

#if WTI_DUAL_EMPHY
#define WTI_SECOND_EMPHY_PORT                       WP_PORT_UPI3  /* second EMPHY port must be connected to UPI3 */
#define WTI_FIRST_UFE_NUM_LINES                     336           /* first UFE number of lines */
#undef SERIAL_X_CPLD_INTERRUPT_MASK
#define SERIAL_X_CPLD_INTERRUPT_MASK   (~(WPX_FPGA_INTR_SERIAL_1_CONNECTOR | WPX_FPGA_INTR_SERIAL_3_CONNECTOR) & 0x7f)
#endif

#if WTI_CAD_0_3_SETUP
#undef WTI_EMPHY_PORT
#define WTI_EMPHY_PORT                              WP_PORT_UPI1  /* EMPHY port */
#endif
#define WTI_UFE_UPI_PORT                            WTI_EMPHY_PORT

#if (WTI_XGI_MODE)
#define WTI_ENET_MODE                               NORMAL /* defines loopback or normal operation on ENET port */
#else
#warning ENET_WROKING_IN_NORMAL_MODE
#define WTI_ENET_MODE                               NORMAL // WP_ENET_LOOPBACK/* defines loopback or normal operation on ENET port */
#endif // WTI_XGI_MODE

#ifdef WP_HW_WINPATH2
#define WTI_ENET_PORT                               WP_PORT_ENET1      /* ENET port */
#define WTI_ENET_TYPE                               WP_ENET_RGMII_1000 /* (WP_ENET_RGMII_10_100) or(WP_ENET_RGMII_1000) */
#define WTI_GMII_TYPE                               WTI_GMII_OV_FIBER /* GMII over fiber or over copper*/
#endif  /* WP_HW_WINPATH2 */
#ifdef WP_HW_WINPATH3
#if WTI_XGI_MODE
#define WTI_ENET_PORT                               WP_PORT_XGI1       /* XGI port: WP_PORT_XGI1 or WP_PORT_XGI2 */
#define WTI_ENET_TYPE                               WP_ENET_XAUI       /* defines XAUI modes for XGI */
#else
#ifdef WP_BOARD_WDS3_SL
#define WTI_ENET_PORT                               WP_PORT_ENET11      /* ENET port */
#else   /* NOT  WP_BOARD_WDS3_SL */
#define WTI_ENET_PORT                               WP_PORT_ENET13      /* ENET port */
#endif  /* NOT WP_BOARD_WDS3_SL */
#define WTI_ENET_TYPE                               WP_ENET_1000_BASE_X /* WP_ENET_1000_BASE_X or  WP_ENET_1000_BASE_X_AN */
#define WTI_GMII_TYPE                               WTI_GMII_OV_FIBER
#endif  /* WTI_XGI_MODE */
#endif  /* WP_HW_WINPATH3 */

#define WTI_ENET_IEEE_802                           0                 /* defines working in IEEE 802.1Q ethernet header */

#if (WTI_2_ENET_DEVICES)
#define WTI_SECOND_ENET_MODE                        WP_ENET_LOOPBACK /* defines loopback or normal operation on ENET port */
#ifdef WP_HW_WINPATH2
/* second ENET port (relevant only in working in simulate network mode or in 2 ENET devices mode) */
#define WTI_SECOND_ENET_PORT                        WP_PORT_ENET2
#define WTI_SECOND_ENET_TYPE                        WP_ENET_RGMII_1000 /* (WP_ENET_RGMII_10_100) or(WP_ENET_RGMII_1000) */
#define WTI_SECOND_GMII_TYPE                        WTI_GMII_OV_FIBER  /* GMII over fiber or over copper for second enet*/
#endif /* WP_HW_WINPATH2 */
#ifdef WP_HW_WINPATH3
/* second ENET port (relevant only in working in simulate network mode or in 2 ENET devices mode) */
#if WTI_XGI_MODE
#define WTI_SECOND_ENET_PORT                        WP_PORT_XGI2       /* Second XGI port: WP_PORT_XGI1 or WP_PORT_XGI2 */
#define WTI_SECOND_ENET_TYPE                        WP_ENET_XAUI       /* defines XAUI modes for XGI */
#else /* WTI_XGI_MODE */
#ifdef WP_BOARD_WDS3_SL
#define WTI_SECOND_ENET_PORT                        WP_PORT_ENET12
#else   /* NOT WP_BOARD_WDS3_SL */
#define WTI_SECOND_ENET_PORT                        WP_PORT_ENET14
#endif  /* NOT WP_BOARD_WDS3_SL */
#define WTI_SECOND_ENET_TYPE                        WP_ENET_1000_BASE_X /* (WP_ENET_RGMII_10_100) or(WP_ENET_RGMII_1000) */
#define WTI_SECOND_GMII_TYPE                        WTI_GMII_OV_FIBER
#endif /* WTI_XGI_MODE */
#endif /* WP_HW_WINPATH3 */
#endif /* WTI_2_ENET_DEVICES */

#if WTI_4_ENET_DEVICES
#define WTI_THIRD_ENET_MODE                         WP_ENET_NORMAL      /* defines loopback or normal operation on ENET port */
#define WTI_THIRD_ENET_PORT                         WP_PORT_ENET15       /* Second XGI port: WP_PORT_XGI1 or WP_PORT_XGI2 */
#define WTI_THIRD_ENET_TYPE                         WP_ENET_1000_BASE_X /* defines XAUI modes for XGI */
#define WTI_FOURTH_ENET_MODE                        WP_ENET_NORMAL      /* defines loopback or normal operation on ENET port */
#define WTI_FOURTH_ENET_PORT                        WP_PORT_ENET16       /* Second XGI port: WP_PORT_XGI1 or WP_PORT_XGI2 */
#define WTI_FOURTH_ENET_TYPE                        WP_ENET_1000_BASE_X /* defines XAUI modes for XGI */
#endif

#define N_ENET_TX_CHANNELS                          8
#define N_ENET_RX_CHANNELS                          8

#define N_ENET_PQ_BLOCK_SIZE                        0
#define N_ENET_PQ_BLOCKS                            0
#define N_IW_FLOW_AGGREGATIONS                      (WTI_MAX_PW * 2)
#define N_IW_ROUTING_SYSTEMS                        1

#if WTI_CESOP_TO_MPLS_FA_CASCADING
#define N_ROUTING_MAX_FLOWS                         ((WTI_MAX_PW + 1) + 4)
#else /* !WTI_CESOP_TO_MPLS_FA_CASCADING */
#define N_ROUTING_MAX_FLOWS                         (WTI_MAX_PW + 1)
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */

#define N_MAX_OUT_OF_BAND_CAS_FLOWS                 2
#define N_POOLS                                     10

#define MD_BLOCK_SIZE          (0x100-1)

#define N_ACTIVE_UFE_SBI_SPE                        12
#if WTI_CAD_0_3_SETUP
#define N_ACTIVE_UFE_OCT_LINES                      4                     /* UFE :number of active OCTAL lines, must be defined if working in UFE OCTAL mode */
#else
#define N_ACTIVE_UFE_OCT_LINES                      8                     /* UFE :number of active OCTAL lines, must be defined if working in UFE OCTAL mode */
#endif /* N_ACTIVE_UFE_OCT_LINES */

#define N_HOST_RX_CHANNELS                          1
#define N_HOST_TX_CHANNELS                          1
#define N_HOST_TX_GROUPS                            N_HOST_TX_CHANNELS

#if WTI_CESOP_MPLS_IW
#define WTI_MPLSoPPP                                0xff030281
#define WTI_MPLS_LABEL                              0x100001ff

#if ( WTI_CESOP_MPLS_OVER_ENET || WTI_CESOP_MEF8_ENCAP)
#define WTI_ETHERNET_HEADER_SIZE                    14
#else
#define WTI_PPP_HEADER_SIZE                         4
#endif
#endif /* WTI_CESOP_MPLS_IW */

#define WTI_NUM_OF_VLAN_TAGS                        0

#define WTI_VLAN_TAG_SIZE                           4

#if WTI_CESOP_MEF8_ENCAP
#ifdef WTI_ETHERNET_HEADER_SIZE
#undef WTI_ETHERNET_HEADER_SIZE
#endif
#define WTI_ETHERNET_HEADER_SIZE                    14
#define WTI_ECID_HEADER_SIZE                        4
#endif /*WTI_CESOP_MEF8_ENCAP*/
#if WTI_CESOP_TO_MPLS_FA_CASCADING
#if WTI_CESOP_MPLS_OVER_ENET
#undef WTI_ETHERNET_HEADER_SIZE
#define WTI_ETHERNET_HEADER_SIZE                    0
#else
#undef WTI_PPP_HEADER_SIZE
#define WTI_PPP_HEADER_SIZE                         0
#endif
#endif/* WTI_CESOP_TO_MPLS_FA_CASCADING */

#define WTI_MPLS_LABEL_SIZE                         4

#if WTI_CESOP_MEF8_ENCAP
#define WTI_ECID_LABEL_SIZE                         4
#endif /*WTI_CESOP_MEF8_ENCAP*/

/* TDI configuration */
#if WTI_CESOP_TDI
#define WTI_ACTIVE_TDI_PORTS                        8      /* number of active TDI ports, must be defined if working in TDI mode */
#if WTI_CI_TESTING_MODE_TDI
#define WTI_TDM_MODE                                WP_TDM_LOOPBACK //WP_TDM_NORMAL
#else
#define WTI_TDM_MODE                                WP_TDM_NORMAL
#endif
#define WTI_TDM_INT_MODE                            WP_TDM_INT_ENABLE
#define WTI_TX_PARAM_BUS_TYPE                       WP_BUS_HOST
#define WTI_TX_PARAM_BUS_BANK                       APP_BANK_HOST
#ifdef WP_BOARD_WDS3_SL
#define WTI_TX_DATA_BUS_TYPE                        WP_BUS_PARAM
#define WTI_TX_DATA_BUS_BANK                        APP_BANK_PARAM
#else  /* NOT WP_BOARD_WDS3_SL */
#define WTI_TX_DATA_BUS_TYPE                        WP_BUS_PACKET
#define WTI_TX_DATA_BUS_BANK                        APP_BANK_PACKET
#endif /* NOT WP_BOARD_WDS3_SL */

#define WTI_TX_INTERRUPT_QUEUE                      WP_IRQT1
#define WTI_RX_PARAM_BUS_TYPE                       WP_BUS_PARAM
#define WTI_RX_PARAM_BUS_BANK                       APP_BANK_PARAM

#ifdef WP_BOARD_WDS3_SL
#define WTI_RX_DATA_BUS_TYPE                        WP_BUS_HOST
#else  /* NOT WP_BOARD_WDS3_SL */
#define WTI_RX_DATA_BUS_TYPE                        WP_BUS_PACKET
#endif /* NOT WP_BOARD_WDS3_SL */

#define WTI_RX_INTERRUPT_QUEUE                      WP_IRQT1

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE

/* WTI_TDI_DCO_CLOCK_SOURCE - select the soruce clock for TDI DCO block */
#ifdef WP_HW_WINPATH3
/*
  WTI_TDI_DCO_CLOCK_SOURCE:
  0 - WP_CLOCK_REC_DCO_CLOCK_BRGI1
  1 - WP_CLOCK_REC_DCO_CLOCK_BRGI2
  2 - WP_CLOCK_REC_DCO_CLOCK_BRGI3
  3 - WP_CLOCK_REC_DCO_CLOCK_BRGI4
  4 - WP_CLOCK_REC_DCO_CLOCK_ENET
  5 - WP_CLOCK_REC_DCO_CLOCK_INTERNAL
*/
#define WTI_TDI_DCO_CLOCK_SOURCE                    WP_CLOCK_REC_DCO_CLOCK_INTERNAL

#else /* WP_HW_WINPATH2 */
/*  WP2 WTI_TDI_DCO_CLOCK_SOURCE:
    WP_CLOCK_REC_DCO_CLOCK_BRGI3
    WP_CLOCK_REC_DCO_CLOCK_INTERNAL
*/
#define WTI_TDI_DCO_CLOCK_SOURCE                    WP_CLOCK_REC_DCO_CLOCK_INTERNAL
#endif /* WP_HW_WINPATH3 */

#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
/* WTI_TDI_TS_CLOCK_SOURCE - select the soruce clock for TDI timestamp block
   WP_CLK_SRC_BRGI1           clock source: BRGin #1
   WP_CLK_SRC_BRGI2,          clock source: BRGin #2
   WP_CLK_SRC_BRGI3,          clock source: BRGin #3
   WP_CLK_SRC_BRGI4,          clock source: BRGin #4
   WP_CLK_SRC_ENET,           clock source: Enet clock
   WP_CLK_SRC_FCLK1,          clock source: Global DCO #1
   WP_CLK_SRC_FCLK2,          clock source: Global DCO #1
   WP_CLK_SRC_INT_SYS         clock source: Internal sys clk */

#define WTI_TDI_TS_CLOCK_SOURCE                     WP_CLK_SRC_BRGI1 /* WP3 tdi timestamp source     */
#endif /* WTI_CESOP_TDI */
#if WTI_CI_TESTING_MODE_TDI || WTI_CI_TESTING_MODE_UFE4
#define WTI_WG_TS2_CLOCK_SOURCE                     WP_CLK_SRC_FCLK2
#else
#define WTI_WG_TS2_CLOCK_SOURCE                     WP_CLK_SRC_BRGI1 /* WP3 wingine timestamp source */
#endif
#define WTI_NONE_ZERO                               1

#ifdef WP_HW_WINPATH3
#if WTI_CESOP_TO_MPLS_FA_CASCADING
/* PCE is currently supported only for the MPLS cascading configuration */
#define WTI_PCE_CLASSIFIER                          1              /* PCE classifier is in use instead of the DFC */
#if WTI_PCE_CLASSIFIER
#include "wt_partition_ecc_util.h"
#endif
#endif
#endif

#if !WTI_PCE_CLASSIFIER
/* Classifier config */
#define NUM_OF_ROUTE_FLOWS                          WTI_MAX_PW
#define MAX_NUM_OF_CLASS_FILTERS                    32
#define MAX_FLOWS_PER_FILTER                        100
#endif

/* Qnodes configuration */
#define N_HOST_QUEUE_BUFFERS                        1000
#define N_HOST_QUEUE_BUFFER_SIZE                    800 /* regular tests need only 256. 800 is for 30/24
                                                           slots tests in CAS mode */
#define N_HOST_QUEUE_RINGS                          100
#define N_HOST_QUEUE_RING_LENGTH                    4

#define N_IW_PSN2TDM_QUEUE_ADJUNCT_POOL_BUFFERS     (26*WTI_MAX_PW)
#define N_IW_PSN2TDM_QUEUE_BUFFERS                  5000
#define N_IW_PSN2TDM_QUEUE_BUFFER_SIZE              600 /* regular tests need only 512. 800 is for 30/24
                                                           slots tests in CAS mode */

#if WTI_8K_CH_SETUP
#define N_IW_TDM2PSN_QUEUE_ADJUNCT_POOL_BUFFERS     10
#define N_IW_TDM2PSN_QUEUE_BUFFERS                  10000
#define N_IW_TDM2PSN_QUEUE_BUFFER_SIZE              800 /* regular tests need only 512. 800 is for 30/24
                                                           slots tests in CAS mode */

#define N_TRANS_TX_QUEUE_RINGS                      (2*WTI_MAX_PW)
#define N_TRANS_TX_QUEUE_RING_LENGTH                4
#define N_TRANS_TX_QUEUE_BUFFERS                    (N_TRANS_TX_QUEUE_RINGS * N_TRANS_TX_QUEUE_RING_LENGTH)

#else
#define N_IW_TDM2PSN_QUEUE_ADJUNCT_POOL_BUFFERS     10
#define N_IW_TDM2PSN_QUEUE_BUFFERS                  5000
#define N_IW_TDM2PSN_QUEUE_BUFFER_SIZE              600 /* regular tests need only 512. 800 is for 30/24
                                                           slots tests in CAS mode */
#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)
#define N_TRANS_TX_QUEUE_RINGS                      (WTI_MAX_PW)
#define N_TRANS_TX_QUEUE_RING_LENGTH                64
#define N_TRANS_TX_QUEUE_BUFFERS                    (N_TRANS_TX_QUEUE_RINGS * N_TRANS_TX_QUEUE_RING_LENGTH)
#else /* CR modes */
#define N_TRANS_TX_QUEUE_RINGS                      (WTI_MAX_PW)
#define N_TRANS_TX_QUEUE_RING_LENGTH                64
#define N_TRANS_TX_QUEUE_BUFFERS                    (N_TRANS_TX_QUEUE_RINGS * N_TRANS_TX_QUEUE_RING_LENGTH)
#endif /* CR modes */

#endif /* WTI_8K_CH_SETUP */

/* TDM --> PSN */
#define WTI_TDM2PSN_RTP_MODE                        WP_ENABLE
#define WTI_TDM2PSN_INTERRUPT_MODE                  WP_DISABLE
#define WTI_TDM2PSN_INTERRUPT_QUEUE                 WP_IRQT1
#define WTI_TDM2PSN_OOBC_INTERRUPT_MODE             WP_DISABLE
#define WTI_TDM2PSN_OOBC_INTERRUPT_QUEUE            WP_IRQT1
#define WTI_TDM2PSN_STAT_MODE                       WP_ENABLE
#define WTI_TDM2PSN_OV_POOL_MODE                    WP_DISABLE
#define WTI_TDM2PSN_L2_LENGTH_UPDATE_MODE           WP_DISABLE
#define WTI_TDM2PSN_L4_CHECKSUM_RECALC_MODE         WP_DISABLE
#if WTI_CESOP_MPLS_IW
#if WTI_CESOP_MPLS_OVER_ENET
#define WTI_L2_HEADER_LEN                           sizeof(WTI_enet_header)
#else
#define WTI_L2_HEADER_LEN                           4
#endif
#else
#define WTI_L2_HEADER_LEN                           sizeof(WTI_enet_header)
#endif
#if WTI_CESOP_MEF8_ENCAP
#define WTI_L3_HEADER_LEN                           0
#else
#define WTI_L3_HEADER_LEN                           sizeof(WTI_ip_header)
//#define WTI_L3_HEADER_LEN                           0
#endif
#define WTI_SRC_IP_OFFSET                           12
#define WTI_DEST_IP_OFFSET                          16
#define WTI_L4_HEADER_LEN                           sizeof(WTI_udp_header)
#define WTI_L4_LENGTH_OFFSET                        4
#define WTI_RTP_HEADER_LEN                          sizeof(WTI_rtp_header)
#define WTI_CONTROL_WORD_LEN                        4
#define WTI_TDM2PSN_TIMESTAMP_MODE                  WP_IW_TIME_STAMP_DISABLE
#define WTI_TDM2PSN_FBP_DROP_THRESHOLD              0xf
#define WTI_TDM2PSN_MTU                             1536
#define WTI_TDM2PSN_L4_LENGTH_UPDATE_MODE           WP_DISABLE
#define WTI_TDM2PSN_INITIAL_SEQ_NUMBER              1
#define WTI_TDM2PSN_OOBC_CAS_INITIAL_SEQ_NUMBER     1000
#define WTI_CAS_LOCATION                            WP_CAS_NIBBLE_LOW

/* PSN --> TDM */
#define WTI_PSN2TDM_INTERRUPT_MODE                  WP_ENABLE
#define WTI_PSN2TDM_INTERRUPT_QUEUE                 WP_IRQT1
#define WTI_PSN2TDM_STAT_MODE                       WP_ENABLE
#define WTI_PSN2TDM_PAYLOAD_TYPE_CHECK              WP_DISABLE
#define WTI_PSN2TDM_PAYLOAD_TYPE                    0x00
#define WTI_PSN2TDM_OOBC_PAYLOAD_TYPE               0x00
#define WTI_PSN2TDM_SSRC_TYPE_CHECK                 WP_DISABLE
#define WTI_PSN2TDM_SSRC_TYPE                       0x00000000
#define WTI_PSN2TDM_OOBC_SSRC_TYPE                  0x00000000
#define WTI_PSN2TDM_CONTROL_WORD_MODE               WP_ENABLE
#define WTI_PSN2TDM_TIMESTAMP_MODE                  WP_IW_TIME_STAMP_DISABLE
#define WTI_PSN2TDM_OV_POOL_MODE                    WP_DISABLE
#define WTI_PSN2TDM_FBP_DROP_THRESHOLD              0
#define WTI_PSN2TDM_MTU                             1536

#define WTI_E1_SLOTS                                32
#define WTI_T1_SLOTS                                24
#define WTI_DS3_SLOTS                               1
#define WTI_E3_SLOTS                                WTI_DS3_SLOTS

#define WTI_NUMBER_OF_SPE_DS3_E3                    12 /* Number of SPE used for transport of DS3 E3 tests*/

#define WTI_T1_LINES_PER_SPE                        28 /* TMX84_T1_LINK_MAX 28 */
#define WTI_E1_LINES_PER_SPE                        21 /* TMX84_E1_LINK_MAX 21 */
#define WTI_TDM_SLOTS_PER_SPE                       672
#define WTI_MIN_DATAUNIT_SIZE                       2
#define WTI_MAX_DATAUNIT_SIZE                       64
#define WTI_MAX_STRING_SIZE                         128
#define WTI_MIN_SLOTS_PER_PHY                       1
#define WTI_UNUSED_SLOT                             0xffffffff

#ifdef __WT_UFE3__
#define WTI_UFE_4_1_2                               0   /* Use UPI 16 bit when working with Spartan. */
#define WTI_MAX_SLOTS                               8064
#else   /* not __WT_UFE3__ */
#ifdef __WT_UFE412__
#define WTI_UFE_4_1_2                               1   /* Use UPI 16 bit when working with Spartan. */
#if WTI_DUAL_EMPHY
#define WTI_MAX_SLOTS                               21504
#else
#define WTI_MAX_SLOTS                               10752
#endif
#else  /* not __WT_UFE412__ */
#ifdef __WT_UFE448__
#define WTI_UFE_4_1_2                               0   /* Use UPI 16 bit when working with Spartan. */
#define WTI_MAX_SLOTS                               43008
#else   /* not __WT_UFE448__ & __WT_UFE412__*/
#define WTI_MAX_SLOTS                               8064
#endif  /* not __WT_UFE448__ & __WT_UFE412__*/
#endif /* not __WT_UFE412__ */
#endif  /* not __WT_UFE3__ */
#define WTI_MAX_SPE_PER_UFE                         WUFE_MAX_N_SPE

/* boards configuration */
#define TWO_BOARDS_SIMULATION                       0
/* #define WTI_BOARD_1 */

#define CLOCK_REC_TABLE_SIZE 64                                /* CR table size in the dps */

#define CR_CHIP_SELEC_VAL                           0xff807f24 /* chip select value */
#define CR_CHIP_SELECT_ADD                          0x1d03080c /* chip select address for WP2 */

#if WTI_CESOP_TDI
/* CR registers in TDI */
#define CR_HW_MODE_REG_ADD                          0x1f134000 /* clock recovery mode reg' address*/
#else
/* CR registers in UFE */
#define CR_HW_MODE_REG_ADD                          0x1da42100 /* clock recovery mode reg' address*/
#endif /* WTI_CESOP_TDI */

/* CAD loopback mode */
#if WTI_CAD_0_3_SETUP
#define WTI_CAD_SYSTEM_LOOPBACK                     0
#endif

#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)
/*
 * Change here in manual compilation (37900b)
 */
#define WTI_CLOCK_REC_MODE                          0        /* 1 - differential, 0 - adaptive */

/*
 * Used for automatic compilation of tests
 */
#if (WT_TRIAL_MASK == 1<<4 || WT_TRIAL_MASK == 1<<6 || WT_TRIAL_MASK == 1<<9)
#define WTI_CLOCK_REC_MODE                          1        /* 1 - differential, 0 - adaptive */
#elif (WT_TRIAL_MASK == 1<<5 || WT_TRIAL_MASK == 1<<7)
#define WTI_CLOCK_REC_MODE                          0        /* 1 - differential, 0 - adaptive */
#endif
#define WTI_EXT_DIFF_MODE_ACTIVE                    0        /* 1 - ext diff, 0 - regular diff */

#if  ((defined  __WT_UFE412__)||(defined  __WT_UFE448__)) && (!WTI_CLOCK_REC_MODE) && (WTI_CESOP_CLOCK_RECOVERY_ENABLE)
#define WTI_CES_SHAPING_ENABLED                     1
#endif   /*  __WT_UFE412__ || __WT_UFE448__  WTI_CESOP_CLOCK_RECOVERY_ENABLE WTI_CLOCK_REC_MODE*/

#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE && !WTI_CESOP_REGRESSION_TEST)
#define WTI_ADM_MODE                                1    /* on CR usually use ADM */
#else
#define WTI_ADM_MODE                                0
#endif

/* The SyncE demo uses two boards , one as syncE master and the second as slave */
#define WTI_SYNC_ETH_ENABLE                         0        /* 1 - diff clock from syncE clock*/
#define WTI_SYNC_ETH_MASTER_ENABLE                  0        /* 1 - SyncE master , 0 - SyncE master*/

#if !(WTI_CESOP_TDI)
#define WTI_ACTIVE_CR_FOR_ASYNC_MODE                64          /* number of active clock recovery for
                                                                   A-Synchronous mode */
#define WTI_ACTIVE_CR_FOR_EXT_DIFF_MODE             336
#else /* In TDI mode each line is different CRS */
#define WTI_ACTIVE_CR_FOR_ASYNC_MODE                WTI_ACTIVE_TDI_PORTS
#define WTI_ACTIVE_CR_FOR_EXT_DIFF_MODE             0
#ifdef WP_HW_WINPATH2
#if WTI_CLOCK_REC_MODE
#define WTI_CLOCK_REC_TDI_LINES_4_7                 0    /* In WP2, TDI differential CR mode,
                                                            choose between lines 0-3 (0) or lines 4-7 (1)
                                                            When 0-3 are used the FPGA (UFE) should be placed on UPI1
                                                            and the correct tdi-cr synthesis should be used

                                                            When 4-7 are used the FPGA (UFE) should be placed on UPI2
                                                            and the correct tdi-cr synthesis should be used
                                                            WTI_ACTIVE_TDI_PORTS <= 8 */
#endif /*WTI_CLOCK_REC_MODE*/
#endif /*WP_HW_WINPATH2*/
#endif

/*------------------- CR Adaptive CAD Backward Compatibly Mode --------------------------------*/
/* In CR Adaptive CAD mode the TS can be taken in DPS instead of UFE (from UFE  synthesis 2.4) */
/* Only the DCO is in the UFE FPGA                                                             */
#if (!(WTI_CESOP_TDI)&&(!(WTI_CLOCK_REC_MODE))&&((WTI_CESOP_CLOCK_RECOVERY_ENABLE)))
#define WTI_CLOCK_REC_ADA_BW_COM_MODE 0
#else
/*In all other mode Backward Compatibly is not supported */
#define WTI_CLOCK_REC_ADA_BW_COM_MODE 0
#endif
/*---------------------------------------------------------------------------------------------*/


/*------------------- CR Differetial TDI PDCR DOCSIS Mode ---------------------------------------*/
/* This mode is supported by the TDI differential mode only and required dedicated FPGA synthesis*/
#if(WTI_CESOP_TDI && WTI_CESOP_RX_TIMING_ENABLE && WTI_CESOP_CLOCK_RECOVERY_ENABLE && WTI_CLOCK_REC_MODE)
#if  WT_TRIAL_MASK == 1<<9
#define WTI_CESOP_DIFF_DOCSIS_MODE                  1         /* DOCSIS differential mode (in tdi only */
#else
#define WTI_CESOP_DIFF_DOCSIS_MODE                  0         /* DOCSIS differential mode (in tdi only */
#endif
#else
#define WTI_CESOP_DIFF_DOCSIS_MODE                  0
#endif
/*---------------------------------------------------------------------------------------------*/

#define WTI_PW_PER_LINE                             1          /* defines the PW location of the second CR
                                                                  master. Used in the two CR master test.
                                                                  Have to give integer after devided 24 (T1)
                                                                  or 32 (E1) with it. */
#else /*No clock Rx or TX Clock recovery */
#define WTI_ACTIVE_CR_FOR_ASYNC_MODE                0          /* No clock recovery */
#define WTI_ACTIVE_CR_FOR_EXT_DIFF_MODE             0
#define WTI_EXT_DIFF_MODE_ACTIVE                    0          /* 1 - ext diff, 0 - regular diff */
#define WTI_CLOCK_REC_MODE                          0          /* 1 - differential, 0 - adaptive */
#define WTI_PW_PER_LINE                             1
#endif


#define WTI_MAX_NUM_CLOCK_TRANSLATORS               0          /* define the number of Clock Translators
                                                                  in the synthesis when CR is not defined.
                                                                  If CR is defined, the number of CT in
                                                                  the synthesis is read from the synthesis.
                                                                  Usualy when CR is not used, the UFE is
                                                                  TX clock slave and therefore no CT are
                                                                  used.*/
/*------------------- TDM2PSN RTP Timestamp Absolute mode ---------------------------------------*/
#define WTI_ACTIVE_ABSOLUTE_MODE                    0
/*
  Absolute Mode:
  In this mode the TS is increment by constant value. This value is determined by the number of clocks that
  are required to transfer a packet (in framed mode it is the number of slots that are required to fill a
  packet in bits).
  For example: in T1 framed mode, 12 slots to DU, DU=24, packet_size = 72.
  => constant = 2(slots per DU)*3(DU per packet)*193(T1). (Alternative option: packet_size/num_of_slots*193)

  How to check ABS mode:
  Run one of the PWE3 tests in ABS mode. Connect the ENET to AdTech. Look on the packet payload; example of
  an expected table shown here:

  Packet Payload:  AA   AA   AA   AA   AA   AA   00   AA   00   AA   00   AA   08   00   45   00
  00   54   81   1E   00   00   40   11   A3   25   11   11   11   11   1A   1A
  1A   1A   10   10   00   01   00   00   00   00   50   00   08   1B > 00   F4
  6E   38 < 00   00   00   00   00   00   08   1B   00   00   00   00   00   00
  00   00   00   00   00   00   00   00   00   00   00   00   00   00   00   00
  00   00   00   00   00   00   00   00   00   00   00   00   00   00   00   00
  00   00   E1   37   E2   1E

  The number between the arrows, > and < (00F4-6E38) is the value of the TS; this is part of the RTP header.
  Subtract two sequential TSs will give the TS constant.
*/
/*--------------------------------------------------------- ---------------------------------------*/
#if (!WTI_CESOP_TDI)
#ifdef __WT_UFE3__
#include "wti_ufe_util.h"
#else
#include "wti_ufe_utility.h"
#include "wpl_sw_semaphore.h"
#endif
#ifdef __WT_UFE3__
#ifdef __WT_PMC__
typedef struct
{
      int (*pTemuxSetup)(int,int);
}WTI_temux_call;
#include "wti_tmx336_util.c"
WTI_temux_call  wti_configure[1] = {{WT_Tmx336SbiSetup}};
#endif
#elif defined  __WT_UFE412__ || defined __WT_UFE448__
#ifdef WT_UFE_FRAMER
#define WTI_FRAMER_SCRIPT 0
WP_U8 prbs_result[WTI_MAX_PW];
#include "wti_flexmux_util.c"
#endif
#else
#error "__WT_UFE__ undefined"
#endif
#endif

#include "wti_cesopsn_defines.h"

#ifdef __linux__
#include "wti_tun_driver_util.c"
#endif
#include "wti_cesopsn_util.h"
#include "wti_cesopsn_statistics.h"

#define VT100BLACK    0
#define VT100RED      1
#define VT100GREEN    2
#define VT100YELLOW   3
#define VT100BLUE     4
#define VT100MAGENTA  5
#define VT100CYAN     6
#define VT100WHITE    7
#define FOREGROUND_COLOR_BASE 40
#define BACKGROUND_COLOR_BASE 30
#define WTI_PRINT_ERROR(err_msg)                \
   do { Vt100Color(VT100RED, VT100WHITE);       \
      printf err_msg; Vt100Normal();            \
   } while (0)
#define FACTOR                 90000000 /* for board 180/90 */
#define WEB_UART_STATUS_RXFE   0x80
#define WEB_UART_STATUS_DR     0x01
#define WEB_UART_STATUS_OE     0x02
#ifdef __PPC__
#define MAP_UA_LSR    (0x1f010f1cL)
#endif
#ifdef __mips__
#define MAP_UA_LSR    (0xbd030f1cL)
#define WT_UART_LINE_STATUS_REG_OFFSET 0xc51c
#endif

#define WTI_PRINT_ERROR(err_msg)                \
   do {                                         \
      Vt100Color(VT100RED, VT100WHITE);         \
      printf err_msg;                           \
      Vt100Normal();                            \
   } while (0)

#define WTI_PRINT_WARNING(err_msg)              \
   do {                                         \
      Vt100Color(VT100YELLOW, VT100BLACK);      \
      printf err_msg;                           \
      Vt100Normal();                            \
   } while (0)

#define WTI_PRINT_NOTE(note_msg)                \
   do {                                         \
      Vt100Color(VT100GREEN, VT100WHITE);       \
      printf note_msg;                          \
      Vt100Normal();                            \
   } while (0)

/* global variables */
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

#define TRUE   1
#define FALSE  0

#define STR_STAR           "******************************************************************"
#define STR_HOT_KEYS       "  Hot keys: !-Main Menu ^-Upper Menu =-Display Cur Menu           "
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

/*const*/ Y_MenuEntry V_MainMenu[];
static Y_MenuEntry *V_CurrMenuP = &V_MainMenu[0];

extern char *STR_MENU_HDR;


WP_CHAR *STR_MENU_HDR = "            SAToP CESoPSN Demo                           ";
WP_CHAR appname[]     = "SAToP CESoPSN Demo";
WP_U32 total_number_of_pw = 0; /* this variable saved the total number of active PW in the system,
                                  mainly for statistics and validation check before entering a function.*/
WP_boolean tag_change = WP_FALSE;
WP_boolean rx_looptime_clock = WP_FALSE;

WP_U32 global_clock_rec_diff_ref_clock = WT_CLOCK_REC_DIFF_REF_CLOCK;
WP_U32 global_clock_rec_remote_diff_ref_clock = WT_CLOCK_REC_REMOTE_DIFF_REF_CLOCK;
WP_U32 global_clock_rec_reg_dco_ref_clock = WT_CLOCK_REC_REG_DCO_REF_CLOCK;

WP_U32 global_active_cr_async_mode = WTI_ACTIVE_CR_FOR_ASYNC_MODE;
WP_U32 global_active_cr_extdiff_mode = WTI_ACTIVE_CR_FOR_EXT_DIFF_MODE;
#if WTI_CLOCK_REC_MODE
WP_U32 global_cr_operation_mode = WP_IW_CLOCK_REC_DIFFERENTIAL_MODE;
#else
WP_U32 global_cr_operation_mode = WP_IW_CLOCK_REC_ADAPTIVE_MODE;
#endif /* WTI_CLOCK_REC_MODE */
WP_U32 host_debug_user_offset = 0;
WP_U32 host_debug_user_value = 0;
WP_U32 host_debug_user_size = 0;

WP_U32 slots_config[WTI_MAX_SLOTS];
WP_U32 first_enet_mpls_fa = 0;

WP_U32 skip_tx=0,skip_rx=0;



void cesop_event_function(WP_tag tag, WP_U32 data, WP_U32 info);
void Vt100Normal        (void);
void Vt100Color         (int foreground, int background);
void Vt100GotoXy        (int x, int y);
void Vt100EraseEOL      (void);
void Vt100ClearScreen   (void);

/* Local APIs */
static void   MemoryDisplay          (void);
static WP_U32 execute_command        (void);
static WP_U32 execute_line           (char *line);
static void   separate               (char *line, WP_U32 *subcommand_count);
static WP_U32 shell_md               (WP_U32 argc,char ** argv);
static int    GetAddress             (WP_U32 *addr, char *token);
static int    shell_decode_number    (WP_U32 *addr, char* token);
static WP_U32 tokenize               (char *line);
static void   remove_space           (char *line);
static WP_U8  WTI_SampleSerial       (void);
static void   WTI_InitDemoStructures (void);
#if WTI_CESOP_TDI
void WTI_TdiTransDeviceClockRecModify(WP_U32 index, WP_U32 val);
#else
static int WTI_SpeIdGet              (WP_U32 e1t1_index);
static int WTI_SpeLinkIdGet          (WP_U32 e1t1_index);
static void CLI_F_Check_UFE_Temperature                    (void);
extern int WTI_LineIndexGet          (WP_U32 spe, WP_U32 sub_spe);
#ifdef __WT_UFE3__
static void WTI_Check_UFE_Temperature_4Automation        (char *StrPrm);
#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)
static void CLI_F_CheckValidE1Line(int line_number);
static int CLI_F_AttacheLineToPwNumber (int pw_index);
static int CLI_F_AttachePwToLineNumber (int line_number);
#endif /* (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE) */
#endif /* WT_UFE3 */
static WP_U32 Temperature_Sensor_Standby_Mode = 0;

#endif

/* external functions */
extern char *gets                    (char *);
#if defined __WT_UFE448__ || defined __WT_UFE412__
extern WP_status WPX_ExternalPllClockOutConfig(WPX_pll_cfg *pllConfigParam);
#endif
/* Extern definition needed after separating
   menues to different files */
extern int  CLI_T_Main                                     (void);
extern void CLI_F_RunDefSetup                              (char *StrPrm);
extern void CLI_F_PsnPortDisableEnable                     (char *StrPrm);
extern void CLI_F_PsnTxChannelDisableEnable				   (char *StrPrm);
void CLI_F_ClockRecDiffDcoXoClockSet					   (char *StrPrm);
extern void CLI_F_UfeShowInfo                              (char *StrPrm);
extern void CLI_F_UfeFpgaMode                              (char *StrPrm);
extern void CLI_F_UfeSbiSpeTransferMode                    (char *StrPrm);
extern void CLI_F_UfeTransferMode                          (char *StrPrm);
extern void CLI_F_UfeSystemClkRecMethodSet                 (char *StrPrm);
extern void CLI_F_UfeLineSbiClkTransSet                    (char *StrPrm);
extern void CLI_F_UfeLineTxClkMasterSet                    (char *StrPrm);
extern void CLI_F_UfeCasMode                               (char *StrPrm);
extern void CLI_F_UfeSbiLoopbackMode                       (char *StrPrm);
extern void CLI_F_UfeTdmLoopbackMode                       (char *StrPrm);
extern void CLI_F_UfeClockMode                             (char *StrPrm);
extern void CLI_F_UfeDisplayEvents                         (char *StrPrm);
extern void CLI_F_UfeUpiPort                               (char *StrPrm);
extern void CLI_F_TransDevShowInfo                         (char *StrPrm);
extern void CLI_F_Pwe3ChannelShowInfo                      (char *StrPrm);
extern void CLI_F_Pwe3ChannelQDepth                        (char *StrPrm);
extern void CLI_WinUtilBusErrCheck                         (char *StrPrm);
extern void CLI_WinUtilWingineBusUtilization               (char *StrPrm);
extern void CLI_WinUtilThreadUtilization                   (char *StrPrm);
extern void CLI_WinUtilThreadArbiterDump                   (char *StrPrm);
extern void CLI_WinUtilSerialArbiterDump                   (char *StrPrm);
extern void CLI_WinUtilTraceStart                          (char *StrPrm);
extern void CLI_WinUtilTraceDump                           (char *StrPrm);
extern void CLI_DebugAreDump                               (char *StrPrm);
extern void CLI_DebugVersionInfo                           (char *StrPrm);
extern void CLI_DebugMemDisplay                            (char *StrPrm);
extern void CLI_DebugMemModify                             (char *StrPrm);
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
extern void CLI_F_ClockRecoveryHoldoverForce               (char *StrPrm);
extern void CLI_F_ClockRecoveryHoldoverClear               (char *StrPrm);
extern void CLI_F_ClockRecoveryShowState                   (char *StrPrm);
extern void CLI_F_ClockRecoverySystemsShowState            (char *StrPrm);
extern void CLI_F_ClockRecoveryOutputEnable                (char *StrPrm);
extern void CLI_F_ClockRecoveryOutputDisable               (char *StrPrm);
extern void CLI_F_ClockRecCommand                          (char *StrPrm);
extern void CLI_F_ClockRecDcoDivisorCalc                   (char *StrPrm);
extern void CLI_F_ClockRecIndirectMemoryReadCgen           (char *StrPrm);
extern void CLI_F_ClockRecIndirectMemoryReadRxTsPort       (char *StrPrm);
extern void CLI_F_ClockRecIndirectMemoryReadRxTsMask       (char *StrPrm);
extern void CLI_F_ClockRecIndirectMemoryReadCt             (char *StrPrm);
extern void CLI_F_ClockRecIndirectMemoryRead               (char *StrPrm);
extern void CLI_F_MapJatToLine                             (WP_U8 spe_ja_index[]);
extern void CLI_F_ClockRecDcoHwRegistersDisplay            (char *StrPrm);
extern void CLI_WinUtilBusErrCheck                         (char *StrPrm);
extern void CLI_WinUtilWingineBusUtilization               (char *StrPrm);
extern void CLI_WinUtilThreadUtilization                   (char *StrPrm);
extern void CLI_WinUtilThreadArbiterDump                   (char *StrPrm);
extern void CLI_WinUtilSerialArbiterDump                   (char *StrPrm);
extern void CLI_WinUtilTraceStart                          (char *StrPrm);
extern void CLI_WinUtilTraceDump                           (char *StrPrm);
extern void CLI_DebugVersionInfo                           (char *StrPrm);
#if defined __WT_UFE412__ || defined __WT_UFE448__
extern void CLI_F_Ufe4CRInternalRamDisplay                 (char *StrPrm);
#endif  /* __WT_UFE412__ __WT_UFE448__ */

#endif
extern void CLI_F_Pwe3ChannelStatmode                      (char *StrPrm);
extern void CLI_F_Pwe3ChannelJitterBufferSize              (char *StrPrm);
extern void CLI_F_Pwe3ChannelRxBufferSize                  (char *StrPrm);
extern void CLI_F_Pwe3ChannelUdpDummyMode                  (char *StrPrm);
extern void CLI_F_Pwe3ChannelTxUdcas                       (char *StrPrm);
extern void CLI_F_Pwe3ChannelTxUdpattern                   (char *StrPrm);
extern void CLI_F_Pwe3ChannelTxDummyUdpattern              (char *StrPrm);
extern void CLI_F_Tdm2PsnIwShowInfo                        (char *StrPrm);
extern void CLI_F_Psn2TdmIwShowInfo                        (char *StrPrm);
extern void CLI_F_SlotsConfigShowInfo                      (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggShowInfo                   (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggTag                        (char *StrPrm);
#if WTI_CESOP_MEF8_ENCAP
extern void CLI_F_Tdm2PsnFlowAggEcidHeader                 (char *StrPrm);
#endif
extern void CLI_F_Tdm2PsnFlowAggEnetHeader                 (char *StrPrm);
#if WTI_NUM_OF_VLAN_TAGS == 2
extern void CLI_F_Tdm2PsnFlowAggOuterVlan                  (char *StrPrm);
#endif
#if WTI_NUM_OF_VLAN_TAGS > 0
extern void CLI_F_Tdm2PsnFlowAggInnerVlan                  (char *StrPrm);
#endif
#if WTI_ENET_IEEE_802
extern void CLI_F_Tdm2PsnFlowAggEnet802Header              (char *StrPrm);
#endif
extern void CLI_F_Tdm2PsnFlowAggIpHeader                   (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggUdpHeader                  (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggRtpHeader                  (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggControlWord                (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggTsParams                   (char *StrPrm);
#if WTI_CESOP_TO_MPLS_FA_CASCADING
extern void CLI_F_Tdm2PsnFlowAggRemarkingValues            (char *StrPrm);
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */
extern void CLI_F_Tdm2PsnFlowAggTsConstSet                 (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggIntMode                    (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggTimeStampMode              (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggOvIwFbpMode                (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggFbpDrop                    (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggMtu                        (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggL3Protocol                 (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggCwOffset                   (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggRtpHeaderMode              (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggRtpHeaderOffset            (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggPrefixLen                  (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggL2LengthUpdateMode         (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggL2HeaderOffset             (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggL3LenOffset                (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggL2LenOffset                (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggL3HeaderOffset             (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggL4LenUpdateMode            (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggL4LengthOffset             (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggL4ChecksumRecalc           (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggLastPacketCwLength         (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggLastPacketIpChecksum       (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggInitSeqNumber              (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggStatMode                   (char *StrPrm);
extern void CLI_F_Psn2TdmFlowAggShowInfo                   (char *StrPrm);
extern void CLI_F_Psn2TdmFlowAggTag                        (char *StrPrm);
extern void CLI_F_Psn2TdmFlowAggIntMode                    (char *StrPrm);
extern void CLI_F_Psn2TdmFlowAggStatMode                   (char *StrPrm);
extern void CLI_F_Psn2TdmFlowAggCwOffset                   (char *StrPrm);
extern void CLI_F_Psn2TdmFlowAggRtpHeaderOffset            (char *StrPrm);
extern void CLI_F_Psn2TdmFlowAggExtractionLength           (char *StrPrm);
extern void CLI_F_Psn2TdmFlowAggCasLocation                (char *StrPrm);
extern void CLI_F_Psn2TdmFlowAggControlWordMode            (char *StrPrm);
extern void CLI_F_Psn2TdmFlowAggPayloadCheckType           (char *StrPrm);
extern void CLI_F_Psn2TdmFlowAggPayloadType                (char *StrPrm);
extern void CLI_F_Psn2TdmFlowAggSsrcCheck                  (char *StrPrm);
extern void CLI_F_Psn2TdmFlowAggSsrc                       (char *StrPrm);
extern void CLI_F_Psn2TdmFlowAggTsParams                   (char *StrPrm);
#if WTI_CESOP_MPLS_IW
#if WTI_CESOP_MPLS_OVER_ENET
extern void CLI_F_Tdm2PsnFlowAggEnetHeaderMpls             (char *StrPrm);
#endif
extern void CLI_F_MplsFlowAggPrefixLength                  (char *StrPrm);
extern void CLI_F_MplsFlowAggPrefixHeaderMpls              (char *StrPrm);
extern void CLI_F_MplsFlowAggRemarkingOffsets              (char *StrPrm);
extern void CLI_F_MplsFlowAggCreate                        (char *StrPrm);
extern void CLI_F_FlowAggShowInfoMpls                      (char *StrPrm);
#endif /* WTI_CESOP_MPLS_IW */
extern void CLI_F_Tdm2PsnRxBinShowInfo                     (char *StrPrm);
extern void CLI_F_Psn2TdmTxBinShowInfo                     (char *StrPrm);
extern void CLI_F_Psn2TdmRxBinShowInfo                     (char *StrPrm);
#ifdef WP_HW_WINPATH3
extern void CLI_F_SaDisplay                                (char *StrPrm);
#endif
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
extern void CLI_F_ClockRecDisplay                          (char *StrPrm);
#endif
extern void CLI_F_Psn2TdmTxBinWindowThreshold              (char *StrPrm);
extern void CLI_F_Psn2TdmTxBinWindowMaxThreshold           (char *StrPrm);
extern void CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold    (char *StrPrm);
extern void CLI_F_Psn2TdmTxBinLOPSCounterThreshold         (char *StrPrm);
extern void CLI_F_Psn2TdmTxBinLOPSDetection                (char *StrPrm);
#if WTI_PCE_CLASSIFIER
extern void CLI_F_Psn2TdmPceShowInfo                       (char *StrPrm);
#else
extern void CLI_F_Psn2TdmDfcShowInfo                       (char *StrPrm);
#endif
extern void CLI_F_PwCreate                                 (char *StrPrm);
#if defined __WT_UFE412__ || defined __WT_UFE448__
extern void CLI_F_UfeLinePWCreate			   (char *StrPrm);
extern void CLI_F_UfeLinePWDelete                          (char *StrPrm); 
#endif
#if WTI_CESOP_TO_MPLS_FA_CASCADING
extern void CLI_F_PwCreateCascaded                         (char *StrPrm);
extern void CLI_F_PwCreateCascadedNoPhyEnable              (char *StrPrm);

#if WTI_DUAL_EMPHY
extern void CLI_F_PwCreateCascadedOnSecondUfe              (char *StrPrm);
#endif
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */
extern void CLI_F_PwDelete                                 (char *StrPrm);
extern void CLI_F_PwEnable                                 (char *StrPrm);
extern void CLI_F_PwDisable                                (char *StrPrm);
extern void CLI_F_PwRemove                                 (char *StrPrm);
extern void CLI_F_PW_AddRemoveInLoop                       (char *StrPrm);
extern void CLI_F_SBI_T1_F_AddRemovePW                     (char *StrPrm);
extern void CLI_F_EmphyTransDeviceDisable                  (char *StrPrm);
extern void CLI_F_Pwe3ChannelDisable                       (char *StrPrm);
extern void CLI_F_EmphyTransDeviceEnable                   (char *StrPrm);
extern void CLI_F_Pwe3ChannelEnable                        (char *StrPrm);
extern void CLI_F_EmphyTransDeviceModify                   (char *StrPrm);
extern void CLI_F_Pwe3TransRxChannelModify                 (char *StrPrm);
extern void CLI_F_Pwe3TransTxChannelModify                 (char *StrPrm);
extern void CLI_F_JitterBufferParamsModify                 (char *StrPrm);
extern void CLI_F_Tdm2PsnCesopFlowAggModify                (char *StrPrm);
extern void CLI_F_Tdm2PsnCesopFlowAggControlWordModify     (char *StrPrm);
extern void CLI_F_CesopPayloadSizeModify                   (char *StrPrm);
extern void CLI_F_Tdm2PsnCesopFlowAggPrefixHeaderModify    (char *StrPrm);
extern void CLI_F_Psn2TdmCesopFlowAggModify                (char *StrPrm);
extern void CLI_F_Psn2TdmCesopFlowAggModifyFilterFactors   (char *StrPrm);
extern void CLI_F_ChangeTxClockMode                        (char *StrPrm);
extern void CLI_F_IwPortStatistics                         (char *StrPrm);
#ifdef WP_HW_WINPATH3
extern void CLI_F_BoardPinConfig                           (char *StrPrm);
#endif
extern void CLI_F_E1UnframedNoCasOctal                     (char *StrPrm);
extern void CLI_F_E1Framed1DS0NoCasOctal                   (char *StrPrm);
extern void CLI_F_E1Framed1DS0NoCasShortPacketsOctal       (char *StrPrm);
extern void CLI_F_E1Framed32DS0NoCasOctal                  (char *StrPrm);
extern void CLI_F_E1Framed1DS0CasUnFrgOctal                (char *StrPrm);
extern void CLI_F_E1Framed30DS0CasUnFrgOctal               (char *StrPrm);
extern void CLI_F_E1Framed30DS0CasUnFrgOctal_withModify    (char *StrPrm);
extern void CLI_F_E1Framed30DS0CasFrgOctal                 (char *StrPrm);
extern void CLI_F_E1Framed1DS0CasFrgOctal                  (char *StrPrm);
extern void CLI_F_T1UnframedNoCasOctal                     (char *StrPrm);
extern void CLI_F_T1UnframedNoCasOctalBoard1               (char *StrPrm);
extern void CLI_F_E1UnframedNoCasOctalBoard1               (char *StrPrm);
extern void CLI_F_T1Framed1DS0NoCasOctal                   (char *StrPrm);
extern void CLI_F_T1Framed24DS0NoCasOctal                  (char *StrPrm);
extern void CLI_F_T1Framed1DS0CasUnFrgOctal                (char *StrPrm);
extern void CLI_F_T1Framed24DS0CasUnFrgOctal               (char *StrPrm);
extern void CLI_F_T1Framed1DS0CasFrgOctal                  (char *StrPrm);
extern void CLI_F_T1Framed24DS0CasFrgOctal                 (char *StrPrm);
extern void CLI_F_T1Framed1DS0CasUnFrgOctalOobc            (char *StrPrm);
extern void CLI_F_E1Framed1DS0CasUnFrgOctalOobc            (char *StrPrm);
extern void CLI_F_E1Framed1DS0CasFrgOctalOobc              (char *StrPrm);
extern void CLI_F_SBI_E1_F_NoCas_252_Line_2016_4xDS0_PWS   (char *StrPrm);
extern void CLI_F_SBI_T1_U_NoCas_336_Line_336_PWS          (char *StrPrm);
extern void CLI_F_SBI_E1_F_NoCas_252_Line_8064_1xDS0_PWS   (char *StrPrm);
#if WTI_CESOP_TDI
extern void CLI_F_TdiShowInfo                              (char *StrPrm);
extern void CLI_F_TdiConfigInit                            (char *StrPrm);
extern void CLI_F_TdiOperationMode                         (char *StrPrm);
extern void CLI_F_TdiFramingMode                           (char *StrPrm);
extern void CLI_F_TdiCasMode                               (char *StrPrm);
extern void CLI_F_PwConfigureUnframedTdi                   (char *StrPrm);
extern void CLI_F_PwConfigureFramedTdi                     (char *StrPrm);
extern void CLI_F_TdiTransDeviceModify                     (char *StrPrm);
extern void CLI_F_TdiTransDeviceStatistics                 (char *StrPrm);
extern void CLI_F_IwPortStatistics                         (char *StrPrm);
extern void CLI_F_E1Framed256DS0CasFrgOctal                (char *StrPrm);
extern void CLI_F_301_TDI_E1_U_CR_NoCas_Two_Boards_N_Line_N_PW (char *StrPrm);
extern void CLI_F_301_TDI_T1_U_CR_NoCas_Two_Boards_N_Line_N_PW (char *StrPrm);
extern void CLI_F_301_TDI_E1_F_CR_NoCas_Two_Boards_N_Line_N_PW (char *StrPrm);
extern void CLI_F_301_TDI_T1_F_CR_NoCas_Two_Boards_N_Line_N_PW (char *StrPrm);
extern void CLI_F_303_TDI_E1_F_CR_NoCas_Two_Boards_N_Line_N_PW_1DS0(char *StrPrm);
extern void CLI_F_303_TDI_T1_F_CR_NoCas_Two_Boards_N_Line_N_PW_1DS0(char *StrPrm);
extern void CLI_F_302_TDI_E1_U_CR_NoCas_Two_Boards_1_Line_1_PW_for_Specific_TDM_Port (char *StrPrm);
extern void CLI_F_304_TDI_E1_U_CR_NoCas_Two_Boards_N_Line_N_PW_Remove_Device_Recreate_Without_CR (char *StrPrm);
extern void CLI_F_304_TDI_T1_U_CR_NoCas_Two_Boards_N_Line_N_PW_Remove_Device_Recreate_Without_CR(char *StrPrm);
extern void CLI_F_304_TDI_E1_F_CR_NoCas_Two_Boards_N_Line_N_PW_Remove_Device_Recreate_Without_CR(char *StrPrm);
extern void CLI_F_304_TDI_T1_F_CR_NoCas_Two_Boards_N_Line_N_PW_Remove_Device_Recreate_Without_CR(char *StrPrm);
extern void CLI_F_305_TDI_E1_F_30xDS0_CR_Cas_Frg_Two_Boards_N_Line_N_PW (char *StrPrm);
extern void CLI_F_305_TDI_T1_F_24xDS0_CR_Cas_Frg_Two_Boards_N_Line_N_PW (char *StrPrm);
extern void CLI_F_306_TDI_E1_F_CR_NoCas_Two_Boards_N_Line_2_pw_per_line(char *StrPrm);
extern void CLI_F_307_TDI_T1_F_CR_NoCas_Two_Boards_N_Line_11_plus_1_pw_per_line(char *StrPrm);
#if WTI_CESOP_TO_MPLS_FA_CASCADING
extern void CLI_F_308_TDI_E1_U_CR_NoCas_Mpls_Cascading     (char *StrPrm);
extern void CLI_F_308_TDI_T1_U_CR_NoCas_Mpls_Cascading     (char *StrPrm);
#endif
#if WTI_CESOP_MEF8_ENCAP
extern void CLI_F_MEF8_ENCAP_1_Vlan                        (char *StrPrm);
#endif

#if WTI_CESOP_TO_MPLS_FA_CASCADING
extern void CLI_F_E1Framed256DS0CasFrgOctalMplsCascading   (char *StrPrm);
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */
extern void CLI_F_E1FramedUnFrg16Tdi                       (char *StrPrm);

#if(WTI_CESOP_RX_TIMING_ENABLE ||(WTI_CESOP_CLOCK_RECOVERY_ENABLE && WTI_CLOCK_REC_MODE))
extern void CLI_F_TdiCrTSInt                               (char *StrPrm);
#endif


#else
extern void CLI_F_PwConfigureUnframed                      (char *StrPrm);
extern void CLI_F_PwConfigureFramed                        (char *StrPrm);
extern void CLI_F_UfeChipInitLineSetup                     (char *StrPrm);
extern void CLI_F_TemuxDump                                (char *StrPrm);
extern void CLI_F_UfeGpioClockSelect                       (char *StrPrm);
#if defined  __WT_UFE412__ || defined __WT_UFE448__
#ifdef WT_UFE_FRAMER
extern void AIS_insertion_enable                           (char *StrPrm);
extern void AIS_insertion_disable                          (char *StrPrm);
extern void CLI_F_PdhAisTowardsLine                        (char *StrPrm);
extern void CLI_F_PdhRdiTowardsLine                        (char *StrPrm);
extern void CLI_F_PdhFEACTowardsLine                        (char *StrPrm);
extern void CLI_F_UfeSocketAlarmsMode                       (char *StrPrm);
#endif
void CLI_F_UfeZarlinkPllClockOutConfig                     (char *StrPrm);
void CLI_F_UfeReadZarlinkPllReg                            (char *StrPrm);
extern void CLI_F_UFETerminateRxTraffic                    (char *StrPrm);
#ifdef WT_UFE_FRAMER
void CLI_F_UFEShallowLoopbackEnableDisable		   (char *StrPrm);
void CLI_F_UFEPDHTxTimingSet		                   (char *StrPrm);
void CLI_F_UFEDeepLoopbackEnableDisable                    (char *StrPrm);
#endif
#endif
extern void CLI_F_UfePDHClockSelect                        (char *StrPrm);
extern void CLI_F_UfeGpioClockRead                         (char *StrPrm);
extern void CLI_F_UfeCheckSfp                              (char *StrPrm);
extern void CLI_F_UfeLineProtectionSwitch                  (char *StrPrm);

extern void CLI_F_UFE_Address_Set                          (char *StrPrm);

extern void CLI_F_UfePdhLoopbackMode                       (char *StrPrm);

extern void CLI_F_Enable_PM_Print                           (char *StrPrm);
   

extern void CLI_F_SbiDifferentLinesInSpeE1E1E1             (char *StrPrm);
extern void CLI_F_SbiDifferentLinesInSpeT1T1T1             (char *StrPrm);
extern void CLI_F_E1UnframedNoCasSbi                       (char *StrPrm);
extern void CLI_F_101_103_SBI_E1_U_CR_NoCas_Two_Boards_N_CD_N_CT_63_Line_63_PW(char *StrPrm);
extern void CLI_F_101_103_SBI_T1_U_CR_NoCas_Two_Boards_N_CD_N_CT_84_Line_84_PW(char *StrPrm);
extern void CLI_F_101_103_SBI_E1_F_CR_NoCas_Two_Boards_N_CD_N_CT_63_Line_63_PW(char *StrPrm);
extern void CLI_F_101_103_SBI_T1_F_CR_NoCas_Two_Boards_N_CD_N_CT_84_Line_84_PW(char *StrPrm);
extern void CLI_F_101_103_SBI_E1_F_1xDS0_CR_NoCas_Two_Boards_N_CD_N_CT_252_Line_252_PW(char *StrPrm);
extern void CLI_F_101_103_SBI_T1_F_1xDS0_CR_NoCas_Two_Boards_N_CD_N_CT_336_Line_336_PW(char *StrPrm);
extern WP_U32 CLI_F_105_SBI_E1_U_CR_NoCas_Two_Boards_1_CD_1_CT_63_line_addremove_PW (char *StrPrm);
extern WP_U32 CLI_F_105_SBI_T1_U_CR_NoCas_Two_Boards_1_CD_1_CT_84_line_addremove_PW (char *StrPrm);
extern void CLI_F_109_SBI_DS3_U_CR_NoCas_Two_Boards_3_CD_1_JAT_3_Line_3_PW(char *StrPrm);
extern void CLI_F_109_SBI_DS3_F_CR_NoCas_Two_Boards_3_CD_1_JAT_3_Line_3_PW(char *StrPrm);
extern void CLI_F_112_SBI_E1_F_30DS0_CR_Cas_Two_Boards_N_CD_N_CT_max_Lines_max_PW (char *StrPrm);
extern void CLI_F_112_SBI_T1_F_24DS0_CR_Cas_Two_Boards_N_CD_N_CT_max_Lines_max_PW (char *StrPrm);
#if WTI_CESOP_TO_MPLS_FA_CASCADING
extern void CLI_F_103_SBI_E1_U_CR_NoCas_Mpls_Cascading(char *StrPrm);
#endif

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE && WTI_CESOP_RX_TIMING_ENABLE
extern void CLI_F_102_SBI_E1_U_CR_NoCas_Two_Boards_1_CD_N_CT_63_Line_63_PW (char *StrPrm);
extern void CLI_F_102_SBI_T1_U_CR_NoCas_Two_Boards_1_CD_N_CT_84_Line_84_PW (char *StrPrm);
extern void CLI_F_102_SBI_E1_F_CR_NoCas_Two_Boards_1_CD_N_CT_63_Line_63_PW (char *StrPrm);
extern void CLI_F_102_SBI_T1_F_CR_NoCas_Two_Boards_1_CD_N_CT_84_Line_84_PW (char *StrPrm);
extern void CLI_F_104_SBI_E1_F_CR_NoCas_Two_Boards_1_CD_1_CT_63_Line_252_PW (char *StrPrm);
extern void CLI_F_104_SBI_T1_F_CR_NoCas_Two_Boards_1_CD_1_CT_84_Line_192_PW (char *StrPrm);
extern WP_U32 CLI_F_105_SBI_E1_F_CR_NoCas_Two_Boards_1_CD_1_CT_63_line_addremove_PW (char *StrPrm);
extern WP_U32 CLI_F_105_SBI_T1_F_CR_NoCas_Two_Boards_1_CD_1_CT_84_line_addremove_PW (char *StrPrm);
extern void CLI_F_106_SBI_E1_U_CR_NoCas_Two_Boards_1_CD_1_CT_63_line_with_shaper_modify (char *StrPrm);
extern void CLI_F_106_SBI_T1_U_CR_NoCas_Two_Boards_1_CD_1_CT_84_line_with_shaper_modify (char *StrPrm);
extern void CLI_F_106_SBI_E1_F_CR_NoCas_Two_Boards_1_CD_1_CT_63_line_with_shaper_modify (char *StrPrm);
extern void CLI_F_106_SBI_T1_F_CR_NoCas_Two_Boards_1_CD_1_CT_84_line_with_shaper_modify (char *StrPrm);
extern void CLI_F_107_SBI_E1_F_CR_NoCas_Two_Boards_1_CD_1_CT_Different_DU_size (char *StrPrm);
extern void CLI_F_107_SBI_T1_F_CR_NoCas_Two_Boards_1_CD_1_CT_Different_DU_size (char *StrPrm);
extern void CLI_F_107_SBI_T1_F_CR_NoCas_Two_Boards_1_CD_1_CT_TSWidth_Overflow (char *StrPrm);
extern void CLI_F_108_SBI_E1_U_CR_NoCas_Two_Boards_1_CD_1_CT_63_line_addremove_PW_with_shaper_modify (char *StrPrm);
extern void CLI_F_108_SBI_T1_U_CR_NoCas_Two_Boards_1_CD_1_CT_63_line_addremove_PW_with_shaper_modify (char *StrPrm);
extern void CLI_F_108_SBI_E1_F_CR_NoCas_Two_Boards_1_CD_1_CT_63_line_addremove_PW_with_shaper_modify (char *StrPrm);
extern void CLI_F_108_SBI_T1_F_CR_NoCas_Two_Boards_1_CD_1_CT_63_line_addremove_PW_with_shaper_modify (char *StrPrm);
extern void CLI_F_110_SBI_E1_U_CR_NoCas_Two_Boards_N_CD_N_CT_max_lines_FA_modify (char *StrPrm);
extern void CLI_F_111_SBI_E1_F_CR_NoCas_Two_Boards_Two_CR_Master_Multiple_PW_per_line (char *StrPrm);
extern void CLI_F_111_SBI_T1_F_CR_NoCas_Two_Boards_Two_CR_Master_Multiple_PW_per_line (char *StrPrm);
extern void CLI_F_112_SBI_E1_F_30DS0_CR_Cas_Two_Boards_N_CD_N_CT_max_Lines_max_PW (char *StrPrm);
extern void CLI_F_112_SBI_T1_F_24DS0_CR_Cas_Two_Boards_N_CD_N_CT_max_Lines_max_PW (char *StrPrm);

#if WTI_EXT_DIFF_MODE_ACTIVE
extern void CLI_F_130_SBI_E1_U_CR_EXTENDED_DIFF_NoCas_Two_Boards_252CD_252Lines_252Pw (char *StrPrm);
extern void CLI_F_130_SBI_T1_U_CR_EXTENDED_DIFF_NoCas_Two_Boards_336CD_336Lines_336Pw (char *StrPrm);
extern void CLI_F_130_SBI_E1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_252CD_252Lines_252Pw (char *StrPrm);
extern void CLI_F_130_SBI_T1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_336CD_336Lines_336Pw (char *StrPrm);
extern void CLI_F_130_SBI_E1_F_1DS0_CR_EXTENDED_DIFF_NoCas_Two_Boards_252CD_252Lines_252Pw (char *StrPrm);
extern void CLI_F_130_SBI_T1_F_1DS0_CR_EXTENDED_DIFF_NoCas_Two_Boards_336CD_336Lines_336Pw (char *StrPrm);
extern void CLI_F_131_SBI_E1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_252CD_252Lines_1008Pw (char *StrPrm);
extern void CLI_F_131_SBI_T1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_336CD_336Lines_1344Pw (char *StrPrm);
extern void CLI_F_132_SBI_T1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_336CD_336Lines_672Pw_1DS0 (char *StrPrm);
extern void CLI_F_132_SBI_E1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_1CD_1Lines_32Pw_1DS0 (char *StrPrm);
extern void CLI_F_133_SBI_E1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_252CD_252Lines_1008Pw_AddRemoveLinesPw (char *StrPrm);
extern void CLI_F_133_SBI_T1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_336CD_336Lines_1344Pw_AddRemoveLinesPw (char *StrPrm);
extern void CLI_F_134_SBI_E3_U_CR_EXTENDED_DIFF_NoCas_Two_Boards_12CD_12Lines_12Pw (char *StrPrm);
extern void CLI_F_134_SBI_DS3_U_CR_EXTENDED_DIFF_NoCas_Two_Boards_12CD_12Lines_12Pw (char *StrPrm);
extern void CLI_F_134_SBI_E3_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_12CD_12Lines_12Pw (char *StrPrm);
extern void CLI_F_134_SBI_DS3_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_12CD_12Lines_12Pw (char *StrPrm);
extern void CLI_F_135_SBI_DS3_U_CR_EXTENDED_DIFF_NoCas_Two_Boards_309CD_1DS3_Lines_308T1_Lines_309Pw (char *StrPrm);
extern void CLI_F_136_SBI_E1_F_30xDS0_CR_EXTENDED_DIFF_Cas_Unrg_Two_Boards_252CD_252Lines_252Pw (char *StrPrm);
extern void CLI_F_137_SBI_E1_U_CR_EXTENDED_DIFF_NoCas_Mpls_Cascading_Two_Boards_252CD_252Lines_252Pw (char *StrPrm);
extern void CLI_F_140_SBI_E1_U_CR_COMBINED_DIFF_ADAP_NoCas_Two_Boards_N_EXT_CD_M_REG_CD_252Lines (char *StrPrm);
extern void CLI_F_140_SBI_T1_U_CR_COMBINED_DIFF_ADAP_NoCas_Two_Boards_N_EXT_CD_M_REG_CD_336Lines (char *StrPrm);
extern void CLI_F_140_SBI_E1_F_CR_COMBINED_DIFF_ADAP_NoCas_Two_Boards_N_EXT_CD_M_REG_CD_252Lines (char *StrPrm);
extern void CLI_F_140_SBI_T1_F_CR_COMBINED_DIFF_ADAP_NoCas_Two_Boards_N_EXT_CD_M_REG_CD_336Lines (char *StrPrm);
extern void CLI_F_150_SBI_E1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_252CD_DIFFERENT_DU_PACKET_SIZE (char *StrPrm);
extern void CLI_F_230_CAD_E1_U_CR_EXTENDED_DIFF_NoCas_Two_Boards_4CD_4Lines (char *StrPrm);
extern void CLI_F_230_CAD_T1_U_CR_EXTENDED_DIFF_NoCas_Two_Boards_4CD_4Lines (char *StrPrm);
extern void CLI_F_230_CAD_E1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_4CD_4Lines (char *StrPrm);
extern void CLI_F_230_CAD_T1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_4CD_4Lines (char *StrPrm);
#endif /* WTI_EXT_DIFF_MODE_ACTIVE */

#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE && WTI_CESOP_RX_TIMING_ENABLE */
extern void CLI_F_E1Framed1DS0NoCasSbi                     (char *StrPrm);
extern void CLI_F_E1Framed32DS0NoCasSbi                    (char *StrPrm);
extern void CLI_F_E1Framed2DS0256PwNoCasSbi                (char *StrPrm);
extern void CLI_F_E1Framed1DS0CasUnFrgSbi                  (char *StrPrm);
extern void CLI_F_E1Framed30DS0CasUnFrgSbi                 (char *StrPrm);
extern void CLI_F_E1Framed1DS0CasFrgSbi                    (char *StrPrm);
extern void CLI_F_E1Framed30DS0CasFrgSbi                   (char *StrPrm);
extern void CLI_F_T1UnframedNoCasSbi                       (char *StrPrm);
extern void CLI_F_T1Framed1DS0NoCasSbi                     (char *StrPrm);
extern void CLI_F_T1Framed24DS0NoCasSbi                    (char *StrPrm);
extern void CLI_F_T1Framed1DS0CasUnFrgSbi                  (char *StrPrm);
extern void CLI_F_T1Framed24DS0CasUnFrgSbi                 (char *StrPrm);
extern void CLI_F_T1Framed1DS0CasFrgSbi                    (char *StrPrm);
extern void CLI_F_T1Framed24DS0CasFrgSbi                   (char *StrPrm);
extern void CLI_F_T1FramedNoCasSbiStress2016Pw             (char *StrPrm);
extern void CLI_F_E1UnframedNoCasSbiAddDelete              (char *StrPrm);
extern void CLI_F_201_CAD_E1_U_CR_NoCas_Two_Boards_N_Line_N_PW  (char *StrPrm);
extern void CLI_F_201_CAD_T1_U_CR_NoCas_Two_Boards_N_Line_N_PW  (char *StrPrm);
extern void CLI_F_201_CAD_E1_F_CR_NoCas_Two_Boards_N_Line_N_PW  (char *StrPrm);
extern void CLI_F_201_CAD_T1_F_CR_NoCas_Two_Boards_N_Line_N_PW  (char *StrPrm);
extern void CLI_F_202_CAD_E1_F_CR_NoCas_Two_Boards_N_Line_N_PW_1DS0(char *StrPrm);
extern void CLI_F_202_CAD_T1_F_CR_NoCas_Two_Boards_N_Line_N_PW_1DS0(char *StrPrm);
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE && WTI_CESOP_RX_TIMING_ENABLE
extern void CLI_F_203_CAD_E1_U_CR_NoCas_Two_Boards_N_Line_N_PW_Addremove_PW (char *StrPrm);
extern void CLI_F_204_CAD_T1_F_CR_NoCas_Two_Boards_8_Line_8_PW_one_CR_Master_addremove_PW(char *StrPrm);
#if WTI_CESOP_TO_MPLS_FA_CASCADING
extern void CLI_F_205_CAD_E1_U_CR_NoCas_Mpls_Cascading(char *StrPrm);
#endif
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE && WTI_CESOP_RX_TIMING_ENABLE */
extern void CLI_F_252xDS1E1UnframedNoCasSbi                 (char *StrPrm);
extern void CLI_F_192PwE1FramedNoCasSbi                    (char *StrPrm);
extern void CLI_F_DS3UnframedNoCasSbi                      (char *StrPrm);
extern void CLI_F_E3UnframedNoCasSbi                       (char *StrPrm);
extern void CLI_F_DS3FramedNoCasSbi                      (char *StrPrm);
extern void CLI_F_E3FramedNoCasSbi                       (char *StrPrm);


extern void CLI_F_EmphyTransDeviceStatistics               (char *StrPrm);
#endif


extern void CLI_F_DifferentLinesOctal                      (char *StrPrm);
#if WTI_CESOP_MPLS_IW
extern void CLI_F_FlexmuxInit                              (char *StrPrm);
extern void CLI_F_FlexmuxPoke                              (char *StrPrm);
extern void CLI_F_FlexmuxPeek                              (char *StrPrm);
extern void CLI_F_FlexmuxSquelch_RecClkSet                 (char *StrPrm);
extern void CLI_F_FlexmuxSquelch_RecClkGet                 (char *StrPrm);
extern void CLI_F_Flexmux_SDHHORdiSetMode                  (char *StrPrm);
extern void CLI_F_Flexmux_SDHHOSSBitsSetTx                 (char *StrPrm);
extern void CLI_F_Flexmux_SDHHOSSBitsGetRx                 (char *StrPrm);
extern void CLI_F_FlexmuxAddresBasePeek                    (char *StrPrm);
extern void CLI_F_SonetSdhE1UnframedNoCas                  (char *StrPrm);
extern void CLI_F_SonetSdhE3UnframedNoCas                  (char *StrPrm);
extern void CLI_F_SonetSdhDS3UnframedNoCas                 (char *StrPrm);
extern void CLI_F_SonetSdhE3FramedNoCas                    (char *StrPrm);
extern void CLI_F_SonetSdhDS3FramedM23NoCas                (char *StrPrm);
extern void CLI_F_SonetSdhDS3FramedCParityNoCas            (char *StrPrm);
extern void CLI_F_SonetSdhDS3CreateDelete                  (char *StrPrm);
extern void CLI_F_SonetSdhDS3EnableDisable                 (char *StrPrm);
extern void CLI_F_SonetSdhE3CreateDelete                   (char *StrPrm);
extern void CLI_F_SonetSdhE3EnableDisable                  (char *StrPrm);
extern void CLI_F_SonetSdhE3DS3Combined                      (char *StrPrm);
extern void CLI_F_SonetSdh3DS31VFP3DS328T1                  (char *StrPrm);
extern void CLI_F_SonetSdh3DS32VFP3DS3                     (char *StrPrm);
extern void CLI_F_SonetSdhDS3Loopback                      (char *StrPrm);
extern void CLI_F_SonetSdhE3Loopback                       (char *StrPrm);
extern void CLI_F_SonetSdhDS3ProtectionSwitch              (char *StrPrm);
extern void CLI_F_SonetSdhE3ProtectionSwitch               (char *StrPrm);

extern void CLI_F_SonetSdhE1FramedNoCas                    (char *StrPrm);
extern void CLI_F_SonetSdhE1FramedWithCas                  (char *StrPrm);
extern void CLI_F_SonetSdhT1UnframedNoCas                  (char *StrPrm);
extern void CLI_F_SonetSdhT1FramedNoCas                    (char *StrPrm);
extern void CLI_F_SonetSdhT1FramedWithCas                  (char *StrPrm);
#ifdef __WT_UFE448__
extern void CLI_F_SonetSdhE1FramedNoCas_2016_PWs_16xDS0_per_PW     (char *StrPrm);
extern void CLI_F_SonetSdhT1FramedNoCas_2688_PWs_12xDS0_per_PW     (char *StrPrm);
#elif defined __WT_UFE412__
extern void CLI_F_SonetSdhClearChannel                     (char *StrPrm);
extern void CLI_F_SonetSdhE1FramedNoCas_8xDS0_1008_PWs     (char *StrPrm);
extern void CLI_F_SonetSdhT1FramedNoCas_6xDS0_1344_PWs     (char *srtPrm);
#endif

extern void CLI_F_SonetSdhE1T1Combined                     (char *StrPrm);
#ifdef __WT_UFE412__
extern void CLI_F_SonetSdh252E1_336T1_252E1                (char *StrPrm);
#elif defined __WT_UFE448__
extern void CLI_F_SonetSdh1008E1_1344T1_1008E1                (char *StrPrm);
#endif
extern void CLI_F_SonetSdhSystemReconfigure                (char *StrPrm);
extern void CLI_F_SonetSdhLineReconfigure                  (char *StrPrm);
extern void CLI_F_SonetSdhPhyReconfigure                   (char *StrPrm);
extern void CLI_F_SonetSdhT1PhyCreateDelete                (char *StrPrm);
extern void CLI_F_SonetSdhE1PhyEnableDisable               (char *StrPrm);
extern void CLI_F_SonetSdhT1EnableDisable                  (char *StrPrm);
extern void CLI_F_SonetSdhE1EnableDisable                  (char *StrPrm);
extern void CLI_F_error_4_e1_on_1_tug2                     (char *StrPrm);
extern void CLI_F_error_disable_before_enable              (char *StrPrm);
extern void CLI_F_error_ReEnable_line                      (char *StrPrm);
extern void CLI_F_Error_Enable_Disable_Disable             (char *StrPrm);
extern void CLI_F_Error_Enable_Before_Create               (char *StrPrm);
extern void CLI_F_CR_101_SonetSdh_E1UnframedNoCas          (char *StrPrm);
extern void CLI_F_CR_101_SonetSdh_T1UnframedNoCas          (char *StrPrm);
extern void CLI_F_CR_101_SonetSdh_E1FramedNoCas            (char *StrPrm);
extern void CLI_F_CR_101_SonetSdh_T1FramedNoCas            (char *StrPrm);
extern void CLI_F_CR_101_SonetSdh_E1Framed1xDS0NoCas       (char *StrPrm);
extern void CLI_F_CR_101_SonetSdh_T1Framed1xDS0NoCas       (char *StrPrm);
extern void CLI_F_CR_102_SonetSdh_E1FramedCas              (char *StrPrm);
extern void CLI_F_CR_102_SonetSdh_T1FramedCas              (char *StrPrm);
extern void CLI_F_CR_103_SonetSdh_E1FramedNoCas_MultiplePwPerLine (char *StrPrm);
extern void CLI_F_CR_103_SonetSdh_T1FramedNoCas_MultiplePwPerLine (char *StrPrm);
extern WP_U32 CLI_F_CR_104_SonetSdh_E1UnframedNoCas_AddRemovePwLine (char *StrPrm);
extern WP_U32 CLI_F_CR_104_SonetSdh_T1UnframedNoCas_AddRemovePwLine (char *StrPrm);
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
extern WP_U32 CLI_F_CR_104_SonetSdh_E1FramedNoCas_AddRemovePwLine   (char *StrPrm);
extern WP_U32 CLI_F_CR_104_SonetSdh_T1FramedNoCas_AddRemovePwLine   (char *StrPrm);
extern void CLI_F_CR_104_SonetSdh_E1FramedCas_AddRemovePwLine     (char *StrPrm);
extern void CLI_F_CR_104_SonetSdh_T1FramedCas_AddRemovePwLine     (char *StrPrm);
extern void CLI_F_CR_105_SonetSdh_E1UnframedNoCas_DifferentDuAndPs(char *StrPrm);
extern void CLI_F_CR_105_SonetSdh_T1UnframedNoCas_DifferentDuAndPs(char *StrPrm);
extern void CLI_F_CR_105_SonetSdh_E1FramedNoCas_DifferentDuAndPs(char *StrPrm);
extern void CLI_F_CR_105_SonetSdh_T1FramedNoCas_DifferentDuAndPs(char *StrPrm);
extern void CLI_F_CR_106_SonetSdh_E1UnframedNoCas_AdapDiffPerLine(char *StrPrm);
extern void CLI_F_CR_106_SonetSdh_T1UnframedNoCas_AdapDiffPerLine(char *StrPrm);
extern void CLI_F_CR_106_SonetSdh_E1FramedNoCas_AdapDiffPerLine(char *StrPrm);
extern void CLI_F_CR_106_SonetSdh_T1FramedNoCas_AdapDiffPerLine(char *StrPrm);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
extern void CLI_F_CR_115_Sanity_Board_Check_SonetSdh_E1UnframedNoCas(char *StrPrm);

#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */
#if WTI_CESOP_TO_MPLS_FA_CASCADING
extern void CLI_F_SBI_T1_U_NoCas_336_Line_336_PWS_Cascaded(char *StrPrm);
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */
extern void CLI_F_EmphyPortStatistics                      (char *StrPrm);
extern void CLI_F_Pwe3RxChannelStatistics                  (char *StrPrm);
extern void CLI_F_Pwe3TxChannelStatistics                  (char *StrPrm);
extern void CLI_F_Pwe3RxChannelStatisticsClear             (char *StrPrm);
extern void CLI_F_Pwe3TxChannelStatisticsClear             (char *StrPrm);
extern void CLI_F_HostChannelStatistics                    (char *StrPrm);
extern void CLI_F_LoadHostBus                              (char *StrPrm);
extern void CLI_F_E1Framed1DS0CasUnFrgOctalOobc_2Boards    (char *StrPrm);
extern void CLI_F_T1Framed1DS0CasUnFrgOctalOobc_2Boards    (char *StrPrm);
extern void CLI_F_E1framed1DS0CasSbiOobc_2Boards           (char *StrPrm);
extern void CLI_F_T1framed1DS0CasSbiOobc_2Boards           (char *StrPrm);
#ifdef __linux__
extern void CLI_F_TunDriverListen                          (char *StrPrm);
#endif
#if (WTI_CESOP_MPLS_IW && (!WTI_CESOP_MPLS_OVER_ENET))
extern void CLI_F_HspiDeviceStatistics                     (char *StrPrm);
#else
extern void CLI_F_EnetDeviceStatistics                     (char *StrPrm);
#endif
#if WTI_CESOP_TO_MPLS_FA_CASCADING
extern void CLI_F_Tdm2PsnMplsL2RouterFlowAggStatistics     (char *StrPrm);
extern void CLI_F_Tdm2PsnMplsL2RouterFlowAggStatisticsClear(char *StrPrm);
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */
extern void CLI_F_Tdm2PsnFlowAggStatistics                 (char *StrPrm);
extern void CLI_F_Psn2TdmFlowAggStatistics                 (char *StrPrm);
extern void CLI_F_Tdm2PsnFlowAggStatisticsClear            (char *StrPrm);
extern void CLI_F_Psn2TdmDenyFlowAggStatistics             (char *StrPrm);
extern void CLI_F_Psn2TdmViolationFlowAggStatistics        (char *StrPrm);
extern void CLI_F_IwGlobalStatistics                       (char *StrPrm);
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
extern void CLI_F_ClockRecStatistics                       (char *StrPrm);
extern void CLI_F_ClockRecStatisticsReset                  (char *StrPrm);
extern void CLI_F_ClockRecDebugPrint                       (char *StrPrm);

#if defined __WT_UFE412__ || defined __WT_UFE448__
extern WP_U32 CLI_F_ClockRecStatisticsPPB                  (char *StrPrm);
extern void CLI_F_ClockRecStatisticsNAN					   (char *StrPrm);
/* Function for internal testing and debug */
extern void CLI_F_ClockRecTimestampStub                    (char *StrPrm);
#endif /* UFE4 */


/* End of function for internal testing and debug */
extern WP_U32 CLI_F_ClockRecSystemsStatisticsStatus        (char *StrPrm);
extern WP_U32 CLI_F_ClockRecSystemsStatisticsReset         (char *StrPrm);
extern void CLI_F_ClockRecoveryPhyReconfigure              (char *StrPrm);
#endif
extern void CLI_F_TrafficStart                             (char *StrPrm);
extern void CLI_F_SimulateInterrupts                       (char *StrPrm);
extern void CLI_F_UfeInterrupts                            (char *StrPrm);
extern void CLI_DefaultIwPwe3CesConfig                     (char *StrPrm);
extern void CLI_F_SendLastCasHostCommand                   (char *StrPrm);
extern void CLI_F_PwRegressionResultsCheck                 (char *StrPrm);
extern void CLI_F_RegressionResultsCheck                   (char *StrPrm);
extern void CLI_F_StatShowMem                              (char *StrPrm);
#if WTI_COLLECT_TRACKING_DATA
extern void CLI_F_StatTimingData                           (char *StrPrm);
extern void CLI_F_ResetTimingData                          (char *StrPrm);
#endif
#if WT_UFE_FRAMER
extern void CLI_F_GetAlarmState                            (char *StrPrm);
extern void CLI_F_DisplayUfeEvents                         (char *StrPrm);
#endif
extern void CLI_F_Quit_No_Reset                            (char *StrPrm);
extern void CLI_F_Quit                                     (char *StrPrm);
extern WTI_system *the_system;
#if defined __WT_UFE412__ || defined __WT_UFE448__
extern WUFE_test_system_setup test_setup;
#endif
extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];

extern /*const*/ Y_MenuEntry V_MainMenu                  [];
extern /*const*/ Y_MenuEntry V_Setup                     [];
#if WTI_CESOP_TDI
extern const Y_MenuEntry V_TdiSetup                      [];
#else
extern const Y_MenuEntry V_UfeSetup                      [];
#endif
extern const Y_MenuEntry V_PwAdd                         [];
extern const Y_MenuEntry V_PwDelete                      [];
extern const Y_MenuEntry V_PwEnable                      [];
extern const Y_MenuEntry V_PwDisable                     [];
extern const Y_MenuEntry V_PwDisplay                     [];
extern const Y_MenuEntry V_SystemDisplay                 [];
extern const Y_MenuEntry V_PwModify                      [];
extern const Y_MenuEntry V_SlotsConfigSetup              [];
extern const Y_MenuEntry V_Pwe3ChannelSetup              [];
extern const Y_MenuEntry V_Tdm2PsnIwSystemSetup          [];
extern const Y_MenuEntry V_Psn2TdmIwSystemSetup          [];
extern const Y_MenuEntry V_Tdm2PsnFlowAggSetup           [];
#if WTI_CESOP_TO_MPLS_FA_CASCADING
extern const Y_MenuEntry V_MplsL2FlowAggSetup            [];
extern const Y_MenuEntry V_CascadePwCreate               [];
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */
extern const Y_MenuEntry V_Psn2TdmFlowAggSetup           [];
extern const Y_MenuEntry V_Psn2TdmTxBindingSetup         [];
extern const Y_MenuEntry V_Tdm2PsnTxBindingSetup         [];
extern const Y_MenuEntry V_PreDefinedSystemConfigurations[];
extern const Y_MenuEntry V_Statistics                    [];
extern const Y_MenuEntry V_Host                          [];
extern const Y_MenuEntry V_Menu_Debug                    [];
extern const Y_MenuEntry V_ClockRecoveryUtil             [];
extern const Y_MenuEntry V_DeviceStatistics              [];
extern const Y_MenuEntry V_ChannelStatistics             [];
extern const Y_MenuEntry V_FlowAggStatistics             [];
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
extern const Y_MenuEntry V_ClockRecoveryIndMemDisplay    [];
#endif
/*const*/ Y_MenuEntry V_MainMenu []=
{
   {
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      17
#else
      16
#endif
      ,TRUE, "Main Menu",                                     {(Y_MenuEntry*) V_MainMenu}},
   {K_Menu, TRUE, " -> System setup",                         {(Y_MenuEntry*) V_Setup}},
#if WTI_CESOP_TDI
   {K_Menu, TRUE, " -> TDM interface setup",                  {(Y_MenuEntry*) V_TdiSetup}},
#else
   {K_Menu, TRUE, " -> Ufe setup",                            {(Y_MenuEntry*) V_UfeSetup}},
#endif
   {K_Menu, TRUE, " -> PW add",                               {(Y_MenuEntry*) V_PwAdd}},
   {K_Menu, TRUE, " -> PW delete",                            {(Y_MenuEntry*) V_PwDelete}},
   {K_Menu, TRUE, " -> PW enable",                            {(Y_MenuEntry*) V_PwEnable}},
   {K_Menu, TRUE, " -> PW disable",                           {(Y_MenuEntry*) V_PwDisable}},
   {K_Menu, TRUE, " -> PW modify",                            {(Y_MenuEntry*) V_PwModify}},
   {K_Menu, TRUE, " -> PW display",                           {(Y_MenuEntry*) V_PwDisplay}},
   {K_Menu, TRUE, " -> System display",                       {(Y_MenuEntry*) V_SystemDisplay}},
   {K_Menu, TRUE, " -> Predefined configurations",            {(Y_MenuEntry*) V_PreDefinedSystemConfigurations}},
   {K_Menu, TRUE, " -> Statistics",                           {(Y_MenuEntry*) V_Statistics}},
   {K_Menu, TRUE, " -> Host interface",                       {(Y_MenuEntry*) V_Host}},
   {K_Menu, TRUE, " -> Debug utilities",                      {(Y_MenuEntry*) V_Menu_Debug}},
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   {K_Menu, TRUE, " -> Clock Recovery",                       {(Y_MenuEntry*) V_ClockRecoveryUtil}},
#endif
   {K_Leaf, TRUE, " -> Memory Display",                       {(Y_MnuLeafP)   MemoryDisplay}},
   {K_Leaf, TRUE, " -> Quit without reset",                   {(Y_MnuLeafP)   CLI_F_Quit_No_Reset}},
   {K_Leaf, TRUE, " -> Quit",                                 {(Y_MnuLeafP)   CLI_F_Quit}},
};

Y_MenuEntry V_Setup []=
{
#if WTI_CESOP_TO_MPLS_FA_CASCADING
#if defined __WT_UFE448__ || defined __WT_UFE412__
   { 7 ,TRUE, "Setup",                                                           {(Y_MenuEntry*) V_MainMenu}},
#else
   { 5 ,TRUE, "Setup",                                                           {(Y_MenuEntry*) V_MainMenu}},
#endif
#else
   { 4 ,TRUE, "Setup",                                                           {(Y_MenuEntry*) V_MainMenu}},
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */
   {K_Leaf, TRUE, " -> Run default setup",                                       {(Y_MnuLeafP)   CLI_F_RunDefSetup}},
   {K_Leaf, TRUE, " -> DisableEnble port [delay in microsec]",                   {(Y_MnuLeafP)   CLI_F_PsnPortDisableEnable}},
   {K_Leaf, TRUE, " -> Disable Enble enet channel [0 - disable 1 - enable  (0-3) - channel (optional) ]",      {(Y_MnuLeafP)   CLI_F_PsnTxChannelDisableEnable}},
   {K_Leaf, TRUE, " -> Set Differential, XO and DCO clocks (all clocks will be set to the same value",      {(Y_MnuLeafP)   CLI_F_ClockRecDiffDcoXoClockSet}},
#if WTI_CESOP_TO_MPLS_FA_CASCADING
   {K_Menu, TRUE, " -> MPLS flow aggregation configuration",                     {(Y_MenuEntry*) V_MplsL2FlowAggSetup}},
#if defined __WT_UFE412__ || defined __WT_UFE448__
   {K_Leaf, TRUE, " -> PW and Line create: [line create - 1, no line create - 0],  [E1/T1(0,1)], [Unframed/Framed (0,1)], [SDH/SONET (0,1)], [CAS/NoCAS (1,0)], [AU4/AU3 (0,1)], number of lines, number of slots, first slot in frame",  {(Y_MnuLeafP) CLI_F_UfeLinePWCreate}},
   {K_Leaf, TRUE, " -> PW and Line delete: [delete line - 1, no line delete - 0]",  {(Y_MnuLeafP) CLI_F_UfeLinePWDelete}},
#endif
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */
};

#if WTI_CESOP_TDI
const Y_MenuEntry V_TdiSetup []=
{
   {5 ,TRUE, "TDM interface Setup",                                                        {(Y_MenuEntry*) V_MainMenu}},
   {K_Leaf, TRUE, " -> Show info",                                                         {(Y_MnuLeafP) CLI_F_TdiShowInfo}},
   {K_Leaf, TRUE, " -> Init TDM interfaces (default config: E1, FRAMED, CAS)",             {(Y_MnuLeafP) CLI_F_TdiConfigInit}},
   {K_Leaf, TRUE, " -> Operation mode [port_index(0-15) op_mode(0-T1, 1-E1)]",             {(Y_MnuLeafP) CLI_F_TdiOperationMode}},
   {K_Leaf, TRUE, " -> Framing mode [port_index(0-15) framing_mode(0-UNFRAMED, 1-FRAMED)]",{(Y_MnuLeafP) CLI_F_TdiFramingMode}},
   {K_Leaf, TRUE, " -> CAS mode [port_index(0-15) cas_mode(0-NO CAS, 1-CAS)]",             {(Y_MnuLeafP) CLI_F_TdiCasMode}},
};
#else
const Y_MenuEntry V_UfeSetup []=
{
   {
#ifdef __WT_UFE3__
      16
#else  /* UFE4 or TDI  */
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
#ifdef WT_UFE_FRAMER
#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING
                  28
#else
                  27
#endif
#else
                  12
#endif /* #ifdef WT_UFE_FRAMER */

#else /* ! WTI_CESOP_CLOCK_RECOVERY_ENABLE */
#ifdef WT_UFE_FRAMER
#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING
                  27
#else
                  26
#endif
#else
                  11
#endif /* #ifdef WT_UFE_FRAMER */

#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
#endif /* WT_UFE3 */
      ,TRUE, "UFE Setup",                                                        {(Y_MenuEntry*) V_MainMenu}},
   {K_Leaf, TRUE, " -> Show info",                                               {(Y_MnuLeafP) CLI_F_UfeShowInfo}},
   {K_Leaf, TRUE, " -> Display UFE events",                                      {(Y_MnuLeafP) CLI_F_UfeDisplayEvents}},
   {K_Leaf, TRUE, " -> Check UFE Temperature",                                   {(Y_MnuLeafP) CLI_F_Check_UFE_Temperature}},
#ifdef __WT_UFE3__
   {K_Leaf, TRUE, " -> UPI port [0-UPI1, 1-UPI2]",                               {(Y_MnuLeafP) CLI_F_UfeUpiPort}},
   {K_Leaf, TRUE, " -> FPGA mode [1-OCTAL, 2-SBI]",                              {(Y_MnuLeafP) CLI_F_UfeFpgaMode}},
   {K_Leaf, TRUE, " -> SPE transfer mode [spe_id (0-2) spe_transfer_mode (0-IDLE, 1-E1, 2-T1, 3-DS3, 4-E3)]",{(Y_MnuLeafP) CLI_F_UfeSbiSpeTransferMode}},
   {K_Leaf, TRUE, " -> Transfer mode [line_index (octal (0-7), SBI (0-83)) transfer_mode (1-T1_FRM, 2-T1_UNFRM 3-E1_FRM 4-E1_UNFRM), clock_rec_mode 0-disable 1-tx 2-rx 3-duplex]",{(Y_MnuLeafP) CLI_F_UfeTransferMode}},
   {K_Leaf, TRUE, " -> CAS mode [line_index (octal (0-7), SBI (0-83)) cas_mode (0-DISABLE, 1-ENABLE)]", {(Y_MnuLeafP) CLI_F_UfeCasMode}},
   {K_Leaf, TRUE, " -> Set clock recovery method (0-adaptive, 1-differential)]", {(Y_MnuLeafP) CLI_F_UfeSystemClkRecMethodSet}},
   {K_Leaf, TRUE, " -> Clock translator ID [line_index(0-83), translator ID (0-31)]", {(Y_MnuLeafP) CLI_F_UfeLineSbiClkTransSet}},
   {K_Leaf, TRUE, " -> Set UFE line Tx clock mode [line_index(0-83))]",          {(Y_MnuLeafP) CLI_F_UfeLineTxClkMasterSet}},
   {K_Leaf, TRUE, " -> SBI loopback mode [0-NORMAL, 1-LOOPBACK, 2-SYSTEM LOOPBACK]",                {(Y_MnuLeafP) CLI_F_UfeSbiLoopbackMode}},
   {K_Leaf, TRUE, " -> TDM loopback mode [0-NO LPBK,1-SYSTEM LPBK,2-LINE LPBK][line_index]", {(Y_MnuLeafP) CLI_F_UfeTdmLoopbackMode}},
   {K_Leaf, TRUE, " -> Clock mode [0-LOOP TIMING, 1-EXTERNAL]",                  {(Y_MnuLeafP) CLI_F_UfeClockMode}},
   {K_Leaf, TRUE, " -> Initialize external chip and create UFE lines",           {(Y_MnuLeafP) CLI_F_UfeChipInitLineSetup}},
   {K_Leaf, TRUE, " -> Temux Register dump",                                     {(Y_MnuLeafP) CLI_F_TemuxDump}},
#else /* __WT_UFE412__  __WT_UFE448__ */
   {K_Leaf, TRUE, " -> GPIO clock Select [GPIO number (0,1) clock select (0-RECV_PDH_TX_CLK_0, 1-RECV_PDH_TX_CLK_1, 2-RECV_PDH_LINE_CLK_0, 3-RECV_PDH_LINE_CLK_1, 4-GP_CLK_IN_0, 5-GP_CLK_IN_1, 6-SYSTEM_CLK, 7-XO_PLL_REF_CLK, 8-RECV_SDH_LINE_CLK_1) Reference Output Clock Select (0-XO_REF_CLK, 1-DIFF_REF_CLK)]",      {(Y_MnuLeafP) CLI_F_UfeGpioClockSelect}},
   {K_Leaf, TRUE, " -> Zarlink PLL clock Config [Clock (0-p0_clk0,1-p1_clk0,2-APLL_CLK0) clock value (p0/1_clk0: Value*8KHz, apll: see table in spec)]",      {(Y_MnuLeafP) CLI_F_UfeZarlinkPllClockOutConfig}},
   {K_Leaf, TRUE, " -> Read Zarlink PLL Register [Register Address]",      {(Y_MnuLeafP) CLI_F_UfeReadZarlinkPllReg}},
   {K_Leaf, TRUE, " -> PDH clock Select [output (0-RECV_PDH_LINE_CLK_0 1-RECV_PDH_LINE_CLK_1) line index (0..1343) rate (optional: 0-E1 1-T1)]",      {(Y_MnuLeafP) CLI_F_UfePDHClockSelect}},
   {K_Leaf, TRUE, " -> GPIO clock state",                                        {(Y_MnuLeafP) CLI_F_UfeGpioClockRead}},
   {K_Leaf, TRUE, " -> Check SFP access",                                    {(Y_MnuLeafP) CLI_F_UfeCheckSfp}},
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   {K_Leaf, TRUE, " -> UFE4 Clock Rec internal ram display [optional: line_index]",         {(Y_MnuLeafP) CLI_F_Ufe4CRInternalRamDisplay}},
#endif
#ifdef WT_UFE_FRAMER
   {K_Leaf, TRUE, " -> Line Protection Switch [flexmux_id][line_port 0,1,2... , 8-bulk ] [protection mode (0-working line, 1-protected line) ]",  {(Y_MnuLeafP) CLI_F_UfeLineProtectionSwitch}},
   {K_Leaf, TRUE, " -> Check UFE Framer PRBS [optional: Line index, or range of indexes separated by '-' or list separated by ','. Otherwise all lines]",       {  (Y_MnuLeafP) CLI_F_UfeFramerPrbsCheck}},
   {K_Leaf, TRUE, " -> UFE Shallow Line Loopback Enable/Disable [port_index (0-4), disable/enable(0,1)]",         {(Y_MnuLeafP) CLI_F_UFEShallowLoopbackEnableDisable}},
   {K_Leaf, TRUE, " -> UFE Deep Line Loopback Enable/Disable [client_port_id , disable/enable(0,1)]",         {(Y_MnuLeafP) CLI_F_UFEDeepLoopbackEnableDisable}},
   {K_Leaf, TRUE, " -> UFE PDH Tx Line Timing Set [client timing(0-Rx ,1-CT, 2-System, 3-CR)]",         {(Y_MnuLeafP) CLI_F_UFEPDHTxTimingSet }},
   {K_Leaf, TRUE, " -> UFE framer poke (write)  [flexmux_id][Address,value]",         {(Y_MnuLeafP)CLI_F_FlexmuxPoke }},
   {K_Leaf, TRUE, " -> UFE framer peek (read)   [flexmux_id][address]",          {(Y_MnuLeafP)CLI_F_FlexmuxPeek }},
   {K_Leaf, TRUE, " -> UFE framer address base peek (read)  [flexmux_id] [address]",   {(Y_MnuLeafP)CLI_F_FlexmuxAddresBasePeek}},
   {K_Leaf, TRUE, " -> UFE framer clock squelch SET [flexmux_id][output sel:0,1][squelch mode:0-disable,1-enable]",   {(Y_MnuLeafP)CLI_F_FlexmuxSquelch_RecClkSet}},
   {K_Leaf, TRUE, " -> UFE framer clock squelch GET [flexmux_id][output sel:0,1]",   {(Y_MnuLeafP)CLI_F_FlexmuxSquelch_RecClkGet}},
#endif
   {K_Leaf, TRUE, " -> PDH loopback mode [0-NO LPBK,1-SYSTEM LPBK,2-LINE LPBK][line_index]", {(Y_MnuLeafP) CLI_F_UfePdhLoopbackMode}},
   {K_Leaf, TRUE, " -> UFE Rx traffic termination [AUG-1][enable]",   {(Y_MnuLeafP)CLI_F_UFETerminateRxTraffic}},
   {K_Leaf, TRUE, " -> UFE Set address [Address(hex)][Value(hex)]",   {(Y_MnuLeafP)CLI_F_UFE_Address_Set}},
#ifdef WT_UFE_FRAMER
   {K_Leaf, TRUE, " -> UFE framer SDH HO RDI Set Mode [flexmux_id][stm4][stm1][stm0][HO type- (0-VC3,1-VC4)] [type- (0-RDI 1 bit mode,1-RDI 3 bit mode)",          {(Y_MnuLeafP)CLI_F_Flexmux_SDHHORdiSetMode }},
   {K_Leaf, TRUE, " -> UFE framer SDH HO SS Bits Set Tx [flexmux_id][stm4][stm1][stm0][HO type- (0-VC3,1-VC4)] [Value- (0x0-0x3)]",          {(Y_MnuLeafP)CLI_F_Flexmux_SDHHOSSBitsSetTx }},
   {K_Leaf, TRUE, " -> UFE framer SDH HO SS Bits Get Rx [flexmux_id][stm4][stm1][stm0][HO type- (0-VC3,1-VC4)]",          {(Y_MnuLeafP)CLI_F_Flexmux_SDHHOSSBitsGetRx }},
   {K_Leaf, TRUE, " -> UFE framer socket alarms enable/disable [0-disable 1-enable]",   {(Y_MnuLeafP)CLI_F_UfeSocketAlarmsMode}},
#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING
   {K_Leaf, TRUE, " -> CLI_F_Enable_PM_Print[0-Port,1-HOP,2-LOP,3-PDH][pm point][0-disable,1-enable]",   {(Y_MnuLeafP)CLI_F_Enable_PM_Print}},
#endif
#endif    
   
#endif /* ifdef  __WT_UFE3__ */

};

#endif  /* not WTI_CESOP_TDI */
const Y_MenuEntry V_PwAdd []=
{
   { 6 ,TRUE, "PW add",                                                          {(Y_MenuEntry*) V_MainMenu}},
   {K_Menu, TRUE, " -> PW add: PW basic configuration",                          {(Y_MenuEntry*) V_SlotsConfigSetup}},
   {K_Menu, TRUE, " -> PW add: Transparent PWE3 Tx channel configuration",       {(Y_MenuEntry*) V_Pwe3ChannelSetup}},
   {K_Menu, TRUE, " -> PW add: TDM to PSN flow aggregation configuration",       {(Y_MenuEntry*) V_Tdm2PsnFlowAggSetup}},
   {K_Menu, TRUE, " -> PW add: PSN to TDM flow aggregation configuration",       {(Y_MenuEntry*) V_Psn2TdmFlowAggSetup}},
   {K_Menu, TRUE, " -> PW add: PSN to TDM Tx binding configuration",             {(Y_MenuEntry*) V_Psn2TdmTxBindingSetup}},
#if WTI_CESOP_TO_MPLS_FA_CASCADING
   {K_Menu, TRUE, " -> PW add: PW create",                                       {(Y_MenuEntry*) V_CascadePwCreate}},
#else /* !WTI_CESOP_TO_MPLS_FA_CASCADING */
   {K_Leaf, TRUE, " -> PW add: PW create [(oobc mode) (rx_clock_master) (tx_clock_master) <index>]",  {(Y_MnuLeafP) CLI_F_PwCreate}},
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */
};

const Y_MenuEntry V_SlotsConfigSetup []=
{
#if WTI_CESOP_TDI
   { 2 ,TRUE, "PW configuration",                                                {(Y_MenuEntry*) V_PwAdd}},
   {K_Leaf, TRUE, " -> Configure PW for UNFRAMED mode [port_index(0-16), transfer_mode(0-T1,1-E1)]",{(Y_MnuLeafP) CLI_F_PwConfigureUnframedTdi}},
   {K_Leaf, TRUE, " -> Configure PW for FRAMED mode [port_index(0-16), transfer_mode(0-T1,1-E1), cas_mode(0-NO_CAS,1-CAS) num_slots(1-32) ,\n\r\tslots_string(slots indices separated by \",\" or by \"-\" for range of slots]", {(Y_MnuLeafP) CLI_F_PwConfigureFramedTdi}},
#else
   { 2 ,TRUE, "PW configuration",                                                {(Y_MenuEntry*) V_PwAdd}},
   {K_Leaf, TRUE, " -> Configure PW for UNFRAMED mode \n\r\t[line index (SBI - E1 0-62, cas T1 0-83, OCTAL - E1 0-7, T1 0-7 ), \n\r\trx dataunit size (2 - 64), \n\r\ttx dataunit size (2 - 64) ]",                               {(Y_MnuLeafP) CLI_F_PwConfigureUnframed}},
   {K_Leaf, TRUE, " -> Configure PW for FRAMED mode \n\r\t[line index (SBI - E1 0-62, cas T1 0-83, OCTAL - E1 0-7, T1 0-7 ), \n\r\trx dataunit size (2 - 64), \n\r\ttx dataunit size (2 - 64), \n\r\tnum_slots (T1 0-23,E1 0-31),\n\r\tslots_string(slots indices separated by \",\" or by \"-\" for range of slots)]",
    {(Y_MnuLeafP) CLI_F_PwConfigureFramed}},
#endif
};

const Y_MenuEntry V_Pwe3ChannelSetup []=
{
   { 6 ,TRUE, "PWE3 channel setup",                                              {(Y_MenuEntry*) V_PwAdd}},
   {K_Leaf, TRUE, " -> Statistics mode [0-DISABLE, 1-ENABLE]",                   {(Y_MnuLeafP) CLI_F_Pwe3ChannelStatmode}},
   {K_Leaf, TRUE, " -> Tx Jitter buffer size [size (power of 2)]",               {(Y_MnuLeafP) CLI_F_Pwe3ChannelJitterBufferSize}},
   {K_Leaf, TRUE, " -> Rx buffer size (size)",                                   {(Y_MnuLeafP) CLI_F_Pwe3ChannelRxBufferSize}},
   {K_Leaf, TRUE, " -> Tx UDP dummy mode [0-last frame 1-UDP]",                  {(Y_MnuLeafP) CLI_F_Pwe3ChannelUdpDummyMode}},
   {K_Leaf, TRUE, " -> Tx udcas nibble [0-f without 0x prefix]",                 {(Y_MnuLeafP) CLI_F_Pwe3ChannelTxUdcas}},
   {K_Leaf, TRUE, " -> Tx underrun udpattern [bytes string separated by \",\" must be num_slots length e.g ee,07 for channnel with 2 slots]",
    {(Y_MnuLeafP) CLI_F_Pwe3ChannelTxUdpattern}},
   {K_Leaf, TRUE, " -> Tx dummy udpattern [bytes string separated by \",\" must be num_slots length e.g ee,07 for channnel with 2 slots]",                                                                                    {(Y_MnuLeafP) CLI_F_Pwe3ChannelTxDummyUdpattern}},
};

const Y_MenuEntry V_Tdm2PsnFlowAggSetup []=
{
   {21
#if WTI_CESOP_TO_MPLS_FA_CASCADING
    + 1
#endif
    + WTI_NUM_OF_VLAN_TAGS, TRUE, "TDM to PSN Flow aggregation setup",                              {(Y_MenuEntry*) V_PwAdd}},
   {K_Leaf, TRUE, " -> Tag [tag (0x)] (default incremental)",                    {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggTag}},
   {K_Leaf, TRUE, " -> Enet header [dest_mac (0x,8 0x,8 0x,8 0x,8 0x,8 0x,8),source_mac (0x,8 0x,8 0x,8 0x,8 0x,8 0x,8), type (0x,16)]",{(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggEnetHeader}},
#if WTI_NUM_OF_VLAN_TAGS == 2
   {K_Leaf, TRUE, " -> Enet Outer vlan [vlan_id]",{(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggOuterVlan}},
#endif
#if WTI_NUM_OF_VLAN_TAGS > 0
   {K_Leaf, TRUE, " -> Enet Inner vlan [vlan_id]",{(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggInnerVlan}},
#endif
   {K_Leaf, TRUE, " -> Ip Header (checksum and length are calculated automatically, insert 0)\n\r\t[ver_ihl_tos_length (0x,32), id_frag_offset (0x,32), ttl_protocol_checksum (0x,32), source_ip (0x,32), dest_ip (0x,32)]", {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggIpHeader}},
   {K_Leaf, TRUE, " -> Udp Header [source_port (0x, 16), dest_port (0x, 16), length (0x, 16), checksum (0x, 16)]", {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggUdpHeader}},
   {K_Leaf, TRUE, " -> Rtp [control (0x, 8), payload_type (0x, 8), seq_number (0x, 16), timestamp (0x, 32), ssrc (0x, 32)]", {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggRtpHeader}},
   {K_Leaf, TRUE, " -> Control word [control (0x, 8), frg_len (0x, 8), seq_number (0x, 16)]", {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggControlWord}},
   {K_Leaf, TRUE, " -> Interrupt Mode [0-DISABLED 1-ENABLED]",                   {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggIntMode}},
   {K_Leaf, TRUE, " -> Statistics Mode [0-DISABLED 1-ENABLED]",                  {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggStatMode}},
   {K_Leaf, TRUE, " -> Timestamp Mode [0-DISABLED 1-ENABLED]",                   {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggTimeStampMode}},
   {K_Leaf, TRUE, " -> Overflow IW FBP [0-DISABLED 1-ENABLED]",                  {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggOvIwFbpMode}},
   {K_Leaf, TRUE, " -> FBP Drop Threshold [0-65355]",                            {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggFbpDrop}},
   {K_Leaf, TRUE, " -> MTU [(size)]",                                            {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggMtu}},
   {K_Leaf, TRUE, " -> L3 Protocol [0-IPv4 1-MPLS]",                             {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggL3Protocol}},
   {K_Leaf, TRUE, " -> Control Word Offset [(1-60)]",                            {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggCwOffset}},
   {K_Leaf, TRUE, " -> RTP Header Mode [0-DISABLED 1-ENABLED]",                  {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggRtpHeaderMode}},
   {K_Leaf, TRUE, " -> RTP header offset [(0-56)]",                              {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggRtpHeaderOffset}},
   {K_Leaf, TRUE, " -> Prefix Length [(0-64)]",                                  {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggPrefixLen}},
   {K_Leaf, TRUE, " -> L2 Length Update Mode [0-DISABLED 1-ENABLED]",            {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggL2LengthUpdateMode}},
   {K_Leaf, TRUE, " -> L2 length Offset [(0-64)]",                               {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggL2LenOffset}},
   {K_Leaf, TRUE, " -> L3 Header offset [(0-64)]",                               {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggL3HeaderOffset}},
   {K_Leaf, TRUE, " -> L4 Length Update Mode [0-DISABLED 1-ENABLED]",            {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggL4LenUpdateMode}},
   {K_Leaf, TRUE, " -> L4 Length Offset [(0-64)]",                               {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggL4LengthOffset}},
   {K_Leaf, TRUE, " -> L4 Checksum Recal Mode [0-DISABLED 1-ENABLED]",           {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggL4ChecksumRecalc}},
   {K_Leaf, TRUE, " -> Last Packet CW Length [(0-63)]",                          {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggLastPacketCwLength}},
   {K_Leaf, TRUE, " -> Last Packet IP Checksum [(checksum)]",                    {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggLastPacketIpChecksum}},
   {K_Leaf, TRUE, " -> Initial Sequence Number [(0-65355)]",                     {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggInitSeqNumber}},
   {K_Leaf, TRUE, " -> TS parameters [ts_used(0-used,1-unused),ts_method(0-WP_IW_CESOP_ABS_TS,1-WP_IW_CESOP_DIFF_TS),ts_diff_if_id(0-31)]", {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggTsParams}},
#if WTI_CESOP_TO_MPLS_FA_CASCADING
   {K_Leaf, TRUE, " -> Remarking values [(n_fields) (val1) (val2) (val3)]",      {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggRemarkingValues}}
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */
};
#if WTI_CESOP_TO_MPLS_FA_CASCADING

const Y_MenuEntry V_CascadePwCreate []=
{
   { 2 ,TRUE, "Pw Create",                                                                                                       {(Y_MenuEntry*) V_PwAdd}},
   {K_Leaf, TRUE, " -> PW add: PW create (direct)   [(oobc mode) (rx_clock_master) (tx_clock_master) <index>]",                  {(Y_MnuLeafP) CLI_F_PwCreate}},
   {K_Leaf, TRUE, " -> PW add: PW create (cascaded) [(oobc mode) (rx_clock_master) (tx_clock_master) (Mpls FA index) <index>]",  {(Y_MnuLeafP) CLI_F_PwCreateCascaded}}
};

const Y_MenuEntry V_MplsL2FlowAggSetup []=
{
#if WTI_CESOP_MPLS_OVER_ENET
   { 6 ,TRUE, "MPLS Flow aggregation setup",                                     {(Y_MenuEntry*) V_Setup}},
   {K_Leaf, TRUE, " -> Enet header [dest_mac (0x,8 0x,8 0x,8 0x,8 0x,8 0x,8),source_mac (0x,8 0x,8 0x,8 0x,8 0x,8 0x,8), type (0x,16)]",{(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggEnetHeaderMpls}},
#else
   { 5 ,TRUE, "MPLS Flow aggregation setup",                                     {(Y_MenuEntry*) V_Setup}},
#endif /* WTI_CESOP_MPLS_OVER_ENET */
   {K_Leaf, TRUE, " -> Prefix length [prefix length]",                           {(Y_MnuLeafP) CLI_F_MplsFlowAggPrefixLength}},
   {K_Leaf, TRUE, " -> Prefix header MPLS [(word1) (word2) (word3) (word4) (word5) (word6)]",  {(Y_MnuLeafP) CLI_F_MplsFlowAggPrefixHeaderMpls}},
   {K_Leaf, TRUE, " -> Remarking offsets [(n_fields) (offset1) (offset2) (offset3)]", {(Y_MnuLeafP) CLI_F_MplsFlowAggRemarkingOffsets}},
   {K_Leaf, TRUE, " -> MPLS FA Create [index]",                                  {(Y_MnuLeafP) CLI_F_MplsFlowAggCreate}},
   {K_Leaf, TRUE, " -> MPLS flow aggregation display [index, dir (0-tdm2psn 1-psn2tdm)]", {(Y_MnuLeafP) CLI_F_FlowAggShowInfoMpls}},
};

#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */

const Y_MenuEntry V_Psn2TdmFlowAggSetup []=
{
   {
      14
      ,TRUE, "PSN to TDM Flow aggregation setup",                     {(Y_MenuEntry*) V_PwAdd}},
   {K_Leaf, TRUE, " -> Show info [pw_index]",                           {(Y_MnuLeafP) CLI_F_Psn2TdmFlowAggShowInfo}},
   {K_Leaf, TRUE, " -> Tag [tag(0x)]",                                  {(Y_MnuLeafP) CLI_F_Psn2TdmFlowAggTag}},
   {K_Leaf, TRUE, " -> Interrupt Mode [0-DISABLED 1-ENABLED]",          {(Y_MnuLeafP) CLI_F_Psn2TdmFlowAggIntMode}},
   {K_Leaf, TRUE, " -> Statistics Mode [0-DISABLED 1-ENABLED]",         {(Y_MnuLeafP) CLI_F_Psn2TdmFlowAggStatMode}},
   {K_Leaf, TRUE, " -> Control Word Offset [0-64]",                     {(Y_MnuLeafP) CLI_F_Psn2TdmFlowAggCwOffset}},
   {K_Leaf, TRUE, " -> RTP header offset [0-64]",                       {(Y_MnuLeafP) CLI_F_Psn2TdmFlowAggRtpHeaderOffset}},
   {K_Leaf, TRUE, " -> Extraction Length [0-64]",                       {(Y_MnuLeafP) CLI_F_Psn2TdmFlowAggExtractionLength}},
   {K_Leaf, TRUE, " -> Cas location [0-NIBBLE HIGH 1-NIBBLE LOW]",      {(Y_MnuLeafP) CLI_F_Psn2TdmFlowAggCasLocation}},
   {K_Leaf, TRUE, " -> Control Word Mode [0-DISABLED 1-ENABLED]",       {(Y_MnuLeafP) CLI_F_Psn2TdmFlowAggControlWordMode}},
   {K_Leaf, TRUE, " -> Payload Check Type [0-DISABLED 1-ENABLED] ",     {(Y_MnuLeafP) CLI_F_Psn2TdmFlowAggPayloadCheckType}},
   {K_Leaf, TRUE, " -> Payload Type",                                   {(Y_MnuLeafP) CLI_F_Psn2TdmFlowAggPayloadType}},
   {K_Leaf, TRUE, " -> SSRC Check [0-DISABLED 1-ENABLED]",              {(Y_MnuLeafP) CLI_F_Psn2TdmFlowAggSsrcCheck}},
   {K_Leaf, TRUE, " -> SSRC",                                           {(Y_MnuLeafP) CLI_F_Psn2TdmFlowAggSsrc}},
   {K_Leaf, TRUE, " -> TS parameters [ts_used(0-used,1-unused), ts_diff_const(0-0xffff)]", {(Y_MnuLeafP) CLI_F_Psn2TdmFlowAggTsParams}},
};

const Y_MenuEntry V_Psn2TdmTxBindingSetup []=
{
   { 4 ,TRUE, "PSN to TDM Tx Binding setup",                            {(Y_MenuEntry*) V_PwAdd}},
   {K_Leaf, TRUE, " -> Window Tx Threshold [(0-Jitter Buffer Size)]",   {(Y_MnuLeafP) CLI_F_Psn2TdmTxBinWindowThreshold}},
   {K_Leaf, TRUE, " -> Window Switchover Threshold [(0-255)]",          {(Y_MnuLeafP) CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold}},
   {K_Leaf, TRUE, " -> LOPS detection [enable(0/1), synch(1-10), out-of-synch(1-15)]",          {(Y_MnuLeafP) CLI_F_Psn2TdmTxBinLOPSDetection }},
   {K_Leaf, TRUE, " -> Consecutive packets [synch(1-10), out-of-synch(1-15)]",                  {(Y_MnuLeafP) CLI_F_Psn2TdmTxBinLOPSCounterThreshold}},
};

const Y_MenuEntry V_PwDelete []=
{
   { 1 ,TRUE, "PW delete",                                                                  {(Y_MenuEntry*) V_MainMenu}},
   {K_Leaf, TRUE, " -> PW delete [pw_index]",                                               {(Y_MnuLeafP) CLI_F_PwDelete}},
};

const Y_MenuEntry V_PwEnable []=
{
   { 3 ,TRUE, "PW Enable",                                                                  {(Y_MenuEntry*) V_MainMenu}},
   {K_Leaf, TRUE, " -> PW : PW enable (Tx and Rx channel enable) [pw_index, clock_rec_master(0,1)]",               {(Y_MnuLeafP) CLI_F_PwEnable}},
   {K_Leaf, TRUE, " -> PW : EMPHY Transparent device enable [pw_index]",                    {(Y_MnuLeafP) CLI_F_EmphyTransDeviceEnable}},
   {K_Leaf, TRUE, " -> PW : Transparent PWE3 channel enable [type(0-Rx,1-Tx),pw_index]",    {(Y_MnuLeafP) CLI_F_Pwe3ChannelEnable}},
};

const Y_MenuEntry V_PwDisable []=
{
   { 3 ,TRUE, "PW Disable",                                                                  {(Y_MenuEntry*) V_MainMenu}},
   {K_Leaf, TRUE, " -> PW : PW disable (Tx and Rx channel disable) [pw_index]",              {(Y_MnuLeafP) CLI_F_PwDisable}},
   {K_Leaf, TRUE, " -> PW : Transparent PWE3 channel disable [type(0-Rx,1-Tx),pw_index]",    {(Y_MnuLeafP) CLI_F_Pwe3ChannelDisable}},
   {K_Leaf, TRUE, " -> PW : EMPHY Transparent device disable [pw_index]",                    {(Y_MnuLeafP) CLI_F_EmphyTransDeviceDisable}},
};

const Y_MenuEntry V_PwModify []=
{
   { 10 ,TRUE, "PW modify",                                                                   {(Y_MenuEntry*) V_MainMenu}},
   {K_Leaf, TRUE, " -> EMPHY Transparent device modify [ \n\r\t(pw_index mod_com value) \n\r\t mod_com - 1 = WP_DEV_MOD_EMPHY_TRANS_MODE value = (0-(WP_EMPHY_TRANS_UNFRAMED | WP_EMPHY_TRANS_T1),1-(WP_EMPHY_TRANS_FRAMED | WP_EMPHY_TRANS_T1),3-(WP_EMPHY_TRANS_FRAMED_CAS | WP_EMPHY_TRANS_T1,4-(WP_EMPHY_TRANS_UNFRAMED | WP_EMPHY_TRANS_E1),5-(WP_EMPHY_TRANS_FRAMED | WP_EMPHY_TRANS_E1),7-(WP_EMPHY_TRANS_FRAMED_CAS | WP_EMPHY_TRANS_E1) \n\r\t mod_com - 2 = WP_DEV_MOD_EMPHY_TRANS_STRUCTURE_SIZE value - decimal\n\r\t mod_com - 3 = WP_DEV_MOD_EMPHY_TRANS_RX_DATAUNIT_SIZE value = (0-WP_DISABLE,1-WP_ENABLE) \n\r\t mod_com - 4 = WP_DEV_MOD_EMPHY_TRANS_TX_DATAUNIT_SIZE value = (0-WP_DISABLE,1-WP_ENABLE)] ",                                                                   {(Y_MnuLeafP) CLI_F_EmphyTransDeviceModify}},
   {K_Leaf, TRUE, " -> Pwe3 transparent Rx channel modify [ \n\r\t(pw_index mod_com value) \n\r\t mod_com - 1 = WP_CH_MOD_TRANS_STATMODE value = (0-WP_DISABLE,1-WP_ENABLE)\n\r\t mod_com - 2 = WP_CH_MOD_TRANS_RX_BUFFERSIZE value - decimal ]",       {(Y_MnuLeafP) CLI_F_Pwe3TransRxChannelModify}},
   {K_Leaf, TRUE, " -> Pwe3 transparent Tx channel modify [ \n\r\t(pw_index mod_com value) \n\r\t mod_com - 1 = WP_CH_MOD_TRANS_STATMODE value = (0-WP_DISABLE,1-WP_ENABLE)\n\r\t mod_com - 2 = WP_CH_MOD_TRANS_TX_UDCAS value - decimal\n\r\t mod_com - 3 = WP_CH_MOD_TRANS_TX_UDP_DUMMY_MODE value = (0-WP_TRANS_TX_LAST_FRAME,1-WP_TRANS_TX_USER_DEFINED_FRAME)\n\r\t mod_com - 4 = WP_CH_MOD_TRANS_TX_UDPATTERN value = pattern bytes string seperated by \",\"\n\r\t mod_com - 5 =  WP_CH_MOD_TRANS_TX_DUMMY_UDPATTERN value = pattern bytes string seperated by \",\" ]",        {(Y_MnuLeafP) CLI_F_Pwe3TransTxChannelModify}},
   {K_Leaf, TRUE, " -> Tx jitter buffer parameters modify [ pw_index, jitter_buffer_size, tx_threshold, switchover_threshold, lops_detection, consec_pkts_in_synch_threshold, consec_pkts_out_synch_threshold ]", {(Y_MnuLeafP) CLI_F_JitterBufferParamsModify}},
   {K_Leaf, TRUE, " -> TDM to PSN Cesop flow aggregation modify [ \n\r\t(pw_index mod_com val val1) \n\r\t mod_com - 1 = WP_IW_FLOW_AGG_MOD_TAG val = tag(decimal) val1 = unused\n\r\t mod_com - 2 = WP_IW_FLOW_AGG_MOD_USER_MON_CONF val = statmode (0 - WP_DISABLE, 1 - WP_ENABLE) val1 = unused\n\r\t mod_com - 3 = WP_IW_FLOW_AGG_MOD_USER_MON_CONF val = timestamp_mode (0 - WP_DISABLE, 1 - WP_ENABLE) val1 = unused\n\r\t mod_com - 4 = WP_IW_FLOW_AGG_MOD_USER_MON_CONF val = intmode(0 - WP_DISABLE, 1 - WP_ENABLE) val1 = unused\n\r\t mod_com - 5 = WP_IW_FLOW_AGG_MOD_USER_MON_CONF val = interruptqueue(0-WP_IRQT0,1-WP_IRQT1,2-WP_IRQT2,3-WP_IRQT3) val1 = unused\n\r\t mod_com - 6 = WP_IW_FLOW_AGG_MOD_TXFUNC val = txfunc(WP_U32 handle) val1 = unused\n\r\t mod_com - 7 = WP_IW_FLOW_AGG_MOD_OV val = ov_pool_mode(0 - WP_DISABLE, 1 - WP_ENABLE) val1 = unused\n\r\t mod_com - 8 = WP_IW_FLOW_AGG_MOD_DROP_THRS val = fbp_drop_threshold val1 = unused\n\r\t mod_com - 9 = WP_IW_FLOW_AGG_MOD_MTU val = mtu val1 = unused\n\r\t mod_com - 10 = WP_IW_FLOW_AGG_CESOP_L3_PROTOCOL val = l3_protocol(0-WP_IW_CESOP_L3_IPV4,1-WP_IW_CESOP_L3_MPLS) val1 = unused\n\r\t mod_com - 11 = WP_IW_FLOW_AGG_CESOP_RTP_HDR val = rtp_header_mode(0 - WP_DISABLE, 1 - WP_ENABLE) val1 = rtp_header_offset(0-64)\n\r\t mod_com - 12 = WP_IW_FLOW_AGG_CESOP_CW_OFFSET val = control_word_offset(0-64) val1 = unused\n\r\t mod_com - 13 = WP_IW_FLOW_AGG_CESOP_PAYLOAD_SIZE val = payload_size val1 = unused\n\r\t mod_com - 14 = WP_IW_FLOW_AGG_CESOP_L2_LEN_UPDATE val = l2_length_update_mode(0 - WP_DISABLE, 1 - WP_ENABLE) val1 = l2_length_offset(0-64)\n\r\t mod_com - 15 = WP_IW_FLOW_AGG_CESOP_L3_HDR_OFFSET val = l3_header_offset(0-64) val1 = unused\n\r\t mod_com - 16 = WP_IW_FLOW_AGG_CESOP_L4_LEN_UPDATE val = l4_length_update_mode(0 - WP_DISABLE, 1 - WP_ENABLE) val1 = l4_length_offset(0-64)\n\r\t mod_com - 17 = WP_IW_FLOW_AGG_CESOP_L4_CHECKSUM_RECALC val = l4_checksum_recalc_mode(0 - WP_DISABLE, 1 - WP_ENABLE) val1 = unused\n\r\t mod_com - 18 = WP_IW_FLOW_AGG_CESOP_LAST_PACKET_CW_LEN val = last_packet_cw_length val1 = unused\n\r\t mod_com - 19 = WP_IW_FLOW_AGG_CESOP_LAST_PACKET_IP_CHECKSUM val = last_packet_ip_checksum val1 = unused\n\r\t mod_com - 20 = WP_IW_FLOW_AGG_CESOP_CW_MODE val = control_word_mode(0 - WP_DISABLE, 1 - WP_ENABLE) val1 = unused\n\r\t mod_com - 22 = WP_IW_FLOW_AGG_CESOP_TS_PARAMS val = ts_diff_if_id val1 = unused\n\r\t mod_com - 23 = WP_IW_FLOW_AGG_CESOP_PAYLOAD_SUP val = payload_suppression(0 - WP_DISABLE, 1 - WP_ENABLE) val1 = unused\n\r\t mod_com - 24 = WP_IW_FLOW_AGG_CESOP_PRIORITY_REMARK val = n_prefix_remark_fields(0-3) val1 = remark_val0]",   {(Y_MnuLeafP) CLI_F_Tdm2PsnCesopFlowAggModify}},
   {K_Leaf, TRUE, " -> TDM to PSN Cesop flow aggregation prefix header modify [ pw_index, prefix_header(bytes string seperated by \",\", must be 64 in length)]", {(Y_MnuLeafP) CLI_F_Tdm2PsnCesopFlowAggPrefixHeaderModify}},
   {K_Leaf, TRUE, " -> PSN to TDM Cesop flow aggregation modify [ pw_index mod_com val val1 : \n\r\t mod_com - 1 = WP_IW_FLOW_AGG_MOD_TAG val = tag(decimal) val1 = unused\n\r\t mod_com - 2 = WP_IW_FLOW_AGG_MOD_USER_MON_CONF val = statmode (0 - WP_DISABLE, 1 - WP_ENABLE) val1 = unused\n\r\t mod_com - 3 = WP_IW_FLOW_AGG_MOD_USER_MON_CONF val = timestamp_mode (0 - WP_DISABLE, 1 - WP_ENABLE) val1 = unused\n\r\t mod_com - 4 = WP_IW_FLOW_AGG_MOD_USER_MON_CONF val = intmode(0 - WP_DISABLE, 1 - WP_ENABLE) val1 = unused\n\r\t mod_com - 5 = WP_IW_FLOW_AGG_MOD_USER_MON_CONF val = interruptqueue(0-WP_IRQT0,1-WP_IRQT1,2-WP_IRQT2,3-WP_IRQT3) val1 = unused\n\r\t mod_com - 6 = WP_IW_FLOW_AGG_MOD_TXFUNC val = txfunc(WP_U32 handle) val1 = unused\n\r\t mod_com - 7 = WP_IW_FLOW_AGG_MOD_OV val = ov_pool_mode(0 - WP_DISABLE, 1 - WP_ENABLE) val1 = unused\n\r\t mod_com - 8 = WP_IW_FLOW_AGG_MOD_DROP_THRS val = fbp_drop_threshold val1 = unused\n\r\t mod_com - 9 = WP_IW_FLOW_AGG_MOD_MTU val = mtu val1 = unused\n\r\t mod_com - 10 = WP_IW_FLOW_AGG_CESOP_EXTRACTION_LEN val = extraction_length(0-64) val1 = unused\n\r\t mod_com - 11 = WP_IW_FLOW_AGG_CESOP_PAYLOAD_TYPE val = payload_type_check(0 - WP_DISABLE, 1 - WP_ENABLE) val1 = payload_type\n\r\t mod_com - 12 = WP_IW_FLOW_AGG_CESOP_SSRC val = ssrc_check(0 - WP_DISABLE, 1 - WP_ENABLE) val1 = ssrc\n\r\t mod_com - 13 = WP_IW_FLOW_AGG_CESOP_PAYLOAD_SIZE\n\r\t mod_com - 14 = WP_IW_FLOW_AGG_CESOP_CLOCK_REC_RESET val = 0, val1 = 0\n\r\t mod com - 15 = WP_IW_FLOW_AGG_CESOP_CLOCK_REC_OPERATION_MODE, val (1-ACR, Type A, 3-ACR Type B), val1 = 0 ]", {(Y_MnuLeafP) CLI_F_Psn2TdmCesopFlowAggModify}},
   {K_Leaf, TRUE, " -> Remove and recreate all PWs [num of loops]", {(Y_MnuLeafP) CLI_F_SBI_T1_F_AddRemovePW}},
   {K_Leaf, TRUE, " -> TDM to PSN Cesop flow aggregation prefix control word modify[pw_index][new control word (one byte)]", {(Y_MnuLeafP) CLI_F_Tdm2PsnCesopFlowAggControlWordModify}},
   {K_Leaf, TRUE, " -> Payload size modify[pw_index][new payload size]", {(Y_MnuLeafP) CLI_F_CesopPayloadSizeModify}},
};

const Y_MenuEntry V_PwDisplay []=
{
#if WTI_CESOP_MPLS_IW
   { 6 ,TRUE, "PW display",                                                                            {(Y_MenuEntry*) V_MainMenu}},
#else
   { 7 ,TRUE, "PW display",                                                                            {(Y_MenuEntry*) V_MainMenu}},
#endif
#if WTI_CESOP_TDI
   {K_Leaf, TRUE, " -> PW Display: TDI transparent device [device_index]",                             {(Y_MnuLeafP) CLI_F_TransDevShowInfo}},
#else
   {K_Leaf, TRUE, " -> PW Display: EMPHY transparent device [line_index][pw_index]",                   {(Y_MnuLeafP) CLI_F_TransDevShowInfo}},
#endif
   {K_Leaf, TRUE, " -> PW Display: PWE3 Transparent channel [type(0-Rx,1-Tx),pw_index]",               {(Y_MnuLeafP) CLI_F_Pwe3ChannelShowInfo}},
   {K_Leaf, TRUE, " -> PW Display: TDM to PSN flow aggregation [pw_index]",                            {(Y_MnuLeafP) CLI_F_Tdm2PsnFlowAggShowInfo}},
   {K_Leaf, TRUE, " -> PW Display: PSN to TDM flow aggregation [pw_index]",                            {(Y_MnuLeafP) CLI_F_Psn2TdmFlowAggShowInfo}},
   {K_Leaf, TRUE, " -> PW Display: TDM to PSN Rx binding [pw_index]",                                  {(Y_MnuLeafP) CLI_F_Tdm2PsnRxBinShowInfo}},
   {K_Leaf, TRUE, " -> PW Display: PSN to TDM Tx binding [pw_index]",                                  {(Y_MnuLeafP) CLI_F_Psn2TdmTxBinShowInfo}},
#if WTI_PCE_CLASSIFIER
   {K_Leaf, TRUE, " -> PW Display: PSN to TDM PCE classification rule [pw_index]",                     {(Y_MnuLeafP) CLI_F_Psn2TdmPceShowInfo}},
#else
#if (!WTI_CESOP_MPLS_IW)
   {K_Leaf, TRUE, " -> PW Display: PSN to TDM DFC classification rule [pw_index]",                     {(Y_MnuLeafP) CLI_F_Psn2TdmDfcShowInfo}},
#endif
#endif
};

const Y_MenuEntry V_SystemDisplay []=
{
#ifdef WP_HW_WINPATH3
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   { 6 ,TRUE, "System display",                                                       {(Y_MenuEntry*) V_MainMenu}},
#else
   { 5 ,TRUE, "System display",                                                       {(Y_MenuEntry*) V_MainMenu}},
#endif
#else
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   { 5 ,TRUE, "System display",                                                       {(Y_MenuEntry*) V_MainMenu}},
#else
   { 4 ,TRUE, "System display",                                                       {(Y_MenuEntry*) V_MainMenu}},
#endif
#endif
   {K_Leaf, TRUE, " -> System Display: Slots assignment table [line_index (0-84)]",   {(Y_MnuLeafP) CLI_F_SlotsConfigShowInfo}},
   {K_Leaf, TRUE, " -> System Display: TDM to PSN IW system",                         {(Y_MnuLeafP) CLI_F_Tdm2PsnIwShowInfo}},
   {K_Leaf, TRUE, " -> System Display: PSN to TDM IW system",                         {(Y_MnuLeafP) CLI_F_Psn2TdmIwShowInfo}},
   {K_Leaf, TRUE, " -> System Display: PSN to TDM Rx binding",                        {(Y_MnuLeafP) CLI_F_Psn2TdmRxBinShowInfo}},
#ifdef WP_HW_WINPATH3
   {K_Leaf, TRUE, " -> System Display: CFU display",                                  {(Y_MnuLeafP) CLI_F_SaDisplay}},
#endif
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   {K_Leaf, TRUE, " -> System Display: Clock Recovery (0-simple, 1-full)",            {(Y_MnuLeafP) CLI_F_ClockRecDisplay}},
#endif
};

#if WTI_CESOP_TDI
const Y_MenuEntry V_PreDefinedSystemConfigurations []=
{
#if WTI_CESOP_TO_MPLS_FA_CASCADING
   { 43 ,TRUE, "System configurations",                                      {(Y_MenuEntry*) V_MainMenu}},
#else /* !WTI_CESOP_TO_MPLS_FA_CASCADING */
   { 40 ,TRUE, "System configurations",                                      {(Y_MenuEntry*) V_MainMenu}},
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */
   {K_Leaf, TRUE, " -> Different lines octal",                               {(Y_MnuLeafP) CLI_F_DifferentLinesOctal}},
   {K_Leaf, TRUE, " -> Tx clock mode [1-external clock 2-rx looptime clock", {(Y_MnuLeafP) CLI_F_ChangeTxClockMode}},
   {K_Leaf, TRUE, " -> CR: 301_TDI_E1_U_CR, No CAS, Two boards N-CD N-lines N-PWs (N=1->16 a-synchronous mode)",  {(Y_MnuLeafP) CLI_F_301_TDI_E1_U_CR_NoCas_Two_Boards_N_Line_N_PW}},
   {K_Leaf, TRUE, " -> CR: 301_TDI_T1_U_CR, No CAS, Two boards N-CD N-lines N-PWs (N=1->16 a-synchronous mode)",  {(Y_MnuLeafP) CLI_F_301_TDI_T1_U_CR_NoCas_Two_Boards_N_Line_N_PW}},
   {K_Leaf, TRUE, " -> CR: 301_TDI_E1_F_CR, No CAS, Two boards N-CD N-lines N-PWs (N=1->16 a-synchronous mode)",  {(Y_MnuLeafP) CLI_F_301_TDI_E1_F_CR_NoCas_Two_Boards_N_Line_N_PW}},
   {K_Leaf, TRUE, " -> CR: 301_TDI_T1_F_CR, No CAS, Two boards N-CD N-lines N-PWs (N=1->16 a-synchronous mode)",  {(Y_MnuLeafP) CLI_F_301_TDI_T1_F_CR_NoCas_Two_Boards_N_Line_N_PW}},
   {K_Leaf, TRUE, " -> CR: 302_TDI_E1_U_CR, No CAS, Two boards 1-CD 1-lines 1-PWs (specified in the test)",  {(Y_MnuLeafP) CLI_F_302_TDI_E1_U_CR_NoCas_Two_Boards_1_Line_1_PW_for_Specific_TDM_Port}},
   {K_Leaf, TRUE, " -> CR: 303_TDI_E1_F_CR, No CAS, Two boards N-CD N-lines N-PWs 1-DS0 (N=1->16 a-synchronous mode)",  {(Y_MnuLeafP) CLI_F_303_TDI_E1_F_CR_NoCas_Two_Boards_N_Line_N_PW_1DS0}},
   {K_Leaf, TRUE, " -> CR: 303_TDI_T1_F_CR, No CAS, Two boards N-CD N-lines N-PWs 1-DS0 (N=1->16 a-synchronous mode)",  {(Y_MnuLeafP) CLI_F_303_TDI_T1_F_CR_NoCas_Two_Boards_N_Line_N_PW_1DS0}},
   {K_Leaf, TRUE, " -> CR: 304_TDI_E1_U_CR, No CAS, Two boards N-CD N-lines N-PWs, Remove CR device, recreate without CR",  {(Y_MnuLeafP) CLI_F_304_TDI_E1_U_CR_NoCas_Two_Boards_N_Line_N_PW_Remove_Device_Recreate_Without_CR}},
   {K_Leaf, TRUE, " -> CR: 304_TDI_T1_U_CR, No CAS, Two boards N-CD N-lines N-PWs, Remove CR device, recreate without CR",  {(Y_MnuLeafP) CLI_F_304_TDI_T1_U_CR_NoCas_Two_Boards_N_Line_N_PW_Remove_Device_Recreate_Without_CR}},
   {K_Leaf, TRUE, " -> CR: 304_TDI_E1_F_CR, No CAS, Two boards N-CD N-lines N-PWs, Remove CR device, recreate without CR",  {(Y_MnuLeafP) CLI_F_304_TDI_E1_F_CR_NoCas_Two_Boards_N_Line_N_PW_Remove_Device_Recreate_Without_CR}},
   {K_Leaf, TRUE, " -> CR: 304_TDI_T1_F_CR, No CAS, Two boards N-CD N-lines N-PWs, Remove CR device, recreate without CR",  {(Y_MnuLeafP) CLI_F_304_TDI_T1_F_CR_NoCas_Two_Boards_N_Line_N_PW_Remove_Device_Recreate_Without_CR}},
   {K_Leaf, TRUE, " -> CR: 305_TDI_E1_F_30xDS0_CR, CAS, Frg, Two boards N-CD N-lines N-PWs",  {(Y_MnuLeafP) CLI_F_305_TDI_E1_F_30xDS0_CR_Cas_Frg_Two_Boards_N_Line_N_PW}},
   {K_Leaf, TRUE, " -> CR: 305_TDI_T1_F_24xDS0_CR, CAS, Frg, Two boards N-CD N-lines N-PWs",  {(Y_MnuLeafP) CLI_F_305_TDI_T1_F_24xDS0_CR_Cas_Frg_Two_Boards_N_Line_N_PW}},
   {K_Leaf, TRUE, " -> CR: 306_TDI_E1_F_16xDS0_CR, No CAS, Two boards N-CD N-lines 2-PWs Per Line",  {(Y_MnuLeafP) CLI_F_306_TDI_E1_F_CR_NoCas_Two_Boards_N_Line_2_pw_per_line}},
   {K_Leaf, TRUE, " -> CR: 307_TDI_T1_F_2xDS0_CR, No CAS, Two boards N-CD N-lines 11+1-PWs Per Line",  {(Y_MnuLeafP) CLI_F_307_TDI_T1_F_CR_NoCas_Two_Boards_N_Line_11_plus_1_pw_per_line}},
#if WTI_CESOP_TO_MPLS_FA_CASCADING
   {K_Leaf, TRUE, " -> CR: 308_TDI_E1_U_MPLS_CASCADING, No CAS, one board",  {(Y_MnuLeafP) CLI_F_308_TDI_E1_U_CR_NoCas_Mpls_Cascading}},
   {K_Leaf, TRUE, " -> CR: 308_TDI_T1_U_MPLS_CASCADING, No CAS, one board",  {(Y_MnuLeafP) CLI_F_308_TDI_T1_U_CR_NoCas_Mpls_Cascading}},
#endif
   {K_Leaf, TRUE, " -> E1, Unframed, No CAS, Octal",                         {(Y_MnuLeafP) CLI_F_E1UnframedNoCasOctal}},
   {K_Leaf, TRUE, " -> E1, Framed, 1xDS0, No CAS, Octal",                    {(Y_MnuLeafP) CLI_F_E1Framed1DS0NoCasOctal}},
   {K_Leaf, TRUE, " -> E1, Framed, 1xDS0, No CAS, Octal short packets with padding ",                    {(Y_MnuLeafP) CLI_F_E1Framed1DS0NoCasShortPacketsOctal}},
   {K_Leaf, TRUE, " -> E1, Framed, 32xDS0, No CAS, Octal",                   {(Y_MnuLeafP) CLI_F_E1Framed32DS0NoCasOctal}},
   {K_Leaf, TRUE, " -> E1, Framed, 1xDS0, CAS, UNFRG, Octal",                {(Y_MnuLeafP) CLI_F_E1Framed1DS0CasUnFrgOctal}},
   {K_Leaf, TRUE, " -> E1, Framed, 30xDS0, CAS, UNFRG, Octal",               {(Y_MnuLeafP) CLI_F_E1Framed30DS0CasUnFrgOctal}},
   {K_Leaf, TRUE, " -> E1, Framed, 30xDS0, CAS, UNFRG, Octal with PW modify",{(Y_MnuLeafP) CLI_F_E1Framed30DS0CasUnFrgOctal_withModify}},
   {K_Leaf, TRUE, " -> E1, Framed, 30xDS0, CAS, FRG, Octal",                 {(Y_MnuLeafP) CLI_F_E1Framed30DS0CasFrgOctal}},
   {K_Leaf, TRUE, " -> E1, Framed, 1xDS0, CAS, FRG, Octal",                  {(Y_MnuLeafP) CLI_F_E1Framed1DS0CasFrgOctal}},
   {K_Leaf, TRUE, " -> T1, Unframed, No CAS, Octal",                         {(Y_MnuLeafP) CLI_F_T1UnframedNoCasOctal}},
   {K_Leaf, TRUE, " -> T1, Framed, 1xDS0, No CAS, Octal",                    {(Y_MnuLeafP) CLI_F_T1Framed1DS0NoCasOctal}},
   {K_Leaf, TRUE, " -> T1, Framed, 24xDS0, No CAS, Octal",                   {(Y_MnuLeafP) CLI_F_T1Framed24DS0NoCasOctal}},
   {K_Leaf, TRUE, " -> T1, Framed, 1xDS0, CAS, UNFRG, Octal",                {(Y_MnuLeafP) CLI_F_T1Framed1DS0CasUnFrgOctal}},
   {K_Leaf, TRUE, " -> T1, Framed, 24xDS0, CAS, UNFRG, Octal",               {(Y_MnuLeafP) CLI_F_T1Framed24DS0CasUnFrgOctal}},
   {K_Leaf, TRUE, " -> T1, Framed, 1xDS0, CAS, FRG, Octal",                  {(Y_MnuLeafP) CLI_F_T1Framed1DS0CasFrgOctal}},
   {K_Leaf, TRUE, " -> T1, Framed, 24xDS0, CAS, FRG, Octal",                 {(Y_MnuLeafP) CLI_F_T1Framed24DS0CasFrgOctal}},
   {K_Leaf, TRUE, " -> T1, Framed, 1xDS0, UNFRG, Octal OOBC",                {(Y_MnuLeafP) CLI_F_T1Framed1DS0CasUnFrgOctalOobc}},
   {K_Leaf, TRUE, " -> E1, Framed, 1xDS0, UNFRG, Octal OOBC",                {(Y_MnuLeafP) CLI_F_E1Framed1DS0CasUnFrgOctalOobc}},
   {K_Leaf, TRUE, " -> T1, Framed, 1xDS0, UNFRG, Octal OOBC, 2 Boards",      {(Y_MnuLeafP) CLI_F_T1Framed1DS0CasUnFrgOctalOobc_2Boards}},
   {K_Leaf, TRUE, " -> E1, Framed, 1xDS0, UNFRG, Octal OOBC, 2 Boards",      {(Y_MnuLeafP) CLI_F_E1Framed1DS0CasUnFrgOctalOobc_2Boards}},
   {K_Leaf, TRUE, " -> E1, Framed, FRG, Octal OOBC",                         {(Y_MnuLeafP) CLI_F_E1Framed1DS0CasFrgOctalOobc}},
   {K_Leaf, TRUE, " -> E1, Framed, 256xDS0, CAS, FRG, Octal",                {(Y_MnuLeafP) CLI_F_E1Framed256DS0CasFrgOctal}},
#if WTI_CESOP_TO_MPLS_FA_CASCADING
   {K_Leaf, TRUE, " -> E1, Framed, 256xDS0, CAS, FRG, Octal - CASCADED",     {(Y_MnuLeafP) CLI_F_E1Framed256DS0CasFrgOctalMplsCascading}},
#endif/* WTI_CESOP_TO_MPLS_FA_CASCADING */
   {K_Leaf, TRUE, " -> E1, Framed, UNFRG, Octal (8 TDI,8 RTDI)",             {(Y_MnuLeafP) CLI_F_E1FramedUnFrg16Tdi}},
};

#else
const Y_MenuEntry V_PreDefinedSystemConfigurations []=
{
#ifdef __WT_UFE3__
   {
#if WTI_CESOP_MEF8_ENCAP
      WTI_CESOP_MEF8_ENCAP +
#endif
#if WTI_CESOP_TO_MPLS_FA_CASCADING
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE && WTI_CESOP_RX_TIMING_ENABLE
#if WTI_EXT_DIFF_MODE_ACTIVE
      112,
#else
      94,
#endif
#else
      71,
#endif
#else /* !WTI_CESOP_TO_MPLS_FA_CASCADING */
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE && WTI_CESOP_RX_TIMING_ENABLE
#if WTI_EXT_DIFF_MODE_ACTIVE
#if WTI_CESOP_TO_MPLS_FA_CASCADING
      110,
#else
      109,
#endif
#else
#if WTI_CESOP_TO_MPLS_FA_CASCADING
      95,
#else
      94,
#endif
#endif
#else
      70,
#endif
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */
      TRUE, "System configurations",                                           {(Y_MenuEntry*) V_MainMenu}},
   {K_Leaf, TRUE, " -> Tx clock mode [1-external clock 2-rx looptime clock",   {(Y_MnuLeafP) CLI_F_ChangeTxClockMode}},
   {K_Leaf, TRUE, " -> Different lines octal",                                 {(Y_MnuLeafP) CLI_F_DifferentLinesOctal}},
   {K_Leaf, TRUE, " -> E1, Unframed, No CAS, Octal",                           {(Y_MnuLeafP) CLI_F_E1UnframedNoCasOctal}},
   {K_Leaf, TRUE, " -> E1, Unframed, No CAS, Octal 8xDS1 board1",              {(Y_MnuLeafP) CLI_F_E1UnframedNoCasOctalBoard1}},
   {K_Leaf, TRUE, " -> E1, Framed, 1xDS0, No CAS, Octal",                      {(Y_MnuLeafP) CLI_F_E1Framed1DS0NoCasOctal}},
   {K_Leaf, TRUE, " -> E1, Framed, 32xDS0, No CAS, Octal",                     {(Y_MnuLeafP) CLI_F_E1Framed32DS0NoCasOctal}},
   {K_Leaf, TRUE, " -> E1, Framed, 1xDS0, CAS, UNFRG, Octal",                  {(Y_MnuLeafP) CLI_F_E1Framed1DS0CasUnFrgOctal}},
   {K_Leaf, TRUE, " -> E1, Framed, 30xDS0, CAS, UNFRG, Octal",                 {(Y_MnuLeafP) CLI_F_E1Framed30DS0CasUnFrgOctal}},
   {K_Leaf, TRUE, " -> E1, Framed, 30xDS0, CAS, FRG, Octal",                   {(Y_MnuLeafP) CLI_F_E1Framed30DS0CasFrgOctal}},
   {K_Leaf, TRUE, " -> E1, Framed, 1xDS0, CAS, FRG, Octal",                    {(Y_MnuLeafP) CLI_F_E1Framed1DS0CasFrgOctal}},
   {K_Leaf, TRUE, " -> T1, Unframed, No CAS, Octal",                           {(Y_MnuLeafP) CLI_F_T1UnframedNoCasOctal}},
   {K_Leaf, TRUE, " -> T1, Unframed, No CAS, Octal 8xDS1 board1",              {(Y_MnuLeafP) CLI_F_T1UnframedNoCasOctalBoard1}},
   {K_Leaf, TRUE, " -> T1, Framed, 1xDS0, No CAS, Octal",                      {(Y_MnuLeafP) CLI_F_T1Framed1DS0NoCasOctal}},
   {K_Leaf, TRUE, " -> T1, Framed, 24xDS0, No CAS, Octal",                     {(Y_MnuLeafP) CLI_F_T1Framed24DS0NoCasOctal}},
   {K_Leaf, TRUE, " -> T1, Framed, 1xDS0, CAS, UNFRG, Octal",                  {(Y_MnuLeafP) CLI_F_T1Framed1DS0CasUnFrgOctal}},
   {K_Leaf, TRUE, " -> T1, Framed, 24xDS0, CAS, UNFRG, Octal",                 {(Y_MnuLeafP) CLI_F_T1Framed24DS0CasUnFrgOctal}},
   {K_Leaf, TRUE, " -> T1, Framed, 1xDS0, CAS, FRG, Octal",                    {(Y_MnuLeafP) CLI_F_T1Framed1DS0CasFrgOctal}},
   {K_Leaf, TRUE, " -> T1, Framed, 24xDS0, CAS, FRG, Octal",                   {(Y_MnuLeafP) CLI_F_T1Framed24DS0CasFrgOctal}},
   {K_Leaf, TRUE, " -> Sbi - different line types in SPE (E1,E1,E1)",          {(Y_MnuLeafP) CLI_F_SbiDifferentLinesInSpeE1E1E1}},
   {K_Leaf, TRUE, " -> Sbi - different line types in SPE (T1,T1,T1)",          {(Y_MnuLeafP) CLI_F_SbiDifferentLinesInSpeT1T1T1}},
   {K_Leaf, TRUE, " -> E1, Unframed, No CAS, Sbi",                             {(Y_MnuLeafP) CLI_F_E1UnframedNoCasSbi}},
   {K_Leaf, TRUE, " -> E1, Framed, 1xDS0, No CAS, Sbi",                        {(Y_MnuLeafP) CLI_F_E1Framed1DS0NoCasSbi}},
   {K_Leaf, TRUE, " -> E1, Framed, 32xDS0, No CAS, Sbi",                       {(Y_MnuLeafP) CLI_F_E1Framed32DS0NoCasSbi}},
   {K_Leaf, TRUE, " -> E1, Framed, 2xDS0 per PW, 256 PW, No CAS, Sbi",         {(Y_MnuLeafP) CLI_F_E1Framed2DS0256PwNoCasSbi}},
   {K_Leaf, TRUE, " -> E1, Framed, 1xDS0, CAS, UNFRG, Sbi",                    {(Y_MnuLeafP) CLI_F_E1Framed1DS0CasUnFrgSbi}},
   {K_Leaf, TRUE, " -> E1, Framed, 30xDS0, CAS, UNFRG, Sbi",                   {(Y_MnuLeafP) CLI_F_E1Framed30DS0CasUnFrgSbi}},
   {K_Leaf, TRUE, " -> E1, Framed, 1xDS0, CAS, FRG, Sbi",                      {(Y_MnuLeafP) CLI_F_E1Framed1DS0CasFrgSbi}},
   {K_Leaf, TRUE, " -> E1, Framed, 30xDS0, CAS, FRG, Sbi",                     {(Y_MnuLeafP) CLI_F_E1Framed30DS0CasFrgSbi}},
   {K_Leaf, TRUE, " -> T1, Unframed, No CAS, Sbi",                             {(Y_MnuLeafP) CLI_F_T1UnframedNoCasSbi}},
   {K_Leaf, TRUE, " -> T1, Framed, 1xDS0, No CAS, Sbi",                        {(Y_MnuLeafP) CLI_F_T1Framed1DS0NoCasSbi}},
   {K_Leaf, TRUE, " -> T1, Framed, 24xDS0, No CAS, Sbi",                       {(Y_MnuLeafP) CLI_F_T1Framed24DS0NoCasSbi}},
   {K_Leaf, TRUE, " -> T1, Framed, 1xDS0, CAS, UNFRG, Sbi",                    {(Y_MnuLeafP) CLI_F_T1Framed1DS0CasUnFrgSbi}},
   {K_Leaf, TRUE, " -> T1, Framed, 24xDS0, CAS, UNFRG, Sbi",                   {(Y_MnuLeafP) CLI_F_T1Framed24DS0CasUnFrgSbi}},
   {K_Leaf, TRUE, " -> T1, Framed, 1xDS0, CAS, FRG, Sbi",                      {(Y_MnuLeafP) CLI_F_T1Framed1DS0CasFrgSbi}},
   {K_Leaf, TRUE, " -> T1, Framed, 24xDS0, CAS, FRG, Sbi",                     {(Y_MnuLeafP) CLI_F_T1Framed24DS0CasFrgSbi}},
   {K_Leaf, TRUE, " -> T1, Framed, No CAS, Sbi, stress 2016 PW",               {(Y_MnuLeafP) CLI_F_T1FramedNoCasSbiStress2016Pw}},
   {K_Leaf, TRUE, " -> E1, Unframed, No CAS, Sbi, Add Delete scenario",        {(Y_MnuLeafP) CLI_F_E1UnframedNoCasSbiAddDelete}},
   {K_Leaf, TRUE, " -> T1, SBI - Framed, 1xDS0, only SPE1, OOBC, 2 Boards",    {(Y_MnuLeafP) CLI_F_T1framed1DS0CasSbiOobc_2Boards}},
   {K_Leaf, TRUE, " -> E1, SBI - Framed, 1xDS0, only SPE1, OOBC, 2 Boards",    {(Y_MnuLeafP) CLI_F_E1framed1DS0CasSbiOobc_2Boards}},
   {K_Leaf, TRUE, " -> CR: 201_CAD_E1_U_CR, No CAS, Two boards N-CD N-lines N-PWs (N=1->16 a-synchronous mode) ",  {(Y_MnuLeafP) CLI_F_201_CAD_E1_U_CR_NoCas_Two_Boards_N_Line_N_PW}},
   {K_Leaf, TRUE, " -> CR: 201_CAD_T1_U_CR, No CAS, Two boards N-CD N-lines N-PWs (N=1->16 a-synchronous mode) ",  {(Y_MnuLeafP) CLI_F_201_CAD_T1_U_CR_NoCas_Two_Boards_N_Line_N_PW}},
   {K_Leaf, TRUE, " -> CR: 201_CAD_E1_F_CR, No CAS, Two boards N-CD N-lines N-PWs (N=1->16 a-synchronous mode) ",  {(Y_MnuLeafP) CLI_F_201_CAD_E1_F_CR_NoCas_Two_Boards_N_Line_N_PW}},
   {K_Leaf, TRUE, " -> CR: 201_CAD_T1_F_CR, No CAS, Two boards N-CD N-lines N-PWs (N=1->16 a-synchronous mode) ",  {(Y_MnuLeafP) CLI_F_201_CAD_T1_F_CR_NoCas_Two_Boards_N_Line_N_PW}},
   {K_Leaf, TRUE, " -> CR: 202_CAD_E1_F_CR, No CAS, Two boards N-CD N-lines N-PWs 1-DS0 (N=1->16 a-synchronous mode) ",  {(Y_MnuLeafP) CLI_F_202_CAD_E1_F_CR_NoCas_Two_Boards_N_Line_N_PW_1DS0}},
   {K_Leaf, TRUE, " -> CR: 202_CAD_T1_F_CR, No CAS, Two boards N-CD N-lines N-PWs 1-DS0 (N=1->16 a-synchronous mode) ",  {(Y_MnuLeafP) CLI_F_202_CAD_T1_F_CR_NoCas_Two_Boards_N_Line_N_PW_1DS0}},
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE && WTI_CESOP_RX_TIMING_ENABLE
   {K_Leaf, TRUE, " -> CR: 203_CAD_E1_U_CR, No CAS, Two boards N-CD N-lines N-PWs add/remove PW",  {(Y_MnuLeafP) CLI_F_203_CAD_E1_U_CR_NoCas_Two_Boards_N_Line_N_PW_Addremove_PW}},
   {K_Leaf, TRUE, " -> CR: 204_CAD_T1_F_CR, No CAS, Two boards N-CD N-lines N-PWs one CR add/remove PW",                    {(Y_MnuLeafP) CLI_F_204_CAD_T1_F_CR_NoCas_Two_Boards_8_Line_8_PW_one_CR_Master_addremove_PW}},
#if WTI_CESOP_TO_MPLS_FA_CASCADING
   {K_Leaf, TRUE, " -> CR: 205_CAD_E1_U_CR, No CAS, MPLS Cascading, Two Boards", {(Y_MnuLeafP) CLI_F_205_CAD_E1_U_CR_NoCas_Mpls_Cascading}},
#endif
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE && WTI_CESOP_RX_TIMING_ENABLE */
   {K_Leaf, TRUE, " -> CR: 101_103_SBI_E1_U_CR N-CD N-CT+CR 1-63/252 lines 1-63/252 PWs(a-synchronous mode)", {(Y_MnuLeafP) CLI_F_101_103_SBI_E1_U_CR_NoCas_Two_Boards_N_CD_N_CT_63_Line_63_PW}},
   {K_Leaf, TRUE, " -> CR: 101_103_SBI_T1_U_CR N-CD N-CT+CR 1-84/336 lines 1-84/336 PWs(a-synchronous mode)", {(Y_MnuLeafP) CLI_F_101_103_SBI_T1_U_CR_NoCas_Two_Boards_N_CD_N_CT_84_Line_84_PW}},
   {K_Leaf, TRUE, " -> CR: 101_103_SBI_E1_F_CR N-CD N-CT+CR 1-63/252 lines 1-63/252 PWs(a-synchronous mode)", {(Y_MnuLeafP) CLI_F_101_103_SBI_E1_F_CR_NoCas_Two_Boards_N_CD_N_CT_63_Line_63_PW}},
   {K_Leaf, TRUE, " -> CR: 101_103_SBI_T1_F_CR N-CD N-CT+CR 1-84/336 lines 1-84/336 PWs(a-synchronous mode)", {(Y_MnuLeafP) CLI_F_101_103_SBI_T1_F_CR_NoCas_Two_Boards_N_CD_N_CT_84_Line_84_PW}},
   {K_Leaf, TRUE, " -> CR: 101_103_SBI_E1_F_1xDS0_CR (slot #31) N-CD N-CT+CR 1-63/252 lines 1-63/252 PWs(a-synchronous mode)", {(Y_MnuLeafP) CLI_F_101_103_SBI_E1_F_1xDS0_CR_NoCas_Two_Boards_N_CD_N_CT_252_Line_252_PW}},
   {K_Leaf, TRUE, " -> CR: 101_103_SBI_T1_F_1xDS0_CR (slot #1) N-CD N-CT+CR 1-84/336 lines 1-84/336 PWs(a-synchronous mode)", {(Y_MnuLeafP) CLI_F_101_103_SBI_T1_F_1xDS0_CR_NoCas_Two_Boards_N_CD_N_CT_336_Line_336_PW}},
   {K_Leaf, TRUE, " -> CR: 105_SBI_E1_U_CR 1-CD 1-CT+CR, 1-63/252 lines add/remove PW (synchronous mode) ", {(Y_MnuLeafP) CLI_F_105_SBI_E1_U_CR_NoCas_Two_Boards_1_CD_1_CT_63_line_addremove_PW}},
   {K_Leaf, TRUE, " -> CR: 105_SBI_T1_U_CR 1-CD 1-CT+CR, 1-84/336 lines add/remove PW (synchronous mode) ", {(Y_MnuLeafP) CLI_F_105_SBI_T1_U_CR_NoCas_Two_Boards_1_CD_1_CT_84_line_addremove_PW}},
   {K_Leaf, TRUE, " -> CR: 112_SBI_E1_F_30DS0_CAS CR N CR + N CT, 1-63/252 lines (a-sync mode)",{(Y_MnuLeafP) CLI_F_112_SBI_E1_F_30DS0_CR_Cas_Two_Boards_N_CD_N_CT_max_Lines_max_PW}},
   {K_Leaf, TRUE, " -> CR: 112_SBI_T1_F_24DS0_CAS CR N CR + N CT, 1-84/336 lines (a-sync mode)",{(Y_MnuLeafP) CLI_F_112_SBI_T1_F_24DS0_CR_Cas_Two_Boards_N_CD_N_CT_max_Lines_max_PW}},
#if WTI_CESOP_TO_MPLS_FA_CASCADING
   {K_Leaf, TRUE, " -> CR: 103_SBI_E1_U_CR No CAS, MPLS Cascading, Two Boards", {(Y_MnuLeafP) CLI_F_103_SBI_E1_U_CR_NoCas_Mpls_Cascading}},
#endif
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE && WTI_CESOP_RX_TIMING_ENABLE
   {K_Leaf, TRUE, " -> CR: 102_SBI_E1_U_CR 1-CD N-CT+CR 1-63/252 lines 1-63/252 PWs",                            {(Y_MnuLeafP) CLI_F_102_SBI_E1_U_CR_NoCas_Two_Boards_1_CD_N_CT_63_Line_63_PW}},
   {K_Leaf, TRUE, " -> CR: 102_SBI_T1_U_CR 1-CD N-CT+CR 1-84/336 lines 1-84/336 PWs",                            {(Y_MnuLeafP) CLI_F_102_SBI_T1_U_CR_NoCas_Two_Boards_1_CD_N_CT_84_Line_84_PW}},
   {K_Leaf, TRUE, " -> CR: 102_SBI_E1_F_CR 1-CD N-CT+CR 1-63/252 lines 1-63/252 PWs",                            {(Y_MnuLeafP) CLI_F_102_SBI_E1_F_CR_NoCas_Two_Boards_1_CD_N_CT_63_Line_63_PW}},
   {K_Leaf, TRUE, " -> CR: 102_SBI_T1_F_CR 1-CD N-CT+CR 1-84/336 lines 1-84/336 PWs",                            {(Y_MnuLeafP) CLI_F_102_SBI_T1_F_CR_NoCas_Two_Boards_1_CD_N_CT_84_Line_84_PW}},
   {K_Leaf, TRUE, " -> CR: 104_SBI_E1_F_CR 1-CD 1-CT+CR 63 lines 252 PWs",                    {(Y_MnuLeafP) CLI_F_104_SBI_E1_F_CR_NoCas_Two_Boards_1_CD_1_CT_63_Line_252_PW}},
   {K_Leaf, TRUE, " -> CR: 104_SBI_T1_F_CR 1-CD 1-CT+CR 84 lines 192 PWs",                    {(Y_MnuLeafP) CLI_F_104_SBI_T1_F_CR_NoCas_Two_Boards_1_CD_1_CT_84_Line_192_PW}},
   {K_Leaf, TRUE, " -> CR: 105_SBI_E1_F_CR 1-CD 1-CT+CR, 1-63/252 lines add/remove PW (synchronous mode)",                    {(Y_MnuLeafP) CLI_F_105_SBI_E1_F_CR_NoCas_Two_Boards_1_CD_1_CT_63_line_addremove_PW}},
   {K_Leaf, TRUE, " -> CR: 105_SBI_T1_F_CR 1-CD 1-CT+CR, 1-84/336 lines add/remove PW (synchronous mode)",                    {(Y_MnuLeafP)CLI_F_105_SBI_T1_F_CR_NoCas_Two_Boards_1_CD_1_CT_84_line_addremove_PW}},
   {K_Leaf, TRUE, " -> CR: 106_SBI_E1_U_CR N-CD N-CT+CR, 63 lines modify shaper",
    {(Y_MnuLeafP)CLI_F_106_SBI_E1_U_CR_NoCas_Two_Boards_1_CD_1_CT_63_line_with_shaper_modify}},
   {K_Leaf, TRUE, " -> CR: 106_SBI_T1_U_CR N-CD N-CT+CR, 84 lines modify shaper",
    {(Y_MnuLeafP)CLI_F_106_SBI_T1_U_CR_NoCas_Two_Boards_1_CD_1_CT_84_line_with_shaper_modify}},
   {K_Leaf, TRUE, " -> CR: 106_SBI_E1_F_CR N-CD N-CT+CR, 63 lines modify shaper",
    {(Y_MnuLeafP)CLI_F_106_SBI_E1_F_CR_NoCas_Two_Boards_1_CD_1_CT_63_line_with_shaper_modify}},
   {K_Leaf, TRUE, " -> CR: 106_SBI_T1_F_CR N-CD N-CT+CR, 84 lines modify shaper",
    {(Y_MnuLeafP)CLI_F_106_SBI_T1_F_CR_NoCas_Two_Boards_1_CD_1_CT_84_line_with_shaper_modify}},
   {K_Leaf, TRUE, " -> CR: 107_SBI_E1_F_CR 1-CD 1-CT+CR 84 lines 63 PWs different DU sizes",                    {(Y_MnuLeafP) CLI_F_107_SBI_E1_F_CR_NoCas_Two_Boards_1_CD_1_CT_Different_DU_size}},
   {K_Leaf, TRUE, " -> CR: 107_SBI_T1_F_CR 1-CD 1-CT+CR 84 lines 84 PWs different DU sizes",                    {(Y_MnuLeafP) CLI_F_107_SBI_T1_F_CR_NoCas_Two_Boards_1_CD_1_CT_Different_DU_size}},
   {K_Leaf, TRUE, " -> CR: 107_SBI_T1_F_CR 1-CD 1-CT+CR 84 lines 84 PW(2slots)s TS width overflow",              {(Y_MnuLeafP) CLI_F_107_SBI_T1_F_CR_NoCas_Two_Boards_1_CD_1_CT_TSWidth_Overflow}},
   {K_Leaf, TRUE, " -> CR: 108_SBI_E1_U_CR 2-CD 2-CT+CR, 84 lines add/remove/modify PW (synchronous mode)",                    {(Y_MnuLeafP)CLI_F_108_SBI_E1_U_CR_NoCas_Two_Boards_1_CD_1_CT_63_line_addremove_PW_with_shaper_modify}},
   {K_Leaf, TRUE, " -> CR: 108_SBI_T1_U_CR 2-CD 2-CT+CR, 84 lines add/remove/modify PW (synchronous mode)",                    {(Y_MnuLeafP)CLI_F_108_SBI_T1_U_CR_NoCas_Two_Boards_1_CD_1_CT_63_line_addremove_PW_with_shaper_modify}},
   {K_Leaf, TRUE, " -> CR: 108_SBI_E1_F_CR 2-CD 2-CT+CR, 84 lines add/remove/modify PW (synchronous mode)",                    {(Y_MnuLeafP)CLI_F_108_SBI_E1_F_CR_NoCas_Two_Boards_1_CD_1_CT_63_line_addremove_PW_with_shaper_modify}},
   {K_Leaf, TRUE, " -> CR: 108_SBI_T1_F_CR 2-CD 2-CT+CR, 84 lines add/remove/modify PW (synchronous mode)",                    {(Y_MnuLeafP)CLI_F_108_SBI_T1_F_CR_NoCas_Two_Boards_1_CD_1_CT_63_line_addremove_PW_with_shaper_modify}},
   {K_Leaf, TRUE, " -> CR: 110_SBI_E1_U_CR N CR+CT, 1-63/252 lines, channel and FA modify (a-sync mode)",{(Y_MnuLeafP) CLI_F_110_SBI_E1_U_CR_NoCas_Two_Boards_N_CD_N_CT_max_lines_FA_modify}},
   {K_Leaf, TRUE, " -> CR: 111_SBI_E1_F_CR 2 CR + 2 CT, 63 lines, multiple PW per line (a-sync mode)",{(Y_MnuLeafP) CLI_F_111_SBI_E1_F_CR_NoCas_Two_Boards_Two_CR_Master_Multiple_PW_per_line}},
   {K_Leaf, TRUE, " -> CR: 111_SBI_T1_F_CR 2 CR + 2 CT, 84 lines, multiple PW per line (a-sync mode)",{(Y_MnuLeafP) CLI_F_111_SBI_T1_F_CR_NoCas_Two_Boards_Two_CR_Master_Multiple_PW_per_line}},
   {K_Leaf, TRUE, " -> CR: 112_SBI_E1_F_30DS0_CAS CR N CR + N CT, 1-63/252 lines (a-sync mode)",{(Y_MnuLeafP) CLI_F_112_SBI_E1_F_30DS0_CR_Cas_Two_Boards_N_CD_N_CT_max_Lines_max_PW}},
   {K_Leaf, TRUE, " -> CR: 112_SBI_T1_F_24DS0_CAS CR N CR + N CT, 1-84/336 lines (a-sync mode)",{(Y_MnuLeafP) CLI_F_112_SBI_T1_F_24DS0_CR_Cas_Two_Boards_N_CD_N_CT_max_Lines_max_PW}},
#if WTI_EXT_DIFF_MODE_ACTIVE
   {K_Leaf, TRUE, " -> CR: 130_SBI_E1_U_Extended_Differential_CR 252-CD, 252-CT+CR 252 lines 252 PWs [<DU Size> <Packet Size>]", {(Y_MnuLeafP) CLI_F_130_SBI_E1_U_CR_EXTENDED_DIFF_NoCas_Two_Boards_252CD_252Lines_252Pw}},
   {K_Leaf, TRUE, " -> CR: 130_SBI_T1_U_Extended_Differential_CR 336-CD, 336-CT+CR 336 lines 336 PWs [<DU Size> <Packet Size>]", {(Y_MnuLeafP) CLI_F_130_SBI_T1_U_CR_EXTENDED_DIFF_NoCas_Two_Boards_336CD_336Lines_336Pw}},
   {K_Leaf, TRUE, " -> CR: 130_SBI_E1_F_Extended_Differential_CR 252-CD, 252-CT+CR 252 lines 252 PWs [<DU Size> <Packet Size>]", {(Y_MnuLeafP) CLI_F_130_SBI_E1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_252CD_252Lines_252Pw}},
   {K_Leaf, TRUE, " -> CR: 130_SBI_T1_F_Extended_Differential_CR 336-CD, 336-CT+CR 336 lines 336 PWs [<DU Size> <Packet Size>]", {(Y_MnuLeafP) CLI_F_130_SBI_T1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_336CD_336Lines_336Pw}},
   {K_Leaf, TRUE, " -> CR: 130_SBI_E1_F_1xDS0_Extended_Differential_CR 252-CD, 252-CT+CR 252 lines 252 PWs [<DU Size> <Packet Size>]", {(Y_MnuLeafP) CLI_F_130_SBI_E1_F_1DS0_CR_EXTENDED_DIFF_NoCas_Two_Boards_252CD_252Lines_252Pw}},
   {K_Leaf, TRUE, " -> CR: 130_SBI_T1_F_1xDS0_Extended_Differential_CR 336-CD, 336-CT+CR 336 lines 336 PWs [<DU Size> <Packet Size>]", {(Y_MnuLeafP) CLI_F_130_SBI_T1_F_1DS0_CR_EXTENDED_DIFF_NoCas_Two_Boards_336CD_336Lines_336Pw}},
   {K_Leaf, TRUE, " -> CR: 131_SBI_E1_F_Extended_Differential_CR 252-CD, 252-CT+CR 252 lines 1008 PWs [<DU Size> <Packet Size>]", {(Y_MnuLeafP) CLI_F_131_SBI_E1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_252CD_252Lines_1008Pw}},
   {K_Leaf, TRUE, " -> CR: 131_SBI_T1_F_Extended_Differential_CR 336-CD, 336-CT+CR 336 lines 1344 PWs [<DU Size> <Packet Size>]", {(Y_MnuLeafP) CLI_F_131_SBI_T1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_336CD_336Lines_1344Pw}},
   {K_Leaf, TRUE, " -> CR: 132_SBI_T1_F_Extended_Differential_CR 336-CD, 336-CT+CR 336 lines 672 PWs", {(Y_MnuLeafP) CLI_F_132_SBI_T1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_336CD_336Lines_672Pw_1DS0}},
   {K_Leaf, TRUE, " -> CR: 132_SBI_E1_F_Extended_Differential_CR 1-CD, 1-CT+CR 1 lines, 32 PWs", {(Y_MnuLeafP) CLI_F_132_SBI_E1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_1CD_1Lines_32Pw_1DS0}},
   {K_Leaf, TRUE, " -> CR: 133_SBI_E1_F_Extended_Differential_CR 252-CD, 252-CT+CR 252 lines 1008 PWs, Remove/recreate lines and PW", {(Y_MnuLeafP) CLI_F_133_SBI_E1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_252CD_252Lines_1008Pw_AddRemoveLinesPw}},
   {K_Leaf, TRUE, " -> CR: 136_SBI_E1_F_30xDS0_Extended_Differential_CR CAS_UNFRG_252-CD, 252-CT+CR 252 lines 252 PWs",  {(Y_MnuLeafP) CLI_F_136_SBI_E1_F_30xDS0_CR_EXTENDED_DIFF_Cas_Unrg_Two_Boards_252CD_252Lines_252Pw}},
#if WTI_CESOP_TO_MPLS_FA_CASCADING
   {K_Leaf, TRUE, " -> CR: 137_SBI_E1_U_Extended_Differential_CR, MPLS Cascading, 252-CD, 252-CT+CR 252 lines 252 PWs [<DU Size> <Packet Size>]", {(Y_MnuLeafP) CLI_F_137_SBI_E1_U_CR_EXTENDED_DIFF_NoCas_Mpls_Cascading_Two_Boards_252CD_252Lines_252Pw}},
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */
   {K_Leaf, TRUE, " -> CR: 140_SBI_E1_U_Ext_Diff_And_Adap_CR 252-CD, up to 32 adap, rest are ext. differential, 252 lines", {(Y_MnuLeafP) CLI_F_140_SBI_E1_U_CR_COMBINED_DIFF_ADAP_NoCas_Two_Boards_N_EXT_CD_M_REG_CD_252Lines}},
   {K_Leaf, TRUE, " -> CR: 140_SBI_T1_U_Ext_Diff_And_Adap_CR 336-CD, up to 32 adap, rest are ext. differential, 336 lines", {(Y_MnuLeafP) CLI_F_140_SBI_T1_U_CR_COMBINED_DIFF_ADAP_NoCas_Two_Boards_N_EXT_CD_M_REG_CD_336Lines}},
   {K_Leaf, TRUE, " -> CR: 140_SBI_E1_F_Ext_Diff_And_Adap_CR 252-CD, up to 32 adap, rest are ext. differential, 252 lines", {(Y_MnuLeafP) CLI_F_140_SBI_E1_F_CR_COMBINED_DIFF_ADAP_NoCas_Two_Boards_N_EXT_CD_M_REG_CD_252Lines}},
   {K_Leaf, TRUE, " -> CR: 140_SBI_T1_F_Ext_Diff_And_Adap_CR 336-CD, up to 32 adap, rest are ext. differential, 336 lines", {(Y_MnuLeafP) CLI_F_140_SBI_T1_F_CR_COMBINED_DIFF_ADAP_NoCas_Two_Boards_N_EXT_CD_M_REG_CD_336Lines}},
   {K_Leaf, TRUE, " -> CR: 150_SBI_E1_F_Extended_Differential_CR 252-CD, 252-CT+CR 252 lines 252 PWs Different DU and PAcket sizes", {(Y_MnuLeafP) CLI_F_150_SBI_E1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_252CD_DIFFERENT_DU_PACKET_SIZE}},
   {K_Leaf, TRUE, " -> CR: 230_CAD_E1_U_Extended_Differential_CR 4-CD, 4 lines", {(Y_MnuLeafP) CLI_F_230_CAD_E1_U_CR_EXTENDED_DIFF_NoCas_Two_Boards_4CD_4Lines}},
   {K_Leaf, TRUE, " -> CR: 230_CAD_T1_U_Extended_Differential_CR 4-CD, 4 lines", {(Y_MnuLeafP) CLI_F_230_CAD_T1_U_CR_EXTENDED_DIFF_NoCas_Two_Boards_4CD_4Lines}},
   {K_Leaf, TRUE, " -> CR: 230_CAD_E1_F_Extended_Differential_CR 4-CD, 4 lines", {(Y_MnuLeafP) CLI_F_230_CAD_E1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_4CD_4Lines}},
   {K_Leaf, TRUE, " -> CR: 230_CAD_T1_F_Extended_Differential_CR 4-CD, 4 lines", {(Y_MnuLeafP) CLI_F_230_CAD_T1_F_CR_EXTENDED_DIFF_NoCas_Two_Boards_4CD_4Lines}},
#endif

#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE && WTI_CESOP_RX_TIMING_ENABLE */
   {K_Leaf, TRUE, " -> DS3 unframed scenario, no CR",                                                                   {(Y_MnuLeafP) CLI_F_DS3UnframedNoCasSbi}},
   {K_Leaf, TRUE, " -> E3 unframed scenario, no CR",                                                                    {(Y_MnuLeafP) CLI_F_E3UnframedNoCasSbi}},
   {K_Leaf, TRUE, " -> DS3 framed scenario, no CR",                                                                     {(Y_MnuLeafP) CLI_F_DS3FramedNoCasSbi}},
   {K_Leaf, TRUE, " -> E3 framed scenario, no CR",                                                                      {(Y_MnuLeafP) CLI_F_E3FramedNoCasSbi}},
   {K_Leaf, TRUE, " -> CR: 109_SBI_DS3_U_CR 3-CD 1-JAT 3-lines 3 PWs",                                         {(Y_MnuLeafP) CLI_F_109_SBI_DS3_U_CR_NoCas_Two_Boards_3_CD_1_JAT_3_Line_3_PW}},
   {K_Leaf, TRUE, " -> CR: 109_SBI_DS3_F_CR 3-CD 1-JAT 3-lines 3 PWs",                                         {(Y_MnuLeafP) CLI_F_109_SBI_DS3_F_CR_NoCas_Two_Boards_3_CD_1_JAT_3_Line_3_PW}},
   {K_Leaf, TRUE, " -> 252 X DS1 E1, Unframed, No CAS, Sbi",                                                               {(Y_MnuLeafP) CLI_F_252xDS1E1UnframedNoCasSbi}},
   {K_Leaf, TRUE, " -> 192 Pw's E1, Framed, No CAS, Sbi",                                                               {(Y_MnuLeafP) CLI_F_192PwE1FramedNoCasSbi}},
   {K_Leaf, TRUE, " -> E1, Framed, UNFRG, Octal OOBC",                                                                  {(Y_MnuLeafP) CLI_F_E1Framed1DS0CasUnFrgOctalOobc}},
   {K_Leaf, TRUE, " -> E1, Framed, FRG, Octal OOBC",                                                                    {(Y_MnuLeafP) CLI_F_E1Framed1DS0CasFrgOctalOobc}},
   {K_Leaf, TRUE, " -> 2016 X (4 DS0) PW's on 252 lines in E1 framed mode,No CAS, Sbi (UFE3 only)",                     {(Y_MnuLeafP) CLI_F_SBI_E1_F_NoCas_252_Line_2016_4xDS0_PWS}},
   {K_Leaf, TRUE, " -> 336 X (24 DS0) PW's on 336 lines in T1 Unframed mode,No CAS, Sbi (UFE3 only)",                   {(Y_MnuLeafP) CLI_F_SBI_T1_U_NoCas_336_Line_336_PWS}},
#if WTI_CESOP_TO_MPLS_FA_CASCADING
   {K_Leaf, TRUE, " -> 336 X (24 DS0) PW's on 336 lines in T1 Unframed mode,No CAS, Sbi - MPLS cascading scenario(UFE3 only)", {(Y_MnuLeafP) CLI_F_SBI_T1_U_NoCas_336_Line_336_PWS_Cascaded}},
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */
   {K_Leaf, TRUE, " -> 8064 X (1 DS0) PW's on 252 lines in E1 framed mode,No CAS, Sbi (UFE3 only)",                     {(Y_MnuLeafP) CLI_F_SBI_E1_F_NoCas_252_Line_8064_1xDS0_PWS}},
#if WTI_CESOP_MEF8_ENCAP
   {K_Leaf, TRUE, " -> MEF8 Encapsulation",                                                                             {(Y_MnuLeafP) CLI_F_MEF8_ENCAP_1_Vlan}},
#endif

#else /*not __WT_UFE3__*/
#ifdef __WT_UFE412__
#if WTI_CESOP_MPLS_IW
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   { 63, TRUE, "System configurations: UFE4 Scenarios",{(Y_MenuEntry*) V_MainMenu}},
#else
   { 53, TRUE, "System configurations: UFE4 Scenarios",{(Y_MenuEntry*) V_MainMenu}},
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
   {K_Leaf, TRUE, " -> Initialize UFE framer (for Flexmux debug) [build_personality (7-1P1, 8-4P4, 15-2P2))]",                     {(Y_MnuLeafP) CLI_F_FlexmuxInit}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh/CAD, Unframed, Without Cas. [0- SDH, 1- SONET, 2- CAD] [physical conf: 0-oc12,1-4oc3,2-2oc3][in SDH:0-AU3,1-AU4][NUM_OF_LINES]",             {(Y_MnuLeafP) CLI_F_SonetSdhE1UnframedNoCas}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh/CAD, Framed, Without Cas. [0- SDH, 1- SONET, 2- CAD][physical conf: 0-oc12,1-4oc3,2-2oc3][in SDH:0-AU3,1-AU4] [NUM_OF_LINES]",               {(Y_MnuLeafP) CLI_F_SonetSdhE1FramedNoCas}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh/CAD, Framed, With Cas. [0- SDH, 1- SONET, 2- CAD] [NUM_OF_LINES] [fragmented?]",                  {(Y_MnuLeafP) CLI_F_SonetSdhE1FramedWithCas}},
   {K_Leaf, TRUE, " -> T1, Sonet/Sdh/CAD, Unframed, Without Cas. [0 - SDH, 1- SONET, 2- CAD][physical conf: 0-oc12,1-4oc3,2-2oc3][in SDH:0-AU3,1-AU4] [NUM_OF_LINES]",            {(Y_MnuLeafP) CLI_F_SonetSdhT1UnframedNoCas}},
   {K_Leaf, TRUE, " -> T1, Sonet/Sdh/CAD, Framed, Without Cas. [0 - SDH, 1- SONET, 2- CAD][physical conf: 0-oc12,1-4oc3,2-2oc3][in SDH:0-AU3,1-AU4] [NUM_OF_LINES]",              {(Y_MnuLeafP) CLI_F_SonetSdhT1FramedNoCas}},
   {K_Leaf, TRUE, " -> T1, Sonet/Sdh/CAD, Framed, With Cas. [0 - SDH, 1- SONET, 2- CAD] [NUM_OF_LINES]",                 {(Y_MnuLeafP) CLI_F_SonetSdhT1FramedWithCas}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, Framed, Without Cas. 1008 PWs 8xds0 for PW. [0- SDH, 1- SONET] [NUM_OF_LINES]",           {(Y_MnuLeafP) CLI_F_SonetSdhE1FramedNoCas_8xDS0_1008_PWs}},
   {K_Leaf, TRUE, " -> T1, Sonet/Sdh, Framed, Without Cas. 1344 PWs 6xds0 for PW. [0- SDH, 1- SONET] [NUM_OF_LINES]",           {(Y_MnuLeafP) CLI_F_SonetSdhT1FramedNoCas_6xDS0_1344_PWs}},
   {K_Leaf, TRUE, " -> Clear Channel, Sonet/Sdh. [0 - SDH/VC44C, 1- SDH/VC4, 2- SONET/STS12, 3- SONET/STS3]",              {(Y_MnuLeafP) CLI_F_SonetSdhClearChannel}},
   {K_Leaf, TRUE, " -> E1 & T1 combined, Sonet/Sdh, Unframed, Without Cas. [0- SDH, 1- SONET] [NUM_OF_LINES]",           {(Y_MnuLeafP) CLI_F_SonetSdhE1T1Combined}},
   {K_Leaf, TRUE, " -> E1-T1-E1, Sonet/Sdh, Unframed, Without Cas [0- SDH, 1- SONET] ",           {(Y_MnuLeafP) CLI_F_SonetSdh252E1_336T1_252E1}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, System Reconfigure. [0- SDH, 1- SONET]",                            {(Y_MnuLeafP) CLI_F_SonetSdhSystemReconfigure}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, Line Reconfigure. [0- SDH, 1- SONET]",                              {(Y_MnuLeafP) CLI_F_SonetSdhLineReconfigure}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, Phy Reconfigure. [0- SDH, 1- SONET]",                               {(Y_MnuLeafP) CLI_F_SonetSdhPhyReconfigure}},
   {K_Leaf, TRUE, " -> T1, Sonet/Sdh, Phy Create/Delete. [0- SDH, 1- SONET]",                            {(Y_MnuLeafP) CLI_F_SonetSdhT1PhyCreateDelete}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, Phy Enable/Disable. [0- SDH, 1- SONET][0-load lines][1-disable][2-enable][3-enable & disable in loop]",                            {(Y_MnuLeafP) CLI_F_SonetSdhE1PhyEnableDisable}},
   {K_Leaf, TRUE, " -> T1, Sonet/Sdh, Line Enable/Disable. [0- SDH, 1- SONET, 2- CAD]",                   {(Y_MnuLeafP) CLI_F_SonetSdhT1EnableDisable}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, Line Enable/Disable. [0- SDH, 1- SONET, 2- CAD]",                   {(Y_MnuLeafP) CLI_F_SonetSdhE1EnableDisable}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, error_4_e1_on_1_tug2. [0- SDH, 1- SONET]",                          {(Y_MnuLeafP) CLI_F_error_4_e1_on_1_tug2}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, error_disable_before_enable. [0- SDH, 1- SONET]",                   {(Y_MnuLeafP) CLI_F_error_disable_before_enable}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, error_ReEnable_line. [0- SDH, 1- SONET]",                           {(Y_MnuLeafP) CLI_F_error_ReEnable_line}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, Error_Enable_Disable_Disable. [0- SDH, 1- SONET]",                  {(Y_MnuLeafP) CLI_F_Error_Enable_Disable_Disable}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, Error_Enable_Before_Create. [0- SDH, 1- SONET]",                    {(Y_MnuLeafP) CLI_F_Error_Enable_Before_Create}},
   {K_Leaf, TRUE, " -> E3, Sonet/Sdh, Unframed, Without Cas. [0- SDH, 1- SONET] [physical conf: 0-oc12,1-4oc3,2-2oc3][in SDH:0-AU3,1-AU4][NUM_OF_LINES]",             {(Y_MnuLeafP) CLI_F_SonetSdhE3UnframedNoCas}},
   {K_Leaf, TRUE, " -> E3, Sonet/Sdh, Framed, Without Cas. [0- SDH, 1- SONET][physical conf: 0-oc12,1-4oc3,2-2oc3][in SDH:0-AU3,1-AU4] [NUM_OF_LINES]",               {(Y_MnuLeafP) CLI_F_SonetSdhE3FramedNoCas}},
   {K_Leaf, TRUE, " -> DS3, Sonet/Sdh, Unframed, Without Cas. [0 - SDH, 1- SONET][physical conf: 0-oc12,1-4oc3,2-2oc3][in SDH:0-AU3,1-AU4] [NUM_OF_LINES]",            {(Y_MnuLeafP) CLI_F_SonetSdhDS3UnframedNoCas}},
   {K_Leaf, TRUE, " -> DS3, Sonet/Sdh, Framed C-Parity, Without Cas. [0 - SDH, 1- SONET][physical conf: 0-oc12,1-4oc3,2-2oc3][in SDH:0-AU3,1-AU4] [NUM_OF_LINES]",              {(Y_MnuLeafP) CLI_F_SonetSdhDS3FramedCParityNoCas}},
   {K_Leaf, TRUE, " -> DS3, Sonet/Sdh, Framed M23, Without Cas. [0 - SDH, 1- SONET][physical conf: 0-oc12,1-4oc3,2-2oc3][in SDH:0-AU3,1-AU4] [NUM_OF_LINES]",              {(Y_MnuLeafP) CLI_F_SonetSdhDS3FramedM23NoCas}},
   {K_Leaf, TRUE, " -> DS3, Sonet/Sdh, Create/Delete. [0 - SDH, 1- SONET][physical conf: 0-oc12,1-4oc3,2-2oc3][in SDH:0-AU3,1-AU4] [NUM_OF_LINES]",              {(Y_MnuLeafP) CLI_F_SonetSdhDS3CreateDelete}},
   {K_Leaf, TRUE, " -> DS3, Sonet/Sdh, Enable/Disable. [0 - SDH, 1- SONET][physical conf: 0-oc12,1-4oc3,2-2oc3][in SDH:0-AU3,1-AU4] [NUM_OF_LINES]",              {(Y_MnuLeafP) CLI_F_SonetSdhDS3EnableDisable}},
   {K_Leaf, TRUE, " -> E3, Sonet/Sdh, Create/Delete. [0 - SDH, 1- SONET][physical conf: 0-oc12,1-4oc3,2-2oc3][in SDH:0-AU3,1-AU4] [NUM_OF_LINES]",              {(Y_MnuLeafP) CLI_F_SonetSdhE3CreateDelete}},
   {K_Leaf, TRUE, " -> E3, Sonet/Sdh, Enable/Disable. [0 - SDH, 1- SONET][physical conf: 0-oc12,1-4oc3,2-2oc3][in SDH:0-AU3,1-AU4] [NUM_OF_LINES]",              {(Y_MnuLeafP) CLI_F_SonetSdhE3EnableDisable}},
   {K_Leaf, TRUE, " -> E3DS3 Combined, Sonet/Sdh. [0 - SDH, 1- SONET][physical conf: 0-oc12,1-4oc3,2-2oc3][in SDH:0-AU3,1-AU4] [NUM_OF_LINES]",  {(Y_MnuLeafP) CLI_F_SonetSdhE3DS3Combined}},
   {K_Leaf, TRUE, " -> DS3VFP Combined, Sonet/Sdh. [0 - SDH, 1- SONET][physical conf: 0-oc12,1-4oc3][in SDH:0-AU3,1-AU4] [NUM_OF_LINES]",    {(Y_MnuLeafP) CLI_F_SonetSdh3DS32VFP3DS3}},
   {K_Leaf, TRUE, " -> DS3VFPT1 Combined, Sonet/Sdh. [0 - SDH, 1- SONET][physical conf: 0-oc12,1-4oc3][in SDH:0-AU3,1-AU4]",  {(Y_MnuLeafP) CLI_F_SonetSdh3DS31VFP3DS328T1}},
   {K_Leaf, TRUE, " -> DS3 Loopback, Sonet/Sdh, [0 - SDH, 1- SONET][physical conf: 0-oc12,1-4oc3,2-2oc3][in SDH:0-AU3,1-AU4] [NUM_OF_LINES]",              {(Y_MnuLeafP) CLI_F_SonetSdhDS3Loopback}},
   {K_Leaf, TRUE, " -> E3 Loopback, Sonet/Sdh, [0 - SDH, 1- SONET][physical conf: 0-oc12,1-4oc3,2-2oc3][in SDH:0-AU3,1-AU4] [NUM_OF_LINES]",              {(Y_MnuLeafP) CLI_F_SonetSdhE3Loopback}},
   {K_Leaf, TRUE, " -> DS3 Protection Switching, Sonet/Sdh, [0 - SDH, 1- SONET][physical conf: 0-oc12,1-4oc3,2-2oc3][in SDH:0-AU3,1-AU4] [NUM_OF_LINES]",              {(Y_MnuLeafP) CLI_F_SonetSdhDS3ProtectionSwitch}},
   {K_Leaf, TRUE, " -> E3 Protection Switching, Sonet/Sdh, [0 - SDH, 1- SONET][physical conf: 0-oc12,1-4oc3,2-2oc3][in SDH:0-AU3,1-AU4] [NUM_OF_LINES]",              {(Y_MnuLeafP) CLI_F_SonetSdhE3ProtectionSwitch}},
   {K_Leaf, TRUE, " -> CR 101, E1, Sonet/Sdh/CAD, Unframed, Without Cas. [NUM_OF_LINES][0- SDH, 1- SONET, 2- CAD]",                            {(Y_MnuLeafP) CLI_F_CR_101_SonetSdh_E1UnframedNoCas}},
   {K_Leaf, TRUE, " -> CR 101, T1, Sonet/Sdh/CAD, Unframed, Without Cas. [NUM_OF_LINES][0- SDH, 1- SONET, 2- CAD]",                            {(Y_MnuLeafP) CLI_F_CR_101_SonetSdh_T1UnframedNoCas}},
   {K_Leaf, TRUE, " -> CR 101, E1, Sonet/Sdh/CAD, Framed, Without Cas. [NUM_OF_LINES][0- SDH, 1- SONET, 2- CAD]",                              {(Y_MnuLeafP) CLI_F_CR_101_SonetSdh_E1FramedNoCas}},
   {K_Leaf, TRUE, " -> CR 101, T1, Sonet/Sdh/CAD, Framed, Without Cas. [NUM_OF_LINES][0- SDH, 1- SONET, 2- CAD]",                              {(Y_MnuLeafP) CLI_F_CR_101_SonetSdh_T1FramedNoCas}},
   {K_Leaf, TRUE, " -> CR 101, E1, Sonet/Sdh/CAD, Framed, 1xDS0, slot #30, Without Cas. [NUM_OF_LINES][FIRST_SLOT_ID][NUM_OF_SLOTS][0- SDH, 1- SONET, 2- CAD]",             {(Y_MnuLeafP) CLI_F_CR_101_SonetSdh_E1Framed1xDS0NoCas}},
   {K_Leaf, TRUE, " -> CR 101, T1, Sonet/Sdh/CAD, Framed, 1xDS0, slot #14, Without Cas. [NUM_OF_LINES][FIRST_SLOT_ID][NUM_OF_SLOTS][0- SDH, 1- SONET, 2- CAD]",            {(Y_MnuLeafP) CLI_F_CR_101_SonetSdh_T1Framed1xDS0NoCas}},
   {K_Leaf, TRUE, " -> CR 102, E1, Framed, With Cas. [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD][0- Unfragmented, 1-Fragmented]",  {(Y_MnuLeafP) CLI_F_CR_102_SonetSdh_E1FramedCas}},
   {K_Leaf, TRUE, " -> CR 102, T1, Framed, With Cas. [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD][0- Unfragmented, 1-Fragmented]",  {(Y_MnuLeafP) CLI_F_CR_102_SonetSdh_T1FramedCas}},
   {K_Leaf, TRUE, " -> CR 103, E1, Framed, No Cas. Multiple PWs per line [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD][#PWs/line][recovered PW]",   {(Y_MnuLeafP) CLI_F_CR_103_SonetSdh_E1FramedNoCas_MultiplePwPerLine}},
   {K_Leaf, TRUE, " -> CR 103, T1, Framed, No Cas. Multiple PWs per line [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD][#PWs/line][recovered PW]",   {(Y_MnuLeafP) CLI_F_CR_103_SonetSdh_T1FramedNoCas_MultiplePwPerLine}},
   {K_Leaf, TRUE, " -> CR 104, E1, Unframed, No Cas. Remove/recreate all PW/lines. [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD][0-remove PW, 1-remove PW+Lines]",  {(Y_MnuLeafP) CLI_F_CR_104_SonetSdh_E1UnframedNoCas_AddRemovePwLine}},
   {K_Leaf, TRUE, " -> CR 104, T1, Unframed, No Cas. Remove/recreate all PW/lines. [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD][0-remove PW, 1-remove PW+Lines]",  {(Y_MnuLeafP) CLI_F_CR_104_SonetSdh_T1UnframedNoCas_AddRemovePwLine}},
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   /* Clock Recovery Tests */
   {K_Leaf, TRUE, " -> CR 104, E1, Framed, No Cas. Remove/recreate all PW/lines. [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD][0-remove PW, 1-remove PW+Lines]",  {(Y_MnuLeafP) CLI_F_CR_104_SonetSdh_E1FramedNoCas_AddRemovePwLine}},
   {K_Leaf, TRUE, " -> CR 104, T1, Framed, No Cas. Remove/recreate all PW/lines. [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD][0-remove PW, 1-remove PW+Lines]",  {(Y_MnuLeafP) CLI_F_CR_104_SonetSdh_T1FramedNoCas_AddRemovePwLine}},
   {K_Leaf, TRUE, " -> CR 105, E1, Unframed, No Cas. Different DU and Packet sizes. [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD]",  {(Y_MnuLeafP) CLI_F_CR_105_SonetSdh_E1UnframedNoCas_DifferentDuAndPs}},
   {K_Leaf, TRUE, " -> CR 105, T1, Unframed, No Cas. Different DU and Packet sizes. [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD]",  {(Y_MnuLeafP) CLI_F_CR_105_SonetSdh_T1UnframedNoCas_DifferentDuAndPs}},
   {K_Leaf, TRUE, " -> CR 105, E1, Framed, No Cas. Different DU and Packet sizes. [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD]",    {(Y_MnuLeafP) CLI_F_CR_105_SonetSdh_E1FramedNoCas_DifferentDuAndPs}},
   {K_Leaf, TRUE, " -> CR 105, T1, Framed, No Cas. Different DU and Packet sizes. [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD]",    {(Y_MnuLeafP) CLI_F_CR_105_SonetSdh_T1FramedNoCas_DifferentDuAndPs}},
   {K_Leaf, TRUE, " -> CR 106, E1, Unframed, Without Cas. Adaptive/Differential per line [NUM_OF_LINES][0-SDH, 1- SONET, 2- CAD]",                        {(Y_MnuLeafP) CLI_F_CR_106_SonetSdh_E1UnframedNoCas_AdapDiffPerLine}},
   {K_Leaf, TRUE, " -> CR 106, T1, Unframed, Without Cas. Adaptive/Differential per line [NUM_OF_LINES][0-SDH, 1- SONET, 2- CAD]",                        {(Y_MnuLeafP) CLI_F_CR_106_SonetSdh_T1UnframedNoCas_AdapDiffPerLine}},
   {K_Leaf, TRUE, " -> CR 106, E1, Framed, Without Cas. Adaptive/Differential per line [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD]",                            {(Y_MnuLeafP) CLI_F_CR_106_SonetSdh_E1FramedNoCas_AdapDiffPerLine}},
   {K_Leaf, TRUE, " -> CR 106, T1, Framed, Without Cas. Adaptive/Differential per line [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD]",                            {(Y_MnuLeafP) CLI_F_CR_106_SonetSdh_T1FramedNoCas_AdapDiffPerLine}},
#endif  /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
#if __WT_UFE412__
   {K_Leaf, TRUE, " -> Sanity Board Test 115, E1, UnFramed, Without Cas. Adaptive/Differential",                                                          { (Y_MnuLeafP) CLI_F_CR_115_Sanity_Board_Check_SonetSdh_E1UnframedNoCas } },
#endif
#else /* !WTI_CESOP_MPLS_IW */
   {0,TRUE, "UFE4 Scenarios are under WTI_CESOP_MPLS_IW only",{(Y_MenuEntry*) V_MainMenu}},
#endif/* WTI_CESOP_MPLS_IW */
#else  /* not __WT_UFE412__*/
#ifdef __WT_UFE448__
#if WTI_CESOP_MPLS_IW
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   { 46, TRUE, "System configurations: UFE4 Scenarios",{(Y_MenuEntry*) V_MainMenu}},
#else
   { 36, TRUE, "System configurations: UFE4 Scenarios",{(Y_MenuEntry*) V_MainMenu}},
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
   {K_Leaf, TRUE, " -> Initialize UFE framer (for Flexmux debug) [build_personality 1-4+4OC12, 2-8+8OC3]",                     {(Y_MnuLeafP) CLI_F_FlexmuxInit}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh/CAD, Unframed, Without Cas. [0- SDH, 1- SONET, 2- CAD] [physical conf: 0-4+4oc12,1-8+8oc3,2-4+4OC12 M,3-4+4OC12 S ][in SDH:0-AU3,1-AU4][NUM_OF_LINES]",             {(Y_MnuLeafP) CLI_F_SonetSdhE1UnframedNoCas}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh/CAD, Framed, Without Cas. [0- SDH, 1- SONET, 2- CAD][physical conf: 0-4+4oc12,1-8+8oc3][in SDH:0-AU3,1-AU4] [NUM_OF_LINES]",               {(Y_MnuLeafP) CLI_F_SonetSdhE1FramedNoCas}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh/CAD, Framed, With Cas. [0- SDH, 1- SONET, 2- CAD][physical conf: 0-4+4oc12,1-8+8oc3][in SDH:0-AU3,1-AU4][NUM_OF_LINES]",                  {(Y_MnuLeafP) CLI_F_SonetSdhE1FramedWithCas}},
   {K_Leaf, TRUE, " -> T1, Sonet/Sdh/CAD, Unframed, Without Cas. [0 - SDH, 1- SONET, 2- CAD][physical conf: 0-4+4oc12,1-8+8oc3][in SDH:0-AU3,1-AU4] [NUM_OF_LINES]",            {(Y_MnuLeafP) CLI_F_SonetSdhT1UnframedNoCas}},
   {K_Leaf, TRUE, " -> T1, Sonet/Sdh/CAD, Framed, Without Cas. [0 - SDH, 1- SONET, 2- CAD][physical conf: 0-4+4oc12,1-8+8oc3][in SDH:0-AU3,1-AU4] [NUM_OF_LINES]",              {(Y_MnuLeafP) CLI_F_SonetSdhT1FramedNoCas}},
   {K_Leaf, TRUE, " -> T1, Sonet/Sdh/CAD, Framed, With Cas. [0 - SDH, 1- SONET, 2- CAD][physical conf: 0-4+4oc12,1-8+8oc3][in SDH:0-AU3,1-AU4][NUM_OF_LINES]",                 {(Y_MnuLeafP) CLI_F_SonetSdhT1FramedWithCas}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, Framed, Without Cas. 2016 PWs 16xDS0 per PW. [0- SDH, 1- SONET]",           {(Y_MnuLeafP) CLI_F_SonetSdhE1FramedNoCas_2016_PWs_16xDS0_per_PW }},
   {K_Leaf, TRUE, " -> T1, Sonet/Sdh, Framed, Without Cas. 2688 PWs 12xDS0 per PW. [0- SDH, 1- SONET]",           {(Y_MnuLeafP) CLI_F_SonetSdhT1FramedNoCas_2688_PWs_12xDS0_per_PW}},
   {K_Leaf, TRUE, " -> E1 & T1 combined, Sonet/Sdh, Unframed, Without Cas. [0- SDH, 1- SONET] [NUM_OF_LINES]",           {(Y_MnuLeafP) CLI_F_SonetSdhE1T1Combined}},
   {K_Leaf, TRUE, " -> E1-T1-E1, Sonet/Sdh, Unframed, Without Cas [0- SDH, 1- SONET] ",           {(Y_MnuLeafP) CLI_F_SonetSdh1008E1_1344T1_1008E1}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, System Reconfigure. [0- SDH, 1- SONET]",                            {(Y_MnuLeafP) CLI_F_SonetSdhSystemReconfigure}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, Line Reconfigure. [0- SDH, 1- SONET]",                              {(Y_MnuLeafP) CLI_F_SonetSdhLineReconfigure}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, Phy Reconfigure. [0- SDH, 1- SONET]",                               {(Y_MnuLeafP) CLI_F_SonetSdhPhyReconfigure}},
   {K_Leaf, TRUE, " -> T1, Sonet/Sdh, Phy Create/Delete. [0- SDH, 1- SONET]",                            {(Y_MnuLeafP) CLI_F_SonetSdhT1PhyCreateDelete}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, Phy Enable/Disable. [0- SDH, 1- SONET][0-load lines][1-disable][2-enable][3-enable & disable in loop]",                            {(Y_MnuLeafP) CLI_F_SonetSdhE1PhyEnableDisable}},
   {K_Leaf, TRUE, " -> T1, Sonet/Sdh, Line Enable/Disable. [0- SDH, 1- SONET, 2- CAD]",                   {(Y_MnuLeafP) CLI_F_SonetSdhT1EnableDisable}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, Line Enable/Disable. [0- SDH, 1- SONET, 2- CAD]",                   {(Y_MnuLeafP) CLI_F_SonetSdhE1EnableDisable}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, error_4_e1_on_1_tug2. [0- SDH, 1- SONET]",                          {(Y_MnuLeafP) CLI_F_error_4_e1_on_1_tug2}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, error_disable_before_enable. [0- SDH, 1- SONET]",                   {(Y_MnuLeafP) CLI_F_error_disable_before_enable}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, error_ReEnable_line. [0- SDH, 1- SONET]",                           {(Y_MnuLeafP) CLI_F_error_ReEnable_line}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, Error_Enable_Disable_Disable. [0- SDH, 1- SONET]",                  {(Y_MnuLeafP) CLI_F_Error_Enable_Disable_Disable}},
   {K_Leaf, TRUE, " -> E1, Sonet/Sdh, Error_Enable_Before_Create. [0- SDH, 1- SONET]",                    {(Y_MnuLeafP) CLI_F_Error_Enable_Before_Create}},
   {K_Leaf, TRUE, " -> CR 101, E1, Sonet/Sdh/CAD, Unframed, Without Cas. [NUM_OF_LINES][0- SDH, 1- SONET, 2- CAD]",                            {(Y_MnuLeafP) CLI_F_CR_101_SonetSdh_E1UnframedNoCas}},
   {K_Leaf, TRUE, " -> CR 101, T1, Sonet/Sdh/CAD, Unframed, Without Cas. [NUM_OF_LINES][0- SDH, 1- SONET, 2- CAD]",                            {(Y_MnuLeafP) CLI_F_CR_101_SonetSdh_T1UnframedNoCas}},
   {K_Leaf, TRUE, " -> CR 101, E1, Sonet/Sdh/CAD, Framed, Without Cas. [NUM_OF_LINES][0- SDH, 1- SONET, 2- CAD]",                              {(Y_MnuLeafP) CLI_F_CR_101_SonetSdh_E1FramedNoCas}},
   {K_Leaf, TRUE, " -> CR 101, T1, Sonet/Sdh/CAD, Framed, Without Cas. [NUM_OF_LINES][0- SDH, 1- SONET, 2- CAD]",                              {(Y_MnuLeafP) CLI_F_CR_101_SonetSdh_T1FramedNoCas}},
   {K_Leaf, TRUE, " -> CR 101, E1, Sonet/Sdh/CAD, Framed, 1xDS0, slot #30, Without Cas. [NUM_OF_LINES][FIRST_SLOT_ID][NUM_OF_SLOTS][0- SDH, 1- SONET, 2- CAD]",             {(Y_MnuLeafP) CLI_F_CR_101_SonetSdh_E1Framed1xDS0NoCas}},
   {K_Leaf, TRUE, " -> CR 101, T1, Sonet/Sdh/CAD, Framed, 1xDS0, slot #14, Without Cas. [NUM_OF_LINES][FIRST_SLOT_ID][NUM_OF_SLOTS][0- SDH, 1- SONET, 2- CAD]",            {(Y_MnuLeafP) CLI_F_CR_101_SonetSdh_T1Framed1xDS0NoCas}},
   {K_Leaf, TRUE, " -> CR 102, E1, Framed, With Cas. [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD][0- Unfragmented, 1-Fragmented]",  {(Y_MnuLeafP) CLI_F_CR_102_SonetSdh_E1FramedCas}},
   {K_Leaf, TRUE, " -> CR 102, T1, Framed, With Cas. [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD][0- Unfragmented, 1-Fragmented]",  {(Y_MnuLeafP) CLI_F_CR_102_SonetSdh_T1FramedCas}},
   {K_Leaf, TRUE, " -> CR 103, E1, Framed, No Cas. Multiple PWs per line [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD][#PWs/line][recovered PW]",   {(Y_MnuLeafP) CLI_F_CR_103_SonetSdh_E1FramedNoCas_MultiplePwPerLine}},
   {K_Leaf, TRUE, " -> CR 103, T1, Framed, No Cas. Multiple PWs per line [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD][#PWs/line][recovered PW]",   {(Y_MnuLeafP) CLI_F_CR_103_SonetSdh_T1FramedNoCas_MultiplePwPerLine}},
   {K_Leaf, TRUE, " -> CR 104, E1, Unframed, No Cas. Remove/recreate all PW/lines. [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD][0-remove PW, 1-remove PW+Lines]",  {(Y_MnuLeafP) CLI_F_CR_104_SonetSdh_E1UnframedNoCas_AddRemovePwLine}},
   {K_Leaf, TRUE, " -> CR 104, T1, Unframed, No Cas. Remove/recreate all PW/lines. [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD][0-remove PW, 1-remove PW+Lines]",  {(Y_MnuLeafP) CLI_F_CR_104_SonetSdh_T1UnframedNoCas_AddRemovePwLine}},
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   /* Clock Recovery Tests */
   {K_Leaf, TRUE, " -> CR 104, E1, Framed, No Cas. Remove/recreate all PW/lines. [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD][0-remove PW, 1-remove PW+Lines]",  {(Y_MnuLeafP) CLI_F_CR_104_SonetSdh_E1FramedNoCas_AddRemovePwLine}},
   {K_Leaf, TRUE, " -> CR 104, T1, Framed, No Cas. Remove/recreate all PW/lines. [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD][0-remove PW, 1-remove PW+Lines]",  {(Y_MnuLeafP) CLI_F_CR_104_SonetSdh_T1FramedNoCas_AddRemovePwLine}},
   {K_Leaf, TRUE, " -> CR 105, E1, Unframed, No Cas. Different DU and Packet sizes. [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD]",  {(Y_MnuLeafP) CLI_F_CR_105_SonetSdh_E1UnframedNoCas_DifferentDuAndPs}},
   {K_Leaf, TRUE, " -> CR 105, T1, Unframed, No Cas. Different DU and Packet sizes. [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD]",  {(Y_MnuLeafP) CLI_F_CR_105_SonetSdh_T1UnframedNoCas_DifferentDuAndPs}},
   {K_Leaf, TRUE, " -> CR 105, E1, Framed, No Cas. Different DU and Packet sizes. [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD]",    {(Y_MnuLeafP) CLI_F_CR_105_SonetSdh_E1FramedNoCas_DifferentDuAndPs}},
   {K_Leaf, TRUE, " -> CR 105, T1, Framed, No Cas. Different DU and Packet sizes. [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD]",    {(Y_MnuLeafP) CLI_F_CR_105_SonetSdh_T1FramedNoCas_DifferentDuAndPs}},
   {K_Leaf, TRUE, " -> CR 106, E1, Unframed, Without Cas. Adaptive/Differential per line [NUM_OF_LINES][0-SDH, 1- SONET, 2- CAD]",                        {(Y_MnuLeafP) CLI_F_CR_106_SonetSdh_E1UnframedNoCas_AdapDiffPerLine}},
   {K_Leaf, TRUE, " -> CR 106, T1, Unframed, Without Cas. Adaptive/Differential per line [NUM_OF_LINES][0-SDH, 1- SONET, 2- CAD]",                        {(Y_MnuLeafP) CLI_F_CR_106_SonetSdh_T1UnframedNoCas_AdapDiffPerLine}},
   {K_Leaf, TRUE, " -> CR 106, E1, Framed, Without Cas. Adaptive/Differential per line [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD]",                            {(Y_MnuLeafP) CLI_F_CR_106_SonetSdh_E1FramedNoCas_AdapDiffPerLine}},
   {K_Leaf, TRUE, " -> CR 106, T1, Framed, Without Cas. Adaptive/Differential per line [NUM_OF_LINES][0-SDH, 1-SONET, 2-CAD]",                            {(Y_MnuLeafP) CLI_F_CR_106_SonetSdh_T1FramedNoCas_AdapDiffPerLine}},
#endif  /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
   {K_Leaf, TRUE, " -> Sanity Board Test 115, E1, UnFramed, Without Cas. Adaptive/Differential",                                                          { (Y_MnuLeafP) CLI_F_CR_115_Sanity_Board_Check_SonetSdh_E1UnframedNoCas } },
#else /* !WTI_CESOP_MPLS_IW */
   {0,TRUE, "UFE4 Scenarios are under WTI_CESOP_MPLS_IW only",{(Y_MenuEntry*) V_MainMenu}},
#endif/* WTI_CESOP_MPLS_IW */
#endif /* __WT_UFE448__ */
#endif /* ifdef __WT__UFE412__*/
#endif /* ifdef __WT_UFE3__ */

};
#endif

const Y_MenuEntry V_Statistics []=
{
   {
#if (WTI_CESOP_MPLS_IW && (!WTI_CESOP_MPLS_OVER_ENET))
#if WT_UFE_FRAMER
      10
#else
       8
#endif
#else
#if WT_UFE_FRAMER
#if WTI_COLLECT_TRACKING_DATA
      13
#else
      11
#endif
#else
#if WTI_COLLECT_TRACKING_DATA
      11
#else
       9
#endif
#endif
#endif
      ,TRUE, "Statistics",                                                                                                   {(Y_MenuEntry*) V_MainMenu}},
   {K_Menu, TRUE, " -> Device statistics",                                                                              {(Y_MenuEntry*) V_DeviceStatistics}},
   {K_Menu, TRUE, " -> Channel statistics",                                                                             {(Y_MenuEntry*) V_ChannelStatistics}},
   {K_Menu, TRUE, " -> Flow Aggregation statistics",                                                                    {(Y_MenuEntry*) V_FlowAggStatistics}},
#if (WTI_CESOP_MPLS_IW && (!WTI_CESOP_MPLS_OVER_ENET))

#if WTI_CESOP_TDI
   {K_Leaf, TRUE, " -> Iw Global Statistics [port_type(0-TRANS port,1-HSPOS port) port_index(0 for HSPOS port)]",       {(Y_MnuLeafP)   CLI_F_IwGlobalStatistics}},
#else
   {K_Leaf, TRUE, " -> Iw Global Statistics [port_type(0-TRANS port,1-HSPOS port)]",                                    {(Y_MnuLeafP)   CLI_F_IwGlobalStatistics}},
#endif

#else
   {K_Leaf, TRUE, " -> PSN->TDM IW port statistics",                                                                    {(Y_MnuLeafP)   CLI_F_IwPortStatistics}},
#if WTI_CESOP_TDI
#if (WTI_4_ENET_DEVICES)
   {K_Leaf, TRUE, " -> Iw Global Statistics [port_type(0-TRANS port,1-ENET1 port, 2-ENET2 port, 3-ENET3 port, 4-ENET4 port) port_index]",           {(Y_MnuLeafP)   CLI_F_IwGlobalStatistics}},
#elif (WTI_2_ENET_DEVICES)
   {K_Leaf, TRUE, " -> Iw Global Statistics [port_type(0-TRANS port,1-ENET1 port, 2-ENET2 port) port_index]",           {(Y_MnuLeafP)   CLI_F_IwGlobalStatistics}},
#else
   {K_Leaf, TRUE, " -> Iw Global Statistics [port_type(0-TRANS port,1-ENET1 port) port_index]",                         {(Y_MnuLeafP)   CLI_F_IwGlobalStatistics}},
#endif
#else
#if (WTI_4_ENET_DEVICES)
   {K_Leaf, TRUE, " -> Iw Global Statistics [port_type(0-TRANS port,1-ENET1 port, 2-ENET2 port, 3-ENET3 port, 4-ENET4 port) port_index]",           {(Y_MnuLeafP)   CLI_F_IwGlobalStatistics}},
#elif (WTI_2_ENET_DEVICES)
   {K_Leaf, TRUE, " -> Iw Global Statistics [port_type(0-TRANS port,1-ENET1 port, 2-ENET2 port)]",                      {(Y_MnuLeafP)   CLI_F_IwGlobalStatistics}},
#else
   {K_Leaf, TRUE, " -> Iw Global Statistics [port_type(0-TRANS port,1-ENET1 port)]",                                    {(Y_MnuLeafP)   CLI_F_IwGlobalStatistics}},
#endif
#endif
#endif
   {K_Leaf, TRUE, " -> Memory Status",                                                                                  {(Y_MnuLeafP)   CLI_F_StatShowMem}},
   {K_Leaf, TRUE, " -> Pwe3StatisticsClear [optional: highest port number]",                                            {(Y_MnuLeafP)   CLI_F_Pwe3StatisticsClear}},
   {K_Leaf, TRUE, " -> Pwe3StatisticsCheck [optional: check RX TS (0-uncheck, 1-check), optional: highest port number, optional: global display - 1 ]",              {(Y_MnuLeafP)   CLI_F_Pwe3StatisticsCheck}},
   {K_Leaf, TRUE, " -> Global statistics Check [optional: clear statistics (0-don't clear, 1-clear), optional: iterations (1-20)]",   {(Y_MnuLeafP) CLI_F_GlobalStatisticsCheck}},
#if WTI_COLLECT_TRACKING_DATA
   {K_Leaf, TRUE, " -> Interrupt and Thread Timing Data", {(Y_MnuLeafP) CLI_F_StatTimingData}},
   {K_Leaf, TRUE, " -> Reset Interrupt and Thread Timing Data", {(Y_MnuLeafP) CLI_F_ResetTimingData}},
#endif
#if WT_UFE_FRAMER
   {K_Leaf, TRUE, " -> Bulk Alarm Get", {(Y_MnuLeafP) CLI_F_GetAlarmState}},
   {K_Leaf, TRUE, " -> Analyze UFE Events", {(Y_MnuLeafP) CLI_F_DisplayUfeEvents}},
#endif
};

const Y_MenuEntry V_Host []=
{
   {
#ifdef __linux__
#if WTI_CESOP_TDI == 0
#ifdef WT_UFE_FRAMER
      11,
#else
      7,
#endif
#else
#ifdef WT_UFE_FRAMER
      7,
#else
      5,
#endif
#endif
#else
#if WTI_CESOP_TDI == 0
#ifdef WT_UFE_FRAMER
      11,
#else
      8,
#endif
#else
#ifdef WT_UFE_FRAMER
      7,
#else
      4,
#endif
#endif
#endif
      TRUE, "Host interface",                                                                              {(Y_MenuEntry*) V_MainMenu}},
   {K_Leaf, TRUE, " -> Service event queues",                                                              {(Y_MnuLeafP)   CLI_F_TrafficStart}},
   {K_Leaf, TRUE, " -> Send Last CAS [pw_index]",                                                          {(Y_MnuLeafP)   CLI_F_SendLastCasHostCommand}},
   {K_Leaf, TRUE, " -> Poll event queues",                                                                 {(Y_MnuLeafP)   CLI_F_SimulateInterrupts}},
#if WTI_CESOP_TDI == 0
   {K_Leaf, TRUE, " -> Ufe interrupts",                                                                    {(Y_MnuLeafP)   CLI_F_UfeInterrupts}},
#endif
   {K_Leaf, TRUE, " -> Rx host Channels statistics [channel_num]",                                         {(Y_MnuLeafP)   CLI_F_HostChannelStatistics}},
#ifdef __linux__
   {K_Leaf, TRUE, " -> listen on TUNNEL driver",                                                           {(Y_MnuLeafP)   CLI_F_TunDriverListen}},
#endif
   {K_Leaf, TRUE, " -> Create load on host bus [Delay-microseconds , Number of read actions each time ] ", {(Y_MnuLeafP)   CLI_F_LoadHostBus}},
#if defined  __WT_UFE412__ || defined __WT_UFE448__
#ifdef WT_UFE_FRAMER
   {K_Leaf, TRUE, " -> Enable AIS insertion on [device]",  {(Y_MnuLeafP) AIS_insertion_enable}},
   {K_Leaf, TRUE, " -> Disable AIS insertion on [device]", {(Y_MnuLeafP) AIS_insertion_disable}},
   {K_Leaf, TRUE, " -> PDH AIS towards line on [device] [pdh] [enable ==> 1, disable ==> 0]", {(Y_MnuLeafP) CLI_F_PdhAisTowardsLine}},
   {K_Leaf, TRUE, " -> PDH RDI towards line on [device] [pdh] [enable ==> 1, disable ==> 0]", {(Y_MnuLeafP) CLI_F_PdhRdiTowardsLine}},
   {K_Leaf, TRUE, " -> PDH FEAC towards line on [device] [pdh] [FEAC Code]", {(Y_MnuLeafP) CLI_F_PdhFEACTowardsLine}},
#endif
#endif
};

#if 1
const Y_MenuEntry V_Menu_DebugBaseDump[] =
{
   {2    , TRUE,  " Main menu",                {(Y_MenuEntry*) V_Menu_Debug}},
   {K_Leaf, TRUE, " 1 Debug memory display [optional: size ] " ,  {(Y_MnuLeafP)CLI_DebugMemDisplay}},
   {K_Leaf, TRUE, " 2 Debug memory set [offset value] ",  {(Y_MnuLeafP)CLI_DebugMemModify}},
};

const Y_MenuEntry V_Menu_Debug[] =
{
   {9    , TRUE,  " Main menu",                   {(Y_MenuEntry*) V_MainMenu}},
   {K_Leaf, TRUE, " Check for Bus Error ",        {(Y_MnuLeafP)CLI_WinUtilBusErrCheck }},
   {K_Leaf, TRUE, " Wingine and Bus Utilization", {(Y_MnuLeafP)CLI_WinUtilWingineBusUtilization}},
   {K_Leaf, TRUE, " Thread Utilization",          {(Y_MnuLeafP)CLI_WinUtilThreadUtilization}},
   {K_Leaf, TRUE, " Thread Arbiter Dump",         {(Y_MnuLeafP)CLI_WinUtilThreadArbiterDump}},
   {K_Leaf, TRUE, " Serial Arbiter Dump",         {(Y_MnuLeafP)CLI_WinUtilSerialArbiterDump}},
   {K_Leaf, TRUE, " DPS Hardware Trace Start",    {(Y_MnuLeafP)CLI_WinUtilTraceStart}},
   {K_Leaf, TRUE, " DPS Hardware Trace Dump",     {(Y_MnuLeafP)CLI_WinUtilTraceStart}},
   {K_Menu, TRUE, " Debug Area (TA b254) ",       {(Y_MenuEntry*)V_Menu_DebugBaseDump}},
   {K_Leaf, TRUE, " Version Info",                {(Y_MnuLeafP)CLI_DebugVersionInfo}},
};
#endif
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
const Y_MenuEntry V_ClockRecoveryUtil []=
{
#ifdef WP_HW_WINPATH3
#if defined __WT_UFE412__ || defined __WT_UFE448__
   {23, TRUE, "Clock recovery utils",                                           {(Y_MenuEntry*) V_MainMenu}},
#else /* not __WT_UFE412__ || __WT_UFE448__ */
   {20, TRUE, "Clock recovery utils",                                           {(Y_MenuEntry*) V_MainMenu}},
#endif  /* not  __WT_UFE412__ || __WT_UFE448__*/

#else /* not WP_HW_WINPATH3 */
   {16, TRUE, "Clock recovery utils",                                           {(Y_MenuEntry*) V_MainMenu}},
#endif /*not WP_HW_WINPATH3 */
   {K_Leaf, TRUE, " -> view number of packets in queue[pw_index]",              {(Y_MnuLeafP) CLI_F_Pwe3ChannelQDepth}},
   {K_Leaf, TRUE, " -> Force holdover state [pw_index]",                        {(Y_MnuLeafP) CLI_F_ClockRecoveryHoldoverForce}},
   {K_Leaf, TRUE, " -> Clear holdover state [pw_index]",                        {(Y_MnuLeafP) CLI_F_ClockRecoveryHoldoverClear}},
   {K_Leaf, TRUE, " -> Show clock recovery systems states",                     {(Y_MnuLeafP) CLI_F_ClockRecoverySystemsShowState}},
   {K_Leaf, TRUE, " -> Show clock recovery state [pw_index]",                   {(Y_MnuLeafP) CLI_F_ClockRecoveryShowState}},
   {K_Leaf, TRUE, " -> Enable clock recovery interface [pw_index]",             {(Y_MnuLeafP) CLI_F_ClockRecoveryOutputEnable}},
   {K_Leaf, TRUE, " -> Disable clock recovery interface [pw_index]",            {(Y_MnuLeafP) CLI_F_ClockRecoveryOutputDisable}},
#if defined __WT_UFE412__ || defined __WT_UFE448__
   {K_Leaf, TRUE, " -> Clock Recovery statistics [pw_index]",             		{(Y_MnuLeafP) CLI_F_ClockRecStatistics}},
   {K_Leaf, TRUE, " -> Clock Recovery statistics reset [pw_index]",     		{(Y_MnuLeafP) CLI_F_ClockRecStatisticsReset}},
#else
   {K_Leaf, TRUE, " -> Clock Recovery statistics [cr table index]",             {(Y_MnuLeafP) CLI_F_ClockRecStatistics}},
   {K_Leaf, TRUE, " -> Clock Recovery statistics reset [cr table index]",       {(Y_MnuLeafP) CLI_F_ClockRecStatisticsReset}},
#endif
   {K_Leaf, TRUE, " -> Clock Recovery Systems statistics Status",               {(Y_MnuLeafP) CLI_F_ClockRecSystemsStatisticsStatus}},
   {K_Leaf, TRUE, " -> Clock Recovery Systems statistics Reset",                {(Y_MnuLeafP) CLI_F_ClockRecSystemsStatisticsReset}},

   {K_Leaf, TRUE, " -> Clock Recovery Command [pw_index, command (0-reset, 1-force freerun)]", {(Y_MnuLeafP) CLI_F_ClockRecCommand}},
   {K_Menu, TRUE, " -> Clock Recovery Indirect Memory Display",                 {(Y_MenuEntry*) V_ClockRecoveryIndMemDisplay}},
   {K_Leaf, TRUE, " -> Clock Recovery Calculate DCO parameters [service clock, diff_ref clock, ppm_offset]", {(Y_MnuLeafP) CLI_F_ClockRecDcoDivisorCalc}},
   {K_Leaf, TRUE, " -> Clock Recovery RX Phy reconfigure [pw_index, enable CR '0' (0-disable, 1-enable)]",           {(Y_MnuLeafP) CLI_F_ClockRecoveryPhyReconfigure}},
#ifdef WP_HW_WINPATH3
   {K_Leaf, TRUE, " -> Configure WDS3 board pins in FPGA [config (1-default, 2-ptp, 3-SyncE), board_number (1,2)]", {(Y_MnuLeafP) CLI_F_BoardPinConfig}},
   {K_Leaf, TRUE, " -> Clock Recovery DCO HW Registers Display [DCO id]",       {(Y_MnuLeafP) CLI_F_ClockRecDcoHwRegistersDisplay}},
   {K_Leaf, TRUE, " -> Global Clock Recovery State [optional: iteration (1-10)]", {(Y_MnuLeafP) CLI_F_GlobalClockRecoveryShowState}},
#if defined __WT_UFE412__ || defined __WT_UFE448__
   {K_Leaf, TRUE, " -> Clock Recovery PPB display [optional: ppb_margins display_all]",  {(Y_MnuLeafP) CLI_F_ClockRecStatisticsPPB}},
   {K_Leaf, TRUE, " -> Clock Recovery NAN display",  {(Y_MnuLeafP) CLI_F_ClockRecStatisticsNAN}},

   //{K_Leaf, TRUE, " -> Clock Recovery Convergence Check",  {(Y_MnuLeafP) CLI_F_ClockRecoveryConvergenceState}},
   {K_Leaf, TRUE, " -> Set timestamp stub (dedicated synthesis required) [1-enable 0-disable]",  {(Y_MnuLeafP) CLI_F_ClockRecTimestampStub}},
#endif /*  __WT_UFE412__ || __WT_UFE448__*/
   {K_Leaf, TRUE, " -> Set Filter factors [pw alpha gama]",  {(Y_MnuLeafP) CLI_F_Psn2TdmCesopFlowAggModifyFilterFactors }},
   {K_Leaf, TRUE, " -> Print Debug [pw seconds]",  {(Y_MnuLeafP) CLI_F_ClockRecDebugPrint }},
#endif /* WP_HW_WINPATH3 */



};

const Y_MenuEntry V_ClockRecoveryIndMemDisplay []=
{
#if  __WT_UFE3__
   {4 ,TRUE, "Clock Recovery Indirect Memory Display",                                          {(Y_MenuEntry*) V_ClockRecoveryIndMemDisplay}},
#else
   {3 ,TRUE, "Clock Recovery Indirect Memory Display",                                          {(Y_MenuEntry*) V_ClockRecoveryIndMemDisplay}},
#endif
   {K_Leaf, TRUE, " -> CGEN Indirect Memory Display [entry (0-31/335)]",                        {(Y_MnuLeafP) CLI_F_ClockRecIndirectMemoryReadCgen}},
   {K_Leaf, TRUE, " -> RX TS PORT Indirect Memory Display [entry (0-255), CR I/F (0-31/335)]",  {(Y_MnuLeafP) CLI_F_ClockRecIndirectMemoryReadRxTsPort}},
   {K_Leaf, TRUE, " -> RX TS MASK Indirect Memory Display [entry (0-255), CR I/F (0-31/335)]",  {(Y_MnuLeafP) CLI_F_ClockRecIndirectMemoryReadRxTsMask}},
#if __WT_UFE3__
   {K_Leaf, TRUE, " -> Clock Translator Indirect Memory Display [entry (0-335)]",                            {(Y_MnuLeafP) CLI_F_ClockRecIndirectMemoryReadCt}},
#endif
};

#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */


const Y_MenuEntry V_DeviceStatistics []=
{
   {
#if WTI_CESOP_TDI
      2
#else
      3
#endif
      ,TRUE, "Device Statistics",                                        {(Y_MenuEntry*) V_Statistics}},
#if (WTI_CESOP_MPLS_IW && (!WTI_CESOP_MPLS_OVER_ENET))
   {K_Leaf, TRUE, " -> Hspi Device Statistics",                           {(Y_MnuLeafP)   CLI_F_HspiDeviceStatistics}},
#else
#if (WTI_4_ENET_DEVICES)
   {K_Leaf, TRUE, " -> Enet Device Statistics [1- First ENET port,2- Second ENET port, 3- Third ENET port,4- Fourth ENET port]",        {(Y_MnuLeafP)   CLI_F_EnetDeviceStatistics}},
#elif (WTI_2_ENET_DEVICES)
   {K_Leaf, TRUE, " -> Enet Device Statistics [1- First ENET port,2- Second ENET port]",        {(Y_MnuLeafP)   CLI_F_EnetDeviceStatistics}},
#else
   {K_Leaf, TRUE, " -> Enet Device Statistics",                           {(Y_MnuLeafP)   CLI_F_EnetDeviceStatistics}},
#endif
#endif
#if WTI_CESOP_TDI
   {K_Leaf, TRUE, " -> TDI Transparent Device Statistics [device_num]",   {(Y_MnuLeafP)   CLI_F_TdiTransDeviceStatistics}},
#else
   {K_Leaf, TRUE, " -> EMPHY Port Statistics ", {(Y_MnuLeafP)   CLI_F_EmphyPortStatistics}},
   {K_Leaf, TRUE, " -> EMPHY Transparent Device Statistics [device_num]", {(Y_MnuLeafP)   CLI_F_EmphyTransDeviceStatistics}},
#endif
};

const Y_MenuEntry V_ChannelStatistics []=
{
   { 4 ,TRUE, "Channel Statistics",                                       {(Y_MenuEntry*) V_Statistics}},
   {K_Leaf, TRUE, " -> PWE3 Rx Channel statistics [pw_index]",            {(Y_MnuLeafP)   CLI_F_Pwe3RxChannelStatistics}},
   {K_Leaf, TRUE, " -> PWE3 Tx Channel statistics [pw_index]",            {(Y_MnuLeafP)   CLI_F_Pwe3TxChannelStatistics}},
   {K_Leaf, TRUE, " -> Clear PWE3 Rx Channel statistics [pw_index]",      {(Y_MnuLeafP)   CLI_F_Pwe3RxChannelStatisticsClear}},
   {K_Leaf, TRUE, " -> Clear PWE3 Tx Channel statistics [pw_index]",      {(Y_MnuLeafP)   CLI_F_Pwe3TxChannelStatisticsClear}},
};

const Y_MenuEntry V_FlowAggStatistics []=
{
   {
#if WTI_CESOP_TO_MPLS_FA_CASCADING
      4,
#else /* !WTI_CESOP_TO_MPLS_FA_CASCADING */
      2,
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */
      TRUE, "Flow Aggregation Statistics",                                                        {(Y_MenuEntry*) V_Statistics}},
   {K_Leaf, TRUE, " -> TDM to PSN Flow Aggregation statistics [flowagg_num, type(0-DATA,1-CAS)]", {(Y_MnuLeafP)   CLI_F_Tdm2PsnFlowAggStatistics}},
   {K_Leaf, TRUE, " -> PSN to TDM Flow Aggregation statistics [flowagg_num]",                     {(Y_MnuLeafP)   CLI_F_Psn2TdmFlowAggStatistics}},
#if WTI_CESOP_TO_MPLS_FA_CASCADING
   {K_Leaf, TRUE, " -> L2 ROUTER MPLS Flow Aggregation statistics [flowagg_num, dir(0-tdm2psn, 1-psn2tdm)]",      {(Y_MnuLeafP)   CLI_F_Tdm2PsnMplsL2RouterFlowAggStatistics}},
   {K_Leaf, TRUE, " -> Clear L2 ROUTER MPLS Flow Aggregation statistics [flowagg_num, dir(0-tdm2psn, 1-psn2tdm)]",{(Y_MnuLeafP)   CLI_F_Tdm2PsnMplsL2RouterFlowAggStatisticsClear}},
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */
};

/*****************************************************************************/
/*****************************************************************************/
/*                         MENU API FUNCTIONS                                */
/*****************************************************************************/
/*****************************************************************************/

/***************************************************************
 * Func name  : F_MenuEngine
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void F_MenuEngine(char* InputBuf)
{
   /* Is HotKey? -> Evaluate HotKey */
   if (F_EvaluateHotKey(InputBuf) == OK)
   {
      return;
   }
   else
   {
      /* Evaluate Menu or Procedure Command */
      F_EvaluateCommand(InputBuf);
   }
} /* F_MenuEngine */

/***************************************************************
 * Func name  : F_EvaluateHotKey
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
int F_EvaluateHotKey(char *InputBuf)
{
   switch (InputBuf[0])
   {
   case '!':   /* Goto main menu  */
      F_SetMainMenu();
      F_DisplayMenu();
      break;
   case '^':   /* Goto upper menu */
      F_SetUpperMenu();
      F_DisplayMenu();
      break;
   case '=':   /* Display current menu again */
      F_DisplayMenu();
      break;
   default:
      return  ERROR;
   }
   return OK;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void F_EvaluateCommand(char *InputBuf)
{
  int          RetVal, Cmd;
  Y_MenuEntry  *MenuEntryP;

  /* Is chosen command is in tolerance */
  Cmd = atoi(InputBuf);
  RetVal = 1;

  if(RetVal < 1)
  {
     printf("STR_MENU_ERR\n");
     return;
  }

  if ((Cmd > (int)V_CurrMenuP->Type) || (Cmd < 1))
  {
     printf("%s\n", STR_MENU_ERR);
     return;
  }

  MenuEntryP = V_CurrMenuP+Cmd;

  /* Is chosen command is visible */
  if (MenuEntryP->Visible != TRUE)
  {
     printf("%s\n", STR_MENU_ERR);
     return;
  }

  /* Is chosen command is menu or procedure */
  if (MenuEntryP->Type == K_Leaf)
  {
    (MenuEntryP->Node.Leaf) (InputBuf);
  }
  else if ((MenuEntryP->Type == K_Menu))
  {
     V_CurrMenuP = (Y_MenuEntry*)MenuEntryP->Node.Menu;
     F_DisplayMenu();
  }

} /* F_EvaluateCommand */

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void F_SetMainMenu    (void)
{

V_CurrMenuP = (Y_MenuEntry*)&V_MainMenu[0];

} /* F_SetMainMenu */

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void F_SetUpperMenu   (void)
{

V_CurrMenuP = (Y_MenuEntry *)V_CurrMenuP->Node.Menu;

} /* F_SetUpperMenu */

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void F_DisplayMenu    (void)
{
   int Counter;
   Y_MenuEntry *CurrMenuEntryP;

   F_DisplayHeader();
   printf("  Name: %s                                              \n",
          V_CurrMenuP->Desc);
   printf("%s\n", STR_STAR);
   CurrMenuEntryP = V_CurrMenuP;

   /* Skip to the 1st menu Item */
   CurrMenuEntryP++;

   for (Counter = 1;
        Counter <= (int)V_CurrMenuP->Type;
        Counter++, CurrMenuEntryP++)
   {
      if (CurrMenuEntryP->Visible == TRUE)
      {
         printf("%d. %s\n", Counter, CurrMenuEntryP->Desc);
      }
      else
      {
         printf("%d. This option is not visible\n", Counter);
      }
   }

#ifdef AUTOMATION_MODE
   if (default_setup==0)
   {     
      ++default_setup;
#if defined (VXWORKS) || defined (__linux__)
      printf("\ninitialize and open the socket\n");
      WT_initSocketConnection(9333);
      printf("\nwaiting for connection\n");
      WT_acceptConnections();
#else
      printf("\ninitialize and open the socket\n");
      WT_WinmoninitSocketConnection();
#endif
   } 
#endif
   
} /* F_DisplayMenu */

/***************************************************************
 * Func name  : F_DisplayHeader
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void F_DisplayHeader(void)
{
   printf("%s\n", STR_STAR);
   printf("%s\n", STR_MENU_HDR);
   printf("%s\n", STR_HOT_KEYS);

} /* F_DisplayHeader */

/*****************************************************************************/
/*                       Demo Global Parameters                              */
/*****************************************************************************/

WTI_enet_header enet_header;
#if WTI_CESOP_MPLS_OVER_ENET
WTI_enet_header mpls_enet_header;
#endif /* WTI_CESOP_MPLS_OVER_ENET */
#if WTI_CESOP_MEF8_ENCAP
WP_U32 ecid_header;
#endif /*WTI_CESOP_MEF8_ENCAP */
WTI_ip_header ip_header;
WTI_udp_header udp_header;
WTI_rtp_header rtp_header;
WTI_control_word control_word;
WP_boolean *reg_results;


void WTI_Terminate(WP_U32 status)
{
   /* Release WDDI */
   WTI_SystemRelease();
   WP_DriverRelease();

printf("%s [ %d ]\n", __FUNCTION__, __LINE__);

   exit(1);
}

/*****************************************************************************
 * Function name: WTI_TerminateOnError
 * Description  : display error messege and exit
 * Input  params: handle - handle/status to be checked for error
 *                s - error messege
 *                line_num - line number to be displayed
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_TerminateOnError(WP_handle handle, WP_CHAR *s, WP_U32 line_num)
{
   if (WP_ERROR_P(handle))
   {
      printf("%s: Terminal error (handle 0x%x) %s\n", appname, handle, s);
      printf("%s: Error type - %s, Line:%d\n", appname, WP_error_name[WP_ERROR(handle)],line_num);
      exit (1);
      /* release CESOP system */
//    if(the_system->in_termination == WP_FALSE)
//    {
//       the_system->in_termination = WP_TRUE;
//       CLI_F_Quit_No_Reset("");
//    }
//    else
//    {
//       exit(1);
//    }
   }
#if WTI_DEBUG_LEVEL > 1
   else
   {
      printf("%s: Handle/Status returned from %s at line %d is 0x%x \n", appname, s, line_num, handle);
   }
#endif
}


/*****************************************************************************
 * Function name: WTI_TerminateOnError
 * Description  : display error messege and exit
 * Input  params: handle - handle/status to be checked for error
 *                s - error messege
 *                line_num - line number to be displayed
 * Output params:
 * Return val   : none
 *****************************************************************************/
WP_U8 WTI_PrintOnError(WP_handle handle, WP_CHAR *s, WP_U32 line_num)
{
   if (WP_ERROR_P(handle))
   {
      printf("%s: Terminal error (handle 0x%x) %s\n", appname, handle, s);
      printf("%s: Error type - %s, Line:%d\n", appname, WP_error_name[WP_ERROR(handle)],line_num);
      return 1;
   }
   else
   {
      return 0;
   }

}


/*****************************************************************************
 * Function name: WTI_TerminatePrintError
 * Description  : display error messege
 * Input  params: s - error messege
 *                line_num - line number to be displayed
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_TerminatePrintError(WP_CHAR *s, WP_U32 LineNum)
{
   printf("%s, Line:%d\n", s, LineNum);
#if A_AUTOMATION
   WT_PrintTestInfo(WT_FAIL);
#endif

   return;
}

/*****************************************************************************
 * Function name: WTI_TerminatePrintError
 * Description  : display error messege
 * Input  params: s - error messege
 *                line_num - line number to be displayed
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_PrintError(WP_CHAR *s, WP_CHAR *filename, WP_U32 LineNum)
{
   printf("%s, File: %s Line:%d\n", s, filename, LineNum);
#if A_AUTOMATION
   WT_PrintTestInfo(WT_FAIL);
#endif

   return;
}

/*****************************************************************************
 * Function name:
 * Description  :
 * Input  params:
 * Output params:
 * Return val   :
 *****************************************************************************/
void WTI_ClearHeaders(void)
{
   memset(&enet_header ,0,sizeof(WTI_enet_header));
   memset(&ip_header ,0,sizeof(WTI_ip_header));
   memset(&udp_header ,0,sizeof(WTI_udp_header));
   memset(&rtp_header ,0,sizeof(WTI_rtp_header));
   memset(&control_word ,0,sizeof(WTI_control_word));
}

/*****************************************************************************
 * Function name:
 * Description  :
 * Input  params:
 * Output params:
 * Return val   :
 *****************************************************************************/
void WTI_DemoConfigStart(void)
{
   /* create an empty CESOP system with no PW */
   WTI_CesopSystemStart();

   WTI_ClearHeaders();
   pw_config.enet_header = (WP_U8 *)&enet_header;
   pw_config.ip_header = &ip_header;
   pw_config.udp_header = (WP_U8 *)&udp_header;
   pw_config.rtp_header = (WP_U8 *)&rtp_header;
   pw_config.control_word = (WP_U8 *)&control_word;
}

/*****************************************************************************/
/*                       Demo Functions Section                              */
/*****************************************************************************/

#if 0
static WP_U32 *history = (WP_U32 *) 0x3ff00000;

void reset_the_record(void)
{
   history[0] = (WP_U32) &history[4];
   history[1] = 0x40000000;
   history[2] = 0;
   history[3] = 0;
   history[4] = 0xdeadbeef;
}

static WP_boolean initialize_record_area = WP_TRUE;

void record(WP_U32 a, WP_U32 b, WP_U32 c, WP_U32 d)
{
   WP_U32 *start;

   if (initialize_record_area == WP_TRUE)
   {
      reset_the_record();
      initialize_record_area = WP_FALSE;
   }

   start = (WP_U32 *) history[0];
   *start++ = a;
   *start++ = b;
   *start++ = c;
   *start++ = d;

   if (start == (WP_U32 *) history[1])
   {
      start = &history[4];
      ++history[2];
   }

   history[0] = (WP_U32) start;
   *start = 0xdeadbeef;
}
#endif

#define convert_to_char(n)  (((n) < 10) ? ((n) + '0') : ((n) + 'a' - 10))

static char *hextoa(int n, char *p)
{
   char d[8];
   int j;

   for (j = 7; j >= 0; --j)
   {
      d[j] = n & 0xf;

      n >>= 4;
   }

   for (j = 0; j <= 7; ++j)
      *p++ = convert_to_char(d[j]);

   return p;
}

void hexdump(void *arg, int nBytes)
{
   char buffer[64];
   int j, k;
   char *p = NULL;

   int val;
   int *addr = (int *) arg;

   if ((((int) addr) & 3) != 0)
   {
      val = ((int) addr) & ~3;
      nBytes += ((int) addr) - val;
      addr = (int *) val;
   }

   for (j = 0; j < sizeof(buffer); ++j)
      buffer[j] = ' ';

   buffer[61] = '\r';
   buffer[62] = '\n';

   buffer[63] = '\0';

   buffer[0] = '0';
   buffer[1] = 'x';
   buffer[10] = ':';
   for (j = 13; j <= 50; j += 12)
   {
      buffer[j] = '0';
      buffer[j + 1] = 'x';
   }

   for (j = 0, k = 0; j < nBytes; j += sizeof(int), ++addr)
   {
      if (k == 0)
      {
         p = &buffer[2];
         p = hextoa((int) addr, p);
         ++p;  /* Step over the ':' */
      }

      val = *addr;
      p += 4;   /* Step over the 2 blanks and the 0x */
      p = hextoa(val, p);

      if ((++k) == 4)
      {
         printf("%s", buffer);
         k = 0;

      }
   }

   if (k)
      printf("%s", buffer);

}

#if 0
void stall(WP_CHAR *f, WP_U32 line, WP_boolean suppress)
{
   WP_U16 global_int_reg;

   global_int_reg = *(WP_U16 *)0x1f230010;
   printf("global_int_reg = 0x%x\n", global_int_reg);

   printf("-----> %s [ %d ]   --------->> Any key to continue: ", f, line);
   getchar();

   if (!suppress)
   {
      //global_int_reg = WUFEI_UfeCoreGlobalIntGet(0);
      global_int_reg = *(WP_U16 *)0x1f230010;
      printf("global_int_reg = 0x%x\n", global_int_reg);
   }
}
#endif

/*****************************************************************************
 * Function name:
 * Description  :
 * Input  params:
 * Output params:
 * Return val   :
 *****************************************************************************/
WP_S32 main(WP_S32 argc, WP_CHAR **argv)
{

   /* init the Demo structures */
   WTI_InitDemoStructures();

   CLI_F_301_TDI_E1_U_CR_NoCas_Two_Boards_N_Line_N_PW ("3 0");
   /* Start Menu Engine */
   CLI_T_Main();

   return 0;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
int CLI_T_Main(void)
{
   char InputBuf[256];

   /* Display main menu */
   F_DisplayMenu();

   while (TRUE)
   {
      printf("%s", STR_ENTER_COMMAND);
      InputBuf[0] = '\0';
#ifdef  __linux__
loop:
      fgets(InputBuf,256,stdin);
      if (InputBuf[0] == '\0')
         goto loop;
#else
#ifdef WP_HW_WINPATH3
      flush();
#endif
loop:
      gets(InputBuf);
      if (InputBuf[0] == '\0')
         goto loop;
#endif
      printf("\n");
      F_MenuEngine(InputBuf);
   }

   return 0;
} /* CLI_T_Main */

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void Vt100Color(int foreground, int background)
{
   printf(
      "\033[%d;%dm",
      FOREGROUND_COLOR_BASE + foreground,
      BACKGROUND_COLOR_BASE + background);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void Vt100Normal(void)
{
   printf("\033[0m");
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void Vt100GotoXy(int x, int y)
{
   printf("\033[%d;%dH",y, x);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void Vt100EraseEOL(void)
{
   printf("\033[K");
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void Vt100ClearScreen(void)
{
   printf("\033[2J");
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/

void CLI_F_RunDefSetup(char *StrPrm)
{
#if WTI_CESOP_TDI
   CLI_F_TdiConfigInit("");
#endif
   WTI_SystemAlloc();
   WTI_DemoConfigStart();
}

#if (!WTI_CESOP_TDI)
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_UfeShowInfo(char *StrPrm)
{
   WP_U32 i;

   printf("\nUFE INFO:\n");
   printf("---------\n\n");
   printf("winpath_id: %d ufe_id: %d\n",
          the_system->ufe.wpid,the_system->ufe.ufe_id);
   printf("upi_id: ");
   if (the_system->ufe.upi_id)
      printf("WP_UPI_2\n");
   else
      printf("WP_UPI_1\n");
   printf("fpga_mode: ");
   if (the_system->ufe.fpga_mode == WT_SBI)
   {
      printf("UFE FPGA mode: WT_SBI\n");
      for (i=0;i<WTI_MAX_SPE_PER_UFE;++i)
      {
         printf("\tsbi_spe_line_type[%d]->",i);
         switch (the_system->ufe.sbi_spe_line_type[i])
         {
            case (WUFE_SBI_TYPE_T1):
               printf("WUFE_SBI_TYPE_T1\n");
               break;
            case (WUFE_SBI_TYPE_E1):
               printf("WUFE_SBI_TYPE_E1\n");
               break;
            case (WUFE_SBI_TYPE_IDLE):
               printf("WUFE_SBI_TYPE_IDLE\n");
               break;
            default:
               break;
         }
      }

#ifdef __WT_UFE3__
      printf("\tmax_sbi_clock_translators: %d\n",
             the_system->ufe.max_sbi_clock_translators);
#endif /* WT_UFE3 */
   }
   else
   {
      printf("UFE FPGA mode: WT_OCTAL\n");
   }

   printf("max_lines: %d\n", the_system->ufe.max_lines);
   printf("max_phys: %d\n", the_system->ufe.max_phys);
   printf("tx_clock_mode: %s\n",
          (the_system->ufe.tx_clock_mode)?"WT_RX_LOOPTIME_CLOCK":"WT_EXT_CLOCK");
   printf("clock_recovery_mode: %s\n",
          (the_system->ufe.system_clock_rec_enable)?"ENABLED":"DISABLED");

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_UfeDisplayEvents(char *StrPrm)
{
   WT_UfeReadEvents(&the_system->ufe);
   return;
}

#if (!WTI_CESOP_TDI)
/* -------------------------------------------------------------------- */
/* ---------------                                      --------------- */
/* ---------------     Temperature Sensor Functions     --------------- */
/* ---------------                                      --------------- */
/* -------------------------------------------------------------------- */
void WT_UfeMemRead(WP_U32 offset, WP_U16 *value)
{
   WP_U32 addr = 0;
   WP_U16 temp_data = 0;
   addr = (offset << UFE_MEM_PORT_SIZE) + WTI_UFE_VIR_BASE;
   temp_data  =  *((WP_U16 volatile *)(addr));
   *value = (WP_U16) temp_data;
}


void WT_UfeMemWrite(WP_U32 offset,  WP_U16 value)
{
   WP_U32 addr = 0;
   addr = (offset << UFE_MEM_PORT_SIZE) + WTI_UFE_VIR_BASE;
   *((WP_U16 volatile *)(addr)) =  value;
}

void WT_IssueOneShot(void)
{
   WP_U16 busy;
   WP_U16 Status;

   /* -------------------------------------- */

   WT_UfeMemWrite(0x7839,0x4c0f); /* 0x4c:Slave Address ; 0x0f:One-Shot Command */
   WT_UfeMemWrite(0x783b,0x0002); /* Send Byte Format                           */


   /* Wait For The Transaction To Finish */
   busy = 0x8000;
   while (busy > 0)
   {
      WP_Delay(5000);

      /* Read Command Register */

      WT_UfeMemRead(0x783b,&busy);
      busy = (busy & 0x8000);
   }

   /* ------------------------ */
   /* Check Sensor Status Byte */
   /* ------------------------ */
   Status = 0x8000;
   while (Status > 0)
   {
      /* Read Status Byte */
      WT_UfeMemWrite(0x7839,0x4c02); /*  0x4c:Slave Address ; 0x02:Read Status Byte              */
      WT_UfeMemWrite(0x783a,0x0000);
      WT_UfeMemWrite(0x783b,0x0001);  /*  Read Byte Format                                       */

      /* Wait For The Transaction To Finish */
      busy = 0x8000;
      while (busy>0)
      {
         WP_Delay(5000);

         /* Read Command Register */
         WT_UfeMemRead(0x783b,&busy);
         busy = (busy & 0x8000);
      }

      /* Check Status */
      WT_UfeMemRead(0x783a,&Status);
      Status = (Status & 0x8000);
   }

} /* WT_IssueOneShot()*/



WP_U16 WT_ReadRemoteTemp(void)
{

   WP_U16 busy;
   WP_U16 Temperature;
   WP_U16 Temperature_Extended;

   /* -------------------------------------- */

   /* Read Remote Temperature */
   WT_UfeMemWrite(0x7839,0x4c01); /*  0x4c:Slave Address ; 0x01:Read Remote Temperature */
   WT_UfeMemWrite(0x783a,0x0000);
   WT_UfeMemWrite(0x783b,0x0001);  /*  Read Byte Format                                  */


   /* Wait For The Transaction To Finish */
   busy = 0x8000;
   while (busy > 0)
   {
      WP_Delay(5000);

      /* Read Command Register */
      WT_UfeMemRead(0x783b,&busy);
      busy = (busy & 0x8000);
   }

   WT_UfeMemRead(0x783a,&Temperature);
   Temperature = (Temperature & 0xff00);


   /* Read Remote Extended Temperature */
   WT_UfeMemWrite(0x7839,0x4c10); /* 0x4c:Slave Address ; 0x01:Read Remote Extended Temperature */
   WT_UfeMemWrite(0x783a,0x0000);
   WT_UfeMemWrite(0x783b,0x0001); /* Read Byte Format                                           */

   /* Wait For The Transaction To Finish */
   busy = 0x8000;
   while (busy > 0)
   {
      WP_Delay(5000);

      /* Read Command Register */
      WT_UfeMemRead(0x783b,&busy);
      busy = (busy & 0x8000);
   }

   WT_UfeMemRead(0x783a,&Temperature_Extended);
   Temperature_Extended = ((Temperature_Extended >> 13) & 0x0007);

   Temperature = (Temperature | Temperature_Extended);

   return Temperature;

} /* WT_ReadRemoteTemp() */


void WT_AsciiWait(void)
{
   WP_U32 i;

   for (i=0 ; i<60 ; i=i+1)
   {
      printf ("-");
      WP_Delay(50000);
      WT_Vt100CrsrLeft(1);
      printf ("\\");
      WP_Delay(50000);
      WT_Vt100CrsrLeft(1);
      printf ("/");
      WP_Delay(50000);
      WT_Vt100CrsrLeft(1);

      if (WT_SampleSerial())
         break;
   }

} /* WT_AsciiWait() */

#endif  /* (!WTI_CESOP_TDI)  */

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
static void CLI_F_Check_UFE_Temperature(void)
{
   WP_U16 busy;
   WP_U16 Temperature;
   WP_U16 TemperatureExtended;
   WP_U16 MaxTemperature;
   WP_U16 MaxTemperatureExtended;

   /* ------------------------------------------------------- */

   /* Check If We We Already Put The Sensor Is Standby Mode */
   if (Temperature_Sensor_Standby_Mode == 0)
   {
      /* Write Configuration Register with 0x40 */
      WT_UfeMemWrite(0x7839,0x4c09); /*  0x4c:Slave Address ; 0x09:Write Config Register */
      WT_UfeMemWrite(0x783a,0x0040);
      WT_UfeMemWrite(0x783b,0x0000); /*  Write Byte Format                               */


      busy = 0x8000;
      while (busy>0)
      {
         WP_Delay(5000);

         /* Read Command Register */
         WT_UfeMemRead(0x783b,&busy);
         busy = (busy & 0x8000);
      }

      /* Set Standby Mode Locally */
      Temperature_Sensor_Standby_Mode = 1;

      printf ("Temperature Sensor Standby Mode Was Set!\n");
   }


   MaxTemperature = 0;
   MaxTemperatureExtended = 0;

   while (WP_TRUE)
   {
      /* Issue One-Shot */
      WT_IssueOneShot();

      /* Read Remote Temperature */
      Temperature = WT_ReadRemoteTemp();
      TemperatureExtended = Temperature & 0x0007;
      TemperatureExtended = (TemperatureExtended * 125);
      Temperature = ((Temperature >> 8) & 0xff);

      if (Temperature > MaxTemperature)
      {
         MaxTemperature = Temperature;
         MaxTemperatureExtended = TemperatureExtended;
      }
      else if (Temperature == MaxTemperature)
      {
         if (TemperatureExtended > MaxTemperatureExtended)
         {
            MaxTemperatureExtended = TemperatureExtended;
         }
      }

      printf ("Remote Temperature:%d.%03d, Max:%d.%03d\n",
              Temperature,TemperatureExtended,
              MaxTemperature,MaxTemperatureExtended);

      /* Wait */
      WT_AsciiWait();

      if (WTI_SampleSerial())
         break;
   }

   printf ("\n");
}

#ifdef __WT_UFE3__
static void WTI_Check_UFE_Temperature_4Automation(char *StrPrm)
{
   WP_U16 busy;
   WP_U16 Temperature;
   WP_U16 TemperatureExtended;
   WP_U16 MaxTemperature;
   WP_U16 MaxTemperatureExtended;
   WP_U32 res, cmd, iterations,i;

   res = sscanf(StrPrm, "%d %d", &cmd, &iterations);
   if (res != 2 && res != 1)
      WTI_TerminatePrintError("WTI_Check_UFE_Temperature_4Automation Invalid number of parameters", __LINE__);

   else if (res == 2)
   {
      if( (iterations < 1) || (iterations > 10) )
         WTI_TerminatePrintError("WTI_Check_UFE_Temperature_4Automation Invalid number of Iterations\n iterations is >0 and <10", __LINE__);
   }
   else if (res ==1)
      iterations = 2;


   /* ------------------------------------------------------- */

   /* Check If We We Already Put The Sensor Is Standby Mode */
   if (Temperature_Sensor_Standby_Mode == 0)
   {
      /* Write Configuration Register with 0x40 */
      WT_UfeMemWrite(0x7839,0x4c09); /*  0x4c:Slave Address ; 0x09:Write Config Register */
      WT_UfeMemWrite(0x783a,0x0040);
      WT_UfeMemWrite(0x783b,0x0000); /*  Write Byte Format                               */


      busy = 0x8000;
      while (busy>0)
      {
         WP_Delay(5000);

         /* Read Command Register */
         WT_UfeMemRead(0x783b,&busy);
         busy = (busy & 0x8000);
      }

      /* Set Standby Mode Locally */
      Temperature_Sensor_Standby_Mode = 1;

      printf ("Temperature Sensor Standby Mode Was Set!\n");
   }


   MaxTemperature = 0;
   MaxTemperatureExtended = 0;

   for (i=0; i<iterations; i++)
   {
      /* Issue One-Shot */
      WT_IssueOneShot();

      /* Read Remote Temperature */
      Temperature = WT_ReadRemoteTemp();
      TemperatureExtended = Temperature & 0x0007;
      TemperatureExtended = (TemperatureExtended * 125);
      Temperature = ((Temperature >> 8) & 0xff);

      if (Temperature > MaxTemperature)
      {
         MaxTemperature = Temperature;
         MaxTemperatureExtended = TemperatureExtended;
      }
      else if (Temperature == MaxTemperature)
      {
         if (TemperatureExtended > MaxTemperatureExtended)
         {
            MaxTemperatureExtended = TemperatureExtended;
         }
      }

      printf ("Remote Temperature:%d.%03d, Max:%d.%03d\n",
              Temperature,TemperatureExtended,
              MaxTemperature,MaxTemperatureExtended);

      WP_Delay(1000000);
   }

   if (MaxTemperature >= 85)
      WTI_PRINT_ERROR(("\n Remote Temperature Is Over 85 !!! \n"));

   printf ("\n");
}
#endif

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_UfeUpiPort(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != WP_PORT_UPI1 && val != WP_PORT_UPI2)
   {
      WTI_TerminatePrintError("CLI_F_UfeUpiPort", __LINE__);
   }

   the_system->ufe.upi_id = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_UfeFpgaMode(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
#ifdef __WT_UFE3__
   if (val != 1 && val != 2)
   {
      WTI_TerminatePrintError("CLI_F_UfeFpgaMode", __LINE__);
      return;
   }

   the_system->ufe.fpga_mode = (val == 1) ? WT_OCTAL : WT_SBI;
#if WTI_DUAL_EMPHY
   the_system->second_ufe.fpga_mode = (val == 1) ? WT_OCTAL : WT_SBI;
#endif

   if (the_system->ufe.fpga_mode == WT_SBI)
   {
      int i;

      for (i=0; i<WUFE_MAX_N_SBI_SPE; i++)
      {
         the_system->ufe.sbi_spe_line_type[i] = WUFE_SBI_TYPE_IDLE;
#if WTI_DUAL_EMPHY
         the_system->second_ufe.sbi_spe_line_type[i] = WUFE_SBI_TYPE_IDLE;
#endif
      }
   }
#else
   if (val != 0 && val != 1 && val != 2)
   {
      WTI_TerminatePrintError("CLI_F_UfeFpgaMode", __LINE__);
      return;
   }

   if (val == 0)
   {
      the_system->ufe.fpga_mode = WT_SDH;
#if WTI_DUAL_EMPHY
      the_system->second_ufe.fpga_mode = WT_SDH;
#endif
   }
   else if (val == 1)
   {
      the_system->ufe.fpga_mode = WT_SONET;
#if WTI_DUAL_EMPHY
      the_system->second_ufe.fpga_mode = WT_SONET;
#endif
   }
   else
   {
      the_system->ufe.fpga_mode = WT_OCTAL;
#if WTI_DUAL_EMPHY
      the_system->second_ufe.fpga_mode = WT_OCTAL;
#endif
   }

#endif
   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_UfeTemuxSetup(char *StrPrm)
{
   WP_U32 res, cmd;
   WP_U32 type, mode;

   res = sscanf(StrPrm, "%d %x", &cmd, &type);
   if (res != 2)
   {
      WTI_TerminatePrintError("ERROR: Invalid number of parameters", __LINE__);
      return;
   }

   if (type >= 10)
   {
      WTI_TerminatePrintError("ERROR: Transfer type is incorrect", __LINE__);
      return;
   }

   mode = 0;

   if(WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)
      mode |= 0x2;

   /* define CT clock for the Temux */
   if(WTI_CESOP_REGRESSION_TEST)
      mode |= 0x1;

   WP_Delay(300);
#ifdef __WT_UFE3__
   wti_configure->pTemuxSetup(mode, type);
#endif
   WP_Delay(300);

   the_system->ufe.temux_used = 0;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_UfeTransferMode(char *StrPrm)
{
   WP_U32 res, cmd, val, e1t1_index, clock_rec_mode;
   WP_U32 transfer_type=0, framing_mode=0;
   WT_ufe_line_params *line_params;

#ifdef __WT_UFE3__
   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &e1t1_index, &val, &clock_rec_mode);

   if (val > 8)
   {
      WTI_TerminatePrintError("CLI_F_UfeTransferMode", __LINE__);
      return;
   }
#else
   val = 0;
   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &e1t1_index, &transfer_type, &framing_mode);
   clock_rec_mode = 0; /* In used only in UFE3 */
#endif

   if (res != 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)
   if (clock_rec_mode > 3)
   {
      WTI_TerminatePrintError("CLI_F_UfeTransferMode", __LINE__);
      return;
   }
#else
   if (clock_rec_mode != 0 &&
       clock_rec_mode != 2)
   {
      WTI_TerminatePrintError("CLI_F_UfeTransferMode", __LINE__);
      return;
   }
#endif

   if (e1t1_index >= MAX_UFE_LINES_USED)
   {
      WTI_TerminatePrintError("CLI_F_UfeTransferMode", __LINE__);
      return;
   }

#if WTI_DUAL_EMPHY
   if (e1t1_index >= WTI_FIRST_UFE_NUM_LINES)
      line_params = &(the_system->second_ufe.line_params[e1t1_index-WTI_FIRST_UFE_NUM_LINES]);
   else
      line_params = &(the_system->ufe.line_params[e1t1_index]);
#else
   line_params = &(the_system->ufe.line_params[e1t1_index]);
#endif

   if (the_system->ufe.fpga_mode == WT_OCTAL)
   {
      if (e1t1_index >= WTI_MAX_OCTAL_LINES)
      {
         WTI_TerminatePrintError("CLI_F_UfeTransferMode", __LINE__);
         return;
      }

      switch (val)
      {
         case (1):
            line_params->transfer_type = WUFE_CAD_TYPE_T1;
            line_params->framed = WUFE_FRAMED;
            break;
         case (2):
            line_params->transfer_type = WUFE_CAD_TYPE_T1;
            line_params->framed = WUFE_UNFRAMED;
            break;
         case (3):
            line_params->transfer_type = WUFE_CAD_TYPE_E1;
            line_params->framed = WUFE_FRAMED;
            break;
         case (4):
            line_params->transfer_type = WUFE_CAD_TYPE_E1;
            line_params->framed = WUFE_UNFRAMED;
            break;
         case (5):
            line_params->transfer_type = WUFE_CAD_TYPE_DS3;
            line_params->framed = WUFE_FRAMED;
            break;
         case (6):
            line_params->transfer_type = WUFE_CAD_TYPE_DS3;
            line_params->framed = WUFE_UNFRAMED;
            break;
         case (7):
            line_params->transfer_type = WUFE_CAD_TYPE_E3;
            line_params->framed = WUFE_FRAMED;
            break;
         case (8):
            line_params->transfer_type = WUFE_CAD_TYPE_E3;
            line_params->framed = WUFE_UNFRAMED;
            break;
         default:
            break;
      }
   }
   else if (the_system->ufe.fpga_mode == WT_SBI)
   {
      int spe_id;
      if (e1t1_index >= WTI_MAX_LINES)
      {
         WTI_TerminatePrintError("CLI_F_UfeTransferMode", __LINE__);
         return;
      }

      spe_id = WTI_SpeIdGet(e1t1_index);
      if (spe_id == -1 || spe_id > (WUFE_MAX_N_SBI_SPE-1))
      {
         WTI_TerminatePrintError("CLI_F_UfeTransferMode", __LINE__);
         return;
      }
      switch (val)
      {

         case (1): /* T1 FRAMED */
         case (5):
            if (the_system->ufe.sbi_spe_line_type[spe_id] != WUFE_SBI_TYPE_T1 &&
                the_system->ufe.sbi_spe_line_type[spe_id] != WUFE_SBI_TYPE_DS3)
            {
               WTI_TerminatePrintError("CLI_F_UfeTransferMode", __LINE__);
               return;
            }
         case (2):
         case (6):
            if (the_system->ufe.sbi_spe_line_type[spe_id] != WUFE_SBI_TYPE_T1 &&
                the_system->ufe.sbi_spe_line_type[spe_id] != WUFE_SBI_TYPE_DS3)
            {
               WTI_TerminatePrintError("CLI_F_UfeTransferMode", __LINE__);
               return;
            }
            line_params->transfer_type = the_system->ufe.sbi_spe_line_type[spe_id];
            if (val == 1  || val == 5 )
            {
               line_params->framed = WUFE_FRAMED;
            }
            else
            {
               line_params->framed = WUFE_UNFRAMED;
            }
            break;

         case (3):
         case (7):
            if (the_system->ufe.sbi_spe_line_type[spe_id] != WUFE_SBI_TYPE_E1 &&
                the_system->ufe.sbi_spe_line_type[spe_id] != WUFE_SBI_TYPE_E3)
            {
               WTI_TerminatePrintError("CLI_F_UfeTransferMode", __LINE__);
               return;
            }
         case (4):
         case (8):
            if (the_system->ufe.sbi_spe_line_type[spe_id] != WUFE_SBI_TYPE_E1 &&
                the_system->ufe.sbi_spe_line_type[spe_id] != WUFE_SBI_TYPE_E3)
            {
               WTI_TerminatePrintError("CLI_F_UfeTransferMode", __LINE__);
               return;
            }
            line_params->transfer_type = the_system->ufe.sbi_spe_line_type[spe_id];
            if (val == 3 || val == 7)
            {
               line_params->framed = WUFE_FRAMED;
            }
            else
            {
               line_params->framed = WUFE_UNFRAMED;
            }
            break;
         default:
            break;
      }
   }
   else
   {
      line_params->transfer_type = transfer_type;
      line_params->framed = framing_mode;
   }

   line_params->valid = 1;
   line_params->clock_rec_mode = clock_rec_mode;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_UfeSystemClkRecMethodSet(char *StrPrm)
{
   WP_U32 res, cmd, clock_rec_method;

   res = sscanf(StrPrm, "%d %d", &cmd, &clock_rec_method);

#ifdef __WT_UFE3__
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (clock_rec_method != 0 &&
       clock_rec_method != 1)
   {
      WTI_TerminatePrintError("CLI_F_UfeSystemClkRecMethodSet", __LINE__);
      return;
   }

   if (clock_rec_method)
      the_system->ufe.clock_rec_method = WUFE_CLOCK_REC_METHOD_DIFFERENTIAL;
   else
      the_system->ufe.clock_rec_method = WUFE_CLOCK_REC_METHOD_ADAPTIVE;

#else
   printf("Warning! Do not use CLI_F_UfeSystemClkRecMethodSet in UFE4!\n");
#endif /* WT_UFE3 */
   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_UfeLineSbiClkTransSet(char *StrPrm)
{
   WP_U32 res, cmd, e1t1_index, clock_trans_id;
   WT_ufe_line_params *line_params;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &e1t1_index, &clock_trans_id);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (e1t1_index >= WTI_MAX_LINES)
   {
      WTI_TerminatePrintError("CLI_F_UfeLineSbiClkTransSet", __LINE__);
      return;
   }

   if (clock_trans_id > WTI_CESOP_MAX_CLOCK_REC_INTERFACES +
       WTI_CESOP_MAX_EXT_DIFF_CR_INTERFACES - 1) /* 367 */
   {
      WTI_TerminatePrintError("CLI_F_UfeLineSbiClkTransSet", __LINE__);
      return;
   }

   if (the_system->ufe.fpga_mode != WT_SBI)
   {
      WTI_TerminatePrintError("CLI_F_UfeLineSbiClkTransSet", __LINE__);
      return;
   }

   if (e1t1_index >= MAX_UFE_LINES_USED)
   {
      WTI_TerminatePrintError("CLI_F_UfeLineSbiClkTransSet", __LINE__);
      return;
   }

   line_params = &(the_system->ufe.line_params[e1t1_index]);
   line_params->clock_translator_id = clock_trans_id;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_UfeLineOctalClkShaperSet(char *StrPrm)
{
   WP_U32 res, cmd, e1t1_index, clock_shaper_id;
   WT_ufe_line_params *line_params;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &e1t1_index, &clock_shaper_id);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (e1t1_index >= WTI_MAX_OCTAL_LINES)
   {
      WTI_TerminatePrintError("CLI_F_UfeLineOctalClkShaperSet", __LINE__);
      return;
   }

   if (clock_shaper_id < WTI_CESOP_MAX_CLOCK_REC_INTERFACES) /* up to 32, regular mode */
   {
      if (clock_shaper_id > WTI_MAX_OCTAL_LINES)
      {
         WTI_TerminatePrintError("CLI_F_UfeLineOctalClkShaperSet", __LINE__);
         return;
      }
   }
   else /* shaper_id >= 32 ==> extended mode */
   {
      if (clock_shaper_id - WTI_CESOP_MAX_CLOCK_REC_INTERFACES > WTI_MAX_OCTAL_LINES)
      {
         WTI_TerminatePrintError("CLI_F_UfeLineOctalClkShaperSet", __LINE__);
         return;
      }
   }


   if (the_system->ufe.fpga_mode != WT_OCTAL)
   {
      WTI_TerminatePrintError("CLI_F_UfeLineOctalClkShaperSet", __LINE__);
      return;
   }

   line_params = &(the_system->ufe.line_params[e1t1_index]);
   line_params->rx_clk_rec_if = clock_shaper_id;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_UfeLineOctalClkTxTimingSet(char *StrPrm)
{
   WP_U32 res, cmd, e1t1_index, extended_active;
   WT_ufe_line_params *line_params;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &e1t1_index, &extended_active);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (e1t1_index >= WTI_MAX_OCTAL_LINES)
   {
      WTI_TerminatePrintError("CLI_F_UfeLineOctalClkTxTimingSet", __LINE__);
      return;
   }

   if (extended_active != 0 && extended_active != 1)
   {
      WTI_TerminatePrintError("CLI_F_UfeLineOctalClkTxTimingSet", __LINE__);
      return;
   }

   /* regular mode available for line_id 0 to 63 */
   if (extended_active == 0)
   {
      if (e1t1_index >= WTI_CESOP_MAX_CLOCK_REC_INTERFACES)
      {
         WTI_TerminatePrintError("CLI_F_UfeLineOctalClkTxTimingSet", __LINE__);
         return;
      }
   }

   if (the_system->ufe.fpga_mode != WT_OCTAL)
   {
      WTI_TerminatePrintError("CLI_F_UfeLineOctalClkTxTimingSet", __LINE__);
      return;
   }


   line_params = &(the_system->ufe.line_params[e1t1_index]);
   line_params->ext_mode_active = extended_active;

   return;
}


/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_UfeLineSbiClkShaperSet(char *StrPrm)
{
   WP_U32 res, cmd, e1t1_index, clock_shaper_id;
   WT_ufe_line_params *line_params;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &e1t1_index, &clock_shaper_id);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (e1t1_index >= WTI_MAX_LINES)
   {
      WTI_TerminatePrintError("CLI_F_UfeLineSbiClkShaperSet", __LINE__);
      return;
   }

   if (clock_shaper_id > WTI_CESOP_MAX_CLOCK_REC_INTERFACES +
       WTI_CESOP_MAX_EXT_DIFF_CR_INTERFACES - 1) /* 367 */
   {
      WTI_TerminatePrintError("CLI_F_UfeLineSbiClkShaperSet", __LINE__);
      return;
   }

   if (the_system->ufe.fpga_mode != WT_SBI)
   {
      WTI_TerminatePrintError("CLI_F_UfeLineSbiClkShaperSet", __LINE__);
      return;
   }
   if (e1t1_index >= MAX_UFE_LINES_USED)
   {
      WTI_TerminatePrintError("CLI_F_UfeLineSbiClkShaperSet", __LINE__);
      return;
   }

   line_params = &(the_system->ufe.line_params[e1t1_index]);
   line_params->rx_clk_rec_if = clock_shaper_id;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_UfeLineTxClkMasterSet(char *StrPrm)
{
   WP_U32 res, cmd, e1t1_index;
   WT_ufe_line_params *line_params;

   res = sscanf(StrPrm, "%d %d", &cmd, &e1t1_index);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (e1t1_index >= WTI_MAX_LINES)
   {
      WTI_TerminatePrintError("CLI_F_UfeLineTxClkMasterSet", __LINE__);
      return;
   }

   if (e1t1_index >= MAX_UFE_LINES_USED)
   {
      WTI_TerminatePrintError("CLI_F_UfeLineTxClkMasterSet", __LINE__);
      return;
   }

   line_params = &(the_system->ufe.line_params[e1t1_index]);
   line_params->tx_clk_master = WUFE_LINE_CLOCK_MASTER;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_UfeCasMode(char *StrPrm)
{
   WP_U32 res, cmd, val, e1t1_index;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &e1t1_index, &val);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (val != 0 && val != 1)
   {
      WTI_TerminatePrintError("CLI_F_UfeCasMode", __LINE__);
      return;
   }

   if (the_system->ufe.fpga_mode == WT_OCTAL)
   {
      if (e1t1_index >= WTI_MAX_OCTAL_LINES)
      {
         WTI_TerminatePrintError("CLI_F_UfeCasMode", __LINE__);
         return;
      }
   }
   else if (the_system->ufe.fpga_mode == WT_SBI)
   {
      if (e1t1_index >= WTI_MAX_LINES)
      {
         WTI_TerminatePrintError("CLI_F_UfeCasMode", __LINE__);
         return;
      }
   }

   if (e1t1_index >= MAX_UFE_LINES_USED)
   {
      WTI_TerminatePrintError("CLI_F_UfeCasMode", __LINE__);
      return;
   }

#if WTI_DUAL_EMPHY
   if (e1t1_index >= WTI_FIRST_UFE_NUM_LINES)
   {
      if (the_system->second_ufe.line_params[e1t1_index-WTI_FIRST_UFE_NUM_LINES].valid == 0)
      {
         WTI_TerminatePrintError("CLI_F_UfeCasMode line not valid", __LINE__);
         return;
      }
   }
   else
   {
      if (the_system->ufe.line_params[e1t1_index].valid == 0)
      {
         WTI_TerminatePrintError("CLI_F_UfeCasMode line not valid", __LINE__);
         return;
      }
   }

   if (e1t1_index >= WTI_FIRST_UFE_NUM_LINES)
   {
      the_system->second_ufe.line_params[e1t1_index-WTI_FIRST_UFE_NUM_LINES].cas_enable =
         (val == 1) ? WUFE_CAS_ENABLE : WUFE_CAS_DISABLE;
   }
   else
   {
      the_system->ufe.line_params[e1t1_index].cas_enable =
         (val == 1) ? WUFE_CAS_ENABLE : WUFE_CAS_DISABLE;
   }
#else
   if (the_system->ufe.line_params[e1t1_index].valid == 0)
   {
      WTI_TerminatePrintError("CLI_F_UfeCasMode line not valid", __LINE__);
      return;
   }

   the_system->ufe.line_params[e1t1_index].cas_enable =
      (val == 1) ? WUFE_CAS_ENABLE : WUFE_CAS_DISABLE;
#endif

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_UfeSbiSpeTransferMode(char *StrPrm)
{
   WP_U32 res, cmd, spe_id, spe_transfer_mode;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &spe_id, &spe_transfer_mode);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (spe_id > (WUFE_MAX_N_SBI_SPE-1))
   {
      WTI_TerminatePrintError("CLI_F_UfeSbiSpeTransferMode", __LINE__);
   }

   if (spe_transfer_mode != 0 &&
       spe_transfer_mode != 1 &&
       spe_transfer_mode != 2 &&
       spe_transfer_mode != 3 &&
       spe_transfer_mode != 4)
   {
      WTI_TerminatePrintError("CLI_F_UfeSbiSpeTransferMode", __LINE__);
      return;
   }

   if (the_system->ufe.fpga_mode == WT_OCTAL)
   {
      WTI_TerminatePrintError("CLI_F_UfeSbiSpeTransferMode - not in SBI mode", __LINE__);
   }

   if (spe_transfer_mode == 0)
      the_system->ufe.sbi_spe_line_type[spe_id] = WUFE_SBI_TYPE_IDLE;
   else
   {
      if (spe_transfer_mode == 1)
         the_system->ufe.sbi_spe_line_type[spe_id]= WUFE_SBI_TYPE_E1;
      else if (spe_transfer_mode == 2)
         the_system->ufe.sbi_spe_line_type[spe_id]= WUFE_SBI_TYPE_T1;
      else if (spe_transfer_mode == 3)
         the_system->ufe.sbi_spe_line_type[spe_id]= WUFE_SBI_TYPE_DS3;
      else if (spe_transfer_mode == 4)
         the_system->ufe.sbi_spe_line_type[spe_id]= WUFE_SBI_TYPE_E3;
      else
      {
         WTI_TerminatePrintError("CLI_F_UfeSbiSpeTransferMode", __LINE__);
      }
   }

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_UfeSbiLoopbackMode(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != 0 && val != 1 && val != 2)
   {
      WTI_TerminatePrintError("CLI_F_UfeLoopbackMode", __LINE__);
   }

   if (the_system->ufe.fpga_mode == WT_OCTAL)
   {
      WTI_TerminatePrintError("CLI_F_UfeLoopbackMode - not in SBI mode", __LINE__);
   }
   else /* WT_SBI */
   {
      if (val == 2)
      {
         /* set SBI system loopback */
         the_system->ufe.operation_mode = WT_OP_MODE_LOOPBACK_SYSTEM;
      }
      else if (val == 1)
      {
         /* set SBI line loopback */
         the_system->ufe.operation_mode = WT_OP_MODE_LOOPBACK_LINE;
      }
      else
      {
         the_system->ufe.operation_mode = WT_OP_MODE_NORMAL;
      }
   }

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_UfeTdmLoopbackMode(char *StrPrm)
{
   WP_U32 res, cmd, val, line_index;
   WP_status status;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &val, &line_index);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != 0 && val != 1 && val != 2)
   {
      WTI_TerminatePrintError("CLI_F_UfeTdmLoopbackMode - invalid type", __LINE__);
   }
#if WTI_CESOP_TDI
   if (line_index >= WTI_ACTIVE_TDI_PORTS)
   {
      WTI_TerminatePrintError("CLI_F_UfeTdmLoopbackMode", __LINE__);
      return;
   }
#else
   if (line_index >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_UfeTdmLoopbackMode", __LINE__);
      return;
   }
#endif
   if (the_system->ufe.fpga_mode == WT_OCTAL)
   {
      WTI_TerminatePrintError("CLI_F_UfeTdmLoopbackMode - not in SBI mode", __LINE__);
   }
   else /* WT_SBI */
   {
      if (val == 0)
      {
         status = WUFE_LineLoopbackConfigure(the_system->ufe.line_handle[line_index],WUFE_LINE_LOOPBACK_TYPE_NONE );
         WTI_TerminateOnError(status , "WUFE_LineLoopbackConfigure",__LINE__);
      }
      else if ( val == 1)
      {
         status = WUFE_LineLoopbackConfigure(the_system->ufe.line_handle[line_index], WUFE_LINE_LOOPBACK_TYPE_SYSTEM);
         WTI_TerminateOnError(status , "WUFE_LineLoopbackConfigure",__LINE__);
      }
      else if ( val == 2)
      {
         status = WUFE_LineLoopbackConfigure(the_system->ufe.line_handle[line_index], WUFE_LINE_LOOPBACK_TYPE_LINE);
         WTI_TerminateOnError(status , "WUFE_LineLoopbackConfigure",__LINE__);
      }
   }

   return;
}




/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_UfePdhLoopbackMode(char *StrPrm)
{
   WP_U32 res, cmd, val,first_line_index, line_index,loopback_line_index,range;
   WP_status status;
   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &val, &line_index,&range);

   if (res != 3 && res != 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != 0 && val != 1 && val != 2)
   {
      WTI_TerminatePrintError("CLI_F_UfePdhLoopbackMode - invalid type", __LINE__);
   }

   if (line_index >= 336)
   {
      WTI_TerminatePrintError("CLI_F_UfePdhLoopbackMode - invalid line_index", __LINE__);
      return;
   }

   if(res == 4)
   {
       if (line_index + range >=336)
       {
           WTI_TerminatePrintError("CLI_F_UfePdhLoopbackMode - invalid line_index and range", __LINE__);
           return;
       }
   }
   if(res == 3)
   {
       range = 1;
   }

   first_line_index=line_index;
   loopback_line_index = line_index;
   while(loopback_line_index < first_line_index + range)
   {
       if((the_system->ufe.line_params[line_index].transfer_type == WUFE_SONET_TYPE_E1 ||
          the_system->ufe.line_params[line_index].transfer_type == WUFE_SDH_TYPE_E1 ||
           the_system->ufe.line_params[line_index].transfer_type == 0) &&
          ((line_index % 28) > 20))
       {
           line_index++;
       }
       else
       {
           if (val == 0)
           {
               status = WUFE_LinePdhLoopbackConfigure(the_system->ufe.line_handle[line_index],WUFE_LINE_LOOPBACK_PDH_TYPE_NONE );
               WTI_TerminateOnError(status , "WUFE_LineLoopbackConfigure",__LINE__);
           }
           
           else if ( val == 1)
           {
               status = WUFE_LinePdhLoopbackConfigure(the_system->ufe.line_handle[line_index], WUFE_LINE_LOOPBACK_PDH_TYPE_SYSTEM);
               WTI_TerminateOnError(status , "WUFE_LineLoopbackConfigure",__LINE__);
           }
           else if ( val == 2)
           {
               status = WUFE_LinePdhLoopbackConfigure(the_system->ufe.line_handle[line_index], WUFE_LINE_LOOPBACK_PDH_TYPE_LINE);
               WTI_TerminateOnError(status , "WUFE_LineLoopbackConfigure",__LINE__);
           }
           line_index++;
           loopback_line_index++;
       }
   }
   return;
}

#if  (defined  __WT_UFE412__|| defined  __WT_UFE448__)
#if WT_UFE_FRAMER
void CLI_F_UfeDisableSocketAlarms(void)
{
    WP_U32 j,status;
    for (j = WPX_UFE_FRAMER_SOCKET_CLIENT_E_RFI; j < WPX_UFE_FRAMER_SOCKET_CLIENT_MAX_ALARM_CATEGORIES; ++j)
    {
        status = WPX_FRMR_DEV_DRV_SOCKET_DisableAlarm(j, j);
        if (status != WPX_UFE_FRAMER_OK)
        {
            printf("********************************************************************\n"
                   "          SOCKET EVENT %d FAILED\n"
                   "********************************************************************\n", j);
            WTI_FlexmuxCheckStatus("WTI_enable_alarms_and_performance_monitoring", status, __LINE__);
        }
#if DEBUG
        else
            printf("%s disabled\n", socket_client_side_alarm_names[j]);
#endif
    }
}


void CLI_F_UfeEnableSocketAlarms(void)
{
    WP_U32 j,status;
    for (j = WPX_UFE_FRAMER_SOCKET_CLIENT_E_RFI; j < WPX_UFE_FRAMER_SOCKET_CLIENT_MAX_ALARM_CATEGORIES; ++j)
   {
      status = WPX_FRMR_DEV_DRV_SOCKET_EnableAlarm(j, j);
      if (status != WPX_UFE_FRAMER_OK)
      {
         printf("********************************************************************\n"
                "          SOCKET EVENT %d FAILED\n"
                "********************************************************************\n", j);
         WTI_FlexmuxCheckStatus("WTI_enable_alarms_and_performance_monitoring", status, __LINE__);
      }
#if DEBUG
      else
         printf("%s enabled\n", socket_client_side_alarm_names[j]);
#endif
   }
}

void CLI_F_UfeSocketAlarmsMode(char *StrPrm)
{
    WP_U32 res,cmd,enable_alarm;
    res = sscanf(StrPrm, "%d %d", &cmd, &enable_alarm);
    if (res != 2)
    {
        WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
        return;
    }
    if(enable_alarm)
        CLI_F_UfeEnableSocketAlarms();
    else
        CLI_F_UfeDisableSocketAlarms();
}

#endif
#endif
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_UfeClockMode(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (val != 0 && val != 1)
   {
      WTI_TerminatePrintError("CLI_F_UfeClockMode", __LINE__);
   }

   the_system->ufe.tx_clock_mode = (val == 0) ?
      WT_RX_LOOPTIME_CLOCK : WT_EXT_CLOCK;
/* WUFE_CLOCK_REC_ENABLE - TS unit is used RX or TX direction*/
#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE ||WTI_CESOP_RX_TIMING_ENABLE)
   the_system->ufe.system_clock_rec_enable = WUFE_SYSTEM_CLOCK_REC_ENABLE;
#else
   the_system->ufe.system_clock_rec_enable = WUFE_SYSTEM_CLOCK_REC_DISABLE;
#endif

   return;
}

#if defined __WT_UFE412__ || defined __WT_UFE448__
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE

/***************************************************************
 * Func name  : CLI_F_Ufe4CRInternalRamDisplay
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Ufe4CRInternalRamDisplay(char *StrPrm)
{

   WP_U32 res, cmd, line_index,display_couner;
   WP_U32 one_line =0 ;
   WP_status status;
   WUFE_clk_rec_info_ram_line clk_rec_info_ram_line;
   WP_U32 t1, t2;
   WP_U32 pos_ind =0,neg_ind =0,invalid_ind =0 ;

   memset(&clk_rec_info_ram_line, 0, sizeof(WUFE_clk_rec_info_ram_line));

   res = sscanf(StrPrm, "%d %d", &cmd, &line_index);

   if (res != 1 && res != 2 )
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (res == 2)
   {
      if(line_index >= MAX_UFE_LINES_USED)
      {
         WTI_TerminatePrintError("Invalid line", __LINE__);
         return;
      }
#if WTI_DUAL_EMPHY
      if( the_system->ufe.line_handle[line_index] == WT_UFE_HANDLE_INVALID && the_system->second_ufe.line_handle[line_index - WTI_FIRST_UFE_NUM_LINES] == WT_UFE_HANDLE_INVALID)
      {
         WTI_TerminatePrintError("Invalid line", __LINE__);
         return;
      }
#else
      if( the_system->ufe.line_handle[line_index] == WT_UFE_HANDLE_INVALID)
      {
         WTI_TerminatePrintError("Invalid line", __LINE__);
         return;
      }
#endif
      one_line =1;
   }


   Vt100GotoXy(1,1);
   Vt100ClearScreen();

   while (1)
   {
      t1 = WP_TimeRead();

      if (WTI_SampleSerial())
      {
         break;
      }

      while (1)
      {
         t2 = WP_TimeRead();
         if( (t2-t1) >= FACTOR/1000)
         {
            Vt100GotoXy(1,1);

            if(one_line)
            {
#if WTI_DUAL_EMPHY
               if(line_index >= WTI_FIRST_UFE_NUM_LINES)
               {
                  status =  WUFE_ClockRecInfo(the_system->second_ufe.ufe_id,
                                              the_system->second_ufe.line_handle[line_index - WTI_FIRST_UFE_NUM_LINES],
                                              WUFE_CLOCK_REC_INFO_RAM_LINE,
                                              &clk_rec_info_ram_line);
                  if(WTI_PrintOnError(status, "WUFE_ClockRecInfo", __LINE__))
                     return;
               }
               else
               {
                  status =  WUFE_ClockRecInfo(the_system->ufe.ufe_id,
                                              the_system->ufe.line_handle[line_index],
                                              WUFE_CLOCK_REC_INFO_RAM_LINE,
                                              &clk_rec_info_ram_line);
                  if(WTI_PrintOnError(status, "WUFE_ClockRecInfo", __LINE__))
                     return;

               }
#else
               status =  WUFE_ClockRecInfo(the_system->ufe.ufe_id,
                                           the_system->ufe.line_handle[line_index],
                                           WUFE_CLOCK_REC_INFO_RAM_LINE,
                                           &clk_rec_info_ram_line);
               if(WTI_PrintOnError(status, "WUFE_ClockRecInfo", __LINE__))
                  return;
#endif


               /* count justifications */
               if(clk_rec_info_ram_line.stuffing_ind == 1)
               {
                  neg_ind++;
               }
               else if(clk_rec_info_ram_line.stuffing_ind == 3)
               {
                  pos_ind++;
               }
               else if(clk_rec_info_ram_line.stuffing_ind == 2)
               {
                  invalid_ind++;
               }

               printf("\nClock Recovery Indirect registers (RAM) line %d\n",line_index);
               printf("-------------------------------------------------------\n\n");
               printf("Delta Gap:             0x%08x  \n",clk_rec_info_ram_line.delta_gap);
               printf("Delta Gap Accumulated: 0x%08x  \n",clk_rec_info_ram_line.current_gap_fraction);
               printf("Justification:         %d %d %d (positive/negative/invalid)\n",pos_ind,neg_ind,invalid_ind);
               printf("\n");
               printf("Current Gap Counter:    %3d (E1 0-10  , T1 0-13)\n",clk_rec_info_ram_line.current_gap_count );
               printf("Gap Vector:             %3d (E1 0-255 , T1 0-192)\n",clk_rec_info_ram_line.max_gap_vector );

            }
            else
            {

               printf("Line:Delta Gap\n");
               printf("-------------------------------------------------------\n\n");

               for (line_index=0 , display_couner=0; line_index<MAX_UFE_LINES_USED; line_index++)
               {
#if WTI_DUAL_EMPHY
                  if(line_index >= WTI_FIRST_UFE_NUM_LINES)
                  {
                     if(the_system->second_ufe.line_handle[line_index - WTI_FIRST_UFE_NUM_LINES] != WT_UFE_HANDLE_INVALID)
                     {
                        status =  WUFE_ClockRecInfo(the_system->second_ufe.ufe_id,
                                                    the_system->second_ufe.line_handle[line_index - WTI_FIRST_UFE_NUM_LINES],
                                                    WUFE_CLOCK_REC_INFO_RAM_LINE,
                                                    &clk_rec_info_ram_line);
                        if(WTI_PrintOnError(status, "WUFE_ClockRecInfo", __LINE__))
                           return;

                        printf("%d:0x%08x(%3d) ",line_index,clk_rec_info_ram_line.delta_gap,clk_rec_info_ram_line.max_gap_vector);

                        if (display_couner++%7 == 6)
                           printf("\n");

                     }
                  }
                  else
                  {
                     if(the_system->ufe.line_handle[line_index] != WT_UFE_HANDLE_INVALID)
                     {
                        status =  WUFE_ClockRecInfo(the_system->ufe.ufe_id,
                                                    the_system->ufe.line_handle[line_index],
                                                    WUFE_CLOCK_REC_INFO_RAM_LINE,
                                                    &clk_rec_info_ram_line);
                        if(WTI_PrintOnError(status, "WUFE_ClockRecInfo", __LINE__))
                           return;

                        printf("%d:0x%08x(%3d) ",line_index,clk_rec_info_ram_line.delta_gap,clk_rec_info_ram_line.max_gap_vector);

                        if (display_couner++%7 == 6)
                           printf("\n");

                     }
                  }
#else /* NOT DUAL EMPHY */
                  if(the_system->ufe.line_handle[line_index] != WT_UFE_HANDLE_INVALID)
                  {

                     status =  WUFE_ClockRecInfo(the_system->ufe.ufe_id,
                                                 the_system->ufe.line_handle[line_index],
                                                 WUFE_CLOCK_REC_INFO_RAM_LINE,
                                                 &clk_rec_info_ram_line);
                     if(WTI_PrintOnError(status, "WUFE_ClockRecInfo", __LINE__))
                        return;

                     printf("%d:0x%08x ",line_index,clk_rec_info_ram_line.delta_gap);

                     if (display_couner++%7 == 6)
                        printf("\n");

                  }  /* valid handle */
#endif
               }  /* for loop */
            }  /* one_line */
            break;
         }  /* t2-t1 */
      }  /* inner while 1 */
   } /* outer while 1 */
   printf("\n");

}


#endif  /*   __WT_UFE412__ || __WT_UFE448__*/
#endif  /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */


#endif  /* (!WTI_CESOP_TDI)  */
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_TransDevShowInfo(char *StrPrm)
{
   WP_U32 res, cmd, pw_index;
#if !WTI_CESOP_TDI
   WP_U32 line_index;
#endif

#if WTI_CESOP_TDI
   res = sscanf(StrPrm, "%d %d", &cmd, &pw_index);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (pw_index > WTI_ACTIVE_TDI_PORTS)
   {
      WTI_TerminatePrintError("CLI_F_TransDevShowInfo", __LINE__);
      return;
   }
#else
   res = sscanf(StrPrm, "%d %d %d", &cmd, &line_index, &pw_index);
   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (pw_index > total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_TransDevShowInfo", __LINE__);
      return;
   }
#endif

#if WTI_CESOP_TDI
   printf("\nDEVICE INFO:\n");
   printf("\n");
   printf("TRANSPARENT CES DEVICE %d ( DEVICE HANDLE %x ):\n",pw_index, the_system->tdm_trans_dev[pw_index]);
   printf("-------------------------------------------------\n");
   printf("\n");

   WP_Display(0,the_system->tdm_trans_dev[pw_index],WP_DISPLAY_DEFAULT,NULL);

   printf("\n\n");
#else
   printf("\nDEVICE INFO:\n");
   printf("\n");
   printf("EMPHY TRANSPARENT DEVICE %d ( DEVICE HANDLE %x ):\n",pw_index, the_system->pw[pw_index].trans_device);
   printf("-------------------------------------------------\n");
   printf("\n");

   WP_Display(0,the_system->pw[pw_index].trans_device,WP_DISPLAY_DEFAULT,NULL);

   printf("UFE SYSTEM DISPLAY FOR UFE ID %d:\n",the_system->ufe.ufe_id);
   printf("-------------------------------------------------\n");
   printf("\n");

   WUFE_SystemDisplay(the_system->ufe.ufe_id);

   printf("\nUFE LINE DISPLAY FOR LINE INDEX %d (LINE HANDLE %x):\n",line_index, the_system->ufe.line_handle[line_index]);
   printf("-------------------------------------------------\n");
   printf("\n");

   WUFE_LineDisplay(the_system->ufe.line_handle[line_index]);

   printf("UFE PHY DISPLAY FOR PW INDEX %d (PHY HANDLE %x):\n", pw_index, the_system->ufe.phy_handle[pw_index]);
   printf("-------------------------------------------------\n");
   printf("\n");

   WUFE_PhyDisplay(the_system->ufe.phy_handle[pw_index]);

#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE||WTI_CESOP_RX_TIMING_ENABLE)
#if __WT_UFE3__
   printf("UFE CLOCK RECOVERY DISPLAY FOR PW INDEX %d (PHY HANDLE %x):\n", pw_index, the_system->ufe.phy_handle[pw_index]);
   printf("-------------------------------------------------\n");
   printf("\n");

   WUFE_ClockRecDisplay(the_system->ufe.phy_handle[pw_index]);
#endif /* WT_UFE3 */
#endif
   printf("\n\n");
#endif

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Pwe3ChannelShowInfo(char *StrPrm)
{
   WP_U32 res, cmd, val, val1, i;
   WP_ch_trans_pwe3 trans_ch_config;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &val, &val1);
   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != 0 && val != 1)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3ChannelShowInfo", __LINE__);
      return;
   }
   if (val1 >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3ChannelShowInfo", __LINE__);
      return;
   }
   if (val)
   {
      memcpy(&trans_ch_config,&pw_info_base[val1].trans_tx_ch_config,sizeof(WP_ch_trans_pwe3));
   }
   else
   {
      memcpy(&trans_ch_config,&pw_info_base[val1].trans_rx_ch_config,sizeof(WP_ch_trans_pwe3));
   }

   printf("PWE3 TRANSPARENT CHANNEL INFO:\n");
   printf("\n");
   if (val)
      printf("PWE3 TRANSPARENT TX CHANNEL %d ( CHANNEL HANDLE %x ):\n",val1, the_system->pw[val1].trans_tx);
   else
      printf("PWE3 TRANSPARENT RX CHANNEL %d ( CHANNEL HANDLE %x ):\n",val1, the_system->pw[val1].trans_rx);
   printf("-------------------------------------------------------------\n");
   printf("\n");

   printf("CHANNEL PARAMETERS:\n");
   printf("\nnum_slots: %d\n",trans_ch_config.num_slots);

   if (val)
   {
      printf("jitter_buffer_size: %d\n",trans_ch_config.tx_jitter_buffer_size);
      printf("udp_dummy_mode: ");
      switch(trans_ch_config.tx_udp_dummy_mode)
      {
         case(WP_TRANS_TX_USER_DEFINED_FRAME):
            printf("WP_TRANS_TX_USER_DEFINED_FRAME\n");
            break;
         case(WP_TRANS_TX_LAST_FRAME):
            printf("WP_TRANS_TX_LAST_FRAME\n");
            break;
         default:
            break;
      }
      printf("tx_udcas: %d\n",trans_ch_config.tx_udcas);

      for (i=0;i<trans_ch_config.num_slots;++i)
      {
         printf("tx_udpattern[%d] -> %x\n",i,trans_ch_config.tx_udpattern[i]);
      }

      for (i=0;i<trans_ch_config.num_slots;++i)
      {
         printf("udp_dummy_pattern[%d] -> %x\n",i,trans_ch_config.tx_udp_dummy_pattern[i]);
      }
      printf("\n");
      WP_Display(0,the_system->pw[val1].trans_tx,WP_DISPLAY_DEFAULT,NULL);
   }
   else
   {
      printf("rx_buffersize: %d\n\n",trans_ch_config.rx_buffersize);
      WP_Display(0,the_system->pw[val1].trans_rx,WP_DISPLAY_DEFAULT,NULL);
   }

   return;
}


/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Pwe3ChannelStatmode(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != WP_ENABLE && val != WP_DISABLE)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3ChannelStatmode", __LINE__);
      return;
   }

   pw_config.statmode = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Pwe3ChannelRxBufferSize(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val == 0)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3ChannelRxBufferSize", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.payload_size = pw_config.rx_buffersize = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Pwe3ChannelJitterBufferSize(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val < 1)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3ChannelJitterBufferSize", __LINE__);
      return;
   }
   pw_config.tx_jitter_buffer_size = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Pwe3ChannelUdpDummyMode(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != WP_TRANS_TX_LAST_FRAME &&
       val != WP_TRANS_TX_USER_DEFINED_FRAME)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3ChannelUdpDummyMode", __LINE__);
      return;
   }

   pw_config.tx_udp_dummy_mode = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Pwe3ChannelTxUdcas(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %x", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val > 15)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3ChannelTxUdcas", __LINE__);
      return;
   }

   pw_config.tx_udcas = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Pwe3ChannelTxUdpattern(char *StrPrm)
{
   WP_U32 res, cmd;
   WP_CHAR temp_string[WTI_MAX_STRING_SIZE], *byte;
   WP_U8 pattern[WTI_E1_SLOTS];
   WP_U32 i;

   res = sscanf(StrPrm, "%d %s", &cmd, temp_string);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   i=0;
   byte = strtok(temp_string,",");
   while (byte)
   {
      pattern[i++] = atoi(byte);
      byte = strtok(NULL,",");
   }

#if WTI_CESOP_TDI
   if ((pw_config.n_slots != WTI_T1_SLOTS) &&
       (i != pw_config.n_slots))
   {
      WTI_TerminatePrintError("CLI_F_Pwe3ChannelTxUdpattern", __LINE__);
      return;
   }
#else
   if (i != pw_config.n_slots)
   {
      printf("i is %d, pw_config.n_slots is %d\n",i,pw_config.n_slots);
      WTI_TerminatePrintError("CLI_F_Pwe3ChannelTxUdpattern", __LINE__);
      return;
   }
#endif

   memcpy(pw_config.tx_udpattern, pattern, WTI_E1_SLOTS);

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Pwe3ChannelTxDummyUdpattern(char *StrPrm)
{
   WP_U32 res, cmd;
   WP_CHAR temp_string[WTI_MAX_STRING_SIZE], *byte;
   WP_U8 pattern[WTI_E1_SLOTS];
   WP_U32 i;

   res = sscanf(StrPrm, "%d %s", &cmd, temp_string);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   i=0;
   byte = strtok(temp_string,",");
   while (byte)
   {
      pattern[i++] = atoi(byte);
      byte = strtok(NULL,",");
   }

#if WTI_CESOP_TDI
   if ((pw_config.n_slots != WTI_T1_SLOTS) &&
       (i != pw_config.n_slots))
   {
      WTI_TerminatePrintError("CLI_F_Pwe3ChannelTxDummyUdpattern", __LINE__);
      return;
   }
#else
   if (i != pw_config.n_slots)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3ChannelTxDummyUdpattern", __LINE__);
      return;
   }
#endif

   memcpy(pw_config.tx_udp_dummy_pattern, pattern, WTI_E1_SLOTS);

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnIwShowInfo(char *StrPrm)
{
   printf("TDM TO PSN IW SYSTEM INFO:\n");
   printf("\n");
   printf("TDM TO PSN CESOP IW SYSTEM ( SYSTEM HANDLE %x ):\n",the_system->tdm2psn_iw_system);
   printf("----------------------------------------------------------\n");
   printf("\n");

   printf("SYSTEM PARAMETERS:\n");
   printf("buffer_gap: %d\n",iw_cesop_system_config.buffer_gap);

   WP_Display(0,the_system->tdm2psn_iw_system,WP_DISPLAY_DEFAULT,NULL);

   return;
}

#if (WTI_CESOP_MPLS_IW && !WTI_PCE_CLASSIFIER)
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmIwShowInfo(char *StrPrm)
{

   printf("PSN TO TDM IW SYSTEM INFO:\n");
   printf("\n");
   printf("PSN TO TDM MPLS ROUTING IW SYSTEM ( SYSTEM HANDLE %x ):\n",the_system->psn2tdm_iw_system);
   printf("----------------------------------------------------------\n");
   printf("\n");

   printf("SYSTEM PARAMETERS:\n");

   printf("max_flows: %d\n",iw_mpls_sys_config.max_flows);
   printf("buffer_gap: %d\n",iw_mpls_sys_config.buffer_gap);
   printf("edge_out_system: %d\n",iw_mpls_sys_config.edge_out_iw_system);

   WP_Display(0,the_system->psn2tdm_iw_system,WP_DISPLAY_DEFAULT,NULL);
}

#else
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmIwShowInfo(char *StrPrm)
{
   printf("PSN TO TDM IW SYSTEM INFO:\n");
   printf("\n");
   printf("PSN TO TDM ROUTING IW SYSTEM ( SYSTEM HANDLE %x ):\n",the_system->psn2tdm_iw_system);
   printf("----------------------------------------------------------\n");
   printf("\n");

   printf("SYSTEM PARAMETERS:\n");
#if !WTI_PCE_CLASSIFIER
   printf("classification_mode: ");
   switch (ipv4_sys_info.classification_mode)
   {
      case (WP_IW_DYNAMIC_CLASS):
         {
            printf("WP_IW_DYNAMIC_CLASS\n");
            break;
         }
      case (WP_IW_PRIORITY_BASED_ROUTING):
         {
            printf("WP_IW_PRIORITY_BASED_ROUTING\n");
            break;
         }
      case (WP_IW_NULL_CLASSIFICATION_ROUTING):
         {
            printf("WP_IW_NULL_CLASSIFICATION_ROUTING\n");
            break;
         }
      case (WP_IW_NO_CLASSIFIER):
         {
            printf("WP_IW_NO_CLASSIFIER\n");
            break;
         }
      default:
         printf("undefined\n");
         break;
   }

   printf("max_lpm_flows: %d\n",ipv4_sys_info.max_lpm_flows);
   printf("dfc_info: %x\n",(WP_U32)ipv4_sys_info.dfc_info);
#endif
   printf("buffer_gap: %d\n",iw_routing_system_config.buffer_gap);
   printf("max_classification_rules: %d\n",iw_routing_system_config.max_classification_rules);
   printf("max_iw_ports: %d\n",iw_routing_system_config.max_iw_ports);

   WP_Display(0,the_system->psn2tdm_iw_system,WP_DISPLAY_DEFAULT,NULL);

   return;
}
#endif

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SlotsConfigShowInfo(char *StrPrm)
{
   WP_U32 res, cmd, e1t1_index;
#if (!WTI_CESOP_TDI)
   WP_U32 i, slot_index;
   int spe_id;
#endif
   res = sscanf(StrPrm, "%d %d", &cmd, &e1t1_index);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

#if (!WTI_CESOP_TDI)
   if (the_system->ufe.fpga_mode == WT_OCTAL)
   {
      WT_ufe_line_params *line_params;

      if (e1t1_index >= MAX_UFE_LINES_USED)
      {
         WTI_TerminatePrintError("CLI_F_SlotsConfigShowInfo", __LINE__);
         return;
      }

      line_params = &(the_system->ufe.line_params[e1t1_index]);

      if (e1t1_index >= WTI_MAX_OCTAL_LINES)
      {
         WTI_TerminatePrintError("CLI_F_SlotsConfigShowInfo - E1/T1 index", __LINE__);
         return;
      }

      if (line_params->transfer_type == WUFE_CAD_TYPE_T1)
      {
         for (i=0;i<WTI_T1_SLOTS;++i)
         {
            if (slots_config[e1t1_index*WTI_E1_SLOTS+i] != WTI_UNUSED_SLOT)
               printf("T1 %d DS0 %d  -> PW %d\n",e1t1_index,i,slots_config[e1t1_index*WTI_E1_SLOTS+i]);
            else
               printf("T1 %d DS0 %d UNUSED\n",e1t1_index,i);
         }
      }
      else if (line_params->transfer_type == WUFE_CAD_TYPE_E1)
      {
         for (i=0;i<WTI_E1_SLOTS;++i)
         {
            if (slots_config[e1t1_index*WTI_E1_SLOTS+i] != WTI_UNUSED_SLOT)
               printf("E1 %d DS0 %d  -> PW %d\n",e1t1_index,i,slots_config[e1t1_index*WTI_E1_SLOTS+i]);
            else
               printf("E1 %d DS0 %d UNUSED\n",e1t1_index,i);
         }
      }
      else if (line_params->transfer_type == WUFE_CAD_TYPE_DS3)
      {
         for (i=0;i<WTI_DS3_SLOTS;++i)
         {
            if (slots_config[e1t1_index*WTI_E1_SLOTS+i] != WTI_UNUSED_SLOT)
               printf("DS3 %d DS0 %d  -> PW %d\n",e1t1_index,i,slots_config[e1t1_index*WTI_E1_SLOTS+i]);
            else
               printf("DS3 %d DS0 %d UNUSED\n",e1t1_index,i);
         }
      }
      else if (line_params->transfer_type == WUFE_CAD_TYPE_DS3)
      {
         for (i=0;i<WTI_E3_SLOTS;++i)
         {
            if (slots_config[e1t1_index*WTI_E1_SLOTS+i] != WTI_UNUSED_SLOT)
               printf("E3 %d DS0 %d  -> PW %d\n",e1t1_index,i,slots_config[e1t1_index*WTI_E1_SLOTS+i]);
            else
               printf("E3 %d DS0 %d UNUSED\n",e1t1_index,i);
         }
      }
      else
      {
         WTI_TerminatePrintError("CLI_F_SlotsConfigShowInfo", __LINE__);
         return;
      }

   }
#if defined __WT_UFE412__ || defined __WT_UFE448__
   else  if (the_system->ufe.fpga_mode == WT_SDH)
   {
      WTI_TerminatePrintError("CLI_F_SlotsConfigShowInfo -- SDH", __LINE__);
      return;
   }
   else  if (the_system->ufe.fpga_mode == WT_SONET)
   {
      WTI_TerminatePrintError("CLI_F_SlotsConfigShowInfo -- SONET", __LINE__);
      return;
   }
#endif
   else /* SBI */
   {
      if (e1t1_index >= WTI_MAX_LINES)
      {
         WTI_TerminatePrintError("CLI_F_SlotsConfigShowInfo", __LINE__);
         return;
      }

      /* get spe_id */
      spe_id = WTI_SpeIdGet(e1t1_index);
      if (spe_id > (WUFE_MAX_N_SBI_SPE-1))
      {
         printf("CLI_F_SlotsConfigShowInfo: spe_id invalid\n");
         return;
      }

      /*get the index to validate the slots status used/unused*/
      slot_index = spe_id * WTI_TDM_SLOTS_PER_SPE;

      /*each SPE contains 28 reserved indexes */
      if (the_system->ufe.sbi_spe_line_type[spe_id] == WUFE_SBI_TYPE_T1)
         slot_index += (e1t1_index - WTI_T1_LINES_PER_SPE*spe_id) * WTI_T1_SLOTS;
      else if(the_system->ufe.sbi_spe_line_type[spe_id] == WUFE_SBI_TYPE_E1)
         slot_index += (e1t1_index - WTI_T1_LINES_PER_SPE*spe_id) * WTI_E1_SLOTS;
      else if(the_system->ufe.sbi_spe_line_type[spe_id] == WUFE_SBI_TYPE_DS3)
         slot_index += (e1t1_index - WTI_T1_LINES_PER_SPE*spe_id) * WTI_DS3_SLOTS;
      else if(the_system->ufe.sbi_spe_line_type[spe_id] == WUFE_SBI_TYPE_E3)
         slot_index += (e1t1_index - WTI_T1_LINES_PER_SPE*spe_id) * WTI_E3_SLOTS;

      if (the_system->ufe.sbi_spe_line_type[spe_id] == WUFE_SBI_TYPE_T1)
      {
         for (i=0;i<WTI_T1_SLOTS;++i)
         {
            if (slots_config[slot_index + i] != WTI_UNUSED_SLOT)
               printf("T1 %d DS0 %d  -> PW %d\n",e1t1_index,i,slots_config[slot_index+i]);
            else
               printf("T1 %d DS0 %d UNUSED\n",e1t1_index,i);
         }
      }
      else if (the_system->ufe.sbi_spe_line_type[spe_id] == WUFE_SBI_TYPE_E1)
      {
         for (i=0;i<WTI_E1_SLOTS;++i)
         {
            if (slots_config[slot_index + i] != WTI_UNUSED_SLOT)
               printf("E1 %d DS0 %d  -> PW %d\n",e1t1_index,i,slots_config[slot_index+i]);
            else
               printf("E1 %d DS0 %d UNUSED\n",e1t1_index,i);
         }
      }
      else if (the_system->ufe.sbi_spe_line_type[spe_id] == WUFE_SBI_TYPE_DS3)
      {
         for (i=0;i<WTI_DS3_SLOTS;++i)
         {
            if (slots_config[slot_index + i] != WTI_UNUSED_SLOT)
               printf("DS3 %d DS0 %d  -> PW %d\n",e1t1_index,i,slots_config[slot_index+i]);
            else
               printf("DS3 %d DS0 %d UNUSED\n",e1t1_index,i);
         }
      }
      else if (the_system->ufe.sbi_spe_line_type[spe_id] == WUFE_SBI_TYPE_E3)
      {
         for (i=0;i<WTI_E3_SLOTS;++i)
         {
            if (slots_config[slot_index + i] != WTI_UNUSED_SLOT)
               printf("E3 %d DS0 %d  -> PW %d\n",e1t1_index,i,slots_config[slot_index+i]);
            else
               printf("E3 %d DS0 %d UNUSED\n",e1t1_index,i);
         }
      }
      else
      {
         WTI_TerminatePrintError("CLI_F_SlotsConfigShowInfo", __LINE__);
         return;

      }

   }

#endif
   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
#ifdef WP_HW_WINPATH3
void CLI_F_BoardPinConfig(char *StrPrm)
{
   WP_U32 res, cmd, configuration, board_id;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &configuration, &board_id);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (configuration != WTI_DEFAULT_PIN_CONFIG &&     /* 1 */
       configuration != WTI_PTP_TWO_BOARDS_CONFIG &&  /* 2 */
       configuration != WTI_SYNC_ETH_CONFIG)          /* 3 */
   {
      WTI_TerminatePrintError("Invalid configuration value", __LINE__);
      return;
   }
   if (configuration == WTI_PTP_TWO_BOARDS_CONFIG &&
       (board_id != 1 && board_id != 2))
   {
      WTI_TerminatePrintError("Invalid board number", __LINE__);
      return;
   }
   if (configuration == WTI_SYNC_ETH_CONFIG &&
       (board_id != 1 && board_id != 2))
   {
      WTI_TerminatePrintError("Invalid board number", __LINE__);
      return;
   }

#if (!WTI_CESOP_TDI)
   WTI_BoardPinConfig(configuration, board_id, wpx_apll_clk0);
#endif

   return;
}

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
/* Function for internal testing and debug */
/* This function sets or clears the TS stub bit in the CR mode register.
   It is used mainly for testing and debug and required a dedicated synthesis. This function
   has no real purpose in a working system and should not be called. */
void CLI_F_ClockRecTimestampStub(char *StrPrm)
{
   WP_U32 res, cmd, enable;

   res = sscanf(StrPrm, "%d %d", &cmd, &enable);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if ((enable != 0) & (enable != 1))
   {
      WTI_TerminatePrintError("Invalid enable value", __LINE__);
      return;
   }
#if (!WTI_CESOP_TDI)
#if defined __WT_UFE412__ || defined __WT_UFE448__
   WT_UfeClockRecTimestampStubSet(enable);
#endif /* WT_UFE4 */
#endif /*!WTI_CESOP_TDI*/
   return;
}
/* End of function for internal testing and debug */
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
#endif

#if (!WTI_CESOP_TDI)

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_PwConfigureFramed(char *StrPrm)
{
   WP_U32 res, cmd, val1, e1t1_index, pw_index, num_slots, i, num_of_params;
   WP_U32 spe_id =0;
   WP_U32 rx_dataunit_size, tx_dataunit_size, slot_index;
   WP_CHAR slots_array[WTI_MAX_STRING_SIZE];
   WP_CHAR *slot, *params, tmp_str[WTI_MAX_STRING_SIZE];
   WP_U32 slots[WTI_E1_SLOTS];

   /* check the number of parameters sent to function */
   num_of_params=0;
   strcpy(tmp_str, StrPrm);
   params = strtok(tmp_str, " ");
   while (params != NULL)
   {
      params = strtok(NULL, " ");
      num_of_params++;
   }
   if (num_of_params == 7)
   {
      res = sscanf(StrPrm, "%d %d %d %d %d %d %s", &cmd, &e1t1_index, &pw_index, &rx_dataunit_size,
                   &tx_dataunit_size, &num_slots, slots_array);
      if (res != 7)
      {
         WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
         return;
      }
      if (pw_index >= WTI_MAX_PW)
      {
         WTI_TerminatePrintError("CLI_F_PwConfigureFramed - pw index", __LINE__);
         return;
      }
   }
   else if (num_of_params == 6)
   {
      res = sscanf(StrPrm, "%d %d %d %d %d %s", &cmd, &e1t1_index, &rx_dataunit_size,
                   &tx_dataunit_size, &num_slots, slots_array);
      if (res != 6)
      {
         WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
         return;
      }
   }
   else /* num_of_params != 6 and num_of_params !=7 => exit */
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   /* get line id or spe id */
   if (the_system->ufe.fpga_mode == WT_OCTAL)
   {
      if (e1t1_index >= WTI_MAX_OCTAL_LINES)
      {
         WTI_TerminatePrintError("CLI_F_PwConfigureFramed - E1/T1 index", __LINE__);
         return;
      }

      if (e1t1_index >= MAX_UFE_LINES_USED)
      {
         WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
         return;
      }
      /* line id */
      if (the_system->ufe.line_params[e1t1_index].framed == WUFE_UNFRAMED)
      {
         printf("ERROR - Invalid configuration: %s line_index %d in unframed mode\n",
                "UFE line is not configured to FRAMED mode", e1t1_index);
         return;
      }

      slot_index = e1t1_index * WTI_E1_SLOTS;
      val1 = the_system->ufe.line_params[e1t1_index].transfer_type;
      spe_id = e1t1_index;
   }
   else if (the_system->ufe.fpga_mode == WT_SBI)
   {
      /* get spe_id */
      spe_id = WTI_SpeIdGet(e1t1_index);

      if (spe_id > (WUFE_MAX_N_SBI_SPE-1))
      {
         printf("CLI_F_PwConfigureFramed: spe_id invalid\n");
         return;
      }

      /*get the index to validate the slots status used/unused*/
      slot_index = spe_id * WTI_TDM_SLOTS_PER_SPE;

      /*each SPE contains 28 reserved indexes */
      if (the_system->ufe.sbi_spe_line_type[spe_id] == WUFE_SBI_TYPE_T1)
         slot_index += (e1t1_index - WTI_T1_LINES_PER_SPE*spe_id) * WTI_T1_SLOTS;
      else if(the_system->ufe.sbi_spe_line_type[spe_id] == WUFE_SBI_TYPE_E1)
         slot_index += (e1t1_index - WTI_T1_LINES_PER_SPE*spe_id) * WTI_E1_SLOTS;
      else if(the_system->ufe.sbi_spe_line_type[spe_id] == WUFE_SBI_TYPE_DS3)
         slot_index += (e1t1_index - WTI_T1_LINES_PER_SPE*spe_id) * WTI_DS3_SLOTS;
      else if(the_system->ufe.sbi_spe_line_type[spe_id] == WUFE_SBI_TYPE_E3)
         slot_index += (e1t1_index - WTI_T1_LINES_PER_SPE*spe_id) * WTI_E3_SLOTS;

      val1 = the_system->ufe.sbi_spe_line_type[spe_id];
   }
   else
   {
#if WTI_DUAL_EMPHY
      if(WTI_EMPHY_SECOND == pw_config.pw_emphy_num)
      {
         slot_index = (e1t1_index * WTI_E1_SLOTS) + (WTI_MAX_SLOTS / 2);
      }
      else
      {
         slot_index = e1t1_index * WTI_E1_SLOTS;
      }
#else
      slot_index = e1t1_index * WTI_E1_SLOTS;
#endif
      val1 = the_system->ufe.line_params[e1t1_index].transfer_type;
   }


   if (rx_dataunit_size < WTI_MIN_DATAUNIT_SIZE ||
       rx_dataunit_size > WTI_MAX_DATAUNIT_SIZE)
   {
      WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
      return;
   }

   if (tx_dataunit_size < WTI_MIN_DATAUNIT_SIZE ||
       tx_dataunit_size > WTI_MAX_DATAUNIT_SIZE)
   {
      WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
      return;
   }

   if (val1 == WUFE_CAD_TYPE_T1 || val1 == WUFE_SBI_TYPE_T1 ||
       val1 == WUFE_SONET_TYPE_T1 || val1 == WUFE_SDH_TYPE_T1)
   {
      if (num_slots < WTI_MIN_SLOTS_PER_PHY ||
          num_slots > WTI_T1_SLOTS)
      {
         WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
         return;
      }
   }
   else if (val1 == WUFE_CAD_TYPE_E1 || val1 == WUFE_SBI_TYPE_E1 ||
            val1 == WUFE_SONET_TYPE_E1 || val1 == WUFE_SDH_TYPE_E1)
   {
      if (num_slots < WTI_MIN_SLOTS_PER_PHY ||
          num_slots > WTI_E1_SLOTS)
      {
         WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
         return;
      }
   }
   else if (val1 == WUFE_CAD_TYPE_DS3 || val1 == WUFE_SBI_TYPE_DS3 ||
            val1 == WUFE_CAD_TYPE_E3 || val1 == WUFE_SBI_TYPE_E3 ||
            val1 == WUFE_SONET_TYPE_DS3 || val1 == WUFE_SDH_TYPE_DS3 ||
            val1 == WUFE_SONET_TYPE_E3 || val1 == WUFE_SDH_TYPE_E3)
   {
      if (num_slots < WTI_MIN_SLOTS_PER_PHY ||
          num_slots > WTI_DS3_SLOTS/* == WTI_E3_SLOTS*/)
      {
         WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
         return;
      }
   }

   i=0;
   if (strstr(slots_array,","))
   {
      slot = strtok(slots_array,",");

      while (slot)
      {
         slots[i++] = atoi(slot);
         slot = strtok(NULL,",");
      }
      if (i!=num_slots)
      {
         WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
         return;
      }
   }
   else if (strstr(slots_array,"-"))
   {
      slot = strtok(slots_array,"-");
      if (slot)
      {
         WP_U32 j=0;

         slots[0] = atoi(slot);
         slot = strtok(NULL,"-");
         j = atoi(slot);
         if (slots[0] + num_slots - 1 != j)
         {
            WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
            return;
         }
         else
         {
            for (i=1;i<num_slots;++i)
            {
               slots[i] = slots[i-1] + 1;
            }
         }
      }
      else
      {
         WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
         return;
      }
   }
   else
   {
      if (num_slots != 1)
      {
         WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
         return;
      }
      slots[0] = atoi(slots_array);
   }

   for (i=0;i<num_slots;++i)
   {
      if (val1 == WUFE_CAD_TYPE_T1 || val1 == WUFE_SBI_TYPE_T1 ||
          val1 == WUFE_SONET_TYPE_T1 || val1 == WUFE_SDH_TYPE_T1) /* T1 */
      {
         if (slots[i]>=WTI_T1_SLOTS)
         {
            WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
            return;
         }
      }
      else
      {
         if (slots[i]>=WTI_E1_SLOTS)
         {
            WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
            return;
         }
      }
      if (slots_config[slot_index + slots[i]] != WTI_UNUSED_SLOT) /* slot already occupied */
      {
    	  WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
         return;
      }
   }

   /* mark slots as occupied by saving the PW index*/
   for (i=0;i<num_slots;++i)
   {
      if (num_of_params == 6)
         slots_config[slot_index + slots[i]] = total_number_of_pw;
      else /* num_of_params = 7 */
      {
         slots_config[slot_index + slots[i]] = pw_index;
      }
   }

   if (the_system->ufe.fpga_mode == WT_OCTAL)
   {
      if (e1t1_index >= MAX_UFE_LINES_USED)
      {
         WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
         return;
      }
      if (the_system->ufe.line_params[e1t1_index].cas_enable == WUFE_CAS_ENABLE)
      {
         if (val1 == WUFE_CAD_TYPE_T1 || val1 == WUFE_SBI_TYPE_T1)
            pw_config.mode = WP_EMPHY_TRANS_FRAMED_CAS | WP_EMPHY_TRANS_T1;
         else if(val1 == WUFE_CAD_TYPE_E1 || val1 == WUFE_SBI_TYPE_E1)
            pw_config.mode = WP_EMPHY_TRANS_FRAMED_CAS | WP_EMPHY_TRANS_E1;
         else
         {
            WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
            return;
         }

      }
      else
      {
         if (val1 == WUFE_CAD_TYPE_T1 || val1 == WUFE_SBI_TYPE_T1)
            pw_config.mode = WP_EMPHY_TRANS_FRAMED | WP_EMPHY_TRANS_T1;
         else if(val1 == WUFE_CAD_TYPE_E1 || val1 == WUFE_SBI_TYPE_E1)
            pw_config.mode = WP_EMPHY_TRANS_FRAMED | WP_EMPHY_TRANS_E1;
         else if(val1 == WUFE_CAD_TYPE_DS3 || val1 == WUFE_SBI_TYPE_DS3)
            pw_config.mode = WP_EMPHY_TRANS_FRAMED | WP_EMPHY_TRANS_DS3;
         else if(val1 == WUFE_CAD_TYPE_E3 || val1 == WUFE_SBI_TYPE_E3)
            pw_config.mode = WP_EMPHY_TRANS_FRAMED | WP_EMPHY_TRANS_E3;
         else
         {
            WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
            return;
         }
      }
   }
   else if (the_system->ufe.fpga_mode == WT_SBI)
   {
      if (e1t1_index >= MAX_UFE_LINES_USED)
      {
         WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
         return;
      }
      if (the_system->ufe.line_params[e1t1_index].cas_enable == WUFE_CAS_ENABLE)
      {
         if (val1 == WUFE_CAD_TYPE_T1 || val1 == WUFE_SBI_TYPE_T1)
            pw_config.mode = WP_EMPHY_TRANS_FRAMED_CAS | WP_EMPHY_TRANS_T1;
         else if(val1 == WUFE_CAD_TYPE_E1 || val1 == WUFE_SBI_TYPE_E1)
            pw_config.mode = WP_EMPHY_TRANS_FRAMED_CAS | WP_EMPHY_TRANS_E1;
         else
         {
            WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
            return;
         }
      }
      else
      {
         if (val1 == WUFE_CAD_TYPE_T1 || val1 == WUFE_SBI_TYPE_T1)
            pw_config.mode = WP_EMPHY_TRANS_FRAMED | WP_EMPHY_TRANS_T1;
         else if(val1 == WUFE_CAD_TYPE_E1 || val1 == WUFE_SBI_TYPE_E1)
            pw_config.mode = WP_EMPHY_TRANS_FRAMED | WP_EMPHY_TRANS_E1;
         else if(val1 == WUFE_CAD_TYPE_DS3 || val1 == WUFE_SBI_TYPE_DS3)
            pw_config.mode = WP_EMPHY_TRANS_FRAMED | WP_EMPHY_TRANS_DS3;
         else if(val1 == WUFE_CAD_TYPE_E3 || val1 == WUFE_SBI_TYPE_E3)
            pw_config.mode = WP_EMPHY_TRANS_FRAMED | WP_EMPHY_TRANS_E3;
         else
         {
            WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
            return;
         }
      }
   }
   else
   {
      if (e1t1_index >= MAX_UFE_LINES_USED)
      {
         WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
         return;
      }
      if (the_system->ufe.line_params[e1t1_index].cas_enable == WUFE_CAS_ENABLE)
      {
         if (val1 == WUFE_SONET_TYPE_T1 || val1 == WUFE_SDH_TYPE_T1)
            pw_config.mode = WP_EMPHY_TRANS_FRAMED_CAS | WP_EMPHY_TRANS_T1;
         else if(val1 == WUFE_SDH_TYPE_E1 || val1 == WUFE_SDH_TYPE_E1)
            pw_config.mode = WP_EMPHY_TRANS_FRAMED_CAS | WP_EMPHY_TRANS_E1;
         else
         {
            WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
            return;
         }
      }
      else
      {
         if (val1 == WUFE_SONET_TYPE_T1 || val1 == WUFE_SDH_TYPE_T1)
            pw_config.mode = WP_EMPHY_TRANS_FRAMED | WP_EMPHY_TRANS_T1;
         else if(val1 == WUFE_SONET_TYPE_E1 || val1 == WUFE_SDH_TYPE_E1)
            pw_config.mode = WP_EMPHY_TRANS_FRAMED | WP_EMPHY_TRANS_E1;
         else if(val1 == WUFE_SONET_TYPE_DS3 || val1 == WUFE_SDH_TYPE_DS3)
            pw_config.mode = WP_EMPHY_TRANS_FRAMED | WP_EMPHY_TRANS_DS3;
         else if(val1 == WUFE_SONET_TYPE_E3 || val1 == WUFE_SDH_TYPE_E3)
            pw_config.mode = WP_EMPHY_TRANS_FRAMED | WP_EMPHY_TRANS_E3;
         else
         {
            WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
            return;
         }
      }
   }


#if WTI_DEBUG_LEVEL > 1
   printf("UFE PHY CONFIGURATION:\n");
   printf("-----------------------\n\n");
   printf("MODE ");
   if (val1 == WUFE_CAD_TYPE_T1 || val1 == WUFE_SBI_TYPE_T1 ||
       val1 == WUFE_SONET_TYPE_T1 || val1 == WUFE_SDH_TYPE_T1) /* T1 */
   {
      printf("T1 FRAMED\nT1 INDEX ");
   }
   else if(val1 == WUFE_CAD_TYPE_E1 || val1 == WUFE_SBI_TYPE_E1 ||
           val1 == WUFE_SONET_TYPE_E1 || val1 == WUFE_SDH_TYPE_E1) /* E1 */
   {
      printf("E1 FRAMED\nE1 INDEX ");
   }
   else if(val1 == WUFE_CAD_TYPE_DS3 || val1 == WUFE_SBI_TYPE_DS3 ||
           val1 == WUFE_SONET_TYPE_DS3 || val1 == WUFE_SDH_TYPE_DS3) /* DS3 */
   {
      printf("DS3 FRAMED\nDS3 INDEX ");
   }
   else if(val1 == WUFE_CAD_TYPE_E3 || val1 == WUFE_SBI_TYPE_E3 ||
           val1 == WUFE_SONET_TYPE_E3 || val1 == WUFE_SDH_TYPE_E3) /* E3 */
   {
      printf("E3 FRAMED\nE3 INDEX ");
   }
   else
   {
      WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
      return;
   }

   printf("e1t1_index = %d\n",e1t1_index);

   printf("NUMBER OF SLOTS %d\n",num_slots);
   for (i=0;i<num_slots;++i)
   {
      printf("DS0 -> %d\n",slots[i]);
   }
   printf("RX DATAUNIT SIZE %d\n",rx_dataunit_size);
   printf("TX DATAUNIT SIZE %d\n\n",tx_dataunit_size);
   printf("CAS MODE: ");
   if (the_system->ufe.line_params[e1t1_index].cas_enable == WUFE_CAS_ENABLE)
      printf("ENABLED\n");
   else
      printf("DISABLED\n");
#endif

   pw_config.spe_id = spe_id;
   pw_config.line_index = e1t1_index;
   if (num_of_params == 6)
      pw_config.phy_index = total_number_of_pw;
   else /* num_of_params = 7 */
      pw_config.phy_index = pw_index;
   pw_config.n_slots = num_slots;

   memcpy(pw_config.slots, slots, (num_slots * sizeof(WP_U32)));
   pw_config.rx_data_unit_size = rx_dataunit_size;
   pw_config.tx_data_unit_size = tx_dataunit_size;

   return;
}

#endif

#if WTI_CESOP_TDI

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_TdiShowInfo(char *StrPrm)
{
   WP_U32 i;

   printf("TDM INTERFACES CONFIGURATION\n\n");

   for (i=0;i<WTI_ACTIVE_TDI_PORTS;++i)
   {
      printf("TDI %d CONFIG:\n", i);
      if (tdm_config[i].op_mode == 0)
         printf("Operation mode - (WP_TDM_TRANS_T1 | WP_TDM_TRANS_PWE3)\n");
      else
         printf("Operation mode - (WP_TDM_TRANS_E1 | WP_TDM_TRANS_PWE3)\n");
      if (tdm_config[i].framing_mode == 0)
         printf("Framing mode - UNFRAMED\n");
      else
         printf("Framing mode - FRAMED\n");
      if (tdm_config[i].cas_mode == 0)
         printf("CAS mode - NO CAS\n");
      else
         printf("CAS mode - CAS\n");
   }

   return;
}

/*****************************************************************************
 * Function name:
 * Description  :
 * Input  params:
 * Output params:
 * Return val   :
 *****************************************************************************/
void CLI_F_TdiConfigInit(char *StrPrm)
{
   WP_U32 i;

   for (i=0;i<WTI_ACTIVE_TDI_PORTS;++i)
   {
      tdm_config[i].op_mode = 1;
      tdm_config[i].framing_mode = 1;
      if (i<8)
         tdm_config[i].cas_mode = 1;
      else
         tdm_config[i].cas_mode = 0;
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_TdiOperationMode(char *StrPrm)
{
   WP_U32 res, cmd, val, val1;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &val, &val1);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= WTI_ACTIVE_TDI_PORTS)
   {
      WTI_TerminatePrintError("CLI_F_TdiOperationMode", __LINE__);
      return;
   }
   if (val1 != 0 && val1 != 1)
   {
      WTI_TerminatePrintError("CLI_F_TdiOperationMode", __LINE__);
      return;
   }

   tdm_config[val].op_mode = val1;


   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_TdiFramingMode(char *StrPrm)
{
   WP_U32 res, cmd, val, val1;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &val, &val1);
   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= WTI_ACTIVE_TDI_PORTS)
   {
      WTI_TerminatePrintError("CLI_F_TdiFramingMode", __LINE__);
      return;
   }

   if (val1 != 0 && val1 != 1)
   {
      WTI_TerminatePrintError("CLI_F_TdiFramingMode", __LINE__);
      return;
   }

   tdm_config[val].framing_mode = val1;


   return;
}


/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_TdiCasMode(char *StrPrm)
{
   WP_U32 res, cmd, val, val1;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &val, &val1);
   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= WTI_ACTIVE_TDI_PORTS)
   {
      WTI_TerminatePrintError("CLI_F_TdiCasMode", __LINE__);
      return;
   }
   if (val1 != 0 && val1 != 1)
   {
      WTI_TerminatePrintError("CLI_F_TdiCasMode", __LINE__);
      return;
   }

   tdm_config[val].cas_mode = val1;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_PwConfigureUnframedTdi(char *StrPrm)
{
   WP_U32 res, cmd, port_index, pw_index, transfer_mode, num_slots, i;
   WP_U32 slots[WTI_E1_SLOTS], num_of_params;
   WP_CHAR *params, tmp_str[WTI_MAX_STRING_SIZE];

   /* check the number of parameters sent to function */
   num_of_params=0;
   strcpy(tmp_str, StrPrm);
   params = strtok(tmp_str, " ");
   while (params != NULL)
   {
      params = strtok(NULL, " ");
      num_of_params++;
   }
   if (num_of_params == 4)
   {
      res = sscanf(StrPrm, "%d %d %d %d", &cmd, &port_index, &pw_index, &transfer_mode);
      if (res != 4)
      {
         WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
         return;
      }
      if (pw_index >= WTI_MAX_PW)
      {
         WTI_TerminatePrintError("CLI_F_PwConfigureUnframedTdi", __LINE__);
         return;
      }
   }
   else if (num_of_params == 3)
   {
      res = sscanf(StrPrm, "%d %d %d", &cmd, &port_index, &transfer_mode);
      if (res != 3)
      {
         WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
         return;
      }
   }
   else /* num_of_params != 3 and num_of_params !=4 => exit */
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (port_index >= WTI_ACTIVE_TDI_PORTS)
   {
      WTI_TerminatePrintError("CLI_F_PwConfigureUnframedTdi", __LINE__);
      return;
   }
   if (transfer_mode > 3)
   {
      WTI_TerminatePrintError("CLI_F_PwConfigureUnframedTdi", __LINE__);
      return;
   }

   if (transfer_mode == 0) /* T1 */
   {
      num_slots = WTI_T1_SLOTS;
   }
   else if (transfer_mode == 1) /* E1 */
   {
      num_slots = WTI_E1_SLOTS;
   }
   else if (transfer_mode == 2) /* DS3 */
   {
      num_slots = WTI_DS3_SLOTS;
   }
   else if (transfer_mode == 3) /* DS3 */
   {
      num_slots = WTI_DS3_SLOTS;
   }

   for (i=0;i<num_slots;++i)
      slots[i] = i;

   /* mark slots as occupied by saving the PW index */
   for (i=0;i<num_slots;++i)
   {
      if (num_of_params == 3)
         slots_config[port_index*WTI_E1_SLOTS+slots[i]] = total_number_of_pw;
      else /* num_of_params = 4 */
         slots_config[port_index*WTI_E1_SLOTS+slots[i]] = pw_index;
   }


#if WTI_DEBUG_LEVEL > 1
   /* print PW configuration */
   printf("PW CONFIGURATION:\n");
   printf("-----------------------\n\n");
   printf("MODE ");
   if (transfer_mode == 0)
   {
      printf("T1 INDEX ");
   }
   else if (transfer_mode == 1)
   {
      printf("E1 INDEX ");
   }
   else if (transfer_mode == 2)
   {
      printf("DS3 INDEX ");
   }
   else if (transfer_mode == 3)
   {
      printf("E3 INDEX ");
   }

   printf("port_index %d\n",port_index);
   printf("CONFIGURED TO UNFRAMED MODE\n\n");
#endif

   /* save PW params */
   pw_config.line_index = port_index;
   pw_config.n_slots = num_slots;
   memcpy(pw_config.slots,slots,num_slots*4);

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_PwConfigureFramedTdi(char *StrPrm)
{
   WP_U32 res, cmd, port_index, pw_index, transfer_mode, num_slots, i, num_of_params;
   WP_CHAR slots_array[WTI_MAX_STRING_SIZE];
   WP_CHAR *slot, *params, tmp_str[WTI_MAX_STRING_SIZE];
   WP_U32 slots[WTI_E1_SLOTS];

   /* check the number of parameters sent to function */
   num_of_params=0;
   strcpy(tmp_str, StrPrm);
   params = strtok(tmp_str, " ");
   while (params != NULL)
   {
      params = strtok(NULL, " ");
      num_of_params++;
   }
   if (num_of_params == 6)
   {
      res = sscanf(StrPrm, "%d %d %d %d %d %s", &cmd, &port_index, &pw_index, &transfer_mode,
                   &num_slots, slots_array);
      if (res != 6)
      {
         WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
         return;
      }
      if (pw_index >= WTI_MAX_PW)
      {
         WTI_TerminatePrintError("CLI_F_PwConfigureFramedTdi - pw index", __LINE__);
         return;
      }
   }
   else if (num_of_params == 5)
   {
      res = sscanf(StrPrm, "%d %d %d %d %s", &cmd, &port_index, &transfer_mode,
                   &num_slots, slots_array);
      if (res != 5)
      {
         WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
         return;
      }
   }
   else /* num_of_params != 5 and num_of_params !=6 => exit */
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (port_index >= WTI_ACTIVE_TDI_PORTS)
   {
      WTI_TerminatePrintError("CLI_F_PwConfigureFramedTdi", __LINE__);
      return;
   }
   if (transfer_mode != 0 && transfer_mode != 1)
   {
      WTI_TerminatePrintError("CLI_F_PwConfigureFramedTdi", __LINE__);
      return;
   }

   if (((transfer_mode == 0) && (num_slots < 1 || num_slots > WTI_T1_SLOTS))|| /* T1 */
       ((transfer_mode == 1) && (num_slots < 1 || num_slots > WTI_E1_SLOTS))|| /* E1 */
       ((transfer_mode == 2) && (num_slots < 1 || num_slots > WTI_DS3_SLOTS))|| /* DS3 */
       ((transfer_mode == 3) && (num_slots < 1 || num_slots > WTI_E3_SLOTS))) /* E3 */
   {
      WTI_TerminatePrintError("CLI_F_PwConfigureFramedTdi", __LINE__);
      return;

   }

   i=0;
   if (strstr(slots_array,","))
   {
      slot = strtok(slots_array,",");

      while (slot)
      {
         slots[i++] = atoi(slot);
         slot = strtok(NULL,",");
      }
      if (i!=num_slots)
      {
         WTI_TerminatePrintError("CLI_F_PwConfigureFramedTdi", __LINE__);
         return;
      }
   }
   else if (strstr(slots_array,"-"))
   {
      slot = strtok(slots_array,"-");
      if (slot)
      {
         WP_U32 j=0;

         slots[0] = atoi(slot);
         slot = strtok(NULL,"-");
         j = atoi(slot);
         if (slots[0] + num_slots - 1 != j)
         {
            WTI_TerminatePrintError("CLI_F_PwConfigureFramedTdi", __LINE__);
            return;
         }
         else
         {
            for (i=1;i<num_slots;++i)
            {
               slots[i] = slots[i-1] + 1;
            }
         }
      }
      else
      {
         WTI_TerminatePrintError("CLI_F_PwConfigureFramedTdi", __LINE__);
         return;
      }
   }
   else
   {
      if (num_slots != 1)
      {
         WTI_TerminatePrintError("CLI_F_PwConfigureFramedTdi", __LINE__);
         return;
      }
      slots[0] = atoi(slots_array);
   }

   for (i=0;i<num_slots;++i)
   {
      switch (transfer_mode)
      {
         case 0: /* T1 */
         {
            if (slots[i]>=WTI_T1_SLOTS)
            {
               WTI_TerminatePrintError("CLI_F_PwConfigureFramedTdi", __LINE__);
               return;
            }
            break;
         }
         case 1: /* E1 */
         {
            if (slots[i]>=WTI_E1_SLOTS)
            {
               WTI_TerminatePrintError("CLI_F_PwConfigureFramedTdi", __LINE__);
               return;
            }
            break;
         }
         case 2: /* DS3 */
         {
            if (slots[i]>=WTI_DS3_SLOTS)
            {
               WTI_TerminatePrintError("CLI_F_PwConfigureFramedTdi", __LINE__);
               return;
            }
            break;
         }
         case 3: /* E3 */
         {
            if (slots[i]>=WTI_E3_SLOTS)
            {
               WTI_TerminatePrintError("CLI_F_PwConfigureFramedTdi", __LINE__);
               return;
            }
            break;
         }
         default:
         {
            WTI_TerminatePrintError("CLI_F_PwConfigureFramedTdi", __LINE__);
            return;
         }

      }
      if (slots_config[port_index*WTI_E1_SLOTS+slots[i]] != WTI_UNUSED_SLOT) /* slot already occupied */
      {
         WTI_TerminatePrintError("CLI_F_PwConfigureFramedTdi", __LINE__);
         return;
      }
   }

   /* mark slots as occupied by saving the PW index*/
   for (i=0;i<num_slots;++i)
   {
      if (num_of_params == 5)
         slots_config[port_index*WTI_E1_SLOTS+slots[i]] = total_number_of_pw;
      else /* num_of_params = 6 */
         slots_config[port_index*WTI_E1_SLOTS+slots[i]] = pw_index;
   }


#if WTI_DEBUG_LEVEL > 1
   printf("PW CONFIGURATION:\n");
   printf("-----------------------\n\n");
   printf("MODE ");
   if (transfer_mode == 0) /* T1 */
   {
      printf("T1 FRAMED\nT1 INDEX ");
   }
   else if (transfer_mode == 1) /* E1 */
   {
      printf("E1 FRAMED\nE1 INDEX ");
   }
   else if (transfer_mode == 2) /* DS3 */
   {
      printf("E1 FRAMED\nDS3 INDEX ");
   }
   else if (transfer_mode == 3) /* E3 */
   {
      printf("E1 FRAMED\nE3 INDEX ");
   }

   printf("port_index %d\n",port_index);

   printf("NUMBER OF SLOTS %d\n",num_slots);
   for (i=0;i<num_slots;++i)
   {
      printf("DS0 -> %d\n",slots[i]);
   }
   printf("CAS MODE: ");
   if (tdm_config[port_index].cas_mode == WTI_TDI_CAS_MODE)
      printf("ENABLED\n");
   else
      printf("DISABLED\n");
#endif

   pw_config.line_index = port_index;
   pw_config.n_slots = num_slots;
   memcpy(pw_config.slots, slots, num_slots*4);

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_TdiTransDeviceModify(char *StrPrm)
{
   WP_U32 res, cmd, val, mod, index;
   WP_command command;
   WP_status status;

   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &index, &mod, &val);

   if (res != 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (index >= WTI_ACTIVE_TDI_PORTS)
   {
      WTI_TerminatePrintError("CLI_F_TdiTransDeviceModify", __LINE__);
      return;
   }
   if (mod != 1)
   {
      WTI_TerminatePrintError("CLI_F_TdiTransDeviceModify", __LINE__);
      return;
   }
   if (val !=0 && val != 1)
   {
      WTI_TerminatePrintError("CLI_F_TdiTransDeviceModify", __LINE__);
      return;
   }

   switch (mod)
   {
      case 1:
      {
         command = WP_DEV_MOD_TDM_TRANS_CES_CLOCK_REC;
         WTI_TdiTransDeviceClockRecModify(index, val);
         break;
      }
      default:
         command = 0;
         break;
   }

   if (command > 0)
   {
      printf("before WP_DeviceModify: pwe3_characteristics = %x\n",
             (((WP_device_tdm_trans_ces *)((&trans_device_config[index])->protocol_specific))->pwe3_ext)->pwe3_characteristics);

      status = WP_DeviceModify(the_system->tdm_trans_dev[index],
                               command,
                               &trans_device_config[index]); /* modified in TdiTransDeviceClockRecModify */
      WTI_TerminateOnError(status, "WP_DeviceModify",__LINE__);
   }
   else
   {
      printf("---- command is 0 for WP_DeviceModify line: %d\n", __LINE__);
   }

   return;


}


#if(WTI_CESOP_RX_TIMING_ENABLE ||( WTI_CESOP_CLOCK_RECOVERY_ENABLE && WTI_CLOCK_REC_MODE))

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_TdiCrTSInt(char *StrPrm)
{
   WP_U32 res,cmd,ts_buffer_size;
   WP_U32 *addr;;
   WP_U32 e1t1,e1t1_index;

   res = sscanf(StrPrm, "%d %d %d",
                &cmd,
                &e1t1 ,&e1t1_index);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (e1t1 != 1 && e1t1 != 2 && e1t1 != 3 && e1t1 != 4)
   {
      WTI_TerminatePrintError("CLI_F_TdiCrTSInt", __LINE__);
      return;
   }

   if (e1t1_index > WTI_ACTIVE_TDI_PORTS)
   {
      WTI_TerminatePrintError("CLI_F_TdiCrTSInt", __LINE__);
      return;
   }
   /* chip select  */
   addr = (WP_U32 *)WPL_PHY_VIRTUAL(0,CR_CHIP_SELECT_ADD);
   *addr = CR_CHIP_SELEC_VAL ; /*chip select*/

   /* Int TDI timemstamp  registers */
   /* (1-T1_FRM, 2-T1_UNFRM 3-E1_FRM 4-E1_UNFRM */

#if WTI_CESOP_DIFF_DOCSIS_MODE

/* DOCSIS mode - E1 = 1061 , T1 = 800 */
   if((e1t1 == 1)||(e1t1 == 2))
   {
      /* T1 */
      ts_buffer_size = 800-1;
   }
   else
   {
      /* E1 */
      ts_buffer_size = 1061-1;
   }


#else

/* Normal mode  - ts_buffer_size = number of line clock required in order to fill the TDI buffer */
/* all unf = 0xff, E1 framed = 0xff, T1 framed = 0xc0 */
   if(e1t1 == 1)
   {
      /* T1 framed */
      ts_buffer_size = (24*8+1)-1;
   }
   else
   {
      /* T1/E1 unframed,E1 framed */
      ts_buffer_size =  32*8-1;
   }
#endif

   /* RX direction */
   addr = (WP_U32 *)WPL_PHY_VIRTUAL(0,WTI_TDI_RX_TS_BASE_ADDRESS + e1t1_index*0x40 + 0x10);
   *(WP_U16 *) addr = ts_buffer_size;

   /* TX direction */
   addr = (WP_U32 *)WPL_PHY_VIRTUAL(0,WTI_TDI_TX_TS_BASE_ADDRESS + e1t1_index*0x40 + 0x10);
   *(WP_U16 *) addr = ts_buffer_size;

   return;
}
#endif /* (WTI_CESOP_RX_TIMING_ENABLE || ( WTI_CESOP_CLOCK_RECOVERY_ENABLE && WTI_CLOCK_REC_MODE)) */


#else
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_PwConfigureUnframed(char *StrPrm)
{
   WP_U32 res, cmd, val1, e1t1_index, pw_index, num_slots=0, i, rx_dataunit_size, tx_dataunit_size;
   WP_U32 num_of_params, spe_id=0;
   WP_U32 slot_index=0;
   WP_U32 slots[WTI_E1_SLOTS];
   WP_CHAR *params, tmp_str[WTI_MAX_STRING_SIZE];

   /* check the number of parameters sent to function */
   num_of_params=0;
   strcpy(tmp_str, StrPrm);
   params = strtok(tmp_str, " ");
   while (params != NULL)
   {
      params = strtok(NULL, " ");
      num_of_params++;
   }
   if (num_of_params == 5)
   {
      res = sscanf(StrPrm, "%d %d %d %d %d",
                   &cmd, &e1t1_index, &pw_index, &rx_dataunit_size, &tx_dataunit_size);
      if (res != 5)
      {
         WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
         return;
      }
      if (pw_index >= WTI_MAX_PW)
      {
         WTI_TerminatePrintError("CLI_F_PwConfigureUnframed", __LINE__);
         return;
      }
   }
   else if (num_of_params == 4)
   {
      res = sscanf(StrPrm, "%d %d %d %d", &cmd, &e1t1_index, &rx_dataunit_size, &tx_dataunit_size);
      if (res != 4)
      {
         WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
         return;
      }
   }
   else /* num_of_params != 4 and num_of_params !=5 => exit */
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (rx_dataunit_size < WTI_MIN_DATAUNIT_SIZE ||
       rx_dataunit_size > WTI_MAX_DATAUNIT_SIZE)
   {
      WTI_TerminatePrintError("CLI_F_PwConfigureUnframed", __LINE__);
      return;
   }
   if (tx_dataunit_size < WTI_MIN_DATAUNIT_SIZE ||
       tx_dataunit_size > WTI_MAX_DATAUNIT_SIZE)
   {
      WTI_TerminatePrintError("CLI_F_PwConfigureUnframed", __LINE__);
      return;
   }

   if (e1t1_index >= MAX_UFE_LINES_USED)
   {
      WTI_TerminatePrintError("CLI_F_PwConfigureUnframed", __LINE__);
      return;
   }
   /* get line id or spe id */
   if (the_system->ufe.fpga_mode == WT_OCTAL)
   {
      if (e1t1_index >= WTI_MAX_OCTAL_LINES)
      {
         WTI_TerminatePrintError("CLI_F_PwConfigureUnframed - E1/T1 index", __LINE__);
         return;
      }

      /* line id */
      spe_id = e1t1_index;

      if (the_system->ufe.line_params[e1t1_index].framed == WUFE_FRAMED)
      {
         printf("ERROR - Invalid configuration: %s\n",
                "UFE line is not configured to UNFRAMED mode");
         return;
      }
      val1 = the_system->ufe.line_params[e1t1_index].transfer_type;
   }
   else if (the_system->ufe.fpga_mode == WT_SBI)
   {
      /* get spe_id */
      spe_id = WTI_SpeIdGet(e1t1_index);

      if (spe_id > (WUFE_MAX_N_SBI_SPE-1))
      {
         printf("CLI_F_PwConfigureUnFramed: spe_id invalid\n");
         return;
      }
      /*each SPE contains 28 reserved indexes */
      if (the_system->ufe.sbi_spe_line_type[spe_id] == WUFE_SBI_TYPE_T1)
         slot_index = (e1t1_index - WTI_T1_LINES_PER_SPE*spe_id) * WTI_T1_SLOTS;
      else if(the_system->ufe.sbi_spe_line_type[spe_id] == WUFE_SBI_TYPE_E1)
         slot_index = (e1t1_index - WTI_T1_LINES_PER_SPE*spe_id) * WTI_E1_SLOTS;
      else if(the_system->ufe.sbi_spe_line_type[spe_id] == WUFE_SBI_TYPE_DS3)
         slot_index = (e1t1_index - WTI_T1_LINES_PER_SPE*spe_id) * WTI_DS3_SLOTS;
      else if(the_system->ufe.sbi_spe_line_type[spe_id] == WUFE_SBI_TYPE_E3)
         slot_index = (e1t1_index - WTI_T1_LINES_PER_SPE*spe_id) * WTI_E3_SLOTS;

      val1 = the_system->ufe.sbi_spe_line_type[spe_id];

   }
   else
   {
      val1 = the_system->ufe.line_params[e1t1_index].transfer_type;

      /*each SPE contains 28 reserved indexes */
      if (val1 == WUFE_SONET_TYPE_T1 || val1 == WUFE_SDH_TYPE_T1)
         slot_index = e1t1_index * WTI_T1_SLOTS;
      else if (val1 == WUFE_SONET_TYPE_E1 || val1 == WUFE_SDH_TYPE_E1)
         slot_index = e1t1_index * WTI_E1_SLOTS;
      else if (val1 == WUFE_SONET_TYPE_DS3 || val1 == WUFE_SDH_TYPE_DS3)
         slot_index = e1t1_index * WTI_DS3_SLOTS;
      else if (val1 == WUFE_SONET_TYPE_E3 || val1 == WUFE_SDH_TYPE_E3)
         slot_index = e1t1_index * WTI_E3_SLOTS;
   }

   if (val1 == WUFE_CAD_TYPE_T1 || val1 == WUFE_SBI_TYPE_T1 ||
       val1 == WUFE_SONET_TYPE_T1 || val1 == WUFE_SDH_TYPE_T1) /* T1 */
   {
      num_slots = WTI_T1_SLOTS;
   }

   else if (val1 == WUFE_CAD_TYPE_E1 || val1 == WUFE_SBI_TYPE_E1 ||
            val1 == WUFE_SONET_TYPE_E1 || val1 == WUFE_SDH_TYPE_E1)/* E1 */
   {
      num_slots = WTI_E1_SLOTS;
   }

   else if (val1 == WUFE_CAD_TYPE_DS3 || val1 == WUFE_SBI_TYPE_DS3 ||
            val1 == WUFE_CAD_TYPE_E3 || val1 == WUFE_SBI_TYPE_E3 ||
            val1 == WUFE_SONET_TYPE_DS3 || val1 == WUFE_SDH_TYPE_DS3 ||
            val1 == WUFE_SONET_TYPE_E3 || val1 == WUFE_SDH_TYPE_E3)
   {
      num_slots = WTI_DS3_SLOTS;
   }
   else if (val1 == WUFE_SDH_TYPE_VC4 || val1 == WUFE_SONET_TYPE_STS3) /* clear channel */
   {
      num_slots = 1;
   }
   else if (val1 == WUFE_SDH_TYPE_VC44C || val1 == WUFE_SONET_TYPE_STS12) /* clear channel */
   {
      num_slots = 1;
   }
   else
   {
      printf("CLI_F_PwConfigureUnFramed: spe_id invalid\n");
      return;
   }

   for (i=0;i<num_slots;++i)
      slots[i] = i;

   /* mark slots as occupied by saving the PW index */
   for (i=0;i<num_slots;++i)
   {
      if (num_of_params == 4)
         slots_config[slot_index + slots[i]] = total_number_of_pw;
      else /* num_of_params = 5 */
         slots_config[slot_index + slots[i]] = pw_index;
   }


#if WTI_DEBUG_LEVEL > 1
   /* print PW configuration */
   printf("UFE PHY CONFIGURATION:\n");
   printf("-----------------------\n\n");
   printf("MODE ");
   if (val1 == WUFE_CAD_TYPE_T1 ||
       val1 == WUFE_SBI_TYPE_T1 ||
       val1 == WUFE_SONET_TYPE_T1 ||
       val1 == WUFE_SDH_TYPE_T1)
   {
      printf("T1 UNFRAMED\nT1 INDEX ");
   }
   else if(val1 == WUFE_CAD_TYPE_DS3 ||
           val1 == WUFE_SBI_TYPE_DS3 ||
           val1 == WUFE_SONET_TYPE_DS3 ||
           val1 == WUFE_SDH_TYPE_DS3)
   {
      printf("DS3 UNFRAMED\nDS3 INDEX ");
   }
   else if(val1 == WUFE_CAD_TYPE_E3 ||
           val1 == WUFE_SBI_TYPE_E3 ||
           val1 == WUFE_SONET_TYPE_E3 ||
           val1 == WUFE_SDH_TYPE_E3)
   {
      printf("E3 UNFRAMED\nE3 INDEX ");
   }
   else if (val1 == WUFE_CAD_TYPE_E1 ||
            val1 == WUFE_SBI_TYPE_E1 ||
            val1 == WUFE_SONET_TYPE_E1 ||
            val1 == WUFE_SDH_TYPE_E1)
   {
      printf("E1 UNFRAMED\nE1 INDEX ");
   }
   else if (val1 == WUFE_SDH_TYPE_VC4)
   {
      printf("VC4 CLEAR CHANNEL\n INDEX ");
   }
   else if (val1 == WUFE_SONET_TYPE_STS3)
   {
      printf("STS3 CLEAR CHANNEL\n INDEX ");
   }
   else if (val1 == WUFE_SDH_TYPE_VC44C)
   {
      printf("VC44C CLEAR CHANNEL\n INDEX ");
   }
   else if (val1 == WUFE_SONET_TYPE_STS12)
   {
      printf("STS12 CLEAR CHANNEL\n INDEX ");
   }
   printf("e1t1_index %d\n",e1t1_index);
   printf("CONFIGURED TO UNFRAMED MODE\n\n");
#endif

   /* save PHY params */
   if (val1 == WUFE_CAD_TYPE_T1 || val1 == WUFE_SBI_TYPE_T1 ||
       val1 == WUFE_SONET_TYPE_T1 || val1 == WUFE_SDH_TYPE_T1)
      pw_config.mode = WP_EMPHY_TRANS_UNFRAMED | WP_EMPHY_TRANS_T1;
   else if( val1 == WUFE_CAD_TYPE_E1 || val1 == WUFE_SBI_TYPE_E1 ||
            val1 == WUFE_SONET_TYPE_E1 || val1 == WUFE_SDH_TYPE_E1)
      pw_config.mode = WP_EMPHY_TRANS_UNFRAMED | WP_EMPHY_TRANS_E1;
   else if( val1 == WUFE_CAD_TYPE_DS3 || val1 == WUFE_SBI_TYPE_DS3 ||
            val1 == WUFE_SONET_TYPE_DS3 || val1 == WUFE_SDH_TYPE_DS3)
      pw_config.mode = WP_EMPHY_TRANS_UNFRAMED | WP_EMPHY_TRANS_DS3;
   else if( val1 == WUFE_CAD_TYPE_E3 || val1 == WUFE_SBI_TYPE_E3 ||
            val1 == WUFE_SONET_TYPE_E3 || val1 == WUFE_SDH_TYPE_E3)
      pw_config.mode = WP_EMPHY_TRANS_UNFRAMED | WP_EMPHY_TRANS_E3;
   else if( val1 == WUFE_SDH_TYPE_VC4 || val1 == WUFE_SONET_TYPE_STS3 ||
      val1 == WUFE_SDH_TYPE_VC44C || val1 == WUFE_SONET_TYPE_STS12)
      pw_config.mode = WP_EMPHY_TRANS_UNFRAMED | WP_EMPHY_TRANS_E3;// patch
   else
   {
      WTI_TerminatePrintError("CLI_F_PwConfigureFramed", __LINE__);
      return;
   }

   pw_config.spe_id = spe_id;
   pw_config.line_index = e1t1_index;
   if (num_of_params == 4)
      pw_config.phy_index = total_number_of_pw;
   else /* num_of_params = 5 */
      pw_config.phy_index = pw_index;
   pw_config.n_slots = num_slots;
   memcpy(pw_config.slots,slots,num_slots);
   pw_config.rx_data_unit_size = rx_dataunit_size;
   pw_config.tx_data_unit_size = tx_dataunit_size;

   return;
}

void CLI_F_TemuxDump(char *StrPrm)
{
#ifdef __WT_UFE3__
   WP_U32 status;

   status = WPX_PmcTmx336RegisterDump(0, 0);
   if (status)
   {
      printf("WPX_PmcTmx336RegisterDump return error %x\n", status);
      exit(1);
   }
#endif
}

#if defined  __WT_UFE412__ || defined __WT_UFE448__
void CLI_F_UfeZarlinkPllClockOutConfig(char *StrPrm)
{
   WUFEI_pll_cfg pll_cfg;

   WP_U32 cmd, clock, res, value;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &clock, &value);

   if(res != 3)
   {
      printf("%s Wrong number of arguments.\n",STR_MENU_ERR);
      return;
   }

   switch(clock)
   {
      case 0: //p0_clk0
         pll_cfg.clock_out = p0_clk0;
         break;
      case 1: //p1_clk0
         pll_cfg.clock_out = p1_clk0;
         break;
      case 2: //APLL_CLK0
         pll_cfg.clock_out = apll_clk0;
         if(value >= f_77_76 && value <= f_12_5)
         {
            pll_cfg.apll_rate = value;
         }
         else
         {
            printf("%s Value not in range of the apll rates.\n",STR_MENU_ERR);
            printf("f_77_76 = 0\nf_38_88 = 1\nf_19_44 = 2\nf_9_72 = 3\nf_51_84 = 4\nf_25_92 = 5\nf_12_96 = 6\nf_6_48 = 7\nf_125 = 8\nf_62_5 = 9\nf_50 = 10\nf_25 = 11\nf_12_5 = 12\n");
            return;
         }
         break;
   }

   pll_cfg.clock_rate = value;
   pll_cfg.ufeid = 0;
   WUFE_BsUfeExternalPllClockOutConfig(&pll_cfg);
}

void CLI_F_UfeReadZarlinkPllReg(char *StrPrm)
{
   WP_U32 cmd, address, res;
   res = sscanf(StrPrm, "%d %x", &cmd, &address);

   if(res != 2)
   {
      printf("Wrong number of params, enter register address in hex\n");
      return;
   }

   printf("Reg address: 0x%x Value: 0x%x\n", address, WUFE_BSReadExtPllReg(address));
}
#endif

void CLI_F_UfeGpioClockSelect(char *StrPrm)
{
#if defined __WT_UFE412__ || defined __WT_UFE448__
   WP_U32 res, cmd, gpio_select, clock_type, ref_clk_out_select;

   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &gpio_select, &clock_type, &ref_clk_out_select);

   if (res != 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if(gpio_select != WUFE_GPIO_0 && gpio_select != WUFE_GPIO_1)
   {
      WTI_TerminatePrintError("CLI_F_UfeGpioClockSelect", __LINE__);
      return;
   }

   if (clock_type != WUFE_GPIO_CLK_OUT_RECV_PDH_TX_CLK_0    &&
       clock_type != WUFE_GPIO_CLK_OUT_RECV_PDH_TX_CLK_1    &&
       clock_type != WUFE_GPIO_CLK_OUT_RECV_PDH_LINE_CLK_0  &&
       clock_type != WUFE_GPIO_CLK_OUT_RECV_PDH_LINE_CLK_1  &&
       clock_type != WUFE_GPIO_CLK_OUT_GP_CLK_IN_0          &&
       clock_type != WUFE_GPIO_CLK_OUT_GP_CLK_IN_1          &&
       clock_type != WUFE_GPIO_CLK_OUT_SYSTEM_CLK           &&
       clock_type != WUFE_GPIO_CLK_OUT_XO_PLL_REF_CLK       &&
       clock_type != WUFE_GPIO_CLK_OUT_RECV_SDH_LINE_CLK_1)
   {
      WTI_TerminatePrintError("CLI_F_UfeGpioClockSelect", __LINE__);
      return;
   }
   if (ref_clk_out_select != WUFE_GPIO_REF_CLK_OUTPUT_XO_REF_CLK &&
       ref_clk_out_select != WUFE_GPIO_REF_CLK_OUTPUT_DIFF_REF_CLK)
   {
      WTI_TerminatePrintError("CLI_F_UfeGpioClockSelect", __LINE__);
      return;
   }
#if WTI_DUAL_EMPHY
   WT_UfeGpioOutputClockSet(&the_system->ufe, gpio_select, clock_type, ref_clk_out_select);
   WT_UfeGpioOutputClockSet(&the_system->second_ufe, gpio_select, clock_type, ref_clk_out_select);
#else
   WT_UfeGpioOutputClockSet(&the_system->ufe, gpio_select, clock_type, ref_clk_out_select);
#endif

#endif
}

void CLI_F_UfePDHClockSelect(char *StrPrm)
{
#if defined __WT_UFE412__ || defined __WT_UFE448__
#ifdef WT_UFE_FRAMER

   WP_U32 res, cmd, line_index=0,rate_cmd=0,clock_rate=0,output_select=0;

   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &output_select, &line_index,&rate_cmd);

   if (res != 3 && res != 4 )
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }


   if (res == 4 )
   {
      if(rate_cmd == 0)
      {
         clock_rate = RECOVERED_CLOCK_RATE_PARAMETER_2048;
      }
      else if(rate_cmd == 1)
      {
         clock_rate = RECOVERED_CLOCK_RATE_PARAMETER_1544;
      }
      else
      {
         WTI_TerminatePrintError("invalid rate", __LINE__);
         return;
      }
   }
   else
   {
      clock_rate = RECOVERED_CLOCK_RATE_PARAMETER_2048;  /* default value */
   }



   if(output_select != WTI_FLEXMUX_RECV_PDH_LINE_CLK_0 && output_select != WTI_FLEXMUX_RECV_PDH_LINE_CLK_1)
   {
      WTI_TerminatePrintError("CLI_F_UfePDHClockSelect", __LINE__);
      return;
   }

   if(line_index >= MAX_UFE_LINES_USED)
   {
      WTI_TerminatePrintError("CLI_F_UfePDHClockSelect Invalid line", __LINE__);
      return;
   }

   if( the_system->ufe.line_handle[line_index] == WT_UFE_HANDLE_INVALID)
   {
      WTI_TerminatePrintError("CLI_F_UfePDHClockSelect Invalid line", __LINE__);
      return;
   }

   WT_UFEPDHOutputClockSet(&the_system->ufe,output_select,line_index,clock_rate );

#endif  /* WT_UFE_FRAMER */
#endif  /*  __WT_UFE412__ || __WT_UFE448__ */
}



void CLI_F_UfeGpioClockRead(char *StrPrm)
{
#if defined __WT_UFE412__ || defined __WT_UFE448__
   WP_U32 res, cmd;
   WP_U16 gpio_register;

   res = sscanf(StrPrm, "%d", &cmd);

   if (res != 1)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   WT_UfeGpioOutputClockRead(&the_system->ufe, &gpio_register);

#endif
}

void CLI_F_UfeCheckSfp(char *StrPrm)
{
#if defined __WT_UFE412__ || defined __WT_UFE448__
   WUFE_status status = 0;
   WP_U8 data[256], data_to_write[256];
   WP_U32 i;

   for (i=0;i<256;++i)
   {
      data[i] = data_to_write[i] = 0;
   }

#if 1
   printf("Make sure SFP 155Mbd is located at slot #0 of UFE412 and press any key to continue\n");
   getchar();

//   ./sfp_test r 0xa1 0x14	// Reads address 0x14 of region 0xa1 for the plugged device
//id: 0 - data read: 44
   status = WUFE_SfpRead (the_system->ufe.ufe_id, 0, 1, 1, 0xa1, 0x14, data);
   if (status != 0)
   {
      printf("status is %x\n", status);
      WTI_TerminatePrintError("WUFE_SfpRead failed", __LINE__);
   }

   if (data[0] != 0x44)
      printf("invalid data read from SFP 0 is %x\n", data[0]);
   else
      printf("correct data read from SFP 0 is %x\n", data[0]);

//./sfp_test r 0xa1 0x2c // Reads address 0x2c of region 0xa1 for the plugged device
//id: 0 - data read: 36
   status = WUFE_SfpRead (the_system->ufe.ufe_id, 0, 1, 1, 0xa1, 0x2c, data);
   if (status != 0)
   {
      printf("status is %x\n", status);
      WTI_TerminatePrintError("WUFE_SfpRead failed", __LINE__);
   }

   if (data[0] != 0x31)
      printf("invalid data read from SFP 0 is %x\n", data[0]);
   else
      printf("correct data read from SFP 0 is %x\n", data[0]);

// plugging add'l device
//./sfp_test r 0xa1 0x2c  // Reads address 0x2c of region 0xa1 for the plugged devices
//id: 0 - data read: 36
//id: 1 - data read: 31

   printf("Replace SFP 155Mbd with 622 Mbd at slot #0 of UFE412 and press any key to continue\n");
   getchar();

   status = WUFE_SfpRead (the_system->ufe.ufe_id, 0, 1, 1, 0xa1, 0x2c, data);
   if (status != 0)
   {
      printf("status is %x\n", status);
      WTI_TerminatePrintError("WUFE_SfpRead failed", __LINE__);
   }

   if (data[0] != 0x36)
      printf("invalid data read from SFP 0 is %x\n", data[0]);
   else
      printf("correct data read from SFP 0 is %x\n", data[0]);

//echo write 0 to Eprom 0 at address 0x80
//./sfp_test W 0 0xa2 0x80 0
    data_to_write[0] = 0;
    status = WUFE_SfpWrite (the_system->ufe.ufe_id, 0, 1, 0xa2, 0x80, data_to_write);
    if (status != 0)
    {
       printf("status is %x\n", status);
       WTI_TerminatePrintError("WUFE_SfpWrite failed", __LINE__);
    }

 //echo write 1 to Eprom 0 at address 0x81
 //./sfp_test W 0 0xa2 0x81 1
    data_to_write[0] = 1;
    status = WUFE_SfpWrite (the_system->ufe.ufe_id, 0, 1, 0xa2, 0x81, data_to_write);
    if (status != 0)
    {
       printf("status is %x\n", status);
       WTI_TerminatePrintError("WUFE_SfpWrite failed", __LINE__);
    }
 //echo write 2 to Eprom 0 at address 0x82
 //./sfp_test W 0 0xa2 0x82 2
    data_to_write[0] = 2;
    status = WUFE_SfpWrite (the_system->ufe.ufe_id, 0, 1, 0xa2, 0x82, data_to_write);
    if (status != 0)
    {
       printf("status is %x\n", status);
       WTI_TerminatePrintError("WUFE_SfpWrite failed", __LINE__);
    }
 //echo write 3 to Eprom 0 at address 0x83
 //./sfp_test W 0 0xa2 0x83 3
    data_to_write[0] = 3;
    status = WUFE_SfpWrite (the_system->ufe.ufe_id, 0, 1, 0xa2, 0x83, data_to_write);
    if (status != 0)
    {
       printf("status is %x\n", status);
       WTI_TerminatePrintError("WUFE_SfpWrite failed", __LINE__);
    }

//    data_to_write[0] = 0;
//    data_to_write[1] = 1;
//    data_to_write[2] = 2;
//    data_to_write[3] = 3;

//    status = WUFE_SfpWrite (the_system->ufe.ufe_id, 0, 4, 0xa2, 0x83, data_to_write);
//    if (status != 0)
//    {
//       printf("status is %x\n", status);
//       WTI_TerminatePrintError("WUFE_SfpWrite failed", __LINE__);
//    }

//./sfp_test R 0xa3 0x80
   status = WUFE_SfpRead (the_system->ufe.ufe_id, 0, 1, 1, 0xa3, 0x80, data);
   if (status != 0)
   {
      printf("status is %x\n", status);
      WTI_TerminatePrintError("WUFE_SfpRead failed", __LINE__);
   }

   if (data[0] != 0)
      printf("invalid data read from SFP 0 is %x\n", data[0]);
   else
      printf("correct data read from SFP 0 is %x\n", data[0]);
//./sfp_test R 0xa3 0x81
   status = WUFE_SfpRead (the_system->ufe.ufe_id, 0, 1, 1, 0xa3, 0x81, data);
   if (status != 0)
   {
      printf("status is %x\n", status);
      WTI_TerminatePrintError("WUFE_SfpRead failed", __LINE__);
   }

   if (data[0] != 1)
      printf("invalid data read from SFP 0 is %x\n", data[0]);
   else
      printf("correct data read from SFP 0 is %x\n", data[0]);
//./sfp_test R 0xa3 0x82
   status = WUFE_SfpRead (the_system->ufe.ufe_id, 0, 1, 1, 0xa3, 0x82, data);
   if (status != 0)
   {
      printf("status is %x\n", status);
      WTI_TerminatePrintError("WUFE_SfpRead failed", __LINE__);
   }

   if (data[0] != 2)
      printf("invalid data read from SFP 0 is %x\n", data[0]);
   else
      printf("correct data read from SFP 0 is %x\n", data[0]);
//./sfp_test R 0xa3 0x83
   status = WUFE_SfpRead (the_system->ufe.ufe_id, 0, 1, 1, 0xa3, 0x83, data);
   if (status != 0)
   {
      printf("status is %x\n", status);
      WTI_TerminatePrintError("WUFE_SfpRead failed", __LINE__);
   }

   if (data[0] != 3)
      printf("invalid data read from SFP 0 is %x\n", data[0]);
   else
      printf("correct data read from SFP 0 is %x\n", data[0]);

//write 0 to Eprom 0 at address 0x80
//write 1 to Eprom 0 at address 0x81
//write 2 to Eprom 0 at address 0x82
//write 3 to Eprom 0 at address 0x83
//id: 0 - data read: 0
//id: 0 - data read: 1
//id: 0 - data read: 2
//id: 0 - data read: 3
#endif
   for (i=0;i<256;++i)
   {
      data[i] = data_to_write[i] = 0;
   }
   printf("Make sure SFP 622Mbd is located in slot 0 and 155Mbd at slot 1 and press any key to continue\n");
   getchar();
   //check read from multiple SFPs
   status = WUFE_SfpRead (the_system->ufe.ufe_id, 0, 1, 0, 0xa1, 0x2c, data);
   if (status != 0)
   {
      printf("status is %x\n", status);
      WTI_TerminatePrintError("WUFE_SfpRead failed", __LINE__);
   }

   if (data[0] != 0x36)
      printf("invalid data read from SFP 0 is %x\n", data[0]);
   else
      printf("correct data read from SFP 0 is %x\n", data[0]);
   if (data[16] != 0x31)
      printf("invalid data read from SFP 1 is %x\n", data[16]);
   else
      printf("correct data read from SFP 1 is %x\n", data[16]);

// check multiple bytes read
//read_sfp 0xa1 0x28 0x10
//id: 0 ->   4c  43  50  2d  36  32  32  41  34  48  44  52  20  20  20  20

   for (i=0;i<256;++i)
   {
      data[i] = data_to_write[i] = 0;
   }
   status = WUFE_SfpRead(the_system->ufe.ufe_id, 0, 16, 0, 0xa1, 0x28, data);
   if (status != 0)
   {
      printf("status is %x\n", status);
      WTI_TerminatePrintError("WUFE_SfpRead failed", __LINE__);
   }

   if (data[0] != 0x4c)
      printf("invalid data read from SFP 0 is %x\n", data[0]);
   else if (data[1] != 0x43)
      printf("invalid data read from SFP 0 is %x\n", data[1]);
   else if (data[2] != 0x50)
      printf("invalid data read from SFP 0 is %x\n", data[2]);
   else if (data[3] != 0x2d)
      printf("invalid data read from SFP 0 is %x\n", data[3]);
   else if (data[4] != 0x36)
      printf("invalid data read from SFP 0 is %x\n", data[4]);
   else if (data[5] != 0x32)
      printf("invalid data read from SFP 0 is %x\n", data[5]);
   else if (data[6] != 0x32)
      printf("invalid data read from SFP 0 is %x\n", data[6]);
   else if (data[7] != 0x41)
      printf("invalid data read from SFP 0 is %x\n", data[7]);
   else if (data[8] != 0x34)
      printf("invalid data read from SFP 0 is %x\n", data[8]);
   else if (data[9] != 0x48)
      printf("invalid data read from SFP 0 is %x\n", data[9]);
   else if (data[10] != 0x44)
      printf("invalid data read from SFP 0 is %x\n", data[10]);
   else if (data[11] != 0x52)
      printf("invalid data read from SFP 0 is %x\n", data[11]);
   else if (data[12] != 0x20)
      printf("invalid data read from SFP 0 is %x\n", data[12]);
   else if (data[13] != 0x20)
      printf("invalid data read from SFP 0 is %x\n", data[13]);
   else if (data[14] != 0x20)
      printf("invalid data read from SFP 0 is %x\n", data[14]);
   else if (data[15] != 0x20)
      printf("invalid data read from SFP 0 is %x\n", data[15]);
   else
      printf("Correct data read from SFP 0 is 4c  43  50  2d  36  32  32  41  34  48  44  52  20  20  20  20\n");

//id: 1 ->   4c  43  50  2d  31  35  35  41  34  48  53  4d  20  20  20  20
   if (data[16] != 0x4c)
      printf("invalid data read from SFP 1 is %x\n", data[16]);
   else if (data[17] != 0x43)
      printf("invalid data read from SFP 1 is %x\n", data[17]);
   else if (data[18] != 0x50)
      printf("invalid data read from SFP 1 is %x\n", data[18]);
   else if (data[19] != 0x2d)
      printf("invalid data read from SFP 1 is %x\n", data[19]);
   else if (data[20] != 0x31)
      printf("invalid data read from SFP 1 is %x\n", data[20]);
   else if (data[21] != 0x35)
      printf("invalid data read from SFP 1 is %x\n", data[21]);
   else if (data[22] != 0x35)
      printf("invalid data read from SFP 1 is %x\n", data[22]);
   else if (data[23] != 0x41)
      printf("invalid data read from SFP 1 is %x\n", data[23]);
   else if (data[24] != 0x34)
      printf("invalid data read from SFP 1 is %x\n", data[24]);
   else if (data[25] != 0x48)
      printf("invalid data read from SFP 1 is %x\n", data[25]);
   else if (data[26] != 0x53)
      printf("invalid data read from SFP 1 is %x\n", data[26]);
   else if (data[27] != 0x4d)
      printf("invalid data read from SFP 1 is %x\n", data[27]);
   else if (data[28] != 0x20)
      printf("invalid data read from SFP 1 is %x\n", data[28]);
   else if (data[29] != 0x20)
      printf("invalid data read from SFP 1 is %x\n", data[29]);
   else if (data[30] != 0x20)
      printf("invalid data read from SFP 1 is %x\n", data[30]);
   else if (data[31] != 0x20)
      printf("invalid data read from SFP 1 is %x\n", data[31]);
   else
      printf("Correct data read from sfp 1 is 4c  43  50  2d  31  35  35  41  34  48  53  4d  20  20  20  20\n");

   for (i=0;i<256;++i)
   {
      data[i] = data_to_write[i] = 0;
   }
   printf("Make sure SFP 622Mbd is located in slot 0 and 155Mbd at slot 6 and press any key to continue\n");
   getchar();
   status = WUFE_SfpRead(the_system->ufe.ufe_id, 0, 16, 0, 0xa1, 0x28, data);
   if (status != 0)
   {
      printf("status is %x\n", status);
      WTI_TerminatePrintError("WUFE_SfpRead failed", __LINE__);
   }

   if (data[0] != 0x4c)
      printf("invalid data read from SFP 0 is %x\n", data[0]);
   else if (data[1] != 0x43)
      printf("invalid data read from SFP 0 is %x\n", data[1]);
   else if (data[2] != 0x50)
      printf("invalid data read from SFP 0 is %x\n", data[2]);
   else if (data[3] != 0x2d)
      printf("invalid data read from SFP 0 is %x\n", data[3]);
   else if (data[4] != 0x36)
      printf("invalid data read from SFP 0 is %x\n", data[4]);
   else if (data[5] != 0x32)
      printf("invalid data read from SFP 0 is %x\n", data[5]);
   else if (data[6] != 0x32)
      printf("invalid data read from SFP 0 is %x\n", data[6]);
   else if (data[7] != 0x41)
      printf("invalid data read from SFP 0 is %x\n", data[7]);
   else if (data[8] != 0x34)
      printf("invalid data read from SFP 0 is %x\n", data[8]);
   else if (data[9] != 0x48)
      printf("invalid data read from SFP 0 is %x\n", data[9]);
   else if (data[10] != 0x44)
      printf("invalid data read from SFP 0 is %x\n", data[10]);
   else if (data[11] != 0x52)
      printf("invalid data read from SFP 0 is %x\n", data[11]);
   else if (data[12] != 0x20)
      printf("invalid data read from SFP 0 is %x\n", data[12]);
   else if (data[13] != 0x20)
      printf("invalid data read from SFP 0 is %x\n", data[13]);
   else if (data[14] != 0x20)
      printf("invalid data read from SFP 0 is %x\n", data[14]);
   else if (data[15] != 0x20)
      printf("invalid data read from SFP 0 is %x\n", data[15]);
   else
      printf("Correct data read from SFP 0 is 4c  43  50  2d  36  32  32  41  34  48  44  52  20  20  20  20\n");

//id: 1 ->   4c  43  50  2d  31  35  35  41  34  48  53  4d  20  20  20  20
   if (data[16] != 0x4c)
      printf("invalid data read from SFP 6 is %x\n", data[16]);
   else if (data[17] != 0x43)
      printf("invalid data read from SFP 6 is %x\n", data[17]);
   else if (data[18] != 0x50)
      printf("invalid data read from SFP 6 is %x\n", data[18]);
   else if (data[19] != 0x2d)
      printf("invalid data read from SFP 6 is %x\n", data[19]);
   else if (data[20] != 0x31)
      printf("invalid data read from SFP 6 is %x\n", data[20]);
   else if (data[21] != 0x35)
      printf("invalid data read from SFP 6 is %x\n", data[21]);
   else if (data[22] != 0x35)
      printf("invalid data read from SFP 6 is %x\n", data[22]);
   else if (data[23] != 0x41)
      printf("invalid data read from SFP 6 is %x\n", data[23]);
   else if (data[24] != 0x34)
      printf("invalid data read from SFP 6 is %x\n", data[24]);
   else if (data[25] != 0x48)
      printf("invalid data read from SFP 6 is %x\n", data[25]);
   else if (data[26] != 0x53)
      printf("invalid data read from SFP 6 is %x\n", data[26]);
   else if (data[27] != 0x4d)
      printf("invalid data read from SFP 6 is %x\n", data[27]);
   else if (data[28] != 0x20)
      printf("invalid data read from SFP 6 is %x\n", data[28]);
   else if (data[29] != 0x20)
      printf("invalid data read from SFP 6 is %x\n", data[29]);
   else if (data[30] != 0x20)
      printf("invalid data read from SFP 6 is %x\n", data[30]);
   else if (data[31] != 0x20)
      printf("invalid data read from SFP 6 is %x\n", data[31]);
   else
      printf("Correct data read from sfp 6 is 4c  43  50  2d  31  35  35  41  34  48  53  4d  20  20  20  20\n");

   for (i=0;i<256;++i)
   {
      data[i] = data_to_write[i] = 0;
   }
//echo data to Eprom 0 at address 0x80
//./sfp_test W 0 0xa2 0x80 8
   for (i=0;i<8;++i)
      data_to_write[i] = (2*i);

   status = WUFE_SfpWrite (the_system->ufe.ufe_id, 0, 8, 0xa2, 0x80, data_to_write);
   if (status != 0)
   {
      printf("status is %x\n", status);
      WTI_TerminatePrintError("WUFE_SfpWrite failed", __LINE__);
   }

//read_sfp 0xa3 0x80 10
   status = WUFE_SfpRead(the_system->ufe.ufe_id, 0, 8, 0, 0xa3, 0x80, data);
   if (status != 0)
   {
      printf("status is %x\n", status);
      WTI_TerminatePrintError("WUFE_SfpRead failed", __LINE__);
   }

   if (data[0] != 0x00)
      printf("invalid data read from SFP 0 is %x\n", data[0]);
   else if (data[1] != 0x02)
      printf("invalid data read from SFP 0 is %x\n", data[1]);
   else if (data[2] != 0x04)
      printf("invalid data read from SFP 0 is %x\n", data[2]);
   else if (data[3] != 0x06)
      printf("invalid data read from SFP 0 is %x\n", data[3]);
   else if (data[4] != 0x08)
      printf("invalid data read from SFP 0 is %x\n", data[4]);
   else if (data[5] != 0x0a)
      printf("invalid data read from SFP 0 is %x\n", data[5]);
   else if (data[6] != 0x0c)
      printf("invalid data read from SFP 0 is %x\n", data[6]);
   else if (data[7] != 0x0e)
      printf("invalid data read from SFP 0 is %x\n", data[7]);
   else
      printf("Correct data read from SFP 0 is 00 02 04 06 08 0a 0c 0e\n");
#endif
}
#if defined  __WT_UFE412__ || defined __WT_UFE448__
void CLI_F_UFETerminateRxTraffic(char *StrPrm)
{
   WP_U32 res, cmd, aug1, enable;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &aug1, &enable);
   if(res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   WT_UfeSystemRxTrafficMask(&the_system->ufe, aug1, enable);
}
#endif
void CLI_F_UFE_Address_Set(char *StrPrm)
{
   WP_U32 res, cmd, address, value;

   res = sscanf(StrPrm, "%d %x %x", &cmd, &address, &value);
   if(res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   /* write the value*/
   printf("value to put 0x%04x in address %#x\n", value, address);
   *((WP_U16*)address) = (WP_U16)value;

   /* read register once again */
   value = *((WP_U16*)address);   
   printf("value  0x%04x was set in register addressed %#x\n", value, address);

}

void CLI_F_UfeLineProtectionSwitch(char *StrPrm)
{
#if defined __WT_UFE412__ || defined __WT_UFE448__
#ifdef WT_UFE_FRAMER
   WP_U32 res, cmd, flexmux_id, line_port_id, use_protection, cur_line_port_id, bulk_port_lines;

   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &flexmux_id, &line_port_id, &use_protection, &bulk_port_lines);

   if ((res != 4) && (res != 5))
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   else if (4 == res) /* switching specific line_port_id*/
   {
      if (line_port_id > 8)
      {
         WTI_TerminatePrintError("CLI_F_UfeLineProtectionSwitch", __LINE__);
         return;
      }           

      WT_UfeLineProtectionSwitch(flexmux_id, line_port_id, use_protection);       
   }
   else if (5 == res) /*bulk switching */
   {
      for (cur_line_port_id = 0;cur_line_port_id<bulk_port_lines; cur_line_port_id++)
      {
         WT_UfeLineProtectionSwitch(flexmux_id, cur_line_port_id, use_protection);      
      }      
   }

#endif /* WT_UFE_FRAMER */
#endif /* WT_UFE4 */
}

#if defined __WT_UFE412__ || defined __WT_UFE448__
#ifdef WT_UFE_FRAMER
void CLI_F_FlexmuxPoke(char *StrPrm)
{
   WP_U32 res, cmd, address, value, flexmux_id;

   res = sscanf(StrPrm, "%d %d %x %x", &cmd, &flexmux_id, &address, &value);
   if (res != 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   WTI_Flexmux_Poke(flexmux_id, address , value );

}

void CLI_F_FlexmuxPeek(char *StrPrm)
{
   WP_U8 res;
   WP_U32 cmd,address, flexmux_id;

   res = sscanf(StrPrm, "%d %d %x", &cmd, &flexmux_id, &address);
   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   WTI_Flexmux_Peek(flexmux_id, address);

}

void CLI_F_FlexmuxAddresBasePeek(char *StrPrm)
{
   WP_U8 res;
   WP_U32 cmd, address, flexmux_id;

   res = sscanf(StrPrm, "%d %d %x", &cmd, &flexmux_id, &address);
   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   WTI_Flexmux_AddressBasePeek(flexmux_id, address);


}
void CLI_F_Flexmux_SDHHORdiSetMode(char *StrPrm)
{
  WP_U8 res;
  WP_U32 cmd,flexmux_id,stm4,stm1,stm0,ho_type,type;


   res = sscanf(StrPrm, "%d %d %d %d %d %d %d", &cmd, &flexmux_id,&stm4,&stm1,&stm0,&ho_type,&type);
   if (res != 7)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   WTI_Flexmux_Sdh_Ho_Rdi_Set_Mode(flexmux_id,stm4,stm1,stm0,ho_type,type); 

}
void ss_bits_cb_37900(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType,U8 Value)
{
   printf("stm1: %d, stm0: %d, SS_RX_Value: %d\n",pCommon_SDH_SONET_LineEndpointType->u.SDH.stm1,pCommon_SDH_SONET_LineEndpointType->u.SDH.stm0,Value);
}

void CLI_F_Flexmux_SDHHOSSBitsSetTx(char *StrPrm)
{
  WP_U8 res;
  WP_U32 cmd,flexmux_id,stm4,stm1,stm0,ho_type,Value;

  
   res = sscanf(StrPrm, "%d %d %d %d %d %d %x", &cmd, &flexmux_id,&stm4,&stm1,&stm0,&ho_type,&Value);
   if (res != 7)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   WTI_Flexmux_Sdh_Ho_SS_Bits_Set_Tx(flexmux_id,stm4,stm1,stm0,ho_type,Value); 
}

void CLI_F_Flexmux_SDHHOSSBitsGetRx(char *StrPrm)
{
  WP_U8 res;
  WP_U32 cmd,flexmux_id,stm4,stm1,stm0,ho_type;


   res = sscanf(StrPrm, "%d %d %d %d %d %d", &cmd, &flexmux_id,&stm4,&stm1,&stm0,&ho_type);
   if (res != 6)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   WPX_FRMR_RegisterSSBitsCallback(&ss_bits_cb_37900);
   WTI_Flexmux_Sdh_Ho_SS_Bits_Get_Rx(flexmux_id,stm4,stm1,stm0,ho_type); 
}


void CLI_F_FlexmuxSquelch_RecClkSet(char *StrPrm)
{
   WP_U8 res;
   WP_U32 cmd, flexmux_id, output_selector, squelch_mode;

   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &flexmux_id, &output_selector, &squelch_mode);
   if (res != 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if ((0 != squelch_mode) && (1 != squelch_mode))
   {
      WTI_TerminatePrintError("CLI_F_FlexmuxSquelch_RecClkSet", __LINE__);
      return;
   }

   if ((0 != output_selector) && (1 != output_selector))
   {
      WTI_TerminatePrintError("CLI_F_FlexmuxSquelch_RecClkSet", __LINE__);
      return;
   }
   

   WTI_Flexmux_Squelch_RecClkSet(flexmux_id, output_selector, squelch_mode);


}

void CLI_F_FlexmuxSquelch_RecClkGet(char *StrPrm)
{
   WP_U8 res;
   WP_U32 cmd, flexmux_id, output_selector;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &flexmux_id, &output_selector);
   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if ((0 != output_selector) && (1 != output_selector))
   {
      WTI_TerminatePrintError("CLI_F_FlexmuxSquelch_RecClkSet", __LINE__);
      return;
   }
   
   WTI_Flexmux_Squelch_RecClkGet(flexmux_id, output_selector);


}


#endif /* WT_UFE_FRAMER */
#endif /* WT_UFE4 */
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_UfeChipInitLineSetup(char *StrPrm)
{
   int i;
   WT_ufe_line_params *line_saved_params;
#ifdef WP_HW_WINPATH2
   WP_status status;
#endif

   if (the_system->ufe.fpga_mode == WT_OCTAL)
   {
      WT_ufe_line_octal_params line_params;

      if (MAX_UFE_LINES_USED < WTI_MAX_OCTAL_LINES)
      {
         WTI_TerminatePrintError("CLI_F_UfeChipInitLineSetup", __LINE__);
         return;
      }

#ifdef WP_HW_WINPATH2
      /* configure correct CPLD and GPIO for UFE+CAD */
      status = WPX_BoardConfigure(the_system->wpid, WPX_CONFIGURE_UFE_UPI2_TDM);
      WTI_TerminateOnError(status, "WPX_BoardConfigure()", __LINE__);
      WP_Delay(10000);
#endif

      for (i=0; i<WTI_MAX_OCTAL_LINES; i++)
      {
         line_saved_params = &(the_system->ufe.line_params[i]);
         if (line_saved_params->valid == 0)
            continue;

         memset(&line_params, 0, sizeof(WT_ufe_line_octal_params));
         line_params.line_id = i;
         line_params.transfer_type = line_saved_params->transfer_type;
         line_params.framed = line_saved_params->framed;
         line_params.cas_enable = line_saved_params->cas_enable;

#if (!(WTI_CLOCK_REC_ADA_BW_COM_MODE))
         line_params.clock_rec_mode = line_saved_params->clock_rec_mode;
#else
         printf("Note: Line is configured to WUFE_LINE_CLOCK_REC_DCO_ONLY\n");
         line_params.clock_rec_mode = WUFE_LINE_CLOCK_REC_DCO_ONLY;
#endif

         line_params.rx_clk_rec_if = line_saved_params->rx_clk_rec_if;
         line_params.ext_mode_active = line_saved_params->ext_mode_active;

         /* Init the octal over tdm */
#ifdef __WT_UFE3__
         /* in UFE3 the comet configuration is called only once and the
            functions configured all ports to the same configuration as defined by
            by the second line configuration */

         if (i == 1)
         {
            WT_UfeTdmCometOverUfeInit(&the_system->ufe, &line_params);
            WP_Delay(10000);
         }
#endif
         WT_UfeLineCreate(&the_system->ufe, i, &line_params, 1);
      }
   }
   else /* WT_SBI */
   {
      WUFE_line_sbi line_params;
      if (MAX_UFE_LINES_USED < N_MAX_UFE_SBI_LINE)
      {
         WTI_TerminatePrintError("CLI_F_UfeChipInitLineSetup", __LINE__);
         return;
      }
      for (i=0; i<N_MAX_UFE_SBI_LINE; i++)
      {
         int link_id, spe_id;

         line_saved_params = &(the_system->ufe.line_params[i]);
         if (line_saved_params->valid == 0)
            continue;

         memset(&line_params, 0, sizeof(WUFE_line_sbi));
         spe_id = WTI_SpeIdGet(i);

         if (spe_id == -1 || spe_id > (WUFE_MAX_N_SBI_SPE-1))
         {
            WTI_TerminatePrintError("CLI_F_UfeChipInitLineSetup", __LINE__);
            return;
         }
         line_params.spe_num = spe_id;
         link_id = WTI_SpeLinkIdGet(i);
         if (link_id == -1)
         {
            WTI_TerminatePrintError("CLI_F_UfeChipInitLineSetup", __LINE__);
            return;
         }
         line_params.sub_spe_id = link_id;
         line_params.framed = line_saved_params->framed;;
         line_params.cas_enable = line_saved_params->cas_enable;
         line_params.clock_rec_mode = line_saved_params->clock_rec_mode;

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
         line_params.tx_clk_master = line_saved_params->tx_clk_master;
         line_params.clock_translator_id = line_saved_params->clock_translator_id;
#else
         line_params.tx_clk_master = WUFE_LINE_CLOCK_SLAVE;
         line_params.clock_translator_id = 0;
#endif

#if WTI_CESOP_RX_TIMING_ENABLE
         line_params.clock_rec_mode = line_saved_params->clock_rec_mode;
         line_params.rx_clk_rec_if = line_saved_params->rx_clk_rec_if; /*0-31 adaptive, 32-367 diff */
#else
         line_params.clock_rec_mode &= ~WUFE_LINE_CLOCK_REC_ENABLE_RX;
         line_params.rx_clk_rec_if = 0;
#endif

#if 0
         printf("line_params.spe_num %d\n", line_params.spe_num);
         printf("line_params.sub_spe_id %d\n", line_params.sub_spe_id);
         printf("line_params.framed %d\n", line_params.framed);
         printf("line_params.cas_enable %d\n", line_params.cas_enable);
         printf("line_params.clock_rec_mode %d\n", line_params.clock_rec_mode);
         printf("line_params.tx_clk_master %d\n", line_params.tx_clk_master);
         printf("line_params.clock_translator_id %d\n", line_params.clock_translator_id);
         printf("line_params.rx_clk_rec_if %d\n\n", line_params.rx_clk_rec_if);
#endif

         WT_UfeLineCreate(&the_system->ufe, i, &line_params, 1);
      }
   }

   return;
}

#endif


/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggShowInfo(char *StrPrm)
{
   WP_U32 res, cmd, val, i;
   WP_iw_agg_cesop tdm2psn_agg_param;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnIwShowInfo", __LINE__);
      return;
   }

   memcpy(&tdm2psn_agg_param,&pw_info_base[val].tdm2psn_agg_param,sizeof(WP_iw_agg_cesop));

   printf("TDM TO PSN FLOW AGGREGATION INFO:\n");
   printf("\n");
   printf("TDM TO PSN FLOW AGGREGATION %d ( FLOW AGG HANDLE %x ):\n",
          val,
          the_system->pw[val].tdm2psn_flow_agg);
   printf("---------------------------------------------------------------\n");
   printf("\n");

   printf("FLOW AGGREGATION PARAMETERS:\n");
   printf("tag: %x\n",tdm2psn_agg_param.tag);
   printf("txfunc: %x\n",tdm2psn_agg_param.txfunc);
   printf("intmode: ");
   if (tdm2psn_agg_param.intmode)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("interruptqueue: %d\n",tdm2psn_agg_param.interruptqueue);
   printf("statmode: ");
   if (tdm2psn_agg_param.statmode)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("timestamp_mode: ");
   if (tdm2psn_agg_param.timestamp_mode)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("ov_pool_mode: ");
   if (tdm2psn_agg_param.ov_pool_mode)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("fbp_drop_threshold: %d\n",tdm2psn_agg_param.fbp_drop_threshold);
   printf("mtu: %d\n",tdm2psn_agg_param.mtu);
   printf("flow_agg_type: %d\n",tdm2psn_agg_param.flow_agg_type);
   printf("l3_protocol: %d\n",tdm2psn_agg_param.l3_protocol);
   printf("control_word_offset: %d\n",tdm2psn_agg_param.control_word_offset);
   printf("rtp_header_mode: ");
   if (tdm2psn_agg_param.rtp_header_mode)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("rtp_header_offset: %d\n",tdm2psn_agg_param.rtp_header_offset);
   printf("payload_size: %d\n",tdm2psn_agg_param.payload_size);
   printf("prefix_length: %d\n",tdm2psn_agg_param.prefix_length);
   for (i=0;i<tdm2psn_agg_param.prefix_length;++i)
      printf("prefix_header[%d]: %x\n",i,tdm2psn_agg_param.prefix_header[i]);
   printf("l2_length_update_mode: ");
   if (tdm2psn_agg_param.l2_length_update_mode)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("l2_length_offset: %d\n",tdm2psn_agg_param.l2_length_offset);
   printf("l3_header_offset: %d\n",tdm2psn_agg_param.l3_header_offset);
   printf("l4_length_update_mode: ");
   if (tdm2psn_agg_param.l4_length_update_mode)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("l4_length_offset: %d\n",tdm2psn_agg_param.l4_length_offset);
   printf("l4_checksum_recalc_mode: ");
   if (tdm2psn_agg_param.l4_checksum_recalc_mode)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("last_packet_cw_length: %d\n",tdm2psn_agg_param.last_packet_cw_length);
   printf("last_packet_ip_checksum: %x\n",tdm2psn_agg_param.last_packet_ip_checksum);
   printf("initial_seq_number: %d\n",tdm2psn_agg_param.initial_seq_number);
   printf("extraction_length: %d\n",tdm2psn_agg_param.extraction_length);
   printf("cas_location: %d\n",tdm2psn_agg_param.cas_location);
   printf("control_word_mode: ");
   if (tdm2psn_agg_param.control_word_mode)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("payload_type_check: ");
   if (tdm2psn_agg_param.payload_type_check)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("payload_type: %x\n",tdm2psn_agg_param.payload_type);
   printf("ssrc_check: ");
   if (tdm2psn_agg_param.ssrc_check)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("ssrc: %x\n",tdm2psn_agg_param.ssrc);

   printf("payload suppression: ");
   if (tdm2psn_agg_param.payload_suppression)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   WP_Display(0,the_system->pw[val].tdm2psn_flow_agg,WP_DISPLAY_DEFAULT,NULL);

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggStatMode(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != WP_DISABLE && val != WP_ENABLE)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggStatMode", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.statmode = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggTag(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %x", &cmd, &val);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.tag = val;

   tag_change = WP_TRUE;

   return;
}

#if WTI_CESOP_MEF8_ENCAP
void CLI_F_Tdm2PsnFlowAggEcidHeader(char *StrPrm)
{
   WP_U32 res, cmd, ecid, oxloz; /* oxloz is a funny name for 0x102 */


   res = sscanf(StrPrm, "%d %x %x",
                &cmd, &ecid, &oxloz);
   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   ecid_header = (oxloz<<20)|ecid;
   return;
}
#endif /* WTI_CESOP_MEF8_ENCAP */

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggEnetHeader(char *StrPrm)
{
   WP_U32 res, cmd;
   WP_U32 d_mac0, d_mac1, d_mac2, d_mac3, d_mac4, d_mac5;
   WP_U32 s_mac0, s_mac1, s_mac2, s_mac3, s_mac4, s_mac5;
   WP_U32 type;

   res = sscanf(StrPrm, "%d %x %x %x %x %x %x %x %x %x %x %x %x %x",
                &cmd,
                &d_mac0, &d_mac1, &d_mac2, &d_mac3, &d_mac4, &d_mac5,
                &s_mac0, &s_mac1, &s_mac2, &s_mac3, &s_mac4, &s_mac5, &type);

   if (res != 14)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   memset(&enet_header ,0,sizeof(WTI_enet_header));

   enet_header.dest_addr[0] = (WP_U8) d_mac0;
   enet_header.dest_addr[1] = (WP_U8) d_mac1;
   enet_header.dest_addr[2] = (WP_U8) d_mac2;
   enet_header.dest_addr[3] = (WP_U8) d_mac3;
   enet_header.dest_addr[4] = (WP_U8) d_mac4;
   enet_header.dest_addr[5] = (WP_U8) d_mac5;
   enet_header.source_addr[0] = (WP_U8) s_mac0;
   enet_header.source_addr[1] = (WP_U8) s_mac1;
   enet_header.source_addr[2] = (WP_U8) s_mac2;
   enet_header.source_addr[3] = (WP_U8) s_mac3;
   enet_header.source_addr[4] = (WP_U8) s_mac4;
   enet_header.source_addr[5] = (WP_U8) s_mac5;
   enet_header.type = (WP_U16) type;

   return;
}

#if WTI_NUM_OF_VLAN_TAGS == 2
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggOuterVlan(char *StrPrm)
{
   WP_U32 vlan_id=0;
   WP_U32 res, cmd;

   res = sscanf(StrPrm, "%d %x",
                &cmd,
                &vlan_id);

   enet_header.outer_type = 0x8101;
   enet_header.outer_vlan = (WP_U16)vlan_id;
}
#endif
#if WTI_NUM_OF_VLAN_TAGS > 0
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggInnerVlan(char *StrPrm)
{
   WP_U32 vlan_id=0;
   WP_U32 res, cmd;

   res = sscanf(StrPrm, "%d %x",
                &cmd,
                &vlan_id);
   enet_header.inner_type = 0x8100;
   enet_header.inner_vlan = (WP_U16)vlan_id;
}
#endif /* WTI_NUM_OF_VLAN_TAGS > 0*/

#if WTI_ENET_IEEE_802
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggEnet802Header(char *StrPrm)
{
   WP_U32 res, cmd;
   WP_U32 d_mac0, d_mac1, d_mac2, d_mac3, d_mac4, d_mac5;
   WP_U32 s_mac0, s_mac1, s_mac2, s_mac3, s_mac4, s_mac5;
   WP_U32 tpid, tci, type;

   res = sscanf(StrPrm, "%d %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x",
                &cmd,
                &d_mac0, &d_mac1, &d_mac2, &d_mac3, &d_mac4, &d_mac5,
                &s_mac0, &s_mac1, &s_mac2, &s_mac3, &s_mac4, &s_mac5,
                &tpid, &tci, &type);

   if (res != 16)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   memset(&enet_header ,0,sizeof(WTI_enet_header));

   enet_header.dest_addr[0] = (WP_U8) d_mac0;
   enet_header.dest_addr[1] = (WP_U8) d_mac1;
   enet_header.dest_addr[2] = (WP_U8) d_mac2;
   enet_header.dest_addr[3] = (WP_U8) d_mac3;
   enet_header.dest_addr[4] = (WP_U8) d_mac4;
   enet_header.dest_addr[5] = (WP_U8) d_mac5;
   enet_header.source_addr[0] = (WP_U8) s_mac0;
   enet_header.source_addr[1] = (WP_U8) s_mac1;
   enet_header.source_addr[2] = (WP_U8) s_mac2;
   enet_header.source_addr[3] = (WP_U8) s_mac3;
   enet_header.source_addr[4] = (WP_U8) s_mac4;
   enet_header.source_addr[5] = (WP_U8) s_mac5;
   enet_header.tpid = (WP_U16) tpid;
   enet_header.tci = (WP_U16) tci;
   enet_header.type = (WP_U16) type;

   return;
}
#endif
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggIpHeader(char *StrPrm)
{
   WP_U32 res, cmd, source_ip, dest_ip,ver_ihl_tos_len,id_frag_offset,ttl_prot_checksum;
   WP_U8  ver_ihl, tos, ttl, protocol, temp_buffer[4];
   WP_U16 total_length, id, frag_offset, checksum;

   res = sscanf(StrPrm, "%d %x %x %x %x %x",
                &cmd,
                &ver_ihl_tos_len,
                &id_frag_offset,
                &ttl_prot_checksum,
                &source_ip,
                &dest_ip);

   if (res != 6)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   memset(&ip_header ,0,sizeof(WTI_ip_header));

   memcpy(temp_buffer,&ver_ihl_tos_len,4);
   memcpy(&ver_ihl,temp_buffer,1);
   memcpy(&tos,&temp_buffer[1],1);
   memcpy(&total_length,&temp_buffer[2],2);

   ip_header.ver_ihl = ver_ihl;
   ip_header.tos = tos;
   ip_header.total_length = total_length;

   memcpy(temp_buffer,&id_frag_offset,4);
   memcpy(&id,temp_buffer,2);
   memcpy(&frag_offset,&temp_buffer[2],2);

   ip_header.id = id;
   ip_header.frag_offset = frag_offset;

   memcpy(temp_buffer,&ttl_prot_checksum,4);
   memcpy(&ttl,temp_buffer,1);
   memcpy(&protocol,&temp_buffer[1],1);
   memcpy(&checksum,&temp_buffer[2],2);

   ip_header.ttl = ttl;
   ip_header.protocol = protocol;
   ip_header.checksum = checksum;

   ip_header.source_addr = source_ip;
   ip_header.dest_addr = dest_ip;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggUdpHeader(char *StrPrm)
{
   WP_U32 res, cmd, source_port, dest_port;
   WP_U32 length, checksum;

   res = sscanf(StrPrm, "%d %x %x %x %x",
                &cmd,
                &source_port,
                &dest_port,
                &length,
                &checksum);

   if (res != 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   memset(&udp_header ,0,sizeof(WTI_udp_header));

   udp_header.source_port = (WP_U16) source_port;
   udp_header.dest_port = (WP_U16) dest_port;
   udp_header.length = (WP_U16) length;
   udp_header.checksum = (WP_U16) checksum;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggRtpHeader(char *StrPrm)
{
   WP_U32 res, cmd;
   WP_U32 control, payload_type, seq_number, timestamp, ssrc;

   res = sscanf(StrPrm, "%d %x %x %x %x %x",
                &cmd,
                &control, &payload_type, &seq_number, &timestamp, &ssrc);

   if (res != 6)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   memset(&rtp_header ,0,sizeof(WTI_rtp_header));

   rtp_header.control = (WP_U8) control;
   rtp_header.payload_type = (WP_U8) payload_type;
   rtp_header.seq_number = (WP_U16) seq_number;
   rtp_header.timestamp = timestamp;
   rtp_header.ssrc = ssrc;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggControlWord(char *StrPrm)
{
   WP_U32 res, cmd;
   WP_U32 control, frg_len,seq_number;

   res = sscanf(StrPrm, "%d %x %x %x",
                &cmd,
                &control, &frg_len, &seq_number);

   if (res != 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   memset(&control_word ,0,sizeof(WTI_control_word));

   control_word.control = (WP_U8) control;
   control_word.frg_len = (WP_U8) frg_len;
   control_word.seq_number = (WP_U16) seq_number;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggTsParams(char *StrPrm)
{
   WP_U32 res, cmd;
   WP_U32 ts_used, ts_method, ts_diff_if_id;

   res = sscanf(StrPrm, "%d %d %d %d",
                &cmd,
                &ts_used, &ts_method, &ts_diff_if_id);

   if (res != 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (ts_used != 0 && ts_used != 1)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggTsParams", __LINE__);
      return;
   }

   if (ts_method != WP_IW_CESOP_ABS_TS
       && ts_method != WP_IW_CESOP_DIFF_TS)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggTsParams", __LINE__);
      return;
   }

#ifdef __WT_UFE3__
   if (ts_used && ts_diff_if_id > WTI_CESOP_MAX_CLOCK_REC_INTERFACES +
       WTI_CESOP_MAX_EXT_DIFF_CR_INTERFACES - 1) /* 367 */
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggTsParams", __LINE__);
      return;
   }
#else
   if (ts_used && ts_diff_if_id > WTI_CESOP_MAX_CLOCK_REC_INTERFACES)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggTsParams", __LINE__);
      return;
   }
#endif
   if (ts_used)
   {
      pw_config.tdm2psn_flow_agg_config.timestamp_mode = WP_IW_TIME_STAMP_ENABLE;
      pw_config.tdm2psn_flow_agg_config.ts_method = ts_method;
      pw_config.tdm2psn_flow_agg_config.ts_diff_if_id = ts_diff_if_id;
      /* ts_const will be initialize later according to the timing mode; CR -> from the UFE, */
      /* ABS -> according to the number of line clock in a packet (in bits) */
      pw_config.tdm2psn_flow_agg_config.ts_const = 0;
   }
   else
   {
      pw_config.tdm2psn_flow_agg_config.timestamp_mode = WP_IW_TIME_STAMP_DISABLE;
      pw_config.tdm2psn_flow_agg_config.ts_method = 0;
      pw_config.tdm2psn_flow_agg_config.ts_diff_if_id = 0;
      pw_config.tdm2psn_flow_agg_config.ts_const = 0;
   }

   return;
}

void CLI_F_Tdm2PsnFlowAggTsConstSet(char *StrPrm)
{
    WP_U32 res, cmd;
    WP_U32 ts_const;

    res = sscanf(StrPrm, "%d %d",
                 &cmd,
                 &ts_const);

    if (res != 2)
    {
       WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
       return;
    }

    pw_config.tdm2psn_flow_agg_config.ts_const = ts_const;

    return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggIntMode(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != WP_DISABLE && val != WP_ENABLE)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggIntMode", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.intmode = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggTimeStampMode(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != WP_IW_TIME_STAMP_DISABLE || val != WP_IW_TIME_STAMP_ENABLE)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggTimeStampMode", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.timestamp_mode = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggOvIwFbpMode(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != WP_IW_OV_POOL_DISABLE || val != WP_IW_OV_POOL_ENABLE)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggOvIwFbpMode", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.ov_pool_mode = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggFbpDrop(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val > 65535)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggFbpDrop", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.fbp_drop_threshold = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggMtu(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.mtu = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggL3Protocol(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != WP_IW_CESOP_L3_IPV4 || val != WP_IW_CESOP_L3_MPLS)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggL3Protocol", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.l3_protocol = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggCwOffset(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.control_word_offset = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggRtpHeaderMode(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != WP_DISABLE && val != WP_ENABLE)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggRtpHeaderMode", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.rtp_header_mode = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggRtpHeaderOffset(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.rtp_header_offset = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggPrefixLen(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val > 64)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggPrefixLen", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.prefix_length = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggL2LengthUpdateMode(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != WP_DISABLE && val != WP_ENABLE)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggL2LengthUpdateMode", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.l2_length_update_mode = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggL2LenOffset(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val > 64)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggL2LenOffset", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.l2_length_offset = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggL3HeaderOffset(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val > 64)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggL3LenOffset", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.l3_header_offset = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggL4LenUpdateMode(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != WP_DISABLE && val != WP_ENABLE)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggL4LenUpdateMode", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.l4_length_update_mode = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggL4LengthOffset(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val > 64)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggL4LengthOffset", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.l4_length_offset = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggL4ChecksumRecalc(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != WP_DISABLE && val != WP_ENABLE)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggL4ChecksumRecalc", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.l4_checksum_recalc_mode = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggLastPacketCwLength(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val > 64)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggLastPacketCwLength", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.last_packet_cw_length = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggLastPacketIpChecksum(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.last_packet_ip_checksum = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggInitSeqNumber(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.initial_seq_number = val;

   return;
}


/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmFlowAggShowInfo(char *StrPrm)
{
   WP_U32 res, cmd, val, i;
   WP_iw_agg_cesop psn2tdm_agg_param;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Psn2TdmIwShowInfo", __LINE__);
      return;
   }

   memcpy(&psn2tdm_agg_param,&pw_info_base[val].psn2tdm_agg_param,sizeof(WP_iw_agg_cesop));

   printf("PSN TO TDM FLOW AGGREGATION INFO:\n");
   printf("\n");
   printf("PSN TO TDM FLOW AGGREGATION %d ( FLOW AGG HANDLE %x ):\n",
          val,
          the_system->pw[val].psn2tdm_flow_agg);
   printf("---------------------------------------------------------------\n");
   printf("\n");

   printf("FLOW AGGREGATION PARAMETERS:\n");
   printf("tag: %x\n",psn2tdm_agg_param.tag);
   printf("txfunc: %x\n",psn2tdm_agg_param.txfunc);
   printf("intmode: ");
   if (psn2tdm_agg_param.intmode)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("interruptqueue: %d\n",psn2tdm_agg_param.interruptqueue);
   printf("statmode: ");
   if (psn2tdm_agg_param.statmode)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("timestamp_mode: ");
   if (psn2tdm_agg_param.timestamp_mode)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("ov_pool_mode: ");
   if (psn2tdm_agg_param.ov_pool_mode)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("fbp_drop_threshold: %d\n",psn2tdm_agg_param.fbp_drop_threshold);
   printf("mtu: %d\n",psn2tdm_agg_param.mtu);
   printf("flow_agg_type: %d\n",psn2tdm_agg_param.flow_agg_type);
   printf("l3_protocol: %d\n",psn2tdm_agg_param.l3_protocol);
   printf("control_word_offset: %d\n",psn2tdm_agg_param.control_word_offset);
   printf("rtp_header_mode: ");
   if (psn2tdm_agg_param.rtp_header_mode)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("rtp_header_offset: %d\n",psn2tdm_agg_param.rtp_header_offset);
   printf("payload_size: %d\n",psn2tdm_agg_param.payload_size);
   printf("prefix_length: %d\n",psn2tdm_agg_param.prefix_length);
   for (i=0;i<psn2tdm_agg_param.prefix_length;++i)
      printf("prefix_header[%d]: %x\n",i,psn2tdm_agg_param.prefix_header[i]);
   printf("l2_length_update_mode: ");
   if (psn2tdm_agg_param.l2_length_update_mode)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("l2_length_offset: %d\n",psn2tdm_agg_param.l2_length_offset);
   printf("l3_header_offset: %d\n",psn2tdm_agg_param.l3_header_offset);
   printf("l4_length_update_mode: ");
   if (psn2tdm_agg_param.l4_length_update_mode)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("l4_length_offset: %d\n",psn2tdm_agg_param.l4_length_offset);
   printf("l4_checksum_recalc_mode: ");
   if (psn2tdm_agg_param.l4_checksum_recalc_mode)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("last_packet_cw_length: %d\n",psn2tdm_agg_param.last_packet_cw_length);
   printf("last_packet_ip_checksum: %x\n",psn2tdm_agg_param.last_packet_ip_checksum);
   printf("initial_seq_number: %d\n",psn2tdm_agg_param.initial_seq_number);
   printf("extraction_length: %d\n",psn2tdm_agg_param.extraction_length);
   printf("cas_location: %d\n",psn2tdm_agg_param.cas_location);
   printf("control_word_mode: ");
   if (psn2tdm_agg_param.control_word_mode)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("payload_type_check: ");
   if (psn2tdm_agg_param.payload_type_check)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("payload_type: %x\n",psn2tdm_agg_param.payload_type);
   printf("ssrc_check: ");
   if (psn2tdm_agg_param.ssrc_check)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("ssrc: %x\n",psn2tdm_agg_param.ssrc);

   WP_Display(0,the_system->pw[val].psn2tdm_flow_agg, WP_DISPLAY_DEFAULT, NULL);

   return;
}


/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmFlowAggStatMode(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != WP_DISABLE && val != WP_ENABLE)
   {
      WTI_TerminatePrintError("CLI_F_FlowAggStatMode", __LINE__);
      return;
   }

   pw_config.psn2tdm_flow_agg_config.statmode = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmFlowAggTag(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %x", &cmd, &val);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   pw_config.psn2tdm_flow_agg_config.tag = val;

   tag_change = WP_TRUE;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmFlowAggIntMode(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != WP_DISABLE && val != WP_ENABLE)
   {
      WTI_TerminatePrintError("CLI_F_Psn2TdmFlowAggIntMode", __LINE__);
      return;
   }

   pw_config.psn2tdm_flow_agg_config.intmode = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmFlowAggCwOffset(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   pw_config.psn2tdm_flow_agg_config.control_word_offset = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmFlowAggRtpHeaderOffset(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   pw_config.psn2tdm_flow_agg_config.rtp_header_offset = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmFlowAggExtractionLength(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   pw_config.psn2tdm_flow_agg_config.extraction_length = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmFlowAggCasLocation(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != WP_CAS_NIBBLE_HIGH && val != WP_CAS_NIBBLE_LOW)
   {
      WTI_TerminatePrintError("CLI_F_Psn2TdmFlowAggCasLocation", __LINE__);
      return;
   }

   pw_config.psn2tdm_flow_agg_config.cas_location = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmFlowAggControlWordMode(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != WP_ENABLE && val != WP_DISABLE)
   {
      WTI_TerminatePrintError("CLI_F_Psn2TdmFlowAggControlWordMode", __LINE__);
      return;
   }

   pw_config.psn2tdm_flow_agg_config.control_word_mode = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmFlowAggPayloadCheckType(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != WP_ENABLE && val != WP_DISABLE)
   {
      WTI_TerminatePrintError("CLI_F_Psn2TdmFlowAggPayloadCheckType", __LINE__);
      return;
   }

   pw_config.psn2tdm_flow_agg_config.payload_type_check = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmFlowAggPayloadType(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   pw_config.psn2tdm_flow_agg_config.payload_type = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmFlowAggSsrcCheck(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != WP_ENABLE && val != WP_DISABLE)
   {
      WTI_TerminatePrintError("CLI_F_Psn2TdmFlowAggSsrcCheck", __LINE__);
      return;
   }

   pw_config.psn2tdm_flow_agg_config.ssrc_check = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmFlowAggSsrc(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   pw_config.psn2tdm_flow_agg_config.ssrc = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmFlowAggTsParams(char *StrPrm)
{
   WP_U32 res, cmd;
   WP_U32 ts_used, ts_const;

   res = sscanf(StrPrm, "%d %d %x",
                &cmd,
                &ts_used, &ts_const);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (ts_used != 0 && ts_used != 1)
   {
      WTI_TerminatePrintError("CLI_F_Psn2TdmFlowAggTsParams", __LINE__);
      return;
   }

   if (ts_const > 0xfffff)
   {
      WTI_TerminatePrintError("CLI_F_Psn2TdmFlowAggTsParams", __LINE__);
      return;
   }

   pw_config.psn2tdm_flow_agg_config.ts_method = 0;
   pw_config.psn2tdm_flow_agg_config.ts_diff_if_id = 0;
   pw_config.psn2tdm_flow_agg_config.ts_const = 0;

   if (ts_used)
   {
      pw_config.psn2tdm_flow_agg_config.timestamp_mode = WP_IW_TIME_STAMP_ENABLE;
      pw_config.psn2tdm_flow_agg_config.ts_const = ts_const;
   }
   else
   {
      pw_config.psn2tdm_flow_agg_config.timestamp_mode = WP_IW_TIME_STAMP_DISABLE;
   }

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnRxBinShowInfo(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnRxBinShowInfo", __LINE__);
      return;
   }

   printf("TDM TO PSN RX BINDING INFO:\n");
   printf("\n");
   printf("TDM TO PSN RX BINDING %d:\n",val);
   printf("---------------------------\n");
   printf("\n");

   printf("RX BINDING PARAMETERS:\n");
   printf("default_aggregation: %x\n",pw_info_base[val].trans_rx_binding_config.default_aggregation);
   printf("mru: %x\n",pw_info_base[val].trans_rx_binding_config.mru);

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmRxBinShowInfo(char *StrPrm)
{
   printf("PSN TO TDM RX BINDING INFO:\n");
   printf("\n");
   printf("PSN TO TDM RX BINDING:\n");
   printf("---------------------------\n");
   printf("\n");

   printf("RX BINDING PARAMETERS:\n");
   printf("encap_mode: ");
   if (iw_info_base.rx_binding_gbe.encap_mode)
      printf("WP_ENABLE\n");
   else
      printf("WP_DISABLE\n");
   printf("l3_support_protocol: ");
   switch (iw_info_base.rx_binding_gbe.l3_support_protocol)
   {
      case (WP_IW_IPV4):
         {
            printf("WP_IW_IPV4\n");
            break;
         }
      case (WP_IW_IPV6):
         {
            printf("WP_IW_IPV6\n");
            break;
         }
      case (WP_IW_MPLS_UNI):
         {
            printf("WP_IW_MPLS_UNI\n");
            break;
         }
      case (WP_IW_MPLS_MULTI):
         {
            printf("WP_IW_MPLS_MULTI\n");
            break;
         }
      default:
         break;
   }

   printf("mru: %x\n",iw_info_base.rx_binding_gbe.mru);

   return;
}

#ifdef WP_HW_WINPATH3
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SaDisplay(char *StrPrm)
{
   WP_Display(0,
              WP_DISPLAY_SA,
              WP_DISPLAY_DEFAULT,
              NULL);
   return;
}
#endif

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmTxBinShowInfo(char *StrPrm)
{
   WP_U32 res, cmd, val;
   WP_iw_tx_binding_cesop cesop_tx_binding_config;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Psn2TdmTxBinShowInfo", __LINE__);
      return;
   }

   memcpy(&cesop_tx_binding_config,&pw_info_base[val].cesop_tx_binding_config,sizeof(WP_iw_tx_binding_cesop));

   printf("PSN TO TDM TX BINDING INFO:\n");
   printf("\n");
   printf("PSN TO TDM TX BINDING:\n");
   printf("---------------------------\n");
   printf("\n");

   printf("TX BINDING PARAMETERS:\n");
   printf("window_tx_threshold: %d\n",cesop_tx_binding_config.window_tx_threshold);
   printf("window_switchover_threshold: %d\n",cesop_tx_binding_config.window_switchover_threshold);
   printf("window_max_threshold: %d\n", cesop_tx_binding_config.window_max_threshold);
   printf("lops_detection: %d\n",cesop_tx_binding_config.lops_detection);
   printf("consec_pkts_in_synch_th: %d\n",cesop_tx_binding_config.consec_pkts_in_synch_th);
   printf("consec_miss_pkts_out_synch_th: %d\n",cesop_tx_binding_config.consec_miss_pkts_out_synch_th);

   return;
}

#if WTI_PCE_CLASSIFIER
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmPceShowInfo(char *StrPrm)
{
   return;
}
#endif

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
void CLI_F_ClockRecDisplay(char *StrPrm)
{
   WP_U32 res, cmd, val;
   WP_U32 flag=0;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val > 1)
   {
      WTI_TerminatePrintError("Invalid parameter, should 0 or 1", __LINE__);
      return;
   }

   if (val == 1)
      flag = WP_DISPLAY_RAW_HANDLES;

   WP_Display(the_system->wpid, WP_DISPLAY_CLOCK_RECOVERY, flag, 0);
}

#endif

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmTxBinWindowThreshold(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= pw_config.tx_jitter_buffer_size)
   {
      WTI_TerminatePrintError("CLI_F_Psn2TdmTxBinWindowThreshold", __LINE__);
      return;
   }

   pw_config.window_tx_threshold = val;

   /* we put the window_max_threshold to be the same as
    * tx_jitter_buffer_size for backward compatibility with all tests */
   pw_config.window_max_threshold = pw_config.tx_jitter_buffer_size;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmTxBinWindowMaxThreshold(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (val > pw_config.tx_jitter_buffer_size ||
       val <= pw_config.window_tx_threshold)
   {
      WTI_TerminatePrintError("CLI_F_Psn2TdmTxBinWindowMaxThreshold", __LINE__);
      return;
   }

   pw_config.window_max_threshold = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val < 1 || val > 255)
   {
      WTI_TerminatePrintError("CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold", __LINE__);
      return;
   }

   pw_config.window_switchover_threshold = val;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmTxBinLOPSCounterThreshold(char *StrPrm)
{
   WP_U32 res, cmd, consec_pkts_in_synch_th, consec_miss_pkts_out_synch_th;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &consec_pkts_in_synch_th, &consec_miss_pkts_out_synch_th);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if ((consec_pkts_in_synch_th > WTI_MAX_CONSEC_PKTS_IN_SYNCH_TH) ||
       (consec_miss_pkts_out_synch_th > WTI_MAX_CONSEC_MISS_PKTS_OUT_SYNCH_TH))
   {
      WTI_TerminatePrintError("CLI_F_Psn2TdmTxBinLOPSCounterThreshold", __LINE__);
      return;
   }

   pw_config.consec_pkts_in_synch_th = consec_pkts_in_synch_th;
   pw_config.consec_miss_pkts_out_synch_th = consec_miss_pkts_out_synch_th;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmTxBinLOPSDetection(char *StrPrm)
{
   WP_U32 res, cmd, lops_detection, consec_pkts_in_synch_th, consec_miss_pkts_out_synch_th;

   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &lops_detection, &consec_pkts_in_synch_th, &consec_miss_pkts_out_synch_th);

   if (res != 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (lops_detection == 0)
   {
      pw_config.lops_detection = WP_DISABLE;
   }
   else
   {
      if ((consec_pkts_in_synch_th > WTI_MAX_CONSEC_PKTS_IN_SYNCH_TH) ||
          (consec_pkts_in_synch_th < 1) ||
          (consec_miss_pkts_out_synch_th > WTI_MAX_CONSEC_MISS_PKTS_OUT_SYNCH_TH) ||
          (consec_miss_pkts_out_synch_th < 1))
      {
         WTI_TerminatePrintError("CLI_F_Psn2TdmTxBinLOPSDetection", __LINE__);
         return;
      }
      pw_config.lops_detection = WP_ENABLE;
      pw_config.consec_pkts_in_synch_th = consec_pkts_in_synch_th;
      pw_config.consec_miss_pkts_out_synch_th = consec_miss_pkts_out_synch_th;
      return;
   }
}

#if (! WTI_CESOP_MPLS_IW)
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmDfcShowInfo(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Psn2TdmDfcShowInfo", __LINE__);
      return;
   }

   printf("DFC INFO:\n");
   printf("\n");
   printf("ROUTE FLOW INDEX: %d\n",val);
   printf("---------------------\n");
   printf("\n");

   printf("FLOW PARAMETERS:\n");
   printf("flow_aggregation: %x\n",dfc_info_base[val].class_cfg.action_info.forwarding_action.flow_aggregation);
   printf("match_action: %x\n",dfc_info_base[val].class_cfg.match_action);
   printf("classifier_string: %s\n",dfc_info_base[val].class_cfg.classifier_string);
   printf("dfc_info: \n");
   printf("input port: %x\n",the_system->pw[val].dfc_flow_info.input_port);
   printf("output_port: %x\n",the_system->pw[val].dfc_flow_info.output_port);
   printf("filter_handle: %x\n",the_system->pw[val].dfc_flow_info.filter_handle);

   return;
}
#endif

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_PwCreate(char *StrPrm)
{
   WP_U32 res, cmd, val=0, pw_index, rx_clock_master_flag, tx_clock_master_flag;

   res = sscanf(StrPrm, "%d %d %d %d %d",
                &cmd, &val, &rx_clock_master_flag, &tx_clock_master_flag, &pw_index);

   if (res != 4 && res != 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != 0 && val != 1)
   {
      WTI_TerminatePrintError("CLI_F_PwCreate", __LINE__);
      return;
   }
   if (rx_clock_master_flag != 0 && rx_clock_master_flag != 1)
   {
      WTI_TerminatePrintError("CLI_F_PwCreate", __LINE__);
      return;
   }
   if (tx_clock_master_flag != 0 && tx_clock_master_flag != 1)
   {
      WTI_TerminatePrintError("CLI_F_PwCreate", __LINE__);
      return;
   }
   if (res == 5)
      if (pw_index >= WTI_MAX_PW)
      {
         WTI_TerminatePrintError("CLI_F_PwCreate", __LINE__);
         return;
      }

#if WTI_CESOP_MPLS_IW
#if WTI_CESOP_MPLS_OVER_ENET
   /* pw is not cascaded to mpls fa */
   pw_config.tdm2psn_flow_agg_config.txfunc = the_system->tx_enet_channel;
#else /* ! WTI_CESOP_MPLS_OVER_ENET */
   /* pw is not cascaded to mpls fa */
   pw_config.tdm2psn_flow_agg_config.txfunc = the_system->hspi_device;
#endif
#endif


   if (res == 4)
      WTI_PwCreate(total_number_of_pw, val, tx_clock_master_flag, rx_clock_master_flag);
   else /* res == 5 */
      WTI_PwCreate(pw_index, val, tx_clock_master_flag, rx_clock_master_flag);

   tag_change = WP_FALSE;

   ++total_number_of_pw; /* saves the total number of PWs in the system. */
}

#if WTI_CESOP_TO_MPLS_FA_CASCADING
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_PwCreateCascaded(char *StrPrm)
{
   WP_U32 res, cmd, val=0, pw_index, rx_clock_master_flag, tx_clock_master_flag, mpls_fa_index;

   res = sscanf(StrPrm, "%d %d %d %d %d %d",
                &cmd, &val, &rx_clock_master_flag, &tx_clock_master_flag, &mpls_fa_index, &pw_index);

   if (res != 5 && res != 6)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   /* oobc value validation*/
   if (val != 0 && val != 1)
   {
      WTI_TerminatePrintError("CLI_F_PwCreateCascaded", __LINE__);
      return;
   }
   if (rx_clock_master_flag != 0 && rx_clock_master_flag != 1)
   {
      WTI_TerminatePrintError("CLI_F_PwCreateCascaded", __LINE__);
      return;
   }
   if (tx_clock_master_flag != 0 && tx_clock_master_flag != 1)
   {
      WTI_TerminatePrintError("CLI_F_PwCreateCascaded", __LINE__);
      return;
   }
   if (mpls_fa_index >= WTI_MAX_NUM_OF_MPLS_FA)
   {
      WTI_TerminatePrintError("CLI_F_PwCreateCascaded : MPLS FA index not valid", __LINE__);
      return;
   }
   if (res == 6)
      if (pw_index >= WTI_MAX_PW)
      {
         WTI_TerminatePrintError("CLI_F_PwCreateCascaded", __LINE__);
         return;
      }

   if (the_system->tdm2psn_mpls_l2_fa_handle[mpls_fa_index] == WTI_INVALID_HANDLE)
   {
      WTI_TerminatePrintError("CLI_F_PwCreateCascaded : MPLS FA not initialized", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.txfunc = the_system->tdm2psn_mpls_l2_fa_handle[mpls_fa_index];

   if (res == 5)
   {
      WTI_PwCreate(total_number_of_pw, val, tx_clock_master_flag, rx_clock_master_flag);
   }
   else /* res == 6 */
   {
      WTI_PwCreate(pw_index, val, tx_clock_master_flag, rx_clock_master_flag);
   }

   tag_change = WP_FALSE;

   ++total_number_of_pw; /* saves the total number of PWs in the system. */
}


/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_PwCreateCascadedNoPhyEnable(char *StrPrm)
{
   WP_U32 res, cmd, val=0, pw_index, rx_clock_master_flag, tx_clock_master_flag, mpls_fa_index;

   res = sscanf(StrPrm, "%d %d %d %d %d %d",
                &cmd, &val, &rx_clock_master_flag, &tx_clock_master_flag, &mpls_fa_index, &pw_index);

   if (res != 5 && res != 6)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != 0 && val != 1)
   {
      WTI_TerminatePrintError("CLI_F_PwCreateCascaded", __LINE__);
      return;
   }
   if (rx_clock_master_flag != 0 && rx_clock_master_flag != 1)
   {
      WTI_TerminatePrintError("CLI_F_PwCreateCascaded", __LINE__);
      return;
   }
   if (tx_clock_master_flag != 0 && tx_clock_master_flag != 1)
   {
      WTI_TerminatePrintError("CLI_F_PwCreateCascaded", __LINE__);
      return;
   }
   if (mpls_fa_index >= WTI_MAX_NUM_OF_MPLS_FA)
   {
      WTI_TerminatePrintError("CLI_F_PwCreateCascaded : MPLS FA index not valid", __LINE__);
      return;
   }
   if (res == 6)
      if (pw_index >= WTI_MAX_PW)
      {
         WTI_TerminatePrintError("CLI_F_PwCreateCascaded", __LINE__);
         return;
      }

   if (the_system->tdm2psn_mpls_l2_fa_handle[mpls_fa_index] == WTI_INVALID_HANDLE)
   {
      WTI_TerminatePrintError("CLI_F_PwCreateCascaded : MPLS FA not initialized", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.txfunc = the_system->tdm2psn_mpls_l2_fa_handle[mpls_fa_index];

   if (res == 5)
      WTI_PwCreateNoPhyEnable(total_number_of_pw, val, tx_clock_master_flag, rx_clock_master_flag);
   else /* res == 6 */
      WTI_PwCreateNoPhyEnable(pw_index, val, tx_clock_master_flag, rx_clock_master_flag);

   tag_change = WP_FALSE;

   ++total_number_of_pw; /* saves the total number of PWs in the system. */
}


#if WTI_DUAL_EMPHY
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_PwCreateCascadedOnSecondUfe(char *StrPrm)
{
   WP_U32 res, cmd, val=0, pw_index, rx_clock_master_flag, tx_clock_master_flag, mpls_fa_index;

   res = sscanf(StrPrm, "%d %d %d %d %d %d",
                &cmd, &val, &rx_clock_master_flag, &tx_clock_master_flag, &mpls_fa_index, &pw_index);

   if (res != 5 && res != 6)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != 0 && val != 1)
   {
      WTI_TerminatePrintError("CLI_F_PwCreateCascadedOnSecondUfe", __LINE__);
      return;
   }
   if (rx_clock_master_flag != 0 && rx_clock_master_flag != 1)
   {
      WTI_TerminatePrintError("CLI_F_PwCreateCascadedOnSecondUfe", __LINE__);
      return;
   }
   if (tx_clock_master_flag != 0 && tx_clock_master_flag != 1)
   {
      WTI_TerminatePrintError("CLI_F_PwCreateCascadedOnSecondUfe", __LINE__);
      return;
   }
   if (mpls_fa_index >= WTI_MAX_NUM_OF_MPLS_FA)
   {
      WTI_TerminatePrintError("CLI_F_PwCreateCascadedOnSecondUfe : MPLS FA index not valid", __LINE__);
      return;
   }
   if (res == 6)
      if (pw_index >= WTI_MAX_PW)
      {
         WTI_TerminatePrintError("CLI_F_PwCreateCascadedOnSecondUfe", __LINE__);
         return;
      }

   if (the_system->tdm2psn_mpls_l2_fa_handle[mpls_fa_index] == WTI_INVALID_HANDLE)
   {
      WTI_TerminatePrintError("CLI_F_PwCreateCascadedOnSecondUfe : MPLS FA not initialized", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.txfunc = the_system->tdm2psn_mpls_l2_fa_handle[mpls_fa_index];

   if (res == 5)
   {
      WTI_PwCreateOnSecondUfe(total_number_of_pw, val, tx_clock_master_flag, rx_clock_master_flag);
   }
   else /* res == 6 */
   {
      WTI_PwCreateOnSecondUfe(pw_index, val, tx_clock_master_flag, rx_clock_master_flag);
   }

   tag_change = WP_FALSE;

   ++total_number_of_pw; /* saves the total number of PWs in the system. */
}

#endif
#else
#define CLI_F_PwCreateCascadedNoPhyEnable(...) printf("Error - Test is not modified to work without MPLS cascading\n"); exit(0)
#define CLI_F_PwCreateCascaded(...) printf("Error - Test is not modified to work without MPLS cascading\n"); exit(0)
#define CLI_F_PwCreateCascadedOnSecondUfe(...) printf("Error - Test is not modified to work without MPLS cascading\n"); exit(0)
#endif  /* WTI_CESOP_TO_MPLS_FA_CASCADING */

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_PwDelete(char *StrPrm)
{
   WP_U32 res, cmd, val, i;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_PwDelete", __LINE__);
      return;
   }

   /* clear slots associated with this PW */
   for (i=0;i<WTI_MAX_SLOTS;++i)
   {
      if (slots_config[i] == val)
      {
         slots_config[i] = WTI_UNUSED_SLOT;
      }
   }

   WTI_PwDelete(val);

   --total_number_of_pw;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_PwEnable(char *StrPrm)
{
   WP_U32 res, cmd, val;

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WP_U32 clock_rec_master;
   WP_status status;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &val, &clock_rec_master);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_PwEnable", __LINE__);
      return;
   }
   if (clock_rec_master != 0 &&
       clock_rec_master != 1)
   {
      WTI_TerminatePrintError("CLI_F_PwEnable", __LINE__);
      return;
   }
#else
   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_PwEnable", __LINE__);
      return;
   }
#endif

   WTI_PwEnable(val);

   the_system->pw[val].clock_rec_tx_enable = 0;


#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   if (clock_rec_master)
   {
      the_system->pw[val].clock_rec_tx_enable = 1;
      status = WP_CesopClockRecEnable(the_system->pw[val].psn2tdm_flow_agg);
      WTI_TerminateOnError(status , "WP_CesopClockRecEnable",__LINE__);
   }
#endif

}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_PwDisable(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_PwDisable", __LINE__);
      return;
   }

  //  skip_tx = 1;
//    printf("skip_tx = 1 \n");
   



   WTI_PwDisable(val);
}
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_PwRemove(char *StrPrm)
{
   WP_U32 res, cmd, pw_index;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];

   res = sscanf(StrPrm, "%d %d", &cmd, &pw_index);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (pw_index >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_PwRemove: can not remove PW; pw_index >= number of PW", __LINE__);
      return;
   }
   sprintf(temp_buf, "0 %d", pw_index);
   CLI_F_PwDisable(temp_buf);
   CLI_F_PwDelete(temp_buf);

}

#ifdef __WT_UFE3__
#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)
/***************************************************************
 * Func name  :
 * Description: This function checks if the E1 line number is valid.
 *
 * Input      : line_number
 * OutPut     : none
 * Return Val : error if needed
 ***************************************************************/
void CLI_F_CheckValidE1Line(int line_number)
{

   if ((line_number < 0) ||
       ((line_number > 20) && (line_number < 28)) ||
       ((line_number > 48) && (line_number < 56)) ||
       ((line_number > 76) && (line_number < 84)) ||
       ((line_number > 104) && (line_number < 112)) ||
       ((line_number > 132) && (line_number < 140)) ||
       ((line_number > 160) && (line_number < 168)) ||
       ((line_number > 188) && (line_number < 196)) ||
       ((line_number > 216) && (line_number < 224)) ||
       ((line_number > 244) && (line_number < 252)) ||
       ((line_number > 272) && (line_number < 280)) ||
       ((line_number > 300) && (line_number < 308)) ||
       ((line_number > 328) && (line_number < 336)) ||
       (line_number > 336))
   {
      WTI_TerminatePrintError("in function: CLI_F_CheckValidE1Line. Invalid line number", __LINE__);
      return;
   }
}

/***************************************************************
 * Func name  :
 * Description: This function binds the line number to PW number in E1 mode.
 *              T1 mode doesn't required any special attachments
 * Input      : pw_number
 * OutPut     : line_number
 * Return Val : (int) pw_number
 ***************************************************************/
int CLI_F_AttacheLineToPwNumber(int pw_number)
{
   WP_U32 line_number;

   if (pw_number < 0 || pw_number >= 252)
   {
      WTI_TerminatePrintError("in function: CLI_F_AttacheLineToPwNumber. Invalid pw number", __LINE__);
      return 0xffffffff;
   }
/* currently, for 1 PW per line only!! */
   /* expract the SPE_id and add the in_spe_offset */
   line_number = (pw_number/21)*28 + (pw_number%21);

   return (line_number);

}

/***************************************************************
 * Func name  :
 * Description: This function binds the PW number to line number in E1 mode.
 *              T1 mode doesn't required any special attachments
 * Input      : line_number
 * OutPut     : pw_number
 * Return Val : (int) pw_number
 ***************************************************************/
int CLI_F_AttachePwToLineNumber(int line_number)
{
   WP_U32 pw_number;

   if (line_number <= 0 || line_number > 336)
   {
      WTI_TerminatePrintError("Invalid line number", __LINE__);
      return 0xffffffff;
   }

   /* expract the SPE_id and add the in_spe_offset */
   pw_number = (line_number/28)*21 + (line_number%28);

   return(pw_number);

}

#endif
#endif /* WT_UFE3 */

#if 0
/***************************************************************
 * Func name  :
 * Description: this function enable the user to remove and recreate PW in loops
 * Input      : number of required loops (0-200), mode (SBI, OCTAL), test type (E1/T1/F/U)
 * OutPut     : none
 * Return Val :
 ***************************************************************/
void CLI_F_PW_AddRemoveInLoop(char *StrPrm)
{
   WP_U32 res, cmd, loops, test_type;
   WP_U32 i, j, e1_t1_lines;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];

   res = sscanf(StrPrm, "%d %d %d", &cmd, &loops, mode, test_type);
   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (loops < 0 || loops > 200)
   {
      WTI_TerminatePrintError("CLI_F_PW_AddRemoveInLoop: Illegal loops number", __LINE__);
      return;
   }
   if (mode != 1 || mode != 2)
   {
      WTI_TerminatePrintError("CLI_F_PW_AddRemoveInLoop: mode not supported", __LINE__);
      return;
   }
   if (test_type != 1 || test_type != 2 || test_type != 3 || test_type != 4)
   {
      WTI_TerminatePrintError("CLI_F_PW_AddRemoveInLoop: Inconsistent test type", __LINE__);
      return;
   }

   if (mode == 1) /* Octal */
   {
      switch (test_type)
      {
         case 1: /* T1, framed */
         {
            CLI_F_Octal_T1_F_AddRemovePW(loops);
         } /* case 1: T1, frm */

         case 2: /* T1, unframed */
         {
            CLI_F_Octal_T1_U_AddRemovePW(loops);
         } /* case 2: T1, unf */

         case 3: /* E1, framed */
         {
            CLI_F_Octal_E1_F_AddRemovePW(loops);
         } /* case 3: E1, frm */

         case 4: /* E1, unframed */
         {
            CLI_F_Octal_E1_U_AddRemovePW(loops);
         } /* case 4: E1, unf */
      } /* switch */
   }
   else /* SBI */
   {
      switch (test_type)
      {
         case 1: /* T1, framed */
         {
            CLI_F_SBI_T1_F_AddRemovePW(loops);
         } /* case 1: T1, frm */

         case 2: /* T1, unframed */
         {
            CLI_F_SBI_T1_U_AddRemovePW(loops);
         } /* case 2: T1, unf */

         case 3: /* E1, framed */
         {
            CLI_F_SBI_E1_F_AddRemovePW(loops);
         } /* case 3: E1, frm */

         case 4: /* E1, unframed */
         {
            CLI_F_SBI_E1_U_AddRemovePW(loops);
         } /* case 4: E1, unf */
      } /* switch test_type*/
   }/* if mode*/
}
#endif /* 0 */
/***************************************************************
 * Func name  :
 * Description: cli for add remove - t1 frm
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SBI_T1_F_AddRemovePW(char *StrPrm)
{
#ifndef WTI_CESOP_TDI
   WP_U32 res, cmd, loops, i, j, line_index, pw_index, counter=0;
   WP_CHAR enet_hdr[WTI_MAX_STRING_SIZE];
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_CHAR exit_factor=NULL;

   res = sscanf(StrPrm, "%d %d", &cmd, &loops);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

/* run test in loop */
   while (counter < loops && exit_factor != '9')
   {
      printf("Type '9' to quit or any other key to continue\n");
      exit_factor = getchar();
      if (exit_factor == '9')
      {
         printf("Exit test\n");
         break;
      }

/* remove PWs */
      printf("Removing...\n");
      WP_Delay(500000);

      for (i=N_ACTIVE_UFE_SBI_SPE; i>0; i--)
      {
         for (j=WTI_T1_LINES_PER_SPE; j>0; j--)
         {
            pw_index = (i-1)*WTI_T1_LINES_PER_SPE + (j-1);
            sprintf(temp_buf, "0 %d", pw_index);
            CLI_F_PwRemove(temp_buf);
         }
      }

      printf("PW removed. Press any key to continue...\n");
      exit_factor = getchar();
      if (exit_factor == '9')
         break;

/* recreate PWs */
      for (i=0; i<N_ACTIVE_UFE_SBI_SPE; i++)
      {
         for (j=0; j<WTI_T1_LINES_PER_SPE; j++)
         {

            line_index = i*28 + j;
            pw_index = i*WTI_T1_LINES_PER_SPE + j;

            sprintf(temp_buf, "0 %d 24 24 24 0-23", line_index);/* The line index of this PW */
            CLI_F_PwConfigureFramed(temp_buf);

            /* PWE3 channel configuration */
            CLI_F_Pwe3ChannelJitterBufferSize("0 32");
            CLI_F_Pwe3ChannelRxBufferSize("0 192");
            CLI_F_Pwe3ChannelStatmode("0 1");
            CLI_F_Pwe3ChannelTxUdpattern("0 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23");
            CLI_F_Pwe3ChannelTxDummyUdpattern("0 24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47");
            CLI_F_Pwe3ChannelUdpDummyMode("0 1");
            CLI_F_Pwe3ChannelTxUdcas("0 f");

            /**************** TDM --> PSN IW *****************/

#if WTI_ENET_IEEE_802
            CLI_F_Tdm2PsnFlowAggEnet802Header(enet_hdr);
#else
            CLI_F_Tdm2PsnFlowAggEnetHeader(enet_hdr);
#endif
#ifdef WTI_BOARD_1
            sprintf(temp_buf, "0 45000000 811e0000 40110000 %x %x",0x0a00010a+pw_index,0x0a00020a+pw_index);
            CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
#else
            sprintf(temp_buf, "0 45000000 811e0000 40110000 %x %x",0x0a00020a+pw_index,0x0a00010a+pw_index);
            CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
#endif
            CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
            CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
            CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");

#if WTI_CLOCK_REC_MODE
            /* Rx timing is used */
            if (pw_index == 0)   /* line 1 use CR, all other don't */
               sprintf(temp_buf, "0 1 1 %x",pw_index);
            else
               sprintf(temp_buf, "0 0 1 %x",pw_index);
            CLI_F_Tdm2PsnFlowAggTsParams(temp_buf); /* [ts_used(1-used,0-unused),ts_method(0-WP_IW_CESOP_ABS_TS,1-WP_IW_CESOP_DIFF_TS_MASTER ),ts_diff_if_id(0-31)] */
#else
            /* Rx timing is not used */
            sprintf(temp_buf, "0 0 1 %x",pw_index);
            CLI_F_Tdm2PsnFlowAggTsParams(temp_buf);/* [ts_used(1-used,0-unused),ts_method(0-WP_IW_CESOP_ABS_TS,1-WP_IW_CESOP_DIFF_TS_MASTER ),ts_diff_if_id(0-31)] */
#endif

            /**************** PSN --> TDM IW *****************/
            /* if clock recovery is set -> Tx timing is used */
            if (pw_index == 0)    /* line 1 use CR, all other don't */
               CLI_F_Psn2TdmFlowAggTsParams("0 1 3000"); /* [ts_used(1-used,0-unused),ts_diff_const(0-0xffff)] */
            else
               CLI_F_Psn2TdmFlowAggTsParams("0 0 0"); /* [ts_used(1-used,0-unused),ts_diff_const(0-0xffff)] */
            CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
            CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 8");
            CLI_F_Psn2TdmTxBinLOPSDetection("0 1 6 4");       /* TO DO : should be configured !!!!! */


            /**************** PSN --> TDM Tx binding*****************/

            /* CR master Rx and Tx direction */
            if (pw_index == 0)   /* pw 1 use CR, all other don't */
               CLI_F_PwCreate("0 0 1 1"); /* CR master */
            else
               CLI_F_PwCreate("0 0 0 0"); /* CR slave */
            if (pw_index == 0)
               sprintf(temp_buf, "0 %d 1",pw_index); /* line 1 is the master, all other slaves */
            else
               sprintf(temp_buf, "0 %d 0",pw_index);
            CLI_F_PwEnable(temp_buf);

         } /* end for (configure each line) */
      } /* for i */

      counter++;
   } /* while */

#endif /* WTI_CESOP_TDI */
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_EmphyTransDeviceDisable(char *StrPrm)
{
   WP_U32 res, cmd, val;
   WP_status status;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_EmphyTransDeviceDisable", __LINE__);
      return;
   }

   status = WP_DeviceDisable(the_system->pw[val].trans_device, WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status, "WP_DeviceDisable Transparent device",__LINE__);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Pwe3ChannelDisable(char *StrPrm)
{
   WP_U32 res, cmd, val, val1;
   WP_status status;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &val, &val1);
   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != 0 && val != 1)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3ChannelDisable", __LINE__);
      return;
   }
   if (val1 >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3ChannelDisable", __LINE__);
      return;
   }
   if (val)
   {
      status = WP_ChannelDisable(the_system->pw[val1].trans_tx);
      WTI_TerminateOnError(status, "WP_ChannelDisable Transparent Tx channel",__LINE__);
   }
   else
   {
      status = WP_ChannelDisable(the_system->pw[val1].trans_rx);
      WTI_TerminateOnError(status, "WP_ChannelDisable Transparent Rx channel",__LINE__);
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_EmphyTransDeviceEnable(char *StrPrm)
{
   WP_U32 res, cmd, val;
   WP_status status;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_EmphyTransDeviceEnable", __LINE__);
      return;
   }

   status = WP_DeviceEnable(the_system->pw[val].trans_device, WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status, "WP_DeviceEnable Transparent device",__LINE__);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Pwe3ChannelEnable(char *StrPrm)
{
   WP_U32 res, cmd, val, val1;
   WP_status status;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &val, &val1);
   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != 0 && val != 1)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3ChannelEnable", __LINE__);
      return;
   }
   if (val1 >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3ChannelEnable", __LINE__);
      return;
   }
   if (val)
   {
      status = WP_ChannelEnable(the_system->pw[val1].trans_tx);
      WTI_TerminateOnError(status, "WP_ChannelEnable Transparent Tx channel",__LINE__);
   }
   else
   {
      status = WP_ChannelEnable(the_system->pw[val1].trans_rx);
      WTI_TerminateOnError(status, "WP_ChannelEnable Transparent Rx channel",__LINE__);
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_EmphyTransDeviceModify(char *StrPrm)
{
   WP_U32 res, cmd, val, mod, index;
   WP_command command;
   WP_status status;

   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &index, &mod, &val);

   if (res != 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (index >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_EmphyTransDeviceModify", __LINE__);
      return;
   }
   if (mod != 1 && mod != 2 && mod != 3 && mod != 4)
   {
      WTI_TerminatePrintError("CLI_F_EmphyTransDeviceModify", __LINE__);
      return;
   }

   switch (mod)
   {
      case 1:
      {
         command = WP_DEV_MOD_EMPHY_TRANS_MODE;
         pw_info_base[index].upi_trans_device.mode = val;

         break;
      }
      case 2:
      {
         command = WP_DEV_MOD_EMPHY_TRANS_STRUCTURE_SIZE;
         pw_info_base[index].upi_trans_device.structure_size = val;

         break;
      }
      case 3:
      {
         command = WP_DEV_MOD_EMPHY_TRANS_RX_DATAUNIT_SIZE;
         pw_info_base[index].upi_trans_device.rx_dataunit_size = val;

         break;
      }
      case 4:
      {
         command = WP_DEV_MOD_EMPHY_TRANS_TX_DATAUNIT_SIZE;
         pw_info_base[index].upi_trans_device.tx_dataunit_size = val;

         break;
      }

      default:
         command = 0;
         break;
   }

   if (command > 0)
   {
      status = WP_DeviceModify(the_system->pw[index].trans_device,
                               command,
                               &pw_info_base[index].upi_trans_device);
      WTI_TerminateOnError(status, "WP_DeviceModify",__LINE__);
   }
   else
   {
      printf("---- command is 0 for WP_DeviceModify line: %d\n", __LINE__);
   }

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Pwe3TransRxChannelModify(char *StrPrm)
{
   WP_U32 res, cmd, val, mod, index;
   WP_command command;
   WP_status status;

   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &index, &mod, &val);

   if (res != 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (index >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3TransRxChannelModify", __LINE__);
      return;
   }
   if (mod != 1 && mod != 2 && mod != 3)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3TransRxChannelModify", __LINE__);
      return;
   }

   switch (mod)
   {
      case 1:
      {
         command = WP_CH_MOD_TRANS_STATMODE;
         pw_info_base[index].trans_rx_ch_config.statmode = val;

         break;
      }
      case 2:
      {
         command = WP_CH_MOD_TRANS_RX_BUFFERSIZE;
         pw_info_base[index].trans_rx_ch_config.rx_buffersize = val;

         status = WP_ChannelDisable(the_system->pw[index].trans_rx);
         WTI_TerminateOnError(status, "WP_ChannelDisable", __LINE__);
         WPL_Delay(5000);
         

         break;
      }
      case 3:
      { /* same as 2 but without the need to disable and re-enable. this is done by the test/user */
         command = WP_CH_MOD_TRANS_RX_BUFFERSIZE;
         pw_info_base[index].trans_rx_ch_config.rx_buffersize = val;

         /*status = WP_ChannelDisable(the_system->pw[index].trans_rx);
           WTI_TerminateOnError(status, "WP_ChannelDisable", __LINE__);
         */
         break;
      }

      default:
         command = 0;
         break;
   }

   if (command > 0)
   {
      status = WP_ChannelModify(the_system->pw[index].trans_rx,
                                command,
                                &pw_info_base[index].trans_rx_ch_config);
      WTI_TerminateOnError(status, "WP_ChannelModify",__LINE__);

      if (mod == 2)
      {
         status = WP_ChannelEnable(the_system->pw[index].trans_rx);
         WTI_TerminateOnError(status, "WP_ChannelEnable", __LINE__);
      }
      if (mod == 3)
      {
         /*status = WP_ChannelEnable(the_system->pw[index].trans_rx);
           WTI_TerminateOnError(status, "WP_ChannelEnable", __LINE__);*/
      }

   }
   else
      printf("---- command is 0 for WP_ChannelModify: line %d\n", __LINE__);

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Pwe3TransTxChannelModify(char *StrPrm)
{
   WP_U32 res, cmd, mod, index;
   WP_CHAR val[WTI_MAX_STRING_SIZE];
   WP_command command;
   WP_status status;
   WP_CHAR *byte;
   WP_U8 pattern[WTI_E1_SLOTS];
   WP_U32 i;

   res = sscanf(StrPrm, "%d %d %d %s", &cmd, &index, &mod, val);

   if (res != 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (index >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3TransTxChannelModify", __LINE__);
      return;
   }
   if (mod != 1 && mod != 2 && mod != 3 && mod != 4 && mod != 5)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3TransTxChannelModify", __LINE__);
      return;
   }

   switch (mod)
   {
      case 1:
      {
         command = WP_CH_MOD_TRANS_STATMODE;
         pw_info_base[index].trans_tx_ch_config.statmode = atoi(val);

         break;
      }
      case 2:
      {
#if (WTI_CESOP_TDI)
         command = WP_CH_MOD_TRANS_TDM_TX_UDCAS;
#else
         command = WP_CH_MOD_TRANS_EMPHY_TX_UDCAS;
#endif
         pw_info_base[index].trans_tx_ch_config.tx_udcas = atoi(val);

         break;
      }
      case 3:
      {
         command = WP_CH_MOD_TRANS_TX_UDP_DUMMY_MODE;
         pw_info_base[index].trans_tx_ch_config.tx_udp_dummy_mode = atoi(val);

         break;
      }
      case 4:
      {
         command = WP_CH_MOD_TRANS_TX_UDPATTERN;

         i=0;
         byte = strtok(val,",");
         while (byte)
         {
            pattern[i++] = atoi(byte);
            byte = strtok(NULL,",");
         }

         if (i >= WTI_E1_SLOTS)
         {
            WTI_TerminatePrintError("CLI_F_Pwe3TransTxChannelModify", __LINE__);
            return;
         }

         memcpy(pw_info_base[index].trans_tx_ch_config.tx_udpattern, pattern, WTI_E1_SLOTS);

         break;
      }
      case 5:
      {
         command = WP_CH_MOD_TRANS_TX_DUMMY_UDPATTERN;

         i=0;
         byte = strtok(val,",");
         while (byte)
         {
            pattern[i++] = atoi(byte);
            byte = strtok(NULL,",");
         }

         if (i >= WTI_E1_SLOTS)
         {
            WTI_TerminatePrintError("CLI_F_Pwe3TransTxChannelModify", __LINE__);
            return;
         }

         memcpy(pw_info_base[index].trans_tx_ch_config.tx_udp_dummy_pattern, pattern, WTI_E1_SLOTS);
         break;
      }

      default:
         break;
   }

   status = WP_ChannelModify(the_system->pw[index].trans_tx,
                             command,
                             &pw_info_base[index].trans_tx_ch_config);
   WTI_TerminateOnError(status, "WP_ChannelModify",__LINE__);

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_JitterBufferParamsModify(char *StrPrm)
{
   WP_U32 res, cmd, index, tx_jitter_buffer_size;
   WP_U32 window_tx_threshold;
   WP_U32 window_switchover_threshold;
   WP_U32 lops_detection, consec_pkts_in_synch_th, consec_miss_pkts_out_synch_th;

   res = sscanf(StrPrm,
                "%d %d %d %d %d %d %d %d",
                &cmd,
                &index,
                &tx_jitter_buffer_size,
                &window_tx_threshold,
                &window_switchover_threshold,
                &lops_detection,
                &consec_pkts_in_synch_th,
                &consec_miss_pkts_out_synch_th);

   if (res != 8)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (index >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_JitterBufferParamsModify", __LINE__);
      return;
   }

   WTI_CesopJitterBufferParamsModify(the_system->psn2tdm_iw_system,

#if WTI_PCE_CLASSIFIER
                                     the_system->pw[index].pce_flow_handle,
#else
#if WTI_CESOP_MPLS_IW
                                     the_system->pw[index].mpls_flow_handle,
#else
                                     the_system->pw[index].dfc_flow_handle,
#endif
#endif
                                     the_system->pw[index].trans_tx,
                                     tx_jitter_buffer_size,
                                     (WP_U16)window_tx_threshold,
                                     (WP_U8)window_switchover_threshold,
                                     lops_detection,
                                     consec_pkts_in_synch_th,
                                     consec_miss_pkts_out_synch_th);

   /* copy configuration structure to internal Demo information base*/
   pw_info_base[index].cesop_tx_binding_config.window_tx_threshold = window_tx_threshold;
   pw_info_base[index].cesop_tx_binding_config.window_switchover_threshold = window_switchover_threshold;
   pw_info_base[index].cesop_tx_binding_config.window_max_threshold = tx_jitter_buffer_size;
   pw_info_base[index].cesop_tx_binding_config.lops_detection = lops_detection;
   pw_info_base[index].cesop_tx_binding_config.consec_pkts_in_synch_th = consec_pkts_in_synch_th;
   pw_info_base[index].cesop_tx_binding_config.consec_miss_pkts_out_synch_th = consec_miss_pkts_out_synch_th;

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnCesopFlowAggModify(char *StrPrm)
{
   WP_U32 res, cmd, mod, index, val, val1;
   WP_command command;
   WP_status status;

   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &index, &mod, &val, &val1);

   if (res != 4 && res != 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (index >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnCesopFlowAggModify", __LINE__);
      return;
   }
   if (mod < 1 || mod > 24)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnCesopFlowAggModify", __LINE__);
      return;
   }

   switch (mod)
   {
      case 1:
      {
         command = WP_IW_FLOW_AGG_MOD_TAG;
         pw_info_base[index].tdm2psn_agg_param.tag = val;

         break;
      }
      case 2:
      {
         command = WP_IW_FLOW_AGG_MOD_USER_MON_CONF;
         pw_info_base[index].tdm2psn_agg_param.statmode = val;

         break;
      }
      case 3:
      {
         command = WP_IW_FLOW_AGG_MOD_USER_MON_CONF;
         pw_info_base[index].tdm2psn_agg_param.timestamp_mode = val;

         break;
      }
      case 4:
      {
         command = WP_IW_FLOW_AGG_MOD_USER_MON_CONF;
         pw_info_base[index].tdm2psn_agg_param.intmode = val;

         break;
      }
      case 5:
      {
         command = WP_IW_FLOW_AGG_MOD_USER_MON_CONF;
         pw_info_base[index].tdm2psn_agg_param.interruptqueue = val;

         break;
      }
      case 6:
      {
         command = WP_IW_FLOW_AGG_MOD_TXFUNC;
         pw_info_base[index].tdm2psn_agg_param.txfunc = val;

         break;
      }
      case 7:
      {
         command = WP_IW_FLOW_AGG_MOD_OV;
         pw_info_base[index].tdm2psn_agg_param.ov_pool_mode = val;

         break;
      }
      case 8:
      {
         command = WP_IW_FLOW_AGG_MOD_DROP_THRS;
         pw_info_base[index].tdm2psn_agg_param.fbp_drop_threshold = val;

         break;
      }
      case 9:
      {
         command = WP_IW_FLOW_AGG_MOD_MTU;
         pw_info_base[index].tdm2psn_agg_param.mtu = val;

         break;
      }
      case 10:
      {
         command = WP_IW_FLOW_AGG_CESOP_L3_PROTOCOL;
         pw_info_base[index].tdm2psn_agg_param.l3_protocol = val;

         break;
      }
      case 11:
      {
         command = WP_IW_FLOW_AGG_CESOP_RTP_HDR;
         pw_info_base[index].tdm2psn_agg_param.rtp_header_mode = val;
         pw_info_base[index].tdm2psn_agg_param.rtp_header_offset = val1;

         break;
      }
      case 12:
      {
         command = WP_IW_FLOW_AGG_CESOP_CW_OFFSET;
         pw_info_base[index].tdm2psn_agg_param.control_word_offset = val;

         break;
      }
      case 13:
      {
         command = WP_IW_FLOW_AGG_CESOP_PAYLOAD_SIZE;
         pw_info_base[index].tdm2psn_agg_param.payload_size = val;

         break;
      }
      case 14:
      {
         command = WP_IW_FLOW_AGG_CESOP_L2_LEN_UPDATE;
         pw_info_base[index].tdm2psn_agg_param.l2_length_update_mode = val;
         pw_info_base[index].tdm2psn_agg_param.l2_length_offset = val1;
         break;
      }
      case 15:
      {
         command = WP_IW_FLOW_AGG_CESOP_L3_HDR_OFFSET;
         pw_info_base[index].tdm2psn_agg_param.l3_header_offset = val;

         break;
      }
      case 16:
      {
         command = WP_IW_FLOW_AGG_CESOP_L4_LEN_UPDATE;
         pw_info_base[index].tdm2psn_agg_param.l4_length_update_mode = val;
         pw_info_base[index].tdm2psn_agg_param.l4_length_offset = val1;

         break;
      }
      case 17:
      {
         command = WP_IW_FLOW_AGG_CESOP_L4_CHECKSUM_RECALC;
         pw_info_base[index].tdm2psn_agg_param.l4_checksum_recalc_mode = val;

         break;
      }
      case 18:
      {
         command = WP_IW_FLOW_AGG_CESOP_LAST_PACKET_CW_LEN;
         pw_info_base[index].tdm2psn_agg_param.last_packet_cw_length = val;

         break;
      }
      case 19:
      {
         command = WP_IW_FLOW_AGG_CESOP_LAST_PACKET_IP_CHECKSUM;
         pw_info_base[index].tdm2psn_agg_param.last_packet_ip_checksum = val;

         break;
      }
      case 20:
      {
         command = WP_IW_FLOW_AGG_CESOP_CW_MODE;
         pw_info_base[index].tdm2psn_agg_param.control_word_mode = val;

         break;
      }
      case 21:
      {
         {
            command = (WP_IW_FLOW_AGG_CESOP_PREFIX |
                       WP_IW_FLOW_AGG_CESOP_LAST_PACKET_CW_LEN |
                       WP_IW_FLOW_AGG_CESOP_LAST_PACKET_IP_CHECKSUM |
                       WP_IW_FLOW_AGG_CESOP_L4_LEN_UPDATE);

            memcpy(&pw_config.tdm2psn_flow_agg_config,
                   &pw_info_base[index].tdm2psn_agg_param,
                   sizeof(WP_iw_agg_cesop));
            pw_config.rx_buffersize = pw_info_base[index].tdm2psn_agg_param.payload_size;
            WTI_FlowAggregationTdm2PsnConfig(&pw_info_base[index].tdm2psn_agg_param, index, 0);
         }

         break;
      }
      case 22:
      {
         /* This case allows the user to change ts_diff_if_id. FA_modify changes it only
            in the FA but no influence the channel itself therefore, RxBindingModify must
            be used after FA_Modify */
         command = WP_IW_FLOW_AGG_CESOP_TS_PARAMS;
         pw_info_base[index].tdm2psn_agg_param.ts_diff_if_id = val;
         break;
      }
      case 23:
      {
         command = WP_IW_FLOW_AGG_CESOP_PAYLOAD_SUP;
         pw_info_base[index].tdm2psn_agg_param.payload_suppression = val;

         break;
      }

      case 24:
      {
         command = WP_IW_FLOW_AGG_CESOP_PRIORITY_REMARK;

         if (val > 3 || val1 > 7)
         {
            printf("invalid parameters to WP_IW_FLOW_AGG_CESOP_PRIORITY_REMARK modification\n");

            return;
         }

         pw_info_base[index].tdm2psn_agg_param.n_prefix_remark_fields = val;
         pw_info_base[index].tdm2psn_agg_param.priority_values[0] = val1;

         break;
      }

      default:
         break;
   }

   status = WP_IwFlowAggregationModify(the_system->pw[index].tdm2psn_flow_agg,
                                       command,
                                       &pw_info_base[index].tdm2psn_agg_param);
   WTI_TerminateOnError(status,"WP_IwFlowAggregationModify", __LINE__);

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/

void CLI_F_Tdm2PsnCesopFlowAggControlWordModify(char *StrPrm)
{
   WP_U32 res, cmd, index;
   WP_command command;
   WP_status status;
   WP_U32 temp_lrm;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &index, &temp_lrm);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (index >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnCesopFlowAggControlWordModify", __LINE__);
      return;
   }

   if (temp_lrm > 15)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnCesopFlowAggControlWordModify", __LINE__);
      return;
   }
   
//    printf("pw_info_base[index].tdm2psn_agg_param.control_word_offset %d\n", pw_info_base[index].tdm2psn_agg_param.control_word_offset);
   
   pw_info_base[index].tdm2psn_agg_param.prefix_header[pw_info_base[index].tdm2psn_agg_param.control_word_offset] = (WP_U8 ) temp_lrm;

   printf("new LRM %x\n", (WP_U32) pw_info_base[index].tdm2psn_agg_param.prefix_header[pw_info_base[index].tdm2psn_agg_param.control_word_offset]);
   
//    for (int kk = 0;kk<pw_info_base[index].tdm2psn_agg_param.prefix_length;kk++)
//       printf("pw_info_base[index].tdm2psn_agg_param.prefix_header[kk] %x\n", pw_info_base[index].tdm2psn_agg_param.prefix_header[kk]);
   
   command = WP_IW_FLOW_AGG_CESOP_PREFIX;

   status = WP_IwFlowAggregationModify(the_system->pw[index].tdm2psn_flow_agg,
                                       command,
                                       &pw_info_base[index].tdm2psn_agg_param);
   WTI_TerminateOnError(status,"WP_IwFlowAggregationModify", __LINE__);

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/

void CLI_F_CesopPayloadSizeModify(char *StrPrm)
{
   WP_U32 res, cmd, pw_index;
   WP_status status;
   WP_U32 new_payload_size;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];

   res = sscanf(StrPrm, "%d %d %d", &cmd, &pw_index, &new_payload_size);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (pw_index >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_CesopPayloadSizeModify", __LINE__);
      return;
   }

   if (new_payload_size > 1536)
   {
      WTI_TerminatePrintError("CLI_F_CesopPayloadSizeModify", __LINE__);
      return;
   }
   
   sprintf(temp_buf, "0 %d", pw_index);
   CLI_F_PwDisable(temp_buf);
   
   sprintf(temp_buf, "0 %d %d %d %d", pw_index, 13, new_payload_size, 0);
   CLI_F_Tdm2PsnCesopFlowAggModify(temp_buf);

   sprintf(temp_buf, "0 %d %d %d %d", pw_index, 13, new_payload_size, 0);
   CLI_F_Psn2TdmCesopFlowAggModify(temp_buf);

   /* delete transparent Rx channel */
   status = WP_ChannelDelete(the_system->pw[pw_index].trans_rx);
   WTI_TerminateOnError(status, "WP_ChannelDelete Transparent Rx channel",__LINE__);

      /* create transparent Rx channel */
   WTI_TransChannelRxConfig(&trans_ch_config);

   trans_ch_config.rx_buffersize = new_payload_size;
   
   the_system->pw[pw_index].trans_rx =
      WP_ChannelCreate(pw_index,
#if WTI_CESOP_TDI
                       the_system->tdm_trans_dev[pw_config.line_index],
#else
                       the_system->pw[pw_index].trans_device,
#endif
                       the_system->qnode[WTI_QNODE_IW_TDM2PSN].handle,
                       WP_CH_RX,
                       WP_TRANSPARENT_PWE3,
                       &trans_ch_config);
   WTI_TerminateOnError(the_system->pw[pw_index].trans_rx,
                   "WP_ChannelCreate trans Rx",
                   __LINE__);

   /* create rx binding */
   WTI_RxBindingDirectMapConfig(&trans_rx_binding_config, pw_index);

   status = WP_IwRxBindingCreate(the_system->pw[pw_index].trans_rx,
                                 the_system->tdm2psn_iw_system,
                                 the_system->qnode[WTI_QNODE_IW_TDM2PSN].handle,
                                 &trans_rx_binding_config);
   WTI_TerminateOnError(status, "WP_IwRxBindingCreate TDM --> PSN", __LINE__);

   /* build IW system PSN --> TDM */
   status = WP_IwSystemBuild(the_system->tdm2psn_iw_system);
   WTI_TerminateOnError(status, "WP_IwSystemBuild TDM ----> PSN",__LINE__);
   
   /* enable transparent PWE3 Rx channel */
//   status = WP_ChannelEnable(the_system->pw[pw_index].trans_rx);
//   WTI_TerminateOnError(status , "WP_ChannelEnable Transparent Rx channel",__LINE__);
   sprintf(temp_buf, "0 %d 0", pw_index);
   CLI_F_PwEnable(temp_buf);

   return;
}

void CLI_F_Tdm2PsnCesopFlowAggPrefixHeaderModify(char *StrPrm)
{
   WP_U32 res, cmd, index;
   WP_command command;
   WP_status status;
   WP_CHAR temp_string[200], *byte;
   WP_U8 pattern[64];
   WP_U32 i;

   res = sscanf(StrPrm, "%d %d %s", &cmd, &index, temp_string);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (index >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnCesopFlowAggPrefixHeaderModify", __LINE__);
      return;
   }

   i=0;
   byte = strtok(temp_string,",");
   while (byte)
   {
      pattern[i++] = atoi(byte);
      byte = strtok(NULL,",");
   }

   if (i != 64)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnCesopFlowAggPrefixHeaderModify", __LINE__);
      return;
   }

   memcpy(pw_info_base[index].tdm2psn_agg_param.prefix_header, pattern, 64);
   command = WP_IW_FLOW_AGG_CESOP_PREFIX;

   status = WP_IwFlowAggregationModify(the_system->pw[index].tdm2psn_flow_agg,
                                       command,
                                       &pw_info_base[index].tdm2psn_agg_param);
   WTI_TerminateOnError(status,"WP_IwFlowAggregationModify", __LINE__);
   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmCesopFlowAggModify(char *StrPrm)
{
   WP_U32 res, cmd, mod, index, val, val1;
   WP_command command;
   WP_status status;

   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &index, &mod, &val, &val1);

   if (res != 4 && res != 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (index >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Psn2TdmCesopFlowAggModify", __LINE__);
      return;
   }
   if (mod < 1 || mod > 17)
   {
      WTI_TerminatePrintError("CLI_F_Psn2TdmCesopFlowAggModify", __LINE__);
      return;
   }

   switch (mod)
   {
      case 1:
      {
         command = WP_IW_FLOW_AGG_MOD_TAG;
         pw_info_base[index].psn2tdm_agg_param.tag = val;

         break;
      }
      case 2:
      {
         command = WP_IW_FLOW_AGG_MOD_USER_MON_CONF;
         pw_info_base[index].psn2tdm_agg_param.statmode = val;

         break;
      }
      case 3:
      {
         command = WP_IW_FLOW_AGG_MOD_USER_MON_CONF;
         pw_info_base[index].psn2tdm_agg_param.timestamp_mode = val;

         break;
      }
      case 4:
      {
         command = WP_IW_FLOW_AGG_MOD_USER_MON_CONF;
         pw_info_base[index].psn2tdm_agg_param.intmode = val;

         break;
      }
      case 5:
      {
         command = WP_IW_FLOW_AGG_MOD_USER_MON_CONF;
         pw_info_base[index].psn2tdm_agg_param.interruptqueue = val;

         break;
      }
      case 6:
      {
         command = WP_IW_FLOW_AGG_MOD_TXFUNC;
         pw_info_base[index].psn2tdm_agg_param.txfunc = val;

         break;
      }
      case 7:
      {
         command = WP_IW_FLOW_AGG_MOD_OV;
         pw_info_base[index].psn2tdm_agg_param.ov_pool_mode = val;

         break;
      }
      case 8:
      {
         command = WP_IW_FLOW_AGG_MOD_DROP_THRS;
         pw_info_base[index].psn2tdm_agg_param.fbp_drop_threshold = val;

         break;
      }
      case 9:
      {
         command = WP_IW_FLOW_AGG_MOD_MTU;
         pw_info_base[index].psn2tdm_agg_param.mtu = val;

         break;
      }
      case 10:
      {
         command = WP_IW_FLOW_AGG_CESOP_EXTRACTION_LEN;
         pw_info_base[index].psn2tdm_agg_param.extraction_length = val;

         break;
      }
      case 11:
      {
         command = WP_IW_FLOW_AGG_CESOP_PAYLOAD_TYPE;
         pw_info_base[index].psn2tdm_agg_param.payload_type_check = val;
         pw_info_base[index].psn2tdm_agg_param.payload_type = val1;

         break;
      }
      case 12:
      {
         command = WP_IW_FLOW_AGG_CESOP_SSRC;
         pw_info_base[index].psn2tdm_agg_param.ssrc_check = val;
         pw_info_base[index].psn2tdm_agg_param.ssrc = val1;

         break;
      }
      case 13:
      {
         command = WP_IW_FLOW_AGG_CESOP_PAYLOAD_SIZE;
         pw_info_base[index].psn2tdm_agg_param.payload_size = val;
         break;
      }

      case 14:
      {
         /* This case uses CR reset command to change parameters that can not be changes on-the-fly
            and therefore a reset command (for CR) is needed to change them.
            Temporal cr_params structure is created with the modified parameters sent to FA modify
            function. */
         WP_clock_rec_params *cr_modify_params;

         command = WP_IW_FLOW_AGG_CESOP_CLOCK_REC_RESET;

         cr_modify_params = pw_info_base[index].psn2tdm_agg_param.clock_rec_params;
         cr_modify_params->integration_factor = cr_modify_params->integration_factor + 1;
         cr_modify_params->divisor_update_period = cr_modify_params->divisor_update_period * 2;
         cr_modify_params->pm_threshold = cr_modify_params->pm_threshold * 2;
         cr_modify_params->max_pm_threshold = cr_modify_params->max_pm_threshold * 2;
         cr_modify_params->holdover_counter_limit = cr_modify_params->holdover_counter_limit / 2;
         cr_modify_params->allowed_dummy_packets = cr_modify_params->allowed_dummy_packets / 2;
         cr_modify_params->statmode = WP_DISABLE; /* stop CR statistics from counting */
         pw_info_base[index].psn2tdm_agg_param.clock_rec_params = cr_modify_params;

         break;
      }

      case 15:
      {
         /* This case changes the ACR operation mode (types A/B). This parameter can be changes
            only when the CR system is disabled.
            Available parameters:
            WP_IW_CLOCK_REC_ADAPTIVE_MODE        = 0x1
            WP_IW_CLOCK_REC_ADAPTIVE_TYPE_A_MODE = WP_IW_CLOCK_REC_ADAPTIVE_MODE
            WP_IW_CLOCK_REC_ADAPTIVE_TYPE_B_MODE = 0x3
         */

         WP_clock_rec_params *cr_modify_params;

         if (val != 0x1 && val != 0x3)
         {
            WTI_TerminatePrintError("CLI_F_Psn2TdmCesopFlowAggModify", __LINE__);
            return;
         }

         command = WP_IW_FLOW_AGG_CESOP_CLOCK_REC_OPERATION_MODE;

         cr_modify_params = pw_info_base[index].psn2tdm_agg_param.clock_rec_params;
         cr_modify_params->operation_mode = val;
         pw_info_base[index].psn2tdm_agg_param.clock_rec_params = cr_modify_params;

         break;
      }

      case 16:
      {         
         WP_clock_rec_params *cr_modify_params;
         
         command = WP_IW_FLOW_AGG_CESOP_CLOCK_REC_RESET;
         cr_modify_params = pw_info_base[index].psn2tdm_agg_param.clock_rec_params;
         cr_modify_params->holdover_counter_limit = 0x00010000;
         pw_info_base[index].psn2tdm_agg_param.clock_rec_params = cr_modify_params;
         
         break;
      }
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      case 17:
      {
    	  WP_clock_rec_params *cr_modify_params;

    	  command = WP_IW_FLOW_AGG_CESOP_CLOCK_REC_JBL_PARAMS;
          cr_modify_params = pw_info_base[index].psn2tdm_agg_param.clock_rec_params;

          cr_modify_params->jb_leveling_correction_size = val;
          cr_modify_params->jb_leveling_threshold_high = the_system->clock_rec[index].jb_leveling_threshold_high + 1;
          cr_modify_params->jb_leveling_threshold_low = the_system->clock_rec[index].jb_leveling_threshold_low - 1;

          if(val1)
        	  cr_modify_params->enhanced_acr_mode |= WP_JITTER_BUFFER_LEVELING_EN;
          else
        	  cr_modify_params->enhanced_acr_mode &= ~WP_JITTER_BUFFER_LEVELING_EN;

          pw_info_base[index].psn2tdm_agg_param.clock_rec_params = cr_modify_params;

    	  break;
      }
#endif

      default:
         break;
   }

   if (command == WP_IW_FLOW_AGG_CESOP_CLOCK_REC_OPERATION_MODE)
   {
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      /* Force holdover the CRS */
      WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
      sprintf(temp_buf, "0 %d", index);
      CLI_F_ClockRecoveryHoldoverForce(temp_buf);
#endif
   }
   if (command == WP_IW_FLOW_AGG_CESOP_CLOCK_REC_RESET ||
       command == WP_IW_FLOW_AGG_CESOP_CLOCK_REC_OPERATION_MODE)
   {
      /* Disable the CR */
      status = WP_CesopClockRecDisable(the_system->pw[index].psn2tdm_flow_agg);
      WTI_TerminateOnError(status , "WP_CesopClockRecDisable",__LINE__);
   }
   if (command == WP_IW_FLOW_AGG_CESOP_CLOCK_REC_RESET)
   {
      WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
      sprintf(temp_buf, "0 %d", index);
      CLI_F_PwDisable(temp_buf);
   }

   status = WP_IwFlowAggregationModify(the_system->pw[index].psn2tdm_flow_agg,
                                       command,
                                       &pw_info_base[index].psn2tdm_agg_param);
   WTI_TerminateOnError(status,"WP_IwFlowAggregationModify", __LINE__);

   if (command == WP_IW_FLOW_AGG_CESOP_CLOCK_REC_RESET)
   {
      WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
      sprintf(temp_buf, "0 %d 1", index);
      CLI_F_PwEnable(temp_buf); /* enable also the CR */
   }
   else if (command == WP_IW_FLOW_AGG_CESOP_CLOCK_REC_OPERATION_MODE)
   {
      /* Enable the CR */
      status = WP_CesopClockRecEnable(the_system->pw[index].psn2tdm_flow_agg);
      WTI_TerminateOnError(status , "WP_CesopClockRecEnable",__LINE__);
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      /* Clear force holdover */
      WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
      sprintf(temp_buf, "0 %d", index);
      CLI_F_ClockRecoveryHoldoverClear(temp_buf);
#endif
   }

   return;
}


/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmCesopFlowAggModifyFilterFactors(char *StrPrm)
{
   WP_U32 res, cmd, index,alpha ,gama ;
   WP_command command;
   WP_status status;
   WP_clock_rec_params *cr_modify_params;
   

   res = sscanf(StrPrm, "%d %d %d %d ", &cmd, &index, &alpha, &gama);

   if (res != 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (index >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Psn2TdmCesopFlowAggModify", __LINE__);
      return;
   }


   if(alpha)
   {
      command = WP_IW_FLOW_AGG_CESOP_CLOCK_REC_DIRECT_FACTOR;
      cr_modify_params = pw_info_base[index].psn2tdm_agg_param.clock_rec_params;
      cr_modify_params->direct_factor = alpha;
      pw_info_base[index].psn2tdm_agg_param.clock_rec_params = cr_modify_params;

      status = WP_IwFlowAggregationModify(the_system->pw[index].psn2tdm_flow_agg,
                                          command,
                                          &pw_info_base[index].psn2tdm_agg_param);
      WTI_TerminateOnError(status,"WP_IwFlowAggregationModify", __LINE__);
   }
   
   if(gama)
   {
      command = WP_IW_FLOW_AGG_CESOP_CLOCK_REC_INT_FACTOR;
      cr_modify_params = pw_info_base[index].psn2tdm_agg_param.clock_rec_params;
      cr_modify_params->integration_factor = gama;
      pw_info_base[index].psn2tdm_agg_param.clock_rec_params = cr_modify_params;
   
      status = WP_IwFlowAggregationModify(the_system->pw[index].psn2tdm_flow_agg,
                                          command,
                                          &pw_info_base[index].psn2tdm_agg_param);
      WTI_TerminateOnError(status,"WP_IwFlowAggregationModify", __LINE__);

   }
    

}


/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_ChangeTxClockMode(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != 1 && val != 2)
   {
      WTI_TerminatePrintError("CLI_F_ChangeTxClockMode", __LINE__);
      return;
   }

   if (val == 1)
      rx_looptime_clock = WP_FALSE;
   else
      rx_looptime_clock = WP_TRUE;

   return;
}

#if (WTI_CESOP_MPLS_IW && (!WTI_CESOP_MPLS_OVER_ENET))
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_HspiDeviceStatistics(char *StrPrm)
{
   WP_U32 t1, t2, screen_coord, temp1, temp2;
   WP_stats_upi_hspos stats_hspos = {0};
   WP_status status;

   memset (&stats_hspos,0,sizeof(WP_stats_upi_hspos));

   Vt100GotoXy(1,1);
   Vt100ClearScreen();

   while (1)
   {
      t1 = WP_TimeRead();

      if (WTI_SampleSerial())
      {
         break;
      }
      while (1)
      {
         t2 = WP_TimeRead();
         if( (t2-t1) >= FACTOR)
         {
            status = WP_DeviceStatistics(the_system->hspi_device, &stats_hspos);
            if (status != WP_OK) return;

            Vt100GotoXy(1,1);
            printf("HSPI DEVICE STATISTICS:\n");
            printf("\n");
            printf("HSPI DEVICE HANDLE %x:\n", the_system->hspi_device);
            printf("-------------------------------------------------\n");
            printf("\n");
            screen_coord = 5;

            temp1 = stats_hspos.rx_host_frames;
            temp2 = stats_hspos.rx_host_frames >> 32;
            printf("rx_host_frames:");
            Vt100GotoXy(23,screen_coord +1);
            printf("%.8x%.8x\n", temp2,temp1);

            temp1 = stats_hspos.rx_iw_frames;
            temp2 = stats_hspos.rx_iw_frames >> 32;
            printf("rx_iw_frames:");
            Vt100GotoXy(23,screen_coord+2);
            printf("%.8x%.8x\n", temp2,temp1);

            temp1 = stats_hspos.rx_err_fbp_underrun;
            temp2 = stats_hspos.rx_err_fbp_underrun >> 32;
            printf("rx_err_fbp_underrun:");
            Vt100GotoXy(23,screen_coord+3);
            printf("%.8x%.8x\n", temp2,temp1);

            temp1 = stats_hspos.rx_err_mru;
            temp2 = stats_hspos.rx_err_mru >> 32;
            printf("rx_err_mru:");
            Vt100GotoXy(23,screen_coord+4);
            printf("%.8x%.8x\n", temp2,temp1);

            temp1 = stats_hspos.rx_err_sdu;
            temp2 = stats_hspos.rx_err_sdu >> 32;
            printf("rx_err_sdu:");
            Vt100GotoXy(23,screen_coord+5);
            printf("%.8x%.8x\n", temp2,temp1);

            temp1 = stats_hspos.rx_err_sm;
            temp2 = stats_hspos.rx_err_sm >> 32;
            printf("rx_err_sm:");
            Vt100GotoXy(23,screen_coord+6);
            printf("%.8x%.8x\n", temp2,temp1);

            temp1 = stats_hspos.rx_err_parity;
            temp2 = stats_hspos.rx_err_parity >> 32;
            printf("rx_err_parity:");
            Vt100GotoXy(23,screen_coord+7);
            printf("%.8x%.8x\n", temp2,temp1);

            temp1 = stats_hspos.tx_frames;
            temp2 = stats_hspos.tx_frames >> 32;
            printf("tx_frames:");
            Vt100GotoXy(23,screen_coord+8);
            printf("%.8x%.8x\n", temp2,temp1);

#ifdef AUTOMATION_MODE
            return;
#else
            break;
#endif
         }
      }
   }
}

#else
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_EnetDeviceStatistics(char *StrPrm)
{
   WP_U32 res, cmd, val;
   WP_stats_enet enet_stats;
   WP_handle enet_handle=0;
   WP_status status;
#if WTI_XGI_MODE

   WP_stats_xgi xgi_stats;

   memset (&xgi_stats,0,sizeof(WP_stats_xgi));
#endif
   res = sscanf(StrPrm, "%d %d", &cmd, &val);

#if (WTI_2_ENET_DEVICES)
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
#else
   if (res != 1 && res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
#endif
   if (res == 1)
      val = 2;

   if (val != 1 && val != 2 && val != 3 && val != 4)
   {
      WTI_TerminatePrintError("CLI_F_EnetDeviceStatistics", __LINE__);
      return;
   }

   memset (&enet_stats,0,sizeof(WP_stats_enet));
#if (WTI_4_ENET_DEVICES)
   switch (val)
   {
      case 1:
         enet_handle = the_system->enet_device;
         break;
      case 2:
         enet_handle = the_system->enet_device1;
         break;
      case 3:
         enet_handle = the_system->enet_device2;
         break;
      case 4:
         enet_handle = the_system->enet_device3;
         break;
      default:
         break;
   }
#elif (WTI_2_ENET_DEVICES)
   enet_handle = (val == 1) ? the_system->enet_device : the_system->enet_device1;
#else
   enet_handle = the_system->enet_device;
#endif

#if WTI_XGI_MODE
   memset (&enet_stats,0,sizeof(WP_stats_xgi));
   status = WP_DeviceStatistics(enet_handle, &xgi_stats);
   if (status)
   {
      printf("Error Reading XGI Statistics\n");
      exit(1);
   }

   printf("----------------------------------------------------\n");

   printf("Display XGI queue:\n");
   WP_Display(0, the_system->tx_enet_channel, WP_DISPLAY_FMU_ONLY, 0);

   printf("\nXGI Device Hardware statistics (MAC):\n");

   printf("rx_packets:           %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_packets).part.high,
          ((WTI_StatField)xgi_stats.rx_packets).part.low);

   printf("tx_packets:           %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.tx_packets).part.high,
          ((WTI_StatField)xgi_stats.tx_packets).part.low);

   printf("rx_frames_64:         %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_frames_64).part.high,
          ((WTI_StatField)xgi_stats.rx_frames_64).part.low);

   printf("tx_frames_64:         %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.tx_frames_64).part.high,
          ((WTI_StatField)xgi_stats.tx_frames_64).part.low);

   printf("rx_frames_65_127:     %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_frames_65_127).part.high,
          ((WTI_StatField)xgi_stats.rx_frames_65_127).part.low);

   printf("tx_frames_65_127:     %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.tx_frames_65_127).part.high,
          ((WTI_StatField)xgi_stats.tx_frames_65_127).part.low);

   printf("rx_frames_128_255:    %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_frames_128_255).part.high,
          ((WTI_StatField)xgi_stats.rx_frames_128_255).part.low);

   printf("tx_frames_128_255:    %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.tx_frames_128_255).part.high,
          ((WTI_StatField)xgi_stats.tx_frames_128_255).part.low);

   printf("rx_frames_256_511:    %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_frames_256_511).part.high,
          ((WTI_StatField)xgi_stats.rx_frames_256_511).part.low);

   printf("tx_frames_256_511:    %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.tx_frames_256_511).part.high,
          ((WTI_StatField)xgi_stats.tx_frames_256_511).part.low);

   printf("rx_frames_512_1023:   %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_frames_512_1023).part.high,
          ((WTI_StatField)xgi_stats.rx_frames_512_1023).part.low);

   printf("tx_frames_512_1023:   %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.tx_frames_512_1023).part.high,
          ((WTI_StatField)xgi_stats.tx_frames_512_1023).part.low);

   printf("rx_frames_1024_1518:  %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_frames_1024_1518).part.high,
          ((WTI_StatField)xgi_stats.rx_frames_1024_1518).part.low);

   printf("tx_frames_1024_1518:  %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.tx_frames_1024_1518).part.high,
          ((WTI_StatField)xgi_stats.tx_frames_1024_1518).part.low);

   printf("rx_frames_1519_max:   %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_frames_1519_max).part.high,
          ((WTI_StatField)xgi_stats.rx_frames_1519_max).part.low);

   printf("tx_frames_1519_max:   %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.tx_frames_1519_max).part.high,
          ((WTI_StatField)xgi_stats.tx_frames_1519_max).part.low);

   printf("rx_bytes:             %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_bytes).part.high,
          ((WTI_StatField)xgi_stats.rx_bytes).part.low);

   printf("rx_err_fcs:           %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_bytes).part.high,
          ((WTI_StatField)xgi_stats.rx_bytes).part.low);

   printf("rx_multicast:         %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_multicast).part.high,
          ((WTI_StatField)xgi_stats.rx_multicast).part.low);

   printf("rx_broadcast:         %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_broadcast).part.high,
          ((WTI_StatField)xgi_stats.rx_broadcast).part.low);

   printf("rx_mac_pause:         %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_mac_pause).part.high,
          ((WTI_StatField)xgi_stats.rx_mac_pause).part.low);

   printf("rx_err_length:        %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_err_length).part.high,
          ((WTI_StatField)xgi_stats.rx_err_length).part.low);

   printf("rx_err_code:          %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_err_code).part.high,
          ((WTI_StatField)xgi_stats.rx_err_code).part.low);

   printf("rx_undersize:         %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_undersize).part.high,
          ((WTI_StatField)xgi_stats.rx_undersize).part.low);

   printf("rx_oversize:          %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_oversize).part.high,
          ((WTI_StatField)xgi_stats.rx_oversize).part.low);

   printf("rx_jabber:            %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_jabber).part.high,
          ((WTI_StatField)xgi_stats.rx_jabber).part.low);

   printf("tx_bytes:             %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.tx_bytes).part.high,
          ((WTI_StatField)xgi_stats.tx_bytes).part.low);

   printf("tx_multicast:         %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.tx_multicast).part.high,
          ((WTI_StatField)xgi_stats.tx_multicast).part.low);

   printf("tx_broadcast:         %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.tx_broadcast).part.high,
          ((WTI_StatField)xgi_stats.tx_broadcast).part.low);

   printf("tx_mac_pause:         %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.tx_mac_pause).part.high,
          ((WTI_StatField)xgi_stats.tx_mac_pause).part.low);

   printf("tx_err_packets:       %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.tx_err_packets).part.high,
          ((WTI_StatField)xgi_stats.tx_err_packets).part.low);
   printf("\nDPS statistics:\n");

   printf("rx_err_overrun:       %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_err_overrun).part.high,
          ((WTI_StatField)xgi_stats.rx_err_overrun).part.low);

   printf("rx_err_maxsdu:        %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_err_sdu).part.high,
          ((WTI_StatField)xgi_stats.rx_err_sdu).part.low);

   printf("rx_err_mac_address:   %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_err_nonvalid_mac).part.high,
          ((WTI_StatField)xgi_stats.rx_err_nonvalid_mac).part.low);

   printf("rx_err_mru:           %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_err_mru).part.high,
          ((WTI_StatField)xgi_stats.rx_err_mru).part.low);

   printf("tx_err_underrun:      %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.tx_err_underrun).part.high,
          ((WTI_StatField)xgi_stats.tx_err_underrun).part.low);

   printf("rx_err_host_full:     %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_err_host_full).part.high,
          ((WTI_StatField)xgi_stats.rx_err_host_full).part.low);

   printf("rx_err_fbp_underrun:  %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_err_fbp_underrun).part.high,
          ((WTI_StatField)xgi_stats.rx_err_fbp_underrun).part.low);

   printf("rx_iw_frames:         %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_iw_frames).part.high,
          ((WTI_StatField)xgi_stats.rx_iw_frames).part.low);

   printf("tx_good_frames:       %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.tx_frames).part.high,
          ((WTI_StatField)xgi_stats.tx_frames).part.low);
   printf("\n");

   printf("rx_host_frames:       %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.rx_host_frames).part.high,
          ((WTI_StatField)xgi_stats.rx_host_frames).part.low);
   printf("\n");

   printf("Serial DPS IF statistics (CFU-PCE):\n");

   printf("Serial DPS IF tx frames             %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.serial_dps_if_stats.tx_frames).part.high,
          ((WTI_StatField)xgi_stats.serial_dps_if_stats.tx_frames).part.low);

   printf("Serial DPS IF tx bytes              %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.serial_dps_if_stats.tx_bytes).part.high,
          ((WTI_StatField)xgi_stats.serial_dps_if_stats.tx_bytes).part.low);

   printf("Serial DPS IF rx frames             %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.serial_dps_if_stats.rx_frames).part.high,
          ((WTI_StatField)xgi_stats.serial_dps_if_stats.rx_frames).part.low);

   printf("Serial DPS IF rx bytes              %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.serial_dps_if_stats.rx_bytes).part.high,
          ((WTI_StatField)xgi_stats.serial_dps_if_stats.rx_bytes).part.low);

   printf("Serial DPS IF rx pce denied frames  %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.serial_dps_if_stats.rx_pce_denied_frames).part.high,
          ((WTI_StatField)xgi_stats.serial_dps_if_stats.rx_pce_denied_frames).part.low);

   printf("Serial DPS IF rx dropped frames     %.8x%.8x\n",
          ((WTI_StatField)xgi_stats.serial_dps_if_stats.rx_dropped_frames).part.high,
          ((WTI_StatField)xgi_stats.serial_dps_if_stats.rx_dropped_frames).part.low);

#else /* Not in XGI mode */
   status = WP_DeviceStatistics(enet_handle, &enet_stats);
   if (status)
   {
      printf("Error Reading Hs Enet Statistics\n");
      exit(1);
   }

   printf("----------------------------------------------------\n");

   printf ("HS ENET Port Statistics (HW)\n");

   printf ("TxRx Frame  64:              %08x%08x\n",
           ((WTI_StatField)enet_stats.txrx_frames_64).part.high,
           ((WTI_StatField)enet_stats.txrx_frames_64).part.low);

   printf ("Tx Bytes:                    %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_bytes).part.high,
           ((WTI_StatField)enet_stats.tx_bytes).part.low);

   printf ("TxRx Frame 127:              %08x%08x\n",
           ((WTI_StatField)enet_stats.txrx_frames_127).part.high,
           ((WTI_StatField)enet_stats.txrx_frames_127).part.low);

   printf ("Tx Packets:                  %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_packets).part.high,
           ((WTI_StatField)enet_stats.tx_packets).part.low);

   printf ("TxRx Frame 255:              %08x%08x\n",
           ((WTI_StatField)enet_stats.txrx_frames_255).part.high,
           ((WTI_StatField)enet_stats.txrx_frames_255).part.low);

   printf ("Tx Multicast:                %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_multicast).part.high,
           ((WTI_StatField)enet_stats.tx_multicast).part.low);

   printf ("TxRx Frame 1023:             %08x%08x\n",
           ((WTI_StatField)enet_stats.txrx_frames_1023).part.high,
           ((WTI_StatField)enet_stats.txrx_frames_1023).part.low);

   printf ("Tx Broadcast:                %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_broadcast).part.high,
           ((WTI_StatField)enet_stats.tx_broadcast).part.low);

   printf ("TxRx Frame 1518:             %08x%08x\n",
           ((WTI_StatField)enet_stats.txrx_frames_1518).part.high,
           ((WTI_StatField)enet_stats.txrx_frames_1518).part.low);

   printf ("Tx Mac Pause:                %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_mac_pause).part.high,
           ((WTI_StatField)enet_stats.tx_mac_pause).part.low);

   printf ("TxRx Frame 1522:             %08x%08x\n",
           ((WTI_StatField)enet_stats.txrx_frames_1522).part.high,
           ((WTI_StatField)enet_stats.txrx_frames_1522).part.low);

   printf ("Tx Defer:                    %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_defer).part.high,
           ((WTI_StatField)enet_stats.tx_defer).part.low);

   printf ("Rx Bytes:                    %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_bytes).part.high,
           ((WTI_StatField)enet_stats.rx_bytes).part.low);

   printf ("Tx Exess Defer:              %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_excess_defer).part.high,
           ((WTI_StatField)enet_stats.tx_excess_defer).part.low);

   printf ("Rx Packets:                  %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_packets).part.high,
           ((WTI_StatField)enet_stats.rx_packets).part.low);

   printf ("Tx Single Collision:         %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_single_collision).part.high,
           ((WTI_StatField)enet_stats.tx_single_collision).part.low);

   printf ("Rx Error FCS:                %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_err_fcs).part.high,
           ((WTI_StatField)enet_stats.rx_err_fcs).part.low);

   printf ("Tx Multi Collision:          %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_multi_collision).part.high,
           ((WTI_StatField)enet_stats.tx_multi_collision).part.low);

   printf ("Rx Multicast:                %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_multicast).part.high,
           ((WTI_StatField)enet_stats.rx_multicast).part.low);

   printf ("Tx Late Collision:           %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_late_collision).part.high,
           ((WTI_StatField)enet_stats.tx_late_collision).part.low);

   printf ("Rx Broadcast:                %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_broadcast).part.high,
           ((WTI_StatField)enet_stats.rx_broadcast).part.low);

   printf ("Tx Excess Collision:         %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_excess_collision).part.high,
           ((WTI_StatField)enet_stats.tx_excess_collision).part.low);

   printf ("Rx Mac Control:              %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_mac_control).part.high,
           ((WTI_StatField)enet_stats.rx_mac_control).part.low);

   printf ("Tx No Collision:             %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_no_collision).part.high,
           ((WTI_StatField)enet_stats.tx_no_collision).part.low);

   printf ("Rx Mac Pause:                %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_mac_pause).part.high,
           ((WTI_StatField)enet_stats.rx_mac_pause).part.low);

   printf ("Tx Mac Pause Honored:        %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_mac_pause_honored).part.high,
           ((WTI_StatField)enet_stats.tx_mac_pause_honored).part.low);

   printf ("Rx Mac Unknown:              %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_mac_unknown).part.high,
           ((WTI_StatField)enet_stats.rx_mac_unknown).part.low);

   printf ("Tx Dropped:                  %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_dropped).part.high,
           ((WTI_StatField)enet_stats.tx_dropped).part.low);

   printf ("Rx Error Alignment:          %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_err_alignment).part.high,
           ((WTI_StatField)enet_stats.rx_err_alignment).part.low);

   printf ("Tx Jabber:                   %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_jabber).part.high,
           ((WTI_StatField)enet_stats.tx_jabber).part.low);

   printf ("Rx Error LEN:                %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_err_length).part.high,
           ((WTI_StatField)enet_stats.rx_err_length).part.low);

   printf ("Tx Errors FCS:               %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_err_fcs).part.high,
           ((WTI_StatField)enet_stats.tx_err_fcs).part.low);

   printf ("Rx Error Code:               %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_err_code).part.high,
           ((WTI_StatField)enet_stats.rx_err_code).part.low);

   printf ("Tx Control:                  %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_control).part.high,
           ((WTI_StatField)enet_stats.tx_control).part.low);

   printf ("Rx False Carrier:            %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_false_carrier).part.high,
           ((WTI_StatField)enet_stats.rx_false_carrier).part.low);

   printf ("Tx Oversize:                 %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_oversize).part.high,
           ((WTI_StatField)enet_stats.tx_oversize).part.low);

   printf ("Rx Undersize:                %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_undersize).part.high,
           ((WTI_StatField)enet_stats.rx_undersize).part.low);

   printf ("Tx Undersize:                %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_undersize).part.high,
           ((WTI_StatField)enet_stats.tx_undersize).part.low);

   printf ("Rx Oversize:                 %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_oversize).part.high,
           ((WTI_StatField)enet_stats.rx_oversize).part.low);

   printf ("Tx Fragments:                %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_fragments).part.high,
           ((WTI_StatField)enet_stats.tx_fragments).part.low);

   printf ("Rx Fragments:                %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_fragments).part.high,
           ((WTI_StatField)enet_stats.rx_fragments).part.low);
   printf ("Rx Jabber:                   %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_jabber).part.high,
           ((WTI_StatField)enet_stats.rx_jabber).part.low);
   printf ("Rx Dropped:                  %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_dropped).part.high,
           ((WTI_StatField)enet_stats.rx_dropped).part.low);

   printf ("HS ENET Device Statistics (DPS)\n");

   printf ("Rx Host Frames:              %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_host_frames).part.high,
           ((WTI_StatField)enet_stats.rx_host_frames).part.low);
   printf ("Rx Iw Frames:                %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_iw_frames).part.high,
           ((WTI_StatField)enet_stats.rx_iw_frames).part.low);
   printf ("Rx Error Host Full:          %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_err_host_full).part.high,
           ((WTI_StatField)enet_stats.rx_err_host_full).part.low);
   printf ("Rx Error Fbp Underrun:       %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_err_fbp_underrun).part.high,
           ((WTI_StatField)enet_stats.rx_err_fbp_underrun).part.low);
   printf ("Rx Error Nonvalid Mac:       %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_err_nonvalid_mac).part.high,
           ((WTI_StatField)enet_stats.rx_err_nonvalid_mac).part.low);
   printf ("Rx Error Mru:                %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_err_mru).part.high,
           ((WTI_StatField)enet_stats.rx_err_mru).part.low);
   printf ("Rx Error Sdu:                %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_err_sdu).part.high,
           ((WTI_StatField)enet_stats.rx_err_sdu).part.low);
   printf ("Tx Error Underrun:           %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_err_underrun).part.high,
           ((WTI_StatField)enet_stats.tx_err_underrun).part.low);
   printf ("Rx Error Overrun:            %08x%08x\n",
           ((WTI_StatField)enet_stats.rx_err_overrun).part.high,
           ((WTI_StatField)enet_stats.rx_err_overrun).part.low);
   printf ("Tx Frames:                   %08x%08x\n",
           ((WTI_StatField)enet_stats.tx_frames).part.high,
           ((WTI_StatField)enet_stats.tx_frames).part.low);
   printf("----------------------------------------------------\n");

#endif /* WTI_XGI_MODE */

}

#endif

#if WTI_CESOP_TDI
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_TdiTransDeviceStatistics(char *StrPrm)
{
   WP_stats_tdi_trans dev_stats = {0};
   WP_status status;
   WP_U32 t1, t2, screen_coord, temp1, temp2;
   WP_U32 cur_line;
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= WTI_ACTIVE_TDI_PORTS)
   {
      WTI_TerminatePrintError("CLI_F_TdiTransDeviceStatistics", __LINE__);
      return;
   }

   Vt100GotoXy(1,1);
   Vt100ClearScreen();

   while (1)
   {
      t1 = WP_TimeRead();

      if (WTI_SampleSerial())
      {
         break;
      }
      while (1)
      {
         t2 = WP_TimeRead();
         if( (t2-t1) >= FACTOR)
         {
            status = WP_DeviceStatistics(the_system->tdm_trans_dev[val], &dev_stats);
            if (status != WP_OK) return;

            Vt100GotoXy(1,1);
            printf("DEVICE STATISTICS:\n");
            printf("\n");
            printf("TDI TRANSPARENT DEVICE %d ( DEVICE HANDLE %x ):\n",val,the_system->tdm_trans_dev[val]);
            printf("-------------------------------------------------\n");
            printf("\n");
            screen_coord = 5;
            cur_line=1;
            temp1 = dev_stats.ces_rx_err_overrun;
            temp2 = dev_stats.ces_rx_err_overrun >> 32;
            printf("Rx overruns:                             Tx underruns:");
            Vt100GotoXy(23,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = dev_stats.ces_tx_err_underrun;
            temp2 = dev_stats.ces_tx_err_underrun >> 32;
            Vt100GotoXy(75,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;
#ifdef AUTOMATION_MODE
            return;
#else
            break;
#endif
         }
      }
   }
}
#else
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_EmphyPortStatistics(char *StrPrm)
{
   WP_stats_emphy emphy_stats={0}; /*  */

   WP_status status;
   WP_U32 t1, t2, screen_coord, temp1, temp2;
   WP_U32 cur_line;
   WP_U32 res, cmd;

   res = sscanf(StrPrm, "%d", &cmd);

   if (res != 1)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   Vt100GotoXy(1,1);
   Vt100ClearScreen();

   while (1)
   {
      t1 = WP_TimeRead();

      if (WTI_SampleSerial())
      {
         break;
      }
      while (1)
      {
         t2 = WP_TimeRead();
         if( (t2-t1) >= FACTOR)
         {
            status = WP_EmphyPortStatistics(the_system->trans_port, &emphy_stats);
            if (status != WP_OK) return;

            Vt100GotoXy(1,1);
            printf("EMPHY Port STATISTICS:\n");
            printf("\n");
            printf("Port HANDLE %x:\n",the_system->trans_port);
            printf("-------------------------------------------------\n");
            printf("\n");
            screen_coord = 5;
            cur_line=1;
            temp1 = emphy_stats.pos_err;
            temp2 = emphy_stats.pos_err >> 32;
            printf("pos_err:                                     rx_err_parity:");
            Vt100GotoXy(23,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = emphy_stats.rx_err_parity;
            temp2 = emphy_stats.rx_err_parity >> 32;
            Vt100GotoXy(75,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;


            temp1 = emphy_stats.rx_unsupported_device;
            temp2 = emphy_stats.rx_unsupported_device >> 32;
            printf("rx_unsupported_device:                       tx_unsupported_device:");
            Vt100GotoXy(23,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = emphy_stats.tx_unsupported_device;
            temp2 = emphy_stats.tx_unsupported_device >> 32;
            Vt100GotoXy(75,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;

            temp1 = emphy_stats.rx_err_sop_eop;
            temp2 = emphy_stats.rx_err_sop_eop >> 32;
            printf("rx_err_sop_eop:                       ");
            Vt100GotoXy(23,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;

            break;
         }
      }
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_EmphyTransDeviceStatistics(char *StrPrm)
{
   WP_stats_emphy_trans dev_stats = {0};
   WP_status status;
   WP_U32 t1, t2, screen_coord, temp1, temp2;
   WP_U32 cur_line;
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_EmphyTransDeviceStatistics", __LINE__);
      return;
   }

   Vt100GotoXy(1,1);
   Vt100ClearScreen();

   while (1)
   {
      t1 = WP_TimeRead();

      if (WTI_SampleSerial())
      {
         break;
      }
      while (1)
      {
         t2 = WP_TimeRead();
         if( (t2-t1) >= FACTOR)
         {
            status = WP_DeviceStatistics(the_system->pw[val].trans_device, &dev_stats);
            if (status != WP_OK) return;

            Vt100GotoXy(1,1);
            printf("DEVICE STATISTICS:\n");
            printf("\n");
            printf("EMPHY TRANSPARENT DEVICE %d ( DEVICE HANDLE %x ):\n",val,the_system->pw[val].trans_device);
            printf("-------------------------------------------------\n");
            printf("\n");
            screen_coord = 5;
            cur_line=1;
            temp1 = dev_stats.phy_buffer_overrun;
            temp2 = dev_stats.phy_buffer_overrun >> 32;
            printf("phy_buffer_overrun:                             phy_out_of_sync:");
            Vt100GotoXy(23,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = dev_stats.phy_out_of_sync;
            temp2 = dev_stats.phy_out_of_sync >> 32;
            Vt100GotoXy(75,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;
#ifdef AUTOMATION_MODE
            return;
#else
            break;
#endif
         }
      }
   }
}
#endif

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Pwe3RxChannelStatistics(char *StrPrm)
{
#if WTI_CESOP_TDI
   WP_stats_tdi_trans_pwe3_rx rx_stats = {0};
#else
   WP_stats_emphy_trans_pwe3_rx rx_stats = {0};
   WP_U32 FBP_Size=0, min_q=0x10000,max_q=0;
#endif
   WP_status status;
   WP_U32 t1, t2, screen_coord, temp1, temp2;
   WP_U32 cur_line;
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3RxChannelStatistics", __LINE__);
      return;
   }

   Vt100GotoXy(1,1);
   Vt100ClearScreen();

   while (1)
   {
      t1 = WP_TimeRead();

      if (WTI_SampleSerial())
      {
         break;
      }
      while (1)
      {
         t2 = WP_TimeRead();
         if( (t2-t1) >= FACTOR)
         {
            status = WP_ChannelStatistics(the_system->pw[val].trans_rx, &rx_stats);
            if (status != WP_OK) return;

#if WTI_CESOP_TDI
            Vt100GotoXy(1,1);
            printf("CHANNEL STATISTICS:\n");
            printf("\n");
            printf("TRANSPARENT PWE3 RX CHANNEL %d ( CHANNEL HANDLE %x ):\n",
                   val,
                   the_system->pw[val].trans_rx);
            printf("-----------------------------------------------------\n");
            printf("\n");
            screen_coord = 5;
            cur_line=1;
            temp1 = rx_stats.empty_fbp;
            temp2 = rx_stats.empty_fbp >> 32;
            printf("empty_fbp:                                                 valid_packets");
            Vt100GotoXy(23,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = rx_stats.valid_packets;
            temp2 = rx_stats.valid_packets >> 32;
            Vt100GotoXy(75,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;
#else
            Vt100GotoXy(1,1);
            printf("CHANNEL STATISTICS:\n");
            printf("\n");
            printf("TRANSPARENT PWE3 RX CHANNEL %d ( CHANNEL HANDLE %x ):\n",
                   val,
                   the_system->pw[val].trans_rx);
            printf("-----------------------------------------------------\n");
            printf("\n");
            screen_coord = 5;
            cur_line=1;
            temp1 = rx_stats.channel_out_of_sync;
            temp2 = rx_stats.channel_out_of_sync >> 32;
            printf("channel_out_of_sync:                                    empty fbp:");
            Vt100GotoXy(23,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = rx_stats.empty_fbp;
            temp2 = rx_stats.empty_fbp >> 32;
            Vt100GotoXy(75,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;
            temp1 = rx_stats.valid_packets;
            temp2 = rx_stats.valid_packets >> 32;
            printf("valid_packets:                                          RX TS sync lost:");
            Vt100GotoXy(23,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = rx_stats.rx_ts_sync_lost;
            temp2 = rx_stats.rx_ts_sync_lost >> 32;
            Vt100GotoXy(75,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;




#if WTI_CES_SHAPING_ENABLED
            if(test_setup.line_cr_operation_mode[(val+(val/21)*7)] == WP_IW_CLOCK_REC_ADAPTIVE_MODE)
            {
            printf("\n");
            printf(" CES shaping stats:    \n");
            printf("--------------------------------------------------------------\n");
            cur_line +=3;
            temp1 = rx_stats.ces_shaping_stats.underrun;
            temp2 = rx_stats.ces_shaping_stats.underrun >> 32;
            printf("underrun (FTD packets = 0):                      l2pi fifo put error:");
            Vt100GotoXy(30,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = rx_stats.ces_shaping_stats.fifo_put_error;
            temp2 = rx_stats.ces_shaping_stats.fifo_put_error >> 32;
            Vt100GotoXy(75,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;


            temp1 = rx_stats.ces_shaping_stats.overrun;
            temp2 = rx_stats.ces_shaping_stats.overrun >> 32;
            printf("overrun  (FTD packets > 1):                      l2pi fifo get error:");
            Vt100GotoXy(30,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = rx_stats.ces_shaping_stats.fifo_get_error;
            temp2 = rx_stats.ces_shaping_stats.fifo_get_error >> 32;
            Vt100GotoXy(75,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;

            temp1 = rx_stats.ces_shaping_stats.invalid_delta_ts;
            temp2 = rx_stats.ces_shaping_stats.invalid_delta_ts >> 32;
            printf("invalid delta ts:                                good packets (fifo put):");
            Vt100GotoXy(30,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = rx_stats.ces_shaping_stats.good_packets;
            temp2 = rx_stats.ces_shaping_stats.good_packets >> 32;
            Vt100GotoXy(75,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;
            }
#endif




            {

               status = WP_QNodeStatus(the_system->qnode[WTI_QNODE_IW_TDM2PSN].handle,
                                       WP_QNODE_STATUS_FIFO,
                                       &FBP_Size);

               if (status != WP_OK)

                  return;

               if(FBP_Size>max_q)

                  max_q=FBP_Size;

               if(FBP_Size<min_q)

                  min_q=FBP_Size;

               printf("\nFree Buffer Pool size: 0x%02x (min 0x%02x,   max 0x%02x)\n",FBP_Size,min_q,max_q);

            }
#endif
#ifdef AUTOMATION_MODE
            return;
#else
            break;
#endif
         }
      }
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Pwe3TxChannelStatistics(char *StrPrm)
{
#if WTI_CESOP_TDI
   WP_stats_tdi_trans_pwe3_tx tx_stats = {0};
#else
   WP_stats_emphy_trans_pwe3_tx tx_stats = {0};
#endif
   WP_U32 qdepth=0 , min_q=0x10000,max_q=0;
   WP_status status;
   WP_U32 t1, t2, screen_coord, temp1, temp2;
   WP_U32 cur_line;
   WP_U32 res, cmd, val;
   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3TxChannelStatistics", __LINE__);
      return;
   }

   Vt100GotoXy(1,1);
   Vt100ClearScreen();

   while (1)
   {
      t1 = WP_TimeRead();

      if (WTI_SampleSerial())
      {
         break;
      }
      while (1)
      {
         t2 = WP_TimeRead();
         if( (t2-t1) >= FACTOR)
         {
            status = WP_ChannelStatistics(the_system->pw[val].trans_tx, &tx_stats);
            if (status != WP_OK) return;
            Vt100GotoXy(1,1);
            printf("CHANNEL STATISTICS:\n");
            printf("\n");
            printf("TRANSPARENT PWE3 TX CHANNEL %d ( CHANNEL HANDLE %x ):\n",
                   val,
                   the_system->pw[val].trans_tx);

            printf("-----------------------------------------------------\n");
            printf("\n");
            screen_coord = 5;
            cur_line=1;
            temp1 = tx_stats.dummy_packet;
            temp2 = tx_stats.dummy_packet >> 32;
            printf("dummy_packet:                                     underrun_dataunit:");
            Vt100GotoXy(23,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = tx_stats.underrun_dataunit;
            temp2 = tx_stats.underrun_dataunit >> 32;
            Vt100GotoXy(75,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;
            temp1 = tx_stats.buffer_underrun;
            temp2 = tx_stats.buffer_underrun >> 32;
            printf("buffer_underrun:                                  valid_packets:");
            Vt100GotoXy(23,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = tx_stats.valid_packets;
            temp2 = tx_stats.valid_packets >> 32;
            Vt100GotoXy(75,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;
            {

               status = WP_ChannelQDepth(the_system->pw[val].trans_tx, &qdepth);

               if (status != WP_OK)

                  return;

               if(qdepth>max_q)

                  max_q=qdepth;

               if(qdepth<min_q)

                  min_q=qdepth;

               printf("Number of packets in queue: 0x%02x (min 0x%02x,   max 0x%02x)\n",qdepth,min_q,max_q);

            }


#ifdef AUTOMATION_MODE
            return;
#else
            break;
#endif
         }
      }
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Pwe3RxChannelStatisticsClear(char *StrPrm)
{
#if WTI_CESOP_TDI
   WP_stats_tdi_trans_pwe3_rx rx_stats = {0};
#else
   WP_stats_emphy_trans_pwe3_rx rx_stats = {0};
#endif
   WP_status status;
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3RxChannelStatisticsClear", __LINE__);
      return;
   }

   status = WP_ChannelStatisticsReset(the_system->pw[val].trans_rx,
                                      &rx_stats);
   WTI_TerminateOnError(status, "WP_ChannelStatisticsReset", __LINE__);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Pwe3TxChannelStatisticsClear(char *StrPrm)
{
#if WTI_CESOP_TDI
   WP_stats_tdi_trans_pwe3_tx tx_stats = {0};
#else
   WP_stats_emphy_trans_pwe3_tx tx_stats = {0};
#endif
   WP_status status;
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3TxChannelStatisticsClear", __LINE__);
      return;
   }

   status = WP_ChannelStatisticsReset(the_system->pw[val].trans_tx,
                                      &tx_stats);
   WTI_TerminateOnError(status, "WP_ChannelStatisticsReset", __LINE__);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_HostChannelStatistics(char *StrPrm)
{
   WP_iw_flow_stats Total={0};
   WP_status status;
   WP_U32 t1, t2, screen_coord, temp1, temp2;
   WP_U32 cur_line;
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= N_HOST_RX_CHANNELS)
   {
      WTI_TerminatePrintError("CLI_F_HostChannelStatistics", __LINE__);
      return;
   }

   Vt100GotoXy(1,1);
   Vt100ClearScreen();

   while (1)
   {
      t1 = WP_TimeRead();

      if (WTI_SampleSerial())
      {
         break;
      }
      while (1)
      {
         t2 = WP_TimeRead();
         if( (t2-t1) >= FACTOR)
         {
            status = WP_IwFlowStatistics(the_system->host_term_flow_agg,WP_IW_FLOW_STAT, &Total);
            if (status != WP_OK) return;

            Vt100GotoXy(1,1);
            printf("HOST FLOW AGGREGATION STATISTICS:\n");
            printf("\n");
            printf("HOST FLOW AGGREGATION HANDLE %x ):\n",the_system->host_term_flow_agg);
            printf("--------------------------------------------------------------\n");
            printf("\n");
            screen_coord = 5;
            cur_line=1;
            temp1 = Total.forward_packet;
            temp2 = Total.forward_packet >> 32;
            printf("forward_packet:                                  fbp_drop_packets:");
            Vt100GotoXy(23,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = Total.fbp_drop_packets;
            temp2 = Total.fbp_drop_packets >> 32;
            Vt100GotoXy(75,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;
            temp1 = Total.mtu_drop_packets;
            temp2 = Total.mtu_drop_packets >> 32;
            printf("mtu_drop_packets:                                tx_queue_drop_packets:");
            Vt100GotoXy(23,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = Total.tx_queue_drop_packets;
            temp2 = Total.tx_queue_drop_packets >> 32;
            Vt100GotoXy(75,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;
#ifdef AUTOMATION_MODE
            return;
#else
            break;
#endif
         }
      }
   }
}

#ifdef __linux__
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_TunDriverListen(char *StrPrm)
{
   if (fork() != 0)
   {
      tun_tread();
   }
}
#endif

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_LoadHostBus(char *StrPrm)
{
    WP_U32 res, cmd, delay,num_read;
    WP_U32 *addr;
    WP_U16 dummy;
    WP_U32 i;


    res = sscanf(StrPrm, "%d %d %d", &cmd, &delay,&num_read);

    if (res != 3)
    {
       WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
       return;
    }
    printf("Read from  Host Bus :  %d read actions every %d msec    \n",num_read,delay);

    while(1)
    {
       if (WTI_SampleSerial())
       {
          break;
       }
       /*Dummy read */
       for (i=0; i<num_read; i++)
       {/* dummy read num_read times one after the other each read of 64 bit takes ~ 20 host clocks*/
        /* i.e. if delay = 1msec -> 1m/(5n*20) = 10000 > = num_read */
          addr = (WP_U32 *)WPL_PHY_VIRTUAL(0,CR_HW_MODE_REG_ADD);
          dummy = *(WP_U16 *) addr;
          /* printf("Read from  Host Bus, delay = %d , read number = %d [%d] \n",delay,i,num_read);*/

       }
       WP_Delay(delay);
    }
}

void CLI_F_IwPortStatistics(char *StrPrm)
{
   WP_route_port_stats iw_stats = {0};
   WP_status status;
   WP_U32 t1=0, t2=0, screen_coord, hor_coord, temp1, temp2;
   WP_U32 i, j=0;

   Vt100GotoXy(1,1);
   Vt100ClearScreen();

   if(the_system->iw_system_mode == WTI_IW_SYSTEM_ROUTING)
   {
      printf("Displaying stats for PSN->TDM IW port (rx_binding.input_port):\n");
   }
   else if(the_system->iw_system_mode == WTI_IW_SYSTEM_BRIDGE)
   {
      printf("Displaying stats for PSN->TDM bport (rx_binding_gbe.input_port):\n");
   }

   while (1)
   {
      t1 = WP_TimeRead();

      if (WTI_SampleSerial())
      {
         break;
      }

      while (1)
      {
         t2 = WP_TimeRead();

         if(WP_TimeDelta(t2,t1) <= FACTOR)
         {
            if(the_system->iw_system_mode == WTI_IW_SYSTEM_ROUTING)
            {
               status =  WP_IwPortStatistics(the_system->iw_port, &iw_stats);
            }
            else if(the_system->iw_system_mode == WTI_IW_SYSTEM_BRIDGE)
            {
               status =  WP_IwPortStatistics(the_system->bridge_iw_port, &iw_stats);
            }

            if (status)
            {
               printf("GetIWStatistics error\n");
               return;
            }

            Vt100GotoXy(1,1);

            screen_coord = 3;
            hor_coord = 0;

            Vt100GotoXy(1,screen_coord);

            temp1 = iw_stats.rx_valid_packets;
            temp2 = iw_stats.rx_valid_packets >> 32;
            Vt100GotoXy(hor_coord+24,screen_coord);
            printf("rx_valid_packets:                       %.8x%.8x\n", temp2,temp1);
            temp1 = iw_stats.rx_ipv4_option_packets;
            temp2 = iw_stats.rx_ipv4_option_packets >> 32;
            Vt100GotoXy(hor_coord+24,screen_coord+1);
            printf("rx_ipv4_option_packets:                 %.8x%.8x\n", temp2,temp1);
            temp1 = iw_stats.rx_non_ip_packets;
            temp2 = iw_stats.rx_non_ip_packets >> 32;
            Vt100GotoXy(hor_coord+24,screen_coord+2);
            printf("rx_non_ip_packets:                      %.8x%.8x\n", temp2,temp1);
            temp1 = iw_stats.rx_compressed_packets;
            temp2 = iw_stats.rx_compressed_packets >> 32;
            Vt100GotoXy(hor_coord+24,screen_coord+3);
            printf("rx_compressed_packets:                  %.8x%.8x\n", temp2,temp1);
            temp1 = iw_stats.rx_valid_mpls_packets;
            temp2 = iw_stats.rx_valid_mpls_packets >> 32;
            Vt100GotoXy(hor_coord+24,screen_coord+4);
            printf("rx_valid_mpls_packets:                  %.8x%.8x\n", temp2,temp1);
            temp1 = iw_stats.rx_protocol_error;
            temp2 = iw_stats.rx_protocol_error >> 32;
            Vt100GotoXy(hor_coord+24,screen_coord+5);
            printf("rx_protocol_error:                      %.8x%.8x\n", temp2,temp1);
            temp1 = iw_stats.rx_checksum_error;
            temp2 = iw_stats.rx_checksum_error >> 32;
            Vt100GotoXy(hor_coord+24,screen_coord+6);
            printf("rx_checksum_error:                      %.8x%.8x\n", temp2,temp1);
            temp1 = iw_stats.rx_discard_classifier;
            temp2 = iw_stats.rx_discard_classifier >> 32;
            Vt100GotoXy(hor_coord+24,screen_coord+7);
            printf("rx_discard_classifier:                  %.8x%.8x\n", temp2,temp1);
            temp1 = iw_stats.rx_mpls_lookup_drop;
            temp2 = iw_stats.rx_mpls_lookup_drop >> 32;
            Vt100GotoXy(hor_coord+24,screen_coord+8);
            printf("rx_mpls_lookup_drop:                    %.8x%.8x\n", temp2,temp1);
            temp1 = iw_stats.tx_forwarded_packets;
            temp2 = iw_stats.tx_forwarded_packets >> 32;
            Vt100GotoXy(hor_coord+24,screen_coord+9);
            printf("tx_forwarded_packets:                   %.8x%.8x\n", temp2,temp1);
            temp1 = iw_stats.rx_ipv6_hop_by_hop_host_term;
            temp2 = iw_stats.rx_ipv6_hop_by_hop_host_term >> 32;
            Vt100GotoXy(hor_coord+24,screen_coord+10);
            printf("rx_ipv6_hop_by_hop_host_term:           %.8x%.8x\n", temp2,temp1);
            temp1 = iw_stats.rx_ipv6_link_local_host_term;
            temp2 = iw_stats.rx_ipv6_link_local_host_term >> 32;
            Vt100GotoXy(hor_coord+24,screen_coord+11);
            printf("rx_ipv6_link_local_host_term:           %.8x%.8x\n", temp2,temp1);
            temp1 = iw_stats.rx_discard_lpm;
            temp2 = iw_stats.rx_discard_lpm >> 32;
            Vt100GotoXy(hor_coord+24,screen_coord+12);
            printf("rx_discard_lpm:                         %.8x%.8x\n", temp2,temp1);

            for(i=0; i<WP_IW_DFC_MAX_NUM_OF_FILTERS; i++)
            {
               if(iw_stats.discard_dfc_filter[i])
               {
                  temp1 = iw_stats.discard_dfc_filter[i];
                  temp2 = iw_stats.discard_dfc_filter[i] >> 32;
                  Vt100GotoXy(hor_coord+24,(screen_coord+13+i));
                  printf("DFC filter %d.discard_dfc_filter:   %.8x%.8x\n", i, temp2, temp1);
                  ++j;
               }
            }

            for(i=0; i<WP_PCE_MAX_NUM_OF_FILTERS; i++)
            {
               if(iw_stats.discard_pce_filter[i])
               {
                  temp1 = iw_stats.discard_pce_filter[i];
                  temp2 = iw_stats.discard_pce_filter[i] >> 32;
                  Vt100GotoXy(hor_coord+24,screen_coord+13+j+i);
                  printf("PCE filter %d.discard_pce_filter:   %.8x%.8x\n", i, temp2, temp1);
                  ++j;
               }
            }

            temp1 = iw_stats.ingress_policer_non_conforming_packets;
            temp2 = iw_stats.ingress_policer_non_conforming_packets >> 32;
            Vt100GotoXy(hor_coord+24,screen_coord+13+j);
            printf("ingress_policer_non_conforming_packets: %.8x%.8x\n", temp2,temp1);

#if WTI_PCE_CLASSIFIER
            //WP_PceGlobalStatisticsReset (WP_WINPATH(0), &pce_global_stats);
            WP_PceFilterStatisticsReset (the_system->pce_filter_handle, &(pce_filter_stats[0]));
            WTI_CheckPceFilterStats(the_system->pce_filter_handle, 0, 0, 0);
#endif

#ifdef AUTOMATION_MODE
            return;
#else
            break;
#endif
         }
      }
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggStatistics(char *StrPrm)
{
   WP_stats_iw_cesop_tdm2psn cesop_stats = {0};
   WP_status status;
   WP_U32 t1, t2, screen_coord, temp1, temp2;
   WP_U32 cur_line;
   WP_U32 res, cmd, val, val1;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &val, &val1);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggStatistics", __LINE__);
      return;
   }
   if (val1 != 0 && val1 != 1)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggStatistics", __LINE__);
      return;
   }

   Vt100GotoXy(1,1);
   Vt100ClearScreen();

   while (1)
   {
      t1 = WP_TimeRead();

      if (WTI_SampleSerial())
      {
         break;
      }
      while (1)
      {
         t2 = WP_TimeRead();
         if( (t2-t1) >= FACTOR)
         {
            if (val1)
            {
               status = WP_IwFlowStatistics(the_system->pw[val].cas_tdm2psn_flow_agg,
                                            WP_IW_FLOW_STAT_CESOP_TDM2PSN,
                                            &cesop_stats);
            }
            else
            {
               status = WP_IwFlowStatistics(the_system->pw[val].tdm2psn_flow_agg,
                                            WP_IW_FLOW_STAT_CESOP_TDM2PSN,
                                            &cesop_stats);
            }

            if (status != WP_OK) return;

            Vt100GotoXy(1,1);
            printf("FLOW AGGREGATION STATISTICS:\n");
            printf("\n");
            if (val1)
               printf("TDM TO PSN CAS DEDICATED FLOW AGGREGATION %d ( FLOW AGGREGATION HANDLE %x ):\n",
                      val,
                      the_system->pw[val].cas_tdm2psn_flow_agg);
            else
               printf("TDM TO PSN FLOW AGGREGATION %d ( FLOW AGGREGATION HANDLE %x ):\n",
                      val,
                      the_system->pw[val].tdm2psn_flow_agg);
            printf("--------------------------------------------------------------\n");
            printf("\n");
            screen_coord = 5;
            cur_line=1;
            temp1 = cesop_stats.forwarded_packets;
            temp2 = cesop_stats.forwarded_packets >> 32;
            printf("forwarded_packets:                                  fbp_drop_packets:");
            Vt100GotoXy(23,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = cesop_stats.fbp_drop_packets;
            temp2 = cesop_stats.fbp_drop_packets >> 32;
            Vt100GotoXy(75,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;
            temp1 = cesop_stats.mtu_drop_packets;
            temp2 = cesop_stats.mtu_drop_packets >> 32;
            printf("mtu_drop_packets:                                   tx_queue_drop_packets:");
            Vt100GotoXy(23,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = cesop_stats.tx_queue_drop_packets;
            temp2 = cesop_stats.tx_queue_drop_packets >> 32;
            Vt100GotoXy(75,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;



#ifdef AUTOMATION_MODE
            return;
#else
            break;
#endif
         }
      }
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmFlowAggStatistics(char *StrPrm)
{
   WP_stats_iw_cesop_psn2tdm cesop_stats = {0};
   WP_status status;
   WP_U32 t1, t2, screen_coord, temp1, temp2;
   WP_U32 cur_line;
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Psn2TdmFlowAggStatistics", __LINE__);
      return;
   }

   Vt100GotoXy(1,1);
   Vt100ClearScreen();

   while (1)
   {
      t1 = WP_TimeRead();

      if (WTI_SampleSerial())
      {
         break;
      }
      while (1)
      {
         t2 = WP_TimeRead();
         if( (t2-t1) >= FACTOR)
         {
            status = WP_IwFlowStatistics(the_system->pw[val].psn2tdm_flow_agg,
                                         WP_IW_FLOW_STAT_CESOP_PSN2TDM,
                                         &cesop_stats);
            if (status != WP_OK) return;

            Vt100GotoXy(1,1);
            printf("FLOW AGGREGATION STATISTICS:\n");
            printf("\n");
            printf("PSN TO TDM FLOW AGGREGATION %d ( FLOW AGGREGATION HANDLE %x ):\n",
                   val,
                   the_system->pw[val].psn2tdm_flow_agg);
            printf("--------------------------------------------------------------\n");
            printf("\n");
            screen_coord = 5;
            cur_line=1;
            temp1 = cesop_stats.forwarded_packets;
            temp2 = cesop_stats.forwarded_packets >> 32;
            printf("forwarded_packets:                        fbp_drop_packets:");
            Vt100GotoXy(26,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = cesop_stats.fbp_drop_packets;
            temp2 = cesop_stats.fbp_drop_packets >> 32;
            Vt100GotoXy(74,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;
            temp1 = cesop_stats.out_of_window_packets;
            temp2 = cesop_stats.out_of_window_packets >> 32;
            printf("out_of_window_packets:                    buffer_overun_dropped_packets:");
            Vt100GotoXy(26,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = cesop_stats.buffer_overun_dropped_packets;
            temp2 = cesop_stats.buffer_overun_dropped_packets >> 32;
            Vt100GotoXy(74,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;
            temp1 = cesop_stats.window_switchover;
            temp2 = cesop_stats.window_switchover >> 32;
            printf("window_switchover:                        buffer_overun_events:");
            Vt100GotoXy(26,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = cesop_stats.buffer_overun_events;
            temp2 = cesop_stats.buffer_overun_events >> 32;
            Vt100GotoXy(74,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;
            temp1 = cesop_stats.stray_packets;
            temp2 = cesop_stats.stray_packets >> 32;
            printf("stray_packets:                            malformed_packets:");
            Vt100GotoXy(26,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = cesop_stats.malformed_packets;
            temp2 = cesop_stats.malformed_packets >> 32;
            Vt100GotoXy(74,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;
            temp1 = cesop_stats.cw_ais_drop_packets;
            temp2 = cesop_stats.cw_ais_drop_packets >> 32;
            printf("cw_ais_drop_packets:                      multiple_packets:");
            Vt100GotoXy(26,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = cesop_stats.multiple_packets;
            temp2 = cesop_stats.multiple_packets >> 32;
            Vt100GotoXy(74,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;
            temp1 = cesop_stats.mpls_drop_packets;
            temp2 = cesop_stats.mpls_drop_packets >> 32;
            printf("mpls_drop_packets:                        denied_packets:");
            Vt100GotoXy(26,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = cesop_stats.denied_packets;
            temp2 = cesop_stats.denied_packets >> 32;
            Vt100GotoXy(74,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;
            temp1 = cesop_stats.out_of_sequence_packets;
            temp2 = cesop_stats.out_of_sequence_packets >> 32;
            printf("out_of_sequence_packets:                  out_of_band_cas_packets:");
            Vt100GotoXy(26,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = cesop_stats.out_of_band_cas_packets;
            temp2 = cesop_stats.out_of_band_cas_packets >> 32;
            Vt100GotoXy(74,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            cur_line++;
            temp1 = cesop_stats.rai_packets;
            temp2 = cesop_stats.rai_packets >> 32;
            printf("rai_packets:                              rdi_dropped_packets:");
            Vt100GotoXy(26,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);
            temp1 = cesop_stats.rdi_dropped_packets;
            temp2 = cesop_stats.rdi_dropped_packets >> 32;
            Vt100GotoXy(74,screen_coord+cur_line);
            printf("%.8x%.8x\n", temp2,temp1);

#ifdef AUTOMATION_MODE
            return;
#else
            break;
#endif
         }
      }
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggStatisticsClear(char *StrPrm)
{
   WP_stats_iw_cesop_tdm2psn cesop_stats = {0};
   WP_status status;
   WP_U32 res, cmd, val, val1;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &val, &val1);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggStatistics", __LINE__);
      return;
   }
   if (val1 != 0 && val1 != 1)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnFlowAggStatistics", __LINE__);
      return;
   }

   if (val1)
   {
      status = WP_IwFlowStatisticsReset(the_system->pw[val].cas_tdm2psn_flow_agg,
                                        WP_IW_FLOW_STAT_CESOP_TDM2PSN,
                                        &cesop_stats);
      WTI_TerminateOnError(status, "WP_IwFlowStatisticsReset", __LINE__);
   }
   else
   {
      status = WP_IwFlowStatisticsReset(the_system->pw[val].tdm2psn_flow_agg,
                                        WP_IW_FLOW_STAT_CESOP_TDM2PSN,
                                        &cesop_stats);
      WTI_TerminateOnError(status, "WP_IwFlowStatisticsReset", __LINE__);
   }

   return;
}

#if WTI_CESOP_MPLS_IW
#if WTI_CESOP_TO_MPLS_FA_CASCADING

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnMplsL2RouterFlowAggStatistics(char *StrPrm)
{
   WP_iw_flow_stats flow_stats = {0};
   WP_status status;
   WP_U32 t1=0, t2=0, screen_coord, hor_coord, temp1, temp2;
   WP_U32 res, cmd, val, val1;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &val, &val1);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= WTI_MAX_NUM_OF_MPLS_FA)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnMplsL2RouterFlowAggStatistics", __LINE__);
      return;
   }
   if (val1 != 0 && val1 != 1)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnMplsL2RouterFlowAggStatistics", __LINE__);
      return;
   }

   Vt100GotoXy(1,1);
   Vt100ClearScreen();

   while (1)
   {
      t1 = WP_TimeRead();

      if (WTI_SampleSerial())
      {
         break;
      }
      while (1)
      {
         t2 = WP_TimeRead();
         if( (t2-t1) >= FACTOR)
         {
            if (val1)
            {
               status = WP_IwFlowStatistics(the_system->psn2tdm_mpls_l2_fa_handle[val],
                                            WP_IW_FLOW_STAT,
                                            &flow_stats);
            }
            else
            {
               status = WP_IwFlowStatistics(the_system->tdm2psn_mpls_l2_fa_handle[val],
                                            WP_IW_FLOW_STAT,
                                            &flow_stats);
            }

            if (status != WP_OK) return;

            Vt100GotoXy(1,1);
            screen_coord = 3;
            hor_coord = 24;

            Vt100GotoXy(26,screen_coord+0);
            printf("forward_packet:");
            temp1 = flow_stats.forward_packet;
            temp2 = flow_stats.forward_packet >> 32;
            Vt100GotoXy(72,screen_coord+0);
            printf("%.8x%.8x\n", temp2,temp1);

            Vt100GotoXy(26,screen_coord+1);
            printf("fbp_drop_packets:");
            temp1 = flow_stats.fbp_drop_packets;
            temp2 = flow_stats.fbp_drop_packets >> 32;
            Vt100GotoXy(72,screen_coord+1);
            printf("%.8x%.8x\n", temp2,temp1);

            Vt100GotoXy(26,screen_coord+2);
            printf("mtu_drop_packets:");
            temp1 = flow_stats.mtu_drop_packets;
            temp2 = flow_stats.mtu_drop_packets >> 32;
            Vt100GotoXy(72,screen_coord+2);
            printf("%.8x%.8x\n", temp2,temp1);

            Vt100GotoXy(26,screen_coord+3);
            printf("ttl_drop_packets:");
            temp1 = flow_stats.ttl_drop_packets;
            temp2 = flow_stats.ttl_drop_packets >> 32;
            Vt100GotoXy(72,screen_coord+3);
            printf("%.8x%.8x\n", temp2,temp1);

            Vt100GotoXy(26,screen_coord+4);
            printf("tx_queue_drop_packets:");
            temp1 = flow_stats.tx_queue_drop_packets;
            temp2 = flow_stats.tx_queue_drop_packets >> 32;
            Vt100GotoXy(72,screen_coord+4);
            printf("%.8x%.8x\n", temp2,temp1);

            Vt100GotoXy(26,screen_coord+5);
            printf("mpls_drop:");
            temp1 = flow_stats.mpls_drop;
            temp2 = flow_stats.mpls_drop >> 32;
            Vt100GotoXy(72,screen_coord+5);
            printf("%.8x%.8x\n", temp2,temp1);

            Vt100GotoXy(26,screen_coord+6);
            printf("denied_packets:");
            temp1 = flow_stats.denied_packets;
            temp2 = flow_stats.denied_packets >> 32;
            Vt100GotoXy(72,screen_coord+6);
            printf("%.8x%.8x\n", temp2,temp1);

            Vt100GotoXy(26,screen_coord+7);
            printf("group_filtered_packets:");
            temp1 = flow_stats.group_filtered_packets;
            temp2 = flow_stats.group_filtered_packets >> 32;
            Vt100GotoXy(72,screen_coord+7);
            printf("%.8x%.8x\n", temp2,temp1);

            Vt100GotoXy(26,screen_coord+8);
            printf("forwarded_bytes:");
            temp1 = flow_stats.forwarded_bytes;
            temp2 = flow_stats.forwarded_bytes >> 32;
            Vt100GotoXy(72,screen_coord+8);
            printf("%.8x%.8x\n", temp2,temp1);

            Vt100GotoXy(26,screen_coord+9);
            printf("gtp_bad_headers:");
            temp1 = flow_stats.gtp_bad_headers;
            temp2 = flow_stats.gtp_bad_headers >> 32;
            Vt100GotoXy(72,screen_coord+9);
            printf("%.8x%.8x\n", temp2,temp1);

            printf("\n");
#ifdef AUTOMATION_MODE
            return;
#else
            break;
#endif
         }
      }
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnMplsL2RouterFlowAggStatisticsClear(char *StrPrm)
{
   WP_iw_flow_stats flow_stats;
   WP_status status;
   WP_U32 res, cmd, val, val1;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &val, &val1);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (val >= WTI_MAX_NUM_OF_MPLS_FA)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnMplsL2RouterFlowAggStatisticsClear", __LINE__);
      return;
   }

   if (val1 != 0 && val1 != 1)
   {
      WTI_TerminatePrintError("CLI_F_Tdm2PsnMplsL2RouterFlowAggStatistics", __LINE__);
      return;
   }

   if (val1)
   {
      status = WP_IwFlowStatisticsReset(the_system->psn2tdm_mpls_l2_fa_handle[val],
                                        WP_IW_FLOW_STAT,
                                        &flow_stats);
      WTI_TerminateOnError(status, "CLI_F_Tdm2PsnMplsL2RouterFlowAggStatisticsClear", __LINE__);
   }
   else
   {
      status = WP_IwFlowStatisticsReset(the_system->tdm2psn_mpls_l2_fa_handle[val],
                                        WP_IW_FLOW_STAT,
                                        &flow_stats);
      WTI_TerminateOnError(status, "CLI_F_Tdm2PsnMplsL2RouterFlowAggStatisticsClear", __LINE__);
   }

   return;
}

#endif  /* WTI_CESOP_TO_MPLS_FA_CASCADING */

#if WTI_CESOP_MPLS_OVER_ENET
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggEnetHeaderMpls(char *StrPrm)
{
   WP_U32 res, cmd;
   WP_U32 d_mac0, d_mac1, d_mac2, d_mac3, d_mac4, d_mac5;
   WP_U32 s_mac0, s_mac1, s_mac2, s_mac3, s_mac4, s_mac5;
   WP_U32 type;

   res = sscanf(StrPrm, "%d %x %x %x %x %x %x %x %x %x %x %x %x %x",
                &cmd,
                &d_mac0, &d_mac1, &d_mac2, &d_mac3, &d_mac4, &d_mac5,
                &s_mac0, &s_mac1, &s_mac2, &s_mac3, &s_mac4, &s_mac5, &type);

   if (res != 14)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   memset(&mpls_enet_header ,0,sizeof(WTI_enet_header));

   mpls_enet_header.dest_addr[0] = (WP_U8) d_mac0;
   mpls_enet_header.dest_addr[1] = (WP_U8) d_mac1;
   mpls_enet_header.dest_addr[2] = (WP_U8) d_mac2;
   mpls_enet_header.dest_addr[3] = (WP_U8) d_mac3;
   mpls_enet_header.dest_addr[4] = (WP_U8) d_mac4;
   mpls_enet_header.dest_addr[5] = (WP_U8) d_mac5;
   mpls_enet_header.source_addr[0] = (WP_U8) s_mac0;
   mpls_enet_header.source_addr[1] = (WP_U8) s_mac1;
   mpls_enet_header.source_addr[2] = (WP_U8) s_mac2;
   mpls_enet_header.source_addr[3] = (WP_U8) s_mac3;
   mpls_enet_header.source_addr[4] = (WP_U8) s_mac4;
   mpls_enet_header.source_addr[5] = (WP_U8) s_mac5;
   mpls_enet_header.type = (WP_U16) type;

   return;
}
#endif /* WTI_CESOP_MPLS_OVER_ENET */

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_MplsFlowAggPrefixLength(char *StrPrm)
{
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("MplsFlowAggPrefixLength - Invalid number of parameters", __LINE__);
      return;
   }

   /* the demo allows up to 26 bytes of prefix header (Enet+2 vlans + PSN tunnel MPLS label) */
   if (val > 26)
   {
      WTI_TerminatePrintError("MplsFlowAggPrefixLength - Invalid number of parameters", __LINE__);
      return;
   }

   mpls_layer2_agg_cfg.prefix_length = val;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_MplsFlowAggPrefixHeaderMpls(char *StrPrm)
{
   WP_U32 res, cmd;
   WP_U8 prefix_header_mpls[256];

   res = sscanf(StrPrm, "%d %x %x %x %x %x %x",
                &cmd,
                (WP_U32 *)&prefix_header_mpls[0],
                (WP_U32 *)&prefix_header_mpls[4],
                (WP_U32 *)&prefix_header_mpls[8],
                (WP_U32 *)&prefix_header_mpls[12],
                (WP_U32 *)&prefix_header_mpls[16],
                (WP_U32 *)&prefix_header_mpls[20]);

   if (res != 7 && res != 3 && res != 2)
   {
      WTI_TerminatePrintError("MplsFlowAggPrefixHeaderMpls - Invalid number of parameters", __LINE__);
      return;
   }

   /* configure the TDM->PSN L2 ROUTER MPLS flow aggregation */

#if WTI_CESOP_MPLS_OVER_ENET
   /* init the rest of the fields in mpls_layer2_agg_cfg */
#if WTI_2_ENET_DEVICES
   if(first_enet_mpls_fa>=WTI_MAX_NUM_OF_MPLS_FA/2)
   {
      mpls_layer2_agg_cfg.txfunc = the_system->tx_enet_channel1;
   }
   else
#endif /* WTI_2_ENET_DEVICES */
   {
      mpls_layer2_agg_cfg.txfunc =the_system->tx_enet_channel;
   }

#if WTI_4_ENET_DEVICES
   switch (first_enet_mpls_fa)
   {
      case 0:
         mpls_layer2_agg_cfg.txfunc = the_system->tx_enet_channel;
         break;
      case 1:
         mpls_layer2_agg_cfg.txfunc = the_system->tx_enet_channel1;
         break;
      case 2:
         mpls_layer2_agg_cfg.txfunc = the_system->tx_enet_channel2;
         break;
      case 3:
         mpls_layer2_agg_cfg.txfunc = the_system->tx_enet_channel3;
         break;
      default:
         break;
   }
#endif

   first_enet_mpls_fa++;
#else /* ! WTI_CESOP_MPLS_OVER_ENET */
   /* init the rest of the fields in mpls_layer2_agg_cfg */
   mpls_layer2_agg_cfg.txfunc = the_system->hspi_device;
#endif /* WTI_CESOP_MPLS_OVER_ENET */

   mpls_layer2_agg_cfg.l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;
   memcpy(&mpls_layer2_agg_cfg.prefix_header_mpls[0],
          prefix_header_mpls,
          mpls_layer2_agg_cfg.prefix_length);

   mpls_layer2_agg_cfg.label_mode = WP_MPLS_LABEL_PUSH;
   mpls_layer2_agg_cfg.num_of_mpls_headers_to_push = 1;
   memcpy(&mpls_layer2_agg_cfg.mpls_push_headers[0],
          &prefix_header_mpls[mpls_layer2_agg_cfg.prefix_length],
          WTI_MPLS_LABEL_SIZE);

   mpls_layer2_agg_cfg.num_of_mpls_headers_to_pop = 0;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_MplsFlowAggRemarkingOffsets(char *StrPrm)
{
   WP_U32 res, cmd, n, ofs[3];
   WP_U8  i;

   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &n, &ofs[0], &ofs[1], &ofs[2]);

   if (res < 2)
   {
      WTI_TerminatePrintError("CLI_F_MplsFlowAggRemarkingOffsets - Invalid number of parameters", __LINE__);
      return;
   }

   if (n>3)
   {
      WTI_TerminatePrintError("CLI_F_MplsFlowAggRemarkingOffsets - Maximum 3 remaking fields!", __LINE__);
      return;
   }

   if (res != 2+n)
   {
      WTI_TerminatePrintError("CLI_F_MplsFlowAggRemarkingOffsets - Invalid number of parameters", __LINE__);
      return;
   }

   mpls_layer2_agg_cfg.n_prefix_remark_fields = n;
   if (n==0)
      return;

   for (i=0;i<n;i++)
   {
      if (ofs[i] >= 44)  /* check that offset is < 44 (nibbles) */
      {
         WTI_TerminatePrintError("MplsFlowAggRemarking - Invalid offset (must be < 44)", __LINE__);
         return;
      }
      mpls_layer2_agg_cfg.prefix_remark_offset[i] = ofs[i];
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Tdm2PsnFlowAggRemarkingValues(char *StrPrm)
{
   WP_U32 res, cmd, n, vals[3];
   WP_U8  i;

   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &n, &vals[0], &vals[1], &vals[2]);

   if (res < 2)
   {
      WTI_TerminatePrintError("CLI_F_MplsFlowAggRemarkingValues - Invalid number of parameters", __LINE__);
      return;
   }

   if (n>3)
   {
      WTI_TerminatePrintError("CLI_F_MplsFlowAggRemarkingValues - Maximum 3 remaking fields!", __LINE__);
      return;
   }

   if (res != 2+n)
   {
      WTI_TerminatePrintError("CLI_F_MplsFlowAggRemarkingValues - Invalid number of parameters", __LINE__);
      return;
   }

   pw_config.tdm2psn_flow_agg_config.n_prefix_remark_fields = n;

   if (n==0)
      return;

   for (i=0;i<n;i++)
   {
      if (vals[i] > 7)  /* check that the priority value is between 0-7 */
      {
         WTI_TerminatePrintError("CLI_F_MplsFlowAggRemarkingValues - Invalid offset (must be < 44)", __LINE__);
         return;
      }

      pw_config.tdm2psn_flow_agg_config.priority_values[i] = vals[i];
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_MplsFlowAggCreate(char *StrPrm)
{
   WP_U32 res, cmd, val;
   WP_status status;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (val>=WTI_MAX_NUM_OF_MPLS_FA)
   {
      WTI_TerminatePrintError("ERROR : MPLS FA index too big.", __LINE__);
      return;
   }

   /* set a unique tag for each TDM->PSN L2 ROUTER MPLS flow aggregation */
   mpls_layer2_agg_cfg.tag = (WTI_TDM2PSN_L2_MPLS_ROUTER_TAG | val);

   /* create the TDM->PSN l2 router MPLS flow aggregation */
   the_system->tdm2psn_mpls_l2_fa_handle[val] =
      WP_IwFlowAggregationCreate(the_system->wpid,
                                 WP_IW_AGG_MPLS_L2_ROUTER,
                                 &mpls_layer2_agg_cfg);
   WTI_TerminateOnError(the_system->tdm2psn_mpls_l2_fa_handle[val],
                        " WP_IwFlowAggregationCreate()- TDM2PSN layer2 router MPLS",
                        __LINE__);

#if WTI_PCE_CLASSIFIER
   /* In case PCE is used, we don't create the corresponding PSN->TDM l2 router MPLS flow aggregation
      (there is no second round classification), only a single classification is done by the PCE on the second (PW)
      MPLS header and the traffic is mapped directly to the cesop flow aggregation */
#else
   /* create the corresponding PSN->TDM l2 router MPLS flow aggregation */

   /* set a unique tag for each PSN->TDM L2 ROUTER MPLS flow aggregation */
   mpls_layer2_agg_cfg.tag = (WTI_PSN2TDM_L2_MPLS_ROUTER_TAG | val);
   mpls_layer2_agg_cfg.l2_header_insert_mode = WP_IW_L2H_INSERT_DISABLE;
   mpls_layer2_agg_cfg.num_of_mpls_headers_to_push = 0;
   mpls_layer2_agg_cfg.num_of_mpls_headers_to_pop = 1;
   mpls_layer2_agg_cfg.n_prefix_remark_fields = 0;

   /* after poping the PSN tunnel MPLS header, the packets are sent back to the
      IW system for second classification on the PW MPLS header - Note that performance wise, it is
      better to configure DFC (if in bridge mode to classify according to both headers and avoid second
      round classification */
   mpls_layer2_agg_cfg.txfunc = the_system->psn2tdm_iw_system;
   mpls_layer2_agg_cfg.label_mode = WP_MPLS_LABEL_POP;

   the_system->psn2tdm_mpls_l2_fa_handle[val] =
      WP_IwFlowAggregationCreate(the_system->wpid,
                                 WP_IW_AGG_MPLS_L2_ROUTER,
                                 &mpls_layer2_agg_cfg);
   WTI_TerminateOnError(the_system->psn2tdm_mpls_l2_fa_handle[val],
                        " WP_IwFlowAggregationCreate()- PSN2TDM layer2 router MPLS",
                        __LINE__);

   /* add rule to the MPLS lookup that maps all cascaded traffic to the relevant PSN to TDM layer2 router MPLS
      flow aggregation */
   mpls_flow_cfg.aggregation = the_system->psn2tdm_mpls_l2_fa_handle[val];
   mpls_flow_cfg.wred_entry = 0;
   memcpy(&mpls_flow_cfg.mpls_label,
          &mpls_layer2_agg_cfg.mpls_push_headers[0],
          WTI_MPLS_LABEL_SIZE);
   mpls_flow_cfg.mpls_label >>= 12;

   mpls_flow_cfg.deny_mode = 0;
   the_system->psn2tdm_mpls_flow_handle[val] =
      WP_IwFlowAdd(the_system->psn2tdm_iw_system,
                   WP_FLOW_MPLS,
                   &mpls_flow_cfg);
   WTI_TerminateOnError(the_system->psn2tdm_mpls_flow_handle[val],
                        "WP_IwFlowAdd() PSN2TDM cascaded MPLS flow",
                        __LINE__);
#endif
   /* build IW system in the PSN-->TDM direction */
   status = WP_IwSystemBuild(the_system->psn2tdm_iw_system);
   WTI_TerminateOnError(status, "WP_IwSystemBuild PSN-->TDM",__LINE__);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_FlowAggShowInfoMpls(char *StrPrm)
{
   WP_U32 res, cmd, val, val1;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &val, &val1);
   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (val >= WTI_MAX_NUM_OF_MPLS_FA)
   {
      WTI_TerminatePrintError("CLI_F_FlowAggShowInfoMpls - invalid FA number", __LINE__);
      return;
   }

   if (val1 > 1)
   {
      WTI_TerminatePrintError("CLI_F_FlowAggShowInfoMpls - invalid direction", __LINE__);
      return;
   }

   if (val1)
      WP_Display(0, the_system->psn2tdm_mpls_flow_handle[val], WP_DISPLAY_DEFAULT, NULL);
   else
      WP_Display(0, the_system->tdm2psn_mpls_l2_fa_handle[val], WP_DISPLAY_DEFAULT, NULL);

   return;
}

#endif  /* WTI_CESOP_MPLS_IW */

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_IwGlobalStatistics(char *StrPrm)
{

   WP_iw_global_stats iw_stats    = {0};
   WP_status status;
   WP_U32 t1=0, t2=0, screen_coord, hor_coord, temp1, temp2;
   WP_U32 res, cmd, val;

#if WTI_CESOP_TDI
   WP_U32 val1;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &val, &val1);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != 0 && val != 1 && val != 2)
   {
      WTI_TerminatePrintError("CLI_F_IwGlobalStatistics", __LINE__);
      return;
   }
   if (val1 >= WTI_ACTIVE_TDI_PORTS)
   {
      WTI_TerminatePrintError("CLI_F_IwGlobalStatistics", __LINE__);
      return;
   }
#else
   res = sscanf(StrPrm, "%d %d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val != 0 && val != 1 && val != 2 && val != 3 && val != 4)
   {
      WTI_TerminatePrintError("CLI_F_IwGlobalStatistics", __LINE__);
      return;
   }
#endif

   Vt100GotoXy(1,1);
   Vt100ClearScreen();

   while (1)
   {
      t1 = WP_TimeRead();

      if (WTI_SampleSerial())
      {
         break;
      }

      while (1)
      {
         t2 = WP_TimeRead();

         if(WP_TimeDelta(t2,t1) <= FACTOR)
         {
            switch (val)
            {
               case 0:
               {
#if WTI_CESOP_TDI
                  if((status =  WP_IwStatistics(the_system->tdm_port[val1], &iw_stats)))
                  {
                     printf("GetIWStatistics error\n");
                     return;
                  }
#else
                  if((status =  WP_IwStatistics(the_system->trans_port, &iw_stats)))
                  {
                     printf("GetIWStatistics error\n");
                     return;
                  }
#endif
                  break;
               }
#if (WTI_CESOP_MPLS_IW && (!WTI_CESOP_MPLS_OVER_ENET))
               case 1:
               {
                  if((status =  WP_IwStatistics(the_system->hspi_port, &iw_stats)))
                  {
                     printf("GetIWStatistics error\n");
                     return;
                  }

                  break;
               }
#else
               case 1:
               {

#if (WTI_2_ENET_DEVICES)
                  if((status =  WP_IwStatistics(the_system->enet_port1, &iw_stats)))
                  {
                     printf("GetIWStatistics error\n");
                     return;
                  }
#else
                  if((status =  WP_IwStatistics(the_system->enet_port, &iw_stats)))
                  {
                     printf("GetIWStatistics error\n");
                     return;
                  }
#endif
                  break;
               }
               case 2:
               {
                  if((status =  WP_IwStatistics(the_system->enet_port, &iw_stats)))
                  {
                     printf("GetIWStatistics error\n");
                     return;
                  }
                  break;
               }
#if (WTI_4_ENET_DEVICES)
               case 3:
               {
                  if((status =  WP_IwStatistics(the_system->enet_port2, &iw_stats)))
                  {
                     printf("GetIWStatistics error\n");
                     return;
                  }
               }
               case 4:
               {
                  if((status =  WP_IwStatistics(the_system->enet_port3, &iw_stats)))
                  {
                     printf("GetIWStatistics error\n");
                     return;
                  }
               }               
#endif
#endif
               default:
                  break;
            }

            Vt100GotoXy(1,1);
            if (!val)
            {
               printf("TDM TO PSN INTER-WORKING GLOBAL STATISTICS:\n");
            }
            else
            {
               printf("PSN TO TDM INTER-WORKING GLOBAL STATISTICS:\n");
            }
            screen_coord = 3;
            hor_coord = 0;

            Vt100GotoXy(1,screen_coord);

            temp1 = iw_stats.denied_packets;
            temp2 = iw_stats.denied_packets >> 32;
            Vt100GotoXy(hor_coord,screen_coord);
            printf("denied_packets:         %.8x%.8x\n", temp2,temp1);

            temp1 = iw_stats.iwbg_fbp_underrun;
            temp2 = iw_stats.iwbg_fbp_underrun >> 32;
            Vt100GotoXy(hor_coord,screen_coord+1);
            printf("iwbg_fbp_underrun:      %.8x%.8x\n", temp2,temp1);

#ifdef AUTOMATION_MODE
            return;
#else
            break;
#endif
         }
      }
   }
}
#if  (defined  __WT_UFE412__|| defined  __WT_UFE448__)
#if WT_UFE_FRAMER
/***************************************************************
 * Func name  : CLI_F_UFEDeepLoopbackEnableDisable
 * Description:
 *
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_UFEDeepLoopbackEnableDisable(char *StrPrm)
{


	WP_U32 res, cmd,client_port_id , enable_loopback;

	res = sscanf(StrPrm, "%d %d %d", &cmd, &client_port_id , &enable_loopback);

	if(res != 3)
	{
		WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
		return;
	}


	if(enable_loopback)
        {
           WTI_FlexmuxEnableLoopback(0, -1,client_port_id , WT_FLEXMUX_DEEP_LINE_LOOPBACK);
           printf("Deep line loopback Enabled\n");
        }
	else
        {
           WTI_FlexmuxDisableLoopback(0, -1,client_port_id , WT_FLEXMUX_DEEP_LINE_LOOPBACK);
           printf("Deep line loopback disabled\n");
        }
}

/***************************************************************
 * Func name  : CLI_F_UFEShallowLoopbackEnableDisable
 * Description: This function enables or disables the shallow line loopback (loopback on the optic - nothing will get to the framer)
 * 				Use this function to emulate physical line disconnection.
 * Input      : Port ID to set the loopback on.
 * OutPut     :
 * Return Val :
 ***************************************************************/

void CLI_F_UFEShallowLoopbackEnableDisable(char *StrPrm)
{


	WP_U32 res, cmd, line_port_id, enable_loopback;

	res = sscanf(StrPrm, "%d %d %d", &cmd, &line_port_id, &enable_loopback);

	if(res != 3)
	{
		WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
		return;
	}


	if(enable_loopback)
        {
           WTI_FlexmuxEnableLoopback(0,line_port_id, -1, WT_FLEXMUX_SHALLOW_LINE_LOOPBACK);
           printf("Shallow line loopback Enabled\n");
        }

	else
        {
           WTI_FlexmuxDisableLoopback(0,line_port_id ,-1 , WT_FLEXMUX_SHALLOW_LINE_LOOPBACK);
           printf("Shallow line loopback Disabled\n");

        }


}

/* This function currently designated to work on first line.*/
void CLI_F_UFEPDHTxTimingSet(char *StrPrm)
{
   WP_U32 res, cmd,client_port_id ,line_timing;

	res = sscanf(StrPrm, "%d %d %d", &cmd, &client_port_id, &line_timing);

	if(res != 3)
	{
		WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
		return;
	}

	if(line_timing > 3)
        {
           WTI_TerminatePrintError("Invalid parameter line_timing (0-3)", __LINE__);
           return;
        }



// WP_S32 client_port, line_port;


// if (ufe->fpga_mode == WT_SONET)
//    {
//       WT_ufe_line_sonet_params *line_params = (WT_ufe_line_sonet_params *) config;

//   client_port = WTI_FlexmuxClientPortIndexGet(index, line_params->transfer_type);
//          line_port = WTI_FlexmuxLinePortIndexGet(index, ufe->ufe_id);
//          if (line_port == -1)
//          {
//             printf("Illegal line port index %d\n",index);
//             exit(1);
//          }


//#if WTI_DEBUG_LEVEL > 0
   if(line_timing == WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH)
   {
      printf("client_port_id %d set to WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH",client_port_id);
   }
   else if(line_timing == WPX_UFE_FRAMER_TRANSMIT_TIMING_INPUT_REFERENCE_CLOCK)
   {
      printf("client_port_id %d set to WPX_UFE_FRAMER_TRANSMIT_TIMING_INPUT_REFERENCE_CLOCK",client_port_id);
   }
   else  if(line_timing == WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING)
   {
      printf("client_port_id %d set to WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING",client_port_id);
   }
   else if(line_timing == WPX_UFE_FRAMER_TRANSMIT_TIMING_WINPATH)
   {
      printf("client_port_id %d set to WPX_UFE_FRAMER_TRANSMIT_TIMING_WINPATH",client_port_id);
   }
//#endif


      WTI_FlexmuxPDHTxTimingSet(0,client_port_id,line_timing); // todo fix the input params
}



#endif /* WT_UFE_FRAMER */
#endif //(defined  __WT_UFE412__|| defined  __WT_UFE448__)

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_PwRegressionResultsCheck(char *StrPrm)
{
   WP_stats_emphy_trans_pwe3_rx rx_stats = {0};
   WP_stats_emphy_trans_pwe3_tx tx_stats = {0};
   WP_iw_global_stats iw_stats = {0};
   WP_stats_iw_cesop_tdm2psn tdm2psncesop_stats = {0};
   WP_stats_iw_cesop_psn2tdm psn2tdmcesop_stats = {0};
   WP_status status;
   WP_U32 sent_packets, ratio, upper_limit;
   WP_U32 expected_tx_data_units, tx_data_units_per_packet, rx_data_units_per_packet;
   WP_boolean iw_global_enet=WP_FALSE, psn2tdm_flow_agg=WP_FALSE, trans_tx_channel=WP_FALSE;
   WP_boolean tdm2psn_flow_agg=WP_FALSE, trans_rx_channel=WP_FALSE;

   rx_data_units_per_packet = (psn2tdm_flow_agg_config.payload_size/pw_config.rx_data_unit_size);
   tx_data_units_per_packet = (psn2tdm_flow_agg_config.payload_size/pw_config.tx_data_unit_size);
   sent_packets = cesop_tx_binding_config.window_switchover_threshold+1;

   /* PSN to TDM IW global statistics*/
#if (WTI_CESOP_MPLS_IW && (!WTI_CESOP_MPLS_OVER_ENET))
   if((status =  WP_IwStatistics(the_system->hspi_port, &iw_stats)))
#else
      if((status =  WP_IwStatistics(the_system->enet_port, &iw_stats)))
#endif
      {
         printf("Statistics error - psn to tdm flow aggregation statistics - PW %d\n",total_number_of_pw);
         return;
      }
      else
      {
         if (iw_stats.denied_packets > 0 || iw_stats.iwbg_fbp_underrun > 0)
         {
            printf("Failure in IW global statistics check - PW %d\n",total_number_of_pw);
            printf("sent_packets %x\n",sent_packets);
            printf("total_number_of_pw %x\n",total_number_of_pw);
         }

      }

   /* PSN to TDM flow agg statistics*/
   if ((status = WP_IwFlowStatistics(the_system->pw[total_number_of_pw-1].psn2tdm_flow_agg,
                                     WP_IW_FLOW_STAT_CESOP_PSN2TDM,
                                     &psn2tdmcesop_stats)))
   {
      printf("Statistics error - psn to tdm flow aggregation statistics - PW %d\n",total_number_of_pw-1);
      return;
   }
   else
   {
      if (psn2tdmcesop_stats.forwarded_packets == sent_packets)
      {
         psn2tdm_flow_agg=WP_TRUE;
      }
      else
      {
         printf("Failure in PSN to TDM statistics check - PW %d\n",total_number_of_pw-1);
         printf("psn2tdmcesop_stats.forwarded_packets %x\n",
                (WP_U32)psn2tdmcesop_stats.forwarded_packets);
         printf("sent_packets %d\n", sent_packets);
      }
   }

   /* PSN to TDM transparent PWE3 Tx channel statistics*/
   if ((status = WP_ChannelStatistics(the_system->pw[total_number_of_pw-1].trans_tx, &tx_stats)))
   {
      printf("Statistics error - transparent PWE3 Tx channel statistics - PW %d\n",total_number_of_pw-1);
      return;
   }
   else
   {
      expected_tx_data_units = ((tx_stats.underrun_dataunit+sent_packets+tx_stats.buffer_underrun)
                                * tx_data_units_per_packet);

      if (expected_tx_data_units >
          ((tx_stats.valid_packets * tx_data_units_per_packet) - tx_data_units_per_packet)
          && ( expected_tx_data_units <
               (tx_stats.valid_packets * tx_data_units_per_packet) + tx_data_units_per_packet))
      {
         trans_tx_channel=WP_TRUE;
      }
      else
      {
         printf("Failure in transparent PWE3 Tx channel statistics check - PW %d\n",total_number_of_pw-1);
         printf("expected_tx_data_units %x\n",expected_tx_data_units);
         printf("tx_stats.underrun_dataunit %x\n",(WP_U32)tx_stats.underrun_dataunit);
         printf("tx_stats.buffer_underrun %x\n",(WP_U32)tx_stats.buffer_underrun);
         printf("tx_data_units_per_packet %d\n",tx_data_units_per_packet);
         printf("tx_stats.valid_packets %x\n",(WP_U32)tx_stats.valid_packets);
         printf("sent_packets %d\n",sent_packets);
      }
   }

   /* TDM to PSN transparent PWE3 Rx channel statistics*/
   if ((status = WP_ChannelStatistics(the_system->pw[total_number_of_pw-1].trans_rx, &rx_stats)))
   {
      printf("Statistics error - transparent PWE3 Rx channel statistics - PW %d\n",total_number_of_pw-1);
      return;
   }
   else
   {
      ratio = (pw_config.rx_data_unit_size / pw_config.tx_data_unit_size);
      upper_limit = ((tx_stats.valid_packets * tx_data_units_per_packet) * 101);

      /* allow mistake of 1% because of sampling time differences */
      if (((rx_stats.valid_packets * tx_data_units_per_packet) * ratio * 100) < upper_limit
          && (tx_stats.valid_packets * tx_data_units_per_packet) <=
          (rx_stats.valid_packets * tx_data_units_per_packet) * ratio)
      {
         trans_rx_channel=WP_TRUE;
      }
      else
      {
         printf("Failure in transparent PWE3 Rx channel statistics check - PW %d\n",total_number_of_pw-1);
         printf("tx_stats.valid_packets %x\n",(WP_U32)tx_stats.valid_packets);
         printf("rx_stats.valid_packets %x\n",(WP_U32)rx_stats.valid_packets);
         printf("ratio %d\n", ratio);
         printf("upper_limit %x\n", upper_limit);
      }
   }

   /* TDM to PSN flow agg statistics */
   if ((status =  WP_IwFlowStatistics(the_system->pw[total_number_of_pw-1].tdm2psn_flow_agg,
                                      WP_IW_FLOW_STAT_CESOP_TDM2PSN,
                                      &tdm2psncesop_stats)))

   {
      printf("Statistics error - tdm to psn flow aggregation statistics - PW %d\n",total_number_of_pw-1);
      return;
   }
   else
   {
      /* allow mistake of 1% because of sampling time differences */
      if (101 * (tdm2psncesop_stats.forwarded_packets * rx_data_units_per_packet)
          > (100 * (rx_stats.valid_packets * tx_data_units_per_packet))
          && (100 * tdm2psncesop_stats.forwarded_packets * rx_data_units_per_packet)
          <= (101 * (rx_stats.valid_packets * tx_data_units_per_packet)))
      {
         tdm2psn_flow_agg=WP_TRUE;
      }
      else
      {
         printf("Failure in TDM to PSN statistics check - PW %d\n",total_number_of_pw-1);
         printf("tdm2psncesop_stats.forwarded_packets %x\n",
                (WP_U32)tdm2psncesop_stats.forwarded_packets);
         printf("rx_data_units_per_packet %d\n", rx_data_units_per_packet);
         printf("rx_stats.valid_packets %x\n", (WP_U32)rx_stats.valid_packets);
      }
   }

   /* set PW results iff all statistics are ok */
   if (iw_global_enet == WP_TRUE
       && psn2tdm_flow_agg == WP_TRUE
       && trans_tx_channel == WP_TRUE
       && trans_rx_channel == WP_TRUE
       && tdm2psn_flow_agg == WP_TRUE
      )
   {
      reg_results[total_number_of_pw-1] = WP_TRUE;
   }
   else
   {
      reg_results[total_number_of_pw-1] = WP_FALSE;
   }
}

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Pwe3ChannelQDepth(char *StrPrm)
{
   WP_status status;
   WP_U32 res, cmd, val, qdepth=0;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3ChannelQDepth", __LINE__);
      return;
   }

   status = WP_ChannelQDepth(the_system->pw[val].trans_tx, &qdepth);
   if (status != WP_OK)
      return;

   printf("Channel handle: %x Number of packets in queue: %x\n",
          the_system->pw[val].trans_tx,
          qdepth);
}


/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_ClockRecoveryHoldoverForce(char *StrPrm)
{
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WP_U32 res, cmd, pw_index;
   WP_status status;

   res = sscanf(StrPrm, "%d %d", &cmd, &pw_index);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (pw_index >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_ClockRecoveryHoldoverForce", __LINE__);
      return;
   }

   status = WP_CesopClockRecHoldoverForce(the_system->pw[pw_index].psn2tdm_flow_agg, WP_FALSE);

   if (strcmp(WP_error_name[WP_ERROR(status)], "WP_ERR_CLOCK_REC_FORCE_HOLDOVER_IS_SET") == 0 )

   {
      printf("WP_ERR_CLOCK_REC_FORCE_HOLDOVER_IS_SET\n");
   }
   else
   {
      WTI_TerminateOnError(status , "CLI_F_ClockRecoveryHoldoverForce",__LINE__);
   }

#else
   printf("Clock Recovery feature is disabled\n");
#endif
   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_ClockRecoveryHoldoverClear(char *StrPrm)
{
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WP_U32 res, cmd, pw_index;
   WP_status status;

   res = sscanf(StrPrm, "%d %d", &cmd, &pw_index);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (pw_index >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_ClockRecoveryHoldoverClear", __LINE__);
      return;
   }

   status = WP_CesopClockRecHoldoverForce(the_system->pw[pw_index].psn2tdm_flow_agg, WP_TRUE);
   if (strcmp(WP_error_name[WP_ERROR(status)], "WP_ERR_CLOCK_REC_FORCE_HOLDOVER_IS_CLEARED") == 0)
   {
      printf("WP_ERR_CLOCK_REC_FORCE_HOLDOVER_IS_CLEARED\n");
   }
   else
   {
      WTI_TerminateOnError(status , "CLI_F_ClockRecoveryHoldoverClear",__LINE__);
   }
#else
   printf("Clock recovery feature is disabled\n");
#endif

   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_ClockRecoveryShowState(char *StrPrm)
{
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WP_U32 res, cmd, pw_index;
   WP_clock_rec_state state;
   WP_status status;

   res = sscanf(StrPrm, "%d %d", &cmd, &pw_index);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if ((pw_index >= total_number_of_pw) && (pw_index != 0))
   {
      WTI_TerminatePrintError("pw_index >= total_number_of_pw", __LINE__);
      return;
   }

//   if (!(the_system->clock_rec[the_system->pw[pw_index].line_index].clock_rec_master))
//   {
//      WTI_TerminatePrintError("Not clock_rec_master", __LINE__);
//      return;
//   }

   status = WP_CesopClockRecStateGet(the_system->pw[pw_index].psn2tdm_flow_agg,
                                     &state);
   if(WP_ERROR_P(status))
   {
      printf("%s: Error type - %s, Line:%d\n", appname, WP_error_name[WP_ERROR(status)],__LINE__);
      WTI_TerminatePrintError("CLI_F_ClockRecoveryShowState",__LINE__);
      return;
   }

   if (state == WP_IW_CLOCK_REC_DEFAULT_WAIT_STATE)
   {
      printf("WP_IW_CLOCK_REC_DEFAULT_WAIT_STATE\n");
   }
   else if (state == WP_IW_CLOCK_REC_HOLDOVER_STATE)
   {
      printf("WP_IW_CLOCK_REC_HOLDOVER_STATE\n");
   }
   else if (state == WP_IW_CLOCK_REC_CONVERGENCE_STATE)
   {
      printf("WP_IW_CLOCK_REC_CONVERGENCE_STATE\n");
   }
   else if (state == WP_IW_CLOCK_REC_PMSM_STATE)
   {
      printf("WP_IW_CLOCK_REC_PMSM_STATE\n");
   }
   else
   {
      printf("WP_IW_CLOCK_REC_NORMAL_STATE\n");
   }

#else
   printf("Clock recovery features are disabled\n");
#endif
   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_ClockRecoverySystemsShowState(char *StrPrm)
{
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WP_U32 res, cmd, pw_index ,t1, t2;
   WP_clock_rec_state state;
   WP_status status;

   res = sscanf(StrPrm, "%d", &cmd);

   if (res != 1)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   Vt100GotoXy(1,1);
   Vt100ClearScreen();

   while (1)
   {
      t1 = WP_TimeRead();

      if (WTI_SampleSerial())
      {
         break;
      }
      while (1)
      {
         t2 = WP_TimeRead();
         if( (t2-t1) >= FACTOR)
         {
            Vt100GotoXy(1,1);
            printf("Clock Recovery Systems states: <PW> <State>");
            Vt100GotoXy(1,2);

            for(pw_index = 0; pw_index < total_number_of_pw ;pw_index++)
            {
               if(the_system->pw[pw_index].clock_rec_tx_enable)
               {

                  status = WP_CesopClockRecStateGet(the_system->pw[pw_index].psn2tdm_flow_agg,
                                                    &state);

                  if(status != WP_OK)
                  {

                     if(!(strcmp(WP_error_name[WP_ERROR(status)],"WP_ERR_CLOCK_REC_DISABLED_FOR_PW")))
                     {

                        /* printf("WP_ERR_CLOCK_REC_DISABLED_FOR_PW\n"); */
                     }
                     else
                     {
                        WTI_TerminateOnError(status , "CLI_F_ClockRecoverySystemsShowState",__LINE__);
                     }
                  }
                  else
                  {
                     printf("PW %3d:",pw_index);

                     if (state == WP_IW_CLOCK_REC_DEFAULT_WAIT_STATE)
                     {
                        printf("DEF_WAIT ");
                     }
                     else if (state == WP_IW_CLOCK_REC_HOLDOVER_STATE)
                     {
                        printf("HOLDOVER ");
                     }
                     else if (state == WP_IW_CLOCK_REC_CONVERGENCE_STATE)
                     {
                        printf("CONVERG  ");
                     }
                     else if (state == WP_IW_CLOCK_REC_PMSM_STATE)
                     {
                        printf("PMSM     ");
                     }
                     else
                     {
                        printf("NORMAL   ");
                     }
                     if (pw_index%7 == 6)
                        printf("\n");
                  }
               }
            }
            printf("\n");

#ifdef AUTOMATION_MODE
            return;
#else
            break;
#endif
         }
      }
   }


#else
   printf("Clock recovery features are disabled\n");
#endif
   return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_ClockRecoveryOutputEnable(char *StrPrm)
{
   WP_U32 res, cmd, pw_index;
   WP_status status;

   res = sscanf(StrPrm, "%d %d", &cmd, &pw_index);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (pw_index >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_ClockRecoveryOutputEnable", __LINE__);
      return;
   }

   status = WP_CesopClockRecEnable(the_system->pw[pw_index].psn2tdm_flow_agg);
   WTI_TerminateOnError(status , "WP_CesopClockRecEnable",__LINE__);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_ClockRecoveryOutputDisable(char *StrPrm)
{
   WP_U32 res, cmd, pw_index;
   WP_status status;

   res = sscanf(StrPrm, "%d %d", &cmd, &pw_index);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (pw_index >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_ClockRecoveryOutputDisable", __LINE__);
      return;
   }

   status = WP_CesopClockRecDisable(the_system->pw[pw_index].psn2tdm_flow_agg);
   WTI_TerminateOnError(status , "WP_CesopClockRecDisable",__LINE__);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_MapJatToLine(WP_U8 spe_ja_index[])
{
   WP_U32 res, val1;
   char StrPrm[256];

   printf("The Setup support only JAT #0: Enter the line ID that is mapped to JAT 0 [val1]\n");
#ifdef __linux__
   fgets(StrPrm, INST_NUM, stdin);
#else
   gets(StrPrm);
#endif

   res = sscanf(StrPrm, "%d", &val1);

   if (res != 1)
   {
      WTI_TerminatePrintError("\nInvalid number of parameters", __LINE__);
      return;
   }

   if(val1 < 0 || val1>11)
   {
      WTI_TerminatePrintError("JAT index must be < 12", __LINE__);
      return;
   }

   /* Set line 'val1' to work with JAT #0 */
   spe_ja_index[val1] = 0;
   /* Set line #0 to work with JAT 'val1' - this is done in order to prevent two lines
      from working with the same JAT */
   spe_ja_index[0] = val1;

   return;
}


void CLI_F_ClockRecCommand(char *StrPrm)
{
   WP_U32 res, cmd, pw_index, cr_cmd;
   WP_status status;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &pw_index, &cr_cmd);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (pw_index >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_ClockRecoveryHoldoverForce", __LINE__);
      return;
   }

   status = WP_CesopClockRecCommand(the_system->pw[pw_index].psn2tdm_flow_agg, cr_cmd);
   if (status != WP_OK)
      printf("WP_CesopClockRecCommand failed: %s\n", WP_error_name[WP_ERROR(status)]);

   return;
}

void CLI_F_ClockRecIndirectMemoryReadCgen(char *StrPrm)
{
#ifdef __WT_UFE3__
#if (!WTI_CESOP_TDI)
   WP_U32 res, cmd, entry;

   res = sscanf(StrPrm, "%d %d", &cmd, &entry);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (entry >= WTI_MAX_LINES)
   {
      WTI_TerminatePrintError("CLI_F_ClockRecIndirectMemoryReadCgen", __LINE__);
      return;
   }

   WT_UfeClockRecIndirectCgenRead(entry);
#endif
#endif /* UFE3 */
   return;
}

void CLI_F_ClockRecIndirectMemoryReadRxTsPort(char *StrPrm)
{
#ifdef __WT_UFE3__
#if (!WTI_CESOP_TDI)
   WP_U32 res, cmd, entry, clk_rec_if;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &entry, &clk_rec_if);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (entry >= WTI_MAX_LINES)
   {
      WTI_TerminatePrintError("CLI_F_ClockRecIndirectMemoryReadRxTsPort", __LINE__);
      return;
   }
   if (clk_rec_if >= global_active_cr_async_mode)
   {
      WTI_TerminatePrintError("CLI_F_ClockRecIndirectMemoryReadRxTsPort", __LINE__);
      return;
   }

   WT_UfeClockRecIndirectPortRamRead(clk_rec_if, entry);

#endif
#endif /* UFE3 */
   return;
}

void CLI_F_ClockRecIndirectMemoryReadRxTsMask(char *StrPrm)
{
#if (!WTI_CESOP_TDI)
   printf("CLI_F_ClockRecIndirectMemoryReadRxTsMask: Currently this function is inactive\n");
#endif
   return;
}

void CLI_F_ClockRecIndirectMemoryReadCt(char *StrPrm)
{
#ifdef __WT_UFE3__
#if (!WTI_CESOP_TDI)
   WP_U32 res, cmd, entry;

   res = sscanf(StrPrm, "%d %d", &cmd, &entry);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (entry >= WTI_MAX_LINES)
   {
      WTI_TerminatePrintError("CLI_F_ClockRecIndirectMemoryReadCt", __LINE__);
      return;
   }

   WT_UfeClockRecIndirectCtRead(entry);
#endif
#endif /* UFE3 */
   return;
}

void CLI_F_ClockRecIndirectMemoryRead(char *StrPrm) /* unused function */
{
#ifdef __WT_UFE3__
#if (!WTI_CESOP_TDI)
   WP_U32 res, cmd, func;
   WP_U32 entry, clk_rec_if;

   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &func, &entry, &clk_rec_if);

   if (res == 0 || res > 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (func >= 2)
   {
      WTI_TerminatePrintError("CLI_F_ClockRecIndirectMemoryRead", __LINE__);
      return;
   }
   if (func == 0)
   {
      if (entry >= WTI_MAX_LINES)
      {
         WTI_TerminatePrintError("CLI_F_ClockRecIndirectMemoryRead", __LINE__);
         return;
      }
   }
   else if (func == 1)
   {
      if (entry >= 256)
      {
         WTI_TerminatePrintError("CLI_F_ClockRecIndirectMemoryRead", __LINE__);
         return;
      }
      if (clk_rec_if >= global_active_cr_async_mode)
      {
         WTI_TerminatePrintError("CLI_F_ClockRecIndirectMemoryRead", __LINE__);
         return;
      }
   }


   if (func == 0)
      WT_UfeClockRecIndirectCgenRead(entry);
   else if (func == 1)
      WT_UfeClockRecIndirectPortRamRead(clk_rec_if, entry);

#endif
#endif /* UFE3 */
   return;

}


void CLI_F_ClockRecDcoDivisorCalc(char *StrPrm)
{
   WP_status status;
   WP_U32 res, cmd, clock_mode_freq, diff_ref_freq, ppm_offset;
   WP_U32 int_part, frac_part, limit_part;

   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &clock_mode_freq, &diff_ref_freq, &ppm_offset);
   if (res != 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (clock_mode_freq == 0 || diff_ref_freq == 0 || ppm_offset == 0)
   {
      WTI_TerminatePrintError("One of the inputs is zero", __LINE__);
      return;
   }

   status = WP_ClockRecDcoDivisorCalc(clock_mode_freq,
                                      diff_ref_freq,
                                      ppm_offset,
                                      &int_part,
                                      &frac_part,
                                      &limit_part);

   if ((strcmp(WP_error_name[WP_ERROR(status)],"WP_ERR_CONFIG_FREQ_MODULO_1000")) == 0)
   {
      printf("Error! WP_ClockRecDcoDivisorCalc: Check value of reference clock frequencies.\n");
      return;
   }
   else if (status != WP_OK)
   {
      printf("WARNING! WP_ClockRecDcoDivisorCalc: DCO fractional part is too close to 1 or to 0\n");
   }

   printf("------- service clock = %d, diff_ref clock = %d, ppm_offset = %d\n", clock_mode_freq, diff_ref_freq, ppm_offset);
   printf("------- int_part = 0x%x, frac_part = 0x%x\n", int_part, frac_part);
   printf("------- limit_part = 0x%x\n", limit_part);

}

#ifdef WP_HW_WINPATH3
void CLI_F_ClockRecDcoHwRegistersDisplay(char *StrPrm)
{
   WP_status status;
   WP_U32 reg0=0, reg1=0, reg2=0, fifo_reg=0;
   WP_U32 res, cmd, dco_id;

   res = sscanf(StrPrm, "%d %d", &cmd, &dco_id);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (dco_id >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_ClockRecDcoHwRegistersDisplay", __LINE__);
      return;
   }

   status = WTI_ClockRecTdiDcoRegRead(dco_id, &reg0, &reg1, &reg2, &fifo_reg);
   WTI_TerminateOnError(status , "CLI_F_ClockRecDcoHwRegistersDisplay",__LINE__);

   printf("DCO %d HW Registers:\n", dco_id);
   printf(" Control Register    0x%x\n", reg0);
   printf(" Fractional Register 0x%x\n", reg1);
   printf(" Divisor Register    0x%x\n", reg2);
   printf(" FIFO State Register 0x%x\n", fifo_reg);

   return;
}
#endif /* WP_HW_WINPATH3 */

/*********************************************************************************
 * The function CLI_F_ClockRecoveryPhyReconfigure was created to verify the PHY
 * reconfigure sequence. This is done by going from PHY with CR enable to CR
 * disable and back to CR enable.
 *
 * Note:
 *  The complite sequence includes also ChannelDisable prior to PhyDisbale
 *  Please refer to the CR user guide for complete sequence.
 *
 *
 * Usage:
 *  1. Run regular CR test (i.e. 101_103_E1_U)
 *  2. From the CR menu, call CLI_F_ClockRecoveryPhyReconfigure("0 i 0") (disable)
 *  3. From the CR menu, call CLI_F_ClockRecoveryPhyReconfigure("0 i 1") (enable)
 *  4. Check CR statistics and CR displays.
 ********************************************************************************/
void CLI_F_ClockRecoveryPhyReconfigure(char *StrPrm)
{
#if ((!WTI_CESOP_TDI) && __WT_UFE3__)
   WP_status status;
   WP_U32 res, cmd, pw_index, enable;
   WP_CHAR temp_string[200];
   WUFE_line_sbi_reconfig ufe_line_config;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &pw_index, &enable);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (pw_index >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_ClockRecoveryPhyReconfigure", __LINE__);
      return;
   }
   if (enable != 0 && enable != 1)
   {
      WTI_TerminatePrintError("CLI_F_ClockRecoveryPhyReconfigure", __LINE__);
      return;
   }

   /* (1) CR to holdover set */
   printf("## 1 ##  CLI_F_ClockRecoveryHoldoverForce\n");
   sprintf(temp_string, "0 %d", pw_index);
   CLI_F_ClockRecoveryHoldoverForce(temp_string);

   /* (2) CR disable */
   printf("## 2 ##  CLI_F_ClockRecoveryOutputDisable\n");
   sprintf(temp_string, "0 %d", pw_index);
   CLI_F_ClockRecoveryOutputDisable(temp_string);

   /* (3) PHY disable */
   printf("## 3 ##  WT_UfePhyDisable\n");
   WT_UfePhyDisable(&the_system->ufe, pw_index);

   /* (4) line disable */
   printf("## 4 ##  WUFE_LineDisable\n");
   status = WUFE_LineDisable((&the_system->ufe)->line_handle[pw_index], WUFE_FULL_DUPLEX);
   WTI_TerminateOnError(status, "WUFE_LineDisable", __LINE__);

#if 1
   /* (5) reconfigure line */
   printf("## 5 ##  WT_UfeLineSbiModify\n");
   memset(&ufe_line_config, 0xffff, sizeof(WUFE_line_sbi_reconfig));
   if (enable)
   {
      ufe_line_config.tx_clk_master = WUFE_LINE_CLOCK_MASTER;
      ufe_line_config.clock_translator_id = 0; /* from '0' to '0' */
      ufe_line_config.clock_rec_mode = WUFE_LINE_CLOCK_REC_ENABLE;
      ufe_line_config.rx_clk_rec_if = 0; /* from '0' to '0' */
   }
   else
   {
      ufe_line_config.tx_clk_master = WUFE_LINE_CLOCK_SLAVE;
      /* ufe_line_config.clock_translator_id = 0xff; */
      ufe_line_config.clock_rec_mode = WUFE_LINE_CLOCK_REC_DISABLE;
      /* ufe_line_config.rx_clk_rec_if = 0xff; */
   }

   WT_UfeLineSbiModify(&(the_system->ufe), pw_index, &ufe_line_config);
#endif /* 0 */

   /* (6) reconfigure PHY */
   printf("## 6 ##  WT_UfePhyRxClkRecReconfigure\n");
   /* set rx_clk_rec_mode when enable. clear rx_clk_rec_mode when disable */
   WT_UfePhyRxClkRecReconfigure(&the_system->ufe, pw_index, enable);

   /* (7) CR master set */
   printf("## 7 ##  WT_UfePhyEnable\n");
   WT_UfeClockRecMasterSet((&the_system->ufe)->phy_handle[pw_index],
                           pw_config.tx_data_unit_size);

   /* (8) line enable */
   printf("## 8 ##  WT_UfeLineEnable\n");
   WT_UfeLineEnable(&(the_system->ufe), pw_index);

   /* (9) PHY enable */
   printf("## 9 ##  WT_UfePhyEnable\n");
   WT_UfePhyEnable(&the_system->ufe, pw_index);

   /* (10) CR enable */
   if (enable)
   {
      printf("## 10 ##  CLI_F_ClockRecoveryOutputEnable\n");
      sprintf(temp_string, "0 %d", pw_index);
      CLI_F_ClockRecoveryOutputEnable(temp_string);

      /* (11) CR holdover clear */
      printf("## 11 ##  CLI_F_ClockRecoveryHoldoverClear\n");
      sprintf(temp_string, "0 %d", pw_index);
      CLI_F_ClockRecoveryHoldoverClear(temp_string);

      /* (12) return CGEN to line #0 */
      printf("## 12 ## WT_UfeClkRecRxCgenModify\n");
      WT_UfeClkRecRxCgenModify(&(the_system->ufe), pw_index);

   }
   else
   {
      printf("## xx ##  No CR to enable and free from HO + CGEN modify (10, 11, 12)\n");
   }
#endif /*!WTI_CESOP_TDI & __WT_UFE3__ */
}

#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_RegressionResultsCheck(char *StrPrm)
{
   WP_U32 i;

   for (i=0; reg_results[i] == WP_TRUE && i < WTI_MAX_PW; ++i);

   if (i == total_number_of_pw)
   {
      printf("Test Passed\n");
      CLI_F_Quit("");
   }
   else
   {
      printf("Test Failed\n");
      CLI_F_Quit("");
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Quit_No_Reset(char *StrPrm)
{
   /* Release WDDI */
   //WTI_SystemRelease();
   //WP_DriverRelease();
   exit(1);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Quit(char *StrPrm)
{
#ifdef __linux__
	printf("System release\n");
	WTI_SystemRelease();
#endif
   WP_DriverRelease();

#ifdef __linux__
   exit(1);
#else
#endif
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_StatShowMem(char *StrPrm)
{
   WTI_ShowMemStat();
}

#if WTI_COLLECT_TRACKING_DATA
void CLI_F_StatTimingData(char *StrPrm)
{
   WTI_DisplayRecords();
}

void CLI_F_ResetTimingData(char *StrPrm)
{
   WTI_InitializeDisplayRecords();
}
#endif

void CLI_F_GetAlarmState(char *StrPrm)
{
#if WTI_FLEXMUX_ENABLE_ALARMS

   void cb_port_alarm_report_only(WP_U8 device, WP_U8 line_port_id, WP_U8 alarm_category, WP_U8 is_asserted);
   void cb_path_alarm_report_only(WP_U8 device_id,
                            WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *p_SDH_Sonet_line_endpoint_type,
                            WP_U8 alarm_category, WP_U8 is_asserted);
   void cb_pdh_alarm_report_only(WP_U8 device_id, WP_U32 iSocketClientPDH, U8 alarm_category, U8 is_asserted);
   WP_U32 j;

   WPX_FRMR_RegisterSonetSdhPortAlarmCallback(&cb_port_alarm_report_only);
   WPX_FRMR_RegisterSonetSdhPathAlarmCallback(&cb_path_alarm_report_only);
   WPX_FRMR_RegisterSocketClientPdhAlarmCallback(&cb_pdh_alarm_report_only);

   for (j = 0; j < WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; ++j)
      WPX_FRMR_ReportDeviceAlarmMap(j);

#else
   printf("ALARMS NOT ENABLED\n");
#endif
}

/*****************************************************************************
 * Function name:
 * Description  :
 * Input  params:
 * Output params:
 * Return val   :
 *****************************************************************************/
static void WTI_InitDemoStructures(void)
{
   int i;

   reg_results = (WP_boolean *)malloc(WTI_MAX_PW *sizeof(WP_boolean));
   if (!reg_results)
   {
      printf("Test Abort. Can't allocate memory for reg_results\n");
      printf("Test Failed.\n");
      WTI_SystemRelease();
      WP_DriverRelease();
   }

   /* slots configuration */
   for (i=0; i<WTI_MAX_SLOTS; i++)
      slots_config[i] = WTI_UNUSED_SLOT;

#if WTI_CESOP_MPLS_OVER_ENET
   memset(&mpls_enet_header ,0,sizeof(WTI_enet_header));

   mpls_enet_header.dest_addr[0] = 0xaa;
   mpls_enet_header.dest_addr[1] = 0xaa;
   mpls_enet_header.dest_addr[2] = 0xaa;
   mpls_enet_header.dest_addr[3] = 0xaa;
   mpls_enet_header.dest_addr[4] = 0xaa;
   mpls_enet_header.dest_addr[5] = 0xaa;
   mpls_enet_header.source_addr[0] = 0xbb;
   mpls_enet_header.source_addr[1] = 0xbb;
   mpls_enet_header.source_addr[2] = 0xbb;
   mpls_enet_header.source_addr[3] = 0xbb;
   mpls_enet_header.source_addr[4] = 0xbb;
   mpls_enet_header.source_addr[5] = 0xbb;
   mpls_enet_header.type = (WP_U16) 0x8847;
#endif /* WTI_CESOP_MPLS_OVER_ENET */
}

#if (!WTI_CESOP_TDI)
/*****************************************************************************
 * Function name:
 * Description  :
 * Input  params:
 * Output params:
 * Return val   :
 *****************************************************************************/
static int WTI_SpeIdGet(WP_U32 e1t1_index)
{
   if (the_system->ufe.fpga_mode == WT_OCTAL)
   {
      WTI_TerminatePrintError("WTI_SpeIdGet", __LINE__);
      return -1;
   }

   if (e1t1_index >= WTI_MAX_LINES)
   {
      WTI_TerminatePrintError("WTI_SpeIdGet", __LINE__);
      return -1;
   }

   return (e1t1_index / WTI_T1_LINES_PER_SPE);
}

static int WTI_SpeLinkIdGet(WP_U32 e1t1_index)
{
   if (the_system->ufe.fpga_mode == WT_OCTAL)
   {
      WTI_TerminatePrintError("WTI_SpeLinkIdGet", __LINE__);
      return -1;
   }

   if (e1t1_index >= WTI_MAX_LINES)
   {
      WTI_TerminatePrintError("WTI_SpeLinkIdGet", __LINE__);
      return -1;
   }

   return (e1t1_index % WTI_T1_LINES_PER_SPE);
}

int WTI_LineIndexGet(WP_U32 spe, WP_U32 sub_spe)
{
   if (the_system->ufe.fpga_mode == WT_OCTAL)
   {
      WTI_TerminatePrintError("WTI_LineIdGet", __LINE__);
      return -1;
   }

   if((the_system->ufe.sbi_spe_line_type[spe] == WUFE_SBI_TYPE_DS3 ||
       the_system->ufe.sbi_spe_line_type[spe] == WUFE_SBI_TYPE_DS3) &&
      sub_spe > 0)
   {
      WTI_TerminatePrintError("WTI_LineIdGet", __LINE__);
      return -1;

   }

   if((the_system->ufe.sbi_spe_line_type[spe] == WUFE_SBI_TYPE_E1 && sub_spe > 20) ||
      (the_system->ufe.sbi_spe_line_type[spe] == WUFE_SBI_TYPE_T1 && sub_spe > 27) ||
      (the_system->ufe.sbi_spe_line_type[spe] == WUFE_SBI_TYPE_DS3 && sub_spe > 0) ||
      (the_system->ufe.sbi_spe_line_type[spe] == WUFE_SBI_TYPE_E3 && sub_spe > 0))
   {
      WTI_TerminatePrintError("WTI_LineIdGet", __LINE__);
      return -1;

   }

   return (spe*WTI_T1_LINES_PER_SPE + sub_spe);

}
#endif

/*****************************************************************************
 * Function name:
 * Description  :
 * Input  params:
 * Output params:
 * Return val   :
 *****************************************************************************/
static WP_U8 WTI_SampleSerial(void)
{
#ifdef WP_HW_WINPATH2
   WP_status status;
#endif

#ifdef __linux__
   char buf[1];

   static int console = 0;

   if( !console)
   {
#ifdef WP_HW_WINPATH2
      console = open( "/dev/tty", O_RDWR | O_NDELAY);
#else
      console = open( "/dev/console", O_RDWR | O_NDELAY);
#endif
   }

   if( read( console, buf, 1) != -1)
      return 1;
   else
      return 0;
#endif

#ifndef __PPC__

#ifdef WP_HW_WINPATH2
   /* sample serial port */
   status = *(WP_U32 volatile *)MAP_UA_LSR;

   if (status & (WEB_UART_STATUS_RXFE | WEB_UART_STATUS_OE | WEB_UART_STATUS_DR))
   {
      return 1;
   }
   else
   {
      return 0;
   }
#endif  /* WP_HW_WINPATH2 */

#ifdef WP_HW_WINPATH3
   WP_U32 uart_line_status_reg;
   WP_U32 data;
   /* sample serial port */
#if (!WTI_CESOP_TDI)
   uart_line_status_reg = WPX_AppWpIntRegBaseGet(0) + WT_UART_LINE_STATUS_REG_OFFSET;
#endif

   data = *(WP_U32 volatile *)uart_line_status_reg;
   if (data & (WEB_UART_STATUS_RXFE |
               WEB_UART_STATUS_OE |
               WEB_UART_STATUS_DR))
      return 1;
   else
      return 0;
#endif  /* WP_HW_WINPATH3 */

#else
   return 0;
#endif
}

/*****************************************************************************
 * Function name:
 * Description  :
 * Input  params:
 * Output params:
 * Return val   :
 *****************************************************************************/
void CLI_F_TrafficStart(char *StrPrm)
{
#if (! WTI_CESOP_MPLS_IW)
   WP_data_unit data_unit;
   WP_data_segment segment;
   WP_U32 header_length, length, i;
   WP_U32 gen_control_word, seq_number, offset;
   WP_U8 pattern = 0x5a;
   WP_status status;
   WP_U32 res, cmd, val, iteration_num;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2 && res != 0)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   seq_number = tdm2psn_flow_agg_config.initial_seq_number;

   if (res)
   {
      iteration_num = val;
   }
   else
   {
      iteration_num = cesop_tx_binding_config.window_switchover_threshold+1;
   }

   for (i=0; i<iteration_num; i++)
   {
      header_length = (WTI_L2_HEADER_LEN + WTI_L3_HEADER_LEN +
                       WTI_L4_HEADER_LEN + WTI_RTP_HEADER_LEN + WTI_CONTROL_WORD_LEN);
      length = header_length + tdm2psn_flow_agg_config.payload_size;
      /* create tx data unit */
      memset(&data_unit, 0, sizeof(WP_data_unit));

      data_unit.segment = &segment;
      data_unit.n_segments = 1;
      data_unit.n_active = 1;
      data_unit.data_size = length;

      data_unit.type = WP_DATA_ENET;
      data_unit.control = 0;

      memset(&segment, 0, sizeof(WP_data_segment));
      segment.next = NULL;
      segment.data_size = length;
      segment.pool_handle = the_system->qnode[WTI_QNODE_IW_PSN2TDM].adjunct_pool;
      segment.data = WP_PoolAlloc(segment.pool_handle);

      memset(segment.data, 0, length);
      memcpy(segment.data,
             (WP_U8 *)pw_config.tdm2psn_flow_agg_config.prefix_header,
             header_length);

      if (length >= 64)
         length = 0;
      gen_control_word = seq_number | (length << 16);
      offset = (WTI_L2_HEADER_LEN + WTI_L3_HEADER_LEN + WTI_L4_HEADER_LEN + WTI_RTP_HEADER_LEN);
      memcpy(segment.data+offset, (WP_U8 *)&gen_control_word, 4);
      offset += 4;
      memset(segment.data+offset, pattern, tdm2psn_flow_agg_config.payload_size);

      seq_number++;
      pattern++;

      status = WP_HostSend(the_system->tx_enet_channel,
                           &data_unit);
      WTI_TerminateOnError(status, "WP_HostSend()", __LINE__);
   }
#endif
}

/*****************************************************************************
 * Function name:
 * Description  :
 * Input  params:
 * Output params:
 * Return val   :
 *****************************************************************************/
void CLI_F_SendLastCasHostCommand(char *StrPrm)
{
   WP_U32 res, cmd, val;
   WP_status status;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_SendLastCasHostCommand", __LINE__);
      return;
   }

   printf("Issue last CAS host command on PW %d\n", val);

   status = WP_CesopSendLastCas(the_system->pw[val].trans_rx,
                                the_system->pw[val].tdm2psn_flow_agg);

   if (status)
   {
      printf("WP_CesopSendLastCas returned an error!\n");
      return;
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_PsnPortDisableEnable(char *StrPrm)
{
   WP_status status;
   WP_handle psn_port;
   WP_U32 val,res,cmd;


#if (WTI_CESOP_MPLS_IW && (!WTI_CESOP_MPLS_OVER_ENET))
   psn_port = the_system->hspi_port;
#else
   psn_port = the_system->enet_port;
#endif

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res == 1)
   {
      printf("USE delay of 100\n");
      val=100;

   }
   else if (res == 2)
   {
      printf("USE delay of %d\n",val);
   }else
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   status = WP_PortDisable(psn_port ,WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status , "WP_PortDisable() PSN port",__LINE__);
   if (val !=0)
      WP_Delay(val);

   status = WP_PortEnable (psn_port ,WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status , "WP_PortEnable() PSN port",__LINE__);

   return;
}
void CLI_F_PsnTxChannelDisableEnable(char *StrPrm)
{
	WP_status status;
	WP_U32 cmd, enable_disable, res , channel=4;
        WP_handle tx_enet_channel=0;

	res = sscanf(StrPrm, "%d %d %d", &cmd, &enable_disable,&channel);

	if(res != 2 && res != 3 )
	{
		printf("Too few parameters\n");
		return;
	}

#ifdef __WT_UFE4_DUAL_EMPHY__
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
        printf("Workaround for bug #3681\n");
        
        CLI_SYSCMD_SetTRS("0 0 32 ffff ffff ffff ffff");
        CLI_SYSCMD_SetTRS("0 4 32 ffff ffff ffff ffff");
#endif
#endif

        if(channel == 4)  /* Default: disable/enable all channels */
        {
           if(enable_disable == 0)
           {
              status = WP_ChannelDisable(the_system->tx_enet_channel);
              WTI_TerminateOnError(status, "WP_ChannelDisable() tx_enet",__LINE__);
#if WTI_2_ENET_DEVICES
              status = WP_ChannelDisable(the_system->tx_enet_channel1);
              WTI_TerminateOnError(status, "WP_ChannelDisable() tx_enet1",__LINE__);
#endif
#if WTI_4_ENET_DEVICES
              status = WP_ChannelDisable(the_system->tx_enet_channel2);
              WTI_TerminateOnError(status, "WP_ChannelDisable() tx_enet2",__LINE__);
              
              status = WP_ChannelDisable(the_system->tx_enet_channel3);
              WTI_TerminateOnError(status, "WP_ChannelDisable() tx_enet3",__LINE__);
#endif
              
           }
           else if(enable_disable == 1)
           {
              status = WP_ChannelEnable(the_system->tx_enet_channel);
              WTI_TerminateOnError(status, "WP_ChannelEnable() tx_enet",__LINE__);
#if WTI_2_ENET_DEVICES
              status = WP_ChannelEnable(the_system->tx_enet_channel1);
              WTI_TerminateOnError(status, "WP_ChannelEnable() tx_enet",__LINE__);
#endif
#if WTI_4_ENET_DEVICES
              status = WP_ChannelEnable(the_system->tx_enet_channel2);
              WTI_TerminateOnError(status, "WP_ChannelEnable() tx_enet2",__LINE__);
              
              status = WP_ChannelEnable(the_system->tx_enet_channel3);
              WTI_TerminateOnError(status, "WP_ChannelEnable() tx_enet3",__LINE__);
#endif
           }
           else
           {
              printf("Error: not a supported choice!!!\n");
           }
        }
        else
        {
           switch(channel)
           {
              case (0):
                 tx_enet_channel = the_system->tx_enet_channel;
                 break;
#if WTI_2_ENET_DEVICES
              case (1):
                 tx_enet_channel = the_system->tx_enet_channel1;
                 break;
#endif
#if WTI_4_ENET_DEVICES
              case (3):
                 tx_enet_channel = the_system->tx_enet_channel2;
                 break;
              case (4):
                 tx_enet_channel = the_system->tx_enet_channel3;
                 break;
#endif
              default:
                 break;
           }
           
           if(enable_disable == 0)
           {
              status = WP_ChannelDisable(tx_enet_channel);
              WTI_TerminateOnError(status, "WP_ChannelDisable() tx_enet",__LINE__);
           }
           else if(enable_disable == 1)
           {
              status = WP_ChannelEnable(tx_enet_channel);
              WTI_TerminateOnError(status, "WP_ChannelEnable() tx_enet",__LINE__);
           }
           else 
           {
              printf("Error: not a supported choice!!!\n");
           }
           
        }
        
}

void CLI_F_ClockRecDiffDcoXoClockSet(char *StrPrm)
{
   WP_U32 cmd, clock_freq, res , sys_clock_freq=0;

	res = sscanf(StrPrm, "%d %d", &cmd, &clock_freq);

	if(res > 2)
	{
		printf("Only one clock frequency allowed for XO, Differential and DCO\n");
		return;
	}

	if(res < 2)
	{
		printf("Too few parameters\n");
		return;
	}

	if(((clock_freq/10000)*10000) != clock_freq)
	{
		printf("Not a valid frequency\n");
		return;
	}

	global_clock_rec_diff_ref_clock = clock_freq;
	global_clock_rec_remote_diff_ref_clock = clock_freq;

        if(WP_DISABLE != global_clock_rec_reg_dco_ref_clock)
	global_clock_rec_reg_dco_ref_clock = clock_freq;

        sys_clock_freq = WPL_GetProcessorSpeed();
        sys_clock_freq = 1000*(sys_clock_freq/1000);
        
#if WTI_CI_TESTING_MODE_TDI || WTI_CI_TESTING_MODE_UFE4
	if(!(clock_freq == (sys_clock_freq/2) || clock_freq <=  sys_clock_freq/4))
	{
           printf("Not a valid frequency relative to system clock (%d Hz)\n",sys_clock_freq);
           return;
	}
#endif


}

/*****************************************************************************
 * Function name:
 * Description  :
 * Input  params:
 * Output params:
 * Return val   :
 *****************************************************************************/
void CLI_F_SimulateInterrupts(char *StrPrm)
{
   WP_status status;
#if 0
   WPI_EventQueueDump(0);
#endif

   switch (WTI_PSN2TDM_INTERRUPT_QUEUE)
   {
      case WP_IRQT0:
      {
         status = WP_PollEvents(0, 0, 400);
         break;
      }
      case WP_IRQT1:
      {
         status = WP_PollEvents(0, 1, 400);
         break;
      }
      case WP_IRQT2:
      {
         status = WP_PollEvents(0, 2, 400);
         break;
      }
      case WP_IRQT3:
      {
         status = WP_PollEvents(0, 3, 400);
         break;
      }

      default:
         printf("Wrong IRQ!!\n");
         return;
   }

   if (status != WP_OK)
      printf("WP_PollEvents failed %s\n", WP_error_name[WP_ERROR(status)]);

   return;
}

#if WTI_CESOP_TDI == 0
/*****************************************************************************
 * Function name:
 * Description  :
 * Input  params:
 * Output params:
 * Return val   :
 *****************************************************************************/
void CLI_F_UfeInterrupts(char *StrPrm)
{
   WT_UfeReadEvents(&the_system->ufe);
   return;
}
#endif

/*****************************************************************************
 * Function name:
 * Description  :
 * Input  params:
 * Output params:
 * Return val   :
 *****************************************************************************/
void CLI_DefaultIwPwe3CesConfig(char *StrPrm)
{
   WP_U32 res, cmd, num_slots, i;

   res = sscanf(StrPrm, "%d %d", &cmd, &num_slots);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   /* PWE3 channel configuration */
   CLI_F_Pwe3ChannelStatmode("0 1");

   /* initialize user defined patterns */
   for (i=0;i<num_slots;++i)
   {
      pw_config.tx_udpattern[i] = 5;
      pw_config.tx_udp_dummy_pattern[i] = 32+i;
   }

   CLI_F_Pwe3ChannelUdpDummyMode("0 1");
   CLI_F_Pwe3ChannelTxUdcas("0 f");

   /**************** TDM --> PSN IW *****************/
   CLI_F_Tdm2PsnFlowAggTsParams("0 0 0 0");
   CLI_F_Tdm2PsnFlowAggTsConstSet("0 4");

   /**************** PSN --> TDM IW *****************/
   CLI_F_Psn2TdmFlowAggTsParams("0 0 0");
   CLI_F_Psn2TdmTxBinLOPSDetection("0 1 6 4");
}

/*****************************************************************************
 * Function name:
 * Description  :
 * Input  params:
 * Output params:
 * Return val   :
 *****************************************************************************/
void cesop_event_function(WP_tag tag, WP_U32 data, WP_U32 info)
{
   WP_status status;

   if (WP_EVENT_FUNCTION_P(data, WP_EVENT_FUNCTION_PWE))
   {
      if (data & WP_EVENT_CESOP_IDLE_START)
      {
         /* receive IDLE start event for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_IDLE_START EVENT OCCURED!\n");

         return;
      }
      else if (data & WP_EVENT_CESOP_IDLE_END)
      {
         /* receive IDLE end event for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_IDLE_END EVENT OCCURED!\n");

         return;
      }
      else if (data & WP_EVENT_CESOP_AIS_START)
      {
         /* receive AIS start event for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_AIS_START EVENT OCCURED!\n");

         return;
      }
      else if (data & WP_EVENT_CESOP_AIS_END)
      {
         /* receive AIS end event for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_AIS_END EVENT OCCURED!\n");

         return;
      }
      else if (data & WP_EVENT_CESOP_RDI_START)
      {
         /* receive RDI start event for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_RDI_START EVENT OCCURED!\n");

         return;
      }
      else if (data & WP_EVENT_CESOP_RDI_END)
      {
         /* receive RDI end event for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_RDI_END EVENT OCCURED!\n");

         return;
      }
      else if (data & WP_EVENT_CESOP_R_BIT_START)
      {
         /* receive R bit start event for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_R_BIT_START EVENT OCCURED!\n");

         return;
      }
      else if (data & WP_EVENT_CESOP_R_BIT_END)
      {
         /* receive R bit end event for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_R_BIT_END EVENT OCCURED!\n");

         return;
      }
      else if (data & WP_EVENT_CESOP_CAS_CHANGE)
      {
         WP_U32 index=0;

         /* receive cas change for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_CAS_CHANGE EVENT OCCURED!\n");

         printf("Flow number %x\n", (WP_U32) (info >> 16));

         if ((info >> 16) == 0x10)
            index = 0;

         else if ((info >> 16) == 0x13)
            index = 1;

         printf("Issue last CAS host command on PW %d\n", index);
         status = WP_CesopSendLastCas(the_system->pw[index].trans_rx,
                                      the_system->pw[index].tdm2psn_flow_agg);

         if (status)
         {
            printf("WP_CesopSendLastCas returned an error!\n");
            return;
         }

         return;
      }
      else if (data & WP_EVENT_CESOP_OOBC_SENT)
      {
         /* receive out of band cas event for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_OOBC_SENT EVENT OCCURED!\n");

         return;
      }
      else if (data & WP_EVENT_CESOP_OOBC_HOST_CMD_FAILED)
      {
         /* receive host command failed event for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_OOBC_HOST_CMD_FAILED EVENT OCCURED!\n");

         return;
      }
      else
         return;
   }
}

/************************** MEMORY DISPLAY ***********************************/

#define MAX_ARGC                15

#define SHELL_MAX_LINE_LEN      79
#define SHELL_MAX_COMMAND_LEN   (4 * SHELL_MAX_LINE_LEN)
#define SHELL_MAX_TOKEN_LEN     (2 * SHELL_MAX_LINE_LEN)

static WP_U32 argc;
static char   *(argv[MAX_ARGC]);
static char   arg_buf[MAX_ARGC][SHELL_MAX_TOKEN_LEN+1];

typedef enum
{
   STATE_TOKENIZE_SEARCH,
   STATE_TOKENIZE_SPECIAL,
   STATE_TOKENIZE_QUOTE_DOUBLE,
   STATE_TOKENIZE_QUOTE_SINGLE,
   STATE_TOKENIZE_DONE,
   STATE_TOKENIZE_WORD,
   STATE_TOKENIZE_QUOTE_DOUBLE_SPECIAL,
   STATE_TOKENIZE_QUOTE_SINGLE_SPECIAL,
   STATE_TOKENIZE_WORD_SPECIAL
} t_tokenize_state;

/* Number prefixes */
typedef struct
{
      char   *name;
      WP_U32 base;
} t_sh_prefix;
static t_sh_prefix sh_prefix[] =
{
   { "0x",  16 },
   { "0X",  16 },
   { "16/", 16 },
   { "10/", 10 },
   { "8/",   8 }
};
#define SHELL_PREFIX_NUMBER_COUNT    (sizeof(sh_prefix)/sizeof(t_sh_prefix))

static void remove_space( char *line )
{
   char   clean[SHELL_MAX_COMMAND_LEN + 1];
   WP_U32 len = 0;
   int   skip;
   int   found_char   = WP_FALSE;
   char   *start       = line;
   int   quote_double = WP_FALSE;
   int   quote_single = WP_FALSE;
   int   special        = WP_FALSE;

   while( line[0] != '\0' )
   {
      skip = WP_FALSE;

      /*  Remove excess whitespace, unless within single or
       *  double quotes.
       */
      if( line[0] == ' ' )
      {
         if( ( !found_char      ||  /* Leading spaces  */
               (line[1] == ' ')   ||  /* Multiple spaces */
               (line[1] == '\0') )  &&  /* Trailing spaces */
             (!quote_double && !quote_single))
            skip = WP_TRUE;
      }
      else if ( line[0] == '\\' )
         special = !special;
      else if ( (line[0] == '\'') && !special )
      {
         if( !quote_double )
            quote_single = !quote_single;
      }
      else if ( (line[0] == '"') && !special )
      {
         if( !quote_single )
            quote_double = !quote_double;
      }
      else
         special = WP_FALSE;

      if( !skip )
      {
         clean[len++] = line[0];
         found_char   = WP_TRUE;
      }
      line++;
   }

   clean[len] = '\0';
   strcpy( start, clean );
}

static WP_U32 tokenize( char *line )
{
   t_tokenize_state  state;
   char         *s;
   WP_U32       len;
   WP_U32       rc;

   state = STATE_TOKENIZE_SEARCH;
   argc  = 0;
   len   = 0;
   s     = argv[argc];
   rc    = WP_OK;

   do
   {
      switch( state )
      {
         case STATE_TOKENIZE_SEARCH :
            switch( *line )
            {
               case '\\' : state = STATE_TOKENIZE_SPECIAL; break;
               case '\'' : state = STATE_TOKENIZE_QUOTE_SINGLE; break;
               case '"'  : state = STATE_TOKENIZE_QUOTE_DOUBLE; break;
               case ' '  : state = STATE_TOKENIZE_SEARCH; break;
               case '\0' : state = STATE_TOKENIZE_DONE; break;
               default :
                  if( argc == MAX_ARGC )
                     rc = -1;
                  else
                  {
                     *s++ = *line;
                     len++;
                     state = STATE_TOKENIZE_WORD;
                  }
                  break;
            }
            break;

         case STATE_TOKENIZE_SPECIAL :
            switch( *line )
            {
               case '\0' : state = STATE_TOKENIZE_DONE; break;
               case '\\' :
               case '\'' :
               case '"'  :
               default   :
                  if( argc == MAX_ARGC )
                     rc = -1;
                  else if( len == SHELL_MAX_TOKEN_LEN )
                     rc = -1;
                  else
                  {
                     *s++ = *line;
                     len++;
                     state = STATE_TOKENIZE_WORD;
                  }
                  break;
            }
            break;

         case STATE_TOKENIZE_QUOTE_DOUBLE :
            switch( *line )
            {
               case '\\' : state = STATE_TOKENIZE_QUOTE_DOUBLE_SPECIAL; break;
               case '"'  : state = STATE_TOKENIZE_WORD; break;
               case '\0' :  /* Should not happen */
                  rc = -1; break;
               case ' ' :
               case '\'' :
               default  :
                  if( argc == MAX_ARGC )
                     rc = -1;
                  else if( len == SHELL_MAX_TOKEN_LEN )
                     rc = -1;
                  else
                  {
                     *s++ = *line;
                     len++;
                     state = STATE_TOKENIZE_QUOTE_DOUBLE;
                  }
                  break;
            }
            break;

         case STATE_TOKENIZE_QUOTE_SINGLE :
            switch( *line )
            {
               case '\\' : state = STATE_TOKENIZE_QUOTE_SINGLE_SPECIAL; break;
               case '\''  : state = STATE_TOKENIZE_WORD; break;
               case '\0' :  /* Should not happen */
                  rc = -1; break;
               case ' ' :
               case '"' :
               default  :
                  if( argc == MAX_ARGC )
                     rc = -1;
                  else if( len == SHELL_MAX_TOKEN_LEN )
                     rc = -1;
                  else
                  {
                     *s++ = *line;
                     len++;
                     state = STATE_TOKENIZE_QUOTE_SINGLE;
                  }
                  break;
            }
            break;

         case STATE_TOKENIZE_QUOTE_DOUBLE_SPECIAL :
            switch( *line )
            {
               case '\0' :  /* Should not happen */
                  rc = -1; break;
               case '\\' :
               case '\'' :
               case '"'  :
               default   :
                  if( argc == MAX_ARGC )
                     rc = -1;
                  else if( len == SHELL_MAX_TOKEN_LEN )
                     rc = -1;
                  else
                  {
                     *s++ = *line;
                     len++;
                     state = STATE_TOKENIZE_QUOTE_DOUBLE;
                  }
                  break;
            }
            break;

         case STATE_TOKENIZE_QUOTE_SINGLE_SPECIAL :
            switch( *line )
            {
               case '\0' :  /* Should not happen */
                  rc = -1; break;
               case '\\' :
               case '\'' :
               case '"'  :
               default   :
                  if( argc == MAX_ARGC )
                     rc = -1;
                  else if( len == SHELL_MAX_TOKEN_LEN )
                     rc = -1;
                  else
                  {
                     *s++ = *line;
                     len++;
                     state = STATE_TOKENIZE_QUOTE_SINGLE;
                  }
                  break;
            }
            break;

         case STATE_TOKENIZE_WORD :
            switch( *line )
            {
               case '\\' : state = STATE_TOKENIZE_WORD_SPECIAL; break;
               case '\'' : state = STATE_TOKENIZE_QUOTE_SINGLE; break;
               case '"'  : state = STATE_TOKENIZE_QUOTE_DOUBLE; break;
               case '\0' :
                  if( argc == MAX_ARGC )
                     rc = -1;
                  else
                  {
                     *s = *line;
                     argc++;
                     state = STATE_TOKENIZE_DONE;
                  }
                  break;

               case ' ' :
                  if( argc == MAX_ARGC )
                     rc = -1;
                  else
                  {
                     *s  = '\0';
                     argc++;
                     s     = argv[argc];
                     len   = 0;
                     state = STATE_TOKENIZE_SEARCH;
                  }
                  break;

               default :
                  if( argc == MAX_ARGC )
                     rc = -1;
                  else if( len == SHELL_MAX_TOKEN_LEN )
                     rc = -1;
                  else
                  {
                     *s++ = *line;
                     len++;
                     state = STATE_TOKENIZE_WORD;
                  }
                  break;
            }
            break;

         case STATE_TOKENIZE_WORD_SPECIAL :
            switch( *line )
            {
               case '\0' :
                  if( argc == MAX_ARGC )
                     rc = -1;
                  else
                  {
                     *s = *line;
                     argc++;
                     state = STATE_TOKENIZE_DONE;
                  }
                  break;

               case '\\' :
               case '\'' :
               case '"'  :
               default   :
                  if( argc == MAX_ARGC )
                     rc = -1;
                  else if( len == SHELL_MAX_TOKEN_LEN )
                     rc = -1;
                  else
                  {
                     *s++ = *line;
                     len++;
                     state = STATE_TOKENIZE_WORD;
                  }
                  break;
            }
            break;

         default : /* Should not happen */
            rc = -1;
            break;
      }

      line++;
   }
   while( (rc == WP_OK) && (state != STATE_TOKENIZE_DONE) );

   return rc;
}

static int shell_decode_number(WP_U32 *addr, char* token)
{
   WP_U32 len, base;
   WP_U32 len_temp;
   char   *endp;
   WP_U32 i;

   len  = 0;   /* In case of no radix prefix */
   base = 16;

   for( i=0; i < SHELL_PREFIX_NUMBER_COUNT; i++ )
   {
      len_temp = strlen( sh_prefix[i].name );
      if( strncmp( token, sh_prefix[i].name, len_temp ) == 0 )
      {
         base  = sh_prefix[i].base;
         len   = len_temp;
         break;
      }
   }

   *addr = (WP_U32)strtoul( &token[len], &endp, base );

   if(*endp == '\0')
      return 0;
   else
      return 1;
}

static int GetAddress ( WP_U32 *addr, char *token )
{
   int rc=0;
   char *ptr;

   while (*token =='*')
      token++;

   ptr = strstr(token,":");
   if (ptr)
      *ptr = 0;

   rc = shell_decode_number(addr, token);

   return rc;
}

static WP_U32 shell_md(WP_U32 argc,char ** argv)
{
   WP_U32 saddr,eaddr;
   WP_U32 addr;
   static WP_U32 LastMemoryAddress = 0;

   if (argc > 0)
   {
      if (GetAddress(&saddr,argv[0]))
         return -1;
   }
   else
      saddr = LastMemoryAddress;
   saddr = (saddr+3)/4*4;  /* align 4 */

   if (argc > 1)
   {
      if (GetAddress(&eaddr,argv[1]))
         return -1;
   }
   else
      eaddr = saddr+MD_BLOCK_SIZE;
   eaddr = (eaddr+3)/4*4;  /* align 4 */

   for (addr=saddr; addr<eaddr; addr+=4)
   {
      if (((addr-saddr)&0xf)==0)
         printf("\n %08x:  ",addr);
      printf("%08x ",*(WP_U32 *)addr);
   }
   printf("\n");

   LastMemoryAddress = eaddr;
   return 0;
}

static WP_U32 execute_command(void)
{
   WP_U32 rc=0;

   if( argc != 0 )
   {
      /* Execute command */
      rc = shell_md( argc, &argv[0] );
   }

   return rc;
}

static void separate( char   *line, WP_U32 *subcommand_count )
{
   int   quote_double = WP_FALSE;
   int   quote_single = WP_FALSE;
   int   special        = WP_FALSE;

   *subcommand_count = 1;

   /*  Find ';' not within single or double quotes and substitute
    *  with '\0'
    */

   while( line[0] != '\0' )
   {
      if( (*line == ';') && !quote_double && !quote_single )
      {
         *line = '\0';
         (*subcommand_count)++;
      }
      else if( *line == '\\' )
         special = !special;
      else if( (*line == '\'') && !special )
      {
         if( !quote_double )
            quote_single = !quote_single;
      }
      else if( (*line == '"') && !special )
      {
         if( !quote_single )
            quote_double = !quote_double;
      }
      else
         special = WP_FALSE;

      line++;
   }
}

static WP_U32 execute_line( char *line )
{
   char   buffer[SHELL_MAX_COMMAND_LEN + 1];
   char   subcommand[SHELL_MAX_COMMAND_LEN + 1];
   char   *line_copy;
   WP_U32 subcommand_count;
   WP_U32 rc=0;
   WP_U32 len;

   line_copy = buffer;

   strcpy( line_copy, line );

   /* Separate line in subcommands based on ';' characters */
   separate( line_copy, &subcommand_count );

   /* Determine repeat (+<n>) count */
   len = strlen(line_copy);
   strcpy( subcommand, line_copy );
   rc = tokenize( subcommand );
   if( rc == WP_OK )
   {
      rc = execute_command();
   }

   return rc;
}

static void MemoryDisplay(void)
{
   char input[300];
   int i;

   /* Init */
   for(i=0; i<MAX_ARGC; i++)
      argv[i] = arg_buf[i];

   strcpy(input, "");
   while (strcmp(input, "exit") != 0)
   {
      printf("\nAddress [start <end>]> (type exit to return to menu) ");
#ifdef  __linux__
      fgets(input,256,stdin);
#else
      gets(input);
#endif

      /* remove extra spaces */
      remove_space(input);
      if (strlen(input) == 0)
         continue;

#ifdef  __linux__
      if (strncmp(input, "exit", 4) == 0)
         break;
#else
      if (strcmp(input, "exit") == 0)
         break;
#endif

      if (execute_line(input) != 0)
      {
         WTI_PRINT_ERROR(("\nERROR: Illegal input\n"));
      }
   }

   printf("\n");
}

WP_status WPE_DebugCallback(const WP_CHAR *file, const WP_CHAR *function,
                            WP_S32 line, WP_S32 error_id)
{
   if (error_id)
      printf("\n\nfile = %s , line = %d\nfunction = %s\n",file,line,function);
   return WP_OK;
}

WP_status WPU_TraceBuffer(WP_S32 argc, WP_CHAR ** argv);
void WPU_WinutilCall(WP_CHAR * input);
#if 1
void CLI_WinUtilBusErrCheck(char *StrPrm)
{
   WPU_WinutilCall("hbm");
}

void CLI_WinUtilWingineBusUtilization(char *StrPrm)
{
   WPU_WinutilCall("sysu");
}

void CLI_WinUtilThreadUtilization(char *StrPrm)
{
   WPU_WinutilCall("trsu");
}

void CLI_WinUtilThreadArbiterDump(char *StrPrm)
{
   WPU_WinutilCall("taot");
   printf("\n");
   WPU_WinutilCall("tast");
   printf("\n");
   WPU_WinutilCall("tab");
   printf("\n");
   WPU_WinutilCall("taom");
   printf("\n");
   WPU_WinutilCall("dmap");
   printf("\n");
   WPU_WinutilCall("bmap");
}

void CLI_WinUtilSerialArbiterDump (char *StrPrm)
{
   WPU_WinutilCall("trsa");
   printf("\n");
   WPU_WinutilCall("smap");
   printf("\n");

}

void CLI_WinUtilTraceStart (char *StrPrm)
{
   /* Todo: Here need to pass paramaeters as serial sync etc*/
   WP_CHAR * argv[2];
   argv[0]="tb";
   argv[1]="init";
   WPU_TraceBuffer(2, argv);
}

void CLI_WinUtilTraceDump (char *StrPrm)
{
   /* Todo: Here need to pass paramaeters as serial sync etc*/
   WP_CHAR * argv[2];
   argv[0]="tb";
   argv[1]="dump";
   WPU_TraceBuffer(2, argv);
}

void CLI_DebugVersionInfo  (char *StrPrm)
{
   WPU_WinutilCall("ver");
}

void CLI_WinUtilTrsDump  (char *StrPrm)
{
   WP_U32 res, cmd;
   WP_U32 trs_num;
   WP_CHAR winutil_cmd[100];

   res = sscanf(StrPrm, "%d %d ", &cmd, &trs_num);

   if (res != 2)
   {
      printf("ERROR:invalid number of parameters\n");
      return;
   }

#ifdef WP_HW_WINPATH3
   if (trs_num > 64)
   {
      printf("ERROR: WP3 invalid TRS number\n");
      return;
   }
#endif

#ifdef WP_HW_WINPATH3
   if (trs_num > 32)
   {
      printf("ERROR: WP2 invalid TRS number\n");
      return;
   }
#endif
   sprintf(winutil_cmd,"trs %d", trs_num);

   WPU_WinutilCall(winutil_cmd);
}
/*****************************************************************************
 *
 * Function: WT_DebugAreaCallback
 *
 * Purpose: Prints the debug area.  Normally used for dps trace information.
 *
 * Inputs:
 *     wpid			winpath ID
 *	   size			size in bytes of the area
 * host_ptr			pointer to the start of the debug block
 *
 * Outputs:
 *     prints the block
 *
 * Return Value:
 *     None
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WT_DebugAreaCallbackMD(WP_U32 wpid, WP_U32 size_allocated, WP_U32 *host_ptr)
{

   WP_U32 size = host_debug_user_size;
   WP_U32 count;
   WP_U32 line_length =4;

   if(size> size_allocated)
      size = size_allocated;

   host_ptr = (WP_U32 *)((WP_U32)host_ptr&0xFFFFFFF0);
   printf("\n Winpath %d, start of debug block 0x%8.8x, size 0x%x bytes\n", wpid, (WP_U32) host_ptr, size);
   for (count = 0 ; count < (size/line_length) ; count++)
   {
      if (!(count % line_length))
         printf("\n%8.8x (%.3d) : ", (WP_U32) host_ptr,count*line_length);

      printf("%8.8x ",*host_ptr++);
   }
   printf("\n");
}

void WT_DebugAreaCallbackMM(WP_U32 wpid, WP_U32 size_allocated, WP_U32 *host_ptr)
{

   if(host_debug_user_offset> size_allocated)
   {
      WTI_TerminatePrintError("Invalid user offset", __LINE__);
      return;
   }

   host_ptr = (WP_U32 *)((WP_U32)host_ptr&0xFFFFFFF0);
   host_ptr += host_debug_user_offset;
   *host_ptr = host_debug_user_value;
}


void CLI_DebugMemModify (char *StrPrm)
{
   WP_status status;
   WP_U32 res, cmd, offset,value;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &offset,&value);

   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }


   host_debug_user_offset = offset;
   host_debug_user_value = value;

   status = WP_ControlRegister(WP_DEBUG_CALLBACK_DEBUG_AREA_LOG, &WT_DebugAreaCallbackMM);
   WTI_TerminateOnError(status, "WP_ControlRegister()",__LINE__);
   status = WP_DebugAreaLog(0,WP_B_DEBUG);
   WTI_TerminateOnError(status, "WP_DebugAreaLog()",__LINE__);
}


void CLI_DebugMemDisplay (char *StrPrm)
{
   WP_status status;
   WP_U32 res, cmd, size;


  res = sscanf(StrPrm, "%d %d", &cmd, &size);

   if (res != 1 && res !=2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if(res == 2)
   {
      host_debug_user_size = size;
   }
   else
   {
      host_debug_user_size = 0x100;
   }

   status = WP_ControlRegister(WP_DEBUG_CALLBACK_DEBUG_AREA_LOG, &WT_DebugAreaCallbackMD);
   WTI_TerminateOnError(status, "WP_ControlRegister()",__LINE__);
   status = WP_DebugAreaLog(0,WP_B_DEBUG);
   WTI_TerminateOnError(status, "WP_DebugAreaLog()",__LINE__);
}
#endif

#if _WT_MODULE_TEST_COMMENT_ENABLE_
/*****************************************************************************
 * NOTE:
 * This is where the shared routines gets included.  Ideally, this would
 * be done as a separately linked module.
 ****************************************************************************/
#endif
#include "wti_cesopsn_util.c"
#include "wti_cesopsn_statistics.c"

#if WTI_CESOP_TDI
#include "wti_cesopsn_predef_cfg.c"
#else
#ifdef __WT_UFE3__
#include "wti_ufe_util.c"
#else
#include "wti_ufe_utility.c"
#endif
#ifdef __WT_UFE3__
#include "wti_cesopsn_predef_cfg.c"
#else
#if __WT_UFE412__
#include "wti_cesopsn_predef_ufe412.c"
#else
#include "wti_cesopsn_predef_ufe448.c"
#endif
#endif
#endif
#if WTI_PCE_CLASSIFIER
#include "wt_partition_ecc_util.c"
#endif

