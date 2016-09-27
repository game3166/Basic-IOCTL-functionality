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
	{ 0x98397cc5, "module_layout" },
	{ 0xbda451a8, "class_unregister" },
	{ 0x21a21ea4, "device_destroy" },
	{ 0xf62d269d, "class_destroy" },
	{ 0xf1faf509, "device_create" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xd6e17aa6, "__class_create" },
	{ 0xd71a9f7b, "__register_chrdev" },
	{ 0x37ff4c06, "copy_from_user_overflow" },
	{ 0x5f5602c6, "current_task" },
	{ 0x2f8794fc, "pid_task" },
	{ 0x5104d374, "find_vpid" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0x50eedeb8, "printk" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "BED117EF92E98A353E2A3D3");
