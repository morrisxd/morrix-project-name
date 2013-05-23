############################################################ Change this block only!  #############################################################

WPI_TARGET 	= mips_linux




# By default this makefile assumes that the test directory reside under the WDDI root directory!
# If you want to change it please redefine WDDI_DIR in this makefile!
TEST_DIR = .
WPI_WDDS_DIR = ../../..
WDDI_DIR = $(WPI_WDDS_DIR)/wddi
ifeq "$(WPI_TARGET)" ""
WPI_TARGET 	= mips_winmon
endif

WPI_BOARD  	= wds
WPI_HOST   	= linux
UFE_TYPE        = ufe412
DPS_IMAGE_NAME  = wpi_dps_image_backhaul

WDDS_OBJECTS = objects
WDDI_SOURCES = /sources

HW_DEVICE       = winpath3
RA_WDDI_VERS    = 4 # my own variable, because wddi 3 and 4 have slight compilation differences

# Where to find the start file
WPI_START_FILE = $(WPI_WDDS_DIR)/$(WDDS_OBJECTS)/$(WPI_HOST)/$(WPI_TARGET)/lib

######################################################## Do not change!!! ########################################################

ifeq "$(WPI_TARGET)" "mips_linux"
LDFLAGS2	= -lpthread
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

# Where to construct the objects and binaries
#TESTOBJ_DIR = $(WPI_WDDS_DIR)/$(WDDS_OBJECTS)/$(WDDI_HOST)/$(WDDI_TARGET_NAME)_$(WDDI_HW_DEVICE)/$(WDDI_BOARD_NAME)/testobj
#BIN_DIR = $(WPI_WDDS_DIR)/$(WDDS_OBJECTS)/$(WDDI_HOST)/$(WDDI_TARGET_NAME)_$(WDDI_HW_DEVICE)/$(WDDI_BOARD_NAME)/bin

# The objects
#TEST_OBJ = $(TESTOBJ_DIR)/$(TEST).o

# Where to build the example objects and executable
OBJ_DIR = $(TEST_DIR)/bin
BIN_DIR = $(TEST_DIR)/bin

# Where the target-specific directory is
WPI_T_DIR = $(WPI_WDDS_DIR)/platform/$(WPI_TARGET)
WPI_PLATFORM_DIR = $(WPI_WDDS_DIR)/platform/$(WPI_TARGET)

# Where to find the make-defines and archives
WPI_HT_DIR = $(WPI_WDDS_DIR)/$(WDDS_OBJECTS)/$(WPI_HOST)/$(WPI_TARGET)
WPI_HTB_DIR = $(WPI_HT_DIR)/$(WPI_BOARD)
WPI_UFE4_DIR = $(WPI_HTB_DIR)/$(UFE_TYPE)
WPI_UFE4_OBJ = $(WPI_UFE4_DIR)/obj

# The includes
API_DIR = $(WDDI_DIR)$(WDDI_SOURCES)/api
PHY_DIR = $(WDDI_DIR)/phy
WPX_UFE_DIR = $(PHY_DIR)/wpx_ufe
UFE_API_DIR = $(WPX_UFE_DIR)/api
UFE_FRMR_DIR = $(WPX_UFE_DIR)/wpx_frmr
UFE4_FPGA_DIR = $(WPX_UFE_DIR)/fpga/ufe4
UFE_FRMR_FLEXMUX_DIR = $(UFE_FRMR_DIR)/device_driver
UFE_INC = -I$(UFE_API_DIR) -I$(UFE4_FPGA_DIR)
UFE_FRAMER_INC = -I$(UFE_FRMR_DIR)/flexmux -I$(UFE_FRMR_DIR) -I$(UFE_FRMR_FLEXMUX_DIR) -I$(UFE_FRMR_FLEXMUX_DIR)/common/inc -I$(UFE_FRMR_FLEXMUX_DIR)/library/00_api_layer/inc -I$(UFE_FRMR_FLEXMUX_DIR)/library/04_low_level_comms_layer/inc
#VENEER_TARGET_DIR = $(WDDI_DIR)/target/$(WPI_TARGET)/include
VENEER_DIR = $(WDDI_DIR)/sources/veneer
BOARD_DIR = $(WDDI_DIR)/board/$(WPI_BOARD)_$(HW_DEVICE)
WINUTIL_DIR = $(WDDI_DIR)/sources/winutil/include/
INCLUDES =  -I. -I$(API_DIR) -I$(VENEER_DIR) -I$(BOARD_DIR) -I$(WINUTIL_DIR) \
           -I$(BOARD_DIR)/.. -I$(WDDI_DIR)/phy -I$(WDDI_DIR)/sources -I$(WDDI_DIR)/sources/include \
	   -I$(WPI_PLATFORM_DIR)/include -I$(WPI_WDDS_DIR)/include -I$(WPI_WDDS_DIR)/platform/include \
	   -I$(WDDI_DIR)/tests/common -I$(API_DIR) -I$(PHY_DIR) 

TEST_CFLAGS = -D$(TLOG_FUNCTION) -DWT_DEBUG_LEVEL=$(TLOG_LEVEL) $(DEFINE_WEB) \
              -DWDDI_TEST=$(TEST) -D$(WP_HW_WINPATHX) -D$(WP_BOARD_NAME) \
	      -DWP_HW_WINPATH3  


# UFE settings
UFE_DIR = $(WDDI_DIR)/phy/wpx_ufe

ifeq "$(UFE_TYPE)" "ufe412"
UFE_FRAMER_DIR = $(UFE_DIR)/wpx_frmr/flexmux
TEST_CFLAGS += -D__WT_UFE4__ -DWUFE_UFE4
TEST_CFLAGS += -D__WT_UFE412__ -DWUFE_UFE4
endif



# Add the UFE libraries
# ---------------------
UFE_LIB_DIR = $(PHY_BOARD_LIB_DIR)/../$(UFE_TYPE)/lib
UFE_LIB = $(UFE_LIB_DIR)/wpx_ufe.a
#UFE_LIB_EXIST := $(shell if [ -f $(UFE_LIB) ]; then echo 1; else echo 0; fi)
#ifeq "$(UFE_LIB_EXIST)" "1"
INCLUDES += $(UFE_INC)
#else
#UFE_LIB =
#endif

UFE_FRAMER_LIB = $(UFE_LIB_DIR)/wpx_phy_ufe_framer.a
#UFE_FRAMER_LIB_EXIST := $(shell if [ -f $(UFE_FRAMER_LIB) ]; then echo 1; else echo 0; fi)
#ifeq "$(UFE_FRAMER_LIB_EXIST)" "1"
INCLUDES += $(UFE_FRAMER_INC)
TEST_CFLAGS += -DWT_UFE_FRAMER
UFE_LIB += $(UFE_FRAMER_LIB)
#else
#UFE_FRAMER_LIB = 
#endif

# UFE4 events object file:
#-------------------------
UFE4_EVENTS_OBJ = $(WPI_UFE4_OBJ)/wufei_events.o

# Get the host-target specific defines
include $(WPI_T_DIR)/$(WPI_HOST)_make.inc

# Where to find wpi_wddi.a wpi_veneer.a
WDDI_LIB_DIR = $(WPI_HT_DIR)/lib

# Where to find wpx_phy.a and wpx_board.a
PHY_BOARD_LIB_DIR = $(WPI_HT_DIR)/$(WPI_BOARD)/lib

WDDI_LIBS = $(WDDI_LIB_DIR)/wpi_wddi.a  \
    $(WDDI_LIB_DIR)/wpi_hw_device.a  $(WDDI_LIB_DIR)/winutil.a \
	$(WDDI_LIB_DIR)/platform.a \

PHY_LIB = $(PHY_BOARD_LIB_DIR)/wpx_phy.a
BOARD_LIB = $(PHY_BOARD_LIB_DIR)/wpx_board.a

WDDI_LIBS += $(PHY_LIB)
WDDI_LIBS += $(UFE_LIB)
WDDI_LIBS += $(BOARD_LIB)

# DPS Image to use
DPS_IMAGE_OBJ = $(WPI_WDDS_DIR)/$(WDDS_OBJECTS)/$(WPI_HOST)/$(WPI_TARGET)/lib/$(DPS_IMAGE_NAME).o

#setting special flag for linux compilation
ifeq "$(WPI_TARGET)" "mips_linux"
SPECIAL_FLAG = -lpthread
endif

TLOG_FUNCTION = WT_LOG_PRINTF
TLOG_LEVEL=1
DEFINE_WEB = -DWEB
WP_HW_WINPATHX = WP_HW_WINPATH3
WP_BOARD_NAME=WP_BOARD_WDS3

# Overwriting WDDI CFLAG define only because I like to be in control
ifeq "$(WPI_TARGET)" "mips_winmon"
CFLAGS += -c -mips32 -G 0 -nostdinc -fno-builtin -fno-inline-functions -fsigned-char -Wpointer-arith -Wstrict-prototypes -Wformat -Wno-main -g -O2
endif
ifeq "$(WPI_TARGET)" "mips_linux"
CFLAGS += -c -mips2  -G 0 -g -O2 -msoft-float -mno-memcpy -pipe -Wstrict-prototypes -Wall -pg
endif

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


#$(TEST_EXE): $(TEST_OBJ)
#	@echo 
#	@echo Linking the test!
#	$(CC) $(LDFLAGS) -o $(TEST_EXE) $(LINK_FILE) $(TEST_OBJ) $(DPS_IMAGE_OBJ) $(WDDI_LIB_DIR)/wpi_wddi.a $(WDDI_LIB_DIR)/wpi_veneer.a $(PHY_BOARD)/wpx_phy.a $(PHY_BOARD)/wpx_board.a $(WPI_HT_DIR)/$(HW_DEVICE)/lib/wpi_hw_device.a $(WIN_UTILS) $(OTHER_LIB) $(SPECIAL_FLAG)

$(TEST_EXE): $(TEST_OBJ)
# If the linker needs a start file, copy it to the local directory
	@echo 
	@echo Linking the test!
ifneq "$(START_FILE)" ""
	-cp $(WPI_START_FILE)/$(START_FILE) $(START_FILE)
endif
# Do the link	
	$(CC) $(LDFLAGS) $(LDFLAGS2) -msoft-float -o $(TEST_EXE) $(LINK_FILE) $(TEST_OBJ) $(DPS_IMAGE_OBJ) $(UFE4_EVENTS_OBJ) \
	$(WDDI_LIBS) $(OTHER_LIB)

# Clean up after everything that might have happened
	rm -rf $(START_FILE)


$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@echo 
	@echo $(CC)
	@echo Compiling the test!
	$(CC) $(CFLAGS) $(TEST_CFLAGS) $(INCLUDES) -o $(OBJ_DIR)/$*.o $(TEST_DIR)/$*.c
