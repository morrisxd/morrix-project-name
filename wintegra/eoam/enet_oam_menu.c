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
#ifdef __linux__
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>
#endif

#include "enet_oam_menu.h"
#include "enet_oam_util.h"



/*
typedef enum
{
   WP_IW_ENET_OAM_FREQ_RATE_DISABLE      = 0,
   WP_IW_ENET_OAM_FREQ_RATE_300_PER_SEC  = 3333,
   WP_IW_ENET_OAM_FREQ_RATE_100_PER_SEC  = 10000,
   WP_IW_ENET_OAM_FREQ_RATE_10_PER_SEC   = 100000,
   WP_IW_ENET_OAM_FREQ_RATE_1_PER_SEC    = 1000000,
   WP_IW_ENET_OAM_FREQ_RATE_1_PER_10_SEC = 10000000,
   WP_IW_ENET_OAM_FREQ_RATE_1_PER_MIN    = 60000000,
   WP_IW_ENET_OAM_FREQ_RATE_1_PER_10_MIN = 600000000
}
WP_iw_enet_oam_freq_rate;
*/
// Must match with WP_iw_enet_oam_freq_rate


char* E_freq_rate[]=
{
    "rate disable    ",
    "300 per second  ",
    "100 per second  ",
    "10 per second   ",
    "1 per second    ",
    "1 per 10 seconds",
    "1 per minute    ",
    "1 per 10 minutes",
};

char *STR_MENU_HDR = "                  Ethernet OAM Demo                     ";
char appname[] = "Ethernet OAM Demo";
static char delimiters[] = ".:-";

extern Y_MenuEntry V_MainMenu[];

static Y_MenuEntry *V_CurrMenuP = &V_MainMenu[0];


Y_MenuEntry V_MainMenu[] = 
{
    {K_Menu, MAIN_MENU_ITEM_NUM, TRUE, "Main Menu", {(Y_MenuEntry *)V_MainMenu}, NULL},
    {K_Menu, 1, TRUE, " -> MA   Menu", {(Y_MenuEntry *)MA_menu}, NULL},
    {K_Menu, 2, TRUE, " -> MEP  Menu", {(Y_MenuEntry *)MEP_menu}, NULL},
    {K_Menu, 3, TRUE, " -> CCM  Menu", {(Y_MenuEntry *)CCM_menu}, NULL},
    {K_Menu, 4, TRUE, " -> LM   Menu", {(Y_MenuEntry *)LM_menu}, NULL},
    {K_Menu, 5, TRUE, " -> DM   Menu", {(Y_MenuEntry *)DM_menu}, NULL},
    {K_Menu, 6, TRUE, " -> Flow Menu", {(Y_MenuEntry *)FLOW_menu}, NULL},
    {K_Menu, 7, TRUE, " -> System PM", {(Y_MenuEntry *)PM_menu}, NULL},
    {K_Info, 8, TRUE, " <> Debug Level", {(void*)(int)CLI_Debug_Level}, NULL},
    {K_Menu, 9, TRUE, " -> Performance Menu", {(Y_MenuEntry *)PD_menu}, NULL},	
    {K_Menu, 10, TRUE," <> Predefined Test Menu", {(Y_MenuEntry *)PT_menu}, NULL},
    {K_Info, 11, TRUE," <> Quit & Reboot", {(void*)(int)CLI_MAIN_Reboot}, NULL},
    {K_Info, 12, TRUE," <> Quit & Keepalive", {(void*)(int)CLI_MAIN_QuitAndKeepalive}, NULL},
};

Y_MenuEntry MA_menu[] = 
{
    {K_Menu, MA_MENU_ITEM_NUM, TRUE, "MA menu",         {(Y_MenuEntry *)V_MainMenu}, NULL},
    {K_Info, 1, TRUE, " <> Show Info",                  {(void*)(int)CLI_MA_ShowInfo}, NULL},
    {K_Menu, 2, TRUE, " -> Create MA(MA ID, rate, MA MAC)",   {(Y_MenuEntry *)V_MA_CreateMAMenu}, NULL},
    {K_Menu, 3, TRUE, " -> Delete MA(MA ID)",           {(Y_MenuEntry *)V_MA_DeleteMAMenu}, NULL},
};

Y_MenuEntry V_MA_CreateMAMenu[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Create MA(MA ID, rate, MA MAC)",  {(Y_MenuEntry *)MA_menu}, NULL},
    {K_Cli, 1, TRUE, "Please input MA ID, rate and MA MAC.",              {(void*)(int)CLI_MA_CreateMA}, CLI_MA_CreateMA_Help},
};

Y_MenuEntry V_MA_DeleteMAMenu[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Delete MA(MA ID)",  {(Y_MenuEntry *)MA_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID.",                {(void*)(int)CLI_MA_DeleteMA}, CLI_MA_DeleteMA_Help},
};

Y_MenuEntry MEP_menu[] = 
{
    {K_Menu, MEP_MENU_ITEM_NUM, TRUE, "MEP menu",                        {(Y_MenuEntry *)V_MainMenu}, NULL},
    {K_Info, 1, TRUE, " <> Show Info",                                   {(void*)(int)CLI_MEP_ShowInfo}, NULL},
    {K_Menu, 2, TRUE, " -> Create Remote MEP(MA ID, MEP ID, MAC, Vlan)", {(Y_MenuEntry *)V_MEP_CreateRemoteMEPMenu}, NULL},
    {K_Menu, 3, TRUE, " -> Delete Remote MEP(MA ID, MEP ID)",            {(Y_MenuEntry *)V_MEP_DeleteRemoteMEPMenu}, NULL},
    {K_Info, 4, TRUE, " <> Show Message Info",                           {(void*)(int)CLI_MSG_ShowInfo}, NULL},
};

Y_MenuEntry V_MEP_CreateRemoteMEPMenu[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Create Remote MEP(MA ID, MEP ID, Vlan, MAC)", {(Y_MenuEntry *)MEP_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID, MEP ID, Vlan and MAC.", {(void*)(int)CLI_MEP_CreateRemoteMEP}, CLI_MEP_CreateRemoteMEP_Help},
};

Y_MenuEntry V_MEP_DeleteRemoteMEPMenu[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Delete Remote MEP(MA ID, MEP ID)", {(Y_MenuEntry *)MEP_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID and MEP ID.", {(void*)(int)CLI_MEP_DeleteRemoteMEP}, CLI_MEP_DeleteRemoteMEP_Help},
};


Y_MenuEntry CCM_menu[] = 
{
    {K_Menu, CCM_MENU_ITEM_NUM, TRUE, "CCM menu", {(Y_MenuEntry *)V_MainMenu}, NULL},
    {K_Info, 1, TRUE, " <> Get CCM status per MEP", {(void*)(int)CLI_CCM_GetCCMStatus}, NULL},
    {K_Menu, 2, TRUE, " -> Enable Tx CCM(MA ID, MEP_ID, LM enable, vlan)", {(Y_MenuEntry *)V_CCM_EnableTxCCM}, NULL},
    {K_Menu, 3, TRUE, " -> Disable Tx CCM(MA ID, MEP ID)", {(Y_MenuEntry *)V_CCM_DisableTxCCM}, NULL},
};

Y_MenuEntry V_CCM_EnableTxCCM[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Enable Tx CCM(MA ID, MEP ID, LM enable, vlan)", {(Y_MenuEntry *)CCM_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID, MEP ID, LM enable and vlan.", {(void*)(int)CLI_CCM_EnableTxCCM}, CLI_CCM_EnableTxCCM_Help},
};

Y_MenuEntry V_CCM_DisableTxCCM[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Disable Tx CCM(MA ID, MEP ID)", {(Y_MenuEntry *)CCM_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID and MEP ID.", {(void*)(int)CLI_CCM_DisableTxCCM}, CLI_CCM_DisableTxCCM_Help},
};

Y_MenuEntry LM_menu[] = 
{
    {K_Menu, LM_MENU_ITEM_NUM, TRUE, "Loss Measurement Menu",       {(Y_MenuEntry *)V_MainMenu}, NULL},
    {K_Menu, 1, TRUE, " -> Enable LMM(MA ID, MEP ID, Vlan, rate)",  {(Y_MenuEntry *)V_LM_EnableLMM}, NULL},
    {K_Menu, 2, TRUE, " -> Disable LMM(MA ID, MEP ID, Vlan)",       {(Y_MenuEntry *)V_LM_DisableLMM}, NULL},
    {K_Menu, 3, TRUE, " -> Get PM stats(MA ID, MEP ID)",            {(Y_MenuEntry *)V_LM_GetPMStats}, NULL},
    {K_Menu, 4, TRUE, " -> Get Loss Ratio(MA ID, MEP ID)",          {(Y_MenuEntry *)V_LM_GetLossRatio}, NULL},
};

Y_MenuEntry V_LM_EnableLMM[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Enable LMM(MA ID, MEP ID, Vlan, rate)", {(Y_MenuEntry *)LM_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID, MEP ID, Vlan and rate.", {(void*)(int)CLI_LM_EnableLMM}, CLI_LM_EnableLMM_Help},
};

Y_MenuEntry V_LM_DisableLMM[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Disable LMM(MA ID, MEP ID, Vlan)", {(Y_MenuEntry *)LM_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID, MEP ID and Vlan.", {(void*)(int)CLI_LM_DisableLMM}, CLI_LM_DisableLMM_Help},
};

Y_MenuEntry V_LM_GetPMStats[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Get PM stats(MA ID, MEP ID)", {(Y_MenuEntry *)LM_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID and MEP ID.", {(void*)(int)CLI_LM_GetPMStats}, CLI_LM_GetPMStats_Help},
};

Y_MenuEntry V_LM_GetLossRatio[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Get Loss Ratio(MA ID, MEP ID)", {(Y_MenuEntry *)LM_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID and MEP ID.", {(void*)(int)CLI_LM_GetLossRatio}, CLI_LM_GetLossRatio_Help},
};

Y_MenuEntry DM_menu[] = 
{
    {K_Menu, DM_MENU_ITEM_NUM, TRUE, "Delay Measurement Menu",                  {(Y_MenuEntry *)V_MainMenu}, NULL},
    {K_Menu, 1, TRUE, " -> Enable DM1(MA ID, MEP ID, Vlan, rate)",              {(Y_MenuEntry *)V_DM_EnableDM1}, NULL},
    {K_Menu, 2, TRUE, " -> Disable DM1(MA ID, MEP ID, Vlan)",                   {(Y_MenuEntry *)V_DM_DisableDM1}, NULL},
    {K_Menu, 3, TRUE, " -> Enable DMM(MA ID, MEP ID, Vlan, rate)",              {(Y_MenuEntry *)V_DM_EnableDMM}, NULL},
    {K_Menu, 4, TRUE, " -> Disable DMM(MA ID, MEP ID, vlan)",                   {(Y_MenuEntry *)V_DM_DisableDMM}, NULL},
    {K_Menu, 5, TRUE, " -> Get Delay measurement counters(MA ID, MEP ID)",      {(Y_MenuEntry *)V_DM_GetDMCounters}, NULL},
    {K_Menu, 6, TRUE, " -> Calculate Delay(MA ID, MEP ID)",                     {(Y_MenuEntry *)V_DM_CalculateDelay}, NULL},
};

Y_MenuEntry V_DM_EnableDM1[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Enable DM1(MA ID, MEP ID, Vlan, rate)", {(Y_MenuEntry *)DM_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID, MEP ID, Vlan and rate.", {(void*)(int)CLI_DM_EnableDM1}, CLI_DM_EnableDM1_Help},
};

Y_MenuEntry V_DM_DisableDM1[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Disable DM1(MA ID, MEP ID, Vlan)", {(Y_MenuEntry *)DM_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID, MEP ID and Vlan.", {(void*)(int)CLI_DM_DisableDM1}, CLI_DM_DisableDM1_Help},
};

Y_MenuEntry V_DM_EnableDMM[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Enable DMM(MA ID, MEP ID, Vlan, rate)", {(Y_MenuEntry *)DM_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID, MEP ID, Vlan and rate.", {(void*)(int)CLI_DM_EnableDMM}, CLI_DM_EnableDMM_Help},
};

Y_MenuEntry V_DM_DisableDMM[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Disable DMM(MA ID, MEP ID, vlan)", {(Y_MenuEntry *)DM_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID, MEP ID and vlan.", {(void*)(int)CLI_DM_DisableDMM}, CLI_DM_DisableDMM_Help},
};

Y_MenuEntry V_DM_GetDMCounters[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Get Delay measurement counters(MA ID, MEP ID)", {(Y_MenuEntry *)DM_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID and MEP ID.", {(void*)(int)CLI_DM_GetDMCounters}, CLI_DM_GetDMCounters_Help},
};

Y_MenuEntry V_DM_CalculateDelay[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Calculate Delay(MA ID, MEP ID)", {(Y_MenuEntry *)DM_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID and MEP ID.", {(void*)(int)CLI_DM_CalculateDelay}, CLI_DM_CalculateDelay_Help},
};

Y_MenuEntry FLOW_menu[] = 
{
    {K_Menu, FLOW_MENU_ITEM_NUM, TRUE, "Flow Menu",                                {(Y_MenuEntry *)V_MainMenu}, NULL},
    {K_Info, 1, TRUE, " <> Show flow info",                                        {(void*)(int)CLI_FLOW_ShowInfo}, NULL},
    {K_Menu, 2, TRUE, " -> Create flow(MA ID, MEP ID, Vlan, UL(1)/DL(0), S-MAC, D-MAC)", {(Y_MenuEntry *)V_FLOW_CreateFlow}, NULL},
    {K_Menu, 3, TRUE, " -> Delete flow(MA ID, MEP ID, Vlan, UL(1)/DL(0), S-MAC, D-MAC)", {(Y_MenuEntry *)V_FLOW_DeleteFlow}, NULL},
    {K_Menu, 4, TRUE, " -> Enable TXFCL count(MA ID, MEP ID, Vlan, S-MAC, D-MAC)", {(Y_MenuEntry *)V_FLOW_EnableTXFCLCount}, NULL},
    {K_Menu, 5, TRUE, " -> Disable TXFCL count(MA ID, MEP ID, Vlan, S-MAC, D-MAC)",{(Y_MenuEntry *)V_FLOW_DisableTXFCLCount}, NULL},
    {K_Menu, 6, TRUE, " -> Enable RXFCL count(MA ID, MEP ID, Vlan, S-MAC, D-MAC)", {(Y_MenuEntry *)V_FLOW_EnableRXFCLCount}, NULL},
    {K_Menu, 7, TRUE, " -> Disable RXFCL count(MA ID, MEP ID, Vlan, S-MAC, D-MAC)",{(Y_MenuEntry *)V_FLOW_DisableRXFCLCount}, NULL},
};

Y_MenuEntry V_FLOW_CreateFlow[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "MA ID, MEP ID, Vlan, UL(1)/DL(1), S-MAC, D-MAC", {(Y_MenuEntry *)FLOW_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID, MEP ID, Vlan, UL(1)/DL(0), S-MAC, D-MAC.", {(void*)(int)CLI_FLOW_CreateFlow}, CLI_FLOW_CreateFlow_Help},
};

Y_MenuEntry V_FLOW_DeleteFlow[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Delete flow(MA ID, MEP ID, Vlan, UL(1)/DL(0), S-MAC, D-MAC)", {(Y_MenuEntry *)FLOW_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID, MEP ID, Vlan, UL(1)/DL(0), S-MAC, D-MAC.", {(void*)(int)CLI_FLOW_DeleteFlow}, CLI_FLOW_DeleteFlow_Help},
};

Y_MenuEntry V_FLOW_EnableTXFCLCount[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Enable TXFCL count(MA ID, MEP ID, Vlan, S-MAC, D-MAC)", {(Y_MenuEntry *)FLOW_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID, MEP ID, Vlan, S-MAC, D-MAC.", {(void*)(int)CLI_FLOW_EnableTXFCLCount}, CLI_FLOW_EnableTXFCLCount_Help},
};

Y_MenuEntry V_FLOW_DisableTXFCLCount[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Disable TXFCL count(MA ID, MEP ID, Vlan, S-MAC, D-MAC)", {(Y_MenuEntry *)FLOW_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID, MEP ID, Vlan, S-MAC, D-MAC.", {(void*)(int)CLI_FLOW_DisableTXFCLCount}, CLI_FLOW_DisableTXFCLCount_Help},
};

Y_MenuEntry V_FLOW_EnableRXFCLCount[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Enable RXFCL count(MA ID, MEP ID, Vlan, S-MAC, D-MAC)", {(Y_MenuEntry *)FLOW_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID, MEP ID, Vlan, S-MAC, D-MAC.", {(void*)(int)CLI_FLOW_EnableRXFCLCount}, CLI_FLOW_EnableRXFCLCount_Help},
};

Y_MenuEntry V_FLOW_DisableRXFCLCount[] = 
{
    {K_Menu, MENU_ITEM_ONLY_ONE, TRUE, "Disable RXFCL count(MA ID, MEP ID, Vlan, S-MAC, D-MAC)", {(Y_MenuEntry *)FLOW_menu}, NULL},
    {K_Cli , 1, TRUE, "Please input MA ID, MEP ID, Vlan, S-MAC, D-MAC", {(void*)(int)CLI_FLOW_DisableRXFCLCount}, CLI_FLOW_DisableRXFCLCount_Help},
};

Y_MenuEntry PM_menu[] = 
{
    {K_Menu, PM_MENU_ITEM_NUM, TRUE, "PM Menu",         {(Y_MenuEntry *)V_MainMenu}, NULL},
    {K_Info, 1, TRUE, " <> Device Stats [and reset]",   {(void*)(int)CLI_PM_ShowStats}, NULL},
    {K_Info, 2, TRUE, " <> Bport  Stats [and reset]",   {(void*)(int)CLI_PM_ShowStats}, NULL},
    {K_Info, 3, TRUE, " <> FlwAgg Stats [and reset]",   {(void*)(int)CLI_PM_ShowStats}, NULL},
    {K_Info, 4, TRUE, " <> Set Port Loopback [and reset]",   {(void*)(int)CLI_PM_SetPortLoopback}, NULL},
};

Y_MenuEntry PD_menu[] = 
{
    {K_Menu, PD_MENU_ITEM_NUM, TRUE, "PD menu",         			{(Y_MenuEntry *)V_MainMenu}, NULL},
    {K_Info, 1, TRUE, " <> Start Performance test",                  {(void*)(int)CLI_Performance_TestStart}, NULL},
    {K_Info, 2, TRUE, " <> Stop Performance test",   				{(void*)(int)CLI_Performance_TestExit}, NULL},
    {K_Info, 3, TRUE, " <> Display Wingine & Bus utilization",		{(void*)(int)CLI_Performance_TestCheckUtil}, NULL},
};

/* predefined test */
Y_MenuEntry PT_menu[] = 
{
    {K_Menu, PT_MENU_ITEM_NUM, TRUE, "PT menu",                   {(Y_MenuEntry *)V_MainMenu}, NULL},
    {K_Info,  1, TRUE, " <> Setup CCM Test (rate)",                {(void*)(int)CLI_PredefinedTest_CcmSetup}, NULL},
    {K_Info,  2, TRUE, " <> Trigger Remote Mep Connectivity Loss", {(void*)(int)CLI_PredefinedTest_CcmRxConnTrigger}, NULL},
    {K_Info,  3, TRUE, " <> Trigger Remote Mep Connectivity Return", {(void*)(int)CLI_PredefinedTest_CcmRxConnReturnTrigger}, NULL},
    {K_Info,  4, TRUE, " <> Display CCM Information",              {(void*)(int)CLI_PredefinedTest_CcmInfoShow}, NULL},
    {K_Info,  5, TRUE, " <> Tear Down CCM Test",                   {(void*)(int)CLI_PredefinedTest_CcmTeardown}, NULL},
    {K_Info,  6, TRUE, " <> Setup LM Test",                        {(void*)(int)CLI_PredefinedTest_LmSetup}, NULL},
    {K_Info,  7, TRUE, " <> Simulate Remote Packet Loss",          {(void*)(int)CLI_PredefinedTest_LmRemoteLossSimulate}, NULL},
    {K_Info,  8, TRUE, " <> Display LM Information",               {(void*)(int)CLI_PredefinedTest_LmInfoShow}, NULL},
    {K_Info,  9, TRUE, " <> Tear Down LM Test",                    {(void*)(int)CLI_PredefinedTest_LmTearDown}, NULL},
    {K_Info, 10, TRUE, " <> Setup 1DM Test",                       {(void*)(int)CLI_PredefinedTest_Dm1Setup}, NULL},
    {K_Info, 11, TRUE, " <> Tear Down 1DM Test",                   {(void*)(int)CLI_PredefinedTest_Dm1TearDown}, NULL},
    {K_Info, 12, TRUE, " <> Setup DDM Test",                       {(void*)(int)CLI_PredefinedTest_DmmSetup}, NULL},
    {K_Info, 13, TRUE, " <> Tear Down DDM Test",                   {(void*)(int)CLI_PredefinedTest_DmmTearDown}, NULL},
    {K_Info, 14, TRUE, " <> Display DM Information",               {(void*)(int)CLI_PredefinedTest_DmInfoShow}, NULL},
    {K_Info, 15, TRUE, " <> Host Send Background Traffic",         {(void*)(int)CLI_PredefinedTest_BkgnPacketHostSend}, NULL},
    {K_Info, 16, TRUE, " <> Display PCE information",         {(void*)(int)CLI_PredefinedTest_PceInfoShow}, NULL},
};


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

    /* Display main menu */
    F_DisplayMenu();

    while (eoam_running)
    {
        printf("%s", STR_ENTER_COMMAND);
        InputBuf[0] = '\0';
#ifdef  __linux__
      //loop:
        fgets(InputBuf, 256, stdin);
        //if (InputBuf[0] == '\0')
            //goto loop;
#else
#ifdef WP_HW_WINPATH3
        flush();
#endif
      //loop:
        gets(InputBuf);
        //if (InputBuf[0] == '\0')
            //goto loop;
#endif
        printf ("\n");
        F_MenuEngine(InputBuf);
    }

    return arg;
}

static int CLI_MAIN_QuitAndKeepalive(char *StrPrm)
{
	exit (0);

	return 0;
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
    eoam_running = FALSE;
    puts("Reboot...\r\n");
    WP_Delay(5);    
    WPE_Eoam_Quit(1);        
    return 0;
}

int CLI_MAIN_Quit(char *StrPrm)
{
    eoam_running = FALSE;
    puts("Quit...\r\n");
    WP_Delay(5);
    WPE_Eoam_Quit(0);        
    return 0;
}

/*****************************************************************************
 * Function name: CLI_PM_ShowStats
 * Description  : 1 to print  Device Stats;
                  2 to print  Bport  Stats;
                  3 to print  FlwAgg Stats; 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
int CLI_PM_ShowStats(char *StrPrm)
{
    int ret;
    unsigned char type;
    unsigned char reset = 0;
    char* pResetStr = NULL;
    type = strtoul(StrPrm, &pResetStr, 0);
    
    if ( strlen(pResetStr) > 0)
    {
        reset = strtoul(pResetStr, NULL, 0);
    }
    
    ret = WPE_Eoam_PM_ShowStats(type, reset);
    
    return ret;
}

/*****************************************************************************
 * Function name: CLI_PM_SetPortLoopback
 * Description  : 1 to set port 0 loopback;
                  2 to set port 1 loopback;
                  3 to set port 2 loopback;
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
int CLI_PM_SetPortLoopback(char *StrPrm)
{
    int ret;
    unsigned char cmd;
    unsigned char type = 1;
    unsigned char reset = 0;
    char* pResetStr = NULL;
    cmd = strtoul(StrPrm, &pResetStr, 0);
    
    if (pResetStr)
    {
        type = strtoul(pResetStr, &pResetStr, 0);
    }
    
    if (pResetStr)
    {
        reset = strtoul(pResetStr, NULL, 0);
    }
    
    ret = WPE_Eoam_PM_SetPortLoopback(type - 1, reset);
    
    return ret;
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
            if (K_Cli == CurrMenuEntryP->Type)
            {
                printf ("%s\n",  CurrMenuEntryP->Desc);
            }
            else
            {
                printf ("%2d. %s\n", Counter, CurrMenuEntryP->Desc);
            }
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
    int RetVal;
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
    if (K_Info == MenuEntryP->Type)
    {
        (MenuEntryP->Node.Leaf)(InputBuf);
        F_DisplayMenu();
    }
    else if (K_Cli == MenuEntryP->Type)
    {
        RetVal = (MenuEntryP->Node.Leaf)(InputBuf);
        if ((RetVal > 0) && (MenuEntryP->Help))
        {
            (MenuEntryP->Help)(InputBuf);
        }    
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

void F_Menu_FreqRateHint(void)
{
    int i;
    printf("Valid rate value list:\n");
    for (i = MENU_FREQ_RATE_MIN; i <= MENU_FREQ_RATE_MAX; i++)
    {
        printf(" %1d: %s\n", i, E_freq_rate[i]);
    }
}

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

/*****************************************************************************
 * Function name: CLI_Debug_Level
 * Description  : Reboot board.
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
static int CLI_Debug_Level(char *StrPrm)
{
    unsigned char index;
    unsigned char level = 0;
    char* pLevelStr = NULL;
    index = strtoul(StrPrm, &pLevelStr, 0);
    if (NULL == pLevelStr)
    {
        printf("Please input Menu Index and debug level(%u~%u).\n", WPE_DEBUG_CRIT, WPE_DEBUG_ALL);
        printf("e.g: %u %u\n", index, WPE_DEBUG_WARNING);
        return ERROR;
    }
    
    level = strtoul(pLevelStr, NULL, 0);
    
    WPE_Eoam_SetDebugLevel(level);
    
    return WP_OK;
}

/******************************* MA API *************************/

static int CLI_MA_ShowInfo(char *StrPrm)
{
    WPE_Eoam_MA_ShowInfo();
    return OK;
}

int CLI_MA_CreateMA(char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id;
    unsigned char mac[6];
    unsigned char rate;
    char* pMacStr = NULL;
    char* pRateStr = NULL;
    
    
    ret = strlen(StrPrm);
    if (ret < 13)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pRateStr, 0);
    if (NULL == pRateStr)
    {
        return ERROR;
    }

    rate = strtoul(pRateStr, &pMacStr, 10);
    if ((NULL == pMacStr) || (strlen(pMacStr) < 10))
    {
        return ERROR;
    }
    
    if ( (rate > MENU_FREQ_RATE_MAX) || (rate < MENU_FREQ_RATE_MIN))
    {
        F_Menu_FreqRateHint();
        return ERROR;
    }
    
    ret = F_ConvertStr2MacAddress(pMacStr, mac);
    if (ret)
    {
        return ERROR;
    }
        
    printf ("Your input MA ID=%u, rate=%u(%s) MA MAC=%02x:%02x:%02x:%02x:%02x:%02x\n", ma_id, rate,
            E_freq_rate[rate], mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    do
    {
        ret = WPE_Eoam_MA_CreateMA(ma_id, rate, mac);
        if (ret)
        {
            return ret;
        }
        
    }while(0);
    
    return WP_OK;
}

void CLI_MA_CreateMA_Help(char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID rate MA_MAC\n");
    printf("e.g: 1234 3 10:37:b9:ad:fe:0c\n");
}

int CLI_MA_DeleteMA(char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id;
    
    
    ret = strlen(StrPrm);
    if (ret < 1)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, NULL, 0);
    
    printf ("Your input MA ID=%u\n", ma_id);

    do
    {
        ret = WPE_Eoam_MA_DeleteMA(ma_id);
        if (ret)
        {
            return ret;
        }
        
        
    }while(0);
    
    return ret;
}

void CLI_MA_DeleteMA_Help(char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID\n");
    printf("e.g: 1234\n");
}

/******************************* MEP API *************************/

int CLI_MSG_ShowInfo(char *StrPrm)
{
    WPE_Eoam_MSG_ShowInfo();

    return OK;
}

int CLI_MEP_ShowInfo(char *StrPrm)
{
    WPE_Eoam_MEP_ShowInfo();

    return OK;
}

//Create Remote MEP(MA ID, MEP ID, MAC, Vlan)
int CLI_MEP_CreateRemoteMEP(char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id;
    unsigned short mep_id, vlan;
    unsigned char mac[6];
    char* pMacStr = NULL;
    char* pMepStr = NULL;
    char* pVlanStr = NULL;
    

    ret = strlen(StrPrm);
    if (ret < 3)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pMepStr, 0);
    if ((NULL == pMepStr))
    {
        printf("Please input MEP ID.\n");
        return ERROR;
    }

    mep_id = strtoul(pMepStr, &pVlanStr, 0);
    if ((NULL == pVlanStr))
    {
        printf("Please input vlan.\n");
        return ERROR;
    }

    vlan = strtoul(pVlanStr, &pMacStr, 0);
    if (NULL == pMacStr)
    {
        printf("Please input mac.\n");
        return ERROR;
    }
    
    ret = F_ConvertStr2MacAddress(pMacStr, mac);
    if (ret)
    {
        return ERROR;
    }

    printf ("Your input MA ID=%u, MEP ID=%u, vlan=%u, MAC=%02x:%02x:%02x:%02x:%02x:%02x\n", ma_id, mep_id,
            vlan, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    do
    {
        ret = WPE_Eoam_MEP_CreateRemoteMEP(ma_id, mep_id, mac, vlan);
        if (ret)
        {
            printf("WPE_Eoam_MEP_CreateRemoteMEP failed. ret=%d\n", ret);
            return ret;
        }
        
    }while(0);
    
    return OK;
}

void CLI_MEP_CreateRemoteMEP_Help(char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID MEP_ID Vlan MAC\n");
    printf("e.g: 1234 78 100 10:37:b9:ad:0e:fc\n");
}

//Delete Remote MEP(MA ID, MEP ID)
int CLI_MEP_DeleteRemoteMEP(char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id;
    unsigned short mep_id;
    

    char* pMepStr = NULL;

    ret = strlen(StrPrm);
    if (ret < 3)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pMepStr, 0);
    if ((NULL == pMepStr))
    {
        return ERROR;
    }

    mep_id = strtoul(pMepStr, NULL, 0);


    printf ("Your input MA ID=%u, MEP ID=%u\n", ma_id, mep_id);

    do
    {
        ret = WPE_Eoam_MEP_DeleteRemoteMEP(ma_id, mep_id);
        if (ret)
        {
            return ret;
        }     
        
    }while(0);
    
    return OK;
}

void CLI_MEP_DeleteRemoteMEP_Help(char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID MEP_ID MAC Vlan\n");
    printf("e.g: 1234 78\n");
}

/******************************* CCM API *************************/

int CLI_CCM_GetCCMStatus(char *StrPrm)
{
    return WPE_Eoam_CCM_GetCCMStatus();
}

//Enable Tx CCM(MA ID, MEP_ID, LM enable, vlan, rate)
int CLI_CCM_EnableTxCCM(char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id;
    unsigned short mep_id, vlan;
    unsigned char lm_enable;
    char* pLmStr = NULL;
    char* pMepStr = NULL;
    char* pVlanStr = NULL;
    
    
    ret = strlen(StrPrm);
    if (ret < 9)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pMepStr, 0);
    if ((NULL == pMepStr))
    {
        return ERROR;
    }

    mep_id = strtoul(pMepStr, &pLmStr, 0);
    if ((NULL == pLmStr))
    {
        return ERROR;
    }
      
    lm_enable = strtoul(pLmStr, &pVlanStr, 0);
    if ((NULL == pVlanStr))
    {
        return ERROR;
    }

    vlan = strtoul(pVlanStr, NULL, 0);

    printf ("Your input MA ID=%u, MEP ID=%u, LM enable=%u, vlan=%u\n", 
            ma_id, mep_id, lm_enable, vlan);

    do
    {
        ret = WPE_Eoam_CCM_EnableTxCCM(ma_id, mep_id, lm_enable, vlan);
        if (ret)
        {
            return ret;
        }                        
        
    }while(0);
    
    return ret;
}

void CLI_CCM_EnableTxCCM_Help(char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID MEP_ID LM_enable vlan\n");
    F_Menu_FreqRateHint();
    printf("e.g: 1234 78 1 100\n");
}

//Disable Tx CCM(MA ID, MEP ID)
int CLI_CCM_DisableTxCCM(char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id;
    unsigned short mep_id;
    char* pMepStr = NULL;
    

    ret = strlen(StrPrm);
    if (ret < 3)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pMepStr, 0);
    if ((NULL == pMepStr))
    {
        return ERROR;
    }

    mep_id = strtoul(pMepStr, NULL, 0);
    
    printf ("Your input MA ID=%u, MEP ID=%u\n", ma_id, mep_id);

    do
    {
        ret = WPE_Eoam_CCM_DisableTxCCM(ma_id, mep_id);
        if (ret)
        {
            return ret;
        }

    }while(0);
    
    return ret;

}

void CLI_CCM_DisableTxCCM_Help(char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID MEP_ID\n");
    printf("e.g: 1234 78\n");

}

/******************************* LM API *************************/

//Enable LMM(MA ID, MEP ID, Vlan, rate)
int CLI_LM_EnableLMM(char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id;
    unsigned short mep_id, vlan, rate;
    
    
    char* pMepStr = NULL;
    char* pVlanStr = NULL;
    char* pRateStr = NULL;
    
    ret = strlen(StrPrm);
    if (ret < 7)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pMepStr, 0);
    if ((NULL == pMepStr))
    {
        return ERROR;
    }

    mep_id = strtoul(pMepStr, &pVlanStr, 0);
    if ((NULL == pVlanStr))
    {
        return ERROR;
    }

    vlan = strtoul(pVlanStr, &pRateStr, 0);
    if ((NULL == pRateStr))
    {
        return ERROR;
    }

    rate = strtoul(pRateStr, NULL, 0);
    
    if ((rate < MENU_FREQ_RATE_MIN) || (rate > MENU_FREQ_RATE_MAX))
    {
        F_Menu_FreqRateHint();
        return ERROR;
    }
    
    printf ("Your input MA ID=%u, MEP ID=%u, vlan=%u, rate=%u\n", 
            ma_id, mep_id, vlan, rate);

    do
    {
        ret = WPE_Eoam_LM_EnableLMM(ma_id, mep_id, vlan, rate);
        if (ret)
        {
            return ret;
        }

    }while(0);
    
    return ret;
}

void CLI_LM_EnableLMM_Help(char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID MEP_ID Vlan rate\n");
    F_Menu_FreqRateHint();
    printf("e.g: 1234 78 100 3\n");

}

//Disable LMM(MA ID, MEP ID, Vlan)
int CLI_LM_DisableLMM(char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id;
    unsigned short mep_id, vlan;
    
    
    char* pMepStr = NULL;
    char* pVlanStr = NULL;
    
    ret = strlen(StrPrm);
    if (ret < 5)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pMepStr, 0);
    if ((NULL == pMepStr))
    {
        return ERROR;
    }

    mep_id = strtoul(pMepStr, &pVlanStr, 0);
    if ((NULL == pVlanStr))
    {
        return ERROR;
    }

    vlan = strtoul(pVlanStr, NULL, 0);
    
    printf ("Your input MA ID=%u, MEP ID=%u, vlan=%u\n", 
            ma_id, mep_id, vlan);

    do
    {
        ret = WPE_Eoam_LM_DisableLMM(ma_id, mep_id, vlan);
        if (ret)
        {
            return ret;
        }
        
    }while(0);
    
    return OK;
}

void CLI_LM_DisableLMM_Help(char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID MEP_ID Vlan\n");
    printf("e.g: 1234 78 100\n");

}

//Get PM stats(MA ID, MEP ID)
int CLI_LM_GetPMStats(char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id;
    unsigned short mep_id;
    

    char* pMepStr = NULL;

    ret = strlen(StrPrm);
    if (ret < 3)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pMepStr, 0);
    if ((NULL == pMepStr))
    {
        return ERROR;
    }

    mep_id = strtoul(pMepStr, NULL, 0);
    
    printf ("Your input MA ID=%u, MEP ID=%u\n", ma_id, mep_id);

            
    do
    {
        ret = WPE_Eoam_LM_GetPMStats(ma_id, mep_id);
        if (ret)
        {
            return ret;
        }
        
    }while(0);
    return ret;
}

void CLI_LM_GetPMStats_Help(char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID MEP_ID\n");
    printf("e.g: 1234 78\n");

}

//Get Loss Ratio(MA ID, MEP ID)
int CLI_LM_GetLossRatio(char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id;
    unsigned short mep_id;
    

    char* pMepStr = NULL;

    ret = strlen(StrPrm);
    if (ret < 3)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pMepStr, 0);
    if ((NULL == pMepStr))
    {
        return ERROR;
    }

    mep_id = strtoul(pMepStr, NULL, 0);

    printf ("Your input MA ID=%u, MEP ID=%u\n", ma_id, mep_id);

            

    do
    {
        ret = WPE_Eoam_LM_GetLossRatio(ma_id, mep_id);
        if (ret)
        {
            return ret;
        }
        
    }while(0);
    
    return ret;
}

void CLI_LM_GetLossRatio_Help(char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID MEP_ID\n");
    printf("e.g: 1234 78\n");

}

/******************************* DM API *************************/
//Enable DM1(MA ID, MEP ID, Vlan, rate)
int CLI_DM_EnableDM1(char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id;
    unsigned short mep_id, vlan, rate;
    

    char* pMepStr = NULL;
    char* pVlanStr = NULL;
    char* pRateStr = NULL;
    
    ret = strlen(StrPrm);
    if (ret < 9)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pMepStr, 0);
    if ((NULL == pMepStr))
    {
        return ERROR;
    }

    mep_id = strtoul(pMepStr, &pVlanStr, 0);
    if ((NULL == pVlanStr))
    {
        return ERROR;
    }

    vlan = strtoul(pVlanStr, &pRateStr, 0);
    if ((NULL == pRateStr))
    {
        return ERROR;
    }

    rate = strtoul(pRateStr, NULL, 0);
    
    if ((rate < MENU_FREQ_RATE_MIN) || (rate > MENU_FREQ_RATE_MAX))
    {
        F_Menu_FreqRateHint();
        return ERROR;
    }

    printf ("Your input MA ID=%u, MEP ID=%u, vlan=%u, rate=%u\n", 
            ma_id, mep_id, vlan, rate);


    do
    {
        ret = WPE_Eoam_DM_EnableDM1(ma_id, mep_id, vlan, rate);        
        if (ret)
        {
            return ret;
        }

    }while(0);
    
    return ret;
}

void CLI_DM_EnableDM1_Help(char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID MEP_ID Vlan rate\n");
    F_Menu_FreqRateHint();
    printf("e.g: 1234 78 100 3\n");
}

//Disable DM1(MA ID, MEP ID, Vlan)
int CLI_DM_DisableDM1(char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id;
    unsigned short mep_id, vlan;
    
    
    char* pMepStr = NULL;
    char* pVlanStr = NULL;
    
    ret = strlen(StrPrm);
    if (ret < 5)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pMepStr, 0);
    if ((NULL == pMepStr))
    {
        return ERROR;
    }

    mep_id = strtoul(pMepStr, &pVlanStr, 0);
    if ((NULL == pVlanStr))
    {
        return ERROR;
    }

    vlan = strtoul(pVlanStr, NULL, 0);
    printf ("Your input MA ID=%u, MEP ID=%u, vlan=%u\n", 
            ma_id, mep_id, vlan);


    do
    {
        ret = WPE_Eoam_DM_DisableDM1(ma_id, mep_id, vlan);        

        if (ret)
        {
            return ret;
        }

    }while(0);
    
    return ret;
}

void CLI_DM_DisableDM1_Help(char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID MEP_ID Vlan\n");
    printf("e.g: 1234 78 100\n");
}

//Enable DMM(MA ID, MEP ID, Vlan, rate)
int CLI_DM_EnableDMM(char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id;
    unsigned short mep_id, vlan, rate;
    
    
    char* pMepStr = NULL;
    char* pVlanStr = NULL;
    char* pRateStr = NULL;
    
    ret = strlen(StrPrm);
    if (ret < 7)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pMepStr, 0);
    if ((NULL == pMepStr))
    {
        return ERROR;
    }

    mep_id = strtoul(pMepStr, &pVlanStr, 0);
    if ((NULL == pVlanStr))
    {
        return ERROR;
    }

    vlan = strtoul(pVlanStr, &pRateStr, 0);
    if ((NULL == pRateStr))
    {
        return ERROR;
    }

    rate = strtoul(pRateStr, NULL, 0);
    
    if ((rate < MENU_FREQ_RATE_MIN) || (rate > MENU_FREQ_RATE_MAX))
    {
        F_Menu_FreqRateHint();
        return ERROR;
    }
        
    printf ("Your input MA ID=%u, MEP ID=%u, vlan=%u, rate=%u\n", 
            ma_id, mep_id, vlan, rate);


    do
    {
        ret = WPE_Eoam_DM_EnableDMM(ma_id, mep_id, vlan, rate);        
        if (ret)
        {
            return ret;
        }

    }while(0);
    
    return ret;
}

void CLI_DM_EnableDMM_Help(char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID MEP_ID Vlan rate\n");
    printf("e.g: 1234 78 100 10\n");
}

//Disable DMM(MA ID, MEP ID, vlan)
int CLI_DM_DisableDMM(char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id;
    unsigned short mep_id, vlan;
    
    
    char* pMepStr = NULL;
    char* pVlanStr = NULL;
    
    ret = strlen(StrPrm);
    if (ret < 5)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pMepStr, 0);
    if ((NULL == pMepStr))
    {
        return ERROR;
    }

    mep_id = strtoul(pMepStr, &pVlanStr, 0);
    if ((NULL == pVlanStr))
    {
        return ERROR;
    }

    vlan = strtoul(pVlanStr, NULL, 0);
    
    printf ("Your input MA ID=%u, MEP ID=%u, vlan=%u\n", 
            ma_id, mep_id, vlan);
            

    do
    {
        ret = WPE_Eoam_DM_DisableDMM(ma_id, mep_id, vlan);        
        if (ret)
        {
            return ret;
        }

    }while(0);
    
    return ret;
}

void CLI_DM_DisableDMM_Help(char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID MEP_ID Vlan\n");
    printf("e.g: 1234 78 100\n");
}

int CLI_DM_GetDMCounters(char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id, mep_id;
    

    char* pMepStr = NULL;

    ret = strlen(StrPrm);
    if (ret < 3)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pMepStr, 0);
    if ((NULL == pMepStr))
    {
        return ERROR;
    }

    mep_id = strtoul(pMepStr, NULL, 0);
    printf ("Your input MA ID=%u, MEP ID=%u\n", ma_id, mep_id);

    do
    {
        ret = WPE_Eoam_DM_GetDMCounters(ma_id, mep_id);            
        if (ret)
        {
            return ret;
        }

    }while(0);
    
    return ret;
}

void CLI_DM_GetDMCounters_Help(char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID MEP_ID\n");
    printf("e.g: 1234 78\n");
}

int CLI_DM_CalculateDelay(char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id, mep_id;
    
    char* pMepStr = NULL;

    ret = strlen(StrPrm);
    if (ret < 3)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pMepStr, 0);
    if ((NULL == pMepStr))
    {
        return ERROR;
    }

    mep_id = strtoul(pMepStr, NULL, 0);
    printf ("Your input MA ID=%u, MEP ID=%u\n", ma_id, mep_id);


    do
    {
        ret = WPE_Eoam_DM_CalculateDelay(ma_id, mep_id);
        if (ret)
        {
            return ret;
        }
    }while(0);
    
    return ret;
}

void CLI_DM_CalculateDelay_Help(char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID MEP_ID\n");
    printf("e.g: 1234 78\n");
}

/******************************* FLOW API *************************/

int CLI_FLOW_ShowInfo(char *StrPrm)
{
    int ret = 0;
    ret = WPE_Eoam_FLOW_ShowInfo();
    return ret;
}


/*
create DL FLOW - MAID=1 MEPID=1 vlan=10 ul=0 smac=1 dmac=2 
Create UL flow = MAID=2 MEPID=2 vlan=10 ul=1 smac=1 dmac=2 

create UL Flow - MAID=1 MEPID=1 vlan=10 ul=1 smac=2 dmac=1 
create DL flow = MAID=2 MEPID=2 vlan=10 ul=0 smac=2 dmac=1 
*/
int CLI_FLOW_CreateFlow(char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id;
    unsigned short mep_id, vlan, ul_dl;
    unsigned char s_mac[6], d_mac[6];
    char* pSMacStr = NULL;
    char* pDMacStr = NULL;
    char* pMepStr = NULL;
    char* pVlanStr = NULL;
    char* pUldlStr = NULL;
    
    
    ret = strlen(StrPrm);
    if (ret < 30)
    {
        return ERROR;
    }

    ret = strlen(StrPrm);
    if (ret < 3)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pMepStr, 0);
    if ((NULL == pMepStr))
    {
        return ERROR;
    }

    mep_id = strtoul(pMepStr, &pVlanStr, 0);
    if ((NULL == pVlanStr))
    {
        return ERROR;
    }

    vlan = strtoul(pVlanStr, &pUldlStr, 0);
    if (NULL == pUldlStr)
    {
        return ERROR;
    }

    ul_dl = strtoul(pUldlStr, &pSMacStr, 0);
    if (NULL == pSMacStr)
    {
        return ERROR;
    }
    
    ret = F_ConvertStr2MacAddress(pSMacStr, s_mac);
    if (ret)
    {
        return ERROR;
    }

    while(' ' == *pSMacStr)
    {
        pSMacStr++;
    }

    pDMacStr = strchr(pSMacStr, ' ');
    if (NULL == pDMacStr)
    {
        return ERROR;
    }

    ret = F_ConvertStr2MacAddress(pDMacStr, d_mac);
    if (ret)
    {
        return ERROR;
    }
    
    printf ("Your input MA ID=%u, MEP ID=%u, Vlan=%u, UL(1)/DL(0)=%u,"
                "S-MAC=%02x:%02x:%02x:%02x:%02x:%02x, "
                "D-MAC=%02x:%02x:%02x:%02x:%02x:%02x\n",
                ma_id, mep_id, vlan, ul_dl,
                s_mac[0], s_mac[1], s_mac[2], s_mac[3], s_mac[4], s_mac[5],
                d_mac[0], d_mac[1], d_mac[2], d_mac[3], d_mac[4], d_mac[5]);


    do
    {
        ret = WPE_Eoam_FLOW_CreateFlow(ul_dl, s_mac, d_mac, vlan, ma_id, mep_id);        
        if (ret)
        {
            return ret;
        }

    }while(0);
    return ret;
}

void CLI_FLOW_CreateFlow_Help(char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID MEP_ID Vlan UL(1)/DL(0) S-MAC D-MAC n");
    printf("e.g: 1234 78 100 1 50-37-b9-ad-0e-fd 40.37.b9.ad.0e.fc\n");
}

int CLI_FLOW_DeleteFlow(char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id;
    unsigned short mep_id, vlan, ul_dl;
    unsigned char s_mac[6], d_mac[6];
    

    char* pSMacStr = NULL;
    char* pDMacStr = NULL;
    char* pMepStr = NULL;
    char* pVlanStr = NULL;
    char* pUldlStr = NULL;
    
    ret = strlen(StrPrm);
    if (ret < 30)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pMepStr, 0);
    if ((NULL == pMepStr))
    {
        return ERROR;
    }

    mep_id = strtoul(pMepStr, &pVlanStr, 0);
    if ((NULL == pVlanStr))
    {
        return ERROR;
    }

    vlan = strtoul(pVlanStr, &pUldlStr, 0);
    if (NULL == pUldlStr)
    {
        return ERROR;
    }

    ul_dl = strtoul(pUldlStr, &pSMacStr, 0);
    if (NULL == pSMacStr)
    {
        return ERROR;
    }
    
    ret = F_ConvertStr2MacAddress(pSMacStr, s_mac);
    if (ret)
    {
        return ERROR;
    }

    while(' ' == *pSMacStr)
    {
        pSMacStr++;
    }

    pDMacStr = strchr(pSMacStr, ' ');
    if (NULL == pDMacStr)
    {
        return ERROR;
    }

    ret = F_ConvertStr2MacAddress(pDMacStr, d_mac);
    if (ret)
    {
        return ERROR;
    }

    printf ("Your input MA ID=%u, MEP ID=%u, Vlan=%u, UL(1)/DL(0)=%u, "
                "S-MAC=%02x:%02x:%02x:%02x:%02x:%02x, "
                "D-MAC=%02x:%02x:%02x:%02x:%02x:%02x\n",
                ma_id, mep_id, vlan, ul_dl, 
                s_mac[0], s_mac[1], s_mac[2], s_mac[3], s_mac[4], s_mac[5],
                d_mac[0], d_mac[1], d_mac[2], d_mac[3], d_mac[4], d_mac[5]);
            

    do
    {
        ret = WPE_Eoam_FLOW_DeleteFlow(ul_dl, s_mac, d_mac, vlan, ma_id, mep_id);        
        if (ret)
        {
            return ret;
        }

    }while(0);
    
    return ret;
}

void CLI_FLOW_DeleteFlow_Help(char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID MEP_ID Vlan UL(1)/DL(0) S-MAC D-MAC n");
    printf("e.g: 1234 78 100 1 50-37-b9-ad-0e-fd 40.37.b9.ad.0e.fc\n");
}


int CLI_FLOW_EnableTXFCLCount(char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id;
    unsigned short mep_id, vlan;
    unsigned char s_mac[6], d_mac[6];
    

    char* pSMacStr = NULL;
    char* pDMacStr = NULL;
    char* pMepStr = NULL;
    char* pVlanStr = NULL;
    
    ret = strlen(StrPrm);
    if (ret < 25)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pMepStr, 0);
    if ((NULL == pMepStr))
    {
        return ERROR;
    }

    mep_id = strtoul(pMepStr, &pVlanStr, 0);
    if ((NULL == pVlanStr))
    {
        return ERROR;
    }

    vlan = strtoul(pVlanStr, &pSMacStr, 0);
    if (NULL == pSMacStr)
    {
        return ERROR;
    }
    
    ret = F_ConvertStr2MacAddress(pSMacStr, s_mac);
    if (ret)
    {
        return ERROR;
    }

    while(' ' == *pSMacStr)
    {
        pSMacStr++;
    }

    pDMacStr = strchr(pSMacStr, ' ');
    if (NULL == pDMacStr)
    {
        return ERROR;
    }

    ret = F_ConvertStr2MacAddress(pDMacStr, d_mac);
    if (ret)
    {
        return ERROR;
    }

    printf ("Your input MA ID=%u, MEP ID=%u, Vlan=%u, "
                "S-MAC=%02x:%02x:%02x:%02x:%02x:%02x, "
                "D-MAC=%02x:%02x:%02x:%02x:%02x:%02x\n",
                ma_id, mep_id, vlan,
                s_mac[0], s_mac[1], s_mac[2], s_mac[3], s_mac[4], s_mac[5],
                d_mac[0], d_mac[1], d_mac[2], d_mac[3], d_mac[4], d_mac[5]);
            

    do
    {
        ret = WPE_Eoam_FLOW_EnableTXFCLCount(s_mac, d_mac, vlan, ma_id, mep_id);
        if (ret)
        {
            return ret;
        }

    }while(0);
    
    return ret;
}


void CLI_FLOW_EnableTXFCLCount_Help(char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID MEP_ID Vlan S-MAC D-MAC n");
    printf("e.g: 1234 78 100 50-37-b9-ad-0e-fd 40.37.b9.ad.0e.fc\n");
}


int CLI_FLOW_DisableTXFCLCount (char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id;
    unsigned short mep_id, vlan;
    unsigned char s_mac[6], d_mac[6];
    

    char* pSMacStr = NULL;
    char* pDMacStr = NULL;
    char* pMepStr = NULL;
    char* pVlanStr = NULL;
    
    ret = strlen(StrPrm);
    if (ret < 25)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pMepStr, 0);
    if ((NULL == pMepStr))
    {
        return ERROR;
    }

    mep_id = strtoul(pMepStr, &pVlanStr, 0);
    if ((NULL == pVlanStr))
    {
        return ERROR;
    }

    vlan = strtoul(pVlanStr, &pSMacStr, 0);
    if (NULL == pSMacStr)
    {
        return ERROR;
    }
    
    ret = F_ConvertStr2MacAddress(pSMacStr, s_mac);
    if (ret)
    {
        return ERROR;
    }

    while(' ' == *pSMacStr)
    {
        pSMacStr++;
    }

    pDMacStr = strchr(pSMacStr, ' ');
    if (NULL == pDMacStr)
    {
        return ERROR;
    }

    ret = F_ConvertStr2MacAddress(pDMacStr, d_mac);
    if (ret)
    {
        return ERROR;
    }

    printf ("Your input MA ID=%u, MEP ID=%u, Vlan=%u, "
                "S-MAC=%02x:%02x:%02x:%02x:%02x:%02x, "
                "D-MAC=%02x:%02x:%02x:%02x:%02x:%02x\n",
                ma_id, mep_id, vlan,
                s_mac[0], s_mac[1], s_mac[2], s_mac[3], s_mac[4], s_mac[5],
                d_mac[0], d_mac[1], d_mac[2], d_mac[3], d_mac[4], d_mac[5]);
            

    do
    {
        ret = WPE_Eoam_FLOW_DisableTXFCLCount(s_mac, d_mac, vlan, ma_id, mep_id);        
        if (ret)
        {
            return ret;
        }
    }while(0);
    
    return ret;
}

void CLI_FLOW_DisableTXFCLCount_Help (char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID MEP_ID Vlan S-MAC D-MAC n");
    printf("e.g: 1234 78 100 50-37-b9-ad-0e-fd 40.37.b9.ad.0e.fc\n");
}

int CLI_FLOW_EnableRXFCLCount (char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id;
    unsigned short mep_id, vlan;
    unsigned char s_mac[6], d_mac[6];

    char* pSMacStr = NULL;
    char* pDMacStr = NULL;
    char* pMepStr = NULL;
    char* pVlanStr = NULL;
    
    
    ret = strlen(StrPrm);
    if (ret < 25)
    {
        return ERROR;
    }

    ret = strlen(StrPrm);
    if (ret < 3)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pMepStr, 0);
    if ((NULL == pMepStr))
    {
        return ERROR;
    }

    mep_id = strtoul(pMepStr, &pVlanStr, 0);
    if ((NULL == pVlanStr))
    {
        return ERROR;
    }

    vlan = strtoul(pVlanStr, &pSMacStr, 0);
    if (NULL == pSMacStr)
    {
        return ERROR;
    }
    
    ret = F_ConvertStr2MacAddress(pSMacStr, s_mac);
    if (ret)
    {
        return ERROR;
    }

    while(' ' == *pSMacStr)
    {
        pSMacStr++;
    }

    pDMacStr = strchr(pSMacStr, ' ');
    if (NULL == pDMacStr)
    {
        return ERROR;
    }

    ret = F_ConvertStr2MacAddress(pDMacStr, d_mac);
    if (ret)
    {
        return ERROR;
    }

    printf ("Your input MA ID=%u, MEP ID=%u, Vlan=%u, "
                "S-MAC=%02x:%02x:%02x:%02x:%02x:%02x, "
                "D-MAC=%02x:%02x:%02x:%02x:%02x:%02x\n",
                ma_id, mep_id, vlan,
                s_mac[0], s_mac[1], s_mac[2], s_mac[3], s_mac[4], s_mac[5],
                d_mac[0], d_mac[1], d_mac[2], d_mac[3], d_mac[4], d_mac[5]);
            

    do
    {
        ret = WPE_Eoam_FLOW_EnableRXFCLCount(s_mac, d_mac, vlan, ma_id, mep_id);        
        if (ret)
        {
            return ret;
        }
              
    }while(0);
    
    return ret;
}

void CLI_FLOW_EnableRXFCLCount_Help (char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID MEP_ID Vlan S-MAC D-MAC n");
    printf("e.g: 1234 78 100 50-37-b9-ad-0e-fd 40.37.b9.ad.0e.f0\n");
}

int CLI_FLOW_DisableRXFCLCount (char *StrPrm)
{
    int ret = 0;
    unsigned int ma_id;
    unsigned short mep_id, vlan;
    unsigned char s_mac[6], d_mac[6];
    

    char* pSMacStr = NULL;
    char* pDMacStr = NULL;
    char* pMepStr = NULL;
    char* pVlanStr = NULL;
    
    ret = strlen(StrPrm);
    if (ret < 25)
    {
        return ERROR;
    }

    ma_id = strtoul(StrPrm, &pMepStr, 0);
    if ((NULL == pMepStr))
    {
        return ERROR;
    }

    mep_id = strtoul(pMepStr, &pVlanStr, 0);
    if ((NULL == pVlanStr))
    {
        return ERROR;
    }

    vlan = strtoul(pVlanStr, &pSMacStr, 0);
    if (NULL == pSMacStr)
    {
        return ERROR;
    }
    
    ret = F_ConvertStr2MacAddress(pSMacStr, s_mac);
    if (ret)
    {
        return ERROR;
    }

    while(' ' == *pSMacStr)
    {
        pSMacStr++;
    }

    pDMacStr = strchr(pSMacStr, ' ');
    if (NULL == pDMacStr)
    {
        return ERROR;
    }

    ret = F_ConvertStr2MacAddress(pDMacStr, d_mac);
    if (ret)
    {
        return ERROR;
    }

    printf ("Your input MA ID=%u, MEP ID=%u, Vlan=%u, "
                "S-MAC=%02x:%02x:%02x:%02x:%02x:%02x, "
                "D-MAC=%02x:%02x:%02x:%02x:%02x:%02x\n",
                ma_id, mep_id, vlan,
                s_mac[0], s_mac[1], s_mac[2], s_mac[3], s_mac[4], s_mac[5],
                d_mac[0], d_mac[1], d_mac[2], d_mac[3], d_mac[4], d_mac[5]);
            

    do
    {
        ret = WPE_Eoam_FLOW_DisableRXFCLCount(s_mac, d_mac, vlan, ma_id, mep_id);        
        if (ret)
        {
            return ret;
        }
   
    }while(0);
    
    return ret;
}

void CLI_FLOW_DisableRXFCLCount_Help (char *StrPrm)
{
    printf("Current menu: %s\n", V_CurrMenuP->Desc);
    printf("Please input parameters for this command. MA_ID MEP_ID Vlan S-MAC D-MAC n");
    printf("e.g: 1234 78 100 50-37-b9-ad-0e-fd 40.37.b9.ad.0e.fc\n");
}

/******************************* Performance Demo API *************************/

int CLI_Performance_TestStart(char *StrPrm)
{
    if (WP_OK != WPE_Eoam_Performance_TestStart())
    {
        return ERROR;
    }
    return 0;
}

int CLI_Performance_TestExit(char *StrPrm)
{
    if (WP_OK != WPE_Eoam_Performance_TestExit())
    {
        return ERROR;
    }
    return 0;
}

int CLI_Performance_TestCheckUtil(char *StrPrm)
{
    if (WP_OK != WPE_Eoam_Performance_CheckUtil())
    {
        return ERROR;
    }
    return 0;
}

int CLI_PredefinedTest_OamEntitySetup(char *StrPrm)
{
    int ret = 0;
    unsigned char rate;
    char* pRateStr = NULL;
    char pBuff[100] = {0};
    
    ret = strlen(StrPrm);
    if (ret < 1)
    {
        return ERROR;
    }

    rate = strtoul(StrPrm, &pRateStr, 0);
    if ( (rate > MENU_FREQ_RATE_MAX) || (rate < MENU_FREQ_RATE_MIN))
    {
        F_Menu_FreqRateHint();
        return ERROR;
    }
    printf ("Your input rate=%u(%s) \n", rate, E_freq_rate[rate]);

    sprintf(pBuff, "1001 %d 00:00:00:00:00:01", rate);
    printf("CreateMA %s\n", pBuff);
    CLI_MA_CreateMA(pBuff);

    sprintf(pBuff, "1002 %d 00:00:00:00:00:02", rate);
    printf("CreateMA %s\n", pBuff);
    CLI_MA_CreateMA(pBuff);

    printf("CreateRemoteMEP 1001 2 1000 00:00:00:00:00:02\n");
    CLI_MEP_CreateRemoteMEP("1001 2 1000 00:00:00:00:00:02");
    printf("CreateRemoteMEP 1002 1 1000 00:00:00:00:00:01\n");
    CLI_MEP_CreateRemoteMEP("1002 1 1000 00:00:00:00:00:01");
    printf("CreateFlow 1001 2 1000 0 00:00:00:01:00:01 00:00:00:02:00:02\n");
    CLI_FLOW_CreateFlow("1001 2 1000 0 00:00:00:01:00:01 00:00:00:02:00:02");
    printf("CreateFlow 1002 1 1000 1 00:00:00:01:00:01 00:00:00:02:00:02\n");
    CLI_FLOW_CreateFlow("1002 1 1000 1 00:00:00:01:00:01 00:00:00:02:00:02");
    printf("CreateFlow 1001 2 1000 1 00:00:00:02:00:02 00:00:00:01:00:01\n");
    CLI_FLOW_CreateFlow("1001 2 1000 1 00:00:00:02:00:02 00:00:00:01:00:01");
    printf("CreateFlow 1002 1 1000 0 00:00:00:02:00:02 00:00:00:01:00:01\n");
    CLI_FLOW_CreateFlow("1002 1 1000 0 00:00:00:02:00:02 00:00:00:01:00:01");
    return 0;
}

int CLI_PredefinedTest_OamEntityTearDown(char *StrPrm)
{
    printf("DeleteFlow 1001 2 1000 0 00:00:00:01:00:01 00:00:00:02:00:02\n");
    CLI_FLOW_DeleteFlow("1001 2 1000 0 00:00:00:01:00:01 00:00:00:02:00:02");
    printf("DeleteFlow 1002 1 1000 1 00:00:00:01:00:01 00:00:00:02:00:02\n");
    CLI_FLOW_DeleteFlow("1002 1 1000 1 00:00:00:01:00:01 00:00:00:02:00:02");
    printf("DeleteFlow 1001 2 1000 1 00:00:00:02:00:02 00:00:00:01:00:01\n");
    CLI_FLOW_DeleteFlow("1001 2 1000 1 00:00:00:02:00:02 00:00:00:01:00:01");
    printf("DeleteFlow 1002 1 1000 0 00:00:00:02:00:02 00:00:00:01:00:01\n");
    CLI_FLOW_DeleteFlow("1002 1 1000 0 00:00:00:02:00:02 00:00:00:01:00:01");
    printf("DeleteRemoteMEP 1001 2 1000 00:00:00:00:00:02\n");
    CLI_MEP_DeleteRemoteMEP("1001 2 1000 00:00:00:00:00:02");
    printf("DeleteRemoteMEP 1002 1 1000 00:00:00:00:00:01\n");
    CLI_MEP_DeleteRemoteMEP("1002 1 1000 00:00:00:00:00:01");
    printf("DeleteMA 1001\n");
    CLI_MA_DeleteMA("1001");
    printf("DeleteMA 1002\n");
    CLI_MA_DeleteMA("1002");
    return 0;
}

int CLI_PredefinedTest_CcmSetup(char *StrPrm)
{
    int ret;
    char* pRateStr = NULL;
    
    ret = strlen(StrPrm);
    if (ret < 1)
    {
        return ERROR;
    }

    ret = strtoul(StrPrm, &pRateStr, 0);
    if ( pRateStr != NULL && strlen(StrPrm) < 1)
    {
        F_Menu_FreqRateHint();
        return ERROR;
    }

    ret = CLI_PredefinedTest_OamEntitySetup(pRateStr);
    if(ret == ERROR)
    {
       return ERROR;
    }
    printf("EnableTxCCM 1001 2 0 1000\n");
    CLI_CCM_EnableTxCCM("1001 2 0 1000");
    printf("EnableTxCCM 1002 1 0 1000\n");
    CLI_CCM_EnableTxCCM("1002 1 0 1000");
    return 0;
}

int CLI_PredefinedTest_CcmRxConnTrigger(char *StrPrm)
{
    printf("DisableTxCCM 1001 2\n");
    CLI_CCM_DisableTxCCM("1001 2");
    return 0;
}

int CLI_PredefinedTest_CcmRxConnReturnTrigger(char *StrPrm)
{
    printf("EnableTxCCM 1001 2 0 1000\n");
    CLI_CCM_EnableTxCCM("1001 2 0 1000");
    return 0;
}

int CLI_PredefinedTest_CcmInfoShow(char *StrPrm)
{
    int ret;
    int opt = 0;
    char* pOptStr = NULL;
    
    ret = strlen(StrPrm);
    if (ret < 1)
    {
        return ERROR;
    }

    ret = strtoul(StrPrm, &pOptStr, 0);
    if ( strlen(pOptStr) > 0)
    {
       opt = strtoul(pOptStr, NULL, 0);
    }

    if(opt == 0 || opt == 1)
    {
        printf("GetCCMStatus\n");
        CLI_CCM_GetCCMStatus(NULL);
    
        printf("ReadCCMEvents\n");
        WPE_Eoam_CfmMessageReceive(NULL);
    }
    if(opt == 0 || opt == 2)
    {
        printf("Show Enet Information\n");
        CLI_PM_ShowStats("1");
    }
    if(opt == 0 || opt == 3)
    {
        printf("Show Bport Information\n");
        CLI_PM_ShowStats("2");
    }
    if(opt == 0 || opt == 4)
    {
        printf("Show Flow Information\n");
        CLI_PM_ShowStats("3");
    }
    if(opt == 5)
    {
        printf("Reset Enet Information\n");
        CLI_PM_ShowStats("1 1");
        printf("Reset Bport Information\n");
        CLI_PM_ShowStats("2 1");
        printf("Reset Flow Information\n");
        CLI_PM_ShowStats("3 1");
    }

    return 0;
}

int CLI_PredefinedTest_CcmTeardown(char *StrPrm)
{
    printf("DisableTxCCM 1001 2\n");
    CLI_CCM_DisableTxCCM("1001 2");
    printf("DisableTxCCM 1002 1\n");
    CLI_CCM_DisableTxCCM("1002 1");
    CLI_PredefinedTest_OamEntityTearDown(NULL);
    return 0;
}

int CLI_PredefinedTest_LmSetup(char *StrPrm)
{
    int ret;

    ret = CLI_PredefinedTest_OamEntitySetup("4");
    if(ret == ERROR)
    {
       return ERROR;
    }
    printf("Debug_Level 4\n");
#if 0
    /* set debug level from warning to all */
    CLI_Debug_Level("8 4");
#endif
    printf("EnableTxCCM 1001 2 1 1000\n");
    CLI_CCM_EnableTxCCM("1001 2 1 1000");
    printf("EnableTxCCM 1002 1 1 1000\n");
    CLI_CCM_EnableTxCCM("1002 1 1 1000");
    return 0;
}

int CLI_PredefinedTest_LmRemoteLossSimulate(char *StrPrm)
{
    printf("DisableRXFCLCount 1002 1 1000 00:00:00:01:00:01 00:00:00:02:00:02\n");
    CLI_FLOW_DisableRXFCLCount("1002 1 1000 00:00:00:01:00:01 00:00:00:02:00:02");
    return 0;
}

int CLI_PredefinedTest_LmInfoShow(char *StrPrm)
{
    printf("Process Pm Message\n");
    WPE_Eoam_PmMessageReceive(NULL);

    printf("GetPMStats 1001 2\n");
    CLI_LM_GetPMStats("1001 2");
    printf("GetLossRatio 1001 2\n");
    CLI_LM_GetLossRatio("1001 2");

    printf("GetPMStats 1002 1\n");
    CLI_LM_GetPMStats("1002 1");
    printf("GetLossRatio 1002 1\n");
    CLI_LM_GetLossRatio("1002 1");
    return 0;
}

int CLI_PredefinedTest_LmTearDown(char *StrPrm)
{
    printf("DisableTxCCM 1001 2\n");
    CLI_CCM_DisableTxCCM("1001 2");
    printf("DisableTxCCM 1002 1\n");
    CLI_CCM_DisableTxCCM("1002 1");
    CLI_PredefinedTest_OamEntityTearDown(NULL);
    return 0;
}

int CLI_PredefinedTest_Dm1Setup(char *StrPrm)
{
    int ret;

    ret = CLI_PredefinedTest_OamEntitySetup("4");
    if(ret == ERROR)
    {
       return ERROR;
    }
    printf("EnableDM1 1001 2 1000 4\n");
    CLI_DM_EnableDM1("1001 2 1000 4");
    printf("EnableDM1 1002 1 1000 4\n");
    CLI_DM_EnableDM1("1002 1 1000 4");
    return 0;
}

int CLI_PredefinedTest_DmInfoShow(char *StrPrm)
{
    printf("Process Pm Message\n");
    WPE_Eoam_PmMessageReceive(NULL);

    printf("GetDMCounters 1001 2\n");
    CLI_DM_GetDMCounters("1001 2");
    printf("CalculateDelay 1001 2\n");
    CLI_DM_CalculateDelay("1001 2");

    printf("GetDMCounters 1002 1\n");
    CLI_DM_GetDMCounters("1002 1");
    printf("CalculateDelay 1002 1\n");
    CLI_DM_CalculateDelay("1002 1");
    return 0;
}

int CLI_PredefinedTest_Dm1TearDown(char *StrPrm)
{
    printf("DisableDM1 1001 2 1000\n");
    CLI_DM_DisableDM1("1001 2 1000");
    printf("DisableDM1 1002 1 1000\n");
    CLI_DM_DisableDM1("1002 1 1000");
    CLI_PredefinedTest_OamEntityTearDown(NULL);
    return 0;
}

int CLI_PredefinedTest_DmmSetup(char *StrPrm)
{
    int ret;

    ret = CLI_PredefinedTest_OamEntitySetup("4");
    if(ret == ERROR)
    {
       return ERROR;
    }
    printf("EnableDMM 1001 2 1000 4\n");
    CLI_DM_EnableDMM("1001 2 1000 4");
    printf("EnableDMM 1002 1 1000 4\n");
    CLI_DM_EnableDMM("1002 1 1000 4");
    return 0;
}

int CLI_PredefinedTest_DmmTearDown(char *StrPrm)
{
    printf("DisableDMM 1001 2 1000\n");
    CLI_DM_DisableDMM("1001 2 1000");
    printf("DisableDMM 1002 1 1000\n");
    CLI_DM_DisableDMM("1002 1 1000");
    CLI_PredefinedTest_OamEntityTearDown(NULL);
    return 0;
}


int CLI_PredefinedTest_BkgnPacketHostSend(char *StrPrm)
{
    WP_U32 i, size=100;
    WP_U8 *data_ptr;
    WP_handle pool = eoam_system.h_pool_buffer_iw;
    WP_handle txch = eoam_system.tx_enet_handle[ENET1];
    int ret = 0;
    char* pOptStr = NULL;
 
    ret = strtoul(StrPrm, &pOptStr, 0);
    if (pOptStr != NULL)
    {
        ret = strtoul(pOptStr, NULL, 0);
        if(ret < 0 || ret > 2)
        {
            printf("invalid option %s, valid [0,1,2].\n", pOptStr);
            return ERROR;
        }
    }
 
    if(ret == 0 || ret == 1)
    {
        data_ptr = WP_PoolAlloc(pool);
        if(data_ptr == 0)
        {
            printf("no free buffer in pool.\n");
            return ERROR;
        }
 
        /* VLAN ID 1000, DMAC 00:00:00:02:00:02, SMAC 00:00:00:01:00:01 */
        WPE_AssemblePacket(&data_ptr[0], 4, 0x00000002);
        WPE_AssemblePacket(&data_ptr[4], 4, 0x00020000);
        WPE_AssemblePacket(&data_ptr[8], 4, 0x00010001);
        WPE_AssemblePacket(&data_ptr[12], 4, 0x810003e8); /* vlan = 1000*/
        WPE_AssemblePacket(&data_ptr[16], 2, 0x0800);
        WPE_AssemblePacket(&data_ptr[18], 4, 0x45040020);/* tos = 4 */
        WPE_AssemblePacket(&data_ptr[22], 4, 0x001b0000);
        WPE_AssemblePacket(&data_ptr[26], 4, 0x0a11e449);/* protocol = 0x11*/
        WPE_AssemblePacket(&data_ptr[30], 4, 0x00010c20);/* sa ip */
        WPE_AssemblePacket(&data_ptr[34], 4, 0xc0440000);/* da ip */
        WPE_AssemblePacket(&data_ptr[38], 4, 0x00190015);
        WPE_AssemblePacket(&data_ptr[42], 4, 0x001b77c0);
        WPE_AssemblePacket(&data_ptr[46], 4, 0x00a4c5ff);
     
        for(i = 50; i < size; i++)
        {
            data_ptr[i] = (i - 50);
        }
     
        printf("send packet 1:\n");
        WPE_HostSendPacket(pool, txch, data_ptr,  size);
     
        if(ret == 0 || ret == 2)
        {
            printf("Waiting for a while...\n");
            for(i=0;i<5000000;i++);
        }
    }
 
    if(ret == 0 || ret == 2)
    {
        data_ptr = WP_PoolAlloc(pool);
        if(data_ptr == 0)
        {
          printf("no free buffer in pool.\n");
          return ERROR;
        }
     
        /* VLAN ID 1000, DMAC 00:00:00:01:00:01, SMAC 00:00:00:02:00:02 */
        WPE_AssemblePacket(&data_ptr[0], 4, 0x00000001);
        WPE_AssemblePacket(&data_ptr[4], 4, 0x00010000);
        WPE_AssemblePacket(&data_ptr[8], 4, 0x00020002);
        WPE_AssemblePacket(&data_ptr[12], 4, 0x810003e8); /* vlan = 1000*/
        WPE_AssemblePacket(&data_ptr[16], 2, 0x0800);
        WPE_AssemblePacket(&data_ptr[18], 4, 0x45040020);/* tos = 4 */
        WPE_AssemblePacket(&data_ptr[22], 4, 0x001b0000);
        WPE_AssemblePacket(&data_ptr[26], 4, 0x0a11e449);/* protocol = 0x11*/
        WPE_AssemblePacket(&data_ptr[30], 4, 0x00010c20);/* sa ip */
        WPE_AssemblePacket(&data_ptr[34], 4, 0xc0440000);/* da ip */
        WPE_AssemblePacket(&data_ptr[38], 4, 0x00190015);
        WPE_AssemblePacket(&data_ptr[42], 4, 0x001b77c0);
        WPE_AssemblePacket(&data_ptr[46], 4, 0x00a4c5ff);
     
        for(i = 50; i < size; i++)
        {
          data_ptr[i] = (i - 50);
        }
     
        printf("send packet 2:\n");
        WPE_HostSendPacket(pool, txch, data_ptr,  size);
    }
    return 0;
}

int CLI_PredefinedTest_BkgnTrafficPceRuleCreate(char *StrPrm)
{
/*
   a place holder. so far use ENET1 iw_port default flow_agg to forward to ENET2 port.
   if add rule, can use WPE_Eoam_Flow_AddFlowRule
*/
    return 0;
}

int CLI_PredefinedTest_PceInfoShow(char *StrPrm)
{
    printf("PceFilterStats\n");
    WPE_Eoam_Pce_ShowInfo(eoam_system.h_pce_filter[0]);
    WPE_Eoam_Pce_ShowInfo(eoam_system.h_pce_filter[1]);

    return 0;
}



