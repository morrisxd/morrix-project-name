#
# Carsten Langgaard, carstenl@mips.com
# Copyright (C) 1999,2000 MIPS Technologies, Inc.  All rights reserved.
#
# Copyright (C) 2008 Wind River Systems, Inc.
#   written by Ralf Baechle <ralf@linux-mips.org>
#
obj-y				:= wds3-winmon.o wds3-init.o wds3-int.o \
				   wds3-memory.o wds3-reset.o wds3-time.o \
				   wds3-cmdline.o

# FIXME FIXME FIXME
obj-$(CONFIG_MIPS_MT_SMTC)	+= wds3-smtc.o

EXTRA_CFLAGS += -Werror
