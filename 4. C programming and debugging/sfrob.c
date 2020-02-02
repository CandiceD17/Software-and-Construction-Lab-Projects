#include <stdio.h>
#include <stdlib.h>

int frobcmp(char const* a, char const* b){
  for(;;a++, b++){
    if(*a == ' ' && *b == ' ')
      return 0;
    if(*a == ' ')
      return -1;
    if(*b == ' ')
      return 1;
    if((*a^42) == (*b^42))
      continue;
    else if((*a^42) < (*b^42))
      return -1;
    else
      //a greater than b
      return 1;
  }
  return 0;
}

void inputErr(){
  if(ferror(stdin)){
    fprintf(stderr, "Error in input");
    exit(1);
  }
}

int p_comp(const void* first, const void* sec){
  char const* a = *(char**) first;
  char const* b = *(char**) sec;
  return frobcmp(a, b);
}

int main() {
  //allocate 2-D array to store input
  char* word = (char*)malloc(sizeof(char));
  char** list = (char**)malloc(sizeof(char*));
  if( !word || !list){
    fprintf(stderr, "Error in memory allocation");
    exit(1);
  }
    
  char cur = getchar();
  if(ferror(stdin)){
      free(word);
      free(list);
      fprintf(stderr, "Error in input");
      exit(1);
  }
  //if empty file
  if(cur==EOF)
      exit(0);
  char next = getchar();
  if(ferror(stdin)){
      free(word);
      free(list);
      fprintf(stderr, "Error in input");
      exit(1);
  }
  
  int num_char = 0;
  int num_word = 0;
  long i;

  while(cur != EOF){
    word[num_char]=cur;
    num_char++;
    //allocate memory for next char
    word = realloc(word, sizeof(char)*(num_char+1));
    if(word==NULL){
        for(i=0; i<num_word; i++){
            free(list[i]);
        }
        free(list);
        fprintf(stderr, "Error in memory allocation");
        exit(1);
    }
    //if end of file
    if(next == EOF && cur == ' ')
      break;
        
    //if end of the word
    if(cur == ' '){
      list[num_word]=word;
      num_word++;
      list = (char**)realloc(list, sizeof(char*)*(num_word+1));
      if(list==NULL){
          for(i=0; i<num_word; i++){
              free(list[i]);
          }
          free(list);
          fprintf(stderr, "Error in memory allocation");
          exit(1);
      }
      //allocate a new memory to store a new word
      word=NULL;
      word=(char*)malloc(sizeof(char));
      if(word==NULL){
          for(i=0; i<num_word; i++){
              free(list[i]);
          }
          free(list);
          fprintf(stderr, "Error in memory allocation");
          exit(1);
      }
      num_char = 0;
    }
        
    //add ' ' for the last word
    if(next == EOF && cur != ' '){
      cur = ' ';
      continue;
    }
    //if consecutive space, then getchar until the next non-space character
    if(cur == ' ' && next == ' '){
      while(cur == ' '){
		cur = getchar();
        if(ferror(stdin)){
            for(i=0; i<num_word; i++){
                free(list[i]);
            }
            free(list);
            fprintf(stderr, "Error in input");
            exit(1);
        }
      }
      if(cur==EOF)
          break;
      next = getchar();
      if(ferror(stdin)){
        for(i=0; i<num_word; i++){
            free(list[i]);
        }
        free(list);
        fprintf(stderr, "Error in input");
        exit(1);
      }
      continue;
    }
        
    //change of parameter
    cur = next;
    next = getchar();
    if(ferror(stdin)){
        for(i=0; i<num_word; i++){
            free(list[i]);
        }
        free(list);
        fprintf(stderr, "Error in input");
        exit(1);
    }
  }
  if(num_char!=0){
    list[num_word]=word;
    num_word++;
  }
  
  qsort(list, num_word, sizeof(char*), p_comp);
    
  long j;
  for(i=0; i<num_word; i++){
    for(j=0;; j++){
      putchar(list[i][j]);
      if(list[i][j]==' ')
	break;
    }
  }

  for(i=0; i<num_word; i++){
    free(list[i]);
  }
  free(list);
    
  return 0;
}
