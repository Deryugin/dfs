/* @author Denis Deryugin
 * @date 26 Dec 2014
 * 
 * NAND-based Dumb File System (DFS).
 */

#ifndef _DFS_H_
#define _DFS_H_

#define DFS_BUFF_SIZE 512

struct dfs_inode {
	int num;
	int page_start;
	int len;
};

struct dfs_superblock {
	int sb_size; /* size of this structure */
	int inode_count;
};

extern int dfs_init(void);
extern int dfs_mount(void);
extern int dfs_read_superblock(void);
extern int dfs_write(int fd, void *buff, size_t size);
extern int dfs_read(int fd, void *buff, size_t size);
extern int dfs_create(struct dfs_inode *parent, struct dfs_inode *new_node);

#endif
