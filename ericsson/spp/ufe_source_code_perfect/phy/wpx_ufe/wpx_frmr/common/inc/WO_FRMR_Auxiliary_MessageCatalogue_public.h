#ifndef  _OMIINO_FRAMER_AUXILIARY_MESSAGE_CATALOGUE_PUBLIC_
#define  _OMIINO_FRAMER_AUXILIARY_MESSAGE_CATALOGUE_PUBLIC_


/*
	About: Copyright

	Copyright (c) 2010  Omiino Ltd

	All rights reserved.  

	This code is provided under license and or Non-disclosure     
	Agreement and must be used solely for the purpose for which it
	was provided. 

	It must not be passed to any third party without
	the written permission of Omiino Ltd.   

	Visit <http://www.omiino.com> or send messages to support@omiino.com.


*/


#ifdef __cplusplus
extern "C"
{
#endif

enum AUXILIARY_RESPONSE_ID
{
	AUXILIARY_RESPONSE_DEVICE_DRIVER_DEFAULT_IGNORE,
	AUXILIARY_RESPONSE_DEVICE_DRIVER_PRODUCT_RELEASE,
	AUXILIARY_RESPONSE_DEVICE_DRIVER_BUILD_DATE_TIME,
	AUXILIARY_RESPONSE_DEVICE_DRIVER_BUILD_VERSION,
	AUXILIARY_RESPONSE_FIRMWARE_VERSION,
	AUXILIARY_RESPONSE_HARDWARE_INFORMATION,
	AUXILIARY_RESPONSE_HARDWARE_VARIANT,
	AUXILIARY_RESPONSE_DEVICE_MODE,
	AUXILIARY_RESPONSE_PORT_GET_RATE,
	AUXILIARY_RESPONSE_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_SHALLOW_LINE_STATE,
	AUXILIARY_RESPONSE_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DEEP_SYSTEM_STATE,
	AUXILIARY_RESPONSE_SONET_SDH_PORT_SECTION_J0_GET_MODE,
	AUXILIARY_RESPONSE_SONET_SDH_PORT_SECTION_J0_GET_TX,
	AUXILIARY_RESPONSE_SONET_SDH_PORT_SECTION_J0_GET_EX,
	AUXILIARY_RESPONSE_SONET_SDH_PORT_SECTION_J0_GET_RX,
	AUXILIARY_RESPONSE_SONET_SDH_PORT_LINE_S1_GET_TX,
	AUXILIARY_RESPONSE_SONET_SDH_PORT_LINE_K1K2_GET_TX,
	AUXILIARY_RESPONSE_SONET_SDH_PORT_LINE_S1_GET_RX,
	AUXILIARY_RESPONSE_SONET_SDH_PORT_LINE_K1K2_GET_RX,
	AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_J1_GETMODE,
	AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_J1_GET_TX,
	AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_J1_GET_EX,
	AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_J1_GET_RX,
	AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_C2_GET_RX,
	AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_C2_GET_EX,
	AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_C2_GET_TX,
	AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_J2_GET_TX,
	AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_J2_GET_EX,
	AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_J2_GET_RX,
	AUXILIARY_RESPONSE_SONET_SDH_PORT_SECTION_B1_GET_SIGNAL_FAIL_THRESHOLD,
	AUXILIARY_RESPONSE_SONET_SDH_PORT_SECTION_B1_GET_SIGNAL_DEGRADE_THRESHOLD,
	AUXILIARY_RESPONSE_SONET_SDH_PORT_SECTION_B2_GET_SIGNAL_FAIL_THRESHOLD,
	AUXILIARY_RESPONSE_SONET_SDH_PORT_SECTION_B2_GET_SIGNAL_DEGRADE_THRESHOLD,
	AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_B3_GET_SIGNAL_FAIL_THRESHOLD,
	AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_B3_GET_SIGNAL_DEGRADE_THRESHOLD,
	AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_FAIL_THRESHOLD,
	AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_DEGRADE_THRESHOLD,
	AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_LABEL_TX,
	AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_LABEL_EX,
	AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_LABEL_RX,
	AUXILIARY_RESPONSE_SONET_SDH_DIAGNOSTIC_LOOPBACK_GET_STATE,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_BANDWIDTH_TYPE,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_FRAMING,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_MAPPING,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_CAS_STATE,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_DEEP_LINE_STATE,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_SHALLOW_SYSTEM_STATE,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_AIS_CONTROL_TOWARDS_LINE_STATE,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_AIS_CONTROL_TOWARDS_SOCKET_STATE,
	AUXILIARY_RESPONSE_DEVICE_GET_BUILD_PERSONALITY,
	AUXILIARY_RESPONSE_SONET_SDH_PORT_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_GET,
	AUXILIARY_RESPONSE_FATAL_ERROR_INTERMEDIATE_REPORT_SEGMENT,
	AUXILIARY_RESPONSE_FATAL_ERROR_FINAL_REPORT_SEGMENT,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_CLOCK_RECOVERY_MODE,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_GET,
	AUXILIARY_RESPONSE_DEVICE_DRIVER_SONET_SDH_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY,
	AUXILIARY_RESPONSE_DEVICE_DRIVER_SOCKET_CLIENT_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY,
	AUXILIARY_RESPONSE_SONET_SDH_PORT_GET_FORCE_STATE,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_TERM_PRBS_GET,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_GEN_PRBS_GET,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_TERM_PRBS_GET,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_GEN_PRBS_GET,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_PRBS_GET_STATE,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_PRBS_GET_STATE,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_GET_CLIENT_APPLICATION,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_SHALLOW_SYSTEM_STATE_STATE,
	AUXILIARY_RESPONSE_BINARY_FINAL_REPORT_SEGMENT,
	AUXILIARY_RESPONSE_BINARY_INTERMEDIATE_REPORT_SEGMENT,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_TIMESTAMP_BYTE_COUNT,
	AUXILIARY_RESPONSE_WORKING,
	AUXILIARY_RESPONSE_LINE_PORT_ALARM,
	AUXILIARY_RESPONSE_PATH_ALARM,
	AUXILIARY_RESPONSE_CLIENT_ALARM,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_TIMESTAMP,
	AUXILIARY_RESPONSE_DEVICE_DRIVER_DISCRETE_CLIENT_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY,
	AUXILIARY_RESPONSE_S1_CHANGE,
	AUXILIARY_RESPONSE_K1K2_CHANGE,
	AUXILIARY_RESPONSE_DEVICE_PROTECTION_MODE,
	AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_RATE_GET,
	AUXILIARY_RESPONSE_PORT_DCC_GET,
	AUXILIARY_RESPONSE_PM_PORT_GET,
	AUXILIARY_RESPONSE_PM_HO_GET,
	AUXILIARY_RESPONSE_PM_LO_GET,
	AUXILIARY_RESPONSE_PM_E1_GET,
	AUXILIARY_RESPONSE_PM_T1_GET,
	AUXILIARY_RESPONSE_DEVICE_CARD_PROTECTION_OPERATING_MODE,
	AUXILIARY_RESPONSE_DEVICE_AIS_INSERTION_MODE,
	AUXILIARY_RESPONSE_PM_PORT_TIMESTAMP,
	AUXILIARY_RESPONSE_PM_HO_TIMESTAMP,
	AUXILIARY_RESPONSE_PM_LO_TIMESTAMP,
	AUXILIARY_RESPONSE_PM_PDH_TIMESTAMP,
	AUXILIARY_RESPONSE_PM_DISCRETE_TIMESTAMP,
	AUXILIARY_RESPONSE_PM_PORT_READ,
	AUXILIARY_RESPONSE_PM_SDH_HO_READ,
	AUXILIARY_RESPONSE_PM_SDH_LO_READ,
	AUXILIARY_RESPONSE_PM_SONET_HO_READ,
	AUXILIARY_RESPONSE_PM_SONET_LO_READ,
	AUXILIARY_RESPONSE_PM_PDH_READ,
	AUXILIARY_RESPONSE_PM_DISCRETE_READ,
	AUXILIARY_RESPONSE_PM_E3T3_GET
};




enum API_ID
{
    API_DEFAULT_RAM_IGNORE,
    API_INITIALIZATION,
    API_DEVICE_IS_READY,
    API_SERVICE_NORTHBOUND_LOWLEVEL_COMMS,
    API_SERVICE_SOUTHBOUND_LOWLEVEL_COMMS,
	API_CONFIGURATION_DEVICE_GET_BUILD_PERSONALITY,
    API_CONFIGURATION_DEVICE_SET_MODE,
    API_CONFIGURATION_DEVICE_GET_MODE,
    API_CONFIGURATION_DEVICE_SET_FRAME_ALIGNMENT_OFFSET,
    API_CONFIGURATION_DEVICE_GET_FRAME_ALIGNMENT_OFFSET,
    API_CONFIGURATION_FACILITIES_SONET_SDH_CREATE_FACILITY,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_CREATE_FACILITY,
    API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_CREATE_FACILITY,
    API_CONFIGURATION_FACILITIES_SONET_SDH_DELETE_FACILITY,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_DELETE_FACILITY,
    API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DELETE_FACILITY,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SET_RATE,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GET_RATE,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_ENABLE_SCRAMBLING,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DISABLE_SCRAMBLING,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GET_SCRAMBLING_STATE,
	API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_FORCE_A,
	API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_FORCE_B,
	API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GET_FORCE_STATE,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_SET,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_GET,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_ENABLE_SHALLOW_LINE,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DISABLE_SHALLOW_LINE,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_GET_SHALLOW_LINE_STATE,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_ENABLE_DEEP_SYSTEM,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DISABLE_DEEP_SYSTEM,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_GET_DEEP_SYSTEM_STATE,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_MODE,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_MODE,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_TX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_EX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_TX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_EX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SET_SIGNAL_FAIL_THRESHOLD,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SET_SIGNAL_DEGRADE_THRESHOLD,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_GET_SIGNAL_FAIL_THRESHOLD,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_GET_SIGNAL_DEGRADE_THRESHOLD,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_SET_TX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_GET_TX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_SET_TX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_GET_TX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SET_SIGNAL_FAIL_THRESHOLD,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SET_SIGNAL_DEGRADE_THRESHOLD,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_GET_SIGNAL_FAIL_THRESHOLD,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_GET_SIGNAL_DEGRADE_THRESHOLD,
    API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_MODE,
    API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GET_MODE,
    API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_TX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_EX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GET_TX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GET_EX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SET_TX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SET_EX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GET_TX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GET_EX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SET_SIGNAL_FAIL_THRESHOLD,
    API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SET_SIGNAL_DEGRADE_THRESHOLD,
    API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_GET_SIGNAL_FAIL_THRESHOLD,
    API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_GET_SIGNAL_DEGRADE_THRESHOLD,
    API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SET_TX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SET_EX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GET_TX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GET_EX,
	API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_SET_TX,
	API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_SET_EX,
	API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_GET_TX,
	API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_GET_EX,
	API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SET_SIGNAL_FAIL_THRESHOLD,
    API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SET_SIGNAL_DEGRADE_THRESHOLD,
    API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_FAIL_THRESHOLD,
    API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_DEGRADE_THRESHOLD,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_BANDWIDTH_TYPE,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_FRAMING,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_FRAMING,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_MAPPING,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_MAPPING,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_CLOCK_RECOVERY,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_CLOCK_RECOVERY,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_ENABLE_CAS,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DISABLE_CAS,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_CAS_STATE,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_ENABLE_TOWARDS_SOCKET,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DISABLE_TOWARDS_SOCKET,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_GET_TOWARDS_SOCKET_STATE,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_ENABLE_TOWARDS_LINE,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DISABLE_TOWARDS_LINE,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_GET_TOWARDS_LINE_STATE,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_SET,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_GET,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_ENABLE_SHALLOW_SYSTEM,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_DISABLE_SHALLOW_SYSTEM,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_GET_SHALLOW_SYSTEM_STATE,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_ENABLE_DEEP_LINE,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_DISABLE_DEEP_LINE,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_GET_DEEP_LINE_STATE,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_TERM_PRBS_SET,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_TERM_PRBS_GET,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_GEN_PRBS_SET,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_GEN_PRBS_GET,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_TERM_PRBS_SET,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_TERM_PRBS_GET,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_GEN_PRBS_SET,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_GEN_PRBS_GET,
    API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_BANDWIDTH_TYPE,
    API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_BANDWIDTH_TYPE,
    API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_FRAMING,
    API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_FRAMING,
    API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_MAPPING,
    API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_MAPPING,
    API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_CLOCK_RECOVERY,
    API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_CLOCK_RECOVERY,
    API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_ENABLE_CAS,
    API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DISABLE_CAS,
    API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_CAS_STATE,
    API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_ENABLE_TOWARDS_SOCKET,
    API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DISABLE_TOWARDS_SOCKET,
    API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_GET_TOWARDS_SOCKET_STATE,
    API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_ENABLE_TOWARDS_LINE,
    API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DISABLE_TOWARDS_LINE,
    API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_GET_TOWARDS_LINE_STATE,
    API_CONFIGURATION_CONNECTIONS_THROUGH_CREATE_CONNECTION,
    API_CONFIGURATION_CONNECTIONS_THROUGH_DELETE_CONNECTION,
    API_CONFIGURATION_CONNECTIONS_ADD_DROP_TO_CLIENT_SOCKET_CLIENT_CREATE_CONNECTION,
    API_CONFIGURATION_CONNECTIONS_ADD_DROP_TO_CLIENT_SOCKET_CLIENT_DELETE_CONNECTION,
    API_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_CREATE_CONNECTION,
    API_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_DELETE_CONNECTION,
	API_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY,
	API_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY,
	API_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY,
	API_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY,
	API_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY,
	API_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY,
    API_STATUS_DEVICE_DRIVER_VERSION_GET_INFORMATION,
    API_STATUS_DEVICE_VERSION_GET_FIRMWARE_INFORMATION,
    API_STATUS_DEVICE_VERSION_GET_HARDWARE_INFORMATION,
    API_STATUS_DEVICE_BUILD_GET_HARDWARE_VARIANT,
    API_STATUS_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_RX,
    API_STATUS_FACILITIES_SONET_SDH_PORT_LINE_S1_GET_RX,
    API_STATUS_FACILITIES_SONET_SDH_PORT_LINE_K1K2_GET_RX,
    API_STATUS_FACILITIES_SONET_SDH_HO_PATH_J1_GET_RX,
    API_STATUS_FACILITIES_SONET_SDH_HO_PATH_C2_GET_RX,
    API_STATUS_FACILITIES_SONET_SDH_LO_PATH_J2_GET_RX,
	API_STATUS_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_GET_RX,
    API_STATUS_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_PRBS_GET_STATE,
    API_STATUS_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_PRBS_GET_STATE,
    API_STATUS_FACILITIES_DISCRETE_CLIENT_DIAGNOSTIC_PRBS_GET_STATE,
    API_PM_SONET_SDH_PORT_ENABLE_MONITORING,
    API_PM_SONET_SDH_PORT_DISABLE_MONITORING,
    API_PM_SONET_SDH_HIGH_ORDER_PATH_ENABLE_MONITORING,
    API_PM_SONET_SDH_HIGH_ORDER_PATH_DISABLE_MONITORING,
    API_PM_SONET_SDH_LOW_ORDER_PATH_ENABLE_MONITORING,
    API_PM_SONET_SDH_LOW_ORDER_PATH_DISABLE_MONITORING,
    API_PM_SONET_SDH_DISCRETE_PORT_ENABLE_MONITORING,
    API_PM_SONET_SDH_DISCRETE_PORT_DISABLE_MONITORING,
	API_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_ENABLE,
	API_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_DISABLE,
	API_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_GET_STATE,
	API_BOOTLOADER_HELLO,
	API_BOOTLOADER_ACK,
	API_BOOTLOADER_NAK,
	API_FIRMWARE_HELLO,
	API_DEVICE_DRIVER_RESET,
	API_CONFIGURATION_DEVICE_SET_BUILD_PERSONALITY,
	API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_GET_SHALLOW_SYSTEM,
	API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_TIMESTAMP_BYTE_COUNT,
	API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_TIMESTAMP_BYTE_COUNT,
	API_CONFIGURATION_LOW_LEVEL_COMMS_ENABLE_ACCESS,
	API_CONFIGURATION_LOW_LEVEL_COMMS_DISABLE_ACCESS,
	
	API_BULK_CONFIGURATION_FACILITIES_SONET_SDH_CREATE_FACILITY,
    API_BULK_CONFIGURATION_FACILITIES_SOCKET_CLIENT_CREATE_FACILITY,
    API_BULK_CONFIGURATION_FACILITIES_SONET_SDH_DELETE_FACILITY,
    API_BULK_CONFIGURATION_FACILITIES_SOCKET_CLIENT_DELETE_FACILITY,

	API_BULK_CONFIGURATION_CONNECTIONS_THROUGH_CREATE_CONNECTION,
    API_BULK_CONFIGURATION_CONNECTIONS_THROUGH_DELETE_CONNECTION,
    API_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_TO_CLIENT_SOCKET_CLIENT_CREATE_CONNECTION,
    API_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_TO_CLIENT_SOCKET_CLIENT_DELETE_CONNECTION,
    
	API_BULK_PM_ENABLE_LINE, 
	API_BULK_PM_DISABLE_LINE,
	API_BULK_PM_ENABLE_HO,   
	API_BULK_PM_DISABLE_HO,  
	API_BULK_PM_ENABLE_LO,   
	API_BULK_PM_DISABLE_LO,  

	API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_ENABLE_TIMESTAMPING,
	API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DISABLE_TIMESTAMPING,
	API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_TIMESTAMPING,

	API_CONFIGURATION_PM_E1_ENABLE_MONITORING_CRC,  
	API_CONFIGURATION_PM_E1_DISABLE_MONITORING_CRC, 
	API_CONFIGURATION_PM_T1_ENABLE_MONITORING_CRC,  
	API_CONFIGURATION_PM_T1_DISABLE_MONITORING_CRC,

	API_BULK_ALARM_ENABLE_LINE,  
	API_BULK_ALARM_DISABLE_LINE, 
	API_BULK_ALARM_ENABLE_HO,    
	API_BULK_ALARM_DISABLE_HO,   
	API_BULK_ALARM_ENABLE_LO,    
	API_BULK_ALARM_DISABLE_LO,   
	API_BULK_ALARM_ENABLE_ALL,   
	API_BULK_ALARM_DISABLE_ALL,  

	API_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY,  
	API_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY,
	API_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY,

	API_PM_COLLECTION_SONET_SDH_PORT,
	API_PM_COLLECTION_SONET_SDH_HO_PATH,
	API_PM_COLLECTION_SONET_SDH_LO_PATH,
	API_PM_COLLECTION_DISCRETE_CLIENT_PORT,
	API_PM_COLLECTION_SOCKET_CLIENT_PDH_E1T1_CLIENT_PORT,

	API_CONFIGURATION_SET_DEVICE_PROTECTION_MODE,
	API_CONFIGURATION_GET_DEVICE_PROTECTION_MODE,

	API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_RATE_GET,

	API_BULK_ALARM_LIST_ALL_ACTIVE,

	API_BULK_SOCKET_INGRESS_SET_PRBS_TERM,
	API_BULK_SOCKET_INGRESS_SET_PRBS_GEN,
	API_BULK_SOCKET_EGRESS_SET_PRBS_TERM,
	API_BULK_SOCKET_EGRESS_SET_PRBS_GEN,

	API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_ENABLE,
	API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_DISABLE,
	API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_GET,

	API_PM_SONET_SDH_PORT_GET_MONITORING,
	API_PM_SONET_SDH_HIGH_ORDER_PATH_GET_MONITORING,
	API_PM_SONET_SDH_LOW_ORDER_PATH_GET_MONITORING,
	API_CONFIGURATION_PM_E1_GET_MONITORING_CRC,
	API_CONFIGURATION_PM_T1_GET_MONITORING_CRC,

	API_CONFIGURATION_SET_DEVICE_CARD_PROTECTION_OPERATION_MODE,
	API_CONFIGURATION_GET_DEVICE_CARD_PROTECTION_OPERATION_MODE,

	API_CONFIGURATION_SET_DEVICE_AIS_INSERTION_MODE,
	API_CONFIGURATION_GET_DEVICE_AIS_INSERTION_MODE,

	API_DEVICE_DIAGNOSTIC_POKE,
	API_DEVICE_DIAGNOSTIC_PEEK,
	API_DEVICE_DIAGNOSTIC_BLOCK_POKE,
	API_DEVICE_DIAGNOSTIC_BLOCK_POKE_VERIFY,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_ENABLE_TOWARDS_LINE,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_DISABLE_TOWARDS_LINE,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_GET_TOWARDS_LINE_STATE,
    API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_ERDI_SetRdiMode,
	API_CONFIGURATION_PM_E3T3_ENABLE_MONITORING,  
	API_CONFIGURATION_PM_E3T3_DISABLE_MONITORING,
	API_CONFIGURATION_PM_E3T3_GET_MONITORING,
	API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SQUELCH_RECOVERED_CLOCK_SET,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SQUELCH_RECOVERED_CLOCK_GET,
	API_PM_COLLECTION_SOCKET_CLIENT_PDH_E3T3_CLIENT_PORT,
    API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SS_BITS_SET_TX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SS_BITS_GET_RX,
    API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SFP_SetTxState,
    API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_E3T3_SET_FEAC,
	MAX_APIS

};


#ifdef __cplusplus
}
#endif


#endif /* _OMIINO_FRAMER_AUXILIARY_MESSAGE_CATALOGUE_PUBLIC_ */