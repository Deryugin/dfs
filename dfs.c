/* @author Denis Deryugin
 * @date 26 Dec 2014
 */

#include "nand.h"
#include "dfs.h"

#define DFS_VERBOSE

static struct dfs_superblock dfs_sb;
static struct inode nodes[DFS_INODES_MAX];
char *file_names[] = {"config", "log"};
char *static_files[] = {"deadbeef", "cafebabe"};

static int dfs_read_block(int bk, void *buff) {
	for (int i = 0; i < NAND_PAGES_PER_BLOCK; i++)
		_read_page(page_from_block(bk) + i, buff + i * NAND_PAGE_SIZE);
}

static int dfs_write_block(int bk, void *buff) {
	for (int i = 0; i < NAND_PAGES_PER_BLOCK; i++)
		_program_page(page_from_block(bk) + i, buff + i * NAND_PAGE_SIZE);
}

static int dfs_write_raw(int pos, const void *buff, size_t size) {
	char bk_buf[NAND_BLOCK_SIZE];
	int src = 0;
	int pg = page_from_pos(pos) % NAND_PAGES_PER_BLOCK;
	int bk = block_from_pos(pos);

	do {
		dfs_read_block(bk, bk_buf);
		_block_erase(bk);
		
		for (pos %= NAND_BLOCK_SIZE; pos < NAND_BLOCK_SIZE && size; pos++, size--)
			bk_buf[pos] = ((const char*)buff)[src++];

		dfs_write_block(bk++, bk_buf);
		pg = pos = 0;
	} while (size != 0);
	
	return 0;
}

static int dfs_read_raw(int pos, void *buff, size_t size) {
	int dst = 0;
	int pg = page_from_pos(pos);
	char pg_buf[NAND_PAGE_SIZE];
	do {
		_read_page(pg++, (void *) pg_buf);

		for (pos %= NAND_PAGE_SIZE; pos < NAND_PAGE_SIZE && size; pos++, size--)
			((char *) buff)[dst++] = pg_buf[pos];
	} while (size != 0);

	return 0;
}

static int dfs_read_superblock(void) {
	return dfs_read_raw(0, (void *) &dfs_sb, sizeof(dfs_sb));
}

static int dfs_write_superblock(void) {
	return dfs_write_raw(0, (void *) &dfs_sb, sizeof(dfs_sb));
}

static int dfs_read_single_inode(int n) {
	return 0;
}

int dfs_read_inodes(void) {
	int pg = page_capacity(sizeof(dfs_sb));
	char buf[DFS_BUF_SIZE];

	for (int i = 0; i < dfs_sb.inode_count; i++) {
		dfs_read_raw(pos_from_page(pg), &nodes[i], sizeof(struct inode));
		pg += page_capacity(sizeof(struct inode));
		dfs_read_raw(pos_from_page(nodes[i].page_start), buf, nodes[i].len * NAND_PAGE_SIZE);
		buf[nodes[i].len * NAND_PAGE_SIZE] = '\0';
		printf("Inode #%d contains %s\n", nodes[i].num, buf);
		if (strcmp(buf, static_files[i]))
			printf("[Warning] File #%d content differs from the original file!\n", i);
	}
	return 0;
}

int dfs_init(void) {
	int node_pt, file_pt;
	char buf[DFS_BUF_SIZE];
	printf("Writing DFS image...\n");

	dfs_sb.sb_size = sizeof(dfs_sb);
	dfs_sb.inode_count = sizeof(static_files) / sizeof(static_files[0]);
	dfs_sb.max_inode_count = DFS_INODES_MAX;
	dfs_write_superblock();

	node_pt = page_capacity(sizeof(dfs_sb));
	file_pt = node_pt + dfs_sb.max_inode_count * page_capacity(sizeof(struct inode));
	for (int i = 0; i < dfs_sb.inode_count; i++) {
		printf("\tAdding file: %s\n", static_files[i]);

		int file_len = page_capacity(strlen(static_files[i]));
		struct inode node = { i, file_pt, file_len};
		strcpy(node.name, file_names[i]);

		dfs_write_raw(pos_from_page(node_pt), &node, sizeof(node));
		node_pt += page_capacity(sizeof(node));
		dfs_write_raw(pos_from_page(file_pt), static_files[i], 1 + strlen(static_files[i]));
		file_pt += page_capacity(strlen(static_files[i]) + 1);
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
	int pos = pos_from_page(fd->node->page_start) + fd->pos;
	
	dfs_write_raw(pos, buff, size);

	fd->pos += size;
	
	return 0;
}

int dfs_read(struct file_desc *fd, void *buff, size_t size) {
	int pos = fd->pos + pos_from_page(fd->node->page_start);
	char tmp[DFS_BUF_SIZE];
	
	dfs_read_raw(pos, buff, size);
	fd->pos += size;

	return 0;
}

int dfs_rename(struct file_desc *fd, const char *name) {
	char *bk_buf[NAND_BLOCK_SIZE];
	int node_pos = sizeof(dfs_sb) + sizeof(struct inode) * fd->node->num;
	int name_offset = (void *) fd->node->name - (void *) &(fd->node->num);
	int bk = block_from_pos(node_pos);
	
	dfs_write_raw(node_pos + name_offset, name, 1 + strlen(name));
	
	dfs_read_raw(node_pos, fd->node, sizeof(struct inode));

	return 0;
}

