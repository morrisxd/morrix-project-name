default: all

LINX ?= $(PWD)/..

include $(LINX)/config.mk

ifneq ($(ASSERT),yes)
CFLAGS += -DNDEBUG
endif

all: linx_bmark

clean: linx_bmark-clean

linx_bmark:
	$(XMAKE) -C linx_bmark linx_bmark

linx_bmark-clean:
	$(XMAKE) -C linx_bmark clean
	$(RM) $(LINX)/bmark/bin $(LINX)/bmark/obj

.PHONY: linx_bmark linx_bmark-clean
