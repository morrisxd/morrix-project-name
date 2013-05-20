all: linx_bmark

LINX ?= $(PWD)/../..
include $(LINX)/config.mk

ifneq ($(ASSERT),yes)
CFLAGS += -DNDEBUG
endif

CFLAGS += -I$(LINX)/include
CFLAGS += -W -Wall -O2

OBJDIR = $(LINX)/bmark/obj
BINDIR = $(LINX)/bmark/bin

ifeq ($(64BIT_SUPPORT),yes)
CFLAGS	+= -m64
LIBLINX = $(LINX)/lib64/liblinx.a
else
LIBLINX = $(LINX)/lib/liblinx.a
endif

linx_bmark: $(OBJDIR)/_BMARK_$(ARCH)
	$(ECHO) "Building linx_bmark"
	$(GCC) $(CFLAGS) \
	-o $(BINDIR)/linx_bmark \
	linx_bmark_attach.c \
        linx_bmark_latency.c \
        linx_bmark_burst.c \
        linx_bmark_throughput.c \
	linx_bmark.c $(LIBLINX)


clean:
	$(RM) $(OBJDIR)/_BMARK_$(ARCH) \
	$(BINDIR)/linx_bmark

$(OBJDIR)/_BMARK_$(ARCH):
	$(MKDIR) $(OBJDIR)
	$(MKDIR) $(BINDIR)
	$(RM) $(OBJDIR)/_BMARK_*
	$(XMAKE) clean
	$(TOUCH) $@

.PHONY: linx_bmark clean
