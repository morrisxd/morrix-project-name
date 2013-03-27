#ifndef  _WPX_UFE_FRAMER_API_PORTING_
#define  _WPX_UFE_FRAMER_API_PORTING_


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



/*
 *
 * Section: PORTABILITY
 *
 */

    
#include <stdio.h>
#include <string.h>



#define U8		unsigned char
#define U16		unsigned short
#define U32		unsigned int

#define WPX_UFE_FRAMER_WP_U8   U8




#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif


    typedef enum
{/* Order of bandwidth do not change !*/
   WPX_UFE_FRAMER_WUFE_CAD_TYPE_T1           =0,
   WPX_UFE_FRAMER_WUFE_CAD_TYPE_E1,
   WPX_UFE_FRAMER_WUFE_CAD_TYPE_HMVIP_4_T1,
   WPX_UFE_FRAMER_WUFE_CAD_TYPE_HMVIP_4_E1,
   WPX_UFE_FRAMER_WUFE_CAD_TYPE_E3,
   WPX_UFE_FRAMER_WUFE_CAD_TYPE_DS3,

   WPX_UFE_FRAMER_WUFE_SBI_TYPE_T1,
   WPX_UFE_FRAMER_WUFE_SBI_TYPE_E1,
   WPX_UFE_FRAMER_WUFE_SBI_TYPE_DS3,
   WPX_UFE_FRAMER_WUFE_SBI_TYPE_E3,
   WPX_UFE_FRAMER_WUFE_SBI_TYPE_IDLE, /* not specified */

   WPX_UFE_FRAMER_WUFE_GENERIC_TYPE_T1,
   WPX_UFE_FRAMER_WUFE_GENERIC_TYPE_E1,
   WPX_UFE_FRAMER_WUFE_GENERIC_TYPE_DS3,
   WPX_UFE_FRAMER_WUFE_GENERIC_TYPE_E3,

   WPX_UFE_FRAMER_WUFE_SDH_TYPE_T1,
   WPX_UFE_FRAMER_WUFE_SDH_TYPE_E1,
   WPX_UFE_FRAMER_WUFE_SDH_TYPE_DS3,
   WPX_UFE_FRAMER_WUFE_SDH_TYPE_E3,
   WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC11,
   WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC12,
   WPX_UFE_FRAMER_WUFE_SDH_TYPE_TU3,
   WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3,
   WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4,
   WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4_4C,
   WPX_UFE_FRAMER_WUFE_SDH_TYPE_VCAT,

   WPX_UFE_FRAMER_WUFE_SONET_TYPE_T1,
   WPX_UFE_FRAMER_WUFE_SONET_TYPE_E1,
   WPX_UFE_FRAMER_WUFE_SONET_TYPE_DS3,
   WPX_UFE_FRAMER_WUFE_SONET_TYPE_E3,
   WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT1_5,
   WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT2,
   WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS1_SPE,
   WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS3,
   WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS12,
   WPX_UFE_FRAMER_WUFE_SONET_TYPE_VCAT

}WPX_UFE_FRAMER_WUFE_line_transf_type;





typedef struct SDH_TYPE
{
    WPX_UFE_FRAMER_WP_U8 stm4;
    WPX_UFE_FRAMER_WP_U8 stm1;
    WPX_UFE_FRAMER_WP_U8 stm0;
    WPX_UFE_FRAMER_WP_U8 tug2;
    WPX_UFE_FRAMER_WP_U8 tu;

} SDH_TYPE;



typedef struct SONET_TYPE
{
    WPX_UFE_FRAMER_WP_U8 sts12;
    WPX_UFE_FRAMER_WP_U8 sts3;
    WPX_UFE_FRAMER_WP_U8 sts1;
    WPX_UFE_FRAMER_WP_U8 vt_group;
    WPX_UFE_FRAMER_WP_U8 vt;

} SONET_TYPE;

typedef struct WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE
{
    WPX_UFE_FRAMER_WUFE_line_transf_type TransferType;

    union
    {
        SDH_TYPE        SDH;
        SONET_TYPE      SONET;
    } u;

} WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE;



#ifdef __cplusplus
}
#endif


#endif /* _WPX_UFE_FRAMER_API_PORTING_ */
