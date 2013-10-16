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
 
unsigned long *syscall_table = (unsigned long *)0xc05d2180; 
 
void (*pages_rw)(struct page *page, int numpages) =  (void *) 0xc012fbb0;
void (*pages_ro)(struct page *page, int numpages) =  (void *) 0xc012fe80;
 
asmlinkage int (*original_write)(unsigned int, const char __user *, size_t);
 
asmlinkage int new_write(unsigned int fd, const char __user *buf, size_t count) {
 
    // hijacked write
 
    printk(KERN_ALERT "WRITE HIJACKED");
 
    return (*original_write)(fd, buf, count);
}
 
static int init(void) {
 
    struct page *sys_call_page_temp;
 
    printk(KERN_ALERT "\nHIJACK INIT\n");
 
    sys_call_page_temp = virt_to_page(&syscall_table);
    pages_rw(sys_call_page_temp, 1);
 
    original_write = (void *)syscall_table[__NR_write];
    syscall_table[__NR_write] = new_write;  
 
    return 0;
}
 
static void exit(void) {
 
    struct page *sys_call_page_temp;
    
    sys_call_page_temp = virt_to_page(syscall_table);
    syscall_table[__NR_write] = original_write;  
    pages_ro(sys_call_page_temp, 1);
     
    printk(KERN_ALERT "MODULE EXIT\n");
 
    return;
}
 
module_init(init);
module_exit(exit);
