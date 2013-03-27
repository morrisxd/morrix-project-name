/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: !!!!!!!!! temp file of include function from WDDI !!!!!!!!!!!!!!!!!!!
 *
 *
 ****************************************************************************/
#ifndef WUFE_API_H
#define WUFE_API_H

#ifndef WUFE_USER_CONFIG_H
#include "wufe_user_config.h"
#endif

#define WUFEI_FIELD(v,s) (((WP_U32) (v)) << (s))
#define WUFEI_VALUE(f,s,w) ((((WP_U32) (f)) >> (s)) & ((1 << (w)) - 1))
#define WUFEI_MASK(s,w) (((1 << (w)) - 1) << (s))

#define WUFEI_MAX(a,b) ((a) > (b) ? (a) : (b))
#define WUFEI_MIN(a,b) ((a) < (b) ? (a) : (b))

#define WUFE_TYPE_SIZE (6)
#define WUFE_TYPE_SHIFT (8 * sizeof(WP_U32) - WUFE_TYPE_SIZE)
#define WUFE_ERROR_P(x) (((x) >> WUFE_TYPE_SHIFT) == ((1 << WUFE_TYPE_SIZE) - 1))
#define WUFE_ERROR(x) ((x) & ((1 << WUFE_TYPE_SHIFT) - 1))

#define WUFEI_RETURN_IF_ERROR(status,x) \
        if ((status = (x), WUFE_ERROR_P(status))) return (status)

#define WUFEI_IF_ERROR_RETURN_DETACH_W(status,x,wufe_id) \
        do{ status = (x); \
        if (WUFE_ERROR_P(status)) {\
             WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id); \
             return (status); }\
} while (0)

#if WUFE_DISPLAY_ENABLE
#define WUFE_PRINT_DISP(y) do { sprintf y ; printf("%s",wufei_log);} while(0)
#else
#define WUFE_PRINT_DISP(y)
#endif

#if WUFE_PRINT_ERR_ENABLE
#define WUFE_PRINT_ERR(y) do { sprintf y ; printf("%s",wufei_log);} while(0)
#else
#define WUFE_PRINT_ERR(y)
#endif

#if WUFE_PRINT_DBG_ENABLE
#define WUFE_PRINT_DBG(y) do { sprintf y ; printf("%s",wufei_log);} while(0)
#else
#define WUFE_PRINT_DBG(y)
#endif

#endif /* WUFE_API_H */
