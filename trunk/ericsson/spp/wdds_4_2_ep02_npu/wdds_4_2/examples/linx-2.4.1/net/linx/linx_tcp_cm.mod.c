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
	{ 0xf9a482f9, "msleep" },
	{ 0x5fb4d7b4, "kernel_sendmsg" },
	{ 0x6980fe91, "param_get_int" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0x79aa04a2, "get_random_bytes" },
	{ 0xf0aa56a7, "malloc_sizes" },
	{ 0x138324ca, "sock_release" },
	{ 0x43d9b4f8, "queue_work" },
	{ 0x6a9f26c9, "init_timer_key" },
	{ 0x665b054c, "sock_create_kern" },
	{ 0xff964b25, "param_set_int" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0xc499ae1e, "kstrdup" },
	{ 0x7d11c268, "jiffies" },
	{ 0xffc7c184, "__init_waitqueue_head" },
	{ 0xa13798f8, "printk_ratelimit" },
	{ 0x9629486a, "per_cpu__cpu_number" },
	{ 0x1cefe352, "wait_for_completion" },
	{ 0xe83fea1, "del_timer_sync" },
	{ 0xde0bdcff, "memset" },
	{ 0xea147363, "printk" },
	{ 0x7fcdd29, "destroy_workqueue" },
	{ 0x46085e4f, "add_timer" },
	{ 0xda8c7344, "kill_pid" },
	{ 0x5cb74f2f, "__create_workqueue_key" },
	{ 0x42b51ffd, "flush_workqueue" },
	{ 0xd79b5a02, "allow_signal" },
	{ 0x16a04060, "module_put" },
	{ 0x2217c537, "db_proc_add" },
	{ 0xd180fb85, "kmem_cache_alloc" },
	{ 0x76518c54, "__alloc_skb" },
	{ 0x3bd1b1f6, "msecs_to_jiffies" },
	{ 0x1000e51, "schedule" },
	{ 0x57e00d37, "kfree_skb" },
	{ 0x6b2dc060, "dump_stack" },
	{ 0x1cc577cd, "db_del_template" },
	{ 0x4e56bdd6, "db_add_template" },
	{ 0x32047ad5, "__per_cpu_offset" },
	{ 0xd42c6a88, "kernel_recvmsg" },
	{ 0xb85c06a1, "init_pid_ns" },
	{ 0x3aa1dbcf, "_spin_unlock_bh" },
	{ 0xc3f264d1, "db_proc_del" },
	{ 0x37a0cba, "kfree" },
	{ 0x236c8c64, "memcpy" },
	{ 0x33d92f9a, "prepare_to_wait" },
	{ 0x9ccb2622, "finish_wait" },
	{ 0x7e9ebb05, "kernel_thread" },
	{ 0xe456bd3a, "complete" },
	{ 0x93cbd1ec, "_spin_lock_bh" },
	{ 0x22b38257, "skb_put" },
	{ 0x3302b500, "copy_from_user" },
	{ 0x8aa44fa5, "find_pid_ns" },
	{ 0xdc43a9c8, "daemonize" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=linx";


MODULE_INFO(srcversion, "28061A6A59689801B671862");
