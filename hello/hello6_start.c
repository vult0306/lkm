#include <linux/module.h>   /* needed for all modules */
#include <linux/kernel.h>   /* needed for kern_info */
#include <linux/init.h>     /* needed for the macros */

#define DRIVER_AUTHOR "vle <letuanvu91@gmail.com>"
#define DRIVER_DESC "A sample hello world driver"

static int __init hello6_init(void) {
    printk(KERN_INFO "hello world 2 \n");
    return 0;
}

module_init(hello6_init);
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);