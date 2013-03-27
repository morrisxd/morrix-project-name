
/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/
#include <stdio.h>
#include <string.h>

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

#ifndef WUFE_UFE_H
#include "wufe_ufe.h"
#endif

#ifndef WUFEI_HW_SBI_H
#include "wufei_hw_clk_rec.h"
#endif

#ifndef WUFEI_HW_CONFIG_H
#include "wufei_hw_config.h"
#endif

#ifndef WUFEI_HW_MEMORY_H
#include "wufei_hw_memory.h"
#endif

#ifndef WUFE_ERRORS_H
#include "wufe_errors.h"
#endif

#ifndef WUFEI_HW_CLK_TRANSLATOR_H
#include "wufei_hw_clk_translator.h"
#endif

#ifndef WUFEI_HW_CLK_REC_H
#include "wufei_hw_clk_rec.h"
#endif

extern WP_U8 debug_flag;

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiClockTranslatorGet
 *
 * Purpose:   read the SBI clock translator register
 *
 *
 * Description:
 *
 * Inputs:
 *     ufeid    - UFE id number
 *     ct_id    - clock translator id
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     clock translator register value
 *
 * Called by:
 *     WUFE_SbiClockTranslatorInit()
 *
 ****************************************************************************/
void WUFEI_HwSbiClockTranslatorGet(WP_U32 ufeid, WP_U16 ct_id, WP_U16* value)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_clk_translator* clk_translator_ptr;
   WP_U16* mode_reg_ptr;
   WUFEI_hw_clk_translator_mode* ct_mode_ptr;

   clk_translator_ptr = &clk_rec_base->clk_translator;
   ct_mode_ptr = &(clk_translator_ptr->mode[ct_id]);
   mode_reg_ptr = &(ct_mode_ptr->mode_reg).value;
   WUFEI_REG_GET(mode_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_SbiClockTranslatorWrite
 *
 * Purpose:   write the SBI clock translator register
 *
 *
 * Description:
 *
 * Inputs:
 *     ufeid          - UFE id number
 *     ct_id           - clock translator id
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     clock translator register value
 *
 * Called by:
 *     WUFE_SbiClockTranslatorInit()
 *
 ****************************************************************************/
void WUFEI_HwSbiClockTranslatorSet(WP_U32 ufeid, WP_U16 ct_id, WP_U16 value)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_clk_translator* clk_translator_ptr;
   WP_U16* mode_reg_ptr;
   WUFEI_hw_clk_translator_mode* ct_mode_ptr;

   clk_translator_ptr = &clk_rec_base->clk_translator;
   ct_mode_ptr = &(clk_translator_ptr->mode[ct_id]);
   mode_reg_ptr = &(ct_mode_ptr->mode_reg).value;
   WUFEI_REG_SET(mode_reg_ptr, value);
}

/*****************************************************************************
 *
 * Function:  
 *
 * Purpose:   
 *
 *
 * Description:
 *
 * Inputs:
 *     ufeid          - UFE id number
 *     ct_id           - clock translator id
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     clock translator register value
 *
 * Called by:
 *     
 *
 ****************************************************************************/
void WUFEI_HwClockRecIndirectCtAddrSet(WP_U32 ufeid, void* indirect_addr)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WP_U16 *indla_ptr, *indha_ptr;

   indla_ptr = &((WUFEI_hw_clk_rec*)clk_rec_base)->indla.value;
   indha_ptr = &((WUFEI_hw_clk_rec*)clk_rec_base)->indha.value;

   WUFEI_REG_INDIRECT_ADDRESS_SET((WP_U32)indha_ptr/WUFEI_ADDR_DRV_TO_HW,
                                  (WP_U32)indla_ptr/WUFEI_ADDR_DRV_TO_HW,
                                  (WP_U32)indirect_addr*WUFEI_INDRCT_ADDR_ABS_TO_DRV,
                                  0, 0);
}

/*****************************************************************************
 *
 * Function:  
 *
 * Purpose:   
 *
 *
 * Description:
 *
 * Inputs:
 *     ufeid          - UFE id number
 *     ct_id           - clock translator id
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     clock translator register value
 *
 * Called by:
 *     
 *
 ****************************************************************************/
void WUFEI_HwClockRecIndirectCtDataSet(WP_U32 ufeid, WP_U16 value)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WP_U16 *inddat_ptr;

   inddat_ptr = &((WUFEI_hw_clk_rec*)clk_rec_base)->inddat.value;

   WUFEI_REG_SET(inddat_ptr, value);
}

/*****************************************************************************
 *
 * Function:  
 *
 * Purpose:   
 *
 *
 * Description:
 *
 * Inputs:
 *     ufeid          - UFE id number
 *     ct_id           - clock translator id
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     clock translator register value
 *
 * Called by:
 *     
 *
 ****************************************************************************/
void WUFEI_HwClockRecIndirectCtDataGet(WP_U32 ufeid, WP_U16* value)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WP_U16 *inddat_ptr;

   inddat_ptr = &((WUFEI_hw_clk_rec*)clk_rec_base)->inddat.value;

   WUFEI_REG_GET(inddat_ptr, value);
}

/*****************************************************************************
 *
 * Function:  
 *
 * Purpose:   
 *
 *
 * Description:
 *
 * Inputs:
 *     ufeid          - UFE id number
 *     ct_id           - clock translator id
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     clock translator register value
 *
 * Called by:
 *     
 *
 ****************************************************************************/
void WUFEI_HwSbiClockTranslatorIndirectGet(WP_U32 ufeid, WP_U16 trib_id, WP_U16* value)
{
   WUFEI_hw_clk_rec_ct_tx_ram *ct_tx_ram_ptr;

   ct_tx_ram_ptr = WUFEI_INDIR_CLK_TRANS_RAM(trib_id);
   WUFEI_HwClockRecIndirectCtAddrSet(ufeid, &(ct_tx_ram_ptr->ct_tx));
   WUFEI_HwClockRecIndirectCtDataGet(ufeid, value);

}

/*****************************************************************************
 *
 * Function:  
 *
 * Purpose:   
 *
 *
 * Description:
 *
 * Inputs:
 *     ufeid          - UFE id number
 *     ct_id           - clock translator id
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     clock translator register value
 *
 * Called by:
 *     
 *
 ****************************************************************************/
void WUFEI_HwSbiClockTranslatorIndirectSet(WP_U32 ufeid, WP_U16 trib_id, WP_U16 value)
{
   WUFEI_hw_clk_rec_ct_tx_ram *ct_tx_ram_ptr;

   ct_tx_ram_ptr = WUFEI_INDIR_CLK_TRANS_RAM(trib_id);
   
   WUFEI_HwClockRecIndirectCtAddrSet(ufeid, &(ct_tx_ram_ptr->ct_tx));
   WUFEI_HwClockRecIndirectCtDataSet(ufeid, value);   
}
   
/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiClockTranslatorAddrGet
 *
 * Purpose:   read the SBI clock translator register
 *
 *
 * Description:
 *
 * Inputs:
 *     ufeid    - UFE id number
 *     ct_id    - clock translator id
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     clock translator register value
 *
 * Called by:
 *     WUFE_SbiClockTranslatorInit()
 *
 ****************************************************************************/
WP_U32 WUFEI_HwSbiClockTranslatorAddrGet(WP_U32 ufeid, WP_U16 ct_id)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_clk_translator* clk_translator_ptr;
   WP_U16* mode_reg_ptr;
   WUFEI_hw_clk_translator_mode* ct_mode_ptr;

   clk_translator_ptr = &clk_rec_base->clk_translator;
   ct_mode_ptr = &(clk_translator_ptr->mode[ct_id]);
   mode_reg_ptr = &(ct_mode_ptr->mode_reg).value;

   return (WP_U32)mode_reg_ptr;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiClockTranslatorEventGet
 *
 * Purpose:   read the SBI clock translator event register
 *
 *
 * Description:
 *
 * Inputs:
 *     ufeid    - UFE id number
 *     ct_reg_arr
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     clock translator register value
 *
 * Called by:
 *     WUFEI_SbiClockTranslatorEvents()
 *
 ****************************************************************************/
void WUFEI_HwSbiClockTranslatorEventGet(WP_U32 ufeid, WP_U16* ct_reg_arr)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_clk_translator* clk_translator_ptr;
   WP_U16* event_reg_ptr, i;

   for(i=0; i<WUFEI_HW_CLK_TRANSLATOR_MAX_N_EVENTS; i++)
   {
      clk_translator_ptr = &clk_rec_base->clk_translator;
      event_reg_ptr = &(clk_translator_ptr->event_reg[i]).value;

      WUFEI_REG_GET(event_reg_ptr, &ct_reg_arr[i]);
   }

}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiClockTranslatorEventSet
 *
 * Purpose:   read the SBI clock translator event register
 *
 *
 * Description:
 *
 * Inputs:
 *     ufeid    - UFE id number
 *     ct_reg_arr
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     clock translator register value
 *
 * Called by:
 *     WUFEI_SbiClockTranslatorEvents()
 *
 ****************************************************************************/
void WUFEI_HwSbiClockTranslatorEventSet(WP_U32 ufeid, WP_U16* ct_reg_arr)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_clk_translator* clk_translator_ptr;
   WP_U16* event_reg_ptr, i;

   for(i=0; i<WUFEI_HW_CLK_TRANSLATOR_MAX_N_EVENTS; i++)
   {
      clk_translator_ptr = &clk_rec_base->clk_translator;
      event_reg_ptr = &(clk_translator_ptr->event_reg[i]).value;

      WUFEI_REG_SET(event_reg_ptr, ct_reg_arr[i]);
   }

}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiClockTranslatorEventmMaskGet
 *
 * Purpose:   read the SBI clock translator event mask register
 *
 *
 * Description:
 *
 * Inputs:
 *     ufeid    - UFE id number
 *     ct_reg_arr
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     clock translator register value
 *
 * Called by:
 *     WUFEI_SbiClockTranslatorEvents()
 *
 ****************************************************************************/
void WUFEI_HwSbiClockTranslatorEventMaskGet(WP_U32 ufeid, WP_U16* ct_reg_arr)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_clk_translator* clk_translator_ptr;
   WP_U16* event_mask_reg_ptr, i;

   for(i=0; i<WUFEI_HW_CLK_TRANSLATOR_MAX_N_EVENTS; i++)
   {
      clk_translator_ptr = &clk_rec_base->clk_translator;
      event_mask_reg_ptr = &(clk_translator_ptr->event_mask_reg[i]).value;

      WUFEI_REG_GET(event_mask_reg_ptr, &ct_reg_arr[i]);
   }

}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiClockTranslatorEventmMaskSet
 *
 * Purpose:   read the SBI clock translator event mask register
 *
 *
 * Description:
 *
 * Inputs:
 *     ufeid    - UFE id number
 *     ct_reg_arr
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     clock translator register value
 *
 * Called by:
 *     WUFEI_SbiClockTranslatorEvents()
 *
 ****************************************************************************/
void WUFEI_HwSbiClockTranslatorEventMaskSet(WP_U32 ufeid, WP_U16* ct_reg_arr)
{
   WUFEI_hw_clk_rec *clk_rec_base = (WUFEI_hw_clk_rec *)WUFEI_UfeClkRecBaseAddrGet(ufeid);
   WUFEI_hw_clk_rec_clk_translator* clk_translator_ptr;
   WP_U16* event_mask_reg_ptr, i;

   for(i=0; i<WUFEI_HW_CLK_TRANSLATOR_MAX_N_EVENTS; i++)
   {
      clk_translator_ptr = &clk_rec_base->clk_translator;
      event_mask_reg_ptr = &(clk_translator_ptr->event_mask_reg[i]).value;

      WUFEI_REG_SET(event_mask_reg_ptr, ct_reg_arr[i]);
   }

}

/*****************************************************************************
 *
 * Function:  WUFEI_HwClockTranslatorWrite
 *
 * Purpose:   write the clock translator register
 *
 *
 * Description:
 *     This function checks if the specific clock translator is valid and
 *     initializes its register.
 *
 * Inputs:
 *     wufe_id          - UFE id number
 *     ct_id            - Clock Translator id
 *     tributary_type   - Line tributary type
 *     framed_mode      - Line framing type
 *     tributary_id     - Line tributary id
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     none
 *
 * Called by:
 *
 ****************************************************************************/
void WUFEI_HwClockTranslatorWrite(WP_U32 wufe_id,
                                  WP_U32 ct_id,
                                  WP_U32 adap_ct_id,
                                  WP_U32 diff_ct_id,
                                  WP_U16 tributary_type,
                                  WP_U16 framed_mode,
                                  WP_U16 tributary_id)
{
   WP_U16 value, ct_tx_ram = 0x0;

   value = WUFEI_HW_SBI_TX_CT_TRIB_TYPE_F(tributary_type) |
      WUFEI_HW_SBI_TX_CT_TRIB_FRAMED_F(framed_mode) |
      WUFEI_HW_SBI_TX_CT_ENABLE_M;

   WUFEI_HwSbiClockTranslatorSet(wufe_id, ct_id, value);

   /* in case one of the CT are N/A, disable the Valid bit and write zeros to CT id */
   if (adap_ct_id == WUFEI_SBI_CT_NON_ACTIVE_INSTANCE)
   {
      ct_tx_ram = WUFEI_HW_SBI_TX_CT_UNIT_ID_F(diff_ct_id) |
         WUFEI_HW_SBI_TX_CT_DIFF_VALID_M;
      ct_tx_ram &= ~WUFEI_HW_SBI_TX_CT_ADAP_VALID_M;
   }
   if (diff_ct_id == WUFEI_SBI_CT_NON_ACTIVE_INSTANCE)
   {
      ct_tx_ram = WUFEI_HW_SBI_TX_CT_UNIT_ID_F(adap_ct_id) |
         WUFEI_HW_SBI_TX_CT_ADAP_VALID_M;
      ct_tx_ram &= ~WUFEI_HW_SBI_TX_CT_DIFF_VALID_M;
   }

   WUFEI_HwSbiClockTranslatorIndirectSet(wufe_id, tributary_id, ct_tx_ram);
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiClockTranslatorTribIdGet
 *
 * Purpose:   build tributary_id field
 *
 *
 * Description:
 *
 * Inputs:
 *     core  - tributary core id
 *     stm   - tributary stm id
 *     spe   - tributary spe id
 *     link  - tributary sub_spe id
 *
 * Outputs:
 *     tributary_id
 *
 * Return Value:
 *     none
 *
 * Called by:
 *
 ****************************************************************************/
WP_U16 WUFEI_HwSbiClockTranslatorTribIdGet(WP_U8 core, WP_U8 stm, WP_U8 spe, WP_U8 link)
{
   WP_U32 value;

   value = WUFEI_HW_SBI_TX_CT_TRIB_ID_CORE_F(core) |
      WUFEI_HW_SBI_TX_CT_TRIB_ID_STM_F(stm) |
      WUFEI_HW_SBI_TX_CT_TRIB_ID_SPE_F(spe) |
      WUFEI_HW_SBI_TX_CT_TRIB_ID_LINK_F(link);

   value &= WUFEI_HW_SBI_TX_CT_TRIB_ID_V(WUFEI_HW_SBI_TX_CT_TRIB_ID_M);

   return ((WP_U16)value);
}


/*****************************************************************************
 *
 * Function:  WUFEI_SbiClockTranslatorTribUpdate
 *
 * Purpose:   writes new tributary id in the CT register
 *
 *
 * Description:
 *
 * Inputs:
 *     ufeid          - UFE id number
 *     ct_id            - Clock translator id
 *     tributary_id     - Line tributary id
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     none
 *
 * Called by:
 *
 ****************************************************************************/
void WUFEI_HwSbiClockTranslatorTribUpdate(WP_U32 ufeid,
                                          WP_U32 ct_id,
                                          WP_U16 tributary_id)
{
   WP_U16 value;

   /* Set Clock Translator entry in RAM */
   value = WUFEI_HW_SBI_TX_CT_UNIT_ID_F(ct_id) |
      WUFEI_HW_SBI_TX_CT_ADAP_VALID_M;
   WUFEI_HwSbiClockTranslatorIndirectSet(ufeid, tributary_id, value);
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiClockTranslatorFindActive
 *
 * Purpose:   Find the number of active entries inside the CT RAM for a given ct_id
 *
 *
 * Description:
 *
 * Inputs:
 *     clock_translator_id   - Clock translator id
 *     *number_of_active     - pointer to number of active entries counter
 *
 * Outputs:
 *     none
 *
 * Return Value:
 *     none
 *
 * Called by: WUFEI_SbiClockTranslatorValidate
 *
 ****************************************************************************/
void WUFEI_HwSbiClockTranslatorFindActive(WP_U32 wufe_id, WP_U16 clock_translator_id,
                                          WP_U16 *number_of_active)
{
   WP_U32 i, n_core;
   WP_U16 ct_trib_id, ext_ct_id;

   *number_of_active = 0;

   if (WUFEI_UfeCoreBandwidthGet(wufe_id) == 0)
      n_core = 2;
   else
      n_core = 1;
   for (i=0; i<(n_core*256); i++)
   {
      WUFEI_HwSbiClockTranslatorIndirectGet(wufe_id, i, &ct_trib_id);
      if (clock_translator_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
      {
         if ((WUFEI_HW_SBI_TX_CT_UNIT_ID_V(ct_trib_id) == clock_translator_id) &&
             (WUFEI_HW_SBI_TX_CT_ADAP_VALID_V(ct_trib_id)))
            (*number_of_active)++;
      }
      else
      {
         ext_ct_id = clock_translator_id - WUFE_CLOCK_REC_REGULAR_INSTANCES;
         if ((WUFEI_HW_SBI_TX_CT_UNIT_ID_V(ct_trib_id) == ext_ct_id) &&
             (WUFEI_HW_SBI_TX_CT_DIFF_VALID_V(ct_trib_id)))
            (*number_of_active)++;
      }
   }
   return;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwSbiClockTranslatorInfoGet
 *
 * Purpose:   Extract spesific entry from CT RAM for a given ct_id
 *
 *
 * Description:
 *
 * Inputs:
 *     wufe_id          - UFE id number
 *     ct_id            - Clock Translator id
 *     line_trib_id     - Line tributary id (core-stm-spe-subspe representation)
 *
 * Outputs:
 *     ct_trib_id       - pointer to CT tributary id info
 *     enable           - pointer to CT enable bit
 *
 * Return Value:
 *     none
 *
 * Called by: WUFEI_SbiClockTranslatorRelease
 *
 ****************************************************************************/
void WUFEI_HwSbiClockTranslatorInfoGet(WP_U32 wufe_id,
                                       WP_U16 ct_id,
                                       WP_U16 line_trib_id,
                                       WP_U16 *ct_trib_id,
                                       WP_U16 *enable)
{
   WP_U16 ct_reg = 0;

   /* Read the Tx clock translator register according to the ct id */
   WUFEI_HwSbiClockTranslatorIndirectGet(wufe_id, line_trib_id, &ct_reg);
   if (ct_id < WUFE_CLOCK_REC_REGULAR_INSTANCES)
   {
      *ct_trib_id = WUFEI_HW_SBI_TX_CT_UNIT_ID_V(ct_reg);
      *enable = WUFEI_HW_SBI_TX_CT_ADAP_VALID_V(ct_reg);
   }
   else
   {
      *ct_trib_id = WUFEI_HW_SBI_TX_CT_UNIT_ID_V(ct_reg);
      *enable = WUFEI_HW_SBI_TX_CT_DIFF_VALID_V(ct_reg);
   }
   return;
}
      
/*****************************************************************************
 *
 * Function:  WUFEI_HwClockRecCtRamEntryRead
 *
 * Purpose:   Display information about indirect SBI clock translator register
 *
 *
 * Description:
 *
 * Inputs:
 *     wufe_id          - UFE id number
 *     ct_ram           - Clock Translator RAM value
 *     entry            - Clock Translator RAM entry
 *
 * Outputs:
 *
 * Return Value:
 *     none
 *
 * Called by: WUFE_ClockRecCtRamEntryRead
 *
 ****************************************************************************/
void WUFEI_HwClockRecCtRamEntryRead(WP_U32 wufe_id, WP_U16 *ct_ram, WP_U16 entry)
{
   WUFEI_hw_clk_rec_ct_tx_ram *ct_ram_ptr;

   /* read the Rx clock translator  register according to the line id */
   ct_ram_ptr = WUFEI_INDIR_CLK_TRANS_RAM(entry);
   WUFEI_HwClockRecIndirectCtAddrSet(wufe_id, &(ct_ram_ptr->ct_tx));
   WUFEI_HwClockRecIndirectCtDataSet(wufe_id, *ct_ram);
   WUFE_PRINT_DISP((wufei_log,"entry: %d   clock translator data: 0x%x\n",entry,*ct_ram));
   return;
}
