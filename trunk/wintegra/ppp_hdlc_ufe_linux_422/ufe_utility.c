/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *  Application : ATM PWE3 UFE4
 *  File Name   : ufe_utility.c
 *  Version     : 1
 *  Date        : 02 Oct 2011
 *  Accompanies : TBD
 *****************************************************************************/

/****************************************************************************
 *
 * Example: wti_ufe_utility.c
 * UFE demo utility functions for PWE3 CES application.
 * Demonstrates the use of UFE API.
 *
 ****************************************************************************/

#include <string.h>

#include "wufe_types.h"    
#include "wufe_errors.h"


#ifndef WTI_UFE_UTILITY_H
#include "ufe_utility.h"
#endif


#ifndef WTI_FLEXMUX_UTIL_H
#include "flexmux_util.h"
#endif

#ifndef WP_TYPES_H
#include "wp_types.h"
#endif

#ifndef WP_PROTOTYPES_H
#include "wp_prototypes.h"
#endif
/****************************************************************************************************************************
 * Function name: WT_UfeTerminateOnError()
 * Description  :   

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

void WT_UfeTerminateOnError(WUFE_status handle, WP_CHAR *s,WP_U32 id,WP_U32 LineNum)
{
	if(handle != WUFE_OK)
	{
		printf("Test Abort %s %d %s 0x%x Line:%d\n", s,id,
				WUFE_error_name[(handle&0x03ffffff)], handle, LineNum);

		exit(1);
	}

} // WT_UfeTerminateOnError

/****************************************************************************************************************************
 * Function name: WT_UfeLineSdhConfig()
 * Description  :   

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

static void WT_UfeLineSdhConfig(WT_ufe_line_sdh_params *line_sdh_params,
		WUFE_line_sdh *line_cfg)
{
	memset(line_cfg, 0, sizeof(WUFE_line_sdh));

	line_cfg->struct_id = WUFE_STRUCT_LINE_SDH;
	if (line_sdh_params->stm4 > 3)
	{
		printf("Invalid stm4 parameter, use 0-3 only \n");
		exit(1);
	}
	else line_cfg->stm4 = line_sdh_params->stm4;

	if (line_sdh_params->stm1 > 3)
	{
		printf("Invalid stm1 parameter, use 0-3 only \n");
		exit(1);
	}
	else line_cfg->stm1 = line_sdh_params->stm1;

	if (line_sdh_params->stm0 > 2)
	{
		printf("Invalid stm0 parameter, use 0-2 only \n");
		exit(1);
	}
	else line_cfg->stm0 = line_sdh_params->stm0;

	if (line_sdh_params->tug2 > 6)
	{
		printf("Invalid tug2 parameter,use 0-6 only \n");
		exit(1);
	}
	else line_cfg->tug2 = line_sdh_params->tug2;

	if (line_sdh_params->tu > 3)
	{
		printf("Invalid tu parameter,use 0-6 only \n");
		exit(1);
	}
	else if ((line_sdh_params->tu == 3)&&
			((line_sdh_params->transfer_type == WUFE_SDH_TYPE_E1) ||
			 (line_sdh_params->transfer_type == WUFE_SDH_TYPE_VC12)))
	{
		printf("Invalid tu parameter, slot 3 not valid for E1 or VC12\n");
		exit(1);
	}
	else line_cfg->tu = line_sdh_params->tu;

	line_cfg->transfer_type = line_sdh_params->transfer_type;
	line_cfg->framed = line_sdh_params->framed;
	line_cfg->cas_enable = line_sdh_params->cas_enable;

	/* Copy clock recovery line parameters */
	if (line_sdh_params->clock_rec_line_params != NULL)
	{
		WUFE_line_clock_rec_params *line_cr_params = line_sdh_params->clock_rec_line_params;
		if (line_cr_params->tx_clk_rec_enable !=0 && line_cr_params->tx_clk_rec_enable != 1)
		{
			printf("Invalid TX Clock Recovery mode\n");
			exit(1);
		}
		if (line_cr_params->clk_rec_physical_clock_enable != 0 && line_cr_params->clk_rec_physical_clock_enable != 1)
		{
			printf("Invalid Physical clock mode\n");
			exit(1);
		}
		if (line_cr_params->clk_rec_physical_clock_id < 0 || line_cr_params->clk_rec_physical_clock_id > 1)
		{
			printf("Invalid Physical clock ID, must be '0' or '1'\n");
			exit(1);
		}
		line_cfg->clock_rec_line_params = line_cr_params;
	}

} // WT_UfeLineSdhConfig

/****************************************************************************************************************************
 * Function name: WT_UfeLineSdhCreate()
 * Description  :   

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

void WT_UfeLineSdhCreate(WT_ufe *ufe,
		WP_U32 line_index,
		WT_ufe_line_sdh_params *line_sdh_params,
		WP_U8 enable)
{
	WUFE_status ufe_status;
	WUFE_line_sdh line_cfg;

	if (ufe->fpga_mode != WT_SDH)
	{
		printf("The UFE FPGA mode is not SDH. Can't create sdh line\n");
		exit(1);
	}

	/* Set the line configuration parameters */
	WT_UfeLineSdhConfig(line_sdh_params, &line_cfg);
	ufe_status = WUFE_LineCreate(&ufe->line_handle[line_index],
			ufe->ufe_id,
			WUFE_STRUCT_LINE_SDH,
			&line_cfg);
	WT_UfeTerminateOnError(ufe_status, "SDH WUFE_LineCreate", line_index, __LINE__);

	if (enable)
	{
		ufe_status = WUFE_LineEnable(ufe->line_handle[line_index], WUFE_FULL_DUPLEX);
		WT_UfeTerminateOnError(ufe_status, "SDH WUFE_LineEnable", line_index, __LINE__);
	}

} // WT_UfeLineSdhCreate

/****************************************************************************************************************************
 * Function name: WT_UfeLineCreate()
 * Description  :   

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/


void WT_UfeLineCreate(WT_ufe *ufe, WP_U32 index, void *config, WP_U8 enable)
{
    WT_ufe_line_sdh_params *line_params;
	int client_port, line_port;
	WTI_flexmux_connection_type con_type =0;    
	/* create the UFE line */
	WT_UfeLineSdhCreate(ufe, index, (WT_ufe_line_sdh_params*) config, enable);


	line_params = (WT_ufe_line_sdh_params *) config;


	client_port = WTI_FlexmuxClientPortIndexGet(index, line_params->transfer_type);
	line_port = WTI_FlexmuxLinePortIndexGet(index, ufe->ufe_id);
	if (line_port == -1)
	{
		printf("Illegal line port index %d\n",index);
		exit(1);
	}


	con_type = (line_params->framed == WUFE_FRAMED) ? WT_FLEXMUX_CONNECTION_TYPE_T1_FRAMED : WT_FLEXMUX_CONNECTION_TYPE_T1_UNFRAMED;

	/* save the UFE line configuration structure */
	memcpy(&ufe->line_params[index].framer_line_params.line_sdh_params, line_params, sizeof(WT_ufe_line_sdh_params));

	/* create connection on the Flexmux framer */
	WTI_FlexmuxConnectionCreate(0,
			line_port,
			client_port,
			con_type,
			config);


} // WT_UfeLineCreate

/****************************************************************************************************************************
 * Function name: WT_UfeLineConfigureAndCreateT1SDH()
 * Description  :   

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/


void WT_UfeLineConfigureAndCreateT1SDH(WP_U32 i, WP_U16 transfer_type, WP_U16 framing_mode, WP_U8 cas_enable, WP_U8 enable, WUFE_line_clock_rec_params *line_cr_params)
{

	WT_ufe_line_sdh_params line_params;

	memset(&line_params, 0, sizeof(WT_ufe_line_sdh_params));

	line_params.stm4 = i / 336;
	line_params.stm1 = (i % 336) / 84;
	line_params.stm0 = (i % 84) / 28;
	line_params.tug2 = (i % 28) / 4;
	line_params.tu = (i % 4);
	line_params.transfer_type = transfer_type;
	line_params.framed = framing_mode;
	line_params.cas_enable = cas_enable;
	if (line_cr_params != NULL)
		line_params.clock_rec_line_params = line_cr_params;

	WT_UfeLineCreate(&ufe, i, &line_params, enable);

} // WT_UfeLineConfigureAndCreateT1SDH
/****************************************************************************************************************************
 * Function name: WT_UfeLineSocketLpbkSetup()
 * Description  :   

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/


void WT_UfeLineSocketLpbkSetup(WT_ufe *ufe, int line_index, WUFE_line_loopback_type lpbk_type)
{
	WUFE_status ufe_status;

	ufe_status = WUFE_LineLoopbackConfigure(ufe->line_handle[line_index],lpbk_type);
	WT_UfeTerminateOnError(ufe_status, "WUFE_LineLoopbackConfigure" ,0,__LINE__);

} // WT_UfeLineSocketLpbkSetup

/****************************************************************************************************************************
 * Function name: WT_UfePhyTransConfig()
 * Description  :   

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

static void WT_UfePhyTransConfig(WT_ufe *ufe,
		WUFE_phy *phy_cfg,
		WP_U32 data_unit_size,
		WP_U32 n_slots,
		WP_U8 *slots_arr)
{
	WUFE_SlotGroup slot_group;

	/************************************
	 * Configure the UFE phy .
	 ************************************/
	memset(phy_cfg, 0, sizeof(WUFE_phy));

	phy_cfg->struct_id = WUFE_STRUCT_PHY_TRANS;
	phy_cfg->tx_fifo_param.transmit_th = 0x1;
	phy_cfg->tx_fifo_param.fast_mode = WUFE_PHY_FIFO_MODE_SLOW;
	phy_cfg->tx_fifo_param.wait_type = WUFE_PHY_TX_WAIT_TYPE_A;

	/* configure the slots */
	memset (&slot_group, 0, sizeof(WUFE_SlotGroup));
	slot_group.n_slots = n_slots;
	memcpy(slot_group.slot_arr, slots_arr, n_slots*sizeof(WP_U8));

	memcpy(&phy_cfg->rx_slot_group,&slot_group,sizeof(slot_group));
	memcpy(&phy_cfg->tx_slot_group,&slot_group,sizeof(slot_group));

	phy_cfg->type.trans.rx_data_unit_size = data_unit_size;

} // WT_UfePhyTransConfig
/****************************************************************************************************************************
 * Function name: App_ufeSystem()
 * Description  :   

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/


void WT_UfePhyTransCreate(WT_ufe *ufe,
		WP_U32 line_index,
		WP_U32 phy_index,
		WP_U32 n_slots,
		WP_U8 *slots_arr,
		WP_U32 ufe_dataunit_size)
{
	WUFE_phy phy_cfg;
	WUFE_status ufe_status;

	WT_UfePhyTransConfig(ufe, &phy_cfg, ufe_dataunit_size, n_slots, slots_arr);
	ufe_status = WUFE_PhyCreate(&ufe->phy_handle[phy_index],
			ufe->line_handle[line_index],
			&phy_cfg,
			WUFE_FULL_DUPLEX);
	WT_UfeTerminateOnError(ufe_status, "WUFE_PhyCreate", phy_index, __LINE__);

} // WT_UfePhyTransCreate

/****************************************************************************************************************************
 * Function name: WT_UfePhyEnable()
 * Description  :   

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

void WT_UfePhyEnable(WT_ufe *ufe, WP_U32 phy_index)
{
	WUFE_status ufe_status;

	ufe_status = WUFE_PhyEnable(ufe->phy_handle[phy_index], WUFE_FULL_DUPLEX);
	WT_UfeTerminateOnError(ufe_status, "WUFE_PhyEnable", phy_index, __LINE__);

} // WT_UfePhyEnable

/****************************************************************************************************************************
 * Function name: WT_UfePhyDelete()
 * Description  :   

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

void WT_UfePhyDelete(WT_ufe *ufe, WP_U32 phy_index)
{
	WP_handle phy_handle = ufe->phy_handle[phy_index];
	WUFE_status ufe_status=WUFE_OK;
	volatile WP_U32 timeout;

	if(phy_handle == WT_UFE_HANDLE_INVALID)
		return;

	timeout = 0;
	do
	{
		ufe_status = WUFE_PhyDelete(phy_handle);
		if (ufe_status != WUFE_OK && ufe_status != WUFE_ERR_PHY_FIFO_NOT_EMPTY)
			break;

		if (++timeout > 100000)
		{
			printf("---------------- Timeout -------------\n");
			break;
		}

		WP_Delay(10);
	}
	while (ufe_status != WUFE_OK);

	if(ufe_status != WUFE_OK)
	{
		WUFE_PhyDisplay(phy_handle);
		WT_UfeTerminateOnError(ufe_status,"WUFE_PhyDelete not complete", timeout ,__LINE__);
	}

	ufe->phy_handle[phy_index] = WT_UFE_HANDLE_INVALID;

} // WT_UfePhyDelete

/****************************************************************************************************************************
 * Function name: WT_UfeLineDelete()
 * Description  :   

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

void WT_UfeLineDelete(WT_ufe *ufe, WP_U32 index)
{
	WUFE_status ufe_status;
#ifdef WT_UFE_FRAMER
	int client_port;
#endif

	if(ufe->line_handle[index] == WT_UFE_HANDLE_INVALID)
		return;

	ufe_status = WUFE_LineDisable(ufe->line_handle[index], WUFE_FULL_DUPLEX);
	WT_UfeTerminateOnError(ufe_status, "WUFE_LineDisable", index, __LINE__);

	ufe_status = WUFE_LineDelete(ufe->line_handle[index]);
	WT_UfeTerminateOnError(ufe_status, "WUFE_LineDelete", index, __LINE__);

	ufe->line_handle[index] = WT_UFE_HANDLE_INVALID;


#ifdef WT_UFE_FRAMER
	/* delete connection on the Flexmux framer */
	if (ufe->fpga_mode == WT_SONET)
	{
		client_port = WTI_FlexmuxClientPortIndexGet(index, ufe->line_params[index].framer_line_params.line_sonet_params.transfer_type);
		WTI_FlexmuxConnectionDelete(0,
				client_port,
				&ufe->line_params[index].framer_line_params.line_sonet_params);
	}
	else if (ufe->fpga_mode == WT_SDH)
	{
		client_port = WTI_FlexmuxClientPortIndexGet(index, ufe->line_params[index].framer_line_params.line_sdh_params.transfer_type);
		WTI_FlexmuxConnectionDelete(0,
				client_port,
				&ufe->line_params[index].framer_line_params.line_sdh_params);
	}
	else
	{
		printf("The UFE FPGA mode is not supported %d\n", ufe->fpga_mode);
		exit(1);
	}

#endif
} // WT_UfeLineDelete

/****************************************************************************************************************************
 * Function name: WT_UfeRelease()
 * Description  :   

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

void  WT_UfeRelease(WT_ufe *ufe)
{
	WUFE_status ufe_status;

	ufe_status = WUFE_UfeRelease(ufe->ufe_id);
	WT_UfeTerminateOnError( ufe_status, "WUFE_UfeRelease (from WT_UfeRelease)",0,__LINE__);

} // WT_UfeRelease

/****************************************************************************************************************************
 * Function name: WT_UfeSystemConfig()
 * Description  :   

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

static void WT_UfeSystemConfig(WT_ufe *ufe, WUFE_system *sys_cfg)
{
	memset(sys_cfg, 0, sizeof(WUFE_system));

	if (ufe->fpga_mode == WT_SONET)
	{
		sys_cfg->line_interface = WUFE_INTERFACE_SONET;
	}
	else if (ufe->fpga_mode == WT_SDH)
	{
		sys_cfg->line_interface = WUFE_INTERFACE_SDH;
	}
	else if (ufe->fpga_mode == WT_OCTAL)
	{
		sys_cfg->line_interface = WUFE_INTERFACE_CAD_E1_T1;
	}
	else
	{
		WUFE_status ufe_status = 0x1;
		WT_UfeTerminateOnError(ufe_status, "HW not SONET or SDH", ufe->fpga_mode, __LINE__);
	}

	sys_cfg->max_line_number = ufe->max_lines;
	sys_cfg->max_phy_number = ufe->max_phys;

	sys_cfg->idle_pattern.tx_idle_byte_data_pattern=0x81;
	sys_cfg->idle_pattern.tx_idle_nibble_cas_pattern=0x4;

	sys_cfg->emphy.rx_parity_mode  = WUFE_RX_PARITY_EVEN;
	sys_cfg->emphy.tx_parity_mode  = WUFE_TX_PARITY_EVEN;
	sys_cfg->emphy.tx_parity_check = WUFE_TX_PARITY_IGNORE;

	sys_cfg->tdm_parity.rx_parity_mode  = WUFE_RX_PARITY_EVEN;
	sys_cfg->tdm_parity.tx_parity_mode  = WUFE_TX_PARITY_EVEN;

	sys_cfg->fifo_manager.loopback = WUFE_FM_LOOPBACK_DISABLE;
	sys_cfg->max_buff_num = WUFE_MAX_BUF_SIZE_64;

	/* If set, enable CR at system level */
	sys_cfg->system_clock_rec_enable = ufe->system_clock_rec_enable;
	sys_cfg->clk_rec_system_params = ufe->clk_rec_system_params;

} // WT_UfeSystemConfig
/****************************************************************************************************************************
 * Function name: WT_UfeSystemCreate()
 * Description  :   Initialize and configure the UFE.

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/


void  WT_UfeSystemCreate(WT_ufe *ufe)
{
	WUFE_system sys_cfg;
	WUFE_status ufe_status;
	WUFE_init_config config;

	memset(&config, 0, sizeof(WUFE_init_config));
	config.ufe_id = ufe->ufe_id;

	if(ufe->upi_id == WP_PORT_UPI2)
		config.attributes |= WUFE_ATTRIB_CONNECT_OFFSET_F(1);

	/* Initialize the UFE*/
	ufe_status = WUFE_UfeInit(&config);
	WT_UfeTerminateOnError(ufe_status,"WUFE_UfeInit",0,__LINE__);

	/************************************
	 * Configure the UFE system.
	 ************************************/

	WT_UfeSystemConfig(ufe, &sys_cfg);

	ufe_status = WUFE_SystemConfig(ufe->ufe_id, &sys_cfg);
	WT_UfeTerminateOnError(ufe_status, "WUFE_SystemConfig  ",0,__LINE__);

} // WT_UfeSystemCreate

/****************************************************************************************************************************
 * Function name: WT_UfeInitializeFirmware()
 * Description  :   

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

void WT_UfeInitializeFirmware(WP_U32 ufe_id)  // burns the synthesis (func #2 - after WP_SysInit() )
{
	WUFE_init_config config;

	memset(&config, 0, sizeof(WUFE_init_config));
	config.ufe_id = ufe_id;
	if(WTI_EMPHY_PORT == WP_PORT_UPI2)
		config.attributes |= WUFE_ATTRIB_CONNECT_OFFSET_F(1);

	WUFE_UfeInitializeFirmware(&config);

} // WT_UfeInitializeFirmware
/****************************************************************************************************************************
 * Function name: WT_UfeSystemEnable()
 * Description  :   

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/


void  WT_UfeSystemEnable(WT_ufe *ufe)
{
	WUFE_status ufe_status;
	WP_U32 property, i;

	/* Enable UFE system */
	property = WUFE_SYS_EMPHY;
	ufe_status = WUFE_SystemEnable(ufe->ufe_id, property, WUFE_FULL_DUPLEX);
	WT_UfeTerminateOnError(ufe_status, "WUFE_SystemEnable ",0,__LINE__);

	for (i=0; i<MAX_UFE_LINES_USED; i++)
		ufe->line_handle[i] = WT_UFE_HANDLE_INVALID;

	for (i=0; i<N_MAX_EMPHY_PHYS; i++)
		ufe->phy_handle[i] = WT_UFE_HANDLE_INVALID;

} // WT_UfeSystemEnable

/****************************************************************************************************************************
 * Function name: WT_UfePhyDisable()
 * Description  :   

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

void WT_UfePhyDisable(WT_ufe *ufe, WP_U32 phy_index)
{
	WUFE_status ufe_status;

	if(ufe->phy_handle[phy_index] == WT_UFE_HANDLE_INVALID)
		return;

	ufe_status = WUFE_PhyDisable(ufe->phy_handle[phy_index], WUFE_FULL_DUPLEX);
	WT_UfeTerminateOnError(ufe_status, "WUFE_PhyDisable", phy_index, __LINE__);

} // WT_UfePhyDisable

