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




void OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_KEY_NODE_Clone(OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE * pTo, OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE * pFrom)
{
    memcpy(pTo, pFrom, sizeof(OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE));
}



int OMIINO_FRAMER_PerformanceMonitoring_KeyManager_Given_CompressedKey_Get_PM_EngineKey(OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_TYPE * pPM_KeyManagerRAM, U16 CompressedKey, OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE *pPM_Engine_Key)
{
	int Result=0;

    OMIINO_FRAMER_ASSERT(NULL!=pPM_KeyManagerRAM,0);

	if (MAX_PERFORMANCE_MONITORING_KEY_MANAGER_NODE_ELEMENTS>CompressedKey)
	{
		if(pPM_KeyManagerRAM->Element[CompressedKey].IsActive)
		{
			Result=1;
			OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_KEY_NODE_Clone(pPM_Engine_Key, &(pPM_KeyManagerRAM->Element[CompressedKey].KeyNode));
		}
	}

	return Result;
}




U16 OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_GetNodeKey(OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_TYPE * pPM_KeyManagerRAM, OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE * pPM_Engine_Key)
{
	U16 CompressedKey=MAX_PERFORMANCE_MONITORING_KEY_MANAGER_NODE_ELEMENTS;

    OMIINO_FRAMER_ASSERT(NULL!=pPM_KeyManagerRAM,0);

	if (MAX_PERFORMANCE_MONITORING_KEY_MANAGER_NODE_ELEMENTS>pPM_KeyManagerRAM->NextFree)
	{
		CompressedKey														= pPM_KeyManagerRAM->NextFree;
		pPM_KeyManagerRAM->NextFree											= pPM_KeyManagerRAM->Element[CompressedKey].Next;

		if(MAX_PERFORMANCE_MONITORING_KEY_MANAGER_NODE_ELEMENTS>pPM_KeyManagerRAM->NextFree)
		{
			pPM_KeyManagerRAM->Element[pPM_KeyManagerRAM->NextFree].Prev	= MAX_PERFORMANCE_MONITORING_KEY_MANAGER_NODE_ELEMENTS;
		}

		OMIINO_FRAMER_ASSERT(0==pPM_KeyManagerRAM->Element[CompressedKey].IsActive,CompressedKey);

		pPM_KeyManagerRAM->Element[CompressedKey].IsActive					= 1;
		pPM_KeyManagerRAM->Element[CompressedKey].Next						= MAX_PERFORMANCE_MONITORING_KEY_MANAGER_NODE_ELEMENTS;
		pPM_KeyManagerRAM->Element[CompressedKey].Prev						= MAX_PERFORMANCE_MONITORING_KEY_MANAGER_NODE_ELEMENTS;

		OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_KEY_NODE_Clone(&(pPM_KeyManagerRAM->Element[CompressedKey].KeyNode), pPM_Engine_Key);
	}
	else
	{
		OMIINO_FRAMER_RSE(pPM_KeyManagerRAM->NextFree);
	}

    OMIINO_FRAMER_ASSERT(MAX_PERFORMANCE_MONITORING_KEY_MANAGER_NODE_ELEMENTS>CompressedKey,CompressedKey);

	return CompressedKey;
}


int OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_KeysMatch(OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE * pFirstPM_Engine_Key, OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE * pSecondPM_Engine_Key)
{
	int Result=1;

	if(pFirstPM_Engine_Key->KeyType!=pSecondPM_Engine_Key->KeyType)
	{
		Result=0;
	}

	if(Result)
	{
		switch(pSecondPM_Engine_Key->KeyType)
		{
			case	OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_PORT:
					if(pFirstPM_Engine_Key->u.LinePort.iPort!=pSecondPM_Engine_Key->u.LinePort.iPort)
					{
						Result=0;
					}
					else
					{
						if(pFirstPM_Engine_Key->u.LinePort.iDataPointIdentifier!=pSecondPM_Engine_Key->u.LinePort.iDataPointIdentifier)
						{
							Result=0;
						}
					}
					break;

			case	OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_HO:
					if(pFirstPM_Engine_Key->u.HO.iDataPointIdentifier!=pSecondPM_Engine_Key->u.HO.iDataPointIdentifier)
					{
						Result=0;
					}
					else
					{
						Result=OMIINO_FRAMER_SONET_SDH_ENDPOINTS_AreIdentical(&pFirstPM_Engine_Key->u.HO.LineEndpoint, &pSecondPM_Engine_Key->u.HO.LineEndpoint);
					}
			case	OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_LO:  
					if(pFirstPM_Engine_Key->u.LO.iDataPointIdentifier!=pSecondPM_Engine_Key->u.LO.iDataPointIdentifier)
					{
						Result=0;
					}
					else
					{
						Result=OMIINO_FRAMER_SONET_SDH_ENDPOINTS_AreIdentical(&pFirstPM_Engine_Key->u.LO.LineEndpoint, &pSecondPM_Engine_Key->u.LO.LineEndpoint);
					}
					break;

			case	OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_DISCRETE_CLIENT:
					Result=0; /* TODO no support for Discrete Client */
					break;

			case	OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_PDH:
					if(pFirstPM_Engine_Key->u.PDH.iDataPointIdentifier!=pSecondPM_Engine_Key->u.PDH.iDataPointIdentifier)
					{
						Result=0;
					}
					else
					{
						if(pFirstPM_Engine_Key->u.PDH.iPDHPort!=pSecondPM_Engine_Key->u.PDH.iPDHPort)
						{
							Result=0;
						}
					}
			default:
					Result=0;
					break;
		}

	}

	return Result;
}


int OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_GivenNodeKeyGetCompressedKey(OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_TYPE * pPM_KeyManagerRAM, OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE * pPM_Engine_Key, U16 * pCompressedKey)
{
	int Result=0;
	U16 i;
	
	*pCompressedKey=MAX_PERFORMANCE_MONITORING_KEY_MANAGER_NODE_ELEMENTS;

	for(i=0;i<MAX_PERFORMANCE_MONITORING_KEY_MANAGER_NODE_ELEMENTS;i++)
	{
		if(pPM_KeyManagerRAM->Element[i].IsActive)
		{
			if(OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_KeysMatch(pPM_Engine_Key, &pPM_KeyManagerRAM->Element[i].KeyNode))
			{
				Result=1;
				*pCompressedKey=i;
				break; /* out of loop */
			}
		}
	}

	return Result;
}

void OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_FreeNode(OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_TYPE * pPM_KeyManagerRAM, U16 CompressedKey)
{        
    OMIINO_FRAMER_ASSERT(NULL!=pPM_KeyManagerRAM,0);
    OMIINO_FRAMER_ASSERT(MAX_PERFORMANCE_MONITORING_KEY_MANAGER_NODE_ELEMENTS>CompressedKey,CompressedKey);
	OMIINO_FRAMER_ASSERT(1==pPM_KeyManagerRAM->Element[CompressedKey].IsActive,CompressedKey);

	if (MAX_PERFORMANCE_MONITORING_KEY_MANAGER_NODE_ELEMENTS==pPM_KeyManagerRAM->NextFree)
	{
		pPM_KeyManagerRAM->NextFree=CompressedKey;
		pPM_KeyManagerRAM->Element[CompressedKey].Next						= MAX_PERFORMANCE_MONITORING_KEY_MANAGER_NODE_ELEMENTS;
	}
	else
	{
		pPM_KeyManagerRAM->Element[pPM_KeyManagerRAM->NextFree].Prev		= CompressedKey;
		pPM_KeyManagerRAM->Element[CompressedKey].Next						= pPM_KeyManagerRAM->NextFree;
		pPM_KeyManagerRAM->NextFree											= CompressedKey;
	}

	pPM_KeyManagerRAM->Element[CompressedKey].Prev						= MAX_PERFORMANCE_MONITORING_KEY_MANAGER_NODE_ELEMENTS;

	pPM_KeyManagerRAM->Element[CompressedKey].IsActive					= 0;
	pPM_KeyManagerRAM->Element[CompressedKey].KeyNode.KeyType			= OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MAX_KEYS;
}


    



void OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_Initialize(OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_TYPE * pPM_KeyManagerRAM)
{
	U16 i;

    OMIINO_FRAMER_ASSERT(NULL!=pPM_KeyManagerRAM,0);

	for(i=0; i<MAX_PERFORMANCE_MONITORING_KEY_MANAGER_NODE_ELEMENTS; i++)
	{
		pPM_KeyManagerRAM->Element[i].IsActive			= 0;
		pPM_KeyManagerRAM->Element[i].KeyNode.KeyType	= OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MAX_KEYS;

		pPM_KeyManagerRAM->Element[i].Next				= i+1;

		if(0==i)
		{
			pPM_KeyManagerRAM->Element[i].Prev			= MAX_PERFORMANCE_MONITORING_KEY_MANAGER_NODE_ELEMENTS;
		}
		else
		{
			pPM_KeyManagerRAM->Element[i].Prev			= i-1;
		}
	}

    pPM_KeyManagerRAM->NextFree=0;
}
