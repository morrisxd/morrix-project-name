#/*****************************************************************************
# * (C) Copyright 2000-2010, Wintegra. All rights reserved.
# * WINTEGRA CONFIDENTIAL PROPRIETARY
# * Contains Confidential Proprietary information of Wintegra.
# * Reverse engineering is prohibited.
# * The copyright notice does not imply publication.
# ****************************************************************************/

WDDS_DIR = 
WDDI_TARGET_NAME =
WDDI_BOARD_NAME =
WDDI_HOST =
WDDI_HW_DEVICE =
UFE_DIR = 
WDDI_DIR = $(WDDS_DIR)/wddi
MORRIS =

ifeq "$(WDDI_HW_DEVICE)" ""
$(error please supply WDDI_HW_DEVICE=[winpath2/winpath3])
endif

ifeq "$(WDDI_HW_DEVICE)" "winpath2"
WP_HW_WINPATHX = WP_HW_WINPATH2 
WDDI_DPS_IMAGE = msa_pwe3
endif

ifeq "$(WDDI_HW_DEVICE)" "winpath3"
WP_HW_WINPATHX = WP_HW_WINPATH3
WDDI_DPS_IMAGE = backhaul
endif

WDDI_TEST = pwe3_satop_cesop_demo

#exe name

#test source name
TEST_SRC = pwe3_satop_cesop_demo.c

WDDS_BUILD_DIR = $(WDDS_DIR)
WDDI_TEST_DIR = .
EXE_COPY =

WDDS_OBJECTS = objects
WDDI_SOURCES = /sources

# Where the target-specific directory is.  Used by make.inc.
WPI_T_DIR = $(WDDS_DIR)/platform/$(WDDI_TARGET_NAME)_$(WDDI_HW_DEVICE)
WPI_HT_DIR = ${WDDS_DIR}/$(WDDS_OBJECTS)/$(WDDI_HOST)/$(WDDI_TARGET_NAME)_$(WDDI_HW_DEVICE)
WPI_HTB_DIR = $(WDDS_DIR)/$(WDDS_OBJECTS)/$(WDDI_HOST)/$(WDDI_TARGET_NAME)_$(WDDI_HW_DEVICE)/$(WDDI_BOARD_NAME)


#WPI_HT_LIBS_X = $(patsubst %, $(WPI_HT_DIR)/lib/%, $(WPI_HT_LIBS))
#WPI_HTB_LIBS_X = $(patsubst %, $(WPI_HTB_DIR)/lib/%, $(WPI_HTB_LIBS))

WDDI_LIB_DIR = $(WDDS_DIR)/$(WDDS_OBJECTS)/$(WDDI_HOST)/$(WDDI_TARGET_NAME)_$(WDDI_HW_DEVICE)/lib
WDDI_BOARD_LIB_DIR = $(WDDS_DIR)/$(WDDS_OBJECTS)/$(WDDI_HOST)/$(WDDI_TARGET_NAME)_$(WDDI_HW_DEVICE)/$(WDDI_BOARD_NAME)/lib
WDDI_HW_DEVICE_LIB_DIR = $(WDDS_DIR)/$(WDDS_OBJECTS)/$(WDDI_HOST)/$(WDDI_TARGET_NAME)_$(WDDI_HW_DEVICE)/lib

DPS_IMAGE_OBJ = $(WDDI_HW_DEVICE_LIB_DIR)/wpi_dps_image_$(WDDI_DPS_IMAGE).o
WDDI_PPS_IMAGE = iwgp_parser
PPS_IMAGE_OBJ = $(WDDI_HW_DEVICE_LIB_DIR)/wpi_pps_image_$(WDDI_PPS_IMAGE).o

include $(WDDS_DIR)/platform/$(WDDI_TARGET_NAME)_$(WDDI_HW_DEVICE)/$(WDDI_HOST)_make.inc	

ifeq "$(WDDI_TARGET_NAME)" "mips_winmon"
TLOG_FUNCTION = WT_LOG_PRINTF
TLOG_LEVEL=1
DEFINE_WEB = -DWEB
MAKE_SREC = no
WDDI_LIBS = $(WDDI_LIB_DIR)/wpi_wddi.a  \
    $(WDDI_HW_DEVICE_LIB_DIR)/wpi_hw_device.a  $(WDDI_BOARD_LIB_DIR)/winutil.a \
	$(WDDI_HW_DEVICE_LIB_DIR)/platform.a \
	$(WDDI_HW_DEVICE_LIB_DIR)/shared.a \

PHY_LIB = $(WDDI_BOARD_LIB_DIR)/wpx_phy.a
BOARD_LIB = $(WDDI_BOARD_LIB_DIR)/wpx_board.a
ifeq "$(LDFLAGS_USES_LD)" "yes"
TEST_LD = $(LD)
else
TEST_LD = $(CC)
endif
endif

ifeq "$(WDDI_TARGET_NAME)" "mips_linux"
TLOG_FUNCTION = WT_LOG_PRINTF
TLOG_LEVEL=1
DEFINE_WEB = -DWEB
TARGET_INCLUDE = $(OS_INC_DIRS)
MAKE_SREC = no
WDDI_LIBS += -lpthread
TEST_LD = $(CC)

WDDI_LIBS = $(WDDI_LIB_DIR)/wpi_wddi.a  \
	$(WDDI_HW_DEVICE_LIB_DIR)/wpi_hw_device.a $(WDDI_HW_DEVICE_LIB_DIR)/winutil.a \
	$(WDDI_HW_DEVICE_LIB_DIR)/platform.a \
	-lpthread
PHY_LIB = $(WDDI_BOARD_LIB_DIR)/wpx_phy.a        
BOARD_LIB = $(WDDI_BOARD_LIB_DIR)/wpx_board.a

endif

#WDDI_LIBS += $(PHY_LIB)
WDDI_LIBS += $(BOARD_LIB)

TEST_CFLAGS = -D$(TLOG_FUNCTION) -DWT_DEBUG_LEVEL=$(TLOG_LEVEL) $(DEFINE_WEB) \
              -DWDDI_TEST=$(WDDI_TEST) -D$(WP_HW_WINPATHX) -DMORRIS2=$(MORRIS)
ifeq "$(MORRIS)" "wds_sl"
TEST_CFLAGS += -DMORRIS_WDS_SL
endif


# Where to construct the objects and binaries
TESTOBJ_DIR = ${WDDS_BUILD_DIR}/$(WDDS_OBJECTS)/$(WDDI_HOST)/$(WDDI_TARGET_NAME)_$(WDDI_HW_DEVICE)/$(WDDI_BOARD_NAME)/testobj
BIN_DIR = ${WDDS_BUILD_DIR}/$(WDDS_OBJECTS)/$(WDDI_HOST)/$(WDDI_TARGET_NAME)_$(WDDI_HW_DEVICE)/$(WDDI_BOARD_NAME)/bin

# The objects
TEST_OBJ = $(TESTOBJ_DIR)/$(WDDI_TEST).o
ifeq "$(MAKE_SREC)" "yes"
TEST_EXE = $(BIN_DIR)/$(WDDI_TEST).srec
TEST_TMP1 = $(BIN_DIR)/$(WDDI_TEST)$(EXE)
TEST_TMP2 = $(BIN_DIR)/$(WDDI_TEST).tmp
else
TEST_EXE = $(BIN_DIR)/$(WDDI_TEST)$(EXE)
TEST_TMP1 = $(BIN_DIR)/$(WDDI_TEST)$(EXE)
TEST_TMP2 = $(BIN_DIR)/$(WDDI_TEST).tmp
endif

# Override the make.inc file to no pass -g or -Werror
CFLAGS_G = -g
CFLAGS_WERROR =

ifeq "$(WDDI_HW_DEVICE)" "winpath2"
TEST_CFLAGS += -DWP_HW_WINPATH2
endif
ifeq "$(WDDI_HW_DEVICE)" "winpath3"
TEST_CFLAGS += -DWP_HW_WINPATH3
endif

# Set the include directories
SOURCES_DIR = $(WDDI_DIR)/$(WDDI_SOURCES)

API_DIR = $(WDDI_DIR)$(WDDI_SOURCES)/api
SERVICE_INCLUDE = $(patsubst %, -I$(WDDI_DIR)/services/%/api, $(SERVICES))
TARGET_DIR = $(WDDI_DIR)/target/$(WDDI_TARGET_NAME)_$(WDDI_HW_DEVICE)/include
PLATFORM_DIR = $(WDDS_DIR)/platform/$(WDDI_TARGET_NAME)_$(WDDI_HW_DEVICE)/include
PHY_DIR = $(WDDI_DIR)/phy
#VENEER_DIR = $(WDDI_DIR)$(WDDI_SOURCES)/veneer
LEGACY_DIR = $(WDDI_TEST_DIR)/legacy
GENERIC_BOARD_DIR = $(WDDI_DIR)/board
BOARD_DIR = $(WDDI_DIR)/board/$(WDDI_BOARD_NAME)_$(WDDI_HW_DEVICE)
EXTENSIONS_DIR = $(WDDI_DIR)/extensions

#add services to include
SERVICE_INCLUDE = $(patsubst %, -I$(WDDI_DIR)/services/%/api, $(SERVICES))

INCLUDES = -I$(SOURCES_DIR) -I$(API_DIR) $(SERVICE_INCLUDE) -I. \
	-I$(TARGET_DIR) -I$(PLATFORM_DIR) -I$(WDDS_DIR)/include -I$(WDDS_DIR)/platform/include \
	-I$(PHY_DIR)  \
	-I$(TEST_COMMON_DIR) $(WDDI_INTERNAL_INCLUDE) $(TARGET_INCLUDE) \
	-I$(LEGACY_DIR) -I$(BOARD_DIR) -I$(GENERIC_BOARD_DIR)  -I ../wddi/board/wds_common\
        -I$(EXTENSIONS_DIR) \
	-I$(WDDI_DIR)$(WDDI_SOURCES)/winutil/include \
	-I$(WDDI_DIR)$(WDDI_SOURCES)/winutil/include/$(WDDI_HW_DEVICE) \
	-I$(WDDI_DIR)$(WDDI_SOURCES)/winutil/api/include \
	-I$(WDDI_DIR)$(WDDI_SOURCES)/winutil/api/include/$(WDDI_HW_DEVICE) \

# UFE settings
UFE_DIR = $(WDDI_DIR)/../ufe
UFE_FPGA_DIR = $(UFE_DIR)/fpga/$(UFE)

ifeq "$(UFE)" "ufe3"
UFE_PMC_DIR = $(UFE_DIR)/wpx_pmc/tmx336
TEST_CFLAGS += -D__WT_UFE3__ -DWUFE_UFE3
TEST_CFLAGS += -D__WT_UFE3__ -DWUFE_UFE3
else
ifeq "$(UFE)" "ufe412"
UFE_FRAMER_DIR = $(UFE_DIR)/wpx_frmr/flexmux
TEST_CFLAGS += -D__WT_UFE4__ -DWUFE_UFE4
TEST_CFLAGS += -D__WT_UFE412__ -DWUFE_UFE4
else
ifeq "$(UFE)" "ufe448"
UFE_FRAMER_DIR = $(UFE_DIR)/wpx_frmr/flexmux
TEST_CFLAGS += -D__WT_UFE4__ -DWUFE_UFE4
TEST_CFLAGS += -D__WT_UFE448__ -DWUFE_UFE4
else
# $(error WUFE must be set to ufe412, ufe448 or ufe3)
endif
endif
endif

UFE_INC = -I$(UFE_DIR)/api -I$(UFE_FPGA_DIR)
UFE_PMC_INC =  -I$(UFE_PMC_DIR) -I$(UFE_PMC_DIR)/inc
UFE_FRAMER_INC = -I$(UFE_FRAMER_DIR) -I$(UFE_FRAMER_DIR)/library/00_api_layer/inc -I$(UFE_FRAMER_DIR)/library/04_low_level_comms_layer/inc


# Add the UFE libraries
# ---------------------
UFE_LIB_DIR = $(WDDI_BOARD_LIB_DIR)/../$(UFE)/lib
UFE_LIB = $(UFE_LIB_DIR)/wpx_ufe.a
UFE_LIB_EXIST := $(shell if [ -f $(UFE_LIB) ]; then echo 1; else echo 0; fi)
ifeq "$(UFE_LIB_EXIST)" "1"
INCLUDES += $(UFE_INC)
else
UFE_LIB =
endif

UFE_PMC_LIB = $(UFE_LIB_DIR)/wpx_phy_pmc.a
UFE_PMC_LIB_EXIST := $(shell if [ -f $(UFE_PMC_LIB) ]; then echo 1; else echo 0; fi)
ifeq "$(UFE_PMC_LIB_EXIST)" "1"
INCLUDES += $(UFE_PMC_INC)
TEST_CFLAGS += -D__WT_PMC__
UFE_LIB += $(UFE_PMC_LIB)
else
UFE_PMC_LIB = 
endif
UFE_FRAMER_LIB = $(UFE_LIB_DIR)/wpx_phy_ufe_framer.a
UFE_FRAMER_LIB_EXIST := $(shell if [ -f $(UFE_FRAMER_LIB) ]; then echo 1; else echo 0; fi)
ifeq "$(UFE_FRAMER_LIB_EXIST)" "1"
INCLUDES += $(UFE_FRAMER_INC)
TEST_CFLAGS += -DWT_UFE_FRAMER
UFE_LIB += $(UFE_FRAMER_LIB)
else
UFE_FRAMER_LIB = 
endif

WDDI_LIBS += $(UFE_LIB)
#WDDI_LIBS += $(PHY_LIB)
WDDI_LIBS += $(BOARD_LIB)

$(TEST_EXE): $(TEST_OBJ)

	@echo print DPS_IMAGE_OBJ  $(DPS_IMAGE_OBJ) UFE_LIB $(UFE_LIB)

	@echo Making $(TEST_EXE) 
# If the linker needs a start file, copy it to the local directory
ifneq "$(START_FILE)" ""
	-cp $(WDDI_LIB_DIR)/$(START_FILE) $(START_FILE)
endif
# Do the link (service lib should come before wddi)
	$(WPI_SET_PATH) $(TEST_LD) $(LDFLAGS) -o $(TEST_TMP1) $(LINK_FILE) $(TEST_OBJ) $(DPS_IMAGE_OBJ) $(PPS_IMAGE_OBJ) \
        $(WDDI_LIBS) $(WDDI_EXTRA_LIB)  $(OTHER_LIB)

#	$(WPI_SET_PATH) $(TEST_LD) $(LDFLAGS) -o $(TEST_TMP1) $(LINK_FILE) $(TEST_OBJ) \
#        $(DPS_IMAGE_OBJ) $(PPS_IMAGE_OBJ) $(GPE_PECS_LIB) \
#        $(WPI_HTB_LIBS_X) $(WPI_HT_LIBS_X) $(WDDI_LIBS) $(GATHER_LIB) 

# Clean up after everything that might have happened
	$(RM) $(TEST_TMP2) $(START_FILE)

$(TEST_OBJ): $(TEST_SRC)
	$(WPI_SET_PATH) $(CC) $(CFLAGS) $(TEST_CFLAGS) $(INCLUDES) -o $(TEST_OBJ) $(TEST_SRC)
