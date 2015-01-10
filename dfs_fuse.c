/* FUSE: Filesystem in Userspace */
#include "dfs.h"

#include <fuse.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>


static struct file_desc* fd_from_path(const char *path) {
	for (int i = 0; i < DFS_INODES_MAX; i++) {
		struct file_desc *fd = dfs_open(i);
		if (!fd)
			break;
		if (strcmp(path, fd->node->name) == 0)
			return fd;
	}

	return NULL;
}

int dfs_fuse_getattr (const char * path, struct stat *buf) {
	memset(buf, 0, sizeof(struct stat));
	
	if (strcmp(path, "/") == 0) {
		buf->st_mode = S_IFDIR | 0644;
		buf->st_nlink = 2;
		return 0;
	} else {
		if (fd_from_path(path + 1)) {
			buf->st_mode = S_IFREG | 0666;
			buf->st_nlink = 1;
			buf->st_size = 8;
			return 0;
		}
	}
	
	return -ENOENT;
}

int dfs_fuse_open (const char *path, struct fuse_file_info *fi) {
	return 0;
}

int dfs_fuse_read (const char *path, char *buf, size_t size,
			off_t offset, struct fuse_file_info *fi) {
	return 0;
	struct file_desc *fd = fd_from_path(path + 1);
	fd->pos = offset;
	size_t len = fd->len;
	if (offset < len) {
		if (offset + size > len)
			size = len - offset;
		dfs_read(fd, buf, size);
	} else
		size = 0;

	return size;
}

int dfs_fuse_readdir (const char *path, void *buf, fuse_fill_dir_t filler,
				off_t offset, struct fuse_file_info *fi) {
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

int dfs_fuse_write(const char *path, const char *buf, size_t size,
			off_t offset, struct fuse_file_info *fi) {
	return 0;
	struct file_desc *fd = fd_from_path(path + 1);
	fd->pos = offset;
	size_t len = fd->len;
	if (offset < len) {
		if (offset + size > len)
			size -= len - offset;
		dfs_write(fd, (void *)buf, size);
	} else
		size = 0;
	return size;
}

struct fuse_operations dfs_fuse_oper = {
	.getattr	= dfs_fuse_getattr,
	.readdir	= dfs_fuse_readdir,
	.open		= dfs_fuse_open,
	.read		= dfs_fuse_read,
	.write		= dfs_fuse_write
};

