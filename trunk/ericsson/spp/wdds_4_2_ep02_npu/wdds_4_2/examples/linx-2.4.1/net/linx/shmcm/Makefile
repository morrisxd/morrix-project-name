# SHMCM Makefile

all: modules

LINX ?= $(PWD)/../../..
include $(LINX)/config.mk

KERNEL_CODE := $(shell awk '/LINUX_VERSION_CODE/ {print $$3}' $(KERNEL)/include/linux/version.h)
VENDOR_VERSION := $(shell awk '/UTS_RELEASE/ {print $$3}' $(KERNEL)/include/linux/version.h)

ifneq ($(VENDOR_VERSION), "")
	EXTRA_CFLAGS += -DGFP_IS_INT
endif

# Kernel version codes
2_6_13 := 132621

# 'gt_or_eq'
gt_or_eq = $(findstring $(1),$(word 2,$(sort $(2) $(1))))

ifeq ($(call gt_or_eq,$(KERNEL_CODE),$(2_6_13)),$(KERNEL_CODE))
	EXTRA_CFLAGS += -DSHMCM_COMPAT
else
  	ifeq ($(findstring 2.6.10_mvlcge401,$(VENDOR_VERSION)),2.6.10_mvlcge401)
    		EXTRA_CFLAGS += -DSHMCM_COMPAT
  	else
    		EXTRA_CFLAGS += -DSHMCM_KZALLOC	
  	endif
endif

ifeq ($(TRACE),yes)
	EXTRA_CFLAGS += -DSHMCM_TRACE
else
	ifeq ($(SHMCM_TRACE),yes)
		EXTRA_CFLAGS += -DSHMCM_TRACE
	endif
endif

EXTRA_CFLAGS += -Wall
ifeq ($(EXTRA_WARNINGS),yes)
EXTRA_CFLAGS += -Wextra #  -Wno-unused-parameter -Wno-sign-compare
endif
EXTRA_CFLAGS += -I$(LINX)/include -I$(LINX)/net/linx \
		-I$(LINX)/net/linx/include \
                -I$(LINX)/net/linx/shmcm

INSTALLDIR = /lib/modules/`uname -r`/kernel/net/linx
MODULES = linx_shm_cm.ko

obj-m := linx_shm_cm.o

linx_shm_cm-y += shmcm.o
linx_shm_cm-y += shmcm_rx.o
linx_shm_cm-y += shmcm_tx.o
linx_shm_cm-y += shmcm_kutils.o

MODVERSIONS = $(shell if [ -f ../Module.symvers ]; \
		then echo Module.symvers; \
		fi)

$(MODVERSIONS): ../Module.symvers
	$(CP) $< $@

modules: $(MODVERSIONS)
	make ARCH=$(ARCH) V=$(V) -C $(KERNEL) M=`pwd` modules
	$(CP) linx_shm_cm.ko ../.

clean:
	make ARCH=$(ARCH) -C $(KERNEL) M=`pwd` clean
	$(RM) Module.symvers modules.order

install: modules
	$(MKDIR) -p $(INSTALLDIR)
	$(ECHO) "Installing $(MODULES)"
	$(CP) $(MODULES) $(INSTALLDIR)
	$(DEPMOD) -a

uninstall:
	$(ECHO) "Uninstalling $(MODULES)"
	$(RM) $(addprefix $(INSTALLDIR)/,$(MODULES))
	$(RMDIR) $(INSTALLDIR)
	$(DEPMOD) -a
