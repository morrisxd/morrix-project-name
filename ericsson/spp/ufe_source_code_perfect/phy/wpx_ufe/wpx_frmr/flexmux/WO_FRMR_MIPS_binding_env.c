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

#include <stdio.h>
#include "WO_FRMR_private.h"
#include "WO_FRMR_MIPS_private.h"





char WPX_UFE_FRAMER_MIPS_Environment_HandleNameBuffer[128];
U8 WPX_UFE_FRAMER_MIPS_Environment_ServedHandle=0;
U8 WPX_UFE_FRAMER_MIPS_Environment_LastHandle=0;


void WPX_UFE_FRAMER_MIPS_EnvironmentTable_EnterCriticalRegion(U8 AnyHandle)
{
	WPX_UFE_FRAMER_MIPS_Environment_LastHandle=AnyHandle;
}


void WPX_UFE_FRAMER_MIPS_EnvironmentTable_LeaveCriticalRegion(U8 AnyHandle)
{
	WPX_UFE_FRAMER_MIPS_Environment_LastHandle=AnyHandle;
}



void WPX_UFE_FRAMER_MIPS_EnvironmentTable_CriticalRegionCreateHandle(U8 *pNewHandle, char * pHandleName)
{
	*pNewHandle=WPX_UFE_FRAMER_MIPS_Environment_ServedHandle++;
	strncpy(WPX_UFE_FRAMER_MIPS_Environment_HandleNameBuffer, pHandleName, 128);
}



void WPX_UFE_FRAMER_MIPS_EnvironmentTable_Trace(char * pTraceMessage)
{
	OMIINO_REMOVE_COMPILER_WARNING(pTraceMessage);
}




#define MAX_FATAL_ERROR_MESSAGE_BYTES_PER_SEGMENT	(32)
int IsFinalSegment(int BytesTransmitted, int BytesToTransmit)
{
	int Result=0;

	if(MAX_FATAL_ERROR_MESSAGE_BYTES_PER_SEGMENT+BytesTransmitted >=BytesToTransmit)
	{
		Result=1;
	}

	return Result;
}

int TransmitFinalSegment(char * pAnyMessageSegment, int BytesToTransmit)
{
	OMIINO_AUX_ResponseWithPayload(0, 999, WPX_UFE_FRAMER_OK, AUXILIARY_RESPONSE_FATAL_ERROR_FINAL_REPORT_SEGMENT, pAnyMessageSegment, (U8)BytesToTransmit);
	return BytesToTransmit;
}

int TransmitNextSegment(char * pAnyMessageSegment)
{
	OMIINO_AUX_ResponseWithPayload(0, 999, WPX_UFE_FRAMER_OK, AUXILIARY_RESPONSE_FATAL_ERROR_INTERMEDIATE_REPORT_SEGMENT, pAnyMessageSegment, MAX_FATAL_ERROR_MESSAGE_BYTES_PER_SEGMENT);
	return MAX_FATAL_ERROR_MESSAGE_BYTES_PER_SEGMENT;
}


void WPX_UFE_FRAMER_MIPS_EnvironmentTable_FatalErrorHandler(char * pFatalErrorMessage)
{
	int BytesToTransmit;
	int BytesTransmitted=0;

	BytesToTransmit=strlen(pFatalErrorMessage);
	
	while(BytesTransmitted<BytesToTransmit)
	{
		if(IsFinalSegment(BytesTransmitted,BytesToTransmit))
		{
			BytesTransmitted+=TransmitFinalSegment(&pFatalErrorMessage[BytesTransmitted], (U8)strlen(&pFatalErrorMessage[BytesTransmitted]));
		}
		else
		{
			BytesTransmitted+=TransmitNextSegment(&pFatalErrorMessage[BytesTransmitted]);
		}
	}
}





void WPX_UFE_FRAMER_MIPS_Initialize_BindingEnvironment(WPX_UFE_FRAMER_ENVIRONMENT_TABLE_TYPE * pEnvironmentTable)
{
	pEnvironmentTable->CriticalRegionCreateHandle=WPX_UFE_FRAMER_MIPS_EnvironmentTable_CriticalRegionCreateHandle;
	pEnvironmentTable->EnterCriticalRegion=WPX_UFE_FRAMER_MIPS_EnvironmentTable_EnterCriticalRegion;
	pEnvironmentTable->FatalErrorHandler=WPX_UFE_FRAMER_MIPS_EnvironmentTable_FatalErrorHandler;
	pEnvironmentTable->LeaveCriticalRegion=WPX_UFE_FRAMER_MIPS_EnvironmentTable_LeaveCriticalRegion;
	pEnvironmentTable->Trace=WPX_UFE_FRAMER_MIPS_EnvironmentTable_Trace;
}
