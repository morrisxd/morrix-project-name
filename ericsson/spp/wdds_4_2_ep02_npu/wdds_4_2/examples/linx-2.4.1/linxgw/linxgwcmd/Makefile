all: echo_config linxgwcmd

LINX ?= $(PWD)/../..
include $(LINX)/config.mk

CFLAGS += -I$(LINX)/include
CFLAGS += -I$(LINX)/linxgw/include
CFLAGS += -Wall -pedantic -D_GNU_SOURCE
CFLAGS += -DNDEBUG

linxgwcmd: $(OBJDIR)/_LINXGWCMD_$(ARCH)
	$(ECHO) "Building $@"
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) $(CFLAGS) linxgwcmd.c $(LINX)/lib64/liblinx.a $(LINX)/lib64/libgw.a -o $(BINDIR)/linxgwcmd
else
	$(GCC32) $(CFLAGS) linxgwcmd.c $(LINX)/lib/liblinx.a $(LINX)/lib/libgw.a -o $(BINDIR)/linxgwcmd
endif

clean:
	$(RM) $(OBJS)
	$(RM) $(BINDIR)/linxgwcmd
ifeq ($(64BIT_SUPPORT),)
	$(RM) $(OBJDIR64)/_LINXGWCMD_$(ARCH)
else
	$(RM) $(OBJDIR)/_LINXGWCMD_$(ARCH)
endif

$(OBJDIR)/_LINXGWCMD_$(ARCH):
ifeq ($(64BIT_SUPPORT),)
	$(RM) $(OBJDIR64)/_LINXGWCMD_*
else
	$(RM) $(OBJDIR)/_LINXGWCMD_*
endif
	$(XMAKE) clean
	$(TOUCH) $@

install: linxgwcmd
	$(ECHO) "Copying linxgwcmd to $(INSTALLBINDIR)"
	$(CP) $(BINDIR)/linxgwcmd $(INSTALLBINDIR)/

uninstall:
	$(ECHO) "Removing linxgwcmd from $(INSTALLBINDIR)"
	$(RM) $(INSTALLBINDIR)/linxgwcmd

.PHONY: linxgwcmd clean
