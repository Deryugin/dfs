/* @author Denis Deryugin
 * @date 26 Dec 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "nand.h"
#include "dfs.h"

int main(int argc, char **argv) {
	printf("This is NAND-flash memory emulator.\n");
	printf(	"\tPage size: \t%d bytes\n"
		"\tBlock size: \t%d pages\n"
		"\tBlock count: \t%d blocks\n"
		"\tTotal: \t\t%d bytes\n",
		DFS_PAGE_SIZE, DFS_BLOCK_SIZE, DFS_BLOCK_COUNT,
		DFS_BLOCK_COUNT * DFS_BLOCK_SIZE * DFS_PAGE_SIZE);

	dfs_init();

	printf("Bye!\n");
	return 0;
}
