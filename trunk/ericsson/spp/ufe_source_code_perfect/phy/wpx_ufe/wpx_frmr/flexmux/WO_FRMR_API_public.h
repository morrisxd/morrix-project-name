#ifndef  _WPX_UFE_FRAMER_API_PUBLIC_
#define  _WPX_UFE_FRAMER_API_PUBLIC_


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
#define XALYO_FRAMER_ACCESS
#include <wpl_mem_access.h>
#include "WO_FRMR_API_porting.h"
#include "WO_FRMR_COMMON_MailboxDefinition.h"





/*
 *
 * Section: BUILD OPTIONS
 *
 */


#define WPX_UFE_FRAMER_DEBUG                                                            (0)
#ifdef  WUFE_DUAL_EMPHY
#define WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES						(2)
#else
#define WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES						(1)
#endif
#define WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS					(16)
#define WPX_UFE_FRAMER_BUILD_OPTION_MAX_J_PER_PORT                                      (4)
#define WPX_UFE_FRAMER_BUILD_OPTION_MAX_K                                               (3)
#define WPX_UFE_FRAMER_BUILD_OPTION_MAX_L                                               (7)
#define WPX_UFE_FRAMER_BUILD_OPTION_MAX_M                                               (4)
#define WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT                                      (12)
#define WPX_UFE_FRAMER_BUILD_OPTION_MAX_V                                               (7)
#define WPX_UFE_FRAMER_BUILD_OPTION_MAX_W                                               (4)

#define WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_DISCRETE_CLIENT_PORTS			(32)
#define WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS				(1344)





/*
 *
 * ERROR CODES
 *
 */



#define WPX_UFE_FRAMER_OK								(0)	
#define WPX_UFE_FRAMER_ERROR_DEVICE_OUT_OF_RANGE					(1)	
#define WPX_UFE_FRAMER_ERROR_LINE_PORT_OUT_OF_RANGE					(2)	
#define WPX_UFE_FRAMER_ERROR_DISCRETE_CLIENT_PORT_OUT_OF_RANGE				(3)	
#define WPX_UFE_FRAMER_ERROR_SOCKET_CLIENT_PORT_OUT_OF_RANGE				(4)	
#define WPX_UFE_FRAMER_ERROR_BANDWIDTH_OUT_OF_RANGE					(5)	
#define WPX_UFE_FRAMER_ERROR_iJ_OUT_OF_RANGE						(6)	
#define WPX_UFE_FRAMER_ERROR_iK_OUT_OF_RANGE						(7)	
#define WPX_UFE_FRAMER_ERROR_iL_OUT_OF_RANGE						(8)	
#define WPX_UFE_FRAMER_ERROR_iM_OUT_OF_RANGE						(9)	
#define WPX_UFE_FRAMER_ERROR_iU_OUT_OF_RANGE						(10)	
#define WPX_UFE_FRAMER_ERROR_iV_OUT_OF_RANGE						(11)	
#define WPX_UFE_FRAMER_ERROR_iW_OUT_OF_RANGE						(12)	
#define WPX_UFE_FRAMER_ERROR_DEVICE_MODE_OUT_OF_RANGE					(13)
#define WPX_UFE_FRAMER_ERROR_PORT_RATE_OUT_OF_RANGE					(14)
#define WPX_UFE_FRAMER_ERROR_TOO_MANY_CHARS_IN_STRING					(15)
#define WPX_UFE_FRAMER_ERROR_BANDWIDTH_NOT_SUPPORTED					(16)
#define WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST					(17)
#define WPX_UFE_FRAMER_ERROR_FACILITY_ALREADY_EXISTS					(18)
#define WPX_UFE_FRAMER_ERROR_INVALID_FACILITY						(19)
#define WPX_UFE_FRAMER_ERROR_FACILITY_PART_OF_CONNECTION				(20)
#define WPX_UFE_FRAMER_ERROR_CONNECTION_BLOCKED_BY_PARENT				(21)
#define WPX_UFE_FRAMER_ERROR_CONNECTION_BLOCKED_BY_CHILD				(22)
#define WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_MISMATCH				(23)
#define WPX_UFE_FRAMER_ERROR_CONNECTION_ALREADY_EXISTS					(24)
#define WPX_UFE_FRAMER_ERROR_CONNECTION_DOES_NOT_EXIST					(25)
#define WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_ALREADY_CONNECTED			(26)
#define WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_NOT_CONNECTED				(27)
#define WPX_UFE_FRAMER_ERROR_C2_VALUE_OUT_OF_RANGE					(28)
#define WPX_UFE_FRAMER_ERROR_S1_VALUE_OUT_OF_RANGE					(29)
#define WPX_UFE_FRAMER_ERROR_K1_VALUE_OUT_OF_RANGE					(30)
#define WPX_UFE_FRAMER_ERROR_K2_VALUE_OUT_OF_RANGE					(31)
#define WPX_UFE_FRAMER_ERROR_BIP_THRESHOLD_OUT_OF_RANGE					(32)
#define WPX_UFE_FRAMER_ERROR_PATH_TRACE_OUT_OF_RANGE					(33)
#define WPX_UFE_FRAMER_ERROR_PRBS_PATTERN_TYPE_OUT_OF_RANGE				(34)
#define WPX_UFE_FRAMER_ERROR_CLIENT_TYPE_OUT_OF_RANGE					(35)
#define WPX_UFE_FRAMER_ERROR_CLIENT_FRAMING_OUT_OF_RANGE				(36)
#define WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_OUT_OF_RANGE				(37)
#define WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE					(38)
#define WPX_UFE_FRAMER_ERROR_DEVICE_CAPABILITY_NOT_DEFINED                              (39)
#define WPX_UFE_FRAMER_ERROR_DEVICE_CAPABILITY_DOES_NOT_SUPPORT_DISCRETE_CLIENT_E1T1	(40)
#define WPX_UFE_FRAMER_ERROR_ALARM_CATEGORY_OUT_OF_RANGE				(41)
#define WPX_UFE_FRAMER_ERROR_DEVICE_NOT_READY                                           (42)
#define WPX_UFE_FRAMER_ERROR_VC4_4C_J_INCORRECT                                         (43)
#define WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER                                   (44)
#define WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PARENT                                 (45)
#define WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_CHILD                                  (46)
#define WPX_UFE_FRAMER_ERROR_FACILITY_PARENT_DOES_NOT_EXIST                             (47)
#define WPX_UFE_FRAMER_ERROR_FACILITY_PARENT_IS_CONNECTED                               (48)
#define WPX_UFE_FRAMER_ERROR_STS_U_INCORRECT                                            (49)
#define WPX_UFE_FRAMER_ERROR_BUILD_OPTION_PREVENTS_FACILITY_CREATION                    (50)
#define WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH                                (51)
#define WPX_UFE_FRAMER_ERROR_BANDWIDTH_NOT_SUPPORTED_IN_DEVICE_MODE			(52)	
#define WPX_UFE_FRAMER_ERROR_RATE_NOT_SUPPORTED_IN_DEVICE_MODE				(53)	
#define WPX_UFE_FRAMER_ERROR_PORT_RATE_NOT_CONFIGURED					(54)
#define WPX_UFE_FRAMER_ERROR_BANDWIDTH_EXCEEDS_PORT_RATE				(55)
#define WPX_UFE_FRAMER_ERROR_DEVICE_CAPABILITY_DOES_NOT_SUPPORT_SONET_SDH		(56)
#define WPX_UFE_FRAMER_ERROR_FEATURE_NOT_SUPPORTED                                      (57)
#define WPX_UFE_FRAMER_ERROR_COMMON_INTERFACE                                           (58)
#define WPX_UFE_FRAMER_ERROR_CLIENT_FRAMING_INVALID_FOR_CLIENT_TYPE			(59)
#define WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE			(60)
#define WPX_UFE_FRAMER_ERROR_CLOCK_RECOVERY_MODE_OUT_OF_RANGE				(61)
#define WPX_UFE_FRAMER_ERROR_CLOCK_RECOVERY_MODE_INVALID_FOR_DISCRETE_CLIENT		(62)
#define WPX_UFE_FRAMER_ERROR_POINT_OUT_OF_RANGE						(63)
#define WPX_UFE_FRAMER_ERROR_NOT_IMPLEMENTED						(64)
#define WPX_UFE_FRAMER_ERROR_CLIENT_APPLICATION_OUT_OF_RANGE				(65)
#define WPX_UFE_FRAMER_ERROR_CLIENT_FRAMING_INVALID_FOR_CLIENT_APPLICATION_MODE		(66)
#define WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_APPLICATION_MODE		(67)
#define WPX_UFE_FRAMER_ERROR_CAS_INVALID_FOR_CLIENT_APPLICATION_MODE			(68)
#define WPX_UFE_FRAMER_ERROR_API_NO_LONGER_SUPPORTED					(69)
#define WPX_UFE_FRAMER_ERROR_PORT_HAS_FACILITIES					(70)
#define WPX_UFE_FRAMER_ERROR_CLIENT_TIMESTAMP_BYTE_COUNT_OUT_OF_RANGE			(71)
#define WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_UNDEFINED				(72)
#define WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_NOT_RECOGNISED				(73)
#define WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_INCONSISTENT_WITH_BUILD_VARIANT		(74)
#define WPX_UFE_FRAMER_ERROR_BUILD_VARIANT_UNDEFINED					(75)
#define WPX_UFE_FRAMER_ERROR_FACILITIES_EXIST						(76)
#define WPX_UFE_FRAMER_ERROR_PROTECTION_PORT_CANNOT_BE_CONFIGURED                       (77)
#define WPX_UFE_FRAMER_ERROR_V5_LABEL_VALUE_OUT_OF_RANGE				(78)
#define WPX_UFE_FRAMER_ERROR_POINT_IS_ALREADY_MONITORED					(79)
#define WPX_UFE_FRAMER_ERROR_POINT_IS_NOT_MONITORED					(80)
#define WPX_UFE_FRAMER_ERROR_CLIENT_REQUEST_NOT_SUPPORTED_FOR_FRAMED_CLIENTS		(81)
#define WPX_UFE_FRAMER_ERROR_CLIENT_PORT_NOT_MAPPED_TO_LINE_ENDPOINT			(82)
#define WPX_UFE_FRAMER_ERROR_PORT_THIS_PORT_RATE_ALREADY_CONFIGURED			(83)
#define WPX_UFE_FRAMER_ERROR_THROUGH_CONNECTED_ENDPOINT_PREVENTS_CONFIGURATION_CHANGE	(84)
#define WPX_UFE_FRAMER_ERROR_TIMESTAMP_GAIN_OUT_OF_RANGE                                (85)
#define WPX_UFE_FRAMER_ERROR_SOCKET_CLIENT_OUTPUT_SELECTOR_OUT_OF_RANGE			(86)
#define WPX_UFE_FRAMER_ERROR_PM_POINT_UNAVAILABLE					(87)
#define WPX_UFE_FRAMER_PM_M_OF_N_LIMIT_REACHED						(88)
#define WPX_UFE_FRAMER_ERROR_PM_TABLE_INDEX_OUT_OF_RANGE				(89)
#define WPX_UFE_FRAMER_ERROR_DEVICE_PROTECTION_MODE_OUT_OF_RANGE			(90)
#define WPX_UFE_FRAMER_ERROR_DEVICE_PROTECTION_MODE_NOT_COMPATIBLE_WITH_PERSONALITY	(91)
#define WPX_UFE_FRAMER_ERROR_PORT_RATE_IS_CONFIGURED					(92)
#define WPX_UFE_FRAMER_ERROR_PERSONALITY_PREVENTS_PORT_RATE				(93)
#define WPX_UFE_FRAMER_ERROR_SOCKET_CLIENT_RATE_PARAMETER_OUT_OF_RANGE			(94)
#define WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT					(95)
#define WPX_UFE_FRAMER_ERROR_SDH_OUTPUT_SELECTOR_OUT_OF_RANGE				(96)
#define WPX_UFE_FRAMER_ERROR_DEVICE_MODE_PREVENTS_REQUEST				(97)
#define WPX_UFE_FRAMER_ERROR_DCC_SETTING_OUT_OF_RANGE					(98)
#define WPX_UFE_FRAMER_ERROR_DEVICE_CARD_PROTECTION_OPERATING_MODE_OUT_OF_RANGE		(99)
#define WPX_UFE_FRAMER_ERROR_DEVICE_CARD_PROTECTION_OPERATING_MODE_NOT_APPLICABLE	(100)
#define WPX_UFE_FRAMER_ERROR_CLIENT_TYPE_NOT_SUPPORTED					(101)
#define WPX_UFE_FRAMER_ERROR_DEVICE_TEST_MODE_PREVENTS_REQUEST				(102)
#define WPX_UFE_FRAMER_ERROR_RDI_MODE_OUT_OF_RANGE                         (103)
#define WPX_UFE_FRAMER_ERROR_PDH_RDI_NOT_SUPPORTED_FOR_UNFRAMED_CLIENTS		(104)
#define WPX_UFE_FRAMER_ERROR_SQUELCH_MODE_OUT_OF_RANGE                      (105)
#define WPX_UFE_FRAMER_ERROR_SS_BITS_VALUE_OUT_OF_RANGE                         (106)
#define WPX_UFE_FRAMER_ERROR_SFP_STATE_OUT_OF_RANGE                         (107)


/* Need to update OMIINO_FRAMER_ErrorCodeToTxt if the above table changes */








/* 
 *
 * Build Personalities
 *
 */ 

#define WPX_UFE_FRAMER_BUILD_PERSONALITY_UNDEFINED													(0)
#define WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12											(1)
#define WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3												(2)
#define WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3_OC12_4_PLUS_4_OC3								(3)
#define WPX_UFE_FRAMER_BUILD_PERSONALITY_16_OC3														(4)
#define WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3_OC12											(5)
#define WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12_4_PLUS_4_OC3								(6)
#define WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12											(7)
#define WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3												(8)
#define WPX_UFE_FRAMER_BUILD_PERSONALITY_64_DISCRETE_ONLY											(9) 
#define WPX_UFE_FRAMER_BUILD_PERSONALITY_32_DISCRETE_ONLY											(10)
#define WPX_UFE_FRAMER_BUILD_PERSONALITY_16_DISCRETE_ONLY											(11)
#define WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12_OC48										(12)
#define WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12_32_DISCRETE								(13)
#define WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_32_DISCRETE									(14)
#define WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3												(15)
#define WPX_UFE_FRAMER_BUILD_PERSONALITY_MAX														(16)




/* 
 *
 * Hardware Builds
 *
 */ 

#define WPX_UFE_FRAMER_HARDWARE_BUILD_UNDEFINED												(0)
#define WPX_UFE_FRAMER_HARDWARE_BUILD_1_OC48_6										(1)
#define WPX_UFE_FRAMER_HARDWARE_BUILD_2_OC24_6										(2)
#define WPX_UFE_FRAMER_HARDWARE_BUILD_3_OC12_6										(3)
#define WPX_UFE_FRAMER_HARDWARE_BUILD_4_DISCRETE_ONLY_6								(4)
#define WPX_UFE_FRAMER_HARDWARE_BUILD_5_OC48_6										(5)
#define WPX_UFE_FRAMER_HARDWARE_BUILD_6_MIXED_6										(6)
#define WPX_UFE_FRAMER_HARDWARE_BUILD_MAX													(7)



/* 
 *
 * DEVICE MODES 
 *
 */ 


#define WPX_UFE_FRAMER_DEVICE_MODE_SDH													(0)
#define WPX_UFE_FRAMER_DEVICE_MODE_SONET												(1)
#define WPX_UFE_FRAMER_MAX_DEVICE_MODES													(2)


/*
 *
 * DEVICE PROTECTION MODES
 *
 */
#define WPX_UFE_DEVICE_PROTECTION_MODE_NONE												(0)
#define WPX_UFE_DEVICE_PROTECTION_MODE_LINE												(1)
#define WPX_UFE_DEVICE_PROTECTION_MODE_CARD												(2)
#define WPX_UFE_MAX_DEVICE_PROTECTION_MODES												(3)


/*
 *
 * DEVICE CARD PROTECTION OPERATING MODES
 *
 */
#define WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_NONE								(0)
#define WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_MASTER							(1)
#define WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_SLAVE								(2)
#define WPX_UFE_MAX_DEVICE_CARD_PROTECTION_OPERATING_MODES								(3)



/* 
 *
 * LINE PORT RATES 
 *
 */ 

#define WPX_UFE_FRAMER_LINE_PORT_RATE_OFF                   							(0)
#define WPX_UFE_FRAMER_LINE_PORT_RATE_STM1                  							(1)
#define WPX_UFE_FRAMER_LINE_PORT_RATE_STM4                  							(2)
#define WPX_UFE_FRAMER_LINE_PORT_RATE_OC3												(3)
#define WPX_UFE_FRAMER_LINE_PORT_RATE_OC12												(4)
#define WPX_UFE_FRAMER_LINE_PORT_MAX_RATES												(5)



/*
 *
 * PERFORMANCE MONITORING POINTS
 *
 */


#define WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B1					(0)
#define WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B2					(1)
#define WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_M1					(2)
#define WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_PORT		(3)


#define WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3					(0)
#define WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1					(1)
#define WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_HO_PATH		(2)


#define WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_BIP				(0)
#define WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_REI				(1)
#define WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_LO_PATH		(2)

#define WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_ENDPOINTS					(3) /* Union of the above */



#define WPX_UFE_FRAMER_CLIENT_SIDE_PERFORMANCE_MONITORING_DATA_POINT_CV_L				(0)
#define WPX_UFE_FRAMER_CLIENT_SIDE_PERFORMANCE_MONITORING_DATA_POINT_CV_P				(1)
#define WPX_UFE_FRAMER_CLIENT_SIDE_PERFORMANCE_MONITORING_DATA_POINT_CV_PFE				(2)
#define WPX_UFE_FRAMER_CLIENT_SIDE_PERFORMANCE_MONITORING_DATA_POINT_CVP_P				(3)
#define WPX_UFE_FRAMER_CLIENT_SIDE_PERFORMANCE_MONITORING_DATA_POINT_CVCP_P				(4)
#define WPX_UFE_FRAMER_CLIENT_SIDE_PERFORMANCE_MONITORING_DATA_POINT_CVCP_PFE		  	(5)
#define WPX_UFE_FRAMER_CLIENT_SIDE_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_PORT		(6)

#define WPX_UFE_FRAMER_PDH_E1_T1_CRC									(0)
#define WPX_UFE_FRAMER_PDH_E3T3_PM_BIP									(1)
#define WPX_UFE_FRAMER_PDH_E3T3_PM_FEBE									(2)
#define WPX_UFE_FRAMER_MAX_PDH_MONITORED_POINTS_PER_PDH_ELEMENT						(3)

/*
 *
 * BIP THRESHOLDS
 *
 */

#define WPX_UFE_FRAMER_THRESHOLD_OFF													(0)
#define WPX_UFE_FRAMER_THRESHOLD_10E3													(1)
#define WPX_UFE_FRAMER_THRESHOLD_10E4													(2)
#define WPX_UFE_FRAMER_THRESHOLD_10E5													(3)
#define WPX_UFE_FRAMER_THRESHOLD_10E6													(4)
#define WPX_UFE_FRAMER_THRESHOLD_10E7													(5)
#define WPX_UFE_FRAMER_THRESHOLD_10E8													(6)
#define WPX_UFE_FRAMER_THRESHOLD_10E9													(7)
#define WPX_UFE_FRAMER_MAX_BIP_THRESHOLD                                    			(8)



/*
 *
 * LINE SIDE SONET/SDH ALARM TYPES
 *
 */


#define WPX_UFE_FRAMER_SDH_LOS															(0)
#define WPX_UFE_FRAMER_SDH_LOF															(1)
#define WPX_UFE_FRAMER_SDH_RS_TIM														(2)	
#define WPX_UFE_FRAMER_SDH_MS_AIS														(3)
#define WPX_UFE_FRAMER_SDH_MS_RDI														(4)	
#define WPX_UFE_FRAMER_SDH_OOF														    (5)	
#define WPX_UFE_FRAMER_SDH_MS_EXC														(6)
#define WPX_UFE_FRAMER_SDH_MS_DEG														(7)
#define WPX_UFE_FRAMER_SDH_AU_AIS														(8)
#define WPX_UFE_FRAMER_SDH_AU_LOP														(9)
#define WPX_UFE_FRAMER_SDH_HP_LOM														(10)	
#define WPX_UFE_FRAMER_SDH_HP_TIM														(11)	
#define WPX_UFE_FRAMER_SDH_HP_PLM														(12)	
#define WPX_UFE_FRAMER_SDH_HP_UNEQ														(13)	
#define WPX_UFE_FRAMER_SDH_HP_RDI														(14)	
#define WPX_UFE_FRAMER_SDH_HP_EXC														(15)
#define WPX_UFE_FRAMER_SDH_HP_DEG														(16)
#define WPX_UFE_FRAMER_SDH_LP_TIM														(17)	
#define WPX_UFE_FRAMER_SDH_LP_PLM														(18)	
#define WPX_UFE_FRAMER_SDH_LP_UNEQ														(19)	
#define WPX_UFE_FRAMER_SDH_LP_RDI														(20)	
#define WPX_UFE_FRAMER_SDH_LP_EXC														(21)
#define WPX_UFE_FRAMER_SDH_LP_DEG														(22)
#define WPX_UFE_FRAMER_SDH_TU_AIS														(23)
#define WPX_UFE_FRAMER_SDH_TU_LOP														(24)
#define WPX_UFE_FRAMER_SONET_LOS														(25)
#define WPX_UFE_FRAMER_SONET_LOF														(26)
#define WPX_UFE_FRAMER_SONET_TIM_S														(27)	
#define WPX_UFE_FRAMER_SONET_AIS_L														(28)
#define WPX_UFE_FRAMER_SONET_RDI_L														(29)	
#define WPX_UFE_FRAMER_SONET_OOF														(30)
#define WPX_UFE_FRAMER_SONET_EXC_L														(31)
#define WPX_UFE_FRAMER_SONET_DEG_L														(32)
#define WPX_UFE_FRAMER_SONET_LOM_P														(33)	
#define WPX_UFE_FRAMER_SONET_TIM_P														(34)	
#define WPX_UFE_FRAMER_SONET_PLM_P														(35)	
#define WPX_UFE_FRAMER_SONET_UNEQ_P														(36)	
#define WPX_UFE_FRAMER_SONET_RDI_P														(37)	
#define WPX_UFE_FRAMER_SONET_EXC_P														(38)
#define WPX_UFE_FRAMER_SONET_DEG_P														(39)
#define WPX_UFE_FRAMER_SONET_TIM_V														(40)	
#define WPX_UFE_FRAMER_SONET_PLM_V														(41)	
#define WPX_UFE_FRAMER_SONET_UNEQ_V														(42)	
#define WPX_UFE_FRAMER_SONET_RDI_V														(43)	
#define WPX_UFE_FRAMER_SONET_EXC_V														(44)
#define WPX_UFE_FRAMER_SONET_DEG_V														(45)
#define WPX_UFE_FRAMER_SONET_AIS_V														(46)
#define WPX_UFE_FRAMER_SONET_LOP_V														(47)
#define WPX_UFE_FRAMER_SONET_AIS_P														(48)	
#define WPX_UFE_FRAMER_SONET_LOP_P														(49)
#define WPX_UFE_FRAMER_SDH_RS_EXC														(50)
#define WPX_UFE_FRAMER_SDH_RS_DEG														(51)
#define WPX_UFE_FRAMER_SONET_EXC_S														(52)
#define WPX_UFE_FRAMER_SONET_DEG_S														(53)
#define WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES									(54)



/*
 *
 * CLIENT ALARM TYPES
 *
 */


#define WPX_UFE_FRAMER_DISCRETE_CLIENT_LOS												(0)
#define WPX_UFE_FRAMER_DISCRETE_CLIENT_LOF												(1)
#define WPX_UFE_FRAMER_DISCRETE_CLIENT_AIS												(2)
#define WPX_UFE_FRAMER_DISCRETE_CLIENT_AIS_CI											(3)
#define WPX_UFE_FRAMER_DISCRETE_CLIENT_RAI												(4)
#define WPX_UFE_FRAMER_DISCRETE_CLIENT_RAI_CI											(5)
#define WPX_UFE_FRAMER_DISCRETE_CLIENT_MAX_ALARM_CATEGORIES								(6)


/*
 *
 * DEVICE LEVEL DIAGNOSTIC ALARM TYPES
 *
 */

#define WPX_UFE_FRAMER_SOCKET_CLIENT_E_RFI												(0)
#define WPX_UFE_FRAMER_SOCKET_CLIENT_A_RAI												(1)
#define WPX_UFE_FRAMER_SOCKET_CLIENT_LOS_AIS											(2)
#define WPX_UFE_FRAMER_SOCKET_CLIENT_OOF_LOF											(3)
#define WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_XA											(4)
#define WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_AIS											(5)
#define WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_LOF										(6)
#define WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_FEAC										(7)
#define WPX_UFE_FRAMER_SOCKET_CLIENT_MAX_ALARM_CATEGORIES								(8)




/*
 *
 * LINE SIDE BANDWIDTH TYPES
 *
 */



#define WPX_UFE_FRAMER_LINE_BANDWIDTH_NULL                                  			(0)
#define WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C                              			(1)
#define WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4                                 			(2)
#define WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI                              			(3)
#define WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO                              			(4)
#define WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12                                			(5)
#define WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11                                			(6)
#define WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C                            			(7)
#define WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C                            			(8)
#define WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C                             			(9)
#define WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1                              			(10)
#define WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2                               			(11)
#define WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15                              			(12)
#define WPX_UFE_FRAMER_MAX_LINE_BANDWIDTH_TYPES          								(13)





	
/*
 *
 * PATH TRACE MODES
 *
 *
 */


#define WPX_UFE_FRAMER_PATH_TRACE_MODE_1_BYTE											(1)
#define WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE											(16)
#define WPX_UFE_FRAMER_PATH_TRACE_MODE_64_BYTE											(64)






/* 
 *
 * CLIENT TYPES 
 *
 */ 


#define WPX_UFE_FRAMER_CLIENT_NULL                         								(0)
#define WPX_UFE_FRAMER_CLIENT_E1                          								(1)
#define WPX_UFE_FRAMER_CLIENT_T1                          								(2)
#define WPX_UFE_FRAMER_CLIENT_E3                         								(3)
#define WPX_UFE_FRAMER_CLIENT_T3                          								(4)
#define WPX_UFE_FRAMER_CLIENT_C4														(5)   
#define WPX_UFE_FRAMER_CLIENT_C4_4C														(6)
#define WPX_UFE_FRAMER_CLIENT_C3														(7)
#define WPX_UFE_FRAMER_CLIENT_C12														(8)
#define WPX_UFE_FRAMER_CLIENT_C11														(9)
#define WPX_UFE_FRAMER_CLIENT_MAX_BANDIWDTH_TYPES										(10)




/* 
 *
 * CLIENT MAPPINGS 
 *
 */ 

#define WPX_UFE_FRAMER_CLIENT_MAPPING_BULK_FILL                     					(0) 
#define WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC                           				(1) 
#define WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_SYNC                         					(2) 
#define WPX_UFE_FRAMER_CLIENT_MAPPING_BYTE_SYNC                       					(3) 
#define WPX_UFE_FRAMER_CLIENT_MAX_MAPPING_TYPES          								(4)


	
/* 
 *
 * CLIENT FRAMING 
 *
 */ 

#define WPX_UFE_FRAMER_CLIENT_FRAMING_NULL	(0)
#define WPX_UFE_FRAMER_CLIENT_FRAMING_SF_D4	(1)
#define WPX_UFE_FRAMER_CLIENT_FRAMING_ESF       (2)
#define WPX_UFE_FRAMER_CLIENT_PCM31CRC          (3)
#define WPX_UFE_FRAMER_CLIENT_UNFRAMED          (4)
#define WPX_UFE_FRAMER_CLIENT_PCM31		(5)
#define	WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_M23       (6)
#define	WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_CPARITY   (7)
#define	WPX_UFE_FRAMER_CLIENT_FRAMING_E3_G751      (8)
#define	WPX_UFE_FRAMER_CLIENT_FRAMING_E3_G751_ATM  (9)
#define WPX_UFE_FRAMER_CLIENT_MAX_FRAMING_TYPES (10)



	
/*
 *
 * PRBS STATES
 *
 */



#define WPX_UFE_FRAMER_PRBS_STATE_IDLE           												(0)
#define WPX_UFE_FRAMER_PRBS_STATE_ERRORS         												(1)
#define WPX_UFE_FRAMER_PRBS_STATE_CLEAR          												(2)
#define WPX_UFE_FRAMER_PRBS_STATE_INACTIVE       												(3)
#define WPX_UFE_FRAMER_PRBS_STATE_MAX_TYPES       												(4)




/*
 *
 * PRBS PATTERN TYPES
 *
 */



#define WPX_UFE_FRAMER_PRBS_DISABLED                                        			(0)
#define WPX_UFE_FRAMER_PRBS_15                                              			(1)
#define WPX_UFE_FRAMER_PRBS_20                                              			(2)
#define WPX_UFE_FRAMER_PRBS_23                                              			(3)
#define WPX_UFE_FRAMER_PRBS_COUNT_SEQUENCE                                  			(4)
#define WPX_UFE_FRAMER_PRBS_ALL_1                                           			(5)
#define WPX_UFE_FRAMER_PRBS_ALL_0                                           			(6)
#define WPX_UFE_FRAMER_PRBS_ALTERNATING_01												(7)
#define WPX_UFE_FRAMER_PRBS_PATTERN_MAX													(8)



/*
 *
 * CLOCK RECOVERY_MODE
 *
 */



#define WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH                                 			(0)
#define WPX_UFE_FRAMER_TRANSMIT_TIMING_INPUT_REFERENCE_CLOCK                   			(1)
#define WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING			                  			(2)
#define WPX_UFE_FRAMER_TRANSMIT_TIMING_WINPATH			                  				(3)
#define WPX_UFE_FRAMER_TRANSMIT_TIMING_MAX				                  				(4)

#define MAX_SOCKET_CLIENT_RECOVERED_CLOCKS												(2)
#define MAX_SONET_SDH_RECOVERED_CLOCKS													(2)

/*
 *
 *Recoverd clcok Squelch Mode
 *
 */

#define WPX_UFE_FRAMER_RECOVERED_CLOCK_SQUELCH_MODE_DISABLE                                  (0)
#define WPX_UFE_FRAMER_RECOVERED_CLOCK_SQUELCH_MODE_ENABLE                                   (1)
#define WPX_UFE_FRAMER_RECOVERED_CLOCK_SQUELCH_MODE_MAX                                      (2)

/*
 *
 * PROTECTION FORCE
 *
 */



#define WPX_UFE_FRAMER_PROTECTION_FORCE_A                                 				(0)
#define WPX_UFE_FRAMER_PROTECTION_FORCE_B					                   			(1)
#define WPX_UFE_FRAMER_PROTECTION_FORCE_MAX					                   			(2)



/*
 *
 * DCC TYPES
 *
 */


#define WPX_UFE_FRAMER_DCC_RS_SECTION_D1_D3                                         	(0)
#define WPX_UFE_FRAMER_DCC_MS_LINE_D4_D12                                              	(1)
#define WPX_UFE_FRAMER_DCC_MAX			                                              	(2)



/*
 *
 * Timestamping parameters
 *
 */

#define MAX_TIMESTAMP_GAIN																(0x0001FFFF)




/*
 *
 * PDH Recovered Clock Rate parameters
 *
 */

#define RECOVERED_CLOCK_RATE_PARAMETER_1544											(1544)
#define RECOVERED_CLOCK_RATE_PARAMETER_2048											(2048)

/*
 *
 * Rdi mode parameters
 *
 */

#define WPX_UFE_FRAMER_RDI_MODE_ENABLE                                         	(0)
#define WPX_UFE_FRAMER_ERDI_MODE_ENABLE                                             (1)
#define WPX_UFE_FRAMER_RDI_MODE_MAX			                                    (2)

#define WPX_UFE_FRAMER_SS_BITS_TX_MAX                           (0x3)

/*
 *
 * SFP Tx Enable parameters
 * this parameter send via port set rate message to the firmware
 * in order to flag weither to enable the SFP TX or not
 *
 */
#define WPX_UFE_FRAMER_SFP_NO_TX_ENABLE (0)
#define WPX_UFE_FRAMER_SFP_TX_ENABLE (1)

/*
 *
 * SFP Tx State parameters
 * this parameter send via SFP set state message to the firmware
 *
 */
#define WPX_UFE_FRAMER_SFP_TX_STATE_DISABLE (0)
#define WPX_UFE_FRAMER_SFP_TX_STATE_ENABLE (1)





/*
 *
 * Section: DEVICE DRIVER TYPE DEFINITIONS
 *
 */



typedef struct WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE
{
	U8																			K1;
	U8																			K2;

} WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE;



typedef struct WPX_UFE_FRAMER_SDH_ENDPOINT_TYPE
{
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;

} WPX_UFE_FRAMER_SDH_ENDPOINT_TYPE;



typedef struct WPX_UFE_FRAMER_SONET_ENDPOINT_TYPE
{
	U8 iU;
	U8 iV;
	U8 iW;

} WPX_UFE_FRAMER_SONET_ENDPOINT_TYPE;



typedef struct WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE
{
	U8 iLinePort;
	U8 Bandwidth;
	
	union
	{
		WPX_UFE_FRAMER_SDH_ENDPOINT_TYPE     sdh;
		WPX_UFE_FRAMER_SONET_ENDPOINT_TYPE   sonet;
	} u;      
      
} WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE;








typedef struct WPX_UFE_FRAMER_ENVIRONMENT_TABLE_TYPE
{
	void (*EnterCriticalRegion)(U8 AnyHandle);
    void (*LeaveCriticalRegion)(U8 AnyHandle);
    void (*CriticalRegionCreateHandle)(U8 *pNewHandle, char * pHandleName);
    void (*Trace)(char * pTraceMessage);
    void (*FatalErrorHandler)(char * pFatalErrorMessage);

} WPX_UFE_FRAMER_ENVIRONMENT_TABLE_TYPE;



/*
 *
 * Section: AUTONOMOUS PERFORMANCE MONITORING CALLBACKS/BINDINGS
 *
 */


typedef void (*WPX_UFE_FRAMER_SONET_SDH_PORT_PERFORMANCE_MONITORING_ANNOUNCE_TYPE)(U8 iDevice, U8 iTableIndex, U32 TimeStamp);
typedef void (*WPX_UFE_FRAMER_SONET_SDH_HO_PATH_PERFORMANCE_MONITORING_ANNOUNCE_TYPE)(U8 iDevice, U8 iTableIndex, U32 TimeStamp);
typedef void (*WPX_UFE_FRAMER_SONET_SDH_LO_PATH_PERFORMANCE_MONITORING_ANNOUNCE_TYPE)(U8 iDevice, U8 iTableIndex, U32 TimeStamp);
typedef void (*WPX_UFE_FRAMER_DISCRETE_CLIENT_PERFORMANCE_MONITORING_ANNOUNCE_TYPE)(U8 iDevice, U8 iTableIndex, U32 TimeStamp);
typedef void (*WPX_UFE_FRAMER_PDH_PERFORMANCE_MONITORING_ANNOUNCE_TYPE)(U8 iDevice, U8 iTableIndex, U32 TimeStamp);



/*
 *
 * Section: AUTONOMOUS ALARM CALLBACKS/BINDINGS
 *
 */

typedef void (*WPX_UFE_FRAMER_SONET_SDH_PORT_ALARM_ANNOUNCE_TYPE)(U8 iDevice, U8 iLinePort, U8 AlarmCategory, U8 IsAsserted);
typedef void (*WPX_UFE_FRAMER_SONET_SDH_PATH_ALARM_ANNOUNCE_TYPE)(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 AlarmCategory, U8 IsAsserted);
typedef void (*WPX_UFE_FRAMER_DISCRETE_CLIENT_PORT_ALARM_ANNOUNCE_TYPE)(U8 iDevice, U8 iDiscreteClientPort, U8 AlarmCategory, U8 IsAsserted);
typedef void (*WPX_UFE_FRAMER_SOCKET_CLIENT_PDH_ALARM_ANNOUNCE_TYPE)(U8 iDevice, U32 iSocketClientPDH, U8 AlarmCategory, U8 IsAsserted);




/*
 *
 * Section: AUTONOMOUS SIGNALLING CALLBACKS/BINDINGS
 *
 */


typedef void (*WPX_UFE_FRAMER_SONET_SDH_SECTION_K1K2_ANNOUNCE_TYPE)(U8 iDevice, U8 iLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE *pRX_K1K2);
typedef void (*WPX_UFE_FRAMER_SONET_SDH_SECTION_S1_ANNOUNCE_TYPE)(U8 iDevice, U8 iLinePort, U8 S1);



typedef struct WPX_UFE_FRAMER_PERFORMANCE_MONITORING_TABLE_ANNOUNCE_TYPE
{
	WPX_UFE_FRAMER_SONET_SDH_PORT_PERFORMANCE_MONITORING_ANNOUNCE_TYPE					SONET_SDH_PortPerformanceMonitoringAnnounce;
	WPX_UFE_FRAMER_SONET_SDH_HO_PATH_PERFORMANCE_MONITORING_ANNOUNCE_TYPE				SONET_SDH_HighOrderPathPerformanceMonitoringAnnounce;
	WPX_UFE_FRAMER_SONET_SDH_LO_PATH_PERFORMANCE_MONITORING_ANNOUNCE_TYPE				SONET_SDH_LowOrderPathPerformanceMonitoringAnnounce;
	WPX_UFE_FRAMER_DISCRETE_CLIENT_PERFORMANCE_MONITORING_ANNOUNCE_TYPE					DiscreteClientPortPerformanceMonitoringAnnounce;
	WPX_UFE_FRAMER_PDH_PERFORMANCE_MONITORING_ANNOUNCE_TYPE								PDH_PerformanceMonitoringAnnounce;


} WPX_UFE_FRAMER_PERFORMANCE_MONITORING_TABLE_ANNOUNCE_TYPE;




typedef struct WPX_UFE_FRAMER_ALARM_OUTPUT_TABLE_ANNOUNCE_TYPE
{
	WPX_UFE_FRAMER_SONET_SDH_PORT_ALARM_ANNOUNCE_TYPE										SONET_SDH_PortAlarmAnnounce;
	WPX_UFE_FRAMER_SONET_SDH_PATH_ALARM_ANNOUNCE_TYPE										SONET_SDH_PathAlarmAnnounce;
	WPX_UFE_FRAMER_DISCRETE_CLIENT_PORT_ALARM_ANNOUNCE_TYPE									DiscreteClientPortAlarmAnnounce;
	WPX_UFE_FRAMER_SOCKET_CLIENT_PDH_ALARM_ANNOUNCE_TYPE									SocketClientSidePortAlarmAnnounce;

} WPX_UFE_FRAMER_ALARM_OUTPUT_TABLE_ANNOUNCE_TYPE;




typedef struct WPX_UFE_FRAMER_SIGNALLING_TABLE_ANNOUNCE_TYPE
{
	WPX_UFE_FRAMER_SONET_SDH_SECTION_K1K2_ANNOUNCE_TYPE										LineSideSectionK1K2Announce;
	WPX_UFE_FRAMER_SONET_SDH_SECTION_S1_ANNOUNCE_TYPE										LineSideSectionS1Announce;

} WPX_UFE_FRAMER_SIGNALLING_TABLE_ANNOUNCE_TYPE;


typedef void (*WPX_UFE_FRAMER_DEVICE_READY_ANNOUNCE_TYPE)(U8 iDevice, unsigned char Reported_HardwareBuildVariant);
typedef void (*WPX_UFE_FRAMER_FIRMWARE_RESTART_ANNOUNCE_TYPE)(U8 iDevice, unsigned char Reported_HardwareBuildVariant);
typedef void (*WPX_UFE_FRAMER_PEEK_RESPONSE_ANNOUNCE_TYPE)(U8 iDevice, U32 Address, U32 Value);
typedef void (*WPX_UFE_FRAMER_BLOCK_POKE_VERIFY_RESPONSE_ANNOUNCE_TYPE)(U8 iDevice, U32 VerifyResult);
typedef void (*WPX_UFE_FRAMER_ALARM_MAP_RESPONSE_ANNOUNCE_TYPE)(U8 iDevice);
typedef void (*WPX_UFE_FRAMER_LOCK_CALLBACK_TYPE)(void);
typedef void (*WPX_UFE_FRAMER_SS_BITS_ANNOUNCE_TYPE)(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType,U8 Value);


typedef struct WPX_UFE_FRAMER_AUTONOMOUS_OUTPUT_TABLE_TYPE
{
	WPX_UFE_FRAMER_ALARM_OUTPUT_TABLE_ANNOUNCE_TYPE											AutonomousAlarmTable;
	WPX_UFE_FRAMER_PERFORMANCE_MONITORING_TABLE_ANNOUNCE_TYPE								AutonomousPerformanceMonitoringTable;
	WPX_UFE_FRAMER_SIGNALLING_TABLE_ANNOUNCE_TYPE											AutonomousSignallingTable;

} WPX_UFE_FRAMER_AUTONOMOUS_OUTPUT_TABLE_TYPE;





#define PM_PORT_ARRAY_SIZE	(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS*WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_PORT*sizeof(U32))
typedef struct WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_PORT_ENDPOINT_TYPE
{
	U32																		        Firewall_TOP;
	U32																		        TimeStamp;
    U32																				Count			[WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS]
                                                                                                    [WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_PORT];
	U8																				IsMonitored		[WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS]
                                                                                                    [WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_PORT];
	U32																		        Firewall_BOT;

} WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_PORT_ENDPOINT_TYPE;


#define PM_HO_SDH_ARRAY_SIZE  (WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS*WPX_UFE_FRAMER_BUILD_OPTION_MAX_J_PER_PORT*WPX_UFE_FRAMER_BUILD_OPTION_MAX_K*WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_HO_PATH*sizeof(U32))
#define PM_HO_SONET_ARRAY_SIZE (WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS*WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT*WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_HO_PATH*sizeof(U32))

typedef struct WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_HO_PATH_ENDPOINT_TYPE
{
	U32																		        Firewall_TOP;
	U32																		        TimeStamp;
    U8                                                                              PortMode;
    union
    {
        U32																			SDH				[WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS]
                                                                                                    [WPX_UFE_FRAMER_BUILD_OPTION_MAX_J_PER_PORT]
                                                                                                    [WPX_UFE_FRAMER_BUILD_OPTION_MAX_K] /* for VC3 Hi */
                                                                                                    [WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_HO_PATH];

        U32																			SONET			[WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS]
                                                                                                    [WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT]
                                                                                                    [WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_HO_PATH];

    } Count;

	union
    {
        U8																			SDH				[WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS]
                                                                                                    [WPX_UFE_FRAMER_BUILD_OPTION_MAX_J_PER_PORT]
                                                                                                    [WPX_UFE_FRAMER_BUILD_OPTION_MAX_K] /* for VC3 Hi */
                                                                                                    [WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_HO_PATH];

        U8																			SONET			[WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS]
                                                                                                    [WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT]
                                                                                                    [WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_HO_PATH];

    } IsMonitored;

    U32																		        Firewall_BOT;
    

} WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_HO_PATH_ENDPOINT_TYPE;


#define PM_LO_SDH_ARRAY_SIZE	(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS*WPX_UFE_FRAMER_BUILD_OPTION_MAX_J_PER_PORT*WPX_UFE_FRAMER_BUILD_OPTION_MAX_K*WPX_UFE_FRAMER_BUILD_OPTION_MAX_L*WPX_UFE_FRAMER_BUILD_OPTION_MAX_M*WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_LO_PATH*sizeof(U32))
#define PM_LO_SONET_ARRAY_SIZE	(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS*WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT*WPX_UFE_FRAMER_BUILD_OPTION_MAX_V*WPX_UFE_FRAMER_BUILD_OPTION_MAX_W*WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_LO_PATH*sizeof(U32))




typedef struct WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_LO_PATH_ENDPOINT_TYPE
{
	U32																		        Firewall_TOP;
	U32																		        TimeStamp;
    U8                                                                              PortMode;
    union
    {
        U32																			SDH				[WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS]
                                                                                                    [WPX_UFE_FRAMER_BUILD_OPTION_MAX_J_PER_PORT]
                                                                                                    [WPX_UFE_FRAMER_BUILD_OPTION_MAX_K]
                                                                                                    [WPX_UFE_FRAMER_BUILD_OPTION_MAX_L]
                                                                                                    [WPX_UFE_FRAMER_BUILD_OPTION_MAX_M]
                                                                                                    [WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_LO_PATH];

        U32																			SONET			[WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS]
                                                                                                    [WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT]
                                                                                                    [WPX_UFE_FRAMER_BUILD_OPTION_MAX_V]
                                                                                                    [WPX_UFE_FRAMER_BUILD_OPTION_MAX_W]
                                                                                                    [WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_LO_PATH];
    } Count;

	union
    {
        U8																			SDH				[WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS]
                                                                                                    [WPX_UFE_FRAMER_BUILD_OPTION_MAX_J_PER_PORT]
                                                                                                    [WPX_UFE_FRAMER_BUILD_OPTION_MAX_K]
                                                                                                    [WPX_UFE_FRAMER_BUILD_OPTION_MAX_L]
                                                                                                    [WPX_UFE_FRAMER_BUILD_OPTION_MAX_M]
                                                                                                    [WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_LO_PATH];

        U8																			SONET			[WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS]
                                                                                                    [WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT]
                                                                                                    [WPX_UFE_FRAMER_BUILD_OPTION_MAX_V]
                                                                                                    [WPX_UFE_FRAMER_BUILD_OPTION_MAX_W]
                                                                                                    [WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_LO_PATH];
    } IsMonitored;

	U32																		        Firewall_BOT;

} WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_LO_PATH_ENDPOINT_TYPE;





#define PM_PDH_ARRAY_SIZE	(WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS*WPX_UFE_FRAMER_MAX_PDH_MONITORED_POINTS_PER_PDH_ELEMENT*sizeof(U32))

typedef struct WPX_UFE_FRAMER_PDH_PERFORMANCE_MONITORING_ENDPOINT_TYPE
{
	U32																		        Firewall_TOP;
	U32																		        TimeStamp;

	U32																				E1T1			[WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS]
																									[WPX_UFE_FRAMER_MAX_PDH_MONITORED_POINTS_PER_PDH_ELEMENT];

	U8																				IsMonitored		[WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS]
																									[WPX_UFE_FRAMER_MAX_PDH_MONITORED_POINTS_PER_PDH_ELEMENT];
	U32																		        Firewall_BOT;
} WPX_UFE_FRAMER_PDH_PERFORMANCE_MONITORING_ENDPOINT_TYPE;





typedef struct WPX_UFE_FRAMER_DISCRETE_CLIENT_PERFORMANCE_MONITORING_ENDPOINT_TYPE
{
	U32																		        Firewall_TOP;
	U32																		        TimeStamp;
    U32																				Count			[WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_DISCRETE_CLIENT_PORTS]
                                                                                                    [WPX_UFE_FRAMER_CLIENT_SIDE_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_PORT];
	U32																				IsMonitored		[WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_DISCRETE_CLIENT_PORTS]
                                                                                                    [WPX_UFE_FRAMER_CLIENT_SIDE_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_PORT];
	U32																		        Firewall_BOT;

} WPX_UFE_FRAMER_DISCRETE_CLIENT_PERFORMANCE_MONITORING_ENDPOINT_TYPE;



	



typedef struct WPX_UFE_FRAMER_PERFORMANCE_MONITORING_TABLE_TYPE
{
    WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_PORT_ENDPOINT_TYPE		*		SONET_SDH_PerformanceMonitoringPortTable;
	U8																		        MAX_SONET_SDH_PerformanceMonitoringPortTableElements;
	WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_HO_PATH_ENDPOINT_TYPE	*		SONET_SDH_PerformanceMonitoringHighOrderPathTable;
	U8																		        MAX_SONET_SDH_PerformanceMonitoringHighOrderPathTableElements;
	WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_LO_PATH_ENDPOINT_TYPE	*		SONET_SDH_PerformanceMonitoringLowOrderPathTable;
	U8																		        MAX_SONET_SDH_PerformanceMonitoringLowOrderPathTableElements;

	WPX_UFE_FRAMER_PDH_PERFORMANCE_MONITORING_ENDPOINT_TYPE					*		PDH_PerformanceMonitoringTable;
	U8																		        MAX_PDH_PerformanceMonitoringTableElements;

    WPX_UFE_FRAMER_DISCRETE_CLIENT_PERFORMANCE_MONITORING_ENDPOINT_TYPE		*		DiscreteClientPerformanceMonitoringTable;
	U8																		        MAX_DiscreteClientPerformanceMonitoringTableElements;

} WPX_UFE_FRAMER_PERFORMANCE_MONITORING_TABLE_TYPE;

				



typedef struct WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE
{
	WPX_UFE_FRAMER_PERFORMANCE_MONITORING_TABLE_TYPE								        Device[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES];

} WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE;

				

typedef void (*WPX_UFE_FRAMER_DEVICE_DIAGNOSTIC_ANNOUNCE_TYPE)(U8 iDevice, U8 DiagnosticAlarmCategory, U8 IsAsserted);




typedef struct WPX_UFE_FRAMER_DEVICE_LEVEL_DIAGNOSTIC_TABLE_TYPE
{
    WPX_UFE_FRAMER_DEVICE_DIAGNOSTIC_ANNOUNCE_TYPE              DeviceDiagnostic;
	
    WPX_UFE_FRAMER_DEVICE_READY_ANNOUNCE_TYPE		        DeviceReady;
    WPX_UFE_FRAMER_FIRMWARE_RESTART_ANNOUNCE_TYPE		FirmwareRestart;
    
    WPX_UFE_FRAMER_PEEK_RESPONSE_ANNOUNCE_TYPE		        PeekResponse;
    WPX_UFE_FRAMER_BLOCK_POKE_VERIFY_RESPONSE_ANNOUNCE_TYPE	BlockPokeVerifyResponse;

    WPX_UFE_FRAMER_ALARM_MAP_RESPONSE_ANNOUNCE_TYPE		AlarmMapResponse;

    WPX_UFE_FRAMER_SS_BITS_ANNOUNCE_TYPE                SSBitsGetRxResponse;

} WPX_UFE_FRAMER_DEVICE_LEVEL_DIAGNOSTIC_TABLE_TYPE;



typedef struct WPX_UFE_FRAMER_OMNISPY_NORTHBOUND_VIRTUAL_SCREEN_TABLE_TYPE
{
	void (*NextPage)(void);
	void (*PrevPage)(void);
	void (*Refresh)(void);

} WPX_UFE_FRAMER_OMNISPY_NORTHBOUND_VIRTUAL_SCREEN_TABLE_TYPE;

typedef void (*WPX_UFE_FRAMER_OMNISPY_NORTHBOUND_ANNOUNCE_TYPE)(U8 iDevice, U32 * pMessage, U32 Length);

typedef struct WPX_UFE_FRAMER_OMNISPY_NORTHBOUND_INTEGRITY_TYPE
{
	void (*Integrity)(void);

} WPX_UFE_FRAMER_OMNISPY_NORTHBOUND_INTEGRITY_TYPE;

typedef struct WPX_UFE_FRAMER_OMNISPY_TABLE_TYPE
{
    WPX_UFE_FRAMER_OMNISPY_NORTHBOUND_ANNOUNCE_TYPE                                                     OmniSpyNorthBoundAnnounce;
	WPX_UFE_FRAMER_OMNISPY_NORTHBOUND_VIRTUAL_SCREEN_TABLE_TYPE											VirtualScreen;
	WPX_UFE_FRAMER_OMNISPY_NORTHBOUND_INTEGRITY_TYPE													PreProductInitialization;

} WPX_UFE_FRAMER_OMNISPY_TABLE_TYPE;



/*
 *
 * Section: Initialization 
 *
 */


void InitializationProcedure(void);

void WPX_UFE_FRAMER_Initialize(	WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE							(*pMailboxMemoryMap)[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES], 
								WPX_UFE_FRAMER_ENVIRONMENT_TABLE_TYPE					*		pEnvironmentTable,
								WPX_UFE_FRAMER_AUTONOMOUS_OUTPUT_TABLE_TYPE				*		pAutonomousOutputTable,
								WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE	*		pPerformanceMonitoringDeviceTable,
                                WPX_UFE_FRAMER_DEVICE_LEVEL_DIAGNOSTIC_TABLE_TYPE       *       pDeviceLevelDiagnosticTable,
                                WPX_UFE_FRAMER_OMNISPY_TABLE_TYPE                       *       pOmniSpyTable,
								U8																TestMode);






/*
 *
 * Section: Low Level Comms Permission API
 *
 */

U8 WPX_FRMR_LowLevelCommsDisableAccess(U8 iDevice);
U8 WPX_FRMR_LowLevelCommsEnableAccess(U8 iDevice);






/*
 *
 * Section: OmniSpy API
 *
 */

U8 WPX_FRMR_OMNISPY_SouthboundMessage(U8 iDevice, U32 * pMessage, U8 Length);





/*
 *
 * Section: DEVICE 
 *
 */



void WPX_UFE_FRAMER_InterruptServiceRequest(U8 iDevice);

U8  WPX_FRMR_ServiceNorthboundLowLevelComms(U8 iDevice);

U8  WPX_UFE_FRAMER_DeviceIsReady(U8 iDevice);

/*
 *
 * Section: CONFIGURATION DEVICE
 *
 */

U8 WPX_FRMR_DEV_GetBuildPersonality(U8 iDevice, U8 * pBuildPersonality);
U8 WPX_FRMR_DEV_SetBuildPersonality(U8 iDevice, U8 BuildPersonality);



U8 WPX_FRMR_SetDevProtectMode(U32 TransactionIdentifier, U8 iDevice, U8 DeviceProtectionMode);
U8 WPX_FRMR_GetDevProtectMode(U8 iDevice, U8 * pDeviceProtectionMode);

U8 WPX_FRMR_SetDevCardProtectOperatingMode(U32 TransactionIdentifier, U8 iDevice, U8 CardProtectionOperatingMode);
U8 WPX_FRMR_GetDevCardProtectOperatingMode(U8 iDevice, U8 * pCardProtectionOperatingMode);



U8 WPX_FRMR_DEV_SetMode(U32 TransactionIdentifier, U8 iDevice, U8 DeviceMode);
U8 WPX_FRMR_DEV_GetMode(U8 iDevice, U8 * pDeviceMode);


U8 WPX_FRMR_DEV_SetFrameAlignmentOffset(U32 TransactionIdentifier, U8 iDevice, U16 FrameAlignmentOffset);
U8 WPX_FRMR_DEV_GetFrameAlignmentOffset(U8 iDevice, U16 * pFrameAlignmentOffset);

U8 WPX_FRMR_DEV_Enable_AIS_Insertion(U32 TransactionIdentifier, U8 iDevice);
U8 WPX_FRMR_DEV_Disable_AIS_Insertion(U32 TransactionIdentifier, U8 iDevice);
U8 WPX_FRMR_DEV_Get_AIS_Insertion(U8 iDevice, U8 * pAIS_InsertionMode);


/*
 *
 * Section: CONFIGURATION PERFORMANCE MONITORING
 *
 */

U8 WPX_FRMR_PM_SONET_SDH_Port_EnableMonitoring(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 iPoint);
U8 WPX_FRMR_PM_SONET_SDH_Port_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 iPoint);
U8 WPX_FRMR_PM_SONET_SDH_HO_Path_EnableMonitoring(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 iPoint);
U8 WPX_FRMR_PM_SONET_SDH_HO_Path_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 iPoint);
U8 WPX_FRMR_PM_SONET_SDH_LO_Path_EnableMonitoring(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 iPoint);
U8 WPX_FRMR_PM_SONET_SDH_LO_Path_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 iPoint);
U8 WPX_UFE_FRAMER_CONFIGURATION_PERFORMANCE_MONITORING_DiscretePort_EnableMonitoring(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 iPoint);
U8 WPX_UFE_FRAMER_CONFIGURATION_PERFORMANCE_MONITORING_DiscretePort_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 iPoint);


U8 WPX_UFE_FRAMER_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_GetMonitoringState(U8 iDevice, U8 iLinePort, U8 iPoint, U8 *pState);
U8 WPX_UFE_FRAMER_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_GetMonitoringState(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 iPoint, U8 *pState);
U8 WPX_UFE_FRAMER_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_GetMonitoringState(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 iPoint, U8 *pState);
U8 WPX_UFE_FRAMER_CONFIGURATION_PERFORMANCE_MONITORING_DiscretePort_GetMonitoringState(U8 iDevice, U8 iDiscreteClientPort, U8 iPoint, U8 *pState);



U8 WPX_FRMR_PM_E1_EnableMonitoringCrc(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH);
U8 WPX_FRMR_PM_E1_DisableMonitoringCrc(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH);
U8 WPX_FRMR_PM_T1_EnableMonitoringCrc(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH);
U8 WPX_FRMR_PM_T1_DisableMonitoringCrc(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH); 
U8 WPX_FRMR_PM_E3T3_EnableMonitoring(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 iPoint);
U8 WPX_FRMR_PM_E3T3_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 iPoint);

U8 WPX_FRMR_PM_E1_GetMonitoringCrc(U8 iDevice, U32 iSocketClientPDH, U8 *pState);
U8 WPX_FRMR_PM_T1_GetMonitoringCrc(U8 iDevice, U32 iSocketClientPDH, U8 *pState);
U8 WPX_FRMR_PM_E3T3_GetMonitoring(U8 iDevice, U32 iSocketClientPDH, U8 iPoint, U8 *pState);



/*
 *
 * Section: CONFIGURATION FACILITIES
 *
 */

U8 WPX_FRMR_SONET_SDH_CreateFacility(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType);

U8 WPX_FRMR_SOCKET_PDH_CreateFacility(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 ClientType);

U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_CreateFacility(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 ClientType);



U8 WPX_FRMR_SONET_SDH_DeleteFacility(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE * pLineEndpoint);

U8 WPX_FRMR_SOCKET_PDH_DeleteFacility(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH);

U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DeleteFacility(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort);



/*
 *
 * Section: CONFIGURATION FACILITIES SONET/SDH PORT
 *
 */


U8 WPX_FRMR_SONET_SDH_PORT_SetRate(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 PortRate);
U8 WPX_FRMR_SONET_SDH_PORT_GetRate(U8 iDevice, U8 iLinePort, U8 * pPortRate);

U8 WPX_FRMR_SONET_SDH_PORT_SetRate_Without_Tx_Enable(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 PortRate);

U8 WPX_FRMR_SONET_SDH_PORT_SFP_SetTxState(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort,U8 Tx_State);

U8 WPX_FRMR_DEV_DIAG_DRV_RecClkGet(U8 iDevice, U8 OutputSelector, U8 * piLinePort);
U8 WPX_FRMR_DEV_DIAG_DRV_RecClkSet(U32 TransactionIdentifier, U8 iDevice, U8 OutputSelector, U8 iLinePort);

U8 WPX_FRMR_DEV_Squelch_RecClkGet(U8 iDevice, U8 OutputSelector, U8* pSquelch_Mode);
U8 WPX_FRMR_DEV_Squelch_RecClkSet(U32 TransactionIdentifier, U8 iDevice, U8 OutputSelector, U8 Squelch_Mode);



U8 WPX_FRMR_SONET_SDH_PORT_DCC_Enable(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 DCC_Mode);
U8 WPX_FRMR_SONET_SDH_PORT_DCC_Disable(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 DCC_Mode);
U8 WPX_FRMR_SONET_SDH_PORT_DCC_Get(U8 iDevice, U8 iLinePort, U8 DCC_Mode, U8 * pDCC_State);





/*
 *
 * Section: PORT PROTECTION
 *
 */

U8 WPX_FRMR_SONET_SDH_PORT_ForceA(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort);
U8 WPX_FRMR_SONET_SDH_PORT_ForceB(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort);
U8 WPX_FRMR_SONET_SDH_PORT_GetForceState(U8 iDevice, U8 iLinePort, U8 * pForceSide);


/*
 *
 * Section: CONFIGURATION FACILITIES SONET/SDH PORT SECTION
 *
 */




U8 WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Mode);
U8 WPX_FRMR_SONET_SDH_PORT_SEC_J0_GetMode(U8 iDevice, U8 iLinePort, U8 * pMode);


U8 WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetTX(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, char * pTX_TraceString);
U8 WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetEX(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, char * pEX_TraceString);

U8 WPX_FRMR_SONET_SDH_PORT_SEC_J0_GetTX(U8 iDevice, U8 iLinePort, char * pTX_TraceString);
U8 WPX_FRMR_SONET_SDH_PORT_SEC_J0_GetEX(U8 iDevice, U8 iLinePort, char * pEX_TraceString);


U8 WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetExcessiveTh(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Threshold);
U8 WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetSignalDegradeTh(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Threshold);

U8 WPX_FRMR_SONET_SDH_PORT_SEC_B1_GetExcessiveTh(U8 iDevice, U8 iLinePort, U8 * pThreshold);
U8 WPX_FRMR_SONET_SDH_PORT_SEC_B1_GetSigDegradeTh(U8 iDevice, U8 iLinePort, U8 * pThreshold);




/*
 *
 * Section: CONFIGURATION FACILITIES SONET/SDH PORT LINE
 *
 */




U8 WPX_FRMR_SONET_SDH_PORT_LINE_K1K2_SetTx(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE * pTX_K1K2);
U8 WPX_FRMR_SONET_SDH_PORT_LINE_K1K2_GetTx(U8 iDevice, U8 iLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE * pTX_K1K2);


U8 WPX_FRMR_SONET_SDH_PORT_LINE_S1_SetTx(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 TX_S1);
U8 WPX_FRMR_SONET_SDH_PORT_LINE_S1_GetTx(U8 iDevice, U8 iLinePort, U8 * pTX_S1);



U8 WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetExcessiveTh(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Threshold);
U8 WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetSigDegradeTh(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Threshold);

U8 WPX_FRMR_SONET_SDH_PORT_LINE_B2_GetExcessiveTh(U8 iDevice, U8 iLinePort, U8 * pThreshold);
U8 WPX_FRMR_SONET_SDH_PORT_LINE_B2_GetSigDegradeTh(U8 iDevice, U8 iLinePort, U8 * pThreshold);






/*
 *
 * Section: CONFIGURATION FACILITIES SONET/SDH HO PATH
 *
 */



U8 WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 Mode);
U8 WPX_FRMR_SONET_SDH_HO_PATH_J1_GetMode(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pMode);


U8 WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, char * pTX_TraceString);
U8 WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, char * pEX_TraceString);

U8 WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, char * pTX_TraceString);
U8 WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, char * pEX_TraceString);

U8 WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 TX_C2);
U8 WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 EX_C2);

U8 WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pTX_C2);
U8 WPX_FRMR_SONET_SDH_HO_PATH_C2_GetEX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pEX_C2);

U8 WPX_FRMR_SONET_SDH_HO_PATH_B3_SetExcessiveTh(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 Threshold);
U8 WPX_FRMR_SONET_SDH_HO_PATH_B3_SetSigDegradeTh(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 Threshold);

U8 WPX_FRMR_SONET_SDH_HO_PATH_B3_GetExcessiveTh(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pThreshold);
U8 WPX_FRMR_SONET_SDH_HO_PATH_B3_GetSigDegradeTh(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pThreshold);

/**************************************************************************************************
*
*   FUNCTION NAME : WPX_FRMR_SONET_SDH_HO_PATH_SetRdiMode
*
*   DESCRIPTION:
*   This function configures the RDI mode for a specific HO facility.
*   the RDI mode will affect two things:
*   1.The value which will be transmitted to the remote end in order to signal HP_RDI
*   2.Which G1 values will raise HP_RDI alarm in the RX
*
*   INPUT PARAMETERS:
*
*       U8 iDevice                  - UFE Framer device number. It identifies which device the API call
*                                     is referring to: [0 - (WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES-1)].
*
*       WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType - The Line Side Endpoint.
*
*       U8* iLinePort               - Used to identify which Line Side port the API  call is referring to.
*       							  available values are 0 to WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS-1
*
*   OUTPUT PARAMETERS:
*       N/A.
*   INPUT/OUTPUT PARAMETERS:
*       N/A.
*
*   RETURN VALUE:
*       Error Code value (U8)[DR - Dynamic Rule; SR - Static Rule]:
*           On Successfully completed operation:
*               (0)             WPX_UFE_FRAMER_OK;
*   NOTES:
*       1. This function should be called by application for every HO facility that been created, after creation.
*       2. By default the RDI mode will be set to ERDI_ENABLE
*
***************************************************************************************************/
U8 WPX_FRMR_SONET_SDH_HO_PATH_SetRdiMode(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 ErdiMode);
/**************************************************************************************************
*
*   FUNCTION NAME : WPX_FRMR_SONET_SDH_HO_PATH_SS_BITS_Set_Tx
*
*   DESCRIPTION:
*   This function configures the Value of the transmited SS bits 
*   The possible values are: 0x0,0x1,0x2 and 0x3 

******************************************************************************************************/

U8 WPX_FRMR_SONET_SDH_HO_PATH_SS_BITS_Set_Tx(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 Value);

/**************************************************************************************************
*
*   FUNCTION NAME : WPX_FRMR_SONET_SDH_HO_PATH_SS_BITS_Set_Tx
*
*   DESCRIPTION:
*   This function returns the received SS_BITS for a specific HO facility 
*   The value is returned via preconfigured Callback function which will be voked in a response to a call to this API 
*   The possible received values are: 0x0,0x1,0x2 and 0x3 

******************************************************************************************************/
U8 WPX_FRMR_SONET_SDH_HO_PATH_SS_BITS_Get_Rx(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType);

/*
 *
 * Section: CONFIGURATION FACILITIES SONET/SDH LO PATH
 *
 */


U8 WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, char * pTX_TraceString);
U8 WPX_FRMR_SONET_SDH_LO_PATH_J2_SetEX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, char * pEX_TraceString);

U8 WPX_FRMR_SONET_SDH_LO_PATH_J2_GetTX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, char * pTX_TraceString);
U8 WPX_FRMR_SONET_SDH_LO_PATH_J2_GetEX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, char * pEX_TraceString);



U8 WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetTX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 TX_V5_SignalLabel);
U8 WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetEX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 EX_V5_SignalLabel);

U8 WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetTX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pTX_V5_SignalLabel);
U8 WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetEX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pEX_V5_SignalLabel);

U8 WPX_FRMR_SONET_SDH_LO_PATH_V5_BIP2_SetExcessiveTh(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 Threshold);
U8 WPX_FRMR_SONET_SDH_LO_PATH_V5_BIP2_SetSigDegradeTh(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 Threshold);

U8 WPX_FRMR_SONET_SDH_LO_PATH_V5_BIP2_GetExcessiveTh(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pThreshold);
U8 WPX_FRMR_SONET_SDH_LO_PATH_V5_BIP2_GetSigDegradeTh(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pThreshold);




/*
 *
 * Section: CONFIGURATION FACILITIES SOCKET CLIENT PDH
 *
 */



U8 WPX_FRMR_SOCKET_PDH_EnableTimeStamp(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U32 Gain);
U8 WPX_FRMR_SOCKET_PDH_DisableTimeStamp(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH);
U8 WPX_FRMR_SOCKET_PDH_GetTimeStamp(U8 iDevice, U32 iSocketClientPDH, U8 * pTimeStamping_IsEnabled);

U8 WPX_FRMR_SOCKET_PDH_SetTimestampByteCnt(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 AnyTimestampByteCount);
U8 WPX_FRMR_SOCKET_PDH_GetTimestampByteCnt(U8 iDevice, U32 iSocketClientPDH, U8 * pAnyTimestampByteCount);

U8 WPX_FRMR_SOCKET_PDH_GetClientType(U8 iDevice, U32 iSocketClientPDH, U8 * pClientType);

U8 WPX_FRMR_SOCKET_PDH_SetFraming(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 Framing);
U8 WPX_FRMR_SOCKET_PDH_GetFraming(U8 iDevice, U32 iSocketClientPDH, U8 * pFraming);

U8 WPX_FRMR_SOCKET_PDH_SetMapping(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 Mapping);
U8 WPX_FRMR_SOCKET_PDH_GetMapping(U8 iDevice, U32 iSocketClientPDH, U8 * pMapping);

U8 WPX_FRMR_SOCKET_PDH_SetClkRec(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 ClockRecoveryMode);
U8 WPX_FRMR_SOCKET_PDH_GetClkRec(U8 iDevice, U32 iSocketClientPDH, U8 * pClockRecoveryMode);
U8 WPX_FRMR_SOCKET_PDH_E3T3_SetFEAC(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 Code);

U8 WPX_FRMR_SOCKET_PDH_EnableCAS(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH);
U8 WPX_FRMR_SOCKET_PDH_DisableCAS(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH);
U8 WPX_FRMR_SOCKET_PDH_Get_CAS_State(U8 iDevice, U32 iSocketClientPDH, U8 * pCAS_IsEnabled);



U8 WPX_FRMR_SOCKET_PDH_AIS_EnableTowardsSocket(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH);
U8 WPX_FRMR_SOCKET_PDH_AIS_DisableTowardsSocket(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH);
U8 WPX_FRMR_SOCKET_PDH_AIS_GetTowardsSocketState(U8 iDevice, U32 iSocketClientPDH, U8 * pAIS_IsEnabled);

U8 WPX_FRMR_SOCKET_PDH_AIS_EnableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH);
U8 WPX_FRMR_SOCKET_PDH_AIS_DisableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH);
U8 WPX_FRMR_SOCKET_PDH_AIS_GetTowardsLineState(U8 iDevice, U32 iSocketClientPDH, U8 * pAIS_IsEnabled);

U8 WPX_FRMR_SOCKET_PDH_RDI_EnableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH);
U8 WPX_FRMR_SOCKET_PDH_RDI_DisableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH);
U8 WPX_FRMR_SOCKET_PDH_RDI_GetTowardsLineState(U8 iDevice, U32 iSocketClientPDH, U8 * pRDI_IsEnabled);




U8 WPX_FRMR_SOCKET_PDH_DIAG_IngressTermPrbsSet(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 PRBS_Pattern);
U8 WPX_FRMR_SOCKET_PDH_DIAG_IngressGenPrbsSet(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 PRBS_Pattern);
U8 WPX_FRMR_SOCKET_PDH_DIAG_IngressTermPrbsGet(U8 iDevice, U32 iSocketClientPDH, U8 * pPRBS_Pattern);
U8 WPX_FRMR_SOCKET_PDH_DIAG_IngressGenPrbsGet(U8 iDevice, U32 iSocketClientPDH, U8 * pPRBS_Pattern);

U8 WPX_FRMR_SOCKET_PDH_DIAG_EgressTermPrbsSet(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 PRBS_Pattern);
U8 WPX_FRMR_SOCKET_PDH_DIAG_EgressGenPrbs_Set(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 PRBS_Pattern);
U8 WPX_FRMR_SOCKET_PDH_DIAG_EgressTermPrbsGet(U8 iDevice, U32 iSocketClientPDH, U8 * pPRBS_Pattern);
U8 WPX_FRMR_SOCKET_PDH_DIAG_EgressGenPrbs_Get(U8 iDevice, U32 iSocketClientPDH, U8 * pPRBS_Pattern);


U8 WPX_FRMR_SOCKET_PDH_DIAG_DRV_RecClkSet(U32 TransactionIdentifier, U8 iDevice, U8 OutputSelector, U32 iSocketClientPDH, U32 ClockRate);
U8 WPX_FRMR_SOCKET_PDH_DIAG_DRV_RecClkGet(U8 iDevice, U8 OutputSelector, U32 * piSocketClientPDH);








/*
 *
 * Section: CONFIGURATION FACILITIES DISCRETE CLIENT
 *
 */




U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GetClientType(U8 iDevice, U8 iDiscreteClientPort, U8 * pClientType);

U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetFraming(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 Framing);
U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GetFraming(U8 iDevice, U8 iDiscreteClientPort, U8 * pFraming);

U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetMapping(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 Mapping);
U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GetMapping(U8 iDevice, U8 iDiscreteClientPort, U8 * pMapping);

U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetClockRecovery(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 ClockRecoveryMode);
U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GetClockRecovery(U8 iDevice, U8 iDiscreteClientPort, U8 * pClockRecoveryMode);

U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_EnableCAS(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort);
U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DisableCAS(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort);
U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_Get_CAS_State(U8 iDevice, U8 iDiscreteClientPort, U8 * pCAS_IsEnabled);



U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_EnableTowardsSocket(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort);
U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DisableTowardsSocket(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort);
U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_GetTowardsSocketState(U8 iDevice, U8 iDiscreteClientPort, U8 * pAIS_IsEnabled);

U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_EnableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort);
U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DisableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort);
U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_GetTowardsLineState(U8 iDevice, U8 iDiscreteClientPort, U8 * pAIS_IsEnabled);







/*
 *
 * Section: CONNECTIONS FUNCTIONS
 *
 */




U8 WPX_FRMR_CONNECTIONS_ThroughCreate(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_FirstLineEndpointType, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_SecondLineEndpointType);
U8 WPX_FRMR_CONNECTIONS_ThroughDelete(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_FirstLineEndpointType, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_SecondLineEndpointType);


U8 WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U32 iSocketClientPDH);
U8 WPX_FRMR_CONNECTIONS_AddDropLineToSocketDelete(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U32 iSocketClientPDH);

U8 WPX_UFE_FRAMER_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_CreateConnection(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U32 iSocketClientPDH);
U8 WPX_UFE_FRAMER_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_DeleteConnection(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U32 iSocketClientPDH);





/*
 *
 * Section: CONFIGURATION FACILITIES DIAGNOSTIC LOOPBACK
 *
 */


U8 WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableShallowLine(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort);
U8 WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableShallowLine(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort);
U8 WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkGetShallowLineState(U8 iDevice, U8 iLinePort, U8 * pLoopbackIsEnabled);

U8 WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableDeepSystem(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort);
U8 WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableDeepSystem(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort);
U8 WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkGetDeepSystemState(U8 iDevice, U8 iLinePort, U8 * pLoopbackIsEnabled);


U8 WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableShallowSystem(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH);
U8 WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableShallowSystem(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH);
U8 WPX_FRMR_SOCKET_PDH_DIAG_LpbkGetShallowSystemState(U8 iDevice, U32 iSocketClientPDH, U8 * pLoopbackIsEnabled);

U8 WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableDeepLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH);
U8 WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableDeepLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH);
U8 WPX_FRMR_SOCKET_PDH_DIAG_LpbkGetDeepLineState(U8 iDevice, U32 iSocketClientPDH, U8 * pLoopbackIsEnabled);


U8 WPX_FRMR_SONET_SDH_DIAG_LpbkEnable(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType);
U8 WPX_FRMR_SONET_SDH_DIAG_LpbkDisable(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType);
U8 WPX_FRMR_SONET_SDH_DIAG_LpbkGetState(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pLoopbackIsEnabled);




/*
 *
 * Section: ALARM REPORTING
 *
 */


U8 WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm(U32 TransactionIdentifier, U8 AlarmCategory);
U8 WPX_FRMR_DEV_DRV_SONET_SDH_DisableAlarm(U32 TransactionIdentifier, U8 AlarmCategory);
U8 WPX_FRMR_DEV_DRV_SONET_SDH_GetAlarm(U8 AlarmCategory, U8 * pIsEnabled);

U8 WPX_FRMR_DEV_DRV_SOCKET_EnableAlarm(U32 TransactionIdentifier, U8 AlarmCategory);
U8 WPX_FRMR_DEV_DRV_SOCKET_DisableAlarm(U32 TransactionIdentifier, U8 AlarmCategory);
U8 WPX_FRMR_DEV_DRV_SOCKET_GetAlarm(U8 AlarmCategory, U8 * pIsEnabled);

U8 WPX_UFE_FRAMER_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_EnableAlarmReportingForAlarmCategory(U32 TransactionIdentifier, U8 AlarmCategory);
U8 WPX_UFE_FRAMER_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_DisableAlarmReportingForAlarmCategory(U32 TransactionIdentifier, U8 AlarmCategory);
U8 WPX_UFE_FRAMER_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_GetAlarmReportingForAlarmCategory(U8 AlarmCategory, U8 * pIsEnabled);






/*
 *
 * Section: STATUS DEVICE DRIVER
 *
 */


U8 WPX_FRMR_STATUS_DEV_DRV_VERSION_GetInfo(char *pDeviceDriverInformation);


/*
 *
 * Section: STATUS DEVICE
 *
 */


U8 WPX_FRMR_STATUS_DEV_VERSION_GetFirmwareInfo(U8 iDevice, char *pFirmwareInformation);
U8 WPX_FRMR_STATUS_DEV_VERSION_GetHardwareInfo(U8 iDevice, char *pHardwareInformation);
U8 WPX_UFE_FRAMER_STATUS_DEVICE_BUILD_GetHardwareVariant(U8 iDevice, U8 * pHardwareBuildPersonality);



/*
 *
 * Section: STATUS FACILITIES SONET/SDH PORT SECTION
 *
 */


U8 WPX_FRMR_STATUS_SONET_SDH_PORT_SEC_J0_GetRX(U8 iDevice, U8 iLinePort, char * pRX_TraceString);



/*
 *
 * Section: STATUS FACILITIES SONET/SDH PORT LINE
 *
 */

U8 WPX_FRMR_STATUS_SONET_SDH_PORT_LINE_S1_GetRX(U8 iDevice, U8 iLinePort, U8 * pRX_S1);

U8 WPX_FRMR_STATUS_SONET_SDH_PORT_LINE_K1K2_GetRX(U8 iDevice, U8 iLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE *pRX_K1K2);



/*
 *
 * Section: STATUS FACILITIES SONET/SDH HO PATH
 *
 */

U8 WPX_FRMR_STATUS_SONET_SDH_HO_PATH_J1_GetRX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, char * pRX_TraceString);

U8 WPX_FRMR_STATUS_SONET_SDH_HO_PATH_C2_GetRX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pRX_C2);



/*
 *
 * Section: STATUS FACILITIES SONET/SDH LO PATH
 *
 */

U8 WPX_FRMR_STATUS_SONET_SDH_LO_PATH_J2_GetRX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, char * pRX_TraceString);

U8 WPX_FRMR_STATUS_SONET_SDH_LO_PATH_V5_SigLabelGetRX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pRX_V5_SignalLabel);


/*
 *
 * Section: STATUS DIAGNOSTICS
 *
 */


U8 WPX_FRMR_STATUS_SOCKET_PDH_DIAG_IngressPrbsGetState(U8 iDevice, U32 iSocketClientPDH, U8 * pPRBS_State);
U8 WPX_FRMR_STATUS_SOCKET_PDH_DIAG_EgressPrbsGetState(U8 iDevice, U32 iSocketClientPDH, U8 * pPRBS_State);









/*
 *
 * Section: STATUS DIAGNOSTICS
 *
 */


U8 WPX_FRMR_STATUS_PM_SONET_SDH_Port_ReadData(U8 iDevice, U8 iTableIndex, U8 iLinePort, U8 iPoint, U32 * pCount);
U8 WPX_FRMR_STATUS_PM_SONET_SDH_HO_PATH_ReadData(U8 iDevice, U8 iTableIndex, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 iPoint, U32 * pCount);
U8 WPX_FRMR_STATUS_PM_SONET_SDH_LO_PATH_ReadData(U8 iDevice, U8 iTableIndex, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 iPoint, U32 * pCount);
U8 WPX_UFE_FRAMER_STATUS_PERFORMANCE_MONITORING_DiscreteClientPort_ReadData(U8 iDevice, U8 iTableIndex, U8 iDiscreteClientPort, U8 iPoint, U32 * pCount);
U8 WPX_FRMR_STATUS_PM_SocketPort_PDH_E1T1_ReadData(U8 iDevice, U8 iTableIndex, U32 iSocketClientPDH, U8 iPoint, U32 * pCount);
U8 WPX_FRMR_STATUS_PM_SocketPort_PDH_E3T3_ReadData(U8 iDevice, U8 iTableIndex, U32 iSocketClientPDH, U8 iPoint, U32 * pCount);




U8 WPX_FRMR_DEV_DIAG_Poke(U32 TransactionIdentifier, U8 iDevice, U32 Address, U32 Value);
U8 WPX_FRMR_DEV_DIAG_Peek(U8 iDevice, U32 Address);
U8 WPX_FRMR_DEV_DIAG_BlockPoke(U32 TransactionIdentifier, U8 iDevice, U32 StartAddress, U32 NumberOfU32AddressesToBeWritten, U32 Value);
U8 WPX_FRMR_DEV_DIAG_BlockPokeVerify(U32 TransactionIdentifier, U8 iDevice, U32 StartAddress, U32 NumberOfU32AddressesToBeWritten, U32 Value);
/* NOTE Peek Response returned via callback WPX_UFE_FRAMER_PEEK_RESPONSE_ANNOUNCE_TYPE PeekResponse */


U8 WPX_FRMR_ReportDeviceAlarmMap(U8 iDevice);
/* NOTE AlarmMap termination indicated via callback if type WPX_UFE_FRAMER_ALARM_MAP_RESPONSE_ANNOUNCE_TYPE */



/*
 *
 * Section: Lab Test and Demo
 *
 */



/* This API needs to be called for the DEMO BOARD ONLY - after set build personality */ 
void WPX_FRMR_DEMO_ONLY_Configure_GTP_ClockSource(U8 iDevice);

/* Initialize the UFE Flexmux framer (To be called from framer application) */ 
U32 WPX_UFE_FRAMER_FlexmuxInit(U32 flexmux_id, U32 flexmux_connector_id);
U32 WPX_UFE_FRAMER_FlexmuxInit_TestMode(U32 flexmux_id, U32 flexmux_connector_id);
   
/* Set the UFE Flexmux framer build personality (To be called from framer application) */ 
U32 WPX_UFE_FRAMER_FlexmuxSetBuildPersonality(U32 flexmux_id, U32 flexmux_hw_config, U32 flexmux_connector_id);
      
/* Service the mailboxes (To be called from framer application) */
void WPX_UFE_FRAMER_FlexmuxServiceMailbox(void *params);

void WPX_UFE_FRAMER_MIPS_StartDriver(int AnyPersonality);
void WPX_UFE_FRAMER_MIPS_StartDriver_TestMode(int AnyPersonality);

/*---------------------------- GENERALIZED ALARM HANDLING -----------------------------------------*/

void WPX_FRMR_RegisterSonetSdhPortAlarmCallback(void *fnc);
void WPX_FRMR_RegisterSonetSdhPathAlarmCallback(void *fnc);
void WPX_FRMR_RegisterDiscreteClientPortAlarmCallback(void *fnc);
void WPX_FRMR_RegisterSocketClientPdhAlarmCallback(void *fnc);

/*---------------------------- GENERALIZED ALARM HANDLING -----------------------------------------*/

/*---------------------------- GENERALIZED PERFORMANCE MONITORING HANDLING ------------------------*/

void WPX_FRMR_PM_RegisterSonetSdhPortAnnounceCallback(void *fnc);
void WPX_FRMR_PM_RegisterSonetSdhHOPathAnnounceCallback(void *fnc);
void WPX_FRMR_PM_RegisterSonetSdhLOPathAnnounceCallback(void *fnc);
void WPX_FRMR_PM_RegisterSocketClientPdhAnnounceCallback(void *fnc);
void WPX_FRMR_RegisterDiscreteClientPortAnnounceCallback(void *fnc);

/*---------------------------- GENERALIZED PERFORMANCE MONITORING HANDLING ------------------------*/

/*---------------------------- GENERALIZED SECTION SIGNALLING HANDLING ----------------------------*/

void WPX_FRMR_RegisterSonetSdhSectionK1K2AnnounceCallback(void *fnc);
void WPX_FRMR_RegisterSonetSdhSectionS1AnnounceCallback(void *fnc);

/*---------------------------- GENERALIZED SECTION SIGNALLING HANDLING ----------------------------*/

/*---------------------Peek Function Register----------------------------*/
void WPX_FRMR_RegisterPeekCallback(void *fnc);
void WPX_FRMR_RegisterBlockPokeVerifyCallback(void *fnc);
void WPX_FRMR_RegisterAlarmMapCallback(void *fnc);
void WPX_FRMR_RegisterFramerLockCallback(void *fnc);


#ifdef __cplusplus
}
#endif


#endif /* _WPX_UFE_FRAMER_API_PUBLIC_ */
