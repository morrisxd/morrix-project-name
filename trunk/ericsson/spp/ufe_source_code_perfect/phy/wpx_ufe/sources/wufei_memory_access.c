/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_memory_access.c
 *
 * Purpose:  UFE memory access functions
 *
 * Functions contained in this file:
 * +-------------------------+--------------------------------------------
 * |  Function name          | Description
 * +-------------------------+--------------------------------------------
 * |  API Function
 * |  -----------------------+--------------------------------------------
 * |  WUFE_DirectWriteWP_U8  / access direct (register) memory with U8 size
 * |  WUFE_DirectWriteWP_U16 / access direct (register) memory with U16 size
 * |  -----------------------+--------------------------------------------
 * |  Internal WUFE Functions
 * |  -----------------------+--------------------------------------------
 * +--------------------------+--------------------------------------------
 ****************************************************************************/

#include <stdio.h>
#include <string.h>

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

#ifndef WUFEI_HW_CONFIG_H
#include "wufei_hw_config.h"
#endif

#ifndef WUFEI_MEMORY_ACCESS_INT_H
#include "wufei_memory_access_int.h"
#endif

#ifndef WUFEI_HW_MEMORY_H
#include "wufei_hw_memory.h"
#endif

#ifndef WUFE_ERRORS_H
#include "wufe_errors.h"
#endif

#ifndef WUFEI_REGISTRY_H
#include "wufei_registry.h"
#endif

#include "wpx_phy_data.h"

#include "api/wp_types.h"
#include "include/core/utility/wpi_services.h"

extern WP_U8 debug_flag;

void WUFE_MEM_16_SET(WP_U32 id, WP_U16* offset, WP_U16 value)
{
   /*check UFE ID */
   WUFEI_MEM_16_SET(id, offset, value);
}

void WUFE_MEM_16_GET(WP_U32 id, WP_U16* offset, WP_U16* value_ptr)
{
   /*check UFE ID */
   WUFEI_MEM_16_GET(id, offset, value_ptr);
}

void WUFE_MemShortRead(WP_U32 base, WP_U32 register_num, WP_U16* value)
{
   WUFEI_MemShortRead(base, register_num, value);
}

void WUFE_MemShortWrite(WP_U32 base, WP_U32 register_num, WP_U16 value)
{
   WUFEI_MemShortWrite(base, register_num, value);
}

void WUFE_MemByteWrite(WP_U32 base, WP_U32 register_num, WP_U8 value)
{
   WUFEI_MemByteWrite(base, register_num, value);
}

void WUFE_MemByteRead(WP_U32 base, WP_U32 register_num, WP_U8*value)
{
   WUFEI_MemByteRead(base, register_num, value);
}

void WUFEI_MemShortWrite(WP_U32 base, WP_U32 register_num, WP_U16 value)
{
   WP_U16* addr;

   addr = (WP_U16*)WPL_PHY_VIRTUAL(0,(base + (register_num << WUFEI_ADDRSHIFT)));

   WP_MEM_SHORT_SET(*addr, value );
}

void WUFEI_MemShortRead(WP_U32 base, WP_U32 register_num, WP_U16*value)
{
   WP_U16* addr;

   addr = (WP_U16*)WPL_PHY_VIRTUAL(0,(base + (register_num << WUFEI_ADDRSHIFT)));

   WP_MEM_SHORT_GET(*value ,*addr  );
}

void WUFEI_ReadModifyWrite(WP_U32 wufe_id, WP_U32 addr, WP_U8 value)
{
   /* provide read modify write for 16 bit access*/
   WP_U16 tmp=0;
   WP_U32* offset = (WP_U32*)(addr & ~0x1);

   WUFEI_MEM_16_GET(wufe_id, offset, &tmp);
   if (addr & 0x1)
      tmp = (tmp & 0xff00) | value;
   else
      tmp = (tmp & 0xff) | (value << 8);

   WUFEI_MEM_16_SET(wufe_id, offset, tmp);
}

/*****************************************************************************
 *
 * Function:   WUFE_IndirectMemset
 *
 * Purpose:    access direct indirect memory for any size of data.
 *
 * Description:
 *     This function set the pattern in the INDIRECT memory"set_size" number of times
 *     according to arr_size.
 *     the function checks the validity of the address and write the data.
 *
 * Inputs:
 *     indirect_addr  The register address
 *     pattern      the byte to be written.
 *     set_size     number of times to write the pattern.
 *
 * Outputs:
 *     write to RAM or ZBT of the UFE.
 *
 * Return Value:
 *     None
 *
 * Called by:
 *     Any function.
 *
 ****************************************************************************/

void WUFEI_IndirectMemset(WP_U32 id, void *indirect_addr, WP_U8 pattern, WP_U32 set_size)
{
   WP_U16 double_byte_pattern;
   WP_U32 set_count=0;

   /* if Address size is outside the valid space */
   WUFEI_INDIRECT_ADDRESS_OK(indirect_addr, set_size);
   WUFEI_INDIRECT_ADDRESS_SET(id, indirect_addr);

   /* Write the first indirect address
      the next address is incremneted by HW
      seprate write to MSP and to LSB */

   if ((WP_U32)indirect_addr%2)
   {
      WUFE_PRINT_ERR((wufei_log," WUFEI_IndirectMemset: indirect addr not aligned"));
      /* The first data is unaligned write single byte */
      /*WUFEI_MEM_8_SET(id, ( WP_U8*)WUFEI_INDIRCT_DATA_REG, pattern);*/
      set_count++;
   }

   /* prepeare the data in WP_U16 */
   double_byte_pattern = pattern+pattern*0x100;

   while ((set_size - set_count) >= 2)
   {/* Write the rest of the data, each write size is 2 bytes */
      WUFEI_MEM_16_SET(id, (WP_U16*)WUFEI_INDIRECT_DATA_REG, double_byte_pattern);
      set_count+=2;
   }

   if ((set_size - set_count) == 1)
   {
      WUFE_PRINT_ERR((wufei_log," WUFEI_IndirectMemset: indirect addr not aligned"));
      /* The last data is unaligned write single byte */
      /*WUFEI_MEM_8_SET(id, ( WP_U8*)WUFEI_INDIRCT_DATA_REG, pattern);*/
   }
}

void WUFE_IndirectMemset(WP_U32 id, void *indirect_addr, WP_U8 pattern, WP_U32 set_size)
{
   WUFEI_IndirectMemset(id, indirect_addr, pattern, set_size);
}

void WUFE_IndirectMemOrMask(WP_U32 wufe_id, WP_U32 indirect_addr, WP_U16 mask)
{
   WP_U16 mem_value=0;
   WUFEI_INDIRECT_ADDRESS_SET(wufe_id, indirect_addr);
   WUFEI_INDIRECT_DATA_16_GET(wufe_id, &mem_value);
   mem_value|=mask;

   WUFEI_INDIRECT_ADDRESS_SET(wufe_id, indirect_addr);
   WUFEI_INDIRECT_DATA_16_SET(wufe_id, mem_value);
}

void WUFE_IndirectMemAndMask(WP_U32 wufe_id, WP_U32 indirect_addr, WP_U16 mask)
{
   WP_U16 mem_value=0;
   WUFEI_INDIRECT_ADDRESS_SET(wufe_id, indirect_addr);
   WUFEI_INDIRECT_DATA_16_GET(wufe_id, &mem_value);
   mem_value|=mask;
   WUFEI_INDIRECT_ADDRESS_SET(wufe_id, indirect_addr);
   WUFEI_INDIRECT_DATA_16_SET(wufe_id, mem_value);
}

void WUFEI_DirectMemOrMask(WP_U32 id, WP_U16* direct_addr, WP_U16 mask)
{
   WP_U32 base = WUFEI_DIRECT_MEM_ADD_START(id);
   WP_U32 reg_addr = base + (WP_U32)direct_addr*WUFEI_ADDR_DRV_TO_HW;
   WUFEI_REG_BIT_SET(reg_addr,mask);
}

void WUFEI_DirectMemAndMask(WP_U32 id, WP_U16* direct_addr, WP_U16 mask)
{
   WP_U32 base = WUFEI_DIRECT_MEM_ADD_START(id);
   WP_U32 reg_addr = base + (WP_U32)direct_addr*WUFEI_ADDR_DRV_TO_HW;
   WUFEI_REG_BIT_CLEAR(reg_addr,mask);
}

void WUFE_DirectMemOrMask(WP_U32 wufe_id, WP_U16* direct_addr, WP_U16 mask)
{
   WUFEI_DirectMemOrMask(wufe_id, direct_addr, mask);
}

void WUFE_DirectMemAndMask(WP_U32 wufe_id, WP_U16*  direct_addr, WP_U16 mask)
{
   WUFEI_DirectMemAndMask(wufe_id, direct_addr, mask);
}
