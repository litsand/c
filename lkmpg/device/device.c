#include <linux/version.h>
#include <linux/module.h>
#include <linux/miscdevice.h>

int in_use = 0;

static int our_open(struct inode *inode, struct file *file)
{

	if(in_use)
		return -EBUSY;
	in_use++;
	printk("device has been opened\n");
	return 0;
}

static int our_release(struct inode *inode, struct file *file)
{

	in_use--;
	printk("device has been closed\n");
	return 0;
}

static int our_ioctl(struct file *file,unsigned int cmd,unsigned long arg)
{

	int retval = 0;
	return retval;

}

static const struct file_operations our_fops =\

{

	.owner = THIS_MODULE,
	.open = &our_open,
	.release = &our_release,
	.unlocked_ioctl = (void*)&our_ioctl,
	.compat_ioctl = (void*)&our_ioctl

}

static struct miscdevice our_device = \
{

	MISC_DYNAMIC_MINOR,
	"interceptor",
	&our_fops
}



static int __init init_module(void)
{

	printk(KERN_INFO "we are in kernel space\n");
	int retval = misc_register(&our_device);
	return retval;

}


static void __exit cleanup_module(void)

{

	printk(KERN_INFO "elvis has left the building\n");
	misc_deregister(&our_device);	
	return;

}

module_init(init_module);
module_exit(cleanup_module);

