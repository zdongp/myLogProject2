#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>

#define    LOG_MAJOR        520
#define    DEVICE_NAME     "LogModule"

static int log_open(struct inode *inode, struct file *file)
{
    printk(KERN_EMERG "log open.\n");
    return 0;
}

static ssize_t log_write(struct file *file, const char __user * buf, size_t count, loff_t *ppos)
{
    
    printk(KERN_EMERG "log write.\n");
    printk(KERN_EMERG "FlyAudio.%d\n", (int)count); 
    return 0;
}

static struct file_operations log_flops = 
{
    .owner  =   THIS_MODULE,
    .open   =   log_open,     
    .write  =   log_write,
};

static int __init log_init(void)
{
    int ret;
    
    ret = register_chrdev(LOG_MAJOR,DEVICE_NAME, &log_flops);
    if (ret < 0) 
    {
      printk(KERN_EMERG DEVICE_NAME " can't register major number.\n");
      return ret;
    }
    printk(KERN_EMERG DEVICE_NAME " initialized.\n");
    return 0;
}

static void __exit log_exit(void)
{
    unregister_chrdev(LOG_MAJOR, DEVICE_NAME);
    printk(KERN_EMERG DEVICE_NAME " removed.\n");
}

module_init(log_init);
module_exit(log_exit);
MODULE_AUTHOR("DONG");
MODULE_LICENSE("GPL");
