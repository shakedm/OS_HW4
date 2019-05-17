#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/utsname.h>
#include <linux/syscall.h>
#include <linux/sched.h>
//#include <stdlib.h>
#include <stdio.h>
#include <asm-i386/unistd.h>
// TODO: add more #include statements as necessary

MODULE_LICENSE("GPL");

// TODO: add command-line arguments
char* program_name;
void** sys_call_table = NULL;
asmlinkage long (*original_sys_kill)(int pid, int sig);


MODULE_PARM(program_name, "s");
// TODO: import original syscall and write new syscall
asmlinkage long our_sys_kill(int pid, int sig_num)
{
    task_t * the_task = find_task_by_pid(pid);
    if ((the_task) && (program_name) && (sig_num==SIGKILL) && (strcmp(find_task_by_pid(pid)->comm,program_name)==0))
    {
        return -EPERM;
    }
    else
    {
        return original_sys_kill(pid,sig_num);
    }
}

void find_sys_call_table(int scan_range) {
void** i =(void*) &system_utsname;
    for (;(*i)!=sys_read; i++) {
    }
    i-=__NR_read;
    sys_call_table =i;

}

int init_module(void) {
   // TODO: complete the function
    find_sys_call_table(200);
    original_sys_kill = sys_call_table[__NR_kill];
    sys_call_table[__NR_kill] =&our_sys_kill;
    return 0;
}

void cleanup_module(void) {
   // TODO: complete the function
    sys_call_table[__NR_kill]=original_sys_kill;
}

