#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h> // For kmalloc and kfree

static ssize_t my_write(struct file * f, const char __user * buff, size_t len, loff_t * offset) {
    char* message = kmalloc((len+1)*sizeof(char), GFP_KERNEL);
    if (!message) return -ENOMEM; // Very important. I don't know what happens when kernel panics, and I don't want to know that.

    // We need to do this.
    // https://unix.stackexchange.com/questions/674962/why-are-copy-from-user-and-copy-to-user-needed-when-the-kernel-is-mappe
    if(copy_from_user(message, buff, len)) {
        kfree(message);
        return -EFAULT;
    }

    message[len] = '\0';
    printk(KERN_INFO "Message received: %s\n", message);
    kfree(message);
    return len;
}

// Global state. I guess this is common in driver code as the entry point returns immediately, so the stack frame is popped immediately
dev_t dev;
struct cdev my_device;

// It is fine if all the other function pointers are NULL
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = my_write,
};

static int my_init(void) {
    printk(KERN_INFO "My module entering\n");

    int ret = alloc_chrdev_region(&dev, 0, 1, "my_custom_device");
    if (ret < 0) {
        printk(KERN_ERR "Cannot allocate character device region\n");
        return ret;
    }
    printk(KERN_INFO "Device major number: %d\n", MAJOR(dev));

    cdev_init(&my_device, &fops);
    cdev_add(&my_device, MKDEV(MAJOR(dev), 0), 1);
    return 0;
}

static void my_exit(void) {
    cdev_del(&my_device);
    unregister_chrdev_region(MKDEV(MAJOR(dev), 0), 1);

    printk(KERN_INFO "My module exiting\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL"); // This is not optional
