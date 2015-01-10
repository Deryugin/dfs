/* @author Denis Deryugin
 * @date 26 Dec 2014
 * 
 * NAND-based Dumb File System (DFS).
 */

#ifndef _DFS_H_
#define _DFS_H_

#define DFS_FUSE

#ifdef DFS_FUSE
#define FUSE_USE_VERSION 26 
#include <fuse/fuse.h>
extern struct fuse_operations dfs_fuse_oper;
#endif

#define DFS_BUF_SIZE 1024
#define DFS_INODES_MAX 4
#define DFS_NAME_MAX_LENGTH 8

#include <stdlib.h>

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

struct file_desc {
	int pos;
	size_t len;
	struct inode *node;
};

extern int dfs_init(void);
extern int dfs_mount(void);

/* Inode interface */
void dfs_read_inode(int n, struct inode);
void dfs_write_inode(int n);
void dfs_delete_inode(int n);

/* Directory interface */
void dfs_link(); /* NIY */
void dfs_unlink(); /* NIY */
void dfs_mkdir(); /* NIY */
void dfs_rmdir(); /* NIY */
extern int dfs_rename(struct file_desc *fd, const char *name);

/* File interface */
extern struct file_desc *dfs_open(int inode);
extern int dfs_write(struct file_desc *fd, void *buff, size_t size);
extern int dfs_read(struct file_desc *fd, void *buff, size_t size);
extern int dfs_create(struct inode *parent, struct inode *new_node);
extern void dfs_lseek(); /* NIY */
extern void dfs_mmap(); /* NIY */

#endif
