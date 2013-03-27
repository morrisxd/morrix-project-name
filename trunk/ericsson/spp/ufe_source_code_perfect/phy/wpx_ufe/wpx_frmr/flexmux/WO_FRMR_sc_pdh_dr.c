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


U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_DeepLineLoopbackOnlySupportedForUnFramedClients(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient)
{
	U8 Result = WPX_UFE_FRAMER_OK; /* DMCC 27th Sept 2010 - CR 324 */

	switch(pClient->Framing)
	{   
		case    WPX_UFE_FRAMER_CLIENT_FRAMING_SF_D4:	/* Drop through */ 
		case    WPX_UFE_FRAMER_CLIENT_FRAMING_ESF:		/* Drop through */ 
		case	WPX_UFE_FRAMER_CLIENT_PCM31CRC:
		case	WPX_UFE_FRAMER_CLIENT_PCM31:
                case    WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_M23: 
                case    WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_CPARITY:
		case    WPX_UFE_FRAMER_CLIENT_FRAMING_E3_G751:
		case    WPX_UFE_FRAMER_CLIENT_FRAMING_E3_G751_ATM:
				Result = WPX_UFE_FRAMER_ERROR_CLIENT_REQUEST_NOT_SUPPORTED_FOR_FRAMED_CLIENTS;
				break;

		case	WPX_UFE_FRAMER_CLIENT_UNFRAMED:
				Result = WPX_UFE_FRAMER_OK;
				break;

		default:
				Result = WPX_UFE_FRAMER_ERROR_CLIENT_REQUEST_NOT_SUPPORTED_FOR_FRAMED_CLIENTS;
				break;
	}

	return Result;
}

U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_RdiTowardsLineOnlySupportedForFramedClients(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient)
{
	U8 Result = WPX_UFE_FRAMER_OK; 

	switch(pClient->Framing)
	{
		case    WPX_UFE_FRAMER_CLIENT_FRAMING_SF_D4:	
		case    WPX_UFE_FRAMER_CLIENT_FRAMING_ESF:		
		case	WPX_UFE_FRAMER_CLIENT_PCM31CRC:
		case	WPX_UFE_FRAMER_CLIENT_PCM31:
 		case	WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_M23:
  		case	WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_CPARITY:
				Result = WPX_UFE_FRAMER_OK;
				break;

		case	WPX_UFE_FRAMER_CLIENT_UNFRAMED:
				Result = WPX_UFE_FRAMER_ERROR_PDH_RDI_NOT_SUPPORTED_FOR_UNFRAMED_CLIENTS;
				break;

		default:
				Result = WPX_UFE_FRAMER_ERROR_PDH_RDI_NOT_SUPPORTED_FOR_UNFRAMED_CLIENTS;
				break;
	}

	return Result;
}


U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_IsFree(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(!OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_IsFree(pClient))
	{
		Result = WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_ALREADY_CONNECTED;
	}

	return Result;
}



U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_IsConnected(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(!OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_IsConnected(pClient))
	{
		Result = WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_NOT_CONNECTED;
	}

	return Result;
}


U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_E1T1_CRC_IsAvailable(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(pClient->Framing)
	{   
		case	WPX_UFE_FRAMER_CLIENT_FRAMING_SF_D4:
				Result = WPX_UFE_FRAMER_ERROR_PM_POINT_UNAVAILABLE;
				break;

		case	WPX_UFE_FRAMER_CLIENT_FRAMING_ESF:
				Result = WPX_UFE_FRAMER_OK;
				break;

		case	WPX_UFE_FRAMER_CLIENT_PCM31CRC:
				Result = WPX_UFE_FRAMER_OK;
				break;

		case	WPX_UFE_FRAMER_CLIENT_UNFRAMED:
				Result = WPX_UFE_FRAMER_ERROR_PM_POINT_UNAVAILABLE;
				break;

		case	WPX_UFE_FRAMER_CLIENT_PCM31:
				Result = WPX_UFE_FRAMER_ERROR_PM_POINT_UNAVAILABLE;
				break;

           case    WPX_UFE_FRAMER_CLIENT_FRAMING_E3_G751:
           case    WPX_UFE_FRAMER_CLIENT_FRAMING_E3_G751_ATM:
           case    WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_M23: 
           case    WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_CPARITY:
				Result = WPX_UFE_FRAMER_ERROR_PM_POINT_UNAVAILABLE;
				break;

		default:
				Result = WPX_UFE_FRAMER_ERROR_PM_POINT_UNAVAILABLE;
				break;
	}

	return Result;
}

U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_E3T3_PM_IsAvailable(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient, U8 iPoint)
{
   U8 Result = WPX_UFE_FRAMER_OK;
   
   if (WPX_UFE_FRAMER_CLIENT_UNFRAMED == pClient->Framing)
   {   
      Result = WPX_UFE_FRAMER_ERROR_PM_POINT_UNAVAILABLE;
   }

   if (WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_CPARITY != pClient->Framing &&
       WPX_UFE_FRAMER_PDH_E3T3_PM_FEBE == iPoint)
   {   
      Result = WPX_UFE_FRAMER_ERROR_PM_POINT_UNAVAILABLE;
   }

   return Result;
}

U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(!OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_Exists(pClient))
	{
		Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
	}

	return Result;
}


U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityDoesNotExist(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_Exists(pClient))
	{
		Result = WPX_UFE_FRAMER_ERROR_FACILITY_ALREADY_EXISTS;
	}

	return Result;
}


U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketIsBlockedByOther(U8 iDevice, U32 iSocketClientPDH, U8 ClientType)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U32															Peer_InclusiveLower=iSocketClientPDH-(iSocketClientPDH%4);
	OMIINO_FRAMER_HIERARCHY_TYPE								* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient=NULL;

	pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);



	switch(ClientType)
	{   
		case    WPX_UFE_FRAMER_CLIENT_E1:  
		case    WPX_UFE_FRAMER_CLIENT_T1:
				Peer_InclusiveLower=iSocketClientPDH-(iSocketClientPDH%336);
				pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, Peer_InclusiveLower);
				if(WPX_UFE_FRAMER_CLIENT_C4_4C==pClient->ClientType)
				{
	                Result = WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
				}

				Peer_InclusiveLower=iSocketClientPDH-(iSocketClientPDH%84);
				pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, Peer_InclusiveLower);
				if(WPX_UFE_FRAMER_CLIENT_C4==pClient->ClientType)
				{
	                Result = WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
				}

				Peer_InclusiveLower=iSocketClientPDH-(iSocketClientPDH%28);
				pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, Peer_InclusiveLower);
				if(WPX_UFE_FRAMER_CLIENT_C3==pClient->ClientType)
				{
	                Result = WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
				}

				if(WPX_UFE_FRAMER_CLIENT_E3==pClient->ClientType)
				{
	                Result = WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
				}

				if(WPX_UFE_FRAMER_CLIENT_T3==pClient->ClientType)
				{
	                Result = WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_T3:
		case    WPX_UFE_FRAMER_CLIENT_E3:
				Peer_InclusiveLower=iSocketClientPDH-(iSocketClientPDH%336);
				pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, Peer_InclusiveLower);
				if(WPX_UFE_FRAMER_CLIENT_C4_4C==pClient->ClientType)
				{
                                   Result = WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
				}

				Peer_InclusiveLower=iSocketClientPDH-(iSocketClientPDH%84);
				pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, Peer_InclusiveLower);
				if(WPX_UFE_FRAMER_CLIENT_C4==pClient->ClientType)
				{
                                   Result = WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
				}

		case    WPX_UFE_FRAMER_CLIENT_C4:	
				Peer_InclusiveLower=iSocketClientPDH-(iSocketClientPDH%336);
				pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, Peer_InclusiveLower);
				if(WPX_UFE_FRAMER_CLIENT_C4_4C==pClient->ClientType)
				{
	                Result = WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_C4_4C:
				Peer_InclusiveLower=iSocketClientPDH-(iSocketClientPDH%336);
				pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, Peer_InclusiveLower);
				if(WPX_UFE_FRAMER_CLIENT_NULL!=pClient->ClientType)
				{
	                Result = WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_C3:
                Result = WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER; /* TODO */
				break;

		case    WPX_UFE_FRAMER_CLIENT_C12:	
		case    WPX_UFE_FRAMER_CLIENT_C11:	
                Result = WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER; /* TODO */
				break;

		default:
				Result = WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
				break;
	}

	return Result;
}



U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketIsCompatibleWithPeers(U8 iDevice, U32 iSocketClientPDH, U8 ClientType)
{
	U8															Result = WPX_UFE_FRAMER_OK;
	U32 														Peer_InclusiveLower=0;
	U32 														Peer_InclusiveUpper=0;
	U32 														iWalkingSocket;
	OMIINO_FRAMER_HIERARCHY_TYPE								* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient=NULL;

	pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

	switch(ClientType)
	{   
		case    WPX_UFE_FRAMER_CLIENT_E1:  
		case    WPX_UFE_FRAMER_CLIENT_T1:
				Peer_InclusiveLower=iSocketClientPDH-(iSocketClientPDH%4);
				Peer_InclusiveUpper=3+Peer_InclusiveLower;
				for(iWalkingSocket=Peer_InclusiveLower; iWalkingSocket<=Peer_InclusiveUpper; iWalkingSocket++)
				{
					if(iSocketClientPDH!=iWalkingSocket)
					{		
						pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iWalkingSocket);
						if(WPX_UFE_FRAMER_CLIENT_NULL!=pClient->ClientType)
						{
							if(ClientType!=pClient->ClientType)
							{
								Result=WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
								break;
							}
						}
					}
				}

				if(WPX_UFE_FRAMER_OK==Result) /* DMCC 20110127 Only 3 out of each set of 4 sockets are available for E1 */
				{
					if(iSocketClientPDH==Peer_InclusiveUpper)
					{
						if(WPX_UFE_FRAMER_CLIENT_E1==ClientType)
						{
							Result=WPX_UFE_FRAMER_ERROR_INVALID_FACILITY;
						}

						if(WPX_UFE_FRAMER_CLIENT_C12==ClientType)
						{
							Result=WPX_UFE_FRAMER_ERROR_INVALID_FACILITY;
						}
					}
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_T3:
		case    WPX_UFE_FRAMER_CLIENT_E3:
				Peer_InclusiveLower=iSocketClientPDH-(iSocketClientPDH%28);
				Peer_InclusiveUpper=28+Peer_InclusiveLower;
				for(iWalkingSocket=Peer_InclusiveLower; iWalkingSocket<=Peer_InclusiveUpper; iWalkingSocket++)
				{
					if(iSocketClientPDH!=iWalkingSocket)
					{		
						pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iWalkingSocket);
						if(WPX_UFE_FRAMER_CLIENT_NULL!=pClient->ClientType)
						{
							Result=WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
							break;
						}
					}
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_C4:	
				Peer_InclusiveLower=iSocketClientPDH-(iSocketClientPDH%84);
				Peer_InclusiveUpper=83+Peer_InclusiveLower;
				for(iWalkingSocket=Peer_InclusiveLower; iWalkingSocket<=Peer_InclusiveUpper; iWalkingSocket++)
				{
					if(iSocketClientPDH!=iWalkingSocket)
					{		
						pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iWalkingSocket);
						if(WPX_UFE_FRAMER_CLIENT_NULL!=pClient->ClientType)
						{
							Result=WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
							break;
						}
					}
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_C4_4C:
				Peer_InclusiveLower=iSocketClientPDH-(iSocketClientPDH%336);
				Peer_InclusiveUpper=335+Peer_InclusiveLower;
				for(iWalkingSocket=Peer_InclusiveLower; iWalkingSocket<=Peer_InclusiveUpper; iWalkingSocket++)
				{
					if(iSocketClientPDH!=iWalkingSocket)
					{		
						pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iWalkingSocket);
						if(WPX_UFE_FRAMER_CLIENT_NULL!=pClient->ClientType)
						{
							Result=WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
							break;
						}
					}
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_C3:
                Result = WPX_UFE_FRAMER_ERROR_INVALID_FACILITY; /* TODO */
				break;

		case    WPX_UFE_FRAMER_CLIENT_C12:	
		case    WPX_UFE_FRAMER_CLIENT_C11:	
                Result = WPX_UFE_FRAMER_ERROR_INVALID_FACILITY; /* TODO */
				break;

		default:
				Result = WPX_UFE_FRAMER_ERROR_INVALID_FACILITY;
				break;
	}

	return Result;
}




U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_Supports_CAS(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(pClient->ClientType)
	{   
		case    WPX_UFE_FRAMER_CLIENT_E1:  
		case    WPX_UFE_FRAMER_CLIENT_E3:
				if(WPX_UFE_FRAMER_CLIENT_UNFRAMED==pClient->Framing)
				{  
					Result = WPX_UFE_FRAMER_ERROR_CAS_INVALID_FOR_CLIENT_APPLICATION_MODE;
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_T1:
		case    WPX_UFE_FRAMER_CLIENT_T3:
				if(WPX_UFE_FRAMER_CLIENT_UNFRAMED==pClient->Framing)
				{  
					Result = WPX_UFE_FRAMER_ERROR_CAS_INVALID_FOR_CLIENT_APPLICATION_MODE;
				}
				break;
 
		case    WPX_UFE_FRAMER_CLIENT_C4:	
		case    WPX_UFE_FRAMER_CLIENT_C4_4C:
		case    WPX_UFE_FRAMER_CLIENT_C3:
		case    WPX_UFE_FRAMER_CLIENT_C12:	
		case    WPX_UFE_FRAMER_CLIENT_C11:	
				Result = WPX_UFE_FRAMER_ERROR_CAS_INVALID_FOR_CLIENT_APPLICATION_MODE;
				break;

		default:
                Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
				break;
	}

	return Result;
}



U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_MappingInRange(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient, U8 Mapping)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(Mapping)
    {   
		case	WPX_UFE_FRAMER_CLIENT_MAPPING_BULK_FILL:
				switch(pClient->ClientType)
				{   
					case    WPX_UFE_FRAMER_CLIENT_E1:  /* Drop through */
					case    WPX_UFE_FRAMER_CLIENT_T1:
							Result = WPX_UFE_FRAMER_OK;
							break;

					case    WPX_UFE_FRAMER_CLIENT_T3:
					case    WPX_UFE_FRAMER_CLIENT_E3:
			                Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
							break;

					case    WPX_UFE_FRAMER_CLIENT_C4:	
					case    WPX_UFE_FRAMER_CLIENT_C4_4C:
					case    WPX_UFE_FRAMER_CLIENT_C3:
					case    WPX_UFE_FRAMER_CLIENT_C12:	
					case    WPX_UFE_FRAMER_CLIENT_C11:	
			                Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
							break;
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC:  /* Drop through */ 
				switch(pClient->ClientType)
				{   
					case    WPX_UFE_FRAMER_CLIENT_E1:  
							switch(pClient->Framing)
							{   
								case	WPX_UFE_FRAMER_CLIENT_PCM31:
								case    WPX_UFE_FRAMER_CLIENT_PCM31CRC: 
								case	WPX_UFE_FRAMER_CLIENT_UNFRAMED:
										Result = WPX_UFE_FRAMER_OK;
										break;
								default:
										Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
										break;
							}
							break;

					case    WPX_UFE_FRAMER_CLIENT_E3:
							switch(pClient->Framing)
							{   
                                                                case    WPX_UFE_FRAMER_CLIENT_FRAMING_E3_G751:
                                                                case    WPX_UFE_FRAMER_CLIENT_FRAMING_E3_G751_ATM:
								case	WPX_UFE_FRAMER_CLIENT_UNFRAMED:
										Result = WPX_UFE_FRAMER_OK;
										break;
								default:
										Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
										break;
							}
							break;

					case    WPX_UFE_FRAMER_CLIENT_T3:
							switch(pClient->Framing)
							{   
                                                                case    WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_M23: 
                                                                case    WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_CPARITY:
								case	WPX_UFE_FRAMER_CLIENT_UNFRAMED:
										Result = WPX_UFE_FRAMER_OK;
										break;
								default:
										Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
										break;
							}
							break;

					case    WPX_UFE_FRAMER_CLIENT_T1:
							switch(pClient->Framing)
							{   
								case    WPX_UFE_FRAMER_CLIENT_FRAMING_SF_D4:  /* Drop through */ 
								case    WPX_UFE_FRAMER_CLIENT_FRAMING_ESF:
								case	WPX_UFE_FRAMER_CLIENT_UNFRAMED:
										Result = WPX_UFE_FRAMER_OK;
										break;
								default:
										Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
										break;
							}
							break;

					case    WPX_UFE_FRAMER_CLIENT_C4:	
					case    WPX_UFE_FRAMER_CLIENT_C4_4C:
					case    WPX_UFE_FRAMER_CLIENT_C3:
					case    WPX_UFE_FRAMER_CLIENT_C12:	
					case    WPX_UFE_FRAMER_CLIENT_C11:	
			                Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
							break;
				}
                break;

		case    WPX_UFE_FRAMER_CLIENT_MAPPING_BYTE_SYNC:
				switch(pClient->ClientType)
				{   
					case    WPX_UFE_FRAMER_CLIENT_E1:  
							switch(pClient->Framing)
							{   
								case	WPX_UFE_FRAMER_CLIENT_PCM31:
								case    WPX_UFE_FRAMER_CLIENT_PCM31CRC: 
										Result = WPX_UFE_FRAMER_OK;
										break;
								default:
										Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
										break;
							}
							break;

					case    WPX_UFE_FRAMER_CLIENT_T1:
							switch(pClient->Framing)
							{   
								case    WPX_UFE_FRAMER_CLIENT_FRAMING_SF_D4:  /* Drop through */ 
								case    WPX_UFE_FRAMER_CLIENT_FRAMING_ESF:
										Result = WPX_UFE_FRAMER_OK;
										break;
								default:
										Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
										break;
							}
							break;

					case    WPX_UFE_FRAMER_CLIENT_C4:	
					case    WPX_UFE_FRAMER_CLIENT_C4_4C:
					case    WPX_UFE_FRAMER_CLIENT_C3:
					case    WPX_UFE_FRAMER_CLIENT_C12:	
					case    WPX_UFE_FRAMER_CLIENT_C11:
                                        case    WPX_UFE_FRAMER_CLIENT_E3:
					case    WPX_UFE_FRAMER_CLIENT_T3:

			                Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
							break;
				}
                break;

		case    WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_SYNC:
				switch(pClient->ClientType)
				{   
					case    WPX_UFE_FRAMER_CLIENT_E1:
					case    WPX_UFE_FRAMER_CLIENT_E3:
					case    WPX_UFE_FRAMER_CLIENT_T3:
							Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
							break;

					case    WPX_UFE_FRAMER_CLIENT_T1:   
							switch(pClient->Framing)
							{   
								case    WPX_UFE_FRAMER_CLIENT_FRAMING_SF_D4:  /* Drop through */ 
								case    WPX_UFE_FRAMER_CLIENT_FRAMING_ESF:
								case	WPX_UFE_FRAMER_CLIENT_UNFRAMED:
										Result = WPX_UFE_FRAMER_OK;
										break;
								default:
										Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
										break;
							}
							break;

					case    WPX_UFE_FRAMER_CLIENT_C4:	
					case    WPX_UFE_FRAMER_CLIENT_C4_4C:
					case    WPX_UFE_FRAMER_CLIENT_C3:
					case    WPX_UFE_FRAMER_CLIENT_C12:	
					case    WPX_UFE_FRAMER_CLIENT_C11:	
			                Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
							break;
				}
				break;

        default:
                Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
                break;

    }

	return Result;
}



U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_FramingInRange(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *pClient, U8 Framing)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(Framing)
    {   
		case    WPX_UFE_FRAMER_CLIENT_UNFRAMED:
				switch(pClient->ClientType)
				{   
					case    WPX_UFE_FRAMER_CLIENT_E1:  
					case    WPX_UFE_FRAMER_CLIENT_E3:
							Result = WPX_UFE_FRAMER_OK;
							break;

					case    WPX_UFE_FRAMER_CLIENT_T1:
					case    WPX_UFE_FRAMER_CLIENT_T3:
							Result = WPX_UFE_FRAMER_OK;
							break;

					case    WPX_UFE_FRAMER_CLIENT_C4:	
					case    WPX_UFE_FRAMER_CLIENT_C4_4C:
					case    WPX_UFE_FRAMER_CLIENT_C3:
					case    WPX_UFE_FRAMER_CLIENT_C12:	
					case    WPX_UFE_FRAMER_CLIENT_C11:	
							Result = WPX_UFE_FRAMER_ERROR_CLIENT_FRAMING_INVALID_FOR_CLIENT_TYPE;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
							break;
				}
				break;

		case	WPX_UFE_FRAMER_CLIENT_PCM31CRC:
		case	WPX_UFE_FRAMER_CLIENT_PCM31:
				switch(pClient->ClientType)
				{   
					case    WPX_UFE_FRAMER_CLIENT_T1:
			                Result = WPX_UFE_FRAMER_ERROR_CLIENT_FRAMING_INVALID_FOR_CLIENT_TYPE;
							break;

					case    WPX_UFE_FRAMER_CLIENT_E1:  /* Drop through */ 
							Result = WPX_UFE_FRAMER_OK;
							break;

					case    WPX_UFE_FRAMER_CLIENT_T3:
					case    WPX_UFE_FRAMER_CLIENT_E3:
					case    WPX_UFE_FRAMER_CLIENT_C4:	
					case    WPX_UFE_FRAMER_CLIENT_C4_4C:
					case    WPX_UFE_FRAMER_CLIENT_C3:
					case    WPX_UFE_FRAMER_CLIENT_C12:	
					case    WPX_UFE_FRAMER_CLIENT_C11:	
							Result = WPX_UFE_FRAMER_ERROR_CLIENT_FRAMING_INVALID_FOR_CLIENT_TYPE;
							break;
					default:
							Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
							break;
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_FRAMING_SF_D4:  /* Drop through */ 
		case    WPX_UFE_FRAMER_CLIENT_FRAMING_ESF:
				switch(pClient->ClientType)
				{   
					case    WPX_UFE_FRAMER_CLIENT_T1:
							Result = WPX_UFE_FRAMER_OK;
							break;

					case    WPX_UFE_FRAMER_CLIENT_E1:  /* Drop through */ 
			                Result = WPX_UFE_FRAMER_ERROR_CLIENT_FRAMING_INVALID_FOR_CLIENT_TYPE;
							break;

					case    WPX_UFE_FRAMER_CLIENT_T3:
					case    WPX_UFE_FRAMER_CLIENT_E3:
					case    WPX_UFE_FRAMER_CLIENT_C4:	
					case    WPX_UFE_FRAMER_CLIENT_C4_4C:
					case    WPX_UFE_FRAMER_CLIENT_C3:
					case    WPX_UFE_FRAMER_CLIENT_C12:	
					case    WPX_UFE_FRAMER_CLIENT_C11:	
							Result = WPX_UFE_FRAMER_ERROR_CLIENT_FRAMING_INVALID_FOR_CLIENT_TYPE;
							break;
					default:
							Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
							break;
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_M23: 
                case    WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_CPARITY:
				switch(pClient->ClientType)
				{   
					case    WPX_UFE_FRAMER_CLIENT_T3:
							Result = WPX_UFE_FRAMER_OK;
							break;

					case    WPX_UFE_FRAMER_CLIENT_E1:
					case    WPX_UFE_FRAMER_CLIENT_T1:
					case    WPX_UFE_FRAMER_CLIENT_E3:
					case    WPX_UFE_FRAMER_CLIENT_C4:	
					case    WPX_UFE_FRAMER_CLIENT_C4_4C:
					case    WPX_UFE_FRAMER_CLIENT_C3:
					case    WPX_UFE_FRAMER_CLIENT_C12:	
					case    WPX_UFE_FRAMER_CLIENT_C11:	
							Result = WPX_UFE_FRAMER_ERROR_CLIENT_FRAMING_INVALID_FOR_CLIENT_TYPE;
							break;
					default:
							Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
							break;
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_FRAMING_E3_G751:
		case    WPX_UFE_FRAMER_CLIENT_FRAMING_E3_G751_ATM:
				switch(pClient->ClientType)
				{   
					case    WPX_UFE_FRAMER_CLIENT_E3:
							Result = WPX_UFE_FRAMER_OK;
							break;

					case    WPX_UFE_FRAMER_CLIENT_E1:
					case    WPX_UFE_FRAMER_CLIENT_T1:
					case    WPX_UFE_FRAMER_CLIENT_T3:
					case    WPX_UFE_FRAMER_CLIENT_C4:	
					case    WPX_UFE_FRAMER_CLIENT_C4_4C:
					case    WPX_UFE_FRAMER_CLIENT_C3:
					case    WPX_UFE_FRAMER_CLIENT_C12:	
					case    WPX_UFE_FRAMER_CLIENT_C11:	
							Result = WPX_UFE_FRAMER_ERROR_CLIENT_FRAMING_INVALID_FOR_CLIENT_TYPE;
							break;
					default:
							Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
							break;
				}
				break;

        default:
                Result = WPX_UFE_FRAMER_ERROR_CLIENT_FRAMING_INVALID_FOR_CLIENT_TYPE;
                break;
    }

	return Result;
}




U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(OMIINO_DEVICE_PERSONALITY_MaxSocketClients(iDevice)<=iSocketClientPDH)
	{
           Result = WPX_UFE_FRAMER_ERROR_SOCKET_CLIENT_PORT_OUT_OF_RANGE;
	}
        if (WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3 == OMIINO_RAM.Device[iDevice].BuildPersonality)
        {
           
           if((336<=iSocketClientPDH)&&(672>iSocketClientPDH))
           {
              Result = WPX_UFE_FRAMER_ERROR_SOCKET_CLIENT_PORT_OUT_OF_RANGE;
           }
        }
        
	return Result;
}



U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 BuildPersonality;
	U8 DeviceProtectionMode;

	OMIINO_FRAMER_Device_GetBuildPersonality(OMIINO_FRAMER_GetRAMForDevice(&OMIINO_RAM, iDevice), &BuildPersonality);
	OMIINO_FRAMER_Device_GetProtectionMode(OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice), &DeviceProtectionMode);
	Result=OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientOkForPortRate(iDevice, BuildPersonality, DeviceProtectionMode, iSocketClientPDH);

	return Result;
}


