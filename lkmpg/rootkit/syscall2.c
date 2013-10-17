#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> 
#include <linux/errno.h> 
#include <linux/types.h>
#include <linux/unistd.h>
#include <asm/cacheflush.h>  
#include <asm/page.h>  
#include <asm/current.h>
#include <linux/sched.h>
#include <linux/kallsyms.h>
 
unsigned long *syscall_table = (unsigned long *)0xc15b5020; 
 
asmlinkage int (*original_write)(unsigned int, const char __user *, size_t);
 
asmlinkage int new_write(unsigned int fd, const char __user *buf, size_t count) {
 
    // hijacked write
 
    printk(KERN_ALERT "WRITE HIJACKED");
 
    return (*original_write)(fd, buf, count);
}
 
static int init(void) {
  printk(KERN_ALERT "\nHIJACK INIT\n");    
    write_cr0 (read_cr0 () & (~ 0x10000));
 
    original_write = (void *)syscall_table[__NR_write];
    syscall_table[__NR_write] = new_write;  
 
    write_cr0 (read_cr0 () | 0x10000);
 
    return 0;
}
 
static void exit(void) {
 
    write_cr0 (read_cr0 () & (~ 0x10000));
 
    syscall_table[__NR_write] = original_write;  
 
    write_cr0 (read_cr0 () | 0x10000);
     
    printk(KERN_ALERT "MODULE EXIT\n");
 
    return;
}
 
module_init(init);
module_exit(exit);
