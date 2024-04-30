#include <linux/module.h>   /* needed for all modules */
#include <linux/kernel.h>   /* needed for kern_info */
#include <linux/init.h>     /* needed for the macros */

#define DRIVER_AUTHOR "vle <letuanvu91@gmail.com>"
#define DRIVER_DESC "A sample hello world driver"

static void __exit hello6_exit(void) {
    printk(KERN_INFO "goodbye world 2\n");
}

module_exit(hello6_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);