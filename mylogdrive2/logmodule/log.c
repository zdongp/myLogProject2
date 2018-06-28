#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/sched.h>  	//wake_up_process()
#include <linux/kthread.h>	//kthread_create()、kthread_run()
#include <linux/err.h>      //IS_ERR()、PTR_ERR()

#define    LOG_MAJOR        520
#define    DEVICE_NAME     "LogModule"
#define    KTHREAD_NAME    "log_task"

static struct task_struct *log_task;

static int log_open(struct inode *inode, struct file *file)
{
    //printk(KERN_EMERG "log open.\n");
    return 0;
}

static struct file_operations log_flops = 
{
    .owner  =   THIS_MODULE,
    .open   =   log_open,     
};


int threadfunc(void *data)
{
	int count = 0;

 	while(1)
	{
    set_current_state(TASK_UNINTERRUPTIBLE);//将当前的状态表示设置为休眠
		if(kthread_should_stop()) 
		{
			printk(KERN_EMERG "log_tesk had been killed!\n");
			break;
		}

    printk(KERN_EMERG "FlyAudio.%04d\n", (int)count++);
    schedule_timeout(HZ);   //休眠，与set_current_state配合使用，需要计算，这里表示休眠一秒
 	}

 	return 0;
}

static int __init log_init(void)
{
    int ret;
    int err;

    ret = register_chrdev(LOG_MAJOR,DEVICE_NAME, &log_flops);
    if (ret < 0) 
    {
      printk(KERN_EMERG DEVICE_NAME " can't register major number.\n");
      return ret;
    }
    printk(KERN_EMERG DEVICE_NAME " initialized.\n");


  	log_task = kthread_create(threadfunc, NULL, KTHREAD_NAME);
  	if(IS_ERR(log_task))
  	{
  		printk("Unable to start kernel thread.\n");
  		err = PTR_ERR(log_task);
  		log_task =NULL;
  		return err;
  	}

  	wake_up_process(log_task);

    return 0;
}

static void __exit log_exit(void)
{
  unregister_chrdev(LOG_MAJOR, DEVICE_NAME);

	if(log_task)
	{
		kthread_stop(log_task);
		log_task = NULL;
	}

  printk(KERN_EMERG DEVICE_NAME " removed.\n");
}

module_init(log_init);
module_exit(log_exit);
MODULE_AUTHOR("DONG");
MODULE_LICENSE("GPL");

