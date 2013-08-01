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

#define TRUE     1
#define FALSE    0


#define STR_STAR           "******************************************************************"
#define STR_HOT_KEYS       "  Hot keys: m=Main Menu, u=Upper Menu, d=Display Cur Menu, q=Quit "
#define STR_ENTER_COMMAND  "# Enter your command:"
#define STR_MENU_ERR       "Err> Wrong menu command"

#define HOTKEY_MAIN_MENU        'm'
#define HOTKEY_UPPER_MENU       'u'
#define HOTKEY_CUR_MENU         'd'
#define HOTKEY_QUIT_PROG        'q'
#define HOTKEY_REBOOT           'r'


#ifndef ERROR
#define ERROR  1
#endif
#define OK     0


typedef int  (*Y_MenuLeafP) (char *StrPrm);

typedef enum
{
    K_Menu,
    K_Leaf,
} E_MenuNodeType;


typedef struct
{
    E_MenuNodeType Type;
    unsigned char Index;
    unsigned char Visible;    
    char *Desc;
    union
    {
        void *Menu;
        Y_MenuLeafP Leaf;
    } Node;
} Y_MenuEntry;




char *STR_MENU_HDR = "           ASB PM COUNTERS APP          ";



Y_MenuEntry V_MainMenu[];

Y_MenuEntry V_UnicastTest[];
Y_MenuEntry V_UnicastTestScenario1[];
Y_MenuEntry V_UnicastTestScenario2[];
Y_MenuEntry V_UnicastTestScenario3[];
Y_MenuEntry V_UnicastTestScenario4[];
Y_MenuEntry V_UnicastTestScenario5[];
Y_MenuEntry V_UnicastTestScenario6[];

Y_MenuEntry V_MulticastTest[];
Y_MenuEntry V_MulticastTestScenario1[];
Y_MenuEntry V_MulticastTestScenario2[];
Y_MenuEntry V_MulticastTestScenario3[];
Y_MenuEntry V_MulticastTestScenario4[];
Y_MenuEntry V_MulticastTestScenario5[];

Y_MenuEntry V_HostTest[];
Y_MenuEntry V_HostTestScenario1[];
Y_MenuEntry V_HostTestScenario2[];
Y_MenuEntry V_HostTestScenario3[];
Y_MenuEntry V_HostTestScenario4[];
Y_MenuEntry V_HostTestScenario5[];
Y_MenuEntry V_HostTestScenario6[];
Y_MenuEntry V_HostTestScenario7[];
Y_MenuEntry V_HostTestScenario8[];
Y_MenuEntry V_HostTestScenario9[];

static Y_MenuEntry *V_CurrMenuP = &V_MainMenu[0];


/******************************* MENU API *************************/
void CLI_T_Main(void);
static int CLI_MAIN_Reboot(char *StrPrm);
int CLI_MAIN_Quit(char *StrPrm);
void F_SetMainMenu (void);
void F_SetUpperMenu (void);
void F_DisplayHeader (void);
void F_DisplayMenu (void);
void F_MenuEngine (char *InputBuf);
void F_EvaluateCommand (char *InputBuf);
int F_EvaluateHotKey (char *InputBuf);




void CLI_Unicast_Scenario1_Setup(void);
void CLI_Unicast_Scenario1_ResetStatistics(void);
void CLI_Unicast_Scenario1_ShowStatistics(void);

void CLI_Unicast_Scenario2_Setup(void);
void CLI_Unicast_Scenario2_ResetStatistics(void);
void CLI_Unicast_Scenario2_ShowStatistics(void);

void CLI_Unicast_Scenario3_Setup(void);
void CLI_Unicast_Scenario3_ResetStatistics(void);
void CLI_Unicast_Scenario3_ShowStatistics(void);

void CLI_Unicast_Scenario4_Setup(void);
void CLI_Unicast_Scenario4_ResetStatistics(void);
void CLI_Unicast_Scenario4_ShowStatistics(void);

void CLI_Unicast_Scenario5_Setup(void);
void CLI_Unicast_Scenario5_ResetStatistics(void);
void CLI_Unicast_Scenario5_ShowStatistics(void);

void CLI_Unicast_Scenario6_Setup(void);
void CLI_Unicast_Scenario6_ResetStatistics(void);
void CLI_Unicast_Scenario6_ShowStatistics(void);

void CLI_Multicast_Scenario1_Setup(void);
void CLI_Multicast_Scenario1_ResetStatistics(void);
void CLI_Multicast_Scenario1_ShowStatistics(void);

void CLI_Multicast_Scenario2_Setup(void);
void CLI_Multicast_Scenario2_ResetStatistics(void);
void CLI_Multicast_Scenario2_ShowStatistics(void);

void CLI_Multicast_Scenario3_Setup(void);
void CLI_Multicast_Scenario3_ResetStatistics(void);
void CLI_Multicast_Scenario3_ShowStatistics(void);

void CLI_Multicast_Scenario4_Setup(void);
void CLI_Multicast_Scenario4_ResetStatistics(void);
void CLI_Multicast_Scenario4_ShowStatistics(void);

void CLI_Multicast_Scenario5_Setup(void);
void CLI_Multicast_Scenario5_ResetStatistics(void);
void CLI_Multicast_Scenario5_ShowStatistics(void);



void CLI_Host_Scenario1_Setup(void);
void CLI_Host_Scenario1_ResetStatistics(void);
void CLI_Host_Scenario1_ShowStatistics(void);

void CLI_Host_Scenario2_Setup(void);
void CLI_Host_Scenario2_ResetStatistics(void);
void CLI_Host_Scenario2_ShowStatistics(void);

void CLI_Host_Scenario3_Setup(void);
void CLI_Host_Scenario3_ResetStatistics(void);
void CLI_Host_Scenario3_ShowStatistics(void);

void CLI_Host_Scenario4_Setup(void);
void CLI_Host_Scenario4_ResetStatistics(void);
void CLI_Host_Scenario4_ShowStatistics(void);

void CLI_Host_Scenario5_Setup(void);
void CLI_Host_Scenario5_ResetStatistics(void);
void CLI_Host_Scenario5_ShowStatistics(void);

void CLI_Host_Scenario6_Setup(void);
void CLI_Host_Scenario6_ResetStatistics(void);
void CLI_Host_Scenario6_ShowStatistics(void);


void CLI_Host_Scenario7_Setup(void);
void CLI_Host_Scenario7_ResetStatistics(void);
void CLI_Host_Scenario7_ShowStatistics(void);


void CLI_Host_Scenario8_Setup(void);
void CLI_Host_Scenario8_ResetStatistics(void);
void CLI_Host_Scenario8_ShowStatistics(void);


void CLI_Host_Scenario9_Setup(void);
void CLI_Host_Scenario9_ResetStatistics(void);
void CLI_Host_Scenario9_ShowStatistics(void);

void CLI_ShowPortStatus(void);

void CLI_HostSendData(void);

int CLI_WinUtil(char *StrPrm);

Y_MenuEntry V_MainMenu[] = 
{
        {K_Menu, 5, TRUE, "Main Menu",         {(Y_MenuEntry *)V_MainMenu}},
        {K_Menu, 1, TRUE, " -> Unicast test",                      {(Y_MenuEntry *)V_UnicastTest}},
        {K_Menu, 2, TRUE, " -> Multicast test",                {(Y_MenuEntry *)V_MulticastTest}},
        {K_Menu, 3, TRUE, " -> Host test",                     {(Y_MenuEntry *)V_HostTest}},
        {K_Leaf, 4, TRUE, " -> Winutil",                              {(void*)(int)CLI_WinUtil}},    
        {K_Leaf, 5, TRUE, " Quit",                              {(void*)(int)CLI_MAIN_Quit}},
};


Y_MenuEntry V_UnicastTest[] = 
{
        {K_Menu, 6, TRUE, "Unicast Test",                       {(Y_MenuEntry *)V_MainMenu}},
        {K_Menu, 1, TRUE, " -> Scenario #1",                    {(void*)(int)V_UnicastTestScenario1}},
        {K_Menu, 2, TRUE, " -> Scenario #2",                    {(void*)(int)V_UnicastTestScenario2}},
        {K_Menu, 3, TRUE, " -> Scenario #3",                    {(void*)(int)V_UnicastTestScenario3}},
        {K_Menu, 4, TRUE, " -> Scenario #4",                    {(void*)(int)V_UnicastTestScenario4}},
        {K_Menu, 5, TRUE, " -> Scenario #5",                    {(void*)(int)V_UnicastTestScenario5}},
        {K_Menu, 6, TRUE, " -> Scenario #6",                    {(void*)(int)V_UnicastTestScenario6}},
};


Y_MenuEntry V_UnicastTestScenario1[] = 
{
        {K_Menu, 5, TRUE, "Unicast Test Scenario #1",         {(Y_MenuEntry *)V_UnicastTest}},
        {K_Leaf, 1, TRUE, " Setup",                           {(void*)(int)CLI_Unicast_Scenario1_Setup}},
        {K_Leaf, 2, TRUE, " RESET STATISTICS",                    {(void*)(int)CLI_Unicast_Scenario1_ResetStatistics}},
        {K_Leaf, 3, TRUE, " SHOW STATISTICS",                    {(void*)(int)CLI_Unicast_Scenario1_ShowStatistics}},
        {K_Leaf, 4, TRUE, " SHOW Port Status",                    {(void*)(int)CLI_ShowPortStatus}},
        {K_Leaf, 5, TRUE, " Send data to ENET",                    {(void*)(int)CLI_HostSendData}},
        
};

Y_MenuEntry V_UnicastTestScenario2[] = 
{
        {K_Menu, 3, TRUE, "Unicast Test Scenario #2",         {(Y_MenuEntry *)V_UnicastTest}},
        {K_Leaf, 1, TRUE, " Setup",                           {(void*)(int)CLI_Unicast_Scenario2_Setup}},
        {K_Leaf, 2, TRUE, " RESET STATISTICS",                    {(void*)(int)CLI_Unicast_Scenario2_ResetStatistics}},
        {K_Leaf, 3, TRUE, " SHOW STATISTICS",                    {(void*)(int)CLI_Unicast_Scenario2_ShowStatistics}},
};

Y_MenuEntry V_UnicastTestScenario3[] = 
{
        {K_Menu, 3, TRUE, "Unicast Test Scenario #3",         {(Y_MenuEntry *)V_UnicastTest}},
        {K_Leaf, 1, TRUE, " Setup",                           {(void*)(int)CLI_Unicast_Scenario3_Setup}},
        {K_Leaf, 2, TRUE, " RESET STATISTICS",                    {(void*)(int)CLI_Unicast_Scenario3_ResetStatistics}},
        {K_Leaf, 3, TRUE, " SHOW STATISTICS",                    {(void*)(int)CLI_Unicast_Scenario3_ShowStatistics}},
};

Y_MenuEntry V_UnicastTestScenario4[] = 
{
        {K_Menu, 3, TRUE, "Unicast Test Scenario #4",         {(Y_MenuEntry *)V_UnicastTest}},
        {K_Leaf, 1, TRUE, " Setup",                           {(void*)(int)CLI_Unicast_Scenario4_Setup}},
        {K_Leaf, 2, TRUE, " RESET STATISTICS",                    {(void*)(int)CLI_Unicast_Scenario4_ResetStatistics}},
        {K_Leaf, 3, TRUE, " SHOW STATISTICS",                    {(void*)(int)CLI_Unicast_Scenario4_ShowStatistics}},
};

Y_MenuEntry V_UnicastTestScenario5[] = 
{
        {K_Menu, 3, TRUE, "Unicast Test Scenario #5",         {(Y_MenuEntry *)V_UnicastTest}},
        {K_Leaf, 1, TRUE, " Setup",                           {(void*)(int)CLI_Unicast_Scenario5_Setup}},
        {K_Leaf, 2, TRUE, " RESET STATISTICS",                    {(void*)(int)CLI_Unicast_Scenario5_ResetStatistics}},
        {K_Leaf, 3, TRUE, " SHOW STATISTICS",                    {(void*)(int)CLI_Unicast_Scenario5_ShowStatistics}},
};

Y_MenuEntry V_UnicastTestScenario6[] = 
{
        {K_Menu, 3, TRUE, "Unicast Test Scenario #6",         {(Y_MenuEntry *)V_UnicastTest}},
        {K_Leaf, 1, TRUE, " Setup",                           {(void*)(int)CLI_Unicast_Scenario6_Setup}},
        {K_Leaf, 2, TRUE, " RESET STATISTICS",                    {(void*)(int)CLI_Unicast_Scenario6_ResetStatistics}},
        {K_Leaf, 3, TRUE, " SHOW STATISTICS",                    {(void*)(int)CLI_Unicast_Scenario6_ShowStatistics}},
};

Y_MenuEntry V_MulticastTest[] = 
{
        {K_Menu, 5, TRUE, "Multicast Test",         {(Y_MenuEntry *)V_MainMenu}},
        {K_Menu, 1, TRUE, " -> Scenario #1",                      {(void*)(int)V_MulticastTestScenario1}},
        {K_Menu, 2, TRUE, " -> Scenario #2",                    {(void*)(int)V_MulticastTestScenario2}},
        {K_Menu, 3, TRUE, " -> Scenario #3",                    {(void*)(int)V_MulticastTestScenario3}},
        {K_Menu, 4, TRUE, " -> Scenario #4",                    {(void*)(int)V_MulticastTestScenario4}},
        {K_Menu, 5, TRUE, " -> Scenario #5",                    {(void*)(int)V_MulticastTestScenario5}},
};


Y_MenuEntry V_MulticastTestScenario1[] = 
{
        {K_Menu, 3, TRUE, "Multicast Test Scenario #1",         {(Y_MenuEntry *)V_MulticastTest}},
        {K_Leaf, 1, TRUE, " Setup",                           {(void*)(int)CLI_Multicast_Scenario1_Setup}},
        {K_Leaf, 2, TRUE, " RESET STATISTICS",                    {(void*)(int)CLI_Multicast_Scenario1_ResetStatistics}},
        {K_Leaf, 3, TRUE, " SHOW STATISTICS",                    {(void*)(int)CLI_Multicast_Scenario1_ShowStatistics}},
};

Y_MenuEntry V_MulticastTestScenario2[] = 
{
        {K_Menu, 3, TRUE, "Multicast Test Scenario #2",         {(Y_MenuEntry *)V_MulticastTest}},
        {K_Leaf, 1, TRUE, " Setup",                           {(void*)(int)CLI_Multicast_Scenario2_Setup}},
        {K_Leaf, 2, TRUE, " RESET STATISTICS",                    {(void*)(int)CLI_Multicast_Scenario2_ResetStatistics}},
        {K_Leaf, 3, TRUE, " SHOW STATISTICS",                    {(void*)(int)CLI_Multicast_Scenario2_ShowStatistics}},
};

Y_MenuEntry V_MulticastTestScenario3[] = 
{
        {K_Menu, 3, TRUE, "Multicast Test Scenario #3",         {(Y_MenuEntry *)V_MulticastTest}},
        {K_Leaf, 1, TRUE, " Setup",                           {(void*)(int)CLI_Multicast_Scenario3_Setup}},
        {K_Leaf, 2, TRUE, " RESET STATISTICS",                    {(void*)(int)CLI_Multicast_Scenario3_ResetStatistics}},
        {K_Leaf, 3, TRUE, " SHOW STATISTICS",                    {(void*)(int)CLI_Multicast_Scenario3_ShowStatistics}},
};

Y_MenuEntry V_MulticastTestScenario4[] = 
{
        {K_Menu, 3, TRUE, "Multicast Test Scenario #4",         {(Y_MenuEntry *)V_MulticastTest}},
        {K_Leaf, 1, TRUE, " Setup",                           {(void*)(int)CLI_Multicast_Scenario4_Setup}},
        {K_Leaf, 2, TRUE, " RESET STATISTICS",                    {(void*)(int)CLI_Multicast_Scenario4_ResetStatistics}},
        {K_Leaf, 3, TRUE, " SHOW STATISTICS",                    {(void*)(int)CLI_Multicast_Scenario4_ShowStatistics}},
};

Y_MenuEntry V_MulticastTestScenario5[] = 
{
        {K_Menu, 3, TRUE, "Multicast Test Scenario #5",         {(Y_MenuEntry *)V_MulticastTest}},
        {K_Leaf, 1, TRUE, " Setup",                           {(void*)(int)CLI_Multicast_Scenario5_Setup}},
        {K_Leaf, 2, TRUE, " RESET STATISTICS",                    {(void*)(int)CLI_Multicast_Scenario5_ResetStatistics}},
        {K_Leaf, 3, TRUE, " SHOW STATISTICS",                    {(void*)(int)CLI_Multicast_Scenario5_ShowStatistics}},
};






Y_MenuEntry V_HostTest[] = 
{
        {K_Menu, 9, TRUE, "Unicast Test",         {(Y_MenuEntry *)V_MainMenu}},
        {K_Menu, 1, TRUE, " -> Scenario #1",                    {(void*)(int)V_HostTestScenario1}},
        {K_Menu, 2, TRUE, " -> Scenario #2",                    {(void*)(int)V_HostTestScenario2}},
        {K_Menu, 3, TRUE, " -> Scenario #3",                    {(void*)(int)V_HostTestScenario3}},
        {K_Menu, 4, TRUE, " -> Scenario #4",                    {(void*)(int)V_HostTestScenario4}},
        {K_Menu, 5, TRUE, " -> Scenario #5",                    {(void*)(int)V_HostTestScenario5}},
        {K_Menu, 6, TRUE, " -> Scenario #6",                    {(void*)(int)V_HostTestScenario6}},
        {K_Menu, 7, TRUE, " -> Scenario #7",                    {(void*)(int)V_HostTestScenario7}},
        {K_Menu, 8, TRUE, " -> Scenario #8",                    {(void*)(int)V_HostTestScenario8}},
        {K_Menu, 9, TRUE, " -> Scenario #9",                    {(void*)(int)V_HostTestScenario9}},
};


Y_MenuEntry V_HostTestScenario1[] = 
{
        {K_Menu, 3, TRUE, "Host Test Scenario #1",         {(Y_MenuEntry *)V_HostTest}},
        {K_Leaf, 1, TRUE, " Setup",                           {(void*)(int)CLI_Host_Scenario1_Setup}},
        {K_Leaf, 2, TRUE, " RESET STATISTICS",                    {(void*)(int)CLI_Host_Scenario1_ResetStatistics}},
        {K_Leaf, 3, TRUE, " SHOW STATISTICS",                    {(void*)(int)CLI_Host_Scenario1_ShowStatistics}},
};

Y_MenuEntry V_HostTestScenario2[] = 
{
        {K_Menu, 3, TRUE, "Host Test Scenario #2",         {(Y_MenuEntry *)V_HostTest}},
        {K_Leaf, 1, TRUE, " Setup",                           {(void*)(int)CLI_Host_Scenario2_Setup}},
        {K_Leaf, 2, TRUE, " RESET STATISTICS",                    {(void*)(int)CLI_Host_Scenario2_ResetStatistics}},
        {K_Leaf, 3, TRUE, " SHOW STATISTICS",                    {(void*)(int)CLI_Host_Scenario2_ShowStatistics}},
};

Y_MenuEntry V_HostTestScenario3[] = 
{
        {K_Menu, 3, TRUE, "Host Test Scenario #3",         {(Y_MenuEntry *)V_HostTest}},
        {K_Leaf, 1, TRUE, " Setup",                           {(void*)(int)CLI_Host_Scenario3_Setup}},
        {K_Leaf, 2, TRUE, " RESET STATISTICS",                    {(void*)(int)CLI_Host_Scenario3_ResetStatistics}},
        {K_Leaf, 3, TRUE, " SHOW STATISTICS",                    {(void*)(int)CLI_Host_Scenario3_ShowStatistics}},
};

Y_MenuEntry V_HostTestScenario4[] = 
{
        {K_Menu, 3, TRUE, "Host Test Scenario #4",         {(Y_MenuEntry *)V_HostTest}},
        {K_Leaf, 1, TRUE, " Setup",                           {(void*)(int)CLI_Host_Scenario4_Setup}},
        {K_Leaf, 2, TRUE, " RESET STATISTICS",                    {(void*)(int)CLI_Host_Scenario4_ResetStatistics}},
        {K_Leaf, 3, TRUE, " SHOW STATISTICS",                    {(void*)(int)CLI_Host_Scenario4_ShowStatistics}},
};

Y_MenuEntry V_HostTestScenario5[] = 
{
        {K_Menu, 3, TRUE, "Host Test Scenario #5",         {(Y_MenuEntry *)V_HostTest}},
        {K_Leaf, 1, TRUE, " Setup",                           {(void*)(int)CLI_Host_Scenario5_Setup}},
        {K_Leaf, 2, TRUE, " RESET STATISTICS",                    {(void*)(int)CLI_Host_Scenario5_ResetStatistics}},
        {K_Leaf, 3, TRUE, " SHOW STATISTICS",                    {(void*)(int)CLI_Host_Scenario5_ShowStatistics}},
};

Y_MenuEntry V_HostTestScenario6[] = 
{
        {K_Menu, 3, TRUE, "Host Test Scenario #6",         {(Y_MenuEntry *)V_HostTest}},
        {K_Leaf, 1, TRUE, " Setup",                           {(void*)(int)CLI_Host_Scenario6_Setup}},
        {K_Leaf, 2, TRUE, " RESET STATISTICS",                    {(void*)(int)CLI_Host_Scenario6_ResetStatistics}},
        {K_Leaf, 3, TRUE, " SHOW STATISTICS",                    {(void*)(int)CLI_Host_Scenario6_ShowStatistics}},
};


Y_MenuEntry V_HostTestScenario7[] = 
{
        {K_Menu, 3, TRUE, "Host Test Scenario #7",         {(Y_MenuEntry *)V_HostTest}},
        {K_Leaf, 1, TRUE, " Setup",                           {(void*)(int)CLI_Host_Scenario7_Setup}},
        {K_Leaf, 2, TRUE, " RESET STATISTICS",                    {(void*)(int)CLI_Host_Scenario7_ResetStatistics}},
        {K_Leaf, 3, TRUE, " SHOW STATISTICS",                    {(void*)(int)CLI_Host_Scenario7_ShowStatistics}},
};


Y_MenuEntry V_HostTestScenario8[] = 
{
        {K_Menu, 3, TRUE, "Host Test Scenario #8",         {(Y_MenuEntry *)V_HostTest}},
        {K_Leaf, 1, TRUE, " Setup",                           {(void*)(int)CLI_Host_Scenario8_Setup}},
        {K_Leaf, 2, TRUE, " RESET STATISTICS",                    {(void*)(int)CLI_Host_Scenario8_ResetStatistics}},
        {K_Leaf, 3, TRUE, " SHOW STATISTICS",                    {(void*)(int)CLI_Host_Scenario8_ShowStatistics}},
};

Y_MenuEntry V_HostTestScenario9[] = 
{
        {K_Menu, 3, TRUE, "Host Test Scenario #9",         {(Y_MenuEntry *)V_HostTest}},
        {K_Leaf, 1, TRUE, " Setup",                           {(void*)(int)CLI_Host_Scenario9_Setup}},
        {K_Leaf, 2, TRUE, " RESET STATISTICS",                    {(void*)(int)CLI_Host_Scenario9_ResetStatistics}},
        {K_Leaf, 3, TRUE, " SHOW STATISTICS",                    {(void*)(int)CLI_Host_Scenario9_ShowStatistics}},
};

#define KEY_BACKSPACE           8
#define KEY_ENTER               10

void CLI_HostSendData(void)
{
    App_DataSend(tx_ge_enet_channel[0],adjunct_buffer_pool);
    return;
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

/*****************************************************************************
 * Function name: CLI_T_Main
 * Description  : cli main function, get input string and call menu engine.
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void CLI_T_Main(void)
{
        static char InputBuf[256];


        /* Display main menu */
        F_DisplayMenu();

        while (1)
        {
                printf("%s", STR_ENTER_COMMAND);
                InputBuf[0] = '\0';

                flush();

                gets(InputBuf);

                printf ("\n");
                F_MenuEngine(InputBuf);
        }

        return;
}



/***************************************************************
 * Func name  : WPE_Reboot
 * Description: Reboot board.
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Reboot(void)
{
#if !defined(__linux__)
/* Rebooting is specific to board and target.  The test
 * directory should not be concerned how this is done.
 * Instead, it calls a board-specific function. */
   if (WPI_REBOOT_ENABLE) 
   {
      printf("Test Rebooting winmon by WT_Reboot\nWinMon>\n");
      WPX_Reboot();
   }
   else
      printf("Not Rebooting with WT_Reboot\nWinMon>\n");
#endif
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
    WPE_Reboot();       
    return 0;
}

int CLI_MAIN_Quit(char *StrPrm)
{
    WP_DriverRelease();
    exit(0);
    return 0;
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

        int Cmd = 1;

        Y_MenuEntry *MenuEntryP;

        /* Is chosen command is in tolerance */
    
        if (1 == V_CurrMenuP->Index)
        {
                Cmd = 1;
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

extern void WPUI_RunWinMon(WP_CHAR *s);

int CLI_WinUtil(char *StrPrm)
{
        WP_CHAR InputBuf[255];
        
        printf ("Enter winutil command >\n");
        gets(InputBuf);
        printf("\n");
        WPUI_RunWinMon(InputBuf);

        return 0;
}





#if 0
extern void  WPE_UnicastFlowaggTestCfgInit(void);
extern void  WPE_MulticastFlowaggTestCfgInit(void);
extern void  WPE_HostFlowaggTestCfgInit(void);
#endif
extern void  WPE_TestCfgInit(void);
extern void WPE_ShowDeviceStatistics(WP_handle device_handle);
extern void WPE_ShowIwPortStatistics(WP_handle iw_port);
extern void WPE_DisplayGenericFlowAggStats(WP_handle agg);
extern WP_handle  dev_enet_ge;
extern WP_handle bport_enet_ge;

WP_handle PCE_rule_handle[2][100];


void CLI_ShowPortStatus(void)
{
    WP_status status;
    WP_enet_link_status_info portInfo;
    portInfo.winnet = NULL;
    status = WP_EnetPortInfoGet(0,port_enet_ge,dev_enet_ge,WP_PORT_ENET_LINK_STATUS_COMMAND,&portInfo);
    App_TerminateOnError(status, "WP_PortInfoGet",__LINE__);
    printf("#########Port %d Status###########\n",APP_ENET_PORT1);
    printf("interface_mode = %d\n",portInfo.interface_mode);
    printf("link_status = %d\n",portInfo.link_status);
    printf("link_speed = %d\n",portInfo.link_speed);
    printf("duplex_mode = %d\n",portInfo.duplex_mode);
    printf("an_enable = %d\n",portInfo.an_enable);
    printf("loopback_mode = %d\n",portInfo.loopback_mode); 
    printf("winnet_number = %d\n",portInfo.winnet_number);
    printf("winnet_group = %d\n",portInfo.winnet_group);
}


/******************unicast test Scenarios**********/


//Scenario 1
void CLI_Unicast_Scenario1_Setup(void)
{
    WP_pce_rule_classification rule_cfg = {0};
    WP_U32 flow_index;
    WPE_TestCfgInit();
    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;

    for(flow_index=0; flow_index < NUM_OF_GE_ENET_TX_CHANNELS; flow_index++)
    {
        rule_cfg.filter_handle = PCE_filter[0];

        rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
        rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
        
        rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
        rule_cfg.rule_fields[1].value.vlan_tag = flow_index + 1;

        rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


        rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
        rule_cfg.match_result[0].param.flow_agg.flow_aggregation = g999_enet_tx_flow_agg[flow_index];  
        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

        PCE_rule_handle[0][flow_index] = WP_PceRuleCreate(WP_WINPATH(0),
                                           WP_PCE_RULE_CLASSIFICATION,
                                           &rule_cfg);
        App_TerminateOnError(PCE_rule_handle[0][flow_index], "WP_PceRuleCreate",__LINE__);
    }


    for(flow_index=0; flow_index < NUM_OF_G999_ENET_TX_CHANNELS; flow_index++)
    {
        rule_cfg.filter_handle = PCE_filter[0];

        rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
        rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
        
        rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
        rule_cfg.rule_fields[1].value.vlan_tag = flow_index + 1;

        rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


        rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
        rule_cfg.match_result[0].param.flow_agg.flow_aggregation = ge_enet_tx_flow_agg[flow_index];  
        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

        PCE_rule_handle[0][flow_index+NUM_OF_GE_ENET_TX_CHANNELS] = WP_PceRuleCreate(WP_WINPATH(0),
                                           WP_PCE_RULE_CLASSIFICATION,
                                           &rule_cfg);
        App_TerminateOnError(PCE_rule_handle[0][flow_index+NUM_OF_GE_ENET_TX_CHANNELS], "WP_PceRuleCreate",__LINE__);
    }

}

void CLI_Unicast_Scenario1_ResetStatistics(void)
{
    WP_U32 flow_index;
    WP_iw_generic_flow_stats flow_stats = {0};
    WP_stats_enet stats_enet = {0};
    WP_bport_stats bport_stats = {0};
    WP_pce_filter_stat filter_stats = {0};
    for(flow_index=0; flow_index < NUM_OF_G999_ENET_TX_CHANNELS; flow_index++)
    {
        WP_IwFlowStatisticsReset(g999_enet_tx_flow_agg[flow_index], WP_IW_FLOW_STAT_GENERIC, &flow_stats);
    }
    for(flow_index=0; flow_index < NUM_OF_GE_ENET_TX_CHANNELS; flow_index++)
    {
        WP_IwFlowStatisticsReset(ge_enet_tx_flow_agg[flow_index], WP_IW_FLOW_STAT_GENERIC, &flow_stats);
    }
}

void CLI_Unicast_Scenario1_ShowStatistics(void)
{
    WP_U32 flow_index;
    
    for(flow_index=0; flow_index < NUM_OF_G999_ENET_TX_CHANNELS; flow_index++)
    {
        WPE_DisplayGenericFlowAggStats(g999_enet_tx_flow_agg[flow_index]);        
    }

    for(flow_index=0; flow_index < NUM_OF_GE_ENET_TX_CHANNELS; flow_index++)
    {
        WPE_DisplayGenericFlowAggStats(ge_enet_tx_flow_agg[flow_index]); 
    }
    WPE_ShowDeviceStatistics(dev_enet_ge);
}
//Scenario 2
void CLI_Unicast_Scenario2_Setup(void)
{
    WP_pce_rule_classification rule_cfg = {0};
    WPE_TestCfgInit();
    WPE_V2PolicerCreate_1(20000000,20000000);
    WPE_V2PolicerCreate_2(10000000,10000000);
    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;

//rule 1
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = g999_enet_tx_flow_agg[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER;
    rule_cfg.match_result[1].param.ingress_policer.policer = policer_handle_v2[0];
    rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][0] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][0], "WP_PceRuleCreate",__LINE__);

//rule 2
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = ge_enet_tx_flow_agg[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER;
    rule_cfg.match_result[1].param.ingress_policer.policer = policer_handle_v2[1];
    rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][1] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][1], "WP_PceRuleCreate",__LINE__);

}

void CLI_Unicast_Scenario2_ResetStatistics(void)
{
    WP_iw_generic_flow_stats flow_stats = {0};
    WP_IwFlowStatisticsReset(g999_enet_tx_flow_agg[0], WP_IW_FLOW_STAT_GENERIC, &flow_stats);
    WP_IwFlowStatisticsReset(ge_enet_tx_flow_agg[0], WP_IW_FLOW_STAT_GENERIC, &flow_stats);    
}

void CLI_Unicast_Scenario2_ShowStatistics(void)
{
    WPE_DisplayGenericFlowAggStats(g999_enet_tx_flow_agg[0]); 
    WPE_DisplayGenericFlowAggStats(ge_enet_tx_flow_agg[0]);
}

//Scenario 3
void CLI_Unicast_Scenario3_Setup(void)
{
    WP_status status;
    WP_pce_rule_classification rule_cfg = {0};
    WPE_TestCfgInit();
    WPE_V2PolicerCreate_1(20000000,20000000);
    WPE_V2PolicerCreate_2(10000000,10000000);
    WP_iw_agg_generic agg_enet_config =
    {
        /*tag*/ agg_tag_counter++,
        /*txfunc*/ 0,
        /*iw_port*/0,
        /*rfcs*/WP_IW_RFCS_ENABLE,
        /*interruptqueue;*/WP_IW_IRQT1,
        /*error_pkt_mode*/WP_IW_ERRPKT_DISCARD,
        /*intmode;*/WP_IW_INT_DISABLE,
        /*statmode;*/WP_IW_STAT_ENABLE,
        /*timestamp_mode;*/WP_IW_TIME_STAMP_DISABLE,
        /*mtu;*/9216,
        /*flow_agg_type;*/WP_IW_FLOW_AGG_PRIMARY,
        /*policer_handle;*/0,
        /*pecs_handle;*/0,
        /*pecs_flow_info;*/0,
        /*pecs_global_info_handle;*/0,
        /*iw_gpmt_handle*/0,
    };
    agg_enet_config.policer_handle = policer_handle_v2[0];
    status = WP_IwFlowAggregationModify(g999_enet_tx_flow_agg[0], 
                                       WP_IW_FLOW_AGG_GENERIC_MOD_POLICER,
                                       &agg_enet_config);
    App_TerminateOnError(status, "WP_IwFlowAggregationModify",__LINE__);

    agg_enet_config.policer_handle = policer_handle_v2[1];
    status = WP_IwFlowAggregationModify(ge_enet_tx_flow_agg[0], 
                                       WP_IW_FLOW_AGG_GENERIC_MOD_POLICER,
                                       &agg_enet_config);
    App_TerminateOnError(status, "WP_IwFlowAggregationModify",__LINE__);

//config rules
    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;

//rule 1
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag =  1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = g999_enet_tx_flow_agg[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][0] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][0], "WP_PceRuleCreate",__LINE__);

//rule 2
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag =  1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = ge_enet_tx_flow_agg[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][1] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][1], "WP_PceRuleCreate",__LINE__);



}

void CLI_Unicast_Scenario3_ResetStatistics(void)
{
    WP_iw_generic_flow_stats flow_stats = {0};
    WP_IwFlowStatisticsReset(g999_enet_tx_flow_agg[0], WP_IW_FLOW_STAT_GENERIC, &flow_stats);
    WP_IwFlowStatisticsReset(ge_enet_tx_flow_agg[0], WP_IW_FLOW_STAT_GENERIC, &flow_stats);    
}

void CLI_Unicast_Scenario3_ShowStatistics(void)
{
    WPE_DisplayGenericFlowAggStats(g999_enet_tx_flow_agg[0]); 
    WPE_DisplayGenericFlowAggStats(ge_enet_tx_flow_agg[0]);
}

//Scenario 4
void CLI_Unicast_Scenario4_Setup(void)
{
    WP_pce_rule_classification rule_cfg = {0};
    WP_U32 flow_index;
    WPE_TestCfgInit();
    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;

    for(flow_index=0; flow_index < NUM_OF_GE_ENET_TX_CHANNELS; flow_index++)
    {
        rule_cfg.filter_handle = PCE_filter[0];

        rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
        rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
        
        rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
        rule_cfg.rule_fields[1].value.vlan_tag = flow_index + 1;

        rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


        rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
        rule_cfg.match_result[0].param.flow_agg.flow_aggregation = g999_enet_tx_flow_agg[flow_index];  
        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

        PCE_rule_handle[0][flow_index] = WP_PceRuleCreate(WP_WINPATH(0),
                                           WP_PCE_RULE_CLASSIFICATION,
                                           &rule_cfg);
        App_TerminateOnError(PCE_rule_handle[0][flow_index], "WP_PceRuleCreate",__LINE__);
}


    for(flow_index=0; flow_index < NUM_OF_G999_ENET_TX_CHANNELS; flow_index++)
    {
        rule_cfg.filter_handle = PCE_filter[0];

        rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
        rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
        
        rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
        rule_cfg.rule_fields[1].value.vlan_tag = flow_index + 1;

        rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


        rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
        rule_cfg.match_result[0].param.flow_agg.flow_aggregation = ge_enet_tx_flow_agg[flow_index];  
        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

        PCE_rule_handle[0][flow_index+NUM_OF_GE_ENET_TX_CHANNELS] = WP_PceRuleCreate(WP_WINPATH(0),
                                           WP_PCE_RULE_CLASSIFICATION,
                                           &rule_cfg);
        App_TerminateOnError(PCE_rule_handle[0][flow_index+NUM_OF_GE_ENET_TX_CHANNELS], "WP_PceRuleCreate",__LINE__);
    }

//set rule to deny SA mac == 0x01 on ge

        rule_cfg.filter_handle = PCE_filter[1];

        rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
        rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
        
        rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_SA;
        rule_cfg.rule_fields[1].value.mac_addr[5] = 0x1;
        rule_cfg.rule_fields[1].value.mac_addr[4] = 0x0;
        rule_cfg.rule_fields[1].value.mac_addr[3] = 0x0;
        rule_cfg.rule_fields[1].value.mac_addr[2] = 0x0;
        rule_cfg.rule_fields[1].value.mac_addr[1] = 0x0;
        rule_cfg.rule_fields[1].value.mac_addr[0] = 0x0;

        rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


        rule_cfg.match_action = WP_PCE_RULE_MATCH_DENY;

        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[1][0] = WP_PceRuleCreate(WP_WINPATH(0),
                                           WP_PCE_RULE_CLASSIFICATION,
                                           &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[1][0], "WP_PceRuleCreate",__LINE__);


//set rule to deny SA mac == 0x02 on g999
        rule_cfg.filter_handle = PCE_filter[1];

        rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
        rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
        
        rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_SA;
        rule_cfg.rule_fields[1].value.mac_addr[5] = 0x2;
        rule_cfg.rule_fields[1].value.mac_addr[4] = 0x0;
        rule_cfg.rule_fields[1].value.mac_addr[3] = 0x0;
        rule_cfg.rule_fields[1].value.mac_addr[2] = 0x0;
        rule_cfg.rule_fields[1].value.mac_addr[1] = 0x0;
        rule_cfg.rule_fields[1].value.mac_addr[0] = 0x0;

        rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


        rule_cfg.match_action = WP_PCE_RULE_MATCH_DENY;

        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[1][1] = WP_PceRuleCreate(WP_WINPATH(0),
                                           WP_PCE_RULE_CLASSIFICATION,
                                           &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[1][1], "WP_PceRuleCreate",__LINE__);
    

}

void CLI_Unicast_Scenario4_ResetStatistics(void)
{
    WP_U32 flow_index;
    WP_iw_generic_flow_stats flow_stats = {0};
    WP_stats_enet stats_enet = {0};
    WP_bport_stats bport_stats = {0};
    WP_pce_filter_stat filter_stats = {0};
    WP_pce_filter_stat pce_filter_stats = {0};
    for(flow_index=0; flow_index < NUM_OF_G999_ENET_TX_CHANNELS; flow_index++)
    {
        WP_IwFlowStatisticsReset(g999_enet_tx_flow_agg[flow_index], WP_IW_FLOW_STAT_GENERIC, &flow_stats);
    }
    for(flow_index=0; flow_index < NUM_OF_GE_ENET_TX_CHANNELS; flow_index++)
    {
        WP_IwFlowStatisticsReset(ge_enet_tx_flow_agg[flow_index], WP_IW_FLOW_STAT_GENERIC, &flow_stats);
    }
    WP_PceFilterStatisticsReset(PCE_filter[1],&pce_filter_stats);
}

void CLI_Unicast_Scenario4_ShowStatistics(void)
{
    WP_U32 flow_index;
    
    for(flow_index=0; flow_index < NUM_OF_G999_ENET_TX_CHANNELS; flow_index++)
    {
        WPE_DisplayGenericFlowAggStats(g999_enet_tx_flow_agg[flow_index]);        
    }

    for(flow_index=0; flow_index < NUM_OF_GE_ENET_TX_CHANNELS; flow_index++)
    {
        WPE_DisplayGenericFlowAggStats(ge_enet_tx_flow_agg[flow_index]); 
    }
    WPE_DisplayPCEStats(PCE_filter[1]);
    
}

//Scenario 5
void CLI_Unicast_Scenario5_Setup(void)
{
    WP_pce_rule_classification rule_cfg = {0};
    WPE_TestCfgInit();
    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;

//rule 1
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = g999_enet_tx_flow_agg[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][0] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][0], "WP_PceRuleCreate",__LINE__);

//rule 2
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = ge_enet_tx_flow_agg[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][1] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][1], "WP_PceRuleCreate",__LINE__);

}

void CLI_Unicast_Scenario5_ResetStatistics(void)
{
    WP_iw_generic_flow_stats flow_stats = {0};
    WP_IwFlowStatisticsReset(g999_enet_tx_flow_agg[0], WP_IW_FLOW_STAT_GENERIC, &flow_stats);
    WP_IwFlowStatisticsReset(ge_enet_tx_flow_agg[0], WP_IW_FLOW_STAT_GENERIC, &flow_stats);    
}

void CLI_Unicast_Scenario5_ShowStatistics(void)
{
    WPE_DisplayGenericFlowAggStats(g999_enet_tx_flow_agg[0]); 
    WPE_DisplayGenericFlowAggStats(ge_enet_tx_flow_agg[0]);
}


//Scenario 6
void CLI_Unicast_Scenario6_Setup(void)
{
    WP_pce_rule_classification rule_cfg = {0};
    WP_U32 flow_index;
    WPE_TestCfgInitwithPecs();
    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;

    for(flow_index=0; flow_index < NUM_OF_GE_ENET_TX_CHANNELS; flow_index++)
    {
        rule_cfg.filter_handle = PCE_filter[0];

        rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
        rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
        
        rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
        rule_cfg.rule_fields[1].value.vlan_tag = flow_index + 1;

        rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


        rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
        rule_cfg.match_result[0].param.flow_agg.flow_aggregation = g999_enet_tx_flow_agg[flow_index];  
        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

        PCE_rule_handle[0][flow_index] = WP_PceRuleCreate(WP_WINPATH(0),
                                           WP_PCE_RULE_CLASSIFICATION,
                                           &rule_cfg);
        App_TerminateOnError(PCE_rule_handle[0][flow_index], "WP_PceRuleCreate",__LINE__);
    }


    for(flow_index=0; flow_index < NUM_OF_G999_ENET_TX_CHANNELS; flow_index++)
    {
        rule_cfg.filter_handle = PCE_filter[0];

        rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
        rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
        
        rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
        rule_cfg.rule_fields[1].value.vlan_tag = flow_index + 1;

        rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


        rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
        rule_cfg.match_result[0].param.flow_agg.flow_aggregation = ge_enet_tx_flow_agg[flow_index];  
        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

        PCE_rule_handle[0][flow_index+NUM_OF_GE_ENET_TX_CHANNELS] = WP_PceRuleCreate(WP_WINPATH(0),
                                           WP_PCE_RULE_CLASSIFICATION,
                                           &rule_cfg);
        App_TerminateOnError(PCE_rule_handle[0][flow_index+NUM_OF_GE_ENET_TX_CHANNELS], "WP_PceRuleCreate",__LINE__);
    }

}

void CLI_Unicast_Scenario6_ResetStatistics(void)
{
    WP_U32 flow_index;
    WP_iw_generic_flow_stats flow_stats = {0};
    WP_stats_enet stats_enet = {0};
    WP_bport_stats bport_stats = {0};
    WP_pce_filter_stat filter_stats = {0};
    for(flow_index=0; flow_index < NUM_OF_G999_ENET_TX_CHANNELS; flow_index++)
    {
        WP_IwFlowStatisticsReset(g999_enet_tx_flow_agg[flow_index], WP_IW_FLOW_STAT_GENERIC, &flow_stats);
    }
    for(flow_index=0; flow_index < NUM_OF_GE_ENET_TX_CHANNELS; flow_index++)
    {
        WP_IwFlowStatisticsReset(ge_enet_tx_flow_agg[flow_index], WP_IW_FLOW_STAT_GENERIC, &flow_stats);
    }
}

void CLI_Unicast_Scenario6_ShowStatistics(void)
{
    WP_U32 flow_index;
    
    for(flow_index=0; flow_index < NUM_OF_G999_ENET_TX_CHANNELS; flow_index++)
    {
        WPE_DisplayGenericFlowAggStats(g999_enet_tx_flow_agg[flow_index]);        
    }

    for(flow_index=0; flow_index < NUM_OF_GE_ENET_TX_CHANNELS; flow_index++)
    {
        WPE_DisplayGenericFlowAggStats(ge_enet_tx_flow_agg[flow_index]); 
    }
}



/***********multicast test********/

//Scenario 1
void CLI_Multicast_Scenario1_Setup(void)
{
    WP_U32 entry = 0;
    WPE_TestCfgInit();
    WPE_MulticastInit();

    //create rules

    WP_pce_rule_classification rule_cfg = {0};
    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;

//rule 1
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = mc_flowagg_handle[1];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][0] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][0], "WP_PceRuleCreate",__LINE__);

//rule 2
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 2;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = g999_enet_tx_flow_agg[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][1] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][1], "WP_PceRuleCreate",__LINE__);
 
//rule 3
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 2;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = mc_flowagg_handle[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][2] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][1], "WP_PceRuleCreate",__LINE__);

}

void CLI_Multicast_Scenario1_ResetStatistics(void)
{
    WP_U32 flow_index;
    WP_iw_generic_flow_stats flow_stats = {0};
    WP_iw_flow_stats stats = {0};
    WP_iw_mc_member_enhanced_stats member_stats = {0};

    for(flow_index=0; flow_index < NUM_OF_G999_ENET_TX_CHANNELS; flow_index++)
    {
        WP_IwFlowStatisticsReset(g999_enet_tx_flow_agg[flow_index], WP_IW_FLOW_STAT_GENERIC, &flow_stats);
    }
    for(flow_index=0; flow_index < NUM_OF_GE_ENET_TX_CHANNELS; flow_index++)
    {
        WP_IwFlowStatisticsReset(ge_enet_tx_flow_agg[flow_index], WP_IW_FLOW_STAT_GENERIC, &flow_stats);
    }
    for(flow_index=0; flow_index < 2; flow_index++)
    {
        WP_IwFlowStatisticsReset(mc_flowagg_handle[flow_index],WP_IW_FLOW_STAT,&stats);
    }

    for(flow_index=0; flow_index < NUM_OF_GE_ENET_TX_CHANNELS; flow_index++)
        WP_IwMcMemberStatisticsReset(group_handle[0],member_handle[flow_index],&member_stats);
    for(flow_index=0; flow_index < NUM_OF_G999_ENET_TX_CHANNELS; flow_index++)
        WP_IwMcMemberStatisticsReset(group_handle[1],member_handle[flow_index+8],&member_stats);

    WP_IwFlowStatisticsReset(agg_host,WP_IW_FLOW_STAT,&stats);
}

void CLI_Multicast_Scenario1_ShowStatistics(void)
{
    WP_U32 flow_index;

    for(flow_index=0; flow_index < NUM_OF_G999_ENET_TX_CHANNELS; flow_index++)
    {
        WPE_DisplayGenericFlowAggStats(g999_enet_tx_flow_agg[flow_index]);        
}

    for(flow_index=0; flow_index < NUM_OF_GE_ENET_TX_CHANNELS; flow_index++)
    {
        WPE_DisplayGenericFlowAggStats(ge_enet_tx_flow_agg[flow_index]); 
    }
    for(flow_index=0; flow_index < 2; flow_index++)
{
        WPE_DisplayFlowAggStats(mc_flowagg_handle[flow_index]);
    }
    for(flow_index=0; flow_index < NUM_OF_GE_ENET_TX_CHANNELS; flow_index++)
        WPE_DisplayMcMemberStats(group_handle[0],member_handle[flow_index]);
    for(flow_index=0; flow_index < NUM_OF_G999_ENET_TX_CHANNELS; flow_index++)
        WPE_DisplayMcMemberStats(group_handle[1],member_handle[flow_index+8]);        

    WPE_DisplayFlowAggStats(agg_host);
}
//Scenario 2
void CLI_Multicast_Scenario2_Setup(void)
{
    WP_U32 entry = 0;
    WPE_TestCfgInit();
    WPE_MulticastInit();
    WP_iw_mc_member mc_member_config =
   {
      /* txfunc */0,
      /* service_class_index */0,
      /* buffers_threshold */0,
      /* iw_port */0,
      /* edit_size */0x20,
      /* data_copy_mode */WP_IW_MC_MEMBER_DATACOPY_DISABLE,
      /*iw_port_check_mode*/WP_IW_MC_MEMBER_IWPORT_CHECK_ENABLE
   };
    mc_member_config.txfunc = g999_enet_tx_flow_agg[0];
    mc_member_config.iw_port = bport_enet_g999; 
    member_handle[16] = WP_IwMcMemberAdd(
    group_handle[0], &mc_member_config);
    App_TerminateOnError(member_handle[16],"WP_IwMcMemberAdd() mc member group 1", __LINE__);

    mc_member_config.txfunc = ge_enet_tx_flow_agg[0];
    mc_member_config.iw_port = bport_enet_ge; 
    member_handle[17] = WP_IwMcMemberAdd(
    group_handle[1], &mc_member_config);
    App_TerminateOnError(member_handle[17],"WP_IwMcMemberAdd() mc member group 2", __LINE__);

    //create rules

    WP_pce_rule_classification rule_cfg = {0};
    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;

//rule 1
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = mc_flowagg_handle[1];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][0] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][0], "WP_PceRuleCreate",__LINE__);
 
//rule 2
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = mc_flowagg_handle[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][1] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][1], "WP_PceRuleCreate",__LINE__);

}

void CLI_Multicast_Scenario2_ResetStatistics(void)
{
    WP_U32 flow_index;
    WP_iw_generic_flow_stats flow_stats = {0};
    WP_iw_flow_stats stats = {0};
    WP_iw_mc_member_enhanced_stats member_stats = {0};

    for(flow_index=0; flow_index < NUM_OF_G999_ENET_TX_CHANNELS; flow_index++)
    {
        WP_IwFlowStatisticsReset(g999_enet_tx_flow_agg[flow_index], WP_IW_FLOW_STAT_GENERIC, &flow_stats);
    }
    for(flow_index=0; flow_index < NUM_OF_GE_ENET_TX_CHANNELS; flow_index++)
    {
        WP_IwFlowStatisticsReset(ge_enet_tx_flow_agg[flow_index], WP_IW_FLOW_STAT_GENERIC, &flow_stats);
    }
    for(flow_index=0; flow_index < 2; flow_index++)
    {
        WP_IwFlowStatisticsReset(mc_flowagg_handle[flow_index],WP_IW_FLOW_STAT,&stats);
    }
    for(flow_index=0; flow_index < NUM_OF_GE_ENET_TX_CHANNELS; flow_index++)
        WP_IwMcMemberStatisticsReset(group_handle[0],member_handle[flow_index],&member_stats);

    WP_IwMcMemberStatisticsReset(group_handle[0],member_handle[17],&member_stats);

    for(flow_index=0; flow_index < NUM_OF_G999_ENET_TX_CHANNELS; flow_index++)
        WP_IwMcMemberStatisticsReset(group_handle[1],member_handle[flow_index+8],&member_stats); 

    WP_IwMcMemberStatisticsReset(group_handle[0],member_handle[16],&member_stats);

}

void CLI_Multicast_Scenario2_ShowStatistics(void)
{
    WP_U32 flow_index;
    
   for(flow_index=0; flow_index < NUM_OF_G999_ENET_TX_CHANNELS; flow_index++)
    {
        WPE_DisplayGenericFlowAggStats(g999_enet_tx_flow_agg[flow_index]);        
    }

    for(flow_index=0; flow_index < NUM_OF_GE_ENET_TX_CHANNELS; flow_index++)
    {
        WPE_DisplayGenericFlowAggStats(ge_enet_tx_flow_agg[flow_index]); 
    }
    for(flow_index=0; flow_index < 2; flow_index++)
    {
        WPE_DisplayFlowAggStats(mc_flowagg_handle[flow_index]);
    }
    for(flow_index=0; flow_index < NUM_OF_GE_ENET_TX_CHANNELS; flow_index++)
        WPE_DisplayMcMemberStats(group_handle[0],member_handle[flow_index]);
    
    WPE_DisplayMcMemberStats(group_handle[0],member_handle[17]);
    
    for(flow_index=0; flow_index < NUM_OF_G999_ENET_TX_CHANNELS; flow_index++)
        WPE_DisplayMcMemberStats(group_handle[1],member_handle[flow_index+8]);        
    
    WPE_DisplayMcMemberStats(group_handle[0],member_handle[16]);
}

//Scenario 3
void CLI_Multicast_Scenario3_Setup(void)
{
    WP_U32 entry = 0;
    WPE_TestCfgInit();
    WPE_V2PolicerCreate_1(10000000,5000000);
    WPE_V2PolicerCreate_2(5000000,5000000);
    WP_iw_mc_group mc_group_config =
    {
       
      /* type */WP_IW_MC_UNRESTRICTED,
      /* mode */WP_IW_MC_MODE_MEMBERS_STAT_ENABLE | WP_IW_MC_MODE_GENERIC_GROUP | WP_IW_MC_MODE_MEMBERS_ENHANCED_STAT_ENABLE,
      /* max_iw_mc_members */NUM_OF_MC_MEMBERS_PER_GROUP,
      /* qnode */0,
      /* encaps_gap */64,
      /* classes_config */NULL,
      /* identifier */WP_UNUSED
       
    };


    WP_iw_agg_multicast mc_agg_config =

   {
      /* tag */0,
      /* type */WP_IW_FLOW_AGG_PRIMARY,
      /* mc_group */0,
      /* mtu */1500,
      /* intmode */WP_IW_INT_DISABLE,
      /* interruptqueue */WP_IW_IRQT1,
      /* statmode */WP_IW_STAT_ENABLE,
      /* timestamp_mode */WP_IW_TIME_STAMP_DISABLE,
      /* ov_pool_mode */WP_IW_OV_POOL_ENABLE,
      /* fbp_drop_threshold */0,
      /* policer_enable */WP_IW_POLICER_DISABLE,
      /* *policer_config */NULL
   };

    WP_iw_mc_member mc_member_config =
   {
      /* txfunc */0,
      /* service_class_index */0,
      /* buffers_threshold */0,
      /* iw_port */0,
      /* edit_size */0x20,
      /* data_copy_mode */WP_IW_MC_MEMBER_DATACOPY_DISABLE,
      /*iw_port_check_mode*/WP_IW_MC_MEMBER_IWPORT_CHECK_ENABLE
   };


 //create mc group 1 piont to ge  
    mc_group_config.qnode = qnode_mc;

    group_handle[0] = WP_IwMcGroupCreate(WP_WINPATH(0), &mc_group_config);
    App_TerminateOnError(group_handle[0],"WP_IwMcGroupCreate() Mc group 1", __LINE__);

    /* Create multicast flow agg group 1*/
    mc_agg_config.mc_group = group_handle[0];
    mc_flowagg_handle[0] = WP_IwFlowAggregationCreate(WP_WINPATH(0),WP_IW_GENERIC_MC_MODE,&mc_agg_config);
    App_TerminateOnError(mc_flowagg_handle[0],"WP_IwFlowAggregationCreate() mc agg 1", __LINE__);

    /* Create Members for the mc Group */
    for(entry=0; entry<4; entry ++)
    {
         mc_member_config.txfunc = ge_enet_tx_flow_agg[entry];//enet tx flow agg
         mc_member_config.iw_port = bport_enet_ge; //output iw port connected to the flow agg which point to tx_ge_enet_channel[entry]
         member_handle[entry] = WP_IwMcMemberAdd(
            group_handle[0], &mc_member_config);
         App_TerminateOnError(member_handle[entry],"WP_IwMcMemberAdd() mc member group 1", __LINE__);
    }

/***create multicast group 2 piont to g999*****/

    group_handle[1] = WP_IwMcGroupCreate(WP_WINPATH(0), &mc_group_config);
    App_TerminateOnError(group_handle[1],"WP_IwMcGroupCreate() Mc group 2", __LINE__);

    /* Create broadcast flow agg using the mc group handle.*/
    mc_agg_config.mc_group = group_handle[1];
    mc_flowagg_handle[1] = WP_IwFlowAggregationCreate(WP_WINPATH(0),WP_IW_GENERIC_MC_MODE,&mc_agg_config);
    App_TerminateOnError(mc_flowagg_handle[1],"WP_IwFlowAggregationCreate() mc agg 2", __LINE__);

    /* Create Members for the mc Group */
    for(entry=0; entry<4; entry ++)
    {
         mc_member_config.txfunc = g999_enet_tx_flow_agg[entry];//enet tx flow agg
         mc_member_config.iw_port = bport_enet_g999; //output iw port connected to the flow agg which point to enet_tx_channel[entry]
         member_handle[entry+8] = WP_IwMcMemberAdd(
            group_handle[1], &mc_member_config);
         App_TerminateOnError(member_handle[entry+8],"WP_IwMcMemberAdd() mc member group 2", __LINE__);
    }


    //create rules

    WP_pce_rule_classification rule_cfg = {0};
    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;

//rule 1
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = mc_flowagg_handle[1];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER;
    rule_cfg.match_result[1].param.ingress_policer.policer = policer_handle_v2[0];
    rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][0] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][0], "WP_PceRuleCreate",__LINE__);

//rule 2
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = mc_flowagg_handle[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER;
    rule_cfg.match_result[1].param.ingress_policer.policer = policer_handle_v2[1];
    rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][1] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][1], "WP_PceRuleCreate",__LINE__);

}

void CLI_Multicast_Scenario3_ResetStatistics(void)
{
    WP_U32 flow_index;
    WP_iw_generic_flow_stats flow_stats = {0};
    WP_iw_flow_stats stats = {0};
    WP_iw_mc_member_enhanced_stats member_stats = {0};
    for(flow_index=0; flow_index < 4; flow_index++)
    {
        WP_IwFlowStatisticsReset(g999_enet_tx_flow_agg[flow_index], WP_IW_FLOW_STAT_GENERIC, &flow_stats);
    }
    for(flow_index=0; flow_index < 4; flow_index++)
    {
        WP_IwFlowStatisticsReset(ge_enet_tx_flow_agg[flow_index], WP_IW_FLOW_STAT_GENERIC, &flow_stats);
    }
    for(flow_index=0; flow_index < 2; flow_index++)
    {
        WP_IwFlowStatisticsReset(mc_flowagg_handle[flow_index],WP_IW_FLOW_STAT,&stats);
    }

    for(flow_index=0; flow_index < 4; flow_index++)
        WP_IwMcMemberStatisticsReset(group_handle[0],member_handle[flow_index],&member_stats);


    for(flow_index=0; flow_index < 4; flow_index++)
        WP_IwMcMemberStatisticsReset(group_handle[1],member_handle[flow_index+8],&member_stats); 

}

void CLI_Multicast_Scenario3_ShowStatistics(void)
{
    WP_U32 flow_index;
    
    for(flow_index=0; flow_index < 4; flow_index++)
    {
        WPE_DisplayGenericFlowAggStats(g999_enet_tx_flow_agg[flow_index]);        
    }

    for(flow_index=0; flow_index < 4; flow_index++)
    {
        WPE_DisplayGenericFlowAggStats(ge_enet_tx_flow_agg[flow_index]); 
    }
    for(flow_index=0; flow_index < 2; flow_index++)
    {
        WPE_DisplayFlowAggStats(mc_flowagg_handle[flow_index]);
    }
    for(flow_index=0; flow_index < 4; flow_index++)
        WPE_DisplayMcMemberStats(group_handle[0],member_handle[flow_index]);
    
    
    for(flow_index=0; flow_index < 4; flow_index++)
        WPE_DisplayMcMemberStats(group_handle[1],member_handle[flow_index+8]); 

}

//Scenario 4
void CLI_Multicast_Scenario4_Setup(void)
{
    WP_U32 entry = 0;
    WPE_TestCfgInit();
    WPE_MulticastInit();


    //create rules

    WP_pce_rule_classification rule_cfg = {0};
    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;

    //rule 1
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = mc_flowagg_handle[1];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][0] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][0], "WP_PceRuleCreate",__LINE__);
 
//rule 2
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = mc_flowagg_handle[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][1] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][1], "WP_PceRuleCreate",__LINE__);

//set rule to deny SA mac == 0x01 on ge

    rule_cfg.filter_handle = PCE_filter[1];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_SA;
    rule_cfg.rule_fields[1].value.mac_addr[5] = 0x1;
    rule_cfg.rule_fields[1].value.mac_addr[4] = 0x0;
    rule_cfg.rule_fields[1].value.mac_addr[3] = 0x0;
    rule_cfg.rule_fields[1].value.mac_addr[2] = 0x0;
    rule_cfg.rule_fields[1].value.mac_addr[1] = 0x0;
    rule_cfg.rule_fields[1].value.mac_addr[0] = 0x0;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_DENY;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[1][0] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[1][0], "WP_PceRuleCreate",__LINE__);


//set rule to deny SA mac == 0x02 on g999
    rule_cfg.filter_handle = PCE_filter[1];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_SA;
    rule_cfg.rule_fields[1].value.mac_addr[5] = 0x2;
    rule_cfg.rule_fields[1].value.mac_addr[4] = 0x0;
    rule_cfg.rule_fields[1].value.mac_addr[3] = 0x0;
    rule_cfg.rule_fields[1].value.mac_addr[2] = 0x0;
    rule_cfg.rule_fields[1].value.mac_addr[1] = 0x0;
    rule_cfg.rule_fields[1].value.mac_addr[0] = 0x0;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_DENY;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[1][1] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[1][1], "WP_PceRuleCreate",__LINE__);



}

void CLI_Multicast_Scenario4_ResetStatistics(void)
{
    WP_U32 flow_index;
    WP_iw_generic_flow_stats flow_stats = {0};
    WP_iw_flow_stats stats = {0};
    WP_iw_mc_member_enhanced_stats member_stats = {0};
    for(flow_index=0; flow_index < 8; flow_index++)
    {
        WP_IwFlowStatisticsReset(g999_enet_tx_flow_agg[flow_index], WP_IW_FLOW_STAT_GENERIC, &flow_stats);
    }
    for(flow_index=0; flow_index < 8; flow_index++)
    {
        WP_IwFlowStatisticsReset(ge_enet_tx_flow_agg[flow_index], WP_IW_FLOW_STAT_GENERIC, &flow_stats);
    }
    for(flow_index=0; flow_index < 2; flow_index++)
    {
        WP_IwFlowStatisticsReset(mc_flowagg_handle[flow_index],WP_IW_FLOW_STAT,&stats);
    }

    for(flow_index=0; flow_index < 8; flow_index++)
        WP_IwMcMemberStatisticsReset(group_handle[0],member_handle[flow_index],&member_stats);


    for(flow_index=0; flow_index < 8; flow_index++)
        WP_IwMcMemberStatisticsReset(group_handle[1],member_handle[flow_index+8],&member_stats); 

}

void CLI_Multicast_Scenario4_ShowStatistics(void)
{
    WP_U32 flow_index;
    
    for(flow_index=0; flow_index < 8; flow_index++)
    {
        WPE_DisplayGenericFlowAggStats(g999_enet_tx_flow_agg[flow_index]);        
    }

    for(flow_index=0; flow_index < 8; flow_index++)
    {
        WPE_DisplayGenericFlowAggStats(ge_enet_tx_flow_agg[flow_index]); 
    }
    for(flow_index=0; flow_index < 2; flow_index++)
    {
        WPE_DisplayFlowAggStats(mc_flowagg_handle[flow_index]);
    }
    for(flow_index=0; flow_index < 8; flow_index++)
        WPE_DisplayMcMemberStats(group_handle[0],member_handle[flow_index]);
    
    
    for(flow_index=0; flow_index < 8; flow_index++)
        WPE_DisplayMcMemberStats(group_handle[1],member_handle[flow_index+8]); 

}

//Scenario 5
void CLI_Multicast_Scenario5_Setup(void)
{
    WP_U32 entry = 0;
    WPE_TestCfgInit();
    WPE_MulticastInit();

    //create rules

    WP_pce_rule_classification rule_cfg = {0};
    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;

    //rule 1
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = mc_flowagg_handle[1];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][0] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][0], "WP_PceRuleCreate",__LINE__);
 
//rule 2
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = mc_flowagg_handle[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][1] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][1], "WP_PceRuleCreate",__LINE__);


}

void CLI_Multicast_Scenario5_ResetStatistics(void)
{
    WP_U32 flow_index;
    WP_iw_generic_flow_stats flow_stats = {0};
    WP_iw_flow_stats stats = {0};
    WP_iw_mc_member_enhanced_stats member_stats = {0};
    for(flow_index=0; flow_index < 8; flow_index++)
    {
        WP_IwFlowStatisticsReset(g999_enet_tx_flow_agg[flow_index], WP_IW_FLOW_STAT_GENERIC, &flow_stats);
    }
    for(flow_index=0; flow_index < 8; flow_index++)
    {
        WP_IwFlowStatisticsReset(ge_enet_tx_flow_agg[flow_index], WP_IW_FLOW_STAT_GENERIC, &flow_stats);
    }
    for(flow_index=0; flow_index < 2; flow_index++)
    {
        WP_IwFlowStatisticsReset(mc_flowagg_handle[flow_index],WP_IW_FLOW_STAT,&stats);
    }

    for(flow_index=0; flow_index < 8; flow_index++)
        WP_IwMcMemberStatisticsReset(group_handle[0],member_handle[flow_index],&member_stats);


    for(flow_index=0; flow_index < 8; flow_index++)
        WP_IwMcMemberStatisticsReset(group_handle[1],member_handle[flow_index+8],&member_stats); 

}

void CLI_Multicast_Scenario5_ShowStatistics(void)
{
    WP_U32 flow_index;
    
    for(flow_index=0; flow_index < 8; flow_index++)
    {
        WPE_DisplayGenericFlowAggStats(g999_enet_tx_flow_agg[flow_index]);        
    }

    for(flow_index=0; flow_index < 8; flow_index++)
    {
        WPE_DisplayGenericFlowAggStats(ge_enet_tx_flow_agg[flow_index]); 
    }
    for(flow_index=0; flow_index < 2; flow_index++)
    {
        WPE_DisplayFlowAggStats(mc_flowagg_handle[flow_index]);
    }
    for(flow_index=0; flow_index < 8; flow_index++)
        WPE_DisplayMcMemberStats(group_handle[0],member_handle[flow_index]);
    
    
    for(flow_index=0; flow_index < 8; flow_index++)
        WPE_DisplayMcMemberStats(group_handle[1],member_handle[flow_index+8]); 

}



/******************Host test Scenarios**********/


//Scenario 1
void CLI_Host_Scenario1_Setup(void)
{
    WP_pce_rule_classification rule_cfg = {0};
    WPE_HostTestCfgInit_1();
    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;
//rule 1
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_host_array[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][0] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][0], "WP_PceRuleCreate",__LINE__);

//rule 2
        rule_cfg.filter_handle = PCE_filter[0];

        rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
        rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
        
        rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 2;

        rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


        rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = g999_enet_tx_flow_agg[0];  
        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][1] = WP_PceRuleCreate(WP_WINPATH(0),
                                           WP_PCE_RULE_CLASSIFICATION,
                                           &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][1], "WP_PceRuleCreate",__LINE__);

//rule 3
        rule_cfg.filter_handle = PCE_filter[0];

        rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
        rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
        
        rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 2;

        rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


        rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_host_array[1];  
        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][2] = WP_PceRuleCreate(WP_WINPATH(0),
                                           WP_PCE_RULE_CLASSIFICATION,
                                           &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][2], "WP_PceRuleCreate",__LINE__);


}

void CLI_Host_Scenario1_ResetStatistics(void)
{
    WP_U32 flow_index;
    WP_iw_generic_flow_stats flow_stats = {0};
    WP_iw_flow_stats stats = {0};

    for(flow_index=0; flow_index < 2; flow_index++)
    {
        WP_IwFlowStatisticsReset(agg_host_array[flow_index],WP_IW_FLOW_STAT,&stats);
    }

    WP_IwFlowStatisticsReset(g999_enet_tx_flow_agg[0],WP_IW_FLOW_STAT_GENERIC,&flow_stats);

}

void CLI_Host_Scenario1_ShowStatistics(void)
{
    WP_U32 flow_index;   

    WPE_DisplayGenericFlowAggStats(g999_enet_tx_flow_agg[0]);        

    for(flow_index=0; flow_index < 2; flow_index++)
    {
        WPE_DisplayFlowAggStats(agg_host_array[flow_index]);
    }     
}
//Scenario 2
void CLI_Host_Scenario2_Setup(void)
{
    WP_pce_rule_classification rule_cfg = {0};

    WPE_HostTestCfgInit_2();
    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;

//rule 1
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_host_array[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER;
    rule_cfg.match_result[1].param.ingress_policer.policer = policer_handle_v2[0];
    rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][0] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][0], "WP_PceRuleCreate",__LINE__);

    //rule 2
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 2;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = g999_enet_tx_flow_agg[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][1] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][1], "WP_PceRuleCreate",__LINE__);

//rule 3
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 2;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_host_array[1];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER;
    rule_cfg.match_result[1].param.ingress_policer.policer = policer_handle_v2[1];
    rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][2] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][2], "WP_PceRuleCreate",__LINE__);

}

void CLI_Host_Scenario2_ResetStatistics(void)
{
    WP_U32 flow_index;
    WP_iw_generic_flow_stats flow_stats = {0};
    WP_iw_flow_stats stats = {0};

    for(flow_index=0; flow_index < 2; flow_index++)
    {
        WP_IwFlowStatisticsReset(agg_host_array[flow_index],WP_IW_FLOW_STAT,&stats);
    }

    WP_IwFlowStatisticsReset(g999_enet_tx_flow_agg[0],WP_IW_FLOW_STAT_GENERIC,&flow_stats);

}

void CLI_Host_Scenario2_ShowStatistics(void)
{
    WP_U32 flow_index;   

    WPE_DisplayGenericFlowAggStats(g999_enet_tx_flow_agg[0]);        

    for(flow_index=0; flow_index < 2; flow_index++)
    {
        WPE_DisplayFlowAggStats(agg_host_array[flow_index]);
    }     
}

//Scenario 3
void CLI_Host_Scenario3_Setup(void)
{
    WP_status status;
    WPE_HostTestCfgInit_3();

}

void CLI_Host_Scenario3_ResetStatistics(void)
{
    WP_iw_flow_stats stats = {0};

    WP_IwFlowStatisticsReset(agg_host_array[0],WP_IW_FLOW_STAT,&stats);
    WP_IwFlowStatisticsReset(agg_host_array[1],WP_IW_FLOW_STAT,&stats);

    WP_IwFlowStatisticsReset(agg_host_array[2],WP_IW_FLOW_STAT,&stats);
}

void CLI_Host_Scenario3_ShowStatistics(void)
{
    WPE_DisplayFlowAggStats(agg_host_array[0]);
    WPE_DisplayFlowAggStats(agg_host_array[1]);

    WPE_DisplayFlowAggStats(agg_host_array[2]); 

//    WPE_FlowStatDisplay(dfc_flow_handle[0]);
}

//Scenario 4
void CLI_Host_Scenario4_Setup(void)
{
    WP_pce_rule_classification rule_cfg = {0};
    WPE_HostTestCfgInit_4();
//config rules
    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;

//rule 1
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag =  1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_host_array[2];  
 //   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER;
//    rule_cfg.match_result[1].param.ingress_policer.policer = policer_handle_v2[0];
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][0] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][0], "WP_PceRuleCreate",__LINE__);

    //rule 2
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag =  2;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = g999_enet_tx_flow_agg[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][1] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][1], "WP_PceRuleCreate",__LINE__);

//rule 3
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag =  2;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_host_array[3];  
//    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER;
//    rule_cfg.match_result[1].param.ingress_policer.policer = policer_handle_v2[1];
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][2] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][2], "WP_PceRuleCreate",__LINE__);


    
}

void CLI_Host_Scenario4_ResetStatistics(void)
{

    WP_iw_generic_flow_stats flow_stats = {0};


   
    WP_IwFlowStatisticsReset(agg_host_array[2],WP_IW_FLOW_STAT_GENERIC,&flow_stats);
   
    WP_IwFlowStatisticsReset(agg_host_array[3],WP_IW_FLOW_STAT_GENERIC,&flow_stats);
    
    WP_IwFlowStatisticsReset(g999_enet_tx_flow_agg[0],WP_IW_FLOW_STAT_GENERIC,&flow_stats);

}

void CLI_Host_Scenario4_ShowStatistics(void)
{
  

    WPE_DisplayGenericFlowAggStats(g999_enet_tx_flow_agg[0]);        

    WPE_DisplayGenericFlowAggStats(agg_host_array[2]);

    WPE_DisplayGenericFlowAggStats(agg_host_array[3]);
         
}

//Scenario 5
void CLI_Host_Scenario5_Setup(void)
{
    WP_pce_rule_classification rule_cfg = {0};
    WPE_HostTestCfgInit_5();
    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;

//rule 1
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_host_array[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][0] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][0], "WP_PceRuleCreate",__LINE__);
  
//rule 2
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 2;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = g999_enet_tx_flow_agg[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][1] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][1], "WP_PceRuleCreate",__LINE__);

//rule 3
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 2;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_host_array[1];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][2] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][2], "WP_PceRuleCreate",__LINE__);


//set rule to deny SA mac == 0x01 on ge

    rule_cfg.filter_handle = PCE_filter[1];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_SA;
    rule_cfg.rule_fields[1].value.mac_addr[5] = 0x1;
    rule_cfg.rule_fields[1].value.mac_addr[4] = 0x0;
    rule_cfg.rule_fields[1].value.mac_addr[3] = 0x0;
    rule_cfg.rule_fields[1].value.mac_addr[2] = 0x0;
    rule_cfg.rule_fields[1].value.mac_addr[1] = 0x0;
    rule_cfg.rule_fields[1].value.mac_addr[0] = 0x0;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_DENY;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[1][0] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[1][0], "WP_PceRuleCreate",__LINE__);


//set rule to deny SA mac == 0x02 on g999
    rule_cfg.filter_handle = PCE_filter[1];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_SA;
    rule_cfg.rule_fields[1].value.mac_addr[5] = 0x2;
    rule_cfg.rule_fields[1].value.mac_addr[4] = 0x0;
    rule_cfg.rule_fields[1].value.mac_addr[3] = 0x0;
    rule_cfg.rule_fields[1].value.mac_addr[2] = 0x0;
    rule_cfg.rule_fields[1].value.mac_addr[1] = 0x0;
    rule_cfg.rule_fields[1].value.mac_addr[0] = 0x0;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_DENY;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[1][1] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[1][1], "WP_PceRuleCreate",__LINE__);
    

}

void CLI_Host_Scenario5_ResetStatistics(void)
{
    WP_iw_generic_flow_stats flow_stats = {0};
    WP_iw_flow_stats stats = {0};


    WP_IwFlowStatisticsReset(agg_host_array[0],WP_IW_FLOW_STAT,&stats);
   
    WP_IwFlowStatisticsReset(agg_host_array[1],WP_IW_FLOW_STAT,&stats);
    
    WP_IwFlowStatisticsReset(g999_enet_tx_flow_agg[0],WP_IW_FLOW_STAT_GENERIC,&flow_stats);

}

void CLI_Host_Scenario5_ShowStatistics(void)
{

    WPE_DisplayGenericFlowAggStats(g999_enet_tx_flow_agg[0]);        

    WPE_DisplayFlowAggStats(agg_host_array[0]);

    WPE_DisplayFlowAggStats(agg_host_array[1]);
         
}



//Scenario 6
void CLI_Host_Scenario6_Setup(void)
{
    WP_pce_rule_classification rule_cfg = {0};
    WPE_HostTestCfgInit_6();
    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;

//rule 1
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_host_array[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][0] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][0], "WP_PceRuleCreate",__LINE__);
  
//rule 2
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 2;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = g999_enet_tx_flow_agg[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][1] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][1], "WP_PceRuleCreate",__LINE__);

//rule 3
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 2;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_host_array[2];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][2] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][2], "WP_PceRuleCreate",__LINE__);

}

void CLI_Host_Scenario6_ResetStatistics(void)
{
    WP_iw_generic_flow_stats flow_stats = {0};
    WP_iw_flow_stats stats = {0};


    WP_IwFlowStatisticsReset(agg_host_array[0],WP_IW_FLOW_STAT,&stats);
   
    WP_IwFlowStatisticsReset(agg_host_array[2],WP_IW_FLOW_STAT,&stats);
    
    WP_IwFlowStatisticsReset(g999_enet_tx_flow_agg[0],WP_IW_FLOW_STAT_GENERIC,&flow_stats);

}

void CLI_Host_Scenario6_ShowStatistics(void)
{

    WPE_DisplayGenericFlowAggStats(g999_enet_tx_flow_agg[0]);        

    WPE_DisplayFlowAggStats(agg_host_array[0]);

    WPE_DisplayFlowAggStats(agg_host_array[2]);
         
}



//Scenario 7
void CLI_Host_Scenario7_Setup(void)
{
    WP_pce_rule_classification rule_cfg = {0};
    WPE_HostTestCfgInit_7();

    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;
  
//rule 1
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 2;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = g999_enet_tx_flow_agg[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][1] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][1], "WP_PceRuleCreate",__LINE__);

}

void CLI_Host_Scenario7_ResetStatistics(void)
{
    WP_iw_generic_flow_stats flow_stats = {0};
    WP_iw_flow_stats stats = {0};


    WP_IwFlowStatisticsReset(agg_host_array[0],WP_IW_FLOW_STAT,&stats);
   
    WP_IwFlowStatisticsReset(agg_host_array[1],WP_IW_FLOW_STAT,&stats);
    
    WP_IwFlowStatisticsReset(g999_enet_tx_flow_agg[0],WP_IW_FLOW_STAT_GENERIC,&flow_stats);

}

void CLI_Host_Scenario7_ShowStatistics(void)
{

    WPE_DisplayGenericFlowAggStats(g999_enet_tx_flow_agg[0]);        

    WPE_DisplayFlowAggStats(agg_host_array[0]);

    WPE_DisplayFlowAggStats(agg_host_array[1]);
         
}

//Scenario 8
void CLI_Host_Scenario8_Setup(void)
{
    WP_pce_rule_classification rule_cfg = {0};
    WPE_HostTestCfgInit_8();

    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;

//rule 1
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_host_array[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][0] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][0], "WP_PceRuleCreate",__LINE__);
  
//rule 2
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 2;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = g999_enet_tx_flow_agg[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][1] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][1], "WP_PceRuleCreate",__LINE__);

//rule 3
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 2;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_host_array[2];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][2] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][2], "WP_PceRuleCreate",__LINE__);

}

void CLI_Host_Scenario8_ResetStatistics(void)
{
    WP_iw_generic_flow_stats flow_stats = {0};
    WP_iw_flow_stats stats = {0};


    WP_IwFlowStatisticsReset(agg_host_array[0],WP_IW_FLOW_STAT,&stats);
   
    WP_IwFlowStatisticsReset(agg_host_array[2],WP_IW_FLOW_STAT,&stats);
    
    WP_IwFlowStatisticsReset(g999_enet_tx_flow_agg[0],WP_IW_FLOW_STAT_GENERIC,&flow_stats);

}

void CLI_Host_Scenario8_ShowStatistics(void)
{

    WPE_DisplayGenericFlowAggStats(g999_enet_tx_flow_agg[0]);        

    WPE_DisplayFlowAggStats(agg_host_array[0]);

    WPE_DisplayFlowAggStats(agg_host_array[2]);
         
}


//Scenario 9
void CLI_Host_Scenario9_Setup(void)
{
    WP_pce_rule_classification rule_cfg = {0};
    WPE_HostTestCfgInit_6();
    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;

//rule 1
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 1;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_host_array[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][0] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][0], "WP_PceRuleCreate",__LINE__);
  
//rule 2
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_ge;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 2;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = g999_enet_tx_flow_agg[0];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][1] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][1], "WP_PceRuleCreate",__LINE__);

//rule 3
    rule_cfg.filter_handle = PCE_filter[0];

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = bport_enet_g999;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
    rule_cfg.rule_fields[1].value.vlan_tag = 2;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_host_array[1];  
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

    PCE_rule_handle[0][2] = WP_PceRuleCreate(WP_WINPATH(0),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);
    App_TerminateOnError(PCE_rule_handle[0][2], "WP_PceRuleCreate",__LINE__);

}

void CLI_Host_Scenario9_ResetStatistics(void)
{
    WP_iw_generic_flow_stats flow_stats = {0};
    WP_iw_flow_stats stats = {0};


    WP_IwFlowStatisticsReset(agg_host_array[0],WP_IW_FLOW_STAT,&stats);
   
    WP_IwFlowStatisticsReset(agg_host_array[1],WP_IW_FLOW_STAT,&stats);
    
    WP_IwFlowStatisticsReset(g999_enet_tx_flow_agg[0],WP_IW_FLOW_STAT_GENERIC,&flow_stats);

}

void CLI_Host_Scenario9_ShowStatistics(void)
{

    WPE_DisplayGenericFlowAggStats(g999_enet_tx_flow_agg[0]);        

    WPE_DisplayFlowAggStats(agg_host_array[0]);

    WPE_DisplayFlowAggStats(agg_host_array[1]);
         
}



