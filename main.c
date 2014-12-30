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
	for (int i = 0, avg_writes = avg_read = max_writes = 0; i < NAND_PAGES_TOTAL; i++) {
		avg_write += pstat[i].write_counter;
		avg_read += pstat[i].read_counter;
		max_writes = fmax(max_writes, pstat[i].write_counter);
	}
	avg_write /= NAND_PAGES_TOTAL;
	avg_read /= NAND_PAGES_TOTAL;

	printf("\tAverage page reads:  %10.2lf\n", avg_read);
	printf("\tAverage page writes: %10.2lf\n", avg_write);
	printf("\tMaximum page writes: %10.2lf\n", max_writes);
}

void print_file(int fn) {
	printf("Opening file #%d\n", fn);
	struct file_desc *fd = dfs_open(fn);
	printf("File name: %s\n", fd->node->name);
	printf("Reading content byte by byte: ");
	for (int i = 0; i < 8; i++) {
		char buff[5]; buff[4] = 0;
		dfs_read(fd, buff, 1);
		printf("%c ", buff[0]);
	}
	printf("\n");
}

int main(int argc, char **argv) {
	printf("This is NAND-flash memory emulator.\n");
	printf(	"\tPage size: \t%d bytes\n"
		"\tBlock size: \t%d pages\n"
		"\tBlock count: \t%d blocks\n"
		"\tTotal: \t\t%d bytes\n",
		NAND_PAGE_SIZE, NAND_PAGES_PER_BLOCK,
		NAND_BLOCKS_TOTAL, NAND_SIZE_TOTAL);

	dfs_init();
	dfs_mount();

	
	print_file(0);
	
	struct file_desc *fd = dfs_open(0);
	dfs_write(fd, "lol", 3);

	print_file(0);

	fd = dfs_open(1);
	print_file(1);
	dfs_rename(fd, "journal");
	print_file(1);

	p_outwear_stat();

	printf("Bye!\n");
	return 0;
}
