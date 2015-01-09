#include "dfs.h"

#ifdef DFS_FUSE
/* FUSE: Filesystem in Userspace */

#include <stdio.h>
#include <fcntl.h>

int dfs_fuse_getattr (const char * path, struct stat *buf) {
	printf("dfs_fuse_getattr\n");

	buf->st_mode = S_IFDIR | 0755;
	buf->st_nlink = 2;
	return 0;
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
	
	return 0;
}

struct fuse_operations dfs_fuse_oper = {
	.getattr	= dfs_fuse_getattr,
	.readdir	= dfs_fuse_readdir,
	.open		= dfs_fuse_open,
	.read		= dfs_fuse_read,
};

#endif
