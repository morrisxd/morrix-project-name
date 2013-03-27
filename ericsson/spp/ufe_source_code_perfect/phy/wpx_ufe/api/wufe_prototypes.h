/***************************************************************************
 * (C) Copyright 2004-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 **************************************************************************/

/***************************************************************************
 *
 * File: wufe_prototypes.h
 *
 * Purpose: Prototype definitions
 *
 *
 **************************************************************************/

#ifndef WUFE_PROTOTYPES_H
#define WUFE_PROTOTYPES_H

/*************************************************
 ****************  MEMORY  **********************
 ************************************************/

extern void WUFE_IndirectMemset(WP_U32 ufe_id,
                                void   *indirect_add,
                                WP_U8  pattern ,
                                WP_U32 set_size);

extern void WUFE_IndirectMemOrMask (WP_U32 wufe_id,
                                    WP_U32 indreict_addr,
                                    WP_U16 mask);

extern void WUFE_IndirectMemAndMask(WP_U32 wufe_id,
                                    WP_U32 indreict_addr,
                                    WP_U16 mask);

extern void WUFE_DirectMemOrMask (WP_U32 wufe_id,
                                  WP_U16 *address,
                                  WP_U16 mask);

extern void WUFE_DirectMemAndMask(WP_U32 wufe_id,
                                  WP_U16 *address,
                                  WP_U16 mask);

extern void WUFE_MEM_16_SET(WP_U32 id,
                            WP_U16 *offset,
                            WP_U16 value);

extern void WUFE_MEM_16_GET(WP_U32 id,
                            WP_U16 *offset,
                            WP_U16 *value_ptr);

extern void WUFE_MemShortRead(WP_U32 base, WP_U32 register_num, WP_U16 *value);

extern void WUFE_MemShortWrite(WP_U32 base, WP_U32 register_num, WP_U16 value);

extern void WUFE_MemByteRead(WP_U32 base, WP_U32 register_num, WP_U8 *value);

extern void WUFE_MemByteWrite(WP_U32 base, WP_U32 register_num, WP_U8 value);

/*************************************************
 ****************  SYSTEM  **********************
 ************************************************/
extern WUFE_status WUFE_SystemConfig(WP_U32       wufe_id,
                                     WUFE_system *sys_cfg);

extern WUFE_status WUFE_SystemDisable(WP_U32 wufe_id,
                                      WP_U32 property,
                                      WP_U32 direction);

extern WUFE_status WUFE_SystemEnable (WP_U32 wufe_id,
                                      WP_U32 property,
                                      WP_U32 direction);

extern WUFE_status WUFE_SystemDisplay(WP_U32 wufe_id);

extern WUFE_status WUFE_SystemReconfigure(WP_U32 wufe_id,
                                          WP_U32 command,
                                          void *config);

extern WUFE_status WUFE_SystemDelete(WP_U32 wufe_id);

extern WUFE_status WUFE_SystemInterruptEnable(WP_U32 ufeid);

extern WUFE_status WUFE_SystemInterruptDisable(WP_U32 ufeid);

extern WUFE_status WUFE_SystemCheckFbp(WP_U32 wufe_id, WP_U32 direction);

/*************************************************
 ****************    LINE   **********************
 ************************************************/
extern WUFE_status WUFE_LineCreate(WUFE_handle   *line_handle,
                                   WP_U32         wufe_id,
                                   WUFE_struct_id type,
                                   void *         line_in_cofig);


extern WUFE_status WUFE_LineEnable (WUFE_handle line_handle,
                                    WP_U32 direction);

extern WUFE_status WUFE_LineDisable(WUFE_handle line_handle,
                                    WP_U32 direction);

extern WUFE_status WUFE_LineDelete (WUFE_handle line_handle);

extern WUFE_status WUFE_LineDisplay(WUFE_handle line_handle);

extern WUFE_status WUFE_LineReconfigure(WUFE_handle line_handle,
                                        WUFE_struct_id type,
                                        void *cofig);

extern WUFE_status WUFE_LineLoopbackConfigure(WUFE_handle ufe_line_handle,
                                              WUFE_line_loopback_type type);
extern WUFE_status WUFE_LinePdhLoopbackConfigure(WUFE_handle ufe_line_handle,
                                       	  	  	 WUFE_line_pdh_loopback_type type);

/*************************************************
 ****************     PHY   **********************
 ************************************************/
extern WUFE_status WUFE_PhyCreate(WUFE_handle *ufe_phy_handle,
                                  WUFE_handle  ufe_line_handle,
                                  WUFE_phy    *ufe_phy_cfg,
                                  WP_U32       direction);

extern WUFE_status WUFE_PhyEnable(WUFE_handle ufe_phy_handle,
                                  WP_U32      direction);

extern WUFE_status WUFE_PhyDisable(WUFE_handle ufe_phy_handle,
                                   WP_U32      direction);

extern WUFE_status WUFE_PhyReconfigure (WUFE_handle ufe_phy_handle,
                                        WUFE_phy   *usr_phy_cfg);

/* WUFE_PhyModify is saved for backward compatibility and should be depricated */
extern WUFE_status WUFE_PhyModify (WUFE_handle ufe_phy_handle,
                                   WUFE_phy   *usr_phy_cfg,
                                   WP_U32      command);

extern WUFE_status WUFE_PhyDisplay(WUFE_handle ufe_phy_handle);

extern WUFE_status WUFE_PhyDelete (WUFE_handle ufe_phy_handle);

extern WUFE_status  WUFE_PrintRspt(WP_U32 wufe_id,
                                   WP_U32 core_id,
                                   WP_U16 phy_id);
extern WUFE_status  WUFE_PrintTspt(WP_U32 wufe_id,
                                   WP_U32 core_id,
                                   WP_U16 phy_id);

extern WUFE_status WUFE_PhyGetLineId(WUFE_handle ufe_phy_handle,
                                     WP_U32 *line_id);

extern WUFE_status WUFE_PhyStateGet( WUFE_handle ufe_phy_handle,
                                     WUFE_phy_state* sync_state);

extern WUFE_status WUFE_PhyTdmSlotsControl(WUFE_handle ufe_phy_handle,
                                           WP_U32 direction,
                                           WP_U8 enable);

/*************************************************
 ****************     DRIVER   *******************
 ************************************************/
extern WUFE_status  WUFE_UfeInit(WUFE_init_config* config);
extern WUFE_status WUFE_UfeInitializeFirmware(WUFE_init_config* config);

extern WUFE_status  WUFE_Ufe2Init(WP_U32  wfeid,
                                  WP_U32  wpid,
                                  WP_U32 upiid,
                                  WP_U32  attibutes);

extern WUFE_status  WUFE_UfeRelease(WP_U32 ufeid);

extern WUFE_status WUFE_GpioOutputClockSet(WP_U32 wufe_id, WP_U32 gpio_number, WP_U8 gpio_clock, WP_U8 gpio_ref_clk_out);
extern WUFE_status WUFE_GpioOutputClockRead(WP_U32 wufe_id, WP_U16 *gpio_register);
extern WUFE_status WUFE_DeviceTypeSet(WP_U32 ufeid,
                                      WUFE_device_type device_type);

#define  WUFE_Ufe2Release(ufeid) WUFE_UfeRelease(ufeid)

/*************************************************
 ****************     EVENTS   *******************
 ************************************************/

/* UFE events */
extern WUFE_status  WUFE_UfeReadEvents(WP_U32 ufeid, WUFE_events *events);

extern  WUFE_status WUFE_UfeEventEnable(WP_U32 ufeid, WUFE_events *events);

extern  WUFE_status WUFE_UfeEventDisable(WP_U32 ufeid, WUFE_events *events);

extern WUFE_status WUFE_UfeCoreHwInterruptEnable(WP_U32 ufeid);
extern WUFE_status WUFE_UfeFramerHwInterruptEnable(WP_U32 ufeid);
extern WUFE_status WUFE_UfeExtPllHwInterruptEnable(WP_U32 ufeid);
extern WUFE_status WUFE_UfeLockLostHwInterruptEnable(WP_U32 ufeid);
extern WUFE_status WUFE_UfeSfpHwInterruptEnable(WP_U32 ufeid);
extern WUFE_status WUFE_UfeCoreHwInterruptDisable(WP_U32 ufeid);
extern WUFE_status WUFE_UfeFramerHwInterruptDisable(WP_U32 ufeid);
extern WUFE_status WUFE_UfeExtPllHwInterruptDisable(WP_U32 ufeid);
extern WUFE_status WUFE_UfeLockLostHwInterruptDisable(WP_U32 ufeid);
extern WUFE_status WUFE_UfeSfpHwInterruptDisable(WP_U32 ufeid);
extern WP_boolean WUFE_UfeFramerHwInterruptGet(WP_U32 ufeid);
extern WP_boolean WUFE_UfeExtPllHwInterruptGet(WP_U32 ufeid);
extern WP_boolean WUFE_UfeCoreHwInterruptGet(WP_U32 ufeid);
extern WP_boolean WUFE_UfeLockLostHwInterruptGet(WP_U32 ufeid);
extern WP_boolean WUFE_UfeSfpHwInterruptGet(WP_U32 ufeid);

/*************************************************
 ****************     UFE SFP READ\WRITE    ******
 ************************************************/
extern WUFE_status WUFE_SfpRead (WP_U32 ufeid, WP_U8 sfp_id, WP_U8 n_bytes, WP_U8 single_sfp_read, WP_U8 device_addr, WP_U8 rom_addr, WP_U8 *rom_value);
extern WUFE_status WUFE_SfpWrite (WP_U32 ufeid, WP_U8 sfp_id, WP_U8 n_bytes, WP_U8 device_addr, WP_U8 rom_addr, WP_U8 *rom_value);

/*************************************************
 ****************     SbiClockTranslator  ********
 ************************************************/

/* SBI clock translator prototypes */
extern WUFE_status WUFE_SbiClockTranslatorDisplay(WP_U32 wufe_id);

/*************************************************
 ****************     SbiClockGen      ***********
 ************************************************/

/* SBI clock generator prototypes */
extern WUFE_status WUFE_ClockRecRxCgenModify(WUFE_handle new_ufe_line_handle);

/*************************************************
 ****************    CLOCK RECOVERY    ***********
 ************************************************/
extern WUFE_status WUFE_ClockRecMasterSet(WUFE_handle ufe_phy_handle,
                                          WP_U8 tx_data_unit_size);

extern WUFE_status WUFE_ClockRecDisplay(WUFE_handle ufe_phy_handle);

extern void WUFE_ClockRecRxClockGenRamDisplay(WP_U32 wufe_id);

extern WUFE_status WUFE_ClockRecInfo(WP_U32 wufe_id,
                                     WUFE_handle ufe_phy_handle,
                                     WUFE_clock_rec_info_type info_type,
                                     void* clk_rec_info);

/* Function for internal testing and debug */
/* This function sets or clears the TS stub bit in the CR mode register.
   It is used mainly for testing and debug and required a dedicated synthesis. This function
   has no real purpose in a working system and should not be called. */
extern WUFE_status WUFE_ClockRecTimestampStubSet(WP_U32 wufe_id, WP_U32 enable);
/* End of function for internal testing and debug */

extern void WUFE_ClockRecRxClockGenRamDisplay(WP_U32 wufe_id);

extern WP_U16 WUFE_ClockRecCgenRamEntryRead(WP_U32 wufe_id, WP_U16 entry);

extern WP_U16 WUFE_ClockRecRxPortRamEntryRead(WP_U32 wufe_id, WP_U16 clk_rec_if, WP_U16 entry);

extern WP_U16 WUFE_ClockRecCtRamEntryRead(WP_U32 wufe_id, WP_U16 entry);

extern WUFE_status WUFE_SbiClockTranslatorDisplay(WP_U32 wufe_id);

extern WUFE_status WUFE_GetCurrentStatus(WP_U32 wufe_id, WP_U32 *current);


/*************************************************
 ****************  PLL  **********************
 ************************************************/
extern WUFE_status WUFE_BsUfeExternalPllClockOutConfig(WUFEI_pll_cfg *pllConfigParam);
extern WP_U32 WUFE_BSReadExtPllReg(WP_U32 address);
extern void WUFE_BSSetExtPllFreeRunMode(WP_U32 ufe_id);


#endif /* WP_PROTOTYPES_H */
