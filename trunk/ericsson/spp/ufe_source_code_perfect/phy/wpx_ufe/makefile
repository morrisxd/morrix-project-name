# (C) Copyright 2000-2006, Wintegra. All rights reserved.
#  WINTEGRA CONFIDENTIAL PROPRIETARY
#  Contains Confidential Proprietary information of Wintegra.
#  Reverse engineering is prohibited.
#  The copyright notice does not imply publication.

# WINTEGRA UFE  make file

WUFE_DIR	= .
WUFE_WDDI_DIR	= $(WUFE_DIR)/../..
WUFE_TARGET 	= $(WPI_TARGET)
WUFE_HOST_0 	= $(OSTYPE:solaris%=solaris)
WUFE_HOST 	= $(WUFE_HOST_0:linux%=linux)

ifeq "$(WPI_WDDS_DIR)" ""
WUFE_WDDS_DIR := $(WUFE_WDDI_DIR)/..
else
WUFE_WDDS_DIR := $(WPI_WDDS_DIR)
endif

ifeq "$(V)" "1"
export SILENTLY =
else
export SILENTLY = -s
endif

ifeq "$(WUFE)" ""
$(error WUFE is not specified)
endif

ifeq "$(WPI_BOARD)" ""
$(error WPI_BOARD must be defined.)
endif

ifeq "$(WUFE)" "ufe3"
UFE_CFLAGS := -DWUFE_UFE3
UFE_VER=ufe3
else
ifeq "$(WUFE)" "ufe4"
$(error ufe4 is not supported anymore, WUFE must be set to ufe412 or ufe448)
else
ifeq "$(WUFE)" "ufe412"
UFE_CFLAGS := -DWUFE_UFE4
UFE_VER=ufe4
else
ifeq "$(WUFE)" "ufe448"
UFE_CFLAGS := -DWUFE_UFE4
UFE_CFLAGS += -DWUFEI_UFE448
UFE_VER=ufe4
else
ifeq "$(WUFE)" "ufe412dual"
UFE_CFLAGS := -DWUFE_UFE4
UFE_VER=ufe4
endif
endif
endif
endif
endif
WPI_BOARD_HW_DEVICE :=$(WPI_BOARD)_$(WPI_HW_DEVICE)

WUFE_FPGA_DIR := $(WUFE_DIR)/fpga/$(UFE_VER)
WUFE_FPGA_BS_DIR := $(WUFE_DIR)/board/$(WPI_BOARD_HW_DEVICE)/$(UFE_VER)
WUFE_BOARD_DATA_FILE := wpx_$(UFE_VER)_data
PMC_DIR=$(WUFE_DIR)/wpx_pmc
UFE_FRAMER_DIR=$(WUFE_DIR)/wpx_frmr


ifneq "$(WUFE_TARGET)" ""
include $(WUFE_WDDS_DIR)/platform/$(WUFE_TARGET)/$(WUFE_HOST)_make.inc
WUFE_CFLAGS	= $(CFLAGS) $(WDDI_CFLAGS) $(UFE_CFLAGS)
endif

WUFE_INCLUDE_DIRS = -I$(WUFE_DIR)/api -I$(WUFE_DIR)/sources/include  \
	-I$(WUFE_DIR)/fpga -I$(WUFE_FPGA_DIR) -I$(WUFE_FPGA_BS_DIR)

ifeq "$(WUFE)" "ufe3"
WUFE_INCLUDE_DIRS += -I$(WUFE_DIR)/sources/include/firmware3
else
ifeq "$(WUFE)" "ufe412"
WUFE_INCLUDE_DIRS += -I$(WUFE_DIR)/sources/include/firmware412
UFE_FRAMER_FIRMWARE_DIR=$(WUFE_DIR)/wpx_frmr/firmware
else
ifeq "$(WUFE)" "ufe448"
WUFE_INCLUDE_DIRS += -I$(WUFE_DIR)/sources/include/firmware448
UFE_FRAMER_FIRMWARE_DIR=$(WUFE_DIR)/wpx_frmr/firmware
else
ifeq "$(WUFE)" "ufe412dual"
WUFE_INCLUDE_DIRS += -I$(WUFE_DIR)/sources/include/firmware412
UFE_FRAMER_FIRMWARE_DIR=$(WUFE_DIR)/wpx_frmr/firmware
endif
endif
endif
endif

override INCLUDES += 	$(WUFE_INCLUDE_DIRS) \
			-I$(WUFE_WDDI_DIR)/target/$(WUFE_TARGET)/include \
			-I$(WUFE_WDDS_DIR)/platform/$(WUFE_TARGET)/include \
			-I$(WUFE_WDDS_DIR)/platform/include \
			-I$(WUFE_WDDI_DIR)/board/$(WPI_BOARD_HW_DEVICE) \
			-I$(WUFE_WDDI_DIR)/sources \
			-I$(WUFE_WDDS_DIR)/include \
                        $(OS_INC_DIRS)

MAKE_CLEAN	= "no"
WUFE_SRC_DIRS	= $(WUFE_DIR)/sources

vpath %.h $(WUFE_INCLUDE_DIRS)
vpath %.c $(WUFE_SRC_DIRS)

# create ufe2/3/4 directory under target/board with obj and lib sub directories
# in case they are missing
export WUFE_OBJ_DIR = \
${WUFE_WDDS_DIR}/objects/$(WUFE_HOST)/$(WUFE_TARGET)/$(WPI_BOARD)/$(WUFE)/obj
export WUFE_LIB_DIR = \
${WUFE_WDDS_DIR}/objects/$(WUFE_HOST)/$(WUFE_TARGET)/$(WPI_BOARD)/$(WUFE)/lib

WUFE_SRC	= $(foreach dir, $(WUFE_SRC_DIRS), $(wildcard $(dir)/*.c))

vpath $(WUFE_BOARD_DATA_FILE).c ${WUFE_WDDI_DIR}/board/$(WPI_BOARD_HW_DEVICE)
WUFE_BOARD_DATA_SRC = ${WUFE_WDDI_DIR}/board/$(WPI_BOARD_HW_DEVICE)/$(WUFE_BOARD_DATA_FILE).c
WUFE_SRC += $(WUFE_BOARD_DATA_SRC)

# UFE FPGA specific sources
vpath %.c $(WUFE_FPGA_DIR)
WUFE_FPGA_SRC = $(foreach dir, $(WUFE_FPGA_DIR), $(wildcard $(dir)/*.c))
WUFE_SRC += $(WUFE_FPGA_SRC)

# UFE FPGA BS sources
vpath %.c $(WUFE_FPGA_BS_DIR)
WUFE_FPGA_BS_SRC = $(foreach dir, $(WUFE_FPGA_BS_DIR), $(wildcard $(dir)/*.c))
WUFE_SRC += $(WUFE_FPGA_BS_SRC)

WUFE_SOURCES    = $(patsubst %.c, %.c, $(notdir $(WUFE_SRC)))
WUFE_OBJS 	= $(patsubst %.c, $(WUFE_OBJ_DIR)/%.o, $(WUFE_SOURCES))
WUFE_DEP	= $(patsubst %.c, $(WUFE_OBJ_DIR)/%.d, $(WUFE_SOURCES))

# No implicit rules to be used during make
.SUFFIXES:

all:	wufelib ufe_framer_firmware_build ufe_framer_build pmc_build

objs:	$(WUFE_OBJS)

wufelib: $(WUFE_LIB_DIR)/wpx_ufe.a

$(WUFE_LIB_DIR)/wpx_ufe.a : $(WUFE_DEP) $(WUFE_OBJS)
	@echo "   AR" wpx_ufe.a
	@$(WPI_SET_PATH) $(AR) -r $@ $(WUFE_OBJS) 2>,ar
	@$(RM) ,ar

$(WUFE_OBJ_DIR)/%.d: %.c
	@echo "   DEP" $*.c
	@set -e; \
	$(WPI_SET_PATH) $(CC) -M $(WUFE_CFLAGS) $(INCLUDES) $< \
	| sed "s!\($*\)\.o[ :]*!$(WUFE_OBJ_DIR)/\1.o $@ : !g" > $@; \
	[ -s $@ ] || $(RM) $@

$(WUFE_OBJ_DIR)/%.o: %.c
	@echo "   CC" $*.c
	@$(WPI_SET_PATH) $(CC) $(WUFE_CFLAGS) $(INCLUDES) -o $@ $<

clean :
	@echo "   CLEAN " wpx_ufe.a for  $(WPI_TARGET):$(WPI_BOARD):$(UFE_VER)
	@$(RM) $(WUFE_DEP) $(WUFE_OBJS) $(WUFE_LIB_DIR)/wpx_ufe.a
	@if [ $(PMC) ] ; then \
		echo "   CLEAN" wpx_phy_pmc.a for $(WPI_TARGET):$(WPI_BOARD):$(UFE_VER); \
		cd $(PMC_DIR); $(MAKE) $(SILENTLY) -f makefile \
		"WPI_TARGET=$(WPI_TARGET)" "WPI_BOARD=$(WPI_BOARD)" \
		"WPI_WDDI_PL_O=$(WPI_WDDI_PL_O)" \
	        "WPI_HW_DEVICE=$(WPI_HW_DEVICE)" \
		"INCLUDE_DIRS=$(INCLUDE_DIRS)" \
	        "WUFE=$(WUFE)" "PMC=$(PMC)" "WUFE_CFLAGS=$(WUFE_CFLAGS)" \
		"MAKE_CLEAN=yes" clean; \
	fi
	@if [ $(UFE_FRAMER_FIRMWARE) ] ; then \
		echo "   CLEAN" wpx_phy_ufe_framer_firmware.a for $(WPI_TARGET):$(WPI_BOARD):$(UFE_VER); \
		cd $(UFE_FRAMER_FIRMWARE_DIR); $(MAKE)  $(SILENTLY) \
		"WPI_TARGET=$(WPI_TARGET)" "WPI_BOARD=$(WPI_BOARD)" \
		"WPI_WDDI_PL_O=$(WPI_WDDI_PL_O)" \
	    "WPI_HW_DEVICE=$(WPI_HW_DEVICE)" \
		"INCLUDE_DIRS=$(INCLUDE_DIRS)" \
	    "WUFE=$(WUFE)" \
		"MAKE_CLEAN=yes" clean; \
	fi
	@if [ $(FRAMER) ] ; then \
		echo "   CLEAN" wpx_phy_ufe_framer.a for $(WPI_TARGET):$(WPI_BOARD):$(UFE_VER); \
		cd $(UFE_FRAMER_DIR); $(MAKE) $(SILENTLY) -f makefile \
		"WPI_TARGET=$(WPI_TARGET)" "WPI_BOARD=$(WPI_BOARD)" \
		"WPI_WDDI_PL_O=$(WPI_WDDI_PL_O)" \
	        "WPI_HW_DEVICE=$(WPI_HW_DEVICE)" \
		"INCLUDE_DIRS=$(INCLUDE_DIRS)" "UFE_CFLAGS=$(UFE_CFLAGS)" \
	        "WUFE=$(WUFE)" "FRAMER=flexmux"  \
		"MAKE_CLEAN=yes" clean; \
	fi
ufe_framer_build:
	@if [ $(FRAMER) ] ; then \
		echo "   MAKE" wpx_phy_ufe_framer.a for $(WPI_TARGET):$(WPI_BOARD):$(UFE_VER); \
		cd $(UFE_FRAMER_DIR); $(MAKE)  $(SILENTLY) -f makefile \
		"WPI_TARGET=$(WPI_TARGET)" "WPI_BOARD=$(WPI_BOARD)" \
		"WPI_WDDI_PL_O=$(WPI_WDDI_PL_O)" \
	        "WPI_HW_DEVICE=$(WPI_HW_DEVICE)" \
		"INCLUDE_DIRS=$(INCLUDE_DIRS)" "UFE_CFLAGS=$(UFE_CFLAGS)" \
	        "WUFE=$(WUFE)" "FRAMER=flexmux"  \
		"MAKE_CLEAN=no" all; \
	fi
ufe_framer_clean:
	@if [ $(FRAMER) ] ; then \
		echo "   CLEAN" wpx_phy_ufe_framer.a for $(WPI_TARGET):$(WPI_BOARD):$(UFE_VER); \
		cd $(UFE_FRAMER_DIR); $(MAKE) $(SILENTLY) -f makefile \
		"WPI_TARGET=$(WPI_TARGET)" "WPI_BOARD=$(WPI_BOARD)" \
		"WPI_WDDI_PL_O=$(WPI_WDDI_PL_O)" \
	        "WPI_HW_DEVICE=$(WPI_HW_DEVICE)" \
		"INCLUDE_DIRS=$(INCLUDE_DIRS)" "UFE_CFLAGS=$(UFE_CFLAGS)" \
	        "WUFE=$(WUFE)" "FRAMER=flexmux" \
		"MAKE_CLEAN=yes" clean; \
	fi
ufe_framer_firmware_clean:
	@if [ $(UFE_FRAMER_FIRMWARE) ] ; then \
	echo "   CLEAN" wpx_phy_ufe_framer_firmware.a for $(WPI_TARGET):$(WPI_BOARD):$(UFE_VER); \
	cd $(UFE_FRAMER_FIRMWARE_DIR); $(MAKE)  $(SILENTLY) \
	"WPI_TARGET=$(WPI_TARGET)" "WPI_BOARD=$(WPI_BOARD)" \
	"WPI_WDDI_PL_O=$(WPI_WDDI_PL_O)" \
	"WPI_HW_DEVICE=$(WPI_HW_DEVICE)" \
	"INCLUDE_DIRS=$(INCLUDE_DIRS)" \
	"WUFE=$(WUFE)" \
	"MAKE_CLEAN=yes" clean; \
	fi
ufe_framer_firmware_build:
	@if [ $(UFE_FRAMER_FIRMWARE) ] ; then \
		echo "   MAKE" wpx_phy_ufe_framer_firmware.a for $(WPI_TARGET):$(WPI_BOARD):$(UFE_VER); \
		cd $(UFE_FRAMER_FIRMWARE_DIR); $(MAKE)  $(SILENTLY) \
		"WPI_TARGET=$(WPI_TARGET)" "WPI_BOARD=$(WPI_BOARD)" \
		"WPI_WDDI_PL_O=$(WPI_WDDI_PL_O)" \
	    "WPI_HW_DEVICE=$(WPI_HW_DEVICE)" \
		"INCLUDE_DIRS=$(INCLUDE_DIRS)" \
	    "WUFE=$(WUFE)" \
		"MAKE_CLEAN=no" all; \
	fi
pmc_build:
	@if [ $(PMC) ] ; then \
		echo "   MAKE" wpx_phy_pmc.a for $(WPI_TARGET):$(WPI_BOARD):$(UFE_VER); \
		cd $(PMC_DIR); $(MAKE)  $(SILENTLY) -f makefile \
		"WPI_TARGET=$(WPI_TARGET)" "WPI_BOARD=$(WPI_BOARD)" \
		"WPI_WDDI_PL_O=$(WPI_WDDI_PL_O)" \
	        "WPI_HW_DEVICE=$(WPI_HW_DEVICE)" \
		"INCLUDE_DIRS=$(INCLUDE_DIRS)" \
	        "WUFE=$(WUFE)" "PMC=$(PMC)" "WUFE_CFLAGS=$(WUFE_CFLAGS)" \
		"MAKE_CLEAN=no" all; \
	fi
pmc_clean:
	@if [ $(PMC) ] ; then \
		echo "   CLEAN" wpx_phy_pmc.a for $(WPI_TARGET):$(WPI_BOARD):$(UFE_VER); \
		cd $(PMC_DIR); $(MAKE) $(SILENTLY) -f makefile \
		"WPI_TARGET=$(WPI_TARGET)" "WPI_BOARD=$(WPI_BOARD)" \
		"WPI_WDDI_PL_O=$(WPI_WDDI_PL_O)" \
	        "WPI_HW_DEVICE=$(WPI_HW_DEVICE)" \
		"INCLUDE_DIRS=$(INCLUDE_DIRS)" \
	        "WUFE=$(WUFE)" "PMC=$(PMC)" "WUFE_CFLAGS=$(WUFE_CFLAGS)" \
		"MAKE_CLEAN=yes" clean; \
	fi
# Don't read the dependence includes unless necessary.
ifeq "$(MAKE_CLEAN)" "no"
-include $(WUFE_DEP)
endif
