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

void print_usage(void) {
	printf( "Flags:\n"
		"\t-h -- print this text\n"
		"\t-f path-to-mount-point -- mount with FUSE\n"
		"\t-t -- run some tests (default action)\n");
}

enum {MOUNT_FUSE, RUN_TESTS, PRINT_HELP} act = RUN_TESTS;
int main(int argc, char **argv) {
	printf("This is NAND-flash memory emulator.\n");
	int opt;
	while ((opt = getopt(argc, argv, "hf:t")) != -1) {
		switch (opt) {
		case 'h':
			print_usage();
			return 0;
		case 'f':
			act = MOUNT_FUSE;
			break;
		case 't':
			act = RUN_TESTS;
			break;
		}
	}

	printf(	"\tPage size: \t%d bytes\n"
		"\tBlock size: \t%d pages\n"
		"\tBlock count: \t%d blocks\n"
		"\tTotal: \t\t%d bytes\n",
		NAND_PAGE_SIZE, NAND_PAGES_PER_BLOCK,
		NAND_BLOCKS_TOTAL, NAND_SIZE_TOTAL);

	dfs_init();
	dfs_mount();

	for (int i = 0; i < DFS_INODES_MAX; i++) {
		struct file_desc *fd = dfs_open(i);
		if (!fd)
			break;
		print_file(i);
	}

	switch (act) {
	case MOUNT_FUSE:
#ifdef DFS_FUSE
		return fuse_main(argc - 1, argv + 1, &dfs_fuse_oper, NULL);
#else
		printf(	"FUSE is not supported,"
			"check configuration and rebuild!\n");
#endif
		break;
	case RUN_TESTS:
		print_file(0);
		
		struct file_desc *fd = dfs_open(0);
		dfs_write(fd, "lol", 3);

		print_file(0);

		fd = dfs_open(1);
		print_file(1);
		dfs_rename(fd, "journal");
		dfs_open(1);
		print_file(1);

		p_outwear_stat();
		break;
	}
	printf("Bye!\n");
	return 0;
}
