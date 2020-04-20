#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <signal.h>
#include <sys/types.h>

#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
 
#define SERVER_W "/tmp/serverWrite"
#define SERVER_R "/tmp/serverRead"
#define SIG_A 42
#define SIG_NO_A 43
#define SIG_B 44
#define SIG_NO_B 45


void main(){
    //SERVER_W is client's lecteur，SERVER_R is client's write
    printf("Program start\n");
    int fd_w,fd_r;
    int pid_w,pid_r;
    char msg_w[BUFSIZ],msg_r[BUFSIZ];
    pid_t pid, server_pid=0;
    // pipe_read
    if(access(SERVER_W,0) < 0){
        pid_r = mkfifo(SERVER_W,0700);
        if(pid_r < 0){
            perror("pipe_read create fail");
            return;
        }
         printf("pipe_read%s create\n",SERVER_W);
    }
    fd_r = open(SERVER_W,O_RDWR);
    if(fd_r < 0){
        perror("open pipe_read fail");
    }
    printf("open pipe_read\n");
 
    //pipe_write
    if(access(SERVER_R,0) < 0){
        pid_w = mkfifo(SERVER_R,0700);
        if(pid_w < 0){
            perror("client create pipe_write fail");
            return;
        }
        printf("pipe_write%s create\n",SERVER_R);
    }
    fd_w = open(SERVER_R,O_RDWR);
    if(fd_w < 0){
        perror("open pipe_write fail");
    }
    printf("open pipe_write\n");
    
    // get pid of server
    while(1){
        if( read(fd_r,msg_r,BUFSIZ) == -1 ){
            perror("client recieve msg fail");
        }else{
            if ((strlen(msg_r) > 3) && (msg_r[0]=='p') && (msg_r[1]='i') && (msg_r[2]=='d')){
                char pidch[10];
                strcpy(pidch, msg_r+3);
                server_pid = atoi(pidch);
                printf("server_pid=%d\n", server_pid);
                break;
            }
        }
    }
    printf("client start communication\n");

    pid = fork();
    if(pid < 0){
        perror("write_processus create fail\n");
    }else if(pid == 0){
        printf("write_processus create\n");
        printf("\n---------------------notify: 1(pressed button A), 2(clear button A), 3(press button B), 4(clear button B)\n");
        int ins=9;
        while(1){
            scanf("%d", &ins);
            switch (ins) {
            case 1:
                kill(server_pid, SIG_A); break;
            case 2:
                kill(server_pid, SIG_NO_A); break;
            case 3: 
                kill(server_pid, SIG_B); break;
            case 4:
                kill(server_pid, SIG_NO_B); break;
            default:
                printf("1(pressed button A), 2(clear button A), 3(press button B), 4(clear button B)\n"); break;
            }             
        }//while
        printf("---------------------notify: client terminated write processus\n");
        _exit(0);
    }else{
        printf("read processus start\n");
        printf("\n---------------------notify: client can recieve msg\n");
        while(1){           
            if( read(fd_r,msg_r,BUFSIZ) == -1 ){
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
        wait(NULL);
    }
    close(fd_w);
    printf("close pipe_read\n");
    close(fd_r);
    printf("close pipe_write\n");
    printf("Bye\n");
    return;
}