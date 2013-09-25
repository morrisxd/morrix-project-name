#ifndef _WT_NEW_AUTOMATION_C_
#define _WT_NEW_AUTOMATION_C_
/**********************************************************************************
 * (C) Copyright 2001-2007, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 **********************************************************************************/

/**********************************************************************************
 *
 * File: wt_new_automation.c
 *
 * Purpose:Allows interactive tests to run both in manual and automation mode.
 *
 * Functions contained in this file:
 * +---------------------------+---------------------------------------------------
 * |  Function name                 | Description
 * +--------------------------------+----------------------------------------------
 * |
 * |                        MENU API FUNCTIONS
 * |
 * |  -----------------------------------------------------------------------------
 * |                                |
 * |  F_MenuEngine                  | receives a string as input
 * |                                | and applies command or evaluates hot key.
 * |  ------------------------------+----------------------------------------------
 * |  F_EvaluateHotKey              | receives a string holding a
 * |                                | hot key and changes the current menu
 * |  ------------------------------+----------------------------------------------
 * |  F_EvaluateCommand             | handles menu switching according
 * |                                | to input string or executes demo function.
 * |  ------------------------------+----------------------------------------------
 * |  F_SetMainMenu                 | sets the main menu as the
 * |                                | current menu.
 * |  ------------------------------+----------------------------------------------
 * |  F_SetDebugMenu                | sets the debug menu as the
 * |                                | current menu.
 * |  ------------------------------+----------------------------------------------
 * |  F_SetUpperMenu                | sets the upper menu as the
 * |                                | current menu.
 * |  ------------------------------+----------------------------------------------
 * |  F_DisplayMenu                 | prints the current menu to the screen.
 * |                                | to the screen.
 * |  ------------------------------+----------------------------------------------
 * |  F_DisplayHeader               | prints the menu headers to the screen.
 * |                                |
 * |  ------------------------------+----------------------------------------------
 * |
 * |                        Automation Functions
 * |
 * |  ------------------------------+----------------------------------------------
 * |  WT_initSocketConnection       | Opens socket, binds it and listens on it
 * |                                |
 * |  ------------------------------+----------------------------------------------
 * |  WT_acceptConnections          | Performs accept on socket
 * |                                |
 * |  ------------------------------+----------------------------------------------
 * |  WT_Gets                       | remembers the menu command inserted by user
 * |                                | and type the sequenceof commands when user
 * |                                | requests
 * |  ------------------------------+----------------------------------------------
 * |  EvaluateControlString         | Uses RetrieveFunction() and RetrieveParams()
 * |                                | to retrieve function and parameters from the
 * |                                | control string and applies the function to
 * |                                | its parameters
 * |  ------------------------------+----------------------------------------------
 * |  FillAutomationArray           | Fills global array of string
 * |                                |
 * |  ------------------------------+----------------------------------------------
 * |  WT_processPacketCommand       | Processes incoming automation packets
 * |                                |
 * |  ------------------------------+----------------------------------------------
 * |  WT_serverUI                   | Performs packet receive
 * |                                |
 * |  ------------------------------+----------------------------------------------
 * |  WT_myerr                      | Prints error messages and terminates
 * |                                |
 * |  ------------------------------+----------------------------------------------
 * |  WT_receivePacket              | Performs buffer receive
 * |                                |
 * |  ------------------------------+----------------------------------------------
 * |  WT_sendPacket                 | Performs buffer send
 * |                                |
 * |  ------------------------------+----------------------------------------------
 * |  WT_WaitForAck                 | send buffer to control and wait for
 * |                                | acknowledge
 * |  ------------------------------+----------------------------------------------
 * |  TranslateAsciiToHex           | translates data from ascii to hexadecimal
 * |                                |
 * |  ------------------------------+----------------------------------------------
 * |  TranslateHexToAscii           | translates data from hexadecimal to ascii
 * |                                |
 **********************************************************************************/

/* External includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <errno.h>
#include <ctype.h>

#ifdef VXWORKS
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sockLib.h>

#elif __linux__

#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#else

#ifdef AUTOMATION_MODE

#define LWIP_COMPAT_SOCKETS

#include "lwip/debug.h"
#include "lwip/opt.h"

#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/sys.h"

#include "lwip/stats.h"
#include "lwip/tcpip.h"

#include "netif/loopif.h"

#include "lwip/ip_addr.h"

#ifdef WP_HW_WINPATH1
#define __asix__
#elif defined WP_HW_WINPATH2
#define __dm9k__
#elif defined WP_HW_WINPATH3
#define __wintegra__
#else
#error "no external processor is defined"
#endif

#ifdef __asix__
#include "netif/asixif.h"
extern void asixif_int_handler(void);
#elif defined __dm9k__
#include "netif/dm9kif.h"
WP_U8 debug_ior(int);

extern void dm9kif_int_handler(void);
#elif defined __wintegra__
#include "netif/winEthIf.h"
#endif
#endif

#endif

#include "wp_wddi.h"
/*****************************************************************************
 * (C) Copyright 2000-2003, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/
/*****************************************************************************
 *
 * Module: $RCSfile: web.h,v $
 * Purpose:
 *
 *
 * Module Originator: Arnon Mordoh
 * Creation Date: Aug 19th, 2001
 *
 * Change History:
 * +------------+--------------------+----------------------------------------
 * | Date       | By                 | Description
 * +------------+--------------------+----------------------------------------
 * |            |                    |
 * +------------+--------------------+----------------------------------------
 * |            |                    |
 * +------------+--------------------+----------------------------------------
 *
 **************************************************************************/

/************************************************************************
 *  WEB physical address MAP and memory controllers settings
 ************************************************************************/
#ifndef _WEB_H
#define _WEB_H


#ifdef WP_HW_WINPATH1
#ifdef __PPC__
#define WEB_WINPATH_BASE        0x1f000000  /* WinPath internal space */
#endif /*__PPC__ */
#ifdef __mips__
#define WEB_WINPATH_BASE        0xbf000000  /* WinPath internal space */
#endif /* __mips__ */
#elif defined WP_HW_WINPATH2
#define GRAM_RIF_SWAP_OFFSET 0x20000
#ifdef __PPC__
#define WEB_WINPATH_BASE        (0x1d000000 + GRAM_RIF_SWAP_OFFSET)  /* WinPath internal space */
#endif /*__PPC__ */
#ifdef __mips__
#define WEB_WINPATH_BASE        (0xbd000000 + GRAM_RIF_SWAP_OFFSET)  /* WinPath internal space */
#endif /* __mips__ */
#elif defined WP_HW_WINPATH3
#ifdef __mips__
#define WEB_WINPATH_BASE        (0xbe000000)  /* WinPath internal space */
#endif /* __mips__ */
#endif /*WP_HW_WINPATH*/

#define WEB_WINPATH_SIZE        0x00040000  /*   1/4 Mbytes           */
#define WEB_WINPATH_MASK        (~(WEB_WINPATH_SIZE-1))

#define WEB_7SEG_DISP           WEB_60X_BASE

#define MAP_CHCIIMT  WEB_WINPATH_BASE       /* Used by memmap.h       */

/************************************************************************
 *  UART status bits
 ************************************************************************/
#define WEB_UART_STATUS_RXFE            0x80
#define WEB_UART_STATUS_TEMT            0x40
#define WEB_UART_STATUS_THRE            0x20
#define WEB_UART_STATUS_BI              0x10
#define WEB_UART_STATUS_FE              0x08
#define WEB_UART_STATUS_PE              0x04
#define WEB_UART_STATUS_OE              0x02
#define WEB_UART_STATUS_DR              0x01

#endif

/************************************************************************/
/*                   AUTOMATION INTERFACE                               */
/************************************************************************/

#ifndef _WT_NEW_AUTOMATION_
#define _WT_NEW_AUTOMATION_

/******************************* AUTOMATION *************************/

/* Defined modes of operation */
#define MANUAL                  0
#define AUTO                    1

/* Types of menu systems */
#define REGULAR                 0
#define SPECIAL                 1
#define TTL                     2

/* Automation constants */
#define INST_NUM                256
#define MAX_TTL_FILE_SIZE       10000
#define MAX_CONTROL_STRING_SIZE 1536
#define QUEUESIZE               3
#define SERVER_PORT             9333
#define MSGMAXSIZE              2048
#define WS_PACKET_LENGTH        2049
#define HOST_NAME_LEN           50
#define INVALID_SOCKET          -1
#define SOCKET_ERROR            -1

#define MAP_GS_IC_SYIR          (WEB_WINPATH_BASE+0x10c74)
#define MAP_GS_IC_SEIR          (WEB_WINPATH_BASE+0x10c70)
#define MAP_GS_IC_SEMR0         (WEB_WINPATH_BASE+0x10c60)
#define MAP_GS_IC_SEMR1         (WEB_WINPATH_BASE+0x10c64)
#define MAP_GS_IC_SYMR0         (WEB_WINPATH_BASE+0x10c68)
#define MAP_GS_IC_SYMR1         (WEB_WINPATH_BASE+0x10c6c)

#define MAP_GS_IC_INTMODE       (WEB_WINPATH_BASE+0x10c78)

#ifdef WP_HW_WINPATH1
#define ASIX_ISR                0xbf202818
#define WEB_CPLD_LED_1          0xbf100010
#elif defined WP_HW_WINPATH2
#define WEB_CPLD_LED_1          (WEB_CPLD_SECONDARY_BASE + 0x40)


#endif
/******************************* MENU API *************************/

#ifdef OK
#undef OK
#endif

#define OK 0
#ifndef ERROR
#define ERROR -1
#endif

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#define TRUE   1
#define FALSE  0
#define TRUE_M 3
#define FALSE_M 2
#define HELP 5

#define STR_STAR           "*************************************************************************"
#define STR_HOT_KEYS       "  Hot keys: !-Main Menu ^-Upper Menu =-Display Cur Menu  d-Debug Menu!    "
#define STR_ENTER_COMMAND  "# Enter your command:"
#define STR_MENU_ERR       "Err> Wrong menu command"

/* Definitions of menus structs */
typedef void (*Y_MnuLeafP) (char *);

typedef enum
{
   K_Menu,
   K_Leaf
} E_MenuNodeType;

typedef struct
{
   E_MenuNodeType    Type;
   char              Visible;
   char              *Desc;
   union
   {
      void           *Menu;
      Y_MnuLeafP     Leaf;
   }  Node;
}  Y_MenuEntry;

typedef void (*sed_int_handler_t)(void);
typedef struct netif my_netif;

extern /*const*/ Y_MenuEntry V_MainMenu[];

extern char *STR_MENU_HDR;
extern void sed_int_handler(void);
extern void sedif_input(my_netif *netif);
extern void tcpdump_init(void);
extern sed_int_handler_t sed_register_handler(sed_int_handler_t fnc);
extern void asixif_int_handler(void);
extern unsigned int imr_shadow;

/*****************************************************************************/
/*****************************************************************************/
/*                         MENU API FUNCTIONS                                */
/*****************************************************************************/
/*****************************************************************************/

void F_MenuEngine     (char * InputBuf);
int  F_EvaluateHotKey (char * InputBuf);
void F_EvaluateCommand(char * InputBuf);
void F_SetMainMenu    (void);
void F_SetDebugMenu   (void);
void F_SetUpperMenu   (void);
void F_SetMenu	      (Y_MenuEntry *Menu);
void F_DisplayMenu    (void);
void F_DisplayHeader  (void);

/*****************************************************************************/
/*****************************************************************************/
/*                         AUTOMATION FUNCTIONS                              */
/*****************************************************************************/
/*****************************************************************************/

/* Gets a string either from the stdin (manual mode) or from the automation
   global array (automation mode) */
WP_CHAR *WT_Gets( char *str);

/* Parses data on the control packet */
void EvaluateControlString(WP_U8 *InputBuf,WP_U32 demo_mode);

/* Fills automation global array of strings */
WP_U32 FillAutomationArray(WP_CHAR *InputBuf, WP_CHAR *params_string,WP_U32 demo_mode);

/* Processes incoming automation packets*/
void WT_processPacketCommand(void);

/* Translates data from ascii to hexadecimal */
void TranslateAsciiToHex(WP_U8 *Hex,WP_CHAR *Ascii,WP_U32 length);

/* Translates data from hexadecimal to ascii*/
void TranslateHexToAscii(WP_CHAR *Hex,WP_CHAR *Ascii,WP_U32 length);

#endif /* _WT_AUTOMATION_ */
extern Y_MenuEntry V_Menu_Debug[];
static Y_MenuEntry *V_CurrMenuP = &V_MainMenu[0];
static Y_MenuEntry *V_PreDMenu = &V_MainMenu[0];

WP_CHAR *instBufSave[0x4096];
WP_U8 demoFunctionMemPtr[MAX_CONTROL_STRING_SIZE];
WP_CHAR instBufLoad[10][INST_NUM];
WP_U32 instIndexSave=0;
WP_U32 instIndexLoad=0;
WP_U32 demo2regRunMode=AUTO;
WP_U32 default_setup=FALSE;
WP_U32 sockfd=0;
WP_U32 glb_socket=0;
WP_U32 glb_f_continue=FALSE;
WP_U32 glb_ttl_trigger=0;
WP_CHAR glb_in_packet[WS_PACKET_LENGTH];
WP_CHAR glb_out_packet[WS_PACKET_LENGTH];
WP_U32 glb_clientfd=0;
WP_U32 glb_demo_mode=REGULAR;
WP_U32 glb_packet_received=FALSE;
WP_U32 glb_timeout=0;
WP_U32 helpSelection=0;
char msg[100];
volatile int intPending=0;
volatile int intTimer=0;


/*****************************************************************************
 *
 * Function: WT_Gets
 *
 * Purpose: allow demo menus to receive their input out of a global array of
 *          strings or from the user according to automation mode
 *
 * Description:uses automation global array of strings to run menus in
 *             automation mode
 *
 * Inputs: char *str
 *
 * Outputs: WP_CHAR *retVal
 *
 * Return Value:
 *
 * Called by:
 *     user application
 ****************************************************************************/

WP_CHAR *WT_Gets(char *str)
{
   WP_CHAR *retVal;

   if (instIndexLoad == instIndexSave)
      demo2regRunMode=MANUAL;

   if( demo2regRunMode == MANUAL)
   {
#ifdef __linux__
      retVal = fgets( str,INST_NUM,stdin );
      /*  instBufSave[instIndexSave] = malloc( strlen( str)+1); */
#else
      retVal = gets( str );
      /*  instBufSave[instIndexSave] = (WP_CHAR*)malloc( strlen( str)+1); */
#endif
      if( (strlen(str)+instIndexSave + 1) > 0x4096)
      {
         printf("ERROR: instBufSave length is not enough %d\n", strlen(str));
         exit(1);
      }
      strcpy(  instBufSave[instIndexSave], str);
   }
   else
   {
      strcpy( str, instBufLoad[instIndexLoad]);
      printf( "Auto instruction: %s\n", str);
      retVal = str;
      instBufSave[instIndexSave] = instBufLoad[instIndexLoad];
      instIndexLoad ++;
   }
   return retVal;
}

/*****************************************************************************/
/*****************************************************************************/
/*                         MENU API FUNCTIONS                                */
/*****************************************************************************/
/*****************************************************************************/

/***************************************************************
 * Func name  : F_MenuEngine
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void F_MenuEngine(char* InputBuf)
{
#ifdef MENU_DEBUG_MODE
   printf("inputlength: %d\n",strlen(InputBuf));
   printf("input echo: '%s'\n",InputBuf);
#endif
   if (strlen(InputBuf)<1 || strcmp(InputBuf, "\n")==0)
     return;
   /* Is HotKey? -> Evaluate HotKey */
   if (F_EvaluateHotKey(InputBuf) == OK)
   {
      return;
   }
   else
   {
      /* Evaluate Menu or Procedure Command */
      F_EvaluateCommand(InputBuf);
   }
} /* F_MenuEngine */

/***************************************************************
 * Func name  : F_EvaluateHotKey
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
int F_EvaluateHotKey(char *InputBuf)
{
   switch (InputBuf[0])
   {
   case '!':   /* Goto main menu  */
      F_SetMainMenu();
      F_DisplayMenu();
      break;
   case '^':   /* Goto upper menu */
      F_SetUpperMenu();
      F_DisplayMenu();
      break;
   case '=':   /* Display current menu again */
      F_DisplayMenu();
      break;
   case 'd':  /*Goto debug menu */
      V_PreDMenu=V_CurrMenuP;
      F_SetDebugMenu();
      F_DisplayMenu();
      break;
   case 'p':
      F_SetMenu((Y_MenuEntry *)V_PreDMenu);
      F_DisplayMenu();
      break;
   default:
      return  ERROR;
   }
   return OK;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void F_EvaluateCommand(char *InputBuf)
{
  int          RetVal, Cmd;
  Y_MenuEntry  *MenuEntryP;
  /* Is chosen command is in tolerance */
  Cmd = atoi(InputBuf);
  RetVal = 1;

  if(strcmp("?",InputBuf)==0)
    Cmd = helpSelection;
  if(RetVal < 1)
  {
     printf("STR_MENU_ERR\n");
     return;
  }
  if ((Cmd > (int)V_CurrMenuP->Type) || (Cmd < 1))
  {
     printf("%s\n", STR_MENU_ERR);
     return;
  }

  MenuEntryP = V_CurrMenuP+Cmd;

  /* Is chosen command is visible */
  if ((MenuEntryP->Visible && 0x01) != TRUE)
  {
     printf("%s\n", STR_MENU_ERR);
     return;
  }

  /* Is chosen command is menu or procedure */
  if (MenuEntryP->Type == K_Leaf)
  {
    (MenuEntryP->Node.Leaf) (InputBuf);
  }
  else if ((MenuEntryP->Type == K_Menu))
  {
     V_CurrMenuP = (Y_MenuEntry*)MenuEntryP->Node.Menu;
     F_DisplayMenu();
  }

} /* F_EvaluateCommand */

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void F_SetMainMenu    (void)
{

V_CurrMenuP = (Y_MenuEntry*)&V_MainMenu[0];

} /* F_SetMainMenu */

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void F_SetDebugMenu (void)
{
V_CurrMenuP = (Y_MenuEntry*)&V_Menu_Debug[0];

} /* F_SetDebugMenu */


/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void F_SetMenu (Y_MenuEntry *Menu)
{

 V_CurrMenuP = Menu;

} /* F_SetMenu*/

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void F_SetUpperMenu   (void)
{

V_CurrMenuP = (Y_MenuEntry *)V_CurrMenuP->Node.Menu;

} /* F_SetUpperMenu */

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void F_DisplayMenu    (void)
{
   int Counter;
   Y_MenuEntry *CurrMenuEntryP;

   F_DisplayHeader();
   printf("  Name: %s  \n",
          V_CurrMenuP->Desc);
   printf("%s\n", STR_STAR);
   CurrMenuEntryP = V_CurrMenuP;

   /* Skip to the 1st menu Item */
   CurrMenuEntryP++;

   for (Counter = 1;
        Counter <= (int)V_CurrMenuP->Type;
        Counter++, CurrMenuEntryP++)
   {
      if (CurrMenuEntryP->Visible == TRUE)
      {
         printf("%d. %s\n", Counter, CurrMenuEntryP->Desc);
      }
      else if (CurrMenuEntryP->Visible == FALSE)
      {
         printf("%d. This option is not visible\n", Counter);
      }else if (CurrMenuEntryP->Visible == TRUE_M)
      {
         printf("%d. %s\n", Counter, CurrMenuEntryP->Desc);
         printf("-------------------------------------------------------------------------\n");
      }else if (CurrMenuEntryP->Visible == FALSE_M)
      {
         printf("%d. This option is not visible\n", Counter);
         printf("-------------------------------------------------------------------------\n");
      }else if (CurrMenuEntryP->Visible == HELP)
      {
         helpSelection = Counter;
         printf("?. Help\n");
      }
   }


} /* F_DisplayMenu */

/***************************************************************
 * Func name  : F_DisplayHeader
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void F_DisplayHeader(void)
{
   printf("\n\n%s\n", STR_STAR);
   printf("%s\n", STR_MENU_HDR);
   printf("%s\n", STR_HOT_KEYS);

} /* F_DisplayHeader */

/*****************************************************************************/
/*****************************************************************************/
/*                         AUTOMATION FUNCTIONS                              */
/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************
 *
 * Function: EvaluateControlString
 *
 * Purpose: apply a demo function to its parameters according to data received
 *          by the control packet
 *
 * Description: Uses RetrieveFunction() and RetrieveParams()
 *              to retrieve function and parameters from
 *              the control string and applies the function
 *              to its parameters
 *
 * Inputs: WP_U8 *InputBuf
 *
 * Outputs: None
 *
 * Return Value:
 *
 * Called by:
 *     user application
 ****************************************************************************/

void EvaluateControlString(WP_U8 *InputBuf,WP_U32 demo_mode)
{
   WP_CHAR function_hexa_string[INST_NUM];
   WP_CHAR function_ascii_string[INST_NUM];
   WP_CHAR params_temp_buffer[INST_NUM];
   WP_CHAR params_ascii_string[MAX_CONTROL_STRING_SIZE]="";
   WP_U32 i=0,j=0,k=0,len=0,length=0;
   WP_CHAR Input[INST_NUM];

   /* parse the control string */
   /* fill function string */
   /* no menu switching*/
   if ((InputBuf[0] == 255) && (InputBuf[1] == 255))
      function_ascii_string[0]='\0';
   else
   {
      while (InputBuf[2*length] != 255)
      {
         if ((InputBuf[2*length] != 221))
         {
            printf("Parsing Error - separate menu indexes by dd\n");
            return;
         }
         function_hexa_string[length]=InputBuf[2*length+1];
         ++length;
      }
      function_hexa_string[length]='\0';
      TranslateHexToAscii(function_hexa_string,function_ascii_string,length);
   }
   i=++length;
   /* fill parameters string */
   /* no parameters*/
   if (InputBuf[2*length-1] == 255)
      params_ascii_string[0]='\0';
   else
   {
      /* bb - string format */
      if  (InputBuf[2*length-1] == 187)
      {
         len=InputBuf[2*length];
         memcpy(params_temp_buffer,&InputBuf[2*length+2],len);
         params_temp_buffer[len]='\0';
         length=0;
         while((params_temp_buffer[length])==0)
            ++length;
         strcat(params_ascii_string," ");
         strcat(params_ascii_string,&params_temp_buffer[length]);
         /* fill automation global array */
         i=FillAutomationArray(function_ascii_string,params_ascii_string,demo_mode);

         /* apply demo function on its parameters using automation global */
         instIndexLoad=0;

         while (i--)
         {
            printf("\n");
            WT_Gets(Input);
            F_MenuEngine(Input);
         }

         return;
      }
      else
      {

      /* while not reaching end of string */
      while (InputBuf[(2*i-1)+(5*j)] != 255)
      {
         switch(InputBuf[(2*i-1)+(5*j)])
         {
            /* cc - convert to char format */
            case (204):
            {
               if (InputBuf[(2*i)+(5*j)]=='\0' && InputBuf[(2*i)+(5*j)+1]=='\0' &&
                   InputBuf[(2*i)+(5*j)+2]=='\0' && InputBuf[(2*i)+(5*j)+3]=='\0')
               {
                  j+=1;
                  break;
               }

               memcpy(params_temp_buffer,&InputBuf[(2*i)+(5*j)],4);
               params_temp_buffer[4]='\0';
               length=0;
               while((params_temp_buffer[length])==0)
                  ++length;
               strcat(params_ascii_string," ");
               strcat(params_ascii_string,&params_temp_buffer[length]);
               j+=1;
               break;
            }
            /* dd - convert to decimal format */
            case (221):
            {
               memcpy(&k,&InputBuf[(2*i)+(5*j)],4);
               sprintf(params_temp_buffer,"%u",k);
               strcat(params_ascii_string," ");
               strcat(params_ascii_string,params_temp_buffer);
               k=0;
               j+=1;
               break;
            }
            /* ee - convert to hexadecimal format */
            case (238):
            {
               memcpy(&k,&InputBuf[(2*i)+(5*j)],4);
               sprintf(params_temp_buffer,"%x",k);
               strcat(params_ascii_string," ");
               strcat(params_ascii_string,params_temp_buffer);
               k=0;
               j+=1;
               break;
            }
            /* delimiter doesn't match formats - print an error message */
            default:
            {
               printf("Parsing Error - precede parameters by cc - (char format),"
                       "dd - (decimal format) or ee - (hexadecimal format)");
               return;
            }
         }
      }
      }
   }
/* fill automation global array */
   i=FillAutomationArray(function_ascii_string,params_ascii_string,demo_mode);

/* apply demo function on its parameters using automation global */
   instIndexLoad=0;

   while (i--)
   {
      printf("\n");
      WT_Gets(Input);
      F_MenuEngine(Input);
   }

   return;
} /* EvaluateControlString */

/*****************************************************************************
 *
 * Function: FillAutomationArray
 *
 * Purpose: fills global array of strings
 *
 * Descrption: receives function string and parameters string and fills the
 *             the global array so execution of demo function can be made
 *             in automation mode
 *
 * Inputs: WP_CHAR *InputBuf, WP_CHAR *params_string
 *
 * Outputs: WP_U32 indx
 *
 * Return Value:
 *
 * Called by:
 *     user application
 ****************************************************************************/
WP_U32 FillAutomationArray(WP_CHAR *InputBuf, WP_CHAR *params_string,WP_U32 demo_mode)
{
   /* WP_U8 *function = (WP_U8*) malloc (MAX_CONTROL_STRING_SIZE * sizeof(WP_U8));*/
   WP_U8 *function = demoFunctionMemPtr;
   WP_U32 i=0,j=0,length=0,indx=0;

   instIndexSave=0;
/* fill automation global array with menu switching commands*/
   length=strlen(InputBuf);
   TranslateAsciiToHex(function,InputBuf,length);
   if (demo_mode==REGULAR)
   {
      j=1;
      strcpy(instBufLoad[0],"!");
      ++instIndexSave;
   }
   for(i=0;i<length/2;++i)
   {
      sprintf(instBufLoad[i+j],"%d",function[i]);
      ++instIndexSave;
   }

/* fill automation global array with function index and its parameters*/
   strcat(instBufLoad[i],params_string);
   indx = i+1;

/* return number of buffers filled in automation global array */
   return indx;
} /* FillAutomationArray */


/*****************************************************************************
 *
 * Function: TranslateAsciiToHex
 *
 * Purpose: translate buffer data from ascii to hexadecimal
 *
 * Description: receives the the ascii buffer to be translated, the target
 *              buffer and data length and translates the data
 *
 * Inputs: WP_U8 *Hex,WP_CHAR *Ascii,WP_U32 length
 *
 * Outputs: None
 *
 * Return Value:
 *
 * Called by:
 *     user application
 ****************************************************************************/

void TranslateAsciiToHex(WP_U8 *Hex,WP_CHAR *Ascii,WP_U32 length)
{
    WP_S32 i,first=0,second=0;

   for (i=0;i<length;i++)
     {
          if ( Ascii[2*i] >= '0' &&  Ascii[2*i] <= '9')
            first =  Ascii[2*i] - '0';
          if ( Ascii[2*i] >= 'a' &&  Ascii[2*i] <= 'f')
            first =  Ascii[2*i] - 'a' + 10;
          if ( Ascii[2*i+1] >= '0' &&  Ascii[2*i+1] <= '9')
            second =  Ascii[2*i+1] - '0';
          if ( Ascii[2*i+1] >= 'a' &&  Ascii[2*i+1] <= 'f')
            second =  Ascii[2*i+1] - 'a' + 10;

          Hex[i]=(WP_CHAR )(first * 16 + second);
     }
    return;
}/* TranslateAsciiToHex */

/*****************************************************************************
 *
 * Function: TranslateHexToAscii
 *
 * Purpose: translate buffer data from hexadecimal to ascii
 *
 * Description: receives the the hexa buffer to be translated, the target
 *              buffer and data length and translates the data
 *
 * Inputs: WP_CHAR *Hex,WP_CHAR *Ascii,WP_U32 length
 *
 * Outputs: None
 *
 * Return Value:
 *
 * Called by:
 *     user application
 ****************************************************************************/

void TranslateHexToAscii(WP_CHAR *Hex,WP_CHAR *Ascii,WP_U32 length)
{
    WP_U32 i,first=0,second=0;

   for (i=0;i<length;i++)
     {
        first = ((Hex[i] & 255)%16);
        second = ((Hex[i] & 255)/16);

        if (first > 9)
           Ascii[2*i+1]= ('a' + (first-10));
        else
           Ascii[2*i+1]= ('0' + first);

        if (second > 9)
           Ascii[2*i]= ('a' + (second-10));
        else
           Ascii[2*i]= ('0' + second);
     }

   Ascii[2*i]=0;

return;
}/* TranslateHexToAscii */


#endif
