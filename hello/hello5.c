#include <linux/module.h>   /* needed for all modules */
#include <linux/kernel.h>   /* needed for KERN_ERR */
#include <linux/init.h>     /* needed for the macros */
#include <linux/moduleparam.h>
#include <linux/stat.h>

#define DRIVER_AUTHOR "vle <letuanvu91@gmail.com>"
#define DRIVER_DESC "A sample hello world driver"

static short int myshort = 1;
static int myint = 420;
static long int mylong = 9999;
static char *mystring = "blah";
static int myintArray[2] = {-1, -1};
static int arr_argc = 0;

/*
 * module_param(foo, int, 0000)
 * foo: parameters name
 * int: data type
 * 0000: permissions bits for exposing parameters in sysfs
 */

module_param(myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(myshort, "A short integer");

module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(myint, "An interger");

module_param(mylong, long, S_IRUSR);
MODULE_PARM_DESC(mylong, "A long integer");

module_param(mystring, charp, 0000);
MODULE_PARM_DESC(mystring, "A character string");

module_param_array(myintArray, int, &arr_argc, 0000);
MODULE_PARM_DESC(myintArray, "An array of integer");

static int __init hello5_init(void) {

    printk(KERN_ERR "hello world 5 \n");
    printk(KERN_ERR "myshort: %hd\n",myshort);
    printk(KERN_ERR "myint: %d\n",myint);
    printk(KERN_ERR "mylong: %ld\n",mylong);
    printk(KERN_ERR "mystring: %s\n",mystring);
    
    printk(KERN_ERR "got %d arguments for myintArray\n",arr_argc);
    for (int i=0; i<sizeof(myintArray)/sizeof(int); i++) {
        printk(KERN_ERR "myintArray[%d]: %d\n", i, myintArray[i]);
    }

    return 0;
}

static void __exit hello5_exit(void) {
    printk(KERN_ERR "goodbye world 5\n");
}

module_init(hello5_init);
module_exit(hello5_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);