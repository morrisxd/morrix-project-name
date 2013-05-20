#-*-Makefile-*-
# Copyright 2001-2009, Wintegra.  All rights reserved.
# WINTEGRA CONFIDENTIAL PROPRIETARY
# Contains Confidential Proprietary information of Wintegra.
# Reverse engineering is prohibited.
# The copyright notice does not imply publication.

# WPI_HOST = linux, WPI_TARGET = winsim, WPI_HW_DEVICE = winpath3

# Path
ifndef WP_PATH_LINUX_WINSIM_WINPATH3
$(warning ******************************************************************)
$(warning *** Environment variable WP_PATH_LINUX_WINSIM_WINPATH3 not set ***)
$(warning ******************************************************************)
endif

WPI_SET_PATH = PATH=$(WP_PATH_LINUX_WINSIM_WINPATH3);

# General tools
AR = ar
CC = gcc
LD = ld
OBJCOPY = objcopy
MV = mv
SED = sed

# Compile/link flags
CFLAGS_G = -g
CFLAGS_WERROR = 
CFLAGS_STD = -std=c99 -pedantic -Wno-declaration-after-statement

CFLAGS = -c $(CFLAGS_STD) \
        -fno-builtin -fsigned-char -m32 \
        -DUSE_SOCKET \
        -Wall -Wpointer-arith -Wstrict-prototypes -Wformat -Wno-main \
        $(CFLAGS_G) $(CFLAGS_WERROR) -O0

LDFLAGS = -m32

# Executable
EXE =

# Zero value means do not do memory check (useful with -O0)
WP_MEM_CHECK = 0

# Winsim works with enhanced sockets
WP_WINSIM_CMD = 1

# Do not clear allocated memory (0 == clear)
WPI_DONT_MEMCLR = 0

# Defines used in building wpi_wddi.a only
LOG_FUNCTION = WPI_LOG_PRINTF
LOG_LEVEL = WPI_DEBUG_LEVEL_2
WDDI_CFLAGS = -DWINSIM -DNOLOAD_DPS -D$(LOG_FUNCTION) \
              -DWPI_DEBUG_LEVEL=$(LOG_LEVEL) \
              -UWPI_USING_ENET_SWITCH \
              -DWPI_OS_ENET_WINPATH=0 -DINTERRUPT_MODEL_FLAT \
              -DWP_MEM_CHECK=$(WP_MEM_CHECK) \
              -DWP_WINSIM_CMD=$(WP_WINSIM_CMD)\
              -DWPI_WDDI_MULTI_THREADING_FINE_GRAIN_LOCKING=0 \
              -DWPI_DONT_MEMCLR=$(WPI_DONT_MEMCLR)

# Defines used in building executables
OTHER_LIB = -lnsl
MAKE_SREC = no
LINK_FILE =
START_FILE =
