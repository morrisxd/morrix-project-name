cmd_/usr/src/wdds_4_2/examples/linx-2.4.1/net/linx/riocm/rio_kutils.o := gcc -Wp,-MD,/usr/src/wdds_4_2/examples/linx-2.4.1/net/linx/riocm/.rio_kutils.o.d  -nostdinc -isystem /usr/lib64/gcc/x86_64-suse-linux/4.3/include -Iinclude -Iinclude2 -I/usr/src/linux-2.6.32.29-0.3/include -I/usr/src/linux-2.6.32.29-0.3/arch/x86/include -include include/linux/autoconf.h   -I/usr/src/wdds_4_2/examples/linx-2.4.1/net/linx/riocm -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -m64 -mtune=generic -mno-red-zone -mcmodel=kernel -funit-at-a-time -maccumulate-outgoing-args -DCONFIG_AS_CFI=1 -DCONFIG_AS_CFI_SIGNAL_FRAME=1 -pipe -Wno-sign-compare -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -fno-stack-protector -fomit-frame-pointer -fasynchronous-unwind-tables -g -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -DRLNH_LITTLE_ENDIAN -DGFP_IS_INT -DRIO_COMPAT -Wall   -I/usr/src/wdds_4_2/examples/linx-2.4.1/include   -I/usr/src/wdds_4_2/examples/linx-2.4.1/net/linx   -I/usr/src/wdds_4_2/examples/linx-2.4.1/net/linx/include   -I/usr/src/wdds_4_2/examples/linx-2.4.1/net/linx/riocm  -DMODULE -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(rio_kutils)"  -D"KBUILD_MODNAME=KBUILD_STR(linx_rio_cm)" -D"DEBUG_HASH=30" -D"DEBUG_HASH2=21" -c -o /usr/src/wdds_4_2/examples/linx-2.4.1/net/linx/riocm/.tmp_rio_kutils.o /usr/src/wdds_4_2/examples/linx-2.4.1/net/linx/riocm/rio_kutils.c

deps_/usr/src/wdds_4_2/examples/linx-2.4.1/net/linx/riocm/rio_kutils.o := \
  /usr/src/wdds_4_2/examples/linx-2.4.1/net/linx/riocm/rio_kutils.c \
  /usr/src/linux-2.6.32.29-0.3/include/linux/ctype.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/debug/slab.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/gfp.h \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/debug/vm.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/arch/populates/node/map.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/cgroup/mem/res/ctlr.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/typecheck.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  include2/asm/types.h \
    $(wildcard include/config/x86/64.h) \
    $(wildcard include/config/xen.h) \
    $(wildcard include/config/highmem64g.h) \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/types.h \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/int-ll64.h \
  include2/asm/bitsperlong.h \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/bitsperlong.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/posix_types.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/stddef.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/compiler-gcc4.h \
  include2/asm/posix_types.h \
    $(wildcard include/config/x86/32.h) \
  include2/asm/posix_types_64.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/bitops.h \
    $(wildcard include/config/generic/find/first/bit.h) \
    $(wildcard include/config/generic/find/last/bit.h) \
    $(wildcard include/config/generic/find/next/bit.h) \
  include2/asm/bitops.h \
    $(wildcard include/config/x86/cmov.h) \
  include2/asm/alternative.h \
    $(wildcard include/config/paravirt.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/stringify.h \
  include2/asm/asm.h \
  include2/asm/cpufeature.h \
    $(wildcard include/config/x86/invlpg.h) \
  include2/asm/required-features.h \
    $(wildcard include/config/x86/minimum/cpu/family.h) \
    $(wildcard include/config/math/emulation.h) \
    $(wildcard include/config/x86/pae.h) \
    $(wildcard include/config/x86/cmpxchg64.h) \
    $(wildcard include/config/x86/use/3dnow.h) \
    $(wildcard include/config/x86/p6/nop.h) \
    $(wildcard include/config/paravirt/mmu.h) \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/bitops/sched.h \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/bitops/hweight.h \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/bitops/fls64.h \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/bitops/ext2-non-atomic.h \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/bitops/le.h \
  include2/asm/byteorder.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/byteorder/little_endian.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/swab.h \
  include2/asm/swab.h \
    $(wildcard include/config/x86/bswap.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/byteorder/generic.h \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/bitops/minix.h \
  include2/asm/thread_info.h \
    $(wildcard include/config/x86/xen.h) \
    $(wildcard include/config/debug/stack/usage.h) \
  include2/asm/page.h \
  include2/asm/page_types.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/const.h \
  include2/asm/page_64_types.h \
    $(wildcard include/config/physical/start.h) \
    $(wildcard include/config/physical/align.h) \
  include2/asm/page_64.h \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/memory_model.h \
    $(wildcard include/config/sparsemem/vmemmap.h) \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/getorder.h \
  include2/asm/processor.h \
    $(wildcard include/config/x86/vsmp.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/paravirt/cpu.h) \
    $(wildcard include/config/m386.h) \
    $(wildcard include/config/m486.h) \
    $(wildcard include/config/x86/debugctlmsr.h) \
  include2/asm/processor-flags.h \
    $(wildcard include/config/vm86.h) \
  include2/asm/vm86.h \
  include2/asm/ptrace.h \
    $(wildcard include/config/x86/ptrace/bts.h) \
  include2/asm/ptrace-abi.h \
  include2/asm/segment.h \
  include2/asm/cache.h \
    $(wildcard include/config/x86/l1/cache/shift.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/linkage.h \
  include2/asm/linkage.h \
    $(wildcard include/config/x86/alignment/16.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  include2/asm/math_emu.h \
  include2/asm/sigcontext.h \
  include2/asm/current.h \
  include2/asm/percpu.h \
    $(wildcard include/config/x86/64/smp.h) \
    $(wildcard include/config/x86/xadd.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/enterprise/support.h) \
    $(wildcard include/config/kmsg/ids.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /usr/lib64/gcc/x86_64-suse-linux/4.3/include/stdarg.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/ratelimit.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/param.h \
  include2/asm/param.h \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/dynamic_debug.h \
  include2/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/percpu.h \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  include2/asm/system.h \
    $(wildcard include/config/ia32/emulation.h) \
    $(wildcard include/config/x86/32/lazy/gs.h) \
    $(wildcard include/config/x86/ppro/fence.h) \
    $(wildcard include/config/x86/oostore.h) \
  include2/asm/cmpxchg.h \
  include2/asm/cmpxchg_64.h \
  include2/asm/nops.h \
    $(wildcard include/config/mk7.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
    $(wildcard include/config/x86.h) \
  include2/asm/irqflags.h \
    $(wildcard include/config/paravirt/irq.h) \
  include2/asm/paravirt.h \
    $(wildcard include/config/paravirt/time.h) \
    $(wildcard include/config/paravirt/apic.h) \
    $(wildcard include/config/highpte.h) \
    $(wildcard include/config/paravirt/spinlocks.h) \
  include2/asm/pgtable_types.h \
    $(wildcard include/config/compat/vdso.h) \
    $(wildcard include/config/proc/fs.h) \
  include2/asm/pgtable_64_types.h \
  include2/asm/paravirt_types.h \
    $(wildcard include/config/x86/local/apic.h) \
    $(wildcard include/config/paravirt/debug.h) \
  include2/asm/desc_defs.h \
  include2/asm/kmap_types.h \
    $(wildcard include/config/debug/highmem.h) \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/kmap_types.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/bitmap.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  include2/asm/string.h \
  include2/asm/string_64.h \
  include2/asm/msr.h \
  include2/asm/msr-index.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/ioctl.h \
  include2/asm/ioctl.h \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/ioctl.h \
  include2/asm/errno.h \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/errno.h \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/errno-base.h \
  include2/asm/cpumask.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/errno.h \
  include2/asm/ds.h \
    $(wildcard include/config/x86/ds.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/err.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/personality.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/math64.h \
  include2/asm/div64.h \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/div64.h \
  include2/asm/ftrace.h \
    $(wildcard include/config/function/tracer.h) \
    $(wildcard include/config/dynamic/ftrace.h) \
  include2/asm/atomic.h \
  include2/asm/atomic_64.h \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/atomic-long.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/prefetch.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/bottom_half.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/spinlock_types.h \
  include2/asm/spinlock_types.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/generic/hardirqs.h) \
  include2/asm/spinlock.h \
  include2/asm/rwlock.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/spinlock_api_smp.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/wait.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/seqlock.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/nodemask.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/linux/bounds.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/memory_hotplug.h \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/memory/hotremove.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/notifier.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include2/asm/rwsem.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/srcu.h \
  include2/asm/mmzone.h \
  include2/asm/mmzone_64.h \
    $(wildcard include/config/numa/emu.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/mmdebug.h \
    $(wildcard include/config/debug/virtual.h) \
  include2/asm/smp.h \
    $(wildcard include/config/x86/io/apic.h) \
    $(wildcard include/config/x86/32/smp.h) \
  include2/asm/mpspec.h \
    $(wildcard include/config/x86/numaq.h) \
    $(wildcard include/config/mca.h) \
    $(wildcard include/config/eisa.h) \
    $(wildcard include/config/x86/mpparse.h) \
    $(wildcard include/config/acpi.h) \
  include2/asm/mpspec_def.h \
  include2/asm/x86_init.h \
  include2/asm/bootparam.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/screen_info.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/apm_bios.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/edd.h \
  include2/asm/e820.h \
    $(wildcard include/config/efi.h) \
    $(wildcard include/config/hibernation.h) \
    $(wildcard include/config/memtest.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/ioport.h \
  include2/asm/ist.h \
  /usr/src/linux-2.6.32.29-0.3/include/video/edid.h \
  include2/asm/apic.h \
    $(wildcard include/config/x86/x2apic.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/delay.h \
  include2/asm/delay.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/pm.h \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/pm/runtime.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/workqueue.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/jiffies.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/timex.h \
    $(wildcard include/config/no/hz.h) \
  include2/asm/timex.h \
  include2/asm/tsc.h \
    $(wildcard include/config/x86/tsc.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects/free.h) \
  include2/asm/apicdef.h \
  include2/asm/fixmap.h \
    $(wildcard include/config/provide/ohci1394/dma/init.h) \
    $(wildcard include/config/x86/visws/apic.h) \
    $(wildcard include/config/x86/f00f/bug.h) \
    $(wildcard include/config/x86/cyclone/timer.h) \
    $(wildcard include/config/pci/mmconfig.h) \
    $(wildcard include/config/intel/txt.h) \
  include2/asm/acpi.h \
    $(wildcard include/config/acpi/numa.h) \
  /usr/src/linux-2.6.32.29-0.3/include/acpi/pdc_intel.h \
  include2/asm/numa.h \
  include2/asm/numa_64.h \
  include2/asm/mmu.h \
  include2/asm/vsyscall.h \
    $(wildcard include/config/generic/time.h) \
  include2/asm/io_apic.h \
  include2/asm/irq_vectors.h \
    $(wildcard include/config/sparse/irq.h) \
  include2/asm/sparsemem.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  include2/asm/topology.h \
    $(wildcard include/config/x86/ht.h) \
    $(wildcard include/config/x86/64/acpi/numa.h) \
  /usr/src/linux-2.6.32.29-0.3/include/asm-generic/topology.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/slab_def.h \
    $(wildcard include/config/kmemtrace.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/kmemtrace.h \
  /usr/src/linux-2.6.32.29-0.3/include/trace/events/kmem.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/tracepoint.h \
    $(wildcard include/config/tracepoints.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/rcupdate.h \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/tree/rcu.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/completion.h \
  /usr/src/linux-2.6.32.29-0.3/include/linux/rcutree.h \
  /usr/src/linux-2.6.32.29-0.3/include/trace/define_trace.h \
    $(wildcard include/config/event/tracing.h) \
  /usr/src/linux-2.6.32.29-0.3/include/linux/kmalloc_sizes.h \
  /usr/src/wdds_4_2/examples/linx-2.4.1/net/linx/riocm/rio_kutils.h \
  include/linux/version.h \

/usr/src/wdds_4_2/examples/linx-2.4.1/net/linx/riocm/rio_kutils.o: $(deps_/usr/src/wdds_4_2/examples/linx-2.4.1/net/linx/riocm/rio_kutils.o)

$(deps_/usr/src/wdds_4_2/examples/linx-2.4.1/net/linx/riocm/rio_kutils.o):
