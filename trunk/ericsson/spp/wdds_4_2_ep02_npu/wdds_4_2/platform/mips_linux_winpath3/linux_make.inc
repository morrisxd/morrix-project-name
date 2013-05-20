#-*-Makefile-*-
# (C) Copyright 2004-2009, Wintegra. All rights reserved.
# WINTEGRA CONFIDENTIAL PROPRIETARY
# Contains Confidential Proprietary information of Wintegra.
# Reverse engineering is prohibited.
# The copyright notice does not imply publication.

# WPI_HOST = linux, WPI_TARGET = mips_linux, WPI_HW_DEVICE = winpath3

# Path
ifndef WP_PATH_LINUX_MIPS_LINUX_WINPATH3
$(warning **********************************************************************)
$(warning *** Environment variable WP_PATH_LINUX_MIPS_LINUX_WINPATH3 not set ***)
$(warning **********************************************************************)
endif

WPI_SET_PATH = PATH=$(WP_PATH_LINUX_MIPS_LINUX_WINPATH3);

# General tools
ifndef CROSS_COMPILE
CROSS_COMPILE = mipsisa32r2-timesys-linux-uclibc-
endif

AR = $(CROSS_COMPILE)ar
AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
CPP = $(CROSS_COMPILE)cpp
OBJCOPY = objcopy
SED = sed

# Compile/link flags that can be changed
CFLAGS_G = -O2
CFLAGS_STD = -std=c99 
CFLAGS_WERROR = -Werror -pedantic -Wno-declaration-after-statement

# CFLAGS
CFLAGS_BASE = -c $(CFLAGS_G) $(CFLAGS_WERROR) $(CFLAGS_STD) -G 0 -mips2 -msoft-float \
        -mno-memcpy  \
        -fomit-frame-pointer \
        -pipe \
        -Wall -Wstrict-prototypes \
        -D__NO_STRING_INLINES -Dlinux

ifdef CONFIG_SMP
CFLAGS_BASE += -D__SMP__ -DSMP
endif

# Defines used in building wpi_wddi.a only
LOG_FUNCTION = WPI_LOG_PRINTF
LOG_LEVEL = WPI_DEBUG_LEVEL_1

ifdef KERNEL_DIR
OS_INC_DIRS = -I$(KERNEL_DIR)
endif

# Defines used in debugging memory access
# Non-zero value means do memory check (if compiler permits)
# Zero value means do not do memory check (useful with -O0)
WP_MEM_CHECK = 1

WDDI_CFLAGS_BASE = \
        -DWPI_REAL_HOST \
        -D$(LOG_FUNCTION) \
        -DWPI_DEBUG_LEVEL=$(LOG_LEVEL) \
        -DWPI_OS_ENET_WINPATH=0 \
        -DINTERRUPT_MODEL_FLAT \
        -DWPI_OS_INT_SERVICE \
        -DWP_MEM_CHECK=$(WP_MEM_CHECK) \
        -DWIN_LINUX \
	-DWPI_WDDI_MULTI_THREADING_FINE_GRAIN_LOCKING=0 \
        -DWP_SHARED_WMM_CMD \

ifdef WPL_PRIVATE_HEAP_ALLOCATION
WDDI_CFLAGS_BASE += -DWPL_PRIVATE_HEAP_ALLOCATION \
	-DWPL_HEAP_BUS=$(WPL_HEAP_BUS) \
	-DWPL_HEAP_SIZE=$(WPL_HEAP_SIZE)
endif

CFLAGS_USER = $(CFLAGS_BASE) \
        -DDBG_TRACE -DDBG_TRACE_MSG=app_printf -DUSE_LINUX_SEMAPHORE=0

WDDI_CFLAGS_USER = $(WDDI_CFLAGS_BASE) -DWPI_THREAD_SAFE

#####################
# Set CFLAGS to USER
#####################
CFLAGS = $(CFLAGS_USER)
WDDI_CFLAGS = $(WDDI_CFLAGS_USER)

LDFLAGS = -G 0 -msoft-float #-static
ASFLAGS =

# Executable
EXE =

# Defines used in building executables
OTHER_LIB =
MAKE_SREC = no
LINK_FILE =
START_FILE =
