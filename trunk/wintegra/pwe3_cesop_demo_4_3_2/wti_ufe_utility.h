/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/****************************************************************************
 *
 * Example: wti_ufe_utility.h
 * Demonstrates prototypes and definitions of UFE utility functions
 *
 ****************************************************************************/
#ifndef WTI_UFE_UTILITY_H
#define WTI_UFE_UTILITY_H

#include "wp_wddi.h"
#include "wufe_ufe.h"

#ifndef WPX_BOARD_DATA_H
#include "wpx_board_data.h"
#endif

/****************************************************************
              UFE define the Card type SDH or SONET.
****************************************************************/

#define N_UFE_SBI_SPE_LINE 28
#ifdef __WT_UFE412__
#define N_MAX_UFE_SBI_LINE 336
#elif defined __WT_UFE448__
#define N_MAX_UFE_SBI_LINE 1344
#endif
#define N_MAX_UFE_ZBT_BUFFERS 0xf000 /*for 4Mb*/
#if WTI_CAD_0_3_SETUP
#define N_MAX_UFE_CAD_LINE 4
#define N_MAX_UFE_OCT_LINE 4
#else
#define N_MAX_UFE_CAD_LINE 64
#define N_MAX_UFE_OCT_LINE 64
#endif /* WTI_CAD_0_3_SETUP */

#define N_MAX_UFE_SPE_INDEX WUFE_MAX_N_CORE*WUFE_MAX_N_STM1*WUFE_MAX_N_SPE

#ifdef __WT_UFE448__
#define N_MAX_UFE_CORES    2
#define MAX_UFE_LINES_USED 1344
#elif WTI_UFE_4_1_2
#define N_MAX_UFE_CORES    1
#if WTI_DUAL_EMPHY
#define MAX_UFE_LINES_USED 672
#else
#define MAX_UFE_LINES_USED 336
#endif
#else /* UFE3 */
#define N_MAX_UFE_CORES    1
#define MAX_UFE_LINES_USED 336
#endif
#define N_MAX_EMPHY_PHYS   4032

#define DBG_MODE 0

#define WT_UFE_HANDLE_INVALID 0xffffffff

#define FACTOR                 90000000 /* for board 180/90 */
#define WEB_UART_STATUS_RXFE   0x80
#define WEB_UART_STATUS_DR     0x01
#define WEB_UART_STATUS_OE     0x02
#ifdef __PPC__
#define MAP_UA_LSR    (0x1f010f1cL)
#endif
#ifdef __mips__
#define MAP_UA_LSR    (0xbd030f1cL)
#endif

#ifdef WP_HW_WINPATH3
#define UFE_MEM_PORT_SIZE                           WP_CLOCK_REC_PORT_SIZE_16
#endif

// #if (WTI_EMPHY_PORT == WP_PORT_UPI2) /* UPI2 */
// #define WTI_EMPHY_PORT_OFFSET    0x200000  /* UPI2 */
// #else /* WTI_EMPHY_PORT  */
// #define WTI_EMPHY_PORT_OFFSET    0x0       /* UPI1 */
// #endif /* WTI_EMPHY_PORT  */

// #define WTI_UFE_BASE                     ( 0x1f200000 + WTI_EMPHY_PORT_OFFSET )

// #define WTI_UFE_VIR_BASE                 (WTI_UFE_BASE + 0xa0000000)

typedef enum
{
   WT_OCTAL =0,
   WT_SBI,
   WT_HMVIP,
   WT_SONET,
   WT_SDH,
} WT_fpga_mode;


typedef enum
{
   WT_CRC_16 = 1,
   WT_CRC_32 = 2,
} WT_crc_type;

typedef enum
{
   WT_OP_MODE_NORMAL          = 0,
   WT_OP_MODE_LOOPBACK_LINE   = 1,
   WT_OP_MODE_LOOPBACK_SYSTEM = 2,
}WT_operation_mode;

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
WUFE_clk_rec_system_params the_system_clock_rec_params;
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

typedef struct
{
   WP_U16                line_id;
   WUFE_line_transf_type transfer_type;
   WUFE_un_framed        framed;
   WUFE_cas_enable       cas_enable;
   WUFE_clock_rec_mode   clock_rec_mode;
   WP_U8                 rx_clk_rec_if;
   WUFE_cad_cr_extended  ext_mode_active;
   WUFE_line_clock_rec_params *clock_rec_line_params;
} WT_ufe_line_octal_params;

typedef struct
{
   WP_U8 stm4;
   WP_U8 stm1;
   WP_U8 stm0;
   WP_U8 tug2;
   WP_U8 tu;
   WUFE_line_transf_type transfer_type;
   WUFE_un_framed        framed;
   WUFE_cas_enable       cas_enable;
   WUFE_line_loopback_type loopback;
   WUFE_line_clock_rec_params *clock_rec_line_params;
   WP_U8 ds3_e3_framing_type;
} WT_ufe_line_sdh_params;

typedef struct
{
   WP_U8 sts12;
   WP_U8 sts3;
   WP_U8 sts1;
   WP_U8 vt_group;
   WP_U8 vt;
   WUFE_line_transf_type transfer_type;
   WUFE_un_framed        framed;
   WUFE_cas_enable       cas_enable;
   WUFE_line_loopback_type loopback;
   WUFE_line_clock_rec_params *clock_rec_line_params;
   WP_U8 ds3_e3_framing_type;
} WT_ufe_line_sonet_params;

typedef union
{
   WT_ufe_line_sonet_params line_sonet_params;
   WT_ufe_line_sdh_params line_sdh_params;
}WT_ufe_framer_line_params;

typedef struct
{
   WP_U8                      valid;
   WUFE_line_transf_type      transfer_type;
   WUFE_un_framed             framed;
   WUFE_cas_enable            cas_enable;
   WUFE_clock_rec_mode        clock_rec_mode;
   WUFE_clock_master          tx_clk_master;
   WP_U16                     clock_translator_id;
   WP_U16                     rx_clk_rec_if;
   WUFE_cad_cr_extended       ext_mode_active;
#ifdef WT_UFE_FRAMER
   WT_ufe_framer_line_params framer_line_params;
#endif
} WT_ufe_line_params;

typedef enum
{
   WT_EXT_CLOCK          = 0,
   WT_RX_LOOPTIME_CLOCK  = 1,
}WT_tx_clk;

typedef struct
{
  /* User Configure parameters */
   WP_U32                wpid;
   WP_U32                ufe_id;
   WP_port               upi_id;
   WT_fpga_mode          fpga_mode;
   WP_U16                max_lines;
   WP_U16                max_phys;
//   WP_U16                max_sbi_clock_translators;
//   WP_U16                max_sbi_ext_diff_clock_translators;
   WUFE_line_transf_type sbi_spe_line_type[N_MAX_UFE_SPE_INDEX];
   WT_crc_type           hdlc_crc;
      WT_tx_clk             tx_clock_mode;//CR related?
   WT_operation_mode     operation_mode;
   WUFE_system_clock_rec system_clock_rec_enable;
   WUFE_clk_rec_system_params   *clk_rec_system_params;
//   WP_U8                 clock_rec_method;
   WT_ufe_line_params    line_params[MAX_UFE_LINES_USED];
   WUFE_handle           line_handle[MAX_UFE_LINES_USED];
   WUFE_handle           phy_handle[N_MAX_EMPHY_PHYS];
   WP_U8                 temux_used;
   WUFE_events           ufe_events;
} WT_ufe;

void WT_UfeLineCreate(WT_ufe *ufe, WP_U32 index, void *config, WP_U8 enable);
void WT_UfeLineDisable(WT_ufe *ufe, WP_U32 line_index);
void WT_UfeLineEnable(WT_ufe *ufe, WP_U32 line_index);
void WT_UfeLineDelete(WT_ufe *ufe, WP_U32 index);
void WT_UfeLineProtectionSwitch(WP_U32 flexmux_id, WP_U32 line_port_id, WP_U32 use_protection);
void WT_UfeLineSonetCreate(WT_ufe *ufe,
                           WP_U32 line_index,
                           WT_ufe_line_sonet_params *line_sonet_params,
                           WP_U8 enable);
void WT_UfeLineSdhCreate(WT_ufe *ufe,
                         WP_U32 line_index,
                         WT_ufe_line_sdh_params *line_sdh_params,
                         WP_U8 enable);
void WT_UfeLineOctalCreate(WT_ufe *ufe,
                           WP_U32 line_index,
                           WT_ufe_line_octal_params *line_octal_params,
                           WP_U8 enable);
void WT_UfePhyTransCreate(WT_ufe *ufe,
                          WP_U32 line_index,
                          WP_U32 phy_index,
                          WP_U32 n_slots,
                          WP_U8 *slots_arr,
                          WP_U32 ufe_dataunit_size,
                          WP_U8 rx_clk_rec_mode);
void WT_UfePhyEnable(WT_ufe *ufe, WP_U32 phy_index);
void WT_UfePhyDisable(WT_ufe *ufe, WP_U32 phy_index);
void WT_UfePhyDelete(WT_ufe *ufe, WP_U32 phy_index);
void WT_UfePhyDisplay(WT_ufe *ufe, WP_U32 phy_index);
void WT_UfeLineSocketLpbkSetup(WT_ufe *ufe, int line_index, WUFE_line_loopback_type lpbk_type);


void  WT_UfeInitializeFirmware(WP_U32 ufe_id);

void  WT_UfeSystemCreate(WT_ufe *ufe);
void  WT_UfeSystemEnable(WT_ufe *ufe);
void WT_UfeReadEvents(WT_ufe *ufe);
void  WT_UfeRelease(WT_ufe *ufe);
WP_U32 WT_UfeReadEvents4Automation(WT_ufe *ufe);
WP_U32 WT_UfeAnalyzeEvents4Automation(WT_ufe *ufe);
void  WT_UfeSystemRxTrafficMask(WT_ufe *ufe, WP_U8 aug1, WP_U8 enable);

void WT_IssueOneShot(void);
WP_U16 WT_ReadRemoteTemp(void);
void WT_AsciiWait(void);
void WT_Vt100CrsrRight(WP_U32 right);
void WT_Vt100CrsrLeft(WP_U32 left);
static WP_U8 WT_SampleSerial(void);
void WT_UfeMemRead(WP_U32 offset, WP_U16 *value);
void WT_UfeMemWrite(WP_U32 offset, WP_U16 value);

#define WT_SELECT_MIN(a,b) ((a) < (b) ? (a) : (b))

/* Clock Recovery empty functions */
#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)
#define WT_UfeClockRecMasterSet(ufe_phy_handle, tx_data_unit_size)
void WT_UfeClockRecInfoGet(WP_U32 wufe_id,
                           WUFE_handle ufe_line_handle,
                           WUFE_clock_rec_info_type info_type,
                           void *clk_rec_info);
WP_U32 WT_ClockRecInterfaceId(WP_U32 line_index);
/* Function for internal testing and debug */
void WT_UfeClockRecTimestampStubSet(WP_U32 enable);
/* End of function for internal testing and debug */
#endif /* CR */

/* IO functions */
void WT_UfeGpioOutputClockSet(WT_ufe *ufe, WP_U32 gpio_number, WP_U8 gpio_clock_type, WP_U8 gpio_ref_clk_out);
void WT_UfeGpioOutputClockRead(WT_ufe *ufe, WP_U16 *gpio_register);
#ifdef WT_UFE_FRAMER
void WT_UFEPDHOutputClockSet(WT_ufe *ufe,WP_U8 output_select, WP_U32 line_index,WP_U32 ClockRate);
WP_U32 CLI_F_UfeFramerPrbsCheck(char *StrPrm);
#endif /* WT_UFE_FRAMER */
#endif
