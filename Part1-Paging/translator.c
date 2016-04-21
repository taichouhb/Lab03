#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	if(argc != 2) {
		printf("Filename argument required");
		exit(1);
	}

	FILE* fp;
	if((fp = fopen(argv[1], "r")) == NULL) {
		printf("Cannot open %s\n", argv[1]);
		exit(1);
	}
		
	char* buf = (char *) malloc(256);
	int offset, pn, index;
	unsigned long address, pn_address, offset_address;

	while(fgets(buf, 256, fp)) {
		if(index == 0) {
			offset = atoi(buf);
		} else if(index == 1) {
			pn = atoi(buf);
		} else {
			address = strtoul(buf, NULL, 16);
			pn_address = ((address<<48)>>48)>>offset;
			offset_address = (address<<(48 + pn))>>(48 + pn);
			printf("virtual address 0x%X is in page number %lu and offset %lu\n", (unsigned int)address, pn_address, offset_address);
		}
		index++;
	}

	fclose(fp);
}
