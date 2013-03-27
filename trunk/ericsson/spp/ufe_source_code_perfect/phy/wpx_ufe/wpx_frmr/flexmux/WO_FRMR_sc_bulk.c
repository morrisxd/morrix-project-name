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


U8 OMIINO_API_INTERNAL_BULK_SOCKET_INGRESS_SET_PRBS_TERM(U32 TransactionIdentifier, U8 iDevice, U8 PRBS_Pattern)
{
	U32 iSocketClientPDH;

	for(iSocketClientPDH=0;iSocketClientPDH<WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS;iSocketClientPDH++)
	{
		OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_Term_PRBS_Set(TransactionIdentifier, iDevice, iSocketClientPDH, PRBS_Pattern);
	}

	return WPX_UFE_FRAMER_OK;
}


U8 OMIINO_API_INTERNAL_BULK_SOCKET_INGRESS_SET_PRBS_GEN(U32 TransactionIdentifier, U8 iDevice, U8 PRBS_Pattern)
{
	U32 iSocketClientPDH;

	for(iSocketClientPDH=0;iSocketClientPDH<WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS;iSocketClientPDH++)
	{
		OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_Gen_PRBS_Set(TransactionIdentifier, iDevice, iSocketClientPDH, PRBS_Pattern);
	}

	return WPX_UFE_FRAMER_OK;
}


U8 OMIINO_API_INTERNAL_BULK_SOCKET_EGRESS_SET_PRBS_TERM(U32 TransactionIdentifier, U8 iDevice, U8 PRBS_Pattern)
{
	U32 iSocketClientPDH;

	for(iSocketClientPDH=0;iSocketClientPDH<WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS;iSocketClientPDH++)
	{
		OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_Term_PRBS_Set(TransactionIdentifier, iDevice, iSocketClientPDH, PRBS_Pattern);
	}

	return WPX_UFE_FRAMER_OK;
}


U8 OMIINO_API_INTERNAL_BULK_SOCKET_EGRESS_SET_PRBS_GEN(U32 TransactionIdentifier, U8 iDevice, U8 PRBS_Pattern)
{
	U32 iSocketClientPDH;

	for(iSocketClientPDH=0;iSocketClientPDH<WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS;iSocketClientPDH++)
	{
		OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_Gen_PRBS_Set(TransactionIdentifier, iDevice, iSocketClientPDH, PRBS_Pattern);
	}

	return WPX_UFE_FRAMER_OK;
}



U8 OMIINO_API_INTERNAL_BULK_SOCKET_CLIENT_PDH_Facility_Create(U32 TransactionIdentifier, U8 iDevice, U8 AnyClientType, U8 Framing, U8 Mapping, U8 ClockRecoveryMode, U8 AnyTimestampByteCount)
{
	U32 iSocketClientPDH;

	for(iSocketClientPDH=0;iSocketClientPDH<WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS;iSocketClientPDH++)
	{
		if(WPX_UFE_FRAMER_OK==OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_CreateFacility(TransactionIdentifier, iDevice, iSocketClientPDH, AnyClientType))
		{
			OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetFraming(TransactionIdentifier, iDevice, iSocketClientPDH, Framing);
			OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetMapping(TransactionIdentifier, iDevice, iSocketClientPDH, Mapping);
			OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetClockRecoveryMode(TransactionIdentifier, iDevice, iSocketClientPDH, ClockRecoveryMode);
			OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetTimestampByteCount(TransactionIdentifier, iDevice, iSocketClientPDH, AnyTimestampByteCount);
		}
	}

	return WPX_UFE_FRAMER_OK;
}


U8 OMIINO_API_INTERNAL_BULK_SOCKET_CLIENT_PDH_Facility_Delete(U32 TransactionIdentifier, U8 iDevice)
{
    U32 iSocketClientPDH;

	for(iSocketClientPDH=0;iSocketClientPDH<WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS;iSocketClientPDH++)
	{
		OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DeleteFacility(TransactionIdentifier, iDevice, iSocketClientPDH);
	}
	return WPX_UFE_FRAMER_OK;
}





