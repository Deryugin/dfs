# NAND-flash memory emulator + Dumb File System (DFS) #

Userspace NAND-flash memory emulator + NAND-based Dumb File System

# Emulator supports following commands #
* \_read\_page
* \_block\_erase
* \_program\_page

# Dumb File System #
Run ./nand_emu -h to see usage.

DFS doesn't support directories, creating, deleting and truncating files yet! However, reading and writing work fine.

Mounting with FUSE supported.
