/*****************************************************************************
 * (C) Copyright 2002-2006, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/************************************************************************/
/*                      TDM CARD INITIALIZATION                         */
/************************************************************************/

/* This initialization code assumes that the host port for the
   QuadFALC/OctalFALC is implemented as a 64-bit port. */

#include <stdio.h>


#ifndef WPI_WDDI_H
#include "include/core/registry/wpi_wddi.h"
#endif

#ifndef WPI_TYPES_H
#include "include/core/registry/wpi_types.h"
#endif

#ifndef WPI_PTP_H
#include "include/iw/ptp/wpi_ptp.h"
#endif

#ifndef WPX_PTP_H
#include "wpx_ptp.h"
#endif

#include "veneer/wpi_board_data_int.h"



static WPTPI_registry ptp_registry = {{0}};


WPTP_status WPTP_SystemInit(WPTP_ptp_system *ptp_system_config)
{
   WPTP_status hw_status;
   WPI_WDDI_ATTACH();
   hw_status =  WPTPI_SystemInit(ptp_system_config);
   WPI_WDDI_DETACH();
   return hw_status;
}




WPTP_status WPTPI_SystemInit(WPTP_ptp_system *ptp_system_config)
{
   WPTP_status hw_status;
   WPTP_REG reg_value1 ;
   wpi_match_mask save;


#if WPTP_SET_BANK
   /* Save current match/mask (without restoring), install ptp values. */
   WPX_SetBank(&save, &wpi_board_data.match_mask->ptp);
#endif

 /* check config struct isnt null  */
   if(ptp_system_config == WPTP_NULL)
   {
      WPTPI_RETURN_ERROR(WPTP_NULL_CONFIG);
   }

 /* check max_clock_domains */
   if(ptp_system_config->max_clock_domains >= WPTP_GLOBAL_SPACE)
   {
      WPTPI_RETURN_ERROR(WPTP_MAX_WDDI_CD_ERROR);
   }

   /* Create registry */
   ptp_registry.ptp_system.ptp_hw_logic_start = ptp_system_config->ptp_hw_logic_start;
   ptp_registry.ptp_system.max_clock_domains = ptp_system_config->max_clock_domains;
   ptp_registry.ptp_system.ignore_bits       = ptp_system_config->ignore_bits;

 /* Check Release id */
  WPTPI_GEN_REG_READ(WPTP_REL_ID,&reg_value1);


  /*if(reg_value1 != WPTP_RELEASE_1 || reg_value2 != WPTP_RELEASE_2)*/
  if(reg_value1 == 0)
  {
     WPTPI_RETURN_ERROR(WPTP_RELEASE_ID_ERROR);
  }

 /* Check board supports defined number of clock domains */
   WPTPI_GEN_REG_READ(WPTP_PTP_CONFIG,&reg_value1);
   reg_value1 &= WPTP_CONFIG_TS_UNITS_M;
   if(reg_value1 < ptp_registry.ptp_system.max_clock_domains)
   {
      WPTPI_RETURN_ERROR(WPTP_HW_CD_NUM_ERROR);
   }

/*   printf("ts units  %u \n",reg_value1 ); */

    /* Soft Reset */
   WPTPI_RETURN_IF_ERROR(hw_status,WPTPI_SoftReset());


 /* Check PLLs are Locked */
   WPTPI_RETURN_IF_ERROR(hw_status, WPTPI_CheckPllLock());


   return WPTP_OK ;
}


WPTP_status WPTP_ClockDomainInit(WP_handle cd_handle, WPTP_clock_domain_config *cd_config )
{
   WPTP_status hw_status;
   WPI_WDDI_ATTACH();
   hw_status =  WPTPI_ClockDomainInit(cd_handle, cd_config);
   WPI_WDDI_DETACH();
   return hw_status;
}



WPTP_status WPTPI_ClockDomainInit(WP_handle cd_handle, WPTP_clock_domain_config *cd_config )
{
   WPTP_REG clock_status ;

   WPTP_status hw_status;
   WP_U32 cd ;

   WPTPI_RETURN_IF_ERROR(hw_status,WPTPI_CheckCDHandle(cd_handle));
   cd = WPI_PTP_HANDLE_INDEX_CLOCK_DOMAIN_V(cd_handle);


   /* check config struct isnt null  */
   if(cd_config == WPTP_NULL)
      WPTPI_RETURN_ERROR(WPTP_NULL_CONFIG);


   /* check clock status is compatible with  clock source */
   WPTPI_CD_REG_READ(WPTP_PTP_STATUS,cd,&clock_status);

   if(cd_config->clock_mode == WPTP_MODE_CLOCK_SOURCE_EXTERNAL &&
      clock_status   == WPTP_STATUS_CLOCK_MODE_SLAVE)
      WPTPI_RETURN_ERROR(WPTP_CONFIG_STATUS_ERROR);

   /* Set clock status */

   /* set clock mode input clock params */
   if(cd_config->clock_mode == WPTP_MODE_CLOCK_SOURCE_EXTERNAL)
   {
      /* EXTERNAL */
      WPTPI_CD_REG_WRITE(WPTP_PTP_MODE,cd,WPTP_MODE_CLOCK_SOURCE_EXTERNAL);
      /* set pre-divider */
      WPTPI_CD_REG_WRITE(WPTP_DIV_REG,cd,cd_config->external_pre_divider);


   }
   else if(cd_config->clock_mode == WPTP_MODE_CLOCK_SOURCE_DCO)
   {
      /* DCO */
      WPTPI_CD_REG_WRITE(WPTP_PTP_MODE,cd,WPTP_MODE_CLOCK_SOURCE_DCO);
      /* set intial dividers */
      WPTPI_CD_REG_WRITE(WPTP_DCO_INT,cd,cd_config->dco_int_nominal);
      WPTPI_CD_REG_WRITE(WPTP_DCO_REM_MSB,cd,cd_config->dco_remainder_nominal_msb);
      WPTPI_CD_REG_WRITE(WPTP_DCO_REM_LSB,cd,cd_config->dco_remainder_nominal_lsb);
   }
   else  WPTPI_RETURN_ERROR(WPTP_CONFIG_MODE_ERROR);


   /* set cycle time values (add values)  */
   WPTPI_CD_REG_WRITE(WPTP_ADD_VAL_NS_INT,cd,cd_config->cycle_time_int);
   WPTPI_CD_REG_WRITE(WPTP_ADD_VAL_NS_FRAC_MSB,cd,
                      WPTP_ADDVALUE_FRAC_MSB_F(cd_config->cycle_time_frac_msb));
   WPTPI_CD_REG_WRITE(WPTP_ADD_VAL_NS_FRAC_LSB,cd,cd_config->cycle_time_frac_lsb);

   return WPTP_OK ;
}

WPTP_status WPTP_DCODividerSet(WP_handle cd_handle,
                               WPTP_REG dco_int,
                               WPTP_REG dco_remainder_msb,
                               WPTP_REG dco_remainder_lsb)
{
   WPTP_status hw_status;
   WP_U32 cd ;
   WPI_WDDI_ATTACH();

   WPTPI_DETACH_RETURN_IF_ERROR(hw_status,WPTPI_CheckCDHandle(cd_handle));
   cd = WPI_PTP_HANDLE_INDEX_CLOCK_DOMAIN_V(cd_handle);

   WPTPI_CD_REG_WRITE(WPTP_DCO_INT,cd,dco_int);
   WPTPI_CD_REG_WRITE(WPTP_DCO_REM_MSB,cd,dco_remainder_msb);
   WPTPI_CD_REG_WRITE(WPTP_DCO_REM_LSB,cd,dco_remainder_lsb);

   WPI_WDDI_DETACH();
   return WPTP_OK ;
}


WPTP_status WPTP_ClockDomainEnable(WP_handle cd_handle)
{
   WPTP_REG clock_source ;
   WPTP_status hw_status;
   WP_U32 cd ;
   WPI_WDDI_ATTACH();

   WPTPI_DETACH_RETURN_IF_ERROR(hw_status,WPTPI_CheckCDHandle(cd_handle));
   cd = WPI_PTP_HANDLE_INDEX_CLOCK_DOMAIN_V(cd_handle);


   WPTPI_CD_REG_READ(WPTP_PTP_MODE,cd,&clock_source);

   /* enable DCO if its a DCO source */
   if(clock_source == WPTP_MODE_CLOCK_SOURCE_DCO)
      WPTPI_CD_REG_WRITE(WPTP_DCO_MODE,cd,WPTP_DCO_ENABLE_F(WPTP_DCO_ENABLED));

   /* enable PTP Ts */
   WPTPI_CD_REG_WRITE(WPTP_TS_MODE,cd,WPTP_TS_ENABLE_F(WPTP_TS_ENABLED));

   WPI_WDDI_DETACH();
   return WPTP_OK ;
}


WPTP_status WPTP_DCOEnable(WP_handle cd_handle)
{
   WPTP_status hw_status;
   WP_U32 cd ;

   WPI_WDDI_ATTACH();

   WPTPI_DETACH_RETURN_IF_ERROR(hw_status,WPTPI_CheckCDHandle(cd_handle));
   cd = WPI_PTP_HANDLE_INDEX_CLOCK_DOMAIN_V(cd_handle);


   /* enable DCO */
   WPTPI_CD_REG_WRITE(WPTP_DCO_MODE,cd, WPTP_DCO_ENABLE_F(WPTP_DCO_ENABLED));

   WPI_WDDI_DETACH();
   return WPTP_OK ;
}

WPTP_status WPTP_TsEnable(WP_handle cd_handle)
{
   WPTP_status hw_status;
   WP_U32 cd ;
   WPI_WDDI_ATTACH();

   WPTPI_DETACH_RETURN_IF_ERROR(hw_status,WPTPI_CheckCDHandle(cd_handle));
   cd = WPI_PTP_HANDLE_INDEX_CLOCK_DOMAIN_V(cd_handle);

   /* enable PTP Ts */
   WPTPI_CD_REG_WRITE(WPTP_TS_MODE,cd,WPTP_TS_ENABLE_F(WPTP_TS_ENABLED));

   WPI_WDDI_DETACH();
   return WPTP_OK ;
}


WPTP_status WPTP_ClockDomainDisable(WP_handle cd_handle)
{
   WPTP_status hw_status;
   WP_U32 cd ;
   WPI_WDDI_ATTACH();

   WPTPI_DETACH_RETURN_IF_ERROR(hw_status,WPTPI_CheckCDHandle(cd_handle));
   cd = WPI_PTP_HANDLE_INDEX_CLOCK_DOMAIN_V(cd_handle);

   /* disable PTP Ts */
   WPTPI_CD_REG_WRITE(WPTP_TS_MODE,cd,WPTP_TS_ENABLE_F(WPTP_TS_DISABLED));
   /* disable DCO */
   WPTPI_CD_REG_WRITE(WPTP_DCO_MODE,cd, WPTP_DCO_ENABLE_F(WPTP_DCO_DISABLED));

   WPI_WDDI_DETACH();
   return WPTP_OK ;
}

WPTP_status WPTP_DCODisable(WP_handle cd_handle)
{
   WPTP_status hw_status;
   WP_U32 cd ;
   WPI_WDDI_ATTACH();

   WPTPI_DETACH_RETURN_IF_ERROR(hw_status,WPTPI_CheckCDHandle(cd_handle));
   cd = WPI_PTP_HANDLE_INDEX_CLOCK_DOMAIN_V(cd_handle);

   /* disable DCO */
   WPTPI_CD_REG_WRITE(WPTP_DCO_MODE,cd, WPTP_DCO_ENABLE_F(WPTP_DCO_DISABLED));

   WPI_WDDI_DETACH();
   return WPTP_OK ;
}

WPTP_status WPTP_TsDisable(WP_handle cd_handle)
{
   WPTP_status hw_status;
   WP_U32 cd ;
   WPI_WDDI_ATTACH();
   WPTPI_DETACH_RETURN_IF_ERROR(hw_status,WPTPI_CheckCDHandle(cd_handle));
   cd = WPI_PTP_HANDLE_INDEX_CLOCK_DOMAIN_V(cd_handle);

   /* disable PTP Ts */
   WPTPI_CD_REG_WRITE(WPTP_TS_MODE,cd,WPTP_TS_ENABLE_F(WPTP_TS_DISABLED));
   WPI_WDDI_DETACH();
   return WPTP_OK ;
}




WPTP_status WPTP_TSRead(WP_handle cd_handle,
                        WPTP_REG *sec_msb,
                        WPTP_REG *sec_mid,
                        WPTP_REG *sec_lsb,
                        WPTP_REG *nsec_msb,
                        WPTP_REG *nsec_lsb)
{
   WPTP_status hw_status;
   WP_U32 cd ;
   WPI_WDDI_ATTACH();
   WPTPI_DETACH_RETURN_IF_ERROR(hw_status,WPTPI_CheckCDHandle(cd_handle));
   cd = WPI_PTP_HANDLE_INDEX_CLOCK_DOMAIN_V(cd_handle);

 /* Must read from SEC MSB first because it latches the 5 regs with the latest Ts */
   WPTPI_CD_REG_READ(WPTP_TIMESTAMP_SEC_MSB,cd,sec_msb);
   WPTPI_CD_REG_READ(WPTP_TIMESTAMP_SEC_MID,cd,sec_mid);
   WPTPI_CD_REG_READ(WPTP_TIMESTAMP_SEC_LSB,cd,sec_lsb);
   WPTPI_CD_REG_READ(WPTP_TIMESTAMP_NSEC_MSB,cd,nsec_msb);
   WPTPI_CD_REG_READ(WPTP_TIMESTAMP_NSEC_LSB,cd,nsec_lsb);

   WPI_WDDI_DETACH();
   return WPTP_OK ;
}


WPTP_status WPTP_SetOffsetAbsolute(WP_handle cd_handle,
                                   WPTP_REG sec_msb,
                                   WPTP_REG sec_mid,
                                   WPTP_REG sec_lsb,
                                   WPTP_REG nsec_msb,
                                   WPTP_REG nsec_lsb)
{
   WPTP_REG hw_status ;
   WP_U32 cd ;

   WPI_WDDI_ATTACH();
   WPTPI_DETACH_RETURN_IF_ERROR(hw_status,WPTPI_CheckCDHandle(cd_handle));
   cd = WPI_PTP_HANDLE_INDEX_CLOCK_DOMAIN_V(cd_handle);

 /* set ABSOLUTE mode */
   WPTPI_CD_REG_WRITE(WPTP_OFFSET_MODE,cd,WPTP_TS_OFFSET_MODE_F(WPTP_TS_OFFSET_MODE_ABSOLUTE));

 /* Must write to NSEC LSB last because it latches the 5 regs with the input */
   WPTPI_CD_REG_WRITE(WPTP_OFFSET_SEC_MSB,cd,sec_msb);
   WPTPI_CD_REG_WRITE(WPTP_OFFSET_SEC_MID,cd,sec_mid);
   WPTPI_CD_REG_WRITE(WPTP_OFFSET_SEC_LSB,cd,sec_lsb);
   WPTPI_CD_REG_WRITE(WPTP_OFFSET_NSEC_MSB,cd,nsec_msb);
   WPTPI_CD_REG_WRITE(WPTP_OFFSET_NSEC_LSB,cd,nsec_lsb);


 /* wait for done */
   WPTPI_DETACH_RETURN_IF_ERROR(hw_status,WPTPI_CheckOffsetDone(cd));
   WPI_WDDI_DETACH();
   return WPTP_OK ;
}



WPTP_status WPTP_SetOffsetAbsoluteONPPS(WP_handle cd_handle,
                                        WPTP_REG sec_msb,
                                        WPTP_REG sec_mid,
                                        WPTP_REG sec_lsb,
                                        WPTP_REG nsec_msb,
                                        WPTP_REG nsec_lsb)
{
   WPTP_REG hw_status ;
   WP_U32 cd ;
   WPI_WDDI_ATTACH();
   WPTPI_DETACH_RETURN_IF_ERROR(hw_status,WPTPI_CheckCDHandle(cd_handle));
   cd = WPI_PTP_HANDLE_INDEX_CLOCK_DOMAIN_V(cd_handle);
   
 /* set ABSOLUTE ON PPS mode */
   WPTPI_CD_REG_WRITE(WPTP_OFFSET_MODE,cd,WPTP_TS_OFFSET_ABS_ONPPS_MODE_F(WPTP_TS_OFFSET_ABS_ONPPS_SET));

 /* Must write to NSEC LSB last because it latches the 5 regs with the input */
   WPTPI_CD_REG_WRITE(WPTP_OFFSET_SEC_MSB,cd,sec_msb);
   WPTPI_CD_REG_WRITE(WPTP_OFFSET_SEC_MID,cd,sec_mid);
   WPTPI_CD_REG_WRITE(WPTP_OFFSET_SEC_LSB,cd,sec_lsb);
   WPTPI_CD_REG_WRITE(WPTP_OFFSET_NSEC_MSB,cd,nsec_msb);
   WPTPI_CD_REG_WRITE(WPTP_OFFSET_NSEC_LSB,cd,nsec_lsb);

   
 /* wait for done */
   WPTPI_DETACH_RETURN_IF_ERROR(hw_status,WPTPI_CheckOffsetOnPPSDone(cd));
   WPI_WDDI_DETACH();
   return WPTP_OK ;
}





WPTP_status WPTP_SetOffsetAddative(WP_handle cd_handle,
                                   WP_U8    add_or_sub,
                                   WPTP_REG sec_msb,
                                   WPTP_REG sec_mid,
                                   WPTP_REG sec_lsb,
                                   WPTP_REG nsec_msb,
                                   WPTP_REG nsec_lsb)
{
   WPTP_REG hw_status ;
   WP_U32 cd ;
   WPI_WDDI_ATTACH();
   WPTPI_DETACH_RETURN_IF_ERROR(hw_status,WPTPI_CheckCDHandle(cd_handle));
   cd = WPI_PTP_HANDLE_INDEX_CLOCK_DOMAIN_V(cd_handle);


 /* set ADDATIVE mode */
   WPTPI_CD_REG_WRITE(WPTP_OFFSET_MODE,cd,WPTP_TS_OFFSET_MODE_F(WPTP_TS_OFFSET_MODE_ADDATIVE));
 /* set add/sub */
   WPTPI_CD_REG_WRITE(WPTP_OFFSET_MODE,cd,WPTP_TS_OFFSET_ADD_MODE_F(add_or_sub));

 /* Must write to NSEC LSB last because it latches the 5 regs with the input */
   WPTPI_CD_REG_WRITE(WPTP_OFFSET_SEC_MSB,cd,sec_msb);
   WPTPI_CD_REG_WRITE(WPTP_OFFSET_SEC_MID,cd,sec_mid);
   WPTPI_CD_REG_WRITE(WPTP_OFFSET_SEC_LSB,cd,sec_lsb);
   WPTPI_CD_REG_WRITE(WPTP_OFFSET_NSEC_MSB,cd,nsec_msb);
   WPTPI_CD_REG_WRITE(WPTP_OFFSET_NSEC_LSB,cd,nsec_lsb);


   WPTPI_DETACH_RETURN_IF_ERROR(hw_status,WPTPI_CheckOffsetDone(cd));
   WPI_WDDI_DETACH();
   return WPTP_OK ;
}


WPTP_status WPTP_SystemDisplay()
{
   WPTP_REG temp;

   printf("\n\nSystemDisplay():\n");
   printf("-------------------------------\n");

   /* Registry */
   printf("ptp_hw_logic_start:    0x%08x\n",ptp_registry.ptp_system.ptp_hw_logic_start);
   printf("max_clock_domains:    0x%08x\n",ptp_registry.ptp_system.max_clock_domains);
   printf("ignore_bits:          0x%08x\n\n",ptp_registry.ptp_system.ignore_bits);


   /* release ID */
   WPTPI_GEN_REG_READ(WPTP_REL_ID,&temp);
   printf("RELEASE ID:           %u\n",temp);
   /* release ID Sec */
   WPTPI_GEN_REG_READ(WPTP_SEC_REL_ID,&temp);
   printf("SEC RELEASE ID:       %u\n",temp);
   /* config - max number of clock domains */
   WPTPI_GEN_REG_READ(WPTP_PTP_CONFIG,&temp);
   printf("MAX TS UNITS:         %u\n",WPTP_CONFIG_TS_UNITS_V(temp));
   /* WPTP_SOFT_RESET */
   WPTPI_GEN_REG_READ(WPTP_SOFT_RESET,&temp);
   printf("SOFT RESET:           %u\n",WPTP_SOFT_RESET_V(temp));
   /* PLL status */
   WPTPI_GEN_REG_READ(WPTP_PLL_STAT,&temp);
   printf("SYSTEM PLL LOCKED:    %u\n",WPTP_PLL_SYS_LOCK_V(temp));
   printf("DCO    PLL LOCKED:    %u\n",WPTP_PLL_DCO_LOCK_V(temp));
   printf("\n\n");


   return WPTP_OK ;
}

WPTP_status WPTP_SoftReset()
{
   WPTP_status hw_status;
   WPI_WDDI_ATTACH();
   hw_status =  WPTPI_SoftReset();
   WPI_WDDI_DETACH();
   return hw_status;
}



WPTP_status WPTPI_SoftReset()
{
   WPTP_REG reg_value1 ;
   /* Soft Reset */


   WPTPI_GEN_REG_WRITE(WPTP_SOFT_RESET,WPTP_SOFT_RESET_F(WPTP_RESET));
   WPTPI_GEN_REG_READ(WPTP_SOFT_RESET,&reg_value1);
   if(WPTP_SOFT_RESET_V(reg_value1) != WPTP_RESET)
      WPTPI_RETURN_ERROR(WPTP_RESET_ERROR);


   WPTPI_Delay(WPTP_RESET_DELAY);


   WPTPI_GEN_REG_WRITE(WPTP_SOFT_RESET,WPTP_SOFT_RESET_F(WPTP_UNRESET));


   WPTPI_GEN_REG_READ(WPTP_SOFT_RESET,&reg_value1);
   if(WPTP_SOFT_RESET_V(reg_value1) != WPTP_UNRESET)
      WPTPI_RETURN_ERROR(WPTP_UNRESET_ERROR);

   return WPTP_OK ;
}


WPTP_status WPTP_CheckPllLock()
{
   WPTP_status hw_status;
   WPI_WDDI_ATTACH();
   hw_status =  WPTPI_CheckPllLock();
   WPI_WDDI_DETACH();
   return hw_status;
}


WPTP_status WPTPI_CheckPllLock()
{
   WPTP_REG reg_value1 ;
   WP_U32 timeout ,timeout_counter ;

   timeout = WPTP_PLL_LOCK_TIMEOUT;
   timeout_counter = timeout;
   do
   {
      WPTPI_GEN_REG_READ(WPTP_PLL_STAT,&reg_value1);
      if((WPTP_PLL_SYS_LOCK_V(reg_value1) == WPTP_PLL_LOCKED ||
          WPTP_PLL_DCO_LOCK_V(reg_value1) == WPTP_PLL_LOCKED))
         break;

      if(timeout_counter == timeout)
         if(debug_flag) printf("waiting for pll to lock....\n");

      WPTPI_Delay(WPTP_PLL_LOCK_DELAY) ;
      if(--timeout_counter == 0 ) WPTPI_RETURN_ERROR(WPTP_PLL_LOCK_ERROR) ;
   }
   while(1);

   return WPTP_OK;
}

 WPTP_status WPTPI_CheckOffsetDone(WP_U32 cd)
{
   WPTP_REG reg_value1 ;
   WP_U32 timeout ,timeout_counter ;

   timeout = WPTP_OFFSET_TIMEOUT;
   timeout_counter = timeout;
   do
   {
      WPTPI_CD_REG_READ(WPTP_OFFSET_STATUS,cd,&reg_value1);
      if(WPTP_OFFSET_STATUS_DONE_V(reg_value1) == WPTP_OFFSET_STATUS_DONE)
         break;

      if(timeout_counter == timeout)
         if(debug_flag) printf("waiting for offset done....\n");

      /* wait  */
      WPTPI_Delay(WPTP_OFFSET_DELAY) ;
      if(--timeout_counter == 0 ) WPTPI_RETURN_ERROR(WPTP_OFFSET_TIMEOUT_ERROR) ;
   }
   while(1);

   return WPTP_OK;
}

WPTP_status WPTPI_CheckOffsetOnPPSDone(WP_U32 cd)
{
   WPTP_REG reg_value1 ;
   WP_U32 timeout ,timeout_counter ;

   timeout = WPTP_OFFSET_ON_PPS_TIMEOUT;
   timeout_counter = timeout;
   do
   {
      /* wait  */
      WPTPI_Delay(WPTP_ONE_SEC_DELAY) ;

      WPTPI_CD_REG_READ(WPTP_OFFSET_STATUS,cd,&reg_value1);
      if(WPTP_OFFSET_ONPPS_STATUS_DONE_V(reg_value1) == WPTP_OFFSET_ONPPS_STATUS_DONE)
         break;

      if(timeout_counter == timeout)
         if(debug_flag) printf("waiting for offset on pps done....\n");

      if(--timeout_counter == 0 ) WPTPI_RETURN_ERROR(WPTP_OFFSET_ONPPS_TIMEOUT_ERROR) ;
   }
   while(1);

   return WPTP_OK;
}



void WPTP_REG_WRITE(WPTP_REG reg, WP_U8 cd_or_gen,WPTP_REG value)
{
 /* note the address space this function assumes and the user resposibility */


   /* if this write is to a global register */
   if(cd_or_gen == WPTP_GLOBAL_SPACE)
      WPTPI_GEN_REG_WRITE(reg,value);
   else
      WPTPI_CD_REG_WRITE(reg,cd_or_gen,value);
}

void WPTP_REG_READ(WPTP_REG reg, WP_U8 cd_or_gen,WPTP_REG *value_ptr)
{
 /* note the address space this function assumes and the user resposibility */


   /* if this read is from a global register */
     if(cd_or_gen == WPTP_GLOBAL_SPACE)
         WPTPI_GEN_REG_READ(reg,value_ptr);
      else
         WPTPI_CD_REG_READ(reg,cd_or_gen,value_ptr);
}




WPTP_status WPTP_ClockDomainDisplay(WP_handle cd_handle)
{
   WPTP_REG temp;
   WPTP_status hw_status;
   WP_U32 cd ;
   WPI_WDDI_ATTACH();
   WPTPI_DETACH_RETURN_IF_ERROR(hw_status,WPTPI_CheckCDHandle(cd_handle));
   cd = WPI_PTP_HANDLE_INDEX_CLOCK_DOMAIN_V(cd_handle);

   printf("ClockDomainDisplay() %d\n",cd);
   printf("Handle = 0x%08x   HW instance No. %u :\n",cd_handle,cd);
/* ptp N global */
/*WPTP_PTP_MODE*/
   WPTPI_CD_REG_READ(WPTP_PTP_MODE,cd,&temp);
   printf("WPTP_PTP_MODE             =  %04x\n",temp);
/*WPTP_PTP_STATUS*/
   WPTPI_CD_REG_READ(WPTP_PTP_STATUS,cd,&temp);
   printf("WPTP_PTP_STATUS           =  %04x\n",temp);
/*WPTP_TS_MODE*/
   WPTPI_CD_REG_READ(WPTP_TS_MODE,cd,&temp);
   printf("WPTP_TS_MODE              =  %04x\n",temp);
/*WPTP_ADD_VAL_NS_INT*/
   WPTPI_CD_REG_READ(WPTP_ADD_VAL_NS_INT,cd,&temp);
   printf("WPTP_ADD_VAL_NS_INT       =  %04x\n",temp);
/*WPTP_ADD_VAL_NS_FRAC_MSB*/
   WPTPI_CD_REG_READ(WPTP_ADD_VAL_NS_FRAC_MSB,cd,&temp);
   printf("WPTP_ADD_VAL_NS_FRAC_MSB  =  %04x\n",temp);
/*WPTP_ADD_VAL_NS_FRAC_LSB*/
   WPTPI_CD_REG_READ(WPTP_ADD_VAL_NS_FRAC_LSB,cd,&temp);
   printf("WPTP_ADD_VAL_NS_FRAC_LSB  =  %04x\n",temp);


/* Ts  - read only */
/*"WPTP_TIMESTAMP_SEC_MSB"*/
   WPTPI_CD_REG_READ(WPTP_TIMESTAMP_SEC_MSB,cd,&temp);
   printf("WPTP_TIMESTAMP_SEC_MSB    =  %0x\n",temp);
/*"WPTP_TIMESTAMP_SEC_MID"*/
   WPTPI_CD_REG_READ(WPTP_TIMESTAMP_SEC_MID,cd,&temp);
   printf("WPTP_TIMESTAMP_SEC_MID    =  %0x\n",temp);
/*"WPTP_TIMESTAMP_SEC_LSB"*/
   WPTPI_CD_REG_READ(WPTP_TIMESTAMP_SEC_LSB,cd,&temp);
   printf("WPTP_TIMESTAMP_SEC_LSB    =  %0x\n",temp);
/*"WPTP_TIMESTAMP_NSEC_MSB"*/
   WPTPI_CD_REG_READ(WPTP_TIMESTAMP_NSEC_MSB,cd,&temp);
   printf("WPTP_TIMESTAMP_NSEC_MSB   =  %0x\n",temp);
/*"WPTP_TIMESTAMP_NSEC_LSB"*/
   WPTPI_CD_REG_READ(WPTP_TIMESTAMP_NSEC_LSB,cd,&temp);
   printf("WPTP_TIMESTAMP_NSEC_LSB   =  %0x\n",temp);


/* Offset related */
/*WPTP_OFFSET_MODE*/
   WPTPI_CD_REG_READ(WPTP_OFFSET_MODE,cd,&temp);
   printf("WPTP_OFFSET_MODE          =  %04x\n",temp);
/*WPTP_OFFSET_STATUS*/
   WPTPI_CD_REG_READ(WPTP_OFFSET_STATUS,cd,&temp);
   printf("WPTP_OFFSET_STATUS        =  %04x\n",temp);
/*WPTP_OFFSET_SEC_MSB*/
   WPTPI_CD_REG_READ(WPTP_OFFSET_SEC_MSB,cd,&temp);
   printf("WPTP_OFFSET_SEC_MSB       =  %04x\n",temp);
/*WPTP_OFFSET_SEC_MID*/
   WPTPI_CD_REG_READ(WPTP_OFFSET_SEC_MID,cd,&temp);
   printf("WPTP_OFFSET_SEC_MID       =  %04x\n",temp);
/*WPTP_OFFSET_SEC_LSB*/
   WPTPI_CD_REG_READ(WPTP_OFFSET_SEC_LSB,cd,&temp);
   printf("WPTP_OFFSET_SEC_LSB       =  %04x\n",temp);
/*WPTP_OFFSET_NSEC_MSB*/
   WPTPI_CD_REG_READ(WPTP_OFFSET_NSEC_MSB,cd,&temp);
   printf("WPTP_OFFSET_NSEC_MSB      =  %04x\n",temp);
/*WPTP_OFFSET_NSEC_LSB*/
   WPTPI_CD_REG_READ(WPTP_OFFSET_NSEC_LSB,cd,&temp);
   printf("WPTP_OFFSET_NSEC_LSB      =  %04x\n",temp);

/* DCO related */
/*WPTP_DCO_MODE*/
   WPTPI_CD_REG_READ(WPTP_DCO_MODE,cd,&temp);
   printf("WPTP_DCO_MODE             =  %04x\n",temp);
/*WPTP_DCO_INT*/
   WPTPI_CD_REG_READ(WPTP_DCO_INT,cd,&temp);
   printf("WPTP_DCO_INT              =  %04x\n",temp);
/*WPTP_DCO_REM_MSB*/
   WPTPI_CD_REG_READ(WPTP_DCO_REM_MSB,cd,&temp);
   printf("WPTP_DCO_REM_MSB          =  %04x\n",temp);
/*WPTP_DCO_REM_LSB*/
   WPTPI_CD_REG_READ(WPTP_DCO_REM_LSB,cd,&temp);
   printf("WPTP_DCO_REM_LSB          =  %04x\n",temp);

/*WPTP_DIV_REG*/
   WPTPI_CD_REG_READ(WPTP_DIV_REG,cd,&temp);
   printf("WPTP_DIV_REG              =  %04x\n",temp);
   WPI_WDDI_DETACH();
   return WPTP_OK ;
}


WPTP_status WPTPI_ReturnError(WPTP_status error_id, WP_U32 line)
{
   WP_CHAR *WPTPI_STRINGS[WPTP_LAST_ERROR]=
      {
         "WPTP_OK" ,
         "WPTP_ERROR",
         "WPTP_RELEASE_ID_ERROR",
         "WPTP_HW_CD_NUM_ERROR",
         "WPTP_CD_HANDLE_ERROR",
         "WPTP_CD_HANDLE_INDEX_ERROR",
         "WPTP_NULL_CONFIG",
         "WPTP_MAX_WDDI_CD_ERROR",
         "WPTP_CONFIG_MODE_ERROR",
         "WPTP_CONFIG_STATUS_ERROR",
         "WPTP_RESET_ERROR",
         "WPTP_UNRESET_ERROR",
         "WPTP_OFFSET_TIMEOUT_ERROR",
         "WPTP_OFFSET_ONPPS_TIMEOUT_ERROR",
         "WPTP_PLL_LOCK_ERROR"
      };

   if(error_id >= WPTP_LAST_ERROR)
   {
      printf("\nPTP HW error in line %u - bad error id with error_id = %u  \n",line,error_id);
      return WPI_ERROR(WP_ERR_PTP_HW_GENERAL) ;
   }

   printf("PTP HW error in line %u  - %s \n",line,WPTPI_STRINGS[error_id]);
   return WPI_ERROR(WP_ERR_PTP_HW_GENERAL) ;
}


 WPTP_status WPTPI_CheckCDHandle(WP_handle cd_handle)
{
   WP_U32 cd ;

/* check handle is a CD handle */
   if(!WPI_PTP_HANDLE_CLOCK_DOMAIN_P(cd_handle))
      return WPTP_CD_HANDLE_ERROR;

   cd = WPI_PTP_HANDLE_INDEX_CLOCK_DOMAIN_V(cd_handle);


/* CD index is in range */
   if(cd > ptp_registry.ptp_system.max_clock_domains)
   {
      return WPTP_CD_HANDLE_INDEX_ERROR;
   }

   return WPTP_OK;
}
