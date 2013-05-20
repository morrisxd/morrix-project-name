default: all

# Where is Linx installed
LINX := $(PWD)

include $(LINX)/config.mk

obj-y += net/linx

all: dir_setup echo_config modules lib linxgw linxcfg linxstat linxdisc example bmark

dir_setup: FORCE
	$(MKDIR) $(BINDIR)
ifneq ($(64BIT_SUPPORT),)
	$(MKDIR) $(OBJDIR64)
	$(MKDIR) $(LIBDIR64)
endif
	$(MKDIR) $(OBJDIR)
	$(MKDIR) $(LIBDIR)

dir_clean: FORCE
	$(RMDIR) $(BINDIR)
ifneq ($(64BIT_SUPPORT),)
	$(RMDIR) $(OBJDIR64)
	$(RMDIR) $(LIBDIR64)
endif
	$(RMDIR) $(OBJDIR)
	$(RMDIR) $(LIBDIR)

clean:  modules-clean lib-clean linxdisc-clean linxstat-clean linxcfg-clean \
	example-clean bmark-clean dir_clean linxgw-clean

linxcfg: dir_setup lib
	$(XMAKE) -C linxcfg all

linxcfg-clean: 
	$(XMAKE) -C linxcfg clean

modules: dir_setup
	$(XMAKE) -C net/linx modules
	$(XMAKE) -C drivers/net modules
	$(XMAKE) -C net/linx/ecm modules
	$(XMAKE) -C net/linx/riocm modules

install: dir_setup
	$(XMAKE) -C net/linx install
	$(XMAKE) -C drivers/net install
	$(XMAKE) -C net/linx/ecm install
	$(XMAKE) -C net/linx/riocm install
	$(XMAKE) -C net/linx/shmcm install
	$(DEPMOD) -a
	$(XMAKE) -C liblinx lib
	$(XMAKE) -C linxcfg install
	$(XMAKE) -C linxstat install
	$(XMAKE) -C linxdisc install


uninstall: dir_setup
	$(XMAKE) -C net/linx uninstall
	$(XMAKE) -C drivers/net uninstall
	$(XMAKE) -C net/linx/ecm uninstall
	$(XMAKE) -C net/linx/riocm uninstall
	$(XMAKE) -C net/linx/shmcm uninstall
	$(DEPMOD) -a
	$(XMAKE) -C linxcfg uninstall
	$(XMAKE) -C linxstat uninstall
	$(XMAKE) -C linxdisc uninstall

modules-clean:
	$(XMAKE) -C net/linx clean
	$(XMAKE) -C drivers/net clean
	$(XMAKE) -C net/linx/ecm clean
	$(XMAKE) -C net/linx/riocm clean
	$(XMAKE) -C net/linx/shmcm clean

lib: dir_setup
	$(XMAKE) -C liblinx lib

lib-clean:
	$(XMAKE) -C liblinx clean

linxgw: dir_setup lib
	$(XMAKE) -C linxgw all

linxgw-clean:
	$(XMAKE) -C linxgw clean

linxdisc: dir_setup lib
	$(XMAKE) -C linxdisc linxdisc

linxdisc-clean:
	$(XMAKE) -C linxdisc clean

linxstat: dir_setup lib
	$(XMAKE) -C linxstat linxstat

linxstat-clean:
	$(XMAKE) -C linxstat clean

example: dir_setup lib
	$(XMAKE) -C example all

example-clean:
	$(XMAKE) -C example clean

bmark: dir_setup lib
	$(XMAKE) -C bmark all

bmark-clean:
	$(XMAKE) -C bmark clean


.PHONY: default all clean \
	linxcfg linxcfg-clean \
	module module-clean \
	lib lib-clean \
	linxdisc linxdisc-clean \
	linxstat linxstat-clean \
	example example-clean \
	dir_setup dir_clean \
	bmark bmark_clean \
	linxgw linxgw-clean
FORCE:
