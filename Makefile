CC=gcc
FUSE_FLAGS=`pkg-config fuse --cflags --libs`
CFLAGS=-c -std=c99 -lm -g -Iinclude $(FUSE_FLAGS)
LDFLAGS=-lm $(FUSE_FLAGS)
SRC=main.c nand.c dfs.c dfs_fuse.c
OBJ=$(SRC:.c=.o)

all: $(SRC) nand_emu
clean:
	rm *.o && rm nand_emu
nand_emu : $(OBJ)
	$(CC) $(OBJ) -o $@ -lm $(FUSE_FLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@
