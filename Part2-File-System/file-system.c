#include <stdio.h>
#include <stdint.h>

typedef struct inode {
	char name[8];
	int32_t size;
	int32_t blockPointers[8];
	int32_t used;
} inode;

typedef struct super{
	char freeblocks[128];
	inode inodes[16];
} super;

FILE* open(char* name) {
	return fopen(name, "r+");
}

void create(char name[8], int32_t size) {
}

int main(int argc, char* argv[]) {

	if(argv[0] == NULL)
		{
			printf("ERROR IN USAGE. INPUT FILE NAME.\n");
			exit(1);
		}	

}

