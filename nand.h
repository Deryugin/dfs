/* @author Denis Deryugin
 * @date 26 Dec 2014
 */

#ifndef _NAND_H_
#define _NAND_H_

#include <stdio.h>
#include <stdio.h>
#include <string.h>

#define DFS_PAGE_SIZE 512
#define DFS_BLOCK_SIZE 32
#define DFS_BLOCK_COUNT 8

#define DFS_SIZE (DFS_BLOCK_SIZE * DFS_BLOCK_COUNT / sizeof(char))

struct dfs_block_stat {
	int erased;
	int read_counter;
	int write_counter;
};

extern char raw_flash[DFS_SIZE];
extern struct dfs_block_stat bstat[DFS_BLOCK_COUNT];

/* TODO check ranges */

static inline char *raw_from_page(int pg) { return raw_flash + pg * DFS_PAGE_SIZE; }
static inline int page_from_raw(char *rw) { return (rw - raw_flash) / DFS_PAGE_SIZE; }

static inline int block_from_page(int pg) { return pg / DFS_BLOCK_SIZE; }
static inline int page_from_block(int bk) { return bk * DFS_BLOCK_SIZE; }

static inline int block_from_raw(char *rw) { return block_from_page(page_from_raw(rw)); }
static inline char * raw_from_block(int bk) { return raw_from_page(page_from_block(bk)); }

/* NAND commands are taken from http://www.eng.umd.edu/~blj/CS-590.26/micron-tn2919.pdf */
extern int _read_page(int pg, char *buff);
extern int _read_page_cache_sequential();
extern int _read_page_cache_sequential_last();
extern int _read();
extern int _random_data_read();
extern int _read_id(void);
extern int _read_status(void);

extern int _program_page(int pg, char *buff);
extern int _program_page_cache();
extern int _program();

extern int _random_data_input();
extern int _block_erase(int block_num);
extern int _reset(void);

extern int dfs_init(void);

#endif

