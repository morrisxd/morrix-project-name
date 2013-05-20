/*************************************************************
 * (C) Copyright 2001-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

/*****************************************************************************
 *
 * Module: $RCSfile: winmon.h,v $
 * Purpose: Application interface to WinMon
 *
 *
 * Module Originator: Arnon Mordoh
 * Creation Date: Sep 20th, 2001
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
 ***************************************************************************/

#ifndef _WINMON_H_
#define _WINMON_H_

#include "wpl_types.h"

#ifndef NO_AUTO_MEMMAP
#define WINMON_MIPS_RAM_BASE     0x01000000
#else
#define WINMON_MIPS_RAM_BASE     0x03000000
#endif
#define WINMON_MIPS_DSEG1_OFFSET 0xa0000000
#define WINMON_MIPS_FLASH_BASE   0x1fc00000
#define WINMON_MIPS_FUNCTION_TABLE_OFFSET  0x500

#ifndef NO_AUTO_MEMMAP
#define WINMON_PPC_RAM_BASE      0x01000000
#else
#define WINMON_PPC_RAM_BASE      0x03000000
#endif
#define WINMON_PPC_FLASH_BASE    0xfff00000
#define WINMON_PPC_FUNCTION_TABLE_OFFSET  0x80

#ifdef LOAD_IN_RAM

#ifdef __mips__
#define WINMON_FUNCTION_TABLE    (WINMON_MIPS_RAM_BASE+\
                                  WINMON_MIPS_DSEG1_OFFSET+\
                                  WINMON_MIPS_FUNCTION_TABLE_OFFSET)
#endif

#ifdef __PPC__
#define WINMON_FUNCTION_TABLE    (WINMON_PPC_RAM_BASE+\
                                  WINMON_PPC_FUNCTION_TABLE_OFFSET)
#endif

#else

#ifdef __mips__
#define WINMON_FUNCTION_TABLE    (WINMON_MIPS_FLASH_BASE+\
                                  WINMON_MIPS_DSEG1_OFFSET+\
                                  WINMON_MIPS_FUNCTION_TABLE_OFFSET)
#endif

#ifdef __PPC__
#define WINMON_FUNCTION_TABLE    (WINMON_PPC_FLASH_BASE+\
                                  WINMON_PPC_FUNCTION_TABLE_OFFSET)
#endif

#endif

#ifdef WINMON_GDB_OUTPUT
#define WINMON_PUTCHAR_OFS      9
#define WINMON_PUTS_OFS         10
#else
#define WINMON_PUTCHAR_OFS      0
#define WINMON_PUTS_OFS         1
#endif

#define WINMON_GETCHAR_OFS          2
#define WINMON_WAIT_OFS          3
#define WINMON_EXIT_OFS          4
#define WINMON_MALLOC_OFS           5
#define WINMON_CLOCK_OFS            6
#define WINMON_DISPLAY_OFS          7
#define WINMON_SIGNAL_OFS           8
#define WINMON_GETENV_OFS           11

#define WINMON_INT_CONNECT_OFS		12
#define WINMON_ENABLE_INTS_OFS		13
#define WINMON_DISABLE_INTS_OFS		14
#define WINMON_ENABLE_INT_OFS		15
#define WINMON_DISABLE_INT_OFS		16
#define WINMON_INT_ACK_OFS		17
#define WINMON_SET_TIMER_INT_OFS	18
#define WINMON_SYSTEM_OFS        	19

#ifndef NO_AUTO_MEMMAP
#define WINMON_MEM_ADDR_OFS             20
#define WINMON_MEM_SIZE_OFS             21
#define WINMON_BUS_ACTIVE_OFS           22
#endif

#define WINMON_FUNC(ofs) (((WP_U32 *)(WINMON_FUNCTION_TABLE))[ofs])

typedef void (*t_winmon_exit)(WP_S32 rc);
typedef WP_S32  (*t_winmon_putchar)(WP_S32 c);
typedef WP_S32  (*t_winmon_puts)(WP_CHAR *s);
typedef WP_S32  (*t_winmon_getchar)(void);
typedef WP_S32  (*t_winmon_wait)(WP_U32 t);
typedef WP_S32  (*t_winmon_malloc)(WP_S32 s);
typedef WP_S32  (*t_winmon_clock)(void);
typedef void (*t_winmon_display)(WP_U8);
typedef void *(*t_winmon_signal)(WP_S32, void *);
typedef WP_CHAR *(*t_winmon_getenv)(WP_CHAR *);

typedef void *(*t_winmon_int_connect)(WP_S32, void (*)(WP_S32));
typedef void (*t_winmon_enable_interrupts)(void);
typedef WP_S32  (*t_winmon_disable_interrupts)(void);
typedef void (*t_winmon_enable_int)(WP_S32);
typedef void (*t_winmon_disable_int)(WP_S32);
typedef void (*t_winmon_int_ack)(WP_S32);
typedef void (*t_winmon_set_timer_int)(void *,WP_S32);
typedef void (*t_winmon_system)(WP_CHAR *);

#ifndef NO_AUTO_MEMMAP
typedef WP_S32  (*t_winmon_get_bus_address)(WP_S32 c);
typedef WP_S32  (*t_winmon_get_bus_size)(WP_S32 c);
typedef WP_S32  (*t_winmon_is_bus_access)(WP_S32 c);
#endif

#ifdef WINMON_CONTEXT

#include "sysio.h"

#define WinMon_putchar(c) SysIO_putchar(c)
#define WinMon_puts(c)    SysIO_puts(c)
#define WinMon_getchar()  SysIO_getchar()
#define WinMon_wait(c)    SysIO_wait(c)
#define WinMon_malloc(s)  SysIO_malloc(s)
#define WinMon_clock()    SysIO_GetClock()
#define WinMon_display(s) SysIO_Display(s)
#define WinMon_signal(e,s)  Int_signal(e,s)
#define WinMon_getenv(name) (WP_CHAR *)Env_getenv(name)

#define WinMon_int_connect(e,s)  	Int_IntConnect(e,s)
#define WinMon_enable_interrupts()	sys_enable_int()
#define WinMon_disable_interrupts()	sys_disable_int()
#define WinMon_enable_int(d)		Int_EnableInterrupt(d)
#define WinMon_disable_int(d)		Int_DisableInterrupt(d)
#define WinMon_int_ack(d)		Int_IntrAck(d)
#define WinMon_set_timer_int(h,t)	Int_SetTimerInt(h,t)

#ifndef NO_AUTO_MEMMAP
#define WinMon_system(c)                shellSystem(c)
#define WinMon_get_bus_address(c)       sys_get_bus_address(c)
#define WinMon_get_bus_size(c)          sys_get-bus_size(c)
#define WinMon_is_bus_access(c)         sys_is_bus_access(c)
#endif

#else /* WINMON_CONTEXT */

#define WinMon_putchar(c)\
                (((t_winmon_putchar)WINMON_FUNC(WINMON_PUTCHAR_OFS))(c))
#define WinMon_puts(c) (((t_winmon_puts)WINMON_FUNC(WINMON_PUTS_OFS))(c))
#define WinMon_getchar() \
  (((t_winmon_getchar)WINMON_FUNC(WINMON_GETCHAR_OFS))())
#define WinMon_wait(c) (((t_winmon_wait)WINMON_FUNC(WINMON_WAIT_OFS))(c))
#define WinMon_exit(rc) (((t_winmon_exit)WINMON_FUNC(WINMON_EXIT_OFS))(rc))
#define WinMon_malloc(s) \
  (((t_winmon_malloc)WINMON_FUNC(WINMON_MALLOC_OFS))(s))
#define WinMon_clock() (((t_winmon_clock)WINMON_FUNC(WINMON_CLOCK_OFS))())
#define WinMon_display(s) \
  (((t_winmon_display)WINMON_FUNC(WINMON_DISPLAY_OFS))(s))
#define WinMon_signal(e,s) \
  (((t_winmon_signal)WINMON_FUNC(WINMON_SIGNAL_OFS))(e,s))
#define WinMon_getenv(name) \
  (((t_winmon_getenv)WINMON_FUNC(WINMON_GETENV_OFS))(name))

#define WinMon_int_connect(e,s) \
  (((t_winmon_int_connect)WINMON_FUNC(WINMON_INT_CONNECT_OFS))(e,s))
#define WinMon_enable_interrupts() \
  (((t_winmon_enable_interrupts)WINMON_FUNC(WINMON_ENABLE_INTS_OFS))())
#define WinMon_disable_interrupts() \
  (((t_winmon_disable_interrupts)WINMON_FUNC(WINMON_DISABLE_INTS_OFS))())

#define WinMon_enable_int(d) \
  (((t_winmon_enable_int)WINMON_FUNC(WINMON_ENABLE_INT_OFS))(d))
#define WinMon_disable_int(d) \
  (((t_winmon_disable_int)WINMON_FUNC(WINMON_DISABLE_INT_OFS))(d))
#define WinMon_int_ack(d) \
  (((t_winmon_int_ack)WINMON_FUNC(WINMON_INT_ACK_OFS))(d))

#define WinMon_set_timer_int(h,t) \
  (((t_winmon_set_timer_int)WINMON_FUNC(WINMON_SET_TIMER_INT_OFS))(h,t))

#ifndef NO_AUTO_MEMMAP
#define WinMon_system(c) (((t_winmon_system)WINMON_FUNC(WINMON_SYSTEM_OFS))(c))

#define WinMon_get_bus_address(c) (((t_winmon_get_bus_address)WINMON_FUNC(WINMON_MEM_ADDR_OFS))(c))
#define WinMon_get_bus_size(c) (((t_winmon_get_bus_size)WINMON_FUNC(WINMON_MEM_SIZE_OFS))(c))
#define WinMon_is_bus_access(c) (((t_winmon_is_bus_access)WINMON_FUNC(WINMON_BUS_ACTIVE_OFS))(c))
#endif

#endif
#endif
