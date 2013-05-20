/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/


#ifndef _WPL_WINIPC_INTERFACE_H_
#define _WPL_WINIPC_INTERFACE_H_


/******************************************************************/
/*                       Includes                                 */
/******************************************************************/

#include "wpl_types.h"
#include <signal.h>

#include "wpl_platform.h"
#include "wpi_veneer_memory.h"


/******************************************************************/
/*                       Typedefs and defines                     */
/******************************************************************/

typedef void (*WPL_sig_handler_type)(WP_S32 i_signal);

/******************************************************************/
/*                       Structures + Enumerators                 */
/******************************************************************/



/******************************************************************/
/*                       External database                        */
/******************************************************************/



/******************************************************************/
/*                       Macros + inlines                         */
/******************************************************************/



/******************************************************************/
/*                       Functions                                */
/******************************************************************/

WP_U32 WPL_WinipcHandlerRegister(WPL_sig_handler_type pi_cb, WP_S32 i_signal);

WP_U32 WPL_WinipcClientRegister(WP_U32 i_client_id);

WP_U32 WPL_WinipcSignalRaise(WP_U32 i_to_client, WP_boolean i_user_event);


#endif
