#
# (C) Copyright 2003-2006, Wintegra.  All rights reserved.
# WINTEGRA CONFIDENTIAL PROPRIETARY
# Contains Confidential Proprietary information of Wintegra.
# Reverse engineering is prohibited.
# The copyright notice does not imply publication.
#

#
# Module: makefile
# Purpose: Called with value for WPI_TARGET to build a specific wpx_board.a
#
ifeq "$(WPI_BOARD)" ""
$(error Must specify WPI_BOARD to build in board directory.)
endif

# Each makefile should know its host and how to find wddi
WPI_WDDI_DIR := ..
WPI_HOST_0 := $(OSTYPE:solaris%=solaris)
WPI_HOST := $(WPI_HOST_0:linux%=linux)

SRC_DIRS := $(WPI_WDDI_DIR)/board/$(WPI_BOARD)
SRC_DIRS += $(WPI_WDDI_DIR)/board

BOARD_SRC := wpx_board_data.c wpx_board_functions.c wpi_platform.c wpx_enet_phy.c
BOARD_SRC += wpx_board_if.c

ifeq "$(WPI_HW_DEVICE)" "winpath3"
ifneq "${WPI_TARGET}" "winsim"
ifneq "${WPI_TARGET}" "verilog"
BOARD_SRC += wpx_pinmux.c wpx_serdes.c
endif
endif
endif

# vpath specifies the directories that are searched for .c files
vpath %.c $(SRC_DIRS)

# The objects, dependences, and archive
BOARD_OBJ := $(patsubst %.c, $(WPI_HTB_OBJ_DIR)/%.o, $(BOARD_SRC))
BOARD_DEP := $(patsubst %.o, %.d, $(BOARD_OBJ))
BOARD_A := $(WPI_HTB_LIB_DIR)/wpx_board.a
BOARD_O := $(WPI_HTB_LIB_DIR)/wpx_board_pl.o

# Get the host-target specific defines
include $(WPI_PLATFORM_DIR)/$(WPI_HOST)_make.inc

# The includes
INCLUDES := $(patsubst %, -I$(WPI_WDDI_DIR)/%, $(INCLUDE_DIRS)) -I$(WPI_PLATFORM_DIR)/include $(OS_INC_DIRS) -I../phy -I$(WPI_WDDS_DIR)/include -I$(WPI_WDDS_DIR)/platform/include

.PHONY: board clean default depend
default:
	@echo Build this from directory ..

ifeq "$(WPI_WDDI_PL_O)" "yes"
board: $(BOARD_A) $(BOARD_O)
else
board: $(BOARD_A)
endif

depend: $(BOARD_DEP)

clean:
	$(RM) $(BOARD_DEP) $(BOARD_OBJ) $(BOARD_A) $(BOARD_O)

$(BOARD_A): $(BOARD_DEP) $(BOARD_OBJ)
	@echo "   AR" wpx_board.a for $(WPI_BOARD):$(WPI_TARGET):$(WPI_HW_DEVICE)
	@$(WPI_SET_PATH) $(AR) -r $(BOARD_A) $(BOARD_OBJ) 2> ,ar

$(BOARD_O): $(BOARD_A)
	$(WPI_SET_PATH) $(LD) -r -o $(BOARD_O) $(BOARD_OBJ)

$(WPI_HTB_OBJ_DIR)/%.o: %.c
	@echo "   CC" $*.c
	$(WPI_SET_PATH) $(CC) $(CFLAGS) $(WDDI_CFLAGS) $(INCLUDES) \
	-DWPX_TARGET_$(WPI_TARGET) -o $@ $<

$(WPI_HTB_OBJ_DIR)/%.d: %.c
	@echo "   DEP" $*.c
	set -e; \
	$(WPI_SET_PATH) $(CC) -M $(CFLAGS) $(WDDI_CFLAGS) $(INCLUDES) \
	-DWPX_TARGET_$(WPI_TARGET) $< \
	| sed "s!\($*\)\.o[ :]*!$(WPI_HTB_OBJ_DIR)/\1.o $@ : !g" > $@; \
	[ -s $@ ] || $(RM) $@

ifeq "$(MAKE_CLEAN)" "no"
-include $(BOARD_DEP)
endif
