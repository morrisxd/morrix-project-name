default: all

LINX ?= $(PWD)/..
include $(LINX)/config.mk

all: linxgws libgw linxgwcmd

clean: linxgws-clean libgw-clean linxgwcmd-clean

linxgws:
	$(XMAKE) -C linxgws linxgws

linxgwcmd:
	$(XMAKE) -C linxgwcmd linxgwcmd

libgw:
	$(XMAKE) -C libgw libgw

linxgws-clean:
	$(XMAKE) -C linxgws clean

linxgwcmd-clean:
	$(XMAKE) -C linxgwcmd clean

libgw-clean:
	$(XMAKE) -C libgw clean

.PHONY: linxgws linxgwcmd linxgws-clean linxgwcmd-clean libgw libgw-clean


