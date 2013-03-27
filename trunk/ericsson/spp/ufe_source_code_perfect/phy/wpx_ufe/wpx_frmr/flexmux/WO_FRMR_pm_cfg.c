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



void OMIINO_FRAMER_PerformanceMonitoring_M_of_N_SetCurrent(OMIINO_FRAMER_PERFORMANCE_MONITORING_M_OF_N_TYPE * pM_of_N)
{
	pM_of_N->Current=0;
}


void OMIINO_FRAMER_PerformanceMonitoring_M_of_N_SetLimit(OMIINO_FRAMER_PERFORMANCE_MONITORING_M_OF_N_TYPE * pM_of_N, int AnyLimit)
{
	pM_of_N->Limit=AnyLimit;
	pM_of_N->Current=0;
}


void OMIINO_FRAMER_PerformanceMonitoring_M_of_N_Inc(OMIINO_FRAMER_PERFORMANCE_MONITORING_M_OF_N_TYPE * pM_of_N)
{
	pM_of_N->Current++;
}


void OMIINO_FRAMER_PerformanceMonitoring_M_of_N_Dec(OMIINO_FRAMER_PERFORMANCE_MONITORING_M_OF_N_TYPE * pM_of_N)
{
	if(0!=(pM_of_N->Current))
	{
		pM_of_N->Current--;
	}
}


