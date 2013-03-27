/*****************************************************************************
 * (C) Copyright 2001-2010, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/************************************************************************/
/*                      TDM CARD Modes of Operation                     */
/************************************************************************/

#ifndef WPX_TDM_MODES_H
#define WPX_TDM_MODES_H

/* Defined modes of operation */
#define WPU_MODE_TDM_CLOCK_8MHZ   0
#define WPU_MODE_TDM_CLOCK_16MHZ  1
#define WPU_MODE_TDM_E1           2
#define WPU_MODE_TDM_E1_S         3
#define WPU_MODE_TDM_E1_X         4
#define WPU_MODE_TDM_E1_MASTER    5
#define WPU_MODE_TDM_E1_SLAVE     6
#define WPU_MODE_TDM_T1           7
#define WPU_MODE_TDM_T1_S         8
#define WPU_MODE_TDM_T1_MASTER    9
#define WPU_MODE_TDM_T1_SLAVE     10

/* Defined modes of operation for Transparent protocol  */
#define WPU_MODE_TDM_TRANS_E1_MASTER            11
#define WPU_MODE_TDM_TRANS_E1_SLAVE             12
#define WPU_MODE_TDM_TRANS_E1_FRAMELESS_MASTER  13
#define WPU_MODE_TDM_TRANS_E1_FRAMELESS_SLAVE   14
#define WPU_MODE_TDM_TRANS_E1_CAS_MASTER        15
#define WPU_MODE_TDM_TRANS_E1_CAS_SLAVE         16
#define WPU_MODE_TDM_TRANS_T1_MASTER            17
#define WPU_MODE_TDM_TRANS_T1_SLAVE             18
#define WPU_MODE_TDM_TRANS_T1_FRAMELESS_MASTER  19
#define WPU_MODE_TDM_TRANS_T1_FRAMELESS_SLAVE   20
#define WPU_MODE_TDM_TRANS_T1_CAS_MASTER        21
#define WPU_MODE_TDM_TRANS_T1_CAS_SLAVE         22

#define WPU_MODE_TDM_UFE_E1_FRAMED              23
#define WPU_MODE_TDM_UFE_E1_UNFRAMED            24
#define WPU_MODE_TDM_UFE_T1_FRAMED              25
#define WPU_MODE_TDM_UFE_T1_UNFRAMED            26

#define WPU_MODE_TDM_UFE_E1_FRAMED_EXT_CLK      27
#define WPU_MODE_TDM_UFE_E1_UNFRAMED_EXT_CLK    28
#define WPU_MODE_TDM_UFE_T1_FRAMED_EXT_TX_CLK   29
#define WPU_MODE_TDM_UFE_T1_UNFRAMED_EXT_CLK    30

#define WPU_MODE_TDM_UFE_E1_SLAVE_CAS           31
#define WPU_MODE_TDM_UFE_TRANS_T1_CAS_MASTER    32
#define WPU_MODE_TDM_UFE_TRANS_T1_CAS_SLAVE     33

#define WPU_MODE_TDM_TRANS_E1_MASTER_4M         34
#define WPU_MODE_TDM_TRANS_E1_MASTER_8M         35

#define WPU_MODE_TDM_E1_MASTER_UNFRAMED         36
#define WPU_MODE_TDM_E1_SLAVE_UNFRAMED          37
#define WPU_MODE_TDM_T1_MASTER_UNFRAMED         38
#define WPU_MODE_TDM_T1_SLAVE_UNFRAMED          39

#define WPU_MODE_TDM_T1_CR_MASTER               40
#define WPU_MODE_TDM_T1_CR_SLAVE                41
#define WPU_MODE_TDM_E1_CR_MASTER               42
#define WPU_MODE_TDM_E1_CR_SLAVE                43
#define WPU_MODE_TDM_E1_CR_CAS_MASTER           44
#define WPU_MODE_TDM_T1_CR_CAS_MASTER           45

#define WPU_MODE_TDM_T1_MASTER_UNFRAMED_PWE3_CES_CR  46
#define WPU_MODE_TDM_T1_SLAVE_UNFRAMED_PWE3_CES_CR   47
#define WPU_MODE_TDM_T1_MASTER_PWE3_CES_CR           48
#define WPU_MODE_TDM_T1_SLAVE_PWE3_CES_CR            49
#define WPU_MODE_TDM_T1_CR_MASTER_PWE3_CES_CR        50
#define WPU_MODE_TDM_T1_CR_SLAVE_PWE3_CES_CR         51
#define WPU_MODE_TDM_T1_CR_CAS_MASTER_PWE3_CES_CR    52
#define WPU_MODE_TDM_E1_MASTER_UNFRAMED_PWE3_CES_CR  53
#define WPU_MODE_TDM_E1_SLAVE_UNFRAMED_PWE3_CES_CR   54
#define WPU_MODE_TDM_E1_MASTER_PWE3_CES_CR           55
#define WPU_MODE_TDM_E1_SLAVE_PWE3_CES_CR            56
#define WPU_MODE_TDM_E1_CR_MASTER_PWE3_CES_CR        57
#define WPU_MODE_TDM_E1_CR_SLAVE_PWE3_CES_CR         58
#define WPU_MODE_TDM_E1_CR_CAS_MASTER_PWE3_CES_CR    59
#define WPU_MODE_TDM_TRANS_T1_CAS_MASTER_PWE3_CES_CR 60
#define WPU_MODE_TDM_TRANS_T1_CAS_SLAVE_PWE3_CES_CR  61
#define WPU_MODE_TDM_TRANS_E1_CAS_MASTER_PWE3_CES_CR 62
#define WPU_MODE_TDM_TRANS_E1_CAS_SLAVE_PWE3_CES_CR  63

#endif
