/*****************************************************************************
 * (C) Copyright Wintegra 2009, All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra, Inc.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 *****************************************************************************/
/*****************************************************************************
 *
 * Example : 
 *
 * Description: Example calling program for the WinPath 3 memory stress test.
 *		Runs memory diagnostics and stress test on Parameter, Packet,
 *		and Host/Application RAM. If run under linux, the memory address
 *		and size definitions will be loaded automatically. If run under
 *		WinMon, the address and size definitions must be input manually
 * Input : 
 *
 * Output: 
 *
 * Setup :
 *
 *WDDI Version: This application was tested using WDDI 4.0.
 *****************************************************************************/
#define _STAND_ALONE_
//#define linux		//uncomment to run under linux
//#define WDS_SL	//uncomment to run under WP3 SL board
//#define FULL_SCAN     //uncomment to run full memory scan
//#define VERBOSE_MODE  //uncomment to unlock debugging messages
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "memtest.h"
#include "memtest.c"

#ifdef linux /*Stand Alone for Linux*/
#include "wpl_types.h"
#include <wpli_platform.h>
#include "wpi_veneer_memory.h"
//#include "wpv_veneer.h"

#else /*Stand Alone for NonLinux*/
#include "address.h"
#endif

/*-----------------------USER MODIFY ------------------------------*/
#ifndef FULL_SCAN /*Normal Repetitions*/
#define TEST_REPETITIONS	   0x50000	/*Number of test repetitions, NOTE: Full Scan takes SUBSTANTIALLY longer*/
#else /*FULL_SCAN Repetitions*/
#define TEST_REPETITIONS	   10		
#endif

#define WINFARMS		   4		/*Number of winfarms to test*/
/*-----------------------USER MODIFY ------------------------------*/

void main(int argc, char **argv)
{

	datum Phys_srcmem;
	datum Virt_srcmem;
	datum Phys_intmem;
	datum Virt_intmem;
	datum Virt_intRegmem;
	datum memSize;
	char *str1 = "Siu Parameter Test: ";
	char *str2 = "Siu Packet Test: ";
	char *str3 = "Siu Internal Test: ";
	char *str4 = "Siu Host Test: ";

#ifdef linux /*Stand Alone for Linux*/
#error linux_was_error
	wpi_veneer_memory o_bus_mapping[7];
	printf("Mapping Busses..\n");
	WPL_BusesMapGet(o_bus_mapping);	
	Phys_intmem = o_bus_mapping[WP_BUS_INTERNAL].start;
	Virt_intmem = Phys_intmem + o_bus_mapping[WP_BUS_INTERNAL].uncached_offset;
	Virt_intRegmem = o_bus_mapping[WPI_BUS_RIF].start + o_bus_mapping[WPI_BUS_RIF].uncached_offset;
	printf("Printing Bus Addresses: \n");
	printf("INTERNAL: Physical: %08X  Virtual: %08X SIZE: %08X \n", o_bus_mapping[WP_BUS_INTERNAL].start,\
		 o_bus_mapping[WP_BUS_INTERNAL].start + o_bus_mapping[WP_BUS_INTERNAL].uncached_offset, o_bus_mapping[WP_BUS_INTERNAL].size);
	printf("PARAM   : Physical: %08X  Virtual: %08X SIZE: %08X \n", o_bus_mapping[WP_BUS_PARAM].start,\
		 o_bus_mapping[WP_BUS_PARAM].start + o_bus_mapping[WP_BUS_PARAM].uncached_offset, o_bus_mapping[WP_BUS_PARAM].size);
#ifndef WDS_SL
#error SL_was_error
	printf("PACKET  : Physical: %08X  Virtual: %08X SIZE: %08X \n", o_bus_mapping[WP_BUS_PACKET].start,\
		 o_bus_mapping[WP_BUS_PACKET].start + o_bus_mapping[WP_BUS_PACKET].uncached_offset, o_bus_mapping[WP_BUS_PACKET].size);
#endif
	printf("HOST    : Physical: %08X  Virtual: %08X SIZE: %08X \n", o_bus_mapping[WP_BUS_HOST].start,\
		 o_bus_mapping[WP_BUS_HOST].start + o_bus_mapping[WP_BUS_HOST].uncached_offset, o_bus_mapping[WP_BUS_HOST].size);
	printf("RIF     : Physical: %08X  Virtual: %08X SIZE: %08X \n", o_bus_mapping[WPI_BUS_RIF].start,\
		 o_bus_mapping[WPI_BUS_RIF].start + o_bus_mapping[WPI_BUS_RIF].uncached_offset, o_bus_mapping[WPI_BUS_RIF].size);
	printf("\n\n");

#else	/*Stand Alone for NonLinux*/
	Phys_intmem = WEB_INTERNAL_BASE_PHYS;
	Virt_intmem = WEB_INTERNAL_BASE_VIRT;
	Virt_intRegmem = INTERNAL_REGISTERS_VIRT;
	printf("Printing Bus Addresses: \n");
	printf("INTERNAL: Physical: %08X  Virtual: %08X SIZE: %08X \n", WEB_INTERNAL_BASE_PHYS, WEB_INTERNAL_BASE_VIRT, WEB_INTERNAL_SIZE);
	printf("PARAM   : Physical: %08X  Virtual: %08X SIZE: %08X \n", WEB_PARAMRAM_BASE_PHYS, WEB_PARAMRAM_BASE_VIRT, WEB_PARAMRAM_SIZE);
#ifndef WDS_SL
	printf("PACKET  : Physical: %08X  Virtual: %08X SIZE: %08X \n", WEB_PACKETRAM_BASE_PHYS, WEB_PACKETRAM_BASE_VIRT, WEB_PACKETRAM_SIZE);
#endif
	printf("HOST    : Physical: %08X  Virtual: %08X SIZE: %08X \n", WEB_SYSTEMRAM_BASE_PHYS, WEB_SYSTEMRAM_BASE_VIRT, WEB_SYSTEMRAM_SIZE);
	printf("RIF     : Virtual : %08X \n",INTERNAL_REGISTERS_VIRT);
        printf("\n\n");
#endif

/*Test Host/Application bus*/
//////////////////////////////////////////////////////////////
#ifdef linux
	Phys_srcmem = o_bus_mapping[WP_BUS_HOST].start;
	Virt_srcmem = Phys_srcmem + o_bus_mapping[WP_BUS_HOST].uncached_offset;
	memSize = o_bus_mapping[WP_BUS_HOST].size;
#else
	Phys_srcmem = WEB_SYSTEMRAM_BASE_PHYS + USED_SYSTEMRAM_SIZE;
	Virt_srcmem = WEB_SYSTEMRAM_BASE_VIRT + USED_SYSTEMRAM_SIZE;
	memSize = WEB_SYSTEMRAM_SIZE - USED_SYSTEMRAM_SIZE - RESERVED_SYSTEMRAM_SPACE;
#endif
	Mem_Test(PROC_TEST, Phys_srcmem, Virt_srcmem, Phys_intmem, Virt_intmem, Virt_intRegmem, SIU_BUS_HOST_APP, TEST_REPETITIONS, WINFARMS,memSize,str4);
	Mem_Test(STRESS_TEST,Phys_srcmem,Virt_srcmem,Phys_intmem, Virt_intmem, Virt_intRegmem, SIU_BUS_HOST_APP, TEST_REPETITIONS, WINFARMS, memSize,str4);






#if 0
/*Test Internal Bus*/
//////////////////////////////////////////////////////////////
#ifdef linux
	Phys_srcmem = o_bus_mapping[WP_BUS_INTERNAL].start;
	Virt_srcmem = Phys_srcmem + o_bus_mapping[WP_BUS_INTERNAL].uncached_offset;	
	memSize = 0x00080000;//only test small portion of internal
#else
	Phys_srcmem = WEB_INTERNAL_BASE_PHYS;
	Virt_srcmem = WEB_INTERNAL_BASE_VIRT;	
	memSize = 0x00080000; //only test small portion of internal
#endif
	Mem_Test(PROC_TEST, Phys_srcmem, Virt_srcmem, Phys_intmem, Virt_intmem, Virt_intRegmem, SIU_BUS_INTERNAL, TEST_REPETITIONS, WINFARMS,memSize,str3);
	Mem_Test(STRESS_TEST,Phys_srcmem, Virt_srcmem, Phys_intmem, Virt_intmem, Virt_intRegmem, SIU_BUS_INTERNAL, TEST_REPETITIONS,WINFARMS,memSize,str3);
	printf("\n\n");
#endif


/*Test Parameter Bus*/
//////////////////////////////////////////////////////////////
#ifdef linux	
 	Phys_srcmem = o_bus_mapping[WP_BUS_PARAM].start;
	Virt_srcmem = Phys_srcmem + o_bus_mapping[WP_BUS_PARAM].uncached_offset;	
        memSize = o_bus_mapping[WP_BUS_PARAM].size;
#else
	Phys_srcmem = WEB_PARAMRAM_BASE_PHYS;
	Virt_srcmem = WEB_PARAMRAM_BASE_VIRT;
	memSize = WEB_PARAMRAM_SIZE;
#endif
	Mem_Test(PROC_TEST, Phys_srcmem, Virt_srcmem, Phys_intmem, Virt_intmem, Virt_intRegmem, SIU_BUS_PARAM, TEST_REPETITIONS, WINFARMS,memSize,str1);
	Mem_Test(STRESS_TEST, Phys_srcmem, Virt_srcmem, Phys_intmem, Virt_intmem, Virt_intRegmem, SIU_BUS_PARAM, TEST_REPETITIONS, WINFARMS,memSize,str1);
	printf("\n\n");

#if 0
/*Test Packet Bus*/
//////////////////////////////////////////////////////////////
#ifndef WDS_SL
#ifdef linux
	Phys_srcmem = o_bus_mapping[WP_BUS_PACKET].start;
	Virt_srcmem = Phys_srcmem + o_bus_mapping[WP_BUS_PACKET].uncached_offset;	
	memSize = o_bus_mapping[WP_BUS_PACKET].size;
#else	
	Phys_srcmem = WEB_PACKETRAM_BASE_PHYS;
	Virt_srcmem = WEB_PACKETRAM_BASE_VIRT;
	memSize = WEB_PACKETRAM_SIZE;
#endif
	Mem_Test(PROC_TEST, Phys_srcmem, Virt_srcmem, Phys_intmem, Virt_intmem, Virt_intRegmem, SIU_BUS_PACKET, TEST_REPETITIONS, WINFARMS,memSize,str2);
	Mem_Test(STRESS_TEST, Phys_srcmem, Virt_srcmem, Phys_intmem, Virt_intmem, Virt_intRegmem, SIU_BUS_PACKET, TEST_REPETITIONS, WINFARMS,memSize,str2);
	printf("\n\n");
#endif
#endif




}

