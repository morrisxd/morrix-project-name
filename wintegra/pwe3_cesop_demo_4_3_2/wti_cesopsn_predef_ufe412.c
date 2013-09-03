/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

extern void WTI_TerminateOnError(WP_handle handle, WP_CHAR *s, WP_U32 line_num);

/*****************************************************************************/
/*********************************   SONET/SDH   *********************************/
/*****************************************************************************/
#define WT_FIRST_LINE_INDEX    0
#if WTI_DUAL_EMPHY
#define WT_MAX_LINE_INDEX      672
#else
#define WT_MAX_LINE_INDEX      336
#endif
#define WT_NUM_OF_LINES_VC12   1
#define WT_NUM_OF_LINES_VC11   1
#define WT_NUM_OF_LINES_CAD_E1 64
#define WT_NUM_OF_LINES_CAD_T1 64
#define WT_NUM_OF_LINES_E3     48
#define WT_NUM_OF_LINES_DS3    48
#define WT_NUM_OF_LINES_VC3    48
#define WT_NUM_OF_LINES_VC4    16
#define WT_NUM_OF_LINES_VC44C  4

#define WTI_VC11_SLOTS 1
#define WTI_VC12_SLOTS 1
#define WTI_VC3_SLOTS 1
#define WTI_VC4_SLOTS 1
#define WTI_VC44C_SLOTS 1

/* Clock Rec defines */
#define WT_LINE_INDEX_TO_FIRST_DCO  0
#define WT_LINE_INDEX_TO_SECOND_DCO 1


#define WTI_ITERATIONS_NUM     4
#define WTI_CHECK_PORT_SWITCHING 0

/* Socket loopback mode  */
#ifdef WT_UFE_FRAMER
#define WTI_SOCKET_LOOPBACK_MODE     WUFE_LINE_LOOPBACK_TYPE_NONE
#else  /* WT_UFE_FRAMER is not defined */
#define WTI_SOCKET_LOOPBACK_MODE     WUFE_LINE_LOOPBACK_TYPE_SYSTEM
#endif /* WT_UFE_FRAMER is not defined */

#ifdef WT_UFE_FRAMER
#ifndef WTI_FLEXMUX_UTIL_H
#include "wti_flexmux_util.c"
#endif
#endif

#if WTI_CLOCK_REC_MODE
#define DELAY_DURATION_IN_SECONDS 15
#else
#define DELAY_DURATION_IN_SECONDS (3*60)
#endif


WUFE_status WT_UfeLinePWCreate(WUFE_test_system_setup *test_setup, WP_U32 create_line);
void WT_UfeMPLS_L2_FA_Create(void);

#if WTI_CESOP_MPLS_IW
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_FlexmuxInit(char *StrPrm)
{
#ifdef WT_UFE_FRAMER

   WP_U32 res, cmd, Personality;

   /*
    *
    * Note on Personality parameter:-
    *
    *
    * For WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12 enter   7
    * For WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3 enter        8
    * For WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3 enter        15
    *
    * For a full list of build personalities see WPX_UFE_FRAMER_API_public.h
    *
    */

   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",0);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /* Calls UFE system create and enable */
   WTI_DemoConfigStart();

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d", &cmd, &Personality);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   /* Initialize the Flexmux framer in SDH mode */
   WPX_UFE_FRAMER_MIPS_StartDriver(Personality);
#endif
}

#ifdef WT_UFE_FRAMER
void WTI_enable_alarms_and_performance_monitoring(int type)
{
   extern void WTI_enable_alarms(int type);
   extern void WTI_enable_performance_monitoring(int type);

   WTI_enable_alarms(type);
   WTI_enable_performance_monitoring(type);
}
#endif

void WT_UfeMPLS_L2_FA_Create(void)
{
	WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
	WP_U32 i;

	for (i=0;i<WTI_MAX_NUM_OF_MPLS_FA;++i)
	{
#if WTI_CESOP_MPLS_OVER_ENET
		/* create the l2 router MPLS flow aggregations */
		CLI_F_Tdm2PsnFlowAggEnetHeaderMpls("0 aa aa aa aa aa aa bb bb bb bb bb bb 8847");
		CLI_F_MplsFlowAggPrefixLength("0 18");
		sprintf(temp_buf,
				"0 %x %x %x %x %x %x",
				3*i,
				3*i+1,
				3*i+2,
				0x81000005,
				(0x88470000 | ((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) >> 4)),
				((((((WTI_MPLS_LABEL) >> 12)  + WTI_MAX_PW + 1 + i) & 0xff) << 28) | 0x00ff0000));
		/*                              [mac destination] [mac source] [vlan] [type] [mpls header] */
		CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
		sprintf(temp_buf, "0 %d %d %d", 2, 28, 41); /* remark offsets for VLAN priority and PSN tunnel EXP bits */

		CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
		sprintf(temp_buf, "0 %d", i);
		CLI_F_MplsFlowAggCreate(temp_buf);
#else
#if WTI_CESOP_NATIVE_MPLS_IW
		/* create the l2 router MPLS flow aggregations */
		CLI_F_MplsFlowAggPrefixLength("0 0");
		sprintf(temp_buf,
				"0 %x",
				(((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
		/*                              [mpls header] */
		CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
		sprintf(temp_buf, "0 %d %d", 1, 5); /* remark offsets for PSN tunnel EXP bits */

		CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
		sprintf(temp_buf, "0 %d", i);
		CLI_F_MplsFlowAggCreate(temp_buf);
#else
		/* create the l2 router MPLS flow aggregations */
		CLI_F_MplsFlowAggPrefixLength("0 4");
		sprintf(temp_buf,
				"0 %x %x",
				WTI_MPLSoPPP,
				(((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
		/*                              [ppp] [mpls header] */
		CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
		sprintf(temp_buf, "0 %d %d", 1, 13); /* remark offsets for PSN tunnel EXP bits */

		CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
		sprintf(temp_buf, "0 %d", i);
		CLI_F_MplsFlowAggCreate(temp_buf);
#endif /* WTI_CESOP_NATIVE_MPLS_IW */
#endif /* WTI_CESOP_MPLS_OVER_ENET */
	}
}

WUFE_status WT_UfeLinePWCreate(WUFE_test_system_setup *test_setup, WP_U32 create_line)
{
   WP_U32 line_index, pw_index = 0, line_offset = 1, num_of_lines = test_setup->num_of_lines, i;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE], setup_str[WTI_MAX_STRING_SIZE];
   //WP_U8 flexmux_mode = 0;
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WUFE_line_clock_rec_params line_cr_params;
#endif
   
	sprintf(setup_str, "Setup is: %d ", test_setup->num_of_lines);

	switch(test_setup->transfer_type)
	{
	case WUFE_SDH_TYPE_E1:
		if(test_setup->framing_mode == WUFE_UNFRAMED)
		{
			strcat(setup_str, "E1 unframed SDH ");
		}
		else
		{
			strcat(setup_str, "E1 framed SDH ");
		}
		break;

	case WUFE_SDH_TYPE_T1:
		if(test_setup->framing_mode == WUFE_UNFRAMED)
		{
			strcat(setup_str, "T1 unframed SDH ");
		}
		else
		{
			strcat(setup_str, "T1 framed SDH ");
		}
		break;

	case WUFE_SONET_TYPE_E1:
		if(test_setup->framing_mode == WUFE_UNFRAMED)
		{
			strcat(setup_str, "E1 unframed SONET ");
		}
		else
		{
			strcat(setup_str, "E1 framed SONET ");
		}
		break;

	case WUFE_SONET_TYPE_T1:
		if(test_setup->framing_mode == WUFE_UNFRAMED)
		{
			strcat(setup_str, "E1 unframed SONET ");
		}
		else
		{
			strcat(setup_str, "E1 framed SONET ");
		}
		break;
	case WUFE_SDH_TYPE_E3:
                line_offset = 28;
		if(test_setup->framing_mode == WUFE_UNFRAMED)
		{
			strcat(setup_str, "E3 unframed SDH ");
		}
		else
		{
			strcat(setup_str, "E3 framed SDH ");
		}
		break;

	case WUFE_SDH_TYPE_DS3:
                line_offset = 28;
		if(test_setup->framing_mode == WUFE_UNFRAMED)
		{
			strcat(setup_str, "DS3 unframed SDH ");
		}
		else
		{
			strcat(setup_str, "DS3 framed SDH ");
		}
		break;

	case WUFE_SONET_TYPE_E3:
                line_offset = 28;
		if(test_setup->framing_mode == WUFE_UNFRAMED)
		{
			strcat(setup_str, "E3 unframed SONET ");
		}
		else
		{
			strcat(setup_str, "E3 framed SONET ");
		}
		break;

	case WUFE_SONET_TYPE_DS3:
                line_offset = 28;
		if(test_setup->framing_mode == WUFE_UNFRAMED)
		{
			strcat(setup_str, "DS3 unframed SONET ");
		}
		else
		{
			strcat(setup_str, "DS3 framed SONET ");
		}
		break;

	default:
		WTI_PrintError("Invalid interface value, Allowed values are: E1, T1, E3, DS3, with SDH or SONET ", __FILE__, __LINE__);
		return WT_FAIL;
		break;
	}

	if(test_setup->num_slots > 1)
	{
		sprintf(temp_buf, "%d slots beginning at slot %d ", test_setup->num_slots, test_setup->first_slot_id);
	}
	else
	{
		sprintf(temp_buf, "1xDS0 at slot %d ", test_setup->first_slot_id);
	}

	strcat(setup_str, temp_buf);

	/*******************************************************************************************/
	/*                                     UFE Phy config                               */
	/*******************************************************************************************/

	if(create_line == 1)
	{
           for (line_index = WT_FIRST_LINE_INDEX; line_index < (num_of_lines*line_offset) + WT_FIRST_LINE_INDEX; line_index+=line_offset)
           {
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
			if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
			{
				/* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
				the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
				if(line_index == WT_LINE_INDEX_TO_FIRST_DCO)
				{
				   the_system->clock_rec[line_index].physical_clock_id = WUFE_CLOCK_REC_DCO_ID_0;
				   WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, WUFE_CLOCK_REC_DCO_ID_0); /* tx_enable, dco_active, dco_number */
				}
				else
				{
				   the_system->clock_rec[line_index].physical_clock_id = WUFE_CLOCK_REC_DCO_ID_1;
				   WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, WUFE_CLOCK_REC_DCO_ID_1); /* tx_enable, dco_active, dco_number */
				}
			}
			else
				WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */
#endif
			if (test_setup->transfer_type == WUFE_SDH_TYPE_E1 || test_setup->transfer_type == WUFE_SONET_TYPE_E1)
			{
				if ((line_index % 28) > 20)
				{
					num_of_lines++;
					continue;
				}

				sprintf(temp_buf, "0 %d %d %d",line_index, test_setup->transfer_type, test_setup->framing_mode);
				CLI_F_UfeTransferMode(temp_buf);
				sprintf(temp_buf, "0 %d %d", line_index, test_setup->cas_mode);
				CLI_F_UfeCasMode(temp_buf);

				/* UFE line configuration, creation and enabling */
#if WTI_DUAL_EMPHY
				if(line_index >= WTI_FIRST_UFE_NUM_LINES)
				{
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
					WT_UfeLineConfigureAndCreateE1(&the_system->second_ufe,
                                              (line_index-WTI_FIRST_UFE_NUM_LINES),
                                              test_setup->transfer_type,
                                              test_setup->framing_mode,
                                              test_setup->cas_mode, 1, &line_cr_params);
#else
					WT_UfeLineConfigureAndCreateE1(&the_system->second_ufe,
                                              (line_index-WTI_FIRST_UFE_NUM_LINES),
                                              test_setup->transfer_type,
                                              test_setup->framing_mode,
                                              test_setup->cas_mode, 1, NULL);
#endif
				}
				else
				{
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
					WT_UfeLineConfigureAndCreateE1(&the_system->ufe,
					                               line_index,
					                               test_setup->transfer_type,
					                               test_setup->framing_mode,
					                               test_setup->cas_mode, 1, &line_cr_params);
#else
					WT_UfeLineConfigureAndCreateE1(&the_system->ufe,
					                               line_index,
					                               test_setup->transfer_type,
					                               test_setup->framing_mode,
					                               test_setup->cas_mode, 1, NULL);
#endif
				}
#else /* not WTI_DUAL_EMPHY */
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
				WT_UfeLineConfigureAndCreateE1(&the_system->ufe,
				                               line_index,
				                               test_setup->transfer_type,
				                               test_setup->framing_mode,
				                               test_setup->cas_mode, 1, &line_cr_params);
#else
				WT_UfeLineConfigureAndCreateE1(&the_system->ufe,
				                               line_index,
				                               test_setup->transfer_type,
				                               test_setup->framing_mode,
				                               test_setup->cas_mode, 1, NULL);
#endif
#endif

			}
			else if (test_setup->transfer_type == WUFE_SDH_TYPE_T1 || test_setup->transfer_type == WUFE_SONET_TYPE_T1)
			{
				sprintf(temp_buf, "0 %d %d %d",line_index, test_setup->transfer_type, test_setup->framing_mode);
				CLI_F_UfeTransferMode(temp_buf);
				sprintf(temp_buf, "0 %d %d",line_index, test_setup->cas_mode);
				CLI_F_UfeCasMode(temp_buf);

				/* UFE line configuration, creation and enabling */
#if WTI_DUAL_EMPHY
				if(line_index >= WTI_FIRST_UFE_NUM_LINES)
				{
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
					WT_UfeLineConfigureAndCreateT1(&the_system->second_ufe,
					                               (line_index-WTI_FIRST_UFE_NUM_LINES),
					                               test_setup->transfer_type,
					                               test_setup->framing_mode, test_setup->cas_mode, 1, &line_cr_params);
#else
					WT_UfeLineConfigureAndCreateT1(&the_system->second_ufe,
					                               (line_index-WTI_FIRST_UFE_NUM_LINES),
					                               test_setup->transfer_type,
					                               test_setup->framing_mode, test_setup->cas_mode, 1, NULL);
#endif
				}
				else
				{
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
					WT_UfeLineConfigureAndCreateT1(&the_system->ufe,
					                               line_index,
					                               test_setup->transfer_type,
					                               test_setup->framing_mode,
					                               test_setup->cas_mode, 1, &line_cr_params);
#else
					WT_UfeLineConfigureAndCreateT1(&the_system->ufe,
					                               line_index,
					                               test_setup->transfer_type,
					                               test_setup->framing_mode,
					                               test_setup->cas_mode, 1, NULL);
#endif
				}
#else /* not WTI_DUAL_EMPHY */
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
				WT_UfeLineConfigureAndCreateT1(&the_system->ufe,
				                               line_index,
				                               test_setup->transfer_type,
				                               test_setup->framing_mode,
				                               test_setup->cas_mode, 1, &line_cr_params);
#else
				WT_UfeLineConfigureAndCreateT1(&the_system->ufe,
                                           line_index,
                                           test_setup->transfer_type,
                                           test_setup->framing_mode,
                                           test_setup->cas_mode, 1, NULL);
#endif
#endif
			}
                        else if (test_setup->transfer_type == WUFE_SDH_TYPE_DS3 || test_setup->transfer_type == WUFE_SONET_TYPE_DS3 ||
                                 test_setup->transfer_type == WUFE_SDH_TYPE_E3 || test_setup->transfer_type == WUFE_SONET_TYPE_E3)
                        {
                           sprintf(temp_buf, "0 %d %d %d",line_index, test_setup->transfer_type, test_setup->framing_mode);
                           CLI_F_UfeTransferMode(temp_buf);
                           sprintf(temp_buf, "0 %d %d",line_index, test_setup->cas_mode);
                           CLI_F_UfeCasMode(temp_buf);
                           
                           /* UFE line configuration, creation and enabling */
#if WTI_DUAL_EMPHY
                           if(line_index >= WTI_FIRST_UFE_NUM_LINES)
                           {
                              WT_UfeLineConfigureAndCreateE3DS3(&the_system->second_ufe,
                                                                (line_index-WTI_FIRST_UFE_NUM_LINES),
                                                                test_setup->transfer_type,
                                                                test_setup->framing_mode, 
                                                                test_setup->ds3_e3_framing_type,
                                                                test_setup->cas_mode, 1, NULL);
                           }
                           else
                           {
                              WT_UfeLineConfigureAndCreateE3DS3(&the_system->ufe,
                                                                line_index,
                                                                test_setup->transfer_type,
                                                                test_setup->framing_mode,
                                                                test_setup->ds3_e3_framing_type,
                                                                test_setup->cas_mode, 1, NULL);
                           }
#else /* not WTI_DUAL_EMPHY */
                           WT_UfeLineConfigureAndCreateE3DS3(&the_system->ufe,
                                                             line_index,
                                                             test_setup->transfer_type,
                                                             test_setup->framing_mode,
                                                             test_setup->ds3_e3_framing_type,
                                                             test_setup->cas_mode, 1, NULL);
#endif
			}
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
			/* Set the defult PSN2TDM clock recoevry paramters */
			WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, test_setup->line_cr_operation_mode[line_index]);
#endif                        

			/* UFE line configuration, creation and enabling */
#if USE_UFE
#if WTI_DUAL_EMPHY
			if (line_index >= WTI_FIRST_UFE_NUM_LINES)
			{
				WT_UfeLineSocketLpbkSetup(&the_system->second_ufe, (line_index-WTI_FIRST_UFE_NUM_LINES), WTI_SOCKET_LOOPBACK_MODE);
			}
			else
			{
				WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
			}
#else
			WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
#endif
#endif/* USE_UFE*/
		}
	}


	/*******************************************************************************************/
	/*                                     UFE PW config                               */
	/*******************************************************************************************/
	num_of_lines = test_setup->num_of_lines;

	for (line_index = WT_FIRST_LINE_INDEX; line_index < (WT_FIRST_LINE_INDEX + (num_of_lines*line_offset)); line_index+=line_offset)
	{
	   if (test_setup->transfer_type == WUFE_SDH_TYPE_E1 || test_setup->transfer_type == WUFE_SONET_TYPE_E1)
	   {
	      if ((line_index % 28) > 20)
	      {
	         num_of_lines++;
	         continue;
	      }
	   }
#if USE_UFE
           the_system->pw[pw_index].line_index = line_index;
           
#if WTI_DUAL_EMPHY
	   if(line_index >= WTI_FIRST_UFE_NUM_LINES)
	   {
	      pw_config.pw_emphy_num = WTI_EMPHY_SECOND;
	   }
	   else
	   {
	      pw_config.pw_emphy_num = WTI_EMPHY_FIRST;
	   }
#endif

	   for (i=0; i < test_setup->pw_per_line; i++)
	   {
	      /*UFE phy and pw configuration*/
#if WTI_DUAL_EMPHY
	      if (test_setup->framing_mode == WUFE_UNFRAMED)
	      {
	         if (test_setup->transfer_type == WUFE_SDH_TYPE_E1 || test_setup->transfer_type == WUFE_SONET_TYPE_E1)
	         {
	            if(line_index >= WTI_FIRST_UFE_NUM_LINES)
	            {
	               sprintf(temp_buf, "0 %d %d 64 64", line_index - WTI_FIRST_UFE_NUM_LINES, pw_index);
	            }
	            else
	            {
	               sprintf(temp_buf, "0 %d %d 64 64", line_index, pw_index);
	            }
	         }
	         else
	         {
	            if(line_index >= WTI_FIRST_UFE_NUM_LINES)
	            {
	               sprintf(temp_buf, "0 %d %d 48 48", line_index - WTI_FIRST_UFE_NUM_LINES, pw_index);
	            }
	            else
	            {
	               sprintf(temp_buf, "0 %d %d 48 48", line_index, pw_index);
	            }
	         }
	         CLI_F_PwConfigureUnframed(temp_buf);
	      }
	      else /* WUFE_FRAMED */
	      {
	         if (test_setup->transfer_type == WUFE_SDH_TYPE_E1 || test_setup->transfer_type == WUFE_SONET_TYPE_E1)
	         {
	            if(test_setup->num_slots > 1)
	            {
	               if(line_index >= WTI_FIRST_UFE_NUM_LINES)
	               {
	                  sprintf(temp_buf, "0 %d %d 64 64 %d %d-%d", line_index - WTI_FIRST_UFE_NUM_LINES,
	                          pw_index, test_setup->num_slots, test_setup->first_slot_id + i*test_setup->num_slots,
	                          test_setup->first_slot_id + i*test_setup->num_slots + test_setup->num_slots - 1);
	               }
	               else
	               {
	                  sprintf(temp_buf, "0 %d %d 64 64 %d %d-%d", line_index,
	                          pw_index, test_setup->num_slots, test_setup->first_slot_id + i*test_setup->num_slots,
	                          test_setup->first_slot_id + i*test_setup->num_slots + test_setup->num_slots - 1);
	               }
	            }
	            else
	            {
	               if(line_index >= WTI_FIRST_UFE_NUM_LINES)
	               {
	                  sprintf(temp_buf, "0 %d %d 40 40 %d %d-%d", line_index - WTI_FIRST_UFE_NUM_LINES,
	                          pw_index, test_setup->num_slots, test_setup->first_slot_id + i*test_setup->num_slots,
	                          test_setup->first_slot_id + i*test_setup->num_slots + test_setup->num_slots - 1);
	               }
	               else
	               {
	                  sprintf(temp_buf, "0 %d %d 40 40 %d %d-%d", line_index,
	                          pw_index, test_setup->num_slots, test_setup->first_slot_id + i*test_setup->num_slots,
	                          test_setup->first_slot_id + i*test_setup->num_slots + test_setup->num_slots - 1);
	               }
	            }
	         }
	         else
	         {
	            if(test_setup->num_slots > 1)
	            {
	               if(line_index >= WTI_FIRST_UFE_NUM_LINES)
	               {
	                  sprintf(temp_buf, "0 %d %d 48 48 %d %d-%d", line_index - WTI_FIRST_UFE_NUM_LINES,
	                          pw_index, test_setup->num_slots, test_setup->first_slot_id + i*test_setup->num_slots,
	                          test_setup->first_slot_id + i*test_setup->num_slots + test_setup->num_slots - 1);
	               }
	               else
	               {
	                  sprintf(temp_buf, "0 %d %d 48 48 %d %d-%d", line_index,
	                          pw_index, test_setup->num_slots, test_setup->first_slot_id + i*test_setup->num_slots,
	                          test_setup->first_slot_id + i*test_setup->num_slots + test_setup->num_slots - 1);
	               }
	            }
	            else
	            {
	               if(line_index >= WTI_FIRST_UFE_NUM_LINES)
	               {
	                  sprintf(temp_buf, "0 %d %d 40 40 %d %d-%d", line_index - WTI_FIRST_UFE_NUM_LINES,
	                          pw_index, test_setup->num_slots, test_setup->first_slot_id + i*test_setup->num_slots,
	                          test_setup->first_slot_id + i*test_setup->num_slots + test_setup->num_slots - 1);
	               }
	               else
	               {
	                  sprintf(temp_buf, "0 %d %d 40 40 %d %d-%d", line_index,
	                          pw_index, test_setup->num_slots, test_setup->first_slot_id + i*test_setup->num_slots,
	                          test_setup->first_slot_id + i*test_setup->num_slots + test_setup->num_slots - 1);
	               }
	            }
	         }

	         CLI_F_PwConfigureFramed(temp_buf);
	      }
#else /* not WTI_DUAL_EMPHY */
	      if (test_setup->framing_mode == WUFE_UNFRAMED)
	      {
	         if (test_setup->transfer_type == WUFE_SDH_TYPE_T1 || test_setup->transfer_type == WUFE_SONET_TYPE_T1)
	         {
	            sprintf(temp_buf, "0 %d %d 48 48", line_index, pw_index);
	         }
	         else
	         {
	            sprintf(temp_buf, "0 %d %d 64 64", line_index, pw_index);
	         }
	         CLI_F_PwConfigureUnframed(temp_buf);
	      }
	      else /* WUFE_FRAMED */
	      {
	         if (test_setup->transfer_type == WUFE_SDH_TYPE_E1 || test_setup->transfer_type == WUFE_SONET_TYPE_E1)
	         {
	            if(test_setup->num_slots > 1)
	            {
	               sprintf(temp_buf, "0 %d %d 64 64 %d %d-%d", line_index,
	                       pw_index, test_setup->num_slots, test_setup->first_slot_id + i*test_setup->num_slots,
	                       test_setup->first_slot_id + i*test_setup->num_slots + test_setup->num_slots - 1);
	            }
	            else
	            {
	               sprintf(temp_buf, "0 %d %d 40 40 %d %d-%d", line_index,
	                       pw_index, test_setup->num_slots, test_setup->first_slot_id + i*test_setup->num_slots,
	                       test_setup->first_slot_id + i*test_setup->num_slots + test_setup->num_slots - 1);
	            }
	         }
	         else
	         {
	            if(test_setup->num_slots > 1)
	            {
	               sprintf(temp_buf, "0 %d %d 48 48 %d %d-%d", line_index,
	                       pw_index, test_setup->num_slots, test_setup->first_slot_id + i*test_setup->num_slots,
	                       test_setup->first_slot_id + i*test_setup->num_slots + test_setup->num_slots - 1);
	            }
	            else
	            {
	               sprintf(temp_buf, "0 %d %d 40 40 %d %d-%d", line_index,
	                       pw_index, test_setup->num_slots, test_setup->first_slot_id + i*test_setup->num_slots,
	                       test_setup->first_slot_id + i*test_setup->num_slots + test_setup->num_slots - 1);
	            }
	         }

	         CLI_F_PwConfigureFramed(temp_buf);
	      }
#endif

	      sprintf(temp_buf, "0 %d", test_setup->num_slots);
	      CLI_DefaultIwPwe3CesConfig(temp_buf);

	      /* PWE3 channel configuration */
	      sprintf(temp_buf, "0 %d", global_jitter_buffer_size);
	      CLI_F_Pwe3ChannelJitterBufferSize(temp_buf);
	      if (test_setup->transfer_type == WUFE_SDH_TYPE_E1 || test_setup->transfer_type == WUFE_SONET_TYPE_E1)
	      {
	         if(test_setup->num_slots > 1)
	         {
	            // CLI_F_Pwe3ChannelRxBufferSize("0 512");
			sprintf(temp_buf, "0 %d", g_rxbuffersize);
	            CLI_F_Pwe3ChannelRxBufferSize(temp_buf);
	         }
	         else
	         {
	            CLI_F_Pwe3ChannelRxBufferSize("0 40");
	         }
	      }
	      else if (test_setup->transfer_type == WUFE_SDH_TYPE_T1 || test_setup->transfer_type == WUFE_SONET_TYPE_T1)
	      {
	         if(test_setup->num_slots > 1)
	         {
	            CLI_F_Pwe3ChannelRxBufferSize("0 192");
	         }
	         else
	         {
	            CLI_F_Pwe3ChannelRxBufferSize("0 40");
	         }
	      }
              else /*E3, DS3*/
              {
	            CLI_F_Pwe3ChannelRxBufferSize("0 256");
              }

	      /**************** TDM --> PSN IW *****************/
	      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
	      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", line_index + 1);
	      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
	      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
	      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
	      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
	      /* Rx timing is used in differential mode */
	      sprintf(temp_buf, "0 1 1 %d", WT_ClockRecInterfaceId(line_index));
	      CLI_F_Tdm2PsnFlowAggTsParams(temp_buf); /* [ts_used(1-used,0-unused),
                                                    ts_method(0-WP_IW_CESOP_ABS_TS,
                                                    1-WP_IW_CESOP_DIFF_TS_MASTER ),
                                                    ts_diff_if_id(0-63)] */
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

	      /**************** PSN --> TDM Tx binding*****************/
	      sprintf(temp_buf, "0 %d", global_jitter_buffer_size/2);
	      CLI_F_Psn2TdmTxBinWindowThreshold(temp_buf);
	      sprintf(temp_buf, "0 %d", global_jitter_buffer_size/4);
	      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 8");

	      CLI_F_Psn2TdmTxBinLOPSDetection("0 1 6 4");

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
#if WTI_CESOP_TO_MPLS_FA_CASCADING

	      if ((pw_index % test_setup->pw_per_line) == test_setup->recovered_pw)
	      {
	         sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
	      }
	      else
	      {
	         sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
	      }

#if WTI_DUAL_EMPHY
	      if(line_index >= WTI_FIRST_UFE_NUM_LINES)
	      {
	         CLI_F_PwCreateCascadedOnSecondUfe(temp_buf);
	      }
	      else
	      {
	         CLI_F_PwCreateCascaded(temp_buf);
	      }
#else
	      CLI_F_PwCreateCascaded(temp_buf);
#endif
#else
	      if ((pw_index % test_setup->pw_per_line) == test_setup->recovered_pw)
	      {
	         sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
	      }
	      else
	      {
	         sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
	      }
#if WTI_DUAL_EMPHY
	      if (line_index>=WTI_FIRST_UFE_NUM_LINES)
	      {
	         CLI_F_PwCreateCascadedOnSecondUfe(temp_buf);
	      }
	      else
	      {
	         CLI_F_PwCreateCascaded(temp_buf);
	      }
#else
	      CLI_F_PwCreateCascaded(temp_buf);
#endif
#endif  /* WTI_CESOP_TO_MPLS_FA_CASCADING */
	      if (((pw_index % test_setup->pw_per_line) == test_setup->recovered_pw) || (test_setup->pw_per_line == 1))
	      {
	         sprintf(temp_buf, "0 %d 1",pw_index);
	      }
	      else
	      {
	         sprintf(temp_buf, "0 %d 0",pw_index);
	      }
	      CLI_F_PwEnable(temp_buf);
#else  /* not WTI_CESOP_CLOCK_RECOVERY_ENABLE */
#if WTI_CESOP_TO_MPLS_FA_CASCADING
	      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
#if WTI_DUAL_EMPHY
	      if(line_index >= WTI_FIRST_UFE_NUM_LINES)
	      {
	         CLI_F_PwCreateCascadedOnSecondUfe(temp_buf);
	      }
	      else
	      {
	         CLI_F_PwCreateCascaded(temp_buf);
	      }
#else
	      CLI_F_PwCreateCascaded(temp_buf);
#endif
#else /* not WTI_CESOP_TO_MPLS_FA_CASCADING */
	      sprintf(temp_buf, "0 %d %d %d %d", 0,0,0,pw_index);
	      CLI_F_PwCreate(temp_buf);
#endif  /* WTI_CESOP_TO_MPLS_FA_CASCADING */

	      sprintf(temp_buf, "0 %d 0",pw_index);
	      CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

	      pw_index++;
	   }
#endif /*USE_UFE */
	}

	sprintf(temp_buf , "%d pw ", pw_index);
	strcat(setup_str, temp_buf);
	printf("%s\n", setup_str);

	return WT_PASS;
}

void CLI_F_UfeLinePWCreate(char *StrPrm)
{
	WUFE_status status, res, cmd;
	WP_U32 num_of_lines,
          create_line,
          num_slots,
          input_transfer_type,
          input_framing_mode,
          input_vc_type,
          input_sonet_sdh,
          first_slot_id = 0,
          input_cas_mode;

	res = sscanf(StrPrm, "%d %d %d %d %d %d %d %d %d %d", &cmd, &create_line,&input_transfer_type,
                                                         &input_framing_mode, &input_sonet_sdh, &input_cas_mode,
                                                         &input_vc_type, &num_of_lines, &num_slots, &first_slot_id);

	/* Supported modes */
	/*
	 * WUFE_SDH_TYPE_T1,
	 * WUFE_SDH_TYPE_E1,
	 * WUFE_SONET_TYPE_T1,
	 * WUFE_SONET_TYPE_E1,
	 */

	/* input_transfer_type E1 - 0, T1 - 1 */
	/* input_framing_mode unframed - 0, framed - 1 */
	/* input_sonet_sdh SDH - 0, SONET - 1*/
	/* input_vc_type WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4 - 0, WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3 - 1 */

	switch(res)
	{
	case 2:
		printf("Using default setup\n");
		break;
	case 3:
		if(input_transfer_type == 0)
		{
			test_setup.transfer_type = WUFE_SDH_TYPE_E1;
		}
		else
		{
			test_setup.transfer_type = WUFE_SDH_TYPE_T1;
		}
		break;
	case 4:
		if(input_transfer_type == 0)
		{
			test_setup.transfer_type = WUFE_SDH_TYPE_E1;
		}
		else
		{
			test_setup.transfer_type = WUFE_SDH_TYPE_T1;
		}
		if(input_framing_mode == 0)
		{
			test_setup.framing_mode = WUFE_UNFRAMED;
		}
		else
		{
			test_setup.framing_mode = WUFE_FRAMED;
		}
		break;
	case 5:
		if(input_transfer_type == 0)
		{
			if(input_sonet_sdh == 0)
			{
				test_setup.transfer_type = WUFE_SDH_TYPE_E1;
			}
			else
			{
				test_setup.transfer_type = WUFE_SONET_TYPE_E1;
			}
		}
		else
		{
			if(input_sonet_sdh == 0)
			{
				test_setup.transfer_type = WUFE_SDH_TYPE_T1;
			}
			else
			{
				test_setup.transfer_type = WUFE_SONET_TYPE_T1;
			}
		}
		if(input_framing_mode == 0)
		{
			test_setup.framing_mode = WUFE_UNFRAMED;
		}
		else
		{
			test_setup.framing_mode = WUFE_FRAMED;
		}
		break;
	case 6:
		if(input_transfer_type == 0)
		{
			if(input_sonet_sdh == 0)
			{
				test_setup.transfer_type = WUFE_SDH_TYPE_E1;
			}
			else
			{
				test_setup.transfer_type = WUFE_SONET_TYPE_E1;
			}
		}
		else
		{
			if(input_sonet_sdh == 0)
			{
				test_setup.transfer_type = WUFE_SDH_TYPE_T1;
			}
			else
			{
				test_setup.transfer_type = WUFE_SONET_TYPE_T1;
			}
		}
		if(input_framing_mode == 0)
		{
			test_setup.framing_mode = WUFE_UNFRAMED;
		}
		else
		{
			test_setup.framing_mode = WUFE_FRAMED;
		}
		if(input_cas_mode == 0)
		{
			test_setup.cas_mode = WUFE_CAS_DISABLE;
		}
		else
		{
			test_setup.cas_mode = WUFE_CAS_ENABLE;
		}
		break;
	case 7:
		if(input_transfer_type == 0)
		{
			if(input_sonet_sdh == 0)
			{
				test_setup.transfer_type = WUFE_SDH_TYPE_E1;
			}
			else
			{
				test_setup.transfer_type = WUFE_SONET_TYPE_E1;
			}
		}
		else
		{
			if(input_sonet_sdh == 0)
			{
				test_setup.transfer_type = WUFE_SDH_TYPE_T1;
			}
			else
			{
				test_setup.transfer_type = WUFE_SONET_TYPE_T1;
			}
		}
		if(input_framing_mode == 0)
		{
			test_setup.framing_mode = WUFE_UNFRAMED;
		}
		else
		{
			test_setup.framing_mode = WUFE_FRAMED;
		}
		if(input_cas_mode == 0)
		{
			test_setup.cas_mode = WUFE_CAS_DISABLE;
		}
		else
		{
			test_setup.cas_mode = WUFE_CAS_ENABLE;
		}
#ifdef WT_UFE_FRAMER
		if(input_vc_type == 0)
		{
			test_setup.vc_type = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
		}
		else
		{
			test_setup.vc_type = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
		}
#endif
		break;
	case 8:

		if(input_transfer_type == 0)
		{
			if(input_sonet_sdh == 0)
			{
				test_setup.transfer_type = WUFE_SDH_TYPE_E1;
			}
			else
			{
				test_setup.transfer_type = WUFE_SONET_TYPE_E1;
			}
		}
		else
		{
			if(input_sonet_sdh == 0)
			{
				test_setup.transfer_type = WUFE_SDH_TYPE_T1;
			}
			else
			{
				test_setup.transfer_type = WUFE_SONET_TYPE_T1;
			}
		}
		if(input_framing_mode == 0)
		{
			test_setup.framing_mode = WUFE_UNFRAMED;
		}
		else
		{
			test_setup.framing_mode = WUFE_FRAMED;
		}
		if(input_cas_mode == 0)
		{
			test_setup.cas_mode = WUFE_CAS_DISABLE;
		}
		else
		{
			test_setup.cas_mode = WUFE_CAS_ENABLE;
		}
#ifdef WT_UFE_FRAMER
		if(input_vc_type == 0)
		{
			test_setup.vc_type = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
		}
		else
		{
			test_setup.vc_type = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
		}
#endif

		if(num_of_lines == 0)
		{
			WTI_PrintError("Invalid number of parameter, number of lines cannot be 0 ", __FILE__, __LINE__);
			return;
		}
		test_setup.num_of_lines = num_of_lines;
		break;
	case 9:

		if(input_transfer_type == 0)
		{
			if(input_sonet_sdh == 0)
			{
				test_setup.transfer_type = WUFE_SDH_TYPE_E1;
			}
			else
			{
				test_setup.transfer_type = WUFE_SONET_TYPE_E1;
			}
		}
		else
		{
			if(input_sonet_sdh == 0)
			{
				test_setup.transfer_type = WUFE_SDH_TYPE_T1;
			}
			else
			{
				test_setup.transfer_type = WUFE_SONET_TYPE_T1;
			}
		}
		if(input_framing_mode == 0)
		{
			test_setup.framing_mode = WUFE_UNFRAMED;
		}
		else
		{
			test_setup.framing_mode = WUFE_FRAMED;
		}
		if(input_cas_mode == 0)
		{
			test_setup.cas_mode = WUFE_CAS_DISABLE;
		}
		else
		{
			test_setup.cas_mode = WUFE_CAS_ENABLE;
		}
#ifdef WT_UFE_FRAMER
		if(input_vc_type == 0)
		{
			test_setup.vc_type = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
		}
		else
		{
			test_setup.vc_type = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
		}
#endif

		if(num_of_lines == 0)
		{
			WTI_PrintError("Invalid number of parameter, number of lines cannot be 0 ", __FILE__, __LINE__);
			return;
		}
		test_setup.num_of_lines = num_of_lines;

		if(num_slots == 0)
		{
			WTI_PrintError("Invalid number of parameter, number of slots cannot be 0 ", __FILE__, __LINE__);
			return;
		}
		test_setup.num_slots = num_slots;
		break;

	case 10:

		if(input_transfer_type == 0)
		{
			if(first_slot_id < 0 || first_slot_id > 31)
			{
				WTI_PrintError("Invalid parameter, first slot cannot be smaller than 0  or larger than 31", __FILE__, __LINE__);
				return;
			}

			if(input_sonet_sdh == 0)
			{
				test_setup.transfer_type = WUFE_SDH_TYPE_E1;
			}
			else
			{
				test_setup.transfer_type = WUFE_SONET_TYPE_E1;
			}
		}
		else
		{
			if(first_slot_id < 0 || first_slot_id > 23)
			{
				WTI_PrintError("Invalid parameter, first slot cannot be smaller than 0  or larger than 23", __FILE__, __LINE__);
				return;
			}

			if(input_sonet_sdh == 0)
			{
				test_setup.transfer_type = WUFE_SDH_TYPE_T1;
			}
			else
			{
				test_setup.transfer_type = WUFE_SONET_TYPE_T1;
			}
		}

		test_setup.first_slot_id = first_slot_id;

		if(input_framing_mode == 0)
		{
			test_setup.framing_mode = WUFE_UNFRAMED;
		}
		else
		{
			test_setup.framing_mode = WUFE_FRAMED;
		}
		if(input_cas_mode == 0)
		{
			test_setup.cas_mode = WUFE_CAS_DISABLE;
		}
		else
		{
			test_setup.cas_mode = WUFE_CAS_ENABLE;
		}
#ifdef WT_UFE_FRAMER
		if(input_vc_type == 0)
		{
			test_setup.vc_type = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
		}
		else
		{
			test_setup.vc_type = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
		}
#endif

		if(num_of_lines == 0)
		{
			WTI_PrintError("Invalid number of parameter, number of lines cannot be 0 ", __FILE__, __LINE__);
			return;
		}
		test_setup.num_of_lines = num_of_lines;

		if(num_slots == 0)
		{
			WTI_PrintError("Invalid number of parameter, number of slots cannot be 0 ", __FILE__, __LINE__);
			return;
		}
		test_setup.num_slots = num_slots;
	break;

	default:
		WTI_PrintError("Invalid number of parameters ", __FILE__, __LINE__);
		return;
	}

	status = WT_UfeLinePWCreate(&test_setup, create_line);

	if(status != WT_PASS)
	{
		WTI_PrintError("ERROR: Lines and PW were not created ", __FILE__, __LINE__);
	}
}

void CLI_F_UfeLinePWDelete(char *StrPrm)
{
	WP_U32 line_index, pw_index, delete_line, num_of_lines, cmd, res, i, line_number;
	WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];

	res = sscanf(StrPrm, "%d %d %d", &cmd, &delete_line, &line_number);

	if(res != 2 && res != 3)
	{
		WTI_PrintError("Error: Incorrect number of parameters", __FILE__, __LINE__);
		return;
	}

	if(delete_line != 0 && delete_line != 1)
	{
		WTI_PrintError("Error: Incorrect parameter 'delete line', may be 0 or 1", __FILE__, __LINE__);
		return;
	}

	/* count the number of active lines to delete */
	for(num_of_lines = 0, i = 0; i < the_system->ufe.max_lines; i++)
	{
		if(the_system->ufe.line_params[i].valid)
		{
			num_of_lines++;
		}
	}
#if WTI_DUAL_EMPHY
	for(i = 0; i < the_system->second_ufe.max_lines; i++)
	{
		if(the_system->ufe.line_params[i].valid)
		{
			num_of_lines++;
		}
	}
#endif

	for (pw_index = total_number_of_pw ;pw_index >0; --pw_index)
	{
		sprintf(temp_buf, "0 %d 0",pw_index-1);
		CLI_F_PwDisable(temp_buf);
        }

         WP_Delay(5000);

         for (pw_index = total_number_of_pw ;pw_index >0; --pw_index)
 	{
           sprintf(temp_buf, "0 %d 0",pw_index-1);
		CLI_F_PwDelete(temp_buf);

		printf("PW %i removed\n", pw_index-1);
		}



	if(delete_line)
	{
		num_of_lines= num_of_lines + ((num_of_lines/21)*7);

		for (line_index = num_of_lines ;line_index >0; --line_index)
		{
			if (!the_system->ufe.line_params[line_index-1].valid)
			{
				continue;
			}

			WT_UfeLineDelete(&the_system->ufe, line_index-1);
#if WTI_DUAL_EMPHY
			WT_UfeLineDelete(&the_system->second_ufe, line_index-1);
#endif
			printf("Line %i removed\n", line_index-1);
		}


	}
}

#ifndef SERIAL_X_CPLD_INTERRUPT_MASK
#error "SERIAL_X_CPLD_INTERRUPT_MASK not defined"
#endif

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhE1UnframedNoCas(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines,vc_type, physical_param;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4 = WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_flexmux_physical_conf physical_configuration;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
   WP_boolean is_e1 = WP_TRUE;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &interface,&physical_param,&vc_type, &num_of_lines);
   if (res != 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters -- 5 required", __LINE__);
      return;
   }
#if WTI_DUAL_EMPHY
   num_of_lines = 2 * num_of_lines;
#endif

   test_setup.framing_mode = WUFE_UNFRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = WTI_E1_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.vc_type = vc_type;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);



   switch(interface)
   {
      case 0:
         test_setup.transfer_type = WUFE_SDH_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
         if(vc_type == 0)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         else if(vc_type == 1)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
         else
         {
            WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-VC3, 1-VC4 ", __LINE__);
            return;
         }
#endif
         break;
      case 1:
         test_setup.transfer_type = WUFE_SONET_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         test_setup.transfer_type = WUFE_CAD_TYPE_E1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }
#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:
         physical_configuration = WT_FLEXMUX_UFE_412_1_PORT_OC12;
         break;
      case 1:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3;
         break;
      case 2:
         physical_configuration = WT_FLEXMUX_UFE_412_2_PORTS_OC3;
         break;
      case 3:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER;
         break;
      case 4:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE;
         break;
#if WTI_DUAL_EMPHY
         case 5:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_UPI1_MASTER_UPI3_SLAVE;
         break;
#endif
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value\n"
                                 "Allowed values are: 0-1xOC12, 1-4xOC3 2-2xOC3", __LINE__);
         return;
   }
#endif

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER

   /* Initialize the Flexmux framer in SDH mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   physical_configuration,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);
   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);
#if 0 /* for clock squelching debug */
   WP_U32 addr = 0xbf230066;
   WP_U16 value;
   
   /* read register */
   value = *((WP_U16*)addr);   
   printf("value 0x%08x\n", value);
   
   /* write the value*/
   value = 0x0a0a;
   printf("value to put 0x%08x\n", value);
   *((WP_U16*)addr) = value;
   
   /* read register once again */
   value = *((WP_U16*)addr);   
   printf("value in register is 0x%08x\n", value);

#endif /* for clock squelching debug */
#ifdef WT_UFE_FRAMER

   /* interface == 0 ==> SDH; interface == 1 ==> SONET */
   if (interface < 2)
   {
      WTI_enable_performance_monitoring_points(interface, is_e1, vc_type, test_setup.framing_mode);
   }

#endif

#ifdef OTE_AUTOMATION
/* In case  using  the OTE automation we want to work with splitter, there we need to disable all the tx enable port except to the first port */
   CLI_F_FlexmuxPoke("0 0 0x88001040 0x00000001");
   CLI_F_FlexmuxPoke("0 0 0x88001044 0x00000001");
   CLI_F_FlexmuxPoke("0 0 0x88001048 0x00000000");
   CLI_F_FlexmuxPoke("0 0 0x8800104C 0x00000000");
   CLI_F_FlexmuxPoke("0 0 0x88001050 0x00000000");
   CLI_F_FlexmuxPoke("0 0 0x88001054 0x00000000");
   CLI_F_FlexmuxPoke("0 0 0x88001058 0x00000000");
   CLI_F_FlexmuxPoke("0 0 0x8800105C 0x00000000");
   
#endif

#ifdef WT_UFE_FRAMER
#if ( !(WTI_CESOP_TDI) && (WTI_CESOP_REGRESSION_TEST) )
#if WT_RUN_PRBS_CHECK
   WP_U8 prbs_errors = 0;
   WP_U8 prbs_tmp = 0;

   printf("Checking PRBS...\n");

   for (i=WT_FIRST_LINE_INDEX;i< (WT_FIRST_LINE_INDEX+num_of_lines);i++)
   {
      if ((i%4) != 3)  /* In E1 transfer type, every tug includes only 3 tu*/
      {
         prbs_tmp = WTI_FlexmuxCheckPrbsResult(0,i,framing_mode);
         prbs_errors |= prbs_tmp;
         if(prbs_tmp)
         {
            printf("PRBS check failed for tu %d\n",i);
         }
      }
   }
   if (prbs_errors == 0)
   {
      printf("PRBS check Passed\n");
   }
   else
   {
      printf("PRBS check failed\n");
   }

#endif
#endif
#endif

#ifdef WT_UFE_FRAMER
#if WTI_CHECK_PORT_SWITCHING
   /*
 {
      WP_U32 line_port_id;
      for (line_port_id = 0; line_port_id<2; line_port_id+=2)
      {
         printf("------------------------------- PORT %d ----------------------------\n", line_port_id);
         WTI_FlexmuxForceB(line_port_id);
         WTI_FlexmuxForceA(line_port_id);
         WTI_FlexmuxForceA(line_port_id);
         WTI_FlexmuxForceB(line_port_id);
         WTI_FlexmuxForceB(line_port_id);
         WTI_FlexmuxForceA(line_port_id);
         WTI_FlexmuxForceB(line_port_id);
         WTI_FlexmuxForceA(line_port_id);
      }
   }
   */

   char ch;
   WP_U32 line_port_id, line_active=1;;

   printf("****************** Port switching check **********************\n");
   for (line_port_id = 0; line_port_id<4; line_port_id+=1)
   {

      printf("Port switching check for line port %d\n",line_port_id);
      printf("line active is %d\n",line_active);
      printf("Press 'q' in order to proceed to the next line\n");

      do{
         printf("a - ForceA, b - ForceB\n");
         ch = getchar();
         if (ch == 'a' )
         {
            WTI_FlexmuxForceA(0,line_port_id);
            line_active = 1;
            printf("line active is %d\n",line_active);
         }
         else if (ch == 'b')
         {
            WTI_FlexmuxForceB(0,line_port_id);
            line_active = 2;
            printf("line active is %d\n",line_active);
         }
      }
      while (ch != 'q');
   }

#endif
#endif
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhE1FramedNoCas(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines, vc_type, physical_param;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_flexmux_physical_conf physical_configuration;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
   WP_boolean is_e1 = WP_TRUE;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &interface,&physical_param,&vc_type,&num_of_lines);
   if (res != 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

#if WTI_DUAL_EMPHY
   num_of_lines = 2 * num_of_lines;
#endif

   test_setup.framing_mode = WUFE_FRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = WTI_E1_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.vc_type = vc_type;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);

   switch(interface)
   {
      case 0:
         test_setup.transfer_type = WUFE_SDH_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
         if(vc_type == 0)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         else if(vc_type == 1)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
         else
         {
            WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-VC3, 1-VC4 ", __LINE__);
            return;
         }
#endif
         break;

      case 1:
         test_setup.transfer_type = WUFE_SONET_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         test_setup.transfer_type = WUFE_CAD_TYPE_E1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }
#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:physical_configuration = WT_FLEXMUX_UFE_412_1_PORT_OC12;
         break;
      case 1:physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3;
         break;
      case 2:physical_configuration = WT_FLEXMUX_UFE_412_2_PORTS_OC3;
         break;
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value, Allowed values are: 0-1xOC12, 1-4xOC3 2-2xOC3 ", __LINE__);
         return;
   }
#endif

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   physical_configuration,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

#ifdef WT_UFE_FRAMER
   /* interface == 0 ==> SDH; interface == 1 ==> SONET */
   if (interface < 2)
      WTI_enable_performance_monitoring_points(interface, is_e1, vc_type, test_setup.framing_mode);

#ifdef OTE_AUTOMATION
/* In case  using  the OTE automation we want to work with splitter, there we need to disable all the tx enable port except to the first port */
   CLI_F_FlexmuxPoke("0 0 0x88001040 0x00000001");
   CLI_F_FlexmuxPoke("0 0 0x88001044 0x00000001");
   CLI_F_FlexmuxPoke("0 0 0x88001048 0x00000000");
   CLI_F_FlexmuxPoke("0 0 0x8800104C 0x00000000");
   CLI_F_FlexmuxPoke("0 0 0x88001050 0x00000000");
   CLI_F_FlexmuxPoke("0 0 0x88001054 0x00000000");
   CLI_F_FlexmuxPoke("0 0 0x88001058 0x00000000");
   CLI_F_FlexmuxPoke("0 0 0x8800105C 0x00000000");
   
#endif

#if ( !(WTI_CESOP_TDI) && (WTI_CESOP_REGRESSION_TEST) )
#if WT_RUN_PRBS_CHECK
   WP_U8 prbs_errors = 0;
   WP_U8 prbs_tmp = 0;


   for (i=WT_FIRST_LINE_INDEX;i< (WT_FIRST_LINE_INDEX+num_of_lines);i++)
   {
      if ((i%4) != 3)  /* In E1 transfer type, every tug includes only 3 tu*/
      {
         prbs_tmp = WTI_FlexmuxCheckPrbsResult(0,i,framing_mode);
         prbs_errors |= prbs_tmp;
         if(prbs_tmp)
         {
            printf("PRBS check failed for tu %d\n",i);
         }
      }
   }
   if (prbs_errors == 0)
   {
      printf("PRBS check Passed\n");
   }
   else
   {
      printf("PRBS check failed\n");
   }
#endif
#endif
#endif

#ifdef WT_UFE_FRAMER
#if WTI_CHECK_PORT_SWITCHING
   {
      WP_U32 line_port_id = 0;
      for (line_port_id = 0; line_port_id<2; line_port_id++)
      {
         WTI_FlexmuxForceB(0,line_port_id);
         WTI_FlexmuxForceA(0,line_port_id);
         WTI_FlexmuxForceA(0,line_port_id);
         WTI_FlexmuxForceB(0,line_port_id);
         WTI_FlexmuxForceB(0,line_port_id);
         WTI_FlexmuxForceA(0,line_port_id);
         WTI_FlexmuxForceB(0,line_port_id);
         WTI_FlexmuxForceA(0,line_port_id);
      }
   }
#endif
#endif
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhE1FramedWithCas(char *StrPrm)
{
   WP_U32 res, cmd, i,j, num_of_lines, num_slots,pw_index=0,fragment;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U32 vc_type = WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
   WP_boolean is_e1 = WP_TRUE;
#endif

   framing_mode = WUFE_FRAMED;
   cas_mode = WUFE_CAS_ENABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &interface,&num_of_lines,&fragment);
   if (res != 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   switch(interface)
   {
   case 0:
      transfer_type = WUFE_SDH_TYPE_E1;
#ifdef WT_UFE_FRAMER
      flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
      break;
   case 1:
      transfer_type = WUFE_SONET_TYPE_E1;
#ifdef WT_UFE_FRAMER
      flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
      break;
   case 2:
      transfer_type = WUFE_CAD_TYPE_E1;
      break;
   default:
      WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
      return;
   }
   num_slots = 30;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i = j = WT_FIRST_LINE_INDEX; j < (WT_FIRST_LINE_INDEX + num_of_lines); ++i)
   {
      if ((i % 28) > 20)
      {
         continue;
      }

      ++j;

      sprintf(temp_buf, "0 %d %d %d", i,transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d", i, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SDH mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   for (i=0;i<WTI_MAX_NUM_OF_MPLS_FA;++i)
   {
#if WTI_CESOP_MPLS_OVER_ENET
      /* create the l2 router MPLS flow aggregations */
      CLI_F_Tdm2PsnFlowAggEnetHeaderMpls("0 aa aa aa aa aa aa bb bb bb bb bb bb 8847");
      CLI_F_MplsFlowAggPrefixLength("0 18");
      sprintf(temp_buf,
              "0 %x %x %x %x %x %x",
              3*i,
              3*i+1,
              3*i+2,
              0x81000005,
              (0x88470000 | ((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) >> 4)),
              ((((((WTI_MPLS_LABEL) >> 12)  + WTI_MAX_PW + 1 + i) & 0xff) << 28) | 0x00ff0000));
      /*                              [mac destination] [mac source] [vlan] [type] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d %d", 2, 28, 41); /* remark offsets for VLAN priority and PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
#if WTI_CESOP_NATIVE_MPLS_IW
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 0");
      sprintf(temp_buf,
              "0 %x",
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 5); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 4");
      sprintf(temp_buf,
              "0 %x %x",
              WTI_MPLSoPPP,
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [ppp] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 13); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#endif /* WTI_CESOP_NATIVE_MPLS_IW */
#endif /* WTI_CESOP_MPLS_OVER_ENET */
   }

   for (i = j = WT_FIRST_LINE_INDEX; j < (WT_FIRST_LINE_INDEX + num_of_lines); ++i)
   {
      if ((i % 28) > 20)
      {
         continue;
      }

      ++j;

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1,NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WTI_SOCKET_LOOPBACK_MODE);
      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64",i,pw_index);
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d 30 30 %d 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31",i,pw_index,num_slots);
         CLI_F_PwConfigureFramed(temp_buf);
      }

      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      if(fragment)
         CLI_F_Pwe3ChannelRxBufferSize("0 240");
      else
         CLI_F_Pwe3ChannelRxBufferSize("0 480");/*16*30*/
      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", i+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");

      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 32");

      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
      pw_index++;
   }
#ifdef WT_UFE_FRAMER
   /* interface == 0 ==> SDH; interface == 1 ==> SONET */
   if (interface < 2)
      WTI_enable_performance_monitoring_points(interface, is_e1, vc_type, framing_mode);
#endif
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhT1UnframedNoCas(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines, vc_type, physical_param;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_flexmux_physical_conf physical_configuration;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
   WP_boolean is_e1 = WP_FALSE;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &interface,&physical_param,&vc_type,&num_of_lines);
   if (res != 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

#if WTI_DUAL_EMPHY
   num_of_lines = 2 * num_of_lines;
#endif
   test_setup.framing_mode = WUFE_UNFRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = WTI_T1_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.vc_type = vc_type;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);

   switch(interface)
   {
      case 0:
         test_setup.transfer_type = WUFE_SDH_TYPE_T1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
     if(vc_type == 0)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         else if(vc_type == 1)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
         else
         {
            WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-VC3, 1-VC4 ", __LINE__);
            return;
         }
#endif
         break;
      case 1:
         test_setup.transfer_type = WUFE_SONET_TYPE_T1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         test_setup.transfer_type = WUFE_CAD_TYPE_T1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }

#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:physical_configuration = WT_FLEXMUX_UFE_412_1_PORT_OC12;
         break;
      case 1:physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3;
         break;
      case 2:physical_configuration = WT_FLEXMUX_UFE_412_2_PORTS_OC3;
         break;
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value, Allowed values are: 0-1xOC12, 1-4xOC3 2-2xOC3 ", __LINE__);
         return;
   }
#endif

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /* Calls UFE system create and enable */
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   physical_configuration,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

#ifdef WT_UFE_FRAMER
   /* interface == 0 ==> SDH; interface == 1 ==> SONET */
   if (interface < 2)
      WTI_enable_performance_monitoring_points(interface, is_e1, vc_type, test_setup.framing_mode);

#endif

#if 0//def OTE_AUTOMATION
/* In case  using  the OTE automation we want to work with splitter, there we need to disable all the tx enable port except to the first port */
   CLI_F_FlexmuxPoke("0 0 0x88001040 0x00000001");
   CLI_F_FlexmuxPoke("0 0 0x88001044 0x00000001");
   CLI_F_FlexmuxPoke("0 0 0x88001048 0x00000000");
   CLI_F_FlexmuxPoke("0 0 0x8800104C 0x00000000");
   CLI_F_FlexmuxPoke("0 0 0x88001050 0x00000000");
   CLI_F_FlexmuxPoke("0 0 0x88001054 0x00000000");
   CLI_F_FlexmuxPoke("0 0 0x88001058 0x00000000");
   CLI_F_FlexmuxPoke("0 0 0x8800105C 0x00000000");
   
#endif


#ifdef WT_UFE_FRAMER
#if (!(WTI_CESOP_TDI) && (WTI_CESOP_REGRESSION_TEST))
#if WT_RUN_PRBS_CHECK
   WP_U8 prbs_errors = 0;
   WP_U8 prbs_tmp = 0;


   for (i=WT_FIRST_LINE_INDEX;i< (WT_FIRST_LINE_INDEX+num_of_lines);i++)
   {
      prbs_tmp = WTI_FlexmuxCheckPrbsResult(0,i,framing_mode);
      prbs_errors |= prbs_tmp;
      if(prbs_tmp)
      {
         printf("PRBS check failed for tu %d\n",i);
      }
   }
   if (prbs_errors == 0)
   {
      printf("PRBS check Passed\n");
   }
   else
   {
      printf("PRBS check failed\n");
   }
#endif
#endif
#endif

#ifdef WT_UFE_FRAMER
#if WTI_CHECK_PORT_SWITCHING
   {
      WP_U32 line_port_id = 0;
      for (line_port_id = 0; line_port_id<2; line_port_id++)
      {
         WTI_FlexmuxForceB(0,line_port_id);
         WTI_FlexmuxForceA(0,line_port_id);
         WTI_FlexmuxForceA(0,line_port_id);
         WTI_FlexmuxForceB(0,line_port_id);
         WTI_FlexmuxForceB(0,line_port_id);
         WTI_FlexmuxForceA(0,line_port_id);
         WTI_FlexmuxForceB(0,line_port_id);
         WTI_FlexmuxForceA(0,line_port_id);
      }
   }
#endif
#endif
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhT1FramedNoCas(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines, vc_type, physical_param;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_flexmux_physical_conf physical_configuration;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
   WP_boolean is_e1 = WP_FALSE;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &interface,&physical_param,&vc_type,&num_of_lines);
   if (res != 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

#if WTI_DUAL_EMPHY
   num_of_lines = 2 * num_of_lines;
#endif
   test_setup.framing_mode = WUFE_FRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = WTI_T1_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.vc_type = vc_type;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);

   switch(interface)
   {
      case 0:
         test_setup.transfer_type = WUFE_SDH_TYPE_T1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
         if(vc_type == 0)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         else if(vc_type == 1)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
         else
         {
            WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-VC3, 1-VC4 ", __LINE__);
            return;
         }
#endif
         break;
      case 1:
         test_setup.transfer_type = WUFE_SONET_TYPE_T1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         test_setup.transfer_type = WUFE_CAD_TYPE_T1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }

#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:physical_configuration = WT_FLEXMUX_UFE_412_1_PORT_OC12;
         break;
      case 1:physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3;
         break;
      case 2:physical_configuration = WT_FLEXMUX_UFE_412_2_PORTS_OC3;
         break;
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value, Allowed values are: 0-1xOC12, 1-4xOC3 2-2xOC3 ", __LINE__);
         return;
   }
#endif

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   physical_configuration,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

#ifdef WT_UFE_FRAMER

   /* interface == 0 ==> SDH; interface == 1 ==> SONET */
   if (interface < 2)
      WTI_enable_performance_monitoring_points(interface, is_e1, vc_type, test_setup.framing_mode);

#endif
#ifdef OTE_AUTOMATION
/* In case  using  the OTE automation we want to work with splitter, there we need to disable all the tx enable port except to the first port */
   CLI_F_FlexmuxPoke("0 0 0x88001040 0x00000001");
   CLI_F_FlexmuxPoke("0 0 0x88001044 0x00000001");
   CLI_F_FlexmuxPoke("0 0 0x88001048 0x00000000");
   CLI_F_FlexmuxPoke("0 0 0x8800104C 0x00000000");
   CLI_F_FlexmuxPoke("0 0 0x88001050 0x00000000");
   CLI_F_FlexmuxPoke("0 0 0x88001054 0x00000000");
   CLI_F_FlexmuxPoke("0 0 0x88001058 0x00000000");
   CLI_F_FlexmuxPoke("0 0 0x8800105C 0x00000000");
   
#endif

#ifdef WT_UFE_FRAMER
#if ( !(WTI_CESOP_TDI) && (WTI_CESOP_REGRESSION_TEST) )
#if WT_RUN_PRBS_CHECK
   WP_U8 prbs_errors = 0;
   WP_U8 prbs_tmp = 0;


   for (i=WT_FIRST_LINE_INDEX;i< (WT_FIRST_LINE_INDEX+num_of_lines);i++)
   {
      prbs_tmp = WTI_FlexmuxCheckPrbsResult(0,i,framing_mode);
      prbs_errors |= prbs_tmp;
      if(prbs_tmp)
      {
         printf("PRBS check failed for tu %d\n",i);
      }
   }
   if (prbs_errors == 0)
   {
      printf("PRBS check Passed\n");
   }
   else
   {
      printf("PRBS check failed\n");
   }
#endif
#endif
#endif

#ifdef WT_UFE_FRAMER
#if WTI_CHECK_PORT_SWITCHING
   {
      WP_U32 line_port_id = 0;
      for (line_port_id = 0; line_port_id<2; line_port_id++)
      {
         WTI_FlexmuxForceB(0,line_port_id);
         WTI_FlexmuxForceA(0,line_port_id);
         WTI_FlexmuxForceA(0,line_port_id);
         WTI_FlexmuxForceB(0,line_port_id);
         WTI_FlexmuxForceB(0,line_port_id);
         WTI_FlexmuxForceA(0,line_port_id);
         WTI_FlexmuxForceB(0,line_port_id);
         WTI_FlexmuxForceA(0,line_port_id);
      }
   }
#endif
#endif

}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhT1FramedWithCas(char *StrPrm)
{
   WP_U32 res, cmd, i, num_of_lines, num_slots;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U32 vc_type = WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
   WP_boolean is_e1 = WP_FALSE;
#endif

   framing_mode = WUFE_FRAMED;
   cas_mode = WUFE_CAS_ENABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &interface, &num_of_lines);
   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   switch(interface)
   {
      case 0:
         transfer_type = WUFE_SDH_TYPE_T1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         transfer_type = WUFE_SONET_TYPE_T1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         transfer_type = WUFE_CAD_TYPE_T1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }

   num_slots = WTI_T1_SLOTS;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX; i< (num_of_lines+WT_FIRST_LINE_INDEX) ;i++)
   {
      sprintf(temp_buf, "0 %d %d %d",i,transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d",i, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   for (i=0;i<WTI_MAX_NUM_OF_MPLS_FA;++i)
   {
#if WTI_CESOP_MPLS_OVER_ENET
      /* create the l2 router MPLS flow aggregations */
      CLI_F_Tdm2PsnFlowAggEnetHeaderMpls("0 aa aa aa aa aa aa bb bb bb bb bb bb 8847");
      CLI_F_MplsFlowAggPrefixLength("0 18");
      sprintf(temp_buf,
              "0 %x %x %x %x %x %x",
              3*i,
              3*i+1,
              3*i+2,
              0x81000005,
              (0x88470000 | ((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) >> 4)),
              ((((((WTI_MPLS_LABEL) >> 12)  + WTI_MAX_PW + 1 + i) & 0xff) << 28) | 0x00ff0000));
      /*                              [mac destination] [mac source] [vlan] [type] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d %d", 2, 28, 41); /* remark offsets for VLAN priority and PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
#if WTI_CESOP_NATIVE_MPLS_IW
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 0");
      sprintf(temp_buf,
              "0 %x",
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 5); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 4");
      sprintf(temp_buf,
              "0 %x %x",
              WTI_MPLSoPPP,
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [ppp] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 13); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#endif /* WTI_CESOP_NATIVE_MPLS_IW */
#endif /* WTI_CESOP_MPLS_OVER_ENET */
   }

   for (i=WT_FIRST_LINE_INDEX; i< (num_of_lines+WT_FIRST_LINE_INDEX) ;i++)
   {
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1, NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WTI_SOCKET_LOOPBACK_MODE);

      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64",i,i);
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d 48 48 %d 0-24",i,i,num_slots);
         CLI_F_PwConfigureFramed(temp_buf);
      }
      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 192");

      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", i+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");

      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 32");

      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(i%WTI_MAX_NUM_OF_MPLS_FA),i);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",i);
      CLI_F_PwEnable(temp_buf);
   }

#ifdef WT_UFE_FRAMER

   /* interface == 0 ==> SDH; interface == 1 ==> SONET */
   if (interface < 2)
      WTI_enable_performance_monitoring_points(interface, is_e1, vc_type, framing_mode);

#endif
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhClearChannel(char *StrPrm)
{
   WP_U32 res, cmd, i,j, num_of_lines, num_slots, pw_index=0,fpga_mode;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WTI_flexmux_physical_conf physical_configuration;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   framing_mode = WUFE_UNFRAMED;
   cas_mode = WUFE_CAS_DISABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d", &cmd, &interface);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters -- 2 required", __LINE__);
      return;
   }

   switch(interface)
   {
      case 0:
         transfer_type = WUFE_SDH_TYPE_VC44C;
         num_of_lines = 1;
         fpga_mode = 0;
#ifdef WT_UFE_FRAMER
         physical_configuration = WT_FLEXMUX_UFE_412_1_PORT_OC12_CLEAR_CHANNEL;
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         transfer_type = WUFE_SDH_TYPE_VC4;
         num_of_lines = 4;
         fpga_mode = 0;
#ifdef WT_UFE_FRAMER
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CLEAR_CHANNEL;
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 2:
         transfer_type = WUFE_SONET_TYPE_STS12;
         num_of_lines = 1;
         fpga_mode = 1;
#ifdef WT_UFE_FRAMER
         physical_configuration = WT_FLEXMUX_UFE_412_1_PORT_OC12_CLEAR_CHANNEL;
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 3:
          transfer_type = WUFE_SONET_TYPE_STS3;
          num_of_lines = 4;
          fpga_mode = 1;
#ifdef WT_UFE_FRAMER
          physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CLEAR_CHANNEL;
          flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
          break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }
   num_slots = WTI_E1_SLOTS;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */

   sprintf(temp_buf, "0 %d",fpga_mode);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=0; i < num_of_lines; ++i)
   {
      j = i*84;
      sprintf(temp_buf, "0 %d %d %d", j, transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d", i, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER

   /* Initialize the Flexmux framer in SDH mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   physical_configuration,
                   flexmux_mode,
                   0,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial 1 to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
/*  WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_1_CPLD_INTERRUPT_MASK);*/
#endif

   for (i=0;i<WTI_MAX_NUM_OF_MPLS_FA;++i)
   {
#if WTI_CESOP_MPLS_OVER_ENET
      /* create the l2 router MPLS flow aggregations */
      CLI_F_Tdm2PsnFlowAggEnetHeaderMpls("0 aa aa aa aa aa aa bb bb bb bb bb bb 8847");
      CLI_F_MplsFlowAggPrefixLength("0 18");
      sprintf(temp_buf,
              "0 %x %x %x %x %x %x",
              3*i,
              3*i+1,
              3*i+2,
              0x81000005,
              (0x88470000 | ((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) >> 4)),
              ((((((WTI_MPLS_LABEL) >> 12)  + WTI_MAX_PW + 1 + i) & 0xff) << 28) | 0x00ff0000));
      /*                              [mac destination] [mac source] [vlan] [type] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d %d", 2, 28, 41); /* remark offsets for VLAN priority and PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
#if WTI_CESOP_NATIVE_MPLS_IW
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 0");
      sprintf(temp_buf,
              "0 %x",
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 5); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 4");
      sprintf(temp_buf,
              "0 %x %x",
              WTI_MPLSoPPP,
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [ppp] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 13); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#endif /* WTI_CESOP_NATIVE_MPLS_IW */
#endif /* WTI_CESOP_MPLS_OVER_ENET */
   }

   for (i=0 ;i < num_of_lines; ++i)
   {
          j = i*84;

          /* UFE line configuration, creation and enabling */
          WT_UfeLineConfigureAndCreateClearChannel(&the_system->ufe, j, transfer_type, framing_mode, cas_mode, 1, NULL);
#if USE_UFE
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, j, WTI_SOCKET_LOOPBACK_MODE);

      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64",j, i);
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64 %d 0-31",j, i, num_slots);
         CLI_F_PwConfigureFramed(temp_buf);
      }
      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);
      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 256");
      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", i+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");
      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 8");
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(i%WTI_MAX_NUM_OF_MPLS_FA),i);
      CLI_F_PwCreateCascaded(temp_buf);
      sprintf(temp_buf, "0 %d 0",i);
      CLI_F_PwEnable(temp_buf);
      pw_index++;
#endif
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhE1FramedNoCas_8xDS0_1008_PWs(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
   WP_boolean is_e1 = WP_TRUE;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &interface, &num_of_lines);
   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
#if WTI_DUAL_EMPHY
   num_of_lines = 2 * num_of_lines;
#endif

   test_setup.framing_mode = WUFE_FRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = 8;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 4;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.vc_type = WTI_FLEXMUX_PHYSICAL_CONF;
#endif

   switch(interface)
   {
   case 0:
      test_setup.transfer_type = WUFE_SDH_TYPE_E1;
#ifdef WT_UFE_FRAMER
      flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
      break;
   case 1:
      test_setup.transfer_type = WUFE_SONET_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET", __LINE__);
         return;
   }


   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,

                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

#ifdef WT_UFE_FRAMER

   /* interface == 0 ==> SDH; interface == 1 ==> SONET */
   if (interface < 2)
      WTI_enable_performance_monitoring_points(interface, is_e1, test_setup.vc_type, test_setup.framing_mode);

#endif
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/

void CLI_F_SonetSdhT1FramedNoCas_6xDS0_1344_PWs(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
   WP_boolean is_e1 = WP_FALSE;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &interface, &num_of_lines);
   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
#if WTI_DUAL_EMPHY
   num_of_lines = 2 * num_of_lines;
#endif

   test_setup.framing_mode = WUFE_FRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = 6;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 4;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.vc_type = WTI_FLEXMUX_PHYSICAL_CONF;
#endif

   switch(interface)
   {
      case 0:
         test_setup.transfer_type = WUFE_SDH_TYPE_T1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         test_setup.transfer_type = WUFE_SONET_TYPE_T1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         test_setup.transfer_type = WUFE_CAD_TYPE_T1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

#ifdef WT_UFE_FRAMER

   /* interface == 0 ==> SDH; interface == 1 ==> SONET */
   if (interface < 2)
      WTI_enable_performance_monitoring_points(interface, is_e1, test_setup.vc_type, test_setup.framing_mode);

#endif
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdh3DS31VFP3DS328T1(char *StrPrm)
{
   WP_U32 res, cmd, i, num_of_lines,vc_type, physical_param,num_slots, num_slots_ds3, num_slots_t1,pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type, transfer_type_t1, transfer_type_ds3, transfer_type_vfp;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WTI_flexmux_physical_conf physical_configuration;
   WP_U8 flexmux_mode=0;
#endif

   framing_mode = WUFE_UNFRAMED;
   cas_mode = WUFE_CAS_DISABLE;
   num_of_lines= 336;
   
   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &interface,&physical_param,&vc_type);
   if (res != 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   switch(interface)
   {
      case 0:
         transfer_type_t1 = WUFE_SDH_TYPE_T1;
         transfer_type_ds3 = WUFE_SDH_TYPE_DS3;
         transfer_type_vfp = WUFE_SDH_TYPE_VC4;

#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
         if(vc_type == 0)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         else if(vc_type == 1)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
         else
         {
            WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-VC3, 1-VC4 ", __LINE__);
            return;
         }
#endif
         break;
      case 1:
         transfer_type_t1 = WUFE_SONET_TYPE_T1;
         transfer_type_ds3 = WUFE_SONET_TYPE_DS3;
         transfer_type_vfp = WUFE_SONET_TYPE_STS3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }

#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:
         physical_configuration = WT_FLEXMUX_UFE_412_1_PORT_OC12;
         break;
      case 1:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3;
         break;
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value\n"
                                 "Allowed values are: 0-1xOC12, 1-4xOC3", __LINE__);
         return;
   }
#endif

   num_slots_ds3 = WTI_DS3_SLOTS;
   num_slots_t1 = WTI_T1_SLOTS;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX;i<(num_of_lines+WT_FIRST_LINE_INDEX);i++)
   {
      if (i<84 || (i>=168 && i<252))
      {
         if(i%28!=0)
            continue;
         transfer_type = transfer_type_ds3;
      }
      else if (i>=84 && i<168)
      {
         if(i!=84)
            continue;
         transfer_type = transfer_type_vfp;
      }
      else
      {
         transfer_type = transfer_type_t1;
      }
      sprintf(temp_buf, "0 %d %d %d",i,transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d",i, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   physical_configuration,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   for (i=0;i<WTI_MAX_NUM_OF_MPLS_FA;++i)
   {
#if WTI_CESOP_MPLS_OVER_ENET
      /* create the l2 router MPLS flow aggregations */
      CLI_F_Tdm2PsnFlowAggEnetHeaderMpls("0 aa aa aa aa aa aa bb bb bb bb bb bb 8847");
      CLI_F_MplsFlowAggPrefixLength("0 18");
      sprintf(temp_buf,
              "0 %x %x %x %x %x %x",
              3*i,
              3*i+1,
              3*i+2,
              0x81000005,
              (0x88470000 | ((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) >> 4)),
              ((((((WTI_MPLS_LABEL) >> 12)  + WTI_MAX_PW + 1 + i) & 0xff) << 28) | 0x00ff0000));
      /*                              [mac destination] [mac source] [vlan] [type] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d %d", 2, 28, 41); /* remark offsets for VLAN priority and PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
#if WTI_CESOP_NATIVE_MPLS_IW
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 0");
      sprintf(temp_buf,
              "0 %x",
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 5); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 4");
      sprintf(temp_buf,
              "0 %x %x",
              WTI_MPLSoPPP,
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [ppp] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 13); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#endif /* WTI_CESOP_NATIVE_MPLS_IW */
#endif /* WTI_CESOP_MPLS_OVER_ENET */
   }

   for (i=WT_FIRST_LINE_INDEX;i< (num_of_lines+WT_FIRST_LINE_INDEX) ;i++)
   {
      if (i<84 || (i>=168 && i<252))
      {
         if(i%28!=0)
            continue;
         num_slots = num_slots_ds3;
         transfer_type = transfer_type_ds3;
         WT_UfeLineConfigureAndCreateE3DS3(&the_system->ufe, i, transfer_type, framing_mode, WT_FLEXMUX_CONNECTION_TYPE_DS3_UNFRAMED, cas_mode, 1, NULL);
      }
      else if (i>=84 && i<168)
      {
         if(i!=84)
            continue;
         num_slots = 1;
         transfer_type = transfer_type_vfp;
         WT_UfeLineConfigureAndCreateClearChannel(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1, NULL);
      }
      else
      {
         num_slots = num_slots_t1;
         transfer_type = transfer_type_t1;
         WT_UfeLineConfigureAndCreateT1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1, NULL);
      }
      printf("Created line %d, phy %d\n",i,pw_index);
      
      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64",i,pw_index);
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64 %d 0-31",i,pw_index,num_slots);
         CLI_F_PwConfigureFramed(temp_buf);
      }
      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 130");

      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", i+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");

      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 32");

      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
      pw_index++;
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdh3DS32VFP3DS3(char *StrPrm)
{

}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhE1T1Combined(char *StrPrm)
{
   WP_U32 res, cmd, i, num_of_lines, num_slots, num_slots_e1, num_slots_t1,pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type, transfer_type_e1, transfer_type_t1;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   framing_mode = WUFE_UNFRAMED;
   cas_mode = WUFE_CAS_DISABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &interface, &num_of_lines);
   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   switch(interface)
   {
      case 0:
         transfer_type_e1 = WUFE_SDH_TYPE_E1;
         transfer_type_t1 = WUFE_SDH_TYPE_T1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         transfer_type_e1 = WUFE_SONET_TYPE_E1;
         transfer_type_t1 = WUFE_SONET_TYPE_T1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         transfer_type_e1 = WUFE_CAD_TYPE_E1;
         transfer_type_t1 = WUFE_CAD_TYPE_T1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }

   num_slots_e1 = WTI_E1_SLOTS;
   num_slots_t1 = WTI_T1_SLOTS;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX;i<(num_of_lines+WT_FIRST_LINE_INDEX);i++)
   {
      if (i<168)
      {
         transfer_type = transfer_type_e1;
      }
      else
      {
         transfer_type = transfer_type_t1;
      }
      sprintf(temp_buf, "0 %d %d %d",i,transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d",i, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   for (i=0;i<WTI_MAX_NUM_OF_MPLS_FA;++i)
   {
#if WTI_CESOP_MPLS_OVER_ENET
      /* create the l2 router MPLS flow aggregations */
      CLI_F_Tdm2PsnFlowAggEnetHeaderMpls("0 aa aa aa aa aa aa bb bb bb bb bb bb 8847");
      CLI_F_MplsFlowAggPrefixLength("0 18");
      sprintf(temp_buf,
              "0 %x %x %x %x %x %x",
              3*i,
              3*i+1,
              3*i+2,
              0x81000005,
              (0x88470000 | ((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) >> 4)),
              ((((((WTI_MPLS_LABEL) >> 12)  + WTI_MAX_PW + 1 + i) & 0xff) << 28) | 0x00ff0000));
      /*                              [mac destination] [mac source] [vlan] [type] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d %d", 2, 28, 41); /* remark offsets for VLAN priority and PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
#if WTI_CESOP_NATIVE_MPLS_IW
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 0");
      sprintf(temp_buf,
              "0 %x",
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 5); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 4");
      sprintf(temp_buf,
              "0 %x %x",
              WTI_MPLSoPPP,
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [ppp] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 13); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#endif /* WTI_CESOP_NATIVE_MPLS_IW */
#endif /* WTI_CESOP_MPLS_OVER_ENET */
   }

   for (i=WT_FIRST_LINE_INDEX;i< (num_of_lines+WT_FIRST_LINE_INDEX) ;i++)
   {
      if (i<168)
      {
         if(i%28>20)
            continue;
         num_slots = num_slots_e1;
         transfer_type = transfer_type_e1;
      }
      else
      {
         num_slots = num_slots_t1;
         transfer_type = transfer_type_t1;
      }

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1T1(&the_system->ufe, i,transfer_type, framing_mode, cas_mode, 1, NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WTI_SOCKET_LOOPBACK_MODE);

      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64",i,pw_index);
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64 %d 0-31",i,pw_index,num_slots);
         CLI_F_PwConfigureFramed(temp_buf);
      }
      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 130");

      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", i+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");

      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 32");

      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
      pw_index++;
   }
}

void CLI_F_SonetSdh252E1_336T1_252E1(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d", &cmd, &interface);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
#if WTI_DUAL_EMPHY
   num_of_lines = 504;
#else
   num_of_lines = 252;
#endif
   test_setup.framing_mode = WUFE_UNFRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = WTI_E1_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.vc_type = WTI_FLEXMUX_PHYSICAL_CONF;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);

   switch(interface)
   {
      case 0:
         test_setup.transfer_type = WUFE_SDH_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         test_setup.transfer_type = WUFE_SONET_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         test_setup.transfer_type = WUFE_CAD_TYPE_E1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SDH mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

#if defined(__linux__)
   /* (4) Unmask FPGA interrupt sources to allow serial 1 to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

   /* Check the WP statistics */
   WPL_Delay(2000000);
   printf("CLI_F_Pwe3StatisticsClear\n");
   sprintf(temp_buf, "7");
   CLI_F_Pwe3StatisticsClear(temp_buf);

   printf("CLI_F_Pwe3StatisticsCheck\n");
#if WTI_DUAL_EMPHY
   sprintf(temp_buf, "8 0 504 1");
#else
   sprintf(temp_buf, "8 0 252 1");
#endif
   CLI_F_Pwe3StatisticsCheck(temp_buf);

   /* Delete the phys lins devices and channels */
   printf("finish create the E1 lines,check the data. press any key to continue\n");
   getchar();

   printf("start delete all E1 phys and lines\n");
   /* Remove all PWs: last to first remove */
   sprintf(temp_buf, "0 %d", /*line_delete*/ 1);
   CLI_F_UfeLinePWDelete(temp_buf);

   printf("\nstart create  T1 phys and lines...\n");

#if WTI_DUAL_EMPHY
   num_of_lines = 672;
#else
   num_of_lines = 336;
#endif
   test_setup.framing_mode = WUFE_UNFRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = WTI_T1_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.vc_type = WTI_FLEXMUX_PHYSICAL_CONF;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);

   switch(interface)
   {
      case 0:
         test_setup.transfer_type = WUFE_SDH_TYPE_T1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         test_setup.transfer_type = WUFE_SONET_TYPE_T1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         test_setup.transfer_type = WUFE_CAD_TYPE_T1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }
   WT_UfeLinePWCreate(&test_setup, 1);

   /* Check the WP statistics */
   WPL_Delay(2000000);
   printf("CLI_F_Pwe3StatisticsClear\n");
   sprintf(temp_buf, "7");
   CLI_F_Pwe3StatisticsClear(temp_buf);

   printf("CLI_F_Pwe3StatisticsCheck\n");
#if WTI_DUAL_EMPHY
   sprintf(temp_buf, "8 0 672 1");
#else
   sprintf(temp_buf, "8 0 336 1");
#endif
   CLI_F_Pwe3StatisticsCheck(temp_buf);

   printf("finish create the T1 lines, check the data, press any key to continue\n");
   getchar();

   printf("start delete all T1 phys and lines\n");
   /* Remove all PWs: last to first remove */
   sprintf(temp_buf, "0 %d", /*line_delete*/ 1);
   CLI_F_UfeLinePWDelete(temp_buf);

   printf("\nstart create  E1 phys and lines...\n");
#if WTI_DUAL_EMPHY
   num_of_lines = 504;
#else
   num_of_lines = 252;
#endif
   test_setup.framing_mode = WUFE_UNFRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = WTI_E1_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.vc_type = WTI_FLEXMUX_PHYSICAL_CONF;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);

   switch(interface)
   {
      case 0:
         test_setup.transfer_type = WUFE_SDH_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         test_setup.transfer_type = WUFE_SONET_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         test_setup.transfer_type = WUFE_CAD_TYPE_E1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }
   WT_UfeLinePWCreate(&test_setup, 1);


   /* Check the WP statistics */
   WPL_Delay(2000000);
   printf("CLI_F_Pwe3StatisticsClear\n");
   sprintf(temp_buf, "7");
   CLI_F_Pwe3StatisticsClear(temp_buf);

   printf("CLI_F_Pwe3StatisticsCheck\n");
#if WTI_DUAL_EMPHY
   sprintf(temp_buf, "8 0 504 1");
#else
   sprintf(temp_buf, "8 0 252 1");
#endif
   CLI_F_Pwe3StatisticsCheck(temp_buf);

   /* Delete the phys lins devices and channels */
   printf("finish create the E1 lines,check the data.press any key to continue\n");
   getchar();
}
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhSystemReconfigure(char *StrPrm)
{
   WP_U32 res, cmd, i,j, num_of_lines, num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
   WUFE_idle_pattern new_idle_pattern;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;

#endif

   framing_mode = WUFE_UNFRAMED;
   cas_mode = WUFE_CAS_DISABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d", &cmd, &interface);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   switch(interface)
   {
      case 0:
         transfer_type = WUFE_SDH_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         transfer_type = WUFE_SONET_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         transfer_type = WUFE_CAD_TYPE_E1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }

   num_of_lines = 252;
   num_slots = WTI_E1_SLOTS;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i = j = WT_FIRST_LINE_INDEX; j < (WT_FIRST_LINE_INDEX + num_of_lines); ++i)
   {
      if ((i % 28) > 20)
      {
         continue;
      }

      ++j;

      sprintf(temp_buf, "0 %d %d %d", i, transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d", i, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   memset(&new_idle_pattern, 0, sizeof(WUFE_idle_pattern));
   new_idle_pattern.tx_idle_byte_data_pattern=0x18;
   new_idle_pattern.tx_idle_nibble_cas_pattern=0x8;
   WT_UfeSystemIdleModify(&the_system->ufe, &new_idle_pattern);

   for (i=0;i<WTI_MAX_NUM_OF_MPLS_FA;++i)
   {
#if WTI_CESOP_MPLS_OVER_ENET
      /* create the l2 router MPLS flow aggregations */
      CLI_F_Tdm2PsnFlowAggEnetHeaderMpls("0 aa aa aa aa aa aa bb bb bb bb bb bb 8847");
      CLI_F_MplsFlowAggPrefixLength("0 18");
      sprintf(temp_buf,
              "0 %x %x %x %x %x %x",
              3*i,
              3*i+1,
              3*i+2,
              0x81000005,
              (0x88470000 | ((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) >> 4)),
              ((((((WTI_MPLS_LABEL) >> 12)  + WTI_MAX_PW + 1 + i) & 0xff) << 28) | 0x00ff0000));
      /*                              [mac destination] [mac source] [vlan] [type] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d %d", 2, 28, 41); /* remark offsets for VLAN priority and PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);


#else
#if WTI_CESOP_NATIVE_MPLS_IW
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 0");
      sprintf(temp_buf,
              "0 %x",
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 5); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 4");
      sprintf(temp_buf,
              "0 %x %x",
              WTI_MPLSoPPP,
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [ppp] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 13); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#endif /* WTI_CESOP_NATIVE_MPLS_IW */
#endif /* WTI_CESOP_MPLS_OVER_ENET */
   }

   for (i = j = WT_FIRST_LINE_INDEX; j < (WT_FIRST_LINE_INDEX + num_of_lines); ++i)
   {
      if ((i % 28) > 20)
      {
         continue;
      }

      ++j;

      /* UFE line configuration, creation and enabling */

      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1, NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WTI_SOCKET_LOOPBACK_MODE);
      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64",i, pw_index);
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64 %d 0-31",i, pw_index, num_slots);
         CLI_F_PwConfigureFramed(temp_buf);
      }
      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 256");

      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", i+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");

      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 32");

      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0", pw_index);
      CLI_F_PwEnable(temp_buf);

      pw_index++;
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhLineReconfigure(char *StrPrm)
{
   WP_U32 res, cmd, i,j, num_of_lines, num_slots ,pw_index = 0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
   WUFE_line_sonet_reconfig new_sonet_config;
   WUFE_line_sdh_reconfig new_sdh_config;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;

#endif

   framing_mode = WUFE_UNFRAMED;
   cas_mode = WUFE_CAS_DISABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d", &cmd, &interface);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   switch(interface)
   {
      case 0:
         transfer_type = WUFE_SDH_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         transfer_type = WUFE_SONET_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         transfer_type = WUFE_CAD_TYPE_E1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }

   num_of_lines = WT_MAX_LINE_INDEX;
   num_slots = WTI_E1_SLOTS;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i = j = WT_FIRST_LINE_INDEX; j < (WT_FIRST_LINE_INDEX + num_of_lines); ++i)
   {
      if ((i % 28) > 20)
      {
         continue;
      }

      ++j;

      sprintf(temp_buf, "0 %d %d %d", i, transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d", i, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   for (i=0;i<WTI_MAX_NUM_OF_MPLS_FA;++i)
   {
#if WTI_CESOP_MPLS_OVER_ENET
      /* create the l2 router MPLS flow aggregations */
      CLI_F_Tdm2PsnFlowAggEnetHeaderMpls("0 aa aa aa aa aa aa bb bb bb bb bb bb 8847");
      CLI_F_MplsFlowAggPrefixLength("0 18");
      sprintf(temp_buf,
              "0 %x %x %x %x %x %x",
              3*i,
              3*i+1,
              3*i+2,
              0x81000005,
              (0x88470000 | ((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) >> 4)),
              ((((((WTI_MPLS_LABEL) >> 12)  + WTI_MAX_PW + 1 + i) & 0xff) << 28) | 0x00ff0000));
      /*                              [mac destination] [mac source] [vlan] [type] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d %d", 2, 28, 41); /* remark offsets for VLAN priority and PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
#if WTI_CESOP_NATIVE_MPLS_IW
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 0");
      sprintf(temp_buf,
              "0 %x",
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 5); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 4");
      sprintf(temp_buf,
              "0 %x %x",
              WTI_MPLSoPPP,
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [ppp] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 13); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#endif /* WTI_CESOP_NATIVE_MPLS_IW */
#endif /* WTI_CESOP_MPLS_OVER_ENET */
   }

   for (i = j = WT_FIRST_LINE_INDEX; j < (WT_FIRST_LINE_INDEX + num_of_lines); ++i)
   {
      if ((i % 28) > 20)
      {
         continue;
      }

      ++j;

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1, NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WTI_SOCKET_LOOPBACK_MODE);


      if (interface == 0)
      {
         memset(&new_sdh_config, 0, sizeof(WUFE_line_sdh_reconfig));
         new_sdh_config.cas_enable = WUFE_CAS_ENABLE;
         WT_UfeLineSdhModify(&the_system->ufe, i, &new_sdh_config);

      }
      else
      {
         memset(&new_sonet_config, 0, sizeof(WUFE_line_sonet_reconfig));
         new_sonet_config.cas_enable = WUFE_CAS_ENABLE;
         WT_UfeLineSonetModify(&the_system->ufe, i, &new_sonet_config);
      }

      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64",i,pw_index);
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64 %d 0-31",i, pw_index, num_slots);
         CLI_F_PwConfigureFramed(temp_buf);
      }
      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 256");

      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", i+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");

      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 32");

      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);
      sprintf(temp_buf, "0 %d 0", pw_index);
      CLI_F_PwEnable(temp_buf);

      pw_index++;
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhPhyReconfigure(char *StrPrm)
{
   WP_U32 res, cmd, i, j,num_of_lines, num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
   WUFE_phy new_phy_cfg;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   framing_mode = WUFE_FRAMED;
   cas_mode = WUFE_CAS_DISABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d", &cmd, &interface);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   switch(interface)
   {
      case 0:
         transfer_type = WUFE_SDH_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         transfer_type = WUFE_SONET_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         transfer_type = WUFE_CAD_TYPE_E1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }

   num_of_lines = WT_MAX_LINE_INDEX;
   num_slots = WTI_E1_SLOTS;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX;i< (num_of_lines+WT_FIRST_LINE_INDEX); i++)
   {
      if ((i % 28) > 20)
      {
         continue;
      }

//      j++;
      sprintf(temp_buf, "0 %d %d %d",i,transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d",i, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   for (i=0;i<WTI_MAX_NUM_OF_MPLS_FA;++i)
   {
#if WTI_CESOP_MPLS_OVER_ENET
      /* create the l2 router MPLS flow aggregations */
      CLI_F_Tdm2PsnFlowAggEnetHeaderMpls("0 aa aa aa aa aa aa bb bb bb bb bb bb 8847");
      CLI_F_MplsFlowAggPrefixLength("0 18");
      sprintf(temp_buf,
              "0 %x %x %x %x %x %x",
              3*i,
              3*i+1,
              3*i+2,
              0x81000005,
              (0x88470000 | ((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) >> 4)),
              ((((((WTI_MPLS_LABEL) >> 12)  + WTI_MAX_PW + 1 + i) & 0xff) << 28) | 0x00ff0000));
      /*                              [mac destination] [mac source] [vlan] [type] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d %d", 2, 28, 41); /* remark offsets for VLAN priority and PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
#if WTI_CESOP_NATIVE_MPLS_IW
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 0");
      sprintf(temp_buf,
              "0 %x",
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 5); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 4");
      sprintf(temp_buf,
              "0 %x %x",
              WTI_MPLSoPPP,
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [ppp] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 13); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#endif /* WTI_CESOP_NATIVE_MPLS_IW */
#endif /* WTI_CESOP_MPLS_OVER_ENET */
   }


   for (i = WT_FIRST_LINE_INDEX; i < (WT_FIRST_LINE_INDEX + num_of_lines); ++i)
   {
      if ((i % 28) > 20)
      {
         continue;
      }

      ++j;

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1, NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WTI_SOCKET_LOOPBACK_MODE);


      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64",i,pw_index);
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64 %d 0-31",i, pw_index, num_slots);
         CLI_F_PwConfigureFramed(temp_buf);
      }
      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 256");

      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", i+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");

      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 32");

      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      memset(&new_phy_cfg, 0xff, sizeof(WUFE_phy));
      new_phy_cfg.rx_slot_group.n_slots = 8;
//      new_phy_cfg.tx_slot_group.n_slots = 8;
      for (j=0;j<8;j++)
      {
         //  new_phy_cfg.tx_slot_group.slot_arr[j] = j;
         new_phy_cfg.rx_slot_group.slot_arr[j] = j;
      }

      /* Disable the phy before phy modifing*/
      WT_UfePhyDisable(&the_system->ufe, pw_index);

      WT_UfePhySlotsModify(&the_system->ufe, pw_index , &new_phy_cfg);

      /* Enable the phy after phy modifing*/

      WT_UfePhyEnable(&the_system->ufe, pw_index);

      sprintf(temp_buf, "0 %d 0", pw_index);
      CLI_F_PwEnable(temp_buf);

      pw_index++;
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhT1PhyCreateDelete(char *StrPrm)
{


   WP_U32 res, cmd, num_of_lines, iterations;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d", &cmd, &interface);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
#if WTI_DUAL_EMPHY
   num_of_lines = 672;
#else
   num_of_lines = 336;
#endif
   test_setup.framing_mode = WUFE_UNFRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = WTI_T1_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.vc_type = WTI_FLEXMUX_PHYSICAL_CONF;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);

   switch(interface)
   {
      case 0:
         test_setup.transfer_type = WUFE_SDH_TYPE_T1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         test_setup.transfer_type = WUFE_SONET_TYPE_T1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         test_setup.transfer_type = WUFE_CAD_TYPE_T1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }



   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SDH mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   //WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

   /* starting the remove-recreate sequence. */
   for (iterations=1; iterations<WTI_ITERATIONS_NUM+1; iterations++)
   {
      printf("################ start of iteration %d   ###############\n", iterations);

      /* Remove all PWs: last to first remove */
      sprintf(temp_buf, "0 %d", /*line_delete*/ 0);
      CLI_F_UfeLinePWDelete(temp_buf);

      /* Wait few milisecond before recreate */
      WP_Delay(60000);

      /* recreate lines (if needed) and PWs */
      WT_UfeLinePWCreate(&test_setup, /* line_delete*/ 0);

      printf("Wait few seconds to let the system become stable...\n");
      WP_Delay(DELAY_DURATION_IN_SECONDS * 1000000);
      /* Global statistics check, including reset. */
//       result = CLI_F_GlobalStatisticsCheck("0");
//       if (result == WT_FAIL)
//       {
//          WTI_PRINT_ERROR(("\n Test Failed. Iteration number: %d\n", iterations));
//          return WT_FAIL;
//       }
       printf(">>>>>>>>>>>>>>>> end of iteration %d   <<<<<<<<<<<<<<<\n", iterations);
   } /* for iterations */
#ifdef WT_UFE_FRAMER
#if ( !(WTI_CESOP_TDI) && (WTI_CESOP_REGRESSION_TEST) )
#if WT_RUN_PRBS_CHECK
            WP_U8 prbs_errors = 0;

            prbs_errors |= WTI_FlexmuxCheckPrbsResult(0,1,framing_mode);
            if (prbs_errors == 0)
            {
               printf("PRBS check Passed\n");
            }
            else
            {
               printf("PRBS check failed\n");
            }
#endif
#endif
#endif
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/

void CLI_F_SonetSdhE1PhyEnableDisable(char *StrPrm)
{
   WP_U32 res, cmd, i, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface, option = 4;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
   //WP_boolean is_e1 = WP_TRUE;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &interface, &option);
   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   test_setup.framing_mode = WUFE_UNFRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
#if WTI_DUAL_EMPHY
   test_setup.num_of_lines = 504;
#else
   test_setup.num_of_lines = 252;
#endif
   test_setup.num_slots = WTI_E1_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
   test_setup.vc_type = WTI_FLEXMUX_PHYSICAL_CONF;

   strcpy(test_setup.test_name, __FUNCTION__);

   if (option >= 0)
   {
      switch(interface)
      {
         case 0:
            test_setup.transfer_type = WUFE_SDH_TYPE_E1;
#ifdef WT_UFE_FRAMER
            flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
            break;
         case 1:
            test_setup.transfer_type = WUFE_SONET_TYPE_E1;
#ifdef WT_UFE_FRAMER
            flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
            break;
         case 2:
            test_setup.transfer_type = WUFE_CAD_TYPE_E1;
            break;
         default:
            WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
            return;
      }

      /* allocate CESoP system */
      WTI_SystemAlloc();

      /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
      sprintf(temp_buf, "0 %d",interface);
      CLI_F_UfeFpgaMode(temp_buf);

      /* create an empty CESOP system with no PW */
      /*Calls UFE system create and enable*/
      WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
      /* Initialize the Flexmux framer in SDH mode */
      WTI_FlexmuxInit(WTI_EMPHY_PORT,
                      0,
                      WTI_FLEXMUX_PHYSICAL_CONF,
                      flexmux_mode,
                      vc_3_4,
                      &flex_global_params);

      if (interface < 2)
         WTI_enable_alarms_and_performance_monitoring(interface);

      /* Unmask FPGA interrupt sources to allow serial to interrupt us */
      //WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

      WT_UfeMPLS_L2_FA_Create();
      WT_UfeLinePWCreate(&test_setup, 1);

   }

   if (option >= 1)

   {
      printf("Disbling all the phys...\n");
      for (pw_index = 252;pw_index>0;--pw_index)
      {
         WT_UfePhyDisable(&the_system->ufe,pw_index-1 );
      }
#if WTI_DUAL_EMPHY
      for (pw_index = 504;pw_index>252;--pw_index)
      {
         WT_UfePhyDisable(&the_system->second_ufe, pw_index-1);
      }
#endif

   }


   if (option >= 2)
   {
      printf("Enabling all the phys...\n");
      for (pw_index = 0;pw_index<252;++pw_index)
      {
         WT_UfePhyEnable(&the_system->ufe, pw_index);
      }
#if WTI_DUAL_EMPHY
      for (pw_index = 252;pw_index<504;++pw_index)
      {
         WT_UfePhyEnable(&the_system->second_ufe, pw_index);
      }
#endif
   }
   if (option == 3)
   {
      for(i=0;i<10;i++)
      {
         /* Disable all phys*/
         printf("Disbling all the phys...\n");
         for (pw_index = 252;pw_index>0;--pw_index)
         {
            WT_UfePhyDisable(&the_system->ufe,pw_index-1 );
         }
#if WTI_DUAL_EMPHY
         for (pw_index = 504;pw_index>252;--pw_index)
         {
            WT_UfePhyDisable(&the_system->second_ufe, pw_index-1);
         }
#endif

         /* Enable all phys*/
         printf("Enabling all the phys...\n");
         for (pw_index = 0;pw_index<252;++pw_index)
         {
            WT_UfePhyEnable(&the_system->ufe, pw_index);
         }
#if WTI_DUAL_EMPHY
         for (pw_index = 252;pw_index<504;++pw_index)
         {
            WT_UfePhyEnable(&the_system->second_ufe, pw_index);
         }
#endif
      }
   }
}



/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/

void CLI_F_SonetSdhT1EnableDisable(char *StrPrm)
{



   WP_U32 res, cmd, i,j, num_of_lines, num_slots;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   framing_mode = WUFE_UNFRAMED;
   cas_mode = WUFE_CAS_DISABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d", &cmd, &interface);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   switch(interface)
   {
      case 0:
         transfer_type = WUFE_SDH_TYPE_T1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         transfer_type = WUFE_SONET_TYPE_T1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         transfer_type = WUFE_CAD_TYPE_T1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }

   num_of_lines = WT_MAX_LINE_INDEX;
   num_slots = WTI_T1_SLOTS;


   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX; i<(num_of_lines+WT_FIRST_LINE_INDEX); i++)
   {
      sprintf(temp_buf, "0 %d %d %d",i,transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d",i, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();
#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SDH mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   for (i=0;i<WTI_MAX_NUM_OF_MPLS_FA;++i)
   {
#if WTI_CESOP_MPLS_OVER_ENET
      /* create the l2 router MPLS flow aggregations */
      CLI_F_Tdm2PsnFlowAggEnetHeaderMpls("0 aa aa aa aa aa aa bb bb bb bb bb bb 8847");
      CLI_F_MplsFlowAggPrefixLength("0 18");
      sprintf(temp_buf,
              "0 %x %x %x %x %x %x",
              3*i,
              3*i+1,
              3*i+2,
              0x81000005,
              (0x88470000 | ((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) >> 4)),
              ((((((WTI_MPLS_LABEL) >> 12)  + WTI_MAX_PW + 1 + i) & 0xff) << 28) | 0x00ff0000));
      /*                              [mac destination] [mac source] [vlan] [type] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d %d", 2, 28, 41); /* remark offsets for VLAN priority and PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
#if WTI_CESOP_NATIVE_MPLS_IW
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 0");
      sprintf(temp_buf,
              "0 %x",
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 5); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 4");
      sprintf(temp_buf,
              "0 %x %x",
              WTI_MPLSoPPP,
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [ppp] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 13); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#endif /* WTI_CESOP_NATIVE_MPLS_IW */
#endif /* WTI_CESOP_MPLS_OVER_ENET */
   }

   for (i=WT_FIRST_LINE_INDEX ; i< (num_of_lines+WT_FIRST_LINE_INDEX); i++)
   {
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1,NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WUFE_LINE_LOOPBACK_TYPE_SYSTEM);

      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 48 48",i,i);
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d 48 48 %d 0-23",i,i,num_slots);
         CLI_F_PwConfigureFramed(temp_buf);
      }
      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 192");

      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", i+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");

      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 32");

      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(i%WTI_MAX_NUM_OF_MPLS_FA),i);
      CLI_F_PwCreateCascaded(temp_buf);
      sprintf(temp_buf, "0 %d 0",i);
      CLI_F_PwEnable(temp_buf);
      for (j=0;j<WTI_ITERATIONS_NUM;j++)
      {
         sprintf(temp_buf, "0 %d",i);
         CLI_F_PwDisable(temp_buf);

         WP_Delay(200000);

         sprintf(temp_buf, "0 %d 0",i);
         CLI_F_PwEnable(temp_buf);
      }
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/

void CLI_F_SonetSdhE1EnableDisable(char *StrPrm)
{

   WP_U32 res, cmd, i, j, num_of_lines, num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   framing_mode = WUFE_UNFRAMED;
   cas_mode = WUFE_CAS_DISABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d", &cmd, &interface);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   switch(interface)
   {
      case 0:
         transfer_type = WUFE_SDH_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         transfer_type = WUFE_SONET_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         transfer_type = WUFE_CAD_TYPE_E1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }

   num_of_lines = WT_MAX_LINE_INDEX;
   num_slots = WTI_E1_SLOTS;



   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX; i< (num_of_lines+WT_FIRST_LINE_INDEX); i++)
   {
      if ((i % 28) > 20)
      {
         continue;
      }

      sprintf(temp_buf, "0 %d %d %d",i,transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d",i, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SDH mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   for (i=0;i<WTI_MAX_NUM_OF_MPLS_FA;++i)
   {
#if WTI_CESOP_MPLS_OVER_ENET
      /* create the l2 router MPLS flow aggregations */
      CLI_F_Tdm2PsnFlowAggEnetHeaderMpls("0 aa aa aa aa aa aa bb bb bb bb bb bb 8847");
      CLI_F_MplsFlowAggPrefixLength("0 18");
      sprintf(temp_buf,
              "0 %x %x %x %x %x %x",
              3*i,
              3*i+1,
              3*i+2,
              0x81000005,
              (0x88470000 | ((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) >> 4)),
              ((((((WTI_MPLS_LABEL) >> 12)  + WTI_MAX_PW + 1 + i) & 0xff) << 28) | 0x00ff0000));
      /*                              [mac destination] [mac source] [vlan] [type] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d %d", 2, 28, 41); /* remark offsets for VLAN priority and PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
#if WTI_CESOP_NATIVE_MPLS_IW
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 0");
      sprintf(temp_buf,
              "0 %x",
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 5); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 4");
      sprintf(temp_buf,
              "0 %x %x",
              WTI_MPLSoPPP,
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [ppp] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 13); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#endif /* WTI_CESOP_NATIVE_MPLS_IW */
#endif /* WTI_CESOP_MPLS_OVER_ENET */
   }

   for (i=WT_FIRST_LINE_INDEX; i<(num_of_lines+WT_FIRST_LINE_INDEX); i++)
   {

      if ((i % 28) > 20)
      {
         continue;
      }
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1, NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WUFE_LINE_LOOPBACK_TYPE_SYSTEM);


      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64",i,pw_index);
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64 %d 0-31",i,pw_index,num_slots);
         CLI_F_PwConfigureFramed(temp_buf);
      }

      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);
      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 130");
      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", i+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");
      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 32");



      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
      pw_index++;

   }
   pw_index=0;
   for (j=0;j<WTI_ITERATIONS_NUM;j++)
   {
      printf("Disabling PW %d\n", pw_index);
      sprintf(temp_buf, "0 %d",pw_index);
      CLI_F_PwDisable(temp_buf);

      WP_Delay(200000);

      printf("Enabling PW %d\n",pw_index);
      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
   }
}


void CLI_F_error_4_e1_on_1_tug2(char *StrPrm)
{
   WP_U32 res, cmd, i, num_of_lines, num_slots;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   framing_mode = WUFE_UNFRAMED;
   cas_mode = WUFE_CAS_DISABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d", &cmd, &interface);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   switch(interface)
   {
      case 0:
         transfer_type = WUFE_SDH_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         transfer_type = WUFE_SONET_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         transfer_type = WUFE_CAD_TYPE_E1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }

   num_of_lines = 4;
   num_slots = WTI_E1_SLOTS;


   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX; i< (num_of_lines+WT_FIRST_LINE_INDEX); i++)
   {
      sprintf(temp_buf, "0 %d %d %d",i,transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d",i, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SDH mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   for (i=0;i<WTI_MAX_NUM_OF_MPLS_FA;++i)
   {
#if WTI_CESOP_MPLS_OVER_ENET
      /* create the l2 router MPLS flow aggregations */
      CLI_F_Tdm2PsnFlowAggEnetHeaderMpls("0 aa aa aa aa aa aa bb bb bb bb bb bb 8847");
      CLI_F_MplsFlowAggPrefixLength("0 18");
      sprintf(temp_buf,
              "0 %x %x %x %x %x %x",
              3*i,
              3*i+1,
              3*i+2,
              0x81000005,
              (0x88470000 | ((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) >> 4)),
              ((((((WTI_MPLS_LABEL) >> 12)  + WTI_MAX_PW + 1 + i) & 0xff) << 28) | 0x00ff0000));
      /*                              [mac destination] [mac source] [vlan] [type] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d %d", 2, 28, 41); /* remark offsets for VLAN priority and PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
#if WTI_CESOP_NATIVE_MPLS_IW
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 0");
      sprintf(temp_buf,
              "0 %x",
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 5); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 4");
      sprintf(temp_buf,
              "0 %x %x",
              WTI_MPLSoPPP,
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [ppp] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 13); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#endif /* WTI_CESOP_NATIVE_MPLS_IW */
#endif /* WTI_CESOP_MPLS_OVER_ENET */
   }

   for (i=0;i<4;i++)
   {
      /* UFE line configuration, creation and enabling */
      if (transfer_type == WUFE_SONET_TYPE_E1)
      {
         WT_ufe_line_sonet_params line_params;

         memset(&line_params, 0, sizeof(WT_ufe_line_sonet_params));

         line_params.sts12 = 0;
         line_params.sts3 = 0;
         line_params.sts1 = 0;
         line_params.vt_group =0;
         line_params.vt = i;
         line_params.transfer_type = transfer_type;
         line_params.framed = framing_mode;
         line_params.cas_enable = cas_mode;
         WT_UfeLineCreate(&the_system->ufe, i, &line_params, 1);
      }
      else if (transfer_type == WUFE_SDH_TYPE_E1)
      {
         WT_ufe_line_sdh_params line_params;

         memset(&line_params, 0, sizeof(WT_ufe_line_sdh_params));

         line_params.stm4 = 0;
         line_params.stm1 = 0;
         line_params.stm0 = 0;
         line_params.tug2 = 0;
         line_params.tu = i;
         line_params.transfer_type = transfer_type;
         line_params.framed = framing_mode;
         line_params.cas_enable = cas_mode;
         WT_UfeLineCreate(&the_system->ufe, i, &line_params, 1);
      }
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WUFE_LINE_LOOPBACK_TYPE_SYSTEM);
      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64",i,i);
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64 %d 0-31",i,i,num_slots);
         CLI_F_PwConfigureFramed(temp_buf);
      }

      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);
      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 130");
      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", i+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");
      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 32");



      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(i%WTI_MAX_NUM_OF_MPLS_FA),i);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",i);
      CLI_F_PwEnable(temp_buf);
   }
}


void CLI_F_error_ReEnable_line(char *StrPrm)
{

   WP_U32 res, cmd, i, num_of_lines, num_slots;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   framing_mode = WUFE_UNFRAMED;
   cas_mode = WUFE_CAS_DISABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d", &cmd, &interface);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   switch(interface)
   {
      case 0:
         transfer_type = WUFE_SDH_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         transfer_type = WUFE_SONET_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         transfer_type = WUFE_CAD_TYPE_E1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }

   num_of_lines = WT_MAX_LINE_INDEX;
   num_slots = WTI_E1_SLOTS;


   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX; i<(num_of_lines+WT_FIRST_LINE_INDEX); i++)
   {
      sprintf(temp_buf, "0 %d %d %d",i,transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d",i, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SDH mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   for (i=0;i<WTI_MAX_NUM_OF_MPLS_FA;++i)
   {
#if WTI_CESOP_MPLS_OVER_ENET
      /* create the l2 router MPLS flow aggregations */
      CLI_F_Tdm2PsnFlowAggEnetHeaderMpls("0 aa aa aa aa aa aa bb bb bb bb bb bb 8847");
      CLI_F_MplsFlowAggPrefixLength("0 18");
      sprintf(temp_buf,
              "0 %x %x %x %x %x %x",
              3*i,
              3*i+1,
              3*i+2,
              0x81000005,
              (0x88470000 | ((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) >> 4)),
              ((((((WTI_MPLS_LABEL) >> 12)  + WTI_MAX_PW + 1 + i) & 0xff) << 28) | 0x00ff0000));
      /*                              [mac destination] [mac source] [vlan] [type] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d %d", 2, 28, 41); /* remark offsets for VLAN priority and PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
#if WTI_CESOP_NATIVE_MPLS_IW
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 0");
      sprintf(temp_buf,
              "0 %x",
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 5); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 4");
      sprintf(temp_buf,
              "0 %x %x",
              WTI_MPLSoPPP,
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [ppp] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 13); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#endif /* WTI_CESOP_NATIVE_MPLS_IW */
#endif /* WTI_CESOP_MPLS_OVER_ENET */
   }

   for (i=WT_FIRST_LINE_INDEX; i<(num_of_lines+WT_FIRST_LINE_INDEX); i++)
   {
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1, NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WUFE_LINE_LOOPBACK_TYPE_SYSTEM);

      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64",i,i);
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64 %d 0-31",i,i,num_slots);
         CLI_F_PwConfigureFramed(temp_buf);
      }

      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);
      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 130");
      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", i+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");
      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 32");

      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(i%WTI_MAX_NUM_OF_MPLS_FA),i);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",i);
      CLI_F_PwEnable(temp_buf);
      sprintf(temp_buf, "0 %d 0",i);
      CLI_F_PwEnable(temp_buf);
   }
}
void CLI_F_error_disable_before_enable(char *StrPrm)
{

   WP_U32 res, cmd, i, num_of_lines, num_slots;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   framing_mode = WUFE_UNFRAMED;
   cas_mode = WUFE_CAS_DISABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d", &cmd, &interface);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   switch(interface)
   {
      case 0:
         transfer_type = WUFE_SDH_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         transfer_type = WUFE_SONET_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         transfer_type = WUFE_CAD_TYPE_E1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }

   num_of_lines = WT_MAX_LINE_INDEX;
   num_slots = WTI_E1_SLOTS;



   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX; i<(num_of_lines+WT_FIRST_LINE_INDEX); i++)
   {
      sprintf(temp_buf, "0 %d %d %d",i,transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d",i, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SDH mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   for (i=0;i<WTI_MAX_NUM_OF_MPLS_FA;++i)
   {
#if WTI_CESOP_MPLS_OVER_ENET
      /* create the l2 router MPLS flow aggregations */
      CLI_F_Tdm2PsnFlowAggEnetHeaderMpls("0 aa aa aa aa aa aa bb bb bb bb bb bb 8847");
      CLI_F_MplsFlowAggPrefixLength("0 18");
      sprintf(temp_buf,
              "0 %x %x %x %x %x %x",
              3*i,
              3*i+1,
              3*i+2,
              0x81000005,
              (0x88470000 | ((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) >> 4)),
              ((((((WTI_MPLS_LABEL) >> 12)  + WTI_MAX_PW + 1 + i) & 0xff) << 28) | 0x00ff0000));
      /*                              [mac destination] [mac source] [vlan] [type] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d %d", 2, 28, 41); /* remark offsets for VLAN priority and PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
#if WTI_CESOP_NATIVE_MPLS_IW
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 0");
      sprintf(temp_buf,
              "0 %x",
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 5); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 4");
      sprintf(temp_buf,
              "0 %x %x",
              WTI_MPLSoPPP,
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [ppp] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 13); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#endif /* WTI_CESOP_NATIVE_MPLS_IW */
#endif /* WTI_CESOP_MPLS_OVER_ENET */
   }

   for (i=WT_FIRST_LINE_INDEX; i<(num_of_lines+WT_FIRST_LINE_INDEX); i++)
   {
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1, NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WUFE_LINE_LOOPBACK_TYPE_SYSTEM);

      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64",i,i);
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64 %d 0-31",i,i,num_slots);
         CLI_F_PwConfigureFramed(temp_buf);
      }

      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);
      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 130");
      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", i+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");
      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 32");

      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(i%WTI_MAX_NUM_OF_MPLS_FA),i);
      CLI_F_PwCreateCascaded(temp_buf);
      sprintf(temp_buf, "0 %d",i);
      CLI_F_PwDisable(temp_buf);
      sprintf(temp_buf, "0 %d 0",i);
      CLI_F_PwEnable(temp_buf);
   }
}

void CLI_F_Error_Enable_Disable_Disable(char *StrPrm)
{

   WP_U32 res, cmd, i, num_of_lines, num_slots;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   framing_mode = WUFE_UNFRAMED;
   cas_mode = WUFE_CAS_DISABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d", &cmd, &interface);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   switch(interface)
   {
      case 0:
         transfer_type = WUFE_SDH_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         transfer_type = WUFE_SONET_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         transfer_type = WUFE_CAD_TYPE_E1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }

   num_of_lines = WT_MAX_LINE_INDEX;
   num_slots = WTI_E1_SLOTS;


   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX; i<(num_of_lines+WT_FIRST_LINE_INDEX); i++)
   {
      sprintf(temp_buf, "0 %d %d %d",i,transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d",i, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SDH mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   for (i=0;i<WTI_MAX_NUM_OF_MPLS_FA;++i)
   {
#if WTI_CESOP_MPLS_OVER_ENET
      /* create the l2 router MPLS flow aggregations */
      CLI_F_Tdm2PsnFlowAggEnetHeaderMpls("0 aa aa aa aa aa aa bb bb bb bb bb bb 8847");
      CLI_F_MplsFlowAggPrefixLength("0 18");
      sprintf(temp_buf,
              "0 %x %x %x %x %x %x",
              3*i,
              3*i+1,
              3*i+2,
              0x81000005,
              (0x88470000 | ((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) >> 4)),
              ((((((WTI_MPLS_LABEL) >> 12)  + WTI_MAX_PW + 1 + i) & 0xff) << 28) | 0x00ff0000));
      /*                              [mac destination] [mac source] [vlan] [type] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d %d", 2, 28, 41); /* remark offsets for VLAN priority and PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
#if WTI_CESOP_NATIVE_MPLS_IW
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 0");
      sprintf(temp_buf,
              "0 %x",
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 5); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 4");
      sprintf(temp_buf,
              "0 %x %x",
              WTI_MPLSoPPP,
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [ppp] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 13); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#endif /* WTI_CESOP_NATIVE_MPLS_IW */
#endif /* WTI_CESOP_MPLS_OVER_ENET */
   }

   for (i=WT_FIRST_LINE_INDEX; i<(num_of_lines+WT_FIRST_LINE_INDEX); i++)
   {
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1, NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WUFE_LINE_LOOPBACK_TYPE_SYSTEM);

      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64",i,i);
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64 %d 0-31",i,i,num_slots);
         CLI_F_PwConfigureFramed(temp_buf);
      }

      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);
      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 130");
      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", i+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");
      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 32");

      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(i%WTI_MAX_NUM_OF_MPLS_FA),i);
      CLI_F_PwCreateCascaded(temp_buf);
      sprintf(temp_buf, "0 %d 0",i);
      CLI_F_PwEnable(temp_buf);
      sprintf(temp_buf, "0 %d",i);
      CLI_F_PwDisable(temp_buf);
      sprintf(temp_buf, "0 %d",i);
      CLI_F_PwDisable(temp_buf);

   }
}

void CLI_F_Error_Enable_Before_Create(char *StrPrm)
{

   WP_U32 res, cmd, i, num_of_lines, num_slots;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   framing_mode = WUFE_UNFRAMED;
   cas_mode = WUFE_CAS_DISABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d", &cmd, &interface);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   switch(interface)
   {
      case 0:
         transfer_type = WUFE_SDH_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         transfer_type = WUFE_SONET_TYPE_E1;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         transfer_type = WUFE_CAD_TYPE_E1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }

   num_of_lines = WT_MAX_LINE_INDEX;
   num_slots = WTI_E1_SLOTS;


   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX; i<(num_of_lines+WT_FIRST_LINE_INDEX); i++)
   {
      sprintf(temp_buf, "0 %d %d %d",i,transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d",i, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SDH mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   for (i=0;i<WTI_MAX_NUM_OF_MPLS_FA;++i)
   {
#if WTI_CESOP_MPLS_OVER_ENET
      /* create the l2 router MPLS flow aggregations */
      CLI_F_Tdm2PsnFlowAggEnetHeaderMpls("0 aa aa aa aa aa aa bb bb bb bb bb bb 8847");
      CLI_F_MplsFlowAggPrefixLength("0 18");
      sprintf(temp_buf,
              "0 %x %x %x %x %x %x",
              3*i,
              3*i+1,
              3*i+2,
              0x81000005,
              (0x88470000 | ((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) >> 4)),
              ((((((WTI_MPLS_LABEL) >> 12)  + WTI_MAX_PW + 1 + i) & 0xff) << 28) | 0x00ff0000));
      /*                              [mac destination] [mac source] [vlan] [type] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d %d", 2, 28, 41); /* remark offsets for VLAN priority and PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
#if WTI_CESOP_NATIVE_MPLS_IW
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 0");
      sprintf(temp_buf,
              "0 %x",
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 5); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 4");
      sprintf(temp_buf,
              "0 %x %x",
              WTI_MPLSoPPP,
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [ppp] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 13); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#endif /* WTI_CESOP_NATIVE_MPLS_IW */
#endif /* WTI_CESOP_MPLS_OVER_ENET */
   }

   for (i=WT_FIRST_LINE_INDEX; i<(num_of_lines+WT_FIRST_LINE_INDEX); i++)
   {
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1, NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WUFE_LINE_LOOPBACK_TYPE_SYSTEM);

      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64",i,i);
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64 %d 0-31",i,i,num_slots);
         CLI_F_PwConfigureFramed(temp_buf);
      }

      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);
      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 130");
      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", i+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");
      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 32");

      sprintf(temp_buf, "0 %d 0",i);
      CLI_F_PwEnable(temp_buf);
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(i%WTI_MAX_NUM_OF_MPLS_FA),i);
      CLI_F_PwCreateCascaded(temp_buf);
      sprintf(temp_buf, "0 %d",i);
      CLI_F_PwDisable(temp_buf);

   }
}

/*********************************************************************
**********************************************************************
***                                                                ***
***       C L O C K   R E C O V E R Y   T E S T S                  ***
***                                                                ***
**********************************************************************
**********************************************************************/

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_CR_101_SonetSdh_E1UnframedNoCas(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WTI_Flexmux_global_setup flex_global_params;
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4 = WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U8 flexmux_mode=0;
#endif


   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 504;
#else
      num_of_lines = 252;
#endif
   }
   else if (res == 2)
   {
      interface = 0; /* use default setup */
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
   }
   else if (res > 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   printf("Running %s [%d][%d]\n",__FUNCTION__,num_of_lines,interface );

   strcpy(test_setup.test_name, __FUNCTION__);
   WTI_SetDefaultE1UnfTestParameters(num_of_lines, interface, &(test_setup.transfer_type),
		   &(test_setup.framing_mode), &(test_setup.cas_mode), &(test_setup.num_slots)

#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /*******************************************************************************************/
   /*                                     UFE and framer config                               */
   /*******************************************************************************************/

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

#ifdef WT_UFE_FRAMER

   if (interface < 2)
      WTI_enable_performance_monitoring_points(interface, WP_TRUE, vc_3_4, test_setup.framing_mode);

#endif
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_CR_101_SonetSdh_T1UnframedNoCas(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WTI_Flexmux_global_setup flex_global_params;
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U8 flexmux_mode=0;
#endif


   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 672;
#else
      num_of_lines = 336;
#endif
   }
   else if (res == 2)
   {
      interface = 0; /* use default setup */
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
   }
   else if (res > 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   printf("Running %s [%d][%d]\n",__FUNCTION__,num_of_lines,interface );

   strcpy(test_setup.test_name, __FUNCTION__);

   WTI_SetDefaultT1UnfTestParameters(num_of_lines, interface, &(test_setup.transfer_type),
									 &(test_setup.framing_mode), &(test_setup.cas_mode), &(test_setup.num_slots)
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /*******************************************************************************************/
   /*                                     UFE and framer config                               */
   /*******************************************************************************************/

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

#ifdef WT_UFE_FRAMER

   if (interface < 2)
      WTI_enable_performance_monitoring_points(interface, WP_FALSE, vc_3_4, test_setup.framing_mode);

#endif
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_CR_101_SonetSdh_E1FramedNoCas(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WTI_Flexmux_global_setup flex_global_params;
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 504;
#else
      num_of_lines = 252;
#endif
   }
   else if (res == 2)
   {
      interface = 0; /* use default setup */
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
   }
   else if (res > 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   printf("Running %s [%d][%d]\n",__FUNCTION__,num_of_lines,interface );

   strcpy(test_setup.test_name, __FUNCTION__);

   WTI_SetDefaultE1FrmTestParameters(num_of_lines, interface, &test_setup.transfer_type,
									 &test_setup.framing_mode, &test_setup.cas_mode, &test_setup.num_slots
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /*******************************************************************************************/
   /*                                     UFE and framer config                               */
   /*******************************************************************************************/

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

#ifdef WT_UFE_FRAMER

   if (interface < 2)
      WTI_enable_performance_monitoring_points(interface, WP_TRUE, vc_3_4, test_setup.framing_mode);

#endif
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_CR_101_SonetSdh_T1FramedNoCas(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WTI_Flexmux_global_setup flex_global_params;
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 672;
#else
      num_of_lines = 336;
#endif
   }
   else if (res == 2)
   {
      interface = 0; /* use default setup */
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
   }
   else if (res > 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   printf("Running %s [%d][%d]\n",__FUNCTION__,num_of_lines,interface );

   strcpy(test_setup.test_name, __FUNCTION__);

   WTI_SetDefaultT1FrmTestParameters(num_of_lines, interface, &test_setup.transfer_type,
									 &test_setup.framing_mode, &test_setup.cas_mode, &test_setup.num_slots
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /*******************************************************************************************/
   /*                                     UFE and framer config                               */
   /*******************************************************************************************/

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

#ifdef WT_UFE_FRAMER

   if (interface < 2)
      WTI_enable_performance_monitoring_points(interface, WP_FALSE, vc_3_4, test_setup.framing_mode);

#endif
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_CR_101_SonetSdh_E1Framed1xDS0NoCas(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines, temp_num_slots = -1, slot_id=-1;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WTI_Flexmux_global_setup flex_global_params;
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &num_of_lines, &slot_id, &temp_num_slots, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 504;
#else
      num_of_lines = 252;
#endif
   }
   else if (res == 2)
   {
      interface = 0; /* use default setup */
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
   }
   else if (res == 3)
   {
	   interface = 0; /* use default setup */
	   if (num_of_lines == 0)
	   {
		   WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
		   return;
	   }
	   if (slot_id < 0 || slot_id >= 32)
	   {
		   WTI_TerminatePrintError("Invalid parameter. Slot ID not in range", __LINE__);
		   return;
	   }
	}
	else if (res == 4)
	{
		interface = 0; /* use default setup */
		if (num_of_lines == 0)
		{
			WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
			return;
		}
		if (slot_id < 0 || slot_id >= 32)
		{
			WTI_TerminatePrintError("Invalid parameter. Slot ID not in range", __LINE__);
			return;
		}
		if((slot_id + temp_num_slots - 1) >= 32)
		{
			WTI_TerminatePrintError("Invalid parameter. Slot ID not in range", __LINE__);
			return;
		}
	}
	else if (res == 5)
	{
		if (num_of_lines == 0)
		{
			WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
			return;
		}
		if (slot_id < 0 || slot_id >= 32)
		{
			WTI_TerminatePrintError("Invalid parameter. Slot ID not in range", __LINE__);
			return;
		}
		if((slot_id + temp_num_slots - 1)>= 32)
		{
			WTI_TerminatePrintError("Invalid parameter. Slot ID not in range", __LINE__);
			return;
		}
	}
	else if (res > 5)
	{
		WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
		return;
	}

   strcpy(test_setup.test_name, __FUNCTION__);

   WTI_SetDefaultE1FrmTestParameters(num_of_lines, interface, &(test_setup.transfer_type),
								     &(test_setup.framing_mode), &(test_setup.cas_mode), &(test_setup.num_slots)
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );
   /* This is 1xDS0 test therefore, change the number of slots to 1 and select the slot ID. */
   if(temp_num_slots == -1)
   {
	   test_setup.num_slots = 1;
   }
   else
   {
	   test_setup.num_slots = temp_num_slots;
   }
   if(slot_id == -1)

   {
	   test_setup.first_slot_id = 30;
   }

   printf("Running %s [%d][%d] first slot ID:%d Slots: %d\n",__FUNCTION__,num_of_lines,interface,test_setup.first_slot_id,test_setup.num_slots);

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /*******************************************************************************************/
   /*                                     UFE and framer config                               */
   /*******************************************************************************************/

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_CR_101_SonetSdh_T1Framed1xDS0NoCas(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines, temp_num_slots = -1, slot_id=-1;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WTI_Flexmux_global_setup flex_global_params;
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &num_of_lines, &slot_id, &temp_num_slots, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 672;
#else
      num_of_lines = 336;
#endif
   }
   else if (res == 2)
   {
      interface = 0; /* use default setup */
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
   }
   else if (res == 3)
   {
	   interface = 0; /* use default setup */
	   if (num_of_lines == 0)
	   {
		   WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
		   return;
	   }
	   if (slot_id < 0 || slot_id >= 24)
	   {
		   WTI_TerminatePrintError("Invalid parameter. Slot ID not in range", __LINE__);
		   return;
	   }
   }
   else if (res == 4)
   {
	   interface = 0; /* use default setup */
	   if (num_of_lines == 0)
	   {
		   WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
		   return;
	   }
		if (slot_id < 0 || slot_id >= 24)
		{
			WTI_TerminatePrintError("Invalid parameter. Slot ID not in range", __LINE__);
			return;
		}
		if((slot_id + temp_num_slots - 1) >= 24)
		{
			WTI_TerminatePrintError("Invalid parameter. Slot ID not in range", __LINE__);
			return;
		}
   }
   else if (res == 5)
   {
	   if (num_of_lines == 0)
	   {
		   WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
		   return;
	   }
	   if (slot_id < 0 || slot_id >= 24)
	   {
		   WTI_TerminatePrintError("Invalid parameter. Slot ID not in range", __LINE__);
		   return;
	   }
	   if((slot_id + temp_num_slots - 1)>= 24)
	   {
		   WTI_TerminatePrintError("Invalid parameter. Slot ID not in range", __LINE__);
		   return;
	   }
	}
   else if (res > 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   strcpy(test_setup.test_name, __FUNCTION__);
   WTI_SetDefaultT1FrmTestParameters(num_of_lines, interface, &test_setup.transfer_type,
								     &test_setup.framing_mode, &test_setup.cas_mode, &test_setup.num_slots
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );
   /* This is 1xDS0 test therefore, change the number of slots to 1 and select the slot ID. */
   if(temp_num_slots == -1)
   {
	   test_setup.num_slots = 1;
   }
   else
   {
	   test_setup.num_slots = temp_num_slots;
   }

   if(slot_id == -1)
   {
	   test_setup.first_slot_id = 14;
   }

   printf("Running %s [%d][%d] first slot ID:%d Slots: %d\n",__FUNCTION__,num_of_lines,interface,slot_id,test_setup.num_slots);


   /* allocate CESoP system */
   WTI_SystemAlloc();

   /*******************************************************************************************/
   /*                                     UFE and framer config                               */
   /*******************************************************************************************/

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_CR_102_SonetSdh_E1FramedCas(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface, fragmented_mode;
#ifdef WT_UFE_FRAMER
   WTI_Flexmux_global_setup flex_global_params;
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &num_of_lines, &interface, &fragmented_mode);

   if (res == 1)
   {
      interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 504;
#else
      num_of_lines = 252;
#endif
      fragmented_mode = 0;
   }
   else if (res == 2)
   {
      interface = 0; /* use default setup */
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
      fragmented_mode = 0;
   }
   else if (res == 4)
   {
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
      if (fragmented_mode != 0 && fragmented_mode != 1)
      {
         WTI_TerminatePrintError("Invalid parameter. Incorrect fragmented type", __LINE__);
         return;
      }
   }
   else if (res > 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   printf("Running %s [%d][%d]\n",__FUNCTION__,num_of_lines,interface );

   strcpy(test_setup.test_name, __FUNCTION__);
   WTI_SetDefaultE1FrmTestParameters(num_of_lines, interface, &test_setup.transfer_type,
								     &test_setup.framing_mode, &test_setup.cas_mode, &test_setup.num_slots
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* Enable CAS mode */
   test_setup.cas_mode = WUFE_CAS_ENABLE;
   test_setup.num_slots = 30;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /*******************************************************************************************/
   /*                                     UFE and framer config                               */
   /*******************************************************************************************/

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_CR_102_SonetSdh_T1FramedCas(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface, fragmented_mode;
#ifdef WT_UFE_FRAMER
   WTI_Flexmux_global_setup flex_global_params;
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &num_of_lines, &interface, &fragmented_mode);

   if (res == 1)
   {
      interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 672;
#else
      num_of_lines = 336;
#endif
      fragmented_mode = 0;
   }
   else if (res == 2)
   {
      interface = 0; /* use default setup */
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
      fragmented_mode = 0;
   }
   else if (res == 4)
   {
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
      if (fragmented_mode != 0 && fragmented_mode != 1)
      {
         WTI_TerminatePrintError("Invalid parameter. Incorrect fragmented type", __LINE__);
         return;
      }
   }
   else if (res > 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   printf("Running %s [%d][%d]\n",__FUNCTION__,num_of_lines,interface );

   strcpy(test_setup.test_name, __FUNCTION__);
   WTI_SetDefaultT1FrmTestParameters(num_of_lines, interface, &test_setup.transfer_type,
								     &test_setup.framing_mode, &test_setup.cas_mode, &test_setup.num_slots
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* Enable CAS mode */
   test_setup.cas_mode = WUFE_CAS_ENABLE;
   test_setup.num_slots = 23;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /*******************************************************************************************/
   /*                                     UFE and framer config                               */
   /*******************************************************************************************/

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_CR_103_SonetSdh_E1FramedNoCas_MultiplePwPerLine(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface, recover_pw = 0, pw_per_line = 4;
#ifdef WT_UFE_FRAMER
   WTI_Flexmux_global_setup flex_global_params;
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &num_of_lines, &interface, &pw_per_line, &recover_pw);

   if (res == 1)
   {
      interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 504;
#else
      num_of_lines = 252;
#endif
      pw_per_line = 4;
      recover_pw = 0;
   }
   else if (res == 2)
   {
      interface = 0; /* use default setup */
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
      pw_per_line = 4;
      recover_pw = 0;
   }
   else if (res == 4)
   {
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
      if (WTI_E1_SLOTS % pw_per_line)
      {
         WTI_TerminatePrintError("Invalid number of PWs per line", __LINE__);
         return;
      }
      recover_pw = 0;
   }
   else if (res == 5)
   {
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
      if (WTI_E1_SLOTS % pw_per_line)
      {
         WTI_TerminatePrintError("Invalid number of PWs per line", __LINE__);
         return;
      }
      if (recover_pw >= pw_per_line)
      {
         printf("WARNING! Selected PW to recover from (%d) is bigger than number of PWs per line (%d). Using PW 0.\n", recover_pw, pw_per_line);
         recover_pw = 0;
      }
   }
   else if (res > 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   printf("Running %s [%d][%d], %d PWs per line, recovered PW: %d\n",__FUNCTION__,num_of_lines,interface, pw_per_line, recover_pw);

   strcpy(test_setup.test_name, __FUNCTION__);
   WTI_SetDefaultE1FrmTestParameters(num_of_lines, interface, &(test_setup.transfer_type),
									 &(test_setup.framing_mode), &(test_setup.cas_mode), &(test_setup.num_slots)
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* Setup for multi-pw */
   test_setup.pw_per_line = pw_per_line;
   test_setup.recovered_pw = recover_pw;

   /* Calculate number of slots */
   test_setup.num_slots = WTI_E1_SLOTS / pw_per_line;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /*******************************************************************************************/
   /*                                     UFE and framer config                               */
   /*******************************************************************************************/

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_CR_103_SonetSdh_T1FramedNoCas_MultiplePwPerLine(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface, recover_pw = 0, pw_per_line = 4;
#ifdef WT_UFE_FRAMER
   WTI_Flexmux_global_setup flex_global_params;
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &num_of_lines, &interface, &pw_per_line, &recover_pw);

   if (res == 1)
   {
      interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 672;
#else
      num_of_lines = 336;
#endif
      pw_per_line = 4;
      recover_pw = 0;
   }
   else if (res == 2)
   {
      interface = 0; /* use default setup */
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
      pw_per_line = 4;
      recover_pw = 0;
   }
   else if (res == 4)
   {
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
      if (WTI_T1_SLOTS % pw_per_line)
      {
         WTI_TerminatePrintError("Invalid number of PWs per line", __LINE__);
         return;
      }
      recover_pw = 0;
   }
   else if (res == 5)
   {
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
      if (WTI_T1_SLOTS % pw_per_line)
      {
         WTI_TerminatePrintError("Invalid number of PWs per line", __LINE__);
         return;
      }
      if (recover_pw >= pw_per_line)
      {
         printf("WORNING! Selected PW to recover from (%d) is bigger than number of PWs per line (%d). Using PW 0.\n", recover_pw, pw_per_line);
         recover_pw = 0;
      }
   }
   else if (res > 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   printf("Running %s [%d][%d], %d PWs per line, recovered PW: %d\n",__FUNCTION__,num_of_lines,interface, pw_per_line, recover_pw);

   strcpy(test_setup.test_name, __FUNCTION__);
   WTI_SetDefaultT1FrmTestParameters(num_of_lines, interface, &test_setup.transfer_type,
									 &test_setup.framing_mode, &test_setup.cas_mode, &test_setup.num_slots
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* Setup for multi-pw */
   test_setup.pw_per_line = pw_per_line;
   test_setup.recovered_pw = recover_pw;

   /* Calculate number of slots */
   test_setup.num_slots = WTI_T1_SLOTS / pw_per_line;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /*******************************************************************************************/
   /*                                     UFE and framer config                               */
   /*******************************************************************************************/

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
WP_U32 CLI_F_CR_104_SonetSdh_E1UnframedNoCas_AddRemovePwLine(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface, line_delete = WP_TRUE, iterations, result;
#ifdef WT_UFE_FRAMER
   WTI_Flexmux_global_setup flex_global_params;
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &num_of_lines, &interface, &line_delete);

   if (res == 1)
   {
      interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 504;
#else
      num_of_lines = 252;
#endif
      line_delete = WP_TRUE;
   }
   else if (res == 2)
   {
      interface = 0; /* use default setup */
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return WT_FAIL;
      }
      line_delete = WP_TRUE;
   }
   else if (res == 4)
   {
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return WT_FAIL;
      }
      if (line_delete != WP_TRUE && line_delete != WP_FALSE)
      {
         WTI_TerminatePrintError("Invalid parameter. Line delete indication should be WP_TRUE or WP_FALSE", __LINE__);
         return WT_FAIL;
      }
   }
   else if (res > 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return WT_FAIL;
   }
   printf("Running %s [%d][%d] remove line command: %d\n",__FUNCTION__,num_of_lines,interface, line_delete);

   strcpy(test_setup.test_name, __FUNCTION__);
   WTI_SetDefaultE1UnfTestParameters(num_of_lines, interface, &test_setup.transfer_type,
		   &test_setup.framing_mode, &test_setup.cas_mode, &test_setup.num_slots
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /*******************************************************************************************/
   /*                                     UFE and framer config                               */
   /*******************************************************************************************/

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   /* Setting up initial system */
   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   printf("Test is running. Wait %d seconds to let the system become stable...\n", DELAY_DURATION_IN_SECONDS * 4);
   WP_Delay(DELAY_DURATION_IN_SECONDS * 4 * 1000000);
#endif

   /* Global statistics check, including reset. */
   result = CLI_F_GlobalStatisticsCheck("0");
   if (result == WT_FAIL)
   {
      /* retry */
      WP_Delay(DELAY_DURATION_IN_SECONDS * 1000000);
      result = CLI_F_GlobalStatisticsCheck("0");
      if (result == WT_FAIL)
      {
         WTI_PRINT_ERROR(("\n Test Failed before remove PW/lines.\n"));
         return WT_FAIL;
      }
   }

   /* starting the remove-recreate sequence. */
   for (iterations=1; iterations<WTI_ITERATIONS_NUM+1; iterations++)
   {
      printf("################ start of iteration %d   ###############\n", iterations);

      /* Remove all PWs: last to first remove */
      sprintf(temp_buf, "0 %d", line_delete);
      CLI_F_UfeLinePWDelete(temp_buf);

      /* Wait few milisecond before recreate */
      WP_Delay(60000);

      /* recreate lines (if needed) and PWs */
      WT_UfeLinePWCreate(&test_setup, line_delete);

      printf("Wait %d seconds to let the system become stable...\n", DELAY_DURATION_IN_SECONDS);
      WP_Delay(DELAY_DURATION_IN_SECONDS * 1000000);
      /* Global statistics check, including reset. */
      result = CLI_F_GlobalStatisticsCheck("0");
      if (result == WT_FAIL)
      {
         WTI_PRINT_ERROR(("\n Test Failed. Iteration number: %d\n", iterations));
         return WT_FAIL;
      }
      printf(">>>>>>>>>>>>>>>> end of iteration %d   <<<<<<<<<<<<<<<\n", iterations);
   } /* for iterations */
   printf("Final verdict - Test Passed\n");

   return WT_PASS;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
WP_U32 CLI_F_CR_104_SonetSdh_T1UnframedNoCas_AddRemovePwLine(char *StrPrm)
{
	WP_U32 res, cmd, num_of_lines;
	WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
	WP_U32 interface, line_delete = WP_TRUE, iterations, result;
#ifdef WT_UFE_FRAMER
	WTI_Flexmux_global_setup flex_global_params;
	WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
	WP_U8 flexmux_mode=0;
#endif

	/* Read parameters from user */
	/*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
	res = sscanf(StrPrm, "%d %d %d %d", &cmd, &num_of_lines, &interface, &line_delete);

	if (res == 1)
	{
		interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 672;
#else
      num_of_lines = 336;
#endif
		line_delete = WP_TRUE;
	}
	else if (res == 2)
	{
		interface = 0; /* use default setup */
		if (num_of_lines == 0)
		{
			WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
			return WT_FAIL;
		}
		line_delete = WP_TRUE;
	}
	else if (res == 4)
	{
		if (num_of_lines == 0)
		{
			WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
			return WT_FAIL;
		}
		if (line_delete != WP_TRUE && line_delete != WP_FALSE)
		{
			WTI_TerminatePrintError("Invalid parameter. Line delete indication should be WP_TRUE or WP_FALSE", __LINE__);
			return WT_FAIL;
		}
	}
	else if (res > 4)
	{
		WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
		return WT_FAIL;
	}
	printf("Running %s [%d][%d] remove line command: %d\n",__FUNCTION__,num_of_lines,interface, line_delete);

	strcpy(test_setup.test_name, __FUNCTION__);
	WTI_SetDefaultT1UnfTestParameters(num_of_lines, interface, &test_setup.transfer_type,
			&test_setup.framing_mode, &test_setup.cas_mode, &test_setup.num_slots
#ifdef WT_UFE_FRAMER
			, &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
	);

	/* allocate CESoP system */
	WTI_SystemAlloc();

	/*******************************************************************************************/
	/*                                     UFE and framer config                               */
	/*******************************************************************************************/

	/* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
	sprintf(temp_buf, "0 %d",interface);
	CLI_F_UfeFpgaMode(temp_buf);

	/* create an empty CESOP system with no PW */
	/*Calls UFE system create and enable*/
	WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
	/* Initialize the Flexmux framer in SONET mode */
	WTI_FlexmuxInit(WTI_EMPHY_PORT,
			0,
			WTI_FLEXMUX_PHYSICAL_CONF,
			flexmux_mode,
			vc_3_4,
			&flex_global_params);

	if (interface < 2)
		WTI_enable_alarms_and_performance_monitoring(interface);

	/* Unmask FPGA interrupt sources to allow serial to interrupt us */
	WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

	WT_UfeMPLS_L2_FA_Create();
	WT_UfeLinePWCreate(&test_setup, 1);

	#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
           printf("Test is running. Wait %d seconds to let the system become stable...\n", DELAY_DURATION_IN_SECONDS * 2);
	   WP_Delay(DELAY_DURATION_IN_SECONDS * 2 * 1000000);
	#endif

	   /* Global statistics check, including reset. */
	   result = CLI_F_GlobalStatisticsCheck("0");
	   if (result == WT_FAIL)
	   {
	      /* retry */
	      WP_Delay(DELAY_DURATION_IN_SECONDS * 1000000);
	      result = CLI_F_GlobalStatisticsCheck("0");
	      if (result == WT_FAIL)
	      {
	         WTI_PRINT_ERROR(("\n Test Failed before remove PW/lines.\n"));
	         return WT_FAIL;
	      }
	   }

	/* starting the remove-recreate sequence. */
	for (iterations=1; iterations<WTI_ITERATIONS_NUM+1; iterations++)
	{
		printf("################ start of iteration %d   ###############\n", iterations);

		sprintf(temp_buf, "0 %d", line_delete);
		CLI_F_UfeLinePWDelete(temp_buf);

		/* Wait few milisecond before recreate */
		WP_Delay(60000);

		WT_UfeLinePWCreate(&test_setup, line_delete);

		printf("Wait %d seconds to let the system become stable...\n", DELAY_DURATION_IN_SECONDS);
		WP_Delay(DELAY_DURATION_IN_SECONDS * 1000000);
		/* Global statistics check, including reset. */
		result = CLI_F_GlobalStatisticsCheck("0");
		if (result == WT_FAIL)
		{
			WTI_PRINT_ERROR(("\n Test Failed. Iteration number: %d\n", iterations));
			return WT_FAIL;
		}
		printf(">>>>>>>>>>>>>>>> end of iteration %d   <<<<<<<<<<<<<<<\n", iterations);
	} /* for iterations */
        printf("Final verdict - Test Passed\n");

	return WT_PASS;
}

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE /* end after test case 106 (end of file) */
/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
WP_U32 CLI_F_CR_104_SonetSdh_E1FramedNoCas_AddRemovePwLine(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface, line_delete = WP_TRUE, iterations, result;
#ifdef WT_UFE_FRAMER
   WTI_Flexmux_global_setup flex_global_params;
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &num_of_lines, &interface, &line_delete);

   if (res == 1)
   {
      interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 504;
#else
      num_of_lines = 252;
#endif
      line_delete = WP_TRUE;
   }
   else if (res == 2)
   {
      interface = 0; /* use default setup */
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return WT_FAIL;
      }
      line_delete = WP_TRUE;
   }
   else if (res == 4)
   {
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return WT_FAIL;
      }
      if (line_delete != WP_TRUE && line_delete != WP_FALSE)
      {
         WTI_TerminatePrintError("Invalid parameter. Line delete indication should be WP_TRUE or WP_FALSE", __LINE__);
         return WT_FAIL;
      }
   }
   else if (res > 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return WT_FAIL;
   }
   printf("Running %s [%d][%d] remove line command: %d\n",__FUNCTION__,num_of_lines,interface, line_delete);

   strcpy(test_setup.test_name, __FUNCTION__);
   WTI_SetDefaultE1FrmTestParameters(num_of_lines, interface, &test_setup.transfer_type,
									 &test_setup.framing_mode, &test_setup.cas_mode, &test_setup.num_slots
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /*******************************************************************************************/
   /*                                     UFE and framer config                               */
   /*******************************************************************************************/

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
           printf("Test is running. Wait %d seconds to let the system become stable...\n", DELAY_DURATION_IN_SECONDS * 2);
	   WP_Delay(DELAY_DURATION_IN_SECONDS * 2 * 1000000);
#endif

   /* Global statistics check, including reset. */
   result = CLI_F_GlobalStatisticsCheck("0");
   if (result == WT_FAIL)
   {
      /* retry */
      WP_Delay(DELAY_DURATION_IN_SECONDS * 1000000);
      result = CLI_F_GlobalStatisticsCheck("0");
      if (result == WT_FAIL)
      {
         WTI_PRINT_ERROR(("\n Test Failed before remove PW/lines.\n"));
         return WT_FAIL;
      }
   }

   /* starting the remove-recreate sequence. */
   for (iterations=1; iterations<WTI_ITERATIONS_NUM+1; iterations++)
   {
      printf("################ start of iteration %d   ###############\n", iterations);
      /* Remove all PWs: last to first remove */
      sprintf(temp_buf, "0 %d", line_delete);
      CLI_F_UfeLinePWDelete(temp_buf);

      /* Wait few milisecond before recreate */
      WP_Delay(60000);

      /* recreate lines (if needed) and PWs */
      WT_UfeLinePWCreate(&test_setup, line_delete);

      printf("Wait %d seconds to let the system become stable...\n", DELAY_DURATION_IN_SECONDS);
      WP_Delay(DELAY_DURATION_IN_SECONDS * 1000000);
      /* Global statistics check, including reset. */
      result = CLI_F_GlobalStatisticsCheck("0");
      if (result == WT_FAIL)
      {
         WTI_PRINT_ERROR(("\n Test Failed. Iteration number: %d\n", iterations));
         return WT_FAIL;
      }
      printf(">>>>>>>>>>>>>>>> end of iteration %d   <<<<<<<<<<<<<<<\n", iterations);
   } /* for iterations */
   printf("Final verdict - Test Passed\n");

   return WT_PASS;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
WP_U32 CLI_F_CR_104_SonetSdh_T1FramedNoCas_AddRemovePwLine(char *StrPrm)
{
	WP_U32 res, cmd, num_of_lines;
	WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
	WP_U32 interface, line_delete = WP_TRUE, iterations, result;
#ifdef WT_UFE_FRAMER
	WTI_Flexmux_global_setup flex_global_params;
	WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
	WP_U8 flexmux_mode=0;
#endif

	/* Read parameters from user */
	/*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
	res = sscanf(StrPrm, "%d %d %d %d", &cmd, &num_of_lines, &interface, &line_delete);

	if (res == 1)
	{
		interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 672;
#else
      num_of_lines = 336;
#endif
		line_delete = WP_TRUE;
	}
	else if (res == 2)
	{
		interface = 0; /* use default setup */
		if (num_of_lines == 0)
		{
			WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
			return WT_FAIL;
		}
		line_delete = WP_TRUE;
	}
	else if (res == 4)
	{
		if (num_of_lines == 0)
		{
			WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
			return WT_FAIL;
		}
		if (line_delete != WP_TRUE && line_delete != WP_FALSE)
		{
			WTI_TerminatePrintError("Invalid parameter. Line delete indication should be WP_TRUE or WP_FALSE", __LINE__);
			return WT_FAIL;
		}
	}
	else if (res > 4)
	{
		WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
		return WT_FAIL;
	}
	printf("Running %s [%d][%d] remove line command: %d\n",__FUNCTION__,num_of_lines,interface, line_delete);

	strcpy(test_setup.test_name, __FUNCTION__);
	WTI_SetDefaultT1FrmTestParameters(num_of_lines, interface, &test_setup.transfer_type,
			&test_setup.framing_mode, &test_setup.cas_mode, &test_setup.num_slots
#ifdef WT_UFE_FRAMER
			, &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
	);

	/* allocate CESoP system */
	WTI_SystemAlloc();

	/*******************************************************************************************/
	/*                                     UFE and framer config                               */
	/*******************************************************************************************/

	/* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
	sprintf(temp_buf, "0 %d",interface);
	CLI_F_UfeFpgaMode(temp_buf);

	/* create an empty CESOP system with no PW */
	/*Calls UFE system create and enable*/
	WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
	/* Initialize the Flexmux framer in SONET mode */
	WTI_FlexmuxInit(WTI_EMPHY_PORT,
			0,
			WTI_FLEXMUX_PHYSICAL_CONF,
			flexmux_mode,
			vc_3_4,
			&flex_global_params);

	if (interface < 2)
		WTI_enable_alarms_and_performance_monitoring(interface);

	/* Unmask FPGA interrupt sources to allow serial to interrupt us */
	WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

	/* Setting up initial system */
	WT_UfeMPLS_L2_FA_Create();
	WT_UfeLinePWCreate(&test_setup, 1);

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
        printf("Test is running. Wait %d seconds to let the system become stable...\n", DELAY_DURATION_IN_SECONDS * 2);
        WP_Delay(DELAY_DURATION_IN_SECONDS * 2 * 1000000);
#endif

	/* Global statistics check, including reset. */
	result = CLI_F_GlobalStatisticsCheck("0");
	if (result == WT_FAIL)
	{
		/* retry */
		WP_Delay(DELAY_DURATION_IN_SECONDS * 1000000);
		result = CLI_F_GlobalStatisticsCheck("0");
		if (result == WT_FAIL)
		{
			WTI_PRINT_ERROR(("\n Test Failed before remove PW/lines.\n"));
			return WT_FAIL;
		}
	}

	/* starting the remove-recreate sequence. */
	for (iterations=1; iterations<WTI_ITERATIONS_NUM+1; iterations++)
	{
		printf("################ start of iteration %d   ###############\n", iterations);

		/* Remove all PWs: last to first remove */
		sprintf(temp_buf, "0 %d", line_delete);
		CLI_F_UfeLinePWDelete(temp_buf);

		/* Wait few milisecond before recreate */
		WP_Delay(60000);

		/* recreate lines (if needed) and PWs */
		WT_UfeLinePWCreate(&test_setup, line_delete);

                printf("Wait %d seconds to let the system become stable...\n", DELAY_DURATION_IN_SECONDS);
		WP_Delay(DELAY_DURATION_IN_SECONDS * 1000000);
		/* Global statistics check, including reset. */
		result = CLI_F_GlobalStatisticsCheck("0");
		if (result == WT_FAIL)
		{
			WTI_PRINT_ERROR(("\n Test Failed. Iteration number: %d\n", iterations));
			return WT_FAIL;
		}
		printf(">>>>>>>>>>>>>>>> end of iteration %d   <<<<<<<<<<<<<<<\n", iterations);
	} /* for iterations */
        printf("Final verdict - Test Passed\n");

	return WT_PASS;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_CR_104_SonetSdh_E1FramedCas_AddRemovePwLine(char *StrPrm)
{
	WP_U32 res, cmd, num_of_lines;
	WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
	WP_U32 interface, fragmented_mode, line_delete = WP_TRUE, iterations, result;
#ifdef WT_UFE_FRAMER
	WTI_Flexmux_global_setup flex_global_params;
	WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
	WP_U8 flexmux_mode=0;
#endif

	/* Read parameters from user */
	/*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
	res = sscanf(StrPrm, "%d %d %d %d", &cmd, &num_of_lines, &interface, &fragmented_mode);

	if (res == 1)
	{
		interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 504;
#else
      num_of_lines = 252;
#endif
		fragmented_mode = 0;
	}
	else if (res == 2)
	{
		interface = 0; /* use default setup */
		if (num_of_lines == 0)
		{
			WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
			return;
		}
		fragmented_mode = 0;
	}
	else if (res == 4)
	{
		if (num_of_lines == 0)
		{
			WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
			return;
		}
		if (fragmented_mode != 0 && fragmented_mode != 1)
		{
			WTI_TerminatePrintError("Invalid parameter. Incorrect fragmented type", __LINE__);
			return;
		}
	}
	else if (res > 4)
	{
		WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
		return;
	}
	printf("Running %s [%d][%d]\n",__FUNCTION__,num_of_lines,interface );

	strcpy(test_setup.test_name, __FUNCTION__);
	WTI_SetDefaultE1FrmTestParameters(num_of_lines, interface, &test_setup.transfer_type,
			&test_setup.framing_mode, &test_setup.cas_mode, &test_setup.num_slots
#ifdef WT_UFE_FRAMER
			, &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
	);

	/* Enable CAS mode */
	test_setup.cas_mode = WUFE_CAS_ENABLE;
	test_setup.num_slots = 30;

	/* allocate CESoP system */
	WTI_SystemAlloc();

	/*******************************************************************************************/
	/*                                     UFE and framer config                               */
	/*******************************************************************************************/

	/* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
	sprintf(temp_buf, "0 %d",interface);
	CLI_F_UfeFpgaMode(temp_buf);

	/* create an empty CESOP system with no PW */
	/*Calls UFE system create and enable*/
	WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
	/* Initialize the Flexmux framer in SONET mode */
	WTI_FlexmuxInit(WTI_EMPHY_PORT,
			0,
			WTI_FLEXMUX_PHYSICAL_CONF,
			flexmux_mode,
			vc_3_4,
			&flex_global_params);

	if (interface < 2)
		WTI_enable_alarms_and_performance_monitoring(interface);

	/* Unmask FPGA interrupt sources to allow serial to interrupt us */
	WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

	/* Setting up initial system */
	WT_UfeMPLS_L2_FA_Create();
	WT_UfeLinePWCreate(&test_setup, 1);


	printf("Test is running. Wait few seconds to let the system become stable...\n");

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
	WP_Delay(DELAY_DURATION_IN_SECONDS * 1000000);
	WP_Delay(DELAY_DURATION_IN_SECONDS * 1000000);
#endif

	/* Global statistics check, including reset. */
	result = CLI_F_GlobalStatisticsCheck("0");
	if (result == WT_FAIL)
	{
		/* retry */
		WP_Delay(DELAY_DURATION_IN_SECONDS * 1000000);
		result = CLI_F_GlobalStatisticsCheck("0");
		if (result == WT_FAIL)
		{
			WTI_PRINT_ERROR(("\n Test Failed before remove PW/lines.\n"));
			return;
		}
	}

	/* starting the remove-recreate sequence. */
	for (iterations=1; iterations<WTI_ITERATIONS_NUM+1; iterations++)
	{
		printf("################ start of iteration %d   ###############\n", iterations);

		/* Remove all PWs: last to first remove */
		sprintf(temp_buf, "0 %d", line_delete);
		CLI_F_UfeLinePWDelete(temp_buf);

		/* Wait few milisecond before recreate */
		WP_Delay(60000);

		/* recreate lines (if needed) and PWs */
		WT_UfeLinePWCreate(&test_setup, line_delete);

		printf("Wait few seconds to let the system become stable...\n");
		WP_Delay(DELAY_DURATION_IN_SECONDS * 1000000);
		/* Global statistics check, including reset. */
		result = CLI_F_GlobalStatisticsCheck("0");
		if (result == WT_FAIL)
		{
			WTI_PRINT_ERROR(("\n Test Failed. Iteration number: %d\n", iterations));
			return;
		}
		printf(">>>>>>>>>>>>>>>> end of iteration %d   <<<<<<<<<<<<<<<\n", iterations);
	} /* for iterations */

	return;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_CR_104_SonetSdh_T1FramedCas_AddRemovePwLine(char *StrPrm)
{
	WP_U32 res, cmd, num_of_lines;
	WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
	WP_U32 interface, fragmented_mode = 0, line_delete = WP_TRUE, iterations, result;
#ifdef WT_UFE_FRAMER
	WTI_Flexmux_global_setup flex_global_params;
	WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
	WP_U8 flexmux_mode=0;
#endif

	/* Read parameters from user */
	/*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
	res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

	if (res == 1)
	{
		interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 672;
#else
      num_of_lines = 336;
#endif
		fragmented_mode = 0;
	}
	else if (res == 2)
	{
		interface = 0; /* use default setup */
		if (num_of_lines == 0)
		{
			WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
			return;
		}
		fragmented_mode = 0;
	}
	else if (res == 4)
	{
		if (num_of_lines == 0)
		{
			WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
			return;
		}
		if (fragmented_mode != 0 && fragmented_mode != 1)
		{
			WTI_TerminatePrintError("Invalid parameter. Incorrect fragmented type", __LINE__);
			return;
		}
	}
	else if (res > 4)
	{
		WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
		return;
	}
	printf("Running %s [%d][%d]\n",__FUNCTION__,num_of_lines,interface );

	strcpy(test_setup.test_name, __FUNCTION__);
	WTI_SetDefaultT1FrmTestParameters(num_of_lines, interface, &test_setup.transfer_type,
			&test_setup.framing_mode, &test_setup.cas_mode, &test_setup.num_slots
#ifdef WT_UFE_FRAMER
			, &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
	);

	/* Enable CAS mode */
	test_setup.cas_mode = WUFE_CAS_ENABLE;
	test_setup.num_slots = 23;

	/* allocate CESoP system */
	WTI_SystemAlloc();

	/*******************************************************************************************/
	/*                                     UFE and framer config                               */
	/*******************************************************************************************/

	/* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
	sprintf(temp_buf, "0 %d",interface);
	CLI_F_UfeFpgaMode(temp_buf);

	/* create an empty CESOP system with no PW */
	/*Calls UFE system create and enable*/
	WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
	/* Initialize the Flexmux framer in SONET mode */
	WTI_FlexmuxInit(WTI_EMPHY_PORT,
			0,
			WTI_FLEXMUX_PHYSICAL_CONF,
			flexmux_mode,
			vc_3_4,
			&flex_global_params);

	if (interface < 2)
		WTI_enable_alarms_and_performance_monitoring(interface);

	/* Unmask FPGA interrupt sources to allow serial to interrupt us */
	WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

	/* Setting up initial system */
	WT_UfeMPLS_L2_FA_Create();
	WT_UfeLinePWCreate(&test_setup, 1);


	printf("Test is running. Wait few seconds to let the system become stable...\n");

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
	WP_Delay(DELAY_DURATION_IN_SECONDS * 1000000);
	WP_Delay(DELAY_DURATION_IN_SECONDS * 1000000);
#endif

	/* Global statistics check, including reset. */
	result = CLI_F_GlobalStatisticsCheck("0");
	if (result == WT_FAIL)
	{
		/* retry */
		WP_Delay(DELAY_DURATION_IN_SECONDS * 1000000);
		result = CLI_F_GlobalStatisticsCheck("0");
		if (result == WT_FAIL)
		{
			WTI_PRINT_ERROR(("\n Test Failed before remove PW/lines.\n"));
			return;
		}
	}

	/* starting the remove-recreate sequence. */
	for (iterations=1; iterations<WTI_ITERATIONS_NUM+1; iterations++)
	{
		printf("################ start of iteration %d   ###############\n", iterations);

		/* Remove all PWs: last to first remove */
		sprintf(temp_buf, "0 %d", line_delete);
		CLI_F_UfeLinePWDelete(temp_buf);

		/* Wait few milisecond before recreate */
		WP_Delay(60000);

		/* recreate lines (if needed) and PWs */
		WT_UfeLinePWCreate(&test_setup, line_delete);

		printf("Wait few seconds to let the system become stable...\n");
		WP_Delay(DELAY_DURATION_IN_SECONDS * 1000000);
		/* Global statistics check, including reset. */
		result = CLI_F_GlobalStatisticsCheck("0");
		if (result == WT_FAIL)
		{
			WTI_PRINT_ERROR(("\n Test Failed. Iteration number: %d\n", iterations));
			return ;
		}
		printf(">>>>>>>>>>>>>>>> end of iteration %d   <<<<<<<<<<<<<<<\n", iterations);
	} /* for iterations */

	return;
}


/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_CR_105_SonetSdh_E1UnframedNoCas_DifferentDuAndPs(char *StrPrm)
{
   WP_U32 res, cmd, line_index, num_of_lines, num_slots = 0, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_UNFRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_E1;
   WUFE_cas_enable cas_mode = WUFE_CAS_DISABLE;
   WP_U32 interface, du_size = 64, ps = 256;
#ifdef WT_UFE_FRAMER
   WTI_Flexmux_global_setup flex_global_params;
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U8 flexmux_mode=0;
#endif
   WUFE_line_clock_rec_params line_cr_params;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 336;
#else
      num_of_lines = 168;
#endif
   }
   else if (res == 2)
   {
      interface = 0; /* use default setup */
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
   }
   else if (res > 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   printf("Running %s [%d][%d]\n",__FUNCTION__,num_of_lines,interface );
   if (num_of_lines != 252) /* This warning should be removed when full bandwidth is supported. */
      printf("!!!!!!!\n!! This test does not run with full bandwidth. Number of lines: %d !!\n!!!!!!!\n", num_of_lines);

   WTI_SetDefaultE1UnfTestParameters(num_of_lines, interface, &transfer_type,
                                     &framing_mode, &cas_mode, &num_slots
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /*******************************************************************************************/
   /*                                     UFE and framer config                               */
   /*******************************************************************************************/

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
      {
         num_of_lines++;
         continue;
      }
      sprintf(temp_buf, "0 %d %d %d",line_index,transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d",line_index, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/
   WT_UfeMPLS_L2_FA_Create();

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {

#if WTI_DUAL_EMPHY
      if(line_index > 216 && line_index < 336)
      {
         num_of_lines++;
         continue;
      }
#endif

      if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
      {
         continue;
      }

      if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
      {
         /* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
         the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
         if(line_index == WT_LINE_INDEX_TO_FIRST_DCO)
         {
            the_system->clock_rec[line_index].physical_clock_id = WUFE_CLOCK_REC_DCO_ID_0;
            WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, WUFE_CLOCK_REC_DCO_ID_0); /* tx_enable, dco_active, dco_number */
         }
         else
         {
            the_system->clock_rec[line_index].physical_clock_id = WUFE_CLOCK_REC_DCO_ID_1;
            WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, WUFE_CLOCK_REC_DCO_ID_1); /* tx_enable, dco_active, dco_number */
         }
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
#if WTI_DUAL_EMPHY
      if(line_index >= WTI_FIRST_UFE_NUM_LINES)
      {
         WT_UfeLineConfigureAndCreateE1(&the_system->second_ufe, line_index-WTI_FIRST_UFE_NUM_LINES, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);
      }
      else
      {
         WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);
      }
#else
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);
#endif

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);

#if WTI_DUAL_EMPHY
         if (line_index >= WTI_FIRST_UFE_NUM_LINES)
         {
            WT_UfeLineSocketLpbkSetup(&the_system->second_ufe, (line_index-WTI_FIRST_UFE_NUM_LINES), WTI_SOCKET_LOOPBACK_MODE);
         }
         else
         {
            WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
         }
#else
         WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
#endif
   }




   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
      {
         continue;
      }
#if WTI_DUAL_EMPHY
      if(line_index > 216 && line_index < 336)
      {
         continue;
      }
#endif

      the_system->pw[pw_index].line_index = line_index;

      /* Different DU and PS sizes */
      /* Select between 2 tests:
         1. Full bandwidth: change only few PWs (due to have load on the EMPHY and ENET.
         2. Partial bandwidth: run with 42 PWs, different DU and PS to each PW.*/
#if 1
      switch (pw_index % 21)
      {
         case 0:
         {
            du_size = 16;
            ps = 48;
            break;
         }
         case 1:
         {
            du_size =64;
            ps = 64;
            break;
         }
         case 2:
         {
            du_size = 32;
            ps = 32;
            break;
         }
         case 3:
         {
            du_size = 32;
            ps = 192;
            break;
         }
          case 4:
         {
            du_size = 4;
            ps = 64;
            break;
         }
         case 5:
         {
            du_size = 8;
            ps = 128;
            break;
         }
         case 6:
         {
            du_size = 64;
            ps = 192;
            break;
         }
         case 7:
         {
            du_size = 61;
            ps = 183;
            break;
         }

         default:
         {
            du_size = 64;
            ps = 256;
            break;
         }
      }
#else
      du_size = 64 / (1<<(pw_index % 3));
      ps = 64 * ((pw_index % 4)+1);
#endif /* 1 */

#if WTI_DUAL_EMPHY
      if(line_index >= WTI_FIRST_UFE_NUM_LINES)
      {
         pw_config.pw_emphy_num = WTI_EMPHY_SECOND;
      }
      else
      {
         pw_config.pw_emphy_num = WTI_EMPHY_FIRST;
      }
#endif

      printf("--> pw_index = %4d    du_size = %3d   ps = %3d\n"
             , pw_index, du_size, ps);
      /*UFE phy and pw configuration*/
#if WTI_DUAL_EMPHY
      if(line_index >= WTI_FIRST_UFE_NUM_LINES)
      {
         sprintf(temp_buf, "0 %d %d %d %d", line_index - WTI_FIRST_UFE_NUM_LINES, pw_index, du_size, du_size);
      }
      else
      {
         sprintf(temp_buf, "0 %d %d %d %d", line_index, pw_index, du_size, du_size);
      }
#else
      sprintf(temp_buf, "0 %d %d %d %d", line_index, pw_index, du_size, du_size);
#endif
      CLI_F_PwConfigureUnframed(temp_buf);

      sprintf(temp_buf, "0 %d", num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      sprintf(temp_buf, "0 %d", ps);
      CLI_F_Pwe3ChannelRxBufferSize(temp_buf);

      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d",pw_index+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");

      /* Get the ts_diff_if_id from UFE driver  */
      /* Rx timing is used in differential mode */
      sprintf(temp_buf, "0 1 1 %d", WT_ClockRecInterfaceId(line_index));
      CLI_F_Tdm2PsnFlowAggTsParams(temp_buf); /* [ts_used(1-used,0-unused),ts_method(0-WP_IW_CESOP_ABS_TS,1-WP_IW_CESOP_DIFF_TS_MASTER ),ts_diff_if_id] */

      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 8");

      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
#if WTI_DUAL_EMPHY
         if(line_index >= WTI_FIRST_UFE_NUM_LINES)
         {
            CLI_F_PwCreateCascadedOnSecondUfe(temp_buf);
         }
         else
         {
            CLI_F_PwCreateCascaded(temp_buf);
         }
#else
         CLI_F_PwCreateCascaded(temp_buf);
#endif

      sprintf(temp_buf, "0 %d 1",pw_index);
      CLI_F_PwEnable(temp_buf);
      pw_index++;
   }

   for(; pw_index > 0 ; pw_index--)
   {
      sprintf(temp_buf, "0 %d 16 0",pw_index - 1);
      CLI_F_Psn2TdmCesopFlowAggModify(temp_buf);
      
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_CR_105_SonetSdh_T1UnframedNoCas_DifferentDuAndPs(char *StrPrm)
{
   WP_U32 res, cmd, line_index, num_of_lines, num_slots = 0, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_UNFRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_T1;
   WUFE_cas_enable cas_mode = WUFE_CAS_DISABLE;
   WP_U32 interface, du_size = 48, ps = 192;
#ifdef WT_UFE_FRAMER
   WTI_Flexmux_global_setup flex_global_params;
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U8 flexmux_mode=0;
#endif
   WUFE_line_clock_rec_params line_cr_params;


   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 600;
#else
      num_of_lines = 300;
#endif
   }
   else if (res == 2)
   {
      interface = 0; /* use default setup */
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
   }
   else if (res > 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   printf("Running %s [%d][%d]\n",__FUNCTION__,num_of_lines,interface );
   if (num_of_lines != 336) /* This warning should be removed when full bandwidth is supported. */
      printf("!!!!!!!\n!! This test does not run with full bandwidth. Number of lines: %d !!\n!!!!!!!\n", num_of_lines);

   WTI_SetDefaultT1UnfTestParameters(num_of_lines, interface, &transfer_type,
                                     &framing_mode, &cas_mode, &num_slots
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /*******************************************************************************************/
   /*                                     UFE and framer config                               */
   /*******************************************************************************************/

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      sprintf(temp_buf, "0 %d %d %d", line_index, transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d",line_index, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/
   WT_UfeMPLS_L2_FA_Create();

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
#if WTI_DUAL_EMPHY
      if(line_index > 300 && line_index < 336)
      {
         num_of_lines++;
         continue;
      }
#endif
      if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
      {
         /* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
         the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
         if(line_index == WT_LINE_INDEX_TO_FIRST_DCO)
         {
            the_system->clock_rec[line_index].physical_clock_id = WUFE_CLOCK_REC_DCO_ID_0;
            WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, WUFE_CLOCK_REC_DCO_ID_0); /* tx_enable, dco_active, dco_number */
         }
         else
         {
            the_system->clock_rec[line_index].physical_clock_id = WUFE_CLOCK_REC_DCO_ID_1;
            WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, WUFE_CLOCK_REC_DCO_ID_1); /* tx_enable, dco_active, dco_number */
         }
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
#if WTI_DUAL_EMPHY
      if(line_index >= WTI_FIRST_UFE_NUM_LINES)
      {
         WT_UfeLineConfigureAndCreateT1(&the_system->second_ufe, line_index-WTI_FIRST_UFE_NUM_LINES, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);
      }
      else
      {
         WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);
      }
#else
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);
#endif

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);

#if WTI_DUAL_EMPHY
         if (line_index >= WTI_FIRST_UFE_NUM_LINES)
         {
            WT_UfeLineSocketLpbkSetup(&the_system->second_ufe, (line_index-WTI_FIRST_UFE_NUM_LINES), WTI_SOCKET_LOOPBACK_MODE);
         }
         else
         {
            WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
         }
#else
         WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
#endif
   }


   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {

#if WTI_DUAL_EMPHY
      if(line_index > 300 && line_index < 336)
      {
         continue;
      }
#endif
      /* Different DU and PS sizes */
      /* Select between 2 tests:
         1. Full bandwidth: change only few PWs (due to have load on the EMPHY and ENET.
         2. Partial bandwidth: run with 70 PWs, different DU and PS to each PW.*/
      the_system->pw[pw_index].line_index = line_index;

#if 1
      switch (pw_index % 28)
      {
         case 0:
         {
            du_size = 12;
            ps = 72;
            break;
         }
         case 1:
         {
            du_size = 24;
            ps = 72;
            break;
         }
         case 2:
         {
            du_size = 64;
            ps = 128;
            break;
         }
         case 3:
         {
            du_size = 48;
            ps = 240;
            break;
         }

         case 4:
         {
            du_size = 48;
            ps = 48;
            break;
         }
         default:
         {
            du_size = 48;
            ps = 192;
            break;
         }
      }

#else
      switch (pw_index % 3)
      {
         case 0:
         {
            du_size = 24;
            break;
         }
         case 1:
         {
            du_size = 48;
            break;
         }
         case 2:
         {
            du_size = 64;
            break;
         }
      }
      switch (pw_index % 4)
      {
         case 0:
         {
            ps = 48;
            break;
         }
         case 1:
         {
            ps = 144;
            break;
         }
         case 2:
         {
            ps = 192;
            break;
         }
         case 3:
         {
            ps = 384;
            break;
         }
      }
      /* Verify PS%DU */
      if ((ps < du_size) || (ps % du_size))
         ps = du_size * 2;
#endif /* 1 */

#if WTI_DUAL_EMPHY
      if(line_index >= WTI_FIRST_UFE_NUM_LINES)
      {
         pw_config.pw_emphy_num = WTI_EMPHY_SECOND;
      }
      else
      {
         pw_config.pw_emphy_num = WTI_EMPHY_FIRST;
      }
#endif

      printf("--> pw_index = %4d    du_size = %3d   ps = %3d\n",
             pw_index, du_size, ps);
      /*UFE phy and pw configuration*/
#if WTI_DUAL_EMPHY
      if (line_index >= WTI_FIRST_UFE_NUM_LINES)
      {
         sprintf(temp_buf, "0 %d %d %d %d", line_index - WTI_FIRST_UFE_NUM_LINES, pw_index, du_size, du_size);
      }
      else
      {
         sprintf(temp_buf, "0 %d %d %d %d", line_index, pw_index, du_size, du_size);
      }
#else
      sprintf(temp_buf, "0 %d %d %d %d", line_index, pw_index, du_size, du_size);
#endif
      CLI_F_PwConfigureUnframed(temp_buf);

      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      sprintf(temp_buf, "0 %d",ps);
      CLI_F_Pwe3ChannelRxBufferSize(temp_buf);

      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", pw_index+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");

      /* Rx timing is used in differential mode */
      sprintf(temp_buf, "0 1 1 %d", WT_ClockRecInterfaceId(line_index));
      CLI_F_Tdm2PsnFlowAggTsParams(temp_buf); /* [ts_used(1-used,0-unused),ts_method(0-WP_IW_CESOP_ABS_TS,1-WP_IW_CESOP_DIFF_TS_MASTER ),ts_diff_if_id(0-63)] */

      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 8");

      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
#if WTI_DUAL_EMPHY
         if(line_index >= WTI_FIRST_UFE_NUM_LINES)
         {
            CLI_F_PwCreateCascadedOnSecondUfe(temp_buf);
         }
         else
         {
            CLI_F_PwCreateCascaded(temp_buf);
         }
#else
         CLI_F_PwCreateCascaded(temp_buf);
#endif

      sprintf(temp_buf, "0 %d 1",pw_index);
      CLI_F_PwEnable(temp_buf);
      pw_index++;
   }
   
   for(; pw_index > 0 ; pw_index--)
   {
      sprintf(temp_buf, "0 %d 16 0",pw_index - 1);
      CLI_F_Psn2TdmCesopFlowAggModify(temp_buf);
      
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_CR_105_SonetSdh_E1FramedNoCas_DifferentDuAndPs(char *StrPrm)
{
   WP_U32 res, cmd, line_index, num_of_lines, num_slots = 0, new_num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_FRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_E1;
   WUFE_cas_enable cas_mode = WUFE_CAS_DISABLE;
   WP_U32 interface, du_size = 64, ps = 256, first_slot = 0, last_slot = 31;
#ifdef WT_UFE_FRAMER
   WTI_Flexmux_global_setup flex_global_params;
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U8 flexmux_mode=0;
#endif
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WUFE_line_clock_rec_params line_cr_params;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 504;
#else
      num_of_lines = 252;
#endif
   }
   else if (res == 2)
   {
      interface = 0; /* use default setup */
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
   }
   else if (res > 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   printf("Running %s [%d][%d]\n",__FUNCTION__,num_of_lines,interface );

   WTI_SetDefaultE1FrmTestParameters(num_of_lines, interface, &transfer_type,
                                     &framing_mode, &cas_mode, &num_slots
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /*******************************************************************************************/
   /*                                     UFE and framer config                               */
   /*******************************************************************************************/

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
      {
         num_of_lines++;
         continue;
      }
      sprintf(temp_buf, "0 %d %d %d", line_index, transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d", line_index, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/
   WT_UfeMPLS_L2_FA_Create();

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
      {
         continue;
      }
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
      {
         /* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
         the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
         if(line_index == WT_LINE_INDEX_TO_FIRST_DCO)
         {
            the_system->clock_rec[line_index].physical_clock_id = WUFE_CLOCK_REC_DCO_ID_0;
            WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, WUFE_CLOCK_REC_DCO_ID_0); /* tx_enable, dco_active, dco_number */
         }
         else
         {
            the_system->clock_rec[line_index].physical_clock_id = WUFE_CLOCK_REC_DCO_ID_1;
            WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, WUFE_CLOCK_REC_DCO_ID_1); /* tx_enable, dco_active, dco_number */
         }
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
#if WTI_DUAL_EMPHY
      if(line_index >= WTI_FIRST_UFE_NUM_LINES)
      {
         WT_UfeLineConfigureAndCreateE1(&the_system->second_ufe, line_index-WTI_FIRST_UFE_NUM_LINES, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);
      }
      else
      {
         WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);
      }
#else
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);
#endif

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);
#else
      /* UFE line configuration, creation and enabling */
#if WTI_DUAL_EMPHY
      if(line_index >= WTI_FIRST_UFE_NUM_LINES)
      {
         WT_UfeLineConfigureAndCreateE1(&the_system->second_ufe, line_index-WTI_FIRST_UFE_NUM_LINES, transfer_type, framing_mode, cas_mode, 1, NULL);
      }
      else
      {
         WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
      }
#else
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

#if WTI_DUAL_EMPHY
         if (line_index >= WTI_FIRST_UFE_NUM_LINES)
         {
            WT_UfeLineSocketLpbkSetup(&the_system->second_ufe, (line_index-WTI_FIRST_UFE_NUM_LINES), WTI_SOCKET_LOOPBACK_MODE);
         }
         else
         {
            WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
         }
#else
         WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
#endif
   }

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      /* Initial state: */
      new_num_slots = num_slots;
      first_slot = 0;
      last_slot = new_num_slots-1;

      if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
      {
         continue;
      }

      the_system->pw[pw_index].line_index = line_index;

      /* Different DU and PS sizes */
      /* Select between 2 tests:
         1. Full bandwidth: change only few PWs (due to have load on the EMPHY and ENET.
         2. Partial bandwidth: run with 42 PWs, different DU and PS to each PW.*/
#if 1
      switch (pw_index % 63)
      {
         case 0:
         {
            du_size = 16;
            ps = 64;
            first_slot = 0;
            last_slot = 15;
            new_num_slots = last_slot-first_slot+1;
            break;
         }
         case 1:
         {
            du_size = 32;
            ps = 192;
            break;
         }
         case 2:
         {
            du_size = 25;
            ps = 50;
            first_slot = 23;
            last_slot = 27;
            new_num_slots = last_slot-first_slot+1;
            break;
         }
         case 3:
         {
            du_size = 60;
            ps = 60;
            first_slot = 1;
            last_slot = 30;
            new_num_slots = last_slot-first_slot+1;
            break;
         }
         case 4:
         {
            du_size = 32;
            ps = 192;
            break;
         }

         default:
         {
            du_size = 64;
            ps = 256;
            break;
         }
      }
#else
      /* This section works well with up to 42 PW. */
      du_size = 64 / (1<<(pw_index % 3));
      ps = 64 * ((pw_index % 4)+1);
      if (du_size < new_num_slots)
      {
         last_slot = du_size-1;
         new_num_slots = du_size;
      }
      else
         last_slot = new_num_slots-1;
#endif /* 1 */

#if WTI_DUAL_EMPHY
      if(line_index >= WTI_FIRST_UFE_NUM_LINES)
      {
         pw_config.pw_emphy_num = WTI_EMPHY_SECOND;
      }
      else
      {
         pw_config.pw_emphy_num = WTI_EMPHY_FIRST;
      }
#endif

      printf("--> pw_index = %4d    du_size = %3d   ps = %3d   new_num_slots = %2d  slots: %2d - %2d\n",
             pw_index, du_size, ps, new_num_slots, first_slot, last_slot);
      /*UFE phy and pw configuration*/
#if WTI_DUAL_EMPHY
      if(line_index >= WTI_FIRST_UFE_NUM_LINES)
      {
         sprintf(temp_buf, "0 %d %d %d %d %d %d-%d",
                 line_index - WTI_FIRST_UFE_NUM_LINES, pw_index, du_size, du_size, new_num_slots, first_slot, last_slot);
      }
      else
      {
         sprintf(temp_buf, "0 %d %d %d %d %d %d-%d",
                 line_index, pw_index, du_size, du_size, new_num_slots, first_slot, last_slot);
      }
#else
      sprintf(temp_buf, "0 %d %d %d %d %d %d-%d",
              line_index, pw_index, du_size, du_size, new_num_slots, first_slot, last_slot);
#endif
      CLI_F_PwConfigureFramed(temp_buf);

      sprintf(temp_buf, "0 %d",new_num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      sprintf(temp_buf, "0 %d", ps);
      CLI_F_Pwe3ChannelRxBufferSize(temp_buf);

      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", pw_index+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      /* Rx timing is used in differential mode */
      sprintf(temp_buf, "0 1 1 %d", WT_ClockRecInterfaceId(line_index));
      CLI_F_Tdm2PsnFlowAggTsParams(temp_buf); /* [ts_used(1-used,0-unused),ts_method(0-WP_IW_CESOP_ABS_TS,1-WP_IW_CESOP_DIFF_TS_MASTER ),ts_diff_if_id(0-63)] */
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 8");

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
#if WTI_CESOP_TO_MPLS_FA_CASCADING
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
#if WTI_DUAL_EMPHY
         if(line_index >= WTI_FIRST_UFE_NUM_LINES)
         {
            CLI_F_PwCreateCascadedOnSecondUfe(temp_buf);
         }
         else
         {
            CLI_F_PwCreateCascaded(temp_buf);
         }
#else
         CLI_F_PwCreateCascaded(temp_buf);
#endif
#else
      sprintf(temp_buf, "0 %d %d %d %d", 0,0,1,pw_index);
      CLI_F_PwCreate(temp_buf);
#endif
      sprintf(temp_buf, "0 %d 1",pw_index);
      CLI_F_PwEnable(temp_buf);
#else
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
#if WTI_DUAL_EMPHY
         if(line_index >= WTI_FIRST_UFE_NUM_LINES)
         {
            CLI_F_PwCreateCascadedOnSecondUfe(temp_buf);
         }
         else
         {
            CLI_F_PwCreateCascaded(temp_buf);
         }
#else
         CLI_F_PwCreateCascaded(temp_buf);
#endif

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
      pw_index++;
   }

   for(; pw_index > 0 ; pw_index--)
   {
      sprintf(temp_buf, "0 %d 16 0",pw_index - 1);
      CLI_F_Psn2TdmCesopFlowAggModify(temp_buf);
      
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_CR_105_SonetSdh_T1FramedNoCas_DifferentDuAndPs(char *StrPrm)
{
   WP_U32 res, cmd, line_index, num_of_lines, num_slots = 0, new_num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_FRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_T1;
   WUFE_cas_enable cas_mode = WUFE_CAS_DISABLE;
   WP_U32 interface, du_size = 48, ps = 192, first_slot = 0, last_slot = 23;
#ifdef WT_UFE_FRAMER
   WTI_Flexmux_global_setup flex_global_params;
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U8 flexmux_mode=0;
#endif
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WUFE_line_clock_rec_params line_cr_params;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 672;
#else
      num_of_lines = 336;
#endif
   }
   else if (res == 2)
   {
      interface = 0; /* use default setup */
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
   }
   else if (res > 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   printf("Running %s [%d][%d]\n",__FUNCTION__,num_of_lines,interface );

   WTI_SetDefaultT1FrmTestParameters(num_of_lines, interface, &transfer_type,
                                     &framing_mode, &cas_mode, &num_slots
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /*******************************************************************************************/
   /*                                     UFE and framer config                               */
   /*******************************************************************************************/

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      sprintf(temp_buf, "0 %d %d %d", line_index, transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d", line_index, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/
   WT_UfeMPLS_L2_FA_Create();

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
      {
         /* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
         the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
         if(line_index == WT_LINE_INDEX_TO_FIRST_DCO)
         {
            the_system->clock_rec[line_index].physical_clock_id = WUFE_CLOCK_REC_DCO_ID_0;
            WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, WUFE_CLOCK_REC_DCO_ID_0); /* tx_enable, dco_active, dco_number */
         }
         else
         {
            the_system->clock_rec[line_index].physical_clock_id = WUFE_CLOCK_REC_DCO_ID_1;
            WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, WUFE_CLOCK_REC_DCO_ID_1); /* tx_enable, dco_active, dco_number */
         }
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
#if WTI_DUAL_EMPHY
      if(line_index >= WTI_FIRST_UFE_NUM_LINES)
      {
         WT_UfeLineConfigureAndCreateT1(&the_system->second_ufe, line_index-WTI_FIRST_UFE_NUM_LINES, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);
      }
      else
      {
         WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);
      }
#else
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);
#endif

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);
#else
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

#if WTI_DUAL_EMPHY
      if (line_index >= WTI_FIRST_UFE_NUM_LINES)
      {
         WT_UfeLineSocketLpbkSetup(&the_system->second_ufe, (line_index-WTI_FIRST_UFE_NUM_LINES), WTI_SOCKET_LOOPBACK_MODE);
      }
      else
      {
         WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
      }
#else
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
#endif
   }

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      /* Initial state: */
      new_num_slots = num_slots;
      first_slot = 0;
      last_slot = new_num_slots-1;

      the_system->pw[pw_index].line_index = line_index;

      /* Different DU and PS sizes */
      /* Select between 2 tests:
         1. Full bandwidth: change only few (due to have load on the EMPHY and ENET.
         2. Partial bandwidth: run with 70 PWs, different DU and PS to each PW.*/
#if 1
      switch (pw_index % 28)
      {
         case 0:
         {
            du_size = 24;
            ps = 72;
            break;
         }
         case 1:
         {
            du_size = 64;
            ps = 128;
            new_num_slots = 16;
            first_slot = 0;
            last_slot = new_num_slots-1;
            break;
         }

         case 2:
         {
            du_size = 48;
            ps = 48;
            new_num_slots = 12;
            first_slot = 0;
            last_slot = new_num_slots-1;
            break;
         }
         case 3:
         {

            du_size = 48;
            ps = 192;
            new_num_slots = 4;
            //first_slot = 0;
            //last_slot = new_num_slots-1;
            first_slot = 4;
            last_slot = new_num_slots+first_slot-1;
            break;
         }
         default:
         {
            du_size = 48;
            ps = 192;
            break;
         }
      }
#else
      /* This section works well with up to 70 PW. */
      switch (pw_index % 3)
      {
         case 0:
         {
            du_size = 24;
            break;
         }
         case 1:
         {
            du_size = 48;
            break;
         }
         case 2:
         {
            du_size = 64;
            new_num_slots = 16; /* In order to contain full number of slots array in DU */
            break;
         }
      }
      switch (pw_index % 4)
      {
         case 0:
         {
            ps = 48;
            break;
         }
         case 1:
         {
            ps = 144;
            break;
         }
         case 2:
         {
            ps = 192;
            break;
         }
         case 3:
         {
            ps = 384;
            break;
         }
      }
      /* Verify PS%DU */
      if ((ps < du_size) || (ps % du_size))
         ps = du_size * 2;
      if (du_size < new_num_slots)
      {
         last_slot = du_size-1;
         new_num_slots = du_size;
      }
      else
         last_slot = new_num_slots-1;
#endif /* 1 */

#if WTI_DUAL_EMPHY
      if(line_index >= WTI_FIRST_UFE_NUM_LINES)
      {
         pw_config.pw_emphy_num = WTI_EMPHY_SECOND;
      }
      else
      {
         pw_config.pw_emphy_num = WTI_EMPHY_FIRST;
      }
#endif

      printf("--> pw_index = %4d    du_size = %3d   ps = %3d   new_num_slots = %2d  slots: %2d - %2d\n",
             pw_index, du_size, ps, new_num_slots, first_slot, last_slot);
      /*UFE phy and pw configuration*/
#if WTI_DUAL_EMPHY
      if(line_index >= WTI_FIRST_UFE_NUM_LINES)
      {
         sprintf(temp_buf, "0 %d %d %d %d %d %d-%d",
                 line_index - WTI_FIRST_UFE_NUM_LINES, pw_index, du_size, du_size, new_num_slots, first_slot, last_slot);
      }
      else
      {
         sprintf(temp_buf, "0 %d %d %d %d %d %d-%d",
                 line_index, pw_index, du_size, du_size, new_num_slots, first_slot, last_slot);
      }
#else
      sprintf(temp_buf, "0 %d %d %d %d %d %d-%d",
              line_index, pw_index, du_size, du_size, new_num_slots, first_slot, last_slot);
#endif
      CLI_F_PwConfigureFramed(temp_buf);

      sprintf(temp_buf, "0 %d",new_num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      sprintf(temp_buf, "0 %d",ps);
      CLI_F_Pwe3ChannelRxBufferSize(temp_buf);

      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", pw_index+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      /* Rx timing is used in differential mode */
      sprintf(temp_buf, "0 1 1 %d", WT_ClockRecInterfaceId(line_index));
      CLI_F_Tdm2PsnFlowAggTsParams(temp_buf); /* [ts_used(1-used,0-unused),ts_method(0-WP_IW_CESOP_ABS_TS,1-WP_IW_CESOP_DIFF_TS_MASTER ),ts_diff_if_id(0-63)] */
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 8");

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
#if WTI_CESOP_TO_MPLS_FA_CASCADING
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
#if WTI_DUAL_EMPHY
         if(line_index >= WTI_FIRST_UFE_NUM_LINES)
         {
            CLI_F_PwCreateCascadedOnSecondUfe(temp_buf);
         }
         else
         {
            CLI_F_PwCreateCascaded(temp_buf);
         }
#else
         CLI_F_PwCreateCascaded(temp_buf);
#endif
#else
      sprintf(temp_buf, "0 %d %d %d %d", 0,0,1,pw_index);
      CLI_F_PwCreate(temp_buf);
#endif
      sprintf(temp_buf, "0 %d 1",pw_index);
      CLI_F_PwEnable(temp_buf);
#else
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
#if WTI_DUAL_EMPHY
         if(line_index >= WTI_FIRST_UFE_NUM_LINES)
         {
            CLI_F_PwCreateCascadedOnSecondUfe(temp_buf);
         }
         else
         {
            CLI_F_PwCreateCascaded(temp_buf);
         }
#else
         CLI_F_PwCreateCascaded(temp_buf);
#endif

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
      pw_index++;
   }

   for(; pw_index > 0 ; pw_index--)
   {
      sprintf(temp_buf, "0 %d 16 0",pw_index - 1);
      CLI_F_Psn2TdmCesopFlowAggModify(temp_buf);
      
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_CR_106_SonetSdh_E1UnframedNoCas_AdapDiffPerLine(char *StrPrm)
{
   WP_U32 res, cmd, i, num_of_lines;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WTI_Flexmux_global_setup flex_global_params;
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 504;
#else
      num_of_lines = 252;
#endif
   }
   else if (res == 2)
   {
      interface = 0; /* use default setup */
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
   }
   else if (res > 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   printf("Running %s [%d][%d]\n",__FUNCTION__,num_of_lines,interface );

   strcpy(test_setup.test_name, __FUNCTION__);
   WTI_SetDefaultE1UnfTestParameters(num_of_lines, interface, &(test_setup.transfer_type),
								     &(test_setup.framing_mode), &(test_setup.cas_mode), &(test_setup.num_slots)
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* Set CR operation mode and set the maximun number of lines in the test */
   for (i=WT_FIRST_LINE_INDEX;i<num_of_lines+WT_FIRST_LINE_INDEX;i++)
   {
	   if((i%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
	   {
		   num_of_lines++;
		   continue;
	   }

	   if ((i%2))
		   test_setup.line_cr_operation_mode[i] = WP_IW_CLOCK_REC_ADAPTIVE_MODE;
	   else
		   test_setup.line_cr_operation_mode[i] = WP_IW_CLOCK_REC_DIFFERENTIAL_MODE;
   }


   /* allocate CESoP system */
   WTI_SystemAlloc();

   /*******************************************************************************************/
   /*                                     UFE and framer config                               */
   /*******************************************************************************************/

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_CR_106_SonetSdh_T1UnframedNoCas_AdapDiffPerLine(char *StrPrm)
{
   WP_U32 res, cmd, i, num_of_lines;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WTI_Flexmux_global_setup flex_global_params;
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 672;
#else
      num_of_lines = 336;
#endif
   }
   else if (res == 2)
   {
      interface = 0; /* use default setup */
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
   }
   else if (res > 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   printf("Running %s [%d][%d]\n",__FUNCTION__,num_of_lines,interface );


   strcpy(test_setup.test_name, __FUNCTION__);
   WTI_SetDefaultT1UnfTestParameters(num_of_lines, interface, &(test_setup.transfer_type),
									 &(test_setup.framing_mode), &(test_setup.cas_mode), &(test_setup.num_slots)
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* Set CR operation mode */
   for (i=WT_FIRST_LINE_INDEX;i<num_of_lines+WT_FIRST_LINE_INDEX;i++)
   {
	   if ((i%2))
		   test_setup.line_cr_operation_mode[i] = WP_IW_CLOCK_REC_ADAPTIVE_MODE;
	   else
		   test_setup.line_cr_operation_mode[i] = WP_IW_CLOCK_REC_DIFFERENTIAL_MODE;
   }

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /*******************************************************************************************/
   /*                                     UFE and framer config                               */
   /*******************************************************************************************/

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_CR_106_SonetSdh_E1FramedNoCas_AdapDiffPerLine(char *StrPrm)
{
   WP_U32 res, cmd, i, num_of_lines;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WTI_Flexmux_global_setup flex_global_params;
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 504;
#else
      num_of_lines = 252;
#endif
   }
   else if (res == 2)
   {
      interface = 0; /* use default setup */
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
   }
   else if (res > 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   printf("Running %s [%d][%d]\n",__FUNCTION__,num_of_lines,interface );

   strcpy(test_setup.test_name, __FUNCTION__);
   WTI_SetDefaultE1FrmTestParameters(num_of_lines, interface, &(test_setup.transfer_type),
									 &(test_setup.framing_mode), &(test_setup.cas_mode), &(test_setup.num_slots)
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* Set CR operation mode and set the maximun number of lines in the test */
   for (i=WT_FIRST_LINE_INDEX;i<num_of_lines+WT_FIRST_LINE_INDEX;i++)
   {
	   if((i%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
	   {
		   num_of_lines++;
		   continue;
	   }

	   if ((i%2))
		   test_setup.line_cr_operation_mode[i] = WP_IW_CLOCK_REC_ADAPTIVE_MODE;
	   else
		   test_setup.line_cr_operation_mode[i] = WP_IW_CLOCK_REC_DIFFERENTIAL_MODE;
   }


   /* allocate CESoP system */
   WTI_SystemAlloc();

   /*******************************************************************************************/
   /*                                     UFE and framer config                               */
   /*******************************************************************************************/

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_CR_106_SonetSdh_T1FramedNoCas_AdapDiffPerLine(char *StrPrm)
{
   WP_U32 res, cmd, i, num_of_lines;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WTI_Flexmux_global_setup flex_global_params;
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
#if WTI_DUAL_EMPHY
      num_of_lines = 672;
#else
      num_of_lines = 336;
#endif
   }
   else if (res == 2)
   {
      interface = 0; /* use default setup */
      if (num_of_lines == 0)
      {
         WTI_TerminatePrintError("Invalid parameter. Number of lines is zero", __LINE__);
         return;
      }
   }
   else if (res > 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   printf("Running %s [%d][%d]\n",__FUNCTION__,num_of_lines,interface );

   strcpy(test_setup.test_name, __FUNCTION__);
   WTI_SetDefaultT1FrmTestParameters(num_of_lines, interface, &(test_setup.transfer_type),
									 &(test_setup.framing_mode), &(test_setup.cas_mode), &(test_setup.num_slots)
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* Set CR operation mode */
   for (i=WT_FIRST_LINE_INDEX;i<num_of_lines+WT_FIRST_LINE_INDEX;i++)
   {
	   if ((i%2))
		   test_setup.line_cr_operation_mode[i] = WP_IW_CLOCK_REC_ADAPTIVE_MODE;
	   else
		   test_setup.line_cr_operation_mode[i] = WP_IW_CLOCK_REC_DIFFERENTIAL_MODE;
   }


   /* allocate CESoP system */
   WTI_SystemAlloc();

   /*******************************************************************************************/
   /*                                     UFE and framer config                               */
   /*******************************************************************************************/

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);
}

#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE (started at test case 104_E1_Frm) */


/*****************************************************************************
 * Function name: CLI_F_CR_115_Sanity_Board_Check_SonetSdh_E1UnframedNoCas
 * Description  : Sanity test for new board that checks the everything on the board
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void CLI_F_CR_115_Sanity_Board_Check_SonetSdh_E1UnframedNoCas(char *StrPrm)
{
   WP_U32 i=0,stats_result=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];

#ifdef WT_UFE_FRAMER
   WUFE_un_framed framing_mode = WUFE_UNFRAMED;
   WP_U8 prbs_errors = 0;
   WP_U8 prbs_tmp = 0;
#endif


   CLI_F_CR_101_SonetSdh_E1UnframedNoCas("0");

#ifdef WT_UFE_FRAMER
   printf("Checking PRBS...\n");

   for (i = WT_FIRST_LINE_INDEX; i < (WT_FIRST_LINE_INDEX+336); i++)
   {
      if((i % 84) > 2)
      continue;

      if ((i % 4) != 3) /* In E1 transfer type, every tug includes only 3 tu*/
      {
         prbs_tmp = WTI_FlexmuxCheckPrbsResult(0, i, framing_mode);
         prbs_errors |= prbs_tmp;
         if (prbs_tmp)
         {
            printf("PRBS check failed for tu %d\n", i);
         }
      }

   }
   if (prbs_errors == 0)
   {
      printf("PRBS check Passed\n");
   }
   else
   {
      printf("PRBS check failed\n");
   }
#endif


   WPL_Delay(60000000);

   stats_result=CLI_F_GlobalStatisticsCheck("0");

   printf("********************************\n");
   printf("*regular lines test finished   *\n");
   printf("*switching to protection lines *\n");
   printf("********************************\n");

   for(i=0;i<4;i++)
   {
      sprintf(temp_buf, "0 0 %d 1",i);
      CLI_F_UfeLineProtectionSwitch(temp_buf);
      printf("line %d switched to protection\n",i);

   }

#ifdef WT_UFE_FRAMER
   printf("Checking PRBS...\n");

   for (i = WT_FIRST_LINE_INDEX; i < (WT_FIRST_LINE_INDEX+336); i++)
   {

      if((i % 84) > 2)
      continue;

      if ((i % 4) != 3) /* In E1 transfer type, every tug includes only 3 tu*/
      {
         prbs_tmp = WTI_FlexmuxCheckPrbsResult(0, i, framing_mode);
         prbs_errors |= prbs_tmp;
         if (prbs_tmp)
         {
            printf("PRBS check failed for tu %d\n", i);
         }
      }
   }

   if (prbs_errors == 0)
   {
      printf("PRBS check Passed\n");
   }
   else
   {
      printf("PRBS check failed\n");
   }
#endif
//   WPL_Delay(60000000);

   stats_result=CLI_F_GlobalStatisticsCheck("0");
}

#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhE3Loopback(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines,vc_type, physical_param;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4 = WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_flexmux_physical_conf physical_configuration;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &interface,&physical_param,&vc_type, &num_of_lines);
   if (res != 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters -- 5 required", __LINE__);
      return;
   }
#if WTI_DUAL_EMPHY
   num_of_lines = 2 * num_of_lines;
#endif

   test_setup.framing_mode = WUFE_UNFRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = WTI_E3_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.ds3_e3_framing_type = WT_FLEXMUX_CONNECTION_TYPE_E3_UNFRAMED;
   test_setup.vc_type = vc_type;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);



   switch(interface)
   {
      case 0:
         test_setup.transfer_type = WUFE_SDH_TYPE_E3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
         if(vc_type == 0)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         else if(vc_type == 1)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
         else
         {
            WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-VC3, 1-VC4 ", __LINE__);
            return;
         }
#endif
         break;
      case 1:
         test_setup.transfer_type = WUFE_SONET_TYPE_E3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET", __LINE__);
         return;
   }
#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:
         physical_configuration = WT_FLEXMUX_UFE_412_1_PORT_OC12;
         break;
      case 1:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3;
         break;
      case 2:
         physical_configuration = WT_FLEXMUX_UFE_412_2_PORTS_OC3;
         break;
      case 3:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER;
         break;
      case 4:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE;
         break;
#if WTI_DUAL_EMPHY
         case 5:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_UPI1_MASTER_UPI3_SLAVE;
         break;
#endif
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value\n"
                                 "Allowed values are: 0-1xOC12, 1-4xOC3 2-2xOC3", __LINE__);
         return;
   }
#endif

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER

   /* Initialize the Flexmux framer in SDH mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   physical_configuration,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);
   WT_UfeLineSocketLpbkSetup(&the_system->ufe, 0, WUFE_LINE_LOOPBACK_TYPE_SYSTEM);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhE3ProtectionSwitch(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines,vc_type, physical_param;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4 = WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_flexmux_physical_conf physical_configuration;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &interface,&physical_param,&vc_type, &num_of_lines);
   if (res != 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters -- 5 required", __LINE__);
      return;
   }
#if WTI_DUAL_EMPHY
   num_of_lines = 2 * num_of_lines;
#endif

   test_setup.framing_mode = WUFE_UNFRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = WTI_E3_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.ds3_e3_framing_type = WT_FLEXMUX_CONNECTION_TYPE_E3_UNFRAMED;
   test_setup.vc_type = vc_type;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);



   switch(interface)
   {
      case 0:
         test_setup.transfer_type = WUFE_SDH_TYPE_E3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
         if(vc_type == 0)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         else if(vc_type == 1)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
         else
         {
            WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-VC3, 1-VC4 ", __LINE__);
            return;
         }
#endif
         break;
      case 1:
         test_setup.transfer_type = WUFE_SONET_TYPE_E3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET", __LINE__);
         return;
   }
#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:
         physical_configuration = WT_FLEXMUX_UFE_412_1_PORT_OC12;
         break;
      case 1:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3;
         break;
      case 2:
         physical_configuration = WT_FLEXMUX_UFE_412_2_PORTS_OC3;
         break;
      case 3:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER;
         break;
      case 4:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE;
         break;
#if WTI_DUAL_EMPHY
         case 5:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_UPI1_MASTER_UPI3_SLAVE;
         break;
#endif
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value\n"
                                 "Allowed values are: 0-1xOC12, 1-4xOC3 2-2xOC3", __LINE__);
         return;
   }
#endif

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER

   /* Initialize the Flexmux framer in SDH mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   physical_configuration,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

#ifdef WT_UFE_FRAMER
   {
      WP_U32 line_port_id = 0;
      for (line_port_id = 0; line_port_id<2; line_port_id++)
      {
         WTI_FlexmuxForceB(0,line_port_id);
         WTI_FlexmuxForceA(0,line_port_id);
         WTI_FlexmuxForceA(0,line_port_id);
         WTI_FlexmuxForceB(0,line_port_id);
         WTI_FlexmuxForceB(0,line_port_id);
         WTI_FlexmuxForceA(0,line_port_id);
         WTI_FlexmuxForceB(0,line_port_id);
         WTI_FlexmuxForceA(0,line_port_id);
      }
   }
#endif

}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhE3UnframedNoCas(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines,vc_type, physical_param;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4 = WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_flexmux_physical_conf physical_configuration;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &interface,&physical_param,&vc_type, &num_of_lines);
   if (res != 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters -- 5 required", __LINE__);
      return;
   }
#if WTI_DUAL_EMPHY
   num_of_lines = 2 * num_of_lines;
#endif

   test_setup.framing_mode = WUFE_UNFRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = WTI_E3_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.ds3_e3_framing_type = WT_FLEXMUX_CONNECTION_TYPE_E3_UNFRAMED;
   test_setup.vc_type = vc_type;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);



   switch(interface)
   {
      case 0:
         test_setup.transfer_type = WUFE_SDH_TYPE_E3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
         if(vc_type == 0)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         else if(vc_type == 1)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
         else
         {
            WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-VC3, 1-VC4 ", __LINE__);
            return;
         }
#endif
         break;
      case 1:
         test_setup.transfer_type = WUFE_SONET_TYPE_E3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET", __LINE__);
         return;
   }
#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:
         physical_configuration = WT_FLEXMUX_UFE_412_1_PORT_OC12;
         break;
      case 1:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3;
         break;
      case 2:
         physical_configuration = WT_FLEXMUX_UFE_412_2_PORTS_OC3;
         break;
      case 3:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER;
         break;
      case 4:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE;
         break;
#if WTI_DUAL_EMPHY
         case 5:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_UPI1_MASTER_UPI3_SLAVE;
         break;
#endif
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value\n"
                                 "Allowed values are: 0-1xOC12, 1-4xOC3 2-2xOC3", __LINE__);
         return;
   }
#endif

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER

   /* Initialize the Flexmux framer in SDH mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   physical_configuration,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhDS3Loopback(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines,vc_type, physical_param;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4 = WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_flexmux_physical_conf physical_configuration;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &interface,&physical_param,&vc_type, &num_of_lines);
   if (res != 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters -- 5 required", __LINE__);
      return;
   }
#if WTI_DUAL_EMPHY
   num_of_lines = 2 * num_of_lines;
#endif

   test_setup.framing_mode = WUFE_UNFRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = WTI_DS3_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.ds3_e3_framing_type = WT_FLEXMUX_CONNECTION_TYPE_DS3_UNFRAMED;
   test_setup.vc_type = vc_type;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);



   switch(interface)
   {
      case 0:
         test_setup.transfer_type = WUFE_SDH_TYPE_DS3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
         if(vc_type == 0)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         else if(vc_type == 1)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
         else
         {
            WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-VC3, 1-VC4 ", __LINE__);
            return;
         }
#endif
         break;
      case 1:
         test_setup.transfer_type = WUFE_SONET_TYPE_DS3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET", __LINE__);
         return;
   }
#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:
         physical_configuration = WT_FLEXMUX_UFE_412_1_PORT_OC12;
         break;
      case 1:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3;
         break;
      case 2:
         physical_configuration = WT_FLEXMUX_UFE_412_2_PORTS_OC3;
         break;
      case 3:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER;
         break;
      case 4:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE;
         break;
#if WTI_DUAL_EMPHY
         case 5:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_UPI1_MASTER_UPI3_SLAVE;
         break;
#endif
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value\n"
                                 "Allowed values are: 0-1xOC12, 1-4xOC3 2-2xOC3", __LINE__);
         return;
   }
#endif

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER

   /* Initialize the Flexmux framer in SDH mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   physical_configuration,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);
   WT_UfeLineSocketLpbkSetup(&the_system->ufe, 0, WUFE_LINE_LOOPBACK_TYPE_SYSTEM);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhDS3UnframedNoCas(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines,vc_type, physical_param;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4 = WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_flexmux_physical_conf physical_configuration;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &interface,&physical_param,&vc_type, &num_of_lines);
   if (res != 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters -- 5 required", __LINE__);
      return;
   }
#if WTI_DUAL_EMPHY
   num_of_lines = 2 * num_of_lines;
#endif

   test_setup.framing_mode = WUFE_UNFRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = WTI_DS3_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.ds3_e3_framing_type = WT_FLEXMUX_CONNECTION_TYPE_DS3_UNFRAMED;
   test_setup.vc_type = vc_type;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);



   switch(interface)
   {
      case 0:
         test_setup.transfer_type = WUFE_SDH_TYPE_DS3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
         if(vc_type == 0)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         else if(vc_type == 1)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
         else
         {
            WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-VC3, 1-VC4 ", __LINE__);
            return;
         }
#endif
         break;
      case 1:
         test_setup.transfer_type = WUFE_SONET_TYPE_DS3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET", __LINE__);
         return;
   }
#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:
         physical_configuration = WT_FLEXMUX_UFE_412_1_PORT_OC12;
         break;
      case 1:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3;
         break;
      case 2:
         physical_configuration = WT_FLEXMUX_UFE_412_2_PORTS_OC3;
         break;
      case 3:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER;
         break;
      case 4:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE;
         break;
#if WTI_DUAL_EMPHY
         case 5:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_UPI1_MASTER_UPI3_SLAVE;
         break;
#endif
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value\n"
                                 "Allowed values are: 0-1xOC12, 1-4xOC3 2-2xOC3", __LINE__);
         return;
   }
#endif

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER

   /* Initialize the Flexmux framer in SDH mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   physical_configuration,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

   WTI_enable_alarms_and_performance_monitoring(interface);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhDS3ProtectionSwitch(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines,vc_type, physical_param;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4 = WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_flexmux_physical_conf physical_configuration;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &interface,&physical_param,&vc_type, &num_of_lines);
   if (res != 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters -- 5 required", __LINE__);
      return;
   }
#if WTI_DUAL_EMPHY
   num_of_lines = 2 * num_of_lines;
#endif

   test_setup.framing_mode = WUFE_UNFRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = WTI_DS3_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.ds3_e3_framing_type = WT_FLEXMUX_CONNECTION_TYPE_DS3_UNFRAMED;
   test_setup.vc_type = vc_type;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);



   switch(interface)
   {
      case 0:
         test_setup.transfer_type = WUFE_SDH_TYPE_DS3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
         if(vc_type == 0)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         else if(vc_type == 1)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
         else
         {
            WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-VC3, 1-VC4 ", __LINE__);
            return;
         }
#endif
         break;
      case 1:
         test_setup.transfer_type = WUFE_SONET_TYPE_DS3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET", __LINE__);
         return;
   }
#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:
         physical_configuration = WT_FLEXMUX_UFE_412_1_PORT_OC12;
         break;
      case 1:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3;
         break;
      case 2:
         physical_configuration = WT_FLEXMUX_UFE_412_2_PORTS_OC3;
         break;
      case 3:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER;
         break;
      case 4:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE;
         break;
#if WTI_DUAL_EMPHY
         case 5:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_UPI1_MASTER_UPI3_SLAVE;
         break;
#endif
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value\n"
                                 "Allowed values are: 0-1xOC12, 1-4xOC3 2-2xOC3", __LINE__);
         return;
   }
#endif

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER

   /* Initialize the Flexmux framer in SDH mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   physical_configuration,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

#ifdef WT_UFE_FRAMER
   {
      WP_U32 line_port_id = 0;
      for (line_port_id = 0; line_port_id<2; line_port_id++)
      {
         WTI_FlexmuxForceB(0,line_port_id);
         WTI_FlexmuxForceA(0,line_port_id);
         WTI_FlexmuxForceA(0,line_port_id);
         WTI_FlexmuxForceB(0,line_port_id);
         WTI_FlexmuxForceB(0,line_port_id);
         WTI_FlexmuxForceA(0,line_port_id);
         WTI_FlexmuxForceB(0,line_port_id);
         WTI_FlexmuxForceA(0,line_port_id);
      }
   }
#endif

}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhE3FramedNoCas(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines, vc_type, physical_param;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_flexmux_physical_conf physical_configuration;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &interface,&physical_param,&vc_type,&num_of_lines);
   if (res != 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

#if WTI_DUAL_EMPHY
   num_of_lines = 2 * num_of_lines;
#endif

   test_setup.framing_mode = WUFE_FRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = WTI_E3_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.ds3_e3_framing_type = WT_FLEXMUX_CONNECTION_TYPE_E3_FRAMED_G751;
   test_setup.vc_type = vc_type;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);

   switch(interface)
   {
      case 0:
         test_setup.transfer_type = WUFE_SDH_TYPE_E3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
         if(vc_type == 0)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         else if(vc_type == 1)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
         else
         {
            WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-VC3, 1-VC4 ", __LINE__);
            return;
         }
#endif
         break;

      case 1:
         test_setup.transfer_type = WUFE_SONET_TYPE_E3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }
#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:physical_configuration = WT_FLEXMUX_UFE_412_1_PORT_OC12;
         break;
      case 1:physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3;
         break;
      case 2:physical_configuration = WT_FLEXMUX_UFE_412_2_PORTS_OC3;
         break;
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value, Allowed values are: 0-1xOC12, 1-4xOC3 2-2xOC3 ", __LINE__);
         return;
   }
#endif

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   physical_configuration,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

   WTI_enable_alarms_and_performance_monitoring(interface);
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhDS3FramedM23NoCas(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines, vc_type, physical_param;
#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING
   WP_U32 i, result;
#endif
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_flexmux_physical_conf physical_configuration;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &interface,&physical_param,&vc_type,&num_of_lines);
   if (res != 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

#if WTI_DUAL_EMPHY
   num_of_lines = 2 * num_of_lines;
#endif

   test_setup.framing_mode = WUFE_FRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = WTI_DS3_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.ds3_e3_framing_type = WT_FLEXMUX_CONNECTION_TYPE_DS3_FRAMED_M23;
   test_setup.vc_type = vc_type;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);

   switch(interface)
   {
      case 0:
         test_setup.transfer_type = WUFE_SDH_TYPE_DS3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
         if(vc_type == 0)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         else if(vc_type == 1)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
         else
         {
            WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-VC3, 1-VC4 ", __LINE__);
            return;
         }
#endif
         break;

      case 1:
         test_setup.transfer_type = WUFE_SONET_TYPE_DS3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }
#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:physical_configuration = WT_FLEXMUX_UFE_412_1_PORT_OC12;
         break;
      case 1:physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3;
         break;
      case 2:physical_configuration = WT_FLEXMUX_UFE_412_2_PORTS_OC3;
         break;
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value, Allowed values are: 0-1xOC12, 1-4xOC3 2-2xOC3 ", __LINE__);
         return;
   }
#endif

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   physical_configuration,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

   WTI_enable_alarms_and_performance_monitoring(interface);

#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING
   for (i = 0; i < 12 ; ++i)
   {
      if ((result = WPX_FRMR_PM_E3T3_EnableMonitoring(
              0, 0, i*28, WPX_UFE_FRAMER_PDH_E3T3_PM_BIP)) != WPX_UFE_FRAMER_OK)
      {
         printf("T3 BIP Monitoring failed on %d with %s\n", i, OMIINO_FRAMER_ErrorCodeToTxt(result));
         break;
      }
   }
#endif
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhDS3FramedCParityNoCas(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines, vc_type, physical_param;
#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING
   WP_U32 i, result;
#endif
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_flexmux_physical_conf physical_configuration;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &interface,&physical_param,&vc_type,&num_of_lines);
   if (res != 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

#if WTI_DUAL_EMPHY
   num_of_lines = 2 * num_of_lines;
#endif

   test_setup.framing_mode = WUFE_FRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = WTI_DS3_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.ds3_e3_framing_type = WT_FLEXMUX_CONNECTION_TYPE_DS3_FRAMED_CPARITY;
   test_setup.vc_type = vc_type;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);

   switch(interface)
   {
      case 0:
         test_setup.transfer_type = WUFE_SDH_TYPE_DS3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
         if(vc_type == 0)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         else if(vc_type == 1)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
         else
         {
            WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-VC3, 1-VC4 ", __LINE__);
            return;
         }
#endif
         break;

      case 1:
         test_setup.transfer_type = WUFE_SONET_TYPE_DS3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }
#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:physical_configuration = WT_FLEXMUX_UFE_412_1_PORT_OC12;
         break;
      case 1:physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3;
         break;
      case 2:physical_configuration = WT_FLEXMUX_UFE_412_2_PORTS_OC3;
         break;
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value, Allowed values are: 0-1xOC12, 1-4xOC3 2-2xOC3 ", __LINE__);
         return;
   }
#endif

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   physical_configuration,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

   WTI_enable_alarms_and_performance_monitoring(interface);

#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING
   
   for (i = 0; i < 12 ; i++)
   {
      if ((result = WPX_FRMR_PM_E3T3_EnableMonitoring(
              0, 0, i*28, WPX_UFE_FRAMER_PDH_E3T3_PM_FEBE)) != WPX_UFE_FRAMER_OK)
      {
         printf("T3 FEBE Monitoring failed on %d with %s\n", i, OMIINO_FRAMER_ErrorCodeToTxt(result));
         break;
      }
      if ((result = WPX_FRMR_PM_E3T3_EnableMonitoring(
              0, 0, i*28, WPX_UFE_FRAMER_PDH_E3T3_PM_BIP)) != WPX_UFE_FRAMER_OK)
      {
         printf("T3 BIP Monitoring failed on %d with %s\n", i, OMIINO_FRAMER_ErrorCodeToTxt(result));
         break;
      }
   }
#endif
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhE3DS3Combined(char *StrPrm)
{
   WP_U32 res, cmd, i, num_of_lines,vc_type, physical_param,num_slots, num_slots_ds3,pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type, transfer_type_ds3, transfer_type_e3;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WTI_flexmux_physical_conf physical_configuration;
   WP_U8 flexmux_mode=0;
#endif

   framing_mode = WUFE_UNFRAMED;
   cas_mode = WUFE_CAS_DISABLE;
   num_of_lines= 336;
   
   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &interface,&physical_param,&vc_type);
   if (res != 4)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   switch(interface)
   {
      case 0:
         transfer_type_ds3 = WUFE_SDH_TYPE_DS3;
         transfer_type_e3 = WUFE_SDH_TYPE_E3;

#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
         if(vc_type == 0)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         else if(vc_type == 1)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
         else
         {
            WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-VC3, 1-VC4 ", __LINE__);
            return;
         }
#endif
         break;
      case 1:
         transfer_type_ds3 = WUFE_SONET_TYPE_DS3;
         transfer_type_e3 = WUFE_SONET_TYPE_E3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }

#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:
         physical_configuration = WT_FLEXMUX_UFE_412_1_PORT_OC12;
         break;
      case 1:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3;
         break;
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value\n"
                                 "Allowed values are: 0-1xOC12, 1-4xOC3", __LINE__);
         return;
   }
#endif

   num_slots_ds3 = WTI_DS3_SLOTS;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX;i<(num_of_lines+WT_FIRST_LINE_INDEX);i++)
   {
      if(i%28!=0)
         continue;
      if ((i/28)%2)
         transfer_type = transfer_type_ds3;
      else
         transfer_type = transfer_type_e3;

      sprintf(temp_buf, "0 %d %d %d",i,transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d",i, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   physical_configuration,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   for (i=0;i<WTI_MAX_NUM_OF_MPLS_FA;++i)
   {
#if WTI_CESOP_MPLS_OVER_ENET
      /* create the l2 router MPLS flow aggregations */
      CLI_F_Tdm2PsnFlowAggEnetHeaderMpls("0 aa aa aa aa aa aa bb bb bb bb bb bb 8847");
      CLI_F_MplsFlowAggPrefixLength("0 18");
      sprintf(temp_buf,
              "0 %x %x %x %x %x %x",
              3*i,
              3*i+1,
              3*i+2,
              0x81000005,
              (0x88470000 | ((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) >> 4)),
              ((((((WTI_MPLS_LABEL) >> 12)  + WTI_MAX_PW + 1 + i) & 0xff) << 28) | 0x00ff0000));
      /*                              [mac destination] [mac source] [vlan] [type] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d %d", 2, 28, 41); /* remark offsets for VLAN priority and PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
#if WTI_CESOP_NATIVE_MPLS_IW
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 0");
      sprintf(temp_buf,
              "0 %x",
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 5); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#else
      /* create the l2 router MPLS flow aggregations */
      CLI_F_MplsFlowAggPrefixLength("0 4");
      sprintf(temp_buf,
              "0 %x %x",
              WTI_MPLSoPPP,
              (((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) << 12) | 0xff));
      /*                              [ppp] [mpls header] */
      CLI_F_MplsFlowAggPrefixHeaderMpls(temp_buf);
      sprintf(temp_buf, "0 %d %d", 1, 13); /* remark offsets for PSN tunnel EXP bits */

      CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
      sprintf(temp_buf, "0 %d", i);
      CLI_F_MplsFlowAggCreate(temp_buf);
#endif /* WTI_CESOP_NATIVE_MPLS_IW */
#endif /* WTI_CESOP_MPLS_OVER_ENET */
   }

   for (i=WT_FIRST_LINE_INDEX;i< (num_of_lines+WT_FIRST_LINE_INDEX) ;i++)
   {
      if(i%28!=0)
         continue;

      if ((i/28)%2)
      {
         WT_UfeLineConfigureAndCreateE3DS3(&the_system->ufe, i, transfer_type_ds3, framing_mode, 
                                           WT_FLEXMUX_CONNECTION_TYPE_DS3_UNFRAMED, cas_mode, 1, NULL);
      }
      else
      {
         WT_UfeLineConfigureAndCreateE3DS3(&the_system->ufe, i, transfer_type_e3, framing_mode, 
                                           WT_FLEXMUX_CONNECTION_TYPE_E3_UNFRAMED, cas_mode, 1, NULL);
      }

      printf("Created line %d, phy %d\n",i,pw_index);

      num_slots = num_slots_ds3;

      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64",i,pw_index);
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64 %d 0-31",i,pw_index,num_slots);
         CLI_F_PwConfigureFramed(temp_buf);
      }
      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 130");

      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", i+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");

      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 32");

      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
      pw_index++;
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhDS3CreateDelete(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines, vc_type, physical_param, iterations, result;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WTI_flexmux_physical_conf physical_configuration;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &interface,&physical_param,&vc_type, &num_of_lines);
   if (res != 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters -- 5 required", __LINE__);
      return;
   }
#if WTI_DUAL_EMPHY
   num_of_lines = 2 * num_of_lines;
#endif

   test_setup.framing_mode = WUFE_UNFRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = WTI_DS3_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.ds3_e3_framing_type = WT_FLEXMUX_CONNECTION_TYPE_DS3_UNFRAMED;
   test_setup.vc_type = vc_type;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);

   switch(interface)
   {
      case 0:
         test_setup.transfer_type = WUFE_SDH_TYPE_DS3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
         if(vc_type == 0)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         else if(vc_type == 1)
            vc_3_4 = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
         else
         {
            WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-VC3, 1-VC4 ", __LINE__);
            return;
         }
#endif
         break;
      case 1:
         test_setup.transfer_type = WUFE_SONET_TYPE_DS3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET", __LINE__);
         return;
   }

#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:
         physical_configuration = WT_FLEXMUX_UFE_412_1_PORT_OC12;
         break;
      case 1:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3;
         break;
      case 2:
         physical_configuration = WT_FLEXMUX_UFE_412_2_PORTS_OC3;
         break;
      case 3:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER;
         break;
      case 4:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE;
         break;
#if WTI_DUAL_EMPHY
         case 5:
         physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_UPI1_MASTER_UPI3_SLAVE;
         break;
#endif
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value\n"
                                 "Allowed values are: 0-1xOC12, 1-4xOC3 2-2xOC3", __LINE__);
         return;
   }
#endif

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SDH mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   physical_configuration,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

   /* starting the remove-recreate sequence. */
   for (iterations=1; iterations<WTI_ITERATIONS_NUM+1; iterations++)
   {
      printf("################ start of iteration %d   ###############\n", iterations);

      /* Remove all PWs: last to first remove */
      sprintf(temp_buf, "0 %d", /*line_delete*/ 0);
      CLI_F_UfeLinePWDelete(temp_buf);

      /* Wait few milisecond before recreate */
      WP_Delay(60000);

      /* recreate lines (if needed) and PWs */
      WT_UfeLinePWCreate(&test_setup, /* line_delete*/ 0);

      printf("Wait few seconds to let the system become stable...\n");
      WP_Delay(DELAY_DURATION_IN_SECONDS * 1000000);
      /* Global statistics check, including reset. */
      result = CLI_F_GlobalStatisticsCheck("0");
      if (result == WT_FAIL)
      {
         WTI_PRINT_ERROR(("\n Test Failed. Iteration number: %d\n", iterations));
         return;
      }
      printf(">>>>>>>>>>>>>>>> end of iteration %d   <<<<<<<<<<<<<<<\n", iterations);
   } /* for iterations */
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhE3CreateDelete(char *StrPrm)
{
   WP_U32 res, cmd, num_of_lines, iterations;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d", &cmd, &interface);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
#if WTI_DUAL_EMPHY
   num_of_lines = 24;
#else
   num_of_lines = 12;
#endif
   test_setup.framing_mode = WUFE_UNFRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = WTI_E3_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.ds3_e3_framing_type = WT_FLEXMUX_CONNECTION_TYPE_E3_UNFRAMED;
   test_setup.vc_type = WTI_FLEXMUX_PHYSICAL_CONF;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);

   switch(interface)
   {
      case 0:
         test_setup.transfer_type = WUFE_SDH_TYPE_E3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         test_setup.transfer_type = WUFE_SONET_TYPE_E3;
#ifdef WT_UFE_FRAMER
         flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   /* create an empty CESOP system with no PW */
   /*Calls UFE system create and enable*/
   WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
   /* Initialize the Flexmux framer in SDH mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

   /* starting the remove-recreate sequence. */
   for (iterations=1; iterations<WTI_ITERATIONS_NUM+1; iterations++)
   {
      printf("################ start of iteration %d   ###############\n", iterations);

      /* Remove all PWs: last to first remove */
      sprintf(temp_buf, "0 %d", /*line_delete*/ 0);
      CLI_F_UfeLinePWDelete(temp_buf);

      /* Wait few milisecond before recreate */
      WP_Delay(60000);

      /* recreate lines (if needed) and PWs */
      WT_UfeLinePWCreate(&test_setup, /* line_delete*/ 0);

      printf("Wait few seconds to let the system become stable...\n");
      WP_Delay(DELAY_DURATION_IN_SECONDS * 1000000);
      /* Global statistics check, including reset. */
//       result = CLI_F_GlobalStatisticsCheck("0");
//       if (result == WT_FAIL)
//       {
//          WTI_PRINT_ERROR(("\n Test Failed. Iteration number: %d\n", iterations));
//          return WT_FAIL;
//       }
      printf(">>>>>>>>>>>>>>>> end of iteration %d   <<<<<<<<<<<<<<<\n", iterations);
   } /* for iterations */
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhDS3EnableDisable(char *StrPrm)
{
   WP_U32 res, cmd, i, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface, option = 4;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &interface, &option);
   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   test_setup.framing_mode = WUFE_UNFRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
#if WTI_DUAL_EMPHY
   test_setup.num_of_lines = 24;
#else
   test_setup.num_of_lines = 12;
#endif
   test_setup.num_slots = WTI_DS3_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.ds3_e3_framing_type = WT_FLEXMUX_CONNECTION_TYPE_DS3_UNFRAMED;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);

   if (option >= 0)
   {
      switch(interface)
      {
         case 0:
            test_setup.transfer_type = WUFE_SDH_TYPE_DS3;
#ifdef WT_UFE_FRAMER
            flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
            break;
         case 1:
            test_setup.transfer_type = WUFE_SONET_TYPE_DS3;
#ifdef WT_UFE_FRAMER
            flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
            break;
         default:
            WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
            return;
      }

      /* allocate CESoP system */
      WTI_SystemAlloc();

      /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
      sprintf(temp_buf, "0 %d",interface);
      CLI_F_UfeFpgaMode(temp_buf);

      /* create an empty CESOP system with no PW */
      /*Calls UFE system create and enable*/
      WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
      /* Initialize the Flexmux framer in SDH mode */
      WTI_FlexmuxInit(WTI_EMPHY_PORT,
                      0,
                      WTI_FLEXMUX_PHYSICAL_CONF,
                      flexmux_mode,
                      vc_3_4,
                      &flex_global_params);

      if (interface < 2)
         WTI_enable_alarms_and_performance_monitoring(interface);
#endif

      WT_UfeMPLS_L2_FA_Create();
      WT_UfeLinePWCreate(&test_setup, 1);
   }

   if (option >= 1)
   {
      printf("Disbling all the phys...\n");
      for (pw_index = 12;pw_index>0;--pw_index)
      {
         WT_UfePhyDisable(&the_system->ufe,pw_index-1 );
      }
#if WTI_DUAL_EMPHY
      for (pw_index = 24;pw_index>12;--pw_index)
      {
         WT_UfePhyDisable(&the_system->second_ufe, pw_index-1);
      }
#endif
   }

   if (option >= 2)
   {
      printf("Enabling all the phys...\n");
      for (pw_index = 0;pw_index<12;++pw_index)
      {
         WT_UfePhyEnable(&the_system->ufe, pw_index);
      }
#if WTI_DUAL_EMPHY
      for (pw_index = 12;pw_index<24;++pw_index)
      {
         WT_UfePhyEnable(&the_system->second_ufe, pw_index);
      }
#endif
   }

   if (option == 3)
   {
      for(i=0;i<10;i++)
      {
         /* Disable all phys*/
         printf("Disbling all the phys...\n");
         for (pw_index = 12;pw_index>0;--pw_index)
         {
            WT_UfePhyDisable(&the_system->ufe,pw_index-1 );
         }
#if WTI_DUAL_EMPHY
         for (pw_index = 24;pw_index>12;--pw_index)
         {
            WT_UfePhyDisable(&the_system->second_ufe, pw_index-1);
         }
#endif

         /* Enable all phys*/
         printf("Enabling all the phys...\n");
         for (pw_index = 0;pw_index<12;++pw_index)
         {
            WT_UfePhyEnable(&the_system->ufe, pw_index);
         }
#if WTI_DUAL_EMPHY
         for (pw_index = 12;pw_index<24;++pw_index)
         {
            WT_UfePhyEnable(&the_system->second_ufe, pw_index);
         }
#endif
      }
   }
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhE3EnableDisable(char *StrPrm)
{
   WP_U32 res, cmd, i, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WP_U32 interface, option = 4;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &interface, &option);
   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   test_setup.framing_mode = WUFE_UNFRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
#if WTI_DUAL_EMPHY
   test_setup.num_of_lines = 24;
#else
   test_setup.num_of_lines = 12;
#endif
   test_setup.num_slots = WTI_E3_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.ds3_e3_framing_type = WT_FLEXMUX_CONNECTION_TYPE_E3_UNFRAMED;
#endif
   strcpy(test_setup.test_name, __FUNCTION__);

   if (option >= 0)
   {
      switch(interface)
      {
         case 0:
            test_setup.transfer_type = WUFE_SDH_TYPE_E3;
#ifdef WT_UFE_FRAMER
            flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
            break;
         case 1:
            test_setup.transfer_type = WUFE_SONET_TYPE_E3;
#ifdef WT_UFE_FRAMER
            flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
            break;
         default:
            WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
            return;
      }

      /* allocate CESoP system */
      WTI_SystemAlloc();

      /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
      sprintf(temp_buf, "0 %d",interface);
      CLI_F_UfeFpgaMode(temp_buf);

      /* create an empty CESOP system with no PW */
      /*Calls UFE system create and enable*/
      WTI_DemoConfigStart();

#ifdef WT_UFE_FRAMER
      /* Initialize the Flexmux framer in SDH mode */
      WTI_FlexmuxInit(WTI_EMPHY_PORT,
                      0,
                      WTI_FLEXMUX_PHYSICAL_CONF,
                      flexmux_mode,
                      vc_3_4,
                      &flex_global_params);

      if (interface < 2)
         WTI_enable_alarms_and_performance_monitoring(interface);
#endif

      WT_UfeMPLS_L2_FA_Create();
      WT_UfeLinePWCreate(&test_setup, 1);
   }

   if (option >= 1)
   {
      printf("Disbling all the phys...\n");
      for (pw_index = 12;pw_index>0;--pw_index)
      {
         WT_UfePhyDisable(&the_system->ufe,pw_index-1 );
      }
#if WTI_DUAL_EMPHY
      for (pw_index = 24;pw_index>12;--pw_index)
      {
         WT_UfePhyDisable(&the_system->second_ufe, pw_index-1);
      }
#endif
   }

   if (option >= 2)
   {
      printf("Enabling all the phys...\n");
      for (pw_index = 0;pw_index<12;++pw_index)
      {
         WT_UfePhyEnable(&the_system->ufe, pw_index);
      }
#if WTI_DUAL_EMPHY
      for (pw_index = 12;pw_index<24;++pw_index)
      {
         WT_UfePhyEnable(&the_system->second_ufe, pw_index);
      }
#endif
   }

   if (option == 3)
   {
      for(i=0;i<10;i++)
      {
         /* Disable all phys*/
         printf("Disbling all the phys...\n");
         for (pw_index = 12;pw_index>0;--pw_index)
         {
            WT_UfePhyDisable(&the_system->ufe,pw_index-1 );
         }
#if WTI_DUAL_EMPHY
         for (pw_index = 24;pw_index>12;--pw_index)
         {
            WT_UfePhyDisable(&the_system->second_ufe, pw_index-1);
         }
#endif

         /* Enable all phys*/
         printf("Enabling all the phys...\n");
         for (pw_index = 0;pw_index<12;++pw_index)
         {
            WT_UfePhyEnable(&the_system->ufe, pw_index);
         }
#if WTI_DUAL_EMPHY
         for (pw_index = 12;pw_index<24;++pw_index)
         {
            WT_UfePhyEnable(&the_system->second_ufe, pw_index);
         }
#endif
      }
   }
}

