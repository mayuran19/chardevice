#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fs.h>
//#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define MAJOR_NUMBER 61

/* forward declaration */
int onebyte_open(struct inode *inode, struct file *filep);
int onebyte_release(struct inode *inode, struct file *filep);
ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos);
ssize_t onebyte_write(struct file *filep, const char *buf, size_t cont, loff_t *f_pos);
static void onebyte_exit(void);

/*definition of file_operation structure */
struct file_operations onebyte_fops = {
	read:		onebyte_read,
	write:		onebyte_write,
	open: 		onebyte_open,
	release:	onebyte_release
};

char *onebyte_data = NULL;
int size;

int onebyte_open(struct inode *inode, struct file *filep)
{
	return 0; //aways successful
}

int onebyte_release(struct inode *inode, struct file *filep)
{
	return 0; //alwasys successful
}

ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos)
{
	int error_count = 0;
	error_count = copy_to_user(buf, onebyte_data, 1);
	int tmp = size;
	size = 0;
	return tmp;
}

ssize_t onebyte_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos)
{
	
	//copy_from_user(onebyte_data, buf, sizeof(char));
	*onebyte_data = buf[0];
	size = 1;
	if(count > 1){
		return -ENOSPC;
	}else{
		return 1;
	}
}

static int onebyte_init(void)
{
	int result;
	// register the device
	printk(KERN_ALERT "mayuran registering module\n");
	result = register_chrdev(MAJOR_NUMBER, "onebyte", &onebyte_fops);
	if(result < 0){
		return result;
	}
	// allocate one byte of memory for storage
	// kmalloc is just like malloc, the second parameter is 
	// the type of memory to be allocated
	// to release the memory allocated by kmalloc, use kfree.
	onebyte_data = kmalloc(sizeof(char), GFP_KERNEL);
	if(!onebyte_data){
		onebyte_exit();
		// cannot allocate memory
		// return no memory error, negative signify a failure
		return -ENOMEM;
	}
	// initialize the value to be X
	*onebyte_data = 'X';
	size = 1;
	printk(KERN_ALERT "This is a onebyte device module\n");
	return 0;
}

static void onebyte_exit(void)
{
	// if the pointer is pointing to something
	if(onebyte_data){
		// free the memory and assign the pointer to NULL
		kfree(onebyte_data);
		onebyte_data = NULL;
	}
	// unregister the device
	unregister_chrdev(MAJOR_NUMBER, "onebyte");
	printk(KERN_ALERT "Onebyte device module is unloaded\n");
}

MODULE_LICENSE("GPL");
module_init(onebyte_init);
module_exit(onebyte_exit);
