#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

FILE* DISK;
super* sblock;

void write_super() {
}

void create(char name[8], int32_t size) {
	inode *new_inode = (inode *) malloc(sizeof(inode));
	strncpy(new_inode->name, name, 8);
	new_inode->size = size;

	int bp_i = 0;
	for(int fb_i = 0; fb_i < 128; fb_i++) {
		if(sblock->freeblocks[fb_i] == 0) {
			new_inode->blockPointers[bp_i] = fb_i;
			if(++bp_i == size) {
				break;
			}
		}
	}

	if(bp_i != size) {
		printf("Not enough space to allocate %d blocks for %s\n", size, name);
		free(new_inode);
		return;
	}

	for(int bp_i = 0; bp_i < 8; bp_i++) {
		sblock->freeblocks[new_inode->blockPointers[bp_i]] = 1;
	}

	for(int i = 0; i < 16; i++) {
		if(&(sblock->inodes[i]) == NULL) {
			sblock->inodes[i] = *new_inode;
		}
	}

	write_super();
}

void delete(char name[8]) {

}

void read(char name[8], int32_t blockNum, char buf[1024]) {

}

void write(char name[8], int32_t blockNum, char buf[1024]) {

}

void ls() {

}

int main(int argc, char* argv[]) {
	//check if argument is given
	if(argc != 2) {
		printf("Filename argument required");
		exit(1);
	}

	//Try to open file
	FILE* fp;
	if((fp = fopen(argv[1], "r")) == NULL) {
		printf("Cannot open %s\n", argv[1]);
		exit(1);
	}

	char* token,* print_buf,* buf = (char *) malloc(256);
	char* line[3];
	int index;

	//read in each line
	while(fgets(buf, 256, fp)) {
		if(index == 0) {
			if((DISK = fopen(argv[1], "r+")) == NULL) {
				printf("Cannot open %s\n", buf);
				exit(1);
			}
			sblock = (super *) malloc(sizeof(super));
			fread(sblock->freeblocks, sizeof(char), 128, DISK);
			fread(sblock->inodes, sizeof(inode), 16, DISK);

		} else {
			int line_index = 0;
			while((token = strsep(&buf, " ")) != NULL) {
				line[line_index] = token;
				line_index++;
			}
			if(line[0] == "C") {
				//TODO filename size shit
				create(line[1], atoi(line[2]));	
			} else if (line[0] == "D") {
				delete(line[1]);	
			} else if (line[0] == "L") {
				ls();
			} else if (line[0] == "R") {
				print_buf = (char *) calloc(1024, sizeof(char));
				read(line[1], atoi(line[2]), print_buf);
				free(print_buf);
			} else if (line[0] == "W") {
				print_buf = (char *) calloc(1024, sizeof(char));
				write(line[1], atoi(line[2]), print_buf);
				free(print_buf);
			} else {
				printf("Unknown command %s\n", line[0]);
			}
		}
		index++;
	}
	free(buf);
	fclose(fp);
}
