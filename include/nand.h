/* @author Denis Deryugin
 * @date 26 Dec 2014
 */

#ifndef _NAND_H_
#define _NAND_H_

#include <stdio.h>
#include <stdio.h>
#include <string.h>

/* Page is a minimal writable unit, while block is a minimal erasable unit.
 * Pages are being grouped into blocks */

#define NAND_PAGE_SIZE 4
#define NAND_PAGES_PER_BLOCK 4
#define NAND_BLOCK_SIZE (NAND_PAGE_SIZE * NAND_PAGES_PER_BLOCK)
#define NAND_BLOCKS_TOTAL 64
#define NAND_PAGES_TOTAL (NAND_BLOCKS_TOTAL * NAND_PAGES_PER_BLOCK)
#define NAND_SIZE_TOTAL (NAND_BLOCKS_TOTAL * NAND_BLOCK_SIZE / sizeof(char))

struct nand_page_stat {
	int read_counter;
	int write_counter;
	int erased;
};

struct nand_block_stat {
	int erase_counter;
};

extern unsigned char raw_flash[NAND_SIZE_TOTAL];
extern struct nand_page_stat pstat[NAND_PAGES_TOTAL];
extern struct nand_block_stat bstat[NAND_BLOCKS_TOTAL];


/* TODO check ranges */
static inline int page_capacity(int bytes) { return (bytes + NAND_PAGE_SIZE - 1) / NAND_PAGE_SIZE; }

static inline int pos_from_page(int pg) { return pg * NAND_PAGE_SIZE; }
static inline int page_from_pos(int pos) { return pos / NAND_PAGE_SIZE; }
static inline int pos_from_block(int block) { return block * NAND_BLOCK_SIZE; }
static inline int block_from_pos(int pos) { return pos / NAND_BLOCK_SIZE; }

static inline unsigned char *raw_from_page(int pg) { return raw_flash + pg * NAND_PAGE_SIZE; }
static inline int page_from_raw(unsigned char *rw) { return (rw - raw_flash) / NAND_PAGE_SIZE; }

static inline int block_from_page(int pg) { return pg / NAND_PAGES_PER_BLOCK; }
static inline int page_from_block(int bk) { return bk * NAND_PAGES_PER_BLOCK; }

static inline int block_from_raw(unsigned char *rw) { return block_from_page(page_from_raw(rw)); }
static inline unsigned char * raw_from_block(int bk) { return raw_from_page(page_from_block(bk)); }

/* NAND commands are taken from http://www.eng.umd.edu/~blj/CS-590.26/micron-tn2919.pdf */
extern int _read_page(int pg, char *buff);
extern int _read_page_cache_sequential();
extern int _read_page_cache_sequential_last();
extern int _read();
extern int _random_data_read();
extern int _read_id(void);
extern int _read_status(void);

extern int _program_page(int pg, unsigned char *buff);
extern int _program_page_cache();
extern int _program();

extern int _random_data_input();
extern int _block_erase(int block_num);
extern int _reset(void);

extern int dfs_init(void);

#endif

