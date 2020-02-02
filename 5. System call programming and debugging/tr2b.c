#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
    if(argc!=3){
        fprintf(stderr,"Incorrect number of input arguments");
        exit(1);
    }
    char* from = argv[1];
    int i;
    int j;
    int size = strlen(from);
    //check if from contains duplicated char
    for(i=0; i<size; i++){
        for(j=i+1; j<size; j++){
            if(from[i]==from[j]){
                fprintf(stderr, "Repeating characters in first argument. Error.");
                exit(1);
            }
        }
    }
    //check if from and to have the same length
    char* to = argv[2];
    if(strlen(to)!=size){
        fprintf(stderr, "Unequal argument length. Error");
        exit(1);
    }
    
    int temp = getchar();
    if(ferror(stdin)){
        fprintf(stderr, "Error in input");
        exit(1);
    }
    while(temp != EOF){
        for(i=0; i<size; i++){
            if(temp == from[i]){
                temp=to[i];
                break;
            }
        }
        putchar(temp);
        temp = getchar();
        if(ferror(stdin)){
            fprintf(stderr, "Error in input");
            exit(1);
        }
    }
    return 0;
}

