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

extern Y_MenuEntry V_MainMenu[];

static Y_MenuEntry *V_CurrMenuP = &V_MainMenu[0];

Y_MenuEntry V_MainMenu[] = {
   {K_Menu, MAIN_MENU_ITEM_NUM, TRUE, "Main Menu", {(Y_MenuEntry *) V_MainMenu}},
   {K_Menu, 1, TRUE, " -> FH Test Menu", {(Y_MenuEntry *) FH_Test_menu}},
   {K_Menu, 2, FALSE, " -> Vlan Menu", {(Y_MenuEntry *) VLAN_menu}},
   {K_Menu, 3, FALSE, " -> Aging Menu", {(Y_MenuEntry *) AGING_menu}},
   {K_Menu, 4, TRUE, " -> Learning Menu (enable here first)", {(Y_MenuEntry *) LEARNING_menu}},
   {K_Menu, 5, TRUE, " -> System Statistics", {(Y_MenuEntry *) STAT_menu}},
   {K_Leaf, 6, TRUE, " <> Dump FDB entry",
    {(void *) (int) CLI_Dump_FDB_entry}},
   {K_Leaf, 7, FALSE, " <> Flush FDB by Vlan",
    {(void *) (int) CLI_Flush_FDB_entry_by_Vlan}},
   {K_Menu, 8, FALSE, " -> Performance test",
    {(Y_MenuEntry *) Performance_menu}},
   {K_Leaf, 9, TRUE, " <> Quit", {(void *) (int) CLI_MAIN_Quit}},
};


/*-----------------------------------------------------------*\
   FH demo show MENU
\*-----------------------------------------------------------*/
Y_MenuEntry FH_Test_menu[] = {
   {K_Menu, FHT_MENU_ITEM_NUM, TRUE, "FiberHome Test Menu",
    {(Y_MenuEntry *) V_MainMenu}},
   {K_Leaf, 1, TRUE, " -> IPV6 filtering (sent to ENET7)",
    {(void *) (int) CLI_FHT_IPV6_filter}},
   {K_Leaf, 2, TRUE, " -> L4 port filtering (DHCP, port 67, 68-send to ENET7)",
    {(void *) (int) CLI_FHT_L4_port_filter}},
   {K_Leaf, 3, TRUE, " -> L4 subtype filtering (offset 10), send to ENET7",
    {(void *) (int) CLI_FHT_L4_subtype_filter}},
   {K_Leaf, 4, TRUE, " -> Reserved MAC filtering (send to ENET7)",
    {(void *) (int) CLI_FHT_Reserved_Mac_filter}},
   {K_Leaf, 5, TRUE, " -> Max Learned Mac by port (traffic blocked case, please ENABLE first)",
    {(void *) (int) CLI_FHT_Max_Learned_Mac}},
   {K_Leaf, 6, TRUE, " -> TC remarking (send to ENET7)",
    {(void *) (int) CLI_FHT_Set_TC_Remarking}},
   {K_Leaf, 7, TRUE, " -> Create IPv6 Broadcast Group (NOT protocol packets, it is data packets)",
    {(void *) (int) CLI_FHT_Set_IPv6_Broadcast_Group}},
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
   {K_Leaf, 3, FALSE, " -> Enable port learning",
    {(void *) (int) CLI_Enable_port_lrn}},
   {K_Leaf, 4, FALSE, " -> Disable port learning",
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
   {K_Leaf, 5, TRUE, " <> Show FlwAgg Stats==========",
    {(void *) (int) CLI_STAT_ShowFlwAggStats}},
   {K_Leaf, 6, TRUE, " <> Clear FlwAgg Stats",
    {(void *) (int) CLI_STAT_ClearFlwAggStats}},
   {K_Leaf, 7, TRUE, " <> Show FlwAgg Reserved1 Mac",
    {(void *) (int) CLI_STAT_ShowFlwAggReservedMacStats}},
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

   for (i = 0; i < NR_GBE; i++)
   {
      printf ("\n               Enet[%d] FlowAgg Statistics\n", i);
      WPT_GenericFlowStatistics (gbe[i].agg_enet);
      //WPT_GenericFlowStatisticsReset(gbe[i].agg_enet);

      printf ("\n+++++++++++++++++++++++++++++++++++++++++\n");
      printf ("+++++++++++++++++++++++++++++++++++++++++\n");
      printf ("+++++++++++++++++++++++++++++++++++++++++\n");
      printf ("\n               Enet[%d] FlowAgg Reserved Mac Statistics\n", i);
      WPT_GenericFlowStatistics (gbe[i].agg_reserved1);

      printf ("\n=========================================\n");
      printf ("=========================================\n");
      printf ("=========================================\n");
      printf ("\n               Enet[%d] FlowAgg IPv6 Broadcast Statistics\n", i);
      WPT_GenericFlowStatistics (gbe[i].agg_multicast);
   }

   printf ("\n              Host FlowAgg Statistics\n");
   WPT_GenericFlowStatistics (default_agg_host);

   printf ("\n               Enet[%d] Host FlowAgg IGMP Statistics\n", i);
   WPT_GenericFlowStatistics (agg_host_igmp);

   return WP_OK;
}





/*****************************************************************************
 * Function name: CLI_STAT_ClearFlwAggStats
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
int CLI_STAT_ShowFlwAggReservedMacStats (char *StrPrm)
{
   WP_U32 i;

   for (i = 0; i < NR_GBE; i++)
   {
      printf ("\n               Reseting Enet[%d] FlowAgg Statistics\n",
              i);
      //WPT_GenericFlowStatistics(gbe[i].agg_enet);
      WPT_GenericFlowStatisticsReset (gbe[i].agg_enet);
   }

   printf ("\n              Reset Host FlowAgg Statistics\n");
   WPT_GenericFlowStatisticsReset (default_agg_host);

   printf ("\n               Done!\n");
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

   for (i = 0; i < NR_GBE; i++)
   {
      printf ("\n               Reseting Enet[%d] FlowAgg Statistics\n",
              i);
      //WPT_GenericFlowStatistics(gbe[i].agg_enet);
      WPT_GenericFlowStatisticsReset (gbe[i].agg_enet);
   }

   printf ("\n              Reset Host FlowAgg Statistics\n");
   WPT_GenericFlowStatisticsReset (default_agg_host);
   printf ("\n              Reset Host FlowAgg IGMP Statistics\n");
   WPT_GenericFlowStatisticsReset (agg_host_igmp);

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

   // WPE_CreateReservedMacPceRule (0, mac);

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


extern void WPE_TrafficRemarkingEnableIPv6 (char *StrPrm,  
                                       WP_handle agg_enet_ipv6,
                                       WP_U8 remarkVal);

int CLI_FHT_Set_TC_Remarking (char *StrPrm)
{
   WP_U32 temp = 0;
   WP_U32 port = 0;

   temp = CLI_GetNumber ("Input port number ( 0 - 1)[0=ENET8(send), 1=ENET7(TC set here)]", 0, 1);
   port = temp;
   temp = CLI_GetNumber ("Max mac number ( 0 - 255)", 1, 255);
   gbe[port].tc_remark = temp;

   WPE_TrafficRemarkingEnableIPv6  ("hello", gbe[port].agg_enet_ipv6, temp);

   return 0;
}

extern void WPE_CreateIPV6GroupBroadcastMatchPceRule (WP_U8 portid, WP_U8 * ipv6);

int CLI_FHT_Set_IPv6_Broadcast_Group (char *StrPrm)
{
   WP_U8 ipv6[16] = { 0x00 };
   WP_CHAR InputBuf[255];
   WP_U8 i = 0;

   printf ("Enter IPV6 address for Creating Broadcast Group>\n");
   printf ("e.g. ff-01-02-03-04-05-06-07-08-09-0a-0b-0c-0d-0e-0f\n");
   get_line (InputBuf);
   printf ("\n");

   F_ConvertStr2Ipv6Address (InputBuf, ipv6);
   for (i = 0; i < 16; i ++)
   {
      printf ("%2d:", ipv6[i]);
   }
   printf ("\n");
   if (0xff != ipv6[0])
   {
      printf ("Must input a broadcast IPv6 address start with 0xff ....\n");
      return 0;
   }

   WPE_CreateIPV6GroupBroadcastMatchPceRule (0, ipv6);


   return 0;
}
