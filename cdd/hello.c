/*
 * chardev.c: Create a read-only char device that says how many times you've read from the dev file
 */

#include <linux/kernel.h>   /* needed for kern_info */
#include <linux/module.h>   /* needed for all modules */
#include <linux/init.h>     /* needed for the macros */
#include <linux/fs.h>
#include <asm/uaccess.h>    /* for put_user */

static int device_open(struct inode*, struct file*);
static int device_release(struct inode*, struct file*);
static ssize_t device_read(struct file*, char*, size_t, loff_t*);
static ssize_t device_write(struct file*, const char*, size_t, loff_t*);

#define DRIVER_AUTHOR "vle <letuanvu91@gmail.com>"
#define DRIVER_DESC "A sample hello world character device driver"
#define SUCCESS 0
#define DEVICE_NAME "chardev"   /* apprears in /proc/devices */
#define BUF_LEN 80              /* max length of the message from the device */

/* 
 * Global variables are declared as static, so are global within the file
 */

static int Major;               /* Major number assigned to our device driver */
static int Device_Open = 0;     /* Is device open? used to prevent multiple access to device */
static char msg[BUF_LEN];       /* the msg the device will give when asked */
static char *msg_Ptr;

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

/* Module load*/
static int __init hello_init(void) {
    printk(KERN_INFO "hello character device driver \n");
    Major = register_chrdev(0, DEVICE_NAME, &fops);
    if (Major < 0) {
        printk(KERN_ALERT "registering char device failed with %d\n", Major);
        return Major;
    }

    printk(KERN_INFO "assigned major number %d. To talk to \n", Major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
    printk(KERN_INFO "Try various minor numbers. Try to cat and echo to the device file\n");
    printk(KERN_INFO "Remove the device file and module when done\n");
    return SUCCESS;
}

/* Module unload*/
static void __exit hello_exit(void) {
    unregister_chrdev(Major, DEVICE_NAME);
}

/* Method */

static int device_open(struct inode *inode, struct file *file)
{
    static int counter = 0;
    if (Device_Open)
        return -EBUSY;

    Device_Open++;
    sprintf(msg, "I already told you %d times Hello world!\n",counter++);
    msg_Ptr = msg;
    try_module_get(THIS_MODULE);
    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
    Device_Open--;
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
    int bytes_read = 0;
    if (*msg_Ptr == 0)
        return 0;

    while(length && *msg_Ptr) {
        put_user(*(msg_Ptr++), buffer++);
        length--;
        bytes_read++;
    }

    return bytes_read;
}

static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t *off) {
    printk(KERN_ALERT "Sorry, this operations isn't supported\n");
    return -EINVAL;
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);