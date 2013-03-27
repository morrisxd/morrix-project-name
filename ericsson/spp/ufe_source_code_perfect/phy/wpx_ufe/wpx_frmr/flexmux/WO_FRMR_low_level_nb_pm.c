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


void OMIINO_LLC_Northbound_PerformanceMonitoringMessage(U8 iDevice, OMIINO_FRAMER_RAM_TYPE * pFramerRAM)
{
    U32 MessageIdentifier=0;
    U8 * pMessage;
    U8  Length;
    U32 U32_Length;

	pMessage=(U8 *)pFramerRAM->Device[iDevice].LLC_NorthboundBuffer.Buffer;
    U32_Length=16+UtilityGetU32(&pMessage[8]);

    if(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX>=U32_Length)
    {
        Length=(U8)U32_Length;

        if(4<Length)
        {
            MessageIdentifier=UtilityGetU32(&pMessage[0]);

            if(OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_PERFORMANCE_MONITORING_MAX_MESSAGES>MessageIdentifier)
            {
				(*pFramerRAM->LLC_Table.PerformanceMonitoringTable.Parser[MessageIdentifier])(MessageIdentifier, iDevice, &pFramerRAM->Device[iDevice].Hierarchy, pMessage, Length);
            }
            else
            {
                if((unsigned int)OMIINO_MESSAGE_CATALOGUE_NULL_MESSAGE!=MessageIdentifier)
                {
                    OMIINO_LLC_Northbound_PerformanceMonitoring_ErrorHandler(MessageIdentifier, iDevice, &pFramerRAM->Device[iDevice].Hierarchy, pMessage, Length, __LINE__, __FILE__);
                }
            }
        }
        else
        {
            OMIINO_LLC_Northbound_PerformanceMonitoring_ErrorHandler(0x0000DEAD, iDevice, &pFramerRAM->Device[iDevice].Hierarchy, pMessage, Length, __LINE__, __FILE__);
        }
    }
    else
    {
        OMIINO_LLC_Northbound_PerformanceMonitoring_ErrorHandler(0x0000DEAD, iDevice, &pFramerRAM->Device[iDevice].Hierarchy, pMessage, 0xFF, __LINE__, __FILE__);
    }
}



void OMIINO_LLC_Northbound_PerformanceMonitoring_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_PERFORMANCE_MONITORING_LINE_PORT(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
    U8  Result=1;
    U8  iNextU8=16;
    U32 iReport;
    U32 N_Reports=0;
    U32 TimeStamp=0;
    U16 CompressedKey;
    U32 Count;
    
    OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE PM_Engine_Key;

    TimeStamp=UtilityGetU32(&pMessage[iNextU8]);
    iNextU8+=4;
  
    N_Reports=UtilityGetU32(&pMessage[iNextU8]);
    iNextU8+=4;
    if(MAX_PM_LINE_PORT_COUNTS_PER_MESSAGE<N_Reports)
    {
        Result=0;
        OMIINO_LLC_Northbound_PerformanceMonitoring_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
    }        

    if(Result)
    {
        for(iReport=0;iReport<N_Reports;iReport++)
        {
            CompressedKey=UtilityGetU16(&pMessage[iNextU8]);
            iNextU8+=2;

            Count=UtilityGetU32(&pMessage[iNextU8]);
			iNextU8+=4;

			if(OMIINO_FRAMER_PerformanceMonitoring_KeyManager_Given_CompressedKey_Get_PM_EngineKey(&(pHierarchyRAM->PerformanceMonitoringKeyManager), CompressedKey, &PM_Engine_Key))
			{

				OMIINO_FRAMER_PerformanceMonitoringEngine_LinePort_DataPointAnnounce(   iDevice, 
																						&OMIINO_RAM.PerformanceMonitoringDeviceTable,
																						&pHierarchyRAM->PerformanceMonitoring,
																						TimeStamp,
																						&PM_Engine_Key,
																						Count);
			}
			else
			{
				Environment_Screen_PM_Port_Header("No Key ", (U32)CompressedKey);
			}
        }
    }
}



void OMIINO_LLC_Northbound_PerformanceMonitoring_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_PERFORMANCE_MONITORING_HO(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
    U8  Result=1;
    U8  iNextU8=16;
    U32 iReport;
    U32 N_Reports=0;
    U32 TimeStamp=0;
    U32 Report;
    U16 CompressedKey;
    U16 Count;
    
    OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE PM_Engine_Key;

    TimeStamp=UtilityGetU32(&pMessage[iNextU8]);
    iNextU8+=4;
  
    N_Reports=UtilityGetU32(&pMessage[iNextU8]);
    iNextU8+=4;
    if(MAX_HO_COUNTS_PER_MESSAGE<N_Reports)
    {
        Result=0;
        OMIINO_LLC_Northbound_PerformanceMonitoring_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
    }  

    if(Result)
    {
        for(iReport=0;iReport<N_Reports;iReport++)
        {
            Report=UtilityGetU32(&pMessage[iNextU8]);
            iNextU8+=4;

            CompressedKey=(((0xFFFF0000&Report)>>16)&0x0000FFFF);
            Count=(0x0000FFFF&Report);

            if(OMIINO_FRAMER_PerformanceMonitoring_KeyManager_Given_CompressedKey_Get_PM_EngineKey(&(pHierarchyRAM->PerformanceMonitoringKeyManager), CompressedKey, &PM_Engine_Key))
			{

				OMIINO_FRAMER_PerformanceMonitoringEngine_HO_DataPointAnnounce( iDevice, 
																				&OMIINO_RAM.PerformanceMonitoringDeviceTable,
																				&pHierarchyRAM->PerformanceMonitoring,
																				TimeStamp,
																				&PM_Engine_Key,
																				Count);
			}
        }
    }
}



void OMIINO_LLC_Northbound_PerformanceMonitoring_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_PERFORMANCE_MONITORING_LO(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
	U8  Result=1;
    U8  iNextU8=16;
    U32 iReport;
    U32 N_Reports=0;
    U32 TimeStamp=0;
    U32 Report;
    U16 CompressedKey;
    U16 Count;
    
    OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE PM_Engine_Key;

    TimeStamp=UtilityGetU32(&pMessage[iNextU8]);
    iNextU8+=4;
  
    N_Reports=UtilityGetU32(&pMessage[iNextU8]);
    iNextU8+=4;
    if(MAX_LO_COUNTS_PER_MESSAGE<N_Reports)
    {
        Result=0;
        OMIINO_LLC_Northbound_PerformanceMonitoring_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
    }  

    if(Result)
    {
        for(iReport=0;iReport<N_Reports;iReport++)
        {
            Report=UtilityGetU32(&pMessage[iNextU8]);
            iNextU8+=4;

            CompressedKey=(((0xFFFF0000&Report)>>16)&0x0000FFFF);
            Count=(0x0000FFFF&Report);

            if(OMIINO_FRAMER_PerformanceMonitoring_KeyManager_Given_CompressedKey_Get_PM_EngineKey(&(pHierarchyRAM->PerformanceMonitoringKeyManager), CompressedKey, &PM_Engine_Key))
			{

				OMIINO_FRAMER_PerformanceMonitoringEngine_LO_DataPointAnnounce( iDevice, 
																				&OMIINO_RAM.PerformanceMonitoringDeviceTable,
																				&pHierarchyRAM->PerformanceMonitoring,
																				TimeStamp,
																				&PM_Engine_Key,
																				Count);
			}
        }
    }
}





void OMIINO_LLC_Northbound_PerformanceMonitoring_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_PERFORMANCE_MONITORING_PDH(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
    U8  Result=1;
    U8  iNextU8=16;
    U32 iReport;
    U32 N_Reports=0;
    U32 TimeStamp=0;
    U32 Report;
    U16 CompressedKey;
    U16 Count;
    
    OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE PM_Engine_Key;

    if(Length<28)  /* No Transaction Identifier with Autonomous Signalling Messages */
    {
        Result=0;
        OMIINO_LLC_Northbound_PerformanceMonitoring_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
    }

    if(Result)
    {
        TimeStamp=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;
       
        if(4<=(Length-iNextU8))
        {
            N_Reports=UtilityGetU32(&pMessage[iNextU8]);
            iNextU8+=4;
            if(MAX_LO_COUNTS_PER_MESSAGE<N_Reports)/*  TODO */
            {
                Result=0;
                OMIINO_LLC_Northbound_PerformanceMonitoring_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__); /* TODO */
            }
        }
        else
        {
            Result=0;
            OMIINO_LLC_Northbound_PerformanceMonitoring_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
        }

        
    }

    if(Result)
    {
        for(iReport=0;iReport<N_Reports;iReport++)
        {
            if(Length>iNextU8)
            {
                Report=UtilityGetU32(&pMessage[iNextU8]);
                iNextU8+=4;

                CompressedKey=(((0xFFFF0000&Report)>>16)&0x0000FFFF);
                Count=(0x0000FFFF&Report);

                if(OMIINO_FRAMER_PerformanceMonitoring_KeyManager_Given_CompressedKey_Get_PM_EngineKey(&(pHierarchyRAM->PerformanceMonitoringKeyManager), CompressedKey, &PM_Engine_Key))
				{

					OMIINO_FRAMER_PerformanceMonitoringEngine_PDH_DataPointAnnounce(	iDevice, 
			                                                                            &OMIINO_RAM.PerformanceMonitoringDeviceTable,
				                                                                        &pHierarchyRAM->PerformanceMonitoring,
					                                                                    TimeStamp,
						                                                                &PM_Engine_Key,
							                                                            Count);
				}
            }
            else
            {
                OMIINO_LLC_Northbound_PerformanceMonitoring_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
                break;
            }
        }
    }
}



void OMIINO_LLC_Northbound_PerformanceMonitoring_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_PERFORMANCE_MONITORING_DISCRETE_CLIENT(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
    U8  Result=1;
    U8  iNextU8=16;
    U32 iReport;
    U32 N_Reports=0;
    U32 TimeStamp=0;
    U32 Report;
    U16 CompressedKey;
    U16 Count;
    
    OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE PM_Engine_Key;

    if(Length<28)  /* No Transaction Identifier with Autonomous Signalling Messages */
    {
        Result=0;
        OMIINO_LLC_Northbound_PerformanceMonitoring_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
    }

    if(Result)
    {
        TimeStamp=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;
       
        if(4<=(Length-iNextU8))
        {
            N_Reports=UtilityGetU32(&pMessage[iNextU8]);
            iNextU8+=4;
            if(MAX_LO_COUNTS_PER_MESSAGE<N_Reports)/*  TODO */
            {
                Result=0;
                OMIINO_LLC_Northbound_PerformanceMonitoring_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__); /* TODO */
            }
            else
            {
                Result=0;
                OMIINO_LLC_Northbound_PerformanceMonitoring_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
            }
        }
        else
        {
            Result=0;
            OMIINO_LLC_Northbound_PerformanceMonitoring_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
        }

        
    }

    if(Result)
    {
        for(iReport=0;iReport<N_Reports;iReport++)
        {
            if(Length>iNextU8)
            {
                Report=UtilityGetU32(&pMessage[iNextU8]);
                iNextU8+=4;

                CompressedKey=(((0xFFFF0000&Report)>>16)&0x0000FFFF);
                Count=(0x0000FFFF&Report);

                if(OMIINO_FRAMER_PerformanceMonitoring_KeyManager_Given_CompressedKey_Get_PM_EngineKey(&(pHierarchyRAM->PerformanceMonitoringKeyManager), CompressedKey, &PM_Engine_Key))
				{

					OMIINO_FRAMER_PerformanceMonitoringEngine_DiscreteClient_DataPointAnnounce(     iDevice, 
						                                                                            &OMIINO_RAM.PerformanceMonitoringDeviceTable,
							                                                                        &pHierarchyRAM->PerformanceMonitoring,
								                                                                    TimeStamp,
									                                                                &PM_Engine_Key,
										                                                            Count);
				}
            }
            else
            {
                OMIINO_LLC_Northbound_PerformanceMonitoring_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
                break;
            }
        }
    }
}




void OMIINO_LLC_Northbound_PerformanceMonitoring_NULL(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
    OMIINO_LLC_Northbound_PerformanceMonitoring_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
}




void OMIINO_LLC_Northbound_PerformanceMonitoring_IGNORE(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
    OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
    OMIINO_REMOVE_COMPILER_WARNING(pHierarchyRAM);
    OMIINO_REMOVE_COMPILER_WARNING(pMessage);
    OMIINO_REMOVE_COMPILER_WARNING(Length);
}


void OMIINO_LLC_Northbound_PerformanceMonitoringTableInitialize(OMNISPY_PM_TABLE_TYPE * pPerformanceMonitoringTable)
{
    int i;

    for(i=0;i<OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_PERFORMANCE_MONITORING_MAX_MESSAGES;i++)
    {
        pPerformanceMonitoringTable->Parser[i]=OMIINO_LLC_Northbound_PerformanceMonitoring_NULL;
    }

    pPerformanceMonitoringTable->Parser[OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_PERFORMANCE_MONITORING_DEFAULT_RAM_IGNORE   ]=OMIINO_LLC_Northbound_PerformanceMonitoring_IGNORE;
    pPerformanceMonitoringTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_PERFORMANCE_MONITORING_LINE_PORT            ]=OMIINO_LLC_Northbound_PerformanceMonitoring_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_PERFORMANCE_MONITORING_LINE_PORT;
    pPerformanceMonitoringTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_PERFORMANCE_MONITORING_HO                   ]=OMIINO_LLC_Northbound_PerformanceMonitoring_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_PERFORMANCE_MONITORING_HO;
    pPerformanceMonitoringTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_PERFORMANCE_MONITORING_LO                   ]=OMIINO_LLC_Northbound_PerformanceMonitoring_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_PERFORMANCE_MONITORING_LO;
    pPerformanceMonitoringTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_PERFORMANCE_MONITORING_DISCRETE_CLIENT      ]=OMIINO_LLC_Northbound_PerformanceMonitoring_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_PERFORMANCE_MONITORING_DISCRETE_CLIENT;
    pPerformanceMonitoringTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_PERFORMANCE_MONITORING_PDH                  ]=OMIINO_LLC_Northbound_PerformanceMonitoring_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_PERFORMANCE_MONITORING_PDH;
}

       
              
              
 
    
