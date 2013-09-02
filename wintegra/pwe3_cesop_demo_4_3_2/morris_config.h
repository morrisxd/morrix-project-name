#ifndef __MORRIS_COFIG_H__
#define __MORRIS_COFIG_H__

#define MORRIS_CR_ON	(1)

#define USE_MORRIS_CONFIG	(0)
#define USE_MORRIS_REF_CLOCK	WPX_UFE_FRAMER_TRANSMIT_TIMING_INPUT_REFERENCE_CLOCK


#define MORRIS_ENABLE_PM_PRINT_COUNTER	(1)
#define MORRIS_PM_PORT_PRINT_WITHOUT_COUNTER	(0)
#define MORRIS_PM_COUNT_CLEAN	(1)
#define MORRIS_PRINT_HO_COUNTER	(0)


#define MORRIS_ENABLE_STM_1	(1)
#define MORRIS_SKIP_ALARMS	(1)


#define MORRIS_USE_VC4	(1)
#define MORRIS_DISABLE_MON_STATE	(0)


#define MORRIS_FACTOR	(1)

#define MORRIS_ENABLE_FREERUN_MODE	(0)
#define WTI_CLOCK_REC_SNAKE_ENABLED	(1)
#define MORRIS_ENABLE_PDH_ALRAM_PRINT	(0)

#define MORRIS_ENABLE_LOS_AIS	(1)
#define MORRIS_ENABLE_PATH_ALARM	(0)


#ifndef MORRIS_SNAKE_NUM
#define MORRIS_SNAKE_NUM
WP_U32 cr_snake_num_of_lines = 16;
#endif


#define MORRIS_CHANGE_LEVELING	(1)


#define MORRIS_DISABLE_ERROR_PRINT_WHEN_STM_1	(0)



#define MORRIS_WDDI_CHANGE	"3rd change: in wddi/sources/include/core/features/wpi_adv_user.h #define WPI_CLOCK_REC_PPM_DELTA_ALLOWED             (50) youll need to recomile WDDI ....  "





#endif


