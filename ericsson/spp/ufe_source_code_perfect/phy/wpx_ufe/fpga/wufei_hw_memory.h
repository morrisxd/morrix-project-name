/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_reg_map.h
 *
 * Purpose: register-access macros for UFE
 *
 ****************************************************************************/
#ifndef WUFEI_HW_MEMORY_H
#define WUFEI_HW_MEMORY_H

#ifndef WUFEI_HW_CONFIG_H
#include "wufei_hw_config.h"
#endif

#ifndef WPL_MEM_ACCESS_H
#include "wpl_mem_access.h"
#endif

#ifndef WUFEI_HW_CORE_INT_H
#include "wufei_hw_core_int.h"
#endif

#define WUFEI_DIRECT_MEM_ADD_START(id)  \
    WUFEI_UfeCoreBaseAddrGet(WUFEI_UFE_ID_V(id), WUFEI_UFE_CORE_ID_V(id))

#define WUFEI_DIRECT_MEM_ADD_END(id)    \
   (WUFEI_DIRECT_MEM_ADD_START(id) + WUFEI_UFE_DIRECT_MEM_SIZE)

/* Check if the indirect memory access is valid  (TDM RAM, Machines and ZBT  memory range) */
#define WUFEI_INDIRECT_ADDRESS_OK(offset,size) \
do {  \
  if ((WP_U32)(offset)+(size)  > WUFEI_INDIRECT_MEM_ADD_EXT_FIFO_END ){ \
  printf("\n!!!!!!!!!!!  ERROR !!!!!!!!!!!!!!!!\n");}\
} while (0)

/*************************************************************************************/
/* Direct access */
extern WP_U8 debug_flag;

/* Write 2 bytes to direct memory (registers memory) */
#define WUFEI_REG_SET(address, value) \
do { \
  WP_U16* addr = (WP_U16*)WPL_PHY_VIRTUAL(0,(WP_U32)address); \
  WP_MEM_SHORT_SET( *addr, value); \
  if(debug_flag) \
  printf("WUFEI_REG_SET --- addr 0x%08x, val 0x%04x\n",(WP_U32)addr, value); \
} while (0)

/* Read 2 bytes from direct memory (registers memory) */
#define WUFEI_REG_GET(address, value) \
do { \
  WP_U16* addr = (WP_U16*)WPL_PHY_VIRTUAL(0,(WP_U32)address); \
  WP_MEM_SHORT_GET( *value, *addr); \
  if(debug_flag) \
  printf("WUFEI_REG_GET --- addr 0x%08x, val 0x%04x\n",(WP_U32)addr, *value); \
} while (0)

/**************************************************************
 * WUFEI_INDIRECT_ADDRESS_SET
 * 1) receives absulut address
 * 2) split the address to 2 MSB and LSB
 * 3) put the LSB in WUFEI_INDIRECT_ADD_LOW_REG
 * 4) put the MSB in WUFEI_INDIRECT_ADD_HIGH_REG
 ***************************************************************/
#define WUFEI_REG_INDIRECT_ADDRESS_SET(indha_ptr,indla_ptr,indirect_addr,addr_base,value_base) \
do { \
   WP_U32 reg_addr = addr_base + (WP_U32)indla_ptr*WUFEI_ADDR_DRV_TO_HW; \
   WP_U16 reg_value = \
   (WP_U16)((WP_U32)(indirect_addr / WUFEI_INDRCT_ADDR_ABS_TO_DRV) & 0x0000ffff);\
   WUFEI_REG_SET(reg_addr, reg_value);\
   reg_addr = addr_base + (WP_U32)indha_ptr*WUFEI_ADDR_DRV_TO_HW; \
   reg_value = \
   (WP_U16)(((WP_U32)(indirect_addr / WUFEI_INDRCT_ADDR_ABS_TO_DRV + value_base)  \
            & 0xffff0000) >> 16 );\
   WUFEI_REG_SET(reg_addr, reg_value);\
} while (0)

#define WUFEI_REG_INDIRECT_ADDRESS_GET(indha_ptr,indla_ptr,indirect_addr,addr_base) \
do { \
   WP_U16 indla, indha; \
   WP_U32 reg_addr = addr_base + (WP_U32)indla_ptr*WUFEI_ADDR_DRV_TO_HW; \
   WUFEI_REG_GET(reg_addr, &indla);\
   reg_addr = addr_base + (WP_U32)indha_ptr*WUFEI_ADDR_DRV_TO_HW; \
   WUFEI_REG_GET(indha_ptr, &indha);\
   *indirect_addr = ((((WP_U32)indha & 0x0000ffff) <<16)|((WP_U32)indla & 0x0000ffff )); \
} while (0)

#define WUFEI_REG_BIT_SET(reg_addr, mask)\
do { \
   WP_U16 mem_value;\
   WUFEI_REG_GET(reg_addr, &mem_value);\
   mem_value |=mask;\
   WUFEI_REG_SET(reg_addr, mem_value);\
} while (0)


#define WUFEI_REG_BIT_CLEAR(reg_addr, mask)\
do { \
   WP_U16 mem_value;\
   WUFEI_REG_GET(reg_addr, &mem_value);\
   mem_value &=mask;\
   WUFEI_REG_SET(reg_addr, mem_value);\
} while (0)

/*************************************************************************************/
#define WUFEI_MEM_16_GET(id, offset, value_ptr) \
do {  \
   WP_U32 core_base = WUFEI_DIRECT_MEM_ADD_START(id); \
   WP_U16* mem_addr = (WP_U16*)WPL_PHY_VIRTUAL(0, \
                            (core_base + (WP_U32)offset*WUFEI_ADDR_DRV_TO_HW));\
   WP_MEM_SHORT_GET(*value_ptr, *mem_addr);\
   if(debug_flag) \
   printf("WUFEI_MEM_16_GET --- addr 0x%08x, val 0x%04x\n",(WP_U32)mem_addr,*value_ptr );\
} while (0)

#define WUFEI_MEM_16_SET(id, offset, value) \
do {  \
  WP_U32 core_base = WUFEI_DIRECT_MEM_ADD_START(id); \
  WP_U16* mem_addr = (WP_U16*)WPL_PHY_VIRTUAL(0, \
                           (core_base + (WP_U32)offset*WUFEI_ADDR_DRV_TO_HW)); \
  WP_MEM_SHORT_SET(*mem_addr, value); \
  if(debug_flag) \
  printf("WUFEI_MEM_16_SET --- addr 0x%08x, val 0x%04x\n",(WP_U32)mem_addr, value);\
}while (0)

/**************************************************************
 * WUFEI_INDIRECT_ADDRESS_SET
 * 1) chenge the address to Absulut address
 * 2) split the address to 2 MSB and LSB
 * 3) put the LSB in WUFEI_INDIRECT_ADD_LOW_REG
 * 4) put the MSB in WUFEI_INDIRECT_ADD_HIGH_REG
 ***************************************************************/
#define WUFEI_INDIRECT_ADDRESS_SET(id, indirect_addr) \
do { \
   WP_U32 core_base = WUFEI_DIRECT_MEM_ADD_START(id); \
   WP_U32 indla_ptr = WUFEI_INDIRECT_ADD_LOW_REG; \
   WP_U32 indha_ptr = WUFEI_INDIRECT_ADD_HIGH_REG;\
   WUFEI_REG_INDIRECT_ADDRESS_SET(indha_ptr,indla_ptr,(WP_U32)indirect_addr,core_base,0);\
} while(0)

#define WUFEI_INDIRECT_ADDRESS_GET(id, val) \
do { \
   WP_U32 core_base = WUFEI_DIRECT_MEM_ADD_START(id); \
   WP_U32 indla_ptr = WUFEI_INDIRECT_ADD_LOW_REG; \
   WP_U32 indha_ptr = WUFEI_INDIRECT_ADD_HIGH_REG; \
   WUFEI_REG_INDIRECT_ADDRESS_GET(indla_ptr,indla_ptr,val,core_base); \
} while(0)

#define WUFEI_INDIRECT_DATA_16_SET(id, data) \
do{\
  WUFEI_MEM_16_SET(id,(WP_U16 *)WUFEI_INDIRECT_DATA_REG,data);\
}while (0)

#define WUFEI_INDIRECT_DATA_16_GET(id, value_ptr) \
do{ \
  WUFEI_MEM_16_GET(id,(WP_U16 *)WUFEI_INDIRECT_DATA_REG,value_ptr); \
} while (0)

/*************************************************************************************/
#define WUFEI_MemByteWrite(base,register_num,value) \
do{\
  WP_U8* addr = \
  (WP_U8 *)WPL_PHY_VIRTUAL(0,(base +(register_num << WUFEI_ADDRSHIFT)));\
  WP_MEM_BYTE_SET(*addr, value );\
  if(debug_flag)\
  printf("\n--BSP WUFEI_MemByteWrite --- addr 0x%08x, val 0x%x\n",(WP_U32)addr,value);\
} while (0)


#define WUFEI_MemByteRead(base,register_num,value) \
do{\
  WP_U8* addr = \
  (WP_U8 *)WPL_PHY_VIRTUAL(0,(base + (register_num << WUFEI_ADDRSHIFT)));\
  WP_MEM_BYTE_GET(*value, *addr);\
  if(debug_flag) \
  printf("--BSP WUFEI_MemByteRead --- addr 0x%08x value 0x%08x\n",(WP_U32)addr,(WP_U32)*value );\
} while (0)

#define WUFEI_REGISTER_CONFIG(reg, mask, value) \
do{ \
  reg &= ~mask; reg |= value; \
} while(0)


#endif /*WUFEI_HW_MEMORY_H*/
