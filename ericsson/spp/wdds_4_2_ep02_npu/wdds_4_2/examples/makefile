# /***************************************************************************
# (C) Copyright Wintegra 2001-2002.  All rights reserved.
# WINTEGRA CONFIDENTIAL PROPRIETARY
# Contains Confidential Proprietary information of Wintegra.
# Reverse engineering is prohibited.
# The copyright notice does not imply publication.
# ***************************************************************************/


# README
# look for "user modify" in the following code for areas to customize for your environment

ifneq "$(example)" ""
  EXAMPLE=$(example)
endif

# Where to find the program sources
ifeq "$(EXAMPLE)" ""
 $(error EXAMPLE must be defined.)
endif
SRC_DIR = $(EXAMPLE)
# user modify
WDDS_DIR = ..
LINX = linx-2.4.1
LIBLINX = $(LINX)/lib/liblinx.a 
EXPAT = expat-2.0.1
# Where to find wddi.  If building for vxworks on cygwin, make sure that
# WDDI_DIR starts with "C:/" and NOT "/cygdrive/c/ otherwise the compiles fail
ifeq "$(WDDS_DIR)" ""
	WDDS_DIR = /wdds
endif

# If no target supplied,  mips_winmon is the default.
ifeq "$(WPI_TARGET)" ""
	WPI_TARGET = mips_winmon
endif

# If no board supplied,  wds2 is the default.
ifeq "$(WPI_BOARD)" ""
	WPI_BOARD = wds
endif

# If no device supplied, winpath2 is the default
# Use winpath2 for WinMax, Winarrow2 and WinPath2 derivatives
ifeq "$(WPI_HW_DEVICE)" ""
	WPI_HW_DEVICE = winpath3
endif

# C flags needed for examples
ifeq "$(WPI_HW_DEVICE)" "winpath2"
        WP_HW_WINPATH_DEV=WP_HW_WINPATH2
else
        WP_HW_WINPATH_DEV=WP_HW_WINPATH3
endif

EXAMPLE_CFLAGS = -D$(WP_HW_WINPATH_DEV) -Wno-error

#sometimes cygwin doesn't export OSTYPE for some reason
ifeq "$(OSTYPE)" ""
OSTYPE = cygwin
endif
ifeq "$(WUFE)" "ufe4"
UFEFLAG = -D__WT_UFE4__ -DWUFE_UFE4
endif
# What the host is.  Currently supported hosts are cygwin and solaris and linux
# WPI_HOST should be either "solaris", "cygwin", or "linux"
WPI_HOST_0 = $(OSTYPE:solaris%=solaris)
WPI_HOST = $(WPI_HOST_0:linux%=linux)

# Where to build the example objects and executable
OBJ_DIR = $(WDDS_DIR)/objects/$(WPI_HOST)/$(WPI_TARGET)_$(WPI_HW_DEVICE)/obj
BIN_DIR = $(WDDS_DIR)/objects/$(WPI_HOST)/$(WPI_TARGET)_$(WPI_HW_DEVICE)/bin

# Where the target-specific directory is
WPI_T_DIR = $(WDDS_DIR)/platform/$(WPI_TARGET)_$(WPI_HW_DEVICE)

# Where to find the make-defines and archives
WPI_HT_DIR = $(WDDS_DIR)/objects/$(WPI_HOST)/$(WPI_TARGET)_$(WPI_HW_DEVICE)

# Where to find the start file
WPI_START_FILE = $(WDDS_DIR)/objects/$(WPI_HOST)/$(WPI_TARGET)_$(WPI_HW_DEVICE)/lib

# Get the host-target specific defines
include $(WPI_T_DIR)/$(WPI_HOST)_make.inc

# Where to find wpi_wddi.a wpi_veneer.a
ARCHIVE_DIR = $(WPI_HT_DIR)/lib

# Where to find wpx_phy.a and wpx_board.a
PHY_BOARD = $(WPI_HT_DIR)/$(WPI_BOARD)/lib

# Where to find the board lib directory 
#WDDI_BOARD_LIB_DIR = $(WDDS_DIR)/objects/$(OSTYPE)/$(WPI_TARGET)/$(WPI_BOARD)/lib

WDDI_HW_LIB = $(WDDS_DIR)/objects/$(WPI_HOST)/$(WPI_TARGET)_$(WPI_HW_DEVICE)/lib/wpi_hw_device.a
UFE4_LIB = $(WDDS_DIR)/objects/$(WPI_HOST)/$(WPI_TARGET)_$(WPI_HW_DEVICE)/$(WPI_BOARD)/ufe412/lib/wpx_phy_ufe_framer.a   $(WDDS_DIR)/objects/$(WPI_HOST)/$(WPI_TARGET)_$(WPI_HW_DEVICE)/$(WPI_BOARD)/ufe412/lib/wpx_ufe.a

# The includes
API_DIR = $(WDDS_DIR)/wddi/sources/api
API_2DIR = $(WDDS_DIR)/wddi/sources
VENEER_TARGET_DIR = $(WDDS_DIR)/platform/$(WPI_TARGET)_$(WPI_HW_DEVICE)/include
VENEER_PLATFORM_DIR = $(WDDS_DIR)/platform/include
VENEER_DIR = $(WDDS_DIR)/wddi/sources/veneer
UTILITY_DIR = $(WDDS_DIR)/wddi/board/$(WPI_BOARD)_$(WPI_HW_DEVICE)
PHY_DIR = $(WDDS_DIR)/wddi/phy
BOARD_DIR = $(WDDS_DIR)/wddi/board
WINUTIL_TARGET_DIR = $(WDDS_DIR)/wddi/sources/winutil/api/include/$(WPI_HW_DEVICE)
WINUTIL_DIR = $(WDDS_DIR)/wddi/sources/winutil/api/include
HARDWARE_DIR += $(WDDS_DIR)/wddi/sources/hardware/$(WPI_HW_DEVICE)
ifeq "$(OSTYPE)" "linux"
COMMON = /shared/code_examples/common
else
COMMON = /cygdrive/e/code_examples/common
endif
INCLUDES =  -I$(API_DIR) -I. -I$(BOARD_DIR) -I$(VENEER_TARGET_DIR) -I$(VENEER_DIR) -I$(VENEER_PLATFORM_DIR) 
INCLUDES += -I$(WINUTIL_TARGET_DIR) -I$(WINUTIL_DIR) -I$(UTILITY_DIR) -I$(PHY_DIR) $(TARGET_INCLUDE) 
INCLUDES += -I$(WDDS_DIR)/include $(OS_INC_DIRS) -I$(WDDS_DIR)/wddi/sources/ -I$(COMMON) 
INCLUDES += -I$(WDDS_DIR)/wddi/target/$(WPI_TARGET)_$(WPI_HW_DEVICE)/include -I$(WDDS_DIR)/wddi/sources/winutil/include 
INCLUDES += -I$(WDDS_DIR)/wddi/sources/winutil/include/$(WPI_HW_DEVICE)
INCLUDES += -I$(WDDS_DIR)/wddi/phy/wpx_ufe/api -I$(WDDS_DIR)/wddi/phy/wpx_ufe/fpga/ufe4 -I$(WDDS_DIR)/wddi/phy/wpx_ufe/wpx_frmr/common/inc
INCLUDES += -I$(LINX)/include -I$(HARDWARE_DIR)
INCLUDES += -I$(EXPAT)/lib
include $(SRC_DIR)/make.list 

# If no source file names in make.list, try the .c extension on the executable name
ifeq "$(EXAMPLE_SRC)" ""
EXAMPLE_SRC = $(EXAMPLE:=.c)
endif

# If no DPS image in make.list, try the .c extension on the executable name
# user modify
ifeq "$(WDDI_DPS_IMAGE)" ""
WDDI_DPS_IMAGE = wpi_dps_image_backhaul.o
endif

# The objects and executable to be built
EXAMPLE_OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(EXAMPLE_SRC))
EXAMPLE_EXE = ./bin/$(EXAMPLE)$(EXE)
EXAMPLE_SREC = $(OBJ_DIR)/$(EXAMPLE).srec

# Where to find the DPS itmage
IMAGE_OBJ = $(WDDS_DIR)/objects/$(WPI_HOST)/$(WPI_TARGET)_$(WPI_HW_DEVICE)/lib/$(WDDI_DPS_IMAGE)

ifneq "$(WDDI_DPS_IMAGE2)" ""
IMAGE_OBJ2 = $(WDDS_DIR)/objects/$(WPI_HOST)/$(WPI_TARGET)_$(WPI_HW_DEVICE)/lib/$(WDDI_DPS_IMAGE2)
endif

# Where to find the WINUTIL image
WINUTIL_OBJ = $(WDDS_DIR)/objects/$(WPI_HOST)/$(WPI_TARGET)_$(WPI_HW_DEVICE)/lib/winutil.a
#an_example: clean $(EXAMPLE_EXE)
#	@echo Made $(EXAMPLE_EXE) for target $(WPI_TARGET)_$(WPI_HW_DEVICE) 
all: $(EXAMPLE_EXE)
clean:
	rm -rf $(EXAMPLE_OBJ) $(EXAMPLE_EXE) $(EXAMPLE_SREC)
an_example:
$(EXAMPLE_EXE): $(EXAMPLE_OBJ)
# If the linker needs a start file, copy it to the local directory
ifneq "$(START_FILE)" ""
	-cp $(WPI_START_FILE)/$(START_FILE) $(START_FILE)
endif
# Do the link
#	$(WPI_SET_PATH) $(CC) $(LDFLAGS) -o $(EXAMPLE_EXE) $(LINK_FILE) $(EXAMPLE_OBJ) $(IMAGE_OBJ) $(IMAGE_OBJ2) $(ARCHIVE_DIR)/wpi_wddi.a $(ARCHIVE_DIR)/wpi_veneer.a $(WDDI_HW_LIB) $(UFE4_LIB) $(PHY_BOARD)/wpx_phy.a $(PHY_BOARD)/wpx_board.a $(WINUTIL_OBJ) $(ARCHIVE_DIR)/platform.a $(LIBLINX) $(OTHER_LIB)
	$(WPI_SET_PATH) $(CC) $(LDFLAGS) -o $(EXAMPLE_EXE) $(LINK_FILE) $(EXAMPLE_OBJ) $(IMAGE_OBJ) $(IMAGE_OBJ2) $(ARCHIVE_DIR)/wpi_wddi.a $(WDDI_HW_LIB) $(UFE4_LIB) $(PHY_BOARD)/wpx_phy.a $(PHY_BOARD)/wpx_board.a $(WINUTIL_OBJ) $(ARCHIVE_DIR)/platform.a  $(LIBLINX) $(OTHER_LIB) -L /usr/local/lib -lexpat
# If the target needs S-records, make them.  tr removes cr's.
ifeq "$(MAKE_SREC)" ""
	$(WPI_SET_PATH) $(OBJCOPY) -O srec $(EXAMPLE_EXE) $(EXAMPLE_SREC).tmp
	tr -d '\015' < $(EXAMPLE_SREC).tmp > $(EXAMPLE_SREC)
########################################
#   USER MODIFY -- where to put the file
#   note that .exe files are loaded via the eld command on WinMon
########################################
# user modify
#	cp $(EXAMPLE_SREC) /srv/tftp/new_srec
endif

# user modify
#	cp $(EXAMPLE_EXE) /tftpboot/james/test.exe

# Clean up after everything that might have happened
	rm -rf $(EXAMPLE_OBJ) $(EXAMPLE_SREC).tmp $(START_FILE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(WPI_SET_PATH) $(CC) $(CFLAGS)  $(UFEFLAG) -DENEA_DEBUG $(EXAMPLE_CFLAGS) $(INCLUDES) -o $(OBJ_DIR)/$*.o $(SRC_DIR)/$*.c
