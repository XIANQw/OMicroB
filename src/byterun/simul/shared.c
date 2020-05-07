
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "shared.h"

int create_sem(int init){
  int sem = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
  if(sem == -1){
    fprintf(stderr, "Error: semget()\n");
    exit(1);
  }
  if(semctl(sem, 0, SETVAL, init) == -1){
    fprintf(stderr, "Error: semctl()\n");
    exit(1);
  }
  return sem;
}

void destroy_sem(int sem){
  if(semctl(sem, 0, IPC_RMID, 0) == -1){
    fprintf(stderr, "Error: semctl()\n");
    exit(1);
  }
}

void *alloc_shm(size_t size){
  if(size == 0){
    return NULL;
  }else{
    int id = shmget(IPC_PRIVATE, size, IPC_CREAT | 0600);
    void *res;
    if(id == -1){
      fprintf(stderr, "Error: shmget()\n");
      exit(1);
    }
    res = shmat(id, NULL, 0);
    if(res == NULL){
      fprintf(stderr, "Error: shmat()\n");
      exit(1);
    }
    if(shmctl(id, IPC_RMID, NULL) == -1) fprintf(stderr, "Error: shmctl()\n");
    return res;
  }
}

void P(int sem){
  struct sembuf sops;
  sops.sem_num = 0;
  sops.sem_op = -1;
  sops.sem_flg = 0;
  if(semop(sem, &sops, 1) == -1){
    fprintf(stderr, "Error: semop()\n");
    exit(1);
  }
}

void V(int sem){
  struct sembuf sops;
  sops.sem_num = 0;
  sops.sem_op = 1;
  sops.sem_flg = 0;
  if(semop(sem, &sops, 1) == -1){
    fprintf(stderr, "Error: semop()\n");
    exit(1);
  }
}

struct shared_use_st* create_shm(key_t id){
    struct shared_use_st* shmdata;
    int shmid = shmget(id, sizeof(struct shared_use_st), 0666|IPC_CREAT);
    if(shmid < 0){
      perror("shmget fail"); exit(1);
    }
    void * shm = shmat(shmid, 0, 0);
    if(shm < 0){
      perror("shmat failed"); exit(1);
    }
    shmdata=(struct shared_use_st*)shm;
    shmdata->shmid = shmid;
    shmdata->written = 0;
    pthread_mutex_init(&shmdata->mute, NULL);
    pthread_cond_init(&shmdata->cond_r, NULL);
    pthread_cond_init(&shmdata->cond_w, NULL);
    printf("id=%d, memory attached at %X\n",shmid, shmdata);
    return shmdata;
}

void free_shm(struct shared_use_st* shmdata){
    if(shmdt(shmdata) == -1){
		perror("shmdt failed"); exit(1);
	}
	if(shmctl(shmdata->shmid, IPC_RMID, 0) == -1){
		perror("shmctl(IPC_RMID) failed"); exit(1);
	}
}