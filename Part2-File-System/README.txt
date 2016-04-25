Within our implementation, out file-system application will have a local copy of the super block for the filesystem called sblock. At all times, this sblock will be either current with the sblock in the file, or will be updated awaiting to be committed to the file. 

The local sblock is a C struct called super. This contains an array of char called freeblocks, which signals which blocks are free and which aren't. Then we have a pointer to an array of inodes within it as well. These inodes are the inodes that reference each file in out file system. 
The inode is C struct which has a name, a size, an array of blockPointers that this file uses and a number signifying how many blocks this inode uses. 

The main function of the file-system program will read in a file specified and do the specific operations depending on whats written on the file. These are the create, delete, write, read, and ls functions.

The create function will first allocate new space for an inode. Then it will copy over the name and the size into the new inode struct. 