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


extern void (*EarlyFatalError)(int ErrorCode);

volatile int _WrapperRemoveCompilerWarning=0; 
#define OMIINO_FOREVER  for(;;){_WrapperRemoveCompilerWarning++;}

void OMIINO_ENVIRONMENT_FATAL_ERROR_HANDLER(int ErrorCode)
{
    if(EarlyFatalError)
    {
        (*EarlyFatalError)(ErrorCode);
    }
    OMIINO_FOREVER;
}





void OMIINO_FRAMER_ENVIRONMENT_WRAPPER_EnterCriticalRegion(U8 AnyHandle)
{
    if(OMIINO_MAGIC==OMIINO_RAM.EnvironmentWrapper.Magic)
    {
        if(OMIINO_RAM.EnvironmentWrapper.EnvironmentTable.EnterCriticalRegion)
        {
            (*OMIINO_RAM.EnvironmentWrapper.EnvironmentTable.EnterCriticalRegion)(AnyHandle);
        }
        else
        {
            OMIINO_ENVIRONMENT_FATAL_ERROR_HANDLER(1);
        }
    }
    else
    {
        OMIINO_ENVIRONMENT_FATAL_ERROR_HANDLER(2);
    }
}





void OMIINO_FRAMER_ENVIRONMENT_WRAPPER_LeaveCriticalRegion(U8 AnyHandle)
{
    if(OMIINO_MAGIC==OMIINO_RAM.EnvironmentWrapper.Magic)
    {
        if(OMIINO_RAM.EnvironmentWrapper.EnvironmentTable.LeaveCriticalRegion)
        {
            (*OMIINO_RAM.EnvironmentWrapper.EnvironmentTable.LeaveCriticalRegion)(AnyHandle);
        }
        else
        {
            OMIINO_ENVIRONMENT_FATAL_ERROR_HANDLER(3);
        }
    }
    else
    {
        OMIINO_ENVIRONMENT_FATAL_ERROR_HANDLER(4);
    }
}



void OMIINO_FRAMER_ENVIRONMENT_WRAPPER_CriticalRegionCreateHandle(U8 *pNewHandle, char * pHandleName)
{
    if(OMIINO_MAGIC==OMIINO_RAM.EnvironmentWrapper.Magic)
    {
        if(OMIINO_RAM.EnvironmentWrapper.EnvironmentTable.CriticalRegionCreateHandle)
        {
            (*OMIINO_RAM.EnvironmentWrapper.EnvironmentTable.CriticalRegionCreateHandle)(pNewHandle, pHandleName);
        }
        else
        {
            OMIINO_ENVIRONMENT_FATAL_ERROR_HANDLER(5);
        }
    }
    else
    {
        OMIINO_ENVIRONMENT_FATAL_ERROR_HANDLER(6);
    }
}


void OMIINO_FRAMER_ENVIRONMENT_WRAPPER_Trace(char *pTraceMessage)
{

    if(OMIINO_MAGIC==OMIINO_RAM.EnvironmentWrapper.Magic)
    {
        if(OMIINO_RAM.EnvironmentWrapper.EnvironmentTable.Trace)
        {    
            (*OMIINO_RAM.EnvironmentWrapper.EnvironmentTable.Trace)(pTraceMessage);
        }
    }
    else
    {
        OMIINO_ENVIRONMENT_FATAL_ERROR_HANDLER(7);
    }
}






void OMIINO_FRAMER_ENVIRONMENT_WRAPPER_FatalErrorHandler(char *pTraceMessage)
{

    if(OMIINO_MAGIC==OMIINO_RAM.EnvironmentWrapper.Magic)
    {
        if(OMIINO_RAM.EnvironmentWrapper.EnvironmentTable.FatalErrorHandler)
        {    
            (*OMIINO_RAM.EnvironmentWrapper.EnvironmentTable.FatalErrorHandler)(pTraceMessage);
        }
    }
    else
    {
        OMIINO_ENVIRONMENT_FATAL_ERROR_HANDLER(8);
    }
}




void OMIINO_FRAMER_ENVIRONMENT_WRAPPER_BindEnvironment(OMIINO_FRAMER_RAM_TYPE *pRAM, WPX_UFE_FRAMER_ENVIRONMENT_TABLE_TYPE *pEnvironment)
{
    pRAM->EnvironmentWrapper.EnvironmentTable.CriticalRegionCreateHandle=pEnvironment->CriticalRegionCreateHandle;
    pRAM->EnvironmentWrapper.EnvironmentTable.EnterCriticalRegion=pEnvironment->EnterCriticalRegion;
    pRAM->EnvironmentWrapper.EnvironmentTable.LeaveCriticalRegion=pEnvironment->LeaveCriticalRegion;
    pRAM->EnvironmentWrapper.EnvironmentTable.FatalErrorHandler=pEnvironment->FatalErrorHandler;
    pRAM->EnvironmentWrapper.EnvironmentTable.Trace=pEnvironment->Trace;
    pRAM->EnvironmentWrapper.Magic=OMIINO_MAGIC;

}



