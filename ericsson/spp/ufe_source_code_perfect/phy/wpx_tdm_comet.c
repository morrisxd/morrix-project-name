/*****************************************************************************
 * (C) Copyright 2006, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/************************************************************************/
/*                      TDM CARD INITIALIZATION                         */
/************************************************************************/

#include <stdio.h>

#ifndef WP_TYPES_H
#include "api/wp_types.h"
#endif
#ifndef WP_WDDI_H
#include "api/wp_wddi.h"
#endif
#ifndef WP_PROTOTYPES_H
#include "api/wp_prototypes.h"
#endif
#ifndef WPI_REG_H
#include "include/core/hardware/wpi_reg.h"
#endif
#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif
#ifndef WPI_HANDLE_H
#include "include/core/registry/wpi_handle.h"     /* For WPI_ERROR */
#endif
#include "veneer/wpi_board_data_int.h"
#include "veneer/wpi_veneer.h"

/* The following file defines the PHY memory map. */
#ifndef WPX_PHY_DATA_H
#include "wpx_phy_data.h"
#endif

#ifndef WPX_TDM_COMET_H
#include "wpx_tdm_comet.h"
#endif
#include "veneer/wpi_veneer.h"
#ifndef WPI_REG_GPIO_H
#ifdef WINPATH2
#include "hardware/winpath2/wpi_hw_reg_gpio.h"
#endif
#endif

/* Forward function declarations */
WP_status WPUI_TdmCometInitEq(wpu_comet_phy *phy,
                             WP_U32 table_size, WP_U8 *table);

WP_status WPUI_TdmCometInitWf(wpu_comet_phy *phy,
                             WP_U32 table_size, WP_U8 *table);

WP_status WPUI_TdmCometCardInit(wpu_comet_phy *xcvr,
                                WP_U32 q_port,
                                WP_U32 mode);

/* T1 Tx Waveform Values for short haul */
WP_U8 wave_form_t1[] = {
   0x00, 0x44, 0x00, 0x00, 0x00,
   0x00, 0x43, 0x00, 0x00, 0x00,
   0x0f, 0x42, 0x00, 0x00, 0x00,
   0x27, 0x42, 0x00, 0x00, 0x00,
   0x34, 0x41, 0x00, 0x00, 0x00,
   0x34, 0x00, 0x00, 0x00, 0x00,
   0x34, 0x00, 0x00, 0x00, 0x00,
   0x34, 0x00, 0x00, 0x00, 0x00,
   0x34, 0x00, 0x00, 0x00, 0x00,
   0x34, 0x00, 0x00, 0x00, 0x00,
   0x34, 0x00, 0x00, 0x00, 0x00,
   0x34, 0x00, 0x00, 0x00, 0x00,
   0x34, 0x00, 0x00, 0x00, 0x00,
   0x33, 0x00, 0x00, 0x00, 0x00,
   0x2b, 0x00, 0x00, 0x00, 0x00,
   0x21, 0x00, 0x00, 0x00, 0x00,
   0x4a, 0x00, 0x00, 0x00, 0x00,
   0x4b, 0x00, 0x00, 0x00, 0x00,
   0x4b, 0x00, 0x00, 0x00, 0x00,
   0x4b, 0x00, 0x00, 0x00, 0x00,
   0x4a, 0x00, 0x00, 0x00, 0x00,
   0x46, 0x00, 0x00, 0x00, 0x00,
   0x45, 0x00, 0x00, 0x00, 0x00,
   0x44, 0x00, 0x00, 0x00, 0x00
};

/* E1 Tx Waveform Values for short haul */
WP_U8 wave_form_e1[] = {
   0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00,
   0x10, 0x00, 0x00, 0x00, 0x00,
   0x35, 0x00, 0x00, 0x00, 0x00,
   0x35, 0x00, 0x00, 0x00, 0x00,
   0x35, 0x00, 0x00, 0x00, 0x00,
   0x35, 0x00, 0x00, 0x00, 0x00,
   0x34, 0x00, 0x00, 0x00, 0x00,
   0x34, 0x00, 0x00, 0x00, 0x00,
   0x34, 0x00, 0x00, 0x00, 0x00,
   0x34, 0x00, 0x00, 0x00, 0x00,
   0x34, 0x00, 0x00, 0x00, 0x00,
   0x34, 0x00, 0x00, 0x00, 0x00,
   0x34, 0x00, 0x00, 0x00, 0x00,
   0x34, 0x00, 0x00, 0x00, 0x00,
   0x05, 0x00, 0x00, 0x00, 0x00,
   0x41, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00
};

/* T1 Rx Equalizer Values */
WP_U32 rlps_ram_t1[] = {
   0x03061C3F, 0x03061C3D, 0x03061C3A, 0x03062C3D, 0x03062C3B, 0x03062C38,
   0x030E2C3F, 0x030E2C3C, 0x030E2C38, 0x03162C3F, 0x03162C3D, 0x03162C3A,
   0x03163C3F, 0x03163C38, 0x0316283B, 0x0316383B, 0x03163CBB, 0x031E3CBF,
   0x031E3CBD, 0x031E3CBA, 0x031E3CB8, 0x03263CBC, 0x032628BA, 0x032638BB,
   0x0B263D3F, 0x0B263D3E, 0x0B263D3D, 0x0B263D3C, 0x0B26293A, 0x0B26393F,
   0x13262DB8, 0x132E2DBF, 0x132E2DBF, 0x132E2DBE, 0x132E2DBD, 0x132E2DBC,
   0x132E2DBB, 0x132E19B8, 0x132E29BF, 0x1B2E1E38, 0x1B361E3F, 0x1B361E3C,
   0x1B361E3B, 0x1B360A3B, 0x1B361A3B, 0x23361EBF, 0x23361EB8, 0x23361EBF,
   0x233E1EBD, 0x2B3E1EBB, 0x2B3E1EB8, 0x2B461EBF, 0x33461EBD, 0x33461EBA,
   0x33461EB8, 0x334E1EBA, 0x32461EBC, 0x3A4E1EBF, 0x3A4E1EBC, 0x3A4E0ABA,
   0x3A4E1AB8, 0x424E1F3F, 0x424E0F3E, 0x424E0F3D, 0x424E0F3C, 0x424E0F3A,
   0x424E0F3B, 0x424E1F3A, 0x4A561F3F, 0x4A561F3B, 0x4A560B38, 0x4A561B3B,
   0x52561FBF, 0x52561FBC, 0x52561FB8, 0x525E1FBF, 0x5A5E1FBE, 0x5A5E1FBC,
   0x5A5E1FBB, 0x5A5E1FB9, 0x5A5C1FBB, 0x62641FBF, 0x62641FBE, 0x62641FBD,
   0x62641FBD, 0x62641FBC, 0x62661FBC, 0x62661BBB, 0x62661BBA, 0x62661BB9,
   0x62661BB8, 0x62662BBD, 0x6A662BBC, 0x6A662BBA, 0x6A663BBD, 0x6A663BBB,
   0x6A663BB8, 0x726E3BBF, 0x726E3BBC, 0x726E3BB8, 0x72763BBF, 0x7A763BBD,
   0x7A763BBA, 0x7A763BB8, 0x7A7E3BBB, 0x827E37BF, 0x827E37BE, 0x827E37BD,
   0x827E37BB, 0x827E37BA, 0x8A7E47BF, 0x8A7E47BF, 0x8A7E47BF, 0x8A7E47BF,
   0x897647B8, 0x898647BB, 0x898657BF, 0x898657BE, 0x898657BD, 0x898657BC,
   0x898657BB, 0x898657BA, 0x918E57B9, 0x918E57B8, 0x918E57BF, 0x918E57BF,
   0x908E57BA, 0x908E47B9, 0x909647BB, 0x909647BA, 0x909647B9, 0x909647B8,
   0x909647BF, 0x909E47BE, 0x909E47BD, 0x909E47BC, 0x909E47BA, 0x989E57BB,
   0x98A657BF, 0x98A657BE, 0x98A657BD, 0x98A657BB, 0x98A657BA, 0xA0A667BB,
   0xA0AE67BA, 0xA0AE67BF, 0xA8AE67BD, 0xA8AE67BB, 0xA8AE67BA, 0xA8AE77BC,
   0xA8AC77BF, 0xA8AC77BD, 0xB0AC77BA, 0xB0AC77B8, 0xB0B477BF, 0xB0B477BC,
   0xB0B477BA, 0xB8BC77B8, 0xB8BC77BF, 0xC0BC77BC, 0xC0BC77BA, 0xC0BC87BB,
   0xC0BA87BE, 0xC0BA87BE, 0xC0BA87BD, 0xC0BA87BC, 0xC0BA87BB, 0xC0BA87BA,
   0xC8B887BB, 0xC8B897BA, 0xC8B897BD, 0xC8B897BB, 0xD0B897BA, 0xD0B8A7BD,
   0xD0B8A7BC, 0xD0B8A7BB, 0xD0BAB7BA, 0xD0B8B7BD, 0xD0B8B7B8, 0xD8B8B7B7,
   0xD8B8B7B6, 0xD8B8B7B5, 0xD8BAC7B4, 0xD8B8C7B6, 0xE0B8C7B5, 0xE0BAD7B4,
   0xE0B8D7B6, 0xE0B8D7B5, 0xE0BAE7B4, 0xE0B8E7B6, 0xE0B8E7B4, 0xE8BAF7B3,
   0xE8B8F7B6, 0xE8B8F7B5, 0xE8BB07B4, 0xE8B907B6, 0xE8B907B5, 0xE8BB17B4,
   0xE8B917B6, 0xE8B917B5, 0xE8BB27B4, 0xF0B927B6, 0xF0B927B4, 0xF0BB37B3,
   0xF0B937B6, 0xF8B937B3, 0xF8B937A8, 0xF8B937AF, 0xF8B937AC, 0xF8BB47AB,
   0xF8B947AE, 0xF8B947AD, 0xF8BB57AC, 0xF8B957AE, 0xF8B957AE, 0xF8BB67AD,
   0xF8B967AF, 0xF8B967AF, 0xF8BB77AA, 0xF8B977AC, 0xF8B977AB, 0xF8BB87AB,
   0xF8B987AD, 0xF8B987AB, 0xF8BB97AB, 0xF8B997AD, 0xF8B997AB, 0xF8BBA7AB,
   0xF8B9A7AD, 0xF8B9A7A8, 0xF8B9A7A7, 0xF8B9A7A5, 0xF8B9A7A3, 0xF8BBB7A3,
   0xF8B9B7A6, 0xF8B9B7A6, 0xF8B9B7A5, 0xF8BBB7A3, 0xF8BDC7A3, 0xF8BBC7A6,
   0xF8BBC7A4, 0xF8BBC7A2, 0xF8B9C7A5, 0xF8B9C7A3, 0xF8B9C7A0, 0xF8B9C79F,
   0xF8B9C79E, 0xF8B9C79D, 0xF8B9C79C, 0xF8B9C79B, 0xF8B9C79A, 0xF8B9C79A,
   0xF8B9C799, 0xF8B9C799, 0xF8B9C798, 0xF8B9C798
};

/* E1 Rx Equalizer Values */
WP_U32 rlps_ram_e1[] = {
   0x03062C3E, 0x03062C3C, 0x03062C3A, 0x03062C38, 0x030E2C3F, 0x030E2C38,
   0x03162C3F, 0x03162C3B, 0x03162C38, 0x03163C3F, 0x03163C38, 0x031E3C3F,
   0x031E3C3C, 0x031E3C3A, 0x031E3C39, 0x031E3C38, 0x031E4C3F, 0x031E4C3C,
   0x031E4C3A, 0x031E4C38, 0x03264C3F, 0x03264C3B, 0x03264C38, 0x032E4C3F,
   0x032E4C3B, 0x032E4C39, 0x032E4C38, 0x032E5C3F, 0x032E5C3D, 0x032E5C3B,
   0x032E5C38, 0x032E6C3F, 0x032E6C38, 0x03366C3F, 0x03366C3C, 0x03366C3A,
   0x03366C38, 0x03367C3F, 0x03367C3D, 0x03367C3C, 0x03367C3A, 0x03367C39,
   0x03367C38, 0x0B3E7C3F, 0x0B3E683F, 0x0B3E683B, 0x0B3E683A, 0x0B3E6838,
   0x0B3E6CBF, 0x133E6CBD, 0x133E6CBC, 0x133E6CBA, 0x133E58B8, 0x133E68BD,
   0x1B3E6D3F, 0x1B3E6D3D, 0x1B3E6D3B, 0x1B3E6D3A, 0x1B3E6D38, 0x1B466D3F,
   0x1B466D3D, 0x23466D3C, 0x23466D3A, 0x23465938, 0x23466938, 0x23466DBF,
   0x23466DBC, 0x2B466DBA, 0x2B466DB8, 0x2B4E6DBF, 0x2A465DB8, 0x2A466DBD,
   0x2A466DBB, 0x324E6DBD, 0x324E6DBB, 0x3A4E59B8, 0x3A4E69B8, 0x3A4E6E3F,
   0x3A4E6E3C, 0x3A4E6E3A, 0x3A4E6E38, 0x3A566E3E, 0x3A566E38, 0x3A5E6E3F,
   0x425E6E3B, 0x425E6E38, 0x425E6A3E, 0x425E6A3D, 0x425E6EBF, 0x4A5E6EBE,
   0x4A5E6EBD, 0x4A5E6EBC, 0x4A5E6EBB, 0x4A5E6EBA, 0x4A5E6EB9, 0x4A665EB9,
   0x4A665EB8, 0x526E5EBF, 0x526E5EBB, 0x526E5EB8, 0x52765EBF, 0x52765EBE,
   0x52765EBD, 0x52765EBC, 0x5A765EBB, 0x5A765EBA, 0x5A765EB9, 0x5A765ABC,
   0x5A745ABF, 0x62764F38, 0x62765F38, 0x62745F3E, 0x62745F3D, 0x62745F3C,
   0x6A745F3B, 0x6A745F3A, 0x6A745F38, 0x6A744B38, 0x6A745B3F, 0x6A6C5FB8,
   0x6A745FBF, 0x72745FBE, 0x72745FBD, 0x72745FBB, 0x72745FBA, 0x72745FB9,
   0x727E5FBF, 0x71765FB8, 0x79865FBB, 0x798E5FBD, 0x798E5FBA, 0x79965FBE,
   0x78965FBE, 0x809E5FBF, 0x809E5FBB, 0x80A65FBD, 0x88A65FBA, 0x88A66FBE,
   0x88A66FBA, 0x90A67FBE, 0x90A67FBB, 0x90A67BBE, 0x90A67BBD, 0x90A67BBB,
   0x98A67BBA, 0x98A67BB8, 0x98A68BBB, 0x98A687BD, 0x98A687BB, 0xA0A687BA,
   0xA0A687B8, 0xA0AE87BE, 0xA0AE87BD, 0xA0AE87BC, 0xA0AE87BA, 0xA0AE87B9,
   0xA8AE87B8, 0xA8B687BE, 0xA8B687BD, 0xA8B687BB, 0xA8B687BA, 0xA8B687B8,
   0xB0BE87BE, 0xB0BE87BD, 0xB0BE87BC, 0xB0BE87BA, 0xB0BE87B9, 0xB8BE87B8,
   0xB8BC87BB, 0xB8BC97BE, 0xB8BC97BB, 0xB8BC97BA, 0xC0BC97B8, 0xC0BCA7BB,
   0xC0BAA7BE, 0xC0BAA7BC, 0xC0BAA7BA, 0xC8BAA7B9, 0xC8BAA7B8, 0xC8B8A7BC,
   0xC8BAB7BA, 0xC8B8B7BF, 0xC8B8B7BE, 0xC8B8B7BD, 0xD0B8B7B7, 0xD0B8B7B6,
   0xD0B8B7B5, 0xD8B8B7B5, 0xD8BAC7B4, 0xD8B8C7B7, 0xD8B8C7B6, 0xD8BAD7B5,
   0xD8B8D7B7, 0xD8B8D7B7, 0xD8B8D7B6, 0xE0BAD7AD, 0xE0BCE7AC, 0xE8BAE7AF,
   0xE8BAE7AE, 0xE8BAE7AD, 0xE8BCF7AB, 0xE8BAF7AE, 0xF0B8F7AE, 0xF0B8F7AD,
   0xF0BB07AB, 0xF0B907AE, 0xF0BB17AC, 0xF0B917AE, 0xF8BB27AC, 0xF8BB37AD,
   0xF8B937AE, 0xF8BB47AC, 0xF8B947AD, 0xF8BB57AB, 0xF8B957AD, 0xF8B957AC,
   0xF8B957AB, 0xF8B967AD, 0xF8BB77AB, 0xF8B977AE, 0xF8B977AD, 0xF8BB87AD,
   0xF8B987AE, 0xF8B987AD, 0xF8BB97AD, 0xF8B997AE, 0xF8B997AC, 0xF8BBA7AC,
   0xF8B9A7AE, 0xF8BBB7AE, 0xF8B9B7AF, 0xF8B9B7AE, 0xF8B9B7AD, 0xF8BBC7AD,
   0xF8B9C7AE, 0xF8B9C7AE, 0xF8B9C7AD, 0xF8B9C7AD, 0xF8B9C7AC, 0xF8B9C7AC,
   0xF8B9C7AB, 0xF8B9C7AB, 0xF8B9C7AB, 0xF8B9C7AA, 0xF8B9C7AA, 0xF8B9C7AA,
   0xF8B9C7AA, 0xF8B9C7A9, 0xF8B9C7A9, 0xF8B9C7A9, 0xF8B9C7A9, 0xF8B9C7A8,
   0xF8B9C7A8, 0xF8B9C7A8, 0xF8B9C7A8, 0xF8B9C7A8
};

WP_status WPU_EnableIndirectRegRW(wpu_comet_phy* phy, WP_U32 reg_type_address)
{
   /* Enable Per Channel Control (PCCE) and Indirect register write (IND) for reg_type_address:
      WPU_TPSC_CFG
      WPU_RPSC_CFG
      WPU_SIGX_CFG
      (Table 1) */

   WP_U8 value;

   WPI_MEM_BYTE_GET(value, phy->reg[reg_type_address]);
   WPI_MEM_BYTE_SET(phy->reg[reg_type_address], (value | WPU_PCCE_ENABLE | WPU_IND_ENABLE));

   return WP_OK;
}

WP_status
WPU_ReadIndirectReg(wpu_comet_phy* phy, WP_U32 reg_type_address, WP_U32 offset, WP_U8* value)
{
   WP_U32 i;
   WP_U32 reg_micro_access_status =          reg_type_address + REG_INDEX(1),
          reg_channel_indirect_data_buffer = reg_type_address + REG_INDEX(3),
          reg_channel_indirect_address =     reg_type_address + REG_INDEX(2);

   WPI_MEM_BYTE_SET(phy->reg[reg_channel_indirect_address], offset | 0x80);

   for (i=0; i<WPU_MAX_BUSY_READ; i++)
   {
      WPI_MEM_BYTE_GET((*value), phy->reg[reg_micro_access_status]);
      if (((*value) & WPU_BIT_BUSY) == 0)
      {
         WPI_MEM_BYTE_GET((*value), phy->reg[reg_channel_indirect_data_buffer]);
         return WP_OK;
      }
   }
   return WP_ERR_WDDI;
}

WP_status
WPU_WriteIndirectReg(wpu_comet_phy* phy, WP_U32 reg_type_address, WP_U32 offset, WP_U8 value)
{
   WP_U32 i;
   WP_U8  temp;
   WP_U32 reg_micro_access_status =          reg_type_address + REG_INDEX(1),
          reg_channel_indirect_data_buffer = reg_type_address + REG_INDEX(3),
          reg_channel_indirect_address =     reg_type_address + REG_INDEX(2);

   WPI_MEM_BYTE_SET(phy->reg[reg_channel_indirect_data_buffer], value);
   WPI_MEM_BYTE_SET(phy->reg[reg_channel_indirect_address],     offset & 0x7F);

   for (i=0; i<WPU_MAX_BUSY_READ; i++)
   {
      WPI_MEM_BYTE_GET(temp, phy->reg[reg_micro_access_status]);
      if ((temp & WPU_BIT_BUSY) == 0)
      {
         return WP_OK;
      }
   }
   return WP_ERR_WDDI;
}

/*****************************************************************************
 *
 * Function:   WPU_TdmCometCardInit
 *
 * Purpose:    T1/E1 card Comet initialization
 *
 *
 * Description:
 *
 *
 * Inputs:
 *     WP_U32 port      (WP_PORT_TDM1 ... WP_PORT_TDM16)
 *     WP_U32 mode      Comet CPLD or Comet Initialization
 *
 * Outputs:
 *     The state of the Comet is changed.
 *
 * Return Value:
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/
static WP_boolean reset_done = WP_FALSE;

WP_status WPU_TdmCometCardInit(WP_U32 port, WP_U32 mode)
{
   volatile wpu_comet *comet;
   wpu_comet_cpld *cpld;

   wpi_match_mask save;
   WP_status status = WP_OK;

   WP_U32 phy;
   WP_U32 i;
   WP_U8 value;
   
   /* Restrict access to match/mask regs. */
   WPI_WDDI_ATTACH();

   if(port >= WP_UFE_UPI1_TDM1 &&
      port <= WP_UFE_UPI2_TDM8 )
   {
      /* Save current match/mask, install UFE2 values. */
      if(port <= WP_UFE_UPI1_TDM8)
          WPX_SetBank(&save, &wpi_board_data.match_mask->ufe[0]);
      else 
         WPX_SetBank(&save, &wpi_board_data.match_mask->ufe[1]);
   }
   else if (port >= WP_UFE_UPI3_TDM1 &&
      port <= WP_UFE_UPI3_TDM8 )
   {
      /* Save current match/mask, install TDM over serial 3 values. */
      WPX_SetBank(&save, &wpi_board_data.match_mask->tdm[2]);
   }

   /* Get address to CPLD/Comet for TDM 1 - 8 */
   if(port >= WP_PORT_TDM1 && port <= WP_PORT_TDM8) {      
      WPX_SetBank(&save, &wpi_board_data.match_mask->tdm[0]);
      cpld = (wpu_comet_cpld *) WPL_PHY_VIRTUAL(0, APP_CPLD_TDM1_COMET);
      comet = (wpu_comet *) WPL_PHY_VIRTUAL(0, APP_TDM1_COMET);
      phy = port - WP_PORT_TDM1;
   }

   /* Get address to CPLD/Comet for TDM 9 - 16 */
   else if(port >= WP_PORT_TDM9 && port <= WP_PORT_TDM16) {
       WPX_SetBank(&save, &wpi_board_data.match_mask->tdm[1]);
       cpld = (wpu_comet_cpld *) WPL_PHY_VIRTUAL(0, APP_CPLD_TDM9_COMET);
       comet = (wpu_comet *) WPL_PHY_VIRTUAL(0, APP_TDM9_COMET);
       phy = port - WP_PORT_TDM9;
   }

   /* Get address to CPLD/Comet for UFE over UPI2 1 - 8 */
   else if(port >= WP_UFE_UPI2_TDM1 && port <= WP_UFE_UPI2_TDM8) {
      cpld = (wpu_comet_cpld *) WPL_PHY_VIRTUAL(0, APP_CPLD_TDM1_COMET);
       comet = (wpu_comet *) WPL_PHY_VIRTUAL(0, APP_TDM1_COMET);
       phy = port - WP_UFE_UPI2_TDM1;
   }
   /* Get address to CPLD/Comet for UFE over UPI3 1 - 8 */
   else if (port >= WP_UFE_UPI3_TDM1 && port <= WP_UFE_UPI3_TDM8) {
      cpld = (wpu_comet_cpld *) WPL_PHY_VIRTUAL(0, APP_CPLD_TDM_UFE_COMET);
      comet = (wpu_comet *) WPL_PHY_VIRTUAL(0, APP_TDM1_UFE_COMET);
      phy = port - WP_UFE_UPI3_TDM1;
   }
   /* Else, invalid PORT value */
   else {
      status = WPI_ERROR(WP_ERR_PORT);
      goto clean_return;
   }

   /* comet phy reset*/
   if(reset_done == WP_FALSE)
   {
      reset_done = WP_TRUE;
      WPI_MEM_BYTE_SET(comet->phy[0].reg[REG_INDEX(0x0e)], 0x3);
      WPL_Delay(100);
      WPI_MEM_BYTE_SET(comet->phy[0].reg[REG_INDEX(0x0e)], 0);
      WPL_Delay(100);
   }
   
   /* This is the default value of the register - rewrite due to bug in COMET HW (read and write) */
   WPI_MEM_BYTE_GET(value, comet->phy[phy].reg[REG_INDEX(0x40)]);
   WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x38);
   /* End of COMET HW bug issue. */

   switch (mode) {
      case WPU_INIT_COMET_CPLD_T1:
      case WPU_INIT_COMET_CPLD_E1:
      case WPU_INIT_COMET_CPLD_CR_T1:
      case WPU_INIT_COMET_CPLD_CR_E1:
          /* Delay before writing to CPLD */
          WPL_Delay(200);

          /* Unreset comet */
          WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(1)], 0xa);

          /* Delay before writing to CPLD */
          WPL_Delay(200);

          /* Framer Enable */
          /* registers 0x08 to 0x0f control channels 1 to 8
             configuration respectively */
          if (mode == WPU_INIT_COMET_CPLD_CR_T1 ||
              mode == WPU_INIT_COMET_CPLD_CR_E1)
          {
             /* regs configured to set all channels to TxCLK/TxSync as inputs
                and TxCLK/TxSync comes from the WDS2 board */
             WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(0x08)], 0x08);
             WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(0x09)], 0x09);
             WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(0x0a)], 0x0a);
             WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(0x0b)], 0x0b);
             WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(0x0c)], 0x0c);
             WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(0x0d)], 0x0d);
             WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(0x0e)], 0x0e);
             WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(0x0f)], 0x0f);
          }
          else
          {/* regs configured to short the TX to the RX clock. this mode doesn't support CR */
             WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(0x08)], 0x10);
             WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(0x09)], 0x11);
             WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(0x0a)], 0x12);
             WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(0x0b)], 0x13);
             WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(0x0c)], 0x14);
             WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(0x0d)], 0x15);
             WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(0x0e)], 0x16);
             WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(0x0f)], 0x17);
          }

          WPL_Delay(200);

          if (mode == WPU_INIT_COMET_CPLD_T1 ||
              mode == WPU_INIT_COMET_CPLD_CR_T1)
            WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(2)], 0x5);
          else
            WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(2)], 0x9);

          if (port >= WP_UFE_UPI3_TDM1 &&
              port <= WP_UFE_UPI3_TDM8 )
          {
             /* In current WDS3 UFE3-CAD mode uses COMET over UPI3 */
             WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(3)], 0x42);
          }
          else
          {
             WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(3)], 0x2);
          }

          /* Turn LED Off */
          WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(4)], 0x8);

#if 0 /* Debug option */          
          {
             WP_U8 tmp_byte;
             
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(0)]);
             printf("REG_INDEX %d value %d\n", 0,tmp_byte);
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(1)]);
             printf("REG_INDEX %d value %d\n", 1,tmp_byte);
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(2)]);
             printf("REG_INDEX %d value %d\n", 2,tmp_byte);
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(3)]);
             printf("REG_INDEX %d value %d\n", 3,tmp_byte);
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(4)]);
             printf("REG_INDEX %d value %d\n", 4,tmp_byte);
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(5)]);
             printf("REG_INDEX %d value %d\n", 5,tmp_byte);
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(6)]);
             printf("REG_INDEX %d value %d\n", 6,tmp_byte);
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(7)]);
             printf("REG_INDEX %d value %d\n", 7,tmp_byte);
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(8)]);
             printf("REG_INDEX %d value %d\n", 8,tmp_byte);
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(9)]);
             printf("REG_INDEX %d value %d\n", 9,tmp_byte);
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(10)]);
             printf("REG_INDEX %d value %d\n", 10,tmp_byte);
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(11)]);
             printf("REG_INDEX %d value %d\n", 11,tmp_byte);
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(12)]);
             printf("REG_INDEX %d value %d\n", 12,tmp_byte);
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(13)]);
             printf("REG_INDEX %d value %d\n", 13,tmp_byte);
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(14)]);
             printf("REG_INDEX %d value %d\n", 14,tmp_byte);
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(15)]);
             printf("REG_INDEX %d value %d\n", 15,tmp_byte);
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(16)]);
             printf("REG_INDEX %d value %d\n", 16,tmp_byte);
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(17)]);
             printf("REG_INDEX %d value %d\n", 17,tmp_byte);
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(18)]);
             printf("REG_INDEX %d value %d\n", 18,tmp_byte);
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(19)]);
             printf("REG_INDEX %d value %d\n", 19,tmp_byte);
             WPI_MEM_BYTE_GET(tmp_byte, cpld->reg[REG_INDEX(20)]);
             printf("REG_INDEX %d value %d\n", 20,tmp_byte);
          }
#endif /* Debug option */

          break;

      /*****************************/
      /* Initialize comet T1 port */
      /*****************************/
      case WPU_MODE_TDM_T1_MASTER_UNFRAMED:
      case WPU_MODE_TDM_T1_SLAVE_UNFRAMED:

           /* Global Configuration Register - set to T1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x00)], 0x0);

           /* Enable Jitter Attenuation and bypass elastic store */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x02)], 0xc0);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x05)], 0x08);

           if (mode == WPU_MODE_TDM_T1_SLAVE_UNFRAMED)
           {
              /* source of Tx CLK:CTCLK, JAT:NO JAT, BACKPLANE is SLAVE */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x1c);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x0a)], 0x00);
           }

           /* (RJAT) - Rx Jitter Attenuation Settings */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x17)], 0x10);

           /* (TJAT) - Tx Jitter Attenuation Settings */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x19)], 0x2f);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1a)], 0x2f);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1b)], 0x10);

           /* Rx: Set to Clock-Master mode */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x30)], 0x00);

           /* Rx: Frame-Pulse Master */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x31)], 0x00);

           if(port >= WP_UFE_UPI2_TDM1 && port <= WP_UFE_UPI3_TDM8)
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x17);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x0f);
           }
           else
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x18);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x80);
           }

           /* Disable Transmit Data link */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x38)], 0x00);

           WPL_Delay(100);
           /* Clock Synthesis Config - Tx Clock = 1.544 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xd6)], 0x1);

           /* Line Coding = B8ZS */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x10)], 0x00);

           /* Reset T1-XBAS register settings */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x54)], 0x0);

           /* Configure T1 framer to UNFRAMED */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x54)], 0x20);

           /* RLPS Voltage Threshold #1*/
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdc)], 0x26);

           /* RLPS Voltage Threshold #2 - This is different for E1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdd)], 0xc3);

           /* Initialize T1 Receive Equalizer RAM */
           if((status = WPUI_TdmCometInitEq((wpu_comet_phy *)&comet->phy[phy],
                  sizeof(rlps_ram_t1), (WP_U8 *)rlps_ram_t1)))
                goto clean_return;

           WPL_Delay(100);

           /* Enable Squelch */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf8)], 0x11);

           /* LOS Detection/Clearance threshold - This is different for E1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf9)], 0x88);

           /* LOS Detection period - This is different for E1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfa)], 0x20);

           /* LOS Clearance period - This is different for E1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfb)], 0x20);

           /* Reset RLPS status and control register */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfe)], 0x00);

           /* Enable RLPS Equalizer */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xff)], 0x0b);

           /* Configure Tx waveform */
           if((status = WPUI_TdmCometInitWf((wpu_comet_phy *)&comet->phy[phy],
                    sizeof(wave_form_t1), (WP_U8 *)wave_form_t1)))
                goto clean_return;

           WPL_Delay(100);
           /* Enable TX for Short Haul -> 0 to 110 ft. */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_XLPG_CFG], 0x10);

           WPL_Delay(100);

           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x01);
           WPL_Delay(200);

           /* Enable Receiver */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xbe)], 0x00);

           break;

      case WPU_MODE_TDM_T1_MASTER:
      case WPU_MODE_TDM_T1_SLAVE:
      case WPU_MODE_TDM_T1_CR_MASTER:
      case WPU_MODE_TDM_T1_CR_SLAVE:
      case WPU_MODE_TDM_T1_CR_CAS_MASTER:
           /* Global Configuration Register - set to T1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x00)], 0x0);

           /* Enable Jitter Attenuation and bypass elastic store */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x02)], 0x20);

           if (mode == WPU_MODE_TDM_T1_SLAVE)
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x0a)], 0x00);
           }

           /* (RJAT) - Rx Jitter Attenuation Settings */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x17)], 0x10);

           /* (TJAT) - Tx Jitter Attenuation Settings */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x19)], 0x2f);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1a)], 0x2f);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1b)], 0x10);

           /* Rx: Set to Clock-Master mode */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x30)], 0x00);

           /* Rx: Frame-Pulse Master */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x31)], 0x00);

           /* Bit offset Disable */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x34)], 0x00);

           /* Tx: Set to Clock-Master mode */
           if(port >= WP_UFE_UPI2_TDM1 && port <= WP_UFE_UPI3_TDM8)
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x38);
           else
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x28);

           /* Tx: Frame-Pulse Master */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x41)], 0x00);

           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x17);

           if(port >= WP_UFE_UPI2_TDM1 && port <= WP_UFE_UPI3_TDM8)
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x17);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x0f);
           }
           else
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x18);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x80);
           }

           /* Disable Transmit Data link */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x38)], 0x00);

           WPL_Delay(100);
           /* Clock Synthesis Config - Tx Clock = 1.544 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xd6)], 0x1);

           /* Line Coding = B8ZS */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x10)], 0x00);

           /* Reset T1-XBAS register settings */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x54)], 0x0);

           /* Configure T1 framer to ESF */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x48)], 0x30);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x54)], 0x30);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x60)], 0x10);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x50)], 0x04);

           /* T1 Input frame format - elastic store */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_TXELST_CFG], 0x00);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_RXELST_CFG], 0x00);

           /* RLPS Voltage Threshold #1*/
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdc)], 0x26);

           /* RLPS Voltage Threshold #2 - This is different for E1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdd)], 0xc3);

           /* Initialize T1 Receive Equalizer RAM */
           if((status = WPUI_TdmCometInitEq((wpu_comet_phy *)&comet->phy[phy],
                  sizeof(rlps_ram_t1), (WP_U8 *)rlps_ram_t1)))
                goto clean_return;

           WPL_Delay(100);

           /* Enable Squelch */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf8)], 0x11);

           /* LOS Detection/Clearance threshold - This is different for E1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf9)], 0x88);

           /* LOS Detection period - This is different for E1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfa)], 0x20);

           /* LOS Clearance period - This is different for E1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfb)], 0x20);

           /* Reset RLPS status and control register */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfe)], 0x00);

           /* Enable RLPS Equalizer */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xff)], 0x0b);

           /* Configure Tx waveform */
           if((status = WPUI_TdmCometInitWf((wpu_comet_phy *)&comet->phy[phy],
                    sizeof(wave_form_t1), (WP_U8 *)wave_form_t1)))
                goto clean_return;

           WPL_Delay(100);
           /* Enable TX for Short Haul -> 0 to 110 ft. */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_XLPG_CFG], 0x10);

           WPL_Delay(100);

           /* Enable TxElasticStore - needed for external */
           /* loopback cable or "master" mode.            */
           if (mode == WPU_MODE_TDM_T1_CR_MASTER)
           {
              /* clock recovery works with Backplane clock (0x01).
                 Change reg[0x06] if clock isn't convergence. */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x01);
           }
           else if (mode == WPU_MODE_TDM_T1_CR_SLAVE)
           {
              /* Bypass TxElasticStore - needed for slave/loop-timed  */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x0c);
           }
           else if (mode == WPU_MODE_TDM_T1_MASTER)
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x0c);
           /* Bypass TxElasticStore - needed for slave/loop-timed  */
           else if (mode == WPU_MODE_TDM_T1_SLAVE)
           {
              /* source of Tx CLK:CTCLK, JAT:NO JAT, BACKPLANE is SLAVE */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x1c);
           }
           else
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x01);
           }

           WPL_Delay(200);

           /* Enable Receiver */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xbe)], 0x00);

           if (mode == WPU_MODE_TDM_T1_CR_CAS_MASTER)
           {
              /* Enable Per Channel Control (PCCE) and Indirect register write (IND) for
                 TPSC (Table 1) */
              WPU_EnableIndirectRegRW((wpu_comet_phy*)(&(comet->phy[phy])), WPU_TPSC_CFG);

              /* Set the SIGC[0] to '0', SIGC[1] to '1'
                 in indirect registers 0x60-0x80 for TPSC */
              for (i=0x60; i<0x80; i++)
              {
                 WPU_WriteIndirectReg(
                    (wpu_comet_phy*)(&(comet->phy[phy])), WPU_TPSC_CFG, i, 0x40);
              }

           }

           break;

      /*****************************/
      /* Initialize commet E1 port */
      /*****************************/

      case WPU_MODE_TDM_E1_MASTER_UNFRAMED:
      case WPU_MODE_TDM_E1_SLAVE_UNFRAMED:
           /* Global Configuration Register - set to E1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x00)], 0x1);

           /* RX: Enable Jitter Attenuation and bypass elastic store */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x02)], 0xc0);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x05)], 0x08);

           if (mode == WPU_MODE_TDM_E1_SLAVE_UNFRAMED)
           {
              /* source of Tx CLK:CTCLK, JAT:NO JAT, BACKPLANE is SLAVE */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x1c);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x0a)], 0x00);
           }

           /* (RJAT) - Rx Jitter Attenuation Settings */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x15)], 0xff);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x16)], 0xff);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x17)], 0x10);

           /* (TJAT) - Tx Jitter Attenuation Settings */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x19)], 0xff);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1a)], 0xff);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1b)], 0x10);

           /* Rx: Set to Clock-Master mode, Rate = 2.048 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x30)], 0x01);

           /* Rx: Frame-Pulse Master */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x31)], 0x00);

           /* Bit offset */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x33)], 0x00);
           if(port >= WP_UFE_UPI2_TDM1 && port <= WP_UFE_UPI3_TDM8)
           { 
              /*this if isn't performed at all */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x34)], 0x00);
              /* Tx: Set to Clock-Master mode, Rate = 2.048 */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x39);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x00);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x00);

           }
           else
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x34)], 0x08);
              /* Tx: Set to Clock-Master mode, Rate = 2.048 */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x29);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x00);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x09);
           }

           /* Tx: Frame-Pulse Master */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x41)], 0x00);

           /* Basic Framing - Disable Signaling/Data link - Generate CRC */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x80)], 0x08);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x90)], 0x40);

           WPL_Delay(100);
           /* Clock Synthesis Config - Tx Clock = 2.048 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xd6)], 0x00);

           /* Line Coding = HDB3 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x10)], 0x00);

           /* E1 Input frame format - elastic store */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_TXELST_CFG], 0x03);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_RXELST_CFG], 0x03);

           /* RLPS Voltage Threshold #1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdc)], 0x26);

           /* RLPS Voltage Threshold #2 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdd)], 0x03);

           /* Initialize E1 Receive Equalizer RAM */
           if((status = WPUI_TdmCometInitEq((wpu_comet_phy *)&comet->phy[phy],
                     sizeof(rlps_ram_e1), (WP_U8 *)rlps_ram_e1)))
                 goto clean_return;

           /* Enable Squelch */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf8)], 0x11);

           /* LOS Detection/Clearance threshold */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf9)], 0xcc);

           /* LOS Detection period */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfa)], 0x2d);

           /* LOS Clearance period */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfb)], 0x2d);

           /* Reset RLPS status and control register */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfe)], 0x00);

           /* Enable RLPS Equalizer */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xff)], 0x0b);

           /* Configure Tx waveform */
           if((status = WPUI_TdmCometInitWf((wpu_comet_phy *)&comet->phy[phy],
                     sizeof(wave_form_e1), (WP_U8 *)wave_form_e1)))
                 goto clean_return;

           /* Enable TX for Short Haul -> 0 to 110 ft. */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_XLPG_CFG], 0x10);

           WPL_Delay(100);

           /* Disable TxElasticStore Bypass - needed */
           /* for external loopback cable.           */
           if (mode == WPU_MODE_TDM_E1_MASTER_UNFRAMED)
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x01);

           /* Bypass TxElasticStore - needed for slave/loop-timed  */
           else
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x0c);
           }


           WPL_Delay(200);
#if 0
           /* add line loopback(for debug mode only: [0xn0A] <= 0001-0000) */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x0a)], 0x10);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x00);
#endif /*0*/
           /* Enable Receiver */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xbe)], 0x00);

          break;

      case WPU_MODE_TDM_E1_MASTER:
      case WPU_MODE_TDM_E1_SLAVE:
      case WPU_MODE_TDM_E1_CR_MASTER:
      case WPU_MODE_TDM_E1_CR_SLAVE:
      case WPU_MODE_TDM_E1_CR_CAS_MASTER:
           /* Global Configuration Register - set to E1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x00)], 0x1);

           /* RX: Enable Jitter Attenuation and bypass elastic store */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x02)], 0x30);

           if (mode == WPU_MODE_TDM_E1_SLAVE)
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x0a)], 0x00);
           }

           /* (RJAT) - Rx Jitter Attenuation Settings */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x15)], 0xff);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x16)], 0xff);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x17)], 0x10);

           /* (TJAT) - Tx Jitter Attenuation Settings */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x19)], 0xff);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1a)], 0xff);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1b)], 0x10);

           /* Rx: Set to Clock-Master mode, Rate = 2.048 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x30)], 0x01);

           /* Rx: Frame-Pulse Master */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x31)], 0x00);

           /* Bit offset */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x33)], 0x00);
           if(port >= WP_UFE_UPI2_TDM1 && port <= WP_UFE_UPI3_TDM8)
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x34)], 0x00);
              /* Tx: Set to Clock-Master mode, Rate = 2.048 */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x39);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x00);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x00);

           }
           else
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x34)], 0x08);
              /* Tx: Set to Clock-Master mode, Rate = 2.048 */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x29);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x00);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x09);
           }

           /* Tx: Frame-Pulse Master */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x41)], 0x00);

           if (mode == WPU_MODE_TDM_E1_CR_CAS_MASTER)
           {
              /* CAS Framing - Enable Signaling/Data link - Generate CRC */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x80)], 0x70);
           }
           else
           {
              /* Basic Framing - Disable Signaling/Data link - Generate CRC */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x80)], 0x10);
           }

           if (mode == WPU_MODE_TDM_E1_CR_SLAVE ||
               mode == WPU_MODE_TDM_E1_CR_MASTER)
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x90)], 0xc0);
           else if (mode == WPU_MODE_TDM_E1_CR_CAS_MASTER)
           {
              /* Enable CAS - set CASDIS=0 */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x90)], 0x80);
           }

           WPL_Delay(100);
           /* Clock Synthesis Config - Tx Clock = 2.048 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xd6)], 0x00);

           /* Line Coding = HDB3 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x10)], 0x00);

           /* E1 Input frame format - elastic store */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_TXELST_CFG], 0x03);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_RXELST_CFG], 0x03);

           /* RLPS Voltage Threshold #1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdc)], 0x26);

           /* RLPS Voltage Threshold #2 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdd)], 0x03);

           /* Initialize E1 Receive Equalizer RAM */
           if((status = WPUI_TdmCometInitEq((wpu_comet_phy *)&comet->phy[phy],
                     sizeof(rlps_ram_e1), (WP_U8 *)rlps_ram_e1)))
                 goto clean_return;

           /* Enable Squelch */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf8)], 0x11);

           /* LOS Detection/Clearance threshold */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf9)], 0xcc);

           /* LOS Detection period */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfa)], 0x2d);

           /* LOS Clearance period */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfb)], 0x2d);

           /* Reset RLPS status and control register */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfe)], 0x00);

           /* Enable RLPS Equalizer */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xff)], 0x0b);

           /* Configure Tx waveform */
           if((status = WPUI_TdmCometInitWf((wpu_comet_phy *)&comet->phy[phy],
                     sizeof(wave_form_e1), (WP_U8 *)wave_form_e1)))
                 goto clean_return;

           /* Enable TX for Short Haul -> 0 to 110 ft. */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_XLPG_CFG], 0x10);

           WPL_Delay(100);

           /* Disable TxElasticStore Bypass - needed */
           /* for external loopback cable.           */
           if (mode == WPU_MODE_TDM_E1_MASTER)
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x0c);

           /* Bypass TxElasticStore - needed for slave/loop-timed  */
           else if (mode == WPU_MODE_TDM_E1_SLAVE)
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x1c);
           }
           else if (mode == WPU_MODE_TDM_E1_CR_MASTER)
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x01);

           /* Bypass TxElasticStore - needed for slave/loop-timed  */
           else if (mode == WPU_MODE_TDM_E1_CR_SLAVE)
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x0c);
           }

           WPL_Delay(200);
#if 0
           /* add line loopback(for debug mode only: [0xn0A] <= 0001-0000) */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x0a)], 0x10);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x00);
#endif /*0*/
           /* Enable Receiver */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xbe)], 0x00);

          break;

      /* T1 CAS */
      case WPU_MODE_TDM_TRANS_T1_CAS_MASTER:
      case WPU_MODE_TDM_TRANS_T1_CAS_SLAVE:

         /* Global Configuration Register - set to T1 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x00)], 0x0);

         /* Enable Jitter Attenuation and bypass elastic store */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x02)], 0x20);

         if (mode == WPU_MODE_TDM_TRANS_T1_CAS_SLAVE)
         {
            /* source of Tx CLK:CTCLK, JAT:NO JAT, BACKPLANE is SLAVE */
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x0a)], 0x00);
         }

         /* (RJAT) - Rx Jitter Attenuation Settings */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x17)], 0x10);

         /* (TJAT) - Tx Jitter Attenuation Settings (1.544Mhz from backplane,
            recovered or CTCLK */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x19)], 0x2f);
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1a)], 0x2f);
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1b)], 0x10);

         /* Rx: Set to Clock-Master mode */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x30)], 0x00);

         /* Rx: Frame-Pulse Master */
         if(port >= WP_UFE_UPI2_TDM1 && port <= WP_UFE_UPI3_TDM8)  /* CAD */
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x31)], 0x04);
            /* FPMODE = 0 (BRFP is output)                                        */
            /* BZSMFP = 1 (BRFP output pulse in the first frame of the multiframe */
         else                                                      /* TDI */
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x31)], 0x00);

         /* Bit offset Disable */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x34)], 0x00);

         /* Tx: Set to backplane Clock-slave mode (Comet is Master) */
         if(port >= WP_UFE_UPI2_TDM1 && port <= WP_UFE_UPI3_TDM8)
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x38);
         else
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x28);

         /* Tx: Frame-Pulse Master */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x41)], 0x00);

         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x17);

         if(port >= WP_UFE_UPI2_TDM1 && port <= WP_UFE_UPI3_TDM8)
         {
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x17);
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x0f);
         }
         else
         {
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x18);
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x80);
         }

         /* Disable Transmit Data link */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x38)], 0x00);

         WPL_Delay(100);
         /* Clock Synthesis Config - Tx Clock = 1.544 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xd6)], 0x1);

         /* Line Coding = B8ZS */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x10)], 0x00);

         /* Reset T1-XBAS register settings */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x54)], 0x0);

         /* Configure T1 framer to ESF */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x48)], 0x30);
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x54)], 0x30); /* ESF=1 B8ZS=1 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x60)], 0x10); /* ESF=1        */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x50)], 0x04); /* ESF=1        */

         /* T1 Input frame format - elastic store */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_TXELST_CFG], 0x00);
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_RXELST_CFG], 0x00);

         /* RLPS Voltage Threshold #1*/
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdc)], 0x26);

         /* RLPS Voltage Threshold #2 - This is different for E1 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdd)], 0xc3);

         /* Initialize T1 Receive Equalizer RAM */
         if((status = WPUI_TdmCometInitEq((wpu_comet_phy *)&comet->phy[phy],
                                          sizeof(rlps_ram_t1), (WP_U8 *)rlps_ram_t1)))
            goto clean_return;

         WPL_Delay(100);

         /* Enable Squelch */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf8)], 0x11);

         /* LOS Detection/Clearance threshold - This is different for E1 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf9)], 0x88);

         /* LOS Detection period - This is different for E1 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfa)], 0x20);

         /* LOS Clearance period - This is different for E1 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfb)], 0x20);

         /* Reset RLPS status and control register */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfe)], 0x00);

         /* Enable RLPS Equalizer */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xff)], 0x0b);

         /* Configure Tx waveform */
         if((status = WPUI_TdmCometInitWf((wpu_comet_phy *)&comet->phy[phy],
                                          sizeof(wave_form_t1), (WP_U8 *)wave_form_t1)))
            goto clean_return;

         WPL_Delay(100);
         /* Enable TX for Short Haul -> 0 to 110 ft. */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_XLPG_CFG], 0x10);

         WPL_Delay(100);

         /* Enable TxElasticStore - needed for external */
         /* loopback cable or "master" mode.            */
         if (mode == WPU_MODE_TDM_TRANS_T1_CAS_MASTER)
         {
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x0c);
         }

         /* Bypass TxElasticStore - needed for slave/loop-timed  */
         else if (mode == WPU_MODE_TDM_TRANS_T1_CAS_SLAVE)
         {
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x1c);
         }

         WPL_Delay(200);

         /* Enable Receiver */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xbe)], 0x00);


         /*    TPSC   */
         /* Enable Per Channel Control (PCCE) and Indirect register write (IND) for
            TPSC (Table 1) */
         WPU_EnableIndirectRegRW((wpu_comet_phy*)(&(comet->phy[phy])), WPU_TPSC_CFG);

         /* Set the INVERT=0, SIGNINV=0 in indirect registers 0x20-0x40 for TPSC */
          for (i=0x20; i<0x40; i++)
          {
             WPU_WriteIndirectReg((wpu_comet_phy*)(&(comet->phy[phy])), WPU_TPSC_CFG, i, 0x00);
          }

         /* Set the SIGC[0] to '0', SIGC[1] to '1' in indirect registers 0x60-0x80 for TPSC */
         for (i=0x60; i<0x80; i++)
         {
            WPU_WriteIndirectReg((wpu_comet_phy*)(&(comet->phy[phy])), WPU_TPSC_CFG, i, 0x40);
         }
          /* RPSC */
          WPU_EnableIndirectRegRW((wpu_comet_phy*)(&(comet->phy[phy])), WPU_RPSC_CFG);
          for (i=0x20; i<0x40; i++)
          {
             WPU_WriteIndirectReg((wpu_comet_phy*)(&(comet->phy[phy])), WPU_RPSC_CFG, i, 0x00);
             /* SIGNINV = 0 */
          }
          /*  SIGX */
          WPU_EnableIndirectRegRW((wpu_comet_phy*)(&(comet->phy[phy])), WPU_SIGX_CFG);
          for (i=0x40; i<0x60; i++)
          {
             WPU_WriteIndirectReg((wpu_comet_phy*)(&(comet->phy[phy])), WPU_SIGX_CFG, i, 0x00);
             /* RINV[1] = 0
                RFIX    = 0
                RPOL    = 0  (fixes to 0) */
          }

         break; /* T1 CAS */

         /* E1 CAS */
      case WPU_MODE_TDM_TRANS_E1_CAS_MASTER:
      case WPU_MODE_TDM_TRANS_E1_CAS_SLAVE:

         /* Global Configuration Register - set to E1 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x00)], 0x1);

         /* RX: Enable Jitter Attenuation and bypass elastic store */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x02)], 0x30);

         if (mode == WPU_MODE_TDM_TRANS_E1_CAS_SLAVE)
         {
            /* source of Tx CLK:CTCLK, JAT:NO JAT, BACKPLANE is SLAVE */
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x1c);
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x0a)], 0x00);
         }

         /* (RJAT) - Rx Jitter Attenuation Settings */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x15)], 0xff);
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x16)], 0xff);
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x17)], 0x10);

         /* (TJAT) - Tx Jitter Attenuation Settings */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x19)], 0xff);
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1a)], 0xff);
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1b)], 0x10);

         /* Rx: Set to Clock-Master mode, Rate = 2.048 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x30)], 0x01);

         /* Rx: Frame-Pulse Master */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x31)], 0x00);

         /* Bit offset */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x33)], 0x00);
         if(port >= WP_UFE_UPI2_TDM1 && port <= WP_UFE_UPI3_TDM8)
         {
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x34)], 0x00);
            /* Tx: Set to backplane Clock-slave mode (Comet is Master), Rate = 2.048 */
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x39);
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x00);
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x00);
         }
         else
         {
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x34)], 0x08);
            /* Tx: Set to backplane Clock-slave mode (Comet is Master), Rate = 2.048 */
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x29);
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x00);
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x09);
         }

         /* Tx: Frame-Pulse Master */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x41)], 0x00);

         /* CAS enable */
         /* For Transmit(table 5)   SIGEN=DLEN='1'  GENCRC='1'  */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x80)], 0x70);
         /* CRCEN='1'  CASDIS='0' to enable multiframe          */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x90)], 0x80);

         WPL_Delay(100);
         /* Clock Synthesis Config - Tx Clock = 2.048 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xd6)], 0x00);

         /* Line Coding = HDB3 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x10)], 0x00);

         /* E1 Input frame format - elastic store */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_TXELST_CFG], 0x03);
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_RXELST_CFG], 0x03);

         /* RLPS Voltage Threshold #1 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdc)], 0x26);

         /* RLPS Voltage Threshold #2 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdd)], 0x03);

         /* Initialize E1 Receive Equalizer RAM */
         if((status = WPUI_TdmCometInitEq((wpu_comet_phy *)&comet->phy[phy],
                                          sizeof(rlps_ram_e1), (WP_U8 *)rlps_ram_e1)))
            goto clean_return;

         /* Enable Squelch */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf8)], 0x11);

         /* LOS Detection/Clearance threshold */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf9)], 0xcc);

         /* LOS Detection period */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfa)], 0x2d);

         /* LOS Clearance period */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfb)], 0x2d);

         /* Reset RLPS status and control register */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfe)], 0x00);

         /* Enable RLPS Equalizer */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xff)], 0x0b);

         /* Configure Tx waveform */
         if((status = WPUI_TdmCometInitWf((wpu_comet_phy *)&comet->phy[phy],
                                          sizeof(wave_form_e1), (WP_U8 *)wave_form_e1)))
            goto clean_return;

         /* Enable TX for Short Haul -> 0 to 110 ft. */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_XLPG_CFG], 0x10);

         WPL_Delay(100);

         /* Disable TxElasticStore Bypass - needed */
         /* for external loopback cable.           */
         if (mode == WPU_MODE_TDM_TRANS_E1_CAS_MASTER)
         {
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x0c);
         }

         /* Bypass TxElasticStore - needed for slave/loop-timed  */
         else if (mode == WPU_MODE_TDM_TRANS_E1_CAS_SLAVE)
         {
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x01);
         }

         WPL_Delay(200);
#if 0
         /* add line loopback(for debug mode only: [0xn0A] <= 0001-0000) */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x0a)], 0x10);
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x00);
#endif /*0*/
         /* Enable Receiver */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xbe)], 0x00);

         break;  /* E1 CAS */

      /*****************************/
      /* Initialize comet T1 port */
      /*****************************/
      case WPU_MODE_TDM_T1_MASTER_UNFRAMED_PWE3_CES_CR:
      case WPU_MODE_TDM_T1_SLAVE_UNFRAMED_PWE3_CES_CR:

           /* Global Configuration Register - set to T1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x00)], 0x0);

           /* Enable Jitter Attenuation and bypass elastic store */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x02)], 0xc0);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x05)], 0x08);

           if (mode == WPU_MODE_TDM_T1_SLAVE_UNFRAMED_PWE3_CES_CR)
           {
              /* source of Tx CLK:CTCLK, JAT:NO JAT, BACKPLANE is SLAVE */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x1c);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x0a)], 0x00);
           }
           else
           {
              WPL_Delay(100);
              
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x01);
              
              WPL_Delay(200);
           }

           /* (RJAT) - Rx Jitter Attenuation Settings */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x17)], 0x10);

           /* (TJAT) - Tx Jitter Attenuation Settings */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x19)], 0x2f);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1a)], 0x2f);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1b)], 0x10);

           /* Rx: Set to Clock-Master mode */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x30)], 0x00);

           /* Rx: Frame-Pulse Master */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x31)], 0x00);

           if(port >= WP_UFE_UPI2_TDM1 && port <= WP_UFE_UPI3_TDM8)
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x17);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x0f);
           }
           else
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x18);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x80);
           }

           /* Disable Transmit Data link */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x38)], 0x00);

           WPL_Delay(100);
           /* Clock Synthesis Config - Tx Clock = 1.544 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xd6)], 0x1);

           /* Line Coding = B8ZS */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x10)], 0x00);

           /* Reset T1-XBAS register settings */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x54)], 0x0);

           /* Configure T1 framer to UNFRAMED */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x54)], 0x20);

           /* RLPS Voltage Threshold #1*/
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdc)], 0x26);

           /* RLPS Voltage Threshold #2 - This is different for E1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdd)], 0xc3);

           /* Initialize T1 Receive Equalizer RAM */
           if((status = WPUI_TdmCometInitEq((wpu_comet_phy *)&comet->phy[phy],
                  sizeof(rlps_ram_t1), (WP_U8 *)rlps_ram_t1)))
                goto clean_return;

           WPL_Delay(100);

           /* Enable Squelch */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf8)], 0x11);

           /* LOS Detection/Clearance threshold - This is different for E1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf9)], 0x88);

           /* LOS Detection period - This is different for E1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfa)], 0x20);

           /* LOS Clearance period - This is different for E1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfb)], 0x20);

           /* Reset RLPS status and control register */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfe)], 0x00);

           /* Enable RLPS Equalizer */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xff)], 0x0b);

           /* Configure Tx waveform */
           if((status = WPUI_TdmCometInitWf((wpu_comet_phy *)&comet->phy[phy],
                    sizeof(wave_form_t1), (WP_U8 *)wave_form_t1)))
                goto clean_return;

           WPL_Delay(100);
           /* Enable TX for Short Haul -> 0 to 110 ft. */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_XLPG_CFG], 0x10);

           WPL_Delay(300);

           /* Enable Receiver */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xbe)], 0x00);

           break;

      case WPU_MODE_TDM_T1_MASTER_PWE3_CES_CR:
      case WPU_MODE_TDM_T1_SLAVE_PWE3_CES_CR:
      case WPU_MODE_TDM_T1_CR_MASTER_PWE3_CES_CR:
      case WPU_MODE_TDM_T1_CR_SLAVE_PWE3_CES_CR:
      case WPU_MODE_TDM_T1_CR_CAS_MASTER_PWE3_CES_CR:
           /* Global Configuration Register - set to T1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x00)], 0x0);

           /* Enable Jitter Attenuation and bypass elastic store */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x02)], 0x20);

           if (mode == WPU_MODE_TDM_T1_SLAVE_PWE3_CES_CR)
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x0a)], 0x00);
           }

           /* (RJAT) - Rx Jitter Attenuation Settings */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x17)], 0x10);

           /* (TJAT) - Tx Jitter Attenuation Settings */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x19)], 0x2f);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1a)], 0x2f);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1b)], 0x10);

           /* Rx: Set to Clock-Master mode */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x30)], 0x00);

           /* Rx: Frame-Pulse Master */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x31)], 0x00);

           /* Bit offset Disable */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x34)], 0x00);

           /* Tx: Set to Clock-Master mode */
           if(port >= WP_UFE_UPI2_TDM1 && port <= WP_UFE_UPI3_TDM8)
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x38);
           else
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x28);

           /* Tx: Frame-Pulse Master */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x41)], 0x00);

           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x17);

           if(port >= WP_UFE_UPI2_TDM1 && port <= WP_UFE_UPI3_TDM8)
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x17);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x0f);
           }
           else
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x18);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x80);
           }

           /* Disable Transmit Data link */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x38)], 0x00);

           WPL_Delay(100);
           /* Clock Synthesis Config - Tx Clock = 1.544 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xd6)], 0x1);

           /* Line Coding = B8ZS */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x10)], 0x00);

           /* Reset T1-XBAS register settings */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x54)], 0x0);

           /* Configure T1 framer to ESF */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x48)], 0x30);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x54)], 0x30);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x60)], 0x10);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x50)], 0x04);

           /* T1 Input frame format - elastic store */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_TXELST_CFG], 0x00);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_RXELST_CFG], 0x00);

           /* RLPS Voltage Threshold #1*/
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdc)], 0x26);

           /* RLPS Voltage Threshold #2 - This is different for E1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdd)], 0xc3);

           /* Initialize T1 Receive Equalizer RAM */
           if((status = WPUI_TdmCometInitEq((wpu_comet_phy *)&comet->phy[phy],
                  sizeof(rlps_ram_t1), (WP_U8 *)rlps_ram_t1)))
                goto clean_return;

           WPL_Delay(100);

           /* Enable Squelch */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf8)], 0x11);

           /* LOS Detection/Clearance threshold - This is different for E1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf9)], 0x88);

           /* LOS Detection period - This is different for E1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfa)], 0x20);

           /* LOS Clearance period - This is different for E1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfb)], 0x20);

           /* Reset RLPS status and control register */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfe)], 0x00);

           /* Enable RLPS Equalizer */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xff)], 0x0b);

           /* Configure Tx waveform */
           if((status = WPUI_TdmCometInitWf((wpu_comet_phy *)&comet->phy[phy],
                    sizeof(wave_form_t1), (WP_U8 *)wave_form_t1)))
                goto clean_return;

           WPL_Delay(100);
           /* Enable TX for Short Haul -> 0 to 110 ft. */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_XLPG_CFG], 0x10);

           WPL_Delay(100);

           /* Enable TxElasticStore - needed for external */
           /* loopback cable or "master" mode.            */
           if (mode == WPU_MODE_TDM_T1_CR_MASTER_PWE3_CES_CR)
           {
              /* clock recovery works with Backplane clock (0x01).
                 Change reg[0x06] if clock isn't convergence. */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x01);
           }
           else if (mode == WPU_MODE_TDM_T1_CR_SLAVE_PWE3_CES_CR)
           {
              /* Bypass TxElasticStore - needed for slave/loop-timed  */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x1c);
           }
           else if (mode == WPU_MODE_TDM_T1_MASTER_PWE3_CES_CR)
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x01);
           /* Bypass TxElasticStore - needed for slave/loop-timed  */
           else if (mode == WPU_MODE_TDM_T1_SLAVE_PWE3_CES_CR)
           {
              /* source of Tx CLK:CTCLK, JAT:NO JAT, BACKPLANE is SLAVE */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x1c);
           }
           else
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x01);
           }

           WPL_Delay(200);

           /* Enable Receiver */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xbe)], 0x00);

           if (mode == WPU_MODE_TDM_T1_CR_CAS_MASTER_PWE3_CES_CR)
           {
              /* Enable Per Channel Control (PCCE) and Indirect register write (IND) for
                 TPSC (Table 1) */
              WPU_EnableIndirectRegRW((wpu_comet_phy*)(&(comet->phy[phy])), WPU_TPSC_CFG);

              /* Set the SIGC[0] to '0', SIGC[1] to '1'
                 in indirect registers 0x60-0x80 for TPSC */
              for (i=0x60; i<0x80; i++)
              {
                 WPU_WriteIndirectReg(
                    (wpu_comet_phy*)(&(comet->phy[phy])), WPU_TPSC_CFG, i, 0x40);
              }

           }

           break;

      /*****************************/
      /* Initialize commet E1 port */
      /*****************************/

      case WPU_MODE_TDM_E1_MASTER_UNFRAMED_PWE3_CES_CR:
      case WPU_MODE_TDM_E1_SLAVE_UNFRAMED_PWE3_CES_CR:
           /* Global Configuration Register - set to E1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x00)], 0x1);

           /* RX: Enable Jitter Attenuation and bypass elastic store */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x02)], 0xc0);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x05)], 0x08);

           if (mode == WPU_MODE_TDM_E1_SLAVE_UNFRAMED_PWE3_CES_CR)
           {
              /* source of Tx CLK:CTCLK, JAT:NO JAT, BACKPLANE is SLAVE */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x0a)], 0x00);
           }

           /* (RJAT) - Rx Jitter Attenuation Settings */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x15)], 0xff);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x16)], 0xff);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x17)], 0x10);

           /* (TJAT) - Tx Jitter Attenuation Settings */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x19)], 0xff);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1a)], 0xff);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1b)], 0x10);

           /* Rx: Set to Clock-Master mode, Rate = 2.048 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x30)], 0x01);

           /* Rx: Frame-Pulse Master */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x31)], 0x00);

           /* Bit offset */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x33)], 0x00);
           if(port >= WP_UFE_UPI2_TDM1 && port <= WP_UFE_UPI3_TDM8)
           { 
              /*this if isn't performed at all */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x34)], 0x00);
              /* Tx: Set to Clock-Master mode, Rate = 2.048 */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x39);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x00);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x00);

           }
           else
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x34)], 0x08);
              /* Tx: Set to Clock-Master mode, Rate = 2.048 */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x29);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x00);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x09);
           }

           /* Tx: Frame-Pulse Master */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x41)], 0x00);

           /* Basic Framing - Disable Signaling/Data link - Generate CRC */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x80)], 0x08);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x90)], 0x40);

           WPL_Delay(100);
           /* Clock Synthesis Config - Tx Clock = 2.048 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xd6)], 0x00);

           /* Line Coding = HDB3 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x10)], 0x00);

           /* E1 Input frame format - elastic store */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_TXELST_CFG], 0x03);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_RXELST_CFG], 0x03);

           /* RLPS Voltage Threshold #1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdc)], 0x26);

           /* RLPS Voltage Threshold #2 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdd)], 0x03);

           /* Initialize E1 Receive Equalizer RAM */
           if((status = WPUI_TdmCometInitEq((wpu_comet_phy *)&comet->phy[phy],
                     sizeof(rlps_ram_e1), (WP_U8 *)rlps_ram_e1)))
                 goto clean_return;

           /* Enable Squelch */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf8)], 0x11);

           /* LOS Detection/Clearance threshold */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf9)], 0xcc);

           /* LOS Detection period */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfa)], 0x2d);

           /* LOS Clearance period */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfb)], 0x2d);

           /* Reset RLPS status and control register */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfe)], 0x00);

           /* Enable RLPS Equalizer */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xff)], 0x0b);

           /* Configure Tx waveform */
           if((status = WPUI_TdmCometInitWf((wpu_comet_phy *)&comet->phy[phy],
                     sizeof(wave_form_e1), (WP_U8 *)wave_form_e1)))
                 goto clean_return;

           /* Enable TX for Short Haul -> 0 to 110 ft. */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_XLPG_CFG], 0x10);

           WPL_Delay(100);

           /* Disable TxElasticStore Bypass - needed */
           /* for external loopback cable.           */
           if (mode == WPU_MODE_TDM_E1_MASTER_UNFRAMED_PWE3_CES_CR)
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x01);

           /* Bypass TxElasticStore - needed for slave/loop-timed  */
           else
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x1c);
           }


           WPL_Delay(200);

           /* Enable Receiver */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xbe)], 0x00);

          break;

      case WPU_MODE_TDM_E1_MASTER_PWE3_CES_CR:
      case WPU_MODE_TDM_E1_SLAVE_PWE3_CES_CR:
      case WPU_MODE_TDM_E1_CR_MASTER_PWE3_CES_CR:
      case WPU_MODE_TDM_E1_CR_SLAVE_PWE3_CES_CR:
      case WPU_MODE_TDM_E1_CR_CAS_MASTER_PWE3_CES_CR:
           /* Global Configuration Register - set to E1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x00)], 0x1);

           /* RX: Enable Jitter Attenuation and bypass elastic store */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x02)], 0x30);

           if (mode == WPU_MODE_TDM_E1_SLAVE_PWE3_CES_CR)
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x0a)], 0x00);
           }

           /* (RJAT) - Rx Jitter Attenuation Settings */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x15)], 0xff);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x16)], 0xff);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x17)], 0x10);

           /* (TJAT) - Tx Jitter Attenuation Settings */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x19)], 0xff);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1a)], 0xff);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1b)], 0x10);

           /* Rx: Set to Clock-Master mode, Rate = 2.048 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x30)], 0x01);

           /* Rx: Frame-Pulse Master */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x31)], 0x00);

           /* Bit offset */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x33)], 0x00);
           if(port >= WP_UFE_UPI2_TDM1 && port <= WP_UFE_UPI3_TDM8)
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x34)], 0x00);
              /* Tx: Set to Clock-Master mode, Rate = 2.048 */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x39);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x00);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x00);

           }
           else
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x34)], 0x08);
              /* Tx: Set to Clock-Master mode, Rate = 2.048 */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x29);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x00);
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x09);
           }

           /* Tx: Frame-Pulse Master */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x41)], 0x00);

           if (mode == WPU_MODE_TDM_E1_CR_CAS_MASTER_PWE3_CES_CR)
           {
              /* CAS Framing - Enable Signaling/Data link - Generate CRC */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x80)], 0x70);
           }
           else
           {
              /* Basic Framing - Disable Signaling/Data link - Generate CRC */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x80)], 0x10);
           }

           if (mode == WPU_MODE_TDM_E1_CR_SLAVE_PWE3_CES_CR ||
               mode == WPU_MODE_TDM_E1_CR_MASTER_PWE3_CES_CR)
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x90)], 0xc0);
           else if (mode == WPU_MODE_TDM_E1_CR_CAS_MASTER_PWE3_CES_CR)
           {
              /* Enable CAS - set CASDIS=0 */
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x90)], 0x80);
           }

           WPL_Delay(100);
           /* Clock Synthesis Config - Tx Clock = 2.048 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xd6)], 0x00);

           /* Line Coding = HDB3 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x10)], 0x00);

           /* E1 Input frame format - elastic store */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_TXELST_CFG], 0x03);
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_RXELST_CFG], 0x03);

           /* RLPS Voltage Threshold #1 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdc)], 0x26);

           /* RLPS Voltage Threshold #2 */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdd)], 0x03);

           /* Initialize E1 Receive Equalizer RAM */
           if((status = WPUI_TdmCometInitEq((wpu_comet_phy *)&comet->phy[phy],
                     sizeof(rlps_ram_e1), (WP_U8 *)rlps_ram_e1)))
                 goto clean_return;

           /* Enable Squelch */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf8)], 0x11);

           /* LOS Detection/Clearance threshold */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf9)], 0xcc);

           /* LOS Detection period */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfa)], 0x2d);

           /* LOS Clearance period */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfb)], 0x2d);

           /* Reset RLPS status and control register */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfe)], 0x00);

           /* Enable RLPS Equalizer */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xff)], 0x0b);

           /* Configure Tx waveform */
           if((status = WPUI_TdmCometInitWf((wpu_comet_phy *)&comet->phy[phy],
                     sizeof(wave_form_e1), (WP_U8 *)wave_form_e1)))
                 goto clean_return;

           /* Enable TX for Short Haul -> 0 to 110 ft. */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_XLPG_CFG], 0x10);

           WPL_Delay(100);

           /* Disable TxElasticStore Bypass - needed */
           /* for external loopback cable.           */
           if (mode == WPU_MODE_TDM_E1_MASTER_PWE3_CES_CR)
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x01);

           /* Bypass TxElasticStore - needed for slave/loop-timed  */
           else if (mode == WPU_MODE_TDM_E1_SLAVE_PWE3_CES_CR)
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x1c);
           }
           else if (mode == WPU_MODE_TDM_E1_CR_MASTER_PWE3_CES_CR)
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x01);

           /* Bypass TxElasticStore - needed for slave/loop-timed  */
           else if (mode == WPU_MODE_TDM_E1_CR_SLAVE_PWE3_CES_CR)
           {
              WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x1c);
           }

           WPL_Delay(200);

           /* Enable Receiver */
           WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xbe)], 0x00);

          break;

      /* T1 CAS */
      case WPU_MODE_TDM_TRANS_T1_CAS_MASTER_PWE3_CES_CR:
      case WPU_MODE_TDM_TRANS_T1_CAS_SLAVE_PWE3_CES_CR:

         /* Global Configuration Register - set to T1 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x00)], 0x0);

         /* Enable Jitter Attenuation and bypass elastic store */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x02)], 0x20);

         if (mode == WPU_MODE_TDM_TRANS_T1_CAS_SLAVE_PWE3_CES_CR)
         {
            /* source of Tx CLK:CTCLK, JAT:NO JAT, BACKPLANE is SLAVE */
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x0a)], 0x00);
         }

         /* (RJAT) - Rx Jitter Attenuation Settings */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x17)], 0x10);

         /* (TJAT) - Tx Jitter Attenuation Settings (1.544Mhz from backplane,
            recovered or CTCLK */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x19)], 0x2f);
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1a)], 0x2f);
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1b)], 0x10);

         /* Rx: Set to Clock-Master mode */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x30)], 0x00);

         /* Rx: Frame-Pulse Master */
         if(port >= WP_UFE_UPI2_TDM1 && port <= WP_UFE_UPI3_TDM8)  /* CAD */
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x31)], 0x04);
            /* FPMODE = 0 (BRFP is output)                                        */
            /* BZSMFP = 1 (BRFP output pulse in the first frame of the multiframe */
         else                                                      /* TDI */
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x31)], 0x00);

         /* Bit offset Disable */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x34)], 0x00);

         /* Tx: Set to backplane Clock-slave mode (Comet is Master) */
         if(port >= WP_UFE_UPI2_TDM1 && port <= WP_UFE_UPI3_TDM8)
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x38);
         else
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x28);

         /* Tx: Frame-Pulse Master */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x41)], 0x00);

         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x17);

         if(port >= WP_UFE_UPI2_TDM1 && port <= WP_UFE_UPI3_TDM8)
         {
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x17);
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x0f);
         }
         else
         {
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x18);
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x80);
         }

         /* Disable Transmit Data link */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x38)], 0x00);

         WPL_Delay(100);
         /* Clock Synthesis Config - Tx Clock = 1.544 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xd6)], 0x1);

         /* Line Coding = B8ZS */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x10)], 0x00);

         /* Reset T1-XBAS register settings */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x54)], 0x0);

         /* Configure T1 framer to ESF */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x48)], 0x30);
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x54)], 0x30); /* ESF=1 B8ZS=1 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x60)], 0x10); /* ESF=1        */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x50)], 0x04); /* ESF=1        */

         /* T1 Input frame format - elastic store */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_TXELST_CFG], 0x00);
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_RXELST_CFG], 0x00);

         /* RLPS Voltage Threshold #1*/
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdc)], 0x26);

         /* RLPS Voltage Threshold #2 - This is different for E1 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdd)], 0xc3);

         /* Initialize T1 Receive Equalizer RAM */
         if((status = WPUI_TdmCometInitEq((wpu_comet_phy *)&comet->phy[phy],
                                          sizeof(rlps_ram_t1), (WP_U8 *)rlps_ram_t1)))
            goto clean_return;

         WPL_Delay(100);

         /* Enable Squelch */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf8)], 0x11);

         /* LOS Detection/Clearance threshold - This is different for E1 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf9)], 0x88);

         /* LOS Detection period - This is different for E1 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfa)], 0x20);

         /* LOS Clearance period - This is different for E1 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfb)], 0x20);

         /* Reset RLPS status and control register */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfe)], 0x00);

         /* Enable RLPS Equalizer */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xff)], 0x0b);

         /* Configure Tx waveform */
         if((status = WPUI_TdmCometInitWf((wpu_comet_phy *)&comet->phy[phy],
                                          sizeof(wave_form_t1), (WP_U8 *)wave_form_t1)))
            goto clean_return;

         WPL_Delay(100);
         /* Enable TX for Short Haul -> 0 to 110 ft. */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_XLPG_CFG], 0x10);

         WPL_Delay(100);

         /* Enable TxElasticStore - needed for external */
         /* loopback cable or "master" mode.            */
         if (mode == WPU_MODE_TDM_TRANS_T1_CAS_MASTER_PWE3_CES_CR)
         {
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x01);
         }

         /* Bypass TxElasticStore - needed for slave/loop-timed  */
         else if (mode == WPU_MODE_TDM_TRANS_T1_CAS_SLAVE_PWE3_CES_CR)
         {
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x1c);
         }

         WPL_Delay(200);

         /* Enable Receiver */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xbe)], 0x00);


         /*    TPSC   */
         /* Enable Per Channel Control (PCCE) and Indirect register write (IND) for
            TPSC (Table 1) */
         WPU_EnableIndirectRegRW((wpu_comet_phy*)(&(comet->phy[phy])), WPU_TPSC_CFG);

         /* Set the INVERT=0, SIGNINV=0 in indirect registers 0x20-0x40 for TPSC */
          for (i=0x20; i<0x40; i++)
          {
             WPU_WriteIndirectReg((wpu_comet_phy*)(&(comet->phy[phy])), WPU_TPSC_CFG, i, 0x00);
          }

         /* Set the SIGC[0] to '0', SIGC[1] to '1' in indirect registers 0x60-0x80 for TPSC */
         for (i=0x60; i<0x80; i++)
         {
            WPU_WriteIndirectReg((wpu_comet_phy*)(&(comet->phy[phy])), WPU_TPSC_CFG, i, 0x40);
         }
          /* RPSC */
          WPU_EnableIndirectRegRW((wpu_comet_phy*)(&(comet->phy[phy])), WPU_RPSC_CFG);
          for (i=0x20; i<0x40; i++)
          {
             WPU_WriteIndirectReg((wpu_comet_phy*)(&(comet->phy[phy])), WPU_RPSC_CFG, i, 0x00);
             /* SIGNINV = 0 */
          }
          /*  SIGX */
          WPU_EnableIndirectRegRW((wpu_comet_phy*)(&(comet->phy[phy])), WPU_SIGX_CFG);
          for (i=0x40; i<0x60; i++)
          {
             WPU_WriteIndirectReg((wpu_comet_phy*)(&(comet->phy[phy])), WPU_SIGX_CFG, i, 0x00);
             /* RINV[1] = 0
                RFIX    = 0
                RPOL    = 0  (fixes to 0) */
          }

         break; /* T1 CAS */

         /* E1 CAS */
      case WPU_MODE_TDM_TRANS_E1_CAS_MASTER_PWE3_CES_CR:
      case WPU_MODE_TDM_TRANS_E1_CAS_SLAVE_PWE3_CES_CR:

         /* Global Configuration Register - set to E1 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x00)], 0x1);

         /* RX: Enable Jitter Attenuation and bypass elastic store */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x02)], 0x30);

         if (mode == WPU_MODE_TDM_TRANS_E1_CAS_SLAVE_PWE3_CES_CR)
         {
            /* source of Tx CLK:CTCLK, JAT:NO JAT, BACKPLANE is SLAVE */
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x0a)], 0x00);
         }

         /* (RJAT) - Rx Jitter Attenuation Settings */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x15)], 0xff);
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x16)], 0xff);
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x17)], 0x10);

         /* (TJAT) - Tx Jitter Attenuation Settings */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x19)], 0xff);
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1a)], 0xff);
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x1b)], 0x10);

         /* Rx: Set to Clock-Master mode, Rate = 2.048 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x30)], 0x01);

         /* Rx: Frame-Pulse Master */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x31)], 0x00);

         /* Bit offset */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x33)], 0x00);
         if(port >= WP_UFE_UPI2_TDM1 && port <= WP_UFE_UPI3_TDM8)
         {
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x34)], 0x00);
            /* Tx: Set to backplane Clock-slave mode (Comet is Master), Rate = 2.048 */
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x39);
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x00);
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x00);
         }
         else
         {
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x34)], 0x08);
            /* Tx: Set to backplane Clock-slave mode (Comet is Master), Rate = 2.048 */
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x40)], 0x29);
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x43)], 0x00);
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x44)], 0x09);
         }

         /* Tx: Frame-Pulse Master */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x41)], 0x00);

         /* CAS enable */
         /* For Transmit(table 5)   SIGEN=DLEN='1'  GENCRC='1'  */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x80)], 0x70);
         /* CRCEN='1'  CASDIS='0' to enable multiframe          */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x90)], 0x80);

         WPL_Delay(100);
         /* Clock Synthesis Config - Tx Clock = 2.048 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xd6)], 0x00);

         /* Line Coding = HDB3 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x10)], 0x00);

         /* E1 Input frame format - elastic store */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_TXELST_CFG], 0x03);
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_RXELST_CFG], 0x03);

         /* RLPS Voltage Threshold #1 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdc)], 0x26);

         /* RLPS Voltage Threshold #2 */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xdd)], 0x03);

         /* Initialize E1 Receive Equalizer RAM */
         if((status = WPUI_TdmCometInitEq((wpu_comet_phy *)&comet->phy[phy],
                                          sizeof(rlps_ram_e1), (WP_U8 *)rlps_ram_e1)))
            goto clean_return;

         /* Enable Squelch */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf8)], 0x11);

         /* LOS Detection/Clearance threshold */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xf9)], 0xcc);

         /* LOS Detection period */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfa)], 0x2d);

         /* LOS Clearance period */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfb)], 0x2d);

         /* Reset RLPS status and control register */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xfe)], 0x00);

         /* Enable RLPS Equalizer */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xff)], 0x0b);

         /* Configure Tx waveform */
         if((status = WPUI_TdmCometInitWf((wpu_comet_phy *)&comet->phy[phy],
                                          sizeof(wave_form_e1), (WP_U8 *)wave_form_e1)))
            goto clean_return;

         /* Enable TX for Short Haul -> 0 to 110 ft. */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[WPU_XLPG_CFG], 0x10);

         WPL_Delay(100);

         /* Disable TxElasticStore Bypass - needed */
         /* for external loopback cable.           */
         if (mode == WPU_MODE_TDM_TRANS_E1_CAS_MASTER_PWE3_CES_CR)
         {
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x01);
         }

         /* Bypass TxElasticStore - needed for slave/loop-timed  */
         else if (mode == WPU_MODE_TDM_TRANS_E1_CAS_SLAVE_PWE3_CES_CR)
         {
            WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0x06)], 0x1c);
         }

         WPL_Delay(200);

         /* Enable Receiver */
         WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(0xbe)], 0x00);

         break;  /* E1 CAS */

      default:
         status = WP_ERR_CONFIG;
   }

  clean_return:

   /* Delay before restoring match and mask registers */
   WPL_Delay(200);

   /* Restore the registers. */
   WPX_RestoreBank(&save);

   /* Relinquish the lock (on the match/mask registers). */
   WPI_WDDI_DETACH();

   return status;
}

/*******************************/
/* Initialize Rx Equalizer RAM */
/*******************************/

WP_status WPUI_TdmCometInitEq(wpu_comet_phy *phy,
                             WP_U32 table_size, WP_U8 *table)
{
   WP_U32 i;
   WP_U8 *ptr_compare;
   WP_U8 value;

   ptr_compare = table;

   /* Writes 4 bytes at a time */
   for(i = 0; i <table_size/4 ; i++) {

       WPI_MEM_BYTE_SET(phy->reg[REG_INDEX(0xd8)], *table++);
       WPI_MEM_BYTE_SET(phy->reg[REG_INDEX(0xd9)], *table++);
       WPI_MEM_BYTE_SET(phy->reg[REG_INDEX(0xda)], *table++);
       WPI_MEM_BYTE_SET(phy->reg[REG_INDEX(0xdb)], *table++);
       WPI_MEM_BYTE_SET(phy->reg[REG_INDEX(0xfd)], 0x00);
       WPI_MEM_BYTE_SET(phy->reg[REG_INDEX(0xfc)], i);

       WPL_Delay(30);

       /* Verify that the value written can be read */
       WPI_MEM_BYTE_SET(phy->reg[REG_INDEX(0xfd)], 0x80);
       WPI_MEM_BYTE_SET(phy->reg[REG_INDEX(0xfc)], i);
       WPI_MEM_BYTE_GET(value, phy->reg[REG_INDEX(0xd8)]);
       if(value != *ptr_compare++)
         return WPI_ERROR(WP_ERR_INIT);
       WPI_MEM_BYTE_GET(value, phy->reg[REG_INDEX(0xd9)]);
       if(value != *ptr_compare++)
         return WPI_ERROR(WP_ERR_INIT);
       WPI_MEM_BYTE_GET(value, phy->reg[REG_INDEX(0xda)]);
       if(value != *ptr_compare++)
         return WPI_ERROR(WP_ERR_INIT);
       WPI_MEM_BYTE_GET(value, phy->reg[REG_INDEX(0xdb)]);
       if(value != *ptr_compare++)
         return WPI_ERROR(WP_ERR_INIT);
   }
   return WP_OK;
}

/********************************************/
/* Initialize Transmit Pulse Waveform Table */
/********************************************/

WP_status WPUI_TdmCometInitWf(wpu_comet_phy *phy,
                             WP_U32 table_size, WP_U8 *table)
{

   WP_U32 i,j;
   WP_U8 sample;

   /* Wave form table has 5 entries per row */
   for(i = 0; i < table_size/5; i++) {

      /* Program sample and interval */
      sample = i << 3;
      for(j = 0; j < 5; j++) {

          WPL_Delay(10);

          /* Sample & Unit Interval selection */
          WPI_MEM_BYTE_SET(phy->reg[REG_INDEX(0xf2)],
                        (sample | j));

          /* Write data */
          WPI_MEM_BYTE_SET(phy->reg[REG_INDEX(0xf3)],
                          *table++);
       }
   }
   return WP_OK;
}

/**************************/
/* Modify a CPLD Register */
/**************************/

WP_status WPU_TdmCpldRegMod(WP_U32 port, WP_U32 reg, WP_U8 value)
{
   wpu_comet_cpld *cpld;

   wpi_match_mask save;
   WP_status status = 0;

   /* Restrict access to match/mask regs. */
   WPI_WDDI_ATTACH();

   /* Get address to CPLD/Comet for TDM 1 - 8 */
   if(port >= WP_PORT_TDM1 && port <= WP_PORT_TDM8)
   {
   /* Save current match/mask, install TDM values. */
      WPX_SetBank(&save, &wpi_board_data.match_mask->tdm[0]);
      cpld = (wpu_comet_cpld *) WPL_PHY_VIRTUAL(0, APP_CPLD_TDM1_COMET);
   }
   
   /* Get address to CPLD/Comet for TDM 9 - 16 */
   else if(port >= WP_PORT_TDM9 && port <= WP_PORT_TDM16)
   {
   /* Save current match/mask, install TDM values. */
      WPX_SetBank(&save, &wpi_board_data.match_mask->tdm[1]);
      cpld = (wpu_comet_cpld *) WPL_PHY_VIRTUAL(0, APP_CPLD_TDM9_COMET);
   }
   
   /* Else, invalid PORT value */
   else {
      status = WPI_ERROR(WP_ERR_PORT);
      /* Relinquish the lock (on the match/mask registers). */
      WPI_WDDI_DETACH();

      return status;
   }

   /* Delay before writing to CPLD */
   WPL_Delay(200);

   WPI_MEM_BYTE_SET(cpld->reg[REG_INDEX(reg)], value);

   /* Restore the registers. */
   WPX_RestoreBank(&save);

   /* Relinquish the lock (on the match/mask registers). */
   WPI_WDDI_DETACH();

   return status;
}

/***************************/
/* Read Comet Register     */
/***************************/

WP_status WPU_TdmCpldRegRead(WP_U32 port, WP_U32 reg, WP_U8 *reg_value)
{
   wpu_comet_cpld *cpld;
   volatile WP_U8 *addr;
   WP_U8 value;

   wpi_match_mask save;
   WP_status status = 0;

   /* Restrict access to match/mask regs. */
   WPI_WDDI_ATTACH();

   /* Get address to CPLD/Comet for TDM 1 - 8 */
   if(port >= WP_PORT_TDM1 && port <= WP_PORT_TDM8)
   {
      /* Save current match/mask, install TDM values. */
      WPX_SetBank(&save, &wpi_board_data.match_mask->tdm[0]);
      cpld = (wpu_comet_cpld *) WPL_PHY_VIRTUAL(0, APP_CPLD_TDM1_COMET);
   }
   
   /* Get address to CPLD/Comet for TDM 9 - 16 */
   else if(port >= WP_PORT_TDM9 && port <= WP_PORT_TDM16)
   {
      /* Save current match/mask, install TDM values. */
      WPX_SetBank(&save, &wpi_board_data.match_mask->tdm[1]);
      cpld = (wpu_comet_cpld *) WPL_PHY_VIRTUAL(0, APP_CPLD_TDM9_COMET);
   }
   
   /* Else, invalid PORT value */
   else {
      status = WPI_ERROR(WP_ERR_PORT);
      /* Relinquish the lock (on the match/mask registers). */
      WPI_WDDI_DETACH();

      return status;
   }

   addr = (volatile WP_U8 *)&(cpld->reg[REG_INDEX(reg)]);
   WPL_Delay(200);
   WPI_MEM_BYTE_GET(value, *addr);

   *reg_value = value;

   /* Restore the registers. */
   WPX_RestoreBank(&save);

   /* Relinquish the lock (on the match/mask registers). */
   WPI_WDDI_DETACH();

   return status;
}

/***************************/
/* Modify a Comet Register */
/***************************/

WP_status WPU_TdmCometRegMod(WP_U32 port, WP_U32 reg, WP_U8 value)
{
   wpu_comet *comet;

   wpi_match_mask save;
   WP_status status = 0;

   WP_U32 phy;

   /* Restrict access to match/mask regs. */
   WPI_WDDI_ATTACH();

   /* Get address to CPLD/Comet for TDM 1 - 8 */
   if(port >= WP_PORT_TDM1 && port <= WP_PORT_TDM8) {
      /* Save current match/mask, install TDM values. */
      WPX_SetBank(&save, &wpi_board_data.match_mask->tdm[0]);
      comet = (wpu_comet *) WPL_PHY_VIRTUAL(0, APP_TDM1_COMET);
      phy = port - WP_PORT_TDM1;
   }

   /* Get address to CPLD/Comet for TDM 9 - 16 */
   else if(port >= WP_PORT_TDM9 && port <= WP_PORT_TDM16) {
      /* Save current match/mask, install TDM values. */
      WPX_SetBank(&save, &wpi_board_data.match_mask->tdm[1]);
      comet = (wpu_comet *) WPL_PHY_VIRTUAL(0, APP_TDM9_COMET);
      phy = port - WP_PORT_TDM9;
   }

   /* Else, invalid PORT value */
   else {
      status = WPI_ERROR(WP_ERR_PORT);
      /* Relinquish the lock (on the match/mask registers). */
      WPI_WDDI_DETACH();

      return status;
   }

   WPI_MEM_BYTE_SET(comet->phy[phy].reg[REG_INDEX(reg)], value);

   /* Restore the registers. */
   WPX_RestoreBank(&save);

   /* Relinquish the lock (on the match/mask registers). */
   WPI_WDDI_DETACH();

   return status;

}

/***************************/
/* Read Comet Register     */
/***************************/

WP_status WPU_TdmCometRegRead(WP_U32 port, WP_U32 reg, WP_U8 *reg_value)
{
   wpu_comet *comet;
   volatile WP_U8 *addr;
   WP_U8 value;
   WP_U32 phy;

   wpi_match_mask save;
   WP_status status = 0;

   /* Restrict access to match/mask regs. */
   WPI_WDDI_ATTACH();

   /* Get address to CPLD/Comet for TDM 1 - 8 */
   if(port >= WP_PORT_TDM1 && port <= WP_PORT_TDM8) {
      /* Save current match/mask, install TDM values. */
      WPX_SetBank(&save, &wpi_board_data.match_mask->tdm[0]);
      comet = (wpu_comet *) WPL_PHY_VIRTUAL(0, APP_TDM1_COMET);
      phy = port - WP_PORT_TDM1;
   }

   /* Get address to CPLD/Comet for TDM 9 - 16 */
   else if(port >= WP_PORT_TDM9 && port <= WP_PORT_TDM16) {
      /* Save current match/mask, install TDM values. */
      WPX_SetBank(&save, &wpi_board_data.match_mask->tdm[1]);
      comet = (wpu_comet *) WPL_PHY_VIRTUAL(0, APP_TDM9_COMET);
      phy = port - WP_PORT_TDM9;
   }

   /* Else, invalid PORT value */
   else {
      status = WPI_ERROR(WP_ERR_PORT);

      /* Relinquish the lock (on the match/mask registers). */
      WPI_WDDI_DETACH();

      return status;
   }

   addr = (volatile WP_U8 *)&(comet->phy[phy].reg[REG_INDEX(reg)]);
   WPL_Delay(200);
   WPI_MEM_BYTE_GET(value, *addr);

   *reg_value = value;

   /* Restore the registers. */
   WPX_RestoreBank(&save);

   /* Relinquish the lock (on the match/mask registers). */
   WPI_WDDI_DETACH();

   return status;
}
