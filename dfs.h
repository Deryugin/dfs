/* @author Denis Deryugin
 * @date 26 Dec 2014
 * 
 * NAND-based Dumb File System (DFS).
 */

#ifndef _DFS_H_
#define _DFS_H_

#define DFS_BUF_SIZE 1024
#define DFS_INODES_MAX 4
#define DFS_NAME_MAX_LENGTH 8

struct inode {
	int num;
	int page_start;
	int len;
	int prev, next; /* Position of next inode */
	char name[DFS_NAME_MAX_LENGTH];
};

struct dfs_superblock {
	int sb_size; /* size of this structure */
	int inode_count;
	int max_inode_count;
};

extern int dfs_init(void);
extern int dfs_mount(void);

/* Inode interface */
void read_inode(int n, struct inode);
void write_inode(int n);
void delete_inode(int n);

/* Directory interface */
void link(); /* NIY */
void unlink(); /* NIY */
void mkdir(); /* NIY */
void rmdir(); /* NIY */
extern int dfs_rename(struct file_desc *fd, const char *name);

/* File interface */
struct file_desc {
	int pos;
	struct inode *node;
};

extern struct file_desc *dfs_open(int inode);
extern int dfs_write(struct file_desc *fd, void *buff, size_t size);
extern int dfs_read(struct file_desc *fd, void *buff, size_t size);
extern int dfs_create(struct inode *parent, struct inode *new_node);
void lseek(); /* NIY */
void mmap(); /* NIY */

#endif
