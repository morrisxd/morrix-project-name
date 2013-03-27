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

#ifndef WUFEI_HW_CAD_H
#include "wufei_hw_cad.h"
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


/*****************************************************************************
 *
 * Function:  WUFEI_HwCadModeSet
 *
 * Purpose:   Sets the mode register for a specific cad
 *
 *
 * Description:
 *     This function sets the mode register of the cad specified.
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     cad_id - id of the cad to set.
 *     value - the value to set in the cad mode register.
 *
 * Return Value:
 *     None
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/

void WUFEI_HwCadModeSet(WP_U32 ufeid, WP_U32 cad_id, WP_U16 value)
{
   WUFEI_hw_cad_block* cad_ptr;
   WP_U16 *mode_ptr;
   WP_U32 cad_base,core_id;

   core_id = WUFEI_CAD_CORE_ID_GET(cad_id);
   cad_base = WUFEI_UfeLiuBaseAddrGet(ufeid);
   cad_id  = cad_id % (WUFE_MAX_N_CAD/2);

   cad_ptr = &((WUFEI_hw_cad*)cad_base)->core[core_id].cad[cad_id];
   mode_ptr =  &((cad_ptr->mode).value);

   WUFEI_REG_SET(mode_ptr, value);

}

/*****************************************************************************
 *
 * Function:  WUFEI_HwCadIdSet
 *
 * Purpose:   Sets the Id register for a specific cad
 *
 *
 * Description:
 *     This function sets the Id register of the cad specified.
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     cad_id - id of the cad to set.
 *     value - the value to set in the cad Id register.
 *
 * Return Value:
 *     None
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/

void WUFEI_HwCadIdSet(WP_U32 ufeid, WP_U32 cad_id, WP_U16 value)
{
   WUFEI_hw_cad_block* cad_ptr;
   WP_U16 *id_ptr;
   WP_U32 cad_base,core_id;

   core_id = WUFEI_CAD_CORE_ID_GET(cad_id);
   cad_base = WUFEI_UfeLiuBaseAddrGet(ufeid);
   cad_id  = cad_id % (WUFE_MAX_N_CAD/2);

   cad_ptr = &((WUFEI_hw_cad*)cad_base)->core[core_id].cad[cad_id];

   id_ptr =  &((cad_ptr->id).value);

   WUFEI_REG_SET(id_ptr, value);

}

/*****************************************************************************
 *
 * Function:  WUFEI_HwCadIntSet
 *
 * Purpose:   Sets the interrupt register for several cads
 *
 *
 * Description:
 *     This function sets the interrupt register from the cads specified.
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     direction - rx or tx.
 *     cad_id - the cad id.
 *     value - the value to be set.
 *
 *   rx_int[0]: Cad 0-7 Ingress (RX) interrupt register
 *   rx_int[1]: Cad 8-15 Ingress (RX) interrupt register
 *   rx_int[2]: Cad 16-23 Ingress (RX) interrupt register
 *   rx_int[3]: Cad 24-31 Ingress (RX) interrupt register
 *   tx_int[0]: Cad 0-7 Egress (TX) interrupt register
 *   tx_int[1]: Cad 8-15 Egress (TX) interrupt register
 *   tx_int[2]: Cad 16-23 Egress (TX) interrupt register
 *   tx_int[3]: Cad 24-31 Egress (TX) interrupt register
 *
 * Return Value:
 *     None.
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
void WUFEI_HwCadIntSet(WP_U32 ufeid, WUFEI_cad_direction direction, WP_U32 cad_id, WP_U16 value)
{
   WUFEI_hw_cad_events* cad_ptr;
   WP_U16 *int_ptr;
   WP_U32 cad_base,core_id;

   core_id = WUFEI_CAD_CORE_ID_GET(cad_id);
   cad_id  = cad_id % (WUFE_MAX_N_CAD/2);
   cad_base = WUFEI_UfeLiuBaseAddrGet(ufeid);

   cad_ptr = &((WUFEI_hw_cad*)cad_base)->core[core_id].cad_events;

   if (direction == WUFEI_CAD_RX)
      int_ptr =  &((cad_ptr->rx_int[cad_id/8]).value);
   else
      int_ptr =  &((cad_ptr->tx_int[cad_id/8]).value);

   WUFEI_REG_SET(int_ptr, value);

}


/*****************************************************************************
 *
 * Function:  WUFEI_HwCadModeGet
 *
 * Purpose:   Gets the mode register from a specific cad
 *
 *
 * Description:
 *     This function gets the mode register from the cad specified.
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     cad_id - id of the cad to get.
 *
 * Return Value:
 *     The value in the mode register of that cad.
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WP_U32 WUFEI_HwCadModeGet(WP_U32 ufeid, WP_U32 cad_id)
{
   WUFEI_hw_cad_block* cad_ptr;
   WP_U16 *mode_ptr;
   WP_U32 cad_base,core_id;
   WP_U16 value;

   core_id = WUFEI_CAD_CORE_ID_GET(cad_id);
   cad_base = WUFEI_UfeLiuBaseAddrGet(ufeid);
   cad_id  = cad_id % (WUFE_MAX_N_CAD/2);
   cad_ptr = &((WUFEI_hw_cad*)cad_base)->core[core_id].cad[cad_id];

   mode_ptr =  &((cad_ptr->mode).value);

   WUFEI_REG_GET(mode_ptr, &value);

   return value;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwCadIntGet
 *
 * Purpose:   Gets the interrupt register for several cads
 *
 *
 * Description:
 *     This function gets the interrupt register from the cads specified.
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     direction - rx or tx.
 *     cad_id - the cad id.
 *
 *   rx_int[0]: Cad 0-7 Ingress (RX) interrupt register
 *   rx_int[1]: Cad 8-15 Ingress (RX) interrupt register
 *   rx_int[2]: Cad 16-23 Ingress (RX) interrupt register
 *   rx_int[3]: Cad 24-31 Ingress (RX) interrupt register
 *   tx_int[0]: Cad 0-7 Egress (TX) interrupt register
 *   tx_int[1]: Cad 8-15 Egress (TX) interrupt register
 *   tx_int[2]: Cad 16-23 Egress (TX) interrupt register
 *   tx_int[3]: Cad 24-31 Egress (TX) interrupt register
 *
 * Return Value:
 *     The value in the interrupt register required.
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WP_U32 WUFEI_HwCadIntGet(WP_U32 ufeid, WUFEI_cad_direction direction, WP_U32 cad_id)
{
   WUFEI_hw_cad_events* cad_ptr;
   WP_U16 *int_ptr;
   WP_U32 cad_base,core_id;
   WP_U16 value;

   core_id = WUFEI_CAD_CORE_ID_GET(cad_id);
   cad_id = cad_id % (WUFE_MAX_N_CAD/2);
   cad_base = WUFEI_UfeLiuBaseAddrGet(ufeid);

   cad_ptr = &((WUFEI_hw_cad*)cad_base)->core[core_id].cad_events;

   if (direction == WUFEI_CAD_RX)
      int_ptr =  &((cad_ptr->rx_int[cad_id/8]).value);
   else
      int_ptr =  &((cad_ptr->tx_int[cad_id/8]).value);

   WUFEI_REG_GET(int_ptr, &value);

   return value;
}


/*****************************************************************************
 *
 * Function:  WUFEI_HwCadIdGet
 *
 * Purpose:   Gets the Id register from a specific cad
 *
 *
 * Description:
 *     This function gets the Id register from the cad specified.
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     cad_id - id of the cad to get.
 *
 * Return Value:
 *     The value in the Id register of that cad.
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WP_U32 WUFEI_HwCadIdGet(WP_U32 ufeid, WP_U32 cad_id)
{
   WUFEI_hw_cad_block* cad_ptr;
   WP_U16 *id_ptr;
   WP_U32 cad_base,core_id;
   WP_U16 value;

   core_id = WUFEI_CAD_CORE_ID_GET(cad_id);
   cad_base = WUFEI_UfeLiuBaseAddrGet(ufeid);
   cad_id  = cad_id % (WUFE_MAX_N_CAD/2);
   cad_ptr = &((WUFEI_hw_cad*)cad_base)->core[core_id].cad[cad_id];

   id_ptr =  &((cad_ptr->id).value);

   WUFEI_REG_GET(id_ptr, &value);

   return value;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwCadGlobalEventGet
 *
 * Purpose:   Gets the global interrupt register
 *
 *
 * Description:
 *     This function gets the global interrupt register.
 *
 * Inputs:
 *     ufeid - the ufe id.
 *
 * Return Value:
 *     The value in the global interrupt register.
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WP_U32 WUFEI_HwCadGlobalEventGet(WP_U32 ufeid)
{

   WP_U16 *global_event_ptr;
   WP_U32 cad_base;
   WP_U16 value;

   cad_base = WUFEI_UfeLiuBaseAddrGet(ufeid);

   global_event_ptr = &((WUFEI_hw_cad*)cad_base)->global_event_reg.value;

   WUFEI_REG_GET(global_event_ptr, &value);

   return value;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwCadGlobalEventMaskGet
 *
 * Purpose:   Gets the global interrupt register
 *
 *
 * Description:
 *     This function gets the global interrupt register.
 *
 * Inputs:
 *     ufeid - the ufe id.
 *
 * Return Value:
 *     The value in the global interrupt register.
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
WP_U32 WUFEI_HwCadGlobalEventMaskGet(WP_U32 ufeid)
{

   WP_U16 *global_mask_ptr;
   WP_U32 cad_base;
   WP_U16 value;

   cad_base = WUFEI_UfeLiuBaseAddrGet(ufeid);

   global_mask_ptr = &((WUFEI_hw_cad*)cad_base)->global_event_mask_reg.value;

   WUFEI_REG_GET(global_mask_ptr, &value);

   return value;
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwCadGlobalEventSet
 *
 * Purpose:   Sets the global Interrupt register.
 *
 *
 * Description:
 *     This function sets the global interrupt register.
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     value - the value to set in the cad interrupt register.
 *
 * Return Value:
 *     None
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
void WUFEI_HwCadGlobalEventSet(WP_U32 ufeid, WP_U16 value)
{
   WP_U16 *global_event_ptr;
   WP_U32 cad_base;

   cad_base = WUFEI_UfeLiuBaseAddrGet(ufeid);

   global_event_ptr = &((WUFEI_hw_cad*)cad_base)->global_event_reg.value;

   WUFEI_REG_SET(global_event_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwCadGlobalEventMaskSet
 *
 * Purpose:   Sets the global Interrupt mask register.
 *
 *
 * Description:
 *     This function sets the global interrupt mask register.
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     value - the value to set in the cad interrupt mask register.
 *
 * Return Value:
 *     None
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
void WUFEI_HwCadGlobalEventMaskSet(WP_U32 ufeid, WP_U16 value)
{
   WP_U16 *global_mask_ptr;
   WP_U32 cad_base;

   cad_base = WUFEI_UfeLiuBaseAddrGet(ufeid);

   global_mask_ptr = &((WUFEI_hw_cad*)cad_base)->global_event_mask_reg.value;

   WUFEI_REG_SET(global_mask_ptr, value);
}

/*****************************************************************************
 *
 * Function:  WUFEI_HwCadSetup
 *
 * Purpose:   Setup all Cads
 *
 * Description:
 *     This function clears all the CAD HW registers.
 *
 * Inputs:
 *     ufeid - the ufe id.
 *     max_n_cad - The amount of CADs to setup.
 *
 * Return Value:
 *     Upon Successful completion a WUFE_OK is returned.
 *
 * Called by:
 *     WUFEI_LineSystemSetup (in wufei_line.c)
 *
 ****************************************************************************/
WUFE_status WUFEI_HwCadSetup(WP_U32 ufeid, WP_U32 max_n_cad)
{
   WP_U32 cad_id;

   /* Cleares the CAD HW registers*/
   for(cad_id = 0; cad_id < max_n_cad; cad_id++ )
   {
      WUFEI_HwCadModeSet(ufeid, cad_id, 0);
      WUFEI_HwCadIdSet(ufeid, cad_id, 0);
   }

   return WUFE_OK;

}

/*****************************************************************************
 *
 * Function:  WUFEI_HwCadCheckAndConfig
 *
 * Purpose:   check the configuration of lines type CAD
 *
 *
 * Description:
 *
 *
 *
 * Inputs:
 *     rx_cad_mod
 *     line_cfg           The user configuration
 *     line_registry      Configs the the field  hmvip_num according to line_cfg
 *
 * Outputs:
 *     rx_cad_mod        the lines mode registers (CAD)
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFEI_LineCadConfig function.
 *
 ****************************************************************************/
WUFE_status WUFEI_HwCadCheckAndConfig(WP_U16               *rx_cad_mod,
                                      WUFE_line_cad        *line_cfg,
                                      WUFEI_line_registry  *line_registry)
{
   /**************************************/
   /****** Check the configuration    ****/
   /**************************************/
   if (line_cfg->framed == WUFE_UNFRAMED)
   {/*UNFRAMED*/
      if(line_cfg->transfer_type != WUFE_CAD_TYPE_HMVIP_4_T1)
      {
         if(line_cfg->transfer_type != WUFE_CAD_TYPE_HMVIP_4_E1)
         {
            *rx_cad_mod |= (WUFEI_CAD_MODE_UN_SLOT_F(WUFEI_UNSLOTED) |
                            WUFEI_CAD_MODE_UN_FRAME_MODE_F(WUFE_UNFRAMED));
         }
         else
         {
            /* if the trnasfer type is WUFE_CAD_TYPE_HMVIP_4_E1
               than treat as framed with 32 slots*/
            *rx_cad_mod |= (WUFEI_CAD_MODE_UN_SLOT_F(WUFEI_SLOTED) |
                            WUFEI_CAD_MODE_UN_FRAME_MODE_F(WUFE_FRAMED));
         }

      }
      else
         return WUFE_ERR_HMVIP_T1_MUST_BE_FRAMED;

   }
   else
   {/*FRAMED*/
      if(line_cfg->transfer_type != WUFE_CAD_TYPE_E3 &&
         line_cfg->transfer_type != WUFE_CAD_TYPE_DS3)
      {
         *rx_cad_mod |= (WUFEI_CAD_MODE_UN_SLOT_F(WUFEI_SLOTED) |
                         WUFEI_CAD_MODE_UN_FRAME_MODE_F(WUFE_FRAMED));
      }
      else
         return WUFE_ERR_DS3_T3_MUST_BE_UNFRAMED;

   }

   if (line_cfg->cad_loopback == WUFE_CAD_LOOPBACK_SYSTEM)
      *rx_cad_mod |=WUFEI_CAD_MODE_SYS_LB_F(1);
   if (line_cfg->cad_loopback == WUFE_CAD_LOOPBACK_LINE)
      *rx_cad_mod |=WUFEI_CAD_MODE_LINE_LB_F(1);

   if ((line_cfg->framed                 > 1)||
       (line_cfg->multi_frame            > 1)||
       (line_cfg->multi_frame_sync_level > 1)||
       (line_cfg->multi_frame_num        > 1)||
       (line_cfg->cas_enable             > 1)||
       (line_cfg->cas_alignment_location > 1)||
       (line_cfg->transfer_type          > WUFE_CAD_TYPE_DS3))
   {
      WUFE_PRINT_ERR(( wufei_log,"framed %d > 1\n", line_cfg->framed));
      WUFE_PRINT_ERR(( wufei_log,"multi_frame %d > 1\n", line_cfg->multi_frame));
      WUFE_PRINT_ERR(( wufei_log,"multi_frame_sync_level %d > 1\n",
                       line_cfg->multi_frame_sync_level));
      WUFE_PRINT_ERR(( wufei_log,"multi_frame_num %d > 1\n", line_cfg->multi_frame_num));
      WUFE_PRINT_ERR(( wufei_log,"cas_enable %d > 1\n", line_cfg->cas_enable));
      WUFE_PRINT_ERR(( wufei_log,"cas_alignment_location %d > 1\n",
                       line_cfg->cas_alignment_location));
      WUFE_PRINT_ERR(( wufei_log,"multi_frame_sync_level %d > 1\n",
                       line_cfg->multi_frame_sync_level));
      WUFE_PRINT_ERR(( wufei_log,"transfer_type %d > WUFE_CAD_TYPE_DS3\n",
                       line_cfg->transfer_type));
      return WUFE_ERR_CAD_INVALID_CFG;
   }

   *rx_cad_mod |=(
      WUFEI_CAD_MODE_UN_FRAME_MODE_F   (line_cfg->framed)|
      WUFEI_CAD_MODE_MULTI_FRAME_SYNC_F(line_cfg->multi_frame)|
      WUFEI_CAD_MODE_M_FRAME_SYNC_F    (line_cfg->multi_frame_sync_level)|
      WUFEI_CAD_MODE_MULTIFRAME_NUM_F  (line_cfg->multi_frame_num)|
      WUFEI_CAD_MODE_CAS_ENABLE_F      (line_cfg->cas_enable)|
      WUFEI_CAD_MODE_CAS_ALIGN_F       (line_cfg->cas_alignment_location));

   /*  test debug option:
    *  =================
    *   *rx_cad_mod |= 0x1000;
    *  transmit data and sync outputs are leatched on Neg */

   switch(line_cfg->transfer_type)
   {
      case (WUFE_CAD_TYPE_E1):
      {
         line_registry->hmvip_num = WUFE_HMVIP_NON;
         *rx_cad_mod |=WUFEI_CAD_MODE_T1_F(WUFEI_CAD_TRANSFER_E1);

         if(line_cfg->cad_id >= WUFEI_N_MAX_CAD_E1)
            return WUFE_ERR_CAD_ID_FOR_TRANSFER_TYPE;
         break;
      }

      case (WUFE_CAD_TYPE_HMVIP_4_E1):
      {
         line_registry->hmvip_num = WUFE_HMVIP_NUM_4;
         *rx_cad_mod |=WUFEI_CAD_MODE_T1_F(WUFEI_CAD_TRANSFER_E1);

         if(line_cfg->cad_id >= WUFEI_N_MAX_CAD_HMVIP_4_E1)
            return WUFE_ERR_CAD_ID_FOR_TRANSFER_TYPE;
         if (WUFE_MAX_HMVIP_INUSE < WUFE_HMVIP_NUM_4)
            return WUFE_ERR_INVALID_TRANSFER_TYPE;
         break;
      }

      case (WUFE_CAD_TYPE_T1):
      {
         line_registry->hmvip_num = WUFE_HMVIP_NON;
         *rx_cad_mod |=WUFEI_CAD_MODE_T1_F(WUFEI_CAD_TRANSFER_T1);
         if(line_cfg->cad_id >= WUFEI_N_MAX_CAD_T1)
            return WUFE_ERR_CAD_ID_FOR_TRANSFER_TYPE;
         break;
      }

      case (WUFE_CAD_TYPE_HMVIP_4_T1):
      {
         line_registry->hmvip_num = WUFE_HMVIP_NUM_4;
         *rx_cad_mod |=WUFEI_CAD_MODE_T1_F(WUFEI_CAD_TRANSFER_T1);
         if(line_cfg->cad_id >= WUFEI_N_MAX_CAD_HMVIP_4_T1)
            return WUFE_ERR_CAD_ID_FOR_TRANSFER_TYPE;
         if (WUFE_MAX_HMVIP_INUSE < WUFE_HMVIP_NUM_4)
            return WUFE_ERR_INVALID_TRANSFER_TYPE;
         break;
      }

      case (WUFE_CAD_TYPE_E3):
      {
         line_registry->hmvip_num = WUFE_HMVIP_NON;
         *rx_cad_mod |=WUFEI_CAD_MODE_T1_F(WUFEI_CAD_TRANSFER_E3);
         if(line_cfg->cad_id >= WUFEI_N_MAX_CAD_E3  )
            return WUFE_ERR_CAD_ID_FOR_TRANSFER_TYPE;
         break;
      }

      case (WUFE_CAD_TYPE_DS3):
      {
         line_registry->hmvip_num = WUFE_HMVIP_NON;
         *rx_cad_mod |=WUFEI_CAD_MODE_T1_F(WUFEI_CAD_TRANSFER_T3);
         if(line_cfg->cad_id >= WUFEI_N_MAX_CAD_DS3  )
            return WUFE_ERR_CAD_ID_FOR_TRANSFER_TYPE;
         break;
      }

      default:
         return WUFE_ERR_INVALID_TRANSFER_TYPE;
   }
   return WUFE_OK;

}

WUFE_status WUFEI_HwCadTdmConfig(WUFEI_system_registry *system_registry,
                                 WUFEI_line_registry *line_registry,
                                 WP_U16 *hw_cad_tdm_ram_id)
{

   if (system_registry->tdm_line_arr[0][line_registry->line_id*4].line_id   ==
       WUFEI_EMPTY_TDM_CAD_ENTRY &&
       system_registry->tdm_line_arr[0][line_registry->line_id*4+1].line_id ==
       WUFEI_EMPTY_TDM_CAD_ENTRY &&
       system_registry->tdm_line_arr[0][line_registry->line_id*4+2].line_id ==
       WUFEI_EMPTY_TDM_CAD_ENTRY &&
       system_registry->tdm_line_arr[0][line_registry->line_id*4+3].line_id ==
       WUFEI_EMPTY_TDM_CAD_ENTRY)
   {/* the 4*32slot unit is empty*/

      system_registry->tdm_line_arr[0][line_registry->line_id*4].line_id   =
         line_registry->line_id;
      system_registry->tdm_line_arr[0][line_registry->line_id*4+1].line_id =
         line_registry->line_id;
      system_registry->tdm_line_arr[0][line_registry->line_id*4+2].line_id =
         line_registry->line_id;
      system_registry->tdm_line_arr[0][line_registry->line_id*4+3].line_id =
         line_registry->line_id;
      *hw_cad_tdm_ram_id= WUFEI_CAD_ID_HMVIP_4_F(line_registry->line_id%32);

      line_registry->spe_id=0;
      line_registry->sub_spe_entry=4*line_registry->line_id;
   }
   else
      return WUFE_ERR_LINE_ALREADY_CONFIGURED;

   return WUFE_OK;
}
