/* @author Denis Deryugin
 * @date 26 Dec 2014
 */

#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include "nand.h"

char raw_flash[DFS_SIZE];
struct dfs_block_stat bstat[DFS_BLOCK_COUNT];

/* NAND commands are taken from http://www.eng.umd.edu/~blj/CS-590.26/micron-tn2919.pdf */
int _read_page();
int _read_page_cache_sequential();
int _read_page_cache_sequential_last();
int _read();
int _random_data_read();
int _read_id(void);
int _read_status(void);

int _program_page(int pg, char *buff);
int _program_page_cache();
int _program();

int _random_data_input();
int _block_erase(int block_num);
int _reset(void);

/* Code 0xFF. Abort current operation. Must be used immediately  after power-up.
 * Could be performed when device is busy */
int _reset(void) {
	/* NIY */
	return -1;
}

/* Code 0xF0. Gives 5 bytes of device-specific information */
int _read_id(void) {
	/* NIY */
	return -1;
}

/* Code 0x70. 8 bit of status information.
 * Could be performed when device is busy */
int _read_status(void) {
	/* NIY */
	return -1;
}

/* Code 0x60. */
int _block_erase(int block_num) {
	if (block_num < 0 || block_num >= DFS_BLOCK_COUNT)
		return -1;

	bstat[block_num].erased = 1;
	memset(raw_by_page(block_num * DFS_BLOCK_SIZE), DFS_PAGE_SIZE, 1);
	return 0;
}

/* Code 0x80. */
int _program_page(int pg, char *buf) {
	int bnum = block_by_page(pg), i;

	if (!bstat[bnum].erased)
		fprintf(stderr, "[Warning] Writing to non-erased block\n");
	
	for (i = 0; i < DFS_PAGE_SIZE; i++)
		raw_flash[i] &= buf[i];

	return 0;
}

int dfs_init(void) {
	int i;
	printf("Initialization...\n");

	for (i = 0; i < DFS_BLOCK_COUNT; i++) {
		bstat[i].erased = 0;
	}
	return 0;
}

