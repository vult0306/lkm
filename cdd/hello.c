/*
 * chardev.c: Create a read-only char device that says how many times you've read from the dev file
 */

#include <linux/atomic.h>
#include <linux/kernel.h>   /* needed for kern_info */
#include <linux/module.h>   /* needed for all modules */
#include <linux/init.h>     /* needed for the macros */
#include <linux/fs.h>
#include <asm/uaccess.h>    /* for put_user */
#include "chardev.h"

static int device_open(struct inode*, struct file*);
static int device_release(struct inode*, struct file*);
static long device_ioctl(struct file*, unsigned int, unsigned long);
static ssize_t device_read(struct file*, char __user*, size_t, loff_t*);
static ssize_t device_write(struct file*, const char __user*, size_t, loff_t*);

#define DRIVER_AUTHOR "vle <letuanvu91@gmail.com>"
#define DRIVER_DESC "A sample hello world character device driver"
#define SUCCESS 0
#define DEVICE_NAME "char_dev"   /* apprears in /proc/devices */
#define BUF_LEN 80              /* max length of the message from the device */

enum {
    CDEV_NOT_USED = 0,
    CDEV_EXCLUSIVE_OPEN = 1,
};

/* 
 * Global variables are declared as static, so are global within the file
 */

static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED);     /* Is device open? used to prevent multiple access to device */
static char msg[BUF_LEN+1];       /* the msg the device will give when asked */

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .unlocked_ioctl = device_ioctl,
    .open = device_open,
    .release = device_release
};

/* Module load*/
static int __init hello_init(void) {
    int ret_val = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);
    if (ret_val < 0) {
        printk(KERN_ALERT "%s failed with %d\n",
        "Sorry, registering the character device ", ret_val);
        return ret_val;
    }

    printk(KERN_INFO "%s The major device number is %d.\n",
    "Registeration is a success", MAJOR_NUM);
    printk(KERN_INFO "If you want to talk to the device driver,\n");
    printk(KERN_INFO "you'll have to create a device file. \n");
    printk(KERN_INFO "We suggest you use:\n");
    printk(KERN_INFO "mknod %s c %d 0\n", DEVICE_FILE_NAME, MAJOR_NUM);
    printk(KERN_INFO "The device file name is important, because\n");
    printk(KERN_INFO "the ioctl program assumes that's the\n");
    printk(KERN_INFO "file you'll use.\n");
    return SUCCESS;
}

/* Module unload*/
static void __exit hello_exit(void) {
    unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
}

/* Method */

static int device_open(struct inode *inode, struct file *file)
{
    pr_info("device_open(%p)\n", file);
    try_module_get(THIS_MODULE);
    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
    pr_info("device release(%p, %p)\n", inode, file);
    module_put(THIS_MODULE);
    return SUCCESS;
}

static ssize_t device_read(struct file *file, char __user *buffer, size_t length, loff_t *offset)
{
    int bytes_read = 0;
    const char *msg_ptr = msg;
    if (!*(msg_ptr + *offset)) {
        *offset = 0;
        return 0;
    }

    msg_ptr += *offset;
    while(length && *msg_ptr) {
        put_user(*(msg_ptr++), buffer++);
        length--;
        bytes_read++;
    }

    pr_info("Read %d bytes, %zu left\n", bytes_read, length);
    *offset += bytes_read;

    return bytes_read;
}

static ssize_t device_write(struct file *file, const char __user *buff, size_t len, loff_t *off)
{
    int i;
    pr_info("device_write (%p, %p, %zu)", file, buff, len);
    for (i =0; i<len && i<BUF_LEN; i++)
        get_user(msg[i], buff + i);

    return i;
}

static long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
    int i;

    if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN))
        return -EBUSY;

    switch (ioctl_num) {
        case IOCTL_SET_MSG:
            /* 
            * Receive a pointer to a message (in user space) and set that
            * to be the device's message. Get the parameter given to 
            * ioctl by the process. 
            */
            char __user *temp = (char __user *)ioctl_param;
            char ch;
            /* 
            * Find the length of the message
            */
            get_user(ch, temp);
            for (i=0; ch && i<BUF_LEN; i++, temp++)
                get_user(ch, temp);
            device_write(file, (char*)ioctl_param, i, 0);
            break;

        case IOCTL_GET_MSG:
            /* 
            * Give the current message to the calling process − 
            * the parameter we got is a pointer, fill it. 
            */
            i = device_read(file, (char*)ioctl_param, 99, 0);
            /* 
            * Put a zero at the end of the buffer, so it will be 
            * properly terminated 
            */
            put_user('\0', (char*)ioctl_param + i);
            break;

        case IOCTL_GET_NTH_BYTE:
            /* 
            * This ioctl is both input (ioctl_param) and 
            * output (the return value of this function) 
            */
           return msg[ioctl_param];
           break;
    }

    return SUCCESS;
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);