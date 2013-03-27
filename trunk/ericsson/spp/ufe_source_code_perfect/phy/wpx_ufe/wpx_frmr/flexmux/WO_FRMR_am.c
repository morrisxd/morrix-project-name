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



#define OMIINO_FRAMER_ALARM_MANAGER_NULL_ALARM					(0)
#define OMIINO_FRAMER_ALARM_MANAGER_PORT_ALARM					(0)
#define OMIINO_FRAMER_ALARM_MANAGER_PATH_ALARM					(1)
#define OMIINO_FRAMER_ALARM_MANAGER_DISCRETE_CLIENT_PORT_ALARM	(2)
#define OMIINO_FRAMER_ALARM_MANAGER_SOCKET_CLIENT_PDH_ALARM		(3)
#define OMIINO_FRAMER_ALARM_MANAGER_MAX_ALARM_INSTANCE_TYPE		(4)




void OMIINO_FRAMER_ALARM_MANAGER_ALARM_ANNOUNCE_CHANGE(U8 iDevice, U8 IsForced, OMIINO_ALARM_MANAGER_ELEMENT_TYPE * pAlarmElement)
{
	switch(pAlarmElement->InstanceType)
	{
		case OMIINO_FRAMER_ALARM_MANAGER_PORT_ALARM:
			OMIINO_ALARM_WRAPPER_SONET_SDH_PORT_ALARM_ANNOUNCE_TYPE(iDevice, pAlarmElement->Instance.u.iLinePort, pAlarmElement->AlarmCategory, pAlarmElement->IsAsserted, IsForced);
			break;

		case OMIINO_FRAMER_ALARM_MANAGER_PATH_ALARM:
			OMIINO_ALARM_WRAPPER_SONET_SDH_PATH_ALARM_ANNOUNCE_TYPE(iDevice, &(pAlarmElement->Instance.u.Endpoint), pAlarmElement->AlarmCategory, pAlarmElement->IsAsserted);
			break;

		case OMIINO_FRAMER_ALARM_MANAGER_DISCRETE_CLIENT_PORT_ALARM:
			OMIINO_ALARM_WRAPPER_DISCRETE_CLIENT_PORT_ALARM_ANNOUNCE_TYPE(iDevice, pAlarmElement->Instance.u.iDiscreteClientPort, pAlarmElement->AlarmCategory, pAlarmElement->IsAsserted);
			break;

		case OMIINO_FRAMER_ALARM_MANAGER_SOCKET_CLIENT_PDH_ALARM:
			OMIINO_ALARM_WRAPPER_SOCKET_CLIENT_PDH_ALARM_ANNOUNCE_TYPE(iDevice, pAlarmElement->Instance.u.iSocketClientPDH, pAlarmElement->AlarmCategory, pAlarmElement->IsAsserted);
			break;

		default:
			OMIINO_FRAMER_RSE(pAlarmElement->AlarmCategory);
			break;
	}
}




void OMIINO_FRAMER_ALARM_MANAGER_ALARM_STATUS_CHANGE(U8 iDevice, OMIINO_FRAMER_ALARM_MANAGER_DATA_TYPE * pAlarmMangerRAM, U32 NodeKey, U8 IsAsserted, U8 IsForced, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy)
{
	U8 ReportingIsEnabled;

	if(MAX_ALARM_MANAGER_NODE_ELEMENTS>NodeKey)
	{
		if(1==pAlarmMangerRAM->Element[NodeKey].IsActive)
		{
			switch(pAlarmMangerRAM->Element[NodeKey].InstanceType)
			{
				case OMIINO_FRAMER_ALARM_MANAGER_PORT_ALARM:
					OMIINO_FRAMER_Device_SONET_SDH_GetAlarmCategoryReporting(pHierarchy, pAlarmMangerRAM->Element[NodeKey].AlarmCategory, &ReportingIsEnabled);
					break;

				case OMIINO_FRAMER_ALARM_MANAGER_PATH_ALARM:
					OMIINO_FRAMER_Device_SONET_SDH_GetAlarmCategoryReporting(pHierarchy, pAlarmMangerRAM->Element[NodeKey].AlarmCategory, &ReportingIsEnabled);
					break;

				case OMIINO_FRAMER_ALARM_MANAGER_DISCRETE_CLIENT_PORT_ALARM:
					OMIINO_FRAMER_Device_DISCRETE_CLIENT_GetAlarmCategoryReporting(pHierarchy, pAlarmMangerRAM->Element[NodeKey].AlarmCategory, &ReportingIsEnabled);
					break;

				case OMIINO_FRAMER_ALARM_MANAGER_SOCKET_CLIENT_PDH_ALARM:
					OMIINO_FRAMER_Device_SOCKET_CLIENT_GetAlarmCategoryReporting(pHierarchy, pAlarmMangerRAM->Element[NodeKey].AlarmCategory, &ReportingIsEnabled);
					break;

				default:
					ReportingIsEnabled=0;
					OMIINO_FRAMER_RSE(pAlarmMangerRAM->Element[NodeKey].AlarmCategory);
					break;
			}

			if(1==ReportingIsEnabled)
			{
				if(pAlarmMangerRAM->Element[NodeKey].IsAsserted!=IsAsserted)
				{
					pAlarmMangerRAM->Element[NodeKey].IsAsserted=IsAsserted;
			
					OMIINO_FRAMER_ALARM_MANAGER_ALARM_ANNOUNCE_CHANGE(iDevice, IsForced, &(pAlarmMangerRAM->Element[NodeKey]));
				}
			}
			else
			{
				if(0==IsAsserted)
				{
					if(1==pAlarmMangerRAM->Element[NodeKey].IsAsserted)
					{
						pAlarmMangerRAM->Element[NodeKey].IsAsserted=IsAsserted;
						OMIINO_FRAMER_ALARM_MANAGER_ALARM_ANNOUNCE_CHANGE(iDevice, IsForced, &(pAlarmMangerRAM->Element[NodeKey]));
					}
				}
			}
		}
	}
	else
	{
		OMIINO_FRAMER_RSE(NodeKey);
	}
}


void OMIINO_FRAMER_ALARM_MANAGER_INSTANCE_Clone(U8 InstanceType, OMIINO_ALARM_MANAGER_INSTANCE_TYPE * pTo, OMIINO_ALARM_MANAGER_INSTANCE_TYPE * pFrom)
{
	switch(InstanceType)
	{
		case	OMIINO_FRAMER_ALARM_MANAGER_PORT_ALARM:
				pTo->u.iLinePort=pFrom->u.iLinePort;
				break;
		case	OMIINO_FRAMER_ALARM_MANAGER_PATH_ALARM:		
				pTo->u.Endpoint.Bandwidth=pFrom->u.Endpoint.Bandwidth;
				pTo->u.Endpoint.iLinePort=pFrom->u.Endpoint.iLinePort;
				if(OMIINO_FRAMER_Is_SDH_Endpoint(&(pFrom->u.Endpoint)))
				{
					pTo->u.Endpoint.u.sdh.iJ=pFrom->u.Endpoint.u.sdh.iJ;
					pTo->u.Endpoint.u.sdh.iK=pFrom->u.Endpoint.u.sdh.iK;
					pTo->u.Endpoint.u.sdh.iL=pFrom->u.Endpoint.u.sdh.iL;
					pTo->u.Endpoint.u.sdh.iM=pFrom->u.Endpoint.u.sdh.iM;
				}
				else
				{
					pTo->u.Endpoint.u.sonet.iU=pFrom->u.Endpoint.u.sonet.iU;
					pTo->u.Endpoint.u.sonet.iV=pFrom->u.Endpoint.u.sonet.iV;
					pTo->u.Endpoint.u.sonet.iW=pFrom->u.Endpoint.u.sonet.iW;
				}
				break;
		case	OMIINO_FRAMER_ALARM_MANAGER_DISCRETE_CLIENT_PORT_ALARM:	
				pTo->u.iDiscreteClientPort=pFrom->u.iDiscreteClientPort;
			break;
		case	OMIINO_FRAMER_ALARM_MANAGER_SOCKET_CLIENT_PDH_ALARM:
				pTo->u.iSocketClientPDH=pFrom->u.iSocketClientPDH;
			break;
		default:
			OMIINO_FRAMER_RSE(InstanceType);
		break;
	}
	

}


void OMIINO_FRAMER_ALARM_MANAGER_StoreAlarmElementInfo(OMIINO_ALARM_MANAGER_ELEMENT_TYPE * pAlarmElement, U8 AlarmCategory, OMIINO_ALARM_MANAGER_INSTANCE_TYPE * pInstance, U8 InstanceType)
{
	OMIINO_FRAMER_ASSERT(0==pAlarmElement->IsActive,AlarmCategory);

	pAlarmElement->InstanceType						= InstanceType;
	pAlarmElement->IsActive							= 1;
	pAlarmElement->AlarmCategory					= AlarmCategory;
	pAlarmElement->IsAsserted						= 0;

	OMIINO_FRAMER_ALARM_MANAGER_INSTANCE_Clone(InstanceType, &(pAlarmElement->Instance), pInstance);
}



U32 OMIINO_FRAMER_ALARM_MANAGER_INTERNAL_GetNodeKey(U8 iDevice, OMIINO_FRAMER_ALARM_MANAGER_DATA_TYPE * pAlarmMangerRAM, U8 AlarmCategory, OMIINO_ALARM_MANAGER_INSTANCE_TYPE * pInstance, U8 InstanceType)
{
	U32 NodeKey=MAX_ALARM_MANAGER_NODE_ELEMENTS;

    OMIINO_FRAMER_ASSERT(NULL!=pAlarmMangerRAM,0);

	if (MAX_ALARM_MANAGER_NODE_ELEMENTS>pAlarmMangerRAM->NextFree)
	{
		NodeKey														= pAlarmMangerRAM->NextFree;
		pAlarmMangerRAM->NextFree									= pAlarmMangerRAM->Element[NodeKey].Next;

		if(MAX_ALARM_MANAGER_NODE_ELEMENTS>pAlarmMangerRAM->NextFree)
		{
			pAlarmMangerRAM->Element[pAlarmMangerRAM->NextFree].Prev	= MAX_ALARM_MANAGER_NODE_ELEMENTS;
		}

		pAlarmMangerRAM->Element[NodeKey].Next						= MAX_ALARM_MANAGER_NODE_ELEMENTS;
		pAlarmMangerRAM->Element[NodeKey].Prev						= MAX_ALARM_MANAGER_NODE_ELEMENTS;


		OMIINO_FRAMER_ALARM_MANAGER_StoreAlarmElementInfo( &(pAlarmMangerRAM->Element[NodeKey]), AlarmCategory, pInstance, InstanceType );
		if(NULL!=pAlarmMangerRAM->AnnounceKeyStored)
		{
			(*pAlarmMangerRAM->AnnounceKeyStored)(iDevice, NodeKey, AlarmCategory, pInstance, InstanceType);
		}
	}
	else
	{
		OMIINO_FRAMER_RSE(pAlarmMangerRAM->NextFree);
	}

    OMIINO_FRAMER_ASSERT(MAX_ALARM_MANAGER_NODE_ELEMENTS>NodeKey,NodeKey);

	return NodeKey;
}



U32 OMIINO_FRAMER_ALARM_MANAGER_SONET_SDH_PORT_GetNodeKey(U8 iDevice, OMIINO_FRAMER_ALARM_MANAGER_DATA_TYPE * pAlarmMangerRAM, U8 AlarmCategory, U8 iLinePort)
{
	U32 NodeKey = MAX_ALARM_MANAGER_NODE_ELEMENTS;
	OMIINO_ALARM_MANAGER_INSTANCE_TYPE Instance;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS>iLinePort,iLinePort);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES>AlarmCategory,AlarmCategory);

	Instance.u.iLinePort = iLinePort;

	NodeKey = OMIINO_FRAMER_ALARM_MANAGER_INTERNAL_GetNodeKey(iDevice, pAlarmMangerRAM, AlarmCategory, &Instance, OMIINO_FRAMER_ALARM_MANAGER_PORT_ALARM);

    OMIINO_FRAMER_ASSERT(MAX_ALARM_MANAGER_NODE_ELEMENTS>NodeKey,NodeKey);

	return NodeKey;
}



U32 OMIINO_FRAMER_ALARM_MANAGER_SONET_SDH_PATH_GetNodeKey(U8 iDevice, OMIINO_FRAMER_ALARM_MANAGER_DATA_TYPE * pAlarmMangerRAM, U8 AlarmCategory, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pEndpoint)
{
	U32 NodeKey = MAX_ALARM_MANAGER_NODE_ELEMENTS;
	OMIINO_ALARM_MANAGER_INSTANCE_TYPE Instance;

	OMIINO_FRAMER_ASSERT(1!=OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pEndpoint),0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES>AlarmCategory,AlarmCategory);

	OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&(Instance.u.Endpoint), pEndpoint);

	NodeKey = OMIINO_FRAMER_ALARM_MANAGER_INTERNAL_GetNodeKey(iDevice, pAlarmMangerRAM, AlarmCategory, &Instance, OMIINO_FRAMER_ALARM_MANAGER_PATH_ALARM);

    OMIINO_FRAMER_ASSERT(MAX_ALARM_MANAGER_NODE_ELEMENTS>NodeKey,NodeKey);

	return NodeKey;
}


U32 OMIINO_FRAMER_ALARM_MANAGER_DISCRETE_CLIENT_GetNodeKey(U8 iDevice, OMIINO_FRAMER_ALARM_MANAGER_DATA_TYPE * pAlarmMangerRAM, U8 AlarmCategory, U8 iDiscreteClientPort)
{
	U32 NodeKey = MAX_ALARM_MANAGER_NODE_ELEMENTS;
	OMIINO_ALARM_MANAGER_INSTANCE_TYPE Instance;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_DISCRETE_CLIENT_PORTS>iDiscreteClientPort,iDiscreteClientPort);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_DISCRETE_CLIENT_MAX_ALARM_CATEGORIES>AlarmCategory,AlarmCategory);

	Instance.u.iDiscreteClientPort = iDiscreteClientPort;

	NodeKey = OMIINO_FRAMER_ALARM_MANAGER_INTERNAL_GetNodeKey(iDevice, pAlarmMangerRAM, AlarmCategory, &Instance, OMIINO_FRAMER_ALARM_MANAGER_DISCRETE_CLIENT_PORT_ALARM);

    OMIINO_FRAMER_ASSERT(MAX_ALARM_MANAGER_NODE_ELEMENTS>NodeKey,NodeKey);

	return NodeKey;
}




U32 OMIINO_FRAMER_ALARM_MANAGER_SOCKET_CLIENT_GetNodeKey(U8 iDevice, OMIINO_FRAMER_ALARM_MANAGER_DATA_TYPE * pAlarmMangerRAM, U8 AlarmCategory, U32 iSocketClient)
{
	U32 NodeKey = MAX_ALARM_MANAGER_NODE_ELEMENTS;
	OMIINO_ALARM_MANAGER_INSTANCE_TYPE Instance;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS>iSocketClient,iSocketClient);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_SOCKET_CLIENT_MAX_ALARM_CATEGORIES>AlarmCategory,AlarmCategory);

	Instance.u.iSocketClientPDH = iSocketClient;

	NodeKey = OMIINO_FRAMER_ALARM_MANAGER_INTERNAL_GetNodeKey(iDevice, pAlarmMangerRAM, AlarmCategory, &Instance, OMIINO_FRAMER_ALARM_MANAGER_SOCKET_CLIENT_PDH_ALARM);

    OMIINO_FRAMER_ASSERT(MAX_ALARM_MANAGER_NODE_ELEMENTS>NodeKey,NodeKey);

	return NodeKey;
}




void OMIINO_FRAMER_ALARM_MANAGER_FreeNode(U8 iDevice, OMIINO_FRAMER_ALARM_MANAGER_DATA_TYPE * pAlarmMangerRAM, U32 NodeKey)
{   
	OMIINO_FRAMER_ASSERT(1==pAlarmMangerRAM->Element[NodeKey].IsActive,NodeKey);
    OMIINO_FRAMER_ASSERT(NULL!=pAlarmMangerRAM,0);
    OMIINO_FRAMER_ASSERT(MAX_ALARM_MANAGER_NODE_ELEMENTS>NodeKey,NodeKey);
	
	if(MAX_ALARM_MANAGER_NODE_ELEMENTS==pAlarmMangerRAM->NextFree)
	{
		pAlarmMangerRAM->NextFree=NodeKey;
		pAlarmMangerRAM->Element[NodeKey].Next						= MAX_ALARM_MANAGER_NODE_ELEMENTS;
	}
	else
	{
		pAlarmMangerRAM->Element[pAlarmMangerRAM->NextFree].Prev	= NodeKey;
		pAlarmMangerRAM->Element[NodeKey].Next						= pAlarmMangerRAM->NextFree;
		pAlarmMangerRAM->NextFree=NodeKey;
	}

	pAlarmMangerRAM->Element[NodeKey].IsActive						= 0;
	pAlarmMangerRAM->Element[NodeKey].AlarmCategory					= WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES;
	pAlarmMangerRAM->Element[NodeKey].IsAsserted					= 0;
	pAlarmMangerRAM->Element[NodeKey].InstanceType					= OMIINO_FRAMER_ALARM_MANAGER_NULL_ALARM;
	pAlarmMangerRAM->Element[NodeKey].Prev							= MAX_ALARM_MANAGER_NODE_ELEMENTS;

	if(NULL!=pAlarmMangerRAM->AnnounceKeyRemoved)
	{
		(*pAlarmMangerRAM->AnnounceKeyRemoved)(iDevice, NodeKey);
	}
	
}



void OMIINO_FRAMER_ALARM_MANAGER_PreInitialize(OMIINO_FRAMER_ALARM_MANAGER_DATA_TYPE * pAlarmMangerRAM)
{
	pAlarmMangerRAM->AnnounceKeyStored=NULL;
	pAlarmMangerRAM->AnnounceKeyRemoved=NULL;
}

void OMIINO_FRAMER_ALARM_MANAGER_Initialize(OMIINO_FRAMER_ALARM_MANAGER_DATA_TYPE * pAlarmMangerRAM)
{
	U32 i;

    OMIINO_FRAMER_ASSERT(NULL!=pAlarmMangerRAM,0);


	for(i=0; i<MAX_ALARM_MANAGER_NODE_ELEMENTS; i++)
	{
		pAlarmMangerRAM->Element[i].IsActive		= 0;
		pAlarmMangerRAM->Element[i].AlarmCategory	= WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES;
		pAlarmMangerRAM->Element[i].IsAsserted		= 0;
		pAlarmMangerRAM->Element[i].InstanceType	= OMIINO_FRAMER_ALARM_MANAGER_NULL_ALARM;

		pAlarmMangerRAM->Element[i].Next			= i+1;

		if(0==i)
		{
			pAlarmMangerRAM->Element[i].Prev		= MAX_ALARM_MANAGER_NODE_ELEMENTS;
		}
		else
		{
			pAlarmMangerRAM->Element[i].Prev		= i-1;
		}
	}

    pAlarmMangerRAM->NextFree=0;
}


