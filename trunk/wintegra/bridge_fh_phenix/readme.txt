1. modify makefile
======
add three new LIBs===>
                  $(WDDI_LIB_DIR)/ws_common.a 				  
				  $(WDDI_LIB_DIR)/shared.a 
				  $(WDDI_LIB_DIR)/wpi_pecs.a 
add new -I ===>
				-I$(WDDI_DIR)$(WDDI_SHARED)/sources/include/pm \
				-I$(WDDI_DIR)$(WDDI_SHARED)/sources/include/sw_log \
				-I$(WDDI_DIR)/board/wds_common

2. WDDS compile command:
=======
	make WPI_TARGET=mips_winmon WPI_BOARD=wds WPI_HW_DEVICE=winpath3 OSTYPE=linux

3. demo compile:
=======
make EXAMPLE=FH_ipv4 OSTYPE=linux

4. demo path:
=======
wdds_4_3\examples\FH_ipv4


