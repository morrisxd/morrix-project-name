default: all

LINX ?= $(PWD)/..

include $(LINX)/config.mk

ifneq ($(ASSERT),yes)
CFLAGS += -DNDEBUG
endif

all: spp_linx

clean: spp_linx_clean

spp_linx:
	$(XMAKE) -C spp_linx spp_linx

spp_linx_clean:
	$(XMAKE) -C spp_linx clean
	$(RM) $(LINX)/example/bin $(LINX)/example/obj

.PHONY: spp_linx  spp_linx-clean
