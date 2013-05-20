all: echo_config linxdisc

LINX ?= $(PWD)/..
include $(LINX)/config.mk

CFLAGS += -I$(LINX)/include
CFLAGS += -O2
CFLAGS += -Wall -pedantic -std=gnu99 -D_GNU_SOURCE
CFLAGS += -DNDEBUG

ifneq ($(LINK_MAX),)
CFLAGS += -DMAX_CONNECTIONS=$(LINK_MAX)
endif

OBJS  = config.o
OBJS += debug.o
OBJS += linxdisc.o
OBJS += util.o

linxdisc: $(OBJDIR)/_LINXDISC_$(ARCH) $(OBJS)
	$(ECHO) "Building $@"
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) $(CFLAGS) -o $(BINDIR)/linxdisc $(OBJS) $(LINX)/lib64/liblinx.a \
		$(LINX)/lib64/libcfg.a
else
	$(GCC32) $(CFLAGS) -o $(BINDIR)/linxdisc $(OBJS) $(LINX)/lib/liblinx.a \
		$(LINX)/lib/libcfg.a
endif

config.o: config.c
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) -c $(CFLAGS) -o $@ $<
else
	$(GCC32) -c $(CFLAGS) -o $@ $<
endif

debug.o: debug.c
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) -c $(CFLAGS) -o $@ $<
else
	$(GCC32) -c $(CFLAGS) -o $@ $<
endif

linxdisc.o: linxdisc.c
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) -c $(CFLAGS) -o $@ $<
else
	$(GCC32) -c $(CFLAGS) -o $@ $<
endif

util.o: util.c
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) -c $(CFLAGS) -o $@ $<
else
	$(GCC32) -c $(CFLAGS) -o $@ $<
endif

clean:
	$(RM) $(OBJS)
	$(RM) $(BINDIR)/linxdisc
ifneq ($(64BIT_SUPPORT),)
	$(RM) $(OBJDIR64)/_LINXDISC_$(ARCH)
else
	$(RM) $(OBJDIR)/_LINXDISC_$(ARCH)
endif

$(OBJDIR)/_LINXDISC_$(ARCH):
ifneq ($(64BIT_SUPPORT),)
	$(RM) $(OBJDIR64)/_LINXDISC_*
else
	$(RM) $(OBJDIR)/_LINXDISC_*
endif
	$(XMAKE) clean
	$(TOUCH) $@

install: linxdisc
	$(ECHO) "Copying linxdisc to $(INSTALLBINDIR)"
	$(CP) $(BINDIR)/linxdisc $(INSTALLBINDIR)/
	$(ECHO) "Copying linxdisc.conf.example to $(INSTALLCONFDIR)"
	$(CP) linxdisc.conf.example $(INSTALLCONFDIR)/

uninstall:
	$(ECHO) "Removing linxdisc from $(INSTALLBINDIR)"
	$(RM) $(INSTALLBINDIR)/linxdisc
	$(ECHO) "Removing linxdisc.conf.example from $(INSTALLCONFDIR)"
	$(RM) $(INSTALLCONFDIR)/linxdisc.conf.example

.PHONY: linxdisc clean
