#
# (C) Copyright 2001-2010, Wintegra.  All rights reserved.
# WINTEGRA CONFIDENTIAL PROPRIETARY
# Contains Confidential Proprietary information of Wintegra.
# Reverse engineering is prohibited.
# The copyright notice does not imply publication.
#
#
# Module: makefile
# Purpose: Compile the flexmux sources
#
ifeq "$(WUFE)" "ufe412"
UFE_VER=ufe4
else
ifeq "$(WUFE)" "ufe448"
UFE_VER=ufe4
else
ifeq "$(WUFE)" "ufe412dual"
UFE_VER=ufe4
else
$(error WUFE must be ufe412 or ufe448.)
endif
endif
endif


FLEXMUX_RELEASE_SRC_DIRS := \
./flexmux \

ifeq "$(WUFE)" "ufe412"
FLEXMUX_RELEASE_SRC_DIRS += ./firmware_image_ufe412
else
ifeq "$(WUFE)" "ufe412dual"
FLEXMUX_RELEASE_SRC_DIRS += ./firmware_image_ufe412
else
FLEXMUX_RELEASE_SRC_DIRS += ./firmware_image_ufe448
endif
endif

FLEXMUX_SRC_DIRS := \
./device_driver/low_level_comms_library/stub\
./device_driver/low_level_comms_library/mailbox\
./device_driver/low_level_comms_library/mailbox/permission/yes\
./device_driver/low_level_comms_library/mailbox/permission/no\
./device_driver/low_level_comms_library/common\
./device_driver/library\
./device_driver/library/11_port_map\
./device_driver/library/09_omnispy\
./device_driver/library/09_omnispy\
./device_driver/library/08_autonomous\
./device_driver/library/07_product\
./device_driver/library/06_environment/wrapper\
./device_driver/library/06_environment/virtual_vga\
./device_driver/library/06_environment/ufe\
./device_driver/library/06_environment/pc_host\
./device_driver/library/06_environment/pc_host/test_harness\
./device_driver/library/06_environment/MIPS\
./device_driver/library/06_environment/memory_map\
./device_driver/library/06_environment/interrupt_handler\
./device_driver/library/06_environment/605_board\
./device_driver/library/05_utilities\
./device_driver/library/05_utilities/sonet_sdh_endpoint\
./device_driver/library/05_utilities/sonet_hierarchy\
./device_driver/library/05_utilities/housekeeping\
./device_driver/library/05_utilities/handle\
./device_driver/library/05_utilities/firewall\
./device_driver/library/05_utilities/error_codes\
./device_driver/library/05_utilities/crc7\
./device_driver/library/05_utilities/bit_manipulation\
./device_driver/library/05_utilities/assert\
./device_driver/library/05_utilities/api_count\
./device_driver/library/05_utilities/alarm_manager\
./device_driver/library/04_low_level_comms_layer/src\
./device_driver/library/03_data_layer\
./device_driver/library/02_model_layer/status/facilities/sonet_sdh/port/section/overhead/J0\
./device_driver/library/02_model_layer/status/facilities/sonet_sdh/port/line/overhead/S1/signalling\
./device_driver/library/02_model_layer/status/facilities/sonet_sdh/port/line/overhead/K1K2/signalling\
./device_driver/library/02_model_layer/status/facilities/sonet_sdh/low_order_path/overhead/V5\
./device_driver/library/02_model_layer/status/facilities/sonet_sdh/low_order_path/overhead/J2\
./device_driver/library/02_model_layer/status/facilities/sonet_sdh/high_order_path/overhead/J1\
./device_driver/library/02_model_layer/status/facilities/sonet_sdh/high_order_path/overhead/C2\
./device_driver/library/02_model_layer/status/facilities/socket_client/pdh/diagnostic/prbs\
./device_driver/library/02_model_layer/status/device_driver\
./device_driver/library/02_model_layer/status/device\
./device_driver/library/02_model_layer/performance_monitoring\
./device_driver/library/02_model_layer/model_control\
./device_driver/library/02_model_layer/configuration/lib/sonet_sdh_signal_label\
./device_driver/library/02_model_layer/configuration/lib/sonet_sdh_bip_threshold\
./device_driver/library/02_model_layer/configuration/lib/diagnostic_loopback\
./device_driver/library/02_model_layer/configuration/lib/ais_control\
./device_driver/library/02_model_layer/configuration/facilities/sonet_sdh/port\
./device_driver/library/02_model_layer/configuration/facilities/sonet_sdh/port/section/J0\
./device_driver/library/02_model_layer/configuration/facilities/sonet_sdh/port/line/S1\
./device_driver/library/02_model_layer/configuration/facilities/sonet_sdh/port/line/K1K2\
./device_driver/library/02_model_layer/configuration/facilities/sonet_sdh/low_order_path\
./device_driver/library/02_model_layer/configuration/facilities/sonet_sdh/low_order_path/overhead/J2\
./device_driver/library/02_model_layer/configuration/facilities/sonet_sdh/high_order_path\
./device_driver/library/02_model_layer/configuration/facilities/sonet_sdh/high_order_path/overhead/J1\
./device_driver/library/02_model_layer/configuration/facilities/sonet_sdh/high_order_path/overhead/C2\
./device_driver/library/02_model_layer/configuration/facilities/sonet_sdh/bulk\
./device_driver/library/02_model_layer/configuration/facilities/socket_client\
./device_driver/library/02_model_layer/configuration/facilities/socket_client/pdh\
./device_driver/library/02_model_layer/configuration/facilities/socket_client/pdh/diagnostic/recovered_clock\
./device_driver/library/02_model_layer/configuration/facilities/socket_client/pdh/diagnostic/prbs\
./device_driver/library/02_model_layer/configuration/facilities/socket_client/bulk\
./device_driver/library/02_model_layer/configuration/facilities/discrete_client\
./device_driver/library/02_model_layer/configuration/device\
./device_driver/library/02_model_layer/configuration/device/diagnostic/recovered_clock\
./device_driver/library/02_model_layer/configuration/connections/through\
./device_driver/library/02_model_layer/configuration/connections/add_drop\
./device_driver/library/02_model_layer/alarms/bulk\
./device_driver/library/01_rules_layer/static\
./device_driver/library/01_rules_layer/dynamic\
./device_driver/library/00_api_layer/src\

ifeq "$(WUFE)" "ufe412"
FLEXMUX_SRC_DIRS += ./device_driver/firmware_image_ufe412
else
ifeq "$(WUFE)" "ufe412dual"
FLEXMUX_SRC_DIRS += ./device_driver/firmware_image_ufe412
else
FLEXMUX_SRC_DIRS += ./device_driver/firmware_image_ufe448
endif
endif


PHY_FLEXMUX_SRC := \
WO_FRMR_ais_control_cfg_lib.c\
WO_FRMR_model_control_fsm.c\
WO_FRMR_alarms_bulk.c\
WO_FRMR_am.c\
WO_FRMR_api.c\
WO_FRMR_api_pl.c\
WO_FRMR_api_count.c\
WO_FRMR_api_internal.c\
WO_FRMR_api_porting.c\
WO_FRMR_assert.c\
WO_FRMR_auto_alarms_wrapper.c\
WO_FRMR_auto_diagnostic_wrapper.c\
WO_FRMR_auto_omnispy_nb_wrapper.c\
WO_FRMR_auto_omnispy_sb.c\
WO_FRMR_auto_pm_wrapper.c\
WO_FRMR_auto_sig_wrapper.c\
WO_FRMR_bit_utility.c\
WO_FRMR_buildinfo.c\
WO_FRMR_cfg_model.c\
WO_FRMR_conn_ad_dc_to_sc.c\
WO_FRMR_conn_ad_line_to_sc_bulk.c\
WO_FRMR_conn_ad_line_to_sc.c\
WO_FRMR_conn_thru.c\
WO_FRMR_conn_thru_bulk.c\
WO_FRMR_conn_dynamic.c\
WO_FRMR_conn_sr.c\
WO_FRMR_crc7.c\
WO_FRMR_dc_bwm_cfg.c\
WO_FRMR_dc_cas_cfg.c\
WO_FRMR_dc_cr_cfg.c\
WO_FRMR_dc_facility.c\
WO_FRMR_dc_framing_cfg.c\
WO_FRMR_dc_mapping_cfg.c\
WO_FRMR_dc_port_dr.c\
WO_FRMR_dc_port_sr.c\
WO_FRMR_device_cfg.c\
WO_FRMR_device_dr.c\
WO_FRMR_device_drive_rc_cfg.c\
WO_FRMR_device_sr.c\
WO_FRMR_diag_lb_cfg_lib.c\
WO_FRMR_diagnostic_sr.c\
WO_FRMR_envwr.c\
WO_FRMR_error_codes.c\
WO_FRMR_firewall.c\
WO_FRMR_firmware_download_fsm.c\
WO_FRMR_handle.c\
WO_FRMR_hardware_variant_dr.c\
WO_FRMR_housekeeping.c\
WO_FRMR_interrupt_handler.c\
WO_FRMR_llc_mailbox.c\
WO_FRMR_llc_mailbox_yes.c\
WO_FRMR_llcnb.c\
WO_FRMR_llcsb.c\
WO_FRMR_low_level_nb_aux_error_handler.c\
WO_FRMR_low_level_nb_aux_omnispy.c\
WO_FRMR_low_level_nb_omnispy.c\
WO_FRMR_low_level_nb_omnispy_response.c\
WO_FRMR_low_level_nb_parse_utilities.c\
WO_FRMR_low_level_nb_pm.c\
WO_FRMR_low_level_nb_signalling.c\
WO_FRMR_low_level_nb_status.c\
WO_FRMR_main_ufe.c\
WO_FRMR_memory_map.c\
WO_FRMR_MIPS_b_alarm_cb.c\
WO_FRMR_MIPS_b_diagnostic_cb.c\
WO_FRMR_MIPS_binding_env.c\
WO_FRMR_MIPS_b_omnispy_callback_table.c\
WO_FRMR_MIPS_b_pm_cb.c\
WO_FRMR_MIPS_b_pm_data_table.c\
WO_FRMR_MIPS_b_sig_cb.c\
WO_FRMR_MIPS_mailbox_comms.c\
WO_FRMR_MIPS_memory_map.c\
WO_FRMR_MIPS_monitor_loop.c\
WO_FRMR_MIPS_print_util.c\
WO_FRMR_MIPS_target_debug_utilities.c\
WO_FRMR_pm_cfg.c\
WO_FRMR_pm_engine.c\
WO_FRMR_pm_key_manager.c\
WO_FRMR_pm_model.c\
WO_FRMR_pm_sr.c\
WO_FRMR_product_capability.c\
WO_FRMR_product_init.c\
WO_FRMR_RAM.c\
WO_FRMR_report_logger.c\
WO_FRMR_sc_bulk.c\
WO_FRMR_sc_facility.c\
WO_FRMR_sc_list_fac.c\
WO_FRMR_sc_pdh_bwm_cfg.c\
WO_FRMR_sc_pdh_cas_cfg.c\
WO_FRMR_sc_pdh_cr_cfg.c\
WO_FRMR_sc_pdh_dr.c\
WO_FRMR_sc_pdh_drive_rc_cfg.c\
WO_FRMR_sc_pdh_dr_lp_mapping.c\
WO_FRMR_sc_pdh_framing_cfg.c\
WO_FRMR_sc_pdh_mapping_cfg.c\
WO_FRMR_sc_pdh_prbs_cfg.c\
WO_FRMR_sc_pdh_prbs_status.c\
WO_FRMR_sc_pdh_sr.c\
WO_FRMR_sc_pdh_ts_bc_cfg.c\
WO_FRMR_screen_alarms.c\
WO_FRMR_screen.c\
WO_FRMR_screen_pm.c\
WO_FRMR_screen_signalling.c\
WO_FRMR_sdh_bip_threshold_cfg_lib.c\
WO_FRMR_sdh_bulk.c\
WO_FRMR_sdh_C2_cfg.c\
WO_FRMR_sdh_C2_status.c\
WO_FRMR_sdh_endpoint.c\
WO_FRMR_sdh_facilities_dr.c\
WO_FRMR_sdh_ho_facility.c\
WO_FRMR_sdh_ho_overhead_sr.c\
WO_FRMR_sdh_J0_cfg.c\
WO_FRMR_sdh_J0_status.c\
WO_FRMR_sdh_J1_cfg.c\
WO_FRMR_sdh_J1_status.c\
WO_FRMR_sdh_J2_cfg.c\
WO_FRMR_sdh_J2_status.c\
WO_FRMR_sdh_K1K2_cfg.c\
WO_FRMR_sdh_K1K2_status.c\
WO_FRMR_sdh_le_static.c\
WO_FRMR_sdh_list_fac.c\
WO_FRMR_sdh_lo_facility.c\
WO_FRMR_sdh_lo_list_fac.c\
WO_FRMR_sdh_lo_overhead_sr.c\
WO_FRMR_sdh_overhead_sr.c\
WO_FRMR_sdh_port_dr.c\
WO_FRMR_sdh_port_line_sr.c\
WO_FRMR_sdh_port_mode_cfg.c\
WO_FRMR_sdh_port_prot_cfg.c\
WO_FRMR_sdh_port_rate_cfg.c\
WO_FRMR_sdh_port_dcc_cfg.c\
WO_FRMR_sdh_port_scrambling_cfg.c\
WO_FRMR_sdh_port_section_sr.c\
WO_FRMR_sdh_port_sr.c\
WO_FRMR_sdh_S1_cfg.c\
WO_FRMR_sdh_S1_status.c\
WO_FRMR_sdh_sl_cfg_lib.c\
WO_FRMR_sdh_V5_sl_status.c\
WO_FRMR_sonet_hierarchy.c\
WO_FRMR_status_device_driver.c\
WO_FRMR_status_device_firmware_version.c\
WO_FRMR_status_device_hardware_variant.c\
WO_FRMR_status_device_hardware_version.c\
WO_FRMR_status_model.c\
WO_FRMR_util.c\
WO_FRMR_vga_driver.c\
WO_FRMR_port_map.c\
WO_FRMR_firmware_image.c

# vpath specifies the directories that are searched for .c files
vpath %.c $(FLEXMUX_RELEASE_SRC_DIRS)/

FRAMER_CFLAGS_STD := -std=gnu99
FRAMER_CFLAGS_ERROR := -Werror

# Where to construct the library
FLEXMUX_OBJ_DIR := ${WPI_WDDS_DIR}/objects/$(WPI_HOST)/$(WPI_TARGET)/$(WPI_BOARD)/$(WUFE)/obj
FLEXMUX_LIB_DIR := ${WPI_WDDS_DIR}/objects/$(WPI_HOST)/$(WPI_TARGET)/$(WPI_BOARD)/$(WUFE)/lib

# The objects, dependences, and archive
PHY_FLEXMUX_OBJ := $(patsubst %.c, $(FLEXMUX_OBJ_DIR)/%.o, $(PHY_FLEXMUX_SRC))
PHY_FLEXMUX_DEP := $(patsubst %.o, %.d, $(PHY_FLEXMUX_OBJ))
PHY_FLEXMUX_O := $(FLEXMUX_LIB_DIR)/wpx_phy_omiino_pl.o

