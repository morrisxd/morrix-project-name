#ifndef __MORRIS_COFIG_H__
#define __MORRIS_COFIG_H__

#define MORRIS_BUILD_TDI	(1)
#define MORRIS_CR_ON	(1)

#define USE_MORRIS_CONFIG	(0)
#define USE_MORRIS_REF_CLOCK	WPX_UFE_FRAMER_TRANSMIT_TIMING_INPUT_REFERENCE_CLOCK
// #define USE_MORRIS_REF_CLOCK	WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH


#define MORRIS_ENABLE_PM_PRINT_COUNTER	(1)
#define MORRIS_PM_PORT_PRINT_WITHOUT_COUNTER	(0)
#define MORRIS_PM_COUNT_CLEAN	(0)
#define MORRIS_PRINT_HO_COUNTER	(0)


#define MORRIS_ENABLE_STM_1	(0)
#define MORRIS_SKIP_ALARMS	(1)


#define MORRIS_USE_VC4	(0)
#define MORRIS_DISABLE_MON_STATE	(0)


#define MORRIS_FACTOR	(1)
#define MORRIS_FACTOR_FOR_SNAKE	(1)

#define MORRIS_ENABLE_FREERUN_MODE	(0)	// disable = looptiming/LOOPTIMING on UFE STM port
#define WTI_CLOCK_REC_SNAKE_ENABLED	(1)
#define MORRIS_ENABLE_PDH_ALRAM_PRINT	(0)

#define MORRIS_ENABLE_LOS_AIS	(0)
#define MORRIS_ENABLE_PATH_ALARM	(0)


#ifndef MORRIS_SNAKE_NUM
#define MORRIS_SNAKE_NUM
WP_U32 cr_snake_num_of_lines = 16;
#endif

#define MORRIS_EXTRA_MENU_ITEM 	(2)
#define MORRIS_PREDEFINED_MENU	(1)

#define MORRIS_CHANGE_LEVELING	(1)
#define MORRIS_CHANGE_LEVELING_FOR_SNAKE	(1)


#define MORRIS_DISABLE_ERROR_PRINT_WHEN_STM_1	(1)

#define MORRIS_DIRECT_FACTOR	(25)
#define MORRIS_INTEGRATION_FACTOR	(28)

/*
 * this offset is for daisy chain.
 */
#define MORRIS_WDDI_CHANGE	"3rd change: in wddi/sources/include/core/features/wpi_adv_user.h #define WPI_CLOCK_REC_PPM_DELTA_ALLOWED             (50) youll need to recomile WDDI ....  "


#define MORRIS_ENABLE_APS	(1)	// can be set=1 forever
#define MORRIS_SET_ALL_TO_HOLDOVER	(1)


////////////////////////////////////////////////////////
#define MORRIS_DISABLE_PORT_ALARM_PRINT	(0)
#define MORRIS_DISABLE_K1K2_PRINT	(0)
#define MORRIS_DISABLE_CURRENT_FORCE_STATE_PRINT	(0)
#define MORRIS_DISABLE_APS_PRINT	(1)
#define MORRIS_ENABLE_ONLY_LOS	(1)

////////////////////////////////////////////////////////
///////////////////// for inter-operation //////////////
////////////////////////////////////////////////////////
#define MORRIS_DISABLE_RTP	(0)
#define MORRIS_DISABLE_L3_HEADER	(1)
#define MORRIS_DISABLE_VLAN_TAG	(1)


#define MORRIS_USE_SMALLEST_BUF_SIZE	(0)
#define MORRIS_CHANGE_RTP_HEADER	(1)

#define MORRIS_USE_1_PW	(0)
#define MORRIS_NEW_PCE 	(0)
// #define MORRIS_ADD_NEW_CALL_FREERUN	(0)

#define MORRIS_USE_ACR	(1)
#define MORRIS_CLI_ENTRY	(1)
#define MORRIS_MPLS_LABEL	(1)
#define MORRIS_USER_CONTROL	(1)

#define MORRIS_USE_ENET		(1)
#define MORRIS_MPLS_INCREAMENT	(10)
#define MORRIS_MPLS_INCREAMENT2	(0)
#define MORRIS_SECOND_RESET	(0)
#define MORRIS_SET_ENET_LOOPBACK	(0)
#define MORRIS_REINIT_FLEXMUX	(1)
#define MORRIS_DISABLE_RELEASE_UFE	(1)
#endif


