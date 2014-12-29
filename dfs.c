/* @author Denis Deryugin
 * @date 26 Dec 2014
 */

#include "nand.h"
#include "dfs.h"

#define DFS_VERBOSE

static struct dfs_superblock dfs_sb;

int dfs_read_superblock(void) {
	
}

int dfs_read_inodes(void) {
	
#ifdef DFS_VERBOSE
#endif
	return 0;
}

char *static_files[] = {"deadbeef", "cafebabe"};

int dfs_init(void) {
	int page_pt, file_pt;
	char buff[DFS_BUFF_SIZE];
	printf("Writing DFS image...\n");

	for (int i = 0; i < NAND_BLOCK_COUNT; i++)
		_block_erase(i);

	dfs_sb.sb_size = sizeof(dfs_sb);
	dfs_sb.inode_count = sizeof(static_files) / sizeof(static_files[0]);

	memset(buff, 0, DFS_BUFF_SIZE);
	memcpy(buff, &dfs_sb, sizeof(dfs_sb));
	page_pt = page_from_pos(sizeof(dfs_sb) + NAND_PAGE_SIZE - 1);
	for (int i = 0; i < page_pt; i++)
		_program_page(i, buff + i * NAND_PAGE_SIZE);
	
	file_pt = page_pt + dfs_sb.inode_count * (page_from_pos(sizeof(struct dfs_inode) + NAND_PAGE_SIZE - 1));
	for (int i = 0; i < dfs_sb.inode_count; i++) {
		printf("\tAdding file: %s\n", static_files[i]);

		int file_len = page_from_pos(strlen(static_files[i] + NAND_PAGE_SIZE - 1));
		struct dfs_inode node = { i, file_pt, file_len};
		memset(buff, 0, DFS_BUFF_SIZE);
		strcpy(buff, static_files[i]);
		for (int j = 0; j < file_len; j++)
			_program_page(file_pt + j, buff + j * NAND_PAGE_SIZE);
		file_pt += file_len;
	}
}

int dfs_mount(void) {
	int i;
	printf("Mounting Dumb File System...\n");

	printf("Reading superblock...\n");
	dfs_read_superblock();

	printf("Getting inodes\n");

	/* for (i = 0; i < DFS_BLOCK_COUNT; i++) {
		bstat[i].erased = 0;
	} */
	return 0;
}

