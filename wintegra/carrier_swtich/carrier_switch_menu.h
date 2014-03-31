#ifndef __CARRIER_SWITCH_MENU_H__
#define __CARRIER_SWITCH_MENU_H__


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
#define PORT_MENU_ITEM_NUM      3

#define VPN_MENU_ITEM_NUM       5
#define SUBPORT_MENU_ITEM_NUM   4
#define AGING_MENU_ITEM_NUM     4
#define VPNPORT_MENU_ITEM_NUM   4
#define MENU_ITEM_ONLY_ONE      1
#define VPSW_FLOW_MENU_ITEM_NUM      3
#define L2VPN_FLOW_MENU_ITEM_NUM      4
#define L3VPN_FLOW_MENU_ITEM_NUM      3
#define POLICER_MENU_ITEM_NUM      3
#define GPMT_MENU_ITEM_NUM      1
#define STAT_MENU_ITEM_NUM      12



#define STR_STAR           "******************************************************************"
#define STR_HOT_KEYS       "  Hot keys: m=Main Menu, u=Upper Menu, d=Display Cur Menu, q=Quit "
#define STR_ENTER_COMMAND  "# Enter your command:"
#define STR_MENU_ERR       "Err> Wrong menu command"

#define HOTKEY_MAIN_MENU        'm'
#define HOTKEY_UPPER_MENU       'u'
#define HOTKEY_CUR_MENU         'd'
#define HOTKEY_QUIT_PROG        'q'
#define HOTKEY_REBOOT           'R'


#define KEY_BACKSPACE           8
#define KEY_ENTER               10

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

/******************************* MENU API *************************/
void *CLI_T_Main(void* arg);
static int CLI_MAIN_Reboot(char *StrPrm);
int CLI_MAIN_Quit(char *StrPrm);
void F_SetMainMenu (void);
void F_SetUpperMenu (void);
void F_DisplayHeader (void);
void F_DisplayMenu (void);
void F_MenuEngine (char *InputBuf);
void F_EvaluateCommand (char *InputBuf);
int F_EvaluateHotKey (char *InputBuf);
int F_GetInputString (void);
int CLI_Show_System_Information(char *StrPrm);

extern Y_MenuEntry Port_menu[];
extern Y_MenuEntry VPN_menu[];
extern Y_MenuEntry SubPort_menu[];
extern Y_MenuEntry VPN_Port_menu[];
extern Y_MenuEntry L2VPN_Flow_menu[];
extern Y_MenuEntry L3VPN_Flow_menu[];
extern Y_MenuEntry Policer_menu[];
extern Y_MenuEntry GPMT_menu[];
extern Y_MenuEntry STAT_menu[];

int CLI_PORT_Show_Port_Info (char *StrPrm);
int CLI_PORT_Set_Port_Mode(char *StrPrm);
int CLI_PORT_Set_AF_Mode(char *StrPrm);

int CLI_SUBPORT_Show_SubPort_Info(char *StrPrm);
int CLI_SUBPORT_Create_SubPort(char *StrPrm);
int CLI_SUBPORT_Delete_SubPort(char *StrPrm);
int CLI_SUBPORT_Set_AF_Mode(char *StrPrm);

int CLI_VPNPORT_Show_VPN_port_info(char *StrPrm);
int CLI_VPNPORT_Create_UNI_VPN_port(char *StrPrm);
int CLI_VPNPORT_Create_NNI_VPN_port(char *StrPrm);
int CLI_VPNPORT_Delete_VPN_port(char *StrPrm);

int CLI_VPN_Show_VPN(char *StrPrm);
int CLI_VPN_Create_L2VPN(char *StrPrm);
int CLI_VPN_Create_L3VPN(char *StrPrm);
int CLI_VPN_Create_VPWS(char *StrPrm);
int CLI_VPN_Delete_VPN(char *StrPrm);


int CLI_L3VPN_Show_L3VPN_Flow_info(char *StrPrm);
int CLI_L3VPN_Create_L3VPN_flow(char *StrPrm);
int CLI_L3VPN_Delete_L3VPN_flow(char *StrPrm);
int CLI_L2VPN_Show_Static_MAC_info(char *StrPrm);
int CLI_L2VPN_Insert_Static_MAC(char *StrPrm);
int CLI_L2VPN_Delete_Static_MAC(char *StrPrm);
int CLI_L2VPN_Delete_Learnt_MAC(char *StrPrm);

int CLI_STAT_ShowDevStats(char *StrPrm);
int CLI_STAT_ClearDevStats(char *StrPrm);
int CLI_STAT_ShowBrgPrtStats(char *StrPrm);
int CLI_STAT_ClearBrgPrtStats(char *StrPrm);
int CLI_STAT_ShowFlwAggStats(char *StrPrm);
int CLI_STAT_ClearFlwAggStats(char *StrPrm);
int CLI_STAT_ShowPceFilterStats(char *StrPrm);
int CLI_STAT_ClearPceFilterStats(char *StrPrm);
int CLI_STAT_ShowPolicerStats(char *StrPrm);
int CLI_STAT_ClearPolicerStats(char *StrPrm);
int CLI_STAT_ShowFloodingStats(char *StrPrm);
int CLI_STAT_ClearFloodingStats(char *StrPrm);


int CLI_POLICER_Show_Policer(char *StrPrm);
int CLI_POLICER_Create_Policer(char *StrPrm);
int CLI_POLICER_Delete_Policer(char *StrPrm);

int CLI_GPMT_Show_Info(char *StrPrm);
int CLI_Display(char *StrPrm);

int CLI_Performance_TestStart(char *StrPrm);
int CLI_Performance_TestExit(char *StrPrm);

int CLI_WinUtil(char *StrPrm);

#endif//__CARRIER_SWITCH_MENU_H__
