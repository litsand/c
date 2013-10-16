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
 
 
static int init(void) {
 
    list_del_init(&__this_module.list);
 
    return 0;
}
 
static void exit(void) {
 
    return;
}
 
module_init(init);
module_exit(exit);
