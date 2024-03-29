/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *  Application: Automatic Protection Switch
 *  File Name   : auto_protection_switch_util.c
 *  Version     : 1
 *  Date        : 20 Jan 2012
 *  Accompanies : TBD
 *****************************************************************************/

/****************************************************************************************************************************
* Function name: App_DataSend()
* Description  : Send a Ethernet packet on the channel handle passed in 
*
* Input  params:  h_tx:   enet1 host tx
*                 h_pool: memory pool buffer
* Output params: 
* Return val   : none
*****************************************************************************************************************************/
void App_DataSend (WP_handle h_tx, WP_handle h_pool)
{

#define APP_DATA_LENGTH 80
    WP_data_unit data_unit = {
        0
    };
    WP_data_segment bufs = {
        0
    };
    WP_status status;
    WP_U8 *km_data_ptr;
    WP_U32 ii;
    WP_U8 dst_mac[6] = {
        0x00, 0x00, 0x22, 0x33, 0x12, 0x06
    };
    WP_U8 src_mac[6] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x02
    };
    WP_U8 vlan_1[4] = {
        0x81, 0x00, 0x00, 0xa
    };
    WP_U8 e_type[2] = {
        0x08, 0x00
    };                          /* IP type */
    WP_U32 l2_header_len =
        sizeof (dst_mac) + sizeof (src_mac) + sizeof (vlan_1) +
        sizeof (e_type);
    WP_U32 ip_header_len = 20;
    WP_U32 payload_len;
    WP_U32 offset;

    bufs.buffer_class = 0;
    bufs.displacement = 0;
    bufs.user_info = 0;
    bufs.next = NULL;
    bufs.pool_handle = h_pool;
    bufs.data_size = APP_DATA_LENGTH;
    data_unit.type = WP_DATA_ENET;
    data_unit.n_active = 1;
    data_unit.data_size = bufs.data_size;
    data_unit.segment = &bufs;
    data_unit.n_segments = 1;
    data_unit.status = 0;
    data_unit.control = 0;      /* Tx protocol specific settings    */
    km_data_ptr = WP_PoolAlloc (h_pool);
    bufs.data = km_data_ptr;
    if (bufs.data == NULL)

    {
        printf ("Pool Alloc failure ! \n");
    }
    WP_MEM_BYTES_FILL (km_data_ptr, 0, pool_buffer_data_iw_cfg.size);
    offset = 0;
    memcpy (km_data_ptr + offset, dst_mac, sizeof (dst_mac));
    offset += sizeof (dst_mac);
    memcpy (km_data_ptr + offset, src_mac, sizeof (src_mac));
    offset += sizeof (src_mac);
    memcpy (km_data_ptr + offset, vlan_1, sizeof (vlan_1));
    offset += sizeof (vlan_1);
    memcpy (km_data_ptr + offset, e_type, sizeof (e_type));
    offset += sizeof (e_type);
    payload_len = APP_DATA_LENGTH - l2_header_len - ip_header_len;

    /* Outer IP header */
    insert_ip_header (0x0a0a0a0a, 0xc0a8fe11, payload_len, 4, offset,
                      km_data_ptr);
    offset += ip_header_len;
    memset (km_data_ptr + offset, 0xa, payload_len);
    printf ("sending packet( ethernet vlan IP) of length %d\n",
            APP_DATA_LENGTH);
    for (ii = 0; ii < APP_DATA_LENGTH; ii++)

    {
        printf ("%.2x", *(km_data_ptr + ii));
        if ((ii != 0) && !(ii % 40))
            printf ("\n");
    }
    printf ("\n");

    /* Use interworking qnode and interworking buffer pool for Gige */
    status = WP_HostSend (h_tx, &data_unit);
    App_TerminateOnError (status, "WP_HostSend()");
    WP_Delay (500000);
    WPI_SimulateInterrupts ();
}

/****************************************************************************************************************************
* Function name: add_task()
* Description  : Event handling. adding task in Q
*
* Input  params: 
* Output params: 
* Return val   : none
*****************************************************************************************************************************/
void add_task (app_task_list * task_list, WP_U32 event_type,
               WP_tag event_tag, void *event_param)
{
    WP_U16 tail = task_list->tail;
    WP_U32 next = tail + 1;

    if (next == task_list->num_elements)
        next = 0;
    if (next != task_list->head)
    {
        task_list->task[tail].event_tag = event_tag;
        task_list->task[tail].event_type = event_type;
        task_list->task[tail].event_param = event_param;
        task_list->tail = next;
    }
}

/****************************************************************************************************************************
* Function name: next_task()
* Description  : Get next task from Queue
*
*
* Input  params: none
* Output params: 
* Return val   : none
*****************************************************************************************************************************/

/* Event handling */
app_task *next_task (app_task_list * task_list, app_task * result)
{
    WP_U32 head = task_list->head;
    WP_U32 tail = task_list->tail;

    if (head == tail)
        return NULL;
    *result = task_list->task[head];
    if (++head == task_list->num_elements)
        head = 0;
    task_list->head = head;
    return result;
}

/****************************************************************************************************************************
* Function name: app_perform_action()
* Description  : print event based on interrupt    
*
*
* Input  params: 
* Output params: 
* Return val   : none
*****************************************************************************************************************************/
void app_perform_action (app_task * task)
{
    WP_U32 tag = task->event_tag;

    switch (task->event_type)

    {
    case WP_EVENT_RX_INDICATE:

        {
            printf ("Rx event on tag %d \n", tag);
            if (tag == tag_enet1_rx)

            {
                printf ("receiving on enet1\n");
                App_DuReceive (h_enet1_rx, WP_DATA_ENET);
            }

            else if (tag == tag_enet2_rx)

            {
                printf ("receiving on enet2\n");
                App_DuReceive (h_enet2_rx, WP_DATA_ENET);
            }

            else if (tag == tag_agg_default_bridge1)

            {
                printf ("receiving on default bridge1\n");
                App_DuReceive (h_bridge1_iwhost, WP_DATA_IW);
            }

            else if (tag == tag_agg_default_bridge2)

            {
                printf ("receiving on default bridge2\n");
                App_DuReceive (h_bridge2_iwhost, WP_DATA_IW);
            }

            else if (tag == tag_agg_lcp)

            {
                printf ("receiving on PPP LCP aggregation\n");
                App_DuReceive (h_bridge2_iwhost, WP_DATA_IW);
            }

            else if (tag == tag_agg_control)

            {
                printf ("receiving on Cisco Control Packet Aggregation\n");
                App_DuReceive (h_bridge2_iwhost, WP_DATA_IW);
            }

            else if ((tag >= tag_hdlcrx)
                     && (tag <= tag_hdlcrx + APP_MAX_HDLC_PORTS))

            {
                printf ("receiving on hdlc %d\n", tag - tag_hdlcrx);
                App_DuReceive (h_hdlc_rx[tag - tag_hdlcrx],
                               WP_DATA_PPP_HDLC);
            }

            else

            {
                printf ("RX event on unknown tag  %d\n", tag);
            }
            break;
        }
    case WP_EVENT_TX_INDICATE:

        {
            printf ("TX event on tag %d\n", tag);
            break;
        }
    case WP_EVENT_STATUS_INDICATE:
        break;
    default:
        printf ("Not processing unknown event\n");
    }
}

/****************************************************************************************************************************
* Function name: App_DuReceive()
* Description  : Receives a packet on a RX channel 
*
*
* Input  params: 
* Output params: 
* Return val   : none
*****************************************************************************************************************************/
void App_DuReceive (WP_handle h_rx, WP_U32 data_type)
{
    WP_data_unit data_unit;
    WP_data_segment data_segment, *curr_buff_ptr;
    WP_U32 kk;
    WP_status status;

    data_unit.segment = &data_segment;
    data_unit.n_segments = 1;
    data_unit.type = data_type;
    data_unit.n_active = 0;
    status = WP_HostReceive (h_rx, &data_unit);
    App_TerminateOnError (status, "WP_HostReceive");
    if (data_unit.n_active > 0)

    {
        curr_buff_ptr = data_unit.segment;
        for (kk = 0; kk < curr_buff_ptr->data_size; kk++)

        {
            printf ("%2.2x", curr_buff_ptr->data[kk]);
        }
        printf ("( %d bytes )\n", curr_buff_ptr->data_size);
    }
}

void App_EventRxIndicate (WP_tag tag, WP_U32 data, WP_U32 info)
{
    add_task (irq_task_list, WP_EVENT_RX_INDICATE, tag, NULL);
}

void App_EventTxIndicate (WP_tag tag, WP_U32 data, WP_U32 info)
{
    add_task (irq_task_list, WP_EVENT_TX_INDICATE, tag, NULL);
}

void App_EventErrorIndicate (WP_tag tag, WP_U32 event, WP_U32 data)
{
    add_task (irq_task_list, WP_EVENT_STATUS_INDICATE, tag, NULL);
}

/****************************************************************************************************************************
* Function name: App_Quit()
* Description  :      
 *
*
* Input  params: none
* Output params: 
 * Return val   : none
*****************************************************************************************************************************/

void WT_Reboot(void)
{
#if !defined(__linux__)
/* 
 * Rebooting is specific to board and target.  The test
 * directory should not be concerned how this is done.
 * Instead, it calls a board-specific function. 
 */
   if (WPI_REBOOT_ENABLE) {
      printf("Test Rebooting winmon by WT_Reboot\nWinMon>\n");
      WPX_Reboot();
   }
   else
      printf("Not Rebooting with WT_Reboot\nWinMon>\n");
#endif
}

void App_Quit(WP_U32 status)
{
   WP_DriverRelease();
   
   if (status)
   {
      WT_Reboot();
   }   
   
   exit(0);
}


/****************************************************************************************************************************
* Function name: App_TerminateOnError()
* Description  :      
*
*
* Input  params: 
* Output params: 
* Return val   : none
*****************************************************************************************************************************/
void App_TerminateOnError (WP_handle handle, WP_CHAR * s)
{
    if (WPI_SimulateInterrupts (), WP_ERROR_P (handle))
    {
        printf ("Test Abort %s %s 0x%x\n", s,
                WP_error_name[WP_ERROR (handle)], handle);

        /* Release WDDI */
        WP_DriverRelease ();
        exit (1);
    }

    else

    {
        if (handle == WP_OK)
            printf ("Status returned from %s : WP_OK\n", s);

        else
            printf ("Handle returned from %s is %#8.8x\n", s, handle);
    }
}

char framer_errmsg[][80];

/****************************************************************************************************************************
* Function name: App_ShowStats()
* Description  : Create menu for showing different stats according to user input 
*
*
* Input  params: none
* Output params: 
* Return val   : none
*****************************************************************************************************************************/
void App_ShowStats (void)
{
    WP_CHAR tmp[20];
    WP_U32 ch;

    do

    {
        printf ("\nMENU(press letter, no need return)");
        printf ("\nEnter 1 to print Device Stats");
#ifdef REMOVE_DULL_STATS
#else
        printf ("\nEnter 2 to print Bport  Stats");
        printf ("\nEnter 3 to print FlwAgg Stats");
        printf ("\nEnter 4 to print All Stats");
#endif
        printf ("\nEnter x to previous menu");
        printf ("\n");

#if 0
        gets (tmp);
        sscanf (tmp, "%d", &ch);

#else /*  */
        memset (tmp, 0, 20);
        ch = getchar ();
#endif /*  */
        switch (ch)

        {
        case '1':
            App_ShowDeviceStats ();
            break;
#ifdef REMOVE_DULL_STATS
#else
        case '2':
            App_ShowBportStats ();
            break;
        case '3':
            App_ShowFlowAggStats ();
            break;
        case '4':
            App_ShowDeviceStats ();
            App_ShowBportStats ();
            App_ShowFlowAggStats ();
            break;
#endif
        case 'x':
            return;
            break;
        default:
            printf ("Invalid Entry\n");
            break;
        }
    }
    while (1);
}



/****************************************************************************************************************************
* Function name: App_Debug()
* Description  :  this function is supported for future use
*
* Input  params: 
* Output params: 
* Return val   : none
*****************************************************************************************************************************/

void App_Debug(WP_boolean debug)
{
   WP_flow_class_rule flow_class_rule;
   WP_flow_class_forwarding_action forwarding_action;

   if(debug)
   {
      printf("Debug is on. Turning it OFF\n");
      forwarding_action.flow_aggregation = h_flow_agg_hdlc[1];
      debug_on = WP_FALSE;
   }
   else
   {
      printf("Debug is off. Turning it ON\n");
      forwarding_action.flow_aggregation = h_bridge1_default_agg;
      debug_on = WP_TRUE;
   }

   flow_class_rule.match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
   flow_class_rule.action_info.forwarding_action = forwarding_action;
   flow_class_rule.wred_entry = 0;

   status = WP_IwFlowModify(h_iw_sys_bridge1, h_bridge1_dfcflow1[0][0], WP_IW_MODIFY_WRITE_THROUGH, 
                            WP_IW_FLOW_MOD_FLOW_AGG, &flow_class_rule);
   App_TerminateOnError(status," WP_IwFlowModify() DFC");

   status = WP_IwSystemBuild(h_iw_sys_bridge1);
   App_TerminateOnError(status," WP_IwSystemBuild()");
}



/****************************************************************************************************************************
* Function name: insert_ip_header()
* Description  : Common routine to select IP Address  and forming IP packet    
*
*
* Input  params: 
            ip_sa : ip source address
            ip_da: ip destination address
            payload_size: ip payload size
            protocol: 
            offset:  the pointer offset from where payload will begin
            header: 
* Output params:  header:  IP header formed
* Return val   : none
*****************************************************************************************************************************/
void insert_ip_header (WP_U32 ip_sa, WP_U32 ip_da, WP_U32 payload_size,
                       WP_U32 protocol, WP_U32 offset, WP_U8 * header)
{
    WP_U32 packet_length;
    WP_U32 temp_sum, i;
    WP_U32 checksum = 0;
    WP_U8 *ptr_ip_data;
    WP_U8 *start_ip_data;

    ptr_ip_data = header + offset;
    start_ip_data = ptr_ip_data;

    /* Reset IP header just in case it has garbage */
    memset (ptr_ip_data, 0, IP_HEADER_SIZE);

    /* IP Version 4, header length = 20 bytes */
    ptr_ip_data[IP_VER_HLEN] = 0x45;

    /* Calculate new Packet length */
    packet_length = payload_size + IP_HEADER_SIZE;
    ptr_ip_data[IP_LENGTH] = (WP_U8) (packet_length >> 8);
    ptr_ip_data[IP_LENGTH + 1] = (WP_U8) (packet_length & 0xff);

    /* Time to Live */
    ptr_ip_data[IP_TTL] = 64;

    /* Protocol */
    ptr_ip_data[IP_PROTOCOL] = protocol;

    /* Get new ip source address */
    ptr_ip_data[IP_SA] = (WP_U8) (ip_sa >> 24);
    ptr_ip_data[IP_SA + 1] = (WP_U8) (ip_sa >> 16);
    ptr_ip_data[IP_SA + 2] = (WP_U8) (ip_sa >> 8);
    ptr_ip_data[IP_SA + 3] = (WP_U8) (ip_sa & 0xff);

    /* Get new ip destination address */
    ptr_ip_data[IP_DA] = (WP_U8) (ip_da >> 24);
    ptr_ip_data[IP_DA + 1] = (WP_U8) (ip_da >> 16);
    ptr_ip_data[IP_DA + 2] = (WP_U8) (ip_da >> 8);
    ptr_ip_data[IP_DA + 3] = (WP_U8) (ip_da & 0xff);
    ptr_ip_data[IP_SER_TYPE] = 0x00;    //0x08; //0x0; // 0x04;  //0x0c; // 0x08;

    /* Calculate header checksum */
    for (i = 0; i < (IP_HEADER_SIZE / 2); i++)

    {
        temp_sum = (ptr_ip_data[0] << 8 | ptr_ip_data[1]);
        checksum += temp_sum;
        ptr_ip_data += 2;
    }
    while (checksum & 0xffff0000)
        checksum = (checksum & 0xffff) + ((checksum & 0xffff0000) >> 16);
    checksum = ~checksum;

    /* Save calculated checksum value */
    start_ip_data[IP_CHECKSUM] = checksum >> 8;
    start_ip_data[IP_CHECKSUM + 1] = checksum & 0xff;
}

/* all 103 item */
char framer_errmsg[][80] = 
{
"WPX_UFE_FRAMER_OK",
"WPX_UFE_FRAMER_ERROR_DEVICE_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_LINE_PORT_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_DISCRETE_CLIENT_PORT_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_SOCKET_CLIENT_PORT_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_BANDWIDTH_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_iJ_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_iK_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_iL_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_iM_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_iU_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_iV_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_iW_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_DEVICE_MODE_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_PORT_RATE_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_TOO_MANY_CHARS_IN_STRING",
"WPX_UFE_FRAMER_ERROR_BANDWIDTH_NOT_SUPPORTED",
"WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST",
"WPX_UFE_FRAMER_ERROR_FACILITY_ALREADY_EXISTS",
"WPX_UFE_FRAMER_ERROR_INVALID_FACILITY",
"WPX_UFE_FRAMER_ERROR_FACILITY_PART_OF_CONNECTION",
"WPX_UFE_FRAMER_ERROR_CONNECTION_BLOCKED_BY_PARENT",
"WPX_UFE_FRAMER_ERROR_CONNECTION_BLOCKED_BY_CHILD",
"WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_MISMATCH",
"WPX_UFE_FRAMER_ERROR_CONNECTION_ALREADY_EXISTS",
"WPX_UFE_FRAMER_ERROR_CONNECTION_DOES_NOT_EXIST",
"WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_ALREADY_CONNECTED",
"WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_NOT_CONNECTED",
"WPX_UFE_FRAMER_ERROR_C2_VALUE_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_S1_VALUE_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_K1_VALUE_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_K2_VALUE_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_BIP_THRESHOLD_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_PATH_TRACE_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_PRBS_PATTERN_TYPE_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_CLIENT_TYPE_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_CLIENT_FRAMING_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_DEVICE_CAPABILITY_NOT_DEFINED",
"WPX_UFE_FRAMER_ERROR_DEVICE_CAPABILITY_DOES_NOT_SUPPORT_DISCRETE_CLIENT_E1T1",
"WPX_UFE_FRAMER_ERROR_ALARM_CATEGORY_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_DEVICE_NOT_READY",
"WPX_UFE_FRAMER_ERROR_VC4_4C_J_INCORRECT",
"WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER",
"WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PARENT",
"WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_CHILD",
"WPX_UFE_FRAMER_ERROR_FACILITY_PARENT_DOES_NOT_EXIST",
"WPX_UFE_FRAMER_ERROR_FACILITY_PARENT_IS_CONNECTED",
"WPX_UFE_FRAMER_ERROR_STS_U_INCORRECT",
"WPX_UFE_FRAMER_ERROR_BUILD_OPTION_PREVENTS_FACILITY_CREATION",
"WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH",
"WPX_UFE_FRAMER_ERROR_BANDWIDTH_NOT_SUPPORTED_IN_DEVICE_MODE",
"WPX_UFE_FRAMER_ERROR_RATE_NOT_SUPPORTED_IN_DEVICE_MODE",
"WPX_UFE_FRAMER_ERROR_PORT_RATE_NOT_CONFIGURED",
"WPX_UFE_FRAMER_ERROR_BANDWIDTH_EXCEEDS_PORT_RATE",
"WPX_UFE_FRAMER_ERROR_DEVICE_CAPABILITY_DOES_NOT_SUPPORT_SONET_SDH",
"WPX_UFE_FRAMER_ERROR_FEATURE_NOT_SUPPORTED",
"WPX_UFE_FRAMER_ERROR_COMMON_INTERFACE",
"WPX_UFE_FRAMER_ERROR_CLIENT_FRAMING_INVALID_FOR_CLIENT_TYPE",
"WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE",
"WPX_UFE_FRAMER_ERROR_CLOCK_RECOVERY_MODE_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_CLOCK_RECOVERY_MODE_INVALID_FOR_DISCRETE_CLIENT",
"WPX_UFE_FRAMER_ERROR_POINT_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_NOT_IMPLEMENTED",
"WPX_UFE_FRAMER_ERROR_CLIENT_APPLICATION_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_CLIENT_FRAMING_INVALID_FOR_CLIENT_APPLICATION_MODE",
"WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_APPLICATION_MODE",
"WPX_UFE_FRAMER_ERROR_CAS_INVALID_FOR_CLIENT_APPLICATION_MODE",
"WPX_UFE_FRAMER_ERROR_API_NO_LONGER_SUPPORTED",
"WPX_UFE_FRAMER_ERROR_PORT_HAS_FACILITIES",
"WPX_UFE_FRAMER_ERROR_CLIENT_TIMESTAMP_BYTE_COUNT_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_UNDEFINED",
"WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_NOT_RECOGNISED",
"WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_INCONSISTENT_WITH_BUILD_VARIANT",
"WPX_UFE_FRAMER_ERROR_BUILD_VARIANT_UNDEFINED",
"WPX_UFE_FRAMER_ERROR_FACILITIES_EXIST",
"WPX_UFE_FRAMER_ERROR_PROTECTION_PORT_CANNOT_BE_CONFIGURED",
"WPX_UFE_FRAMER_ERROR_V5_LABEL_VALUE_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_POINT_IS_ALREADY_MONITORED",
"WPX_UFE_FRAMER_ERROR_POINT_IS_NOT_MONITORED",
"WPX_UFE_FRAMER_ERROR_CLIENT_REQUEST_NOT_SUPPORTED_FOR_FRAMED_CLIENTS",
"WPX_UFE_FRAMER_ERROR_CLIENT_PORT_NOT_MAPPED_TO_LINE_ENDPOINT",
"WPX_UFE_FRAMER_ERROR_PORT_THIS_PORT_RATE_ALREADY_CONFIGURED",
"WPX_UFE_FRAMER_ERROR_THROUGH_CONNECTED_ENDPOINT_PREVENTS_CONFIGURATION_CHANGE",
"WPX_UFE_FRAMER_ERROR_TIMESTAMP_GAIN_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_SOCKET_CLIENT_OUTPUT_SELECTOR_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_PM_POINT_UNAVAILABLE",
"WPX_UFE_FRAMER_PM_M_OF_N_LIMIT_REACHED",
"WPX_UFE_FRAMER_ERROR_PM_TABLE_INDEX_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_DEVICE_PROTECTION_MODE_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_DEVICE_PROTECTION_MODE_NOT_COMPATIBLE_WITH_PERSONALITY",
"WPX_UFE_FRAMER_ERROR_PORT_RATE_IS_CONFIGURED",
"WPX_UFE_FRAMER_ERROR_PERSONALITY_PREVENTS_PORT_RATE",
"WPX_UFE_FRAMER_ERROR_SOCKET_CLIENT_RATE_PARAMETER_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT",
"WPX_UFE_FRAMER_ERROR_SDH_OUTPUT_SELECTOR_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_DEVICE_MODE_PREVENTS_REQUEST",
"WPX_UFE_FRAMER_ERROR_DCC_SETTING_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_DEVICE_CARD_PROTECTION_OPERATING_MODE_OUT_OF_RANGE",
"WPX_UFE_FRAMER_ERROR_DEVICE_CARD_PROTECTION_OPERATING_MODE_NOT_APPLICABLE",
"WPX_UFE_FRAMER_ERROR_CLIENT_TYPE_NOT_SUPPORTED",
"WPX_UFE_FRAMER_ERROR_DEVICE_TEST_MODE_PREVENTS_REQUEST"
};





