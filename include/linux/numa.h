#ifndef _LINUX_NUMA_H
#define _LINUX_NUMA_H

//在新的linux3.x~linux4.x的内核中，内核移除了pg_data_t的pgdat_next之指针域, 同时也删除了pgdat_list链表, 参见Remove pgdat list和Remove pgdat list ver.2
//
//但是定义了一个大小为MAX_NUMNODES类型为pg_data_t数组node_data,数组的大小根据CONFIG_NODES_SHIFT的配置决定. 对于UMA来说，NODES_SHIFT为0，所以MAX_NUMNODES的值为1.

//在UMA结构的机器中, 只有一个node结点即contig_page_data, 此时NODE_DATA直接指向了全局的contig_page_data, 而与node的编号nid无关
#ifdef CONFIG_NODES_SHIFT
#define NODES_SHIFT     CONFIG_NODES_SHIFT
#else
#define NODES_SHIFT     0
#endif

#define MAX_NUMNODES    (1 << NODES_SHIFT)

#define	NUMA_NO_NODE	(-1)

#endif /* _LINUX_NUMA_H */
