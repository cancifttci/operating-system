//Can CIFTCI - 270201080
//TESTED ON LINUX 4.17.4 KERNEL
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched/task.h>
#include <linux/sched/signal.h>
#include <linux/sched/mm.h>
#include <linux/mm.h>
#include <linux/stat.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE1(get_memory_usage, int, pid)  //takes one argument which is pid
{
    int retVal = 0;				// return value
    struct task_struct *task;		//task structure that contain information that we need about process
    struct mm_struct *mm;			//mm struct that contain information that we need about process' memory info. All memmory information is inside this struct
    unsigned long temp;		// variable that will store total memory usage amount of a process

    for_each_process(task) {			
        if(task->pid == pid) {				// we reach all processes with for_each_process. if pid=task->pid than we get necessary info for process that we search for 
            mm = get_task_mm(task);		// getting memory struct from task struct
            if (mm){	
                temp = get_mm_rss(mm) << PAGE_SHIFT;	//total allocated pages times page size. this will get us to the total bytes for current process
                mmput(mm);		//mmput gets rid of the mappings and all user-spac
		retVal = temp;			// assigning return value
                printk(KERN_INFO "Process PID : %d VmRSS = %lu bytes\n", task->pid, temp); //print to the log
            }
        }
    }
    return retVal;
}