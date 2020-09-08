#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <string.h>
#include <ctype.h>

int frobcmp(char const *a, char const *b, int casey) { 
        size_t i = 0; 
        int len_a, len_b; 
        int len_a2, len_b2;
        unsigned char x, y;  
        for(len_a2=0; a[len_a2] != ' '; len_a2++){}
        for(len_b2=0; b[len_b2] != ' '; len_b2++){}
    if(casey){
        for(; a[i] != ' ' && b[i] != ' '; i++){
                x = (unsigned char) (a[i] ^ 42); 
                y = (unsigned char) (b[i] ^ 42);
                if(casey == 1){
                        x = toupper((a[i] ^ 42)); 
                        y = toupper((b[i] ^ 42));
                }
                if(x != y)
                        return x-y; 
        }
    } else {
        for(; a[i] != ' ' && b[i] != ' '; i++){
                if((a[i] ^ 42) < (b[i] ^ 42)){
                        return -1; 
                }
                if((a[i] ^ 42) > (b[i] ^ 42)){
                        return 1; 
                }
        }
    }
        if(len_a2 < len_b2)
                return -1; 
        else if(len_a2 > len_b2)
                return 1; 
        return 0; 
}

int compare(void const * a, void const * b) {
  return frobcmp(*(char **)a, *(char **)b, 0);
}

int compare2(void const * a, void const * b) {
  return frobcmp(*(char **)a, *(char **)b, 1);
}
char ** extract(unsigned int *count, size_t *size) {
        char ** cast = (char **) malloc(sizeof(char*));
        char * word = (char *) malloc(sizeof(char));
        size_t cast_i = 0;
        size_t word_i = 0;
        *count = 0;
        *size = 0;
        if(cast == NULL || word == NULL){
                fprintf(stderr, "Error: Malloc\n");
                exit(1);
        }
        while(1){
                int x = getchar();
                if(ferror(stdin) != 0) {
                        fprintf(stderr, "Error: Input failed w/  %d\n", ferror(stdin));
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
/*
void extract2(struct stat fileData, char * file, char ** list, char * word, int i, int i2, int i3,
 int count, int header, char casey ) { 
	if(S_ISREG(fileData.st_mode)) {
        if(fileData.st_size > 0) {
                file = (char *) malloc(fileData.st_size * sizeof(char));
                      if(file == NULL) {
                        write(2, "Malloc failed", 25);
                        exit(1);
                        }
        if(read(0, file, fileData.st_size) < 0) {
                write(2, "Error: Input Failed", 25);
                exit(1);
        }
      for(size_t j = 0; j < fileData.st_size; j++) {
                if(file[j] == ' ')
                count++;
      }
      if(file[fileData.st_size - 1] != ' ')
                count++;
      list = (char **) malloc((count) * sizeof(char *));
      if(list == NULL) {
                write(2, "Error: Malloc Failed", 25);
                exit(1);
      }
      for(size_t j = 0; j < fileData.st_size; j++) {
                if(i == count - 1) {
                word = (char *) realloc(word, (i3 + 1) * sizeof(char));
                if(word == NULL) {
                        write(2, "Memory reallocation failed\n", 27);
                        exit(1);
                }
                word[i3] = file[j];
                if(file[j] == ' ') {
                        list[i] = file + header;
                        free(word);
                        i3 = 0;
                        i++;
                } else
                        i3++;
                }
                else if(file[j] == ' ') {
                        list[i] = file + header;
                                i++;
                        header = j + 1;
                        }
                }
                }
                } else {
                        list = (char **) malloc(1024 * sizeof(char*));
                        if(list == NULL) {
                        write(2, "Error: Malloc", 25);
                        exit(1);
                        }
                }
}
*/
void extract2(struct stat fileData, char * file, char ** list, char * word, int i, int i2, int i3,
 int count, int header, char casey ) {
                char c[1];
                while(read(0, c, 1)) {
                        if(c < 0) {
                        write(2, "Error: Input Failed", 25);
                        exit(1);
                }
                word = (char *) realloc(word, (i3 + 1) * sizeof(char));
                if(word == NULL) {
                        write(2, "Error: Realloc Failed", 25);
                        exit(1);
                }
                word[i3] = c[0];
                if(c[0] == ' ') {
                        list = (char **) realloc(list, (i + 1) * sizeof(char*));
                        if(list == NULL) {
                                write(2, "Error: Realloc Failed", 25);
                                exit(1);
                        }
                list[i] = word;
                if(i2 == -1)
                        i2 = i;
                i++;
                i3= 0;
                word = (char *) malloc(sizeof(char));
                if(word == NULL) {
                        write(2, "Error: Malloc Failed\n", 25);
                        exit(1);
                        }
                } else
                        i3++;
                }
                if(i3 > 0) {
                        word = (char *) realloc(word, (i3 + 1) * sizeof(char));
                        list = (char **) realloc(list, (i + 1) * sizeof(char*));
                if(word == NULL || list == NULL) {
                        write(2, "Error: Malloc Failed", 25);
                        exit(1);
                        }
                word[i3] = ' ';
                list[i] = word;
                if(i2 == -1)
                        i2 = i;
                i++;
                }
                count = i;
}
char ** printer2(struct stat fileData, char * file, char ** list, char * word, int i, int i2, int i3, 
int count, int header, char casey ){
                char x[1];
                while(read(0, x, 1)) {
                        if(x < 0) {
                        write(2, "Error: Input Failed", 25);
                        exit(1);
                }
                word = (char *) realloc(word, (i3 + 1) * sizeof(char));
                if(word == NULL) {
                        write(2, "Error: Realloc Failed", 25);
                        exit(1);
                }
                word[i3] = x[0];
                if(x[0] == ' ') {
                        list = (char **) realloc(list, (i + 1) * sizeof(char*));
                        if(list == NULL) {
                                write(2, "Error: Realloc Failed", 25);
                                exit(1);
                        }
                list[i] = word;
                if(i2 == -1)
                        i2 = i;
                i++;
                i3= 0;
                word = (char *) malloc(sizeof(char));
                if(word == NULL) {
                        write(2, "Error: Malloc Failed\n", 25);
                        exit(1);
                        }
                } else
                        i3++;
                }
                if(i3 > 0) {
                        word = (char *) realloc(word, (i3 + 1) * sizeof(char));
                        list = (char **) realloc(list, (i + 1) * sizeof(char*));
                if(word == NULL || list == NULL) {
                        write(2, "Error: Malloc Failed", 25);
                        exit(1);
                        }
                word[i3] = ' ';
                list[i] = word;
                if(i2 == -1)
                        i2 = i;
                i++;
                }
                count = i;
  
                char ** sorted = (char**) malloc((count) * sizeof(char *));
                if(sorted == NULL) {
                        write(2, "Malloc Failed", 25);
                        exit(1);
                        }
                for(size_t j = 0; j < count; j++) {
                        sorted[j] = list[j];
                }
                if(casey){
                        qsort(sorted, count, sizeof(char *), compare2);
                } else {
                        qsort(sorted, count, sizeof(char *), compare);
                }
                for(size_t j = 0; j < count; j++) {
                        for(size_t k = 0; sorted[j][k] != ' '; k++)
                                if(write(1, &sorted[j][k], 1) < 1) {
                                        write(2, "Error: Print Failed", 25);
                                        exit(1);
                                }
                                if(write(1, " ", 1) < 1) {
                                        write(2, "Error: Print Failed", 25);
                                        exit(1);
                                }
                        }	
		return sorted; 
}

void printer(unsigned int * count, size_t * size, char ** extraction){
        qsort(extraction, *count, sizeof(char *), compare);
        for(size_t word_i = 0; word_i < *count; word_i++) {
                for(size_t char_index = 0; extraction[word_i][char_index] != ' '; char_index++)
                        if(fprintf(stdout, "%c", extraction[word_i][char_index]) < 0) {
                                fprintf(stderr, "Error: Output Failure\n");
                                exit(1);
                        }
                if(fprintf(stdout, " ") < 0) {
                        fprintf(stderr, "Error: Output Failure\n");
                        exit(1);
                }
                free(extraction[word_i]);
        }
        free(count);
        free(size);
        free(extraction);
}
 
int main(int argc, char *argv[]) {  
/*	unsigned int * count = malloc(sizeof(unsigned int));
	size_t * size = malloc(sizeof(size_t));
	if(count == NULL || size == NULL) {
                fprintf(stderr, "Error: Malloc\n");
                exit(1);
        }
        char ** extraction = extract(count, size);
        printer(count, size, extraction);
*/
	struct stat fileData; 
	char * file = NULL;
	char ** list = NULL;
 	char * word = NULL;
  	size_t header = 0;
  	int i = 0;
  	int count = 0;
  	int i2 = -1;
  	int i3 = 0;
  	char casey = 0;
  	if(argc > 1) {
    	if(argv[1][0] == '-' && argv[1][1] == 'f' && argv[1][2] == '\0')
      		casey = 1;
    	else {
      		write(2, "Error: Invalid Args", 25);
      		exit(1);
    	     }
 	 }	 
	 if(fstat(0, &fileData) < 0) {
    		write(2, "Error: fstat error", 25);
    		exit(1);
  	}
        if(S_ISREG(fileData.st_mode)) {
        if(fileData.st_size > 0) {
                file = (char *) malloc(fileData.st_size * sizeof(char));
                      if(file == NULL) {
                        write(2, "Malloc failed", 25);
                        exit(1);
                        }
        if(read(0, file, fileData.st_size) < 0) {
                write(2, "Error: Input Failed", 25);
                exit(1);
        }
      for(size_t j = 0; j < fileData.st_size; j++) {
                if(file[j] == ' ')
                count++;
      }
      if(file[fileData.st_size - 1] != ' ')
                count++;
      list = (char **) malloc((count) * sizeof(char *));
      if(list == NULL) {
                write(2, "Error: Malloc Failed", 25);
                exit(1);
      }
      for(size_t j = 0; j < fileData.st_size; j++) {
                if(i == count - 1) {
                word = (char *) realloc(word, (i3 + 1) * sizeof(char));
                if(word == NULL) {
                        write(2, "Memory reallocation failed\n", 27);
                        exit(1);
                }
                word[i3] = file[j];
                if(file[j] == ' ') {
                        list[i] = file + header;
                        free(word);
                        i3 = 0;
                        i++;
                } else
                        i3++;
                }
                else if(file[j] == ' ') {
                        list[i] = file + header;
                                i++;
                        header = j + 1;
                        }
                }
                }
                } else {
                        list = (char **) malloc(1024 * sizeof(char*));
                        if(list == NULL) {
                        write(2, "Error: Malloc", 25);
                        exit(1);
                        }
                }
//	extract2(fileData, file, list, word, i, i2, i3, count, header, casey); 
	char ** sorted = printer2(fileData,  file, list, word, i, i2, i3, count, header, casey); 
 	if(S_ISREG(fileData.st_mode))
    		free(file);
  	if(i2 != -1) {
    		for(size_t j = i2; j < count; j++)
      			free(list[j]);
  		}
  	free(list);
  	free(sorted);
  	return 0;
}
  
