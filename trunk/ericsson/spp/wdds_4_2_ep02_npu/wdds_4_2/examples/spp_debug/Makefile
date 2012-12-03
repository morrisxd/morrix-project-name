all: spp_linx

#LINX ?= $(PWD)/../..
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


spp_linx: $(OBJDIR)/_BASIC_$(ARCH)
	$(ECHO) "Building spp_wddi"
#	$(GCC) $(CFLAGS) -o $(BINDIR)/spp_wddi_server -g spp_wddi_server.c $(LIBLINX)
	$(GCC) $(CFLAGS) -o $(BINDIR)/spp_wddi_debug_test  -g spp_wddi_debug_test.c $(LIBLINX)
	@cp ./menu.txt $(BINDIR)/

clean:
	$(RM) $(OBJDIR)/_BASIC_$(ARCH) \
	$(BINDIR)/spp_wddi_server \
	$(BINDIR)/spp_wddi_client

$(OBJDIR)/_BASIC_$(ARCH):
	$(MKDIR) $(OBJDIR)
	$(MKDIR) $(BINDIR)
	$(RM) $(OBJDIR)/_BASIC_*
	$(XMAKE) clean
	$(TOUCH) $@

.PHONY: spp_linx clean
