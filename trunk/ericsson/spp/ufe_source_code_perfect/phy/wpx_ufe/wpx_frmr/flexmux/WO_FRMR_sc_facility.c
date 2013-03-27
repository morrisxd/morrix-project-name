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


void OMIINO_FRAMER_SOCKET_CLIENT_PDH_Set_Default_Timestamping(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient)
{
	pClient->TimestampingIsEnabled=0;
}


void OMIINO_FRAMER_SOCKET_CLIENT_PDH_Set_Default_TimestampByteCount(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient)
{
	switch(pClient->ClientType)
	{   



		case    WPX_UFE_FRAMER_CLIENT_C4:	
		case    WPX_UFE_FRAMER_CLIENT_C4_4C:
		case    WPX_UFE_FRAMER_CLIENT_C3:
		case    WPX_UFE_FRAMER_CLIENT_C12:	
		case    WPX_UFE_FRAMER_CLIENT_C11:
				pClient->TimestampByteCount=0;
				/*
				 *
				 * DMCC 20110301 No Timestamp support for containers - confirmed by AB.
				 *
				 */
				break;






		case    WPX_UFE_FRAMER_CLIENT_E1:
				pClient->TimestampByteCount=32;
				break;

		case    WPX_UFE_FRAMER_CLIENT_E3:
				pClient->TimestampByteCount=0;
				
				break;

		case    WPX_UFE_FRAMER_CLIENT_T1:
				pClient->TimestampByteCount=0xC0;
				break;

		case    WPX_UFE_FRAMER_CLIENT_T3:
				pClient->TimestampByteCount=0;
				break;

		default:
				OMIINO_FRAMER_RSE(pClient->ClientType);
				break;
	}
}


void OMIINO_FRAMER_SOCKET_CLIENT_PDH_SetDefaultFramingAndMappingBasedOnClientType(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient)
{
	switch(pClient->ClientType)
	{   
		case    WPX_UFE_FRAMER_CLIENT_C4:	
		case    WPX_UFE_FRAMER_CLIENT_C4_4C:
		case    WPX_UFE_FRAMER_CLIENT_C3:
		case    WPX_UFE_FRAMER_CLIENT_C12:	
		case    WPX_UFE_FRAMER_CLIENT_C11:
				pClient->Framing=WPX_UFE_FRAMER_CLIENT_UNFRAMED;
				pClient->Mapping=WPX_UFE_FRAMER_CLIENT_MAPPING_BULK_FILL;
				break;

		case    WPX_UFE_FRAMER_CLIENT_E1:
				pClient->Framing=WPX_UFE_FRAMER_CLIENT_UNFRAMED;
				pClient->Mapping=WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC;
				break;

		case    WPX_UFE_FRAMER_CLIENT_E3:
				pClient->Framing=WPX_UFE_FRAMER_CLIENT_UNFRAMED;
				pClient->Mapping=WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC;
				break;

		case    WPX_UFE_FRAMER_CLIENT_T1:
				pClient->Framing=WPX_UFE_FRAMER_CLIENT_UNFRAMED;
				pClient->Mapping=WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC;
				break;

		case    WPX_UFE_FRAMER_CLIENT_T3:
				pClient->Framing=WPX_UFE_FRAMER_CLIENT_UNFRAMED;
				pClient->Mapping=WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC;
				break;

		default:
				OMIINO_FRAMER_RSE(pClient->ClientType);
				break;
	}
}




void OMIINO_FRAMER_SOCKET_CLIENT_PDH_Initialize_PM(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient)
{
	int i;

	switch(pClient->ClientType)
	{   

		case    WPX_UFE_FRAMER_CLIENT_C4:	
		case    WPX_UFE_FRAMER_CLIENT_C4_4C:
		case    WPX_UFE_FRAMER_CLIENT_C3:
		case    WPX_UFE_FRAMER_CLIENT_C12:	
		case    WPX_UFE_FRAMER_CLIENT_C11:
				/* Ignore */
				break;

		case    WPX_UFE_FRAMER_CLIENT_E1:
		case    WPX_UFE_FRAMER_CLIENT_T1:
				for(i=0;i<WPX_UFE_FRAMER_MAX_PDH_MONITORED_POINTS_PER_PDH_ELEMENT;i++)
				{
					pClient->PM.PM_CompressedKey[i]=0xFFFF;
					pClient->PM.PM_State[i]=0;
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_E3:
		case    WPX_UFE_FRAMER_CLIENT_T3:
				for(i=0;i<WPX_UFE_FRAMER_MAX_PDH_MONITORED_POINTS_PER_PDH_ELEMENT;i++)
				{
					pClient->PM.PM_CompressedKey[i]=0xFFFF;
					pClient->PM.PM_State[i]=0;
				}
				break;

		default:
				OMIINO_FRAMER_RSE(pClient->ClientType);
				break;
	}
}



void OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_Create(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient, U8 AnyClientType)
{
    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST==pClient->FacilityState,pClient->FacilityState);
    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CLIENT_MAX_BANDIWDTH_TYPES>AnyClientType,AnyClientType);
    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CLIENT_NULL==pClient->ClientType,pClient->ClientType);

    pClient->FacilityState=WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE;
    pClient->ClientType=AnyClientType;

	OMIINO_FRAMER_SOCKET_CLIENT_PDH_SetDefaultFramingAndMappingBasedOnClientType(pClient);
	OMIINO_FRAMER_SOCKET_CLIENT_PDH_Set_Default_TimestampByteCount(pClient);
	OMIINO_FRAMER_SOCKET_CLIENT_PDH_Set_Default_Timestamping(pClient);

	OMIINO_FRAMER_SOCKET_CLIENT_PDH_Initialize_PM(pClient);
}


void OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_Delete(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient)
{
    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE==pClient->FacilityState,pClient->FacilityState);
    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CLIENT_NULL!=pClient->ClientType,pClient->ClientType);
    
    pClient->FacilityState=WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST;
    pClient->ClientType=WPX_UFE_FRAMER_CLIENT_NULL;
}


void OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_MakeConnected(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient)
{
    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE==pClient->FacilityState,pClient->FacilityState);
    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CLIENT_NULL!=pClient->ClientType,pClient->ClientType);

    pClient->FacilityState=WPX_UFE_FRAMER_FACILITY_STATE_CONNECTED;
}


void OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_MakeUnConnected(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient)
{
    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_FACILITY_STATE_CONNECTED==pClient->FacilityState,pClient->FacilityState);
    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CLIENT_NULL!=pClient->ClientType,pClient->ClientType);

    pClient->FacilityState=WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE;
}


U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_GetState(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient)
{
    return (pClient->FacilityState);
}


U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_IsConnected(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient)
{
    return(WPX_UFE_FRAMER_FACILITY_STATE_CONNECTED==pClient->FacilityState);
}


U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_IsFree(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient)
{
    return(WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE==pClient->FacilityState);
}


U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_Exists(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient)
{
    return(WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST!=pClient->FacilityState);
}






