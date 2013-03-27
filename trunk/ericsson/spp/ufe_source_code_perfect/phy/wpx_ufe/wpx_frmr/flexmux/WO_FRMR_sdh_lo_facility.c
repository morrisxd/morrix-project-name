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



void OMIINO_FRAMER_SONET_SDH_LO_Facility_Create(OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE *pLO_Path, U8 AnyBandwidth)
{
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST==pLO_Path->Configuration.FacilityState,pLO_Path->Configuration.FacilityState);
    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_MAX_LINE_BANDWIDTH_TYPES>AnyBandwidth,AnyBandwidth);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_LINE_BANDWIDTH_NULL==pLO_Path->Bandwidth.Configuration.Payload,pLO_Path->Bandwidth.Configuration.Payload);

	pLO_Path->Configuration.FacilityState=WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE;
	pLO_Path->Bandwidth.Configuration.Payload=AnyBandwidth;
}


void OMIINO_FRAMER_SONET_SDH_LO_Facility_Delete(OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE *pLO_Path)
{
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE==pLO_Path->Configuration.FacilityState,pLO_Path->Configuration.FacilityState);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_LINE_BANDWIDTH_NULL!=pLO_Path->Bandwidth.Configuration.Payload,pLO_Path->Bandwidth.Configuration.Payload);
    
	pLO_Path->Configuration.FacilityState=WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST;
	pLO_Path->Bandwidth.Configuration.Payload=WPX_UFE_FRAMER_LINE_BANDWIDTH_NULL;
}


void OMIINO_FRAMER_SONET_SDH_LO_Facility_MakeConnected(OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE *pLO_Path)
{
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE==pLO_Path->Configuration.FacilityState,pLO_Path->Configuration.FacilityState);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_LINE_BANDWIDTH_NULL!=pLO_Path->Bandwidth.Configuration.Payload,pLO_Path->Bandwidth.Configuration.Payload);

	pLO_Path->Configuration.FacilityState=WPX_UFE_FRAMER_FACILITY_STATE_CONNECTED;
}


void OMIINO_FRAMER_SONET_SDH_LO_Facility_MakeUnConnected(OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE *pLO_Path)
{
    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_FACILITY_STATE_CONNECTED==pLO_Path->Configuration.FacilityState,pLO_Path->Configuration.FacilityState);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_LINE_BANDWIDTH_NULL!=pLO_Path->Bandwidth.Configuration.Payload,pLO_Path->Bandwidth.Configuration.Payload);

    pLO_Path->Configuration.FacilityState=WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE;
}


U8 OMIINO_FRAMER_SONET_SDH_LO_Facility_GetState(OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE *pLO_Path)
{
    return (pLO_Path->Configuration.FacilityState);
}


U8 OMIINO_FRAMER_SONET_SDH_LO_Facility_IsConnected(OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE *pLO_Path)
{
    return(WPX_UFE_FRAMER_FACILITY_STATE_CONNECTED==pLO_Path->Configuration.FacilityState);
}


U8 OMIINO_FRAMER_SONET_SDH_LO_Facility_IsFree(OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE *pLO_Path)
{
    return(WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE==pLO_Path->Configuration.FacilityState);
}


U8 OMIINO_FRAMER_SONET_SDH_LO_Facility_Exists(OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE *pLO_Path)
{
    return(WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST!=pLO_Path->Configuration.FacilityState);
}






