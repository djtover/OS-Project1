#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/fcntl.h>

#include "md5.h"


#define msgSize 20
#define md5Size 32
char *output[32];
void sigMd5(int sig){
    // signal(SIGINT,sigMd5);
    if(strlen(output) == 32){
        printf("encrypted by process %d : %s\n",getpid(),output);
    }
}


int main(){
    char input[msgSize];
    char preMd5[msgSize];
    char postMd5[md5Size];

    int pipe1[2],pipe2[2];
    int pid;

    if(pipe(pipe1)==-1 || pipe(pipe2)==-1 ){
        return 1;
    }

    pid = fork();
    printf("pid = %d",pid);
    if( pid == 0){
        read(pipe1[0], preMd5,msgSize);
        output = md5(preMd5);
        write(pipe2[1], postMd5,md5Size);
        pid_t parent = getppid();
        kill(parent,SIGINT);
    }
    else if(pid > 0){
        fgets(input,msgSize,stdin);
        write(pipe1[1],input,msgSize+1);
        read(pipe2[0],output,md5Size);
        exit(0);
    }
    else{
        return 1;
    }



    return 0;
}
