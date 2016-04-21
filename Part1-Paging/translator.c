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
		
	char* buf = (char *) malloc(sizeof(256));
	int offset, page_number, index;
	
	while(fgets(buf, sizeof(buf), fp)) {
		if(index == 0) {
			offset = atoi(buf);
		} else if(index == 1) {
			page_number = atoi(buf);
		} else {
			strsep(&buf, " ");	
		}
		index++;
	}

	fclose(fp);
}
