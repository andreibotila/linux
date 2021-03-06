/*
 * SO2 lab3 - task 3
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

MODULE_DESCRIPTION("Memory processing");
MODULE_AUTHOR("SO2");
MODULE_LICENSE("GPL");

struct task_info {
	pid_t pid;
	unsigned long timestamp;
};

static struct task_info *ti1, *ti2, *ti3, *ti4;

static struct task_info *task_info_alloc(int pid)
{
	struct task_info *ti;

	/* allocated and initialize a task_info struct */
	ti = kmalloc(sizeof(*ti), GFP_KERNEL);
	if (ti == NULL) {
		return NULL;
	}

	ti->pid = pid;
	ti->timestamp = jiffies;

	return ti;
}

static int memory_init(void)
{
	/* call task_info_alloc for current pid */
	ti1 = task_info_alloc(current->pid);	

	/* call task_info_alloc for parent PID */
	ti2 = task_info_alloc(current->real_parent->pid);

	/* call task_info alloc for next process PID */
	ti3 = task_info_alloc(next_task(current)->pid);

	/* call text_info_alloc for next process of the next process */
	ti4 = task_info_alloc(next_task(next_task(current))->pid);

	return 0;
}

static void memory_exit(void)
{

	/* print ti* field values */
	printk(KERN_INFO "ti1 pid = %d, timestamp = %lu", ti1->pid, ti1->timestamp);
	printk(KERN_INFO "ti2 pid = %d, timestamp = %lu", ti2->pid, ti2->timestamp);
	printk(KERN_INFO "ti3 pid = %d, timestamp = %lu", ti3->pid, ti3->timestamp);
	printk(KERN_INFO "ti4 pid = %d, timestamp = %lu", ti4->pid, ti4->timestamp);	

	/* free ti* structures */
	kfree(ti1);
	kfree(ti2);
	kfree(ti3);
	kfree(ti4);
}

module_init(memory_init);
module_exit(memory_exit);
