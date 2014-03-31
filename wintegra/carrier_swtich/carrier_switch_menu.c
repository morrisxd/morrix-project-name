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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif


#include "external.h"

#include "carrier_switch_menu.h"
#include "carrier_switch_util.h"

#ifdef WP_HW_WINPATH3
#include "winutil/api/include/winpath3/wpu_ta.h"
#endif 

#ifdef WP_HW_WINPATH4
#include "winutil/api/include/winpath4/wpu_ta.h"
#endif 

char *STR_MENU_HDR = "                  Carrier Switch Demo                     ";
char appname[] = "Carrier Switch Demo";
static char val_str[256];

extern Y_MenuEntry V_MainMenu[];

static Y_MenuEntry *V_CurrMenuP = &V_MainMenu[0];


Y_MenuEntry V_MainMenu[] = 
{
        {K_Menu, MAIN_MENU_ITEM_NUM, TRUE, "Main Menu",         {(Y_MenuEntry *)V_MainMenu}},
        {K_Menu, 1, TRUE, " -> Port Menu",                      {(Y_MenuEntry *)Port_menu}},
        {K_Menu, 2, TRUE, " -> VPN Menu",                       {(Y_MenuEntry *)VPN_menu}},
        {K_Menu, 3, TRUE, " -> SubPort Menu",                   {(Y_MenuEntry *)SubPort_menu}},
        {K_Menu, 4, TRUE, " -> VPN Port Menu",                  {(Y_MenuEntry *)VPN_Port_menu}},
        {K_Menu, 5, TRUE, " -> MAC Menu",                       {(Y_MenuEntry *)L2VPN_Flow_menu}},
        {K_Menu, 6, TRUE, " -> IP Route Menu",                  {(Y_MenuEntry *)L3VPN_Flow_menu}},
        {K_Menu, 7, TRUE, " -> Policer Menu",                   {(Y_MenuEntry *)Policer_menu}},
        {K_Menu, 8, TRUE, " -> GPMT Menu",                      {(Y_MenuEntry *)GPMT_menu}},
        {K_Menu, 9, TRUE, " -> Statistics",                     {(Y_MenuEntry *)STAT_menu}},        
        {K_Leaf, 10,TRUE, " -> WinUtil",                        {(void*)(int)CLI_WinUtil}},
        {K_Leaf, 11,TRUE, " -> Display",                        {(void*)(int)CLI_Display}},
        {K_Leaf, 12,TRUE, " <> Quit",                           {(void*)(int)CLI_MAIN_Quit}},
};

Y_MenuEntry Port_menu[] = 
{
        {K_Menu, PORT_MENU_ITEM_NUM, TRUE, "Port Menu",         {(Y_MenuEntry *)V_MainMenu}},
        {K_Leaf, 1, TRUE, " <> Show port info",                 {(void*)(int)CLI_PORT_Show_Port_Info}},
        {K_Leaf, 2, TRUE, " -> Set port mode",                  {(void*)(int)CLI_PORT_Set_Port_Mode}},
        {K_Leaf, 3, TRUE, " -> Set AF mode",                    {(void*)(int)CLI_PORT_Set_AF_Mode}},
};


Y_MenuEntry VPN_menu[] = 
{
        {K_Menu, VPN_MENU_ITEM_NUM, TRUE, "VPN Menu",           {(Y_MenuEntry *)V_MainMenu}},
        {K_Leaf, 1, TRUE, " <> Show VPN",                       {(void*)(int)CLI_VPN_Show_VPN}},
        {K_Leaf, 2, TRUE, " <> Create VSI(L2 VPN)",             {(void*)(int)CLI_VPN_Create_L2VPN}},
        {K_Leaf, 3, TRUE, " <> Create VRF(L3 VPN)",             {(void*)(int)CLI_VPN_Create_L3VPN}},
        {K_Leaf, 4, TRUE, " <> Create VPWS",                    {(void*)(int)CLI_VPN_Create_VPWS}},
        {K_Leaf, 5, TRUE, " -> Delete VPN",                     {(void*)(int)CLI_VPN_Delete_VPN}},
};


Y_MenuEntry SubPort_menu[] = 
{
        {K_Menu, SUBPORT_MENU_ITEM_NUM, TRUE, "SubPort Menu",           {(Y_MenuEntry *)V_MainMenu}},
        {K_Leaf, 1, TRUE, " <> Show SubPort Info",                      {(void*)(int)CLI_SUBPORT_Show_SubPort_Info}},
        {K_Leaf, 2, TRUE, " -> Create SubPort",                         {(void*)(int)CLI_SUBPORT_Create_SubPort}},
        {K_Leaf, 3, TRUE, " -> Delete SubPort",                         {(void*)(int)CLI_SUBPORT_Delete_SubPort}},
        {K_Leaf, 4, TRUE, " -> Set AF mode",                            {(void*)(int)CLI_SUBPORT_Set_AF_Mode}},        
};


Y_MenuEntry VPN_Port_menu[] = 
{
        {K_Menu, VPNPORT_MENU_ITEM_NUM, TRUE, "VPN Port Menu",          {(Y_MenuEntry *)V_MainMenu}},
        {K_Leaf, 1, TRUE, " <> Show VPN port info",                     {(void*)(int)CLI_VPNPORT_Show_VPN_port_info}},
        {K_Leaf, 2, TRUE, " -> Create UNI VPN port",                    {(void*)(int)CLI_VPNPORT_Create_UNI_VPN_port}},
        {K_Leaf, 3, TRUE, " -> Create NNI VPN port",                    {(void*)(int)CLI_VPNPORT_Create_NNI_VPN_port}},
        {K_Leaf, 4, TRUE, " -> Delete VPN port",                        {(void*)(int)CLI_VPNPORT_Delete_VPN_port}},

};


Y_MenuEntry L2VPN_Flow_menu[] = 
{
        {K_Menu, L2VPN_FLOW_MENU_ITEM_NUM, TRUE, "Static MAC Menu",     {(Y_MenuEntry *)V_MainMenu}},
        {K_Leaf, 1, TRUE, " <> Show MAC table",                         {(void*)(int)CLI_L2VPN_Show_Static_MAC_info}},
        {K_Leaf, 2, TRUE, " -> Insert static MAC",                      {(void*)(int)CLI_L2VPN_Insert_Static_MAC}},
        {K_Leaf, 3, TRUE, " -> Delete static MAC",                      {(void*)(int)CLI_L2VPN_Delete_Static_MAC}},
        {K_Leaf, 4, TRUE, " -> Delete learnt MAC",                      {(void*)(int)CLI_L2VPN_Delete_Learnt_MAC}},

};

Y_MenuEntry L3VPN_Flow_menu[] = 
{
        {K_Menu, L3VPN_FLOW_MENU_ITEM_NUM, TRUE, "IP Route config",     {(Y_MenuEntry *)V_MainMenu}},
        {K_Leaf, 1, TRUE, " <> Show IP route Info",                     {(void*)(int)CLI_L3VPN_Show_L3VPN_Flow_info}},
        {K_Leaf, 2, TRUE, " -> Create IP route",                        {(void*)(int)CLI_L3VPN_Create_L3VPN_flow}},
        {K_Leaf, 3, TRUE, " -> Delete IP route",                        {(void*)(int)CLI_L3VPN_Delete_L3VPN_flow}},

};

Y_MenuEntry Policer_menu[] = 
{
    {K_Menu, POLICER_MENU_ITEM_NUM, TRUE, "Policer Menu",               {(Y_MenuEntry *)V_MainMenu}},
    {K_Leaf, 1, TRUE, " <> Show TCM Policer",                           {(void*)(int)CLI_POLICER_Show_Policer}},
    {K_Leaf, 2, TRUE, " -> Create TCM Policer",                         {(void*)(int)CLI_POLICER_Create_Policer}},
    {K_Leaf, 3, TRUE, " -> Delete TCM policer",                         {(void*)(int)CLI_POLICER_Delete_Policer}},
};

Y_MenuEntry GPMT_menu[] =
{
    {K_Menu, GPMT_MENU_ITEM_NUM, TRUE, "GPMT Menu",                     {(Y_MenuEntry *)V_MainMenu}},
    {K_Leaf, 1, TRUE, " <> Show GPMT Info",                             {(void*)(int)CLI_GPMT_Show_Info}},

};

Y_MenuEntry STAT_menu[] = 
{
    {K_Menu, STAT_MENU_ITEM_NUM, TRUE, "Statistics Menu",               {(Y_MenuEntry *)V_MainMenu}},
    {K_Leaf, 1, TRUE, " <> Show Device Stats ",                         {(void*)(int)CLI_STAT_ShowDevStats}},
    {K_Leaf, 2, TRUE, " <> Clear Device Stats",                         {(void*)(int)CLI_STAT_ClearDevStats}},
    {K_Leaf, 3, TRUE, " <> Show Bport  Stats",                          {(void*)(int)CLI_STAT_ShowBrgPrtStats}},
    {K_Leaf, 4, TRUE, " <> Clear Bport  Stats",                         {(void*)(int)CLI_STAT_ClearBrgPrtStats}},
    {K_Leaf, 5, TRUE, " <> Show FlwAgg Stats",                          {(void*)(int)CLI_STAT_ShowFlwAggStats}},
    {K_Leaf, 6, TRUE, " <> Clear FlwAgg Stats",                         {(void*)(int)CLI_STAT_ClearFlwAggStats}},
    {K_Leaf, 7, TRUE, " <> Show PceFilter Stats",                       {(void*)(int)CLI_STAT_ShowPceFilterStats}},
    {K_Leaf, 8, TRUE, " <> Clear PceFilter Stats",                      {(void*)(int)CLI_STAT_ClearPceFilterStats}},
    {K_Leaf, 9, TRUE, " <> Show Policer Stats",                         {(void*)(int)CLI_STAT_ShowPolicerStats}},
    {K_Leaf, 10,TRUE, " <> Clear Policer Stats",                        {(void*)(int)CLI_STAT_ClearPolicerStats}},
    {K_Leaf, 11, TRUE," <> Show Flooding Stats",                        {(void*)(int)CLI_STAT_ShowFloodingStats}},
    {K_Leaf, 12,TRUE, " <> Clear Flooding Stats",                       {(void*)(int)CLI_STAT_ClearFloodingStats}},

};

#ifdef __linux__
static struct termios oldt;

void restore_terminal_settings(void)
{
        //Apply saved settings
        tcsetattr(0, TCSANOW, &oldt);
}

//make terminal read 1 char at a time
void disable_terminal_return(void)
{
        struct termios newt;

        //save terminal settings
        tcgetattr(0, &oldt);
        //init new settings
        newt = oldt;
        //change settings
        newt.c_lflag &= ~(ICANON | ECHO);
        //apply settings
        tcsetattr(0, TCSANOW, &newt);

        //make sure settings will be restored when program ends
        atexit(restore_terminal_settings);
}


int get_line(char *buf)
{
        int ch, index, done, i;
        char line[256];
    
        done = 0;
        index = 0;
        memset(line, 0, sizeof(line));
        
        /* Key reading loop */
        while (!done) {
                ch = getchar();
                putchar(ch);
                
                switch (ch)
                {
                case KEY_ENTER:
                        /* skip space */
                        i = 0;
                        while (!(isalpha(line[i]) || isdigit(line[i])) && (i < index)) i++;

                        index -= i;
                        memcpy(buf, line + i, index);
                        buf[index] = '\0';
                        done = 1;
                        break;
                case KEY_BACKSPACE:
                        if (index > 0)
                                index--;
                        printf("\33[K"); 
                        break;
                default:
                        if (index >= 256)
                        {
                                done  = 1;
                                index = 0;
                                break;
                        }
                        
                        line[index++] = ch;
                }
        }

        return index;
}

#else
#define get_line(x) gets(x);printf("\n")
#endif

static char delimiters[] = ".:-";
int F_ConvertStr2MacAddress(char *pStr, unsigned char* pMac)
{
    static char buff[128] = {0};

    char *token = NULL;
    int i = strlen(pStr);
    if ((i < 10) || (i > 128))
    {
            return -1;
    }
    memset(buff, 0, sizeof(buff));
    strcpy(buff, pStr);

    token = strtok(buff, delimiters);
    if (NULL == token)
    {
            return -2;
    }

    pMac[0] = strtoul(token, NULL, 16);

    for (i = 1; i < 6; i++)
    {
        token = strtok (NULL, delimiters);
        if (NULL == token)
        {
                return -3;
        }
        pMac[i] = strtoul(token, NULL, 16);
    }

    return 0;
    
}

unsigned int F_ConvertStr2IPv4Address(char *pStr)
{
    static  char buff[32] = {0};
    struct in_addr addr;

     char *token = NULL;
    int i = strlen(pStr);
    if ((i < 7) || (i > 32))
    {
        return 0;
    }

    while(!isdigit(*pStr))
    {
        pStr++;
    }

    token = pStr;
    while(*token)
    {
       if ((isdigit(*token)) || ('.' == *token))
       {
            token++;
       }
       else
       {
            break;
       }
    }

    i = token - pStr;
    if ((i < 7) || (i > 32))
    {
        return 0;
    }

    memset(buff, 0, sizeof(buff));
    strncpy(buff, pStr, i);

    i = inet_pton(AF_INET, buff, &addr);
    if (i <= 0)
    {
        return 0;
    }

    return ntohl(addr.s_addr);    
  
    
}

/*****************************************************************************
 * Function name: CLI_T_Main
 * Description  : cli main function, get input string and call menu engine.
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void *CLI_T_Main(void* arg)
{
        static char InputBuf[256];

#ifdef  __linux__
        disable_terminal_return();
#endif
        
        /* Display main menu */
        F_DisplayMenu();

        while (demo_running)
        {
                printf("%s", STR_ENTER_COMMAND);
                InputBuf[0] = '\0';
#ifdef  __linux__
                get_line(InputBuf);
#else
                flush();
                gets(InputBuf);
#endif
                printf ("\n");
                F_MenuEngine(InputBuf);
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
static int CLI_MAIN_Reboot(char *StrPrm)
{
        demo_running = FALSE;
        puts("Reboot...\r\n");
        WP_Delay(5);    
        WPE_Bridge_Quit(1);        
        return 0;
}


int CLI_MAIN_Quit(char *StrPrm)
{
        demo_running = FALSE;
        puts("Quit...\r\n");
        WP_Delay(5);
        WPE_Bridge_Quit(0);        
        return 0;
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

        V_CurrMenuP = (Y_MenuEntry *)&V_MainMenu[0];

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

        V_CurrMenuP = (Y_MenuEntry *)V_CurrMenuP->Node.Menu;

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
        printf ("  Name: %s                                              \n", V_CurrMenuP->Desc);
        printf ("%s\n", STR_STAR);
        CurrMenuEntryP = V_CurrMenuP;

        /* Skip to the 1st menu Item */
        CurrMenuEntryP++;

        for (Counter = 1; Counter <= V_CurrMenuP->Index; Counter++, CurrMenuEntryP++)
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
                Cmd = atoi(InputBuf);
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
                (MenuEntryP->Node.Leaf)(InputBuf);
                F_DisplayMenu();
        }
        else if (K_Menu == MenuEntryP->Type)
        {
                V_CurrMenuP = (Y_MenuEntry *)MenuEntryP->Node.Menu;
                F_DisplayMenu();
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
        case HOTKEY_MAIN_MENU:                  /* Goto main menu  */
                F_SetMainMenu();
                F_DisplayMenu();
                break;

        case HOTKEY_UPPER_MENU:                  /* Goto upper menu */
                F_SetUpperMenu();
                F_DisplayMenu();
                break;

        case HOTKEY_CUR_MENU:                  /* Display current menu again */
                F_DisplayMenu();
                break;
        
        case HOTKEY_QUIT_PROG:                  /* Quit */
                CLI_MAIN_Quit("quit");
                break;

        case HOTKEY_REBOOT:
                CLI_MAIN_Reboot("reboot");
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
void F_MenuEngine(char *InputBuf)
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

WP_U32 CLI_GetNumber(WP_CHAR *text, WP_U32 low, WP_U32 high)
{
        int ret_val;
        WP_U32  val;
   
        while(1)
        {
                printf("\n\n%s: \n", text);
      
                /* Get Shaping Parameter */
                get_line(val_str);
                ret_val = strlen(val_str);
                if((ret_val == 0) || (strlen(val_str) > 10))
                {
                        continue;
                }
                val = atoi(val_str);
      
                if (val >= low && val <= high)
                        break;
        }
   
        printf("\nyou choose = %d\n\n", val);
        return val;
}


int CLI_Display(char *StrPrm)
{
    int handle = 0;
    int param = 0;
    char* pPortStr = NULL;
    printf("Please input handle, param.\n");
    
    get_line(val_str);
    handle = strtoul(val_str, &pPortStr, 0);
    if (pPortStr)
    {
        param = strtoul(pPortStr, NULL, 0);
    }

    WPE_Display(handle, param);
    
    return 0;
}


int CLI_L3VPN_Show_L3VPN_Flow_info(char *StrPrm)
{

    WPE_IPRouteShow();
        return WP_OK;
}


int CLI_L3VPN_Create_L3VPN_flow(char *StrPrm)
{
    int ret = 0;

    WP_U8 vpnid, egress_port;
    WP_U32 d_ip, prefix;
    char* pPortStr = NULL;
    char* pPrefixStr = NULL;
    char* pDaStr = NULL;
    printf("Please input vpnid, egress VPN port, dst ipv4 address, prefix.\n");

    while(0 == ret)
    {
        pPortStr = NULL;
        pPrefixStr = NULL;
        pDaStr = NULL;    
        get_line(val_str);
        
        vpnid = strtoul(val_str, &pPortStr, 0);
        if (NULL == pPortStr)
        {
            printf("Invalid input!\n");
            return -1;
        }
        
        egress_port = strtoul(pPortStr, &pDaStr, 0);
        if (NULL == pDaStr)
        {
            printf("Invalid input!\n");
            return -1;        
        }

        while(!isdigit(*pDaStr))
        {
            pDaStr++;
        }
        
        d_ip = F_ConvertStr2IPv4Address(pDaStr);
        if (0 == d_ip)
        {
            printf("Invalid input!\n");
            return -1;                      
        } 

        pPrefixStr = strchr(pDaStr, ' ');
        if (NULL == pPrefixStr)
        {
            printf("Invalid input!\n");
            return -1;
        }

        while(!isdigit(*pPrefixStr))
        {
            pPrefixStr++;
        }
        
        prefix = F_ConvertStr2IPv4Address(pPrefixStr);
        if (0 == prefix)
        {
            printf("Invalid input!\n");
            return -1;                      
        }

        ret = WPE_IPRouteCreate(vpnid, egress_port, prefix, d_ip);
        printf("done, ret = %d\n", ret);
    }
    return WP_OK;
}


int CLI_L3VPN_Delete_L3VPN_flow(char *StrPrm)
{
    int ret = 0;

    WP_U16 vpnid;
    WP_U32 d_ip, prefix;
    char* pPrefixStr = NULL;
    char* pDaStr = NULL;
    printf("Please input vpnid, dst ipv4 address, prefix.\n");
    while(0 == ret)
    {
        pPrefixStr = NULL;
        pDaStr = NULL;

        get_line(val_str);
        
        vpnid = strtoul(val_str, &pDaStr, 0);
        if (NULL == pDaStr)
        {
            printf("Invalid input!\n");
            return -1;
        }

        while(!isdigit(*pDaStr))
        {
            pDaStr++;
        }
        
        d_ip = F_ConvertStr2IPv4Address(pDaStr);
        if (0 == d_ip)
        {
            printf("Invalid input!\n");
            return -1;                      
        } 

        pPrefixStr = strchr(pDaStr, ' ');
        if (NULL == pPrefixStr)
        {
            printf("Invalid input!\n");
            return -1;
        }

        while(!isdigit(*pPrefixStr))
        {
            pPrefixStr++;
        }
        
        prefix = F_ConvertStr2IPv4Address(pPrefixStr);
        if (0 == prefix)
        {
            printf("Invalid input!\n");
            return -1;                      
        }
        
        ret = WPE_IPRouteDelete(vpnid, prefix, d_ip);
        
        printf("done, ret = %d\n", ret);
    }
    return WP_OK;
}

int CLI_L2VPN_Show_Static_MAC_info(char *StrPrm)
{
    WPE_StaticMACShow();
        return WP_OK;
}


int CLI_L2VPN_Insert_Static_MAC(char *StrPrm)
{
    int ret = 0;

    WP_U16 vpnid, egress_port;
    WP_U8  d_mac[6];
    char* pPortStr = NULL;
    char* pDaMacStr = NULL;
    printf("Please input vpnid, egress VPN port, dst mac.\n");

    while(0 == ret)
    {
         pPortStr = NULL;
         pDaMacStr = NULL;
    
        get_line(val_str);
        
        vpnid = strtoul(val_str, &pPortStr, 0);
        if (NULL == pPortStr)
        {
            printf("Invalid input!\n");
            return -1;
        }
        
        egress_port = strtoul(pPortStr, &pDaMacStr, 0);
        if (NULL == pDaMacStr)
        {
            printf("Invalid input!\n");
            return -1;        
        }

        ret = F_ConvertStr2MacAddress(pDaMacStr, d_mac);
        if (ret)
        {
            printf("Invalid input!\n");
            return -1;
        }
        ret = WPE_StaticMACInsert(vpnid, egress_port, d_mac);
        printf("done, ret = %d\n", ret);
    }
    return WP_OK;
}


int CLI_L2VPN_Delete_Static_MAC(char *StrPrm)
{
    int ret = 0;

    WP_U8 vpnid;
    WP_U8  d_mac[6];

    char* pDaMacStr = NULL;
    printf("Please input vpnid, dst mac.\n");

    while(0 == ret)
    {
         pDaMacStr = NULL;
    
        get_line(val_str);
        
        vpnid = strtoul(val_str, &pDaMacStr, 0);
        if (NULL == pDaMacStr)
        {
            printf("Invalid input!\n");
            return -1;        
        }

        ret = F_ConvertStr2MacAddress(pDaMacStr, d_mac);
        if (ret)
        {
            printf("Invalid input!\n");
            return -1;
        }
        ret = WPE_StaticMACDelete(vpnid, d_mac);
        printf("done, ret = %d\n", ret);
    }
    return ret;
}

int CLI_L2VPN_Delete_Learnt_MAC(char *StrPrm)
{
    int ret = 0;

    WP_U8 vpnid;
    WP_U8  d_mac[6];

    char* pDaMacStr = NULL;
    printf("Please input vpnid, dst mac.\n");

    while(0 == ret)
    {
         pDaMacStr = NULL;
    
        get_line(val_str);
        
        vpnid = strtoul(val_str, &pDaMacStr, 0);
        if (NULL == pDaMacStr)
        {
            printf("Invalid input!\n");
            return -1;        
        }

        ret = F_ConvertStr2MacAddress(pDaMacStr, d_mac);
        if (ret)
        {
            printf("Invalid input!\n");
            return -1;
        }
        ret = WPE_DelLearntMac(vpnid, d_mac);
        printf("done, ret = %d\n", ret);
    }
    return ret;
}


/******************************* vlan flooding groups *************************/


int CLI_VPN_Create_L2VPN(char *StrPrm)
{
    int ret = 0;

    ret = WPE_VPNSystemCreateVSI(0);

    printf("done, vpnid = %d\n", ret);
    
    return ret;

}

int CLI_VPN_Create_L3VPN(char *StrPrm)
{
    int ret = 0;

    ret = WPE_VPNSystemCreateVRF(0);
    printf("done, vpnid = %d\n", ret);
    return ret;

}

int CLI_VPN_Create_VPWS(char *StrPrm)
{
    int ret = 0;

    ret = WPE_VPNSystemCreateVPWS(0);
    printf("done, vpnid = %d\n", ret);
    return ret;

}


int CLI_VPN_Delete_VPN(char *StrPrm)
{   
    int ret = 0;

    WP_U16 vpnid;
    printf("Please input vpnid.\n");
    get_line(val_str);
    
    vpnid = strtoul(val_str, NULL, 0);

    ret = WPE_VPNSystemDelete(vpnid);
    
    return ret;

}


int CLI_VPN_Show_VPN(char *StrPrm)
{

    WPE_VPNSystemInfo();
    
    return OK;
}




/***********************multicast***************************/
int CLI_SUBPORT_Show_SubPort_Info(char *StrPrm)
{

    WPE_SubPortShowInfo();

    return OK;
}


int CLI_SUBPORT_Create_SubPort(char *StrPrm)
{
    int ret = 1;

    WP_U16 port, vlan, defvlan;
    WP_U8  s_mac[6], d_mac[6];
    char* pVlanStr = NULL;
    char* pDefVlanStr = NULL;
    char* pSaMacStr = NULL;
    char* pDaMacStr = NULL;
    printf("Please input portid, vlan, default vlan, s-mac and d-mac.\n");

    while(ret > 0)
    {
        get_line(val_str);
        if (strlen(val_str) < 3)
        {
            break;
        }
        pVlanStr = NULL;
        pDefVlanStr = NULL;
        pSaMacStr = NULL;
        pDaMacStr = NULL;        
        port = 0;
        vlan = 0;
        defvlan = 0;
        memset(s_mac, 0, 6);
        memset(d_mac, 0, 6);
        
        port = strtoul(val_str, &pVlanStr, 0);
        if (NULL == pVlanStr)
        {
            printf("Invalid input!\n");
            return -1;
        }
        
        while (('\0' != *pVlanStr) && (!isalnum(*pVlanStr)))
        {
            pVlanStr++;
        }
        vlan = strtoul(pVlanStr, &pDefVlanStr, 0);  
        
        if (NULL == pDefVlanStr)
        {
            printf("Invalid input!\n");
            return -1;
        }

        while (('\0' != *pDefVlanStr) && (!isalnum(*pDefVlanStr)))
        {
            pDefVlanStr++;
        }
        
        defvlan = strtoul(pDefVlanStr, &pSaMacStr, 0);
        
        while (('\0' != *pSaMacStr) && (!isalnum(*pSaMacStr)))
        {
            pSaMacStr++;
        }
        
        if (pSaMacStr && (strlen(pSaMacStr) > 10))
        {            
            ret = F_ConvertStr2MacAddress(pSaMacStr, s_mac);
            if (ret)
            {
                printf("Invalid input!\n");
                return -1;
            }           
            
            pDaMacStr = strchr(pSaMacStr, ' ');
            if (NULL == pDaMacStr)
            {
                printf("Invalid input!\n");
                return -1;
            
            }
            
            ret = F_ConvertStr2MacAddress(pDaMacStr, d_mac);
            if (ret)
            {
                printf("Invalid input!\n");
                return -1;
            
            }      
        }

        ret = WPE_SubPortCreate(port, vlan, defvlan, s_mac, d_mac);

        printf("Done, sub port id = %d\r\n", ret);
    }
    return OK;
}


int CLI_SUBPORT_Delete_SubPort(char *StrPrm)
{
    int ret = 0;
    WP_U16  port, subport;
    char* pSubportStr = NULL;
    printf("Please input port, sub port.\n");

    do
    {
        get_line(val_str);
        port = 0;
        subport = 0;
        pSubportStr = NULL;

        port = strtoul(val_str, &pSubportStr, 0);
        if (NULL == pSubportStr)
        {
            printf("Invalid input!\n");
            return -1;        
        }

        subport = strtoul(pSubportStr, NULL, 0);

        ret = WPE_SubPortDelete(port, subport);
    }while(0 == ret);

    return OK;
}

int CLI_SUBPORT_Set_AF_Mode(char *StrPrm)
{
    int ret = 0;

    WP_U8 port, subport, spwfq;
    WP_U8 weight1=0, weight2=0, weight3=0, weight4=0;
    char* pSubPortStr = NULL;
    char* pSpWfqStr = NULL;
    char* pWeight1Str = NULL;
    char* pWeight2Str = NULL;
    char* pWeight3Str = NULL;
    char* pWeight4Str = NULL;
    printf("Please input port, subport, sp/wfq(0/1) [weight1, weight2, weight3 and weight4].\n");

    while (0 == ret)
    {
         pSubPortStr = NULL;
         pSpWfqStr = NULL;
         pWeight1Str = NULL;
         pWeight2Str = NULL;
         pWeight3Str = NULL;
         pWeight4Str = NULL;

        get_line(val_str);

        port = strtoul(val_str, &pSubPortStr, 0);
        if (NULL == pSubPortStr)
        {
            printf("Invalid input!\n");
            return -1;
        }
        
        subport = strtoul(pSubPortStr, &pSpWfqStr, 0);
        if (NULL == pSpWfqStr)
        {
            printf("Invalid input!\n");
            return -1;
        }

        spwfq = strtoul(pSpWfqStr, &pWeight1Str, 0);

        if (spwfq)
        {
            if (NULL == pWeight1Str)
            {
                printf("Invalid input!\n");
                return -1;        
            }
            weight1 = strtoul(pWeight1Str, &pWeight2Str, 0);
            if (NULL == pWeight2Str)
            {
                printf("Invalid input!\n");
                return -1;
            }        
            weight2 = strtoul(pWeight2Str, &pWeight3Str, 0);
            if (NULL == pWeight3Str)
            {
                printf("Invalid input!\n");
                return -1;
            }  
            weight3 = strtoul(pWeight3Str, &pWeight4Str, 0);
            if (NULL == pWeight4Str)
            {
                printf("Invalid input!\n");
                return -1;
            }  
            weight4 = strtoul(pWeight4Str, NULL, 0);

        }

        ret = WPE_SubPortSetAfmode(port, subport, spwfq, weight1, weight2, weight3, weight4);
    }
    return ret;
}


int CLI_POLICER_Show_Policer(char *StrPrm)
{
    WPE_PolicerShow();
    return OK;
}


int CLI_POLICER_Create_Policer(char *StrPrm)
{
    int ret = 0;
    WP_U8  colermode;
    WP_U16  vpnport = 0;
    WP_U32  cir, eir, cbs, ebs;
    char* pCIRStr = NULL;
    char* pEIRStr = NULL;

    char* pCBSStr = NULL;
    char* pEBSStr = NULL;
    char* pColorStr = NULL;
    printf("Please input vpnport, CIR, EIR, CBS, EBS, color mode(0/1).\n");

    while(0 == ret)
    {
         pCIRStr = NULL;
         pEIRStr = NULL;
        
         pCBSStr = NULL;
         pEBSStr = NULL;
         pColorStr = NULL;
    
        get_line(val_str);
        
        vpnport = strtoul(val_str, &pCIRStr, 0);
        if (NULL == pCIRStr)
        {
                printf("Input invalid!\n");
                return -1;            
        }

        cir = strtoul(pCIRStr, &pEIRStr, 0);
        if (NULL == pCIRStr)
        {
                printf("Input invalid!\n");
                return -1;            
        }    
        
        eir = strtoul(pEIRStr, &pCBSStr, 0);
        if (NULL == pCBSStr)
        {
                printf("Input invalid!\n");
                return -1;            
        } 
        cbs = strtoul(pCBSStr, &pEBSStr, 0);
        if (NULL == pEBSStr)
        {
                printf("Input invalid!\n");
                return -1;            
        } 
        ebs = strtoul(pEBSStr, &pColorStr, 0);
        if (NULL == pColorStr)
        {
                printf("Input invalid!\n");
                return -1;            
        }
        colermode = strtoul(pColorStr, NULL, 0);

        ret = WPE_PolicerCreate(vpnport, cir, eir, cbs, ebs, colermode);
    }
    return ret;
}

int CLI_POLICER_Delete_Policer(char *StrPrm)
{
    int ret = 0;
    WP_U16  vpnport = 0;
    printf("Please input vpnport.\n");
    while(0 ==ret)
    {
        get_line(val_str);
        
        vpnport = strtoul(val_str, NULL, 0);  

        ret = WPE_PolicerDelete(vpnport);
    }
    return ret;
}


int CLI_PORT_Show_Port_Info (char *StrPrm)
{

    WPE_PortShowInfo();
    
    return OK;
}

int CLI_PORT_Set_Port_Mode(char *StrPrm)
{
    int ret = 0;

    WP_U16 port, uninni, defvlan;
    WP_U8  s_mac[6], d_mac[6];
    char* pUniNniStr = NULL;
    char* pDefVlanStr = NULL;
    char* pSaMacStr = NULL;
    char* pDaMacStr = NULL;
    printf("Please input portid, uni/nni(0/1), default vlan, s-mac and d-mac.\n");
    WPE_PortListDisplay();

    while(0 == ret)
    {
        pUniNniStr = NULL;
        pDefVlanStr = NULL;
        pSaMacStr = NULL;
        pDaMacStr = NULL;

        get_line(val_str);
    
        port = strtoul(val_str, &pUniNniStr, 0);
        if (NULL == pUniNniStr)
        {
            printf("Invalid input!\n");
            return -1;
        }
        
        uninni = strtoul(pUniNniStr, &pDefVlanStr, 0);
        if (NULL == pDefVlanStr)
        {
            printf("Invalid input!\n");
            return -1;        
        }

        defvlan = strtoul(pDefVlanStr, &pSaMacStr, 0);
        if (NULL == pSaMacStr)
        {
            printf("Invalid input!\n");
            return -1;        
        }

        while(' ' == *pSaMacStr)
        {
            pSaMacStr++;
        }

        ret = F_ConvertStr2MacAddress(pSaMacStr, s_mac);
        if (ret)
        {
            printf("Invalid input!\n");
            return -1;
        }

        pDaMacStr = strchr(pSaMacStr, ' ');
        if (NULL == pDaMacStr)
        {
            return ERROR;
        }

        ret = F_ConvertStr2MacAddress(pDaMacStr, d_mac);
        if (ret)
        {
            return ERROR;
        }

        ret = WPE_PortSet(port, uninni, defvlan, s_mac, d_mac);
    }
    
    return ret;
}

int CLI_PORT_Set_AF_Mode(char *StrPrm)
{
    int ret = 0;

    WP_U8 port, spwfq;
    WP_U16 weight1=0, weight2=0, weight3=0, weight4=0;
    char* pSpWfqStr = NULL;
    char* pWeight1Str = NULL;
    char* pWeight2Str = NULL;
    char* pWeight3Str = NULL;
    char* pWeight4Str = NULL;
    
    WPE_PortListDisplay();
    printf("Please input portid, sp/wfq(0/1) [weight1, weight2, weight3 and weight4].\n");

    while(0 == ret)
    {
        pSpWfqStr = NULL;
        pWeight1Str = NULL;
        pWeight2Str = NULL;
        pWeight3Str = NULL;
        pWeight4Str = NULL;
    
        get_line(val_str);
        
        port = strtoul(val_str, &pSpWfqStr, 0);
        if (NULL == pSpWfqStr)
        {
            printf("Invalid input!\n");
            return -1;
        }

        spwfq = strtoul(pSpWfqStr, &pWeight1Str, 0);

        if (spwfq)
        {
            if (NULL == pWeight1Str)
            {
                printf("Invalid input!\n");
                return -1;        
            }
            weight1 = strtoul(pWeight1Str, &pWeight2Str, 0);
            if (NULL == pWeight2Str)
            {
                printf("Invalid input!\n");
                return -1;
            }        
            weight2 = strtoul(pWeight2Str, &pWeight3Str, 0);
            if (NULL == pWeight3Str)
            {
                printf("Invalid input!\n");
                return -1;
            }  
            weight3 = strtoul(pWeight3Str, &pWeight4Str, 0);
            if (NULL == pWeight4Str)
            {
                printf("Invalid input!\n");
                return -1;
            }  
            weight4 = strtoul(pWeight4Str, NULL, 0);

        }

        ret  = WPE_PortSetAfmode(port, spwfq, weight1, weight2, weight3, weight4);
    }
    return ret;
}


int CLI_VPNPORT_Show_VPN_port_info(char *StrPrm)
{
    WPE_VPNPortShow();

    return 0;
}

int CLI_VPNPORT_Create_UNI_VPN_port(char *StrPrm)
{
    int ret = 1;
    WP_U16 port, subport, s_tag, c_tag, vpnid, egress_s_tag, egress_c_tag;
    char* pSubportStr = NULL;
    char* pSTagStr = NULL;
    char* pCTagStr = NULL;
    char* pVPNStr = NULL;
    char* pEgressSTagStr = NULL;
    char* pEgressCTagStr = NULL;    

    printf("Please input port, subport, s-tag, c-tag, vpnid, egress s-tag, egress c-tag.\n");
    while(ret > 0)
    {
         pSubportStr = NULL;
         pSTagStr = NULL;
         pCTagStr = NULL;
         pVPNStr = NULL;
         pEgressSTagStr = NULL;
         pEgressCTagStr = NULL;    

        get_line(val_str);
        
        port = strtoul(val_str, &pSubportStr, 0);
        if (NULL == pSubportStr)
        {
            printf("Invalid input!\n");
            return -1;
        }
        subport = strtoul(pSubportStr, &pSTagStr, 0);
        if (NULL == pSTagStr)
        {
            printf("Invalid input!\n");
            return -1;
        }    
        s_tag = strtoul(pSTagStr, &pCTagStr, 0);
        if (NULL == pCTagStr)
        {
            printf("Invalid input!\n");
            return -1;
        } 
        c_tag = strtoul(pCTagStr, &pVPNStr, 0);
        if (NULL == pVPNStr)
        {
            printf("Invalid input!\n");
            return -1;
        } 
        vpnid = strtoul(pVPNStr, &pEgressSTagStr, 0);
        if (NULL == pEgressSTagStr)
        {
            printf("Invalid input!\n");
            return -1;
        } 
        egress_s_tag = strtoul(pEgressSTagStr, &pEgressCTagStr, 0);
        if (NULL == pEgressCTagStr)
        {
            printf("Invalid input!\n");
            return -1;
        }
        egress_c_tag = strtoul(pEgressCTagStr, NULL, 0);

        ret = WPE_VPNPortCreateUNI(port, subport, vpnid, s_tag, c_tag, egress_s_tag, egress_c_tag);
        printf("done, uni vpn port id = %d\n", ret);
    }
    
    return ret;
}


int CLI_VPNPORT_Create_NNI_VPN_port(char *StrPrm)
{
    int ret = 1;
    WP_U16 port, pw_label, lsp_label, vpnid;
    char* pPWLabelStr = NULL;
    char* pLSPLabelStr = NULL;
    char* pVPNStr = NULL;

    printf("Please input port, PW label, lsp lable and vpnid.\n");
    while(ret > 0)
    {
         pPWLabelStr = NULL;
         pLSPLabelStr = NULL;
         pVPNStr = NULL;
    
        get_line(val_str);
        
        port = strtoul(val_str, &pPWLabelStr, 0);
        if (NULL == pPWLabelStr)
        {
            printf("Invalid input!\n");
            return -1;
        }
        
        pw_label = strtoul(pPWLabelStr, &pLSPLabelStr, 0);
        if (NULL == pLSPLabelStr)
        {
            printf("Invalid input!\n");
            return -1;
        }
        
        lsp_label = strtoul(pLSPLabelStr, &pVPNStr, 0);
        if (NULL == pVPNStr)
        {
            printf("Invalid input!\n");
            return -1;
        }
        
        vpnid = strtoul(pVPNStr, NULL, 0);

        ret = WPE_VPNPortCreateNNI(port, vpnid, pw_label, lsp_label);
        printf("done, nni vpn port id = %d\n", ret);
    }
    return ret;
}


int CLI_VPNPORT_Delete_VPN_port(char *StrPrm)
{
    int ret = 0;
    WP_U16 vpnport;

    printf("Please input vpn portid.\n");
    while(0 == ret)
    {    
        get_line(val_str);
        
        vpnport = strtoul(val_str, NULL, 0);

        ret = WPE_VPNPortDelete(vpnport);
    }
    return ret;
}

int CLI_GPMT_Show_Info(char *StrPrm)
{
    WPE_GPMT_Show();
    return OK;
}

/*****************************************************************************
 * Function name: CLI_STAT_ShowStats
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
int CLI_STAT_ShowDevStats(char *StrPrm)
{

        WPE_LocalDisplayDeviceStats();
        return WP_OK;
}

/*****************************************************************************
 * Function name: CLI_STAT_ClearDevStats
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
int CLI_STAT_ClearDevStats(char *StrPrm)
{
    WPE_LocalDeviceStatsReset();

        return WP_OK;
}

/*****************************************************************************
 * Function name: CLI_STAT_ShowBrgPrtStats
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
int CLI_STAT_ShowBrgPrtStats(char *StrPrm)
{
    WPE_DisplayBridgingPortStats();


        return WP_OK;
}

/*****************************************************************************
 * Function name: CLI_STAT_ClearBrgPrtStats
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
int CLI_STAT_ClearBrgPrtStats(char *StrPrm)
{
    WPE_IwBridgingPortStatsReset();


        return WP_OK;
}

/*****************************************************************************
 * Function name: CLI_STAT_ShowFlwAggStats
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
int CLI_STAT_ShowFlwAggStats(char *StrPrm)
{
    WPE_GenericFlowStatistics();
        return WP_OK;
}

/*****************************************************************************
 * Function name: CLI_STAT_ClearFlwAggStats
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
int CLI_STAT_ClearFlwAggStats(char *StrPrm)
{
    WPE_GenericFlowStatisticsReset();

        return WP_OK;
}



/*****************************************************************************
 * Function name: CLI_STAT_ShowPceFilterStats
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
int CLI_STAT_ShowPceFilterStats(char *StrPrm)
{
    WPE_PCEStatistics();
        return WP_OK;
}

/*****************************************************************************
 * Function name: CLI_STAT_ClearFlwAggStats
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
int CLI_STAT_ClearPceFilterStats(char *StrPrm)
{
    WPE_PCEStatisticsReset();

        return WP_OK;
}

int CLI_STAT_ShowPolicerStats(char *StrPrm)
{
    WPE_PolicerStatistics();
        return WP_OK;
}


/*****************************************************************************
 * Function name: CLI_STAT_ClearFlwAggStats
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
int CLI_STAT_ClearPolicerStats(char *StrPrm)
{
    WPE_PolicerStatisticsReset();

        return WP_OK;
}

int CLI_STAT_ShowFloodingStats(char *StrPrm)
{
    WPE_FloodingStatistics();
        return WP_OK;
}


/*****************************************************************************
 * Function name: CLI_STAT_ClearFlwAggStats
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
int CLI_STAT_ClearFloodingStats(char *StrPrm)
{
    WPE_FloodingStatisticsReset();

        return WP_OK;
}


int CLI_WinUtil(char *StrPrm)
{
    WP_CHAR InputBuf[255];
    
    printf ("Enter winutil command >\n");
    get_line(InputBuf);
    printf("\n");
    WPUI_RunWinMon(InputBuf);

    return 0;
}

