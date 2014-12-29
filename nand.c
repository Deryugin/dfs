/* @author Denis Deryugin
 * @date 26 Dec 2014
 */

#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include "nand.h"

char raw_flash[NAND_SIZE];
struct nand_page_stat pstat[NAND_PAGE_COUNT];
struct nand_block_stat bstat[NAND_BLOCK_COUNT];

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

/* Code 0x0030. Read one page to buff */
int _read_page(int pg, char *buff) {
	pstat[pg].read_counter++;
	memcpy(buff, raw_from_page(pg), NAND_PAGE_SIZE);
	return 0;
}

/* Code 0x60. */
int _block_erase(int block_num) {
	bstat[block_num].erase_counter++;
	memset(raw_from_page(block_num * NAND_BLOCK_SIZE), NAND_PAGE_SIZE, 1);
	for (int i = page_from_block(block_num); i < page_from_block(block_num + 1); i++)
		pstat[i].erased = 1;
	return 0;
}

/* Code 0x80. */
int _program_page(int pg, char *buf) {
	if (!pstat[pg].erased)
		fprintf(stderr, "[Warning] Writing to non-erased page\n");
	
	pstat[pg].write_counter++;

	for (int i = 0; i < NAND_PAGE_SIZE; i++)
		raw_flash[i] &= buf[i];

	return 0;
}

