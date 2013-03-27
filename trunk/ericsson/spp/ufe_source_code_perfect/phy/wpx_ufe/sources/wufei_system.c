/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_system.c
 *
 * Purpose: the UFE system functions
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif
#ifndef WUFEI_HW_CONFIG_H
#include "wufei_hw_config.h"
#endif
#ifndef WUFEI_HW_MEMORY_H
#include "wufei_hw_memory.h"
#endif
#ifndef WUFEI_HW_EMPHY_H
#include "wufei_hw_emphy.h"
#endif
#ifndef WUFEI_HW_TDM_H
#include "wufei_hw_tdm.h"
#endif
#ifndef WUFEI_HW_FIFO_MANAG_H
#include "wufei_hw_fifo_manager.h"
#endif
#ifndef WUFEI_HW_LIU_INT_H
#include "wufei_hw_liu_int.h"
#endif
#ifndef WUFE_ERRORS_H
#include "wufe_errors.h"
#endif
#ifndef WUFE_SYSTEM_H
#include "wufe_system.h"
#endif
#ifndef WUFE_SBI_H
#include "wufe_sbi.h"
#endif
#ifndef WUFEI_HW_SBI_INT_H
#include "wufei_hw_sbi_int.h"
#endif
#ifndef WUFEI_HW_EMPHY_INT_H
#include "wufei_hw_emphy_int.h"
#endif
#ifndef WUFEI_SYSTEM_H
#include "wufei_system.h"
#endif
#ifndef WUFEI_REGISTRY_H
#include "wufei_registry.h"
#endif
#ifndef WUFEI_REGISTRY_INT_H
#include "wufei_registry_int.h"
#endif
#ifndef WUFE_INT_H
#include "wufei_int.h"
#endif
#ifndef WUFEI_LINE_H
#include "wufei_line.h"
#endif
#ifndef WUFEI_HW_EVENTS_INT_H
#include "wufei_hw_events_int.h"
#endif
#ifndef WUFEI_EVENTS_INT_H
#include "wufei_events_int.h"
#endif
#ifndef WUFEI_HW_TDM_INT_H
#include "wufei_hw_tdm_int.h"
#endif
#ifndef WUFEI_HW_MACHINE_INT_H
#include "wufei_hw_machine_int.h"
#endif
#ifndef WUFEI_HW_FM_INT_H
#include "wufei_hw_fm_int.h"
#endif
#ifndef WUFEI_HW_GLOBALS_H
#include "wufei_hw_globals.h"
#endif
#ifndef WUFEI_HW_GLOBALS_INT_H
#include "wufei_hw_globals_int.h"
#endif
#ifndef WUFE_IDENTITY_H
#include "wufe_identity.h"
#endif
#ifndef WUFEI_MEMORY_ACCESS_INT_H
#include "wufei_memory_access_int.h"
#endif
#ifndef WUFEI_CLK_REC_H
#include "wufei_clk_rec.h"
#endif
#include "api/wp_types.h"
#include "include/core/utility/wpi_services.h"
#ifndef WUFEI_HW_CLK_REC_INT_H
#include "wufei_hw_clk_rec_int.h"
#endif
#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif
WP_U8 debug_flag = 0;

extern WP_U32 wpi_32x32_32(WP_U32 a, WP_U32 b, WP_U32 c);

static WUFE_status WUFEI_SystemCoreSetup(WUFEI_system_registry* system_registry);

static WUFE_status WUFEI_SystemFifoMangSetup(WUFEI_system_registry* system_registry,
                                             WP_U32 coreid);
static WUFE_status WUFEI_SystemCasTdmSetup(WUFEI_system_registry* system_registry, WP_U32 coreid);

static WUFE_status WUFEI_SystemCoreEnable(WUFEI_system_registry* system_registry,
                                          WP_U32 property,
                                          WP_U32 direction);
static WUFE_status WUFEI_SystemSbiEnable(WUFEI_system_registry* system_registry,
                                         WP_U32 property);


static WUFE_status WUFEI_SystemCoreDisable(WUFEI_system_registry* system_registry,
                                           WP_U32 property,
                                           WP_U32 direction);

static WUFE_status WUFEI_SystemSbiDisable(WUFEI_system_registry* system_registry,
                                          WP_U32 property);

static WUFE_status WUFEI_SystemCoreDisplay(WUFEI_system_registry* system_registry);
static WUFE_status WUFEI_SystemFifoMangDisplay(WUFEI_system_registry* system_registry,
                                               WP_U32 coreid);
WUFE_status WUFEI_SystemBufferDescDisplay(WUFEI_system_registry* system_registry,
                                          WP_U32 coreid);

static WUFE_status WUFEI_SystemTdmDisplay(WUFEI_system_registry* system_registry, WP_U32 coreid);

/*****************************************************************************
 *
 * Function:  WUFEI_SystemCasTdmSetup
 *
 * Purpose:   configure the UFE TDM
 *
 * Description:
 *     This function HW registers of TDM module
 *
 * Inputs:
 *     system_registry The registry entry created for this UFE
 *
 * Outputs:
 *     write to external HW TDM registers
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_SystemCasTdmSetup(WUFEI_system_registry* system_registry, WP_U32 coreid)
{
   WP_U16 tdm_hw_mode, idle_data, idle_cas;
   WP_U32 id = WUFEI_ID(system_registry->wufe_id, coreid);
   WUFE_status status;

   idle_data = system_registry->cfg.idle_pattern.tx_idle_byte_data_pattern;
   idle_cas = system_registry->cfg.idle_pattern.tx_idle_nibble_cas_pattern;

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      WUFEI_RETURN_IF_ERROR(status, WUFEI_HwTdmIdleCasSet(system_registry, idle_data, idle_cas, coreid));
      return WUFE_OK;
   }

   /* TDM idle DATA pattern  */
   tdm_hw_mode =
      WUFEI_TDM_MODE_DATA_PATTERN_F(idle_data);

   /* TDM idle CAS pattern  */
   if (idle_cas > 0xf)
      return WUFE_ERR_ILLEGAL_CAS_PATTERN;

   tdm_hw_mode |=
      WUFEI_TDM_MODE_CAS_PATTERN_F(idle_cas);

   WUFEI_MEM_16_SET(id, (WP_U16 *)WUFEI_TDM_IDLE, tdm_hw_mode);
   /*
    * optional init reset the TDM table
    * not a must because the Line create resets its own TDM area
    */

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_SystemCoreSetup
 *
 * Purpose:   configure the UFE Cores
 *
 * Description:
 *     This function configures the initialized UFE cores
 *
 * Inputs:
 *     system_registry The registry entry created for this UFE
 *
 * Outputs:
 *     write the TDM, EMPHY, FIFO manager registers for all
 *     initialized cores
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_SystemCoreSetup(WUFEI_system_registry* system_registry)
{
   WP_U32 coreid, ufeid = system_registry->wufe_id;
   WUFE_status status=0;

   for(coreid = 0; coreid < WUFE_MAX_N_CORE; coreid++)
   {
      if(system_registry->hw_config.core_enabled[coreid])
      {
         /* EMPHY reset */
         /* The EMPHY has to be initiated before the phy setup !!!*/
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwEmphySetup(system_registry, coreid));

      }
   }

   /* PHY reset & assign BD , init phy objects */
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_PhySystemSetup(system_registry));

   for(coreid = 0; coreid < WUFE_MAX_N_CORE; coreid++)
   {
      if(system_registry->hw_config.core_enabled[coreid])
      {
         /* FIFO MANAGER init*/
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_SystemFifoMangSetup(system_registry, coreid));

         if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
         {/*Initialize tdm indirect registers*/
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwTdmIndirectSetup(system_registry, coreid));

            WUFEI_RETURN_IF_ERROR(status, WUFEI_HwMachineSetup(ufeid, coreid));
         }
         else if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
         {
            /* PHY reset & assign BD , init phy objects */
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_PhySystemTdmSlotsSetup(system_registry, coreid));
         }
         else
         {
            WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
            return WUFE_ERR_INVALID_CONFIG;
         }
         /* TDM Cas init */
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_SystemCasTdmSetup(system_registry, coreid));
      }
   }
   /* Events init */
   WUFEI_RETURN_IF_ERROR(status, WUFEI_HwEventSetup(ufeid));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_FifoManagWrite
 * Purpose:   configure the UFE FIFO of the FIFO manager
 *
 *
 * Description:
 *     This function configures the control registers of the FIFO manager.
 *
 *
 * Inputs:
 *     wufe_id the     UFE identifier
 *     fifo_registers  the FIFO information
 *     direction       RX or TX
 * Outputs:
 *     write to external HW FIFO manager registers
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_FifoManagWrite(WP_U32 wufe_id,
                                        WUFEI_hw_fifom fifo_registers,
                                        WP_U32 direction)
{
   WUFEI_hw_fifom *fifo_ptr;

   fifo_ptr=(WUFEI_hw_fifom *)WUFEI_FIFOM(direction);

   WUFEI_MEM_16_SET(wufe_id,
                    &(fifo_ptr->fnwp),
                    fifo_registers.fnwp);

   WUFEI_MEM_16_SET(wufe_id,
                    &(fifo_ptr->fllp),
                    fifo_registers.fllp);

   WUFEI_MEM_16_SET(wufe_id,
                    &(fifo_ptr->fnfb),
                    fifo_registers.fnfb);
   return WUFE_OK;

}

WUFE_status WUFEI_SystemBufferDescDisplay(WUFEI_system_registry* system_registry,
                                          WP_U32 coreid)
{
   WP_U32 memsize = WUFEI_SystemHwConfigZbtSizeGet(system_registry, coreid);
   WP_U32 rx_fbp_size, tx_fbp_size, tx_phy_num, rx_phy_num, i;
   WP_U32 id, max_n_phys_per_core, max_buff_num;

   id = WUFEI_ID(system_registry->wufe_id,coreid);
   max_n_phys_per_core = system_registry->max_phy_per_core;

  /*max buffers calculation (memsizeMb*15/16)/4/16*/
   max_buff_num = memsize*WUFEI_1MB_SIZE*15/0x400;

   rx_fbp_size=system_registry->rx_fbp_size[coreid];
   tx_fbp_size=system_registry->tx_fbp_size[coreid];

   /* create local values*/
   if (system_registry->cfg.max_phy_number >= max_n_phys_per_core)
   {
      rx_phy_num  = max_n_phys_per_core;
      tx_phy_num  = max_n_phys_per_core;
   }
   else
   {
      rx_phy_num  = system_registry->cfg.max_phy_number;
      tx_phy_num  = system_registry->cfg.max_phy_number;
   }

   if ((rx_fbp_size+ tx_fbp_size) >= max_buff_num)
      return WUFE_NOT_ENOUGH_MEM_FOR_BUFFER;

   if (rx_fbp_size <= rx_phy_num )
      return WUFE_NOT_ENOUGH_RX_BUFFERS;

   if (tx_fbp_size <= tx_phy_num)
      return WUFE_NOT_ENOUGH_TX_BUFFERS;

   WUFE_PRINT_DISP((wufei_log,"\tFIFO MANAGER - buffer descriptor\n"));
   debug_flag = 0;

   WUFEI_INDIRECT_ADDRESS_SET(id, WUFEI_INDIRECT_MEM_ADD_EXT_FIFO_START);

   for (i=0; i<max_buff_num; i++)
   {
      WP_U16 tmp_val1=0 , tmp_val2=0;

      WUFEI_INDIRECT_DATA_16_GET(id, &tmp_val1); /*bd.control_bits_1 =0;*/
      WUFEI_INDIRECT_DATA_16_GET(id, &tmp_val2); /* bd.next_buff_desc =i+1 ;*/
      WUFE_PRINT_DISP((wufei_log,"\tbd.control_bits_1 0x%04x  bd.next_buff_desc 0x%04x\n",
                      tmp_val1, tmp_val2));

   }
   debug_flag = 0;

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_SystemFifoMangSetup
 *
 * Purpose:   configure the UFE FIFO manager
 *
 *
 * Description:
 *     This function creates the FBP and init the HW FIFO registers
 *
 *
 * Inputs:
 *     system_registry The registry entry created for this UFE
 *
 * Outputs:
 *     write to external UFE Memory and HW FIFO manager registers
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_NOT_ENOUGH_MEM_FOR_BUFFER
 *     WUFE_NOT_ENOUGH_RX_BUFFERS
 *     WUFE_NOT_ENOUGH_TX_BUFFERS
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/
static WUFE_status WUFEI_SystemFifoMangSetup(WUFEI_system_registry* system_registry,
                                             WP_U32 coreid)
{
   WP_U32 memsize;
   WP_U32 rx_fbp_size, tx_fbp_size, tx_phy_num, rx_phy_num, i;
   WUFEI_hw_fifom fifo_registers;
   WP_U32 id, max_n_phys_per_core, max_buff_num;
   WUFE_status status;

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      WUFEI_RETURN_IF_ERROR(status,WUFEI_HwFmSetPriorityTables(system_registry, coreid));
      WUFEI_RETURN_IF_ERROR(status,WUFEI_HwFmIndirectSetup(system_registry, coreid));
      if (system_registry->cfg.fifo_manager.loopback == WUFE_FM_LOOPBACK_ENABLE)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwFmLoopbackEnable(system_registry, coreid));
      }
      else
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwFmLoopbackDisable(system_registry, coreid));
      }
      return WUFE_OK;
   }

   id = WUFEI_ID(system_registry->wufe_id,coreid);
   max_n_phys_per_core = system_registry->max_phy_per_core;

   memsize = WUFEI_SystemHwConfigZbtSizeGet(system_registry, coreid);

   /*max buffers calculation (memsizeMb*15/16)/4/16*/
   max_buff_num = memsize*WUFEI_1MB_SIZE*15/0x400;

   rx_fbp_size = system_registry->rx_fbp_size[coreid];
   tx_fbp_size = system_registry->tx_fbp_size[coreid];

   /* create local values*/
   if (system_registry->cfg.max_phy_number >= max_n_phys_per_core)
   {
      rx_phy_num  = max_n_phys_per_core;
      tx_phy_num  = max_n_phys_per_core;
   }
   else
   {
      rx_phy_num  = system_registry->cfg.max_phy_number;
      tx_phy_num  = system_registry->cfg.max_phy_number;
   }

#if 0
   printf("mem_size 0x%x bytes\n", memsize*WUFEI_1MB_SIZE);
   printf("rx_fbp_size %d, tx_fbp_size %d\n", rx_fbp_size, tx_fbp_size);
   printf("rx_phy_num %d,  tx_phy_num %d\n", rx_phy_num, tx_phy_num);
   printf("max_buff_num 0x%x\n", max_buff_num);
   printf("ufeid %d, coreid %d\n",system_registry->wufe_id,coreid);
#endif

   if ((rx_fbp_size+ tx_fbp_size) >= max_buff_num)
      return WUFE_NOT_ENOUGH_MEM_FOR_BUFFER;

   if (rx_fbp_size <= rx_phy_num )
      return WUFE_NOT_ENOUGH_RX_BUFFERS;

   if (tx_fbp_size <= tx_phy_num)
      return WUFE_NOT_ENOUGH_TX_BUFFERS;

   /* Fisrt BD - write the Address in the indirect memory registers
      for the rest BD the Address will be incremented by HW */

   WUFEI_INDIRECT_ADDRESS_SET(id, WUFEI_INDIRECT_MEM_ADD_EXT_FIFO_START);

   /* Create the rest BDs*/
   for (i=0; i<max_buff_num; i++)
   {
      WUFEI_INDIRECT_DATA_16_SET(id, 0); /*bd.control_bits_1 =0;*/
      WUFEI_INDIRECT_DATA_16_SET(id, i+1); /* bd.next_buff_desc =i+1 ;*/
   }

#if WUFE_DEBUG_VERIFY_WRITE
   WUFEI_INDIRECT_ADDRESS_SET(id, WUFEI_INDIRECT_MEM_ADD_EXT_FIFO_START);

   for (i=0; i<max_buff_num; i++)
   {
      WP_U16 tmp_val1 , tmp_val2;

      WUFEI_INDIRECT_DATA_16_GET(id, &tmp_val1); /*bd.control_bits_1 =0;*/
      if(tmp_val1)
      {
         WUFE_PRINT_ERR((wufei_log,
                         "WUFEI_SystemFifoMangSetup: ZBT init fail (tmp_val1 0x%04X !=  0\n",
                         tmp_val1));

         return WUFE_ERR_MEM_INCONSIST;
      }

      WUFEI_INDIRECT_DATA_16_GET(id, &tmp_val2); /* bd.next_buff_desc =i+1 ;*/
      if(tmp_val2 !=  (i+1))
      {
         WUFE_PRINT_ERR(
            (wufei_log,
             "WUFEI_SystemFifoMangSetup: ZBT init fail (tmp_val2 0x%04X !=  0x%04x)\n",
             tmp_val2,  i+1));
         return WUFE_ERR_MEM_INCONSIST;
      }
   }
#endif

   /* FIFO Maneger HW registers*/
   fifo_registers.fnwp =rx_phy_num;                /*Free buffer pool Next Write Pointer    */
   fifo_registers.fllp =rx_fbp_size-1;             /*Free buffer pool Last Link Pointer     */
   fifo_registers.fnfb =rx_fbp_size - rx_phy_num ; /*Free buffer pool Number of Free Buffers */

   WUFEI_RETURN_IF_ERROR( status, WUFEI_FifoManagWrite(id, fifo_registers, WUFEI_RX));

   fifo_registers.fnwp =rx_fbp_size + tx_phy_num ;    /*Free buffer pool Next Write Pointer    */
   fifo_registers.fllp =rx_fbp_size + tx_fbp_size-1;  /*Free buffer pool Last Link Pointer     */
   fifo_registers.fnfb =tx_fbp_size - tx_phy_num;     /*Free buffer pool Number of Free Buffers */

   WUFEI_RETURN_IF_ERROR( status, WUFEI_FifoManagWrite(id, fifo_registers, WUFEI_TX));

   return WUFE_OK;
}

static WUFE_status WUFEI_SystemHwConfigSetup(WUFEI_system_registry *system_registry)
{
   WUFE_status status;

   memset(&system_registry->hw_config, 0, sizeof(WUFEI_system_hw_config));

   WUFEI_RETURN_IF_ERROR(status,WUFEI_UfeCoreHwConfigSetup(&system_registry->hw_config,
                                                           system_registry->wufe_id,
                                                           system_registry->cfg.max_buff_num));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_SystemConfig
 *
 * Purpose:   Configure the UFE either for creation or for modify
 *
 *
 * Description:
 *     It does the following:
 *      - Creates UFE system registry for lines and PHYs
 *      - Allocates memory for UFE lines and PHYs
 *      - Initiates registry parameters
 *      - Resets the UFE LIU registers
 *      - Initializes ZBT memory for FBP for all cores (UFE FIFO manager configuration)
 *      - Clears UFE TDM RAM (Socket Interface initialization)
 *      - Clears PHY's related tables and writes initial buffers there.
 *      - Initializes EMPHY block
 *      - Initialize UFE interrupt registers
 *      - Initializes Clock Recovery registers (if exist)
 *
 *
 * Inputs:
 *     wufe_id     The ufe indentifier.
 *     sys_cfg     the system configuration.
 *
 * Outputs:
 *     write to external UFE HW modules and the registery modifications.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_HANDLE
 *
 * Called by:
 *     The Application
 *
 ****************************************************************************/
WUFE_status WUFE_SystemConfig(WP_U32 wufe_id, WUFE_system *sys_cfg)
{
   WUFEI_system_registry *system_registry;
   WUFE_status status;
   WP_U32 i;
   if(!sys_cfg)
      return (WUFE_ERR_INVALID_CONFIG);

    /*sets the UFE lock */
    WUFEI_UFE_LOCK(wufe_id);
    /* read the system registry from the UFE registry */
    system_registry = WUFEI_SystemRegistryGet(wufe_id);


   if (!system_registry)
   {
      /* the registry has no instance for this UFE --> Create */
      system_registry = (WUFEI_system_registry*)WUFEI_MALLOC(sizeof(WUFEI_system_registry));
      if (!system_registry)
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,WUFE_ERR_MALLOC, wufe_id);
      memset(system_registry,0,sizeof(WUFEI_system_registry));

      /* copy the configuration parameters */
      memcpy(&(system_registry->cfg), sys_cfg, sizeof(WUFE_system));
      system_registry->wufe_id = wufe_id;
      for(i=0;i<WUFEI_MAX_PDH_LOOPBACK;i++)
      {
          system_registry->pdh_line_loopback[i]=WUFEI_MAX_OC48_LINES;
          system_registry->pdh_system_loopback[i]=WUFEI_MAX_OC48_LINES;
      }
      /* write the allocated  MEM block to the registry */
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_SystemeRegistrySet(wufe_id, system_registry),
                                     wufe_id);


      WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_SystemHwConfigSetup(system_registry), wufe_id);

      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_UfeCoreRegistrySetup(system_registry), wufe_id);

      /* Set GPIO output clock with default values */
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_UfeGpioClockDefaultSetup(system_registry), wufe_id);

      /* LINE Reset and init line objects */
      WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_LineSystemSetup(system_registry), wufe_id);

      /* UFE COREs reset ant initialization*/
      WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_SystemCoreSetup(system_registry), wufe_id);

      if(WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4 &&
         system_registry->cfg.line_interface != WUFE_INTERFACE_CAD_E1_T1 &&
         system_registry->cfg.line_interface != WUFE_INTERFACE_SONET &&
         system_registry->cfg.line_interface != WUFE_INTERFACE_SDH)
      {
         WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type. UFE4 supports only SONET/SDH/CAD\n"));
         return WUFE_ERR_INVALID_CONFIG;
      }

      if(system_registry->cfg.line_interface == WUFE_INTERFACE_SBI)
      {
         /* SBI clock translator init */
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFEI_SbiClockTranslatorSetup(system_registry),
                                        wufe_id);
      }

      if (system_registry->cfg.system_clock_rec_enable == WUFE_SYSTEM_CLOCK_REC_ENABLE)
      {
         if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
         {
            /* Clock Recovery Reset and init clock recovery objects */
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_ClockRecSystemSetup(system_registry,
                                                                     sys_cfg->clock_rec_sys_params),
                                           wufe_id);
         }
         else
         {
            /* In UFE4, check XO and DIFF PLL lock register */
            WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                           WUFEI_BsFpgaPllClockStatusGet(wufe_id),
                                           wufe_id);
         }
      }
      else if (system_registry->cfg.system_clock_rec_enable != WUFE_SYSTEM_CLOCK_REC_DISABLE)
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS,
                                        wufe_id);
      }
   }
   else
   {/* the registry has been created for this UFE */
      WUFE_PRINT_ERR((wufei_log,"WUFE: system already created !\n"));
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
   }

   /* In UFE4 set the default mode - mask all HW interrupts*/
   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      if (WUFEI_CheckBoardSupport(wufe_id) == WP_TRUE)
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_UfeCoreHwInterruptDisable(wufe_id));

         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_UfeFramerHwInterruptDisable(wufe_id));

         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_UfeExtPllHwInterruptDisable(wufe_id));
      }

      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_SystemRegistryUfeEmphyNumSet(wufe_id),
                                     wufe_id);
      WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);
   }


   system_registry->sys_enable.state_mode = WUFEI_SYSTEM_DISABLE;

   /* <--- unlock registry ---> */
   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_SystemSbiDisable
 *
 * Purpose: Disables the SBI configuration and SBI events
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     property            global SBI configuration mask
 *
 * Outputs:
 *     write to external UFE HW modules and the registery modifications.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_SystemDisable
 *
 ****************************************************************************/
static WUFE_status WUFEI_SystemSbiDisable(WUFEI_system_registry* system_registry, WP_U32 property)
{
   WUFE_status status;
   WP_U32 coreid, ufeid;
   ((void)property);
   ufeid = system_registry->wufe_id;

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwSbiGmrDisable(system_registry, property));

   for(coreid = 0; coreid < WUFE_MAX_N_CORE; coreid++)
   {
      if(system_registry->hw_config.core_enabled[coreid])
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwSbiSpeModeDisable(system_registry, property, coreid));
      }
   }

   WUFEI_HwEventSbiDisable(ufeid, (WP_U16)WUFEI_HW_EVENT_SBI);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_SystemSbiEnable
 *
 * Purpose: Enables the SBI configuration and SBI events
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     system_registry     UFE system registry
 *     property            global SBI configuration mask
 *
 * Outputs:
 *     write to external UFE HW modules and the registery modifications.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_SystemEnable
 *
 ****************************************************************************/
static WUFE_status WUFEI_SystemSbiEnable(WUFEI_system_registry* system_registry, WP_U32 property)
{
   WUFE_status status;
   WP_U32 coreid, ufeid = system_registry->wufe_id;
   ((void)property);
   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwSbiGmrEnable(system_registry, property));

   for(coreid = 0; coreid < WUFE_MAX_N_CORE; coreid++)
   {
      if(system_registry->hw_config.core_enabled[coreid])
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwSbiSpeModeEnable(system_registry, property, coreid));

      }
   }

   WUFEI_HwEventSbiEnable(ufeid, (WP_U16)WUFEI_HW_EVENT_SBI);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_SystemCoreDisable
 *
 * Purpose:   Disables the UFE Core HW (Emphy, Events, Socket interface)
 *
 *
 * Description:
 *     This function disables the CORE HW of the UFE system.
 *     The disable is of either RX or TX or both.
 *
 *
 * Inputs:
 *     wufe_id     The UFE identifier.
 *     direction   The UFE direction to be disabled .
 *
 * Outputs:
 *     write to external UFE EMPHY HW.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_HANDLE
 *     WUFE_ERR_INVALID_DIRECTION
 *
 * Called by:
 *     WUFEI_SystemDisable
 *
 ****************************************************************************/
static WUFE_status WUFEI_SystemCoreDisable(WUFEI_system_registry* system_registry,
                                           WP_U32 property,
                                           WP_U32 direction)
{
   WUFE_status status;
   WP_U32 coreid, ufeid;

   ufeid = system_registry->wufe_id;

   for(coreid = 0; coreid < WUFE_MAX_N_CORE; coreid++)
   {
      if(system_registry->hw_config.core_enabled[coreid])
      {
         if (property & WUFE_SYS_EMPHY)
         {
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwEmphyDisable(system_registry,coreid,direction));
         }

         if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
         {
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwTdmDisable(system_registry,coreid,direction));

            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwFmDisable(system_registry,coreid,direction));
         }
         else if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
         {
            /*Stops requests assertion to the FM*/
            WUFEI_RETURN_IF_ERROR(status,WUFEI_LiuSocketDisable(ufeid, coreid, direction));
         }
         else
         {
            WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
            return WUFE_ERR_INVALID_CONFIG;
         }
      }
   }

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_SystemCoreEnable
 *
 * Purpose:   Enables the UFE Core HW (Emphy, Events, Socket interface)
 *
 *
 * Description:
 *     This function enables the CORE HW of the UFE system.
 *     The disable is of either RX or TX or both.
 *
 *
 * Inputs:
 *     wufe_id     The UFE identifier.
 *     direction   The UFE direction to be disabled .
 *
 * Outputs:
 *     write to external UFE EMPHY HW.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_HANDLE
 *     WUFE_ERR_INVALID_DIRECTION
 *
 * Called by:
 *     WUFEI_SystemEnable
 *
 ****************************************************************************/
static WUFE_status WUFEI_SystemCoreEnable(WUFEI_system_registry* system_registry,
                                          WP_U32 property,
                                          WP_U32 direction)
{
   WUFE_status status;
   WP_U32 coreid, ufeid;

   ufeid = system_registry->wufe_id;

   for(coreid = 0; coreid < WUFE_MAX_N_CORE; coreid++)
   {
      if(system_registry->hw_config.core_enabled[coreid])
      {
         if (property & WUFE_SYS_EMPHY)
         {
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwEmphyEnable(system_registry,coreid,direction));
         }

         if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
         {
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_UfeCoreDdrStateGet(ufeid, coreid));

            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwTdmEnable(system_registry,coreid,direction));

            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_HwFmEnable(system_registry,coreid,direction));
         }
         else if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
         {
            /*TDM asserts requests to the FM*/
            WUFEI_RETURN_IF_ERROR(status, WUFEI_LiuSocketEnable(ufeid, coreid, direction));
         }
         else
         {
            WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
            return WUFE_ERR_INVALID_CONFIG;
         }
      }
   }

   return WUFE_OK;
}


static WUFE_status WUFEI_SystemEventsEnable(WUFEI_system_registry* system_registry)
{
   WUFE_events ufe_events;
   WP_U32 coreid, ufeid;

   memset(&ufe_events, 0, sizeof(WUFE_events));

   ufeid = system_registry->wufe_id;

   for(coreid = 0; coreid < WUFE_MAX_N_CORE; coreid++)
   {
      if(system_registry->hw_config.core_enabled[coreid])
      {
         if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
         {
            ufe_events.tdm_events.event_reg = WUFE_EVENT_TDM;
            ufe_events.fm_events.event_reg = WUFE_EVENT_FM;
            ufe_events.machine_events.rx_event_reg = WUFE_EVENT_MACHINE_RX;
            ufe_events.machine_events.tx_event_reg = WUFE_EVENT_MACHINE_TX;
         }
         else if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
         {
            /*Enables the UFE core events*/
            ufe_events.core_events[coreid].event_reg = WUFE_EVENT_CORE;
         }
         else
         {
            WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
            return WUFE_ERR_INVALID_CONFIG;
         }

         ufe_events.emphy_events[coreid].event_reg = WUFE_EVENT_EMPHY;
      }
   }

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
   {
      /* SBI/CAD events enabled during appropriate LIU enable*/
      ufe_events.liu_events.event_reg = WUFE_EVENT_LIU;

      if (system_registry->hw_config.clk_rec_hw_config)
      {
         if (system_registry->cfg.line_interface == WUFE_INTERFACE_SBI)
            ufe_events.cr_events.event_reg |= WUFE_EVENT_CR_CT_COUNTER_OVERFLOW;
      }
   }

   /*Enables the UFE core events*/
   return (WUFEI_HwEventEnable(ufeid, &ufe_events));
}

static WUFE_status WUFEI_SystemEventsDisable(WUFEI_system_registry* system_registry)
{
   WUFE_events ufe_events;
   WP_U32 coreid, ufeid;

   memset(&ufe_events, 0, sizeof(WUFE_events));

   ufeid = system_registry->wufe_id;

   for(coreid = 0; coreid < WUFE_MAX_N_CORE; coreid++)
   {
      if(system_registry->hw_config.core_enabled[coreid])
      {
         if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
         {
            ufe_events.tdm_events.event_reg = WUFE_EVENT_TDM;
            ufe_events.fm_events.event_reg = WUFE_EVENT_FM;
            ufe_events.machine_events.rx_event_reg = WUFE_EVENT_MACHINE_RX;
            ufe_events.machine_events.tx_event_reg = WUFE_EVENT_MACHINE_TX;
         }
         else if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
         {
            /*Disables the UFE core events*/
            ufe_events.core_events[coreid].event_reg = WUFE_EVENT_CORE;
         }
         else
         {
            WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
            return WUFE_ERR_INVALID_CONFIG;
         }

         ufe_events.emphy_events[coreid].event_reg = WUFE_EVENT_EMPHY;
      }
   }

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
   {
      /* SBI/CAD events disabled during appropriate LIU enable*/
      ufe_events.liu_events.event_reg = WUFE_EVENT_LIU;

      if (system_registry->hw_config.clk_rec_hw_config)
      {
         if (system_registry->cfg.line_interface == WUFE_INTERFACE_SBI)
            ufe_events.cr_events.event_reg |= WUFE_EVENT_CR_CT_COUNTER_OVERFLOW;
      }
   }

   /*Disables the UFE core events*/
   return (WUFEI_HwEventDisable(ufeid, &ufe_events));
}

/*****************************************************************************
 *
 * Function:  WUFE_SystemEnable
 *
 * Purpose:   Enables the properties passed in the UFE (EMPHY or SBI)
 *
 *
 * Description:
 *     This function enables the EMPHY modules of the UFE system.
 *     The enable is of either RX or TX or both.
 *
 *
 * Inputs:
 *     wufe_id     The UFE identifier.
 *     properties  EMPHY or SBI.
 *     direction   The UFE direction to be enabled .
 *
 * Outputs:
 *     write to external UFE EMPHY HW or SBI HW.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_HANDLE
 *
 * Called by:
 *     The Application
 *
 ****************************************************************************/
WUFE_status WUFE_SystemEnable(WP_U32 wufe_id, WP_U32 property, WP_U32 direction)
{
   WUFE_status status;
   WUFEI_system_registry *system_registry;

   /* lock the wufe_id UFE*/
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

   if(system_registry->sys_enable.state_mode != WUFEI_SYSTEM_DISABLE)
   {
      if(system_registry->sys_enable.state_mode != WUFEI_SYSTEM_ENABLE)
      {
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         WUFE_PRINT_ERR((wufei_log,
                         "WUFE_SystemEnable sys_enable.state_mode =%d  \n",
                         system_registry->sys_enable.state_mode));
         return WUFE_ERR_INVALID_REGISTRY_STATE;
      }
      else
      {
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         return WUFE_ERR_SYSTEM_NOT_DISABLED;
      }
   }

   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_SystemCoreEnable(system_registry,
                                                         property,
                                                         direction),
                                  wufe_id);

   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_SystemEventsEnable(system_registry),
                                  wufe_id);

   if(system_registry->cfg.line_interface == WUFE_INTERFACE_SBI)
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_SystemSbiEnable(system_registry, property),
                                     wufe_id);

   WUFEI_UfeCoreGlobalIntMaskSet(wufe_id, 0);

   system_registry->sys_enable.state_mode = WUFEI_SYSTEM_ENABLE;

   /* Enable the system CR */
   WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_ClockRecSystemEnable(system_registry), wufe_id);

   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_SystemDisable
 *
 * Purpose:   Disables the UFE HW
 *
 *
 * Description:
 *     This function disables the EMPHY, CORE, SBI  modules of the UFE system.
 *     The disable is of either RX or TX or both.
 *
 *
 * Inputs:
 *     wufe_id     The UFE identifier.
 *     direction   The UFE direction to be disabled .
 *
 * Outputs:
 *     write to external UFE EMPHY HW.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_HANDLE
 *     WUFE_ERR_INVALID_DIRECTION
 *
 * Called by:
 *     WUFE_SystemDisable, WUFE_UfeRelease
 *
 ****************************************************************************/
WUFE_status WUFEI_SystemDisable(WP_U32 wufe_id, WP_U32 property, WP_U32 direction)
{
   WUFE_status status;
   WUFEI_system_registry *system_registry;

   system_registry = WUFEI_SystemRegistryGet(wufe_id);

   /*already deleted*/
   if(!system_registry)
      return WUFE_OK;

   /*already disabled*/
   if(system_registry->sys_enable.state_mode == WUFEI_SYSTEM_DISABLE)
      return WUFE_OK;

   /* Disable the system CR */
   WUFEI_RETURN_IF_ERROR(status, WUFEI_ClockRecSystemDisable(system_registry));

   WUFEI_RETURN_IF_ERROR(status, WUFEI_SystemCoreDisable(system_registry,
                                                         property,
                                                         direction));

   WUFEI_RETURN_IF_ERROR(status, WUFEI_SystemEventsDisable(system_registry));

   if(system_registry->cfg.line_interface == WUFE_INTERFACE_SBI)
      WUFEI_RETURN_IF_ERROR(status, WUFEI_SystemSbiDisable(system_registry,
                                                           property));

   system_registry->sys_enable.state_mode = WUFEI_SYSTEM_DISABLE;

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_SystemDisable
 *
 * Purpose:   Disables the UFE EMPHY
 *
 *
 * Description:
 *     This function disables the EMPHY modules of the UFE system.
 *     The disable is of either RX or TX or both.
 *
 *
 * Inputs:
 *     wufe_id     The UFE identifier.
 *     direction   The UFE direction to be disabled .
 *
 * Outputs:
 *     write to external UFE EMPHY HW.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_EMPHY_ALREADY_DISABLED
 *
 * Called by:
 *     The Application
 *
 ****************************************************************************/
WUFE_status WUFE_SystemDisable(WP_U32 wufe_id, WP_U32 property, WP_U32 direction)
{
   WUFE_status status;

   WUFEI_system_registry *system_registry;

   /* lock the wufe_id UFE*/
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

   if(!system_registry)
   {
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_OK;
   }

   if(system_registry->sys_enable.state_mode != WUFEI_SYSTEM_ENABLE)
   {
      if(system_registry->sys_enable.state_mode != WUFEI_SYSTEM_DISABLE)
      {
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         WUFE_PRINT_ERR((wufei_log,
                         "WUFE_SystemDisable sys_enable.state_mode =%d  \n",
                         system_registry->sys_enable.state_mode));
         return WUFE_ERR_INVALID_REGISTRY_STATE;
      }
      else
      {
         return WUFE_ERR_SYSTEM_ALREADY_DISABLED;
      }
   }

   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_SystemDisable(wufe_id, property, direction),
                                  wufe_id);

   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_SystemInterruptEnable
 *
 * Purpose:   Enables the HW host interrupt signal
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid     The UFE identifier.
 *
 * Outputs:
 *     write to external UFE EMPHY HW.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_INVALID_REGISTRY_STATE
 *
 * Called by:
 *     The Application
 *
 ****************************************************************************/
WUFE_status WUFE_SystemInterruptEnable(WP_U32 ufeid)
{
   WUFEI_system_registry *system_registry;
   WUFE_status status;

   /*sets the UFE lock */
   WUFEI_UFE_LOCK(ufeid);

   /*read the system registry from the UFE registry */
   system_registry = WUFEI_SystemRegistryGet(ufeid);
   if(!system_registry)
   {
      WUFEI_UFE_UNLOCK(ufeid);
      return WUFE_ERR_INVALID_REGISTRY_STATE;
   }

   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_UfeCoreGlobalIntEnable(ufeid,
                                                               0/*prototype compatibility*/),
                                  ufeid);
   /*releases the UFE lock*/
   WUFEI_UFE_UNLOCK(ufeid);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_SystemInterruptDisable
 *
 * Purpose:   Disables the HW host interrupt signal
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid     The UFE identifier.
 *
 * Outputs:
 *     write to external UFE EMPHY HW.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *     WUFE_ERR_INVALID_REGISTRY_STATE
 *
 * Called by:
 *     The Application
 *
 ****************************************************************************/
WUFE_status WUFE_SystemInterruptDisable(WP_U32 ufeid)
{
   WUFEI_system_registry *system_registry;
   WUFE_status status;

   /*sets the UFE lock */
   WUFEI_UFE_LOCK(ufeid);

   /*read the system registry from the UFE registry */
   system_registry = WUFEI_SystemRegistryGet(ufeid);
   if(!system_registry)
   {
      WUFEI_UFE_UNLOCK(ufeid);
      return WUFE_ERR_INVALID_REGISTRY_STATE;
   }

   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_UfeCoreGlobalIntDisable(ufeid, 0/*dummy*/),
                                  ufeid);
   /*releases the UFE lock*/
   WUFEI_UFE_UNLOCK(ufeid);

   return WUFE_OK;
}

static WUFE_status WUFEI_SystemCoreDisplay(WUFEI_system_registry* system_registry)
{
   WUFE_status status = 0;
   WP_U32 coreid,i;

   for(coreid = 0; coreid < WUFE_MAX_N_CORE; coreid++)
   {
      if(system_registry->hw_config.core_enabled[coreid])
      {
         if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
         {
            WP_U32 ufeid = system_registry->wufe_id;

            WUFEI_HwEmphyDisplay(ufeid, coreid);
            WUFEI_HwFmModeDisplay(ufeid, coreid);
            WUFEI_HwTdmDisplay(ufeid,coreid);

            WUFE_PRINT_DISP((wufei_log,"TDM PDH line loopback is active for the following lines: (loopback_index,line_index)\n"));
            for(i=0;i<WUFEI_MAX_PDH_LOOPBACK;i++)
            {
                if(system_registry->pdh_line_loopback[i]!=WUFEI_MAX_OC48_LINES)
                    WUFE_PRINT_DISP((wufei_log,"loopback index: %d, line_index: %d\n",i,system_registry->pdh_line_loopback[i]));
            }
            WUFE_PRINT_DISP((wufei_log,"TDM PDH system loopback is active for the following lines: (loopback_index,line_index)\n"));
            for(i=0;i<WUFEI_MAX_PDH_LOOPBACK;i++)
            {
                if(system_registry->pdh_system_loopback[i]!=WUFEI_MAX_OC48_LINES)
                    WUFE_PRINT_DISP((wufei_log,"loopback index: %d, line_index: %d\n",i,system_registry->pdh_system_loopback[i]));
            }
         }
         else if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
         {
         /*WUFEI_RETURN_IF_ERROR(status,
           WUFEI_SystemBufferDescDisplay(system_registry, coreid));*/
            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_SystemFifoMangDisplay(system_registry,coreid));

            WUFEI_RETURN_IF_ERROR(status,
                                  WUFEI_SystemTdmDisplay(system_registry,coreid));
         }
         else
         {
            WUFE_PRINT_ERR((wufei_log, "Invalid FPGA type\n"));
            return WUFE_ERR_INVALID_CONFIG;
         }
      }
   }

   if(system_registry->cfg.line_interface == WUFE_INTERFACE_SBI)
   {
      WUFEI_HwSbiGlobalDisplay(system_registry);
   }

   return WUFE_OK;
}

static WUFE_status WUFEI_SystemFifoMangDisplay(WUFEI_system_registry* system_registry,
                                               WP_U32 coreid)
{
   WUFEI_hw_fifom *fifo_ptr;
   WUFEI_hw_fifom rx_fifo_val;
   WUFEI_hw_fifom tx_fifo_val;
   WP_U32 memsize;
   WP_U32 max_buff_num;
   WP_U32 id = WUFEI_ID(system_registry->wufe_id,coreid);

   fifo_ptr=(WUFEI_hw_fifom *)WUFEI_FIFOM(WUFEI_RX);

   WUFEI_MEM_16_GET(id, &(fifo_ptr->fnfb), &rx_fifo_val.fnfb);
   WUFEI_MEM_16_GET(id, &(fifo_ptr->fllp), &rx_fifo_val.fllp);

   WUFEI_MEM_16_GET(id, &(fifo_ptr->fnwp), &rx_fifo_val.fnwp);

   fifo_ptr=(WUFEI_hw_fifom *)WUFEI_FIFOM(WUFEI_TX);

   WUFEI_MEM_16_GET(id, &(fifo_ptr->fnfb), &tx_fifo_val.fnfb);
   WUFEI_MEM_16_GET(id, &(fifo_ptr->fllp), &tx_fifo_val.fllp);

   WUFEI_MEM_16_GET(id, &(fifo_ptr->fnwp), &tx_fifo_val.fnwp);

   memsize = WUFEI_SystemHwConfigZbtSizeGet(system_registry, coreid);
   if (system_registry->hw_config.zbt_internal[coreid])
      max_buff_num = memsize*WUFEI_1MB_SIZE*15/0x400;
   else
      max_buff_num = WUFEI_HW_INTERNAL_FIFO_MEM_SIZE*WUFEI_1MB_SIZE*15/0x400;

   WUFE_PRINT_DISP((wufei_log, "\nFIFO Manager Parameter for core: %d\n",
                    coreid));

   WUFE_PRINT_DISP((wufei_log, "\nZBT size: 0x%x bytes; Max buffers: %d; Max PHYs: %d\n",
                    memsize*WUFEI_1MB_SIZE, max_buff_num, system_registry->cfg.max_phy_number));

   WUFE_PRINT_DISP((wufei_log, "Total number of RX available buffers: 0x%x \n",
                    system_registry->rx_fbp_size[coreid]));

   WUFE_PRINT_DISP((wufei_log, "Num of buffers in FBP RX 0x%x \n", rx_fifo_val.fnfb));

   WUFE_PRINT_DISP((wufei_log, "Num of buffers currently allocated to phys is 0x%x \n",
                    (system_registry->rx_fbp_size[coreid]-
                     rx_fifo_val.fnfb)));

   WUFE_PRINT_DISP((wufei_log, "RX fifo next write  0x%04X   last link 0x%04X\n",
                    rx_fifo_val.fnwp, rx_fifo_val.fllp));

   WUFE_PRINT_DISP((wufei_log, "Total number of TX available buffers: 0x%x \n",
                    system_registry->tx_fbp_size[coreid]));

   WUFE_PRINT_DISP((wufei_log, "Num of buffers in FBP TX 0x%x \n", tx_fifo_val.fnfb));

   WUFE_PRINT_DISP((wufei_log, "TX fifo next write  0x%04X   last link 0x%04X \n",
                    tx_fifo_val.fnwp, tx_fifo_val.fllp));

   return WUFE_OK;
}

static WUFE_status WUFEI_SystemTdmDisplay(WUFEI_system_registry* system_registry, WP_U32 coreid)
{
   WUFEI_hw_idle_pattern  hw_pattern;
   WP_U16 tdm_wh_mode = 0xffff;
   WP_U32 id = WUFEI_ID(system_registry->wufe_id,coreid);

   WUFEI_MEM_16_GET(id,(WP_U16 *)WUFEI_TDM_IDLE, &tdm_wh_mode);

   hw_pattern.idle_data_byte  = WUFEI_TDM_MODE_DATA_PATTERN_V(tdm_wh_mode);
   hw_pattern.idle_cas_nibble = WUFEI_TDM_MODE_CAS_PATTERN_V(tdm_wh_mode);

   WUFE_PRINT_DISP((wufei_log,
                    "CAS idle nibble pattern 0x%02x (== 0x%02x) \t",
                    hw_pattern.idle_cas_nibble,
                    system_registry->cfg.idle_pattern.tx_idle_nibble_cas_pattern));

   WUFE_PRINT_DISP((wufei_log,
                    "DATA idle pattern 0x%02x (== 0x%02x) \n",
                    hw_pattern.idle_data_byte,
                    system_registry->cfg.idle_pattern.tx_idle_byte_data_pattern));

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFE_SystemDisplay
 *
 * Purpose:   Displays the UFE EMPHY
 *
 *
 * Description:
 *     This function display the modules of the UFE system.
 *     The disable is of either RX or TX or both.
 *
 *
 * Inputs:
 *     wufe_id     The UFE identifier.
 *     direction   The UFE direction to be disabled .
 *
 * Outputs:
 *     write to external UFE EMPHY HW.
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     The Application
 *
 ****************************************************************************/
WUFE_status WUFE_SystemDisplay(WP_U32 wufe_id)
{
   WUFE_status status;
   WUFEI_system_registry *system_registry;
   WP_U32 syn_build_time;

   /* lock the wufe_id UFE*/
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

   WUFE_PRINT_DISP((wufei_log,
                    "\n #####  SYSTEM DISPLAY (wufe_id %d) #####\nUFE SW Version %d.%d.%d\n",
                    wufe_id, WUFE_UFE_MAJOR_VERSION, WUFE_UFE_MINOR_VERSION, WUFE_UFE_SUB_VERSION));

   WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                  WUFEI_UfeSynBuildTimeGet(wufe_id, &syn_build_time), 
                                  wufe_id);
   
   WUFE_PRINT_DISP((wufei_log,"\nHW build time                 %d\n",
                    syn_build_time));

   WUFEI_UfeCoreHwConfigDisplay(&system_registry->hw_config, wufe_id);

   WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_SystemCoreDisplay(system_registry), wufe_id);

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE3)
   {
      WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                     WUFEI_SbiClockTranslatorDisplay(system_registry),
                                     wufe_id);

      WUFEI_HwClockRecInclkMuxSelDisplay(wufe_id);
   }

   WUFEI_ClockRecSystemDisplay(system_registry);

   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);

   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_SystemDelete
 *
 * Purpose:   release the UFE memory
 *
 *
 * Description:
 *     This function releases all the memory allocated.
 *
 *
 * Inputs:
 *     wufe_id     The UFE identifier.
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     WUFE_SystemDelete, WUFE_UfeRelease
 *
 ****************************************************************************/
WUFE_status WUFEI_SystemDelete(WP_U32 wufe_id)
{
   WUFE_status status;
   WUFEI_system_registry *system_registry;

   system_registry = WUFEI_SystemRegistryGet(wufe_id);

   /*already deleted*/
   if(!system_registry)
      return WUFE_OK;

   WUFEI_RETURN_IF_ERROR(status, WUFEI_LineSystemDelete(system_registry));
   WUFEI_RETURN_IF_ERROR(status, WUFEI_PhySystemDelete(system_registry));

   if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
   {
      /* Release memory allocate for clock recovery structure */
      WUFEI_RETURN_IF_ERROR(status, WUFEI_ClockRecSystemDelete(system_registry));

      /* Free clock translator objects*/
      WUFEI_RETURN_IF_ERROR(status, WUFEI_SbiClockTranslatorDelete(system_registry));

      /* Release memory allocate for hw_config clk_rec structure */
      WUFEI_RETURN_IF_ERROR(status, WUFEI_UfeCoreHwConfigDelete(&system_registry->hw_config));
   }

   /* clear the  the allocated  MEM block from the registry */
   WUFEI_RETURN_IF_ERROR(status, WUFEI_SystemeRegistrySet(system_registry->wufe_id,NULL));

   WUFEI_FREE(system_registry);

   return WUFE_OK;

}

/*****************************************************************************
 *
 * Function:  WUFE_SystemDelete
 *
 * Purpose:   release the UFE memory
 *
 *
 * Description:
 *     This function releases all the memory allocated.
 *
 *
 * Inputs:
 *     wufe_id     The UFE identifier.
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     The Application
 *
 ****************************************************************************/
WUFE_status WUFE_SystemDelete(WP_U32 wufe_id)
{
   WUFE_status status;

   WUFEI_system_registry *system_registry;

   /* lock the wufe_id UFE*/
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

   if(!system_registry)
   {
      WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
      return WUFE_OK;
   }

   if(system_registry->sys_enable.state_mode != WUFEI_SYSTEM_DISABLE)
   {
      if(system_registry->sys_enable.state_mode != WUFEI_SYSTEM_ENABLE)
      {
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         WUFE_PRINT_ERR((wufei_log,
                         "WUFE_SystemDelete sys_enable.state_mode =%d  \n",
                         system_registry->sys_enable.state_mode));
         return WUFE_ERR_INVALID_REGISTRY_STATE;
      }
      else
      {
         WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
         return WUFE_ERR_SYSTEM_NOT_DISABLED;
      }
   }

   WUFEI_IF_ERROR_RETURN_DETACH_W(status, WUFEI_SystemDelete(wufe_id),wufe_id);

   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
   return WUFE_OK;

}

/*****************************************************************************
 *
 * Function:  WUFE_SystemCheckFbp
 *
 * Purpose:   check the ZBT BDs link list
 *
 *
 * Description:
 *
 *
 *
 * Inputs:
 *     wufe_id     The UFE identifier.
 *     direction      WUFE_RX_DIRECTION =1,or   WUFE_TX_DIRECTION =2,
 *
 * Outputs:
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *
 * Called by:
 *     The Application
 *
 ****************************************************************************/
WUFE_status WUFE_SystemCheckFbp(WP_U32 wufe_id, WP_U32 direction )
{
   WUFE_status status;
   WUFEI_system_registry *system_registry;
   WP_U32 rx_fbp_size, tx_fbp_size, tx_phy_num, rx_phy_num;
   WP_U16 lastBd,tmpBd, count ;
   WP_U32 ii, max_n_phys_per_core;
   WUFEI_hw_fifom fifo_registers;
   WUFEI_hw_fifom *fifo_ptr;
   WP_U32 core_id;

   memset(&fifo_registers, 0, sizeof(WUFEI_hw_fifom));

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      WUFE_PRINT_ERR((wufei_log, "UFE4 Message: WUFE_SystemCheckFbp is relevant for UFE3 only\n"));
      return WUFE_OK;
   }

   /* lock the wufe_id UFE*/
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

   max_n_phys_per_core = system_registry->max_phy_per_core;

   for (core_id=0; core_id < WUFE_MAX_N_CORE; core_id++)
   {
      WP_U32 id = WUFEI_ID(wufe_id, core_id);

      rx_fbp_size=system_registry->rx_fbp_size[core_id];
      tx_fbp_size=system_registry->tx_fbp_size[core_id];

      /* create local values*/
      if (system_registry->cfg.max_phy_number >= max_n_phys_per_core)
      {
         rx_phy_num  = max_n_phys_per_core;
         tx_phy_num  = max_n_phys_per_core;
      }
      else
      {
         rx_phy_num  = system_registry->cfg.max_phy_number;
         tx_phy_num  = system_registry->cfg.max_phy_number;
      }
      /* set parameters accoreding to Directiuon */
      if(direction == WUFE_RX_DIRECTION)
      {
         count = rx_fbp_size - rx_phy_num;
         fifo_ptr=(WUFEI_hw_fifom *)WUFEI_FIFOM(WUFEI_RX);
      }
      else if (direction == WUFE_TX_DIRECTION)
      {
         count = tx_fbp_size - tx_phy_num;
         fifo_ptr=(WUFEI_hw_fifom *)WUFEI_FIFOM(WUFEI_TX);
      }
      else
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFE_ERR_INVALID_DIRECTION,
                                        wufe_id);

      /* get counter */
      WUFEI_MEM_16_GET(id,
                       &(fifo_ptr->fnfb),
                       &(fifo_registers.fnfb));

      WUFE_PRINT_DBG((wufei_log,
                      "WUFE_SystemCheckFbp: core %d register count %d =? Driver init count %d\n",
                      core_id,
                      fifo_registers.fnfb,
                      count));

      if (fifo_registers.fnfb != count)
      {
         WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                        WUFE_ERR_FIFO_INCONSIST_FBP_COUNT,
                                        wufe_id);
      }

      /* get first */
      WUFEI_MEM_16_GET(id,
                       &(fifo_ptr->fnwp),
                       &fifo_registers.fnwp);

      tmpBd  = fifo_registers.fnwp;

      /* get last */
      WUFEI_MEM_16_GET(id,
                       &(fifo_ptr->fllp),
                       &fifo_registers.fllp);

      lastBd = fifo_registers.fllp;

      WUFE_PRINT_DBG((wufei_log,
                      "WUFE_SystemCheckFbp: core %d first BD 0x%04X   last BD 0x%04X\n",
                      core_id,
                      tmpBd,
                      lastBd));

      for (ii=0;ii<count;ii++)
      {
         WP_U32 tmpPtr =
            (tmpBd *
             WUFEI_INDRCT_ADDR_ABS_TO_DRV *
             2 /* number of 16 bit word to point the rigth BD */) +
            2 /* offset to the BD.next filed */;
         WP_U16 value_ptr=0;

         WUFEI_INDIRECT_ADDRESS_SET(id, (WUFEI_INDIRECT_MEM_ADD_EXT_FIFO_START+tmpPtr));
         WUFEI_INDIRECT_DATA_16_GET(id, &(value_ptr));

         if(ii != count -2U)
         {
            /* check the next BD is not the first or the last
               i.e. check for loops */
            if (fifo_registers.fnwp == value_ptr ||
                fifo_registers.fllp == value_ptr )
            {/**/
               WUFE_PRINT_ERR((wufei_log, "%d) core %d inderect address 0x%X   ",
                               ii,core_id, WUFEI_INDIRECT_MEM_ADD_EXT_FIFO_START+tmpPtr));
               WUFE_PRINT_ERR((wufei_log, "tmpBd 0x%04x = value_ptr 0x%04X\n",tmpBd, value_ptr));
               WUFE_PRINT_ERR((wufei_log,
                               "\ncount=%d ii=%d fifo_registers.fnwp 0x%X == value_ptr 0x%X\n",
                               count,ii,fifo_registers.fnwp, value_ptr));
               WUFE_PRINT_ERR((wufei_log,
                               "count=%d ii=%d fifo_registers.fllp 0x%X == value_ptr 0x%X\n",
                               count,ii,fifo_registers.fllp, value_ptr));
               WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                              WUFE_ERR_FIFO_INCONSIST_FBP_PTR,
                                              wufe_id);
            }

            /* change the pointer to the next BD. */
            tmpBd = value_ptr;
         }
         else
         {
            tmpBd = value_ptr;

            WUFE_PRINT_DBG((wufei_log,
            "WUFE_SystemCheckFbp: core %d register-last 0x%04X =? 0x%04X link-list-last\n",
                            core_id,
                            lastBd,
                            value_ptr));

            if (lastBd != tmpBd)
            {
               WUFE_PRINT_ERR((wufei_log, "%d) core %d inderect address 0x%X   ",
                               ii,core_id,WUFEI_INDIRECT_MEM_ADD_EXT_FIFO_START+tmpPtr));
               WUFE_PRINT_ERR((wufei_log, "tmpBd 0x%04x = value_ptr 0x%04X\n",tmpBd, value_ptr));

               WUFEI_IF_ERROR_RETURN_DETACH_W(status,
                                              WUFE_ERR_FIFO_INCONSIST_FBP_PTR,
                                              wufe_id);
            }
            WUFE_PRINT_DBG((wufei_log, "WUFE_SystemCheckFbp: core %d check passed\n", core_id));
         }
      }
   }

   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
   return WUFE_OK;

}

static WUFE_status WUFEI_SystemSbiSpeLineCheck(WUFEI_system_registry* system_registry, WP_U32 spe)
{
   WP_U32 i, line;

   line = spe * WUFE_MAX_N_LINE_PER_SPE;
   for (i = line; i< line+WUFE_MAX_N_LINE_PER_SPE; i++)
   {
      if (system_registry->line_arr[i].state_mode != WUFEI_LINE_EMPTY)
         return WUFE_ERR_SYSTEM_RECONFIG_SBI_SPE_LINES_EXIST;
   }

   return WUFE_OK;
}

static WUFE_status WUFEI_SystemSbiReconfigure(WUFEI_system_registry* system_registry,
                                              WUFE_sbi_global_reconfig *config)
{
   WP_U32 entry;
   WUFE_status status;

   WUFEI_RETURN_IF_ERROR(status,
                         WUFEI_HwSbiGmrReconfigure(system_registry, config));

   for (entry=0; entry < WUFE_MAX_N_SBI_SPE; entry++)
   {
      if (config->transfer_type_spe[entry] != WUFE_NO_CHANGE)
      {
         WP_U32 coreid, stm1, spe;

         coreid = entry / (WUFE_MAX_N_STM1*WUFE_MAX_N_SPE);
         stm1 = (entry % (WUFE_MAX_N_STM1*WUFE_MAX_N_SPE)) / WUFE_MAX_N_SPE;
         spe = entry % WUFE_MAX_N_SPE;

         /* check that all lines of this spe aren't exist */
         WUFEI_RETURN_IF_ERROR(status, WUFEI_SystemSbiSpeLineCheck(system_registry, entry));
         /* change the spe type */
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_HwSbiSpeModeReconfigure(system_registry->wufe_id,
                                                             coreid,stm1,spe,
                                                             config->transfer_type_spe[entry]));
         system_registry->cfg.sbi.transfer_type_spe[entry] =
            config->transfer_type_spe[entry];
      }
   }

   return WUFE_OK;
}

static WUFE_status WUFEI_SystemIdlePatternReconfigure(WUFEI_system_registry* system_registry,
                                                      WUFE_idle_pattern *config)
{
   WP_U32 coreid;
   WUFE_status status=0;

   if (config->tx_idle_nibble_cas_pattern > 0xf)
      return WUFE_ERR_ILLEGAL_CAS_PATTERN;

   system_registry->cfg.idle_pattern.tx_idle_byte_data_pattern =
      config->tx_idle_byte_data_pattern;

   system_registry->cfg.idle_pattern.tx_idle_nibble_cas_pattern =
      config->tx_idle_nibble_cas_pattern;

   for(coreid = 0; coreid < WUFE_MAX_N_CORE; coreid++)
   {
      if(system_registry->hw_config.core_enabled[coreid])
      {
         WUFEI_RETURN_IF_ERROR(status,
                               WUFEI_SystemCasTdmSetup(system_registry, coreid));
      }
   }
   return WUFE_OK;

}

static WUFE_status WUFEI_SystemBuildPersonalityReconfigure(WUFEI_system_registry* system_registry,
                                                           WUFE_framer_build_personality config)
{
   if (config > WUFE_FRAMER_PERSONALITY_448_8_OC3)
      return WUFE_ERR_ILLEGAL_BUILD_PERSONALITY;

   system_registry->cfg.build_personality = config;

   return WUFE_OK;
}

WUFE_status WUFE_SystemReconfigure(WP_U32 wufe_id, WP_U32 command, void *config)
{
   WUFEI_system_registry *system_registry;
   WUFE_status status;

   /* lock the wufe_id UFE*/
   WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id, &system_registry,status);

   switch (command)
   {
      case WUFE_SYSTEM_RECONFIG_SBI:
         status = WUFEI_SystemSbiReconfigure(system_registry,
                                             (WUFE_sbi_global_reconfig *)config);
         break;
      case WUFE_SYSTEM_RECONFIG_IDLE_PATTERN:
         status = WUFEI_SystemIdlePatternReconfigure(system_registry,
                                                     (WUFE_idle_pattern *)config);
         break;
      case WUFE_SYSTEM_RECONFIG_FM_LOOPBACK:
         status = WUFEI_HwFmLoopbackReconfigure(system_registry,
                                                (WUFE_fifo_manager *)config);
         break;
      case WUFE_SYSTEM_RECONFIG_BUILD_PERSONALITY:
         status = WUFEI_SystemBuildPersonalityReconfigure(system_registry,
                                                          (WUFE_framer_build_personality) config);
         break;
      default:
         status = WUFE_ERR_SYSTEM_RECONFIG_CMD;
   }
   
   WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id);
   return status;
}

WUFE_framer_type WUFE_SystemFramerTypeGet(WP_U32 wufe_id)
{
   WUFE_framer_type framer_type;
   ((void)wufe_id);   
   if (WUFEI_FPGA_TYPE != WUFEI_FPGA_TYPE_UFE4)
   {
      WUFE_PRINT_ERR((wufei_log,"WUFE_SystemFramerType is relevant for UFE4 only\n"));
      return WUFE_FRAMER_TYPE_UNUSED;
   }

   framer_type = WUFEI_UfeFramerTypeGet(wufe_id);
   return framer_type;
}

WP_U32 WUFEI_SystemHwConfigLiuSbi(WUFEI_system_registry *system_registry)
{
   if (system_registry->hw_config.line_interface == WUFEI_GEN_CFG_SYS_REG_LIFM_SBI)
      return WUFE_TRUE;
   else
      return WUFE_FALSE;
}

WP_U32 WUFEI_SystemHwConfigLiuCad(WUFEI_system_registry *system_registry)
{
   if (system_registry->hw_config.line_interface == WUFEI_GEN_CFG_SYS_REG_LIFM_CAD)
      return WUFE_TRUE;
   else
      return WUFE_FALSE;
}

WP_U32 WUFEI_SystemHwConfigFullModel(WUFEI_system_registry *system_registry)
{
   if (system_registry->hw_config.full_ufe_model == WUFEI_GEN_CFG_SYS_REG_HM_FULL)
      return WUFE_TRUE;
   else
      return WUFE_FALSE;
}

WP_U32 WUFEI_SystemHwConfigAtmMachineValid(WUFEI_system_registry *system_registry, WP_U32 coreid)
{
   if (system_registry->hw_config.state_machine[coreid] & WUFEI_GEN_CFG_SYS_REG_MACHINE_ATM)
      return WUFE_TRUE;
   else
      return WUFE_FALSE;
}

WP_U32 WUFEI_SystemHwConfigHdlcMachineValid(WUFEI_system_registry *system_registry, WP_U32 coreid)
{
   if (system_registry->hw_config.state_machine[coreid] & WUFEI_GEN_CFG_SYS_REG_MACHINE_HDLC)
      return WUFE_TRUE;
   else
      return WUFE_FALSE;
}

WP_U32 WUFEI_SystemHwConfigPosMachineValid(WUFEI_system_registry *system_registry, WP_U32 coreid)
{
   ((void)system_registry);
   ((void)coreid);   
#if 0
   if (system_registry->hw_config.state_machine[coreid] & WUFEI_GEN_CFG_SYS_REG_MACHINE_POS)
      return WUFE_TRUE;
   else
      return WUFE_FALSE;
#else
   /* this functionality is not implemented yet */
   return WUFE_TRUE;
#endif
}

WP_U32 WUFEI_SystemHwConfigTransMachineValid(WUFEI_system_registry *system_registry,
                                             WP_U32 coreid)
{
   if (system_registry->hw_config.state_machine[coreid] & WUFEI_GEN_CFG_SYS_REG_MACHINE_TRANS)
      return WUFE_TRUE;
   else
      return WUFE_FALSE;
}

WUFE_status WUFE_DeviceTypeSet(WP_U32 ufeid, WUFE_device_type device_type)
{
   
   WUFE_status status;

   if (WUFEI_FPGA_TYPE == WUFEI_FPGA_TYPE_UFE4)
   {
      if (!((WUFE_DEVICE_TYPE_UFE412_T1 <= device_type)&& (WUFE_DEVICE_TYPE_UFE448_M48 >= device_type)))
      {
         return WUFE_ERR_INVALID_DEVICE_TYPE;
      }
      
      WUFEI_RETURN_IF_ERROR(status, WUFEI_DeviceTypeSet(ufeid,device_type));
   }
   
   return WUFE_OK;
}




