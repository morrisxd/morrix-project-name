/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2012.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************/
/*                       External Includes                                   */
/*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#ifdef __linux__
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>
#include <termios.h>
#endif

#include "external.h"

#include "provider_bridge_menu.h"
#include "provider_bridge_util.h"

char *STR_MENU_HDR =
   "                  Ethernet Bridge Demo                     ";
char appname[] = "Ethernet Bridge Demo";
static char val_str[32];

#define DATA_LENGTH          (68 + 32)
extern WP_handle iw_pool;
extern Y_MenuEntry V_MainMenu[];

static Y_MenuEntry *V_CurrMenuP = &V_MainMenu[0];

Y_MenuEntry V_MainMenu[] = {
   {K_Menu, MAIN_MENU_ITEM_NUM, TRUE, "Main Menu",
    {(Y_MenuEntry *) V_MainMenu}},
   {K_Menu, 1, FALSE, " -> FH Test Menu", {(Y_MenuEntry *) FH_Test_menu}},
   {K_Menu, 2, FALSE, " -> Vlan Menu", {(Y_MenuEntry *) VLAN_menu}},
   {K_Menu, 3, FALSE, " -> Aging Menu", {(Y_MenuEntry *) AGING_menu}},
   {K_Menu, 4, FALSE, " -> Learning Menu", {(Y_MenuEntry *) LEARNING_menu}},
   {K_Menu, 5, TRUE, " -> System Statistics", {(Y_MenuEntry *) STAT_menu}},
   {K_Leaf, 6, FALSE, " <> Dump FDB entry",
    {(void *) (int) CLI_Dump_FDB_entry}},
   {K_Leaf, 7, FALSE, " <> Flush FDB by Vlan",
    {(void *) (int) CLI_Flush_FDB_entry_by_Vlan}},
   {K_Menu, 8, FALSE, " -> Performance test",
    {(Y_MenuEntry *) Performance_menu}},
   {K_Leaf, 9, TRUE, " <> Quit", {(void *) (int) CLI_MAIN_Quit}},
   {K_Menu, 10, TRUE, " -> IPV4 test", {(Y_MenuEntry *) IPV4_menu}},//Phenix
};

Y_MenuEntry IPV4_menu[] = {
   {K_Menu, IPV4_MENU_ITEM_NUM, TRUE, "IPV4 Menu",
    {(Y_MenuEntry *) V_MainMenu}},
   {K_Leaf, 1, TRUE, " <> Enable suppress broadcast storm ",
    {(void *) (int) CLI_BC_Suppress_Enable}},
    {K_Leaf, 2, TRUE, " <> Disable suppress broadcast storm ",
    {(void *) (int) CLI_BC_Suppress_Disable}},
   {K_Leaf, 3, TRUE, " <> Enable suppress unknow-unicast storm ",
    {(void *) (int) CLI_Unknown_Unicast_Suppress_Enable}},
    {K_Leaf, 4, TRUE, " <> Disable suppress unknow-unicast storm ",
    {(void *) (int) CLI_Unknown_Unicast_Suppress_Disable}},   
   {K_Leaf, 5, TRUE, " <> Vlan COS Trans",
    {(void *) (int) CLI_VlanCos_Trans}},
   {K_Leaf, 6, TRUE, " <> Mac Binding",
    {(void *) (int) CLI_MacBinding_Create}},   
    {K_Leaf, 7, TRUE, " <> Get Policer statistics ",
    {(void *) (int) CLI_STAT_PrintPolicerStatistics}},
   {K_Leaf, 8, TRUE, " <> Reset Policer statistics",
    {(void *) (int) CLI_STAT_ResetPolicerStatistics}},
   {K_Leaf, 9, TRUE, " <> Send Packets",
    {(void *) (int) CLI_Send_Packets}},   
};

Y_MenuEntry FH_Test_menu[] = {
   {K_Menu, FHT_MENU_ITEM_NUM, TRUE, "FiberHome Test Menu",
    {(Y_MenuEntry *) V_MainMenu}},
   {K_Leaf, 1, TRUE, " -> IPV6 filtering",
    {(void *) (int) CLI_FHT_IPV6_filter}},
   {K_Leaf, 2, TRUE, " -> L4 port filtering",
    {(void *) (int) CLI_FHT_L4_port_filter}},
   {K_Leaf, 3, TRUE, " -> L4 subtype filtering",
    {(void *) (int) CLI_FHT_L4_subtype_filter}},
   {K_Leaf, 4, TRUE, " -> Reserved MAC filtering",
    {(void *) (int) CLI_FHT_Reserved_Mac_filter}},
   {K_Leaf, 5, TRUE, " -> Max Learned Mac by port",
    {(void *) (int) CLI_FHT_Max_Learned_Mac}},
   {K_Leaf, 6, TRUE, " -> Case34 Uc Bc packet -> Vlan Prio",
    {(void *) (int) CLI_FHT_Case34_UcBc2Prio}},
   {K_Leaf, 7, TRUE, " -> Case35 SMAC DMAC SIP DIP TOS -> SVLAN",
    {(void *) (int) CLI_FHT_Case35_SmacDmacSipDipTos2SVlan}},
   {K_Leaf, 8, TRUE, " -> Case36 VLAN -> VLAN",
    {(void *) (int) CLI_FHT_Case36_Vlan2Vlan}},
   {K_Leaf, 9, TRUE, " -> Case37 Tagg Stack Tagg tunneling",
    {(void *) (int) CLI_FHT_Case37_Tag_Stack_Tunneling}},
};

Y_MenuEntry VLAN_menu[] = {
   {K_Menu, VLAN_MENU_ITEM_NUM, TRUE, "Vlan Menu",
    {(Y_MenuEntry *) V_MainMenu}},
   {K_Leaf, 1, TRUE, " <> Show Vlan", {(void *) (int) CLI_VLAN_Show_Vlan}},
   {K_Leaf, 2, TRUE, " -> Create Vlan",
    {(void *) (int) CLI_VLAN_Create_Vlan}},
   {K_Leaf, 3, TRUE, " -> Delete Vlan",
    {(void *) (int) CLI_VLAN_Delete_Vlan}},
   {K_Leaf, 4, TRUE, " -> Set Vlan Tag",
    {(void *) (int) CLI_VLAN_Set_Default_Tag}},
};

Y_MenuEntry MC_menu[] = {
   {K_Menu, MC_MENU_ITEM_NUM, TRUE, "Multicast group",
    {(Y_MenuEntry *) V_MainMenu}},
   {K_Leaf, 1, TRUE, " <> Show Multicast group",
    {(void *) (int) CLI_MC_Show_Muticast_group}},
   {K_Leaf, 2, TRUE, " -> Create Multicast group",
    {(void *) (int) CLI_MC_Create_Muticast_group}},
   {K_Leaf, 3, TRUE, " -> Add Multicast member",
    {(void *) (int) CLI_MC_Add_Muticast_member}},
   {K_Leaf, 4, TRUE, " -> Delete Multicast group",
    {(void *) (int) CLI_MC_Delete_Muticast_group}},
   {K_Leaf, 5, TRUE, " -> Remove Multicast member",
    {(void *) (int) CLI_MC_Remove_Muticast_member}},
};

Y_MenuEntry AGING_menu[] = {
   {K_Menu, AGING_MENU_ITEM_NUM, TRUE, "Enable/disable Aging",
    {(Y_MenuEntry *) V_MainMenu}},
   {K_Leaf, 1, TRUE, " -> Enable aging",
    {(void *) (int) CLI_Enable_aging}},
   {K_Leaf, 2, TRUE, " -> Disable aging",
    {(void *) (int) CLI_Disable_aging}},
   {K_Leaf, 3, TRUE, " -> Set aging period",
    {(void *) (int) CLI_Set_aging_period}},
   {K_Leaf, 4, TRUE, " -> Set aging weight",
    {(void *) (int) CLI_Set_aging_weight}},
};

Y_MenuEntry LEARNING_menu[] = {
   {K_Menu, LEARNING_MENU_ITEM_NUM, TRUE, "Enable/disable learning",
    {(Y_MenuEntry *) V_MainMenu}},
   {K_Leaf, 1, TRUE, " -> Enable learning",
    {(void *) (int) CLI_Enable_learning}},
   {K_Leaf, 2, TRUE, " -> Disable learning",
    {(void *) (int) CLI_Disable_learning}},
   {K_Leaf, 3, TRUE, " -> Enable port learning",
    {(void *) (int) CLI_Enable_port_lrn}},
   {K_Leaf, 4, TRUE, " -> Disable port learning",
    {(void *) (int) CLI_Disable_port_lrn}},
};

Y_MenuEntry STAT_menu[] = {
   {K_Menu, STAT_MENU_ITEM_NUM, TRUE, "Statistics Menu",
    {(Y_MenuEntry *) V_MainMenu}},
   {K_Leaf, 1, TRUE, " <> Show Device Stats ",
    {(void *) (int) CLI_STAT_ShowDevStats}},
   {K_Leaf, 2, TRUE, " <> Clear Device Stats",
    {(void *) (int) CLI_STAT_ClearDevStats}},
   {K_Leaf, 3, TRUE, " <> Show Bport  Stats",
    {(void *) (int) CLI_STAT_ShowBrgPrtStats}},
   {K_Leaf, 4, TRUE, " <> Clear Bport  Stats",
    {(void *) (int) CLI_STAT_ClearBrgPrtStats}},
   {K_Leaf, 5, TRUE, " <> Show FlwAgg Stats",
    {(void *) (int) CLI_STAT_ShowFlwAggStats}},
   {K_Leaf, 6, TRUE, " <> Clear FlwAgg Stats",
    {(void *) (int) CLI_STAT_ClearFlwAggStats}},
};

Y_MenuEntry Performance_menu[] = {
   {K_Menu, PERF_MENU_ITEM_NUM, TRUE, "Performance Test menu",
    {(Y_MenuEntry *) V_MainMenu}},
   {K_Leaf, 1, TRUE, " <> Start Performance test",
    {(void *) (int) CLI_Performance_TestStart}},
   {K_Leaf, 2, TRUE, " <> Stop Performance test",
    {(void *) (int) CLI_Performance_TestExit}},
};

static struct termios oldt;

void restore_terminal_settings (void)
{
   //Apply saved settings
   tcsetattr (0, TCSANOW, &oldt);
}

//make terminal read 1 char at a time
void disable_terminal_return (void)
{
   struct termios newt;

   //save terminal settings
   tcgetattr (0, &oldt);
   //init new settings
   newt = oldt;
   //change settings
   newt.c_lflag &= ~(ICANON | ECHO);
   //apply settings
   tcsetattr (0, TCSANOW, &newt);

   //make sure settings will be restored when program ends
   atexit (restore_terminal_settings);
}

int get_line (char *buf)
{
   int ch, index, done, i;
   char line[256];

   done = 0;
   index = 0;
   memset (line, 0, sizeof (line));

   /* Key reading loop */
   while (!done)
   {
      ch = getchar ();
      putchar (ch);

      switch (ch)
      {
      case KEY_ENTER:
         /* skip space */
         i = 0;
         while (!(isalpha (line[i]) || isdigit (line[i])) && (i < index))
            i++;

         index -= i;
         memcpy (buf, line + i, index);
         buf[index] = '\0';
         done = 1;
         break;
      case KEY_BACKSPACE:
         if (index > 0)
            index--;
         printf ("\33[K");
         break;
      default:
         if (index >= 256)
         {
            done = 1;
            index = 0;
            break;
         }

         line[index++] = ch;
      }
   }

   return index;
}

/*****************************************************************************
 * Function name: CLI_T_Main
 * Description  : cli main function, get input string and call menu engine.
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void *CLI_T_Main (void *arg)
{
   static char InputBuf[256];

   disable_terminal_return ();

   /* Display main menu */
   F_DisplayMenu ();

   while (demo_running)
   {
      printf ("%s", STR_ENTER_COMMAND);
      InputBuf[0] = '\0';
#ifdef  __linux__
      //loop:
      //fgets(InputBuf, 256, stdin);

      get_line (InputBuf);
#else
#ifdef WP_HW_WINPATH3
      flush ();
#endif
      gets (InputBuf);
#endif
      printf ("\n");
      F_MenuEngine (InputBuf);
   }

   return arg;
}

/*****************************************************************************
 * Function name: CLI_MAIN_Reboot
 * Description  : Reboot board.
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
static int CLI_MAIN_Reboot (char *StrPrm)
{
   demo_running = FALSE;
   puts ("Reboot...\r\n");
   WP_Delay (5);
   WPE_Bridge_Quit (1);
   return 0;
}

int CLI_MAIN_Quit (char *StrPrm)
{
   demo_running = FALSE;
   puts ("Quit...\r\n");
   WP_Delay (5);
   WPE_Bridge_Quit (0);
   return 0;
}

//Phenix
int CLI_STAT_PrintPolicerStatistics(void)
{
   WP_status status_ingress;
   WP_stats_policer_v2 policer_stats;

   memset(&policer_stats,0,sizeof(policer_stats));
   
   printf("\n********************POLICER STATISTICS*****************************");    
   {
      status_ingress = WP_PolicerStatistics(policer_handle, &policer_stats);
      printf("\ncbs- \t %08x%08x\t  ",
                        (WP_U32)(((policer_stats.non_conforming_cbs)>>32)&0xffffffff),
                        (WP_U32)((policer_stats.non_conforming_cbs)&0xffffffff));
      printf("\nebs- \t %08x%08x\t  ",
                        (WP_U32)(((policer_stats.non_conforming_ebs)>>32)&0xffffffff),
                        (WP_U32)((policer_stats.non_conforming_ebs)&0xffffffff));      
   printf("\n===============================================================================\n");      
   }

   return 0;
}

//Phenix
int CLI_STAT_ResetPolicerStatistics(void)
{
   WP_PolicerStatisticsReset(policer_handle);
   return 0;
}

/*****************************************************************************
 * Function name: CLI_STAT_ShowStats
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
int CLI_STAT_ShowDevStats (char *StrPrm)
{
   WP_U32 i;

   for (i = 0; i < NR_GBE; i++)
   {
      printf ("\n               Enet[%d] Device Statistics\n", i);
      WPT_LocalDisplayDeviceStats (gbe[i].dev_enet);
   }

   return WP_OK;
}

/*****************************************************************************
 * Function name: CLI_STAT_ClearDevStats
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
int CLI_STAT_ClearDevStats (char *StrPrm)
{
   WP_U32 i;

   for (i = 0; i < NR_GBE; i++)
   {
      printf ("\n               Reseting Enet[%d] Device Statistics\n", i);
      WPT_LocalDeviceStatsReset (gbe[i].dev_enet);
   }

   printf ("\n               Done!\n");
   return WP_OK;
}

/*****************************************************************************
 * Function name: CLI_STAT_ShowBrgPrtStats
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
int CLI_STAT_ShowBrgPrtStats (char *StrPrm)
{
   WP_U32 i;

   for (i = 0; i < NR_GBE; i++)
   {
      printf ("\n               Enet[%d] BridgePorts Statistics\n", i);
      WPT_DisplayBridgingPortStats (gbe[i].bport_enet, "ENET");
   }

   printf ("\n               Host BridgePorts Statistics\n");
   WPT_DisplayBridgingPortStats (h_iw_port_general_host, "HOST");

   return WP_OK;
}

/*****************************************************************************
 * Function name: CLI_STAT_ClearBrgPrtStats
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
int CLI_STAT_ClearBrgPrtStats (char *StrPrm)
{
   WP_U32 i;

   for (i = 0; i < NR_GBE; i++)
   {
      printf
         ("\n               Reseting Enet[%d] BridgePorts Statistics\n",
          i);
      WPT_IwBridgingPortStatsReset (gbe[i].bport_enet);
   }

   printf ("\n               Reseting Host BridgePorts Statistics\n");
   WPT_IwBridgingPortStatsReset (h_iw_port_general_host);

   return WP_OK;
}

/*****************************************************************************
 * Function name: CLI_STAT_ShowFlwAggStats
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
int CLI_STAT_ShowFlwAggStats (char *StrPrm)
{
   WP_U32 i;
   WP_U32 j;
   WP_status status;
  WP_iw_mc_member_stats  stats;
  memset(&stats,0,sizeof(stats));

   for (i = 0; i < NR_GBE; i++)
   {
      printf ("\n               Enet[%d] FlowAgg Statistics\n", i);
      WPT_GenericFlowStatistics (gbe[i].agg_enet);
      //WPT_GenericFlowStatisticsReset(gbe[i].agg_enet);
   }

   printf ("\n              Host FlowAgg Statistics\n");
   WPT_GenericFlowStatistics (default_agg_host);

   printf ("\n              MC FlowAgg Statistics\n");

	for(i=0;i<NUM_OF_VLAN_GROUPS;i++)
	{
		if(1== vlan_groups[i].valid)
		{
		for(j=0;j<IW_SYS_MAX_VLAN_MEMBERS;j++)
			{
			if(vlan_groups[i].member_handle[j])
				{
		   printf ("member %d (%x)\n",(vlan_groups[i].member_handle[j] & 0x000000ff), vlan_groups[i].member_handle[j]);


		status =WP_IwMcMemberStatistics(vlan_groups[i].group_handle, vlan_groups[i].member_handle[j], &stats);
                   App_TerminateOnError (status, "WP_IwMcMemberStatistics", __LINE__);

	   printf ("buffers_counter:                                    %08x%08x\n",
	           ((WPT_StatField *) & stats.buffers_counter)->part.high,
	           ((WPT_StatField *) & stats.buffers_counter)->part.low);

	   printf ("threshold_dropped_frames:                   %08x%08x\n",
	           ((WPT_StatField *) & stats.threshold_dropped_frames)->part.high,
	           ((WPT_StatField *) & stats.threshold_dropped_frames)->part.low);

	   printf ("class_threshold_dropped_frames:          %08x%08x\n",
	           ((WPT_StatField *) & stats.class_threshold_dropped_frames)->part.high,
	           ((WPT_StatField *) & stats.class_threshold_dropped_frames)->part.low);

	   printf ("forwarded_frames:                                %08x%08x\n",
	           ((WPT_StatField *) & stats.forwarded_frames)->part.high,
	           ((WPT_StatField *) & stats.forwarded_frames)->part.low);

	   printf ("fbp_drop_packets:                                 %08x%08x\n",
	           ((WPT_StatField *) & stats.fbp_drop_packets)->part.high,
	           ((WPT_StatField *) & stats.fbp_drop_packets)->part.low);

	   printf ("mtu_drop_packets:                                %08x%08x\n",
	           ((WPT_StatField *) & stats.mtu_drop_packets)->part.high,
	           ((WPT_StatField *) & stats.mtu_drop_packets)->part.low);

	   printf ("ttl_drop_packets:                                   %08x%08x\n",
	           ((WPT_StatField *) & stats.ttl_drop_packets)->part.high,
	           ((WPT_StatField *) & stats.ttl_drop_packets)->part.low);

	   printf ("tx_queue_drop_packets:                        %08x%08x\n",
	           ((WPT_StatField *) & stats.tx_queue_drop_packets)->part.high,
	           ((WPT_StatField *) & stats.tx_queue_drop_packets)->part.low);

	   printf ("mpls_drop:                                             %08x%08x\n",
	           ((WPT_StatField *) & stats.mpls_drop)->part.high,
	           ((WPT_StatField *) & stats.mpls_drop)->part.low);

	   printf ("denied_packets:                                      %08x%08x\n",
	           ((WPT_StatField *) & stats.denied_packets)->part.high,
	           ((WPT_StatField *) & stats.denied_packets)->part.low);


	   printf ("group_filtered_packets:                           %08x%08x\n",
	           ((WPT_StatField *) & stats.group_filtered_packets)->part.high,
	           ((WPT_StatField *) & stats.group_filtered_packets)->part.low);

	   printf ("forwarded_bytes:                                    %08x%08x\n",
	           ((WPT_StatField *) & stats.forwarded_bytes)->part.high,
	           ((WPT_StatField *) & stats.forwarded_bytes)->part.low);

	   printf ("gtp_bad_headers:                                    %08x%08x\n",
	           ((WPT_StatField *) & stats.gtp_bad_headers)->part.high,
	           ((WPT_StatField *) & stats.gtp_bad_headers)->part.low);

	   printf ("policer_non_conforming_packets:            %08x%08x\n",
	           ((WPT_StatField *) & stats.policer_non_conforming_packets)->part.high,
	           ((WPT_StatField *) & stats.policer_non_conforming_packets)->part.low);

	   printf ("fbp_underrun:                                          %08x%08x\n",
	           ((WPT_StatField *) & stats.fbp_underrun)->part.high,
	           ((WPT_StatField *) & stats.fbp_underrun)->part.low);

	   printf ("src_ip_filter_dropped_frames:                  %08x%08x\n",
	           ((WPT_StatField *) & stats.src_ip_filter_dropped_frames)->part.high,
	           ((WPT_StatField *) & stats.src_ip_filter_dropped_frames)->part.low);	  
				}
		}
			}
	}

   return WP_OK;
}

/*****************************************************************************
 * Function name: CLI_STAT_ClearFlwAggStats
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
int CLI_STAT_ClearFlwAggStats (char *StrPrm)
{
   WP_U32 i;
   WP_U32 j;
  WP_iw_mc_member_stats  stats;
  memset(&stats,0,sizeof(stats));

   for (i = 0; i < NR_GBE; i++)
   {
      printf ("\n               Reseting Enet[%d] FlowAgg Statistics\n",
              i);
      //WPT_GenericFlowStatistics(gbe[i].agg_enet);
      WPT_GenericFlowStatisticsReset (gbe[i].agg_enet);
   }

   printf ("\n              Reset Host FlowAgg Statistics\n");
   WPT_GenericFlowStatisticsReset (default_agg_host);

	for(i=0;i<NUM_OF_VLAN_GROUPS;i++)
	{
		if(1== vlan_groups[i].valid)
		{
		for(j=0;j<IW_SYS_MAX_VLAN_MEMBERS;j++)
			{
			if(vlan_groups[i].member_handle[j])
				{
		   printf ("Reset member [%d](%x)\n",(vlan_groups[i].member_handle[j] & 0x000000ff), vlan_groups[i].member_handle[j]);


		WP_IwMcMemberStatisticsReset(vlan_groups[i].group_handle, vlan_groups[i].member_handle[j], &stats);
				}
			}
		}
   }
   printf ("\n               Done!\n");
   return WP_OK;
}

/*****************************************************************************
 * Function name: F_SetMainMenu
 * Description  : Set current pointer to main menu.
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void F_SetMainMenu (void)
{

   V_CurrMenuP = (Y_MenuEntry *) & V_MainMenu[0];

}

/*****************************************************************************
 * Function name: F_SetUpperMenu
 * Description  : Set current pointer to upper menu.
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void F_SetUpperMenu (void)
{

   V_CurrMenuP = (Y_MenuEntry *) V_CurrMenuP->Node.Menu;

}

/*****************************************************************************
 * Function name: F_DisplayHeader
 * Description  : Display menu headline.
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void F_DisplayHeader (void)
{
   printf ("%s\n", STR_STAR);
   printf ("%s\n", STR_MENU_HDR);
   printf ("%s\n", STR_HOT_KEYS);
}

/*****************************************************************************
 * Function name: F_DisplayMenu
 * Description  : Display menu and its all items.
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void F_DisplayMenu (void)
{
   int Counter;

   Y_MenuEntry *CurrMenuEntryP;

   F_DisplayHeader ();
   printf ("  Name: %s                                              \n",
           V_CurrMenuP->Desc);
   printf ("%s\n", STR_STAR);
   CurrMenuEntryP = V_CurrMenuP;

   /* Skip to the 1st menu Item */
   CurrMenuEntryP++;

   for (Counter = 1; Counter <= V_CurrMenuP->Index;
        Counter++, CurrMenuEntryP++)
   {
      if (CurrMenuEntryP->Visible == TRUE)
      {
         printf ("%2d. %s\n", Counter, CurrMenuEntryP->Desc);
      }
   }

}

/***************************************************************
 * Func name  : F_EvaluateCommand
 * Description: Check menu item type and evaluate command.
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void F_EvaluateCommand (char *InputBuf)
{

   int Cmd = MENU_ITEM_ONLY_ONE;

   Y_MenuEntry *MenuEntryP;

   /* Is chosen command is in tolerance */

   if (MENU_ITEM_ONLY_ONE == V_CurrMenuP->Index)
   {
      Cmd = MENU_ITEM_ONLY_ONE;
   }
   else
   {
      Cmd = atoi (InputBuf);
      if ((Cmd > V_CurrMenuP->Index) || (Cmd < 1))
      {
         printf ("%s\n", STR_MENU_ERR);
         return;
      }
   }

   MenuEntryP = V_CurrMenuP + Cmd;

   /* Is chosen command is visible */
   if (MenuEntryP->Visible != TRUE)
   {
      printf ("%s\n", STR_MENU_ERR);
      return;
   }

   /* Is chosen command is menu or procedure */
   if (K_Leaf == MenuEntryP->Type)
   {
      (MenuEntryP->Node.Leaf) (InputBuf);
      F_DisplayMenu ();
   }
   else if (K_Menu == MenuEntryP->Type)
   {
      V_CurrMenuP = (Y_MenuEntry *) MenuEntryP->Node.Menu;
      F_DisplayMenu ();
   }

}

/***************************************************************
 * Func name  : F_EvaluateHotKey
 * Description: Check menu hot keys by priority.
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
int F_EvaluateHotKey (char *InputBuf)
{
   switch (InputBuf[0])
   {
   case HOTKEY_MAIN_MENU:      /* Goto main menu  */
      F_SetMainMenu ();
      F_DisplayMenu ();
      break;

   case HOTKEY_UPPER_MENU:     /* Goto upper menu */
      F_SetUpperMenu ();
      F_DisplayMenu ();
      break;

   case HOTKEY_CUR_MENU:       /* Display current menu again */
      F_DisplayMenu ();
      break;

   case HOTKEY_QUIT_PROG:      /* Quit */
      CLI_MAIN_Quit ("quit");
      break;

   case HOTKEY_REBOOT:
      CLI_MAIN_Reboot ("reboot");
      break;

   default:
      return ERROR;
   }
   return OK;
}

/***************************************************************
 * Func name  : F_MenuEngine
 * Description: Check hot keys first, then command.
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void F_MenuEngine (char *InputBuf)
{
   if (InputBuf[0] == '\0')
   {
      return;
   }

   /* Is HotKey? -> Evaluate HotKey */
   if (F_EvaluateHotKey (InputBuf) == OK)
   {
      return;
   }
   else
   {
      /* Evaluate Menu or Procedure Command */
      F_EvaluateCommand (InputBuf);
   }
}

WP_U32 CLI_GetNumber (WP_CHAR * text, WP_U32 low, WP_U32 high)
{
   int ret_val;
   WP_U32 val;

   while (1)
   {
      printf ("\n\n%s: \n", text);

      /* Get Shaping Parameter */
      ret_val = get_line (val_str);
      if ((ret_val == 0) || (strlen (val_str) > 10))
      {
         continue;
      }
      val = atoi (val_str);

      if (val >= low && val <= high)
         break;
   }

   printf ("\nyou choose = %d\n\n", val);
   return val;
}

/******************************* vlan flooding groups *************************/

static char delimiters[] = ".:-";
int F_ConvertStr2MacAddress (char *pStr, unsigned char *pMac)
{
   static char buff[128] = { 0 };

   char *token = NULL;
   int i = strlen (pStr);

   if ((i < 10) || (i > 128))
   {
      return -1;
   }
   memset (buff, 0, sizeof (buff));
   strcpy (buff, pStr);

   token = strtok (buff, delimiters);
   if (NULL == token)
   {
      return -2;
   }

   pMac[0] = strtoul (token, NULL, 16);

   for (i = 1; i < 6; i++)
   {
      token = strtok (NULL, delimiters);
      if (NULL == token)
      {
         return -3;
      }
      pMac[i] = strtoul (token, NULL, 16);
   }

   return 0;

}

int BC_enable = 0;

int CLI_BC_Suppress_Enable(char *StrPrm)
{
   WP_U16 vlan;
   WP_U32 port;

   char *pPortStr = NULL;

   
   printf ("\nPlease vlan and port pair. Enter \"Exit\" if end\n");
   printf ("\nport index: 0-ENET8; 1-ENET7.\n");
   printf ("\ne.g: 101 1\n");

   while (1)
   {
      memset (val_str, 0, 32);
      get_line (val_str);

      if (!strncmp (val_str, "Exit", 4) || !strncmp (val_str, "exit", 4))
         break;

      vlan = (WP_U16) strtoul (val_str, &pPortStr, 0);

      pPortStr = strchr (pPortStr, ' ');

      if (NULL == pPortStr)
      {
         printf ("Port Number invalid!\n");
         return -1;
      }
      port = (WP_U32) strtoul (pPortStr, NULL, 0);
      if(1 == BC_enable)
      {
      	printf("Attention!!You have set this!!!\n");
        return 0;
      }
      if(bc_rule)
      {
	      WPE_DeleteBCPCERules(port,bc_rule);
      }

      WPE_CreateBCPceRulesWithPolicer(port,vlan);
      BC_enable = 1;
   }

   return OK;
}

int CLI_BC_Suppress_Disable (char *StrPrm)
{
   WP_U16 vlan;
   WP_U32 port;

   char *pPortStr = NULL;


   printf ("\nPlease vlan and port pair.Enter \"Exit\" if end\n");
   printf ("\nport index: 0-ENET8; 1-ENET7.\n");
   printf ("\ne.g: 101 1\n");

   while (1)
   {
      memset (val_str, 0, 32);
      get_line (val_str);

      if (!strncmp (val_str, "Exit", 4) || !strncmp (val_str, "exit", 4))
         break;

      vlan = (WP_U16) strtoul (val_str, &pPortStr, 0);

      pPortStr = strchr (pPortStr, ' ');

      if (NULL == pPortStr)
      {
         printf ("Port Number invalid!\n");
         return -1;
      }
      port = (WP_U32) strtoul (pPortStr, NULL, 0);
      if(0 == BC_enable)
      {
      	printf("Attention!!You have clear this!!!\n");
        return 0;
      }

      if(bc_rule_policer)
      {
	      WPE_DeleteBCPCERules(port,bc_rule_policer);
      }
      WPE_CreateBCPceRulesWithOutPolicer(port,vlan);
      BC_enable = 0;
   }
   return OK;
}

int un_enable[10000]={0};
int CLI_Unknown_Unicast_Suppress_Enable (char *StrPrm)
{
   WP_U16 vlan;
   int i;
   
   printf ("\nPlease input vlan. Enter \"Exit\" if end\n");
   printf ("\ne.g: 101\n");

   while (1)
   {
      memset (val_str, 0, 32);
      get_line (val_str);

      if (!strncmp (val_str, "Exit", 4) || !strncmp (val_str, "exit", 4))
         break;

      vlan = (WP_U16) strtoul (val_str, NULL, 0);

      for(i=0;i<NUM_OF_VLAN_GROUPS;i++)
      {

	       if((1 == un_enable[vlan])&&(vlan_groups[i].vlan == vlan))
	      {
	      	printf("Attention!!You have set this!!!\n");
	        return 0;
	      }
      }

      for(i=0;i<NUM_OF_VLAN_GROUPS;i++)
      {
          if ((vlan_groups[i].vlan == vlan) && (vlan_groups[i].valid == 1)&&(vlan_groups[i].PCE_rule_handle))
      	{
            WPE_DeleteUnkonwnUnicast_rules(vlan, 0,0);
            WPE_DeleteUnkonwnUnicast_rules (vlan, 1,0);
      	}
      }
      WPE_DeleteUnkonwnUnicastGroup( vlan, 0);
      WPE_DeleteUnkonwnUnicastGroup( vlan, 1);  
	  
      WPE_CreateUnknownUnicastGroupWithPolicer (vlan, 0);
      //WPE_CreateLearningFlowAggPceRule (0, vlan);
      WPE_CreateUnknownUnicastGroupWithPolicer (vlan, 1);
      //WPE_CreateLearningFlowAggPceRule (1, vlan);
      for(i=0;i<NUM_OF_VLAN_GROUPS;i++)
      {
	 if ((vlan_groups[i].vlan == vlan) && (vlan_groups[i].valid == 1)&&(!vlan_groups[i].PCE_rule_handle_policer))
	 {
      	      WPE_CreateUnknownUnicastPceRulesWithPolicer(vlan, &vlan_groups[i]);
         }
      }
      un_enable[vlan] = 1;
   }

   return OK;
}

int CLI_Unknown_Unicast_Suppress_Disable (char *StrPrm)
{
   WP_U16 vlan;
   int i;
   
   printf ("\nPlease input vlan.Enter \"Exit\" if end\n");
   printf ("\ne.g: 101\n");

   while (1)
   {
      memset (val_str, 0, 32);

      get_line (val_str);

      if (!strncmp (val_str, "Exit", 4) || !strncmp (val_str, "exit", 4))
         break;

      vlan = (WP_U16) strtoul (val_str, NULL, 0);

      for(i=0;i<NUM_OF_VLAN_GROUPS;i++)
      {

	       if((0 == un_enable[vlan])&&(vlan_groups[i].vlan == vlan))
	      {
	      	printf("Attention!!You have clear this!!!\n");
	        return 0;
	      }
      }

      for(i=0;i<NUM_OF_VLAN_GROUPS;i++)
      {
          if ((vlan_groups[i].vlan == vlan) && (vlan_groups[i].valid == 1)&&(vlan_groups[i].PCE_rule_handle_policer))
      	{
            WPE_DeleteUnkonwnUnicast_rules(vlan, 0,1);
            WPE_DeleteUnkonwnUnicast_rules (vlan, 1,1);
      	}
      }

      WPE_DeleteUnkonwnUnicastGroup( vlan, 0);
      WPE_DeleteUnkonwnUnicastGroup( vlan, 1);  

      WPE_CreateUnknownUnicastGroup (vlan, 0);
      //WPE_CreateLearningFlowAggPceRule (0, vlan);
      WPE_CreateUnknownUnicastGroup (vlan, 1);
      //WPE_CreateLearningFlowAggPceRule (1, vlan);
      for(i=0;i<NUM_OF_VLAN_GROUPS;i++)
      {
	 if ((vlan_groups[i].vlan == vlan) && (vlan_groups[i].valid == 1)&&(!vlan_groups[i].PCE_rule_handle))
	 {
      	      WPE_CreateUnknownUnicastPceRules(vlan, &vlan_groups[i]);
         }
      }
      un_enable[vlan] = 0;

   }

   return OK;
}
int CLI_VlanCos_Trans (char *StrPrm)
{
   WP_U16 in_vlan;
   WP_U16 out_vlan;
   WP_U8  cos;
   WP_U32 port;

   char *pPortStr = NULL;
   char *pCosStr = NULL;
   char *pVlanStr = NULL;

   
   printf ("\nPlease input the original vlan, port  and the updated COS and vlan . Enter \"Exit\" if end\n");
   printf ("\nport index: 0-ENET8; 1-ENET7.\n");
   printf ("\ne.g: 100 1 7 200 (100---original vlan,1---port index, 7--- updated COS, 200 --- updated vlan)\n");

   while (1)
   {
      memset (val_str, 0, 32);
      get_line (val_str);

      if (!strncmp (val_str, "Exit", 4) || !strncmp (val_str, "exit", 4))
         break;

      in_vlan = (WP_U16) strtoul (val_str, &pPortStr, 0);

      pPortStr = strchr (pPortStr, ' ');

      if (NULL == pPortStr)
      {
         printf ("port Number invalid!\n");
         return -1;
      }
	  
      port = (WP_U32) strtoul (pPortStr, &pCosStr, 0);
	  
      pCosStr = strchr (pCosStr, ' ');
      if (NULL == pCosStr)
      {
         printf ("COS value invalid!\n");
         return -1;
      }
	  
      cos = (WP_U8) strtoul (pCosStr, &pVlanStr, 0);
      if(cos>7)
      {
	      printf("Please input the right COS value[0--7]\n");
              return 0;
      }
      pVlanStr = strchr (pVlanStr, ' ');
      if (NULL == pVlanStr)
      {
         printf ("Vlan Number invalid!\n");
         return -1;
      }
	  
      out_vlan = (WP_U32) strtoul (pVlanStr, NULL, 0);

      //printf("(%d-%d-%d-%d)\n",in_vlan,port,cos,out_vlan);
      WPE_CreateVlanCosPceRules(in_vlan,port,cos,out_vlan);      
   }

   return OK;
}

int CLI_MacBinding_Create (char *StrPrm)
{
   unsigned char mac[6];
   WP_U32 port;

   char *pMacStr = val_str;
   char *pPortStr = NULL;


   printf ("Please input MAC.\n");
   printf ("\nport index: 0-ENET8; 1-ENET7.\n");

   printf ("\nPlease input MAC and port pair. Enter \"Exit\" if end\n");

   printf ("e.g: 01-02-03-04-05-00 1\n");
   get_line (val_str);

   F_ConvertStr2MacAddress (pMacStr, mac);

      pPortStr = strchr (pMacStr, ' ');

      if (NULL == pPortStr)
      {
         printf ("Port Number invalid!\n");
         return -1;
      }
      port = (WP_U32) strtoul (pPortStr, NULL, 0);

   WPE_CreateMacBindingPceRule (0, mac);

   return OK;
}


int CLI_VLAN_Create_Vlan (char *StrPrm)
{
   WP_U16 vlan;
   WP_U32 port;

   char *pPortStr = NULL;

   printf ("\nPlease input vlan and port pair. Enter \"Exit\" if end\n");
   printf ("\nport index: 0-ENET8; 1-ENET7.\n");
   printf ("\ne.g: 101 1\n");

   while (1)
   {
      memset (val_str, 0, 32);
      get_line (val_str);

      if (!strncmp (val_str, "Exit", 4) || !strncmp (val_str, "exit", 4))
         break;

      vlan = (WP_U16) strtoul (val_str, &pPortStr, 0);

      pPortStr = strchr (pPortStr, ' ');

      if (NULL == pPortStr)
      {
         printf ("Port Number invalid!\n");
         return -1;
      }
      port = (WP_U32) strtoul (pPortStr, NULL, 0);

      WPE_CreateUnknownUnicastGroup (vlan, port);
      WPE_CreateLearningFlowAggPceRule (port, vlan);
   }

   return OK;
}

int CLI_VLAN_Delete_Vlan (char *StrPrm)
{
   WP_U16 vlan;
   WP_U32 port;

   char *pPortStr = NULL;

   printf ("\nPlease input vlan and port pair.Enter \"Exit\" if end\n");
   printf ("\nport index: 0-ENET7; 1-ENET7.\n");
   printf ("\ne.g: 101 1\n");

   while (1)
   {
      memset (val_str, 0, 32);

      get_line (val_str);

      if (!strncmp (val_str, "Exit", 4) || !strncmp (val_str, "exit", 4))
         break;

      vlan = (WP_U16) strtoul (val_str, &pPortStr, 0);

      pPortStr = strchr (pPortStr, ' ');
      if (NULL == pPortStr)
      {
         printf ("Port Number invalid!\n");
         return -1;
      }
      port = (WP_U32) strtoul (pPortStr, NULL, 0);

      WPE_DeleteUnkonwnUnicastGroup (vlan, port);
      WPE_DeleteLearningFlowAggPceRule (port, vlan);
   }

   return OK;
}

int CLI_VLAN_Show_Vlan (char *StrPrm)
{
   WP_U32 i = 0;
   WP_U32 j = 0;
   WP_U16 vlan;

   char *pMacStr = val_str;

   printf ("Please input vlan.\n");
   printf ("e.g: 100\n");
   get_line (val_str);

   vlan = strtoul (pMacStr, NULL, 0);

   for (i = 0; i < NUM_OF_VLAN_GROUPS; i++)
   {
      if ((vlan_groups[i].vlan == vlan) && (vlan_groups[i].valid == 1))
      {
         printf ("\nVLAN Group: %d          Members(Port Number): ", vlan);
         for (j = 0; j < IW_SYS_MAX_VLAN_MEMBERS; j++)
         {
            if (vlan_groups[i].member_handle[j])
               printf (" %d,", j);
         }
         printf ("\n\n\n");

         break;
      }

   }
   return OK;
}

int CLI_VLAN_Set_Default_Tag (char *StrPrm)
{
   WP_U16 vlanTag;
   WP_U32 portNum;

   char *pPortStr = val_str;

   printf ("\nPlease input Port Number and Default VLAN Tag\n");
   printf ("\ne.g: 1 101\n");
   get_line (val_str);

   portNum = (WP_U16) strtoul (val_str, &pPortStr, 0);

   pPortStr = strchr (pPortStr, ' ');
   if (NULL == pPortStr)
   {
      printf ("Vlan Tag invalid!\n");
      return -1;
   }
   vlanTag = (WP_U32) strtoul (pPortStr, NULL, 0);

   WPE_BridgePortsVlanTagSet (portNum, vlanTag);

   return OK;
}

/***********************multicast***************************/
int CLI_MC_Show_Muticast_group (char *StrPrm)
{
   WP_U32 i = 0;
   WP_U32 j = 0;
   WP_U16 vlan;
   unsigned char mac[6];
   char *pMacStr = val_str;

   printf ("Please input vlan and multicast mac.\n");
   printf ("e.g: 100 01-00-5e-00-01-01\n");
   get_line (val_str);

   vlan = strtoul (val_str, &pMacStr, 0);
   if (NULL == pMacStr)
   {
      return ERROR;
   }
   pMacStr = strchr (pMacStr, ' ');
   if (NULL == pMacStr)
   {
      return ERROR;
   }
   while (' ' == *pMacStr)
   {
      pMacStr++;
   }
   F_ConvertStr2MacAddress (pMacStr, mac);

   for (i = 0; i < NUM_OF_MC_GROUPS; i++)
   {
      if ((mc_groups[i].vlan == vlan) && (mc_groups[i].valid == 1)
          && (!memcmp (mac, mc_groups[i].mc_mac, 6)))
      {
         printf
            ("\nMC Group: MAC = %02x-%02x-%02x-%02x-%02x-%02x, VID = %d          Members(Port Number): ",
             mc_groups[i].mc_mac[0], mc_groups[i].mc_mac[1],
             mc_groups[i].mc_mac[2], mc_groups[i].mc_mac[3],
             mc_groups[i].mc_mac[4], mc_groups[i].mc_mac[5], vlan);
         for (j = 0; j < IW_SYS_MAX_MC_MEMBERS; j++)
         {
            if (mc_groups[i].member_handle[j])
               printf (" %d,", j);
         }
         printf ("\n\n\n");

         break;
      }

   }
   return OK;
}

int CLI_MC_Create_Muticast_group (char *StrPrm)
{

   WP_U16 vlan;
   unsigned char mac[6];
   char *pMacStr = NULL;

   printf ("\nPlease input vlan and multicast MAC address.\n");
   printf ("\ne.g: 101 01-00-5e-00-01-01\n");
   get_line (val_str);

   vlan = (WP_U16) strtoul (val_str, &pMacStr, 0);

   pMacStr = strchr (pMacStr, ' ');
   if (NULL == pMacStr)
   {
      printf ("MAC address invalid!\n");
      return -1;
   }
   while (' ' == *pMacStr)
   {
      pMacStr++;
   }
   F_ConvertStr2MacAddress (pMacStr, mac);

   WPE_CreateMulticastGroup (mac, vlan);
   return OK;
}

int CLI_MC_Add_Muticast_member (char *StrPrm)
{
   WP_U16 vlan;
   WP_U32 port;
   unsigned char mac[6];
   char *pPortStr = NULL;
   char *pMacStr = NULL;

   printf ("\nPlease input vlan,port and multicast MAC address.\n");
   printf ("\nport index: 0-ENET7; 1-ENET7.\n");
   printf ("\ne.g: 101 1 01-00-5e-00-01-01\n");
   get_line (val_str);

   vlan = (WP_U16) strtoul (val_str, &pPortStr, 0);

   pPortStr = strchr (pPortStr, ' ');
   if (NULL == pPortStr)
   {
      printf ("Port number invalid!\n");
      return -1;
   }
   port = (WP_U32) strtoul (pPortStr, &pMacStr, 0);
   pMacStr = strchr (pMacStr, ' ');
   if (NULL == pMacStr)
   {
      printf ("MAC address invalid!\n");
      return -1;
   }
   while (' ' == *pMacStr)
   {
      pMacStr++;
   }
   F_ConvertStr2MacAddress (pMacStr, mac);

   WPE_AddMulticastMember (mac, vlan, port);
   return OK;
}

int CLI_MC_Delete_Muticast_group (char *StrPrm)
{
   WP_U16 vlan;
   unsigned char mac[6];
   char *pMacStr = NULL;

   printf ("\nPlease input vlan, and multicast mac pair.\n");
   printf ("\ne.g: 101 01-00-5e-00-01-01\n");

   get_line (val_str);

   vlan = (WP_U16) strtoul (val_str, &pMacStr, 0);

   pMacStr = strchr (pMacStr, ' ');
   if (NULL == pMacStr)
   {
      printf ("MAC address invalid!\n");
      return -1;
   }
   while (' ' == *pMacStr)
   {
      pMacStr++;
   }
   F_ConvertStr2MacAddress (pMacStr, mac);

   WPE_DeleteMulticastGroup (mac, vlan);

   return OK;
}

int CLI_MC_Remove_Muticast_member (char *StrPrm)
{
   WP_U16 vlan;
   WP_U32 port;
   unsigned char mac[6];
   char *pPortStr = NULL;
   char *pMacStr = NULL;

   printf ("\nPlease input vlan,port and multicast mac pair.\n");
   printf ("\nport index: 0-ENET8; 1-ENET7.\n");
   printf ("\ne.g: 101 1 01-00-5e-00-01-01\n");

   get_line (val_str);

   vlan = (WP_U16) strtoul (val_str, &pPortStr, 0);

   pPortStr = strchr (pPortStr, ' ');
   if (NULL == pPortStr)
   {
      printf ("Port number invalid!\n");
      return -1;
   }
   port = (WP_U32) strtoul (pPortStr, &pMacStr, 0);
   pMacStr = strchr (pMacStr, ' ');
   if (NULL == pMacStr)
   {
      printf ("MAC address invalid!\n");
      return -1;
   }
   while (' ' == *pMacStr)
   {
      pMacStr++;
   }
   F_ConvertStr2MacAddress (pMacStr, mac);

   WPE_RemoveMulticastMember (mac, vlan, port);

   return OK;
}

int CLI_Enable_aging (char *StrPrm)
{
   if (aging_enable)
   {
      OS_AgingTimerDelete ();
   }

   aging_enable = 1;

   // create and start the timer
   OS_AgingTimerCreate ();
   OS_AgingTimerStart ();

   return OK;
}

int CLI_Disable_aging (char *StrPrm)
{
   if (aging_enable)
   {
      OS_AgingTimerDelete ();
   }

   aging_enable = 0;

   return OK;
}

int CLI_Set_aging_period (char *StrPrm)
{
   WP_U32 temp = 0;

   temp =
      CLI_GetNumber ("Configure Aging period ( 1 - 1000, in seconds)", 1,
                     1000);
   aging_period = temp;

   return OK;
}

int CLI_Set_aging_weight (char *StrPrm)
{
   WP_U32 temp = 0;

   temp = CLI_GetNumber ("Configure Aging Weight ( 1 - 10)", 1, 10);
   aging_weight = temp;

   return OK;
}

int CLI_Enable_learning (char *StrPrm)
{
   learning_enable = 1;

   return OK;
}

int CLI_Disable_learning (char *StrPrm)
{
   learning_enable = 0;

   return OK;
}

int CLI_Enable_port_lrn (char *StrPrm)
{
   WP_U32 temp;

   if (!learning_enable)
   {
      printf ("learning is not enabled, please enable it first.\n");
      return OK;
   }

   temp = CLI_GetNumber ("Input port(0 - 1 for GBEs)", 0, 1);
   WPE_DisableEnablePortLearning (temp, 1);

   return OK;
}

int CLI_Disable_port_lrn (char *StrPrm)
{
   WP_U32 temp;

   if (!learning_enable)
   {
      printf ("learning is not enabled, please enable it first.\n");
      return OK;
   }

   temp = CLI_GetNumber ("Input port(0 - 1 for GBEs)", 0, 1);
   WPE_DisableEnablePortLearning (temp, 0);

   return OK;
}

int CLI_Dump_FDB_entry (char *StrPrm)
{
   //WPE_DumpFwdPCERules();
   WPE_PrintFDBList ();
   //WPE_DumpAllPCERules();
   return OK;
}

int CLI_Flush_FDB_entry_by_Vlan (char *StrPrm)
{
   WP_U16 vlan;

   printf ("\nPlease input vlan.\n");
   printf ("\ne.g: 101\n");
   get_line (val_str);

   vlan = (WP_U16) strtoul (val_str, NULL, 0);
   WPE_DeleteVlanPCERules (vlan);
   return OK;
}

/******************************* Performance Demo API *************************/

int CLI_Performance_TestStart (char *StrPrm)
{
   if (PMTest_enable)
   {
      printf ("already enabled, exit firstly.\n");
      return 0;
   }

   PMTest_enable = 1;
   PMTest_delta = 0;
   learnt_rules = 0;

   return 0;
}

int CLI_Performance_TestExit (char *StrPrm)
{
   if (!PMTest_enable)
      return 0;

   PMTest_enable = 0;

   return 0;
}

int CLI_WinUtil (char *StrPrm)
{
   WP_CHAR InputBuf[255];

   printf ("Enter winutil command >\n");
   get_line (InputBuf);
   printf ("\n");
   WPUI_RunWinMon (InputBuf);

   return 0;
}

/* ------------------------- Fiberhome Test CLI --------------------------*/

int F_ConvertStr2Ipv6Address (char *pStr, unsigned char *ipv6)
{
   static char buff[128] = { 0 };

   char *token = NULL;
   int i = strlen (pStr);

   if ((i < 10) || (i > 128))
   {
      return -1;
   }
   memset (buff, 0, sizeof (buff));
   strcpy (buff, pStr);

   token = strtok (buff, delimiters);
   if (NULL == token)
   {
      return -2;
   }

   ipv6[0] = strtoul (token, NULL, 16);

   for (i = 1; i < 16; i++)
   {
      token = strtok (NULL, delimiters);
      if (NULL == token)
      {
         return -3;
      }
      ipv6[i] = strtoul (token, NULL, 16);
   }

   return 0;
}

int CLI_FHT_IPV6_filter (char *StrPrm)
{
   WP_U8 ipv6[16] = { 0x00 };
   WP_CHAR InputBuf[255];

   printf ("Enter IPV6 address >\n");
   printf ("e.g. 00-01-02-03-04-05-06-07-08-09-0a-0b-0c-0d-0e-0f\n");
   get_line (InputBuf);
   printf ("\n");

   F_ConvertStr2Ipv6Address (InputBuf, ipv6);

   WPE_CreateIPV6MatchPceRule (0, ipv6);

   return 0;
}

int CLI_FHT_L4_port_filter (char *StrPrm)
{
   WP_U32 temp;

   temp =
      CLI_GetNumber ("Configure L4 port number ( 1 - 10000)", 1, 10000);

   WPE_CreateL4PortPceRule (0, temp);

   return 0;
}

int CLI_FHT_L4_subtype_filter (char *StrPrm)
{
   WP_U32 temp;

   temp = CLI_GetNumber ("Configure L4 subtype ( 1 - 65535)", 1, 65535);

   WPE_CreateL4SubtypePceRule (0, temp);

   return 0;
}

int CLI_FHT_Reserved_Mac_filter (char *StrPrm)
{
   unsigned char mac[6];
   char *pMacStr = val_str;

   printf ("Please input MAC.\n");
   printf ("e.g: 01-00-5e-00-01-01\n");
   get_line (val_str);

   F_ConvertStr2MacAddress (pMacStr, mac);

   WPE_CreateReservedMacPceRule (0, mac);

   return 0;
}

int CLI_FHT_Max_Learned_Mac (char *StrPrm)
{
   WP_U32 temp, port;

   temp = CLI_GetNumber ("Input port number ( 0 - 1)", 0, 1);

   port = temp;

   temp = CLI_GetNumber ("Max mac number ( 0 - 65535)", 1, 65535);

   gbe[port].max_learned_mac = temp;

   return 0;
}

int CLI_FHT_Case34_UcBc2Prio (char *StrPrm)
{
   WP_U32 temp, portid;
   
   WP_pce_filter_classification filter_class = { 0 };
   WP_handle filter_34;
   WP_handle filter_set_34;
   WP_pce_filter_set fs_level;
   WP_pce_rule_classification rule_cfg = { 0 };
   WP_handle port_handle = 0, agg_handle = 0, h_pce_rule = 0;

   WP_pce_if_params_pkt_rx_channel pce_if_params = { 0 };
   WP_handle pce_if_handle;
   WP_status status;
   WP_U32 ii;
   WP_handle iw_sys = dl_general_iwsys_bridge;

   temp = CLI_GetNumber ("Input port number ( 0 - 1)", 0, 1);

   portid = temp;


   WP_U32 i;

   WP_iw_agg_generic dl_tx_agg_gbe[1] = {
      {
       /*tag */ 2,
       /*txfunc */ 0,
       /*iw_port */ 0,
       /*rfcs */ WP_IW_RFCS_ENABLE,
       /*interruptqueue; */ WP_IW_IRQT1,
       /*error_pkt_mode */ WP_IW_ERRPKT_DISCARD,
       /*intmode; */ WP_IW_INT_DISABLE,
       /*statmode; */ WP_IW_STAT_ENABLE,
       /*timestamp_mode; */ WP_IW_TIME_STAMP_DISABLE,
       /*mtu; */ 9216,
       /*flow_agg_type; */ WP_IW_FLOW_AGG_PRIMARY,
       /*policer_handle; */ 0,
       /*pecs_handle; */ 0,
       /*pecs_flow_info; */ 0,
       /*pecs_global_info_handle; */ 0,
       },
   };
   WPE_gpe_brouter_pecs_flow_info brouter_pecs_flow_info[] = {
      /*  */
      {
       WPE_GPE_BROUTER_PECS_EXTRACT_DISABLE, /* header_extract_mode */
       0,                       /* extraction_size */
       WPE_GPE_BROUTER_PECS_ADD_DISABLE,  /* prefix_add_mode */
       4,                       /* prefix_add_size */
       14,                      /* prefix_ip_offset */
       WPE_GPE_BROUTER_PECS_REPLACE_MAC_DISABLE,   /* mac_replace_mode */
       {0x11, 0x22, 0x33, 0x44, 0x55, 0x00}, /* mac_da */
       {0x66, 0x77, 0x88, 0x99, 0xaa, 0x00}, /* mac_sa */
       0,                       /* vlan_edit_options */

       {0},                     /* prefix_remark_options */
       WPE_GPE_BROUTER_PECS_TTL_DISABLE,  /* ttl_mode */
       WPE_GPE_BROUTER_PECS_TOS_REMARKING_DISABLE, /* tos_remarking_mode */
       {                        /* prefix */
        0x11, 0x22, 0x33, 0x44, 0x55, 0x00,
        0x11, 0x12, 0x13, 0x14, 0x17, 0x00,
        0x81, 0, 0, 1,
        0x88, 0x47,
        0x11, 0x11, 0x10, 0x80,
        0x22, 0x22, 0x21, 0x80,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0}
       }
   };

   // keatin here use modify maybe better. or delete it first.

   /* DL GE TX flow agg */
   for (i = 0; i < NR_GBE; i++)
   {
      if(gbe[i].agg_enet != 0)
      	{
      	   WP_IwFlowAggregationDelete(gbe[i].agg_enet);
      	}
   
      dl_tx_agg_gbe->txfunc = gbe[i].tx_chan_enet;
      dl_tx_agg_gbe->iw_port = gbe[i].bport_enet;
      dl_tx_agg_gbe->pecs_handle = pecs_handles[0];
      dl_tx_agg_gbe->pecs_flow_info = (void *) &brouter_pecs_flow_info[0];
      gbe[i].agg_enet =
         WP_IwFlowAggregationCreate (WP_WINPATH (DEFAULT_WPID),
                                     WP_IW_GENERIC_MODE, &dl_tx_agg_gbe);
      App_TerminateOnError (gbe[i].agg_enet,
                            "WP_IwFlowAggregationCreate()", __LINE__);
   }

   
   /* create unknown unicast classification filter */
   filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   filter_class.no_match_result[0].result_type =
      WP_PCE_RESULT_LAST;

   filter_class.filter_fields[0].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[0].mask_mode =
      WP_PCE_MASK_PARSER_FLAG_BC;
   filter_class.filter_fields[0].field_id =
      WP_PCE_FIELD_ID_PARSER_FLAGS;
   filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   filter_34 = WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID),
                          WP_PCE_FILTER_CLASSIFICATION,
                          &filter_class);
   App_TerminateOnError (filter_34,"WP_PceFilterCreate", __LINE__);

   /* filter set with learning */
   fs_level.filter_set_level = 0;
   fs_level.next_filter_set = WP_UNUSED;
   fs_level.filters[0] = filter_34;
   fs_level.filters[1] = WP_UNUSED;

   filter_set_34 =
      WP_PceFilterSetCreate (WP_WINPATH (DEFAULT_WPID), &fs_level);
   App_TerminateOnError (filter_set_34, "WP_PceFilterSetCreate",
                         __LINE__);

   switch (portid)
   {
   case 0:
   case 1:
      port_handle = gbe[portid].bport_enet;
      agg_handle = gbe[1 - portid].agg_enet;
      break;
   default:
      printf ("NO such port : %d\n", portid);
      return 0;
   }

   // rule1 for uc
   rule_cfg.enabled = WP_ENABLE;

   rule_cfg.filter_handle = filter_34;

   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_PARSER_FLAGS;
   rule_cfg.rule_fields[0].value.parser_flags = 0;

   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;

   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_handle;

   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_EXT_VLAN_PRIO;
   rule_cfg.match_result[1].param.prefix_priority_remarking.value[0] = 0;

   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

   h_pce_rule = WP_PceRuleCreate (WP_WINPATH (DEFAULT_WPID),
                                  WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   if (WP_ERROR (h_pce_rule) == WP_ERR_PCE_RULE_ALREADY_EXISTS)
   {
      printf ("PCE rule - %d already exist!\n", 0);
   }
   else
   {
      App_TerminateOnError (h_pce_rule, "WP_PceRuleCreate", __LINE__);
   }

   // rule2 for bc
   rule_cfg.enabled = WP_ENABLE;

   rule_cfg.filter_handle = filter_34;

   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_PARSER_FLAGS;
   rule_cfg.rule_fields[0].value.parser_flags = WP_PCE_MASK_PARSER_FLAG_BC;

   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;

   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_handle;

   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_EXT_VLAN_PRIO;
   rule_cfg.match_result[1].param.prefix_priority_remarking.value[0] = 0;

   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

   h_pce_rule = WP_PceRuleCreate (WP_WINPATH (DEFAULT_WPID),
                                  WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   if (WP_ERROR (h_pce_rule) == WP_ERR_PCE_RULE_ALREADY_EXISTS)
   {
      printf ("PCE rule - %d already exist!\n", 1);
   }
   else
   {
      App_TerminateOnError (h_pce_rule, "WP_PceRuleCreate", __LINE__);
   }


   WP_rx_binding_bridging rx_binding_cfg[1] = {
      {
       /*  encap_mode */ 0,
       /*  mru; */ 512,
       /*  vcfcs; */ 0,
       /*  input_port; */ 0
       }
   };

   pce_if_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
   pce_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;
   pce_if_params.filter_set_handle = filter_set_34;
   pce_if_params.ip_header_validation = WP_DISABLE;
   pce_if_params.user_programmable_fields_handle = 0;

   pce_if_handle =
      WP_PceInterfaceCreate (WP_PCE_IF_TYPE_PKT_RX_CHANNEL,
                             &pce_if_params);
   App_TerminateOnError (pce_if_handle, "WP_PceInterfaceCreate()",
                         __LINE__);

   /* Modify the PCE interface for GE RX channel handle */
   for (ii = 0; ii < NR_GBE; ii++)
   {
      rx_binding_cfg[0].pce_if_handle = pce_if_handle;
      status = WP_IwRxBindingModify (gbe[ii].rx_chan_enet,
                                     iw_sys,
                                     qniw,
                                     WP_IW_RX_BIND_MOD_PCE_INTERFACE,
                                     &rx_binding_cfg[0]);
      App_TerminateOnError (status, "WP_IwRxBindingModify", __LINE__);
   }


   return 0;
}

int CLI_FHT_Case35_SmacDmacSipDipTos2SVlan (char *StrPrm)
{
   return 0;
}

int CLI_FHT_Case36_Vlan2Vlan (char *StrPrm)
{
   return 0;
}

int CLI_FHT_Case37_Tag_Stack_Tunneling (char *StrPrm)
{
   return 0;
}

void App_TranslateAsciiToHex(WP_U8 *Hex,WP_CHAR *Ascii,WP_U32 length)
{
    WP_U32 i,first=0,second=0;

   for (i=0;i<length;i++)
     {

          if ( Ascii[2*i] >= '0' &&  Ascii[2*i] <= '9')
            first =  Ascii[2*i] - '0';

          if ( Ascii[2*i] >= 'a' &&  Ascii[2*i] <= 'f')
             first =  Ascii[2*i] - 'a' + 10;

          if ( Ascii[2*i] >= 'A' &&  Ascii[2*i] <= 'F')
             first =  Ascii[2*i] - 'A' + 10;

          if ( Ascii[2*i+1] >= '0' &&  Ascii[2*i+1] <= '9')
            second =  Ascii[2*i+1] - '0';
          if ( Ascii[2*i+1] >= 'a' &&  Ascii[2*i+1] <= 'f')
            second =  Ascii[2*i+1] - 'a' + 10;
          if ( Ascii[2*i+1] >= 'A' &&  Ascii[2*i+1] <= 'F')
            second =  Ascii[2*i+1] - 'A' + 10;

          Hex[i]=(WP_U8 )(first * 16 + second);
     }
    return;
}


void WPE_DataUnitSendToEnet(const WP_CHAR* mac_dst,
                               const WP_CHAR* mac_src,
                               const WP_CHAR* svlan_etype,
                               const WP_CHAR* svlan,
                               const WP_CHAR* cvlan_etype,
                               const WP_CHAR* cvlan,
                               const WP_CHAR* slow_protocol_etype,
                               const WP_CHAR* slow_protocol,
                               const WP_CHAR* ip_dst,
                               const WP_CHAR* ip_src,
                               WP_handle tx_channel_handle)
{
   WP_status status;
   WP_U8 *km_data = NULL;
   WP_CHAR ascii_buffer[DATA_LENGTH * 2 + 1];

   WP_CHAR temp_print[40];

   WP_U8 hex_buffer[DATA_LENGTH * 2];
   WP_data_segment data_segment;
   WP_data_unit data_unit;
#if 0
   WP_U16 checksum;
#endif
   WP_U8 ip_offset;
   //WP_CHAR TempBuf[4];
   WP_U8 ii;

   memset(hex_buffer, 0, DATA_LENGTH * 2);
   memset(ascii_buffer, 0, DATA_LENGTH * 2 + 1);
   memset(&data_unit, 0, sizeof(data_unit));
   memset(&data_segment, 0, sizeof(data_segment));
   km_data = WP_PoolAlloc(iw_pool);

      data_unit.data_size = 100;

   data_unit.n_active = 1;

   data_segment.data = km_data;
   data_segment.pool_handle = iw_pool;
   data_segment.next = NULL;
   data_segment.displacement = 0;

   data_unit.n_segments = 1;

  // switch (WPI_HANDLE_SUBTYPE(tx_channel_handle))
  // {
   //   case WP_IW_HOST:
   //      data_unit.type = WP_DATA_IW;
         /* data_unit.type = WP_DATA_IW | WP_DATA_RETURN_ENABLE; */
  //       break;
  //    case WP_ENET:
         data_unit.type = WP_DATA_ENET;
   //      break;
 //  }

   data_unit.control = 0;

   ip_offset = 14;

   strcpy(ascii_buffer, mac_dst);/* MAC dst address */
   strcat(ascii_buffer, mac_src);/* MAC src address */

   if (svlan[0] != 0)
   {
      strcat(ascii_buffer, svlan_etype);/* ETHER_TYPE */
      strcat(ascii_buffer, svlan); /* SVLAN TAG */
      data_unit.data_size += 4;
   }

   if (cvlan[0] != 0)
   {
      strcat(ascii_buffer, cvlan_etype);/* ETHER_TYPE */
      strcat(ascii_buffer, cvlan); /* CVLAN TAG */
      data_unit.data_size += 8;
   }
   if (slow_protocol[0] != 0)
   {
      strcat(ascii_buffer, slow_protocol_etype);/* ETHER_TYPE */
      strcat(ascii_buffer, slow_protocol); /* slow_protocol TAG */
      data_unit.data_size += 8;
   }

   {
         strcat(ascii_buffer, "0800");/* IPv4 TYPE */
         strcat(ascii_buffer, "4504002e");/* tos = 0x4 */
         strcat(ascii_buffer, "001b0000");
         //         strcat (ascii_buffer, "0a110000");/* protocol = 0x11, empty checksum */
         strcat(ascii_buffer, "0a11f1ad");/* protocol = 0x11, empty checksum */

         strcat(ascii_buffer, ip_src);/* IP src address */
         strcat(ascii_buffer, ip_dst);/* IP src address */
   }

   strcat(ascii_buffer, "00190015");
   strcat(ascii_buffer, "00280000");/* length, UDP with no checksum */

   data_segment.data_size = data_unit.data_size;
   data_unit.segment = (&data_segment);

   strcat(ascii_buffer, "00a4c5c43a528a3f012363655647899abf2e");

   App_TranslateAsciiToHex(hex_buffer, ascii_buffer, data_unit.data_size);

   memset(temp_print, 0, sizeof(temp_print));

#if 1
   printf("ascii buffer size = %d\n", strlen(ascii_buffer));
   printf("data size = %d\n", data_unit.data_size);
   printf("\n\nSent Packet Zahi:\n\n");

   for (ii = 0; ii < 17; ii++)
   {
      memcpy(temp_print, ascii_buffer + ii * 8, 8);
      printf(" %s\n", temp_print);
      memset(temp_print, 0, sizeof(temp_print));
   }
   printf("\n");

#endif

#if 0
   if (svlan[0] != 0)
   ip_offset += 4;
   if(cvlan[0] != 0)
   ip_offset += 4;

   checksum = WTE_IpChecksumCalc(hex_buffer + ip_offset, 20);
   memcpy (hex_buffer + ip_offset + 10, &checksum, 2);
#endif

   WP_MEM_BYTES_FILL(km_data, '0', data_unit.data_size);
   WP_MEM_BYTES_SET(km_data, (WP_U8*) hex_buffer, data_unit.data_size);

#if DEBUG_PRINT
   printf("data_unit.segment->data: 0x%x\n", (WP_U32) data_unit.segment->data);
#endif

   status = WP_HostSend(tx_channel_handle, &data_unit);
   App_TerminateOnError(status, "WP_HostSend", __LINE__);

   return;
}

int CLI_Send_Packets (char *StrPrm)
{
   WPE_DataUnitSendToEnet("FFFFFFFFFFFF", /* mac_dst */
                             "040101010101", /* mac_src */
                             "8100", /* svlan_etype */
                             "0001", /* svlan */
                             "", /* cvlan_etype */
                             "", /* cvlan */
                             "8809", /* slow_protocol_etype */
                             "DC", /* slow_protocol */
                             "12121212", /* ip_dst */
                             "abcdef01", /* ip_src */
                             gbe[1].tx_chan_enet);

   return 0;

}
