all: echo_config libcfg linxcfg mklink rmlink mkethcon rmethcon mktcpcon rmtcpcon mkshmcon rmshmcon mkriocon rmriocon

LINX ?= $(PWD)/..
include $(LINX)/config.mk

ifneq ($(ASSERT),yes)
CFLAGS += -DNDEBUG
endif

CFLAGS += -I$(LINX)/include
CFLAGS += -I$(LINX)/net/linx/include
CFLAGS += -I$(LINX)/linxcfg
CFLAGS += -O2 -std=gnu99
CFLAGS += -DUSER_SPACE_COMMAND 
CFLAGS += -Wall 

PROGS = linxcfg mklink rmlink mkethcon rmethcon mktcpcon rmtcpcon mkshmcon rmshmcon mkriocon rmriocon
LINXCFG_OBJS = linxcfg.o config.o linx_cfg_eth.o linx_cfg_tcp.o linx_cfg_rio.o

libcfg:	$(OBJDIR)/_LINXCFG_$(ARCH)
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) -c $(CFLAGS) -o $(OBJDIR64)/db_utils.o db_utils.c
	$(GCC64) -c $(CFLAGS) -o $(OBJDIR64)/ethcm_utils.o ethcm_utils.c
	$(GCC64) -c $(CFLAGS) -o $(OBJDIR64)/riocm_utils.o riocm_utils.c
	$(GCC64) -c $(CFLAGS) -o $(OBJDIR64)/shmcm_utils.o shmcm_utils.c
	$(GCC64) -c $(CFLAGS) -o $(OBJDIR64)/tcpcm_utils.o tcpcm_utils.c
	$(GCC64) -c $(CFLAGS) -o $(OBJDIR64)/rlnh_utils.o rlnh_utils.c
	$(GCC64) -c $(CFLAGS) -o $(OBJDIR64)/libcfg.o libcfg.c
	$(AR) cru $(LIBDIR64)/libcfg.a $(OBJDIR64)/db_utils.o $(OBJDIR64)/rlnh_utils.o \
		$(OBJDIR64)/libcfg.o $(OBJDIR64)/ethcm_utils.o $(OBJDIR64)/tcpcm_utils.o \
		$(OBJDIR64)/shmcm_utils.o $(OBJDIR64)/riocm_utils.o
endif
	$(GCC32) -c $(CFLAGS) -o $(OBJDIR)/db_utils.o db_utils.c
	$(GCC32) -c $(CFLAGS) -o $(OBJDIR)/ethcm_utils.o ethcm_utils.c
	$(GCC32) -c $(CFLAGS) -o $(OBJDIR)/riocm_utils.o riocm_utils.c
	$(GCC32) -c $(CFLAGS) -o $(OBJDIR)/shmcm_utils.o shmcm_utils.c
	$(GCC32) -c $(CFLAGS) -o $(OBJDIR)/tcpcm_utils.o tcpcm_utils.c
	$(GCC32) -c $(CFLAGS) -o $(OBJDIR)/rlnh_utils.o rlnh_utils.c
	$(GCC32) -c $(CFLAGS) -o $(OBJDIR)/libcfg.o libcfg.c
	$(AR) cru $(LIBDIR)/libcfg.a $(OBJDIR)/db_utils.o $(OBJDIR)/rlnh_utils.o \
		$(OBJDIR)/libcfg.o $(OBJDIR)/tcpcm_utils.o $(OBJDIR)/ethcm_utils.o \
		$(OBJDIR)/shmcm_utils.o $(OBJDIR)/riocm_utils.o

mkethcon:	$(OBJDIR)/_LINXCFG_$(ARCH)
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) $(CFLAGS) -c mkethcon.c
	$(GCC64) $(CFLAGS) -o $(BINDIR)/$@ mkethcon.o $(LINX)/lib64/libcfg.a \
		$(LINX)/lib64/liblinx.a
else
	$(GCC32) $(CFLAGS) -c mkethcon.c
	$(GCC32) $(CFLAGS) -o $(BINDIR)/$@ mkethcon.o $(LINX)/lib/libcfg.a \
		$(LINX)/lib/liblinx.a
endif

rmethcon:	$(OBJDIR)/_LINXCFG_$(ARCH)
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) $(CFLAGS) -c rmethcon.c
	$(GCC64) $(CFLAGS) -o $(BINDIR)/$@ rmethcon.o $(LINX)/lib64/libcfg.a
else
	$(GCC32) $(CFLAGS) -c rmethcon.c
	$(GCC32) $(CFLAGS) -o $(BINDIR)/$@ rmethcon.o $(LINX)/lib/libcfg.a
endif

mkriocon:	$(OBJDIR)/_LINXCFG_$(ARCH)
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) $(CFLAGS) -c mkriocon.c
	$(GCC64) $(CFLAGS) -o $(BINDIR)/$@ mkriocon.o $(LINX)/lib64/libcfg.a \
		$(LINX)/lib64/liblinx.a
else
	$(GCC32) $(CFLAGS) -c mkriocon.c
	$(GCC32) $(CFLAGS) -o $(BINDIR)/$@ mkriocon.o $(LINX)/lib/libcfg.a \
		$(LINX)/lib/liblinx.a
endif

rmriocon:	$(OBJDIR)/_LINXCFG_$(ARCH)
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) $(CFLAGS) -c rmriocon.c
	$(GCC64) $(CFLAGS) -o $(BINDIR)/$@ rmriocon.o $(LINX)/lib64/libcfg.a
else
	$(GCC32) $(CFLAGS) -c rmriocon.c
	$(GCC32) $(CFLAGS) -o $(BINDIR)/$@ rmriocon.o $(LINX)/lib/libcfg.a
endif

mktcpcon:	$(OBJDIR)/_LINXCFG_$(ARCH)
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) $(CFLAGS) -c mktcpcon.c
	$(GCC64) $(CFLAGS) -o $(BINDIR)/$@ mktcpcon.o $(LINX)/lib64/libcfg.a
else
	$(GCC32) $(CFLAGS) -c mktcpcon.c
	$(GCC32) $(CFLAGS) -o $(BINDIR)/$@ mktcpcon.o $(LINX)/lib/libcfg.a
endif

rmtcpcon:	$(OBJDIR)/_LINXCFG_$(ARCH)
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) $(CFLAGS) -c rmtcpcon.c
	$(GCC64) $(CFLAGS) -o $(BINDIR)/$@ rmtcpcon.o $(LINX)/lib64/libcfg.a
else
	$(GCC32) $(CFLAGS) -c rmtcpcon.c
	$(GCC32) $(CFLAGS) -o $(BINDIR)/$@ rmtcpcon.o $(LINX)/lib/libcfg.a
endif

mkshmcon:	$(OBJDIR)/_LINXCFG_$(ARCH)
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) $(CFLAGS) -c mkshmcon.c
	$(GCC64) $(CFLAGS) -o $(BINDIR)/$@ mkshmcon.o $(LINX)/lib64/libcfg.a \
		$(LINX)/lib64/liblinx.a
else
	$(GCC32) $(CFLAGS) -c mkshmcon.c
	$(GCC32) $(CFLAGS) -o $(BINDIR)/$@ mkshmcon.o $(LINX)/lib/libcfg.a \
		$(LINX)/lib/liblinx.a
endif

rmshmcon:	$(OBJDIR)/_LINXCFG_$(ARCH)
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) $(CFLAGS) -c rmshmcon.c
	$(GCC64) $(CFLAGS) -o $(BINDIR)/$@ rmshmcon.o $(LINX)/lib64/libcfg.a
else
	$(GCC32) $(CFLAGS) -c rmshmcon.c
	$(GCC32) $(CFLAGS) -o $(BINDIR)/$@ rmshmcon.o $(LINX)/lib/libcfg.a
endif

mklink:		$(OBJDIR)/_LINXCFG_$(ARCH)
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) $(CFLAGS) -c mklink.c
	$(GCC64) $(CFLAGS) -o $(BINDIR)/$@ mklink.o $(LINX)/lib64/libcfg.a
else
	$(GCC32) $(CFLAGS) -c mklink.c
	$(GCC32) $(CFLAGS) -o $(BINDIR)/$@ mklink.o $(LINX)/lib/libcfg.a
endif

rmlink:		$(OBJDIR)/_LINXCFG_$(ARCH)
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) $(CFLAGS) -c rmlink.c
	$(GCC64) $(CFLAGS) -o $(BINDIR)/$@ rmlink.o $(LINX)/lib64/libcfg.a
else
	$(GCC32) $(CFLAGS) -c rmlink.c
	$(GCC32) $(CFLAGS) -o $(BINDIR)/$@ rmlink.o $(LINX)/lib/libcfg.a
endif

linxcfg:	$(OBJDIR)/_LINXCFG_$(ARCH)
	$(ECHO) "Building config programs"
ifneq ($(64BIT_SUPPORT),)
	$(GCC64) $(CFLAGS) -c linx_cfg_eth.c
	$(GCC64) $(CFLAGS) -c linx_cfg_rio.c
	$(GCC64) $(CFLAGS) -c linx_cfg_tcp.c
	$(GCC64) $(CFLAGS) -c linxcfg.c
	$(GCC64) $(CFLAGS) -c config.c
	$(GCC64) $(CFLAGS) -o $(BINDIR)/$@ $(LINXCFG_OBJS) $(LINX)/lib64/libcfg.a \
		$(LINX)/lib64/liblinx.a
else
	$(GCC32) $(CFLAGS) -c linx_cfg_eth.c
	$(GCC32) $(CFLAGS) -c linx_cfg_rio.c
	$(GCC32) $(CFLAGS) -c linx_cfg_tcp.c
	$(GCC32) $(CFLAGS) -c linxcfg.c
	$(GCC32) $(CFLAGS) -c config.c
	$(GCC32) $(CFLAGS) -o $(BINDIR)/$@ $(LINXCFG_OBJS) $(LINX)/lib/libcfg.a \
		$(LINX)/lib/liblinx.a
endif

clean:
	$(RM) $(PROGS)
ifneq ($(64BIT_SUPPORT),)
	$(RM) $(OBJDIR64)/_LINXCFG_$(ARCH)
else
	$(RM) $(OBJDIR)/_LINXCFG_$(ARCH)
endif
	$(RM) *.o

$(OBJDIR)/_LINXCFG_$(ARCH):
	$(MKDIR) $(BINDIR)
ifneq ($(64BIT_SUPPORT),)
	$(MKDIR) $(OBJDIR64)
	$(RM) $(OBJDIR64)/_LINXCFG_*
else
	$(MKDIR) $(OBJDIR)
	$(RM) $(OBJDIR)/_LINXCFG_*
endif
	$(XMAKE) clean
	$(TOUCH) $@

install: libcfg $(PROGS)
	$(ECHO) "Copying $(PROGS) to $(INSTALLBINDIR)"
	$(CP) $(addprefix $(BINDIR)/,$(PROGS)) $(INSTALLBINDIR)/

uninstall:
	$(ECHO) "Removing $(PROGS) from $(INSTALLBINDIR)"
	$(RM) $(addprefix $(INSTALLBINDIR)/,$(PROGS))

.PHONY: linxcfg mklink rmlink mkethcon rmethcon mktcpcon rmtcpcon mkshmcon rmshmcon mkriocon rmriocon clean
