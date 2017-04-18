#ifndef _LINUX_SHM_H_
#define _LINUX_SHM_H_

#include <linux/list.h>
#include <asm/page.h>
#include <uapi/linux/shm.h>
#include <asm/shmparam.h>
//该数据结构中最重要的部分就是shm_file这个字段。它指向了共享内存对应的文件。在该结构中有一个字段，f_mapping，

//它指向了该内存段使用的页面（物理内存）。同时，结构中，也包含一个字段，f_path，用于指向文件系统中的文件(dentry->inode)，

//这样就建立了物理内存和文件系统的桥梁。当进程需要创建或者attach共享内存的时候，在用户态，会先向虚拟内存系统申请各自的vma_struct，

//并将其插入到各自任务的红黑树中，该结构中有一个成员vm_file，它指向的就是struct file(shm_file)。这样虚拟内存、共享内存（文件系统）和物理内存就建立了连接。


struct shmid_kernel /* private to the kernel */
{	
	struct kern_ipc_perm	shm_perm;		 // 访问权限的信息
	struct file		*shm_file;			 	 // 指向虚拟文件系统的指针
	unsigned long		shm_nattch;			 // 有多少个进程attach上了这个共享内存段
	unsigned long		shm_segsz;			 // 共享内存段大小
	time_t			shm_atim;
	time_t			shm_dtim;
	time_t			shm_ctim;
	pid_t			shm_cprid;
	pid_t			shm_lprid;
	struct user_struct	*mlock_user;

	/* The task created the shm object.  NULL if the task is dead. */
	struct task_struct	*shm_creator;
	struct list_head	shm_clist;	/* list by creator */
};

/* shm_mode upper byte flags */
#define	SHM_DEST	01000	/* segment will be destroyed on last detach */
#define SHM_LOCKED      02000   /* segment will not be swapped */
#define SHM_HUGETLB     04000   /* segment will use huge TLB pages */
#define SHM_NORESERVE   010000  /* don't check for reservations */

/* Bits [26:31] are reserved */

/*
 * When SHM_HUGETLB is set bits [26:31] encode the log2 of the huge page size.
 * This gives us 6 bits, which is enough until someone invents 128 bit address
 * spaces.
 *
 * Assume these are all power of twos.
 * When 0 use the default page size.
 */
#define SHM_HUGE_SHIFT  26
#define SHM_HUGE_MASK   0x3f
#define SHM_HUGE_2MB    (21 << SHM_HUGE_SHIFT)
#define SHM_HUGE_1GB    (30 << SHM_HUGE_SHIFT)

#ifdef CONFIG_SYSVIPC
struct sysv_shm {
	struct list_head shm_clist;
};

long do_shmat(int shmid, char __user *shmaddr, int shmflg, unsigned long *addr,
	      unsigned long shmlba);
bool is_file_shm_hugepages(struct file *file);
void exit_shm(struct task_struct *task);
#define shm_init_task(task) INIT_LIST_HEAD(&(task)->sysvshm.shm_clist)
#else
struct sysv_shm {
	/* empty */
};

static inline long do_shmat(int shmid, char __user *shmaddr,
			    int shmflg, unsigned long *addr,
			    unsigned long shmlba)
{
	return -ENOSYS;
}
static inline bool is_file_shm_hugepages(struct file *file)
{
	return false;
}
static inline void exit_shm(struct task_struct *task)
{
}
static inline void shm_init_task(struct task_struct *task)
{
}
#endif

#endif /* _LINUX_SHM_H_ */
