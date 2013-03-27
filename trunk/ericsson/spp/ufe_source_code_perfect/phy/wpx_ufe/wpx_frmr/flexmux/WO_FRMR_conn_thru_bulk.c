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


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC4_4C_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&SecondLineEndpoint, 2+iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C); 

	OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_CreateConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);

}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC4_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&SecondLineEndpoint, 2+iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4); 

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		FirstLineEndpoint.u.sdh.iJ=iJ;
		SecondLineEndpoint.u.sdh.iJ=iJ;
		OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_CreateConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC3_HI_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 iK;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&SecondLineEndpoint, 2+iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI); 

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		FirstLineEndpoint.u.sdh.iJ=iJ;
		SecondLineEndpoint.u.sdh.iJ=iJ;

		for(iK=0;iK<3;iK++)
		{
			FirstLineEndpoint.u.sdh.iK=iK;
			SecondLineEndpoint.u.sdh.iK=iK;
			OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_CreateConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
		}
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC3_LO_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 iK;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&SecondLineEndpoint, 2+iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO); 

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		FirstLineEndpoint.u.sdh.iJ=iJ;
		SecondLineEndpoint.u.sdh.iJ=iJ;

		for(iK=0;iK<3;iK++)
		{
			FirstLineEndpoint.u.sdh.iK=iK;
			SecondLineEndpoint.u.sdh.iK=iK;

			OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_CreateConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
		}
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC12_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&SecondLineEndpoint, 2+iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12); 

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		FirstLineEndpoint.u.sdh.iJ=iJ;
		SecondLineEndpoint.u.sdh.iJ=iJ;

		for(iK=0;iK<3;iK++)
		{
			FirstLineEndpoint.u.sdh.iK=iK;
			SecondLineEndpoint.u.sdh.iK=iK;

			for(iL=0;iL<7;iL++)
			{
				FirstLineEndpoint.u.sdh.iL=iL;
				SecondLineEndpoint.u.sdh.iL=iL;

				for(iM=0;iM<3;iM++)
				{
					FirstLineEndpoint.u.sdh.iM=iM;
					SecondLineEndpoint.u.sdh.iM=iM;
					OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_CreateConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
				}
			}
		}
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC11_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&SecondLineEndpoint, 2+iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11); 

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		FirstLineEndpoint.u.sdh.iJ=iJ;
		SecondLineEndpoint.u.sdh.iJ=iJ;

		for(iK=0;iK<3;iK++)
		{
			FirstLineEndpoint.u.sdh.iK=iK;
			SecondLineEndpoint.u.sdh.iK=iK;

			for(iL=0;iL<7;iL++)
			{
				FirstLineEndpoint.u.sdh.iL=iL;
				SecondLineEndpoint.u.sdh.iL=iL;

				for(iM=0;iM<4;iM++)
				{
					FirstLineEndpoint.u.sdh.iM=iM;
					SecondLineEndpoint.u.sdh.iM=iM;
					OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_CreateConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
				}
			}
		}
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_STS48C_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&SecondLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C); 

	OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_CreateConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);

}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_STS12C_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&SecondLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C); 

	OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_CreateConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_STS3C_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iU;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&SecondLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C); 


	for(iU=0;iU<MAX_iU;iU+=3)
	{
		FirstLineEndpoint.u.sonet.iU=iU;
		SecondLineEndpoint.u.sonet.iU=iU;
		OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_CreateConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_STS1_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iU;


	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&SecondLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1); 

	for(iU=0;iU<MAX_iU;iU++)
	{
		FirstLineEndpoint.u.sonet.iU=iU;
		SecondLineEndpoint.u.sonet.iU=iU;
		OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_CreateConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VT2_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iU;
	U8 iV;
	U8 iW;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&SecondLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2); 

	for(iU=0;iU<MAX_iU;iU++)
	{
		FirstLineEndpoint.u.sonet.iU=iU;
		SecondLineEndpoint.u.sonet.iU=iU;

		for(iV=0;iV<7;iV++)
		{
			FirstLineEndpoint.u.sonet.iV=iV;
			SecondLineEndpoint.u.sonet.iV=iV;

			for(iW=0;iW<3;iW++)
			{
				FirstLineEndpoint.u.sonet.iW=iW;
				SecondLineEndpoint.u.sonet.iW=iW;
				OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_CreateConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
			}
		}
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VT15_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iU;
	U8 iV;
	U8 iW;
 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&SecondLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15); 

	for(iU=0;iU<MAX_iU;iU++)
	{
		FirstLineEndpoint.u.sonet.iU=iU;
		SecondLineEndpoint.u.sonet.iU=iU;

		for(iV=0;iV<7;iV++)
		{
			FirstLineEndpoint.u.sonet.iV=iV;
			SecondLineEndpoint.u.sonet.iV=iV;

			for(iW=0;iW<4;iW++)
			{
				FirstLineEndpoint.u.sonet.iW=iW;
				SecondLineEndpoint.u.sonet.iW=iW;
				OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_CreateConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
			}
		}
	}
}


















void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC4_4C_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;


	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&SecondLineEndpoint, 2+iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C); 

	OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_DeleteConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC4_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&SecondLineEndpoint, 2+iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4); 

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		FirstLineEndpoint.u.sdh.iJ=iJ;
		SecondLineEndpoint.u.sdh.iJ=iJ;
		OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_DeleteConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC3_HI_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 iK;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&SecondLineEndpoint, 2+iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI); 

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		FirstLineEndpoint.u.sdh.iJ=iJ;
		SecondLineEndpoint.u.sdh.iJ=iJ;

		for(iK=0;iK<3;iK++)
		{
			FirstLineEndpoint.u.sdh.iK=iK;
			SecondLineEndpoint.u.sdh.iK=iK;
			OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_DeleteConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
		}
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC3_LO_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 iK;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&SecondLineEndpoint, 2+iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO); 

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		FirstLineEndpoint.u.sdh.iJ=iJ;
		SecondLineEndpoint.u.sdh.iJ=iJ;

		for(iK=0;iK<3;iK++)
		{
			FirstLineEndpoint.u.sdh.iK=iK;
			SecondLineEndpoint.u.sdh.iK=iK;

			OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_DeleteConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
		}
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC12_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&SecondLineEndpoint, 2+iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12); 


	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		FirstLineEndpoint.u.sdh.iJ=iJ;
		SecondLineEndpoint.u.sdh.iJ=iJ;

		for(iK=0;iK<3;iK++)
		{
			FirstLineEndpoint.u.sdh.iK=iK;
			SecondLineEndpoint.u.sdh.iK=iK;

			for(iL=0;iL<7;iL++)
			{
				FirstLineEndpoint.u.sdh.iL=iL;
				SecondLineEndpoint.u.sdh.iL=iL;


				for(iM=0;iM<3;iM++)
				{
					FirstLineEndpoint.u.sdh.iM=iM;
					SecondLineEndpoint.u.sdh.iM=iM;
					OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_DeleteConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
				}
			}
		}
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC11_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&SecondLineEndpoint, 2+iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11); 

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		FirstLineEndpoint.u.sdh.iJ=iJ;
		SecondLineEndpoint.u.sdh.iJ=iJ;

		for(iK=0;iK<3;iK++)
		{
			FirstLineEndpoint.u.sdh.iK=iK;
			SecondLineEndpoint.u.sdh.iK=iK;

			for(iL=0;iL<7;iL++)
			{
				FirstLineEndpoint.u.sdh.iL=iL;
				SecondLineEndpoint.u.sdh.iL=iL;

				for(iM=0;iM<4;iM++)
				{
					FirstLineEndpoint.u.sdh.iM=iM;
					SecondLineEndpoint.u.sdh.iM=iM;
					OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_DeleteConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
				}
			}
		}
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_STS48C_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&SecondLineEndpoint, 2+iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C); 

	OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_DeleteConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);

}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_STS12C_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&SecondLineEndpoint, 2+iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C); 

	OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_DeleteConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_STS3C_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iU;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&SecondLineEndpoint, 2+iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C); 

	for(iU=0;iU<MAX_iU;iU+=3)
	{
		FirstLineEndpoint.u.sonet.iU=iU;
		SecondLineEndpoint.u.sonet.iU=iU;
		OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_DeleteConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_STS1_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iU;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&SecondLineEndpoint, 2+iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1);


	for(iU=0;iU<MAX_iU;iU++)
	{
		FirstLineEndpoint.u.sonet.iU=iU;
		SecondLineEndpoint.u.sonet.iU=iU;
		OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_DeleteConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VT2_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iU;
	U8 iV;
	U8 iW;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&SecondLineEndpoint, 2+iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2);

	for(iU=0;iU<MAX_iU;iU++)
	{
		FirstLineEndpoint.u.sonet.iU=iU;
		SecondLineEndpoint.u.sonet.iU=iU;

		for(iV=0;iV<7;iV++)
		{
			FirstLineEndpoint.u.sonet.iV=iV;
			SecondLineEndpoint.u.sonet.iV=iV;

			for(iW=0;iW<3;iW++)
			{
				FirstLineEndpoint.u.sonet.iW=iW;
				SecondLineEndpoint.u.sonet.iW=iW;
				OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_DeleteConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
			}
		}
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VT15_Connection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iU;
	U8 iV;
	U8 iW;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&FirstLineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15); 
	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&SecondLineEndpoint, 2+iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15);

	for(iU=0;iU<MAX_iU;iU++)
	{
		FirstLineEndpoint.u.sonet.iU=iU;
		SecondLineEndpoint.u.sonet.iU=iU;

		for(iV=0;iV<7;iV++)
		{
			FirstLineEndpoint.u.sonet.iV=iV;
			SecondLineEndpoint.u.sonet.iV=iV;

			for(iW=0;iW<4;iW++)
			{
				FirstLineEndpoint.u.sonet.iW=iW;
				SecondLineEndpoint.u.sonet.iW=iW;
				OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_DeleteConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
			}
		}
	}
}
