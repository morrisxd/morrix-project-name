#-*-Makefile-*-
# Copyright 2001-2009, Wintegra.  All rights reserved.
# WINTEGRA CONFIDENTIAL PROPRIETARY
# Contains Confidential Proprietary information of Wintegra.
# Reverse engineering is prohibited.
# The copyright notice does not imply publication.

# WPI_HOST = linux, WPI_TARGET = mips_winmon, WPI_HW_DEVICE = winpath3

# Path
ifndef WP_PATH_LINUX_MIPS_WINMON_WINPATH3
$(warning ***********************************************************************)
$(warning *** Environment variable WP_PATH_LINUX_MIPS_WINMON_WINPATH3 not set ***)
$(warning ***********************************************************************)
endif

WPI_SET_PATH = PATH=$(WP_PATH_LINUX_MIPS_WINMON_WINPATH3);

# General tools
TOOLCHAIN=mips-sde-elf
AR = $(TOOLCHAIN)-ar
AS = $(TOOLCHAIN)-as
CC = $(TOOLCHAIN)-gcc
LD =  $(TOOLCHAIN)-ld
CPP = $(TOOLCHAIN)-cpp
OBJCOPY = $(TOOLCHAIN)-objcopy
MV = mv
SED = sed

# Compile/link flags
# If optimization is not -O2, then it is probably desirable to
# also change the value of WP_MEM_CHECK to 0
CFLAGS_G = #-g
CFLAGS_STD = -std=c99
CFLAGS_WERROR = -Werror -pedantic -Wno-declaration-after-statement

CFLAGS = -c -mips32 -G 0\
        -nostdinc $(CFLAGS_STD) \
        -fno-builtin -fno-inline-functions -fsigned-char -fno-pic -msoft-float\
        -Wall -Wpointer-arith -Wstrict-prototypes -Wformat -Wno-main \
        $(CFLAGS_G) $(CFLAGS_WERROR) -O2

LDFLAGS = -mips32 -nostdlib -nostartfiles -fno-pic -msoft-float
ASFLAGS = $(CFLAGS_G) -mips32

# Executable
EXE = .exe

# Defines used in debugging memory access
# Non-zero value means do memory check (if compiler permits)
# Zero value means do not do memory check (useful with -O0)
WP_MEM_CHECK = 1

# Defines used in building wpi_wddi.a only
LOG_FUNCTION = WPI_LOG_PRINTF
LOG_LEVEL = WPI_DEBUG_LEVEL_1
WDDI_CFLAGS = -DWPI_REAL_HOST -D$(LOG_FUNCTION) \
        -DWPI_DEBUG_LEVEL=$(LOG_LEVEL) \
        -UWPI_USING_ENET_SWITCH \
        -DWPI_OS_ENET_WINPATH=0 -DINTERRUPT_MODEL_FLAT -DWPI_OS_INT_SERVICE \
	-DWPI_WDDI_MULTI_THREADING_FINE_GRAIN_LOCKING=0 \
	-DWPL_BSP_VERSION_CHECK=1 \
        -DWP_MEM_CHECK=$(WP_MEM_CHECK)

# Defines used in building executables
OTHER_LIB = $(WPI_T_DIR)/lib/mtlib.a $(WPI_T_DIR)/lib/libcsde.a
MAKE_SREC = yes
LINK_FILE = -T $(WPI_T_DIR)/link.xn
LINK_FILE_0 = -T $(WPI_T_DIR)/link.winmon.xn
LINK_FILE_1 = -T $(WPI_T_DIR)/link.minimon.xn
START_FILE = start.o
START_FILE_DIR = $(WPI_HT_DIR)/lib
