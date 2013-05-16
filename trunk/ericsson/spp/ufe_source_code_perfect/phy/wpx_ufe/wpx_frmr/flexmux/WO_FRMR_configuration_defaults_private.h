#ifndef  _OMIINO_FRAMER_CONFIGURATION_DEFAULTS_PRIVATE_
#define  _OMIINO_FRAMER_CONFIGURATION_DEFAULTS_PRIVATE_


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



#include "WO_FRMR_API_public.h"


#define OMIINO_DEFAULT_M_OF_N_PM_MONITORED_POINTS									(1056)  /* 8*3 + 12*2 + 336*2 + 336*1 */

#define OMIINO_DEFAULT_DEVICE_MODE													(WPX_UFE_FRAMER_DEVICE_MODE_SDH)
#define OMIINO_DEFAULT_DEVICE_FRAME_ALIGNMENT_OFFSET								(0x0000)
#define OMIINO_DEFAULT_DEVICE_PROTECTION_MODE										(WPX_UFE_DEVICE_PROTECTION_MODE_LINE)
#define OMIINO_DEFAULT_DEVICE_CARD_PROTECTION_OPERATING_MODE						(WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_NONE)
#define OMIINO_DEFAULT_DEVICE_AIS_INSERTION_STATE									(0)

#define OMIINO_DEFAULT_SONET_SDH_LINE_PORT_MODE										(WPX_UFE_FRAMER_DEVICE_MODE_SDH)
#define OMIINO_DEFAULT_SONET_SDH_LINE_PORT_RATE										(WPX_UFE_FRAMER_LINE_PORT_RATE_OFF)
#define OMIINO_DEFAULT_SONET_SDH_LINE_PORT_SCRAMBLING								(0x01)
#define OMIINO_DEFAULT_SONET_SDH_LINE_PORT_DIAGNOSTIC_LOOPBACK						(0x00)
	

#define OMIINO_DEFAULT_SONET_SDH_LINE_PORT_SECTION_J0_MODE							(WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE)
#define OMIINO_DEFAULT_SONET_SDH_LINE_PORT_SECTION_J0_TX							"WPX_UFE_J0_TX__"
#define OMIINO_DEFAULT_SONET_SDH_LINE_PORT_SECTION_J0_EX							"WPX_UFE_J0_EX__"
#define OMIINO_DEFAULT_SONET_SDH_LINE_PORT_SECTION_J0_RX							"WPX_UFE_J0_RX__"
#define OMIINO_DEFAULT_SONET_SDH_LINE_PORT_SECTION_B1_SIGNAL_DEGRADE_THRESHOLD		(WPX_UFE_FRAMER_THRESHOLD_OFF)
#define OMIINO_DEFAULT_SONET_SDH_LINE_PORT_SECTION_B1_SIGNAL_FAIL_THRESHOLD			(WPX_UFE_FRAMER_THRESHOLD_OFF)


#define OMIINO_DEFAULT_SONET_SDH_LINE_PORT_LINE_K1_TX								(0x00)
#define OMIINO_DEFAULT_SONET_SDH_LINE_PORT_LINE_K2_TX								(0x00)
#define OMIINO_DEFAULT_SONET_SDH_LINE_PORT_LINE_S1_TX								(0x00)
#define OMIINO_DEFAULT_SONET_SDH_LINE_PORT_LINE_K1_RX								(0x00)
#define OMIINO_DEFAULT_SONET_SDH_LINE_PORT_LINE_K2_RX								(0x00)
#define OMIINO_DEFAULT_SONET_SDH_LINE_PORT_LINE_S1_RX								(0x00)
#define OMIINO_DEFAULT_SONET_SDH_LINE_PORT_LINE_B2_SIGNAL_DEGRADE_THRESHOLD			(WPX_UFE_FRAMER_THRESHOLD_OFF)
#define OMIINO_DEFAULT_SONET_SDH_LINE_PORT_LINE_B2_SIGNAL_FAIL_THRESHOLD			(WPX_UFE_FRAMER_THRESHOLD_OFF)


#define OMIINO_DEFAULT_SONET_SDH_HO_PATH_J1_MODE									(WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE)
#define OMIINO_DEFAULT_SONET_SDH_HO_PATH_J1_TX										"WPX_UFE_J1_TX__"
#define OMIINO_DEFAULT_SONET_SDH_HO_PATH_J1_EX										"WPX_UFE_J1_EX__"
#define OMIINO_DEFAULT_SONET_SDH_HO_PATH_J1_RX										"WPX_UFE_J1_RX__"
#define OMIINO_DEFAULT_SONET_SDH_HO_PATH_C2_TX										(0x00)
#define OMIINO_DEFAULT_SONET_SDH_HO_PATH_C2_EX										(0x00)
#define OMIINO_DEFAULT_SONET_SDH_HO_PATH_C2_RX										(0x00)
#define OMIINO_DEFAULT_SONET_SDH_HO_PATH_B3_SIGNAL_DEGRADE_THRESHOLD				(WPX_UFE_FRAMER_THRESHOLD_OFF)
#define OMIINO_DEFAULT_SONET_SDH_HO_PATH_B3_SIGNAL_FAIL_THRESHOLD					(WPX_UFE_FRAMER_THRESHOLD_OFF)



#define OMIINO_DEFAULT_SONET_SDH_LO_PATH_J2_MODE									(WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE)
#define OMIINO_DEFAULT_SONET_SDH_LO_PATH_J2_TX										"WPX_UFE_J2_TX__"
#define OMIINO_DEFAULT_SONET_SDH_LO_PATH_J2_EX										"WPX_UFE_J2_EX__"
#define OMIINO_DEFAULT_SONET_SDH_LO_PATH_J2_RX										"WPX_UFE_J2_RX__"
#define OMIINO_DEFAULT_SONET_SDH_LO_PATH_SIGNAL_LABEL_TX							(0x00)
#define OMIINO_DEFAULT_SONET_SDH_LO_PATH_SIGNAL_LABEL_EX							(0x00)
#define OMIINO_DEFAULT_SONET_SDH_LO_PATH_SIGNAL_LABEL_RX							(0x00)
#define OMIINO_DEFAULT_SONET_SDH_LO_PATH_V5_BIP2_SIGNAL_DEGRADE_THRESHOLD			(WPX_UFE_FRAMER_THRESHOLD_10E7)
#define OMIINO_DEFAULT_SONET_SDH_LO_PATH_V5_BIP2_SIGNAL_FAIL_THRESHOLD				(WPX_UFE_FRAMER_THRESHOLD_10E3)


#define OMIINO_DEFAULT_PRBS_PATTERN													(WPX_UFE_FRAMER_PRBS_DISABLED)


#define OMIINO_DEFAULT_DISCRETE_CLIENT_BANDWIDTH									(WPX_UFE_FRAMER_CLIENT_NULL)
#define OMIINO_DEFAULT_DISCRETE_CLIENT_FRAMING										(WPX_UFE_FRAMER_CLIENT_FRAMING_NULL)
#define OMIINO_DEFAULT_DISCRETE_CLIENT_MAPPING										(WPX_UFE_FRAMER_CLIENT_MAPPING_BULK_FILL)
#define OMIINO_DEFAULT_DISCRETE_CLIENT_CAS											(0)
#define OMIINO_DEFAULT_DISCRETE_CLIENT_CLOCK_RECOVERY								(WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH)
#define OMIINO_DEFAULT_DISCRETE_CLIENT_AIS_CONTROL									(0)
#define OMIINO_DEFAULT_DISCRETE_CLIENT_DIAGNOSTIC_LOOPBACK							(0)
#define OMIINO_DEFAULT_DISCRETE_CLIENT_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK				(0)


#define OMIINO_DEFAULT_SOCKET_CLIENT_PDH_BANDWIDTH									(WPX_UFE_FRAMER_CLIENT_NULL)
#define OMIINO_DEFAULT_SOCKET_CLIENT_PDH_FRAMING									(WPX_UFE_FRAMER_CLIENT_FRAMING_NULL)
#define OMIINO_DEFAULT_SOCKET_CLIENT_PDH_MAPPING									(WPX_UFE_FRAMER_CLIENT_MAPPING_BULK_FILL)
#define OMIINO_DEFAULT_SOCKET_CLIENT_PDH_CAS										(1)
#define OMIINO_DEFAULT_SOCKET_CLIENT_PDH_CLOCK_RECOVERY								(WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH)
#define OMIINO_DEFAULT_SOCKET_CLIENT_PDH_AIS_CONTROL								(0)
#define OMIINO_DEFAULT_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK						(0)
#define OMIINO_DEFAULT_SOCKET_CLIENT_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK				(0)
#define OMIINO_DEFAULT_SOCKET_CLIENT_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_RATE_PARAMETER (0)


#ifdef __cplusplus
}
#endif


#endif /* _OMIINO_FRAMER_CONFIGURATION_DEFAULTS_PRIVATE_ */