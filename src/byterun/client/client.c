#include <fcntl.h> //pid_t
#include <pthread.h> //pthread_t
#include <unistd.h> //read, write
#include<sys/stat.h> //mkfifo
#include <sys/shm.h>
#include "gui.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
 
#define SERVER_W "/tmp/serverWrite"
#define SERVER_R "/tmp/serverRead"

int pid_w,pid_r;
char msg_w[BUFSIZ],msg_r[BUFSIZ];
pid_t server_pid=0;
struct shared_use_st *shw, *shr;
void *vshr, *vshw;

void* client_sender(void * arg){
    printf("write_processus create\n");
    printf("\n---------------------notify: 1(pressed button A), 2(clear button A), 3(press button B), 4(clear button B)\n");
    while(1){
        if(shw->written == 1) continue;
        scanf("%s", msg_w);
        printf("you send %s\n", msg_w);
        strcpy(shw->text, msg_w);          
        if(!strcmp("EOF", shw->text)) break;
        shw->written=1;
    }
    printf("---------------------notify: client terminated write processus\n");
}

void* client_lisener(void * arg){
    printf("read processus start\n");
    printf("\n---------------------notify: client can recieve msg\n");
    while(1){           
        if(shr->written==0) continue;
        if(strlen(shr->text) > 0 ){
            printf("**************** client read: len=%ld\n%s\n", strlen(shr->text), shr->text);
            if (strcmp("EOF",shr->text) == 0){
                printf("server quit, stop read\n");
                break;
            }
        }else{
            printf("msg is empty\n");
        }
        shr->written=0;   
    }
    printf("---------------------notify：client terminated read processus\n");
}


void main(){
    //SERVER_W is client's lecteur，SERVER_R is client's write
    printf("Program start\n");
    // shm of server_write
    int shrid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);
    if(shrid < 0){
      perror("shmget fail"); exit(1);
    }
    vshr = shmat(shrid, 0, 0);
    if(vshr < 0){
      perror("shmat failed"); exit(1);
    }
    shr=(struct shared_use_st*)vshr;
    printf("server_write: id=%d, memory attached at %X\n",shrid, vshr);

    // shm of server_read
    int shwid = shmget((key_t)1230, sizeof(struct shared_use_st), 0666|IPC_CREAT);
    if(shwid < 0){
      perror("shmget fail"); exit(1);
    }
    vshw = shmat(shwid, 0, 0);
    if(vshw < 0){
      perror("shmat failed"); exit(1);
    }
    shw=(struct shared_use_st*)vshw;
    printf("server_read: id=%d, memory attached at %X\n",shwid, vshw);

    pthread_t sender, lisener;
    if(pthread_create(&sender, NULL, (void*)&client_sender, NULL)==-1){
        perror("sender create fail");
    }
    if(pthread_create(&lisener, NULL, (void*)&client_lisener, NULL)==-1){
        perror("lisener create fail");
    }
    pthread_join(sender, NULL);
    pthread_join(lisener, NULL);
    
	//把共享内存从当前进程中分离
	if(shmdt(shr) == -1){
		fprintf(stderr, "shmdt failed\n"); exit(EXIT_FAILURE);
	}
	//删除共享内存
	if(shmctl(shrid, IPC_RMID, 0) == -1){
		fprintf(stderr, "shmctl(IPC_RMID) failed\n"); exit(EXIT_FAILURE);
	}
	if(shmdt(shw) == -1){
		fprintf(stderr, "shmdt failed\n"); exit(EXIT_FAILURE);
	}
	//删除共享内存
	if(shmctl(shwid, IPC_RMID, 0) == -1){
		fprintf(stderr, "shmctl(IPC_RMID) failed\n"); exit(EXIT_FAILURE);
	}
    return;
}