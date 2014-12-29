/* @author Denis Deryugin
 * @date 26 Dec 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "nand.h"
#include "dfs.h"

void p_outwear_stat(void) {
	printf("Cells outwear stats:\n");
	double avg_write, avg_read, max_writes;
	for (int i = 0, avg_writes = avg_read = max_writes = 0; i < NAND_PAGE_COUNT; i++) {
		avg_write += pstat[i].write_counter;
		avg_read += pstat[i].read_counter;
		max_writes = fmax(max_writes, pstat[i].write_counter);
	}
	avg_write /= NAND_PAGE_COUNT;
	avg_read /= NAND_PAGE_COUNT;

	printf("\tAverage page reads:  %10.2lf\n", avg_read);
	printf("\tAverage page writes: %10.2lf\n", avg_write);
	printf("\tMaximum page writes: %10.2lf\n", max_writes);
}


int main(int argc, char **argv) {
	printf("This is NAND-flash memory emulator.\n");
	printf(	"\tPage size: \t%d bytes\n"
		"\tBlock size: \t%d pages\n"
		"\tBlock count: \t%d blocks\n"
		"\tTotal: \t\t%d bytes\n",
		NAND_PAGE_SIZE, NAND_BLOCK_SIZE, NAND_BLOCK_COUNT,
		NAND_BLOCK_COUNT * NAND_BLOCK_SIZE * NAND_PAGE_SIZE);

	dfs_init();

	p_outwear_stat();

	printf("Bye!\n");
	return 0;
}
