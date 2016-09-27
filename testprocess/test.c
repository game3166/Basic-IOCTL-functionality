#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/stat.h>
#include <linux/sched.h>
#include <linux/jiffies.h>
#include<linux/time.h>
#include<linux/sched.h>
#include<linux/list.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/device.h>         
#include <linux/kernel.h>          
#include <linux/ioctl.h>
#include "procinfo.h"
#define  DEVICE_NAME "getprocinfo"    
#define  CLASS_NAME  "procinfo"        
 

static int majorNumber;                                             
static struct class*  procClass  = NULL; 
static struct device* procDevice = NULL; 
static int usr_pid;

struct ioc_arg 
{
	pid_t ipid;
	struct procinfo pinfo;
};
#define MAGIC_NO  'k'  //Magic number for the device ioctl command

static int getproc_open(struct inode *inode, struct file *file)
{
	return 0;
}


static int getproc_release(struct inode *inode, struct file *file)
{
	return 0;
}

//Kernel side IOCTL function
static int getproc_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret,size,direction;
    void __user *u_arg = (void __user *)arg;  /*getting pointer from user space pointer*/
   /* Wrapper structure for PID from user and the procinfo structure */
   struct ioc_arg ioc;
      printk (KERN_ALERT " In IOCTL command\n");
     struct task_struct *task1=NULL;
	struct list_head *list;
    	int count =0;
 
     /* Decoding the ioctl command */
     if (_IOC_TYPE (cmd) != MAGIC_NO)
    {
        printk (KERN_ALERT " got invalid case, CMD=%d\n", cmd);
        return -EINVAL;
    }
    
    direction = _IOC_DIR (cmd); 
    size = _IOC_SIZE (cmd);
    
    switch (direction) {

            /* IOCTL Write functionality - writing to the device */
	    case _IOC_WRITE:
	    printk(KERN_ALERT "Reading = %d bytes from user-space and writing to device\n",size);
	    ret = copy_from_user (&ioc, u_arg, size);
	    printk(KERN_ALERT "PID from user %d \n",ioc.ipid);
	    usr_pid = ioc.ipid;
	    return ret;
	    break;
    
            /* IOCTL Read functionality - Reading from the device */
	    case _IOC_READ:
	
		printk (KERN_ALERT "Reading device and writing = %d bytes to user-space\n", size);
                 printk(KERN_ALERT "USER PID %d \n",usr_pid);
		  /* Assigning the task_struct according to the PID */
	       if(usr_pid>0)
		{      
			printk(KERN_ALERT "Accessing task_struct of PID %d\n",cmd);
	      	       task1 = pid_task(find_vpid(usr_pid), PIDTYPE_PID);
		}
	 
	       if(usr_pid==0)
	       {  
			printk(KERN_ALERT "Accessing task_struct of current process\n");
			task1 = current;
	       }

	       if(usr_pid<0)
	       {
			printk(KERN_ALERT "Accessing task_struct of parent of current process\n");
	 		task1 = pid_task(find_vpid(current->parent->pid), PIDTYPE_PID);
		}
	 
	      /* Iterating through the list to count the number of siblings of a process */
	    list_for_each(list, &task1->sibling)
	   {
	     count++;
	   } 
	   ioc.pinfo.pid = task1->pid;
	   ioc.pinfo.ppid = task1->parent->pid;
	   ioc.pinfo.start_time = task1->start_time;
	   ioc.pinfo.num_sib = count-1;
           printk(KERN_ALERT "TASK INFO --> PID %d , PPID %d\n",task1->pid,task1->parent->pid);
           printk(KERN_ALERT "IOC INFO --> PID %d , PPID %d\n",ioc.pinfo.pid,ioc.pinfo.pid);
            /* Sending the wrapper structure to user space */
	   ret = copy_to_user(u_arg, &ioc, size); 
	   return ret;
	   break;

	default:
        printk (KERN_ALERT " got invalid case, CMD=%d\n", cmd);
        return -EINVAL;


  }
}

/* File operation structure */
static const struct file_operations getproc_fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = getproc_ioctl,
    .open = getproc_open,
    .release = getproc_release,
};

/* Refernce -> http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/ */
static int __init hello_init(void)
{
     majorNumber = register_chrdev(0, DEVICE_NAME, &getproc_fops);
   if (majorNumber<0){
      printk(KERN_ALERT "Getprocinfo failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "Getprocinfo: registered correctly with major number %d\n", majorNumber);
 
   // Register the device class
   procClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(procClass)){                // Check for error and clean if there is
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(procClass);          
   }
   printk(KERN_INFO "device class registered\n");
 
   // Register the device driver
   procDevice = device_create(procClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(procDevice)){               // Clean up if there is an error
      class_destroy(procClass);           
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Couldn't create the device\n");
      return PTR_ERR(procDevice);
   }
    printk(KERN_ALERT "In Kernel Space\n");
    return 0;  
}

static void __exit hello_exit(void)
{
   device_destroy(procClass, MKDEV(majorNumber, 0));     // remove device
   class_unregister(procClass);                          // unregister device class
   class_destroy(procClass);                             // remove device class
   unregister_chrdev(majorNumber, DEVICE_NAME);             // unregister major number 
   printk(KERN_ALERT "Goodbye \n");
}


module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");











