/* @author Denis Deryugin
 * @date 26 Dec 2014
 * 
 * NAND-based Dumb File System (DFS).
 */

#ifndef _DFS_H_
#define _DFS_H_

#define DFS_BUF_SIZE 1024
#define DFS_INODES_MAX 128
#define DFS_NAME_MAX_LENGTH 8

struct dfs_inode {
	int num;
	int page_start;
	int len;
	char name[DFS_NAME_MAX_LENGTH];
};

struct dfs_superblock {
	int sb_size; /* size of this structure */
	int inode_count;
};

extern int dfs_init(void);
extern int dfs_mount(void);
extern int dfs_read_superblock(void);

/* File system interface */
struct file_desc {
	int pos;
	struct dfs_inode *node;
};

extern struct file_desc *dfs_open(int inode);
extern int dfs_write(struct file_desc *fd, void *buff, size_t size);
extern int dfs_read(struct file_desc *fd, void *buff, size_t size);
extern int dfs_create(struct dfs_inode *parent, struct dfs_inode *new_node);
extern int dfs_rename(struct file_desc *fd, const char *name);

#endif
