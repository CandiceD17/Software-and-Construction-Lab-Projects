#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv){
    if(argc!=3){
        fprintf(stderr,"Incorrect number of input arguments");
        exit(1);
    }
    char* from = argv[1];
    long i;
    long j;
    long size = strlen(from);
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
    
    char buf[1];
    int temp = read(0, buf, 1);
    if(temp<0){
        fprintf(stderr, "Error in input.");
        exit(1);
    }
    while(temp>0){
        for(i=0; i<size; i++){
            if(buf[0]==from[i]){
                buf[0]=to[i];
                break;
            }
        }
        write(1, buf, 1);
        temp = read(0, buf, 1);
        if(temp<0){
            fprintf(stderr, "Error in input.");
            exit(1);
        }
    }
    return 0;
}
