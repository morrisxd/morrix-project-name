/*****************************************************************************
 * (C) Copyright Wintegra 2001, All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra, Inc.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ***************************************************************************/
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

// XALYO
#define LOAD_IN_RAM

//#define WINMON_MIPS_RAM_BASE     0x01000000 // XALYO
#define WINMON_MIPS_RAM_BASE     0x00000000
#define WINMON_MIPS_DSEG1_OFFSET 0xa0000000
#define WINMON_MIPS_FLASH_BASE   0x1fc00000
#define WINMON_MIPS_FUNCTION_TABLE_OFFSET  0x500


#ifdef LOAD_IN_RAM


#define WINMON_FUNCTION_TABLE    (WINMON_MIPS_RAM_BASE+\
                                  WINMON_MIPS_DSEG1_OFFSET+\
                                  WINMON_MIPS_FUNCTION_TABLE_OFFSET)

#else

#define WINMON_FUNCTION_TABLE    (WINMON_MIPS_FLASH_BASE+\
                                  WINMON_MIPS_DSEG1_OFFSET+\
                                  WINMON_MIPS_FUNCTION_TABLE_OFFSET)
#endif

#ifdef WINMON_GDB_OUTPUT
#define WINMON_PUTCHAR_OFS             9
#define WINMON_PUTS_OFS               10
#else
#define WINMON_PUTCHAR_OFS             0
#define WINMON_PUTS_OFS                1
#endif

#define WINMON_GETCHAR_OFS               2
#define WINMON_WAIT_OFS         	 3
#define WINMON_EXIT_OFS         	 4
#define WINMON_SETENV_OFS                5
#define WINMON_CLOCK_OFS                 6
#define WINMON_DISPLAY_OFS               7
#define WINMON_SIGNAL_OFS                8
#define WINMON_GETENV_OFS               11
#define WINMON_INT_CONNECT_OFS		12
#define WINMON_ENABLE_INTS_OFS		13
#define WINMON_DISABLE_INTS_OFS		14
#define WINMON_ENABLE_INT_OFS		15
#define WINMON_DISABLE_INT_OFS		16
#define WINMON_INT_ACK_OFS		17
#define WINMON_SET_TIMER_INT_OFS	18
#define WINMON_SYSTEM_OFS        	19
#define WINMON_MEM_ADDR_OFS             20
#define WINMON_MEM_SIZE_OFS             21
#define WINMON_BUS_ACTIVE_OFS           22
#define WINMON_OS_MANAGE_OFS            23
#define WINMON_PRINT_OFS                24
#define WINMON_FLUSH_OFS                25
#define WINMON_MUTEX_OP_OFS             26
#define WINMON_SET_MMU_MAP_OFS          27
#define WINON_INIT_MMU_OFS              28
#define WINON_INV_MMU_OFS               29
#define WINMON_GET_BUS_VADDR_OFS        30
#define WINMON_INT_CLOCK_OFS            31
#define WINMON_SET_INT_PARTITION_OFS    32
#define WINMON_GET_CORE_ID_OFS          33
#define WINMON_GET_TC_ID_OFS            34
#define WINMON_SET_MT_PRINTS            35
#define WINMON_GET_MINIMON_DESC         36
#define WINMON_GET_MULTI_WP_DESC        37


#define WINMON_FUNC(ofs) (((WP_U32 *)(WINMON_FUNCTION_TABLE))[ofs])

typedef void (*t_winmon_exit)(WP_S32 rc);
typedef WP_S32  (*t_winmon_putchar)(WP_S32 c);
typedef WP_S32  (*t_winmon_puts)(WP_CHAR *s);
typedef WP_S32  (*t_winmon_getchar)(void);
typedef WP_S32  (*t_winmon_setenv)(WP_CHAR *, WP_CHAR *);
typedef WP_S32  (*t_winmon_wait)(WP_U32 t);
typedef WP_S32  (*t_winmon_clock)(void);
typedef void (*t_winmon_display)(WP_U8);
typedef void *(*t_winmon_signal)(WP_S32, void *);
typedef WP_CHAR *(*t_winmon_getenv)(WP_CHAR *);


typedef void *(*t_winmon_int_connect)(WP_S32, void (*)(WP_S32));
typedef void (*t_winmon_enable_interrupts)(void);
typedef WP_S32  (*t_winmon_disable_interrupts)(void);
typedef void (*t_winmon_enable_int)(WP_S32, WP_S32);
typedef void (*t_winmon_disable_int)(WP_S32, WP_S32);
typedef void (*t_winmon_int_ack)(WP_S32);
typedef void (*t_winmon_set_timer_int)(void *,WP_S32);
typedef void (*t_winmon_system)(WP_CHAR *);

typedef WP_S32  (*t_winmon_get_bus_address)(WP_S32 c);
typedef WP_S32  (*t_winmon_get_bus_size)(WP_S32 c);
typedef WP_S32  (*t_winmon_is_bus_access)(WP_S32 c);

typedef WP_S32  (*t_winmon_set_manange_os)(WP_S32 c);

typedef WP_S32  (*t_winmon_print)(WP_S32 c);
typedef void  (*t_winmon_flush)(void);
typedef WP_S32  (*t_winmon_mutex_op)( WP_S32 lock, WP_S32 wait_mode );

typedef WP_U32  (*t_winmon_set_mmu_map)(WP_U32 a, WP_U32 b, WP_U32 c, WP_U32 d);
typedef WP_U32  (*t_winmon_init_mmu)(void);
typedef void          (*t_winmon_invalidate_mmu)(WP_U32 a);
typedef WP_U32  (*t_winmon_get_bus_vaddr)(WP_U32 c);

typedef void          (*t_winmon_internal_clock)(WP_U32 *intval, WP_U32 *fracval);

typedef WP_U32  (*t_winmon_set_int_partition)(WP_U32 partition, WP_U32 size);

typedef WP_U32  (*t_winmon_get_core_id)(void);
typedef WP_U32  (*t_winmon_get_tc_id)(void);

typedef void          (*t_winmon_set_mt_prints)(WP_S32);

typedef WP_U32* (*t_winmon_get_minmon_desc)(void);
typedef WP_U32* (*t_winmon_get_multi_wp_desc)(void);


#ifdef WINMON_CONTEXT

#include "sysio.h"
#include "sysdcio.h"
#include "cache_mmu.h"
#include "intr.h"

#define WinMon_putchar(c) SysDCIO_putchar(c)
#define WinMon_puts(c)    SysDCIO_puts(c)
#define WinMon_getchar()  SysIO_getchar()
#define WinMon_wait(c)    SysIO_wait(c)
#define WinMon_clock()    SysIO_GetClock()
#define WinMon_display(s) SysIO_Display(s)
#define WinMon_signal(e,s)  Int_signal(e,s)
#define WinMon_getenv(name) (WP_CHAR *)Env_getenv(name)
#define WinMon_setenv(var,name) Env_setenv(var, name)

#define WinMon_int_connect(e,s)  	Int_IntConnect(e,s)
#define WinMon_enable_interrupts()	sys_enable_int()
#define WinMon_disable_interrupts()	sys_disable_int()
#define WinMon_enable_int(d,c)		Int_EnableInterrupt(d,c)
#define WinMon_disable_int(d,c)		Int_DisableInterrupt(d,c)
#define WinMon_int_ack(d)		Int_IntrAck(d)
#define WinMon_set_timer_int(h,t)	Int_SetTimerInt(h,t)
#define WinMon_system(c)                shellSystem(c)

#define WinMon_get_bus_address(c)       sys_get_bus_address(c)
#define WinMon_get_bus_size(c)          sys_get_bus_size(c)
#define WinMon_is_bus_access(c)         sys_is_bus_access(c)

#define WinMon_set_manange_os(c)        sys_set_management_os(c)

#define WinMon_print(c)                 SysDCIO_putchar(c)
#define WinMon_flush()                  SysIO_FlushUART()
#define WinMon_mutex_op(c,d)            SysDCIO_MutexOp(c,d)

#define WinMon_set_mmu_map(a,b,c,d)          sys_add_mmu_mapping_entries(a,b,c,d)
#define WinMon_init_mmu()                    sys_mmu_init_mapping()
#define WinMon_invalidate_mmu(r)             sys_invalidate_all_mmu_setup(r)
#define WinMon_get_bus_vaddr(c)              sys_get_bus_vaddr(c)

#define WinMon_internal_clock(i, f)          SysIO_GetInternalClock(i, f)

#define WinMon_set_int_partition(p, s)       sys_allocate_partition(p, s)

#define WinMon_get_core_id()                 sys_get_core_id();
#define WinMon_get_tc_id()                   sys_get_tc_id();

#define Winmon_set_mt_prints(b)              SysDCIO_set_mt_prints(b);

#define Winmon_get_minimon_desc()            MinimonGetDescriptor();
#define Winmon_get_multi_wp_desc()           SysIO_get_multiple_winpath_desc();

#else

#define WinMon_putchar(c)\
                (((t_winmon_putchar)WINMON_FUNC(WINMON_PUTCHAR_OFS))(c))
#define WinMon_puts(c) (((t_winmon_puts)WINMON_FUNC(WINMON_PUTS_OFS))(c))
#define WinMon_getchar() \
  (((t_winmon_getchar)WINMON_FUNC(WINMON_GETCHAR_OFS))())
#define WinMon_wait(c) (((t_winmon_wait)WINMON_FUNC(WINMON_WAIT_OFS))(c))
#define WinMon_exit(rc) (((t_winmon_exit)WINMON_FUNC(WINMON_EXIT_OFS))(rc))
#define WinMon_clock() (((t_winmon_clock)WINMON_FUNC(WINMON_CLOCK_OFS))())
#define WinMon_display(s) \
  (((t_winmon_display)WINMON_FUNC(WINMON_DISPLAY_OFS))(s))
#define WinMon_signal(e,s) \
  (((t_winmon_signal)WINMON_FUNC(WINMON_SIGNAL_OFS))(e,s))
#define WinMon_getenv(name) \
  (((t_winmon_getenv)WINMON_FUNC(WINMON_GETENV_OFS))(name))
#define WinMon_setenv(var, name) \
  (((t_winmon_setenv)WINMON_FUNC(WINMON_SETENV_OFS))(var, name))


#define WinMon_int_connect(e,s) \
  (((t_winmon_int_connect)WINMON_FUNC(WINMON_INT_CONNECT_OFS))(e,s))
#define WinMon_enable_interrupts() \
  (((t_winmon_enable_interrupts)WINMON_FUNC(WINMON_ENABLE_INTS_OFS))())
#define WinMon_disable_interrupts() \
  (((t_winmon_disable_interrupts)WINMON_FUNC(WINMON_DISABLE_INTS_OFS))())

#define WinMon_enable_int(d,c) \
  (((t_winmon_enable_int)WINMON_FUNC(WINMON_ENABLE_INT_OFS))(d,c))
#define WinMon_disable_int(d,c) \
  (((t_winmon_disable_int)WINMON_FUNC(WINMON_DISABLE_INT_OFS))(d,c))
#define WinMon_int_ack(d) \
  (((t_winmon_int_ack)WINMON_FUNC(WINMON_INT_ACK_OFS))(d))

#define WinMon_set_timer_int(h,t) \
  (((t_winmon_set_timer_int)WINMON_FUNC(WINMON_SET_TIMER_INT_OFS))(h,t))
#define WinMon_system(c) (((t_winmon_system)WINMON_FUNC(WINMON_SYSTEM_OFS))(c))

#define WinMon_get_bus_address(c) (((t_winmon_get_bus_address)WINMON_FUNC(WINMON_MEM_ADDR_OFS))(c))
#define WinMon_get_bus_size(c) (((t_winmon_get_bus_size)WINMON_FUNC(WINMON_MEM_SIZE_OFS))(c))
#define WinMon_is_bus_access(c) (((t_winmon_is_bus_access)WINMON_FUNC(WINMON_BUS_ACTIVE_OFS))(c))

#define WinMon_set_manange_os(c) (((t_winmon_set_manange_os)WINMON_FUNC(WINMON_OS_MANAGE_OFS))(c))

#define WinMon_print(c) (((t_winmon_print)WINMON_FUNC(WINMON_PRINT_OFS))(c))
#define WinMon_flush() (((t_winmon_flush)WINMON_FUNC(WINMON_FLUSH_OFS))())
#define WinMon_mutex_op(c,d) \
  (((t_winmon_mutex_op)WINMON_FUNC(WINMON_MUTEX_OP_OFS))(c,d))
#define WinMon_set_mmu_map(a,b,c,d)          (((t_winmon_set_mmu_map)WINMON_FUNC(WINMON_SET_MMU_MAP_OFS))(a,b,c,d))
#define WinMon_init_mmu()                    (((t_winmon_init_mmu)WINMON_FUNC(WINON_INIT_MMU_OFS))())
#define WinMon_invalidate_mmu(r)             (((t_winmon_invalidate_mmu)WINMON_FUNC(WINON_INV_MMU_OFS))(r)) 

#define WinMon_get_bus_vaddr(c)              (((t_winmon_get_bus_vaddr)WINMON_FUNC(WINMON_GET_BUS_VADDR_OFS))(c)) 

#define WinMon_internal_clock(i, f)          (((t_winmon_internal_clock)WINMON_FUNC(WINMON_INT_CLOCK_OFS))(i, f)) 

#define WinMon_set_int_partition(p, s)       (((t_winmon_set_int_partition)WINMON_FUNC(WINMON_SET_INT_PARTITION_OFS))(p, s)) 

#define WinMon_get_core_id()                 (((t_winmon_get_core_id)WINMON_FUNC(WINMON_GET_CORE_ID_OFS))())
#define WinMon_get_tc_id()                   (((t_winmon_get_tc_id)WINMON_FUNC(WINMON_GET_TC_ID_OFS))())

#define Winmon_set_mt_prints(b)              (((t_winmon_set_mt_prints)WINMON_FUNC(WINMON_SET_MT_PRINTS))(b))

#define Winmon_get_minimon_desc()            (((t_winmon_get_minmon_desc)WINMON_FUNC(WINMON_GET_MINIMON_DESC))())
#define Winmon_get_multi_wp_desc()            (((t_winmon_get_multi_wp_desc)WINMON_FUNC(WINMON_GET_MULTI_WP_DESC))())

#endif
#endif
