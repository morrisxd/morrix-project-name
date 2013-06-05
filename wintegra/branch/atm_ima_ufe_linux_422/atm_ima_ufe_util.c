/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2012.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *  Appplication: ATM IMA UFE
 *  File Name   : atm_ima_ufe_util.c
 *
 *****************************************************************************/

/*****************************************************************************
 * Function name: App_DataSend
 * Description  : Send a Ethernet packet on the channel handle passed in 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/

extern WP_U32 pkts_lock;

void App_DataSend (WP_handle h_tx, WP_handle h_pool)
{
#define APP_DATA_LENGTH 80

   WP_data_unit data_unit = { 0 };
   WP_data_segment bufs = { 0 };
   WP_status status;
   WP_U8 *km_data_ptr;
   WP_U32 ii;                   //,packet_type;
   WP_U8 dst_mac[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a };
   WP_U8 src_mac[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 };
   WP_U8 mpls_header[4] = { 0x00, 0x01, 0x31, 0x64 }; // mpls header
   //WP_U8 mpls_header[4] = {0x00,0x01,0x41,0x64}; // mpls header
   WP_U8 e_type[2] = { 0x88, 0x47 };   // MPLS
   WP_U32 l2_header_len =
      sizeof (dst_mac) + sizeof (src_mac) + sizeof (mpls_header) +
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

   km_data_ptr = WP_PoolAlloc (h_pool);
   bufs.data = km_data_ptr;
   if (bufs.data == NULL)
   {
      printf ("Pool Alloc failure ! \n");
   }

   WP_MEM_BYTES_FILL (km_data_ptr, APP_DATA_LENGTH,
                      pool_buffer_data_iw_cfg.size);
   //WP_MEM_BYTES_FILL(km_data_ptr, 0, pool_buffer_data_iw_cfg.size);
   offset = 0;
   memcpy (km_data_ptr + offset, dst_mac, sizeof (dst_mac));
   offset += sizeof (dst_mac);
   memcpy (km_data_ptr + offset, src_mac, sizeof (src_mac));
   offset += sizeof (src_mac);
   memcpy (km_data_ptr + offset, e_type, sizeof (e_type));
   offset += sizeof (e_type);

   memcpy (km_data_ptr + offset, mpls_header, sizeof (mpls_header));
   offset += sizeof (mpls_header);

   payload_len = APP_DATA_LENGTH - l2_header_len - ip_header_len;

   /* Outer IP header */
   insert_ip_header (0x0a0a0a0a, 0xc0a8fe11, payload_len, 4, offset,
                     km_data_ptr);
   offset += ip_header_len;

   memset (km_data_ptr + offset, 0xa, payload_len);

#if 0
   printf ("sending packet of length %d\n", APP_DATA_LENGTH);
#endif
   for (ii = 0; ii < APP_DATA_LENGTH; ii++)
   {
#if 0
      printf ("%.2x", *(km_data_ptr + ii));
      if ((ii != 0) && !(ii % 40))
         printf ("\n");
#endif
   }
#if 0
   printf ("\n");
#endif
   /* Use interworking qnode and interworking buffer pool for Gige */
   status = WP_HostSend (h_tx, &data_unit);
   App_TerminateOnError (status, "WP_HostSend()");
   WP_Delay (50);

   WPI_SimulateInterrupts ();
}

/*****************************************************************************
 * Function name: add_task
 * Description  : Event handling , added task in the list
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void add_task (app_task_list * task_list, WP_U32 event_type,
               WP_tag event_tag, void *event_param)
{
   WP_U16 tail = task_list->tail;
   WP_U32 next = tail + 1;

   printf ("add_task: ready to lock\n");
   fflush (stdout);
   // WPL_Lock (WPL_THREAD_LOCK_KEY, &pkts_lock);

   printf ("add_task: ready to lock-2\n");
   fflush (stdout);
   if (next == task_list->num_elements)
      next = 0;

   printf ("add_task: ready to lock-3\n");
   fflush (stdout);
   if (next != task_list->head)
   {
      task_list->task[tail].event_tag = event_tag;
      task_list->task[tail].event_type = event_type;
      task_list->task[tail].event_param = event_param;
      task_list->tail = next;
   }

   WPL_Unlock (WPL_THREAD_LOCK_KEY, &pkts_lock);
   printf ("event inserted into queue list\n");
   fflush (stdout);
}

/*******************************************************************************
 * This function adds a task to a task queue.
 * Inputs
 *   task_list:    pointer to the task list
 *   tag:          the task applies to the channel with this tag
 *   action        the action to be applied
 *
 * Output   none
 * Return value  none
 *
 ******************************************************************************/
void ima_add_task (ima_app_task_list * task_list, WP_U32 data, WP_U32 info)
{
   WP_U32 in = task_list->in;
   WP_U32 out = task_list->out;
   WP_U32 size = task_list->size;
   WP_U32 inp = in + 1;

   if (inp == size)
      inp = 0;
   if (inp != out)
   {
      task_list->task[in].data = data;
      task_list->task[in].info = info;
      task_list->in = inp;
   }
}

/*****************************************************************************
 * Function name: next_task
 * Description	: Event handling , returning next task
 * Input  params: None
 * Output params: None
 * Return val	: None
 *****************************************************************************/
app_task *next_task (app_task_list * task_list, app_task * result)
{
   WP_U32 head = task_list->head;
   WP_U32 tail = task_list->tail;

   WPL_Lock (WPL_THREAD_LOCK_KEY, &pkts_lock);

   if (head == tail)
   {
      WPL_Unlock (WPL_THREAD_LOCK_KEY, &pkts_lock);
      return NULL;
   }

   *result = task_list->task[head];
   if (++head == task_list->num_elements)
      head = 0;
   task_list->head = head;

   // WPL_Unlock(WPL_THREAD_LOCK_KEY, &pkts_lock);

   return result;
}

extern int g_flag_thread;

/********************************************************************************
 * Function name: ima_next_task
 * This function gets a task from a task queue.
 * Inputs
 *   task_list:    pointer to the task list
 *   result:       pointer to where to copy task
 *
 *  Output
 *   *result:      the task
 *
 * Return value
 *   NULL if no tasks were on the list
 *   result if a task was on the list
 * Note
 * The task is copied from the task queue so that another
 * thread can immediately use the entry in the task queue.
 *********************************************************************************/
WP_ima_event *ima_next_task (ima_app_task_list * task_list,
                             WP_ima_event * result)
{
   WP_U32 in = task_list->in;
   WP_U32 out = task_list->out;

   if (out != in)
   {
      *result = task_list->task[out];
      out++;
      if (out == task_list->size)
         out = 0;
      task_list->out = out;
      return result;
   }
   if (!g_flag_thread)
   {
      printf ("ima_next_task: in(%18d),out(%18d)\r", in, out);
      fflush (stdout);
   }

   return NULL;
}

/*****************************************************************************
 * Function name: app_perform_action
 * Description  : Event handling
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
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
            printf ("receiving on enet2(event catch)\n");
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
         else if ((tag >= tag_tdmrx)
                  && (tag <= tag_tdmrx + APP_MAX_TDM_PORTS))
         {
            printf ("receiving on tdm %d\n", tag - tag_tdmrx);
            App_DuReceive (h_tdm_rx[tag - tag_tdmrx], WP_DATA_PPP_HDLC);
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

static char *ima_events_name[] = {
   "WP_IMA_EVENT_TTS 0",
   "WP_IMA_EVENT_TOS 1",
   "WP_IMA_EVENT_TXQEMPTY 2",
   "WP_IMA_EVENT_TXQFULL 3",
   "WP_IMA_EVENT_OPERATIONAL_CHANGE 4",
   "gap 5",
   "WP_IMA_EVENT_GSU_LASR_RX 6",
   "WP_IMA_EVENT_GSU_LASR_TX 7",
   "WP_IMA_EVENT_FE_ICP_CHANGE 8",
   "WP_IMA_EVENT_NE_ICP_CHANGE 9",
   "WP_IMA_EVENT_TICK 10",
   "WP_IMA_EVENT_PROPRIETARY 11",
   "WP_IMA_EVENT_BANDWIDTH_CHANGE 12"
};

/*******************************************************************************
 * Function name: ima_app_perform_action 
 * Description  : This function executes a task
 *
 * Inputs
 *   task:         pointer to the task to be executed.
 *
 * Output:   none
 * Return value  none
 ********************************************************************************/
void ima_app_perform_action (WP_ima_event * task)
{
   WP_status status;

   WP_U32 data;
   WP_U32 info;
   WP_U32 action;

   WP_U32 port;
   WP_U32 group;

   data = task->data;
   info = task->info;
   action = data & 0xff;

   group = (data >> 16) & 0xff;
   port = (data >> 8) & 0xff;

   switch (action)
   {
   case WP_IMA_EVENT_BANDWIDTH_CHANGE:
   case WP_IMA_EVENT_TXQEMPTY:
   case WP_IMA_EVENT_TTS:
   case WP_IMA_EVENT_OPERATIONAL_CHANGE:
   case WP_IMA_EVENT_FE_ICP_CHANGE:
   case WP_IMA_EVENT_NE_ICP_CHANGE:
   case WP_IMA_EVENT_PROPRIETARY:
   case WP_IMA_EVENT_GSU_LASR_TX:
   case WP_IMA_EVENT_GSU_LASR_RX:
   case WP_IMA_EVENT_TOS:
      // break;

   case WP_IMA_EVENT_TICK:

      status = WP_ImaEvent (ima_sys_handle, task);
      if (status)
         App_TerminateOnError (status, "WP_ImaEvent()");

      if (action == WP_IMA_EVENT_TICK)
         ticks++;

      if (action != WP_IMA_EVENT_TICK)
      {
         printf ("event(%s) cleared!\n", ima_events_name[action % 13]);
      }
      break;

   default:
      printf ("---Unknown IMA event 0x%x 0x%x\n", data, info);
      break;
   }
}

/*****************************************************************************
 * Function name: App_DuReceive
 * Description  : Receives a packet on a RX channel
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
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
   printf ("got a pakcets\n");
   fflush (stdout);
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

/* IMA Callback functions */
static void App_ImaEvent (WP_tag tag, WP_U32 data, WP_U32 info)
{
   ima_add_task (ima_irq_task_list, data, info);
}

/*****************************************************************************
 * Function name: display_events
 * Description  : Display Events
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/

void display_events (void)
{
   WP_ima_event a_task;
   WP_ima_event *task;

   WPI_SimulateInterrupts ();
   while ((task = ima_next_task (ima_irq_task_list, &a_task)))
      ima_app_perform_action (task);
}

/*****************************************************************************
 * Function name: App_Quit
 * Description  : exit from the application
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void WT_Reboot (void)
{
#if !defined(__linux__)
/* Rebooting is specific to board and target.  The test
 * directory should not be concerned how this is done.
 * Instead, it calls a board-specific function. */
   if (WPI_REBOOT_ENABLE)
   {
      printf ("Test Rebooting winmon by WT_Reboot\nWinMon>\n");
      WPX_Reboot ();
   }
   else
      printf ("Not Rebooting with WT_Reboot\nWinMon>\n");
#endif
}

void App_Quit (WP_U32 status)
{
   WP_DriverRelease ();

   if (status)
   {
      WT_Reboot ();
   }

   exit (0);
}

/*****************************************************************************
 * Function name: App_TerminateOnError
 * Description  : Terminate the application if error occur else print the success result
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_TerminateOnError (WP_handle handle, WP_CHAR * s)
{
   static WP_U32 pkts_cnt = 0;

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
      {
         printf ("Status returned from %s : WP_OK, pkts(%15d)\r", s,
                 pkts_cnt++);
         fflush (stdout);
      }
      else
         printf ("Handle returned from %s is %#8.8x\n", s, handle);
   }
}

/*****************************************************************************
 * Function name: App_Debug
 * Description  :  
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_Debug (WP_boolean debug)
{
   WP_flow_class_rule flow_class_rule;
   WP_flow_class_forwarding_action forwarding_action;

   if (debug)
   {
      printf ("Debug is on. Turning it OFF\n");
      forwarding_action.flow_aggregation = h_flow_agg_tdm[1];
      debug_on = WP_FALSE;
   }
   else
   {
      printf ("Debug is off. Turning it ON\n");
      forwarding_action.flow_aggregation = h_bridge1_default_agg;
      debug_on = WP_TRUE;
   }

   flow_class_rule.match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
   flow_class_rule.action_info.forwarding_action = forwarding_action;
   flow_class_rule.wred_entry = 0;

   status =
      WP_IwFlowModify (h_iw_sys_bridge1, h_bridge1_dfcflow1[0],
                       WP_IW_MODIFY_WRITE_THROUGH, WP_IW_FLOW_MOD_FLOW_AGG,
                       &flow_class_rule);
   App_TerminateOnError (status, " WP_IwFlowModify() DFC");

   status = WP_IwSystemBuild (h_iw_sys_bridge1);
   App_TerminateOnError (status, " WP_IwSystemBuild()");
}

/*****************************************************************************
 * Function name: insert_ip_header
 * Description  : Common routine to select IP Address 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
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

   /* DSCP priority is set here        */
   /* MSB 6 bits are used for priority */
   ptr_ip_data[IP_SER_TYPE] = 0x00; //0x0; // 0x04;  //0x0c; // 0x08; 

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
