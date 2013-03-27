#
# (C) Copyright 2001-2005, Wintegra.  All rights reserved.
# WINTEGRA CONFIDENTIAL PROPRIETARY
# Contains Confidential Proprietary information of Wintegra.
# Reverse engineering is prohibited.
# The copyright notice does not imply publication.
#

#
# Module: makefile
# Purpose: Called with value for WPI_TARGET to build a specific wpu_phy.a
#

ifeq "$(WPI_BOARD)" ""
$(error Must specify WPI_BOARD to build in phy directory.)
endif

# Each makefile should know its host and how to find wddi
WPI_WDDI_DIR := ..
WPI_HOST_0 := $(OSTYPE:solaris%=solaris)
WPI_HOST := $(WPI_HOST_0:linux%=linux)

SRC_DIRS := $(WPI_WDDI_DIR)/phy

ifeq "$(WPI_HW_DEVICE)" "winpath2"
ifeq "$(WPI_BOARD)" "wds"
PHY_SRC := \
wpx_oc12_pmc5357.c \
wpx_oc3_pmc5351.c \
wpx_tdm_comet.c \
wpx_genet_mac.c \
wpx_pin_mux_selector.c \
wpx_ptp.c
else
PHY_SRC := \
wpx_oc12_pmc5357.c \
wpx_oc3_pmc5351.c \
wpx_tdm_comet.c\
wpx_genet_mac.c \
wpx_pin_mux_selector.c
endif
else
# case of winpath 3
	ifeq "$(WPI_BOARD)" "amc2c87w3"
	PHY_SRC := \
	wpx_genet_mac.c \
	wpx_pin_mux_selector.c
	else 
	PHY_SRC := \
	wpx_oc12_pmc5357.c \
	wpx_oc3_pmc5351.c \
	wpx_tdm_comet.c\
	wpx_genet_mac.c \
	wpx_pin_mux_selector.c \
	wpx_ptp_wp3.c \
	wpx_fpga_dc.c
	endif
endif

# vpath specifies the directories that are searched for .c files
vpath %.c $(SRC_DIRS)

# The objects, dependences, and archive
PHY_OBJ := $(patsubst %.c, $(WPI_HTB_OBJ_DIR)/%.o, $(PHY_SRC))
PHY_DEP := $(patsubst %.o, %.d, $(PHY_OBJ))
PHY_A := $(WPI_HTB_LIB_DIR)/wpx_phy.a
PHY_O := $(WPI_HTB_LIB_DIR)/wpx_phy_pl.o

# Get the host-target specific defines
include $(WPI_PLATFORM_DIR)/$(WPI_HOST)_make.inc


# The includes
INCLUDES := $(patsubst %, -I$(WPI_WDDI_DIR)/%, $(INCLUDE_DIRS)) -I$(WPI_PLATFORM_DIR)/include $(OS_INC_DIRS) -I$(WPI_WDDS_DIR)/include -I$(WPI_WDDS_DIR)/platform/include

.PHONY: phy clean default depend
default:
	@echo Build this from directory ..

ifeq "$(WPI_WDDI_PL_O)" "yes"
phy: $(PHY_A) $(PHY_O)
else
phy: $(PHY_A)
endif

depend: $(PHY_DEP)

clean:
	$(RM) $(PHY_DEP) $(PHY_OBJ) $(PHY_A) $(PHY_O)

$(PHY_A): $(PHY_DEP) $(PHY_OBJ)
	@echo "   AR" wpx_phy.a for $(WPI_TARGET):$(WPI_BOARD):$(WPI_HW_DEVICE)
	@$(WPI_SET_PATH) $(AR) -r $(PHY_A) $(PHY_OBJ) 2> ,ar

$(PHY_O): $(PHY_A)
	$(WPI_SET_PATH) $(LD) -r -o $(PHY_O) $(PHY_OBJ)

$(WPI_HTB_OBJ_DIR)/%.o: %.c
	@echo "   CC" $*.c
	$(WPI_SET_PATH) $(CC) $(CFLAGS) $(WDDI_CFLAGS) $(INCLUDES) -o $@ $<

$(WPI_HTB_OBJ_DIR)/%.d: %.c
	@echo "   DEP" $*.c
	set -e; \
	$(WPI_SET_PATH) $(CC) -M $(CFLAGS) $(WDDI_CFLAGS) $(INCLUDES) $< \
	| sed "s!\($*\)\.o[ :]*!$(WPI_HTB_OBJ_DIR)/\1.o $@ : !g" > $@; \
	[ -s $@ ] || $(RM) $@

ifeq "$(MAKE_CLEAN)" "no"
-include $(PHY_DEP)
endif
