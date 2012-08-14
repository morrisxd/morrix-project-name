#ifndef __BRIDGE_MENU_H__
#define __BRIDGE_MENU_H__


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
#define MAIN_MENU_ITEM_NUM      10
#define FHT_MENU_ITEM_NUM       9
#define VLAN_MENU_ITEM_NUM      4
#define MC_MENU_ITEM_NUM        5
#define AGING_MENU_ITEM_NUM     4
#define LEARNING_MENU_ITEM_NUM  4
#define MENU_ITEM_ONLY_ONE      1
#define STAT_MENU_ITEM_NUM      6
#define PERF_MENU_ITEM_NUM      2

#define IPV4_MENU_ITEM_NUM       9//Phenix


#define STR_STAR           "******************************************************************"
#define STR_HOT_KEYS       "  Hot keys: m=Main Menu, u=Upper Menu, d=Display Cur Menu, q=Quit "
#define STR_ENTER_COMMAND  "# Enter your command:"
#define STR_MENU_ERR       "Err> Wrong menu command"

#define HOTKEY_MAIN_MENU        'm'
#define HOTKEY_UPPER_MENU       'u'
#define HOTKEY_CUR_MENU         'd'
#define HOTKEY_QUIT_PROG        'q'
#define HOTKEY_REBOOT           'r'

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

extern Y_MenuEntry STAT_menu[];
int CLI_STAT_ShowDevStats(char *StrPrm);
int CLI_STAT_ClearDevStats(char *StrPrm);
int CLI_STAT_ShowBrgPrtStats(char *StrPrm);
int CLI_STAT_ClearBrgPrtStats(char *StrPrm);
int CLI_STAT_ShowFlwAggStats(char *StrPrm);
int CLI_STAT_ClearFlwAggStats(char *StrPrm);

int CLI_STAT_PrintPolicerStatistics(void);//phenix

int CLI_STAT_ResetPolicerStatistics(void);//phenix

extern Y_MenuEntry Init_configuration_menu[];

int CLI_VLAN_Show_Vlan(char *StrPrm);
int CLI_VLAN_Create_Vlan(char *StrPrm);
int CLI_VLAN_Delete_Vlan(char *StrPrm);
int CLI_VLAN_Set_Default_Tag (char *StrPrm);

int CLI_MC_Show_Muticast_group(char *StrPrm);
int CLI_MC_Create_Muticast_group(char *StrPrm);
int CLI_MC_Add_Muticast_member(char *StrPrm);
int CLI_MC_Delete_Muticast_group(char *StrPrm);
int CLI_MC_Remove_Muticast_member(char *StrPrm);

int CLI_Enable_aging(char *StrPrm);
int CLI_Disable_aging(char *StrPrm);
int CLI_Set_aging_period(char *StrPrm);
int CLI_Set_aging_weight(char *StrPrm);

int CLI_Enable_learning(char *StrPrm);
int CLI_Disable_learning(char *StrPrm);
int CLI_Enable_port_lrn(char *StrPrm);
int CLI_Disable_port_lrn(char *StrPrm);
int CLI_Create_Lrn_FlwAgg_rule(char *StrPrm);
int CLI_Delete_Lrn_FlwAgg_rule(char *StrPrm);

int CLI_Dump_FDB_entry(char *StrPrm);
int CLI_Flush_FDB_entry_by_Vlan(char *StrPrm);

extern Y_MenuEntry FH_Test_menu[];
extern Y_MenuEntry VLAN_menu[];
extern Y_MenuEntry MC_menu[];
extern Y_MenuEntry AGING_menu[];
extern Y_MenuEntry LEARNING_menu[];
extern Y_MenuEntry Performance_menu[];
extern Y_MenuEntry IPV4_menu[];//Phenix

int CLI_BC_Suppress_Enable(char *StrPrm);//Phenix
int CLI_BC_Suppress_Disable (char *StrPrm);//Phenix
int CLI_Unknown_Unicast_Suppress_Enable (char *StrPrm);//Phenix
int CLI_Unknown_Unicast_Suppress_Disable (char *StrPrm);//Phenix
int CLI_VlanCos_Trans (char *StrPrm);//Phenix
int CLI_MacBinding_Create(char *StrPrm);//Phenix

int CLI_Performance_TestStart(char *StrPrm);
int CLI_Performance_TestExit(char *StrPrm);

int CLI_WinUtil(char *StrPrm);

int CLI_FHT_IPV6_filter(char *StrPrm);
int CLI_FHT_L4_port_filter(char *StrPrm);
int CLI_FHT_L4_subtype_filter(char *StrPrm);
int CLI_FHT_Reserved_Mac_filter(char *StrPrM);
int CLI_FHT_Max_Learned_Mac(char *StrPrm);

int CLI_FHT_Case34_UcBc2Prio (char *StrPrm);
int CLI_FHT_Case35_SmacDmacSipDipTos2SVlan (char *StrPrm);
int CLI_FHT_Case36_Vlan2Vlan (char *StrPrm);
int CLI_FHT_Case37_Tag_Stack_Tunneling (char *StrPrm);

int CLI_Send_Packets (char *StrPrm);
#endif
