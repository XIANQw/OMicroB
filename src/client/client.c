#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
 
#define SERVER_W "serverWrite"
#define SERVER_R "serverRead"
 
void main(){
    //客户端的读是服务端的写，服务端的写是客户端的读
    printf("Program start\n");
    int fd_w,fd_r;
    int pid_w,pid_r;
    char msg_w[BUFSIZ],msg_r[BUFSIZ];
    pid_t pid;
 
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
 
    printf("client start communication\n");
 
    pid = fork();
    if(pid < 0){
        perror("write_processus create fail\n");
    }else if(pid == 0){
        printf("write_processus create\n");
        printf("\n---------------------notify: 1(pressed button A), 2(clear button A), 3(press button B), 4(clear button B)\n");
        while(1){
            scanf("%s",msg_w);
            if (((strlen(msg_w) > 1) || (msg_w[0] > '4') || (msg_w[0] < '1')) && (strcmp("EOF",msg_w) != 0)) {
                printf("1(pressed button A), 2(clear button A), 3(press button B), 4(clear button B)\n");
                continue;
            }
            if ( write(fd_w,msg_w,strlen(msg_w)+1) == -1){
                perror("client send msg fail");
            }else{
                printf("****************client send: %s\n",msg_w);
                if (strcmp("EOF",msg_w) == 0){       
                    break;
                }
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
                    printf("**************** client read:\n%s\n",msg_r);
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