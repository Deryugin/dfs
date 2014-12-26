CC=gcc
CFLAGS=-c
SRC=main.c nand.c
OBJ=$(SRC:.c=.o)

all: $(SRC) nand_emu
clean:
	rm *.o && rm nand_emu
nand_emu : $(OBJ)
	$(CC) $(OBJ) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
