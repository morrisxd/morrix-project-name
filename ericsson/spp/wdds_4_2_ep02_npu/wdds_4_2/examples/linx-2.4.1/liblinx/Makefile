all: echo_config lib

LINX ?= $(PWD)/..
include $(LINX)/config.mk

include $(LINX)/common.mk

CFLAGS += -I$(LINX)/include
CFLAGS += -I$(LINX)/lib
CFLAGS += -O2
CFLAGS += -Wall -pedantic -std=gnu99

ifneq ($(ASSERT),yes)
CFLAGS += -DNDEBUG
endif

lib: $(OBJDIR)/_LIBLINX_$(ARCH)
	$(ECHO) "Making liblinx.a"
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) -c $(CFLAGS) -o $(OBJDIR64)/linx.o linx.c
	$(AR) cru $(LIBDIR64)/liblinx.a $(OBJDIR64)/linx.o
endif
	$(GCC32) -c $(CFLAGS) -o $(OBJDIR)/linx.o linx.c
	$(AR) cru $(LIBDIR)/liblinx.a $(OBJDIR)/linx.o

clean:
ifneq ($(64BIT_SUPPORT),)
	$(RM) $(OBJDIR64)/_LIBLINX_$(ARCH) $(OBJDIR64)/linx.o $(LIBDIR64)/liblinx.a
endif
	$(RM) $(OBJDIR)/_LIBLINX_$(ARCH) $(OBJDIR)/linx.o $(LIBDIR)/liblinx.a

$(OBJDIR)/_LIBLINX_$(ARCH):
ifneq ($(64BIT_SUPPORT),)
	$(RM) $(OBJDIR64)/_LIBLINX_*
endif
	$(RM) $(OBJDIR)/_LIBLINX_*
	$(XMAKE) clean
	$(TOUCH) $@

.PHONY: lib clean
