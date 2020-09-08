#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 

int main(int argc, char *argv[]){
	if(argc != 3){
		fprintf(stderr, "Error: Incorrect Arguments"); 
		exit(1); 
	}
	if(strlen(argv[1]) != strlen(argv[2])){
		fprintf(stderr, "Error: From and To of different size");
		exit(1);
	}
        char *from = argv[1];
        char *to = argv[2];
	int map[256] = {0}; 
	size_t i = 0; 
	int x = 0; 
	for(; i < strlen(from); i++){
		if(map[from[i]] != 0){
			fprintf(stderr, "Error: Duplicates not allowed"); 
			exit(1); 
		} else {
			map[from[i]] = to[i]; 
		}
	}
	while((x = getchar()) != EOF) {
		if(ferror(stdin) != 0){
			fprintf(stderr, "Error: Failed due to: %d\n", ferror(stdin)); 
			exit(1); 
		}
		char op = (map[x] == 0) ? x : map[x]; 
		if(putchar(op) == EOF) {
			fprintf(stderr, "Error: Failed output");
			exit(1);  
		}
	}
	return 0; 
}

