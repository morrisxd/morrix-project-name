############################################################ Change this block only!  #############################################################

WPI_WDDS_DIR = /users/haimfa/projects/winpath/wdds/WDDS_4_2_GA_RC4_build_1468_final
WPI_TARGET	= mips_winmon
WPI_BOARD  	= wds
HW_DEVICE   = winpath3
DPS_IMAGE_NAME	= wpi_dps_image_backhaul

######################################################## Do not change below this line!!! ########################################################

TEST_DIR = .
WDDI_DIR = $(WPI_WDDS_DIR)/wddi
WPI_HOST   	= linux

ifeq "$(WPI_TARGET)" "mips_linux"
MY_LDFLAGS = -lpthread -muclibc
CFLAGS += -muclibc
endif

ifeq "$(WPI_TARGET)" "mips_winmon"
WPI_TARGET 	= mips_winmon_$(HW_DEVICE)
endif

ifeq "$(WPI_TARGET)" "mips_linux"
WPI_TARGET 	= mips_linux_$(HW_DEVICE)
endif

ifeq "$(TEST)" ""
$(error usage "make TEST=testname")
endif

# Where to build the example objects and executable
OBJ_DIR = $(TEST_DIR)/bin
BIN_DIR = $(TEST_DIR)/bin

# Where the target-specific directory is
WPI_PLATFORM_DIR = $(WPI_WDDS_DIR)/platform/$(WPI_TARGET)
WPI_T_DIR=$(WPI_WDDS_DIR)/platform/$(WPI_TARGET)

# Where to find the make-defines and archives
WPI_HT_DIR = $(WPI_WDDS_DIR)/objects/$(WPI_HOST)/$(WPI_TARGET)

# Where to find wpx_phy.a and wpx_board.a
PHY_BOARD = $(WPI_HT_DIR)/$(WPI_BOARD)/lib

# Get the host-target specific defines
include $(WPI_PLATFORM_DIR)/$(WPI_HOST)_make.inc


# DPS Image to use
DPS_IMAGE_OBJ = $(WPI_WDDS_DIR)/objects/$(WPI_HOST)/$(WPI_TARGET)/lib/$(DPS_IMAGE_NAME).o


ifeq "$(HW_DEVICE)" "winpath2"
        WP_HW_WINPATH_DEV=WP_HW_WINPATH2
endif
ifeq "$(HW_DEVICE)" "winpath3"
        WP_HW_WINPATH_DEV=WP_HW_WINPATH3
endif

CFLAGS += -D$(WP_HW_WINPATH_DEV)

# The includes
API_DIR = $(WDDI_DIR)/sources/api
BOARD_DIR = $(WDDI_DIR)/board/$(WPI_BOARD)_$(HW_DEVICE)
INCLUDES = -I. -I$(API_DIR) -I$(BOARD_DIR) \
	   -I$(BOARD_DIR)/../ -I$(WDDI_DIR)/phy -I$(WDDI_DIR)/sources/ -I$(WDDI_DIR)/sources/include/ \
	   -I$(WPI_PLATFORM_DIR)/include -I$(WPI_WDDS_DIR)/include -I$(WPI_WDDS_DIR)/platform/include \
	   -I$(WDDI_DIR)/tests/common/


# The objects and executable to be built
TEST_NAME = $(basename $(TEST))
TEST_OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(TEST_NAME).c)
TEST_EXE = $(BIN_DIR)/$(TEST_NAME)$(EXE)

all:    clean $(TEST_EXE)
	@echo 
	@echo Made $(TEST_EXE) for target $(WPI_TARGET) 


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
	@echo 
	@echo Linking the test!
	$(CC) $(LDFLAGS) $(MY_LDFLAGS) -o $(TEST_EXE) $(LINK_FILE) $(TEST_OBJ) $(DPS_IMAGE_OBJ) \
	$(WPI_HT_DIR)/lib/ws_common.a $(WPI_HT_DIR)/lib/wpi_wddi.a \
	$(WPI_HT_DIR)/lib/winutil.a $(PHY_BOARD)/wpx_phy.a $(PHY_BOARD)/wpx_board.a $(WPI_HT_DIR)/lib/platform.a $(WPI_HT_DIR)/lib/wpi_hw_device.a $(OTHER_LIB)
