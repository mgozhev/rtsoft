#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/io.h>
#include <asm/uaccess.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/mutex.h>


#define CBUF_SIZE 32

#define DEVICE "saw"

DEFINE_MUTEX(cbuf_lock);

static DECLARE_WAIT_QUEUE_HEAD(wq);
static int flag = 0;

static char cbuffer [CBUF_SIZE];
static int cbuf_begin, cbuf_end, cbuf_len;

static int blackboard_count = 1;
static dev_t blackboard_dev; /* Dynamically allocated now. = MKDEV (202, 0); */

static struct cdev blackboard_cdev;


#define SAW_MAX 100

struct task_struct *ts;
int thread (void *data)
{
	int n;
	n = 0;
	while (1)
	{
		mutex_lock(&cbuf_lock);
		cbuffer[cbuf_end] = n;
		cbuf_end = (cbuf_end + 1) % CBUF_SIZE;
		if (cbuf_begin==cbuf_end) {
			cbuf_begin = (cbuf_begin + 1) % CBUF_SIZE;
			
		}
		else {
			cbuf_len++;
		}

		mutex_unlock(&cbuf_lock);
        	
		flag = 1;
        	wake_up_interruptible(&wq);


		n++;
		if (n > SAW_MAX) n = 0;
		
	
		msleep (100);
		if (kthread_should_stop()) break;
	}
	return 0;
}


static int
blackboard_release (struct inode *inode, struct file *file)
{
    return 0;
}

static int
blackboard_open (struct inode *inode, struct file *file)
{
    file->f_pos = 0;
    return 0;
}

static ssize_t
blackboard_read (struct file *file, char __user *buf, size_t count, 
        loff_t *f_pos)
{
    size_t transfer_size=0;

	

        if (cbuf_len == 0) {
            wait_event_interruptible (wq, flag != 0);
        }
        
	mutex_lock(&cbuf_lock);
        transfer_size = count < cbuf_len ? count : cbuf_len;
        
        if (cbuf_begin + transfer_size > CBUF_SIZE) {
            if (copy_to_user (buf, cbuffer + cbuf_begin, CBUF_SIZE - cbuf_begin))
                return -EFAULT;
            if (copy_to_user (buf + (CBUF_SIZE - cbuf_begin), cbuffer, 
                        transfer_size - (CBUF_SIZE - cbuf_begin)))
                return -EFAULT;
        } else {
            if (copy_to_user (buf, cbuffer + cbuf_begin, transfer_size))
                return -EFAULT;
        }
       
        cbuf_len -= transfer_size;
        cbuf_begin = (cbuf_begin + transfer_size) % CBUF_SIZE;
	
	mutex_unlock(&cbuf_lock);
       

 
        flag = 0;
        wake_up_interruptible(&wq);
        
    return transfer_size;
}

static ssize_t
blackboard_write (struct file *file, const char __user *buf, size_t count,
	   loff_t *f_pos)
{
    size_t transfer_size;
    size_t total_transfer;
	

	return -EIO;

    total_transfer = count;

	printk("wr: cnt-%d\n", count);
    while (total_transfer) {
	printk("wr: tt-%d\n", total_transfer);
        if (cbuf_len == CBUF_SIZE) {
            wait_event_interruptible (wq, flag != 1);
        }
       
        transfer_size = total_transfer < CBUF_SIZE - cbuf_len ?
                total_transfer : CBUF_SIZE - cbuf_len;

        if (cbuf_end + transfer_size > CBUF_SIZE) {
            if (copy_from_user (cbuffer + cbuf_end, buf, CBUF_SIZE - cbuf_end))
                return -EFAULT;
            if (copy_from_user (cbuffer, buf + (CBUF_SIZE - cbuf_end), 
                        transfer_size - (CBUF_SIZE - cbuf_end)))
                return -EFAULT;

        } else {
            if (copy_from_user (cbuffer + cbuf_end, buf, transfer_size))
                return -EFAULT;
        }
	
	buf += transfer_size;
        cbuf_len += transfer_size;
        cbuf_end = (cbuf_end + transfer_size) % CBUF_SIZE;
        
        flag = 1;

        wake_up_interruptible (&wq);
	if (0==total_transfer)
            wait_event_interruptible (wq, flag != 1);
	printk("wr: ts-%d\n", transfer_size);
	total_transfer -= transfer_size;
    }

	printk("wr: end\n");

	
    return count;
}

static const struct file_operations 
blackboard_fops = {
	.owner = THIS_MODULE,
	.open = blackboard_open,
	.release = blackboard_release,
	.read = blackboard_read,
	.write = blackboard_write,
};

static int __init
blackboard_init (void)
{
    cbuf_begin = cbuf_end = cbuf_len = 0; 
   
	ts = kthread_run (thread, NULL, "saw signal thread");
 
    if (alloc_chrdev_region (&blackboard_dev, 0, blackboard_count, 
                DEVICE)) {
		return -ENODEV;
	}

	cdev_init (&blackboard_cdev, &blackboard_fops);

	if (cdev_add (&blackboard_cdev, blackboard_dev, blackboard_count)) {
	    unregister_chrdev_region (blackboard_dev, blackboard_count);
		return -ENODEV;
	}

    return 0;
}

static void __exit 
blackboard_exit (void)
{
	kthread_stop (ts);
	cdev_del (&blackboard_cdev);
	unregister_chrdev_region (blackboard_dev, blackboard_count);
}

module_init (blackboard_init);
module_exit (blackboard_exit);

MODULE_LICENSE ("GPL");
MODULE_DESCRIPTION ("cyclic buffer device.");
MODULE_AUTHOR ("student");

