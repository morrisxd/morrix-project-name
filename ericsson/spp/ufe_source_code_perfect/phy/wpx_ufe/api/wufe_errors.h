/*************************************************************
 * (C) Copyright 2004-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

/***************************************************************************
 *
 * File: wufe_errors.h
 *
 * Purpose: Define error indications returned by the ufe drivewr.
 * This file is included in
 *
 **************************************************************************/

#ifndef WUFE_ERRORS_H
#define WUFE_ERRORS_H

/* The following two lines must not be changed*/
enum WUFE_error
{
   WUFE_OK = 0,
   WUFE_ERR_HANDLE=(WP_S32)0xfc000001,
   WUFE_ERR_NULL_POINTER,
   WUFE_ERR_INVALID_WUFE_ID,
   WUFE_ERR_INVALID_REGISTRY_STATE,
   WUFE_ERR_INVALID_REGISTRY_COMMIT_ACTION,
   WUFE_ERR_INVALID_DIRECTION,
   WUFE_ERR_INVALID_CONFIG,
   WUFE_ERR_HW_CONFIG_STATE_MACHINE,
   WUFE_ERR_HW_CONFIG_CORE_ENABLED,
   WUFE_ERR_HW_CONFIG_NOT_MATCH,
   WUFE_ERR_HW_PLL_LOCK,
   WUFE_ERR_HW_RESET,
   WUFE_ERR_NOT_IMPLEMENTED,
   WUFE_ERR_HW_CONFIG_FIFO_MEMORY_NOT_MATCH,
   WUFE_ERR_HW_CONFIG_FIFO_MEMORY_SIZE,
   WUFE_ERR_HW_CONFIG_INTERNAL_ZBT_MUST_BE_128K,
   WUFE_ERR_INVALID_GPIO_CLOCK,
   WUFE_ERR_UFE_INIT_BEFORE_UPI_ENABLE,
   WUFE_ERR_MAPPING_ATTRIB,
   WUFE_ERR_INIT_B_SIG_NOT_ASSERTED,
   WUFE_ERR_CONF_DONE_SIG_NOT_ASSERTED,
   WUFE_ERR_DEVICE_TYPE_NOT_SET,
   WUFE_ERR_INVALID_DEVICE_TYPE,
   WUFE_ERR_BANDWIDTH_NOT_SUPPORTED,
   WUFE_ERR_WUFE_UFEINITIALIZEFIRMWARE_SUPPORT_ONLY_UFE4,
   WUFE_ERR_WUFE_LINE_PDH_LOOPBACK_SUPPORT_ONLY_UFE4,
   WUFE_ERR_INVALID_EMPHY_WP_PINMUX_CFG,
   WUFE_ERR_HALF_DUPLEX_NOT_SUPPORTED_FOR_UFE4,
   /*EVENTS*/
   WUFE_ERR_EVENT_INVALID_CONFIG,
   /* SYSTEM */
   WUFE_ERR_SYSTEM_RECONFIG_CMD,
   WUFE_ERR_SYSTEM_RECONFIG_SBI_SPE_LINES_EXIST,
   WUFE_ERR_ILLEGAL_BUILD_PERSONALITY,
   WUFE_PORT_INFO_GET_ERR,
   /*MAPPING*/
   WUFE_ERR_MAPPING_NOT_INITIALIZED,
   WUFE_ERR_MAPPING_DUPLICATE,
   /* MEMORY ERRORS*/
   WUFE_ERR_MALLOC,
   /* FIFO MANAGER ERRORS*/
   WUFE_NOT_ENOUGH_MEM_FOR_BUFFER ,
   WUFE_NOT_ENOUGH_RX_BUFFERS,
   WUFE_NOT_ENOUGH_TX_BUFFERS,
   WUFE_ERR_MEM_INCONSIST,
   /* FIFO EMPHY ERRORS*/
   WUFE_ERR_ILLEGAL_EMPHY_PRI_RX_VALUE,
   WUFE_ERR_ILLEGAL_EMPHY_PRI_TX_VALUE,
   WUFE_ERR_ILLEGAL_EMPHY_PRI_TX_INFRAME_VALUE,
   WUFE_ERR_ILLEGAL_EMPHY_PARITY,
   WUFE_ERR_PARITY_CHECK_NOT_SUPPORTED,
   WUFE_ERR_SYSTEM_ALREADY_DISABLED,
   WUFE_ERR_SYSTEM_NOT_DISABLED,
   WUFE_ERR_EMPHY_CORE1_OFFSET_CONFIG,

   /* FIFO TDM ERRORS*/
   WUFE_ERR_ILLEGAL_CAS_PATTERN,
   WUFE_ERR_NO_SYSTEM_CREATED,
   WUFE_ERR_SYSTEM_NOT_ENABLED,
   WUFE_ERR_ILLEGAL_TDM_PARITY,

   /* LINE ERRORS*/
   WUFE_ERR_ILLEGAL_NUM_LINES,
   WUFE_ERR_LINE_INVALID_TYPE,
   WUFE_ERR_CAD_ID_FOR_TRANSFER_TYPE,
   WUFE_ERR_LINE_ILLEGAL_ID,
   WUFE_ERR_HMVIP_T1_MUST_BE_FRAMED,
   WUFE_ERR_DS3_T3_MUST_BE_UNFRAMED,
   WUFE_ERR_INVALID_TRANSFER_TYPE,
   WUFE_ERR_CAD_INVALID_CFG,
   WUFE_ERR_SPE_NOT_EMPTY,
   WUFE_ERR_CAD_SPE_FULL_OR_WITH_SPACES,
   WUFE_ERR_INCONSISTENT_SPE_ARRAY,
   WUFE_ERR_LINES_NOT_SORTED_IN_DECREASING_ORDER,
   WUFE_ERR_LINE_ALREADY_ACTIVE,
   WUFE_ERR_LINE_ALREADY_DISABLED,
   WUFE_ERR_LINE_NOT_DISABLED,
   WUFE_ERR_LINE_NOT_ENABLED,
   WUFE_ERR_LINE_EMPTY,
   WUFE_ERR_LINE_ALREADY_CONFIGURED,
   WUFE_ERR_LINE_CONTAIN_PHYS,
   WUFE_ERR_LINE_CONTAIN_ACTIVE_PHYS,
   WUFE_ERR_LINE_RECONFIG,
   WUFE_ERR_LINE_MALFUNCTION,
   WUFE_ERR_LINE_LOOPBACK_TYPE_LINE_NOT_ENABLED_IN_HW,
   WUFE_ERR_LINE_LOOPBACK_TYPE_SYSTEM_NOT_ENABLED_IN_HW,
   WUFE_ERR_LINE_INVALID_FPGA,
   WUFE_ERR_INVALID_TUG2_TRANSFER_TYPE,
   WUFE_ERR_ENTRY_NOT_EMPTY,
   WUFE_ERR_VC44C_STM1_STM0_TUG2_TU_MUST_BE_ZERO,
   WUFE_ERR_VC4_STM0_TUG2_TU_MUST_BE_ZERO,
   WUFE_ERR_VC3_TUG2_TU_MUST_BE_ZERO,
   WUFE_ERR_CAS_INVALID_CFG,

   WUFE_ERR_DS3_SUB_SPE_MUST_BE_ZERO,
   WUFE_ERR_INDIRECT_DATA_SET_TIMEOUT,
   WUFE_ERR_INDIRECT_DATA_GET_TIMEOUT,
   WUFE_SFP_ACCESS_ERROR,
   WUFE_ERR_LINE_PDH_LOOPBACK_MAX_NUM_OF_LOOPBACKS,
   WUFE_ERR_LINE_PDH_LINE_AND_SYSTEM_LOOPBACKS_MUST_NOT_BE_ENABLED_SIMULTANEOUSLY,
   WUFE_ERR_LINE_PDH_LOOPBACK_ALREADY_ENABLED,
   /* SBI CLOCK TRANSLATOR ERRORS */
   WUFE_ERR_MAX_SBI_CT,
   WUFE_ERR_SBI_CT_NOT_INITIALIZED,
   WUFE_ERR_SBI_CT_ID_NOT_EXIST,
   WUFE_ERR_SBI_CT_ID_NOT_MATCH_TRIB_TYPE,
   WUFE_ERR_SBI_CT_TRIB_TYPE_NOT_MATCH,
   WUFE_ERR_SBI_CT_LINE_NOT_FOUND,
   WUFE_ERR_SBI_CT_MORE_THAN_ONE_ACTIVE,

   /* CLOCK RECOVERY CONFIGURATION ERRORS */
   WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID,
   WUFE_ERR_CLOCK_REC_NUM_TX_IF_INVALID,
   WUFE_ERR_CLOCK_REC_INVALID_LINE_IN_EXTENDED_MODE,
   WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED,
   WUFE_ERR_CLOCK_REC_NOT_ENABLED,
   WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS,
   WUFE_ERR_CLOCK_REC_LINE_PARAMS,
   WUFE_ERR_CLOCK_REC_NO_MASTER_PHY,
   WUFE_ERR_CLOCK_REC_ACTIVE_SLAVE_PHY,
   WUFE_ERR_CLOCK_REC_METHOD_INVALID,
   WUFE_ERR_CLOCK_REC_INVALID_INFO_TYPE,
   WUFE_ERR_CLOCK_REC_INVALID_CONFIG,
   WUFE_ERR_CLOCK_REC_INVALID_PROTOCOL_CONFIG,
   WUFE_ERR_CLOCK_REC_INVALID_TS_WIDTH,
   WUFE_ERR_CLOCK_REC_INVALID_PHY_ID,
   WUFE_ERR_CLOCK_REC_INVALID_TRANSFER_TYPE,
   WUFE_ERR_CLOCK_REC_INVALID_FRAMING_MODE,
   WUFE_ERR_CLOCK_REC_INVALID_DATA_UNIT_SIZE,
   WUFE_ERR_CLOCK_REC_NO_FREE_RAM_ENTRY,
   WUFE_ERR_CLOCK_REC_CGEN_LINE_NOT_FOUND,
   WUFE_ERR_CLOCK_REC_INVALID_DIRECTION,
   WUFE_ERR_CLOCK_REC_PHYSICAL_CLOCK_ALREADY_ACTIVE,
   WUFE_ERR_CLOCK_REC_INVALID_PHYSICAL_CLOCK_ID,

   /* PHY ERRORS */
   WUFE_ERR_ILLEGAL_NUM_PHYS,
   WUFE_ERR_PHY_INVALID_TYPE,
   WUFE_ERR_INVALID_FIFO_CFG,
   WUFE_ERR_SLOT_CONFIG,
   WUFE_ERR_NO_FREE_PHY,
   WUFE_ERR_ILLEGAL_PHY_ID,
   WUFE_ERR_PHY_CONFIG,
   WUFE_ERR_PHY_NOT_CONNECTED,
   WUFE_ERR_PHY_ACTIVE,
   WUFE_ERR_PHY_DISABLED,
   WUFE_ERR_PHY_NOT_DISABLED,
   WUFE_ERR_PHY_ALREADY_DISABLED,
   WUFE_ERR_PHY_FIFO_NOT_EMPTY,
   WUFE_ERR_PHY_ASSIGNED_ERR,
   WUFE_ERR_PHY_SUM_MAX_COUNT_TOO_BIG,
   WUFE_ERR_FIFO_INCONSIST_FBP_COUNT,
   WUFE_ERR_FIFO_INCONSIST_FBP_PTR,
   WUFE_ERR_PHY_NOT_ATM,
   WUFE_ERR_PHY_ACTIVE_MASTER,
   WUFE_ERR_PHY_INVALID_FPGA,

   WUFE_ERR_DDR_NOT_READY,
   WUFE_ERR_MAX_BUFF_SIZE_NOT_IMPLEMENTED,
   WUFE_ERR_EXT_PLL_WRONG_CONFIG,

   /* TEMX84 ERRORS */
   WUFE_ERR_TEMUX_START,
   WUFE_ERR_TEMUX_1,
   WUFE_ERR_TEMUX_2,
   WUFE_ERR_TEMUX_3,
   WUFE_ERR_TEMUX_4,
   WUFE_ERR_TEMUX_5,
   WUFE_ERR_TEMUX_6,
   WUFE_ERR_TEMUX_7,
   WUFE_ERR_TEMUX_8,
   WUFE_ERR_TEMUX_9,
   WUFE_ERR_TEMUX_10,
   WUFE_ERR_TEMUX_11,
   WUFE_ERR_TEMUX_12,
   WUFE_ERR_TEMUX_13,
   WUFE_ERR_TEMUX_14,
   WUFE_ERR_TEMUX_15,
   WUFE_ERR_TEMUX_16,
   WUFE_ERR_TEMUX_17,
   WUFE_ERR_TEMUX_18,
   WUFE_ERR_TEMUX_19,
   WUFE_ERR_TEMUX_20,
   WUFE_ERR_TEMUX_21,
   WUFE_ERR_TEMUX_22,
   WUFE_ERR_TEMUX_23,
   WUFE_ERR_TEMUX_24,
   WUFE_ERR_TEMUX_25,
   WUFE_ERR_TEMUX_26,
   WUFE_ERR_TEMUX_27,
   WUFE_ERR_TEMUX_28,
   WUFE_ERR_TEMUX_29,
   WUFE_ERR_TEMUX_30,
   WUFE_ERR_TEMUX_31,
   WUFE_ERR_TEMUX_32,
   WUFE_ERR_TEMUX_33,
   WUFE_ERR_TEMUX_34,
   WUFE_ERR_TEMUX_35,
   WUFE_ERR_TEMUX_36,
   WUFE_ERR_TEMUX_37,
   WUFE_ERR_TEMUX_38,
   WUFE_ERR_TEMUX_39,
   WUFE_ERR_PMC_ARROW622_CONFIG,
   WUFE_ERR_PMC_ARROW622_RESET,

   /* FLEXMUX ERRORS */
   WUFE_ERR_FLEXMUX_FAILURE_COMMUNICATION_WITH_FRAMER,
   WUFE_ERR_FLEXMUX_FAILURE_DEVICE_NOT_READY,
   WUFE_ERR_FLEXMUX_FAILURE_BUILD_PERSONALITY,

   /* PLL ERRORS */
   WUFE_ERR_INVALID_PLL_CLOCK_RATE,
};

extern WP_CHAR *WUFE_error_name[];

#endif /*WUFE_ERRORS_H*/
