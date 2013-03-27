/*--------------------------------------------------------------------------*/
/*                                                                          */
/*        Copyright (c) 2010  Omiino Ltd                                    */
/*                                                                          */
/*        All rights reserved.                                              */
/*        This code is provided under license and or Non-disclosure         */
/*        Agreement and must be used solely for the purpose for which it    */
/*        was provided. It must not be passed to any third party without    */
/*        the written permission of Omiino Ltd.                             */
/*                                                                          */
/*                       Firmware Load Header File                          */
/*                                                                          */
/*                  Automatically generated - DO NOT EDIT                   */
/*                                                                          */
/*                Generation Timestamp - 12/12/2012 6:31:20 PM                */
/*                                                                          */
/*                      File Generation Information                         */
/*                      ---------------------------                         */
/*                                                                          */
/* Generated From Elf File:                                                 */
/* Firmware_EDP.elf                                                         */
/* In Directory:                                                            */
/* /users/yogevh/projects/winpath/WDDS_4_2_1_DEV_1537_UFE412_FOR_ERRICSON/ob*/
/*jects/linux/mips_winmon_winpath3/wds/ufe412/obj                           */
/* Last Time Elf File Was Written:                                          */
/* 12/12/2012 6:31:37 PM                                                    */
/*                                                                          */
/* Intermediate File Update Time:                                           */
/* 12/12/2012 6:31:19 PM                                                    */
/*                                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/


#ifndef __WO_FRMR_FIRMWARE_IMAGE_H__
#define __WO_FRMR_FIRMWARE_IMAGE_H__


#define FIRMWARE_LOAD_UFE4_FIRMWARE_IMAGE_DATA_TYPE_NUMBER_OF_BLOCKS (5936)
#define FIRMWARE_LOAD_UFE4_FIRMWARE_IMAGE_DATA_TYPE_BLOCK_SIZE       (24)
#define FirmwareDateStringLength (22)

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned int UFE4_FIRMWARE_IMAGE_DATA_TYPE[FIRMWARE_LOAD_UFE4_FIRMWARE_IMAGE_DATA_TYPE_NUMBER_OF_BLOCKS][FIRMWARE_LOAD_UFE4_FIRMWARE_IMAGE_DATA_TYPE_BLOCK_SIZE] ;
extern char UFE4FirmwareTimeStamp[FirmwareDateStringLength] ;

#ifdef __cplusplus
}
#endif

#endif /* __WO_FRMR_FIRMWARE_IMAGE_H__ */
