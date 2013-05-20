# (C) Copyright 2000-2009, Wintegra. All rights reserved.
#  WINTEGRA CONFIDENTIAL PROPRIETARY
#  Contains Confidential Proprietary information of Wintegra.
#  Reverse engineering is prohibited.
#  The copyright notice does not imply publication.

OBJ_DIR = $(WPI_HT_OBJ_DIR)
LIB_DIR = $(WPI_HT_LIB_DIR)
SRC_DIR = sources
INC_DIR = include


PF_HOST_0 := $(OSTYPE:solaris%=solaris)
PF_HOST := $(PF_HOST_0:linux%=linux)

PF_SRC = $(wildcard $(WPI_WDDS_DIR)/platform/$(WPI_TARGET)/$(SRC_DIR)/*.c)
PF_SRC := $(notdir $(PF_SRC))
START_SRC = start.s

vpath %.h $(INC_DIR)
vpath %.c $(SRC_DIR)


include $(PF_HOST)_make.inc

PF_OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(PF_SRC))
PF_DEP = $(patsubst %.c, $(OBJ_DIR)/%.d, $(PF_SRC))
PF_A = $(LIB_DIR)/platform.a
START_OBJ = $(patsubst %.s, $(WPI_HT_LIB_DIR)/%.o, $(START_SRC))

INCLUDES = -I $(WPI_WDDS_DIR)/platform/$(WPI_TARGET)/$(INC_DIR) -I $(WPI_WDDS_DIR)/include -I $(WPI_WDDS_DIR)/platform/include

.PHONY: wpv_more_veneer platform clean

default:
	$(error This makefile should be invoked only by WDDS/makefile)

depend: $(PF_DEP)

clean:
	$(RM) $(PF_DEP) $(PF_OBJ) $(PF_A) $(START_OBJ)

$(PF_A): $(PF_DEP) $(PF_OBJ)
	@echo "   AR" $(PF_A)
	@$(WPI_SET_PATH) $(AR) -r $(PF_A) $(PF_OBJ) 2> ,ar

$(OBJ_DIR)/%.o: %.c
	@echo "   CC" $*.c
	$(WPI_SET_PATH) $(CC) $(CFLAGS) $(WDDI_CFLAGS) $(INCLUDES) -o $@ $<

$(OBJ_DIR)/%.d: %.c
	@echo "   DEP" $*.c
	set -e; \
	$(WPI_SET_PATH) $(CC) -M $(CFLAGS) $(WDDI_CFLAGS) $(INCLUDES) $< \
	| sed "s!\($*\)\.o[ :]*!$(WPI_HT_OBJ_DIR)/\1.o $@ : !g" > $@; \
	[ -s $@ ] || $(RM) $@


$(WPI_HT_LIB_DIR)/start.o: sources/start.s
	@echo "   CC" start.s
	$(WPI_SET_PATH) $(CC) -x c -E -P $(STDIO) sources/start.s > start.i \
          && $(WPI_SET_PATH) $(AS) $(ASFLAGS) -o $@ start.i
	$(RM) start.i

$(WPI_HT_OBJ_DIR)/%.o: sources/%.s
	@echo "   AS" $*.s
	$(WPI_SET_PATH) $(AS) $(ASFLAGS) -o $@ $<

platform: $(PF_A) $(START_OBJ)

ifeq "$(MAKE_CLEAN)" "no"
-include $(PF_DEP)
endif
