#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xd2e13a54, "module_layout" },
	{ 0xad970138, "per_cpu__current_task" },
	{ 0x5a34a45c, "__kmalloc" },
	{ 0x25ec1b28, "strlen" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0x8eb58c6e, "__wake_up_sync" },
	{ 0x79aa04a2, "get_random_bytes" },
	{ 0xf0aa56a7, "malloc_sizes" },
	{ 0xd4db04e0, "skb_clone" },
	{ 0x20000329, "simple_strtoul" },
	{ 0x63ecad53, "register_netdevice_notifier" },
	{ 0x43d9b4f8, "queue_work" },
	{ 0x6a9f26c9, "init_timer_key" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0x7d11c268, "jiffies" },
	{ 0xfe769456, "unregister_netdevice_notifier" },
	{ 0x2a0e66c5, "skb_trim" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xffc7c184, "__init_waitqueue_head" },
	{ 0x9629486a, "per_cpu__cpu_number" },
	{ 0xe83fea1, "del_timer_sync" },
	{ 0xfe8673e2, "skb_queue_purge" },
	{ 0xde0bdcff, "memset" },
	{ 0xea147363, "printk" },
	{ 0xf397b9aa, "__tasklet_schedule" },
	{ 0x6dcaeb88, "per_cpu__kernel_stack" },
	{ 0x7fcdd29, "destroy_workqueue" },
	{ 0x1c8e3c31, "dev_remove_pack" },
	{ 0xa5808bbf, "tasklet_init" },
	{ 0x45450063, "mod_timer" },
	{ 0x5cb74f2f, "__create_workqueue_key" },
	{ 0xb7eedf8d, "skb_pull" },
	{ 0x26534032, "init_net" },
	{ 0x42b51ffd, "flush_workqueue" },
	{ 0x79ad224b, "tasklet_kill" },
	{ 0x16a04060, "module_put" },
	{ 0xdb74d231, "skb_queue_tail" },
	{ 0x2217c537, "db_proc_add" },
	{ 0xd180fb85, "kmem_cache_alloc" },
	{ 0x76518c54, "__alloc_skb" },
	{ 0x3bd1b1f6, "msecs_to_jiffies" },
	{ 0x1000e51, "schedule" },
	{ 0x57e00d37, "kfree_skb" },
	{ 0x1cc577cd, "db_del_template" },
	{ 0x4e56bdd6, "db_add_template" },
	{ 0x32047ad5, "__per_cpu_offset" },
	{ 0x642e54ac, "__wake_up" },
	{ 0x3aa1dbcf, "_spin_unlock_bh" },
	{ 0xc3f264d1, "db_proc_del" },
	{ 0x37a0cba, "kfree" },
	{ 0x236c8c64, "memcpy" },
	{ 0x33d92f9a, "prepare_to_wait" },
	{ 0x9ccb2622, "finish_wait" },
	{ 0xdf7f8dcc, "skb_dequeue" },
	{ 0x43bd4db9, "dev_add_pack" },
	{ 0xb4f0a02a, "dev_queue_xmit" },
	{ 0x93cbd1ec, "_spin_lock_bh" },
	{ 0x22b38257, "skb_put" },
	{ 0x3302b500, "copy_from_user" },
	{ 0xe914e41e, "strcpy" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=linx";


MODULE_INFO(srcversion, "3B198E1D919418461EBBCEE");
