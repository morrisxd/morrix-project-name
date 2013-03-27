/*****************************************************************************
 * (C) Copyright 2001-2006, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/************************************************************************/
/*                                             */
/************************************************************************/


#ifndef WPX_PTP_H
#define WPX_PTP_H


// #ifndef WPI_PTP_H
// #include "include/iw/ptp/wpi_ptp.h"
// #endif

#include "api/wp_types.h"
#include "../board/wpx_board_if.h"





/*************************/
/*  PTP HW DRIVER        */
/*************************/


/* Types */
typedef WP_U32 WPTP_status;
typedef WP_U16 WPTP_REG;


enum WPTP_error
{
   WPTP_OK = 0,
   WPTP_ERROR,
   WPTP_RELEASE_ID_ERROR,
   WPTP_HW_CD_NUM_ERROR,
   WPTP_CD_HANDLE_ERROR,
   WPTP_CD_HANDLE_INDEX_ERROR,
   WPTP_NULL_CONFIG,
   WPTP_MAX_WDDI_CD_ERROR,
   WPTP_CONFIG_MODE_ERROR,
   WPTP_CONFIG_STATUS_ERROR,
   WPTP_RESET_ERROR,
   WPTP_UNRESET_ERROR,
   WPTP_OFFSET_TIMEOUT_ERROR,
   WPTP_OFFSET_ONPPS_TIMEOUT_ERROR,
   WPTP_PLL_LOCK_ERROR,
   WPTP_LAST_ERROR
/*   WPTP_ERR_INVALID_...,*/
};




#define WPTP_SET_BANK  1


#define WPTPI_MAX_ERROR_CHARS_LENGTH 40

#define WPTP_NULL  0


typedef struct{
      WP_U32          ptp_hw_logic_start;
      WP_U16          max_clock_domains;
      WP_U8           ignore_bits;
} WPTP_ptp_system;


typedef struct{
      WP_U8    clock_mode;
      WP_U8    dco_int_nominal;
      WP_U16   dco_remainder_nominal_msb;
      WP_U16   dco_remainder_nominal_lsb;
      WP_U16   external_pre_divider;
      WP_U16   cycle_time_int;
      WP_U8    cycle_time_frac_msb;
      WP_U16   cycle_time_frac_lsb;
} WPTP_clock_domain_config;



typedef struct{
      WPTP_ptp_system ptp_system;
}WPTPI_registry;




/*************************/
/* API FUNCTIONS         */
/*************************/

WPTP_status WPTP_SystemInit(WPTP_ptp_system *ptp_system_config);
WPTP_status WPTP_ClockDomainInit(WP_handle cd_handle, WPTP_clock_domain_config *cd_config );
WPTP_status WPTP_ClockDomainEnable(WP_handle cd_handle);
WPTP_status WPTP_ClockDomainDisable(WP_handle cd_handle);
WPTP_status WPTP_TSRead(WP_handle cd_handle,
                        WPTP_REG *sec_msb,
                        WPTP_REG *sec_mid,
                        WPTP_REG *sec_lsb,
                        WPTP_REG *nsec_msb,
                        WPTP_REG *nsec_lsb);
WPTP_status WPTP_SetOffsetAbsolute(WP_handle cd_handle,
                                   WPTP_REG sec_msb,
                                   WPTP_REG sec_mid,
                                   WPTP_REG sec_lsb,
                                   WPTP_REG nsec_msb,
                                   WPTP_REG nsec_lsb);

WPTP_status WPTP_SetOffsetAbsoluteONPPS(WP_handle cd_handle,
                                        WPTP_REG sec_msb,
                                        WPTP_REG sec_mid,
                                        WPTP_REG sec_lsb,
                                        WPTP_REG nsec_msb,
                                        WPTP_REG nsec_lsb);


WPTP_status WPTP_SetOffsetAddative(WP_handle cd_handle,
                                   WP_U8    add_or_sub,
                                   WPTP_REG sec_msb,
                                   WPTP_REG sec_mid,
                                   WPTP_REG sec_lsb,
                                   WPTP_REG nsec_msb,
                                   WPTP_REG nsec_lsb);

WPTP_status WPTP_DCODividerSet(WP_handle cd_handle,
                               WPTP_REG dco_int,
                               WPTP_REG dco_remainder_msb,
                               WPTP_REG dco_remainder_lsb);



WPTP_status WPTP_SystemDisplay(void);
WPTP_status WPTP_ClockDomainDisplay(WP_handle cd_handle);
WPTP_status WPTP_SoftReset(void);
WPTP_status WPTP_CheckPllLock(void);


 /* advanced user access to registers */
void WPTP_REG_WRITE(WPTP_REG reg, WP_U8 cd_or_gen,WPTP_REG value);
void WPTP_REG_READ(WPTP_REG reg, WP_U8 cd_or_gen,WPTP_REG *value_ptr);



/*************************/
/* INTERNAL FUNCTIONS    */
/*************************/
WPTP_status WPTPI_SystemInit(WPTP_ptp_system *ptp_system_config);
WPTP_status WPTPI_ClockDomainInit(WP_handle cd_handle, WPTP_clock_domain_config *cd_config );
WPTP_status WPTPI_SoftReset(void);
WPTP_status WPTPI_CheckPllLock(void);



WPTP_status WPTPI_CheckCDHandle(WP_handle cd_handle);
WPTP_status WPTPI_CheckOffsetDone(WP_U32);
WPTP_status WPTPI_CheckOffsetOnPPSDone(WP_U32);
WPTP_status WPTPI_ReturnError(WPTP_status error_id, WP_U32 line);



 
#define WPTP_GLOBAL_SPACE  255
#define WPTP_PLL_LOCK_DELAY          100
#define WPTP_PLL_LOCK_TIMEOUT       0x1000    /* times  WPTP_PLL_LOCK_DELAY */
#define WPTP_RESET_DELAY             0
#define WPTP_OFFSET_DELAY            100
#define WPTP_ONE_SEC_DELAY           1000000   /* 1000000 microsecs */
#define WPTP_OFFSET_TIMEOUT         0x1000     /* times  WPTP_OFFSET_DELAY */
#define WPTP_OFFSET_ON_PPS_TIMEOUT   1     /* times  WPTP_ONE_SEC_DELAY + WPTP_OFFSET_DELAY */

/* GLOBAL  FPGA PTP ADDRESS */
#define WPTP_REL_ID                          0x0000
#define WPTP_SEC_REL_ID                      0x0001
#define WPTP_PTP_CONFIG                      0x0010
#define WPTP_SOFT_RESET                      0x0020
#define WPTP_PLL_STAT                        0x0021



/* CD0 FPGA PTP ADDRESS */
/* ptp N global */
#define WPTP_PTP_MODE                        0x0000
#define WPTP_PTP_STATUS                      0x0001
#define WPTP_TS_MODE                         0x0100
#define WPTP_ADD_VAL_NS_INT                  0x0101
#define WPTP_ADD_VAL_NS_FRAC_MSB             0x0102
#define WPTP_ADD_VAL_NS_FRAC_LSB             0x0103

/* Ts  - read only */
#define WPTP_TIMESTAMP_SEC_MSB               0x0104
#define WPTP_TIMESTAMP_SEC_MID               0x0105
#define WPTP_TIMESTAMP_SEC_LSB               0x0106
#define WPTP_TIMESTAMP_NSEC_MSB              0x0107
#define WPTP_TIMESTAMP_NSEC_LSB              0x0108

/* Offset related */
#define WPTP_OFFSET_MODE                     0x0109
#define WPTP_OFFSET_STATUS                   0x010a
#define WPTP_OFFSET_SEC_MSB                  0x010b
#define WPTP_OFFSET_SEC_MID                  0x010c
#define WPTP_OFFSET_SEC_LSB                  0x010d
#define WPTP_OFFSET_NSEC_MSB                 0x010e
#define WPTP_OFFSET_NSEC_LSB                 0x010f

/* DCO related */
#define WPTP_DCO_MODE                        0x0180
#define WPTP_DCO_INT                         0x0181
#define WPTP_DCO_REM_MSB                     0x0182
#define WPTP_DCO_REM_LSB                     0x0183

#define WPTP_DIV_REG                         0x01C0


/* CD BASES relative offset */

#define WPTPI_RELATIVE_REG_BASE_GENERAL       0x0000

#define WPTPI_RELATIVE_REG_BASE_FIRST_CD      0x0800
#define WPTPI_CD_ADDRESS_SPACE_SIZE           0x0200


/* WINMON ADDRESSES */
/* Note! all read/write are direct access, so phy addresses are used */
/* Any other access or DPS should have the Virtual adrresses */


#define WPTPI_REG_ADDRESS(reg,rel_os,base)\
 (((rel_os + reg)<<ptp_registry.ptp_system.ignore_bits) + base)



extern WP_U8 debug_flag;

/* SET & GET are taken from UFE3 driver */
/* with additional built-in base extraction from registry */
/* and a phy to vir convertion of that base address */
#define WPTPI_REG_SET(reg,rel_os,value) \
do { \
  volatile  WP_U16* addr = (volatile  WP_U16*) \
     WPL_PHY_VIRTUAL(0,WPTPI_REG_ADDRESS(\
  reg,rel_os,ptp_registry.ptp_system.ptp_hw_logic_start)); \
  WP_MEM_SHORT_SET( *addr, value); \
  if(debug_flag) \
  printf("WPTPI_REG_SET --- addr 0x%08x, val 0x%04x\n",(WP_U32)addr, value); \
} while (0)

/* Read 2 bytes from direct memory (registers memory) */
#define WPTPI_REG_GET(reg,rel_os,value) \
do { \
  volatile  WP_U16* addr = (volatile WP_U16*) \
   WPL_PHY_VIRTUAL(0,WPTPI_REG_ADDRESS(\
  reg,rel_os,ptp_registry.ptp_system.ptp_hw_logic_start)); \
  WP_MEM_SHORT_GET( *value, *addr); \
  if(debug_flag) \
  printf("WPTPI_REG_GET --- addr 0x%08x, val 0x%04x\n",(WP_U32)addr, *value); \
} while (0)


#define WPTPI_CD_OS(cd_os) (WPTPI_RELATIVE_REG_BASE_GENERAL\
  + WPTPI_RELATIVE_REG_BASE_FIRST_CD\
  + (WPTPI_CD_ADDRESS_SPACE_SIZE*cd_os))\

#define WPTPI_CD_REG_WRITE(reg,cd_os,value) WPTPI_REG_SET(reg,WPTPI_CD_OS(cd_os),value)
#define WPTPI_GEN_REG_WRITE(reg,value)   WPTPI_REG_SET(reg,WPTPI_RELATIVE_REG_BASE_GENERAL,value)
#define WPTPI_CD_REG_READ(reg,cd_os,value) WPTPI_REG_GET(reg,WPTPI_CD_OS(cd_os),value)
#define WPTPI_GEN_REG_READ(reg,value)   WPTPI_REG_GET(reg,WPTPI_RELATIVE_REG_BASE_GENERAL,value)




#define WPTPI_FIELD(v,s) (((WPTP_REG) (v)) << (s))
#define WPTPI_VALUE(f,s,w) ((((WPTP_REG) (f)) >> (s)) & ((1 << (w)) - 1))
#define WPTPI_MASK(s,w) ((((WPTP_REG) ((1 << (w)) - 1)) << (s)))

#define WPTPI_MASK_BITS(n) ((1 << (n)) - 1)
#define WPTPI_SHIFT_MASK(x,s,m) ((((WPTP_REG)(x)) >> (s)) & (m))


#define WPTPI_RETURN_ERROR(x) return WPTPI_ReturnError(x,__LINE__)
#define WPTPI_RETURN_IF_ERROR(status,x) \
        if ((status = (x), status!= WPTP_OK)) WPTPI_RETURN_ERROR(status)
#define WPTPI_DETACH_RETURN_IF_ERROR(status,x) \
       do{ status = (x); \
            if(status!= WPTP_OK)\
            { WPI_WDDI_DETACH(); WPTPI_RETURN_ERROR(status);}}while(0)



#define WPTPI_Delay(x) WPL_Delay(x)

/* DEFINES & MACROS PER HW REGISTER  (bit map) */



/* GLOBAL REGS */

/* WPTP_REL_ID */
/* WPTP_SEC_REL_ID */

/* WPTP_PTP_CONFIG */
#define WPTP_CONFIG_TS_UNITS_S   (0)
#define WPTP_CONFIG_TS_UNITS_W   (8)
#define WPTP_CONFIG_TS_UNITS_F(v) WPTPI_FIELD(v, WPTP_CONFIG_TS_UNITS_S)
#define WPTP_CONFIG_TS_UNITS_V(f) \
        WPTPI_VALUE(f, WPTP_CONFIG_TS_UNITS_S, WPTP_CONFIG_TS_UNITS_W)
#define WPTP_CONFIG_TS_UNITS_M \
        WPTPI_MASK(WPTP_CONFIG_TS_UNITS_S, WPTP_CONFIG_TS_UNITS_W)
/* WPTP_SOFT_RESET */
#define WPTP_RESET           1
#define WPTP_UNRESET         0
#define WPTP_SOFT_RESET_S   (0)
#define WPTP_SOFT_RESET_W   (1)
#define WPTP_SOFT_RESET_F(v) WPTPI_FIELD(v, WPTP_SOFT_RESET_S)
#define WPTP_SOFT_RESET_V(f) \
        WPTPI_VALUE(f, WPTP_SOFT_RESET_S, WPTP_SOFT_RESET_W)
#define WPTP_SOFT_RESET_M \
        WPTPI_MASK(WPTP_SOFT_RESET_S, WPTP_SOFT_RESET_W)
/* WPTP_PLL_STAT */
#define WPTP_PLL_LOCKED        1
#define WPTP_PLL_SYS_LOCK_S   (0)
#define WPTP_PLL_DCO_LOCK_S   (1)
#define WPTP_PLL_SYS_LOCK_W   (1)
#define WPTP_PLL_DCO_LOCK_W   (1)
#define WPTP_PLL_SYS_LOCK_F(v) WPTPI_FIELD(v, WPTP_PLL_SYS_LOCK_S)
#define WPTP_PLL_SYS_LOCK_V(f) \
        WPTPI_VALUE(f, WPTP_PLL_SYS_LOCK_S, WPTP_PLL_SYS_LOCK_W)
#define WPTP_PLL_SYS_LOCK_M \
        WPTPI_MASK(WPTP_PLL_SYS_LOCK_S, WPTP_PLL_SYS_LOCK_W)
#define WPTP_PLL_DCO_LOCK_F(v) WPTPI_FIELD(v, WPTP_PLL_DCO_LOCK_S)
#define WPTP_PLL_DCO_LOCK_V(f) \
        WPTPI_VALUE(f, WPTP_PLL_DCO_LOCK_S, WPTP_PLL_DCO_LOCK_W)
#define WPTP_PLL_DCO_LOCK_M \
        WPTPI_MASK(WPTP_PLL_DCO_LOCK_S, WPTP_PLL_DCO_LOCK_W)


/* PTP_MODE_n */

#define WPTP_MODE_CLOCK_SOURCE_EXTERNAL       0
#define WPTP_MODE_CLOCK_SOURCE_DCO            1

#define WPTP_MODE_CLOCK_SOURCE_S (0)
#define WPTP_MODE_CLOCK_SOURCE_W (1)

#define WPTP_MODE_CLOCK_SOURCE_F(v) WPTPI_FIELD(v, WPTP_MODE_CLOCK_SOURCE_S)
#define WPTP_MODE_CLOCK_SOURCE_V(f) \
        WPTPI_VALUE(f, WPTP_MODE_CLOCK_SOURCE_S, WPTP_MODE_CLOCK_SOURCE_W)
#define WPTP_MODE_CLOCK_SOURCE_M \
        WPTPI_MASK(WPTP_MODE_CLOCK_SOURCE_S, WPTP_MODE_CLOCK_SOURCE_W)



/* PTP_STATUS_n */

#define WPTP_STATUS_CLOCK_MODE_MASTER          0
#define WPTP_STATUS_CLOCK_MODE_SLAVE           1
#define WPTP_STATUS_CLOCK_MODE_COMBINATIONAL   2

#define WPTP_STATUS_CLOCK_MODE_S (0)
#define WPTP_STATUS_CLOCK_MODE_W (2)

#define WPTP_STATUS_CLOCK_MODE_F(v) WPTPI_FIELD(v, WPTP_STATUS_CLOCK_MODE_S)
#define WPTP_STATUS_CLOCK_MODE_V(f) \
        WPTPI_VALUE(f, WPTP_STATUS_CLOCK_MODE_S, WPTP_STATUS_CLOCK_MODE_W)
#define WPTP_STATUS_CLOCK_MODE_M \
        WPTPI_MASK(WPTP_STATUS_CLOCK_MODE_S, WPTP_STATUS_CLOCK_MODE_W)



/* PTP_DCO_MODE_n */

#define WPTP_DCO_DISABLED          0
#define WPTP_DCO_ENABLED           1

#define WPTP_DCO_ENABLE_S        (0)
#define WPTP_DCO_ENABLE_W        (1)

#define WPTP_DCO_ENABLE_F(v) WPTPI_FIELD(v, WPTP_DCO_ENABLE_S)
#define WPTP_DCO_ENABLE_V(f) \
        WPTPI_VALUE(f, WPTP_DCO_ENABLE_S, WPTP_DCO_ENABLE_W)
#define WPTP_DCO_ENABLE_M \
        WPTPI_MASK(WPTP_DCO_ENABLE_S, WPTP_DCO_ENABLE_W)





/* DCO_INT_n */
/* 1 byte */

/*  DCO_REM_MSB*/

/*  DCO_REM_LSB*/

/* DIV_REG */

#define WPTP_DCO_PRE_DIVIDER_DISABLED  0


/* TS_GLOBAL_n */
#define WPTP_TS_DISABLED     0      /* TS stoppes and keeps last value */
#define WPTP_TS_ENABLED      1

#define WPTP_TS_ENABLE_S     (0)
#define WPTP_TS_ENABLE_W     (1)

#define WPTP_TS_ENABLE_F(v) WPTPI_FIELD(v, WPTP_TS_ENABLE_S)
#define WPTP_TS_ENABLE_V(f) \
        WPTPI_VALUE(f, WPTP_TS_ENABLE_S, WPTP_TS_ENABLE_W)
#define WPTP_TS_ENABLE_M \
        WPTPI_MASK(WPTP_TS_ENABLE_S, WPTP_TS_ENABLE_W)


/* OFF_MODE_n */

#define WPTP_TS_OFFSET_MODE_ADDATIVE     0
#define WPTP_TS_OFFSET_MODE_ABSOLUTE     1

#define WPTP_TS_OFFSET_MODE_S     (0)
#define WPTP_TS_OFFSET_MODE_W     (1)

#define WPTP_TS_OFFSET_MODE_F(v) WPTPI_FIELD(v, WPTP_TS_OFFSET_MODE_S)
#define WPTP_TS_OFFSET_MODE_V(f) \
        WPTPI_VALUE(f, WPTP_TS_OFFSET_MODE_S, WPTP_TS_OFFSET_MODE_W)
#define WPTP_TS_OFFSET_MODE_M \
        WPTPI_MASK(WPTP_TS_OFFSET_MODE_S, WPTP_TS_OFFSET_MODE_W)


#define WPTP_TS_OFFSET_ADDATIVE_ADD     0
#define WPTP_TS_OFFSET_ADDATIVE_SUB     1

#define WPTP_TS_OFFSET_ADD_MODE_S     (1)
#define WPTP_TS_OFFSET_ADD_MODE_W     (1)

#define WPTP_TS_OFFSET_ADD_MODE_F(v) WPTPI_FIELD(v, WPTP_TS_OFFSET_ADD_MODE_S)
#define WPTP_TS_OFFSET_ADD_MODE_V(f) \
        WPTPI_VALUE(f, WPTP_TS_OFFSET_ADD_MODE_S, WPTP_TS_OFFSET_ADD_MODE_W)
#define WPTP_TS_OFFSET_ADD_MODE_M \
        WPTPI_MASK(WPTP_TS_OFFSET_ADD_MODE_S, WPTP_TS_OFFSET_ADD_MODE_W)


#define WPTP_TS_OFFSET_ABS_ONPPS_CLEAR     0
#define WPTP_TS_OFFSET_ABS_ONPPS_SET       1

#define WPTP_TS_OFFSET_ABS_ONPPS_MODE_S     (2)
#define WPTP_TS_OFFSET_ABS_ONPPS_MODE_W     (1)

#define WPTP_TS_OFFSET_ABS_ONPPS_MODE_F(v) WPTPI_FIELD(v, WPTP_TS_OFFSET_ABS_ONPPS_MODE_S)
#define WPTP_TS_OFFSET_ABS_ONPPS_MODE_V(f) \
        WPTPI_VALUE(f, WPTP_TS_OFFSET_ABS_ONPPS_MODE_S, WPTP_TS_OFFSET_ABS_ONPPS_MODE_W)
#define WPTP_TS_OFFSET_ABS_ONPPS_MODE_M \
        WPTPI_MASK(WPTP_TS_OFFSET_ABS_ONPPS_MODE_S, WPTP_TS_OFFSET_ABS_ONPPS_MODE_W)



/* OFF_STATUS_n */

#define WPTP_OFFSET_STATUS_DONE        1

#define WPTP_OFFSET_STATUS_DONE_S     (0)
#define WPTP_OFFSET_STATUS_DONE_W     (1)

#define WPTP_OFFSET_STATUS_DONE_F(v) WPTPI_FIELD(v, WPTP_OFFSET_STATUS_DONE_S)
#define WPTP_OFFSET_STATUS_DONE_V(f) \
        WPTPI_VALUE(f, WPTP_OFFSET_STATUS_DONE_S, WPTP_OFFSET_STATUS_DONE_W)
#define WPTP_OFFSET_STATUS_DONE_M \
        WPTPI_MASK(WPTP_OFFSET_STATUS_DONE_S, WPTP_OFFSET_STATUS_DONE_W)


#define WPTP_OFFSET_ONPPS_STATUS_DONE        1

#define WPTP_OFFSET_ONPPS_STATUS_DONE_S     (1)
#define WPTP_OFFSET_ONPPS_STATUS_DONE_W     (1)

#define WPTP_OFFSET_ONPPS_STATUS_DONE_F(v) WPTPI_FIELD(v, WPTP_OFFSET_ONPPS_STATUS_DONE_S)
#define WPTP_OFFSET_ONPPS_STATUS_DONE_V(f) \
        WPTPI_VALUE(f, WPTP_OFFSET_ONPPS_STATUS_DONE_S, WPTP_OFFSET_ONPPS_STATUS_DONE_W)
#define WPTP_OFFSET_ONPPS_STATUS_DONE_M \
        WPTPI_MASK(WPTP_OFFSET_ONPPS_STATUS_DONE_S, WPTP_OFFSET_ONPPS_STATUS_DONE_W)

/* OFFSET MSB secs */
/* OFFSET MID secs */
/* OFFSET LSB secs */
/* OFFSET MSB nano secs */
/* OFFSET LSB nano secs */


/* ADDVALUE nano secs */

#define WPTP_ADDVALUE_FRAC_MSB_S     (0)
#define WPTP_ADDVALUE_FRAC_MSB_W      (8)

#define WPTP_ADDVALUE_FRAC_MSB_F(v) WPTPI_FIELD(v, WPTP_ADDVALUE_FRAC_MSB_S)
#define WPTP_ADDVALUE_FRAC_MSB_V(f) \
        WPTPI_VALUE(f, WPTP_ADDVALUE_FRAC_MSB_S, WPTP_ADDVALUE_FRAC_MSB_W)
#define WPTP_ADDVALUE_FRAC_MSB_M \
        WPTPI_MASK(WPTP_ADDVALUE_FRAC_MSB_S, WPTP_ADDVALUE_FRAC_MSB_W)


/* ADDVALUE nano secs fractional LSB */


/* TIMESTAMP MSB secs */
/* TIMESTAMP MID secs */
/* TIMESTAMP LSB secs */

/* TIMESTAMP MSB nano secs */
/* TIMESTAMP LSB nano secs */




#endif  /*WPX_PTP_H*/
