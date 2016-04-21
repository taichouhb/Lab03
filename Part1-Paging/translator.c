//imports
#include <stdio.h>
#include <stdlib.h>

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
	
	char* buf = (char *) malloc(256);
	int offset, pn, index;
	unsigned long address, pn_address, offset_address;
	
	//read in each line
	while(fgets(buf, 256, fp)) {
		//if first, store offset
		if(index == 0) {
			offset = atoi(buf);
		//if second, store page number
		} else if(index == 1) {
			pn = atoi(buf);
		} else {
			//convert value to hex
			address = strtoul(buf, NULL, 16);
			//clear uncessary bits and move by offset
			pn_address = ((address<<48)>>48)>>offset;
			//clear uncecessary bits + page number and move by pn and unecessary bits
			offset_address = (address<<(48 + pn))>>(48 + pn);
			printf("virtual address 0x%X is in page number %lu and offset %lu\n", (unsigned int)address, pn_address, offset_address);
		}
		index++;
	}
	free(buf);
	fclose(fp);
}
