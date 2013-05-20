/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: platform/mips_linux_winpath3/sources/wpl_wmm.c
 *
 * Purpose: Winpath's platform dependent functions
 *
 * Functions contained in this file:
 *
 * +---------------------------+----------------------------------------------
 * |  Function name            | Description
 * +---------------------------+----------------------------------------------
 * |
 * |  API Functions
 * |  -------------------------+----------------------------------------------
 * |  -------------------------+----------------------------------------------
 * |  
 * |  Internal Functions
 * |  -------------------------+----------------------------------------------
 * |  -------------------------+----------------------------------------------
 * |  
 * |  Internal Functions (static)
 * |  -------------------------+----------------------------------------------
 * +---------------------------+----------------------------------------------
 *
 ****************************************************************************/

/******************************************************************/
/*                       Includes                                 */
/******************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <wpl_platform.h>

/******************************************************************/
/*                       Typedefs and defines                     */
/******************************************************************/

#define WPL_WMM_DELTA 0x1000
#define WPL_WMM_BASE  0x2000
#define WPL_WMM(base, t) (base + WPL_WMM_BASE + WPL_WMM_DELTA * t);
#define WPL_FIELD(v,s) (((WP_U32) (v)) << (s))
#define WPL_WMM_HCR_CMD_V_S 31
#define WPL_WMM_HCR_CMD_V_F(v) WPL_FIELD(v, WPL_WMM_HCR_CMD_V_S)
#define WPL_WMM_HCR_CMD_V_VALID 1

/******************************************************************/
/*                       Structures + Enumerators                 */
/******************************************************************/


/******************************************************************/
/*                       Global Database                          */
/******************************************************************/

/******************************************************************/
/*                       Local Database                           */
/******************************************************************/

WP_S32 wmm_fd = -1;

/******************************************************************/
/*                       Macros + inlines                         */
/******************************************************************/

/******************************************************************/
/*                       Forward Declarations                     */
/******************************************************************/

WP_U32 WPLI_WmmHostCmdHelper(WP_U32 wmm_base, WP_U32 *reg, WP_U32 *addr, WP_U32 *cmd);

/******************************************************************/
/*                       API Functions                            */
/******************************************************************/

WP_U32 WPL_WmmHostCmd(WP_U32 wpid, WP_U8 bustype, WP_U32 *hcr_cmd,
                      WP_U32 *hcr_reg, WP_U32 *host_address)
{
   WP_U8* wmm_base;
   WP_U32 wmm_cmd;

   wmm_base = WPL_WMM(WPL_RegBaseGet(0), bustype);
   wmm_cmd = *hcr_cmd | WPL_WMM_HCR_CMD_V_F(WPL_WMM_HCR_CMD_V_VALID);

   /* checks whether WMM host command are shared with the BSP
    * (such as SED for mips linux). In this case the target
    * specific function must be called to issue the specific
    * WMM host command.
    */
   {
      WP_S32 ret_val;

      *hcr_cmd = wmm_cmd;
      ret_val = WPLI_WmmHostCmdHelper((WP_U32)wmm_base, hcr_reg, host_address, hcr_cmd);
      if (ret_val == -1)
         return WPL_ERR(WPL_ERR_PLATFORM);
      if (ret_val == 1)
         return WPL_ERR(WPL_ERR_PREVIOUS_WMM_HOST_CMD_FAILED);
      if (ret_val == 2)
         return WPL_ERR(WPL_ERR_WMM_HOST_CMD_FAILED);
   }

   return WP_TRUE;
}

void WPL_FifoInitialize(void *wmm)
{
}

WP_U32 WPLI_WmmHostCmdHelper(WP_U32 wmm_base, WP_U32 *reg, WP_U32 *addr, WP_U32 *cmd)
{
   WPL_wmm_host_cmd wmm_host_cmd_req;

   if (wmm_fd == -1)
   {
      if ((wmm_fd = open("/dev/wp_siu1", O_RDWR)) < 0)
      {
         perror("open");
         return WPL_ERR(WPL_ERR_PLATFORM);
      }
   }

   wmm_host_cmd_req.base = wmm_base;
   wmm_host_cmd_req.cmd = *cmd;
   wmm_host_cmd_req.reg = *reg;
   wmm_host_cmd_req.address = *addr;
   wmm_host_cmd_req.status = 0;

   if (ioctl(wmm_fd, WPL_IOCTL_WMM_HOST_CMD, &wmm_host_cmd_req) != 0)
   {
      return -1;
   }

   *addr = wmm_host_cmd_req.address;
   *reg = wmm_host_cmd_req.reg;
   *cmd = wmm_host_cmd_req.cmd;

   return (wmm_host_cmd_req.status);
}
