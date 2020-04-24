#include <fcntl.h> //pid_t
#include <pthread.h> //pthread_t
#include <unistd.h> //read, write
#include<sys/stat.h> //mkfifo

#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
 
#define SERVER_W "/tmp/serverWrite"
#define SERVER_R "/tmp/serverRead"
 

int fd_w,fd_r;
// pipe_write, pipe_read
int pid_w,pid_r;
// IO buffer
char msg_w[BUFSIZ], msg_r[BUFSIZ];


void* fun_lisener(void * arg){
    printf("server: read processus start\n");
    printf("\n---------------------server can recieve msg\n");
    while(1){
        if(read(fd_r,msg_r,BUFSIZ)==-1){
            perror("server recieve msg fail");
        }else{
            if((strlen(msg_r) > 1) && (strcmp("EOF",msg_r) != 0)){
                printf("invalid instruction\n");
            } else if(strlen(msg_r)==1){
                switch (msg_r[0]){
                case '1':
                    printf("Button A is pressed\n"); break;
                case '2':
                    printf("Button A is cleared\n"); break;
                case '3':
                    printf("Button B is pressed\n"); break;
                case '4':
                    printf("Button B is cleared\n"); break;
                default:
                    printf("invalid instruction\n"); break;
                }
            } else if (strcmp("EOF",msg_r) == 0){
                printf("client quit, stop read\n"); break;   
            } else{
                printf("server: msg is empty\n");
            }
            printf("server finish read\n\n");    
        }
    }//while
    printf("---------------------notify：terminate read processus\n");
}

void* fun_sender(void * arg){
    //write processus
    printf("write processus starts\n");
    printf("\n---------------------notify：server can write msg\n");
    while(1){  
        scanf("%s",msg_w);
        if ( write(fd_w,msg_w,strlen(msg_w)+1) == -1){
            perror("server send msg fail");
        }else{
            printf("**************** server send :%s\n",msg_w);
            printf("server finish send\n\n");
            if (strcmp("EOF",msg_w) == 0){       
                break;
            }
        }
    }
    printf("---------------------notify：server terminate write processus\n");
}


void main() {
    printf("Program start\n");
    //pipe_write exist or not
    if(access(SERVER_W,0) < 0){
        printf("pipe_write%s doesn't exist\n",SERVER_W);
        return ;
    }
    //open pipe_write
    fd_w = open(SERVER_W,O_RDWR);
    if(fd_w < 0){
        perror("open pipe_write fail");
    }
    printf("open pipe_write\n");
 
    //pipe_read exist or not
    if(access(SERVER_R,0) < 0){
        printf("pipe_read%s doesn't exist\n",SERVER_R);
        return ;
    }
    //open pipe_read
    fd_r = open(SERVER_R,O_RDWR);
    if(fd_r < 0){
        perror("open pipe_read fail");
    }
    printf("open pipe_read\n");
    snprintf(msg_w, BUFSIZ, "pid%d", getpid());
    write(fd_w, msg_w, strlen(msg_w));
    printf("server: start communication\n");

    pthread_t sender, lisener;
    if(pthread_create(&sender, NULL, (void *)&fun_sender, NULL) == -1){
        perror("create sender fail");
    }
    if(pthread_create(&lisener, NULL, (void *)&fun_lisener, NULL) == -1){
        perror("create lisener fail");
    }
    pthread_join(sender, NULL);
    pthread_join(lisener, NULL);
 
    close(fd_w);
    printf("close pipe_write\n");
    close(fd_r);
    printf("close pipe_read\n");
    printf("Bye\n");
    return;
}