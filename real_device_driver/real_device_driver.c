#include <linux/module.h>   /* needed for all modules */
#include <linux/kernel.h>   /* needed for kern_info */
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>     /*kmalloc()*/
#include <linux/uaccess.h>  /*copy_to/from_user*/
#include <linux/err.h>


#define DRIVER_AUTHOR "vle <letuanvu91@gmail.com>"
#define DRIVER_DESC "Real device driver"
#define mem_size    1024

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
uint8_t *kernel_buffer;

/*
** Function prototypes
*/

static int      __init etx_driver_init(void);
static void     __exit etx_driver_exit(void);
static int      etx_open(struct inode *inode, struct file *file);
static int      etx_release(struct inode *inode, struct file *file);
static ssize_t  etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t  etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off);

/*
** File operators structure
*/

static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = etx_read,
    .write      = etx_write,
    .open       = etx_open,
    .release    = etx_release,
};

/*
** Function implementation
*/

static int etx_open(struct inode *inode, struct file *file)
{
    pr_info("Device file opened...!\n");
    return 0;
}

static int etx_release(struct inode *inode, struct file *file)
{
    pr_info("Device file closed...!\n");
    return 0;
}

static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    // Copy the data from the kernel space to the user-space
    if (copy_to_user(buf, kernel_buffer, mem_size))
    {
        pr_err("Data read: Err!\n");
    }
    pr_info("Data read: Done!\n");
    return mem_size;
}

static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    // Copy the data from the kernel space to the user-space
    if (copy_from_user(kernel_buffer, buf, len))
    {
        pr_err("Data write: Err!\n");
    }
    pr_info("Data write: Done!\n");
    return len;
}

static int __init etx_driver_init(void)
{
    /* Allocating Major number */
    if ((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) < 0)
    {
        pr_info("Cannot allocate major number\n");
        return -1;
    }
    pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

    /* Creating cdev structure */
    cdev_init(&etx_cdev, &fops);

    /* Adding character device to the system*/
    if ((cdev_add(&etx_cdev, dev, 1)) < 0)
    {
        pr_info("Cannot add the device to the system\n");
        goto r_device;
    }

    /* Creating struct class */
    if(IS_ERR(dev_class = class_create(THIS_MODULE,"etx_class"))){
        pr_err("Cannot create the struct class\n");
        goto r_device;
    }
    /* Creating device */
    if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"etx_device"))){
        pr_err("Cannot create the Device 1\n");
        goto r_device;
    }

    /* Creating Physical memory */
    if ((kernel_buffer = kzalloc(mem_size, GFP_KERNEL)) == 0)
    {
        pr_info("Cannot allocate memory in kernel\n");
        goto r_device;
    }

    strcpy(kernel_buffer, "hello_world");

    pr_info("Device driver insert...Done!!!\n");
    return 0;

r_device:
    device_destroy(dev_class, dev);
    cdev_del(&etx_cdev);
    class_destroy(dev_class);
    unregister_chrdev_region(dev, 1);
    return -1;
}

static void __exit etx_driver_exit(void)
{
    kfree(kernel_buffer);
    device_destroy(dev_class, dev);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("Device driver remove...Done\n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);