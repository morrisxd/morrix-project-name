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
	{ 0x5a34a45c, "__kmalloc" },
	{ 0x6980fe91, "param_get_int" },
	{ 0x79aa04a2, "get_random_bytes" },
	{ 0xff964b25, "param_set_int" },
	{ 0xb7e032a, "free_netdev" },
	{ 0x582a9a3e, "register_netdev" },
	{ 0x97ba559c, "skb_push" },
	{ 0x7eea2382, "alloc_netdev_mq" },
	{ 0x37a0cba, "kfree" },
	{ 0x90f824fa, "unregister_netdev" },
	{ 0xb437566d, "consume_skb" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "9DD18141210632ADE716FA7");
