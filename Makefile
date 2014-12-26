all: nand_emu

nand_emu:
	gcc main.c nand.c -o $@
