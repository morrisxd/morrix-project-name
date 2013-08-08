extern void WTI_TerminateOnError(WP_handle handle, WP_CHAR *s, WP_U32 line_num);
/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************/
/*********************************   SONET/SDH   *********************************/
/*****************************************************************************/
#define WT_FIRST_LINE_INDEX    0
#define WT_MAX_LINE_INDEX      1344
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


#define WT_LINE_INDEX_TO_FIRST_DCO  0
#define WT_LINE_INDEX_TO_SECOND_DCO 1

#define WTI_ITERATIONS_NUM       3
#define WTI_CHECK_PORT_SWITCHING 0
/* Socket loopback mode  */

#ifdef WT_UFE_FRAMER
#define WTI_SOCKET_LOOPBACK_MODE     WUFE_LINE_LOOPBACK_TYPE_NONE 
#else  /* WT_UFE_FRAMER is not define */
#define WTI_SOCKET_LOOPBACK_MODE     WUFE_LINE_LOOPBACK_TYPE_SYSTEM
#endif /* WT_UFE_FRAMER is not define */

#ifdef WT_UFE_FRAMER
#ifndef WTI_FLEXMUX_UTIL_H
#include "wti_flexmux_util.c"
#endif
#endif

#define DELAY_DURATION_IN_SECONDS 3

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
static void WTI_enable_alarms_and_performance_monitoring(int type)
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

// 		CLI_F_MplsFlowAggPrefixLength("0 14");
// 		sprintf(temp_buf,
// 				"0 %x %x %x %x %x %x",
// 				3*i,
// 				3*i+1,
// 				3*i+2,
// 				(0x88470000 | ((((WTI_MPLS_LABEL) >> 12) + WTI_MAX_PW + 1 + i) >> 4)),
//                                 ((((((WTI_MPLS_LABEL) >> 12)  + WTI_MAX_PW + 1 + i) & 0xff) << 28) | 0x0eff0000),
//                                 0);

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




#if 0
           /* This feature demonstartes the remarking feature of the PW priority (PW priority which is set by the user 
              in the PSN to TDM FA is copied to EXP and VLAN priority fields. This feature is disabled in the default 
              configuration */
           sprintf(temp_buf, "0 %d %d %d", 2, 28, 41); /* remark offsets for VLAN priority and PSN tunnel EXP bits */

           CLI_F_MplsFlowAggRemarkingOffsets(temp_buf);
#endif
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
	WP_U32 line_index, pw_index = 0, num_of_lines = test_setup->num_of_lines, i;
	WP_CHAR temp_buf[WTI_MAX_STRING_SIZE], setup_str[WTI_MAX_STRING_SIZE];
	WP_U32 output_mpls_fa_index;
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
              
           default:
              WTI_PrintError("Invalid interface value, Allowed values are: E1, T1, with SDH or SONET ", __FILE__, __LINE__);
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
           for (line_index = WT_FIRST_LINE_INDEX; line_index < num_of_lines + WT_FIRST_LINE_INDEX; line_index++)
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
                 if ((line_index % WTI_T1_LINES_PER_SPE) >= WTI_E1_LINES_PER_SPE)
                 {
                    num_of_lines++;
                    continue;
                 }
                 
#ifdef WT_UFE_FRAMER
                 if ((WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION == WTI_FLEXMUX_PHYSICAL_CONF) ||
                     (WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION == physical_configuration))
                 {
                    if((line_index >= 336) && (line_index < 672))
                    {
                       num_of_lines++;
                       continue;
                    }
                 }
#endif
                 
                 sprintf(temp_buf, "0 %d %d %d",line_index, test_setup->transfer_type, test_setup->framing_mode);
                 CLI_F_UfeTransferMode(temp_buf);
                 sprintf(temp_buf, "0 %d %d", line_index, test_setup->cas_mode);
                 CLI_F_UfeCasMode(temp_buf);
                 
                 /* UFE line configuration, creation and enabling */
                 
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
              else /* T1 */
              {
#ifdef WT_UFE_FRAMER
                 if (WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION == WTI_FLEXMUX_PHYSICAL_CONF)
                 {
                    if((line_index >= 336) && (line_index < 672))
                    {
                       num_of_lines++;
                       continue;
                    }
                 }
#endif
                 sprintf(temp_buf, "0 %d %d %d",line_index, test_setup->transfer_type, test_setup->framing_mode);
                 CLI_F_UfeTransferMode(temp_buf);
                 sprintf(temp_buf, "0 %d %d",line_index, test_setup->cas_mode);
                 CLI_F_UfeCasMode(temp_buf);
                 
                 /* UFE line configuration, creation and enabling */
                 
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
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
              /* Set the defult PSN2TDM clock recoevry paramters */
              WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, test_setup->line_cr_operation_mode[line_index]);
#endif
              
              /* UFE line configuration, creation and enabling */
              
              WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
           }
	}
        
        
	/*******************************************************************************************/
	/*                                     UFE PW config                               */
	/*******************************************************************************************/
	num_of_lines = test_setup->num_of_lines;
        
	for (line_index = WT_FIRST_LINE_INDEX; line_index < (WT_FIRST_LINE_INDEX + num_of_lines); line_index++)
	{
	   if (test_setup->transfer_type == WUFE_SDH_TYPE_E1 || test_setup->transfer_type == WUFE_SONET_TYPE_E1)
	   {
	      if ((line_index % WTI_T1_LINES_PER_SPE) >= WTI_E1_LINES_PER_SPE)
	      {
	         num_of_lines++;
	         continue;
	      }
	   }
#ifdef WT_UFE_FRAMER
           if ((WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION == WTI_FLEXMUX_PHYSICAL_CONF) ||
               (WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION == physical_configuration))
           {
              if((line_index >= 336) && (line_index < 672))
              {
                 num_of_lines++;
                 continue;
              }
           }
#endif 
           
	   for (i=0; i < test_setup->pw_per_line; i++)
	   {
	      /*UFE phy and pw configuration*/
	      if (test_setup->framing_mode == WUFE_UNFRAMED)
	      {
	         if (test_setup->transfer_type == WUFE_SDH_TYPE_E1 || test_setup->transfer_type == WUFE_SONET_TYPE_E1)
	         {
	            sprintf(temp_buf, "0 %d %d 64 64", line_index, pw_index);
	         }
	         else
	         {
	            sprintf(temp_buf, "0 %d %d 48 48", line_index, pw_index);
	         }
	         CLI_F_PwConfigureUnframed(temp_buf);
	      }
	      else /* WUFE_FRAMED */
	      {
	         if (test_setup->transfer_type == WUFE_SDH_TYPE_E1 || test_setup->transfer_type == WUFE_SONET_TYPE_E1)
	         {
	            if(test_setup->num_slots > 1)
	            {
                       if (30 == test_setup->num_slots)
                       {                          
                          sprintf(temp_buf, "0 %d %d 30 30 %d 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31", line_index,
                                  pw_index, test_setup->num_slots);
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
              
	      sprintf(temp_buf, "0 %d", test_setup->num_slots);
	      CLI_DefaultIwPwe3CesConfig(temp_buf);
              
	      /* PWE3 channel configuration */
	      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
	      if (test_setup->transfer_type == WUFE_SDH_TYPE_E1 || test_setup->transfer_type == WUFE_SONET_TYPE_E1)
	      {
                 if(test_setup->num_slots == 30)
                 {
                    CLI_F_Pwe3ChannelRxBufferSize("0 120");
                 }
	         else if(test_setup->num_slots > 1)
	         {
	            CLI_F_Pwe3ChannelRxBufferSize("0 128");
	         }
                 
	         else //single slot
	         {
	            CLI_F_Pwe3ChannelRxBufferSize("0 40");
	         }
	      }
	      else /* T1 */
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
              
	      /**************** TDM --> PSN IW *****************/
	      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
	      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d", line_index + 1);
	      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
	      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
	      CLI_F_Tdm2PsnFlowAggRtpHeader("0 80 78 0 0 deadbeef");
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
	      CLI_F_Psn2TdmTxBinWindowThreshold("0 8");
	      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 8");
	      CLI_F_Psn2TdmTxBinLOPSDetection("0 1 6 4");
              
#if WTI_XGI_MODE
      output_mpls_fa_index = 0;
#else
#if (WTI_FLEXMUX_PHYSICAL_CONF == WT_FLEXMUX_UFE_448_4_PORTS)
      output_mpls_fa_index = (pw_index/336);
#else
      output_mpls_fa_index = (pw_index/126);
#endif
#endif
            
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
#if WTI_CESOP_TO_MPLS_FA_CASCADING
	      if ((pw_index % test_setup->pw_per_line) == test_setup->recovered_pw)
	      {
	         sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,output_mpls_fa_index,pw_index);
	      }
	      else
	      {
	         sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,output_mpls_fa_index,pw_index);
	      }
              
	      CLI_F_PwCreateCascaded(temp_buf);
#else
	      if ((pw_index % test_setup->pw_per_line) == test_setup->recovered_pw)
	      {
	         sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,output_mpls_fa_index,pw_index);
	      }
	      else
	      {
	         sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,output_mpls_fa_index,pw_index);
	      }
              
	      CLI_F_PwCreateCascaded(temp_buf);
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
              output_mpls_fa_index = (pw_index%WTI_MAX_NUM_OF_MPLS_FA);
              
	      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,output_mpls_fa_index,pw_index);
              
	      CLI_F_PwCreateCascaded(temp_buf);
#else /* not WTI_CESOP_TO_MPLS_FA_CASCADING */
	      sprintf(temp_buf, "0 %d %d %d %d", 0,0,0,pw_index);
	      CLI_F_PwCreate(temp_buf);
#endif  /* WTI_CESOP_TO_MPLS_FA_CASCADING */
              
	      sprintf(temp_buf, "0 %d 0",pw_index);
	      CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
              the_system->pw[pw_index].line_index = line_index;
	      pw_index++;
	   }
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
   
   for (pw_index = total_number_of_pw ;pw_index >0; --pw_index)
   {
      sprintf(temp_buf, "0 %d 0",pw_index-1);
      CLI_F_PwDisable(temp_buf);
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
   WP_U32 res, cmd, i,j, num_of_lines, num_slots, pw_index=0,vc_type, physical_param;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
   WP_U32  output_mpls_fa_index;
   
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_flexmux_physical_conf physical_configuration;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
   WP_boolean is_e1 = WP_TRUE;
#endif

   framing_mode = WUFE_UNFRAMED;
   cas_mode = WUFE_CAS_DISABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &interface,&physical_param,&vc_type, &num_of_lines);

   if (res != 5)
   {
      WTI_TerminatePrintError("Invalid number of parameters -- 5 required", __LINE__);
      return;
   }

   switch(interface)
   {
      case 0:
         transfer_type = WUFE_SDH_TYPE_E1;
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

#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:
         physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12;
         break;
      case 1:
         physical_configuration = WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION;
         break;
      case 2:
         physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12_CARD_PROTECTION_MASTER;
         break;
      case 3:
         physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12_CARD_PROTECTION_SLAVE;
         break;
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value\n"
                                 "Allowed values are: 0-4xOC12, 1-8xOC3", __LINE__);
         return;
   }
#endif
   num_slots = WTI_E1_SLOTS;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i = j = WT_FIRST_LINE_INDEX; j < (WT_FIRST_LINE_INDEX + num_of_lines); ++i)
   {
      if(i%28>20)
      {
         continue;
      }
#ifdef WT_UFE_FRAMER
      if (WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION == physical_configuration)
      {
         
         if((i >= 336) && (i < 672))
         {
            continue;
         }
      }
#endif      
      j++;

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
//   return ;
   
   for (i = j = WT_FIRST_LINE_INDEX;j < (WT_FIRST_LINE_INDEX + num_of_lines); ++i)
   {

      if ((i%28)>20)
      {
         continue;      
      }
#ifdef WT_UFE_FRAMER 
      if (WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION == physical_configuration)
      {
         
         if( (i >= 336) && (i < 672))
         {
            continue;
         }
      }
#endif
      if (i%100 == 0)
         printf("i = %d\n", i);
   
      j++;
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1, NULL);
#if USE_UFE
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
      CLI_F_Pwe3ChannelRxBufferSize("0 512");
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
#if WTI_XGI_MODE
      output_mpls_fa_index = 0;
#else
#if (WTI_FLEXMUX_PHYSICAL_CONF == WT_FLEXMUX_UFE_448_4_PORTS)
      output_mpls_fa_index = (pw_index/336);
#else
      output_mpls_fa_index = (pw_index/126);
#endif
#endif
      
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,output_mpls_fa_index,pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
      pw_index++;
#endif
   }

#ifdef WT_UFE_FRAMER

   /* interface == 0 ==> SDH; interface == 1 ==> SONET */
   if (interface < 2)
   {
      WTI_enable_performance_monitoring_points(interface, is_e1, vc_type, test_setup.framing_mode);
   }

#endif

#ifdef WT_UFE_FRAMER
#if WTI_CHECK_PORT_SWITCHING

   char ch;
   WP_U32 line_port_id, line_active=1;;
   WP_U8 num_of_ports=0;


   if (physical_configuration == WT_FLEXMUX_UFE_448_4_PORTS_OC12                        ||
       physical_configuration == WT_FLEXMUX_UFE_448_4_PORTS_OC12_CARD_PROTECTION_MASTER ||
       physical_configuration == WT_FLEXMUX_UFE_448_4_PORTS_OC12_CARD_PROTECTION_SLAVE)
   {
      num_of_ports = 4;

   }
   else if (physical_configuration == WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION)
   {
      num_of_ports = 8;

   }

   printf("****************** Port switching check **********************\n");
   for (line_port_id = 0; line_port_id<num_of_ports; line_port_id+=1)
   {

      printf("\n\nPort switching check for line port %d\n",line_port_id);
      printf("line active is %d\n",line_active);
      printf("Press 'q' in order to proceed to the next line\n");

      do{
         printf("a - ForceA, b - ForceB\n");
         ch = getchar();
         if (ch == 'a' )
         {
            WTI_FlexmuxForceA(line_port_id);
            line_active = 1;
            printf("line active is %d\n",line_active);
         }
         else if (ch == 'b')
         {
            WTI_FlexmuxForceB(line_port_id);
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
   WP_U32 res, cmd, i,j, num_of_lines, num_slots,pw_index=0,vc_type, physical_param;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_flexmux_physical_conf physical_configuration;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
   WP_boolean is_e1 = WP_TRUE;
#endif

   framing_mode = WUFE_FRAMED;
   cas_mode = WUFE_CAS_DISABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &interface,&physical_param,&vc_type,&num_of_lines);
   if (res != 5)
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
#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:
         physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12;
         break;
      case 1:
         physical_configuration = WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION;
         break;
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value\n"
                                 "Allowed values are: 0-4xOC12, 1-8xOC3", __LINE__);
         return;
   }
#endif
   num_slots = WTI_E1_SLOTS;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i = j = WT_FIRST_LINE_INDEX; j < (WT_FIRST_LINE_INDEX + num_of_lines); ++i)
   {
      if((i%28>20))
      {
         continue;
      }
#ifdef WT_UFE_FRAMER
      if (WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION == physical_configuration)
      {
         
         if((i >= 336) && (i < 672))
         {
            continue;
         }
      }
#endif

      j++;
      
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

   for (i = j = WT_FIRST_LINE_INDEX; j < (WT_FIRST_LINE_INDEX + num_of_lines); ++i)
   {
      if((i%28>20))
      {
         continue;
      }
#ifdef WT_UFE_FRAMER
      if (WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION == physical_configuration)
      {
         
         if( (i >= 336) && (i < 672))
         {
            continue;
         }
      }
#endif

      j++;

      if (i%100 == 0)
         printf("i = %d\n",i);

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
//         sprintf(temp_buf, "0 %d %d 60 60 %d 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31",i,pw_index,num_slots);
         sprintf(temp_buf, "0 %d %d 64 64 %d 0-31",i,pw_index,num_slots);
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

      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
      pw_index++;
   }

#ifdef WT_UFE_FRAMER

   /* interface == 0 ==> SDH; interface == 1 ==> SONET */
   if (interface < 2)
   {
      WTI_enable_performance_monitoring_points(interface, is_e1, vc_type, test_setup.framing_mode);
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
void CLI_F_SonetSdhE1FramedWithCas(char *StrPrm)
{
   WP_U32 res, cmd, i, num_of_lines, num_slots,pw_index = 0,vc_type ,physical_param;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_flexmux_physical_conf physical_configuration;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   framing_mode = WUFE_FRAMED;
   cas_mode = WUFE_CAS_ENABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &interface,&physical_param,&vc_type,&num_of_lines);
   if (res != 5)
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
#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:
         physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12;
         break;
      case 1:
         physical_configuration = WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION;
         break;
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value\n"
                                 "Allowed values are: 0-4xOC12, 1-8xOC3", __LINE__);
         return;
   }
#endif
   num_slots = WTI_E1_SLOTS;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
   {
      if((i%28>20))
      {
         num_of_lines++;
         continue;
      }
#ifdef WT_UFE_FRAMER
      if (WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION == physical_configuration)
      {
         
         if((i >= 336) && (i < 672))
         {
            continue;
         }
      }
#endif
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
                   WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC,
                   &flex_global_params);
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

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
   {
      if((i%28>20))
      {
         continue;
      }
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
         sprintf(temp_buf, "0 %d %d 64 64 %d 0-31",i,pw_index,num_slots);
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

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
   }
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
   WP_U32 res, cmd, i, j, num_of_lines, num_slots,vc_type, physical_param,pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_flexmux_physical_conf physical_configuration;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
   WP_boolean is_e1 = WP_FALSE;
#endif

   framing_mode = WUFE_UNFRAMED;
   cas_mode = WUFE_CAS_DISABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &interface,&physical_param,&vc_type,&num_of_lines);
   if (res != 5)
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


#ifdef WT_UFE_FRAMER

   switch(physical_param)
   {
      case 0:
         physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12;
         break;
      case 1:
         physical_configuration = WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION;
         break;
      case 2:
         physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12_CARD_PROTECTION_MASTER;
         break;
      case 3:
         physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12_CARD_PROTECTION_SLAVE;
         break;
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value\n"
                                 "Allowed values are: 0-4xOC12, 1-8xOC3", __LINE__);
         return;

   }
#endif
   num_slots = WTI_T1_SLOTS;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i = j = WT_FIRST_LINE_INDEX;j < (WT_FIRST_LINE_INDEX + num_of_lines); ++i)
   {
#ifdef WT_UFE_FRAMER
      if (WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION == physical_configuration)
      {
         
         if((i >= 336) && (i < 672))
         {
            continue;
         }
      }
#endif      
      j++;
      
      sprintf(temp_buf, "0 %d %d %d", i, transfer_type, framing_mode);
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

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (i = j = WT_FIRST_LINE_INDEX;j < (WT_FIRST_LINE_INDEX + num_of_lines); ++i)
   {

#ifdef WT_UFE_FRAMER
      if (WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION == physical_configuration)
      {
         
         if( (i >= 336) && (i < 672))
         {
            continue;
         }
      }
#endif
      j++;
      if (i%100 == 0)
         printf("i = %d\n", i);      

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1, NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WTI_SOCKET_LOOPBACK_MODE);

      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64",i,pw_index);
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d 48 48 %d 0-23",i,pw_index,num_slots);
         CLI_F_PwConfigureFramed(temp_buf);
      }
      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 576");

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
   {
      WTI_enable_performance_monitoring_points(interface, is_e1, vc_type, test_setup.framing_mode);
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
void CLI_F_SonetSdhT1FramedNoCas(char *StrPrm)
{
   WP_U32 res, cmd, i,j, num_of_lines, num_slots,vc_type, physical_param,pw_index=0;
#if WTI_TWO_CORES
   WP_U32 num_of_lines_core0 = 0,num_of_lines_core1 = 0;
#endif  
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_flexmux_physical_conf physical_configuration;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
   WP_boolean is_e1 = WP_FALSE;
#endif

   framing_mode = WUFE_FRAMED;
   cas_mode = WUFE_CAS_DISABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &interface,&physical_param,&vc_type,&num_of_lines);
   if (res != 5)
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
#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:
         physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12;
         break;
      case 1:
         physical_configuration = WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION;
         break;
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value\n"
                                 "Allowed values are: 0-4xOC12, 1-8xOC3", __LINE__);
         return;
   }
#endif
   num_slots = WTI_T1_SLOTS;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);
#define WTI_TWO_CORES 0

#if WTI_TWO_CORES
   num_of_lines_core0 = num_of_lines/2;
   num_of_lines_core1 = num_of_lines - num_of_lines_core0;
   printf("num_of_lines_core0 %d, num_of_lines_core1 %d\n", num_of_lines_core0, num_of_lines_core1);
#endif

#if WTI_TWO_CORES
   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines_core0); i++)
#else
   for (i = j = WT_FIRST_LINE_INDEX;j < (WT_FIRST_LINE_INDEX + num_of_lines); ++i)
#endif
   {
#ifdef WT_UFE_FRAMER
      if (WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION == physical_configuration)
      {
         
         if( (i >= 336) && (i < 672))
         {
            continue;
         }
      }
#endif
      j++;
      sprintf(temp_buf, "0 %d %d %d",i,transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d",i, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }


#if WTI_TWO_CORES
   for (i=672; i<(672+num_of_lines_core1); i++)
   {
      sprintf(temp_buf, "0 %d %d %d",i,transfer_type, framing_mode);
      CLI_F_UfeTransferMode(temp_buf);
      sprintf(temp_buf, "0 %d %d",i, cas_mode);
      CLI_F_UfeCasMode(temp_buf);
   }
#endif

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
   
   

#if WTI_TWO_CORES   
   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines_core0); i++)
#else
   for (i = j = WT_FIRST_LINE_INDEX;j < (WT_FIRST_LINE_INDEX + num_of_lines); ++i)   
#endif
   {
#ifdef WT_UFE_FRAMER
      if (WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION == physical_configuration)
      {
         
         if( (i >= 336) && (i < 672))
         {
            continue;
         }
      }

      
#endif
      j++;
      if (i%100 == 0)
         printf("i = %d\n",i);

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1,NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WTI_SOCKET_LOOPBACK_MODE);

      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 48 48",i,pw_index);
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d 48 48 %d 0-23",i,pw_index,num_slots);
         CLI_F_PwConfigureFramed(temp_buf);
      }
      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 576");

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

#if WTI_TWO_CORES
   printf("stat configuring core 1...\n");
   
   for (i=672; i<(672+num_of_lines_core1); i++)
   {

      if (i%100 == 0)
         printf("i = %d\n",i);

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1,NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WTI_SOCKET_LOOPBACK_MODE);

      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 48 48",i,pw_index);
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d 48 48 %d 0-23",i,pw_index,num_slots);
         CLI_F_PwConfigureFramed(temp_buf);
      }
      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 576");

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
#endif

#ifdef WT_UFE_FRAMER

   /* interface == 0 ==> SDH; interface == 1 ==> SONET */
   if (interface < 2)
   {
      WTI_enable_performance_monitoring_points(interface, is_e1, vc_type, test_setup.framing_mode);
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
void CLI_F_SonetSdhT1FramedWithCas(char *StrPrm)
{
   WP_U32 res, cmd, i, num_of_lines, num_slots,vc_type, physical_param;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_flexmux_physical_conf physical_configuration;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   framing_mode = WUFE_FRAMED;
   cas_mode = WUFE_CAS_ENABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &interface,&physical_param,&vc_type,&num_of_lines);
   if (res != 5)
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
#ifdef WT_UFE_FRAMER
   switch(physical_param)
   {
      case 0:
         physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12;
         break;
      case 1:
         physical_configuration = WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION;
         break;
      default:
         WTI_TerminatePrintError("Invalid physical_configuration value\n"
                                 "Allowed values are: 0-4xOC12, 1-8xOC3", __LINE__);
         return;
   }
#endif
   num_slots = WTI_T1_SLOTS;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
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
                   physical_configuration,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);
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

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
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
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_SonetSdhE1FramedNoCas_2016_PWs_16xDS0_per_PW(char *StrPrm)
{
   WP_U32 res, cmd;
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
   res = sscanf(StrPrm, "%d %d ", &cmd, &interface);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   test_setup.framing_mode = WUFE_FRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = 1008;
   test_setup.num_slots = 16;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 2;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.vc_type = vc_3_4;
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

#ifdef WT_UFE_FRAMER
   physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12;
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

}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/

void CLI_F_SonetSdhT1FramedNoCas_2688_PWs_12xDS0_per_PW(char *StrPrm)
{
   WP_U32 res, cmd;
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
   res = sscanf(StrPrm, "%d %d ", &cmd, &interface);
   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   test_setup.framing_mode = WUFE_FRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = WT_MAX_LINE_INDEX - WT_FIRST_LINE_INDEX;
   test_setup.num_slots = 12;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 2;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.vc_type = vc_3_4;
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

#ifdef WT_UFE_FRAMER
   physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12;
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
   WP_U32 res, cmd, i, num_of_lines, num_slots, num_slots_e1, num_slots_t1, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type, transfer_type_e1, transfer_type_t1;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_flexmux_physical_conf physical_configuration;
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
#ifdef WT_UFE_FRAMER
   physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12;
#endif
   num_slots_e1 = WTI_E1_SLOTS;
   num_slots_t1 = WTI_T1_SLOTS;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
   {
      if (i<672)
      {
         if(i%28>20)
         {
            continue;
         }
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

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
   {
      if (i<672)
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

      printf("i %d, pw_index %d\n", i, pw_index);
      
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1T1(&the_system->ufe, i,transfer_type, framing_mode, cas_mode, 1, NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WTI_SOCKET_LOOPBACK_MODE);

      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d %d %d", i, pw_index, (2 * num_slots), (2 * num_slots));
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d %d %d %d 0-31", i, pw_index, (2 * num_slots), (2 * num_slots),  num_slots);
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

      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA), pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0", pw_index);
      CLI_F_PwEnable(temp_buf);
      pw_index++;
   }
}

void CLI_F_SonetSdh1008E1_1344T1_1008E1(char *StrPrm)
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

   num_of_lines = 1008;
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

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);
#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);

   /* Check the WP statistics */
   WPL_Delay(2000000);
   printf("CLI_F_Pwe3StatisticsClear\n");
   sprintf(temp_buf, "7");
   CLI_F_Pwe3StatisticsClear(temp_buf);

   printf("CLI_F_Pwe3StatisticsCheck\n");
   sprintf(temp_buf, "8 0 1008 1");
   CLI_F_Pwe3StatisticsCheck(temp_buf);

   /* Delete the phys lins devices and channels */
   printf("finish create the E1 lines,check the data. press any key to continue\n");
   getchar();

   printf("start delete all E1 phys and lines\n");
   /* Remove all PWs: last to first remove */
   sprintf(temp_buf, "0 %d", /*line_delete*/ 1);
   CLI_F_UfeLinePWDelete(temp_buf);

   printf("\nstart create  T1 phys and lines...\n");

   num_of_lines = 1344;
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
   sprintf(temp_buf, "8 0 1344 1");
   CLI_F_Pwe3StatisticsCheck(temp_buf);

   printf("finish create the T1 lines, check the data, press any key to continue\n");
   getchar();

   printf("start delete all T1 phys and lines\n");
   /* Remove all PWs: last to first remove */
   sprintf(temp_buf, "0 %d", /*line_delete*/ 1);
   CLI_F_UfeLinePWDelete(temp_buf);

   printf("\nstart create  E1 phys and lines...\n");

   num_of_lines = 1008;
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
   sprintf(temp_buf, "8 0 1008 1");
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
   WP_U32 res, cmd, i, num_of_lines, num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
   WUFE_idle_pattern new_idle_pattern;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
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
#ifdef WT_UFE_FRAMER
         physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12;
#endif
   num_of_lines = WT_MAX_LINE_INDEX;
   num_slots = WTI_E1_SLOTS;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i = WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
   {
      if((i%28)>20)
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
                   physical_configuration,
                   flexmux_mode,
                   vc_3_4,
                   &flex_global_params);

   if (interface < 2)
      WTI_enable_alarms_and_performance_monitoring(interface);

   /* Unmask FPGA interrupt sources to allow serial to interrupt us */
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   /* reconfiguring the idle data pattern and the idle cas pattern */
   printf("reconfiguring the idle data pattern and the idle cas pattern...\n");
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

   for (i = WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
   {
      if((i%28)>20)
      {
         continue;
      }

      /* UFE line configuration, creation and enabling */

      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1, NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, /*WUFE_LINE_LOOPBACK_TYPE_SYSTEM*/ WTI_SOCKET_LOOPBACK_MODE);
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
   /* This scenario is not available, because it is using the cas feature which is not availabe, yet */
#if 0
   WP_U32 res, cmd, i, num_of_lines, num_slots ,pw_index = 0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
   WUFE_line_sonet_reconfig new_sonet_config;
   WUFE_line_sdh_reconfig new_sdh_config;
#ifdef WT_UFE_FRAMER
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

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
   {

      if((i%28)>20)
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
   /* Initialize the Flexmux framer in SONET mode */
   WTI_FlexmuxInit(WTI_EMPHY_PORT,
                   0,
                   WTI_FLEXMUX_PHYSICAL_CONF,
                   flexmux_mode,
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

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
   {
      if((i%28)>20)
      {
         continue;
      }

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
#endif
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
   WTI_flexmux_physical_conf physical_configuration;
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
#ifdef WT_UFE_FRAMER
   physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12;
#endif
   num_of_lines = WT_MAX_LINE_INDEX;
   num_slots = WTI_E1_SLOTS;

   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
   {
      if((i%28)>20)
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

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
   {

      if((i%28)>20)
      {
         continue;
      }
      if (i%100 == 0)
         printf("i = %d\n", i);

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
#if 1
      memset(&new_phy_cfg, 0xff, sizeof(WUFE_phy));
      new_phy_cfg.rx_slot_group.n_slots = 8;
      for (j=0;j<8;j++)
      {
         new_phy_cfg.rx_slot_group.slot_arr[j] = j;
      }

      /* Disable the phy before phy modifing*/
      WT_UfePhyDisable(&the_system->ufe, pw_index);

      WT_UfePhySlotsModify(&the_system->ufe, pw_index , &new_phy_cfg);

      /* Enable the phy after phy modifing*/

      WT_UfePhyEnable(&the_system->ufe, pw_index);
#endif
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
#if 0//yacov

   WP_U32 res, cmd, i,j, num_of_lines, num_slots,pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WUFE_line_clock_rec_params line_cr_params;
#endif
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
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
#ifdef WT_UFE_FRAMER
   physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12;
#endif
   num_of_lines = WT_MAX_LINE_INDEX;
   num_slots = WTI_T1_SLOTS;


   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
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

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   for (i=0; i<num_of_lines; i++)
   {
      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(i, global_cr_operation_mode);
   }
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
   {

      if (i%100 == 0)
         printf("i = %d\n",i);
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      if(i == 0 || i == 1)
      {
         /* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
         the_system->clock_rec[i].physical_clock_enable = WP_TRUE;
         the_system->clock_rec[i].physical_clock_id = 0;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, i); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);
#else
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */


      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WTI_SOCKET_LOOPBACK_MODE);

      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64",i,i);
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
      CLI_F_Pwe3ChannelRxBufferSize("0 576");

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
   }


#if 1
   WPL_Delay(2000000);
   for (j=0;j<WTI_ITERATIONS_NUM;j++)
   {

      /* Check the WP statistics */
      printf("CLI_F_Pwe3StatisticsClear\n");
      sprintf(temp_buf, "7");
      CLI_F_Pwe3StatisticsClear(temp_buf);
      WPL_Delay(2000000);
      printf("CLI_F_Pwe3StatisticsCheck\n");
      sprintf(temp_buf, "8 0 1344 1");
      CLI_F_Pwe3StatisticsCheck(temp_buf);

      printf("finish create the T1 lines,check the data. press any key to continue\n");
      getchar();
      printf("Delete the phys lins devices and channels...\n");

      /* Delete the phys lins devices and channels */
      for (i = WT_MAX_LINE_INDEX;i>0; --i)
      {
         pw_index = i-1;

         /* Disabling the WP channels*/
         sprintf(temp_buf, "0 %d",pw_index);
         CLI_F_PwDisable(temp_buf);
         /* Delete channels,devices,phys */
         CLI_F_PwDelete(temp_buf);

         /* Delete UFE lines*/
         WT_UfeLineDelete(&the_system->ufe, pw_index);
      }

      /* Delay between the deletion and creation*/
      WPL_Delay(2000000);

      printf("Create the phys lins devices and channels...\n");

      for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
      {

         if (i%100 == 0)
            printf("i = %d\n",i);
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
            sprintf(temp_buf, "0 %d %d 48 48 %d 0-23",i,i,num_slots);
            CLI_F_PwConfigureFramed(temp_buf);
         }
         sprintf(temp_buf, "0 %d",num_slots);
         CLI_DefaultIwPwe3CesConfig(temp_buf);

         /* PWE3 channel configuration */
         CLI_F_Pwe3ChannelJitterBufferSize("0 32");
         CLI_F_Pwe3ChannelRxBufferSize("0 576");

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
      }

   }

#endif
#else//yacov
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
   res = sscanf(StrPrm, "%d %d %d", &cmd, &interface, &num_of_lines);
   if (res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   //num_of_lines = 1344;
   test_setup.framing_mode = WUFE_UNFRAMED;
   test_setup.cas_mode = WUFE_CAS_DISABLE;
   test_setup.num_of_lines = num_of_lines;
   test_setup.num_slots = WTI_T1_SLOTS;
   test_setup.first_slot_id = 0;
   test_setup.pw_per_line = 1;
   test_setup.recovered_pw = 0;
#ifdef WT_UFE_FRAMER
   test_setup.vc_type = vc_3_4;
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
   WPX_Ufe412CpldInterruptMaskSet(0, SERIAL_X_CPLD_INTERRUPT_MASK);

#endif

   WT_UfeMPLS_L2_FA_Create();
   WT_UfeLinePWCreate(&test_setup, 1);


#if 1//me     
   /* starting the remove-recreate sequence. */
   for (iterations=1; iterations<2/*WTI_ITERATIONS_NUM+1*/; iterations++)
   {
   WPL_Delay(3000000);
   
      /* Check the WP statistics */
      printf("CLI_F_Pwe3StatisticsClear\n");
      sprintf(temp_buf, "7");
      CLI_F_Pwe3StatisticsClear(temp_buf);
      WPL_Delay(2000000);
      printf("CLI_F_Pwe3StatisticsCheck\n");
      sprintf(temp_buf, "8 0 %d 1", num_of_lines);
      CLI_F_Pwe3StatisticsCheck(temp_buf);
      getchar();

      printf("################ start of iteration %d   ###############\n", iterations);

      /* Remove all PWs: last to first remove */
      sprintf(temp_buf, "0 %d", /*line_delete*/ 0);
      CLI_F_UfeLinePWDelete(temp_buf);

      /* Wait few milisecond before recreate */
      WPL_Delay(60000);

      /* recreate lines (if needed) and PWs */
      WT_UfeLinePWCreate(&test_setup, /* line_delete*/ 0);

      printf("Wait few seconds to let the system become stable...\n");
      WPL_Delay(DELAY_DURATION_IN_SECONDS * 1000000);
      /* Global statistics check, including reset. */
//       result = CLI_F_GlobalStatisticsCheck("0");
//       if (result == WT_FAIL)
//       {
//          WTI_PRINT_ERROR(("\n Test Failed. Iteration number: %d\n", iterations));
//          return WT_FAIL;
//       }
      printf(">>>>>>>>>>>>>>>> end of iteration %d   <<<<<<<<<<<<<<<\n", iterations);
   } /* for iterations */
#endif//me
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
#endif//yacov
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
   WP_U32 interface, pw_index=0;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
   WTI_Flexmux_global_setup flex_global_params;
   WTI_flexmux_physical_conf physical_configuration;
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


#ifdef WT_UFE_FRAMER
   physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12;
#endif
   num_of_lines = WT_MAX_LINE_INDEX;
   num_slots = WTI_T1_SLOTS;


   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
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

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
   {
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1,NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WTI_SOCKET_LOOPBACK_MODE);

      /*UFE phy and pw configuration*/
      if (framing_mode == WUFE_UNFRAMED)
      {
         sprintf(temp_buf, "0 %d %d 64 64",i, pw_index);
         CLI_F_PwConfigureUnframed(temp_buf);
      }
      else if (framing_mode == WUFE_FRAMED)
      {
         sprintf(temp_buf, "0 %d %d 48 48 %d 0-23",i, pw_index,num_slots);
         CLI_F_PwConfigureFramed(temp_buf);
      }
      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 576");

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

      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA), pw_index);
      CLI_F_PwCreateCascaded(temp_buf);
      sprintf(temp_buf, "0 %d 0", pw_index);
      CLI_F_PwEnable(temp_buf);

      /*disabling and enabling the phy and the line */
      for (j=0;j<WTI_ITERATIONS_NUM;j++)
      {
         if (i%100 == 0)
            printf("Disabling and enabling  phy %d and line %d\n",i,i);

         WT_UfePhyDisable(&the_system->ufe, pw_index);
         WPL_Delay(2000);
         WT_UfePhyEnable(&the_system->ufe, pw_index);
      }
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

void CLI_F_SonetSdhE1EnableDisable(char *StrPrm)
{
   WP_U32 res, cmd, i, num_of_lines, num_slots,j,pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
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
#ifdef WT_UFE_FRAMER
   physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12;
#endif
   num_of_lines = WT_MAX_LINE_INDEX;
   num_slots = WTI_E1_SLOTS;



   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i = WT_FIRST_LINE_INDEX; i < (num_of_lines+WT_FIRST_LINE_INDEX); i++)
   {
      if((i%28)>20)
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

   for (i=WT_FIRST_LINE_INDEX; i<(num_of_lines+WT_FIRST_LINE_INDEX); i++)
   {
      if((i%28)>20)
      {
         continue;
      }

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

      /*disabling and enabling the phy and the line */
      for (j=0;j<WTI_ITERATIONS_NUM;j++)
      {
         if (pw_index%100 == 0)
            printf("Disabling and enabling  phy %d and line %d\n",pw_index,i);
         WT_UfePhyDisable(&the_system->ufe,pw_index);
         WT_UfeLineDisable(&the_system->ufe,i);
         WP_Delay(2000);
         WT_UfeLineEnable(&the_system->ufe,i);
         WT_UfePhyEnable(&the_system->ufe, pw_index);
      }
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

void CLI_F_SonetSdhE1PhyEnableDisable(char *StrPrm)
{
   WP_U32 res, cmd, i,num_of_lines, num_slots, pw_index=0, iter;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface, option = 4;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   framing_mode = WUFE_UNFRAMED;
   cas_mode = WUFE_CAS_DISABLE;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &interface, &option);
   if (res != 3)
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

   for (i=WT_FIRST_LINE_INDEX;i < (WT_FIRST_LINE_INDEX+num_of_lines);i++)
   {
      if((i%28)>20)
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

   for (i=WT_FIRST_LINE_INDEX;i < (WT_FIRST_LINE_INDEX+num_of_lines);i++)
   {

      if (i%100 == 0)
         printf("i = %d\n",i);

      if((i%28)>20)
      {
         continue;
      }
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

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
      if (option >= 0)
      {
         //printf("option0\n");
      }
   
      if (option >= 1)
      {
         //printf("option1\n");
         WT_UfePhyDisable(&the_system->ufe, pw_index);
      }

      if (option >= 2)
      {
         //printf("option2\n");
         WT_UfePhyEnable(&the_system->ufe, pw_index);
      }

      if (option >= 3)
      {
         //printf("option3\n");
         for(iter=0;iter<2;i++)
         {
            WPL_Delay(5000);
            WT_UfePhyDisable(&the_system->ufe, pw_index);
            WPL_Delay(50000);
            WT_UfePhyEnable(&the_system->ufe, pw_index);
         }
      }
      pw_index++;
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
#ifdef WT_UFE_FRAMER
         physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12;
#endif
   num_of_lines = 4;
   num_slots = WTI_E1_SLOTS;


   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
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
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WTI_SOCKET_LOOPBACK_MODE);
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
#ifdef WT_UFE_FRAMER
         physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12;
#endif
   num_of_lines = WT_MAX_LINE_INDEX;
   num_slots = WTI_E1_SLOTS;


   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
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

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
   {
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1, NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WTI_SOCKET_LOOPBACK_MODE);

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

      /* Error should be in this function - enabling the line twice */
      WT_UfeLineEnable(&the_system->ufe,i);

      sprintf(temp_buf, "0 %d 0",i);
      CLI_F_PwEnable(temp_buf);

   }
}
void CLI_F_error_disable_before_enable(char *StrPrm)
{

   WP_U32 res, cmd, i, num_of_lines, num_slots,pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
   WUFE_status ufe_status;

#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
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

#ifdef WT_UFE_FRAMER
   physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12;
#endif
   num_of_lines = WT_MAX_LINE_INDEX;
   num_slots = WTI_E1_SLOTS;



   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
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

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
   {

      if ((i % 28) > 20)
      {
         continue;
      }
      if (i%100 == 0)
         printf("i = %d\n",i);

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
      CLI_F_PwCreateCascadedNoPhyEnable(temp_buf);

      /* enable the WP channels */
      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);

      /* Error should occur here - disabling the phy before enabling it. */
      ufe_status = WUFE_PhyDisable(the_system->ufe.phy_handle[pw_index], WUFE_FULL_DUPLEX);
      WT_UfeTerminateOnError (ufe_status, "WUFE_PhyDisable",pw_index , __LINE__);

      pw_index++;


   }
}

void CLI_F_Error_Enable_Disable_Disable(char *StrPrm)
{
   WP_U32 res, cmd, i, num_of_lines, num_slots, pw_index = 0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
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
#ifdef WT_UFE_FRAMER
         physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12;
#endif
   num_of_lines = WT_MAX_LINE_INDEX;
   num_slots = WTI_E1_SLOTS;


   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
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

   for (i=WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
   {

      if ((i%28)>20)
      {
         continue;      
      }
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1, NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WTI_SOCKET_LOOPBACK_MODE);

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

      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);
      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
//       sprintf(temp_buf, "0 %d",i);
//       CLI_F_PwDisable(temp_buf);
//       sprintf(temp_buf, "0 %d",i);
//       CLI_F_PwDisable(temp_buf);

      WT_UfePhyDisable(&the_system->ufe,i);
      /*Error should occur here - disabling the phy twice  */
      WT_UfePhyDisable(&the_system->ufe,i);
      pw_index++;


   }
}

void CLI_F_Error_Enable_Before_Create(char *StrPrm)
{

   WP_U32 res, cmd, i,j, num_of_lines, num_slots;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode;
   WUFE_line_transf_type transfer_type;
   WUFE_cas_enable cas_mode;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
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
#ifdef WT_UFE_FRAMER
         physical_configuration = WT_FLEXMUX_UFE_448_4_PORTS_OC12;
#endif
   num_of_lines = WT_MAX_LINE_INDEX ;
   num_slots = WTI_E1_SLOTS;


   /* allocate CESoP system */
   WTI_SystemAlloc();

   /* FPGA mode [0-SDH, 1-SONET, 2-CAD] */
   sprintf(temp_buf, "0 %d",interface);
   CLI_F_UfeFpgaMode(temp_buf);

   for (i = j = WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
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

   for (i = j=  WT_FIRST_LINE_INDEX; i<(WT_FIRST_LINE_INDEX+num_of_lines); i++)
   {


      if ((i % 28) > 20)
      {
         continue;
      }

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, i, transfer_type, framing_mode, cas_mode, 1, NULL);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, i, WTI_SOCKET_LOOPBACK_MODE);

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

      /* Error should occur here - enabling the phy before create the phy in the function CLI_F_PwCreateCascaded */
      WT_UfePhyEnable(&the_system->ufe, i);

      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(i%WTI_MAX_NUM_OF_MPLS_FA),i);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",i);
      CLI_F_PwEnable(temp_buf);
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
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
      num_of_lines = 4*252;
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
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif


   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
      num_of_lines = 4*336;
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
   res = sscanf(StrPrm, "%d %d %d", &cmd,&num_of_lines,&interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
      num_of_lines = 4*252;
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
   res = sscanf(StrPrm, "%d %d %d", &cmd,&num_of_lines,&interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
      num_of_lines = 4*336;
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
   WP_U32 res, cmd, i, line_index, num_of_lines, num_slots, slot_id, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_FRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_E1;
   WUFE_cas_enable cas_mode = WUFE_CAS_DISABLE;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
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
      num_of_lines = 4*252;
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
   /* This is 1xDS0 test therefore, change the number of slots to 1 and select the slot ID. */
   num_slots = 1;
   slot_id = 30;

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
         the_system->clock_rec[line_index].physical_clock_id = line_index;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);
#else
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
   }

   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/

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

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
      {
         continue;
      }

      /*UFE phy and pw configuration*/
      sprintf(temp_buf, "0 %d %d 40 40 %d %d", line_index, pw_index, num_slots, slot_id);
      CLI_F_PwConfigureFramed(temp_buf);

      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 40");

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
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 1",pw_index);
      CLI_F_PwEnable(temp_buf);
#else
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
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
void CLI_F_CR_101_SonetSdh_T1Framed1xDS0NoCas(char *StrPrm)
{
   WP_U32 res, cmd, i, line_index, num_of_lines, num_slots, slot_id, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_FRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_T1;
   WUFE_cas_enable cas_mode = WUFE_CAS_DISABLE;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
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
      num_of_lines = 4*336;
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
   /* This is 1xDS0 test therefore, change the number of slots to 1 and select the slot ID. */
   num_slots = 1;
   slot_id = 14;

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

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
      {
         /* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
         the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
         the_system->clock_rec[line_index].physical_clock_id = line_index;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);
#else
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index,WTI_SOCKET_LOOPBACK_MODE);
   }


   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/

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

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {

      /*UFE phy and pw configuration*/
      sprintf(temp_buf, "0 %d %d 40 40 %d %d", line_index, pw_index, num_slots, slot_id);
      CLI_F_PwConfigureFramed(temp_buf);

      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 40");

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
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 1",pw_index);
      CLI_F_PwEnable(temp_buf);
#else
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
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
void CLI_F_CR_102_SonetSdh_E1FramedCas(char *StrPrm)
{
   WP_U32 res, cmd, i, line_index, num_of_lines, num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_FRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_E1;
   WUFE_cas_enable cas_mode = WUFE_CAS_ENABLE;
   WP_U32 interface, fragmented_mode;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WUFE_line_clock_rec_params line_cr_params;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &num_of_lines, &interface, &fragmented_mode);

   if (res == 1)
   {
      interface = 0; /* use default setup */
      num_of_lines = 4*252;
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

   WTI_SetDefaultE1FrmTestParameters(num_of_lines, interface, &transfer_type,
                                     &framing_mode, &cas_mode, &num_slots
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* Enable CAS mode */
   cas_mode = WUFE_CAS_ENABLE;
   num_slots = 30;

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
         the_system->clock_rec[line_index].physical_clock_id = line_index;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);
#else
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
   }

   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/

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

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
      {
         continue;
      }

      /*UFE phy and pw configuration*/
      sprintf(temp_buf, "0 %d %d 30 30 %d 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31", line_index, pw_index, num_slots);
      CLI_F_PwConfigureFramed(temp_buf);

      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      if (fragmented_mode)
         CLI_F_Pwe3ChannelRxBufferSize("0 120"); /* 16*30/2 */
      else
         CLI_F_Pwe3ChannelRxBufferSize("0 480"); /* 16*30 */

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
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 1",pw_index);
      CLI_F_PwEnable(temp_buf);
#else
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
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
void CLI_F_CR_102_SonetSdh_T1FramedCas(char *StrPrm)
{
   WP_U32 res, cmd, i, line_index, num_of_lines, num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_FRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_T1;
   WUFE_cas_enable cas_mode = WUFE_CAS_ENABLE;
   WP_U32 interface, fragmented_mode;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WUFE_line_clock_rec_params line_cr_params;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &num_of_lines, &interface, &fragmented_mode);

   if (res == 1)
   {
      interface = 0; /* use default setup */
      num_of_lines = 4*336;
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

   WTI_SetDefaultT1FrmTestParameters(num_of_lines, interface, &transfer_type,
                                     &framing_mode, &cas_mode, &num_slots
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* Enable CAS mode */
   cas_mode = WUFE_CAS_ENABLE;
   num_slots = 24;

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

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
      {
         /* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
         the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
         the_system->clock_rec[line_index].physical_clock_id = line_index;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);
#else
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index,WTI_SOCKET_LOOPBACK_MODE);
   }


   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/

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

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {

      /*UFE phy and pw configuration*/
      sprintf(temp_buf, "0 %d %d 24 24 %d 0-23", line_index, pw_index, num_slots);
      CLI_F_PwConfigureFramed(temp_buf);

      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      if (fragmented_mode)
         CLI_F_Pwe3ChannelRxBufferSize("0 192"); /* 48*24/2 */
      else
         CLI_F_Pwe3ChannelRxBufferSize("0 384"); /* 48*24 */

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
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 1",pw_index);
      CLI_F_PwEnable(temp_buf);
#else
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
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
void CLI_F_CR_103_SonetSdh_E1FramedNoCas_MultiplePwPerLine(char *StrPrm)
{
   WP_U32 res, cmd, i, line_index, num_of_lines, num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_FRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_E1;
   WUFE_cas_enable cas_mode = WUFE_CAS_DISABLE;
   WP_U32 interface, recover_pw = 0, pw_per_line = 4;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WUFE_line_clock_rec_params line_cr_params;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &num_of_lines, &interface, &pw_per_line, &recover_pw);

   if (res == 1)
   {
      interface = 0; /* use default setup */
      num_of_lines = 4*252;
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

   WTI_SetDefaultE1FrmTestParameters(num_of_lines, interface, &transfer_type,
                                     &framing_mode, &cas_mode, &num_slots
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* Calculate number of slots */
   num_slots = WTI_E1_SLOTS / pw_per_line;

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
         the_system->clock_rec[line_index].physical_clock_id = line_index;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);
#else
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
   }

   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/

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

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
      {
         continue;
      }

      /* Configure each PW in line */
      for (i=0; i<pw_per_line; i++)
      {
         /*UFE phy and pw configuration*/
         sprintf(temp_buf, "0 %d %d 64 64 %d %d-%d",
                 line_index, pw_index, num_slots,
                 (num_slots*(pw_index%pw_per_line)), (num_slots*(pw_index%pw_per_line)+(num_slots-1)));
         CLI_F_PwConfigureFramed(temp_buf);

         sprintf(temp_buf, "0 %d",num_slots);
         CLI_DefaultIwPwe3CesConfig(temp_buf);

         /* PWE3 channel configuration */
         CLI_F_Pwe3ChannelJitterBufferSize("0 32");
         CLI_F_Pwe3ChannelRxBufferSize("0 256");

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
         if ((pw_index % pw_per_line) == recover_pw)
            sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
         else
            sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
         CLI_F_PwCreateCascaded(temp_buf);

         if ((pw_index % pw_per_line) == recover_pw)
            sprintf(temp_buf, "0 %d 1",pw_index);
         else
            sprintf(temp_buf, "0 %d 0",pw_index);
         CLI_F_PwEnable(temp_buf);
#else
         sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
         CLI_F_PwCreateCascaded(temp_buf);

         sprintf(temp_buf, "0 %d 0",pw_index);
         CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
         pw_index++;
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
void CLI_F_CR_103_SonetSdh_T1FramedNoCas_MultiplePwPerLine(char *StrPrm)
{
   WP_U32 res, cmd, i, line_index, num_of_lines, num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_FRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_T1;
   WUFE_cas_enable cas_mode = WUFE_CAS_DISABLE;
   WP_U32 interface, recover_pw = 0, pw_per_line = 4;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WUFE_line_clock_rec_params line_cr_params;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d %d", &cmd, &num_of_lines, &interface, &pw_per_line, &recover_pw);

   if (res == 1)
   {
      interface = 0; /* use default setup */
      num_of_lines = 4*336;
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

   WTI_SetDefaultT1FrmTestParameters(num_of_lines, interface, &transfer_type,
                                     &framing_mode, &cas_mode, &num_slots
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* Calculate number of slots */
   num_slots = WTI_T1_SLOTS / pw_per_line;

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

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
      {
         /* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
         the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
         the_system->clock_rec[line_index].physical_clock_id = line_index;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);
#else
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index,WTI_SOCKET_LOOPBACK_MODE);
   }


   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/

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

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {

      /* Configure each PW in line */
      for (i=0; i<pw_per_line; i++)
      {
         /*UFE phy and pw configuration*/
         sprintf(temp_buf, "0 %d %d 48 48 %d %d-%d",
                 line_index, pw_index, num_slots,
                 (num_slots*(pw_index%pw_per_line)), (num_slots*(pw_index%pw_per_line)+(num_slots-1)));
         CLI_F_PwConfigureFramed(temp_buf);

         sprintf(temp_buf, "0 %d",num_slots);
         CLI_DefaultIwPwe3CesConfig(temp_buf);

         /* PWE3 channel configuration */
         CLI_F_Pwe3ChannelJitterBufferSize("0 32");
         CLI_F_Pwe3ChannelRxBufferSize("0 192");

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
         if ((pw_index % pw_per_line) == recover_pw)
            sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
         else
            sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
         CLI_F_PwCreateCascaded(temp_buf);

         if ((pw_index % pw_per_line) == recover_pw)
            sprintf(temp_buf, "0 %d 1",pw_index);
         else
            sprintf(temp_buf, "0 %d 0",pw_index);
         CLI_F_PwEnable(temp_buf);
#else
         sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
         CLI_F_PwCreateCascaded(temp_buf);

         sprintf(temp_buf, "0 %d 0",pw_index);
         CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
         pw_index++;
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
WP_U32 CLI_F_CR_104_SonetSdh_E1UnframedNoCas_AddRemovePwLine(char *StrPrm)
{
   WP_U32 res, cmd, i, line_index, num_of_lines, num_slots = 0, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_UNFRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_E1;
   WUFE_cas_enable cas_mode = WUFE_CAS_DISABLE;
   WP_U32 interface, line_delete = WP_TRUE, iterations, result;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WUFE_line_clock_rec_params line_cr_params;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &num_of_lines, &interface, &line_delete);

   if (res == 1)
   {
      interface = 0; /* use default setup */
      num_of_lines = 4*252;
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
         the_system->clock_rec[line_index].physical_clock_id = line_index;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);
#else
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
   }


   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/

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

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
      {
         continue;
      }

      /*UFE phy and pw configuration*/
      sprintf(temp_buf, "0 %d %d 64 64", line_index, pw_index);
      CLI_F_PwConfigureUnframed(temp_buf);

      sprintf(temp_buf, "0 %d", num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 256");

      /**************** TDM --> PSN IW *****************/
      CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
      sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d",pw_index+1);
      CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
      CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
      CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
      CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      /* Get the ts_diff_if_id from UFE driver  */
      /* Rx timing is used in differential mode */
      sprintf(temp_buf, "0 1 1 %d", WT_ClockRecInterfaceId(line_index));
      CLI_F_Tdm2PsnFlowAggTsParams(temp_buf); /* [ts_used(1-used,0-unused),ts_method(0-WP_IW_CESOP_ABS_TS,1-WP_IW_CESOP_DIFF_TS_MASTER ),ts_diff_if_id] */
#endif

      /**************** PSN --> TDM Tx binding*****************/
      CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
      CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 8");

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 1",pw_index);
      CLI_F_PwEnable(temp_buf);
#else
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
      pw_index++;
   }

   printf("Test is running. Wait few seconds to let the system become stable...\n");
   WP_Delay(10000000);
   /* Global statistics check, including reset. */
   result = CLI_F_GlobalStatisticsCheck("0");
   if (result == WT_FAIL)
   {
      /* retry */
      WP_Delay(1000000);
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
      for (i=pw_index; i>0; i--)
      {
         printf("remove PW #%d\n", i-1);
         sprintf(temp_buf, "0 %d",i-1);
         CLI_F_PwRemove(temp_buf);
      }

      /* If needed, remove all lines: first to last remove */
      if (line_delete == WP_TRUE)
      {
         for (line_index = WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
         {
            if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
               continue;

            printf("remove line #%d\n", line_index);
            /* line disable and line delete */
            WT_UfeLineDelete(&the_system->ufe, line_index);
         }
      } /* line_delete */

      /* Wait few milisecond before recreate */
      WP_Delay(60000);

      /* recreate lines (if needed) and PWs */
      if (line_delete)
      {
         for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
         {
            if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
               continue;

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
            if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
            {
               /* Add DCO configuration */
               the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
               the_system->clock_rec[line_index].physical_clock_id = line_index;
               WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
            }
            else
               WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

            /* UFE line configuration, creation and enabling */
            WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

            /* Set the defult PSN2TDM clock recoevry paramters */
            WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);
#else
            /* UFE line configuration, creation and enabling */
            WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

            WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
         }

         /**********************************/
         /*    CESOP MPLS CONFIGURATION    */
         /**********************************/
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
      } /* line_delete */

      /****************************/
      /*    PW CONFIGURATION      */
      /****************************/
      pw_index = 0;
      for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
      {
         if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
            continue;

         printf("PW configuration with PW #%d and line #%d\n", pw_index, line_index);
         /*UFE phy and pw configuration*/
         sprintf(temp_buf, "0 %d %d 64 64", line_index, pw_index);
         CLI_F_PwConfigureUnframed(temp_buf);

         sprintf(temp_buf, "0 %d", num_slots);
         CLI_DefaultIwPwe3CesConfig(temp_buf);

         /* PWE3 channel configuration */
         CLI_F_Pwe3ChannelJitterBufferSize("0 32");
         CLI_F_Pwe3ChannelRxBufferSize("0 256");

         /**************** TDM --> PSN IW *****************/
         CLI_F_Tdm2PsnFlowAggEnetHeader("0 aa aa aa aa aa aa 00 aa 00 aa 00 aa 0800");
         sprintf(temp_buf, "0 45000000 811e0000 40110000 1a1a1a1a %d",pw_index+1);
         CLI_F_Tdm2PsnFlowAggIpHeader(temp_buf);
         CLI_F_Tdm2PsnFlowAggUdpHeader("0 1010 0001 0 0");
         CLI_F_Tdm2PsnFlowAggRtpHeader("0 50 0 0 0 0");
         CLI_F_Tdm2PsnFlowAggControlWord("0 0 0 0");

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
         /* Get the ts_diff_if_id from UFE driver  */
         /* Rx timing is used in differential mode */
         sprintf(temp_buf, "0 1 1 %d", WT_ClockRecInterfaceId(line_index));
         CLI_F_Tdm2PsnFlowAggTsParams(temp_buf); /* [ts_used(1-used,0-unused),ts_method(0-WP_IW_CESOP_ABS_TS,1-WP_IW_CESOP_DIFF_TS_MASTER ),ts_diff_if_id] */
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

         /**************** PSN --> TDM Tx binding*****************/
         CLI_F_Psn2TdmTxBinWindowThreshold("0 16");
         CLI_F_Psn2TdmTxBinWindowSwitchoverThreshold("0 8");

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
         printf("create PW #%d\n", pw_index);
         sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
         CLI_F_PwCreateCascaded(temp_buf);

         printf("enable PW #%d\n", pw_index);
         sprintf(temp_buf, "0 %d 1",pw_index);
         CLI_F_PwEnable(temp_buf);
#else
         sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
         CLI_F_PwCreateCascaded(temp_buf);

         sprintf(temp_buf, "0 %d 0",pw_index);
         CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
         pw_index++;
      }

      printf("Wait few seconds to let the system become stable...\n");
      WP_Delay(3000000);
      /* Global statistics check, including reset. */
      result = CLI_F_GlobalStatisticsCheck("0");
      if (result == WT_FAIL)
      {
         WTI_PRINT_ERROR(("\n Test Failed. Iteration number: %d\n", iterations));
         return WT_FAIL;
      }
      printf(">>>>>>>>>>>>>>>> end of iteration %d   <<<<<<<<<<<<<<<\n", iterations);
   } /* for iterations */
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
   WP_U32 res, cmd, i, line_index, num_of_lines, num_slots = 0, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_UNFRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_T1;
   WUFE_cas_enable cas_mode = WUFE_CAS_DISABLE;
   WP_U32 interface, line_delete = WP_TRUE, iterations, result;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WUFE_line_clock_rec_params line_cr_params;
#endif


   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &num_of_lines, &interface, &line_delete);

   if (res == 1)
   {
      interface = 0; /* use default setup */
      num_of_lines = 4*336;
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


   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
      {
         /* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
         the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
         the_system->clock_rec[line_index].physical_clock_id = line_index;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);
#else
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
   }


   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/

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

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      /*UFE phy and pw configuration*/
      sprintf(temp_buf, "0 %d %d 48 48", line_index, pw_index);
      CLI_F_PwConfigureUnframed(temp_buf);

      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 192");

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
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 1",pw_index);
      CLI_F_PwEnable(temp_buf);
#else
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
      pw_index++;
   }

   printf("Test is running. Wait few seconds to let the system become stable...\n");
   WP_Delay(10000000);
   /* Global statistics check, including reset. */
   result = CLI_F_GlobalStatisticsCheck("0");
   if (result == WT_FAIL)
   {
      /* retry */
      WP_Delay(1000000);
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
      for (i=pw_index; i>0; i--)
      {
         printf("remove PW #%d\n", i-1);
         sprintf(temp_buf, "0 %d",i-1);
         CLI_F_PwRemove(temp_buf);
      }

      /* If needed, remove all lines: first to last remove */
      if (line_delete == WP_TRUE)
      {
         for (line_index = WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
         {
            printf("remove line #%d\n", line_index);
            /* line disable and line delete */
            WT_UfeLineDelete(&the_system->ufe, line_index);
         }
      } /* line_delete */

      /* Wait few milisecond before recreate */
      WP_Delay(60000);

      /* recreate lines (if needed) and PWs */
      if (line_delete)
      {
         for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
         {
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
            if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
            {
               /* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
               the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
               the_system->clock_rec[line_index].physical_clock_id = line_index;
               WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
            }
            else
               WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

            /* UFE line configuration, creation and enabling */
            WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

            /* Set the defult PSN2TDM clock recoevry paramters */
            WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);
#else
            /* UFE line configuration, creation and enabling */
            WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

            WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
         }

         /**********************************/
         /*    CESOP MPLS CONFIGURATION    */
         /**********************************/
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
      } /* line_delete */

      /****************************/
      /*    PW CONFIGURATION      */
      /****************************/
      pw_index = 0;
      for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
      {
         /*UFE phy and pw configuration*/
         sprintf(temp_buf, "0 %d %d 48 48", line_index, pw_index);
         CLI_F_PwConfigureUnframed(temp_buf);

         sprintf(temp_buf, "0 %d",num_slots);
         CLI_DefaultIwPwe3CesConfig(temp_buf);

         /* PWE3 channel configuration */
         CLI_F_Pwe3ChannelJitterBufferSize("0 32");
         CLI_F_Pwe3ChannelRxBufferSize("0 192");

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
         sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
         CLI_F_PwCreateCascaded(temp_buf);

         sprintf(temp_buf, "0 %d 1",pw_index);
         CLI_F_PwEnable(temp_buf);
#else
         sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
         CLI_F_PwCreateCascaded(temp_buf);

         sprintf(temp_buf, "0 %d 0",pw_index);
         CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
         pw_index++;
      }

      printf("Wait few seconds to let the system become stable...\n");
      WP_Delay(3000000);
      /* Global statistics check, including reset. */
      result = CLI_F_GlobalStatisticsCheck("0");
      if (result == WT_FAIL)
      {
         WTI_PRINT_ERROR(("\n Test Failed. Iteration number: %d\n", iterations));
         return WT_FAIL;
      }
      printf(">>>>>>>>>>>>>>>> end of iteration %d   <<<<<<<<<<<<<<<\n", iterations);
   } /* for iterations */
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
   WP_U32 res, cmd, i, line_index, num_of_lines, num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_FRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_E1;
   WUFE_cas_enable cas_mode = WUFE_CAS_DISABLE;
   WP_U32 interface, line_delete = WP_TRUE, iterations, result;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WUFE_line_clock_rec_params line_cr_params;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &num_of_lines, &interface, &line_delete);

   if (res == 1)
   {
      interface = 0; /* use default setup */
      num_of_lines = 4*252;
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
         the_system->clock_rec[line_index].physical_clock_id = line_index;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);
#else
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
   }

   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/

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

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
      {
         continue;
      }

      /*UFE phy and pw configuration*/
      sprintf(temp_buf, "0 %d %d 64 64 %d 0-31", line_index, pw_index, num_slots);
      CLI_F_PwConfigureFramed(temp_buf);

      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 256");

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
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 1",pw_index);
      CLI_F_PwEnable(temp_buf);
#else
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
      pw_index++;
   }

   printf("Test is running. Wait few seconds to let the system become stable...\n");
   WP_Delay(10000000);
   /* Global statistics check, including reset. */
   result = CLI_F_GlobalStatisticsCheck("0");
   if (result == WT_FAIL)
   {
      /* retry */
      WP_Delay(1000000);
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
      for (i=pw_index; i>0; i--)
      {
         printf("remove PW #%d\n", i-1);
         sprintf(temp_buf, "0 %d",i-1);
         CLI_F_PwRemove(temp_buf);
      }

      /* If needed, remove all lines: first to last remove */
      if (line_delete == WP_TRUE)
      {
         for (line_index = WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
         {
            if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
               continue;

            printf("remove line #%d\n", line_index);
            /* line disable and line delete */
            WT_UfeLineDelete(&the_system->ufe, line_index);
         }
      } /* line_delete */

      /* Wait few milisecond before recreate */
      WP_Delay(60000);

      /* recreate lines (if needed) and PWs */
      if (line_delete)
      {
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
               the_system->clock_rec[line_index].physical_clock_id = line_index;
               WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
            }
            else
               WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

            /* UFE line configuration, creation and enabling */
            WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

            /* Set the defult PSN2TDM clock recoevry paramters */
            WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);
#else
            /* UFE line configuration, creation and enabling */
            WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

            WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
         }

         /**********************************/
         /*    CESOP MPLS CONFIGURATION    */
         /**********************************/
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
      } /* line_delete */

      /****************************/
      /*    PW CONFIGURATION      */
      /****************************/
      pw_index = 0;
      for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
      {
         if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
         {
            continue;
         }

         /*UFE phy and pw configuration*/
         sprintf(temp_buf, "0 %d %d 64 64 %d 0-31", line_index, pw_index, num_slots);
         CLI_F_PwConfigureFramed(temp_buf);

         sprintf(temp_buf, "0 %d",num_slots);
         CLI_DefaultIwPwe3CesConfig(temp_buf);

         /* PWE3 channel configuration */
         CLI_F_Pwe3ChannelJitterBufferSize("0 32");
         CLI_F_Pwe3ChannelRxBufferSize("0 256");

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
         sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
         CLI_F_PwCreateCascaded(temp_buf);

         sprintf(temp_buf, "0 %d 1",pw_index);
         CLI_F_PwEnable(temp_buf);
#else
         sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
         CLI_F_PwCreateCascaded(temp_buf);

         sprintf(temp_buf, "0 %d 0",pw_index);
         CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
         pw_index++;
      }

      printf("Wait few seconds to let the system become stable...\n");
      WP_Delay(3000000);
      /* Global statistics check, including reset. */
      result = CLI_F_GlobalStatisticsCheck("0");
      if (result == WT_FAIL)
      {
         WTI_PRINT_ERROR(("\n Test Failed. Iteration number: %d\n", iterations));
         return WT_FAIL;
      }
      printf(">>>>>>>>>>>>>>>> end of iteration %d   <<<<<<<<<<<<<<<\n", iterations);
   } /* for iterations */
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
   WP_U32 res, cmd, i, line_index, num_of_lines, num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_FRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_T1;
   WUFE_cas_enable cas_mode = WUFE_CAS_DISABLE;
   WP_U32 interface, line_delete = WP_TRUE, iterations, result;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WUFE_line_clock_rec_params line_cr_params;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &num_of_lines, &interface, &line_delete);

   if (res == 1)
   {
      interface = 0; /* use default setup */
      num_of_lines = 4*336;
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

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
      {
         /* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
         the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
         the_system->clock_rec[line_index].physical_clock_id = line_index;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);
#else
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index,WTI_SOCKET_LOOPBACK_MODE);
   }


   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/

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

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {

      /*UFE phy and pw configuration*/
      sprintf(temp_buf, "0 %d %d 48 48 %d 0-23", line_index, pw_index, num_slots);
      CLI_F_PwConfigureFramed(temp_buf);

      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 192");

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
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 1",pw_index);
      CLI_F_PwEnable(temp_buf);
#else
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
      pw_index++;
   }

   printf("Test is running. Wait few seconds to let the system become stable...\n");
   WP_Delay(10000000);
   /* Global statistics check, including reset. */
   result = CLI_F_GlobalStatisticsCheck("0");
   if (result == WT_FAIL)
   {
      /* retry */
      WP_Delay(1000000);
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
      for (i=pw_index; i>0; i--)
      {
         printf("remove PW #%d\n", i-1);
         sprintf(temp_buf, "0 %d",i-1);
         CLI_F_PwRemove(temp_buf);
      }

      /* If needed, remove all lines: first to last remove */
      if (line_delete == WP_TRUE)
      {
         for (line_index = WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
         {
            printf("remove line #%d\n", line_index);
            /* line disable and line delete */
            WT_UfeLineDelete(&the_system->ufe, line_index);
         }
      } /* line_delete */

      /* Wait few milisecond before recreate */
      WP_Delay(60000);

      /* recreate lines (if needed) and PWs */
      if (line_delete)
      {
         for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
         {
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
            if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
            {
               /* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
               the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
               the_system->clock_rec[line_index].physical_clock_id = line_index;
               WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
            }
            else
               WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

            /* UFE line configuration, creation and enabling */
            WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

            /* Set the defult PSN2TDM clock recoevry paramters */
            WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);
#else
            /* UFE line configuration, creation and enabling */
            WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

            WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index,WTI_SOCKET_LOOPBACK_MODE);
         }

         /**********************************/
         /*    CESOP MPLS CONFIGURATION    */
         /**********************************/
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
      } /* line_delete */

      /****************************/
      /*    PW CONFIGURATION      */
      /****************************/
      pw_index = 0;
      for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
      {

         /*UFE phy and pw configuration*/
         sprintf(temp_buf, "0 %d %d 48 48 %d 0-23", line_index, pw_index, num_slots);
         CLI_F_PwConfigureFramed(temp_buf);

         sprintf(temp_buf, "0 %d",num_slots);
         CLI_DefaultIwPwe3CesConfig(temp_buf);

         /* PWE3 channel configuration */
         CLI_F_Pwe3ChannelJitterBufferSize("0 32");
         CLI_F_Pwe3ChannelRxBufferSize("0 192");

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
         sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
         CLI_F_PwCreateCascaded(temp_buf);

         sprintf(temp_buf, "0 %d 1",pw_index);
         CLI_F_PwEnable(temp_buf);
#else
         sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
         CLI_F_PwCreateCascaded(temp_buf);

         sprintf(temp_buf, "0 %d 0",pw_index);
         CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
         pw_index++;
      }

      printf("Wait few seconds to let the system become stable...\n");
      WP_Delay(3000000);
      /* Global statistics check, including reset. */
      result = CLI_F_GlobalStatisticsCheck("0");
      if (result == WT_FAIL)
      {
         WTI_PRINT_ERROR(("\n Test Failed. Iteration number: %d\n", iterations));
         return WT_FAIL;
      }
      printf(">>>>>>>>>>>>>>>> end of iteration %d   <<<<<<<<<<<<<<<\n", iterations);
   } /* for iterations */
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
   WP_U32 res, cmd, i, line_index, num_of_lines, num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_FRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_E1;
   WUFE_cas_enable cas_mode = WUFE_CAS_ENABLE;
   WP_U32 interface, fragmented_mode;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WUFE_line_clock_rec_params line_cr_params;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &num_of_lines, &interface, &fragmented_mode);

   if (res == 1)
   {
      interface = 0; /* use default setup */
      num_of_lines = 4*252;
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

   WTI_SetDefaultE1FrmTestParameters(num_of_lines, interface, &transfer_type,
                                     &framing_mode, &cas_mode, &num_slots
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* Enable CAS mode */
   cas_mode = WUFE_CAS_ENABLE;
   num_slots = 30;

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
         the_system->clock_rec[line_index].physical_clock_id = line_index;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);
#else
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
   }

   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/

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

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
      {
         continue;
      }

      /*UFE phy and pw configuration*/
      sprintf(temp_buf, "0 %d %d 30 30 %d 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31", line_index, pw_index, num_slots);
      CLI_F_PwConfigureFramed(temp_buf);

      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      if (fragmented_mode)
         CLI_F_Pwe3ChannelRxBufferSize("0 120"); /* 16*30/2 */
      else
         CLI_F_Pwe3ChannelRxBufferSize("0 480"); /* 16*30 */

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
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 1",pw_index);
      CLI_F_PwEnable(temp_buf);
#else
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
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
void CLI_F_CR_104_SonetSdh_T1FramedCas_AddRemovePwLine(char *StrPrm)
{
   WP_U32 res, cmd, i, line_index, num_of_lines, num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_FRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_T1;
   WUFE_cas_enable cas_mode = WUFE_CAS_ENABLE;
   WP_U32 interface, fragmented_mode;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WUFE_line_clock_rec_params line_cr_params;
#endif

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface, &fragmented_mode);

   if (res == 1)
   {
      interface = 0; /* use default setup */
      num_of_lines = 4*336;
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

   WTI_SetDefaultT1FrmTestParameters(num_of_lines, interface, &transfer_type,
                                     &framing_mode, &cas_mode, &num_slots
#ifdef WT_UFE_FRAMER
                                     , &flexmux_mode /* in case of WT_UFE_FRAMER only */
#endif
      );

   /* Enable CAS mode */
   cas_mode = WUFE_CAS_ENABLE;
   num_slots = 24;

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

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
      {
         /* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
         the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
         the_system->clock_rec[line_index].physical_clock_id = line_index;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);
#else
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index,WTI_SOCKET_LOOPBACK_MODE);
   }


   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/

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

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {

      /*UFE phy and pw configuration*/
      sprintf(temp_buf, "0 %d %d 24 24 %d 0-23", line_index, pw_index, num_slots);
      CLI_F_PwConfigureFramed(temp_buf);

      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      if (fragmented_mode)
         CLI_F_Pwe3ChannelRxBufferSize("0 192"); /* 48*24/2 */
      else
         CLI_F_Pwe3ChannelRxBufferSize("0 384"); /* 48*24 */

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
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 1",pw_index);
      CLI_F_PwEnable(temp_buf);
#else
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
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
void CLI_F_CR_105_SonetSdh_E1UnframedNoCas_DifferentDuAndPs(char *StrPrm)
{
   WP_U32 res, cmd, i, line_index, num_of_lines, num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_UNFRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_E1;
   WUFE_cas_enable cas_mode = WUFE_CAS_DISABLE;
   WP_U32 interface, du_size = 64, ps = 256;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif
   WUFE_line_clock_rec_params line_cr_params;

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
      num_of_lines = 200;
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

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
      {
         continue;
      }

      if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
      {
         /* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
         the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
         the_system->clock_rec[line_index].physical_clock_id = line_index;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);

      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
   }


   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/

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

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
      {
         continue;
      }

      /* Different DU and PS sizes */
      /* Select between 2 tests:
         1. Full bandwidth: change only 4 PWs (due to have load on the EMPHY and ENET.
         2. Partial bandwidth: run with 42 PWs, different DU and PS to each PW.*/
#if 1
      switch (pw_index % 126)
      {
         case 0:
         {
            du_size = 16;
            ps = 48;
            break;
         }
         case 1:
         {
            du_size = 32;
            ps = 192;
            break;
         }
         default:
         {
            du_size = 64;
            ps = 576;
            break;
         }
      }
#else
      du_size = 64 / (1<<(pw_index % 3));
      ps = 64 * ((pw_index % 4)+1);
#endif /* 1 */

      printf("--> pw_index = %4d    du_size = %3d   ps = %3d\n"
             , pw_index, du_size, ps);
      /*UFE phy and pw configuration*/
      sprintf(temp_buf, "0 %d %d %d %d", line_index, pw_index, du_size, du_size);
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
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 1",pw_index);
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
void CLI_F_CR_105_SonetSdh_T1UnframedNoCas_DifferentDuAndPs(char *StrPrm)
{
   WP_U32 res, cmd, i, line_index, num_of_lines, num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_UNFRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_T1;
   WUFE_cas_enable cas_mode = WUFE_CAS_DISABLE;
   WP_U32 interface, du_size = 48, ps = 192;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif
   WUFE_line_clock_rec_params line_cr_params;


   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
      num_of_lines = 300;
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


   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
      {
         /* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
         the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
         the_system->clock_rec[line_index].physical_clock_id = line_index;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);

      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
   }


   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/

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

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      /* Different DU and PS sizes */
      /* Select between 2 tests:
         1. Full bandwidth: change only 4 PWs (due to have load on the EMPHY and ENET.
         2. Partial bandwidth: run with 70 PWs, different DU and PS to each PW.*/
#if 1
      switch (pw_index % 168)
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
            break;
         }
         default:
         {
            du_size = 48;
            ps = 576;
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

      printf("--> pw_index = %4d    du_size = %3d   ps = %3d\n",
             pw_index, du_size, ps);
      /*UFE phy and pw configuration*/
      sprintf(temp_buf, "0 %d %d %d %d", line_index, pw_index, du_size, du_size);
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
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 1",pw_index);
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
void CLI_F_CR_105_SonetSdh_E1FramedNoCas_DifferentDuAndPs(char *StrPrm)
{
   WP_U32 res, cmd, i, line_index, num_of_lines, num_slots, new_num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_FRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_E1;
   WUFE_cas_enable cas_mode = WUFE_CAS_DISABLE;
   WP_U32 interface, du_size = 64, ps = 256, first_slot = 0, last_slot = 31;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
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
      num_of_lines = 4*252;
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
         the_system->clock_rec[line_index].physical_clock_id = line_index;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);
#else
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
   }

   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/

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

      /* Different DU and PS sizes */
      /* Select between 2 tests:
         1. Full bandwidth: change only 4 PWs (due to have load on the EMPHY and ENET.
         2. Partial bandwidth: run with 42 PWs, different DU and PS to each PW.*/
#if 1
      switch (pw_index % 126)
      {
         case 0:
         {
            du_size = 16;
            ps = 64;
            first_slot = 0;
            last_slot = 15;
            new_num_slots = du_size;
            break;
         }
         case 1:
         {
            du_size = 32;
            ps = 192;
            break;
         }
         default:
         {
            du_size = 64;
            ps = 576;
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

      printf("--> pw_index = %4d    du_size = %3d   ps = %3d   new_num_slots = %2d  slots: %2d - %2d\n",
             pw_index, du_size, ps, new_num_slots, first_slot, last_slot);
      /*UFE phy and pw configuration*/
      sprintf(temp_buf, "0 %d %d %d %d %d %d-%d",
              line_index, pw_index, du_size, du_size, new_num_slots, first_slot, last_slot);
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
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 1",pw_index);
      CLI_F_PwEnable(temp_buf);
#else
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
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
void CLI_F_CR_105_SonetSdh_T1FramedNoCas_DifferentDuAndPs(char *StrPrm)
{
   WP_U32 res, cmd, i, line_index, num_of_lines, num_slots, new_num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_FRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_T1;
   WUFE_cas_enable cas_mode = WUFE_CAS_DISABLE;
   WP_U32 interface, du_size = 48, ps = 192, first_slot = 0, last_slot = 23;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
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
      num_of_lines = 4*336;
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

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
      {
         /* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
         the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
         the_system->clock_rec[line_index].physical_clock_id = line_index;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, global_cr_operation_mode);
#else
      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, NULL);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index,WTI_SOCKET_LOOPBACK_MODE);
   }


   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/

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

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      /* Initial state: */
      new_num_slots = num_slots;
      first_slot = 0;
      last_slot = new_num_slots-1;

      /* Different DU and PS sizes */
      /* Select between 2 tests:
         1. Full bandwidth: change only 4 PWs (due to have load on the EMPHY and ENET.
         2. Partial bandwidth: run with 70 PWs, different DU and PS to each PW.*/
#if 1
      switch (pw_index % 168)
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
         default:
         {
            du_size = 48;
            ps = 576;
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

      printf("--> pw_index = %4d    du_size = %3d   ps = %3d   new_num_slots = %2d  slots: %2d - %2d\n",
             pw_index, du_size, ps, new_num_slots, first_slot, last_slot);
      /*UFE phy and pw configuration*/
      sprintf(temp_buf, "0 %d %d %d %d %d %d-%d",
              line_index, pw_index, du_size, du_size, new_num_slots, first_slot, last_slot);
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
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,1,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 1",pw_index);
      CLI_F_PwEnable(temp_buf);
#else
      sprintf(temp_buf, "0 %d %d %d %d %d", 0,0,0,(pw_index%WTI_MAX_NUM_OF_MPLS_FA),pw_index);
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 0",pw_index);
      CLI_F_PwEnable(temp_buf);
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
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
void CLI_F_CR_106_SonetSdh_E1UnframedNoCas_AdapDiffPerLine(char *StrPrm)
{
   WP_U32 res, cmd, i, line_index, num_of_lines, num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_UNFRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_E1;
   WUFE_cas_enable cas_mode = WUFE_CAS_DISABLE;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif
   WUFE_line_clock_rec_params line_cr_params;
   WP_U32 line_cr_operation_mode[WTI_CESOP_MAX_CLOCK_REC_INTERFACES] = {0};

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
      num_of_lines = 4*252;
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

   /* Set CR operation mode and set the maximun number of lines in the test */
   for (i=WT_FIRST_LINE_INDEX;i<num_of_lines+WT_FIRST_LINE_INDEX;i++)
   {
      if((i%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
      {
         num_of_lines++;
         continue;
      }

      if ((i%2))
         line_cr_operation_mode[i] = WP_IW_CLOCK_REC_DIFFERENTIAL_MODE;
      else
         line_cr_operation_mode[i] = WP_IW_CLOCK_REC_ADAPTIVE_MODE;
   }


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

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
      {
         continue;
      }
      if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
      {
         /* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
         the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
         the_system->clock_rec[line_index].physical_clock_id = line_index;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, line_cr_operation_mode[line_index]);
      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
   }


   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/

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

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
      {
         continue;
      }

      /*UFE phy and pw configuration*/
      sprintf(temp_buf, "0 %d %d 64 64", line_index, pw_index);
      CLI_F_PwConfigureUnframed(temp_buf);

      sprintf(temp_buf, "0 %d", num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 256");

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
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 1",pw_index);
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
void CLI_F_CR_106_SonetSdh_T1UnframedNoCas_AdapDiffPerLine(char *StrPrm)
{
   WP_U32 res, cmd, i, line_index, num_of_lines, num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_UNFRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_T1;
   WUFE_cas_enable cas_mode = WUFE_CAS_DISABLE;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif
   WUFE_line_clock_rec_params line_cr_params;
   WP_U32 line_cr_operation_mode[WTI_CESOP_MAX_CLOCK_REC_INTERFACES] = {0};

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
      num_of_lines = 4*336;
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

   /* Set CR operation mode */
   for (i=WT_FIRST_LINE_INDEX;i<num_of_lines+WT_FIRST_LINE_INDEX;i++)
   {
      if (i<5)
         line_cr_operation_mode[i] = WP_IW_CLOCK_REC_ADAPTIVE_MODE;
      else
         line_cr_operation_mode[i] = WP_IW_CLOCK_REC_DIFFERENTIAL_MODE;
   }

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


   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
      {
         /* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
         the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
         the_system->clock_rec[line_index].physical_clock_id = line_index;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, line_cr_operation_mode[line_index]);

      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
   }


   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/

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

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      /*UFE phy and pw configuration*/
      sprintf(temp_buf, "0 %d %d 48 48", line_index, pw_index);
      CLI_F_PwConfigureUnframed(temp_buf);

      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 192");

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
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 1",pw_index);
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
void CLI_F_CR_106_SonetSdh_E1FramedNoCas_AdapDiffPerLine(char *StrPrm)
{
   WP_U32 res, cmd, i, line_index, num_of_lines, num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_FRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_E1;
   WUFE_cas_enable cas_mode = WUFE_CAS_DISABLE;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif
   WUFE_line_clock_rec_params line_cr_params;
   WP_U32 line_cr_operation_mode[WTI_CESOP_MAX_CLOCK_REC_INTERFACES] = {0};

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
      num_of_lines = 4*252;
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

   /* Set CR operation mode and set the maximun number of lines in the test */
   for (i=WT_FIRST_LINE_INDEX;i<num_of_lines+WT_FIRST_LINE_INDEX;i++)
   {
      if((i%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
      {
         num_of_lines++;
         continue;
      }

      if ((i%2))
         line_cr_operation_mode[i] = WP_IW_CLOCK_REC_DIFFERENTIAL_MODE;
      else
         line_cr_operation_mode[i] = WP_IW_CLOCK_REC_ADAPTIVE_MODE;
   }

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

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
      {
         continue;
      }
      if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
      {
         /* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
         the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
         the_system->clock_rec[line_index].physical_clock_id = line_index;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateE1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, line_cr_operation_mode[line_index]);

      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index, WTI_SOCKET_LOOPBACK_MODE);
   }

   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/

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

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      if((line_index%WTI_T1_LINES_PER_SPE)>=WTI_E1_LINES_PER_SPE)
      {
         continue;
      }

      /*UFE phy and pw configuration*/
      sprintf(temp_buf, "0 %d %d 64 64 %d 0-31", line_index, pw_index, num_slots);
      CLI_F_PwConfigureFramed(temp_buf);

      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 256");

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
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 1",pw_index);
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
void CLI_F_CR_106_SonetSdh_T1FramedNoCas_AdapDiffPerLine(char *StrPrm)
{
   WP_U32 res, cmd, i, line_index, num_of_lines, num_slots, pw_index=0;
   WP_CHAR temp_buf[WTI_MAX_STRING_SIZE];
   WUFE_un_framed framing_mode = WUFE_FRAMED;
   WUFE_line_transf_type transfer_type = WUFE_SDH_TYPE_T1;
   WUFE_cas_enable cas_mode = WUFE_CAS_DISABLE;
   WP_U32 interface;
#ifdef WT_UFE_FRAMER
   WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4=WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC;
   WTI_Flexmux_global_setup flex_global_params;
   WP_U8 flexmux_mode=0;
#endif
   WUFE_line_clock_rec_params line_cr_params;
   WP_U32 line_cr_operation_mode[WTI_CESOP_MAX_CLOCK_REC_INTERFACES] = {0};

   /* Read parameters from user */
   /*Interface: 0 -SDH, 1- SONET, 2 -CAD*/
   res = sscanf(StrPrm, "%d %d %d", &cmd, &num_of_lines, &interface);

   if (res == 1)
   {
      interface = 0; /* use default setup */
      num_of_lines = 4*336;
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

   /* Set CR operation mode */
   for (i=WT_FIRST_LINE_INDEX;i<num_of_lines+WT_FIRST_LINE_INDEX;i++)
   {
      if (i<1)
         line_cr_operation_mode[i] = WP_IW_CLOCK_REC_ADAPTIVE_MODE;
      else
         line_cr_operation_mode[i] = WP_IW_CLOCK_REC_DIFFERENTIAL_MODE;
   }

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

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {
      if(line_index == WT_LINE_INDEX_TO_FIRST_DCO || line_index == WT_LINE_INDEX_TO_SECOND_DCO)
      {
         /* Add DCO configuration , note for debug, the dco output mux is at 0xbf230066*/
         the_system->clock_rec[line_index].physical_clock_enable = WP_TRUE;
         the_system->clock_rec[line_index].physical_clock_id = line_index;
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 1, line_index); /* tx_enable, dco_active, dco_number */
      }
      else
         WT_UfeClockRecLineConfigure(&line_cr_params, 1, 0, 0); /* tx_enable, dco_active, dco_number */

      /* UFE line configuration, creation and enabling */
      WT_UfeLineConfigureAndCreateT1(&the_system->ufe, line_index, transfer_type, framing_mode, cas_mode, 1, &line_cr_params);

      /* Set the defult PSN2TDM clock recoevry paramters */
      WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(line_index, line_cr_operation_mode[line_index]);

      WT_UfeLineSocketLpbkSetup(&the_system->ufe, line_index,WTI_SOCKET_LOOPBACK_MODE);
   }


   /*******************************************************************************************/
   /*                                     CESOP MPLS CONFIGURATION                            */
   /*******************************************************************************************/

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

   /*******************************************************************************************/
   /*                                     PW CONFIGURATION                                    */
   /*******************************************************************************************/

   for (line_index=WT_FIRST_LINE_INDEX; line_index<(num_of_lines+WT_FIRST_LINE_INDEX); line_index++)
   {

      /*UFE phy and pw configuration*/
      sprintf(temp_buf, "0 %d %d 48 48 %d 0-23", line_index, pw_index, num_slots);
      CLI_F_PwConfigureFramed(temp_buf);

      sprintf(temp_buf, "0 %d",num_slots);
      CLI_DefaultIwPwe3CesConfig(temp_buf);

      /* PWE3 channel configuration */
      CLI_F_Pwe3ChannelJitterBufferSize("0 32");
      CLI_F_Pwe3ChannelRxBufferSize("0 192");

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
      CLI_F_PwCreateCascaded(temp_buf);

      sprintf(temp_buf, "0 %d 1",pw_index);
      CLI_F_PwEnable(temp_buf);
      pw_index++;
   }
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





   CLI_F_CR_101_SonetSdh_E1UnframedNoCas( "0 504");
   

   WPL_Delay(30000000);

   stats_result=CLI_F_GlobalStatisticsCheck("0");
   
   printf("********************************\n");
   printf("*regular lines test finished   *\n");
   printf("*switching to protection lines *\n");
   printf("********************************\n");

   for(i=0;i<8;i++)
   {
      sprintf(temp_buf, "0 0 %d 1",i);
      CLI_F_UfeLineProtectionSwitch(temp_buf);
      printf("line %d switched to protection\n",i);

   }


   WPL_Delay(30000000);

   stats_result=CLI_F_GlobalStatisticsCheck("0");
}
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */
