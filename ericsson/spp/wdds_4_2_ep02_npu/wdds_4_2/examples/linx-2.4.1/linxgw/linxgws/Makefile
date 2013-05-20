all: echo_config linxgws

LINX ?= $(PWD)/../..
include $(LINX)/config.mk

CFLAGS += -I$(LINX)/include
CFLAGS += -I$(LINX)/linxgw/include
CFLAGS += -Wall -D_GNU_SOURCE
CFLAGS += -DNDEBUG
CFLAGS += -std=gnu99

linxgws: $(OBJDIR)/_LINXGWS_$(ARCH)

	$(ECHO) "Building $@"
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) $(CFLAGS) -c linxgws.c linxgws_main.c linxgws_msghandler.c
	$(GCC64) $(CFLAGS) -o $(BINDIR)/linxgws linxgws.o linxgws_main.o \
	linxgws_msghandler.o $(LINX)/lib64/liblinx.a
else
	$(GCC32) $(CFLAGS) -c linxgws.c linxgws_main.c linxgws_msghandler.c
	$(GCC32) $(CFLAGS) -o $(BINDIR)/linxgws linxgws.o linxgws_main.o \
	linxgws_msghandler.o $(LINX)/lib/liblinx.a
endif

clean:
	$(RM) linxgws.o
	$(RM) $(BINDIR)/linxgws
ifeq ($(64BIT_SUPPORT),)
	$(RM) $(OBJDIR64)/_LINXGWS_$(ARCH)
else
	$(RM) $(OBJDIR)/_LINXGWS_$(ARCH)
endif

$(OBJDIR)/_LINXGWS_$(ARCH):
ifeq ($(64BIT_SUPPORT),)
	$(RM) $(OBJDIR64)/_LINXGWS_*
else
	$(RM) $(OBJDIR)/_LINXGWS_*
endif
	$(XMAKE) clean
	$(TOUCH) $@

install: linxgws
	$(ECHO) "Copying linxgws to $(INSTALLBINDIR)"
	$(CP) $(BINDIR)/linxgws $(INSTALLBINDIR)/

uninstall:
	$(ECHO) "Removing linxgws from $(INSTALLBINDIR)"
	$(RM) $(INSTALLBINDIR)/linxgws

.PHONY: linxgws clean
