/* @author Denis Deryugin
 * @date 26 Dec 2014
 * 
 * NAND-based Dumb File System (DFS).
 */

#ifndef _DFS_H_
#define _DFS_H_

struct inode {
	int num;
	int page_start;
	int len;
};

struct dfs_superblock {
	int size;
	int inode_count;
};

extern int dfs_init(void);
extern int dfs_mount(void);
extern int dfs_read_superblock(void);
extern int dfs_write(int fd, void *buff, size_t size);
extern int dfs_read(int fd, void *buff, size_t size);
extern int dfs_create(struct inode *parent, struct inode *new_node);

#endif
