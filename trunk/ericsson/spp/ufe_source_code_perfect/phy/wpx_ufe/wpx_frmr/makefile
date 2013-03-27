# (C) Copyright 2001-2010, Wintegra.  All rights reserved.
# WINTEGRA CONFIDENTIAL PROPRIETARY
# Contains Confidential Proprietary information of Wintegra.
# Reverse engineering is prohibited.
# The copyright notice does not imply publication.
#
# Module: makefile

ifeq "$(WPI_TARGET)" ""
$(error WPI_TARGET must be defined.)
endif

ifeq "$(WPI_BOARD)" ""
$(error WPI_BOARD must be specified.)
endif

ifeq "$(WUFE)" ""
$(error WUFE must be specified.)
endif

ifeq "$(WUFE)" "ufe412"
UFE_VER=ufe4
else
ifeq "$(WUFE)" "ufe448"
UFE_VER=ufe4
else
ifeq "$(WUFE)" "ufe412dual"
UFE_VER=ufe4
else
$(error WUFE must be ufe412 or ufe448.)
endif
endif
endif
ifeq "$(FRAMER)" ""
$(error FRAMER flexmux must be specified.)
endif

WPI_BOARD_HW_DEVICE :=$(WPI_BOARD)_$(WPI_HW_DEVICE)

# Each makefile should know its host and how to find wddi
ifeq "$(WPI_WDDS_DIR)" ""
WPI_WDDS_DIR = ../../../..
endif
WPI_WDDI_DIR := $(WPI_WDDS_DIR)/wddi
WPI_UFE_DIR := $(WPI_WDDS_DIR)/wddi/phy/wpx_ufe
WPI_FLEXMUX_DIR := $(WPI_UFE_DIR)/wpx_frmr/flexmux
WPI_HOST_0 := $(OSTYPE:solaris%=solaris)
WPI_HOST := $(WPI_HOST_0:linux%=linux)
ifeq "$(WUFE)" "ufe412"
FIRMWARE_IMAGE_DIR:=$(WPI_FLEXMUX_DIR)/../firmware_image_ufe412
else
ifeq "$(WUFE)" "ufe448"
FIRMWARE_IMAGE_DIR:=$(WPI_FLEXMUX_DIR)/../firmware_image_ufe448
else
ifeq "$(WUFE)" "ufe412dual"
FIRMWARE_IMAGE_DIR:=$(WPI_FLEXMUX_DIR)/../firmware_image_ufe412
endif
endif
endif
include $(WPI_FLEXMUX_DIR)/makefile
# vpath %.c $(FLEXMUX_SRC_DIRS)
# Where to construct the library
UFE_FRAMER_OBJ_DIR := ${WPI_WDDS_DIR}/objects/$(WPI_HOST)/$(WPI_TARGET)/$(WPI_BOARD)/$(WUFE)/obj
UFE_FRAMER_LIB_DIR := ${WPI_WDDS_DIR}/objects/$(WPI_HOST)/$(WPI_TARGET)/$(WPI_BOARD)/$(WUFE)/lib

# Platform variables
WPI_PLATFORM_DIR := $(WPI_WDDS_DIR)/platform/$(WPI_TARGET)

# Get the host-target specific defines
include $(WPI_PLATFORM_DIR)/$(WPI_HOST)_make.inc

# The includes
INCLUDES := $(OS_INC_DIRS)                                                     \
	-I../inc                                                               \
	-I$(WPI_WDDI_DIR)/target/$(WPI_TARGET)/include                         \
	-I$(WPI_WDDI_DIR)/board/$(WPI_BOARD_HW_DEVICE)                         \
	-I$(WPI_WDDI_DIR)/sources                                              \
	-I$(WPI_WDDI_DIR)/sources/api                                          \
	-I$(WPI_WDDS_DIR)/include                                              \
	-I$(WPI_WDDS_DIR)/platform/include                                     \
	-I$(WPI_PLATFORM_DIR)/include                                          \
	-I$(WPI_UFE_DIR)/fpga/                                    \
	-I$(WPI_UFE_DIR)/fpga/$(UFE_VER)                          \
	-I$(WPI_UFE_DIR)/board/$(WPI_BOARD_HW_DEVICE)/$(UFE_VER)  \
	-I$(WPI_UFE_DIR)/api                                      \
	-I$(WPI_FLEXMUX_DIR)                                                   \
	-I$(WPI_FLEXMUX_DIR)/00_api_layer/inc                                  \
	-I$(WPI_FLEXMUX_DIR)/02_model_layer/configuration/defaults/inc         \
	-I$(WPI_FLEXMUX_DIR)/04_low_level_comms_layer/inc                      \
	-I$(WPI_FLEXMUX_DIR)/10_qc                                             \
	-I$(WPI_FLEXMUX_DIR)/../low_level_comms_library/api                    \
	-I$(WPI_FLEXMUX_DIR)/../common/inc                                  \
	-I$(FIRMWARE_IMAGE_DIR)

include $(WPI_FLEXMUX_DIR)/makefile

HEADER_DEPS := $(wildcard $(WPI_FLEXMUX_DIR)/*.h \
		$(WPI_FLEXMUX_DIR)/00_api_layer/inc/*.h \
		$(WPI_FLEXMUX_DIR)/02_model_layer/configuration/defaults/inc/*.h \
		$(WPI_FLEXMUX_DIR)/04_low_level_comms_layer/inc/*.h \
		$(WPI_FLEXMUX_DIR)/10_qc/*.h \
		$(WPI_FLEXMUX_DIR)/../low_level_comms_library/api/*.h \
		$(WPI_FLEXMUX_DIR)/../../common/inc/*.h \
		$(FIRMWARE_IMAGE_DIR)/*.h) 
CFLAGS_STD = $(FRAMER_CFLAGS_STD)
CFLAGS_WERROR = $(FRAMER_CFLAGS_WERROR)

WUFE_CFLAGS	= $(CFLAGS) $(WDDI_CFLAGS) $(UFE_CFLAGS)

ifeq "$(WUFE)" "ufe412dual"  
WUFE_CFLAGS += -DWUFE_DUAL_EMPHY
else
WUFE_CFLAGS += -UWUFE_DUAL_EMPHY
endif

# The objects, dependences, and archive
PHY_UFE_FRAMER_OBJ := $(PHY_FLEXMUX_OBJ)
PHY_UFE_FRAMER_DEP := $(patsubst %.o, %.d, $(PHY_UFE_FRAMER_OBJ))
PHY_UFE_FRAMER_A := $(UFE_FRAMER_LIB_DIR)/wpx_phy_ufe_framer.a
PHY_UFE_FRAMER_O := $(UFE_FRAMER_LIB_DIR)/wpx_phy_ufe_framer_pl.o

.PHONY: all clean default depend

all:ufe_framer_phy
default:
	@echo Build this from directory ..

ifeq "$(WPI_WDDI_PL_O)" "yes"
ufe_framer_phy: $(PHY_UFE_FRAMER_A) $(PHY_UFE_FRAMER_O)
else
ufe_framer_phy: $(PHY_UFE_FRAMER_A)
endif

depend: $(PHY_UFE_FRAMER_DEP)

clean:
	$(RM) $(PHY_UFE_FRAMER_DEP) $(PHY_UFE_FRAMER_OBJ) $(PHY_UFE_FRAMER_A) $(PHY_UFE_FRAMER_O)

$(PHY_UFE_FRAMER_A): $(PHY_UFE_FRAMER_DEP) $(PHY_UFE_FRAMER_OBJ)
	@echo "   AR" wpx_phy_ufe_framer.a
	$(WPI_SET_PATH) $(AR) -r $(PHY_UFE_FRAMER_A) $(PHY_UFE_FRAMER_OBJ) 2>,ar
	$(RM) ,ar

$(PHY_UFE_FRAMER_O): $(PHY_UFE_FRAMER_A)
	@echo "   LD" $(PHY_UFE_FRAMER_O)
	$(WPI_SET_PATH) $(LD) -r -o $(PHY_UFE_FRAMER_O) $(PHY_UFE_FRAMER_OBJ)

$(UFE_FRAMER_OBJ_DIR)/%.o: $(FRAMER)/%.c $(HEADER_DEPS)
	@echo "   CC" $*.c
	$(WPI_SET_PATH) $(CC) $(WUFE_CFLAGS) $(INCLUDES) -o $@ $<

$(UFE_FRAMER_OBJ_DIR)/%.o: %.c $(HEADER_DEPS)
	@echo "   CC" $*.c
	$(WPI_SET_PATH) $(CC) $(WUFE_CFLAGS) $(INCLUDES) -o $@ $<

$(UFE_FRAMER_OBJ_DIR)/%.d: $(FRAMER)/%.c $(HEADER_DEPS)
#	@echo "   bbbb" $(PHY_UFE_FRAMER_DEP)
	@echo "   DEP" $*.c
	set -e; \
	$(WPI_SET_PATH) $(CC) -M $(WUFE_CFLAGS) $(INCLUDES) $< \
	| sed "s!\($*\)\.o[ :]*!$(UFE_FRAMER_OBJ_DIR)/\1.o $@ : !g" > $@; \
	[ -s $@ ] || $(RM) $

$(UFE_FRAMER_OBJ_DIR)/%.d: %.c $(HEADER_DEPS)
#	@echo "   bbbb" $(PHY_UFE_FRAMER_DEP)
	@echo "   DEP" $*.c
	set -e; \
	$(WPI_SET_PATH) $(CC) -M $(WUFE_CFLAGS) $(INCLUDES) $< \
	| sed "s!\($*\)\.o[ :]*!$(UFE_FRAMER_OBJ_DIR)/\1.o $@ : !g" > $@; \
	[ -s $@ ] || $(RM) $@

ifeq "$(MAKE_CLEAN)" "no"
-include $(PHY_UFE_FRAMER_DEP)
endif
