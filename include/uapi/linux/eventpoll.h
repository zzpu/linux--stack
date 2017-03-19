/*
 *  include/linux/eventpoll.h ( Efficient event polling implementation )
 *  Copyright (C) 2001,...,2006	 Davide Libenzi
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Davide Libenzi <davidel@xmailserver.org>
 *
 */

#ifndef _UAPI_LINUX_EVENTPOLL_H
#define _UAPI_LINUX_EVENTPOLL_H

/* For O_CLOEXEC */
#include <linux/fcntl.h>
#include <linux/types.h>

/* Flags for epoll_create1.  */
#define EPOLL_CLOEXEC O_CLOEXEC

/* Valid opcodes to issue to sys_epoll_ctl() */
#define EPOLL_CTL_ADD 1
#define EPOLL_CTL_DEL 2
#define EPOLL_CTL_MOD 3

/* Set exclusive wakeup mode for the target file descriptor */
#define EPOLLEXCLUSIVE (1 << 28)

/*
 * Request the handling of system wakeup events so as to prevent system suspends
 * from happening while those events are being processed.
 *
 * Assuming neither EPOLLET nor EPOLLONESHOT is set, system suspends will not be
 * re-allowed until epoll_wait is called again after consuming the wakeup
 * event(s).
 *
 * Requires CAP_BLOCK_SUSPEND
 */
#define EPOLLWAKEUP (1 << 29)

/* Set the One Shot behaviour for the target file descriptor */
#define EPOLLONESHOT (1 << 30)

/* Set the Edge Triggered behaviour for the target file descriptor */
#define EPOLLET (1 << 31)

/* 
 * On x86-64 make the 64bit structure have the same alignment as the
 * 32bit structure. This makes 32bit emulation easier.
 *
 * UML/x86_64 needs the same packing as x86_64
 */
#ifdef __x86_64__
#define EPOLL_PACKED __attribute__((packed))
#else
#define EPOLL_PACKED
#endif
// typedef union epoll_data {
//	   void *ptr;
//	   int fd;
//	   __uint32_t u32;
//	   __uint64_t u64;
// } epoll_data_t;

//events可以是以下几个宏的集合： 
//EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）； 
//EPOLLOUT：表示对应的文件描述符可以写； 
//EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）； 
//EPOLLERR：表示对应的文件描述符发生错误； 
//EPOLLHUP：表示对应的文件描述符被挂断； 
//EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。 
//EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里



//感兴趣的事件和被触发的事件
struct epoll_event {
	__u32 events;
	__u64 data;
} EPOLL_PACKED;



#ifdef CONFIG_PM_SLEEP
static inline void ep_take_care_of_epollwakeup(struct epoll_event *epev)
{
	if ((epev->events & EPOLLWAKEUP) && !capable(CAP_BLOCK_SUSPEND))
		epev->events &= ~EPOLLWAKEUP;
}
#else
static inline void ep_take_care_of_epollwakeup(struct epoll_event *epev)
{
	epev->events &= ~EPOLLWAKEUP;
}
#endif
#endif /* _UAPI_LINUX_EVENTPOLL_H */
