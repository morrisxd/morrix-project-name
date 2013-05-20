all: echo_config lib

LINX ?= $(PWD)/../..
include $(LINX)/config.mk

include $(LINX)/common.mk

CFLAGS += -I$(LINX)/include
CFLAGS += -I$(LINX)/linxgw/include
CFLAGS += -O2
CFLAGS += -Wall -pedantic -std=gnu99

ifneq ($(ASSERT),yes)
CFLAGS += -DNDEBUG
endif

libgw: $(OBJDIR)/_LIBGW_$(ARCH)
	$(ECHO) "Making libgw.a"
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) -c $(CFLAGS) -o $(OBJDIR64)/ose_gw.o ose_gw.c
	$(AR) cru $(LIBDIR64)/libgw.a $(OBJDIR64)/ose_gw.o
endif
	$(GCC32) -c $(CFLAGS) -o $(OBJDIR)/ose_gw.o ose_gw.c
	$(AR) cru $(LIBDIR)/libgw.a $(OBJDIR)/ose_gw.o

clean:
ifneq ($(64BIT_SUPPORT),)
	$(RM) $(OBJDIR64)/_LIBGW_$(ARCH) $(OBJDIR64)/ose_gw.o $(LIBDIR64)/libgw.a
endif
	$(RM) $(OBJDIR)/_LIBGW_$(ARCH) $(OBJDIR)/ose_gw.o $(LIBDIR)/libgw.a

$(OBJDIR)/_LIBGW_$(ARCH):
ifneq ($(64BIT_SUPPORT),)
	$(RM) $(OBJDIR64)/_LIBGW_*
endif
	$(RM) $(OBJDIR)/_LIBGW_*
	$(XMAKE) clean
	$(TOUCH) $@

.PHONY: libgw clean
