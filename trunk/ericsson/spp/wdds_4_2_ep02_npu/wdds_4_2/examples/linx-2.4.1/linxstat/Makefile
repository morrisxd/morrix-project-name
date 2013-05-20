all: echo_config linxstat

LINX ?= $(PWD)/..
include $(LINX)/config.mk

ifneq ($(ASSERT),yes)
CFLAGS += -DNDEBUG
endif

CFLAGS += -I$(LINX)/include
CFLAGS += -Wall -std=gnu99  

linxstat: $(OBJDIR)/_LINXSTAT_$(ARCH)
	$(ECHO) "Building $@"
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) $(CFLAGS) -o $(BINDIR)/linxstat linxstat.c
else
	$(GCC32) $(CFLAGS) -o $(BINDIR)/linxstat linxstat.c
endif

clean:
	$(RM) $(BINDIR)/linxstat
ifneq ($(64BIT_SUPPORT),)
	$(RM) $(OBJDIR64)/_LINXSTAT_$(ARCH)
else
	$(RM) $(OBJDIR)/_LINXSTAT_$(ARCH)
endif

$(OBJDIR)/_LINXSTAT_$(ARCH):
	$(MKDIR) $(BINDIR)
ifneq ($(64BIT_SUPPORT),)
	$(MKDIR) $(OBJDIR64)
	$(RM) $(OBJDIR64)/_LINXSTAT_*
else
	$(MKDIR) $(OBJDIR)
	$(RM) $(OBJDIR)/_LINXSTAT_*
endif
	$(XMAKE) clean
	$(TOUCH) $@

install: linxstat
	$(ECHO) "Copying linxstat to $(INSTALLBINDIR)"
	$(CP) $(BINDIR)/linxstat $(INSTALLBINDIR)/

uninstall:
	$(ECHO) "Removing linxstat from $(INSTALLBINDIR)"
	$(RM) $(INSTALLBINDIR)/linxstat

.PHONY: linxstat clean
