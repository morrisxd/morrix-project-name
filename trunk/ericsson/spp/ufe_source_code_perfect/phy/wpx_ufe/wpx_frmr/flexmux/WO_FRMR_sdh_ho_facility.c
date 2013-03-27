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



void OMIINO_FRAMER_SONET_SDH_HO_Facility_Create(OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE *pHO_Path, U8 AnyBandwidth, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST==pHO_Path->Configuration.FacilityState,pHO_Path->Configuration.FacilityState);
    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_MAX_LINE_BANDWIDTH_TYPES>AnyBandwidth,AnyBandwidth);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_LINE_BANDWIDTH_NULL==pHO_Path->Bandwidth.Configuration.Payload,pHO_Path->Bandwidth.Configuration.Payload);

    pHO_Path->Configuration.FacilityState=WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE;
	pHO_Path->Bandwidth.Configuration.Payload=AnyBandwidth;

	pHO_Path->Configuration.iLow=OMIINO_FRAMER_CONFIGURATION_Calculate_SONET_SDH_HO_PATH_OverheadiLow(pLineEndpoint);
}


void OMIINO_FRAMER_SONET_SDH_HO_Facility_Delete(OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE *pHO_Path)
{
    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE==pHO_Path->Configuration.FacilityState,pHO_Path->Configuration.FacilityState);
    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_LINE_BANDWIDTH_NULL!=pHO_Path->Bandwidth.Configuration.Payload,pHO_Path->Bandwidth.Configuration.Payload);
    
    pHO_Path->Configuration.FacilityState=WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST;
    pHO_Path->Bandwidth.Configuration.Payload=WPX_UFE_FRAMER_LINE_BANDWIDTH_NULL;
	OMIINO_FRAMER_CONFIGURATION_Initialize_SONET_SDH_HO_PATH_OverheadiLow(&(pHO_Path->Configuration));
}


void OMIINO_FRAMER_SONET_SDH_HO_Facility_MakeConnected(OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE *pHO_Path)
{
    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE==pHO_Path->Configuration.FacilityState,pHO_Path->Configuration.FacilityState);
    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_LINE_BANDWIDTH_NULL!=pHO_Path->Bandwidth.Configuration.Payload,pHO_Path->Bandwidth.Configuration.Payload);

	pHO_Path->Configuration.FacilityState=WPX_UFE_FRAMER_FACILITY_STATE_CONNECTED;
}


void OMIINO_FRAMER_SONET_SDH_HO_Facility_MakeUnConnected(OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE *pHO_Path)
{
    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_FACILITY_STATE_CONNECTED==pHO_Path->Configuration.FacilityState,pHO_Path->Configuration.FacilityState);
    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_LINE_BANDWIDTH_NULL!=pHO_Path->Bandwidth.Configuration.Payload,pHO_Path->Bandwidth.Configuration.Payload);

    pHO_Path->Configuration.FacilityState=WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE;
}


U8 OMIINO_FRAMER_SONET_SDH_HO_Facility_GetState(OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE *pHO_Path)
{
    return (pHO_Path->Configuration.FacilityState);
}


U8 OMIINO_FRAMER_SONET_SDH_HO_Facility_IsConnected(OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE *pHO_Path)
{
    return(WPX_UFE_FRAMER_FACILITY_STATE_CONNECTED==pHO_Path->Configuration.FacilityState);
}


U8 OMIINO_FRAMER_SONET_SDH_HO_Facility_IsFree(OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE *pHO_Path)
{
    return(WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE==pHO_Path->Configuration.FacilityState);
}


U8 OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE *pHO_Path)
{
    return(WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST!=pHO_Path->Configuration.FacilityState);
}






