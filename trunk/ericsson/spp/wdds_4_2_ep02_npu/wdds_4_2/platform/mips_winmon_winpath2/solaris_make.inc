#-*-Makefile-*-
# Copyright 2001-2009, Wintegra.  All rights reserved.
# WINTEGRA CONFIDENTIAL PROPRIETARY
# Contains Confidential Proprietary information of Wintegra.
# Reverse engineering is prohibited.
# The copyright notice does not imply publication.

# WPI_HOST = solaris, WPI_TARGET = mips_winmon, WPI_HW_DEVICE = winpath2

# Path
ifndef WP_PATH_SOLARIS_MIPS_WINMON_WINPATH2
$(warning *************************************************************************)
$(warning *** Environment variable WP_PATH_SOLARIS_MIPS_WINMON_WINPATH2 not set ***)
$(warning *************************************************************************)
endif

WPI_SET_PATH = PATH=$(WP_PATH_SOLARIS_MIPS_WINMON_WINPATH2)
PPATH = .

# General tools
AR = sde-ar
AS = sde-as
CC = sde-gcc
LD =  sde-ld
CPP = sde-cpp
OBJCOPY = sde-objcopy
MV = mv
SED = sed

# Compile/link flags
CFLAGS_G = -g
CFLAGS_WERROR = -Werror -pedantic -Wno-declaration-after-statement
CFLAGS_STD = -std=c99

CFLAGS = -c -mips32 -Wall $(CFLAGS_STD) \
        -nostdinc \
	-G 0 \
        -fno-builtin -fno-inline-functions -fno-strict-aliasing -fsigned-char \
        -Wpointer-arith -Wstrict-prototypes -Wformat -Wno-main \
        $(CFLAGS_G) $(CFLAGS_WERROR) -O2

LDFLAGS = -mips32 -nostdlib -nostartfiles
ASFLAGS = $(CFLAGS_G) -mips32

# Executable
EXE =

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
	-DWP_MEM_CHECK=$(WP_MEM_CHECK)

# Defines used in building executables
OTHER_LIB = $(WPI_T_DIR)/lib/libcsde.a
MAKE_SREC = yes
LINK_FILE = -T $(WPI_T_DIR)/link.xn
START_FILE = start.o
START_FILE_DIR = $(WPI_HT_DIR)/lib
