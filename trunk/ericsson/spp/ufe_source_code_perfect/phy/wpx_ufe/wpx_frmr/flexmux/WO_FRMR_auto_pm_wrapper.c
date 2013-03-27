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


void OMIINO_PERFORMANCE_MONITORING_WRAPPER_BindCallbacks(OMIINO_PERFORMANCE_MONITORING_TABLE_PRIVATE_ANNOUNCE_TYPE *pTo, WPX_UFE_FRAMER_PERFORMANCE_MONITORING_TABLE_ANNOUNCE_TYPE *pFrom)
{
    pTo->DiscreteClientPortPrivatePerformanceMonitoringAnnounce.Callback=pFrom->DiscreteClientPortPerformanceMonitoringAnnounce;
    pTo->DiscreteClientPortPrivatePerformanceMonitoringAnnounce.EnterCount=0;
    pTo->DiscreteClientPortPrivatePerformanceMonitoringAnnounce.ExitCount=0;
    pTo->DiscreteClientPortPrivatePerformanceMonitoringAnnounce.UnBoundCalls=0;
    
    
    pTo->SONET_SDH_PrivateHighOrderPathPerformanceMonitoringAnnounce.Callback=pFrom->SONET_SDH_HighOrderPathPerformanceMonitoringAnnounce;
    pTo->SONET_SDH_PrivateHighOrderPathPerformanceMonitoringAnnounce.EnterCount=0;
    pTo->SONET_SDH_PrivateHighOrderPathPerformanceMonitoringAnnounce.ExitCount=0;
    pTo->SONET_SDH_PrivateHighOrderPathPerformanceMonitoringAnnounce.UnBoundCalls=0;
    
    
    pTo->SONET_SDH_PrivateLowOrderPathPerformanceMonitoringAnnounce.Callback=pFrom->SONET_SDH_LowOrderPathPerformanceMonitoringAnnounce;
    pTo->SONET_SDH_PrivateLowOrderPathPerformanceMonitoringAnnounce.EnterCount=0;
    pTo->SONET_SDH_PrivateLowOrderPathPerformanceMonitoringAnnounce.ExitCount=0;
    pTo->SONET_SDH_PrivateLowOrderPathPerformanceMonitoringAnnounce.UnBoundCalls=0;
    
    
    pTo->SONET_SDH_PrivatePortPerformanceMonitoringAnnounce.Callback=pFrom->SONET_SDH_PortPerformanceMonitoringAnnounce;
    pTo->SONET_SDH_PrivatePortPerformanceMonitoringAnnounce.EnterCount=0;
    pTo->SONET_SDH_PrivatePortPerformanceMonitoringAnnounce.ExitCount=0;
    pTo->SONET_SDH_PrivatePortPerformanceMonitoringAnnounce.UnBoundCalls=0;

	pTo->PDH_PrivatePerformanceMonitoringAnnounce.Callback=pFrom->PDH_PerformanceMonitoringAnnounce;
    pTo->PDH_PrivatePerformanceMonitoringAnnounce.EnterCount=0;
    pTo->PDH_PrivatePerformanceMonitoringAnnounce.ExitCount=0;
    pTo->PDH_PrivatePerformanceMonitoringAnnounce.UnBoundCalls=0;

}



void OMIINO_PERFORMANCE_MONITORING_WRAPPER_SONET_SDH_PORT_PERFORMANCE_MONITORING_ANNOUNCE_TYPE(U8 iDevice, U8 iTableIndex, U32 TimeStamp)
{
    if(OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.SONET_SDH_PrivatePortPerformanceMonitoringAnnounce.Callback)
    {
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.SONET_SDH_PrivatePortPerformanceMonitoringAnnounce.EnterCount++;
        (*OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.SONET_SDH_PrivatePortPerformanceMonitoringAnnounce.Callback)(iDevice, iTableIndex, TimeStamp);
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.SONET_SDH_PrivatePortPerformanceMonitoringAnnounce.ExitCount++;
    }
    else
    {
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.SONET_SDH_PrivatePortPerformanceMonitoringAnnounce.UnBoundCalls++;
    }
}



void OMIINO_PERFORMANCE_MONITORING_WRAPPER_API_SONET_SDH_HO_PATH_PERFORMANCE_MONITORING_ANNOUNCE_TYPE(U8 iDevice, U8 iTableIndex, U32 TimeStamp)
{
    if(OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.SONET_SDH_PrivateHighOrderPathPerformanceMonitoringAnnounce.Callback)
    {
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.SONET_SDH_PrivateHighOrderPathPerformanceMonitoringAnnounce.EnterCount++;
        (*OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.SONET_SDH_PrivateHighOrderPathPerformanceMonitoringAnnounce.Callback)(iDevice, iTableIndex, TimeStamp);
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.SONET_SDH_PrivateHighOrderPathPerformanceMonitoringAnnounce.ExitCount++;
    }
    else
    {
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.SONET_SDH_PrivateHighOrderPathPerformanceMonitoringAnnounce.UnBoundCalls++;
    }
}



void OMIINO_PERFORMANCE_MONITORING_WRAPPER_API_SONET_SDH_LO_PATH_PERFORMANCE_MONITORING_ANNOUNCE_TYPE(U8 iDevice, U8 iTableIndex, U32 TimeStamp)
{
    if(OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.SONET_SDH_PrivateLowOrderPathPerformanceMonitoringAnnounce.Callback)
    {
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.SONET_SDH_PrivateLowOrderPathPerformanceMonitoringAnnounce.EnterCount++;
        (*OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.SONET_SDH_PrivateLowOrderPathPerformanceMonitoringAnnounce.Callback)(iDevice, iTableIndex, TimeStamp);
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.SONET_SDH_PrivateLowOrderPathPerformanceMonitoringAnnounce.ExitCount++;
    }
    else
    {
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.SONET_SDH_PrivateLowOrderPathPerformanceMonitoringAnnounce.UnBoundCalls++;
    }
}



void OMIINO_PERFORMANCE_MONITORING_WRAPPER_API_DISCRETE_CLIENT_PERFORMANCE_MONITORING_ANNOUNCE_TYPE(U8 iDevice, U8 iTableIndex, U32 TimeStamp)
{
    if(OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.DiscreteClientPortPrivatePerformanceMonitoringAnnounce.Callback)
    {
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.DiscreteClientPortPrivatePerformanceMonitoringAnnounce.EnterCount++;
        (*OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.DiscreteClientPortPrivatePerformanceMonitoringAnnounce.Callback)(iDevice, iTableIndex, TimeStamp);
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.DiscreteClientPortPrivatePerformanceMonitoringAnnounce.ExitCount++;
    }
    else
    {
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.DiscreteClientPortPrivatePerformanceMonitoringAnnounce.UnBoundCalls++;
    }
}




void OMIINO_PERFORMANCE_MONITORING_WRAPPER_API_PDH_PERFORMANCE_MONITORING_ANNOUNCE_TYPE(U8 iDevice, U8 iTableIndex, U32 TimeStamp)
{
    if(OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.PDH_PrivatePerformanceMonitoringAnnounce.Callback)
    {
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.PDH_PrivatePerformanceMonitoringAnnounce.EnterCount++;
        (*OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.PDH_PrivatePerformanceMonitoringAnnounce.Callback)(iDevice, iTableIndex, TimeStamp);
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.PDH_PrivatePerformanceMonitoringAnnounce.ExitCount++;
    }
    else
    {
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable.PDH_PrivatePerformanceMonitoringAnnounce.UnBoundCalls++;
    }
}

