all: npumon

LINX ?= $(PWD)/../linx-2.4.1
include $(LINX)/config.mk

ifneq ($(ASSERT),yes)
CFLAGS += -DNDEBUG
endif

ifeq "$(ENEA)" "yes"
CFLAGS += -DENEA_DEBUG
endif

CFLAGS += -I$(LINX)/include
CFLAGS += -Wall  -std=gnu99 #-pedantic

OBJDIR = $(PWD)/../obj
BINDIR = $(PWD)/../bin

ifeq ($(64BIT_SUPPORT),yes)
CFLAGS	+= -m64
LIBLINX = $(LINX)/lib64/liblinx.a
else
//CFLAGS	+= -m32
LIBLINX = $(LINX)/lib/liblinx.a
endif


npumon: $(OBJDIR)/_BASIC_$(ARCH)
	$(ECHO) "Building npumon"
	$(GCC) $(CFLAGS) -o $(BINDIR)/npumon  -g npumon.c $(LIBLINX)

clean:
	$(RM) $(OBJDIR)/_BASIC_$(ARCH) \
	$(BINDIR)/npumon

$(OBJDIR)/_BASIC_$(ARCH):
	$(MKDIR) $(OBJDIR)
	$(MKDIR) $(BINDIR)
	$(RM) $(OBJDIR)/_BASIC_*
	$(XMAKE) clean
	$(TOUCH) $@

.PHONY: npumon clean
