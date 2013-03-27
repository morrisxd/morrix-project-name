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

#include <stdlib.h>

#include "WO_FRMR_private.h"
#include "WO_FRMR_MIPS_private.h"



void WPX_UFE_FRAMER_MIPS_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_OMNISPY_RELAY_NORTHBOUND_DATAGRAM_EXIT(void)
{
	WPX_UFE_FRAMER_MIPS_Message("[OMIINO] Exit to local shell request received from OMNISHELL\n");
	exit(1); /* only one message currently */
}


void WPX_UFE_FRAMER_MIPS_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_OMNISPY_RELAY_NORTHBOUND_DATAGRAM_QC(void)
{
	QC();
}



void WPX_UFE_FRAMER_MIPS_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_OMNISPY_RELAY_NORTHBOUND_DATAGRAM_LIST_FACILITIES(U8 iDevice)
{
	U8 iLinePort;
	for(iLinePort=0;iLinePort<WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS;iLinePort++)
	{
		OMIINO_FRAMER_SONET_SDH_Port_List_Facilities(iDevice, iLinePort);
	}
	OMIINO_FRAMER_SOCKET_CLIENT_PDH_FacilitiesForDevice(iDevice, OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice)); 
}



void OmnispyCallbackTable_OmniSpyNorthBoundAnnounce(U8 iDevice, U32 * pMessage, U32 Length)
{
	U8 *pMessage8=(U8 *)pMessage;

	OMIINO_REMOVE_COMPILER_WARNING(iDevice);
	OMIINO_REMOVE_COMPILER_WARNING(Length);

	switch(pMessage8[16])
	{

		case	OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_OMNISPY_RELAY_NORTHBOUND_DATAGRAM_EXIT:
				WPX_UFE_FRAMER_MIPS_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_OMNISPY_RELAY_NORTHBOUND_DATAGRAM_EXIT();
				break;

		case	OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_OMNISPY_RELAY_NORTHBOUND_DATAGRAM_QC:
				WPX_UFE_FRAMER_MIPS_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_OMNISPY_RELAY_NORTHBOUND_DATAGRAM_QC();
				break;

		case	OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_OMNISPY_RELAY_NORTHBOUND_DATAGRAM_LIST_FACILITIES:
				WPX_UFE_FRAMER_MIPS_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_OMNISPY_RELAY_NORTHBOUND_DATAGRAM_LIST_FACILITIES(iDevice);
				break;

		default:
				printf("[OMIINO] OmnispyCallbackTable_OmniSpyNorthBoundAnnounce Default\n");
				break;
	}
}


void OmnispyCallbackTable_PreProductInitialization_Integrity(void)
{
}


void OmnispyCallbackTable_VirtualScreen_NextPage(void)
{
}



void OmnispyCallbackTable_VirtualScreen_PrevPage(void)
{
}



void OmnispyCallbackTable_VirtualScreen_Refresh(void)
{
}



void WPX_UFE_FRAMER_MIPS_Initialize_BindingOmnispyCallbackTable(WPX_UFE_FRAMER_OMNISPY_TABLE_TYPE * pOmnispyCallbackTable)
{
	pOmnispyCallbackTable->OmniSpyNorthBoundAnnounce=OmnispyCallbackTable_OmniSpyNorthBoundAnnounce;
	pOmnispyCallbackTable->PreProductInitialization.Integrity=OmnispyCallbackTable_PreProductInitialization_Integrity;
	pOmnispyCallbackTable->VirtualScreen.NextPage=OmnispyCallbackTable_VirtualScreen_NextPage;
	pOmnispyCallbackTable->VirtualScreen.PrevPage=OmnispyCallbackTable_VirtualScreen_PrevPage;
	pOmnispyCallbackTable->VirtualScreen.Refresh=OmnispyCallbackTable_VirtualScreen_Refresh;
}
