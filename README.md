# echo_dd
Echo Device Driver. My first simple device driver.

## Quick Start
Get the corresponding linux headers package from your package manager. I use Ubuntu, as such I will provide copy-pastable commands for Ubuntu.
```shell
sudo apt update
sudo apt install linux-headers-$(uname -r)
```

Then you can just run:
```shell
make
```

This will use Linux Kernel specific flags/options to build your Kernel Module. And after this you have to load this module into the kernel.
```shell
sudo insmod main.ko
```

After inserting the module. You need to create a node in the filesystem that points to this character device we created.
```shell
sudo mknod /dev/my_device c <device major number> 0
```
I have made it so that major number is allocated by the kernel itself and I am printing it in the logs. So you can get the major number by:

```shell
sudo dmesg | tail
```

Now, try pipe-ing in and string.
```shell
sudo echo "Hello, World!" > /dev/my_device
```

Then look at the logs:
```shell
sudo dmesg | tail
```
The message entered should be echoed back.

Complete script, just replace the major number with an actual value:
```shell
sudo apt update
sudo apt install linux-headers-$(uname -r)

make

sudo insmod main.ko
sudo mknod /dev/my_device c <device major number> 0

sudo echo "Hello, World!" > /dev/my_device
sudo dmesg | tail
```

## Understanding
What I just created is a character device as opposed to a block device. These are similar to keyboard, mouse etc. The high level flow of what I did:
1. Defined an operation using `file_operations`.
2. Used that to register and create a character device via `cdev_init`.
3. Implemented the custom write callback to copy data from userland to kernelland and just `printk` it.

I am guessing we expose the APIs for reading and writing to a device by creating a character or block device. We have to internally handle the Bus communication (let's say USB or PCI). We talk in the same protocol as the micro-controller or micro-processor on the I/O device.

