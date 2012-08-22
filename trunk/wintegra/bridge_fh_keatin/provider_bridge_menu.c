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
   {K_Menu, MAIN_MENU_ITEM_NUM, TRUE, "Main Menu",
    {(Y_MenuEntry *) V_MainMenu}},
   {K_Menu, 1, TRUE, " -> FH Test Menu", {(Y_MenuEntry *) FH_Test_menu}},
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
};

extern int CLI_FHT_Case36_SmacDmacSipDipTos2SVlan (char *StrPrm);

Y_MenuEntry FH_Test_menu[] = {
   {K_Menu, FHT_MENU_ITEM_NUM, TRUE, "FiberHome Test Menu",
    {(Y_MenuEntry *) V_MainMenu}},
   {K_Leaf, 1, FALSE, " -> IPV6 filtering",
    {(void *) (int) CLI_FHT_IPV6_filter}},
   {K_Leaf, 2, FALSE, " -> L4 port filtering",
    {(void *) (int) CLI_FHT_L4_port_filter}},
   {K_Leaf, 3, FALSE, " -> L4 subtype filtering",
    {(void *) (int) CLI_FHT_L4_subtype_filter}},
   {K_Leaf, 4, FALSE, " -> Reserved MAC filtering",
    {(void *) (int) CLI_FHT_Reserved_Mac_filter}},
   {K_Leaf, 5, FALSE, " -> Max Learned Mac by port",
    {(void *) (int) CLI_FHT_Max_Learned_Mac}},
   {K_Leaf, 6, TRUE, " -> Case34 Uc Bc packet -> Vlan Prio",
    {(void *) (int) CLI_FHT_Case34_UcBc2Prio}},
   {K_Leaf, 7, TRUE, " -> Case35 SMAC DMAC SIP DIP TOS -> differ SVLAN",
    {(void *) (int) CLI_FHT_Case35_SmacDmacSipDipTos2SVlan}},
   {K_Leaf, 8, TRUE, " -> Case36 SMAC DMAC SIP DIP TOS -> same SVLAN",
    {(void *) (int) CLI_FHT_Case36_SmacDmacSipDipTos2SVlan}},
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

#ifdef __linux__   //keatin
  static struct termios oldt;
#endif

void restore_terminal_settings (void)
{
   //Apply saved settings
#ifdef __linux__   //keatin
   tcsetattr (0, TCSANOW, &oldt);
#endif
}

//make terminal read 1 char at a time
void disable_terminal_return (void)
{
#ifdef __linux__   //keatin
   
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
#endif
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
      case KEY_RETURN:
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
   }

   printf ("\n              Host FlowAgg Statistics\n");
   WPT_GenericFlowStatistics (default_agg_host);

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
#ifdef __linux__   //keatin
   if (aging_enable)
   {
      OS_AgingTimerDelete ();
   }

   aging_enable = 1;

   // create and start the timer
   OS_AgingTimerCreate ();
   OS_AgingTimerStart ();
#endif
   return OK;
}

int CLI_Disable_aging (char *StrPrm)
{
#ifdef __linux__   //keatin
   if (aging_enable)
   {
      OS_AgingTimerDelete ();
   }

   aging_enable = 0;
#endif
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


/********************  ADDED BY KEATIN ***********************/
#define MORRIS_MOD
#define HOST_SEND 
#undef HOST_SEND

struct  testingWddiObj testwddiobj = {0, {0}, 0, {0}, 0};
WP_pce_filter_stat pce_filter_stats[256];

void WPE_PrintPceFilterStats(WP_handle filter)
{
   WP_U32 index;
   WP_U32 temp1, temp2;

   index = filter & 0xFF;

   WP_PceFilterStatistics (filter, &(pce_filter_stats[index]));

   printf("PCE Filter %d Statistics:\n", index);

   temp1 = pce_filter_stats[index].denied_pkts;
   temp2 = pce_filter_stats[index].denied_pkts >> 32;
   if (temp1 || temp2)
      printf("denied_pkts            %08X %08X\n", temp2, temp1);

   temp1 = pce_filter_stats[index].matched_pkts;
   temp2 = pce_filter_stats[index].matched_pkts >> 32;
   if (temp1 || temp2)
      printf("matched_pkts           %08X %08X\n", temp2, temp1);

   temp1 = pce_filter_stats[index].no_matched_pkts;
   temp2 = pce_filter_stats[index].no_matched_pkts >> 32;
   if (temp1 || temp2)
      printf("no_matched_pkts        %08X %08X\n", temp2, temp1);
}



// msb copied first.
void assemble_packet(WP_U8 *data_ptr, WP_U16 size, WP_U32 value)
{
  WP_U16 i;
  if(size > 4)
    size = 4;
  for(i = 0; i < size; i++)
  {
    data_ptr[i] = (value >> (8*(size - 1 - i))) & 0xff;
  }
}


//  data_ptr = WP_PoolAlloc(pool);
//  size = packet size

void CLI_HostSendPacket(WP_U8 *data_ptr, WP_U16 size)
{
   WP_U32 k;
   WP_status status;
   WP_data_segment* curr_buf;
   WP_data_unit data_unit;
   WP_data_segment tx_buf;

   WP_handle pool = qniw_adjpool;
   WP_handle txchan = gbe[0].tx_chan_enet;

   data_unit.data_size = size;
   data_unit.n_active = 1;

   tx_buf.data_size = data_unit.data_size;
   tx_buf.data = data_ptr;
   tx_buf.pool_handle = pool;
   tx_buf.displacement = 0;
   tx_buf.user_info = 0;
   tx_buf.next = NULL;
   tx_buf.buffer_class = 0;

   data_unit.control = 0;
   data_unit.segment = (&tx_buf);
   data_unit.n_segments = 1;
   data_unit.n_active = 1;
   data_unit.type = WP_DATA_ENET;
   data_unit.data_return = 0;
   data_unit.status = 0;

   printf ("send packet:\n ");
   curr_buf = data_unit.segment;
   for (k = 0; k < curr_buf->data_size; k++)
   {
     if(k %16 == 0) printf("\n0x%04x: ", k);
     if(k %4 == 0 && k %16 != 0) printf(" ", k);
     printf("%2.2x", curr_buf->data[k]);
   }
   printf("\n");

   status = WP_HostSend(txchan, &data_unit);
   App_TerminateOnError (status,
                        "WP_HostSend()", __LINE__);

}

int CLI_FHT_ClearTestingWddiObj (void)
{
   WP_U32 i;
   if(testwddiobj.enable == 0)
   {
      return 0;
   }
   for(i = 0; i < 10; i++)
   {
     if(testwddiobj.filter[i] != 0)
     {
       WP_PceFilterDelete(testwddiobj.filter[i]);
       testwddiobj.filter[i] = 0;
     }
    }
   if(testwddiobj.filter_set != 0)
   {
     WP_PceFilterSetDelete(testwddiobj.filter_set);
     testwddiobj.filter_set = 0;
   }
   for(i = 0; i < 100; i++)
   {
     if(testwddiobj.rules[i] != 0)
     {
       WP_PceRuleDelete(testwddiobj.rules[i]);
       testwddiobj.rules[i] = 0;
     }
   }
   if(testwddiobj.txfa != 0)
   {
     WP_IwFlowAggregationDelete(testwddiobj.txfa);
     testwddiobj.txfa = 0;
   }
   testwddiobj.enable = 0;
   return 1;
}

// host -> enet1 tx chan -> enet1 rx chan -> pce -> fa -> enet2 tx chan -> enet2 rx -> ht_fa -> ht_rx_chan
// 

/*
case34
in this case, uc packet, change vlan prio = 1
in this case, bc packet, change vlan prio = 2
*/

int CLI_FHT_Case34_UcBc2Prio (char *StrPrm)
{
   WP_U32 temp, portid;
   
   WP_pce_filter_classification filter_class = { 0 };
   WP_handle filter;
   WP_handle filter_set;
   WP_handle txfa;
   WP_pce_filter_set fs_level;
   WP_pce_rule_classification rule_cfg = { 0 };
   WP_handle port_handle = 0, agg_handle = 0, h_pce_rule = 0;

   WP_pce_if_params_pkt_rx_channel pce_if_params = { 0 };
   WP_handle pce_if_handle;
   WP_status status;
   WP_U32 ii;
   WP_handle iw_sys = dl_general_iwsys_bridge;

#ifdef HOST_SEND
  temp = 0;
#else
  temp = CLI_GetNumber ("Input port number ( 0 - 1)", 0, 1);
#endif
  portid = temp;

   CLI_FHT_ClearTestingWddiObj();

   WP_U32 i = 0;

   WPE_vlan_edit_options veoption = {
      /* egress_rule; */{
	    WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
	  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
	  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
	  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
	  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
	  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
      },
      /* replace_int_vlan_mode;*/ WPE_GPE_BROUTER_PECS_REPLACE_VLAN_DISABLE,
      /* int_vlan_tag;*/ 0,
      /*  int_vlan_etype_source;*/ WPE_GPE_BROUTER_PECS_VLAN_ETYPE_8100,
      /*  replace_ext_vlan_mode;*/WPE_GPE_BROUTER_PECS_REPLACE_VLAN_PRIORITY,
      /* ext_vlan_tag;*/0,
      /*  ext_vlan_etype_source;*/WPE_GPE_BROUTER_PECS_VLAN_ETYPE_8100,
      /* vlan_stag_etype;*/0x8100,
      /*  replace_sub_port_vlan_mode;*/WPE_GPE_BROUTER_PECS_SUB_PORT_VLAN_DISABLE,
      /* sub_port_vlan_tag;*/0x0,
   };

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
       &veoption,                       /* vlan_edit_options */
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
   i = 0;
   // keatin here use modify maybe better. or delete it first.

   /* DL GE TX flow agg */

  dl_tx_agg_gbe->txfunc = gbe[1].tx_chan_enet;
  dl_tx_agg_gbe->iw_port = gbe[1].bport_enet;
  dl_tx_agg_gbe->pecs_handle = pecs_handles[0];
  dl_tx_agg_gbe->pecs_flow_info = (void *) &brouter_pecs_flow_info[0];
  txfa = WP_IwFlowAggregationCreate (WP_WINPATH (DEFAULT_WPID),
                                 WP_IW_GENERIC_MODE, &dl_tx_agg_gbe);
  App_TerminateOnError (txfa,
                      "WP_IwFlowAggregationCreate()", __LINE__);
  testwddiobj.txfa = txfa;
  
  /* create unknown unicast classification filter */
  filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
  filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;
  
  filter_class.no_match_result[0].result_type =
    WP_PCE_RESULT_LAST;
  
  filter_class.filter_fields[0].field_mode =  WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
  filter_class.filter_fields[0].mask_mode =  WP_PCE_FIELD_MASK_USED;
  filter_class.filter_fields[0].mask.parser_flags  = WP_PCE_MASK_PARSER_FLAG_MC; // here
  filter_class.filter_fields[0].field_id =  WP_PCE_FIELD_ID_PARSER_FLAGS;
  
  filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
  
  filter = WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID),
                        WP_PCE_FILTER_CLASSIFICATION,
                        &filter_class);
  App_TerminateOnError (filter,"WP_PceFilterCreate", __LINE__);
  testwddiobj.filter[0] = filter;
  testwddiobj.enable = 1;
  
  /* filter set */
  fs_level.filter_set_level = 0;
  fs_level.next_filter_set = WP_UNUSED;
  fs_level.filters[0] = filter;
  fs_level.filters[1] = WP_UNUSED;
  
  filter_set =
    WP_PceFilterSetCreate (WP_WINPATH (DEFAULT_WPID), &fs_level);
  App_TerminateOnError (filter_set, "WP_PceFilterSetCreate",
                       __LINE__);
  testwddiobj.filter_set = filter_set;
  
  switch (portid)
  {
  case 0:
  case 1:
    port_handle = gbe[portid].bport_enet;
    agg_handle = txfa;
    break;
  default:
    printf ("NO such port : %d\n", portid);
    return 0;
  }
  
  // rule1 for uc
  rule_cfg.enabled = WP_ENABLE;
  
  rule_cfg.filter_handle = filter;
  
  rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_PARSER_FLAGS;
  rule_cfg.rule_fields[0].value.parser_flags = 0;
  
  rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
  
  rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
  
  rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
  rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_handle;   // flowAgg
  
  rule_cfg.match_result[1].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
  #if 0
  rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_EXT_VLAN_PRIO;
  rule_cfg.match_result[1].param.prefix_priority_remarking.value[0] = 1;
  rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
  rule_cfg.match_result[1].param.prefix_priority_remarking.value[1] = 0;  
  #else
  rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
  rule_cfg.match_result[1].param.prefix_priority_remarking.value[0] = 0;
  rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_INT_VLAN_PRIO;
  rule_cfg.match_result[1].param.prefix_priority_remarking.value[1] = 3;  // modify PRIO to 3 if old = 0
  #endif

  rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
  rule_cfg.match_result[1].param.prefix_priority_remarking.value[2] = 0;
  rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
  rule_cfg.match_result[1].param.prefix_priority_remarking.value[3] = 0;
  
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
  testwddiobj.rules[0]= h_pce_rule;
  
  // rule2 for bc =================================================
  rule_cfg.enabled = WP_ENABLE;
  
  rule_cfg.filter_handle = filter;
  
  rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_PARSER_FLAGS;
  rule_cfg.rule_fields[0].value.parser_flags = WP_PCE_MASK_PARSER_FLAG_MC; // set BC flag
  
  rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
  
  rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
  
  rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
  rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_handle;
  
  rule_cfg.match_result[1].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
  
  #if 0
  rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_EXT_VLAN_PRIO;
  rule_cfg.match_result[1].param.prefix_priority_remarking.value[0] = 2;
  rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
  rule_cfg.match_result[1].param.prefix_priority_remarking.value[1] = 0;  
  #else
  rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
  rule_cfg.match_result[1].param.prefix_priority_remarking.value[0] = 0;
  rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_INT_VLAN_PRIO;
  rule_cfg.match_result[1].param.prefix_priority_remarking.value[1] = 4;  
  #endif
  rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
  rule_cfg.match_result[1].param.prefix_priority_remarking.value[2] = 0;
  rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
  rule_cfg.match_result[1].param.prefix_priority_remarking.value[3] = 0;
  
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
  testwddiobj.rules[1]= h_pce_rule;
  
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
  pce_if_params.filter_set_handle = filter_set; // =======================
  pce_if_params.ip_header_validation = WP_DISABLE;
  //pce_if_params.user_programmable_fields_handle = 0;//fht
  
  pce_if_handle =
    WP_PceInterfaceCreate (WP_PCE_IF_TYPE_PKT_RX_CHANNEL,
                           &pce_if_params);
  App_TerminateOnError (pce_if_handle, "WP_PceInterfaceCreate()",
                       __LINE__);
  
  /* Modify the PCE interface for GE RX channel handle */
  for (ii = 0; ii < 1 /*NR_GBE*/; ii++)  // leave enet2 to host for checking
  {
    rx_binding_cfg[0].pce_if_handle = pce_if_handle;
    rx_binding_cfg[0].input_port = gbe[ii].bport_enet;
    
    status = WP_IwRxBindingModify (gbe[ii].rx_chan_enet,
                                   iw_sys,
                                   qniw,
                                   WP_IW_RX_BIND_MOD_PCE_INTERFACE,
                                   &rx_binding_cfg[0]);
    App_TerminateOnError (status, "WP_IwRxBindingModify", __LINE__);
  }

  WP_PceFilterStatisticsReset(filter, &(pce_filter_stats[filter & 0xFF]));

#ifdef HOST_SEND
  // send a tx packet
  CLI_HostSendPacketCase34();
  
  printf("Waiting for a while...\n");
  for(i=0;i<5000000;i++);
  
  WPE_PrintPceFilterStats(filter);
#endif

  return 0;
}


void CLI_HostSendPacketCase34(void)
{
   WP_U32 i, size=100;
   WP_U8 *data_ptr;
   WP_handle pool = qniw_adjpool;

   data_ptr = WP_PoolAlloc(pool);
   if(data_ptr == 0)
   {
     printf("no free buffer in pool.\n");
     return ;
   }
   printf("assembiling packet at 0x%08x\n", &data_ptr[0]);

   // uc packet,  dmac=00...01 smac=00..02
   assemble_packet(&data_ptr[0], 4, 0x00000000);
   assemble_packet(&data_ptr[4], 4, 0x00010000);
   assemble_packet(&data_ptr[8], 4, 0x00000002);
   assemble_packet(&data_ptr[12], 4, 0x81000001); /* vlan = 1, should got 0x81003000*/
   assemble_packet(&data_ptr[16], 2, 0x0800);   // VLAN type
   assemble_packet(&data_ptr[18], 4, 0x45040020);/* tos = 4 */
   assemble_packet(&data_ptr[22], 4, 0x001b0000);
   assemble_packet(&data_ptr[26], 4, 0x0a11e449);/* protocol = 0x11*/
   assemble_packet(&data_ptr[30], 4, 0x00010c20);/* sa ip */
   assemble_packet(&data_ptr[34], 4, 0xc0440000);/* da ip */
   assemble_packet(&data_ptr[38], 4, 0x00190015);
   assemble_packet(&data_ptr[42], 4, 0x001b77c0);
   assemble_packet(&data_ptr[46], 4, 0x00a4c5ff);

   printf("assembiling packet payload\n");
   for(i = 50; i < size; i++)
   {
     data_ptr[i] = (i - 50);
   }

   printf("send packet 1\n");
   CLI_HostSendPacket(data_ptr,  size);
   printf("Waiting for a while...\n");
   for(i=0;i<5000000;i++);

   data_ptr = WP_PoolAlloc(pool);
   if(data_ptr == 0)
   {
     printf("no free buffer in pool.\n");
     return ;
   }
   printf("assembiling packet at 0x%08x\n", &data_ptr[0]);

   // uc packet,  dmac=00...01 smac=00..02
   assemble_packet(&data_ptr[0], 4, 0xffffffff);
   assemble_packet(&data_ptr[4], 4, 0xffff0000);
   assemble_packet(&data_ptr[8], 4, 0x00000002);
   assemble_packet(&data_ptr[12], 4, 0x81000002);/* vlan = 1, should 0x81008002*/
   assemble_packet(&data_ptr[16], 2, 0x0800);
   assemble_packet(&data_ptr[18], 4, 0x45040020);/* tos = 4 */
   assemble_packet(&data_ptr[22], 4, 0x001b0000);
   assemble_packet(&data_ptr[26], 4, 0x0a11e449);/* protocol = 0x11*/
   assemble_packet(&data_ptr[30], 4, 0x00010c20);/* sa ip */
   assemble_packet(&data_ptr[34], 4, 0xc0440000);/* da ip */
   assemble_packet(&data_ptr[38], 4, 0x00190015);
   assemble_packet(&data_ptr[42], 4, 0x001b77c0);
   assemble_packet(&data_ptr[46], 4, 0x00a4c5ff);

   printf("assembiling packet payload\n");
   for(i = 50; i < size; i++)
   {
     data_ptr[i] = (i - 50);
   }

   printf("send packet 2\n");
   CLI_HostSendPacket(data_ptr,  size);
   
}

/*
case 35
smac=00..02, dmac=00..01, sip=10.0.0.2,dip=10.0.0.1,tos=1 vlan ->100
smac=00..04, dmac=00..03, sip=10.0.0.4,dip=10.0.0.3,tos=3 vlan ->300

this case is not tested pass in WDDS4.1 for not able to set INT_VLAN update and FA together in one filter
I believed it can be set in WDDS4.3
if in WDDS4.3 can not do it either, we shall split the mathing result into 2 filters.
please refer to Case36, which is an example of spliting.
*/

int CLI_FHT_Case35_SmacDmacSipDipTos2SVlan (char *StrPrm)
{
   WP_U32 temp, portid;
   
   WP_pce_filter_classification filter_class = { 0 };
   WP_handle filter;
   WP_handle filter_set;
   WP_handle txfa;
   WP_pce_filter_set fs_level;
   WP_pce_rule_classification rule_cfg = { 0 };
   WP_handle port_handle = 0, agg_handle = 0, h_pce_rule = 0;

   WP_pce_if_params_pkt_rx_channel pce_if_params = { 0 };
   WP_handle pce_if_handle;
   WP_status status;
   WP_U32 ii;
   WP_handle iw_sys = dl_general_iwsys_bridge;

#ifdef HOST_SEND
  temp = 0;
#else
  temp = CLI_GetNumber ("Input port number ( 0 - 1)", 0, 1);
#endif
  portid = temp;

   CLI_FHT_ClearTestingWddiObj();

   WP_U32 i = 0;

   WPE_vlan_edit_options veoption = {
      /* egress_rule; */{
	    WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
	  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
	  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
	  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
	  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
	  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
      },
      /* replace_int_vlan_mode;*/ WPE_GPE_BROUTER_PECS_REPLACE_VLAN_ID,
      /* int_vlan_tag;*/ 11,  /////////////////////////
      /*  int_vlan_etype_source;*/ WPE_GPE_BROUTER_PECS_VLAN_ETYPE_8100,
      /*  replace_ext_vlan_mode;*/WPE_GPE_BROUTER_PECS_REPLACE_VLAN_PRIORITY,
      /* ext_vlan_tag;*/0,
      /*  ext_vlan_etype_source;*/WPE_GPE_BROUTER_PECS_VLAN_ETYPE_8100,
      /* vlan_stag_etype;*/0x8100,
      /*  replace_sub_port_vlan_mode;*/WPE_GPE_BROUTER_PECS_SUB_PORT_VLAN_DISABLE,
      /* sub_port_vlan_tag;*/0x0,
   };

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
       &veoption,                       /* vlan_edit_options */
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
   i = 0;
   /* DL GE TX flow agg */

  dl_tx_agg_gbe->txfunc = gbe[1].tx_chan_enet;
  dl_tx_agg_gbe->iw_port = gbe[1].bport_enet;
  dl_tx_agg_gbe->pecs_handle = pecs_handles[0];
  dl_tx_agg_gbe->pecs_flow_info = (void *) &brouter_pecs_flow_info[0];
  txfa = WP_IwFlowAggregationCreate (WP_WINPATH (DEFAULT_WPID),
                                 WP_IW_GENERIC_MODE, &dl_tx_agg_gbe);
  App_TerminateOnError (txfa,
                        "WP_IwFlowAggregationCreate()", __LINE__);
  testwddiobj.txfa = txfa;
  
  /* create unknown unicast classification filter */
  filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
  filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;
  
  filter_class.no_match_result[0].result_type =
    WP_PCE_RESULT_LAST;
  
  filter_class.filter_fields[0].field_mode =  WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
  filter_class.filter_fields[0].mask_mode =  WP_PCE_FIELD_MASK_NOT_USED;
  filter_class.filter_fields[0].field_id =  WP_PCE_FIELD_ID_MAC_DA;

  filter_class.filter_fields[1].field_mode =  WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
  filter_class.filter_fields[1].mask_mode =  WP_PCE_FIELD_MASK_NOT_USED;
  filter_class.filter_fields[1].field_id =  WP_PCE_FIELD_ID_MAC_SA;

  filter_class.filter_fields[2].field_mode =  WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
  filter_class.filter_fields[2].mask_mode =  WP_PCE_FIELD_MASK_NOT_USED;
  filter_class.filter_fields[2].field_id =  WP_PCE_FIELD_ID_IPV4_SA;
  
  filter_class.filter_fields[3].field_mode =  WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
  filter_class.filter_fields[3].mask_mode =  WP_PCE_FIELD_MASK_NOT_USED;
  filter_class.filter_fields[3].field_id =  WP_PCE_FIELD_ID_IPV4_DA;
  
  filter_class.filter_fields[4].field_mode =  WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
  filter_class.filter_fields[4].mask_mode =  WP_PCE_FIELD_MASK_NOT_USED;
  filter_class.filter_fields[4].field_id =  WP_PCE_FIELD_ID_IPV4_TOS;
  
  filter_class.filter_fields[5].field_id = WP_PCE_FIELD_ID_LAST;
  
  filter = WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID),
                        WP_PCE_FILTER_CLASSIFICATION,
                        &filter_class);
  App_TerminateOnError (filter,"WP_PceFilterCreate", __LINE__);
  testwddiobj.filter[0] = filter;
  testwddiobj.enable = 1;
  
  /* filter set */
  fs_level.filter_set_level = 0;
  fs_level.next_filter_set = WP_UNUSED;
  fs_level.filters[0] = filter;
  fs_level.filters[1] = WP_UNUSED;
  
  filter_set =
    WP_PceFilterSetCreate (WP_WINPATH (DEFAULT_WPID), &fs_level);
  App_TerminateOnError (filter_set, "WP_PceFilterSetCreate",
                       __LINE__);
  testwddiobj.filter_set = filter_set;
  
  switch (portid)
  {
  case 0:
  case 1:
    port_handle = gbe[portid].bport_enet;
    agg_handle = txfa;
    break;
  default:
    printf ("NO such port : %d\n", portid);
    return 0;
  }
  
  // rule1 for  smac=00..01, dmac=00..02, sip=10.0.0.1,dip=10.0.0.2,tos=1 vlan ->100
  rule_cfg.enabled = WP_ENABLE;
  
  rule_cfg.filter_handle = filter;
  
  rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
  rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
  rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
  rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
  rule_cfg.rule_fields[0].value.mac_addr[3] = 0x00;
  rule_cfg.rule_fields[0].value.mac_addr[4] = 0x00;
  rule_cfg.rule_fields[0].value.mac_addr[5] = 0x01;
  
  rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_SA;
  rule_cfg.rule_fields[1].value.mac_addr[0] = 0x00;
  rule_cfg.rule_fields[1].value.mac_addr[1] = 0x00;
  rule_cfg.rule_fields[1].value.mac_addr[2] = 0x00;
  rule_cfg.rule_fields[1].value.mac_addr[3] = 0x00;
  rule_cfg.rule_fields[1].value.mac_addr[4] = 0x00;
  rule_cfg.rule_fields[1].value.mac_addr[5] = 0x02;

  rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_IPV4_SA;
  rule_cfg.rule_fields[2].value.ipv4_addr = 0x0a000002;
  
  rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_IPV4_DA;
  rule_cfg.rule_fields[3].value.ipv4_addr = 0x0a000001;
  
  rule_cfg.rule_fields[4].field_id = WP_PCE_FIELD_ID_IPV4_TOS;
  rule_cfg.rule_fields[4].value.ipv4_tos = 0x1;
  
  rule_cfg.rule_fields[5].field_id = WP_PCE_FIELD_ID_LAST;

  rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
  
  rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;// ===============
  rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_handle;

  rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INT_VLAN_UPDATE/*WP_PCE_RESULT_INT_VLAN_UPDATE*/;//===========
  rule_cfg.match_result[1].param.int_vlan.vlan_tag = 0x100;
  rule_cfg.match_result[1].param.int_vlan.mask = 0xfff;

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
  testwddiobj.rules[0]= h_pce_rule;

  // rule2 for  smac=00..03, dmac=00..04, sip=10.0.0.3,dip=10.0.0.4,tos=3  vlan -> 300
  rule_cfg.enabled = WP_ENABLE;
  
  rule_cfg.filter_handle = filter;
  
  rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
  rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
  rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
  rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
  rule_cfg.rule_fields[0].value.mac_addr[3] = 0x00;
  rule_cfg.rule_fields[0].value.mac_addr[4] = 0x00;
  rule_cfg.rule_fields[0].value.mac_addr[5] = 0x03;
  
  rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_SA;
  rule_cfg.rule_fields[1].value.mac_addr[0] = 0x00;
  rule_cfg.rule_fields[1].value.mac_addr[1] = 0x00;
  rule_cfg.rule_fields[1].value.mac_addr[2] = 0x00;
  rule_cfg.rule_fields[1].value.mac_addr[3] = 0x00;
  rule_cfg.rule_fields[1].value.mac_addr[4] = 0x00;
  rule_cfg.rule_fields[1].value.mac_addr[5] = 0x04;

  rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_IPV4_SA;
  rule_cfg.rule_fields[2].value.ipv4_addr = 0x0a000004;
  
  rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_IPV4_DA;
  rule_cfg.rule_fields[3].value.ipv4_addr = 0x0a000003;
  
  rule_cfg.rule_fields[4].field_id = WP_PCE_FIELD_ID_IPV4_TOS;
  rule_cfg.rule_fields[4].value.ipv4_tos = 0x3;

  rule_cfg.rule_fields[5].field_id = WP_PCE_FIELD_ID_LAST;
  
  // results
  rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
  
  rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
  rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_handle;
  
  rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INT_VLAN_UPDATE;
  rule_cfg.match_result[1].param.int_vlan.vlan_tag = 0x300;
  rule_cfg.match_result[1].param.int_vlan.mask = 0xfff;
  
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
  testwddiobj.rules[1]= h_pce_rule;

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
  pce_if_params.filter_set_handle = filter_set;
  pce_if_params.ip_header_validation = WP_DISABLE;
  //pce_if_params.user_programmable_fields_handle = 0;//fht
  
  pce_if_handle =
    WP_PceInterfaceCreate (WP_PCE_IF_TYPE_PKT_RX_CHANNEL,
                           &pce_if_params);
  App_TerminateOnError (pce_if_handle, "WP_PceInterfaceCreate()",
                       __LINE__);
  
  /* Modify the PCE interface for GE RX channel handle */
  for (ii = 0; ii < 1 /*NR_GBE*/; ii++)  // leave enet2 to host for checking
  {
    rx_binding_cfg[0].pce_if_handle = pce_if_handle;
    rx_binding_cfg[0].input_port = gbe[ii].bport_enet;
    
    status = WP_IwRxBindingModify (gbe[ii].rx_chan_enet,
                                   iw_sys,
                                   qniw,
                                   WP_IW_RX_BIND_MOD_PCE_INTERFACE,
                                   &rx_binding_cfg[0]);
    App_TerminateOnError (status, "WP_IwRxBindingModify", __LINE__);
  }

  WP_PceFilterStatisticsReset(filter, &(pce_filter_stats[filter & 0xFF]));

#ifdef HOST_SEND
  // send a tx packet
  CLI_HostSendPacketCase35();
  
  printf("Waiting for a while...\n");
  for(i=0;i<5000000;i++);
  
  WPE_PrintPceFilterStats(filter);
#endif

   return 0;
}

void CLI_HostSendPacketCase35(void)
{
   WP_U32 i, size=100;
   WP_U8 *data_ptr;
   WP_handle pool = qniw_adjpool;

   data_ptr = WP_PoolAlloc(pool);
   if(data_ptr == 0)
   {
     printf("no free buffer in pool.\n");
     return ;
   }
   printf("assembiling packet at 0x%08x\n", &data_ptr[0]);

   // uc packet,  dmac=00...01 smac=00..02
   assemble_packet(&data_ptr[0], 4, 0x00000000);
   assemble_packet(&data_ptr[4], 4, 0x00010000);
   assemble_packet(&data_ptr[8], 4, 0x00000002);
   assemble_packet(&data_ptr[12], 4, 0x81000001);/* vlan = 1*/
   assemble_packet(&data_ptr[16], 2, 0x0800);
   assemble_packet(&data_ptr[18], 4, 0x45010020);/* tos = 1 */
   assemble_packet(&data_ptr[22], 4, 0x001b0000);
   assemble_packet(&data_ptr[26], 4, 0x0a11e449);/* protocol = 0x11*/
   assemble_packet(&data_ptr[30], 4, 0x0a000001);/* sa ip */
   assemble_packet(&data_ptr[34], 4, 0x0a000002);/* da ip */
   assemble_packet(&data_ptr[38], 4, 0x00190015);
   assemble_packet(&data_ptr[42], 4, 0x001b77c0);
   assemble_packet(&data_ptr[46], 4, 0x00a4c5ff);

   printf("assembiling packet payload\n");
   for(i = 50; i < size; i++)
   {
     data_ptr[i] = (i - 50);
   }

   printf("send packet 1\n");
   CLI_HostSendPacket(data_ptr,  size);
   printf("Waiting for a while...\n");
   for(i=0;i<5000000;i++);

   data_ptr = WP_PoolAlloc(pool);
   if(data_ptr == 0)
   {
     printf("no free buffer in pool.\n");
     return ;
   }
   printf("assembiling packet at 0x%08x\n", &data_ptr[0]);

   // uc packet,  dmac=00...03 smac=00..04
   assemble_packet(&data_ptr[0], 4, 0x00000000);
   assemble_packet(&data_ptr[4], 4, 0x00030000);
   assemble_packet(&data_ptr[8], 4, 0x00000004);
   assemble_packet(&data_ptr[12], 4, 0x81000001);/* vlan = 1*/
   assemble_packet(&data_ptr[16], 2, 0x0800);
   assemble_packet(&data_ptr[18], 4, 0x45030020);/* tos = 3 */
   assemble_packet(&data_ptr[22], 4, 0x001b0000);
   assemble_packet(&data_ptr[26], 4, 0x0a11e449);/* protocol = 0x11*/
   assemble_packet(&data_ptr[30], 4, 0x0a000003);/* sa ip */
   assemble_packet(&data_ptr[34], 4, 0x0a000004);/* da ip */
   assemble_packet(&data_ptr[38], 4, 0x00190015);
   assemble_packet(&data_ptr[42], 4, 0x001b77c0);
   assemble_packet(&data_ptr[46], 4, 0x00a4c5ff);


   printf("assembiling packet payload\n");
   for(i = 50; i < size; i++)
   {
     data_ptr[i] = (i - 50);
   }

   printf("send packet 2\n");
   CLI_HostSendPacket(data_ptr,  size);
   
}
#define NEW_VLAN  (0x100)

/*
case 35
smac=00..01, dmac=00..02, sip=10.0.0.1,dip=10.0.0.2,tos=1 vlan ->100
smac=00..03, dmac=00..04, sip=10.0.0.3,dip=10.0.0.4,tos=3 vlan ->300

this case is not tested pass in WDDS4.1 for not able to set INT_VLAN update and FA together in one filter
I believed it can be set in WDDS4.3
if in WDDS4.3 can not do it either, we shall split the mathing result into 2 filters.
please refer to Case36, which is an example of spliting.
*/

int CLI_FHT_Case36_SmacDmacSipDipTos2SVlan (char *StrPrm)
{
   WP_U32 temp, portid;
   
   WP_pce_filter_classification filter_class = { 0 };
   WP_handle filter;
   WP_handle filter_set;
   WP_handle txfa;
   WP_pce_filter_set fs_level;
   WP_pce_rule_classification rule_cfg = { 0 };
   WP_handle port_handle = 0, agg_handle = 0, h_pce_rule = 0;

   WP_pce_if_params_pkt_rx_channel pce_if_params = { 0 };
   WP_handle pce_if_handle;
   WP_status status;
   WP_U32 ii;
   WP_handle iw_sys = dl_general_iwsys_bridge;

#ifdef HOST_SEND
  temp = 0;
#else
  temp = CLI_GetNumber ("Input port number ( 0 - 1)", 0, 1);
#endif
  portid = temp;

   CLI_FHT_ClearTestingWddiObj();

   WP_U32 i = 0;

   WPE_vlan_edit_options veoption = {
      /* egress_rule; */{
	    WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
	  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
	  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
	  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
	  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
	  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
      },
      /* replace_int_vlan_mode;*/ WPE_GPE_BROUTER_PECS_REPLACE_VLAN_ID,
      /* int_vlan_tag;*/ NEW_VLAN,
      /*  int_vlan_etype_source;*/ WPE_GPE_BROUTER_PECS_VLAN_ETYPE_8100,
      /*  replace_ext_vlan_mode;*/WPE_GPE_BROUTER_PECS_REPLACE_VLAN_PRIORITY,
      /* ext_vlan_tag;*/0,
      /*  ext_vlan_etype_source;*/WPE_GPE_BROUTER_PECS_VLAN_ETYPE_8100,
      /* vlan_stag_etype;*/0x8100,
      /*  replace_sub_port_vlan_mode;*/WPE_GPE_BROUTER_PECS_SUB_PORT_VLAN_DISABLE,
      /* sub_port_vlan_tag;*/0x0,
   };

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
       &veoption,                       /* vlan_edit_options */
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
   i = 0;
   /* DL GE TX flow agg */

  dl_tx_agg_gbe->txfunc = gbe[1].tx_chan_enet;
  dl_tx_agg_gbe->iw_port = gbe[1].bport_enet;
  dl_tx_agg_gbe->pecs_handle = pecs_handles[0];
  dl_tx_agg_gbe->pecs_flow_info = (void *) &brouter_pecs_flow_info[0];
  txfa = WP_IwFlowAggregationCreate (WP_WINPATH (DEFAULT_WPID),
                                 WP_IW_GENERIC_MODE, &dl_tx_agg_gbe);
  App_TerminateOnError (txfa,
                        "WP_IwFlowAggregationCreate()", __LINE__);
  testwddiobj.txfa = txfa;
  
  /* create unknown unicast classification filter */
  filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
  filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;
  
  filter_class.no_match_result[0].result_type =
    WP_PCE_RESULT_LAST;
   /* MAC_DA, MAC_SA, IPV4_DSA, IPV4_DA */ 
  filter_class.filter_fields[0].field_mode =  WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
  filter_class.filter_fields[0].mask_mode =  WP_PCE_FIELD_MASK_NOT_USED;
  filter_class.filter_fields[0].field_id =  WP_PCE_FIELD_ID_MAC_DA;

  filter_class.filter_fields[1].field_mode =  WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
  filter_class.filter_fields[1].mask_mode =  WP_PCE_FIELD_MASK_NOT_USED;
  filter_class.filter_fields[1].field_id =  WP_PCE_FIELD_ID_MAC_SA;

  filter_class.filter_fields[2].field_mode =  WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
  filter_class.filter_fields[2].mask_mode =  WP_PCE_FIELD_MASK_NOT_USED;
  filter_class.filter_fields[2].field_id =  WP_PCE_FIELD_ID_IPV4_SA;
  
  filter_class.filter_fields[3].field_mode =  WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
  filter_class.filter_fields[3].mask_mode =  WP_PCE_FIELD_MASK_NOT_USED;
  filter_class.filter_fields[3].field_id =  WP_PCE_FIELD_ID_IPV4_DA;
  
  filter_class.filter_fields[4].field_mode =  WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
  filter_class.filter_fields[4].mask_mode =  WP_PCE_FIELD_MASK_NOT_USED;
  filter_class.filter_fields[4].field_id =  WP_PCE_FIELD_ID_IPV4_TOS;
  
  filter_class.filter_fields[5].field_id = WP_PCE_FIELD_ID_LAST;
  
  filter = WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID),
                        WP_PCE_FILTER_CLASSIFICATION,
                        &filter_class);
  App_TerminateOnError (filter,"WP_PceFilterCreate", __LINE__);
  testwddiobj.filter[0] = filter;
  testwddiobj.enable = 1;

  filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
  filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;
  
  filter_class.no_match_result[0].result_type =
    WP_PCE_RESULT_LAST;
  
  filter_class.filter_fields[0].field_mode =  WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
  filter_class.filter_fields[0].mask_mode =  WP_PCE_FIELD_MASK_NOT_USED;
  filter_class.filter_fields[0].field_id =  WP_PCE_FIELD_ID_EXT_VLAN_TAG;
  
  filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
  
  filter = WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID),
                        WP_PCE_FILTER_CLASSIFICATION,
                        &filter_class);
  App_TerminateOnError (filter,"WP_PceFilterCreate", __LINE__);
  testwddiobj.filter[1] = filter;
  testwddiobj.enable = 1;
  
  /* filter set */
  fs_level.filter_set_level = 0;
  fs_level.next_filter_set = WP_UNUSED;
  fs_level.filters[0] = testwddiobj.filter[0];
  fs_level.filters[1] = testwddiobj.filter[1];
  fs_level.filters[2] = WP_UNUSED;
  
  filter_set =
    WP_PceFilterSetCreate (WP_WINPATH (DEFAULT_WPID), &fs_level);
  App_TerminateOnError (filter_set, "WP_PceFilterSetCreate",
                       __LINE__);
  testwddiobj.filter_set = filter_set;
  
  switch (portid)
  {
  case 0:
  case 1:
    port_handle = gbe[portid].bport_enet;
    agg_handle = txfa;
    break;
  default:
    printf ("NO such port : %d\n", portid);
    return 0;
  }
  
  // rule1 for  smac=00..01, dmac=00..02, sip=10.0.0.1,dip=10.0.0.2,tos=1 vlan ->100
  rule_cfg.enabled = WP_ENABLE;
  
  rule_cfg.filter_handle = testwddiobj.filter[0];
  
  rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
  rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
  rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
  rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
  rule_cfg.rule_fields[0].value.mac_addr[3] = 0x00;
  rule_cfg.rule_fields[0].value.mac_addr[4] = 0x00;
  rule_cfg.rule_fields[0].value.mac_addr[5] = 0x01;
  
  rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_SA;
  rule_cfg.rule_fields[1].value.mac_addr[0] = 0x00;
  rule_cfg.rule_fields[1].value.mac_addr[1] = 0x00;
  rule_cfg.rule_fields[1].value.mac_addr[2] = 0x00;
  rule_cfg.rule_fields[1].value.mac_addr[3] = 0x00;
  rule_cfg.rule_fields[1].value.mac_addr[4] = 0x00;
  rule_cfg.rule_fields[1].value.mac_addr[5] = 0x02;

  rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_IPV4_SA;
  rule_cfg.rule_fields[2].value.ipv4_addr = 0x0a000002;
  
  rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_IPV4_DA;
  rule_cfg.rule_fields[3].value.ipv4_addr = 0x0a000001;
  
  rule_cfg.rule_fields[4].field_id = WP_PCE_FIELD_ID_IPV4_TOS;
  rule_cfg.rule_fields[4].value.ipv4_tos = 0x1;
  
  rule_cfg.rule_fields[5].field_id = WP_PCE_FIELD_ID_LAST;

  rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
  
  rule_cfg.match_result[0].result_type = WP_PCE_RESULT_EXT_VLAN_UPDATE/*WP_PCE_RESULT_EXT_VLAN_UPDATE*/;
  rule_cfg.match_result[0].param.ext_vlan.vlan_tag = NEW_VLAN /* 0x100*/;
  rule_cfg.match_result[0].param.ext_vlan.mask = 0xfff;

  rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

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
  testwddiobj.rules[0]= h_pce_rule;

#if 0
  rule_cfg.enabled = WP_ENABLE;
  
  rule_cfg.filter_handle = testwddiobj.filter[1];
  
  rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
  rule_cfg.rule_fields[0].value.vlan_tag = NEW_VLAN /* 0x100*/;

  rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

  rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
  
  rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
  rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_handle;
  rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

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
  testwddiobj.rules[1]= h_pce_rule;
#endif


  // rule2 for  smac=00..03, dmac=00..04, sip=10.0.0.3,dip=10.0.0.4,tos=3  vlan -> 100
  rule_cfg.enabled = WP_ENABLE;
  
  rule_cfg.filter_handle = testwddiobj.filter[0];
  
  rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
  rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
  rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
  rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
  rule_cfg.rule_fields[0].value.mac_addr[3] = 0x00;
  rule_cfg.rule_fields[0].value.mac_addr[4] = 0x00;
  rule_cfg.rule_fields[0].value.mac_addr[5] = 0x03;
  
  rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_SA;
  rule_cfg.rule_fields[1].value.mac_addr[0] = 0x00;
  rule_cfg.rule_fields[1].value.mac_addr[1] = 0x00;
  rule_cfg.rule_fields[1].value.mac_addr[2] = 0x00;
  rule_cfg.rule_fields[1].value.mac_addr[3] = 0x00;
  rule_cfg.rule_fields[1].value.mac_addr[4] = 0x00;
  rule_cfg.rule_fields[1].value.mac_addr[5] = 0x04;

  rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_IPV4_SA;
  rule_cfg.rule_fields[2].value.ipv4_addr = 0x0a000004;
  
  rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_IPV4_DA;
  rule_cfg.rule_fields[3].value.ipv4_addr = 0x0a000003;
  
  rule_cfg.rule_fields[4].field_id = WP_PCE_FIELD_ID_IPV4_TOS;
  rule_cfg.rule_fields[4].value.ipv4_tos = 0x3;

  rule_cfg.rule_fields[5].field_id = WP_PCE_FIELD_ID_LAST;
  
  // results
  rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
  // rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
                                          
  rule_cfg.match_result[0].result_type =  WP_PCE_RESULT_EXT_VLAN_UPDATE;
  rule_cfg.match_result[0].param.ext_vlan/*int_vlan*/.vlan_tag = NEW_VLAN /* 0x100*/;
  rule_cfg.match_result[0].param.ext_vlan/*int_vlan*/.mask = 0xfff;
  
  rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
  
  h_pce_rule = WP_PceRuleCreate (WP_WINPATH (DEFAULT_WPID),
                                WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
  if (WP_ERROR (h_pce_rule) == WP_ERR_PCE_RULE_ALREADY_EXISTS)
  {
    printf ("PCE rule - %d already exist!\n", 1);
  }
  else
  {
/////////////////////////////////////////////////////////////////////
    App_TerminateOnError (h_pce_rule, "WP_PceRuleCreate", __LINE__);
  }
  testwddiobj.rules[3]= h_pce_rule;

#if 1 // N:1 then only one matching fa for filter[1] is ok.
  rule_cfg.enabled = WP_ENABLE;
  
  rule_cfg.filter_handle = testwddiobj.filter[1];
  
  rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
  rule_cfg.rule_fields[0].value./*ext_vlan.*/vlan_tag = /*0x300*/NEW_VLAN /* 0x100*/;

  rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

  rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
  
  rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
  rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_handle;
  rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

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
  testwddiobj.rules[4]= h_pce_rule;
#endif

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
  pce_if_params.filter_set_handle = filter_set;
  pce_if_params.ip_header_validation = WP_DISABLE;
  //pce_if_params.user_programmable_fields_handle = 0;//fht
  
  pce_if_handle =
    WP_PceInterfaceCreate (WP_PCE_IF_TYPE_PKT_RX_CHANNEL,
                           &pce_if_params);
  App_TerminateOnError (pce_if_handle, "WP_PceInterfaceCreate()",
                       __LINE__);
  
  /* Modify the PCE interface for GE RX channel handle */
  for (ii = 0; ii < 1 /*NR_GBE*/; ii++)  // leave enet2 to host for checking
  {
    rx_binding_cfg[0].pce_if_handle = pce_if_handle;
    rx_binding_cfg[0].input_port = gbe[ii].bport_enet;
    
    status = WP_IwRxBindingModify (gbe[ii].rx_chan_enet,
                                   iw_sys,
                                   qniw,
                                   WP_IW_RX_BIND_MOD_PCE_INTERFACE,
                                   &rx_binding_cfg[0]);
    App_TerminateOnError (status, "WP_IwRxBindingModify", __LINE__);
  }

  WP_PceFilterStatisticsReset(filter, &(pce_filter_stats[filter & 0xFF]));

#ifdef HOST_SEND
  // send a tx packet
  CLI_HostSendPacketCase36();
  
  printf("Waiting for a while...\n");
  for(i=0;i<5000000;i++);
  
  WPE_PrintPceFilterStats(filter);
#endif

   return 0;
}

void CLI_HostSendPacketCase36(void)
{
   WP_U32 i, size=100;
   WP_U8 *data_ptr;
   WP_handle pool = qniw_adjpool;

   data_ptr = WP_PoolAlloc(pool);
   if(data_ptr == 0)
   {
     printf("no free buffer in pool.\n");
     return ;
   }
   printf("assembiling packet at 0x%08x\n", &data_ptr[0]);

   // uc packet,  dmac=00...01 smac=00..02
   // uc packet,  dmac=00...01 smac=00..02
   assemble_packet(&data_ptr[0], 4, 0x00000000);
   assemble_packet(&data_ptr[4], 4, 0x00010000);
   assemble_packet(&data_ptr[8], 4, 0x00000002);
   assemble_packet(&data_ptr[12], 4, 0x81000100);/* s-vlan = 100*/
   assemble_packet(&data_ptr[16], 4, 0x81000200);/* c-vlan = 200*/
   assemble_packet(&data_ptr[20], 2, 0x0800);
   assemble_packet(&data_ptr[24], 4, 0x45010020);/* tos = 1 */
   assemble_packet(&data_ptr[28], 4, 0x001b0000);
   assemble_packet(&data_ptr[32], 4, 0x0a11e449);/* protocol = 0x11*/
   assemble_packet(&data_ptr[36], 4, 0x0a000001);/* sa ip */
   assemble_packet(&data_ptr[40], 4, 0x0a000002);/* da ip */
   assemble_packet(&data_ptr[44], 4, 0x00190015);
   assemble_packet(&data_ptr[48], 4, 0x001b77c0);
   assemble_packet(&data_ptr[52], 4, 0x00a4c5ff);

   printf("assembiling packet payload\n");
   for(i = 50; i < size; i++)
   {
     data_ptr[i] = (i - 50);
   }

   printf("send packet 1\n");
   CLI_HostSendPacket(data_ptr,  size);
   printf("Waiting for a while...\n");
   for(i=0;i<5000000;i++);

   data_ptr = WP_PoolAlloc(pool);
   if(data_ptr == 0)
   {
     printf("no free buffer in pool.\n");
     return ;
   }
   printf("assembiling packet at 0x%08x\n", &data_ptr[0]);

   // uc packet,  dmac=00...03 smac=00..04
   assemble_packet(&data_ptr[0], 4, 0x00000000);
   assemble_packet(&data_ptr[4], 4, 0x00010000);
   assemble_packet(&data_ptr[8], 4, 0x00000002);
   assemble_packet(&data_ptr[12], 4, 0x81000300);/* vlan = 300*/
   assemble_packet(&data_ptr[16], 4, 0x81000400);/* vlan = 400*/
   assemble_packet(&data_ptr[20], 2, 0x0800);
   assemble_packet(&data_ptr[24], 4, 0x45010020);/* tos = 1 */
   assemble_packet(&data_ptr[28], 4, 0x001b0000);
   assemble_packet(&data_ptr[32], 4, 0x0a11e449);/* protocol = 0x11*/
   assemble_packet(&data_ptr[36], 4, 0x0a000003);/* sa ip */
   assemble_packet(&data_ptr[40], 4, 0x0a000004);/* da ip */
   assemble_packet(&data_ptr[44], 4, 0x00190015);
   assemble_packet(&data_ptr[48], 4, 0x001b77c0);
   assemble_packet(&data_ptr[52], 4, 0x00a4c5ff);


   printf("assembiling packet payload\n");
   for(i = 50; i < size; i++)
   {
     data_ptr[i] = (i - 50);
   }

   printf("send packet 2\n");
   CLI_HostSendPacket(data_ptr,  size);
   
}

#if 0   // the old case 36 translate stag,  but requirement said translate same as case 35
/*
case36
in this case,  match ext vlan range from 0x100 - 0x200, change ext vlan = 1
for no matching packet, drop --- test OK
*/

int CLI_FHT_Case36_Vlan2Vlan (char *StrPrm)
{
   WP_U32 temp, portid;
   
   WP_pce_filter_classification filter_class = { 0 };
   WP_handle filter;
   WP_handle filter_set;
   WP_handle txfa;
   WP_pce_filter_set fs_level;
   WP_pce_rule_classification rule_cfg = { 0 };
   WP_handle port_handle = 0, agg_handle = 0, h_pce_rule = 0;

   WP_pce_if_params_pkt_rx_channel pce_if_params = { 0 };
   WP_handle pce_if_handle;
   WP_status status;
   WP_U32 ii;
   WP_handle iw_sys = dl_general_iwsys_bridge;

#ifdef HOST_SEND
  temp = 0;
#else
  temp = CLI_GetNumber ("Input port number ( 0 - 1)", 0, 1);
#endif
  portid = temp;

   CLI_FHT_ClearTestingWddiObj();

   WP_U32 i = 0;

   WPE_vlan_edit_options veoption = {
      /* egress_rule; */{
	    WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
	  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
	  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
	  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
	  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED,
	  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED,
      },
      /* replace_int_vlan_mode;*/ WPE_GPE_BROUTER_PECS_REPLACE_VLAN_DISABLE, // WPE_GPE_BROUTER_PECS_REPLACE_VLAN_ID
      /* int_vlan_tag;*/ 0x10,
      /*  int_vlan_etype_source;*/ WPE_GPE_BROUTER_PECS_VLAN_ETYPE_8100,
      /*  replace_ext_vlan_mode;*/ WPE_GPE_BROUTER_PECS_REPLACE_VLAN_ID, // WPE_GPE_BROUTER_PECS_REPLACE_VLAN_DISABLE,
      /* ext_vlan_tag;*/0x30,
      /*  ext_vlan_etype_source;*/ WPE_GPE_BROUTER_PECS_VLAN_ETYPE_8100,
      /* vlan_stag_etype;*/0x9100,
      /*  replace_sub_port_vlan_mode;*/ WPE_GPE_BROUTER_PECS_SUB_PORT_VLAN_DISABLE,
      /* sub_port_vlan_tag;*/0x0,
   };

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
       &veoption,                       /* vlan_edit_options */
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
   i = 0;
   /* DL GE TX flow agg */

  dl_tx_agg_gbe->txfunc = gbe[1].tx_chan_enet;
  dl_tx_agg_gbe->iw_port = gbe[1].bport_enet;
  dl_tx_agg_gbe->pecs_handle = pecs_handles[0];
  dl_tx_agg_gbe->pecs_flow_info = (void *) &brouter_pecs_flow_info[0];
  txfa = WP_IwFlowAggregationCreate (WP_WINPATH (DEFAULT_WPID),
                                 WP_IW_GENERIC_MODE, &dl_tx_agg_gbe);
  App_TerminateOnError (txfa,
                        "WP_IwFlowAggregationCreate()", __LINE__);
  testwddiobj.txfa = txfa;
  
  /* create unknown unicast classification filter */
  filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
  filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;
  
  filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;
  
  filter_class.filter_fields[0].field_mode =  WP_PCE_FIELD_MODE_COMPARE_RANGE_LOW;
  filter_class.filter_fields[0].mask_mode =  WP_PCE_FIELD_MASK_NOT_USED;
  filter_class.filter_fields[0].field_id =  WP_PCE_FIELD_ID_EXT_VLAN_TAG;

  filter_class.filter_fields[1].field_mode =  WP_PCE_FIELD_MODE_COMPARE_RANGE_HIGH;
  filter_class.filter_fields[1].mask_mode =  WP_PCE_FIELD_MASK_NOT_USED;
  filter_class.filter_fields[1].field_id =  WP_PCE_FIELD_ID_EXT_VLAN_TAG;
  
  filter_class.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
  
  filter = WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID),
                        WP_PCE_FILTER_CLASSIFICATION,
                        &filter_class);
  App_TerminateOnError (filter,"WP_PceFilterCreate", __LINE__);
  testwddiobj.filter[0] = filter;
  testwddiobj.enable = 1;

#if 0   // can not add this kind of filter of no match of a flowagg
  filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_ACCEPT;
  filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;
  
  filter_class.no_match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
  filter_class.no_match_result[0].param.flow_agg.flow_aggregation = agg_handle;
  filter_class.no_match_result[1].result_type = WP_PCE_RESULT_LAST;
  
  filter_class.filter_fields[0].field_id = WP_PCE_FIELD_ID_LAST;

  filter = WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID),
                        WP_PCE_FILTER_CLASSIFICATION,
                        &filter_class);
  App_TerminateOnError (filter,"WP_PceFilterCreate", __LINE__);
  testwddiobj.filter[1] = filter;
#endif

// because of WDDS4.1 issue, have to seperate 2 filter to have 2 differrent results
  filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
  filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;
  
  filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;
  
  filter_class.filter_fields[0].field_mode =  WP_PCE_FIELD_MODE_COMPARE_RANGE_LOW;
  filter_class.filter_fields[0].mask_mode =  WP_PCE_FIELD_MASK_NOT_USED;
  filter_class.filter_fields[0].field_id =  WP_PCE_FIELD_ID_EXT_VLAN_TAG;

  filter_class.filter_fields[1].field_mode =  WP_PCE_FIELD_MODE_COMPARE_RANGE_HIGH;
  filter_class.filter_fields[1].mask_mode =  WP_PCE_FIELD_MASK_NOT_USED;
  filter_class.filter_fields[1].field_id =  WP_PCE_FIELD_ID_EXT_VLAN_TAG;
  
  filter_class.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
  
  filter = WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID),
                        WP_PCE_FILTER_CLASSIFICATION,
                        &filter_class);
  App_TerminateOnError (filter,"WP_PceFilterCreate", __LINE__);
  testwddiobj.filter[1] = filter;
  testwddiobj.enable = 1;

  
  /* filter set */
  fs_level.filter_set_level = 0;
  fs_level.next_filter_set = WP_UNUSED;
  fs_level.filters[0] = testwddiobj.filter[0];
  fs_level.filters[1] = testwddiobj.filter[1];
  fs_level.filters[2] = WP_UNUSED;
  
  filter_set =
    WP_PceFilterSetCreate (WP_WINPATH (DEFAULT_WPID), &fs_level);
  App_TerminateOnError (filter_set, "WP_PceFilterSetCreate",
                       __LINE__);
  testwddiobj.filter_set = filter_set;
  
  switch (portid)
  {
  case 0:
  case 1:
    port_handle = gbe[portid].bport_enet;
    agg_handle = txfa;
    break;
  default:
    printf ("NO such port : %d\n", portid);
    return 0;
  }
  
  // rule1
  rule_cfg.enabled = WP_ENABLE;
  
  rule_cfg.filter_handle = testwddiobj.filter[0];
  
  rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
  rule_cfg.rule_fields[0].value.vlan_tag = 0x100;
  rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
  rule_cfg.rule_fields[1].value.vlan_tag = 0x200;
  // range 100 - 200,  so 300 is denied.
  
  rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
  
  rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
#if 0
  rule_cfg.match_result[0].result_type = WP_PCE_RESULT_INT_VLAN_UPDATE;
  rule_cfg.match_result[0].param.int_vlan.vlan_tag = 0x1;
  rule_cfg.match_result[0].param.int_vlan.mask = 0xfff;
#else
  rule_cfg.match_result[0].result_type = WP_PCE_RESULT_EXT_VLAN_UPDATE;
  rule_cfg.match_result[0].param.ext_vlan.vlan_tag = 0x1;   //================
  rule_cfg.match_result[0].param.ext_vlan.mask = 0xfff;
  // please also change vlan_edit_option ext_replace_mode=vlanid
  // and after filter1, ext vlan is updated, filter2 need to match new vlan.
#endif
  
  rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
  
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
  testwddiobj.rules[0]= h_pce_rule;
  
  // rule2
  rule_cfg.enabled = WP_ENABLE;
  
  rule_cfg.filter_handle = testwddiobj.filter[1];
  
  rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
  rule_cfg.rule_fields[0].value.vlan_tag = 0x1;
  rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
  rule_cfg.rule_fields[1].value.vlan_tag = 0x2;
  // and after filter1, ext vlan is updated, filter2 need to match new vlan.
  
  rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
  
  rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
  rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
  rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_handle;
  
  rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
  
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
  testwddiobj.rules[1]= h_pce_rule;
  
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
  pce_if_params.filter_set_handle = filter_set;
  pce_if_params.ip_header_validation = WP_DISABLE;
  //pce_if_params.user_programmable_fields_handle = 0;//fht
  
  pce_if_handle =
    WP_PceInterfaceCreate (WP_PCE_IF_TYPE_PKT_RX_CHANNEL,
                           &pce_if_params);
  App_TerminateOnError (pce_if_handle, "WP_PceInterfaceCreate()",
                       __LINE__);

   /* Modify the PCE interface for GE RX channel handle */
  for (ii = 0; ii < 1 /*NR_GBE*/; ii++)  // leave enet2 to host for checking
  {
    rx_binding_cfg[0].pce_if_handle = pce_if_handle;
    rx_binding_cfg[0].input_port = gbe[ii].bport_enet;
    
    status = WP_IwRxBindingModify (gbe[ii].rx_chan_enet,
                                   iw_sys,
                                   qniw,
                                   WP_IW_RX_BIND_MOD_PCE_INTERFACE,
                                   &rx_binding_cfg[0]);
    App_TerminateOnError (status, "WP_IwRxBindingModify", __LINE__);
  }

  filter = testwddiobj.filter[0];
  WP_PceFilterStatisticsReset(filter, &(pce_filter_stats[ filter & 0xFF]));
  filter = testwddiobj.filter[1];
  WP_PceFilterStatisticsReset(filter, &(pce_filter_stats[ filter & 0xFF]));

#ifdef HOST_SEND
  // send a tx packet
  CLI_HostSendPacketCase36();
  
  printf("Waiting for a while...\n");
  for(i=0;i<5000000;i++);
  
  WPE_PrintPceFilterStats(testwddiobj.filter[0]);
  WPE_PrintPceFilterStats(testwddiobj.filter[1]);
#endif

   return 0;
}


void CLI_HostSendPacketCase36(void)
{
   WP_U32 i, size=100;
   WP_U8 *data_ptr;
   WP_handle pool = qniw_adjpool;

   data_ptr = WP_PoolAlloc(pool);
   if(data_ptr == 0)
   {
     printf("no free buffer in pool.\n");
     return ;
   }
   printf("assembiling packet at 0x%08x\n", &data_ptr[0]);

   // uc packet,  dmac=00...01 smac=00..02
   assemble_packet(&data_ptr[0], 4, 0x00000000);
   assemble_packet(&data_ptr[4], 4, 0x00010000);
   assemble_packet(&data_ptr[8], 4, 0x00000002);
   assemble_packet(&data_ptr[12], 4, 0x81000100);/* vlan = 100*/
   assemble_packet(&data_ptr[16], 4, 0x81000100);/* vlan = 100*/
   assemble_packet(&data_ptr[20], 2, 0x0800);
   assemble_packet(&data_ptr[24], 4, 0x45010020);/* tos = 1 */
   assemble_packet(&data_ptr[28], 4, 0x001b0000);
   assemble_packet(&data_ptr[32], 4, 0x0a11e449);/* protocol = 0x11*/
   assemble_packet(&data_ptr[36], 4, 0x0a000001);/* sa ip */
   assemble_packet(&data_ptr[40], 4, 0x0a000002);/* da ip */
   assemble_packet(&data_ptr[44], 4, 0x00190015);
   assemble_packet(&data_ptr[48], 4, 0x001b77c0);
   assemble_packet(&data_ptr[52], 4, 0x00a4c5ff);

   printf("assembiling packet payload\n");
   for(i = 50; i < size; i++)
   {
     data_ptr[i] = (i - 50);
   }

   printf("send packet 1\n");
   CLI_HostSendPacket(data_ptr,  size);
   printf("Waiting for a while...\n");
   for(i=0;i<5000000;i++);

   data_ptr = WP_PoolAlloc(pool);
   if(data_ptr == 0)
   {
     printf("no free buffer in pool.\n");
     return ;
   }
   printf("assembiling packet at 0x%08x\n", &data_ptr[0]);

   // uc packet,  dmac=00...03 smac=00..04
   assemble_packet(&data_ptr[0], 4, 0x00000000);
   assemble_packet(&data_ptr[4], 4, 0x00030000);
   assemble_packet(&data_ptr[8], 4, 0x00000004);
   assemble_packet(&data_ptr[12], 4, 0x81000300);/* vlan = 300*/
   assemble_packet(&data_ptr[16], 2, 0x0800);
   assemble_packet(&data_ptr[18], 4, 0x45030020);/* tos = 3 */
   assemble_packet(&data_ptr[22], 4, 0x001b0000);
   assemble_packet(&data_ptr[26], 4, 0x0a11e449);/* protocol = 0x11*/
   assemble_packet(&data_ptr[30], 4, 0x0a000003);/* sa ip */
   assemble_packet(&data_ptr[34], 4, 0x0a000004);/* da ip */
   assemble_packet(&data_ptr[38], 4, 0x00190015);
   assemble_packet(&data_ptr[42], 4, 0x001b77c0);
   assemble_packet(&data_ptr[46], 4, 0x00a4c5ff);


   printf("assembiling packet payload\n");
   for(i = 50; i < size; i++)
   {
     data_ptr[i] = (i - 50);
   }

   printf("send packet 2\n");
   CLI_HostSendPacket(data_ptr,  size);
   
}
#endif

/*
case37
in this case, add a s-tag vlan, not change anything.
use int vlan range 1 - 0x200
the matching one: vlan = 1, add stag 0x81000100, by pecs fa
the matching one: vlan = 0x300, pass through, by bport fa
*/

int CLI_FHT_Case37_Tag_Stack_Tunneling (char *StrPrm)
{
  WP_U32 temp, portid;
  
  WP_pce_filter_classification filter_class = { 0 };
  WP_handle filter;
  WP_handle filter_set;
  WP_handle txfa;
  WP_pce_filter_set fs_level;
  WP_pce_rule_classification rule_cfg = { 0 };
  WP_handle port_handle = 0, agg_handle = 0, h_pce_rule = 0;
  
  WP_pce_if_params_pkt_rx_channel pce_if_params = { 0 };
  WP_handle pce_if_handle;
  WP_status status;
  WP_U32 ii;
  WP_handle iw_sys = dl_general_iwsys_bridge;
  
#ifdef HOST_SEND
  temp = 0;
#else
  temp = CLI_GetNumber ("Input port number ( 0 - 1)", 0, 1);
#endif
  portid = temp;
  
  CLI_FHT_ClearTestingWddiObj();
  
  WP_U32 i = 0;
  
  WPE_vlan_edit_options veoption = {
    /* egress_rule; */{
    WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED,
  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED,
  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED,
  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED,
  	WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED,
    },
    /* replace_int_vlan_mode;*/ WPE_GPE_BROUTER_PECS_REPLACE_VLAN_DISABLE,
    /* int_vlan_tag;*/ 0,
    /*  int_vlan_etype_source;*/ WPE_GPE_BROUTER_PECS_VLAN_ETYPE_8100,
    /*  replace_ext_vlan_mode;*/ WPE_GPE_BROUTER_PECS_REPLACE_VLAN_ID,
    /* ext_vlan_tag;*/ 0x100,
    /*  ext_vlan_etype_source;*/ WPE_GPE_BROUTER_PECS_VLAN_ETYPE_8100,
    /* vlan_stag_etype;*/0x8100,
    /*  replace_sub_port_vlan_mode;*/ WPE_GPE_BROUTER_PECS_SUB_PORT_VLAN_DISABLE,
    /* sub_port_vlan_tag;*/0x0,
  };

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
#if 0
     &veoption,                       /* vlan_edit_options */
#else
      0,
#endif
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
   veoption.int_vlan_tag = 0;  
   i = 0;    
  // keatin here use modify maybe better. or delete it first.
  
  /* DL GE TX flow agg */
  
  dl_tx_agg_gbe->txfunc = gbe[1].tx_chan_enet;
  dl_tx_agg_gbe->iw_port = gbe[1].bport_enet;
  dl_tx_agg_gbe->pecs_handle = pecs_handles[0];
  dl_tx_agg_gbe->pecs_flow_info = (void *) &brouter_pecs_flow_info[0];
  txfa =
   WP_IwFlowAggregationCreate (WP_WINPATH (DEFAULT_WPID),
                               WP_IW_GENERIC_MODE, &dl_tx_agg_gbe);
  App_TerminateOnError (txfa,
                      "WP_IwFlowAggregationCreate()", __LINE__);
  testwddiobj.txfa = txfa;
  
  /* create unknown unicast classification filter */
  filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
  filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

  filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;
  
  filter_class.filter_fields[0].field_mode =  WP_PCE_FIELD_MODE_COMPARE_RANGE_LOW;
  filter_class.filter_fields[0].mask_mode =  WP_PCE_FIELD_MASK_NOT_USED;
  filter_class.filter_fields[0].field_id =  WP_PCE_FIELD_ID_INT_VLAN_TAG;

  filter_class.filter_fields[1].field_mode =  WP_PCE_FIELD_MODE_COMPARE_RANGE_HIGH;
  filter_class.filter_fields[1].mask_mode =  WP_PCE_FIELD_MASK_NOT_USED;
  filter_class.filter_fields[1].field_id =  WP_PCE_FIELD_ID_INT_VLAN_TAG;
  
  filter_class.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
#ifdef MORRIS_MOD
  filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
  filter_class.filter_fields[0].field_id =  WP_PCE_FIELD_ID_PARSER_FLAGS;
  filter_class.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
  filter_class.filter_fields[0].mask_mode =  WP_PCE_FIELD_MASK_USED;
  filter_class.filter_fields[0].mask.parser_flags  = WP_PCE_MASK_PARSER_FLAG_BC; // here


  filter_class.filter_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
  filter_class.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
  filter_class.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
  filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
#endif

  filter = WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID),
                        WP_PCE_FILTER_CLASSIFICATION,
                        &filter_class);
  App_TerminateOnError (filter,"WP_PceFilterCreate", __LINE__);
  testwddiobj.filter[0] = filter;
  testwddiobj.enable = 1;
  
  /* filter set */
  fs_level.filter_set_level = 0;
  fs_level.next_filter_set = WP_UNUSED;
  fs_level.filters[0] = filter;
  fs_level.filters[1] = WP_UNUSED;
  
  filter_set =
    WP_PceFilterSetCreate (WP_WINPATH (DEFAULT_WPID), &fs_level);
  App_TerminateOnError (filter_set, "WP_PceFilterSetCreate",
                       __LINE__);
  testwddiobj.filter_set = filter_set;
  
  switch (portid)
  {
  case 0:
  case 1:
    port_handle = gbe[portid].bport_enet;
    agg_handle = txfa;
    break;
  default:
    printf ("NO such port : %d\n", portid);
    return 0;
  }
  
  // rule1 for uc
  rule_cfg.enabled = WP_ENABLE;
  
  rule_cfg.filter_handle = filter;
  
  rule_cfg.filter_handle = testwddiobj.filter[0];
  
  rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
  rule_cfg.rule_fields[0].value.vlan_tag = 0x1;
  rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
  rule_cfg.rule_fields[1].value.vlan_tag = 0x200;
  // range 1 - 200,  so 300 is denied.
  
#ifdef MORRIS_MOD
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_PARSER_FLAGS;
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
   rule_cfg.rule_fields[0].value.parser_flags = 0;
   rule_cfg.rule_fields[0].value.iw_port_handle = gbe[0].bport_enet;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
#endif
  
  rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
  
  rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
  rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
  rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_handle;
  
  rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
  
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
  testwddiobj.rules[0]= h_pce_rule;
    
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
  pce_if_params.filter_set_handle = filter_set;
  pce_if_params.ip_header_validation = WP_DISABLE;
  //pce_if_params.user_programmable_fields_handle = 0;//fht
  
  pce_if_handle =
    WP_PceInterfaceCreate (WP_PCE_IF_TYPE_PKT_RX_CHANNEL,
                           &pce_if_params);
  App_TerminateOnError (pce_if_handle, "WP_PceInterfaceCreate()",
                       __LINE__);

   /* Modify the PCE interface for GE RX channel handle */
  for (ii = 0; ii < 1 /*NR_GBE*/; ii++)  // leave enet2 to host for checking
  {
#if 1
    rx_binding_cfg[0].pce_if_handle = pce_if_handle /*0*/;
#else
    rx_binding_cfg[0].pce_if_handle = 0;
#endif
    rx_binding_cfg[0].input_port = gbe[ii].bport_enet;
    
    status = WP_IwRxBindingModify (gbe[ii].rx_chan_enet,
                                   iw_sys,
                                   qniw,
                                   WP_IW_RX_BIND_MOD_PCE_INTERFACE,
                                   &rx_binding_cfg[0]);
    App_TerminateOnError (status, "WP_IwRxBindingModify", __LINE__);
  }

  filter = testwddiobj.filter[0];
  WP_PceFilterStatisticsReset(filter, &(pce_filter_stats[ filter & 0xFF]));

#ifdef HOST_SEND
  // send a tx packet
  CLI_HostSendPacketCase37();
  
  printf("Waiting for a while...\n");
  for(i=0;i<5000000;i++);
  
  WPE_PrintPceFilterStats(testwddiobj.filter[0]);
#endif
   return 0;
}


void CLI_HostSendPacketCase37(void)
{
   WP_U32 i, size=100;
   WP_U8 *data_ptr;
   WP_handle pool = qniw_adjpool;

   data_ptr = WP_PoolAlloc(pool);
   if(data_ptr == 0)
   {
     printf("no free buffer in pool.\n");
     return ;
   }
   printf("assembiling packet at 0x%08x\n", &data_ptr[0]);

   // uc packet,  dmac=00...01 smac=00..02
   assemble_packet(&data_ptr[0], 4, 0x00000000);
   assemble_packet(&data_ptr[4], 4, 0x00010000);
   assemble_packet(&data_ptr[8], 4, 0x00000002);
   assemble_packet(&data_ptr[12], 4, 0x81000001); /* vlan = 1*/
   assemble_packet(&data_ptr[16], 2, 0x0800);
   assemble_packet(&data_ptr[18], 4, 0x45040020);/* tos = 4 */
   assemble_packet(&data_ptr[22], 4, 0x001b0000);
   assemble_packet(&data_ptr[26], 4, 0x0a11e449);/* protocol = 0x11*/
   assemble_packet(&data_ptr[30], 4, 0x00010c20);/* sa ip */
   assemble_packet(&data_ptr[34], 4, 0xc0440000);/* da ip */
   assemble_packet(&data_ptr[38], 4, 0x00190015);
   assemble_packet(&data_ptr[42], 4, 0x001b77c0);
   assemble_packet(&data_ptr[46], 4, 0x00a4c5ff);

   printf("assembiling packet payload\n");
   for(i = 50; i < size; i++)
   {
     data_ptr[i] = (i - 50);
   }

   printf("send packet 1\n");
   CLI_HostSendPacket(data_ptr,  size);
   printf("Waiting for a while...\n");
   for(i=0;i<5000000;i++);

   data_ptr = WP_PoolAlloc(pool);
   if(data_ptr == 0)
   {
     printf("no free buffer in pool.\n");
     return ;
   }
   printf("assembiling packet at 0x%08x\n", &data_ptr[0]);

   // uc packet,  dmac=00...01 smac=00..02
   assemble_packet(&data_ptr[0], 4, 0xffffffff);
   assemble_packet(&data_ptr[4], 4, 0xffff0000);
   assemble_packet(&data_ptr[8], 4, 0x00000002);
   assemble_packet(&data_ptr[12], 4, 0x81000300);/* vlan = 0x300 */
   assemble_packet(&data_ptr[16], 2, 0x0800);
   assemble_packet(&data_ptr[18], 4, 0x45040020);/* tos = 4 */
   assemble_packet(&data_ptr[22], 4, 0x001b0000);
   assemble_packet(&data_ptr[26], 4, 0x0a11e449);/* protocol = 0x11*/
   assemble_packet(&data_ptr[30], 4, 0x00010c20);/* sa ip */
   assemble_packet(&data_ptr[34], 4, 0xc0440000);/* da ip */
   assemble_packet(&data_ptr[38], 4, 0x00190015);
   assemble_packet(&data_ptr[42], 4, 0x001b77c0);
   assemble_packet(&data_ptr[46], 4, 0x00a4c5ff);

   printf("assembiling packet payload\n");
   for(i = 50; i < size; i++)
   {
     data_ptr[i] = (i - 50);
   }

   printf("send packet 2\n");
   CLI_HostSendPacket(data_ptr,  size);
   
}

/********************  END OF ADDED BY KEATIN 20120810 ***********************/


