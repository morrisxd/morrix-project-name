/************************************************************************
**                                                                     **
**                         ALCATEL-LUCENT TELECOM                      **
**                                                                     **
************************************************************************/

/*************************** IDENTIFICATION *****************************
**
** Project          : MDU R2.2 
**
** Module           : NPU::LxFwd[::ETH Interfaces]
**
** File name        : WP_LxFwd_ETH_Init_Dbg.c
**
** Description      : ETH interfaces in the  LxFwd module 
**                     for the WP3 
**
** Reference        : 
** 
** Contact          : 
**
** Creation date    :
**
************************************************************************/
                                                                        
/*#######################################################################
 #                                                                      #
 #  COMPILER OPTIONS                                                    #
 #                                                                      #
 ######################################################################*/
                                                                                                                                                
/*#######################################################################
 #                                                                      #
 #  INCLUDES                                                            #
 #                                                                      #
 ######################################################################*/
#ifdef __cplusplus
extern "C"{
#endif

#include <types/basic_types.h>
#include <api/wp_wddi.h>
#include <error/error.h>
#include <debug/trace.h>
#include <debug/dispatch.h> 
#include <xapi/xapi.h>
#include <stdlib.h>
#include <assert/AsamAssert.h>

#include <WP_LxFwd_itfwrap_ETH/WP_LxFwd_ETH.h>
#include <include/WP_LxFwd_Eth_Internal.h>
#include <WP_LxFwd_itfwrap_RM/WP_LxFwd_Rm.h>
#include <EthLine_hww/EthHwwInterface.h>
#include <GenLxFwdWrapItf/lxFwd_protection.h>

extern void pmc_main (void);
extern void WPE_CLI_input (char InputBuf[256]);
#ifdef __cplusplus
}
#endif


/*#######################################################################
 #                                                                      #
 # DEFINES, MACROS                                                      #
 #                                                                      #
 ######################################################################*/

/*####################################################################### 
 #                                                                      #
 # EXTERN CONSTANT AND EXTERN VARIABLE DECLARATIONS (discouraged)       #
 #                                                                      #
 ######################################################################*/

/*#######################################################################
 #                                                                      #
 #  TYPE DEFINITIONS                                                    #
 #                                                                      #
 ######################################################################*/
u_int32 wp_eth_trace_g ;
/*#######################################################################
 #                                                                      #
 #  INTERFACE CONSTANT AND INTERFACE VARIABLE DEFINITIONS               #
 #                                                                      #
 ######################################################################*/

/*#######################################################################
 #                                                                      #
 #  STATIC CONSTANT AND STATIC VARIABLE DEFINITIONS                     #
 #                                                                      #
 ######################################################################*/

/*#######################################################################
 #                                                                      #
 # INTERNAL FUNCTION PROTOTYPES                                         #
 #                                                                      #
 ######################################################################*/

/*#######################################################################
 #                                                                      #
 #  INTERFACE FUNCTION DEFINITIONS                                      #
 #                                                                      #
 ######################################################################*/
static T_bool isValid_device(port_device_number deviceIdx)
{
    if(deviceIdx >= WT_MAX_NUM_OF_ENET_DEVS)  
    {
        return FALSE;
    }
    
    if((WT_lxFwd_GetPortIdx(deviceIdx)) >= WT_MAX_NUM_OF_ENET_PORTS)
    {
        return FALSE;
    }
    
    return TRUE;
}

static u_int32 WT_lxFwd_Eth_Debug_Enable( T_bool enable )
{
    g_eth_dbg_flag = enable ;
    return 0 ;
}

static void WT_lxFwd_Eth_debugHelpSetFlowControl(void)
{
    dbg_printf(" set flowcontrol <GE1|subtending> <disabled|enabled>\r\n");
}

static void WT_lxFwd_Eth_debugHelpSetInterfaceMode(void)
{
    dbg_printf(" set interfacemode <interface> <mode>\r\n");
    dbg_printf("     <interface: host|lt1a|lt1b|lt2a|lt2b|lt3a|lt3b|lt4a|lt4b|dsp|GE1|GE2|GPON1|GPON2|subtending>\r\n");
    dbg_printf("     <mode     : mii|rmii|gmii|tbi|smii|sssmii|rgmii10100|rgmii1000|rtbi|tbian|\r\n"
               "                 sgmii10|sgmii100|sgmii1000|sgmii2500|sgmiian|1000basex|1000basexan>\r\n");
}

static void WT_lxFwd_Eth_debugHelpSet(void)
{
    WT_lxFwd_Eth_debugHelpSetFlowControl();
    WT_lxFwd_Eth_debugHelpSetInterfaceMode();
}

static void WT_lxFwd_Eth_debugHelpShowFlowControl(void)
{
    dbg_printf(" show flowcontrol <GE1|subtending>\r\n");
}

static void WT_lxFwd_Eth_debugHelpShowInterfaceMode(void)
{
    dbg_printf(" show interfacemode <interface>\r\n");
    dbg_printf("      <interface: host|lt1a|lt1b|lt2a|lt2b|lt3a|lt3b|lt4a|lt4b|dsp|GE1|GE2|GPON1|GPON2|subtending>\r\n");
}

static void WT_lxFwd_Eth_debugHelpShowLinkStatus(void)
{
    dbg_printf(" show linkstatus <interface>\r\n");
    dbg_printf("      <interface: lt1a|lt1b|lt2a|lt2b|lt3a|lt3b|lt4a|lt4b|dsp|GE1|GE2|GPON1|GPON2|subtending>\r\n");
}

static void WT_lxFwd_Eth_debugHelpShow(void)
{
    WT_lxFwd_Eth_debugHelpShowFlowControl();
    WT_lxFwd_Eth_debugHelpShowInterfaceMode();
    WT_lxFwd_Eth_debugHelpShowLinkStatus();
}

static void WT_lxFwd_Eth_debugHelpWp3RegCmd(void)
{
    dbg_printf(" wp3 reg read cmd :wp3r <addr> \r\n");
    dbg_printf(" wp3 reg write cmd:wp3w <addr> <data>\r\n"); 
}

static void WT_lxFwd_Eth_Debug_Help(void)
{
    dbg_printf("WP3_Hww ETH Help Menu\n\r");
    dbg_printf("pmc_main\n\r");
    WT_lxFwd_Eth_debugHelpSet();
    WT_lxFwd_Eth_debugHelpShow();  
    WT_lxFwd_Eth_debugHelpWp3RegCmd(); 
    dbg_printf("\r\n");
    return;
}

static void WT_lxFwd_Eth_debugHelpErrorPrompt(WT_LXFWD_ETH_DEBUG_HELP_TYPE type_i)
{
    dbg_printf("No such command!\r\n");
    dbg_printf("Supported Command(s):\r\n");
    switch(type_i)
    {
    case WT_LXFWD_ETH_DEBUG_HELP_TYPE_ALL:
        WT_lxFwd_Eth_Debug_Help();
        break;
    case WT_LXFWD_ETH_DEBUG_HELP_TYPE_SET:
        WT_lxFwd_Eth_debugHelpSet();
        break;
    case WT_LXFWD_ETH_DEBUG_HELP_TYPE_SET_FLOWCONTROL:
        WT_lxFwd_Eth_debugHelpSetFlowControl();
        break;
    case WT_LXFWD_ETH_DEBUG_HELP_TYPE_SET_INTERFACEMODE:
        WT_lxFwd_Eth_debugHelpSetInterfaceMode();
        break;
    case WT_LXFWD_ETH_DEBUG_HELP_TYPE_SHOW:
        WT_lxFwd_Eth_debugHelpShow();
        break;
    case WT_LXFWD_ETH_DEBUG_HELP_TYPE_SHOW_FLOWCONTROL:
        WT_lxFwd_Eth_debugHelpShowFlowControl();
        break;
    case WT_LXFWD_ETH_DEBUG_HELP_TYPE_SHOW_INTERFACEMODE:
        WT_lxFwd_Eth_debugHelpShowInterfaceMode();
        break;
    case WT_LXFWD_ETH_DEBUG_HELP_TYPE_SHOW_LINKSTATUS:
        WT_lxFwd_Eth_debugHelpShowLinkStatus();
        break;
    default:
        break;
    }
}

static void WT_lxFwd_Eth_debugPrintResult(u_int32 rv)
{
    if(rv == 0)
        dbg_printf("Success.\r\n");
    else
        dbg_printf("Fail, rv=%d.\r\n", rv);
}

static void WT_lxFwd_Eth_debugSetFlowControl(char *str, int *index)
{
    char tempstr[32] = {0};
    WT_ETHERNETMGT_PORT_DEVICE_NUMBER type;
    WT_ETHERNETMGT_FLOW_CONTROL isEnable;

    if(dbg_scanstr(str,index,tempstr) != 0)
        goto WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SET_FLOWCONTROL;
    if(strcmp(tempstr,"GE1") == 0)
        type = WT_ETHERNETMGT_PORT_DEVICE_NTIO;
    else if(strcmp(tempstr,"subtending") == 0)
        type = WT_ETHERNETMGT_PORT_DEVICE_SUBTENDING;
    else
        goto WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SET_FLOWCONTROL;

    if(!isValid_device((port_device_number)type))    
    {
      dbg_printf("there is no %s used\r\n",tempstr);
      return;
    } 

    if(dbg_scanstr(str,index,tempstr) != 0)
        goto WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SET_FLOWCONTROL;
    if(strcmp(tempstr,"disabled") == 0)
        isEnable = WT_ETHERNETMGT_FLOW_DISABLE;
    else if(strcmp(tempstr,"enabled") == 0)
        isEnable = WT_ETHERNETMGT_FLOW_ENABLE;
    else
        goto WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SET_FLOWCONTROL;

    WT_lxFwd_SetNniFlowControl(type, isEnable);

    WT_lxFwd_Eth_debugPrintResult(0);

    return;

WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SET_FLOWCONTROL:
    WT_lxFwd_Eth_debugHelpErrorPrompt(WT_LXFWD_ETH_DEBUG_HELP_TYPE_SET_FLOWCONTROL);
}

static void WT_lxFwd_Eth_debugSetInterfaceMode(char *str, int *index)
{
    char tempstr[32] = {0};
    WP_U16 port_sys_idx;
    WT_ETHERNETMGT_INTERFACE_MODE mode;

    if(dbg_scanstr(str, index, tempstr) != 0)
        goto WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SET_INTERFACEMODE;
    if(strcmp(tempstr, "host") == 0)
        port_sys_idx = WT_ETHERNETMGT_PORT_DEVICE_HOST;
    else if(strcmp(tempstr, "lt1a") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE1;
    else if(strcmp(tempstr, "lt1b") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE2;
    else if(strcmp(tempstr, "lt2a") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE3;
    else if(strcmp(tempstr, "lt2b") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE4;
    else if(strcmp(tempstr, "lt3a") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE5;
    else if(strcmp(tempstr, "lt3b") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE6;
    else if(strcmp(tempstr, "lt4a") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE7;
    else if(strcmp(tempstr, "lt4b") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE8;
    else if(strcmp(tempstr, "dsp") == 0)
        port_sys_idx = WT_ETHERNETMGT_PORT_DEVICE_DSP;
    else if(strcmp(tempstr, "GE1") == 0)
        port_sys_idx = WT_ETHERNETMGT_PORT_DEVICE_NTIO;
    else if(strcmp(tempstr, "GE2") == 0)
        port_sys_idx = WT_ETHERNETMGT_PORT_DEVICE_NTIO_B; 
    else if(strcmp(tempstr, "subtending") == 0)   
        port_sys_idx = WT_ETHERNETMGT_PORT_DEVICE_SUBTENDING;
    else if(strcmp(tempstr, "GPON1") == 0)
        port_sys_idx = WT_ETHERNETMGT_PORT_DEVICE_GPON0;   
    else if(strcmp(tempstr, "GPON2") == 0)
        port_sys_idx = WT_ETHERNETMGT_PORT_DEVICE_GPON1;    
    else
        goto WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SET_INTERFACEMODE;

    if(!isValid_device((port_device_number)port_sys_idx))    
    {
      dbg_printf("there is no %s used\r\n",tempstr);
      return;
    } 

    if(dbg_scanstr(str, index, tempstr) != 0)
        goto WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SET_INTERFACEMODE;
    if(strcmp(tempstr,"mii") == 0)
        mode = WT_ETHERNETMGT_MII;
    else if(strcmp(tempstr,"rmii") == 0)
        mode = WT_ETHERNETMGT_RMII;
    else if(strcmp(tempstr,"gmii") == 0)
        mode = WT_ETHERNETMGT_GMII;
    else if(strcmp(tempstr,"tbi") == 0)
        mode = WT_ETHERNETMGT_TBI;
    else if(strcmp(tempstr,"smii") == 0)
        mode = WT_ETHERNETMGT_SMII;
    else if(strcmp(tempstr,"sssmii") == 0)
        mode = WT_ETHERNETMGT_SS_SMII;
    else if(strcmp(tempstr,"rgmii10100") == 0)
        mode = WT_ETHERNETMGT_RGMII_10_100;
    else if(strcmp(tempstr,"rgmii1000") == 0)
        mode = WT_ETHERNETMGT_RGMII_1000;
    else if(strcmp(tempstr,"rtbi") == 0)
        mode = WT_ETHERNETMGT_RTBI;
    else if(strcmp(tempstr,"tbian") == 0)
        mode = WT_ETHERNETMGT_TBI_AN;
    else if(strcmp(tempstr,"sgmii10") == 0)
        mode = WT_ETHERNETMGT_SGMII_10;
    else if(strcmp(tempstr,"sgmii100") == 0)
        mode = WT_ETHERNETMGT_SGMII_100;
    else if(strcmp(tempstr,"sgmii1000") == 0)
        mode = WT_ETHERNETMGT_SGMII_1000;
    else if(strcmp(tempstr,"sgmii2500") == 0)
        mode = WT_ETHERNETMGT_SGMII_2500;
    else if(strcmp(tempstr,"sgmiian") == 0)
        mode = WT_ETHERNETMGT_SGMII_AN;
    else if(strcmp(tempstr,"1000basex") == 0)
        mode = WT_ETHERNETMGT_1000_BASE_X;
    else if(strcmp(tempstr,"1000basexan") == 0)
        mode = WT_ETHERNETMGT_1000_BASE_X_AN;
    else if(strcmp(tempstr,"xaui") == 0)
        mode = WT_ETHERNETMGT_XAUI;
    else
        goto WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SET_INTERFACEMODE;

    WT_lxFwd_ModifyPortInterfaceMode(port_sys_idx, mode);

    WT_lxFwd_Eth_debugPrintResult(0);

    return;

WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SET_INTERFACEMODE:
    WT_lxFwd_Eth_debugHelpErrorPrompt(WT_LXFWD_ETH_DEBUG_HELP_TYPE_SET_INTERFACEMODE);
}

static void WT_lxFwd_Eth_debugShowFlowControl(char *str, int *index)
{
    char tempstr[32] = {0};
    WT_ETHERNETMGT_PORT_DEVICE_NUMBER type;
    WT_ETHERNETMGT_FLOW_CONTROL isEnable;

    if(dbg_scanstr(str,index,tempstr) != 0)
        goto WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SHOW_FLOWCONTROL;
    if(strcmp(tempstr,"GE1") == 0)
        type = WT_ETHERNETMGT_PORT_DEVICE_NTIO;
    else if(strcmp(tempstr,"subtending") == 0)
        type = WT_ETHERNETMGT_PORT_DEVICE_SUBTENDING;   
    else
        goto WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SHOW_FLOWCONTROL;

    if(!isValid_device((port_device_number)type))    
    {
      dbg_printf("there is no %s used\r\n",tempstr);
      return;
    } 
    
    WT_lxFwd_GetNniFlowControl(type, &isEnable);

    dbg_printf("%s's flow control status is %s.\r\n",
               type == WT_ETHERNETMGT_PORT_DEVICE_NTIO ? "GE1" : "Subtending",
               isEnable == WT_ETHERNETMGT_FLOW_DISABLE ? "disabled" : "enabled");

    return;

WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SHOW_FLOWCONTROL:
    WT_lxFwd_Eth_debugHelpErrorPrompt(WT_LXFWD_ETH_DEBUG_HELP_TYPE_SHOW_FLOWCONTROL);
}

static void WT_lxFwd_Eth_debugShowInterfaceMode(char *str, int *index)
{
    char tempstr[32] = {0};
    WP_U16 port_sys_idx;
    WT_ETHERNETMGT_INTERFACE_MODE mode;
    u_int8 typeString[16] = {0};

    if(dbg_scanstr(str,index,tempstr) != 0)
        goto WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SHOW_INTERFACEMODE;
    if(strcmp(tempstr, "host") == 0)
        port_sys_idx = WT_ETHERNETMGT_PORT_DEVICE_HOST;
    else if(strcmp(tempstr, "lt1a") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE1;
    else if(strcmp(tempstr, "lt1b") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE2;
    else if(strcmp(tempstr, "lt2a") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE3;
    else if(strcmp(tempstr, "lt2b") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE4;
    else if(strcmp(tempstr, "lt3a") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE5;
    else if(strcmp(tempstr, "lt3b") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE6;
    else if(strcmp(tempstr, "lt4a") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE7;
    else if(strcmp(tempstr, "lt4b") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE8;
    else if(strcmp(tempstr, "dsp") == 0)
        port_sys_idx = WT_ETHERNETMGT_PORT_DEVICE_DSP;
    else if(strcmp(tempstr, "GE1") == 0)
        port_sys_idx = WT_ETHERNETMGT_PORT_DEVICE_NTIO;
    else if(strcmp(tempstr, "GE2") == 0)
        port_sys_idx = WT_ETHERNETMGT_PORT_DEVICE_NTIO_B;        
    else if(strcmp(tempstr, "subtending") == 0)
        port_sys_idx = WT_ETHERNETMGT_PORT_DEVICE_SUBTENDING;
    else if(strcmp(tempstr, "GPON1") == 0)
        port_sys_idx = WT_ETHERNETMGT_PORT_DEVICE_GPON0;   
    else if(strcmp(tempstr, "GPON2") == 0)
        port_sys_idx = WT_ETHERNETMGT_PORT_DEVICE_GPON1;           
    else
        goto WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SHOW_INTERFACEMODE;
        
    if(!isValid_device((port_device_number)port_sys_idx))    
    {
      dbg_printf("there is no %s used\r\n",tempstr);
      return;
    }         

    mode = WT_lxFwd_GetPortInterfaceMode(port_sys_idx);

    switch(mode)
    {
    case WT_ETHERNETMGT_MII:
        strcpy(typeString, "MII");
        break;
    case WT_ETHERNETMGT_RMII:
        strcpy(typeString, "RMII");
        break;
    case WT_ETHERNETMGT_GMII:
        strcpy(typeString, "GMII");
        break;
    case WT_ETHERNETMGT_TBI:
        strcpy(typeString, "TBI");
        break;
    case WT_ETHERNETMGT_SMII:
        strcpy(typeString, "SMII");
        break;
    case WT_ETHERNETMGT_SS_SMII:
        strcpy(typeString, "SS_SMII");
        break;
    case WT_ETHERNETMGT_RGMII_10_100:
        strcpy(typeString, "RGMII_10_100");
        break;
    case WT_ETHERNETMGT_RGMII_1000:
        strcpy(typeString, "RGMII_1000");
        break;
    case WT_ETHERNETMGT_RTBI:
        strcpy(typeString, "RTBI");
        break;
    case WT_ETHERNETMGT_TBI_AN:
        strcpy(typeString, "TBI_AN");
        break;
    case WT_ETHERNETMGT_SGMII_10:
        strcpy(typeString, "SGMII_10");
        break;
    case WT_ETHERNETMGT_SGMII_100:
        strcpy(typeString, "SGMII_100");
        break;
    case WT_ETHERNETMGT_SGMII_1000:
        strcpy(typeString, "SGMII_1000");
        break;
    case WT_ETHERNETMGT_SGMII_2500:
        strcpy(typeString, "SGMII_2500");
        break;
    case WT_ETHERNETMGT_SGMII_AN:
        strcpy(typeString, "SGMII_AN");
        break;
    case WT_ETHERNETMGT_1000_BASE_X:
        strcpy(typeString, "1000_BASE_X");
        break;
    case WT_ETHERNETMGT_1000_BASE_X_AN:
        strcpy(typeString, "1000_BASE_X_AN");
        break;
    case WT_ETHERNETMGT_XAUI:
        strcpy(typeString, "XAUI");
        break;
    default:
        strcpy(typeString, "unknown");
    }

    dbg_printf("%s's interface mode is %s.\r\n", tempstr, typeString);

    return;

WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SHOW_INTERFACEMODE:
    WT_lxFwd_Eth_debugHelpErrorPrompt(WT_LXFWD_ETH_DEBUG_HELP_TYPE_SHOW_INTERFACEMODE);
}

static void WT_lxFwd_Eth_debugShowLinkStatus(char *str, int *index)
{
    char tempstr[32] = {0};
    WT_ETHERNETMGT_PORT_DEVICE_NUMBER port_sys_idx;
    int32 status, rv;
    T_GePortLogicalID logicalId;

    if(dbg_scanstr(str,index,tempstr) != 0)
        goto WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SHOW_LINKSTATUS;
    if(strcmp(tempstr, "lt1a") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE1;
    else if(strcmp(tempstr, "lt1b") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE2;
    else if(strcmp(tempstr, "lt2a") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE3;
    else if(strcmp(tempstr, "lt2b") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE4;
    else if(strcmp(tempstr, "lt3a") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE5;
    else if(strcmp(tempstr, "lt3b") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE6;
    else if(strcmp(tempstr, "lt4a") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE7;
    else if(strcmp(tempstr, "lt4b") == 0)
        port_sys_idx = WT_ETHERNETMGT_LT_PORT_DEVICE8;
    else if(strcmp(tempstr, "dsp") == 0)
        port_sys_idx = WT_ETHERNETMGT_PORT_DEVICE_DSP;
    else if(strcmp(tempstr, "GE1") == 0)
        port_sys_idx = WT_ETHERNETMGT_PORT_DEVICE_NTIO;
    else if(strcmp(tempstr, "GE2") == 0)
        port_sys_idx = WT_ETHERNETMGT_PORT_DEVICE_NTIO_B;
    else if(strcmp(tempstr, "GPON1") == 0)
        port_sys_idx = WT_ETHERNETMGT_PORT_DEVICE_GPON0;   
    else if(strcmp(tempstr, "GPON2") == 0)
        port_sys_idx = WT_ETHERNETMGT_PORT_DEVICE_GPON1;              
    else if(strcmp(tempstr, "subtending") == 0)
        port_sys_idx = WT_ETHERNETMGT_PORT_DEVICE_SUBTENDING;
    else
        goto WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SHOW_LINKSTATUS;

    if(!isValid_device((port_device_number)port_sys_idx))    
    {
      dbg_printf("there is no %s used\r\n",tempstr);
      return;
    }  

    switch(port_sys_idx)
    {
    case WT_ETHERNETMGT_PORT_DEVICE_NTIO:
    {    
         logicalId = GEPort1;
         rv = ethHww_getNniLinkState(logicalId, &status);
         if(rv == ETH_HWW_OK)   
             dbg_printf("%s's link status is %s.\r\n", 
                 tempstr, status == ETH_HWW_LINK_UP ? "up" : "down");
         else
             dbg_printf("Get %s's link status return %d.\r\n", 
                 tempstr, rv);
         break;     
    } 

    case WT_ETHERNETMGT_PORT_DEVICE_NTIO_B:
    {    
         logicalId = GEPort2;
         rv = ethHww_getNniLinkState(logicalId, &status);
         if(rv == ETH_HWW_OK)   
             dbg_printf("%s's link status is %s.\r\n", 
                 tempstr, status == ETH_HWW_LINK_UP ? "up" : "down");
         else
             dbg_printf("Get %s's link status return %d.\r\n", 
                 tempstr, rv);
         break;     
    }             
                     
    case WT_ETHERNETMGT_PORT_DEVICE_SUBTENDING:
    {
        logicalId = SubtendingPort;
        rv = ethHww_getNniLinkState(logicalId, &status);
        if(rv == ETH_HWW_OK)   
            dbg_printf("%s's link status is %s.\r\n", 
                tempstr, status == ETH_HWW_LINK_UP ? "up" : "down");
        else
            dbg_printf("Get %s's link status return %d.\r\n", 
                tempstr, rv);    
        break;                   
    }        
    
    default:
        status = WT_lxFwd_GetLinkStatus(port_sys_idx);
        dbg_printf("%s's link status is %s.\r\n",
           tempstr, status == WT_ETHERNETMGT_LINK_UP? "up" : "down");
        break;
    }
    return;

WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SHOW_LINKSTATUS:
    WT_lxFwd_Eth_debugHelpErrorPrompt(WT_LXFWD_ETH_DEBUG_HELP_TYPE_SHOW_LINKSTATUS);
}

static void wp3_reg_read_cmd(char *str, int *idx)
{
    unsigned int addr = 0;
    u_int32 data = 0;
    unsigned int rv = 0;

    rv = dbg_scanhex(str, idx, &addr);
    if (rv != 0)
    {
        WT_lxFwd_Eth_debugHelpWp3RegCmd();
        return ;
    }
    
    WT_lxFwd_Wp3RegRead((u_int32)addr, &data);
    
    dbg_printf("wp3_reg_read_cmd:addr:0x%x,data:0x%x.\r\n", addr, data);
       
    return ;
}

static void wp3_reg_write_cmd(char *str, int *idx)
{
    unsigned int addr = 0;
    unsigned int data = 0;
    unsigned int rv = 0;

    rv = dbg_scanhex(str, idx, &addr);
    if ( rv != 0)
    {
        WT_lxFwd_Eth_debugHelpWp3RegCmd();
        return ;
    }
    
    rv = dbg_scanhex(str, idx, &data);
    if ( rv !=0)
    {
        WT_lxFwd_Eth_debugHelpWp3RegCmd();
        return ;
    }
    
    WT_lxFwd_Wp3RegWrite((u_int32)addr, (u_int32)data);
    
    dbg_printf("wp3_reg_write_cmd:addr:0x%x,data:0x%x.\r\n", addr, data);
    
    return ;
}

static u_int32 WT_lxFwd_Eth_Debug_Cmd(char *str, int *index, char *cmd)
{  
    char tempstr[32] = {0};
    
//    if(WT_Hww_Semaphore_Lock()!= E_lxFwd_Err_NoError)
//    {
//        dbg_printf("WT_Hww_Semaphore_Lock failed\r\n") ;
//        return 0;
//    }

    WT_lxFwd_Eth_Debug_Enable(TRUE);
    
    do 
    {
        if (strcmp(cmd, "pmc_main") == 0)
        {
         pmc_main();
         break;   
        }  
        else if (strcmp(cmd, "pmc_show") == 0)
        {
         if(dbg_scanstr(str, index, tempstr) == 0)
         {
            WPE_CLI_input(tempstr);
         }
         break;   
        }                
        else if (strcmp(cmd, "set") == 0)
        {
            if(dbg_scanstr(str, index, tempstr) != 0)
                goto WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SET;
    
            if(strcmp(tempstr, "flowcontrol") == 0)
            {
                WT_lxFwd_Eth_debugSetFlowControl(str,index);
                break;
            }
            if(strcmp(tempstr, "interfacemode") == 0)
            {
                WT_lxFwd_Eth_debugSetInterfaceMode(str,index);
                break;
            }
            else
                goto WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SET;
    
    WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_SET:
            WT_lxFwd_Eth_debugHelpErrorPrompt(WT_LXFWD_ETH_DEBUG_HELP_TYPE_SET);
            break;
        }
        else if (strcmp(cmd ,"show") == 0)
        {
            if(dbg_scanstr(str, index, tempstr) != 0)
                goto WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_GET;
    
            if(strcmp(tempstr, "flowcontrol") == 0)
            {
                WT_lxFwd_Eth_debugShowFlowControl(str,index);
                break;
            }
            if(strcmp(tempstr, "interfacemode") == 0)
            {
                WT_lxFwd_Eth_debugShowInterfaceMode(str,index);
                break;
            }
            if(strcmp(tempstr, "linkstatus") == 0)
            {
                WT_lxFwd_Eth_debugShowLinkStatus(str,index);
                break;
            }
            else
                goto WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_GET;
           
    WT_LXFWD_ETH_DEBUG_HELP_ERROR_PROMPT_GET:
            WT_lxFwd_Eth_debugHelpErrorPrompt(WT_LXFWD_ETH_DEBUG_HELP_TYPE_SHOW);
            break;
        }
        else if (strcmp(cmd ,"wp3r") == 0)
        {
            wp3_reg_read_cmd(str, index);
            break ;
        }  
        else if (strcmp(cmd ,"wp3w") == 0)
        {
            wp3_reg_write_cmd(str, index);
            break ;
        }  
      
    } while (0);

//        WT_Hww_Semaphore_Unlock();
        WT_lxFwd_Eth_Debug_Enable(FALSE);
        return 1;     
}

void WT_lxFwd_Eth_Init(void)
{   
    assert_r(dbg_link("wteth", WT_lxFwd_Eth_Debug_Help, WT_lxFwd_Eth_Debug_Cmd) \
        == ERR_NO_ERROR) ;

    assert_r(trc_register("WTETH",100,&wp_eth_trace_g) == ERR_NO_ERROR);
    
    return;
}

