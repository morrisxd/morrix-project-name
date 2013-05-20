all: linx_basic

LINX ?= $(PWD)/../..
include $(LINX)/config.mk

ifneq ($(ASSERT),yes)
CFLAGS += -DNDEBUG
endif

CFLAGS += -I$(LINX)/include
CFLAGS += -Wall -pedantic -std=gnu99

OBJDIR = $(LINX)/example/obj
BINDIR = $(LINX)/example/bin

ifeq ($(64BIT_SUPPORT),yes)
CFLAGS	+= -m64
LIBLINX = $(LINX)/lib64/liblinx.a
else
LIBLINX = $(LINX)/lib/liblinx.a
endif


linx_basic: $(OBJDIR)/_BASIC_$(ARCH)
	$(ECHO) "Building linx_basic"
	$(GCC) $(CFLAGS) -o $(BINDIR)/linx_basic_client linx_basic_client.c $(LIBLINX)
	$(GCC) $(CFLAGS) -o $(BINDIR)/linx_basic_server linx_basic_server.c $(LIBLINX)


clean:
	$(RM) $(OBJDIR)/_BASIC_$(ARCH) \
	$(BINDIR)/linx_basic_client \
	$(BINDIR)/linx_basic_server

$(OBJDIR)/_BASIC_$(ARCH):
	$(MKDIR) $(OBJDIR)
	$(MKDIR) $(BINDIR)
	$(RM) $(OBJDIR)/_BASIC_*
	$(XMAKE) clean
	$(TOUCH) $@

.PHONY: linx_basic clean
