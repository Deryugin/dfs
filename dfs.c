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
	printf("Creating DFS image...\n");
	memset(raw_flash, 0, NAND_SIZE);
	struct dfs_superblock sb = { NAND_SIZE, sizeof(static_files) };
#ifdef DFS_VERBOSE
	printf("\tFile count: %d\n", sb.inode_count);
#endif
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

