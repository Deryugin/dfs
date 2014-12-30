/* @author Denis Deryugin
 * @date 26 Dec 2014
 */

#include "nand.h"
#include "dfs.h"

#define DFS_VERBOSE

static struct dfs_superblock dfs_sb;
static struct dfs_inode nodes[DFS_INODES_MAX];
char *static_files[] = {"deadbeef", "cafebabe"};

int dfs_read_superblock(void) {
	char buf[DFS_BUF_SIZE];
	
	for (int i = 0; i < page_capacity(sizeof(dfs_sb)); i++) {
		_read_page(i, buf);
		memcpy(((char*) &dfs_sb) + i * NAND_PAGE_SIZE, buf, NAND_PAGE_SIZE);
	}
}

int dfs_read_inodes(void) {
	int pg = page_capacity(sizeof(dfs_sb));
	char buf[DFS_BUF_SIZE];

	for (int i = 0; i < dfs_sb.inode_count; i++) {
		for (int j = 0; j < page_capacity(sizeof(nodes[0])); j++) {
			_read_page(pg++, buf);
			memcpy((char*)&nodes[i] + j * NAND_PAGE_SIZE, buf, NAND_PAGE_SIZE);
		}
		for (int j = 0; j < nodes[i].len; j++) {
			_read_page(nodes[i].page_start + j, buf + j * NAND_PAGE_SIZE);
		}
		buf[nodes[i].len * NAND_PAGE_SIZE] = '\0';
		printf("Inode #%d contains %s\n", nodes[i].num, buf);
		if (strcmp(buf, static_files[i])) {
			printf("[Warning] File #%d content differs from the original file!\n", i);
		}
	}
	return 0;
}

int dfs_init(void) {
	int page_pt, file_pt;
	char buf[DFS_BUF_SIZE];
	printf("Writing DFS image...\n");

	for (int i = 0; i < NAND_BLOCKS_TOTAL; i++)
		_block_erase(i);

	dfs_sb.sb_size = sizeof(dfs_sb);
	dfs_sb.inode_count = sizeof(static_files) / sizeof(static_files[0]);

	memset(buf, 0, DFS_BUF_SIZE);
	memcpy(buf, &dfs_sb, sizeof(dfs_sb));
	page_pt = page_capacity(sizeof(dfs_sb));
	for (int i = 0; i < page_pt; i++)
		_program_page(i, buf + i * NAND_PAGE_SIZE);
	
	file_pt = page_pt + dfs_sb.inode_count * page_capacity(sizeof(struct dfs_inode));
	for (int i = 0; i < dfs_sb.inode_count; i++) {
		printf("\tAdding file: %s\n", static_files[i]);

		int file_len = page_capacity(strlen(static_files[i]));
		struct dfs_inode node = { i, file_pt, file_len};
		memset(buf, 0, DFS_BUF_SIZE);
		memcpy(buf, &node, sizeof(node));
		for (int j = 0; j < page_capacity(sizeof(node)); j++)
			_program_page(page_pt++, buf + j * NAND_PAGE_SIZE);
		memset(buf, 0, DFS_BUF_SIZE);
		strcpy(buf, static_files[i]);
		for (int j = 0; j < file_len; j++)
			_program_page(file_pt++, buf + j * NAND_PAGE_SIZE);
	}
}

int dfs_mount(void) {
	int i;
	printf("Mounting Dumb File System...\n");

	printf("Reading superblock...\n");
	dfs_read_superblock();
	printf("\tFile count: %d\n", dfs_sb.inode_count);
	printf("Getting inodes\n");
	dfs_read_inodes();

	return 0;
}

/*---------------------------------*\
 	File System Interface		 
\*---------------------------------*/

struct file_desc fds[DFS_INODES_MAX];
int fds_cnt = 0;
struct file_desc *dfs_open(int inode) {
	fds[fds_cnt].pos = 0;
	fds[fds_cnt].node = &nodes[inode];
	return &fds[fds_cnt++];
}

int dfs_write(struct file_desc *fd, void *buff, size_t size) {
	char bk_buf[NAND_BLOCK_SIZE];
	
	int pos = pos_from_page(fd->node->page_start) + fd->pos;
	int pg = page_from_pos(pos), bk = block_from_pos(pos);

	for (int i = pos; i < pos + size; i++) {
		if (i % NAND_BLOCK_SIZE == 0) {
			
		}
	}

	fd->pos += size;
	
	return 0;
}

int dfs_read(struct file_desc *fd, void *buff, size_t size) {
	int pos = fd->pos + pos_from_page(fd->node->page_start);
	char tmp[DFS_BUF_SIZE];
	
	for (int i = 0; i < page_capacity(size); i++)
		_read_page(i + page_from_pos(pos), tmp + i * NAND_PAGE_SIZE);
	
	memcpy(buff, tmp + (fd->pos % NAND_PAGE_SIZE), size);
	fd->pos += size;

	return 0;
}

int dfs_create(struct dfs_inode *parent, struct dfs_inode *new_node) {
	return 0;
}

