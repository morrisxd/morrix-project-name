all: echo_config modules

LINX ?= $(PWD)/../..

include $(LINX)/config.mk

EXTRA_CFLAGS += -Wall

OBJDIR = $(LINX)/obj

INSTALLDIR = /lib/modules/`uname -r`/kernel/drivers/net
MODULES = linxtrace.ko

obj-m := linxtrace.o

linxtrace-y += linxtracedrv.o

ifeq ($(LINXTRACE_DBG),yes)
EXTRA_CFLAGS += -DLINXTRACE_DBG
endif

ifeq ($(EXTRA_WARNINGS),yes)
EXTRA_CFLAGS += -Wextra # -Wno-unused-parameter -Wno-sign-compare
endif

modules: $(OBJDIR)/_MODULES_$(ARCH)
	$(ECHO) "Building $@."
	$(XMAKE) ARCH=$(ARCH) V=$(V) -C $(KERNEL) M=`pwd` modules

clean:
	$(RM) $(OBJDIR)/_MODULES_* *.o */*.o *.cmd .tmp* *.ko .*.cmd \
	*.mod.c */*.cmd */.*.cmd Module.symvers Module.markers \
	modules.order

install: modules
	$(MKDIR) -p $(INSTALLDIR)
	$(ECHO) "Installing $(MODULES)"
	$(CP) $(MODULES) $(INSTALLDIR)
	$(DEPMOD) -a

uninstall:
	$(ECHO) "Uninstalling $(MODULES)"
	$(RM) $(addprefix $(INSTALLDIR)/,$(MODULES))
	$(DEPMOD) -a


$(OBJDIR)/_MODULES_$(ARCH):
	$(MKDIR) $(OBJDIR)
	$(RM) $(OBJDIR)/_MODULES_*
	$(XMAKE) clean
	$(TOUCH) $@
