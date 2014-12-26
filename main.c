#include <stdio.h>
#include "nand.h"

int main() {
	printf("This is NAND-flash memory emulator.\n");
	printf(	"\tPage size: \t%d bytes\n"
		"\tBlock size: \t%d pages\n"
		"\tBlock count: \t%d blocks\n"
		"\tTotal: \t\t%d bytes\n",
		DFS_PAGE_SIZE, DFS_BLOCK_SIZE, DFS_BLOCK_COUNT, DFS_BLOCK_COUNT * DFS_BLOCK_SIZE * DFS_PAGE_SIZE);
	dfs_init();

	return 0;
}
