/* @author Denis Deryugin
 * @date 26 Dec 2014
 */

#include "nand.h"
#include "dfs.h"

#define DFS_VERBOSE

int dfs_read_superblock(void) {
	
}

int dfs_read_inodes(void) {
	
#ifdef DFS_VERBOSE
#endif
	return 0;
}

char *static_files[] = {"deadbeef", "cafebabe"};

int dfs_init(void) {
	int pt, file_pt;
	char buff[DFS_BUFF_SIZE];
	printf("Writing DFS image...\n");

	for (int i = 0; i < NAND_BLOCK_COUNT; i++)
		_block_erase(i);

	struct dfs_superblock sb = {	sizeof(sb),
					sizeof(static_files) / sizeof(static_files[0]) };
	memset(buff, 0, DFS_BUFF_SIZE);
	memcpy(buff, &sb, sizeof(sb));
	for (int i = 0; i < page_from_pos(sizeof(sb) + NAND_PAGE_SIZE - 1); i++)
		_program_page(i, buff + i * NAND_PAGE_SIZE);
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

