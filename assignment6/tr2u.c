#include <stdlib.h> 
#include <string.h>
#include <unistd.h> 


int main(int argc, char *argv[]){
	if(argc != 3){
		write(2, "Error: Incorrect Arguments", 25); 
		exit(1); 
	}
	if(strlen(argv[1]) != strlen(argv[2])){
		write(2, "Error: From and To of different size", 25);
		exit(1);
	}
        char *from = argv[1];
        char *to = argv[2];
	int map[256] = {0}; 
	size_t i = 0; 
	char x[1]; 
	for(; i < strlen(from); i++){
		if(map[from[i]] != 0){
			write(2, "Error: Duplicates not allowed", 25); 
			exit(1); 
		} else {
			map[from[i]] = to[i]; 
		}
	}
	while(read(0,x,1)) {
		if(x < 0){
			write(2, "Error: Failed due to: %d\n", 25); 
			exit(1); 
		}
		char op = (map[x[0]] == 0) ? x[0] : map[x[0]]; 
		if(write(1, &op, 1) < 1) {
			write(2, "Error: Failed output", 25);
			exit(1);  
		}
	}
	return 0; 
}

