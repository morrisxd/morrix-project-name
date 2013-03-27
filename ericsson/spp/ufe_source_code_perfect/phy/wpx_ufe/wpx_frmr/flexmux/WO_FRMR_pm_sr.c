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
/*--------------------------------------------------------------------------*/



#include "WO_FRMR_private.h"


U8 OMIINO_FRAMER_PerformanceMonitoringStaticRule_PortPointInRange(U8 iPoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_PORT<=iPoint)
	{
		Result = WPX_UFE_FRAMER_ERROR_POINT_OUT_OF_RANGE;
	}

	return Result;
}


U8 OMIINO_FRAMER_PerformanceMonitoringStaticRule_HoPointInRange(U8 iPoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_HO_PATH<=iPoint)
	{
		Result = WPX_UFE_FRAMER_ERROR_POINT_OUT_OF_RANGE;
	}

	return Result;
}

U8 OMIINO_FRAMER_PerformanceMonitoringStaticRule_LoPointInRange(U8 iPoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_LO_PATH<=iPoint)
	{
		Result = WPX_UFE_FRAMER_ERROR_POINT_OUT_OF_RANGE;
	}

	return Result;
}



U8 OMIINO_FRAMER_PerformanceMonitoringStaticRule_DiscretePortPointInRange(U8 iPoint)
{
	OMIINO_REMOVE_COMPILER_WARNING(iPoint); /* TODO */

	return WPX_UFE_FRAMER_ERROR_POINT_OUT_OF_RANGE;
}

