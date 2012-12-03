 /*****************************************************************************
 * (C) Copyright 2000 - 2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/
/*****************************************************************************
 *
 * Module : wpu_tb.c
 *
 * Purpose: enables the trace buffer with the possible configuration options
 *
 *****************************************************************************/
#include "wpui_includes.h"
#include "wpu_wf.h"
#include "wpui_traceutil.h"
#include "wpu_tb.h"
#include "wpui_hw_util.h"
#include "wpu_ta.h"
#include "wpu_memory.h"
#include "wpui_serial.h"

#define TRACE_OFF           0xFFFFFFFF
#define ADDRESS_LIMIT 		0xFFFF
#define MASK_LIMIT 			15
#define SERIAL_LIMIT  		64
#define COUNTER_LIMIT 		16
#define ET_COUNTER_LIMIT 	64
#define ET_MASK_LIMIT 		0xFF
#define WPI_DPS_ACCESS_SIZE 0x100

enum WPI_CFU_ACCESS_INDEX
{
  WPI_CFU_TOTAL_DPS_IMAGE_NUM = 0,
  WPI_CFU_DPS_IMAGE_ENTRYPTS_NUM,
  WPI_CFU_DPS_IMAGE_ENTRYPTS_1,
  WPI_CFU_DPS_IMAGE_ENTRYPTS_2,
  WPI_CFU_DPS_IMAGE_ENTRYPTS_3,
  WPI_CFU_DPS_IMAGE_ENTRYPTS_4,


  WPI_CFU_LAST_INDEX,
};

t_cmd_option tb_options[] =
{
{ "init", "Initialize trace buffer" },
{ "  -w", "Trace buffer wrap disable" },
{ "  -t", "Task switch trace disable" },
{ "  -c", "Change of flow trace disable" },
{ "  -r", "Register trace disable" },
{ "  -s", "Enable automatic writes of SYNC entries to the trace-buffers" },
{ "  -e", "End trace disable" },
{ "  -start_addr", "Activate the address trace condition - Start Address" },
{ "  -end_addr  ", "Activate the address trace condition - End Address" },
{ "  -trace_all", "Activate the trace all condition" },
{ "  -serial", "Activate the serial trace condition" },
{ "      serial Name   ", "Serials to trace- at least 1 should be given" },
{ "  -trs [trs_num]", "Activate the TRS trace condition. Trace only TRS number trs_num" },
{ "  -counter", "Events counter - will enable the trace only after counter + 1\n\r"
	" \t\t     occurrences of the trace condition" },
{ "  -et_counter", "End-trace counter- end trace occurs after counter +1 end-trace signals" },
{ "  -et_mask", "Mask for the 8 End Trace inputs" },
{ "      et_mask", "0 - means that the input is not active. mask should start with 0x" },
{ "  -no_stop_trace", "Do not stop tracing upon end-trace event" },
{ "  -no_stop_wg", "Do not stop wingines upon end-trace event" },
{ "  -no_stop", "Do not stop trace or wingines upon end-trace event" },
{ "dump", "Dump the trace buffer" },
{ "presc <presc_val>", "Set the prescalar value to presc_val" },
{ "status", "Print current status of trace options" } ,
{ "-wp_no <wp_no>", "optional - The Winpath number of the trace buffer (0 in default )."}};

t_cmd tb_cmd =
{ "tb",
		"tb init [-w, -t, -c, -r, -s, -e] \n\r"
	"\t\t[-no_stop_trace | -no_stop_wg | -no_stop] \n\r"
	"\t\t[-start_addr <start_addr> [sa_mask_val]] \n\r"
	"\t\t[-end_addr   <end_addr>   [ea_mask_val]] \n\r"
	"\t\t[-trace_all\n\r"
	"\t\t[-serial <serial1> [serial2] [serial3] [serial4] [serial5]]\n\r"
	"\t\t[-trs <trs_num>]\n\r"
	"\t\t[-counter <0-15> ]\n\r"
	"\t\t[-et_counter <0-63>]\n\r"
	"\t\t[-et_mask <et_mask>]\n\r"
   "\t\t[-wp_no <wp_no>]\n\r"
	"\t    dump [-wp_no <wp_no>]\n\r"
	"\t    presc <presc_val> [-wp_no <wp_no>]\n\r"
	"\t    status [-wp_no <wp_no>]\n\r",
	"TB = Trace Buffer\n\r"
	"\tTrace buffer initialization options, dump command, prescalar setting and\n\t"
	"\tstatus prints.\n\r",
	tb_options,
	24,
	0,
	"Trace buffer initialization options, dump command, prescalar setting and \n\r\t\t\tstatus prints" };


enum
{
	INIT = 0,
	DISABLE_WRAP,
	DISABLE_TASK_SWITCH_TRACE,
	DISABLE_CHANGE_OF_FLOW_TRACE,
	DISABLE_REGISTER_TRACE,
	ENABLE_SYNC,
	DISABLE_END_TRACE,
	NO_STOP_TRACE,
	NO_STOP_WG,
	NO_STOP,
	START_ADDR,
	END_ADDR,
	TRACE_ALL,
	SERIAL,
	TRS,
	EVENT_COUNTER,
	ET_COUNTER,
	ET_MASK,
	PRESC,
	MASK,
	DUMP,
	TB_STATUS,
	WINPATH_NUM,
	HELP,
	SHORT_HELP,
	ARRAY_SIZE
};

static S_Arg Args[ARRAY_SIZE] =
{
{ NoArg,   "init"			, WP_FALSE, WP_FALSE, 0 },
{ NoArg,   "-w"				, WP_FALSE, WP_FALSE, 0 },
{ NoArg,   "-t"				, WP_FALSE, WP_FALSE, 0 },
{ NoArg,   "-c"				, WP_FALSE, WP_FALSE, 0 },
{ NoArg,   "-r"				, WP_FALSE, WP_FALSE, 0 },
{ NoArg,   "-s"				, WP_FALSE, WP_FALSE, 0 },
{ NoArg,   "-e"				, WP_FALSE, WP_FALSE, 0 },
{ NoArg,   "-no_stop_trace"	, WP_FALSE, WP_FALSE, 0 },
{ NoArg,   "-no_stop_wg"	, WP_FALSE, WP_FALSE, 0 },
{ NoArg,   "-no_stop"		, WP_FALSE, WP_FALSE, 0 },
{ WithArg, "-start_addr"	, WP_FALSE, WP_FALSE, 2,{{ HexBaseType, 0, "", WP_FALSE },{ HexBaseType, 0, "", WP_TRUE } } },
{ WithArg, "-end_addr"		, WP_FALSE, WP_FALSE, 2,{{ HexBaseType, 0, "", WP_FALSE },{ HexBaseType, 0, "", WP_TRUE } } },
{ NoArg,   "-trace_all"		, WP_FALSE, WP_FALSE, 0 },
{ WithArg, "-serial"		, WP_FALSE, WP_FALSE, 5,{{ StringType , 0, "", WP_TRUE } ,{ StringType , 0, "", WP_TRUE },{ StringType, 0, "", WP_TRUE },{ StringType, 0, "", WP_TRUE },{ StringType, 0, "", WP_TRUE } } },
{ WithArg, "-trs"			, WP_FALSE, WP_FALSE, 1,{{ DecBaseType, 0, "", WP_FALSE } } },
{ WithArg, "-counter"		, WP_FALSE, WP_FALSE, 1,{{ DecBaseType, 0, "", WP_FALSE } } },
{ WithArg, "-et_counter"	, WP_FALSE, WP_FALSE, 1,{{ DecBaseType, 0, "", WP_FALSE } } },
{ WithArg, "-et_mask"		, WP_FALSE, WP_FALSE, 1,{{ HexBaseType, 0, "", WP_FALSE } } },
{ WithArg, "presc"			, WP_FALSE, WP_FALSE, 1,{{ DecBaseType, 0, "", WP_FALSE } } },
{ WithArg, "mask"			, WP_FALSE, WP_FALSE, 1,{{ DecBaseType, 0, "", WP_FALSE } } },
{ NoArg,   "dump"			, WP_FALSE, WP_FALSE, 0 },
{ NoArg,   "status"			, WP_FALSE, WP_FALSE, 0 },
{ WithArg, "-wp_no", WP_FALSE, WP_FALSE, 1, {{ DecBaseType, 0, "", WP_TRUE }}},
{ NoArg,   "-help"			, WP_FALSE, WP_FALSE, 0 },
{ NoArg,   "-sh"			, WP_FALSE, WP_FALSE, 0 } };

WP_status WPUI_PrintTraceStatus(WP_U32 wp_no, WPU_wf_info* winfarms);
WP_status WPUI_PrintTraceData(WP_U32 wp_no);
WP_status WPUI_CheckSerialName(WP_CHAR *arg, WP_U32 *polling_mask);

/*****************************************************************************
 * Function:    WPU_TraceBuffer
 * Description:
 * Inputs:
 * Outputs:
 * Return Value:
 *****************************************************************************/
#ifdef _ADDONS_
WP_S32 main(WP_S32 argc, WP_CHAR ** argv)
#else
WP_status WPU_TraceBuffer(WP_S32 argc, WP_CHAR ** argv)
#endif
{
	WP_U32 i, j;
	WPU_wf_info g_winfarms;
	WP_U32 wp_no = 0;


	if(WPUI_Generic_Parsing(Args, ARRAY_SIZE, argv, argc) != WP_OK)
	{
		WPU_DUMP("\nERROR: Parsing Failed\n");
		return WPI_BASIC_ERROR(WP_ERR_WINUTIL_INVALID_PARAMETER);
	}

	if(Args[HELP].is_present)
	{
		WPUI_Help(&tb_cmd, 1);
		return WP_OK;
	}

	if(Args[SHORT_HELP].is_present)
	{
		WPUI_Help(&tb_cmd, 0);
		return WP_OK;
	}

	if (Args[WINPATH_NUM].is_present)
   {

      wp_no = Args[WINPATH_NUM].arg_data[0].numeric_data;
      if (((WP_S32)wp_no) < 0 || ((WP_S32)wp_no) >  WPU_GetNumOfWinpaths()-1)
      {
         WPU_DUMP("ERROR: Invalid WinPath Number!");
         return WPI_BASIC_ERROR(WP_ERR_WINUTIL_INVALID_PARAMETER);
      }
   }


   WPU_WF(wp_no, 0, WPU_WF_CMD_GET_NUM_OF_ACTIVE_WF, &g_winfarms);
   if(Args[INIT].is_present)
   {
      memset(g_winfarms.trace_buffer, 0, sizeof(g_winfarms.trace_buffer));

      for(i = 0; i < g_winfarms.num_of_active_wf; i++)
      {

         g_winfarms.trace_buffer[i].trace_enable	= 1;
         g_winfarms.trace_buffer[i].trace_all 	= 0;
         g_winfarms.trace_buffer[i].wrap 				= (Args[DISABLE_WRAP].is_present)				 	? 0 : 1;
         g_winfarms.trace_buffer[i].trace_task_switch 	= (Args[DISABLE_TASK_SWITCH_TRACE].is_present) 		? 0 : 1;
         g_winfarms.trace_buffer[i].trace_change_of_flow = (Args[DISABLE_CHANGE_OF_FLOW_TRACE].is_present) 	? 0 : 1;
         g_winfarms.trace_buffer[i].trace_register 		= (Args[DISABLE_REGISTER_TRACE].is_present) 		? 0 : 1;
         g_winfarms.trace_buffer[i].sync_enable			= (Args[ENABLE_SYNC].is_present)					? 1	: 0;
         g_winfarms.trace_buffer[i].end_trace_en 		= (Args[DISABLE_END_TRACE].is_present)				? 0 : 1;
         g_winfarms.trace_buffer[i].stop_wg 				= ((Args[NO_STOP].is_present)||(Args[NO_STOP_WG].is_present)) ? 0 : 1;
         g_winfarms.trace_buffer[i].stop_trace 			= ((Args[NO_STOP].is_present)||(Args[NO_STOP_TRACE].is_present)) ? 0 : 1;

         g_winfarms.trace_buffer[i].addr_condition_en = 0;
         g_winfarms.trace_buffer[i].start_addr        = 0;
         g_winfarms.trace_buffer[i].end_addr			 = 0;
         g_winfarms.trace_buffer[i].start_addr_mask	 = 0;
         g_winfarms.trace_buffer[i].end_addr_mask	   = 0;
         g_winfarms.trace_buffer[i].serial_condition_en = 0;
         g_winfarms.trace_buffer[i].trs_condition_en  = 0;
         g_winfarms.trace_buffer[i].trs_number 		 = 0;
             g_winfarms.trace_buffer[i].end_trace_mask    = 0xff;

         g_winfarms.trace_buffer[i].end_trace_counter = 0;
         g_winfarms.trace_buffer[i].event_counter 	 = 0;

         if(Args[TRACE_ALL].is_present)
         {
            g_winfarms.trace_buffer[i].trace_all 	= 1;
         }

         if(Args[SERIAL].is_present)
         {

            WP_U32 serial_num, numOfOn = 1;

            g_winfarms.trace_buffer[i].serial_condition_en = 1;

            if(WPUI_CheckSerialName(Args[SERIAL].arg_data[0].string_data, &serial_num) != WP_OK)
            {
               WPU_DUMP("\nERROR: <%s> Not A Valid Serial Name\n", Args[SERIAL].arg_data[0].string_data);
               return WPI_BASIC_ERROR(WP_ERR_WINUTIL_INVALID_PARAMETER);
            }
            g_winfarms.trace_buffer[i].serials[0] = serial_num;

            for(j = 1; j < WPU_TRACE_NUM_OF_SERIAL; j++) /* Check for the rest of the serial numbers */
            {

               if(strcmp(Args[SERIAL].arg_data[j].string_data,"") == 0 || Args[SERIAL].num_of_data_args < j )
               {

                  g_winfarms.trace_buffer[i].serials[j] = TRACE_OFF;
                  break;
               }
               else
               {
                  numOfOn++;
                  if(WPUI_CheckSerialName(Args[SERIAL].arg_data[j].string_data, &serial_num) != WP_OK)
                  {
                     WPU_DUMP("\nERROR: <%s> Not A Valid Serial Name\n", Args[SERIAL].arg_data[j].string_data);
                     return WPI_BASIC_ERROR(WP_ERR_WINUTIL_INVALID_PARAMETER);
                  }
                  g_winfarms.trace_buffer[i].serials[j] = serial_num;
               }
            }


            /*in wp3: if the number of serials to trace is more than 2, we need to make sure that
              the third is only cfu serial*/
            if (numOfOn>2)
            {
               CHECK_WP3_SERIAL_ORDER
            }

            for(; j < WPU_TRACE_NUM_OF_SERIAL; j++)
            {

               g_winfarms.trace_buffer[i].serials[j] = TRACE_OFF;
            }


         }


         /* checking if the 'trace all' condition was on */
         if(Args[SERIAL].is_present )
         {
            WPU_wf_info g_winfarmsTEMP;
            WPU_WF(wp_no, 0, WPU_WF_CMD_GET_TRACE_STATUS, &g_winfarmsTEMP);
            if (g_winfarmsTEMP.trace_buffer[i].trace_all)
            {
              WPU_DUMP("\nERROR: 'trace by serial' cannot be configured since 'trace all' was configured before.\n");
              return WPI_BASIC_ERROR(WP_ERR_WINUTIL_INVALID_PARAMETER);
            }
         }

         if((Args[START_ADDR].is_present) && (Args[END_ADDR].is_present))
         {
            g_winfarms.trace_buffer[i].addr_condition_en = 1;
            g_winfarms.trace_buffer[i].start_addr 		 = Args[START_ADDR].arg_data[0].numeric_data;
            g_winfarms.trace_buffer[i].end_addr 		 = Args[END_ADDR].arg_data[0].numeric_data;

            if((g_winfarms.trace_buffer[i].end_addr > ADDRESS_LIMIT) || (g_winfarms.trace_buffer[i].start_addr > ADDRESS_LIMIT))
            {
               WPU_DUMP("\nERROR: Address Is Above Limit");
               return WPI_BASIC_ERROR(WP_ERR_WINUTIL_INVALID_PARAMETER);
            }

            if(Args[START_ADDR].is_optional_present)
            {
               g_winfarms.trace_buffer[i].start_addr_mask = Args[START_ADDR].arg_data[1].numeric_data;
               if(g_winfarms.trace_buffer[i].start_addr_mask > MASK_LIMIT)
               {
                  WPU_DUMP("\nERROR: Mask Is Above Limit\n");
                  return WPI_BASIC_ERROR(WP_ERR_WINUTIL_INVALID_PARAMETER);
               }
            }

            if(Args[END_ADDR].is_optional_present)
            {
               g_winfarms.trace_buffer[i].end_addr_mask = Args[END_ADDR].arg_data[1].numeric_data;
               if(g_winfarms.trace_buffer[i].end_addr_mask > MASK_LIMIT)
               {
                  WPU_DUMP("\nERROR: Mask Is Above Limit\n");
                  return WPI_BASIC_ERROR(WP_ERR_WINUTIL_INVALID_PARAMETER);
               }
            }
         }

#ifdef TRACE_BY_PROTOCOL_ENABLE
      if(Args[PROTOCOL].is_present)
      {
        g_winfarms.trace_buffer[i].addr_condition_en = 1;

        WP_U32 value = 0;
        WP_U32 offset = Args[PROTOCOL].arg_data[0].numeric_data;
        WP_U32 index = WPI_CFU_DPS_IMAGE_ENTRYPTS_1 + i;

        WPUI_ReadFromWddi(0,index,offset,&value);

        g_winfarms.trace_buffer[i].start_addr 	 = value;
        g_winfarms.trace_buffer[i].end_addr 		 = 0xFFFF;

        if((g_winfarms.trace_buffer[i].end_addr > ADDRESS_LIMIT) || (g_winfarms.trace_buffer[i].start_addr > ADDRESS_LIMIT))
        {
         WPU_DUMP("\nERROR: Address Is Above Limit");
         return WPI_BASIC_ERROR(WP_ERR_WINUTIL_INVALID_PARAMETER);
        }

      }
#endif
         if(Args[SERIAL].is_present)
         {
            WP_U32 serial_num;

            g_winfarms.trace_buffer[i].serial_condition_en = 1;

            if(WPUI_CheckSerialName(Args[SERIAL].arg_data[0].string_data, &serial_num) == WP_NOT_A_SERIAL_NAME)
            {
               WPU_DUMP("\nERROR: <%s> Not A Valid Serial Name\n", Args[SERIAL].arg_data[0].string_data);
               return WPI_BASIC_ERROR(WP_ERR_WINUTIL_INVALID_PARAMETER);
            }
            g_winfarms.trace_buffer[i].serials[0] = serial_num;

            for(j = 1; j < WPU_TRACE_NUM_OF_SERIAL; j++) /* Check for the rest of the serial numbers */
            {
               if (strlen(Args[SERIAL].arg_data[j].string_data) == 0)
                  break;

               if(WPUI_CheckSerialName(Args[SERIAL].arg_data[j].string_data, &serial_num) == WP_NOT_A_SERIAL_NAME)
                  break;
               else
                  g_winfarms.trace_buffer[i].serials[j] = serial_num;
            }

            if( j < WPU_TRACE_NUM_OF_SERIAL) /*Disable unused traces*/
            {
              for(; j < WPU_TRACE_NUM_OF_SERIAL; j++)
               g_winfarms.trace_buffer[i].serials[j] = TRACE_OFF;
            }
            else /*If TSER #3 was set, send warning*/
               WPU_DUMP("\nWARNING: Trace Serial #3 may overwrite other traces\n");
         }

         if( Args[TRS].is_present)
         {
            g_winfarms.trace_buffer[i].trs_condition_en = 1;
            g_winfarms.trace_buffer[i].trs_number = Args[TRS].arg_data[0].numeric_data;

            if(g_winfarms.trace_buffer[i].trs_number >= WPI_TRS_SETS)
            {
               WPU_DUMP("\nERROR: Invalid parameter for tb init -trs %u: TRS above limit.\n", g_winfarms.trace_buffer[i].trs_number);
               return WPI_BASIC_ERROR(WP_ERR_WINUTIL_INVALID_PARAMETER);
            }
         }
         if( Args[EVENT_COUNTER].is_present)
         {
            g_winfarms.trace_buffer[i].event_counter = Args[EVENT_COUNTER].arg_data[0].numeric_data;
            if(g_winfarms.trace_buffer[i].event_counter >= COUNTER_LIMIT)
            {
               WPU_DUMP("\nERROR: Invalid parameter for tb init -counter %u: COUNTER above limit.\n", g_winfarms.trace_buffer[i].event_counter);
               return WPI_BASIC_ERROR(WP_ERR_WINUTIL_INVALID_PARAMETER);
            }
         }
         if( Args[ET_COUNTER].is_present)
         {
            g_winfarms.trace_buffer[i].end_trace_counter = Args[ET_COUNTER].arg_data[0].numeric_data;
            if(g_winfarms.trace_buffer[i].end_trace_counter >= ET_COUNTER_LIMIT)
            {
               WPU_DUMP("\nERROR: Invalid parameter for tb init -et_counter %u: ET_COUNTER above limit.\n", g_winfarms.trace_buffer[i].end_trace_counter);
               return WPI_BASIC_ERROR(WP_ERR_WINUTIL_INVALID_PARAMETER);
            }
         }
         if( Args[ET_MASK].is_present)
         {
            g_winfarms.trace_buffer[i].end_trace_mask = Args[ET_MASK].arg_data[0].numeric_data;
            if(g_winfarms.trace_buffer[i].end_trace_mask >= ET_MASK_LIMIT)
            {
               WPU_DUMP("\nERROR: Invalid parameter for tb init -et_mask %u: ET_MASK above limit.\n", g_winfarms.trace_buffer[i].end_trace_mask);
               return WPI_BASIC_ERROR(WP_ERR_WINUTIL_INVALID_PARAMETER);
            }
         }

         /* checking if the user want to turn on the 'trace all' condition and 'by serial'
          condition at the same init command*/
         if (g_winfarms.trace_buffer[i].serial_condition_en && g_winfarms.trace_buffer[i].trace_all)
         {
            WPU_DUMP("\nERROR: cannot activate the serial condition if trace all was set.\n");
            return WPI_BASIC_ERROR(WP_ERR_WINUTIL_INVALID_PARAMETER);
         }
      }
      WPU_WF(wp_no, 0, WPU_WF_CMD_TRACE_REGS_INIT, &g_winfarms);
   }
   else
   {
      if(Args[PRESC].is_present)
      {
         if(Args[PRESC].arg_data[0].numeric_data)
         {
            g_winfarms.prescalar = Args[PRESC].arg_data[0].numeric_data;
         }
         else
         {
            WPU_DUMP("\nERROR: Invalid Pre-Scalar Value %d\n", Args[PRESC].arg_data[0].numeric_data);
            return WPI_BASIC_ERROR(WP_ERR_WINUTIL_INVALID_PARAMETER);
         }

         WPU_WF(wp_no, 0, WPU_WF_CMD_SET_PRESCALAR, &g_winfarms);
      }
      else
      {
         if(Args[TB_STATUS].is_present)
         {
            WPUI_PrintTraceStatus(wp_no, &g_winfarms);
         }
         else
         {

            if(Args[DUMP].is_present)
            {
               WPUI_PrintTraceData(wp_no);
            }
            else
            {
               WPU_DUMP("\nERROR: No Arguments Were Typed !\n");
               return WPI_BASIC_ERROR(WP_ERR_WINUTIL_INVALID_PARAMETER);
            }
         }
      }
   }

	return WP_OK;
}

/*****************************************************************************
 * Function:    WPUI_PrintTraceData
 * Purpose: dumps trace data for a winfarm
 * Inputs: WP_S32 winfarm_no
 * Outputs: none
 * Return Value: none
 *****************************************************************************/
WP_status WPUI_PrintTraceData(WP_U32 wp_no)
{
	WP_U32 wf;
	WP_U32 i,j;
	WPU_wf_info g_winfarms;

	WP_CHAR chip_rev_name[256];
	WP_CHAR* chip_name_ptr = chip_rev_name;


	WPU_WF(wp_no, 0, WPU_WF_CMD_GET_NUM_OF_ACTIVE_WF, &g_winfarms);
	WPU_WF(wp_no, 0, WPU_WF_CMD_GET_VERSION, &g_winfarms);
	WPU_WF(wp_no, 0, WPU_WF_CMD_GET_TRACE, &g_winfarms);
	WPU_WF(wp_no, 0, WPU_WF_CMD_GET_PRESCALAR, &g_winfarms);


	chip_name_ptr = WPUI_ChipRevisionName(wp_no, chip_name_ptr);

	if (strcmp(chip_name_ptr, "unknown") == 0)
		strcpy(chip_name_ptr, REV);

	WPU_DUMP(TDUMP_OPEN_TAG_FORMAT, chip_name_ptr, g_winfarms.prescalar);

	for(wf = 0; wf < g_winfarms.num_of_active_wf; wf++)
	{
		WPUI_SetDtcrSaved(wp_no,wf);
		WPU_DUMP(WF_OPEN_TAG_FORMAT_B,wf,WPUI_GetDtcrSaved(wp_no,wf)[0],WPUI_GetDtcrSaved(wp_no,wf)[1],WPUI_GetDtcrSaved(wp_no,wf)[2],WPUI_GetDtcrSaved(wp_no,wf)[3],g_winfarms.version[wf].dps_build);

		for(i = 0; i < WPU_TRACE_BUFFER_SIZE; i += COL_NUMBER)
		{
			for(j = 0; j < COL_NUMBER; j++)
			{
				WP_U32 *ptr = (WP_U32 *) (&(g_winfarms.trace_buffer[wf].buffer[i + j]));
				WPU_DUMP( "%2x%08x ", ptr[1],ptr[0]);
			}
			WPU_DUMP("\n");
		}
		WPU_DUMP(WF_CLOSE_TAG_FORMAT);
	}
	WPU_DUMP(TDUMP_CLOSE_TAG_FORMAT);
	return WP_OK;

}

#ifdef TRACE_BY_PROTOCOL_ENABLE
void WPUI_ReadFromWddi(WP_U32 wpid, WP_U32 index, WP_U32 offset, WP_U32* value)
{
	WPU_ta_info ta;
	WPU_mem_info mem;
	WP_U32 indexAddr = 0;
	WP_U32 dataAddr = 0;
	WP_U8 arr[16] = {0};
	/*get ta base 255, retrieve index*/
	WPU_TA( wpid, (void*)WPI_TA_B_WDDI_ACCESS, WPU_TA_CMD_GET_ONE_BASE, &ta);
	indexAddr = ((ta.bases[WPI_TA_B_WDDI_ACCESS]&0xFFFFFFF0)+WPI_DPS_ACCESS_SIZE);
	indexAddr += index*4;
	mem.addr = indexAddr;
	mem.size = 4;
	mem.array = arr;
	/*Move to wanted index, get data address*/
	WPU_MEM(wpid,0,WPU_MEM_CMD_READ_PHYSICAL,&mem);
	dataAddr = ((WP_U32 *)mem.array)[0];
	/*Move by specified offset, get data*/
	dataAddr  += offset*4;
	WPUI_ReadWord(dataAddr,value);
}
#endif
