ifeq ($(VERBOSE),yes)
VERB         =
XMAKE_SILENT =
V            = 1
else
VERB         = @
ifneq ($(NEED_KERNEL),yes)
XMAKE_SILENT = -s
else
XMAKE_SILENT =
endif
V            = 0
endif

XMAKE := $(VERB)$(MAKE) $(XMAKE_SILENT)
AR    := $(VERB)$(CROSS_COMPILE)ar
GCC   := $(VERB)$(CROSS_COMPILE)gcc

ifeq ($(ARCH),arm)
GCC32 := $(GCC)
else
ifeq ($(findstring mips,$(ARCH)),mips)
GCC32 := $(GCC) -mabi=n32 -march=octeon
GCC64 := $(GCC) -mabi=64 -march=octeon
else
GCC32 := $(GCC) -m32
GCC64 := $(GCC) -m64
endif
endif

CPP   := $(CROSS_COMPILE)gcc -E
STRIP := $(VERB)$(CROSS_COMPILE)strip
ECHO  := $(VERB)echo
FIND  := $(VERB)find
MKDIR := $(VERB)-mkdir -p
RMDIR := $(VERB)-rm -rf
RM    := $(VERB)rm -rf
TOUCH := $(VERB)touch
TAR   := $(VERB)tar
CP    := $(VERB)cp
DEPMOD := $(VERB)depmod

OBJDIR   = $(LINX)/obj
OBJDIR64 = $(LINX)/obj64
LIBDIR   = $(LINX)/lib
LIBDIR64 = $(LINX)/lib64
BINDIR   = $(LINX)/bin

INSTALLBINDIR = /usr/bin
INSTALLCONFDIR = /etc
