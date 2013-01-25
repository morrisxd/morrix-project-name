############################################################ Change this block only!  #############################################################

WPI_WDDS_DIR = ../../..
WPI_APP_TARGET     = mips_linux

ifeq "$(WPI_TARGET)" "mips_winmon"
WPI_APP_TARGET     = mips_winmon
endif

ifeq "$(WPI_TARGET)" "mips_linux"
WPI_APP_TARGET     = mips_linux
endif

WPI_APP_TARGET 	?= mips_winmon

ifeq "$(WPI_HW_DEVICE)" "winpath3"
WPI_APP_HW_DEVICE       = winpath3
endif

ifeq "$(WPI_HW_DEVICE)" "winpath2"
WPI_APP_HW_DEVICE       = winpath2
endif

WPI_APP_HW_DEVICE       ?= winpath3

ifeq "$(WPI_BOARD)" "wds"
WPI_APP_BOARD      = wds
endif

ifeq "$(WPI_BOARD)" "wds_sl"
WPI_APP_BOARD      = wds_sl
endif

WPI_APP_BOARD  	?= wds
DPS_IMAGE_NAME	= wpi_dps_image_backhaul

######################################################## Do not change below this line!!! ########################################################

TEST_DIR = .
WDDI_DIR = $(WPI_WDDS_DIR)/wddi
WPI_HOST   	= linux
WDDS_OBJECTS = objects

ifeq "$(WPI_APP_TARGET)" "mips_linux"
MY_LDFLAGS = -lpthread -muclibc
CFLAGS += -muclibc
endif

ifeq "$(WPI_APP_TARGET)" "mips_winmon"
WPI_APP_TARGET 	= mips_winmon_$(WPI_APP_HW_DEVICE)
endif

ifeq "$(WPI_APP_TARGET)" "mips_linux"
WPI_APP_TARGET 	= mips_linux_$(WPI_APP_HW_DEVICE)
endif

ifeq "$(TEST)" ""
$(error usage "make TEST=testname")
endif

# Where to find the start file
WPI_START_FILE = $(WPI_WDDS_DIR)/$(WDDS_OBJECTS)/$(WPI_HOST)/$(WPI_APP_TARGET)/lib

# Where to build the example objects and executable
OBJ_DIR = $(TEST_DIR)/bin
BIN_DIR = $(TEST_DIR)/bin

# Where the target-specific directory is
WPI_PLATFORM_DIR = $(WPI_WDDS_DIR)/platform/$(WPI_APP_TARGET)
WPI_T_DIR=$(WPI_WDDS_DIR)/platform/$(WPI_APP_TARGET)

# Where to find the make-defines and archives
WPI_HT_DIR = $(WPI_WDDS_DIR)/objects/$(WPI_HOST)/$(WPI_APP_TARGET)

# Where to find wpx_phy.a and wpx_board.a
PHY_BOARD = $(WPI_HT_DIR)/$(WPI_APP_BOARD)/lib

# Get the host-target specific defines
include $(WPI_PLATFORM_DIR)/$(WPI_HOST)_make.inc


# DPS Image to use
DPS_IMAGE_OBJ = $(WPI_WDDS_DIR)/objects/$(WPI_HOST)/$(WPI_APP_TARGET)/lib/$(DPS_IMAGE_NAME).o

# PPS Image to use
PPS_IMAGE_OBJ = $(WPI_WDDS_DIR)/objects/$(WPI_HOST)/$(WPI_APP_TARGET)/lib/wpi_pps_image_iwgp_parser.o


ifeq "$(WPI_APP_HW_DEVICE)" "winpath2"
        WP_HW_WINPATH_DEV=WP_HW_WINPATH2
endif
ifeq "$(WPI_APP_HW_DEVICE)" "winpath3"
        WP_HW_WINPATH_DEV=WP_HW_WINPATH3
endif

#make compiler happy
CFLAGS += -Wno-declaration-after-statement -Wno-shadow -Wno-type-limits -Wno-sign-compare -Wno-cast-qual -Wno-empty-body -Wno-missing-field-initializers -Wno-unused-parameter -Wno-extra

CFLAGS += -D$(WP_HW_WINPATH_DEV)

# REBOOT settings
ifeq "$(WPI_REBOOT)" "Reboot"
CFLAGS      += -DWPI_APP_REBOOT
endif

# The includes
API_DIR = $(WDDI_DIR)/sources/api
BOARD_DIR = $(WDDI_DIR)/board/$(WPI_APP_BOARD)_$(WPI_APP_HW_DEVICE)
INCLUDES = -I. -I$(API_DIR) -I$(BOARD_DIR) \
	   -I$(BOARD_DIR)/../ -I$(WDDI_DIR)/board/wds_common -I$(WDDI_DIR)/sources/ -I$(WDDI_DIR)/sources/include/ \
	   -I$(WPI_PLATFORM_DIR)/include -I$(WPI_WDDS_DIR)/include -I$(WPI_WDDS_DIR)/platform/include \
	   -I$(WDDI_DIR)/tests/common/ -I$(WPI_WDDS_DIR)/gpe/util/sources/api -I$(WDDI_DIR)/sources/include/core/pce \
	   -I$(WDDI_DIR)/sources/winutil/api/include/$(WPI_APP_HW_DEVICE)/ -I$(WDDI_DIR)/sources/winutil/include/$(WPI_APP_HW_DEVICE)/ \
	   -I$(WPI_WDDS_DIR)/shared/sources/include/
# The objects and executable to be built
TEST_NAME = $(basename $(TEST))
TEST_OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(TEST_NAME).c)
TEST_EXE = $(BIN_DIR)/$(TEST_NAME)$(EXE)

all:    clean $(TEST_EXE)
	@echo 
	@echo Made $(TEST_EXE) for target $(WPI_APP_TARGET) 


clean:
	@echo
	@echo Cleaning!
	mkdir -p $(BIN_DIR)
	rm -rf $(TEST_OBJ) $(TEST_EXE)

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@echo 
	@echo $(CC)
	@echo Compiling the test!
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OBJ_DIR)/$*.o $(TEST_DIR)/$*.c

$(TEST_EXE): $(TEST_OBJ)
# If the linker needs a start file, copy it to the local directory
	@echo 
	@echo Linking the test!
ifneq "$(START_FILE)" ""
	-cp $(WPI_START_FILE)/$(START_FILE) $(START_FILE)
endif
	$(CC) $(LDFLAGS) $(MY_LDFLAGS) -o $(TEST_EXE) $(LINK_FILE) $(TEST_OBJ) $(DPS_IMAGE_OBJ) $(PPS_IMAGE_OBJ) \
	 $(WPI_HT_DIR)/lib/wpi_wddi.a \
	$(WPI_HT_DIR)/$(WPI_APP_BOARD)/lib/winutil.a $(PHY_BOARD)/wpx_board.a $(WPI_HT_DIR)/lib/platform.a $(WPI_HT_DIR)/lib/wpi_hw_device.a $(OTHER_LIB) \
	$(WPI_HT_DIR)/lib/wpi_pecs.a $(WPI_HT_DIR)/lib/shared.a

# Clean up after everything that might have happened
	rm -rf $(START_FILE)
