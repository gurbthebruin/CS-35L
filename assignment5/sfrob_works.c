#include <stdlib.h>
#include <stdio.h> 
#include <string.h> 

int frobcmp(char const *a, char const *b);

int frobcmp(char const *a, char const *b){ 
	size_t i = 0; 
	int len_a, len_b; 
	int len_a2, len_b2; 
	for(len_a2=0; a[len_a2] != ' '; len_a2++){}
	for(len_b2=0; b[len_b2] != ' '; len_b2++){}
	for(; a[i] != ' ' && b[i] != ' '; i++){
		if((a[i] ^ 42) < (b[i] ^ 42)){
			return -1; 
		}
		if((a[i] ^ 42) > (b[i] ^ 42)){
			return 1; 
		}
	}
	if(len_a2 < len_b2)
		return -1; 
	else if(len_a2 > len_b2)
		return 1; 
	return 0; 
}

int compare(const void *a, const void *b) {
	return frobcmp(*(char**)(a), *(char**)(b)); 	
}

char ** extract(unsigned int *count, size_t *size) {
	char ** cast = (char **) malloc(sizeof(char*)); 
	char * word = (char *) malloc(sizeof(char)); 
	size_t cast_i=0; 
	size_t word_i=0; 
	*count = 0; 
	*size = 0; 
	if(cast == NULL || word == NULL){
		fprintf(stderr, "Error: Malloc\n"); 
		exit(1); 
	}
	while(1){
		int x = getchar(); 
		if(ferror(stdin) != 0) {
			fprintf(stderr, "Input failed with error code %d\n", ferror(stdin));
			exit(1);
			}
		if(feof(stdin))
			break; 
		word = (char *) realloc(word, (cast_i + 1) * sizeof(char)); 
		if(word == NULL){
			fprintf(stderr, "Error: Malloc\n"); 
			exit(1); 
		}
		word[word_i] = x; 
		if(x == ' ') {
			*size = (word_i + 1) > *size ? (word_i + 1) : *size; 
			cast = (char **) realloc(cast, (cast_i + 1) * sizeof(char *));
			if (cast == NULL) {
				fprintf(stderr, "Error: Malloc\n");
				exit(1); 	
		}			
		cast[cast_i] = word; 
		cast_i++; 
		word_i = 0; 
		word = (char *) malloc(sizeof(char)); 
	} else
		word_i++; 
	}
	if(word_i > 0) {
    	*size = (word_i + 1) >  *size ? (word_i + 1) : *size;
    	word = (char *) realloc(word, (word_i + 1) * sizeof(char));
    	cast = (char **) realloc(cast, (cast_i + 1) * sizeof(char*));
    	if(word == NULL || cast == NULL) {
      		fprintf(stderr, "Error: Mallocn");
		exit(1);
    	}
	word[word_i] = ' ';
	cast[cast_i] = word;
	cast_i++;
	}
	*count = cast_i;
	if(*count == 0)
	free(word);
	return cast;
}
	
int main(){
	unsigned int * count = malloc(sizeof(unsigned int));
	size_t * size = malloc(sizeof(size_t));
	if(count == NULL || size == NULL) {
		fprintf(stderr, "Error: Malloc\n");
    		exit(1);
  	}
  	char ** buf = extract(count, size); 
	qsort(buf, *count, sizeof(char *), compare);
	for(size_t word_i = 0; word_i < *count; word_i++) {
		for(size_t char_index = 0; buf[word_i][char_index] != ' '; char_index++)
			if(fprintf(stdout, "%c", buf[word_i][char_index]) < 0) {
				fprintf(stderr, "Error: Output Failure\n");
				exit(1);
			}
		if(fprintf(stdout, " ") < 0) {
			fprintf(stderr, "Error: Output Failure\n");
			exit(1); 
		}
		free(buf[word_i]);
	}
	free(count);
	free(size);
	free(buf);
	return 0; 			
}
