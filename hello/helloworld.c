#include <linux/module.h>   /* needed for all modules */
#include <linux/kernel.h>   /* needed for kern_info */

#define DRIVER_AUTHOR "vle <letuanvu91@gmail.com>"
#define DRIVER_DESC "A sample hello world driver"

int init_module(void) {
    printk(KERN_INFO "hello world \n");
    return 0;
}

void cleanup_module(void) {
    printk(KERN_INFO "goodbye world\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);