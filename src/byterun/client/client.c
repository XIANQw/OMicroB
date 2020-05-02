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
int pid_w,pid_r;
char msg_w[BUFSIZ],msg_r[BUFSIZ];
pid_t server_pid=0;

void* fun_sender(void * arg){
    printf("write_processus create\n");
    printf("\n---------------------notify: 1(pressed button A), 2(clear button A), 3(press button B), 4(clear button B)\n");
    while(1){
        scanf("%s", msg_w);
        write(fd_w, msg_w, strlen(msg_w));          
        if(!strcmp("EOF", msg_w)) break;
    }
    printf("---------------------notify: client terminated write processus\n");
}

void* fun_lisener(void * arg){
    printf("read processus start\n");
    printf("\n---------------------notify: client can recieve msg\n");
    while(1){           
        if(read(fd_r,msg_r,BUFSIZ) == -1){
            perror("client recieve msg fail");
        }else{
            if( strlen(msg_r) > 0 ){
                printf("**************** client read: len=%ld\n%s\n", strlen(msg_r), msg_r);
                if (strcmp("EOF",msg_r) == 0){
                    printf("server quit, stop read\n");
                    break;
                }
            }else{
                printf("msg is empty\n");
            }   
        }
    }//while
    printf("---------------------notify：client terminated read processus\n");
}


void main(){
    //SERVER_W is client's lecteur，SERVER_R is client's write
    printf("Program start\n");
    // pipe_read
    if(access(SERVER_W,0) < 0){
        pid_r = mkfifo(SERVER_W,0700);
        if(pid_r < 0){
            perror("pipe_read create fail"); exit(0);
        }
        printf("pipe_read%s create\n",SERVER_W);
    }
    fd_r = open(SERVER_W,O_RDWR);
    if(fd_r < 0){
        perror("open pipe_read fail"); exit(0);
    }
    printf("open pipe_read\n");
 
    //pipe_write
    if(access(SERVER_R,0) < 0){
        pid_w = mkfifo(SERVER_R,0700);
        if(pid_w < 0){
            perror("client create pipe_write fail"); exit(0);
        }
        printf("pipe_write%s create\n",SERVER_R);
    }
    fd_w = open(SERVER_R,O_RDWR);
    if(fd_w < 0){
        perror("open pipe_write fail"); exit(0);
    }
    printf("open pipe_write\n");
    
    // get pid of server
    while(1){
        if( read(fd_r,msg_r,BUFSIZ) == -1 ){
            perror("client recieve msg fail"); exit(0);
        }
        if ((strlen(msg_r) > 3) && (msg_r[0]=='p') && (msg_r[1]='i') && (msg_r[2]=='d')){
            char pidch[10];
            strcpy(pidch, msg_r+3);
            server_pid = atoi(pidch);
            printf("server_pid=%d\n", server_pid);
            break;
        }    
    }
    printf("client start communication\n");

    pthread_t sender, lisener;
    if(pthread_create(&sender, NULL, (void*)&fun_sender, NULL)==-1){
        perror("sender create fail");
    }
    if(pthread_create(&lisener, NULL, (void*)&fun_lisener, NULL)==-1){
        perror("lisener create fail");
    }
    pthread_join(sender, NULL);
    pthread_join(lisener, NULL);
    
    close(fd_w);
    printf("close pipe_read\n");
    close(fd_r);
    printf("close pipe_write\n");
    printf("Bye\n");
    return;
}