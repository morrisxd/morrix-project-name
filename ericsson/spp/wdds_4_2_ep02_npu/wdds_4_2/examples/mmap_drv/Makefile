EXTRA_CFLAGS = -I$(PWD) -Wall -Werror

ifndef KERNEL_SOURCE
    KVER=`uname -r`
    KERNEL_SOURCE=/lib/modules/$(KVER)/build
endif

TOOLS_C = xsreset.c xsgettemp.c xsconsole.c xsgetfwversion.c xsprogi2c.c xsprogflash.c xsenv.c xsgetrevision.c xscheckwinmon.c xssfpdiag.c

TOOLS  	= $(patsubst %.c, %, $(TOOLS_C))

TOOLS_CFLAGS = -I$(PWD) -Wall -Werror

# Rule to make mmap driver module and tools
default: $(TOOLS)
	make -C $(KERNEL_SOURCE) M=`pwd` modules

% : %.c
	gcc $(TOOLS_CFLAGS) $^ -o $@ -lm

# The cleaner
clean:
	make -C $(KERNEL_SOURCE) M=`pwd` clean
	$(RM) $(TOOLS) *~

# The driver module itself
obj-m      = wp_mmap_drv.o

