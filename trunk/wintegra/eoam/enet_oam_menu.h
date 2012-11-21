/*****************************************************************************
 * (C) Copyright 2007, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

#ifndef EOAM_MENU_H
#define EOAM_MENU_H


/*****************************************************************************/
/*                       Menu defines                                        */
/*****************************************************************************/

#ifdef OK
#undef OK
#endif

#define OK     0
#define FAIL   -1
#ifndef ERROR
#define ERROR  1
#endif

#ifdef TRUE
#undef TRUE
#endif

#define TRUE   1
#define FALSE  0

#define CLI_INPUT_BUFF_LEN      128
#define MAIN_MENU_ITEM_NUM      12
#define MA_MENU_ITEM_NUM        3
#define MEP_MENU_ITEM_NUM       4
#define CCM_MENU_ITEM_NUM       3
#define LM_MENU_ITEM_NUM        4
#define DM_MENU_ITEM_NUM        6
#define FLOW_MENU_ITEM_NUM      7
#define MENU_ITEM_ONLY_ONE      1
#define PM_MENU_ITEM_NUM        4
#define PD_MENU_ITEM_NUM        3
#define PT_MENU_ITEM_NUM        16

#define MENU_FREQ_RATE_MIN      1
#define MENU_FREQ_RATE_MAX      7

#define STR_STAR           "******************************************************************"
#define STR_HOT_KEYS       "  Hot keys: m=Main Menu, u=Upper Menu, d=Display Cur Menu, q=Quit "
#define STR_ENTER_COMMAND  "# Enter your command:"
#define STR_MENU_ERR       "Err> Wrong menu command"

#define HOTKEY_MAIN_MENU        'm'
#define HOTKEY_UPPER_MENU       'u'
#define HOTKEY_CUR_MENU         'd'
#define HOTKEY_QUIT_PROG        'q'
#define HOTKEY_REBOOT           'r'

typedef int  (*Y_MenuLeafP) (char *StrPrm);

typedef void (*Y_MenuHintP) (char *StrPrm);

typedef enum
{
    K_Menu,
    K_Info,
    K_Cli,
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
    Y_MenuHintP Help;
} Y_MenuEntry;

/******************************* MENU API *************************/
extern char* E_freq_rate[];
void *CLI_T_Main(void* arg);
static int CLI_MAIN_Reboot(char *StrPrm);
static int CLI_MAIN_QuitAndKeepalive(char *StrPrm);
int CLI_MAIN_Quit(char *StrPrm);
void F_SetMainMenu (void);
void F_SetUpperMenu (void);
void F_DisplayHeader (void);
void F_DisplayMenu (void);
void F_MenuEngine (char *InputBuf);
void F_EvaluateCommand (char *InputBuf);
int F_EvaluateHotKey (char *InputBuf);
int F_GetInputString (void);
static int CLI_Debug_Level(char *StrPrm);

extern Y_MenuEntry PM_menu[];
int CLI_PM_ShowStats(char *StrPrm);
int CLI_PM_SetPortLoopback(char *StrPrm);

extern Y_MenuEntry MA_menu[];
extern Y_MenuEntry V_MA_CreateMAMenu[];
extern Y_MenuEntry V_MA_DeleteMAMenu[];

static int CLI_MA_ShowInfo (char *StrPrm);
int CLI_MA_CreateMA (char *StrPrm);
int CLI_MA_DeleteMA (char *StrPrm);
void CLI_MA_CreateMA_Help (char *StrPrm);
void CLI_MA_DeleteMA_Help (char *StrPrm);

extern Y_MenuEntry MEP_menu[];
extern Y_MenuEntry V_MEP_CreateRemoteMEPMenu[];
extern Y_MenuEntry V_MEP_DeleteRemoteMEPMenu[];

int CLI_MSG_ShowInfo(char *StrPrm);
int CLI_MEP_ShowInfo (char *StrPrm);
int CLI_MEP_CreateRemoteMEP (char *StrPrm);
int CLI_MEP_DeleteRemoteMEP (char *StrPrm);
void CLI_MEP_CreateRemoteMEP_Help (char *StrPrm);
void CLI_MEP_DeleteRemoteMEP_Help (char *StrPrm);

extern Y_MenuEntry CCM_menu[];
extern Y_MenuEntry V_CCM_EnableTxCCM[];
extern Y_MenuEntry V_CCM_DisableTxCCM[];

int CLI_CCM_GetCCMStatus (char *StrPrm);
int CLI_CCM_EnableTxCCM(char *StrPrm);
int CLI_CCM_DisableTxCCM(char *StrPrm);
void CLI_CCM_EnableTxCCM_Help(char *StrPrm);
void CLI_CCM_DisableTxCCM_Help(char *StrPrm);

extern Y_MenuEntry LM_menu[];
extern Y_MenuEntry V_LM_EnableLMM[];
extern Y_MenuEntry V_LM_DisableLMM[];
extern Y_MenuEntry V_LM_GetPMStats[];
extern Y_MenuEntry V_LM_GetLossRatio[];

int CLI_LM_EnableLMM (char *StrPrm);
int CLI_LM_DisableLMM (char *StrPrm);
int CLI_LM_GetPMStats (char *StrPrm);
int CLI_LM_GetLossRatio (char *StrPrm);
void CLI_LM_EnableLMM_Help (char *StrPrm);
void CLI_LM_DisableLMM_Help (char *StrPrm);
void CLI_LM_GetPMStats_Help (char *StrPrm);
void CLI_LM_GetLossRatio_Help (char *StrPrm);

extern Y_MenuEntry DM_menu[];
extern Y_MenuEntry V_DM_EnableDM1[];
extern Y_MenuEntry V_DM_DisableDM1[];
extern Y_MenuEntry V_DM_EnableDMM[];
extern Y_MenuEntry V_DM_DisableDMM[];
extern Y_MenuEntry V_DM_GetDMCounters[];
extern Y_MenuEntry V_DM_CalculateDelay[];

int CLI_DM_EnableDM1 (char *StrPrm);
int CLI_DM_DisableDM1 (char *StrPrm);
int CLI_DM_EnableDMM (char *StrPrm);
int CLI_DM_DisableDMM (char *StrPrm);
int CLI_DM_GetDMCounters (char *StrPrm);
int CLI_DM_CalculateDelay (char *StrPrm);
void CLI_DM_EnableDM1_Help (char *StrPrm);
void CLI_DM_DisableDM1_Help (char *StrPrm);
void CLI_DM_EnableDMM_Help (char *StrPrm);
void CLI_DM_DisableDMM_Help (char *StrPrm);
void CLI_DM_GetDMCounters_Help (char *StrPrm);
void CLI_DM_CalculateDelay_Help (char *StrPrm);

extern Y_MenuEntry FLOW_menu[];
extern Y_MenuEntry V_FLOW_CreateFlow[];
extern Y_MenuEntry V_FLOW_DeleteFlow[];
extern Y_MenuEntry V_FLOW_EnableTXFCLCount[];
extern Y_MenuEntry V_FLOW_DisableTXFCLCount[];
extern Y_MenuEntry V_FLOW_EnableRXFCLCount[];
extern Y_MenuEntry V_FLOW_DisableRXFCLCount[];

int CLI_FLOW_ShowInfo (char *StrPrm);
int CLI_FLOW_CreateFlow (char *StrPrm);
int CLI_FLOW_DeleteFlow (char *StrPrm);
int CLI_FLOW_EnableTXFCLCount (char *StrPrm);
int CLI_FLOW_DisableTXFCLCount (char *StrPrm);
int CLI_FLOW_EnableRXFCLCount (char *StrPrm);
int CLI_FLOW_DisableRXFCLCount (char *StrPrm);
void CLI_FLOW_ShowInfo_Help (char *StrPrm);
void CLI_FLOW_CreateFlow_Help (char *StrPrm);
void CLI_FLOW_DeleteFlow_Help (char *StrPrm);
void CLI_FLOW_EnableTXFCLCount_Help (char *StrPrm);
void CLI_FLOW_DisableTXFCLCount_Help (char *StrPrm);
void CLI_FLOW_EnableRXFCLCount_Help (char *StrPrm);
void CLI_FLOW_DisableRXFCLCount_Help (char *StrPrm);

extern Y_MenuEntry PD_menu[];

int CLI_Performance_TestStart(char *StrPrm);
int CLI_Performance_TestExit(char *StrPrm);
int CLI_Performance_TestCheckUtil(char *StrPrm);

extern Y_MenuEntry PT_menu[];
int CLI_PredefinedTest_CcmSetup(char *StrPrm);
int CLI_PredefinedTest_CcmRxConnTrigger(char *StrPrm);
int CLI_PredefinedTest_CcmRxConnReturnTrigger(char *StrPrm);
int CLI_PredefinedTest_CcmInfoShow(char *StrPrm);
int CLI_PredefinedTest_CcmTeardown(char *StrPrm);
int CLI_PredefinedTest_LmSetup(char *StrPrm);
int CLI_PredefinedTest_LmRemoteLossSimulate(char *StrPrm);
int CLI_PredefinedTest_LmInfoShow(char *StrPrm);
int CLI_PredefinedTest_LmTearDown(char *StrPrm);
int CLI_PredefinedTest_Dm1Setup(char *StrPrm);
int CLI_PredefinedTest_Dm1TearDown(char *StrPrm);
int CLI_PredefinedTest_DmmSetup(char *StrPrm);
int CLI_PredefinedTest_DmmTearDown(char *StrPrm);
int CLI_PredefinedTest_DmInfoShow(char *StrPrm);
int CLI_PredefinedTest_BkgnPacketHostSend(char *StrPrm);
int CLI_PredefinedTest_PceInfoShow(char *StrPrm);

#endif//EOAM_MENU_H


