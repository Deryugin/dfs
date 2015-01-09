#include "dfs.h"

#ifdef DFS_FUSE
/* FUSE: Filesystem in Userspace */
#include <fuse.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

int dfs_fuse_getattr (const char * path, struct stat *buf) {
	printf("dfs_fuse_getattr\n");
	
	if (strcmp(path, "/") == 0) {
		buf->st_mode = S_IFDIR | 0755;
		buf->st_nlink = 2;
		return 0;
	} else {
		path++;
		for (int i = 0; i < DFS_INODES_MAX; i++) {
			struct file_desc *fd = dfs_open(i);
			if (!fd)
				break;
			if (strcmp(path, fd->node->name) == 0) {
				buf->st_mode = S_IFREG | 0444;
				buf->st_nlink = 2;
				buf->st_size = 1;
				return 0;
			}
		}
	}
	
	return -ENOENT;
}

int dfs_fuse_open (const char *path, struct fuse_file_info *fi) {
	printf("dfs_fuse_open\n");
	return 0;
}

int dfs_fuse_read (const char *path, char *buf, size_t suze,
			off_t offset, struct fuse_file_info *fi) {
	printf("dfs_fuse_read\n");
	return 0;
}

int dfs_fuse_readdir (const char *path, void *buf, fuse_fill_dir_t filler,
				off_t offset, struct fuse_file_info *fi) {
	printf("dfs_fuse_readdir\n");
	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	
	for (int i = 0; i < DFS_INODES_MAX; i++) {
		struct file_desc *fd = dfs_open(i);
		if (!fd)
			break;
		filler(buf, fd->node->name, NULL, 0);
	}

	return 0;
}

struct fuse_operations dfs_fuse_oper = {
	.getattr	= dfs_fuse_getattr,
	.readdir	= dfs_fuse_readdir,
	.open		= dfs_fuse_open,
	.read		= dfs_fuse_read,
};

#endif
