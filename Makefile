CC=gcc
CFLAGS=-c -std=c99 -lm -g
LDFLAGS=-lm
SRC=main.c nand.c dfs.c
OBJ=$(SRC:.c=.o)

all: $(SRC) nand_emu
clean:
	rm *.o && rm nand_emu
nand_emu : $(OBJ)
	$(CC) $(OBJ) -o $@ -lm

.c.o:
	$(CC) $(CFLAGS) $< -o $@
