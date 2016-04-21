#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct va {
	unsigned address;
	int page;
	int offset;
}va;

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
		
	char* token,* split,* buf = (char *) malloc(256);
	
	int offset, page_number, index;
	
	while(fgets(buf, 256, fp)) {
		if(index == 0) {
			offset = atoi(buf);
		} else if(index == 1) {
			page_number = atoi(buf);
		} else {
			split = buf;
			while((token = strsep(&split, " ")) != NULL) {
				printf("%s\n", token);
			}
			free(token);
		}
		index++;
	}

	fclose(fp);
}
