/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/****************************************************************************
 *
 * Example: wt_ufe_util.h
 * Demonstrates prototypes and definitions of UFE2 utility functions
 *
 ****************************************************************************/
#ifndef WT_UFE_UTIL_H
#define WT_UFE_UTIL_H

#include "wp_wddi.h"
#include "wufe_ufe.h"

/****************************************************************
              UFE define the Card type SBI or CAD.
****************************************************************/


#define N_UFE_SBI_SPE_LINE 28

#ifdef __WT_UFE3__
#define MAX_UFE_LINES_USED 336
#define N_MAX_UFE_SBI_LINE 336
#define N_MAX_UFE_CORES    2
#define N_MAX_UFE_ZBT_BUFFERS 0xf000 /*for 4Mb*/
#define N_MAX_EMPHY_PHYS   8064
#if WTI_CAD_0_3_SETUP
#define N_MAX_UFE_CAD_LINE 64
#define N_MAX_UFE_OCT_LINE 64
#else
#define N_MAX_UFE_CAD_LINE 64
#define N_MAX_UFE_OCT_LINE 64
#endif /* WTI_CAD_0_3_SETUP */
#endif

#define N_MAX_UFE_SPE_INDEX WUFE_MAX_N_CORE*WUFE_MAX_N_STM1*WUFE_MAX_N_SPE

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
#else  /* NOT WP_HW_WINPATH3 */
#define UFE_MEM_PORT_SIZE                           WP_CLOCK_REC_PORT_SIZE_64
#endif /* NOT WP_HW_WINPATH3 */

typedef enum
{
   WT_OCTAL = 0,
   WT_SBI,
   WT_HMVIP
} WT_fpga_mode;


typedef enum
{
   WT_CRC_16 = 1,
   WT_CRC_32 = 2,
} WT_crc_type;


typedef enum
{
   WT_EXT_CLOCK          = 0,
   WT_RX_LOOPTIME_CLOCK  = 1,

}WT_tx_clk;

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
} WT_ufe_line_octal_params;

typedef struct
{
   WP_U8                 valid;
   WUFE_line_transf_type transfer_type;
   WUFE_un_framed        framed;
   WUFE_cas_enable       cas_enable;
   WUFE_clock_rec_mode   clock_rec_mode;
   WUFE_clock_master     tx_clk_master;
   WP_U16                clock_translator_id;
   WP_U16                rx_clk_rec_if;
   WUFE_cad_cr_extended  ext_mode_active;
} WT_ufe_line_params;

typedef struct
{
  /* User Configure parameters */
   WP_U32                wpid;
   WP_U32                ufe_id;
   WP_U32                core_id[N_MAX_UFE_CORES];
   WP_port               upi_id;
   WT_fpga_mode          fpga_mode;
   WP_U16                max_lines;
   WP_U16                max_phys;
   WP_U16                max_sbi_clock_translators;
   WP_U16                max_sbi_ext_diff_clock_translators;
   WUFE_line_transf_type sbi_spe_line_type[N_MAX_UFE_SPE_INDEX];
   WT_crc_type           hdlc_crc;
   WT_tx_clk             tx_clock_mode;
   WT_operation_mode     operation_mode;
   WUFE_system_clock_rec system_clock_rec_enable;
   WUFE_clk_rec_system_params   *clk_rec_system_params; /* Not in used in UFE3 */
   WP_U8                 clock_rec_method;

   /*****************************************************************************/
   /* internal handeled data stucture user Read only */

   WT_ufe_line_params    line_params[MAX_UFE_LINES_USED];
   WUFE_handle           line_handle[MAX_UFE_LINES_USED];
   WUFE_handle           phy_handle[N_MAX_EMPHY_PHYS];
   WP_U8                 temux_used;
   WUFE_events           ufe_events;
} WT_ufe;

WP_U32 upi_index = (WTI_EMPHY_PORT == WP_PORT_UPI1) ? 0 : 1;

void WT_UfeLineCreate(WT_ufe *ufe, WP_U32 index, void *config, WP_U8 enable);
void WT_UfeLineEnable(WT_ufe *ufe, WP_U32 line_index);
void WT_UfeLineDelete(WT_ufe *ufe, WP_U32 index);
void WT_UfeLineSbiCreate(WT_ufe *ufe, WP_U32 line_index, WUFE_line_sbi *line_cfg,
                         WP_U8 enable);
void  WT_UfeLineOctalCreate(WT_ufe *ufe,
                             WP_U32 line_index,
                             WT_ufe_line_octal_params *line_octal_params,
                             WP_U8 enable);
void WT_UfePhyTransCreate(WT_ufe *ufe,
                          WP_U32 line_index,
                          WP_U32 phy_index,
                          WP_U32 n_slots,
                          WP_U8 *slots_arr,
                          WP_U32 ufe2_dataunit_size,
                          WP_U8 rx_clk_rec_mode);
void WT_UfePhyEnable(WT_ufe *ufe, WP_U32 phy_index);
void WT_UfePhyDisable(WT_ufe *ufe, WP_U32 phy_index);
void WT_UfePhyDelete(WT_ufe *ufe, WP_U32 phy_index);
void WT_UfePhyRxClkRecReconfigure(WT_ufe *ufe, WP_U32 phy_index, WP_U32 enable);
void WT_UfeLineSbiModify(WT_ufe *ufe, int line_index, WUFE_line_sbi_reconfig *config);
void WT_UfeClkRecRxCgenModify(WT_ufe *ufe, WP_U32 new_line_index);

void  WT_UfeSystemCreate(WT_ufe *ufe);
void  WT_UfeSystemEnable(WT_ufe *ufe);
void WT_UfeReadEvents(WT_ufe *ufe);
WP_U32 WT_UfeReadEvents4Automation(WT_ufe *ufe);
WP_U32 WT_UfeAnalyzeEvents4Automation(WT_ufe *ufe);
void  WT_UfeRelease(WT_ufe *ufe);

void WT_UfeTdmOctalOverUfeInit(WT_ufe *ufe, WT_ufe_line_octal_params *line_octal_params);


void WT_UfeClockRecMasterSet(WUFE_handle ufe_phy_handle,
                              WP_U8 tx_data_unit_size);

void WT_UfeClockRecInfoGet(WP_U32 wufe_id,
                             WUFE_handle ufe_phy_handle,
                             WUFE_clock_rec_info_type info_type,
                             void* clk_rec_info);

void WT_UfeClockRecIndirectCgenRead(WP_U16 entry);
void WT_UfeClockRecIndirectPortRamRead(WP_U16 clk_rec_if, WP_U16 entry);
void WT_UfeClockRecIndirectCtRead(WP_U16 entry);

void WT_IssueOneShot(void);
WP_U16 WT_ReadRemoteTemp(void);
void WT_AsciiWait(void);
void WT_Vt100CrsrRight(WP_U32 right);
void WT_Vt100CrsrLeft(WP_U32 left);
WP_U8 WT_SampleSerial(void);
void WT_UfeMemRead(WP_U32 offset, WP_U16 *value);
void WT_UfeMemWrite(WP_U32 offset, WP_U16 value);

#if __WT_UFE3__
void WT_UfeTdmCometOverUfeInit(WT_ufe *ufe, WT_ufe_line_octal_params *line_octal_params);
#endif

#define WT_SELECT_MIN(a,b) ((a) < (b) ? (a) : (b))


#endif
