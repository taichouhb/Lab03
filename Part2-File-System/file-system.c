w#include <stdio.h>
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
	inode* inodes[16];
} super;

FILE* DISK;
super* sblock;


void write_super(){
	//move file pointer to the start of the disk
	rewind(DISK);
	//write in the current free blocks
	fwrite(sblock->freeblocks, 128, sizeof(char), DISK);
	//move to the area of the inodes of the super block
	fseek(DISK, 128, SEEK_SET);
	for(int i = 0; i < 16; i++) {
		//write each inode into disk, essentially updating it
		fwrite(sblock->inodes[i], sizeof(inode), sizeof(char), DISK);
	}
}

void delete_inode(inode *new_inode, int assigned_index) {
	if(assigned_index > -1) {
		sblock->inodes[assigned_index] = (inode *) malloc(sizeof(inode));
	}

	free(new_inode);
}

void create(char name[8], int32_t size) {
	inode *new_inode = (inode *) malloc(sizeof(inode));
	strncpy(new_inode->name, name, 8);
	new_inode->size = size;
	new_inode->used = 1;

	int bp_i = 0;
	for(int fb_i = 1; fb_i < 128; fb_i++) {
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

	int assigned_index = -1;
	for(int i = 0; i < 16; i++) {
		if(strcmp(sblock->inodes[i]->name, name) == 0) {
			printf("%s already exists\n", name);
			delete_inode(new_inode, assigned_index);
			return;
		} else if(sblock->inodes[i]->used == 0 && assigned_index == -1) {
			assigned_index = i;
			free(sblock->inodes[i]);
			sblock->inodes[i] = new_inode;
		}
	}

	if(assigned_index == -1) {
		printf("File storage full, cannot create: %s\n", name);
		delete_inode(new_inode, assigned_index);
		return;
	}

	for(int bp_i = 0; bp_i < 8; bp_i++) {
		sblock->freeblocks[new_inode->blockPointers[bp_i]] = 1;
	}

	write_super();
	printf("Created: %s\n", name);
}

void delete(char name[8]) {
	char *empty_buf = (char *) calloc(1024, sizeof(char)); 
	int32_t deleted_blocks[8];
	int del_block_pointer = 0;
	//loop through inodes, to find name match
	for(int i = 0; i < 16; i++) {
		//when match found...
		if(strcmp(name, sblock->inodes[i]->name) == 0) {
			//delete the contents of the data at the disk by overwriting
			inode* node = sblock->inodes[i];
			for(int j = 0; j < 8; j++){
				if(node->blockPointers[j] == 0) {
					continue;
				} else if(node->blockPointers[j] != 0) {
					//go to that block in data and delete it.
					deleted_blocks[del_block_pointer++] = node->blockPointers[j];
					int response = fseek(DISK, node->blockPointers[j] * 1024, SEEK_SET);
					//overwrite data at that point with an empty buffer
					fwrite(empty_buf, 1024, 1, DISK);
				}
			}
			//now clear the inode
			free(node);
			//now set the inodes array in sblock properly
			for(int k = 0; k < 8; k ++){
				sblock->freeblocks[deleted_blocks[k]] = 0;
			}
			write_super();
			//no need to loop through the rest of the inodes
			break;
		}
	}
}

void read(char name[8], int32_t blockNum, char buf[1024]) {
	//read the specified block from this file into the specified buffer; 
	//blockNum can range from 0 to 7
	inode* node;
	for(int i = 0; i < 16; i++){ //find node with the name that was passed in
		if(strcmp(sblock->inodes[i]->name,name) == 0){
			node = sblock->inodes[i];
			break;
		}
	}

	if(node == NULL){ //if node is not found
		printf("%s was not found\n", name);
	} else {
		if(!fseek(DISK, node->blockPointers[blockNum] * 1024, SEEK_SET)){ 
			//if seek was unsuccesful
			printf("pointer was not moved succesfully\n");
		} else {
			//read data that is being pointed to from the fseek call and write to buf
			fread(buf, 1024, sizeof(char), DISK);
		}
	}
}

void write(char name[8], int32_t blockNum, char buf[1024]) {
	for(int i = 0; i < 16 ; i++){
		//loop through all the inodes
		//if the inode matches the name, then write
		if(strcmp(name, sblock->inodes[i]->name) == 0) {
			//set file pointer to the correct position based on what was in blockPointer at the blockNum
			fseek(DISK, sblock->inodes[i]->blockPointers[blockNum] * 1024, SEEK_SET);
			//write to disk
			fwrite(buf, 1024, 1, DISK);
			break;
		}
	}
	
}

void ls() {
	for(int i = 0; i < 16; i++){ //go through each inode and print each name, if it has a file there
		if(sblock->inodes[i]->name != NULL){
			printf("%s ", sblock->inodes[i]->name);
		}
	}
	printf("\n");
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
	int index = 0;

	//read in each line
	while(fgets(buf, 256, fp)) {
		if(index == 0) {
			token = strtok(buf, "\n");
			if((DISK = fopen(token, "r+")) == NULL) {
				printf("Cannot open %s\n", buf);
				exit(1);
			}
			sblock = (super *) malloc(sizeof(super));
			fread(sblock->freeblocks, sizeof(char), 128, DISK);
			for(int i = 0; i < 16; i++) {
				sblock->inodes[i] = (inode *) malloc(sizeof(inode));
				fread(sblock->inodes[i], sizeof(inode), sizeof(char), DISK);
			}

		} else {
			int line_index = 0;
			while((token = strsep(&buf, " ")) != NULL) {
				if(strcmp(token,"") == 0) {
					continue;
				}
				token = strtok(token, "\n");
				line[line_index] = token;
				line_index++;
			}
			buf = (char *) malloc(256);

			if(strcmp(line[0],"C") == 0) {
				if(strlen(line[1]) > 7) {
					printf("%s is greater than 8 characters long\n", line[1]);
				}
				create(line[1], atoi(line[2]));
			} else if (strcmp(line[0],"D") == 0) {
				delete(line[1]);	
			} else if (strcmp(line[0],"L") == 0) {
				ls();
			} else if (strcmp(line[0],"R") == 0) {
				print_buf = (char *) calloc(1024, sizeof(char));
				read(line[1], atoi(line[2]), print_buf);
				printf("%s\n", print_buf);
				free(print_buf);
			} else if (strcmp(line[0],"W") == 0) {
				print_buf = (char *) calloc(1024, sizeof(char));
				write(line[1], atoi(line[2]), print_buf);
				printf("%s\n", print_buf);
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

