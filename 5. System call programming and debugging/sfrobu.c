#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>

int f=0;

int frobcmp(char const* a, char const* b){
    for(;;a++, b++){
        if(*a == 0 && *b == 0)
            return 0;
        if(*a == 0)
            return -1;
        if(*b == 0)
            return 1;
        if(f==0){
            if((*a^42) == (*b^42))
                continue;
            else if((*a^42) < (*b^42))
                return -1;
            else
            //a greater than b
                return 1;
        }
        else {
            if((toupper(*a^42)) == toupper((*b^42)))
                continue;
            else if(toupper((*a^42)) < toupper((*b^42)))
                return -1;
            else
                //a greater than b
                return 1;
        }
    }
    return 0;
}

int p_comp(const void* first, const void* sec){
    char const* a = *(char**) first;
    char const* b = *(char**) sec;
    return frobcmp(a, b);
}

void clean(char** list, int size){
    int i;
    for(i=0; i<size; i++)
        free(list[i]);
    free(list);
}


int main(int argc, char** argv){
    if(argc>2){
        fprintf(stderr, "Cannot accept more than 1 argument. Error.");
        exit(1);
    }
    else if(argc==2){
        char* option = "-f";
        if(strcmp(argv[1],option)!=0){
            fprintf(stderr, "The only argument should be '-f'. Error.");
            exit(1);
        }
        //set the f-flag to true
        f = 1;
    }
    
    char** list;
    struct stat fs;
    if(fstat(STDIN_FILENO, &fs)<0){
        fprintf(stderr, "fstat Error.");
        exit(1);
    }
    int count = 0;
    int num_word=0;
    //flag for growing file
    int growing = 0;
    //if the input is a regular file
    if(S_ISREG(fs.st_mode)){
        char* buf = (char*)malloc(fs.st_size*sizeof(char));
        if(!buf){
            fprintf(stderr,"Error in memory allocation");
            exit(1);
        }
        int state = read(0,buf,fs.st_size);
        if(state>0){
            int i;
            //whether there is a word in this line
            int flag_word = 0;
            //allocate 2-d array
            for(i=0; i<fs.st_size;i++){
                if(flag_word && buf[i]==' '){
                    count++;
                    flag_word=0;
                }
                else if(!flag_word && buf[i]!=' ')
                    flag_word=1;
                if(flag_word && i==fs.st_size-1 && buf[i]!=' ')
                    count++;
            }
            list = (char**)malloc(count*sizeof(char*));
            if(!list){
                fprintf(stderr,"Error in memory allocation");
                exit(1);
            }
            //put words into the 2-d array
            int index=0, left=0, right=0;
            flag_word=0;
            char* store;
            while(right<fs.st_size){
                int count_space=0;
                for(;right<fs.st_size;right++){
                    if(buf[right]!=' ')
                        flag_word=1;
                    else if(flag_word && buf[right]==' '){
                        flag_word=0;
                        right++;
                        break;
                    }
                    //count consecutive space
                    else if(!flag_word && buf[right]==' ')
                        count_space++;
                }
                left+=count_space;
                int length = right-left;
                store=(char*)malloc(length*sizeof(char));
                if(!store){
                    clean(list,index);
                    fprintf(stderr, "Error in memory allocation");
                    exit(1);
                }
                int a;
                for(a=0; a<length; a++)
                    store[a]=buf[left+a];
                //add a trailing space for the last word
                if(buf[right-1]!=' '){
                    store=(char*)realloc(store, (length+1)*sizeof(char));
                    if(!store){
                        clean(list, index);
                        fprintf(stderr, "Error in memory allocation");
                        exit(1);
                    }
                    store[length]=' ';
                }
                list[index]=store;
                index++;
                left=right;
            }
            num_word=count;
            
            //check growing
            //sleep(10);
            lseek(0,0,SEEK_SET);
            int original = fs.st_size;
            if(fstat(0, &fs)<0){
                clean(list, count);
                fprintf(stderr, "fstat Error.");
                exit(1);
            }
            if(fs.st_size>original){
                growing=1;
                list = (char**)realloc(list, (count+1)*sizeof(char*));
                if(!list){
                    clean(list, count);
                    fprintf(stderr,"Error in memory allocation");
                    exit(1);
                }
                lseek(0, original, SEEK_SET);
            }
        }
    }
    else{
        //if not a file, intiialize
        list = (char**)malloc(sizeof(char*));
        if(!list){
            fprintf(stderr,"Error in memory allocation");
            exit(1);
        }
        growing=1;
    }
    
   
    if(growing){
    int num_char=0;
    char cur[1];
    int state_cur = read(0, cur, 1);
    if(state_cur<0){
        clean(list, num_word);
        fprintf(stderr, "Error in reading input.");
        exit(1);
    }
    char next[1];
    int state_next = read(0, next, 1);
    if(state_next<0){
        clean(list, num_word);
        fprintf(stderr, "Error in reading input.");
        exit(1);
    }
    char* word = (char*)malloc(sizeof(char));
    if(!word){
        clean(list, num_word);
        fprintf(stderr,"Error in memory allocation");
        exit(1);
    }
    
    while(state_cur>0){
        word[num_char]=cur[0];
        num_char++;
        word = realloc(word, sizeof(char)*(num_char+1));
        if(word==NULL){
            clean(list, num_word);
            fprintf(stderr, "Error in memory allocation");
            exit(1);
        }
        //if end of file
        if( state_next==0 && cur[0]==' ')
            break;
        
        //if end of the word
        if(cur[0] == ' '){
            list[num_word]=word;
            num_word++;
            list = (char**)realloc(list, sizeof(char*)*(num_word+1));
            if(list==NULL){
                clean(list, num_word);
                fprintf(stderr, "Error in memory allocation");
                exit(1);
            }
            word=NULL;
            word=(char*)malloc(sizeof(char));
            if(word==NULL){
                clean(list, num_word);
                fprintf(stderr, "Error in memory allocation");
                exit(1);
            }
            num_char = 0;
        }
        
        //add ' ' for the last word
        if(state_next==0 && cur[0] != ' '){
            cur[0] = ' ';
            continue;
        }
        //consecutive space
        if(cur[0]==' ' && next[0]==' '){
            while(cur[0]==' '){
                state_cur=read(0,cur,1);
                if(state_cur<0){
                    clean(list, num_word);
                    fprintf(stderr, "Error in reading input.");
                    exit(1);
                }
            }
            state_next=read(0,next,1);
            if(state_next<0){
                clean(list, num_word);
                fprintf(stderr, "Error in reading input.");
                exit(1);
            }
            continue;
        }
        
        cur[0]=next[0];
        state_cur = state_next;
        state_next = read(0, next, 1);
        if(state_next<0){
            clean(list, num_word);
            fprintf(stderr, "Error in reading input.");
            exit(1);
        }
    }
    if(word){
        list[num_word]=word;
        num_word++;
    }}
   
    qsort(list, num_word, sizeof(char*), p_comp);
    
    long i;
    long j;
    for(i=0; i<num_word; i++){
        for(j=0; ; j++){
            int state_write = write(1,&list[i][j],1);
            if(state_write<0){
                clean(list, num_word);
                fprintf(stderr, "Error in output.");
                exit(1);
            }
            if(list[i][j]==' '){
                break;
            }
        }
    }
    
    clean(list, num_word);
    
    return 0;
}
