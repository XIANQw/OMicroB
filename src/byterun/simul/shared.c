
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


Env create_env(key_t id){
  Env env;
  int shmid = shmget(id, sizeof(struct shared_use_st), 0666|IPC_CREAT);
  if(shmid < 0){
    perror("shmget fail"); exit(1);
  }
  void * shm = shmat(shmid, 0, 0);
  if(shm < (void *)0){
    perror("shmat failed"); exit(1);
  }
  env=(Env)shm;
  env->shmid=shmid;
  return env;
}

void printEnv(Env shm_env){
  printf("nbpins=(%d,%d) nbleds=%d\n", shm_env->nb_pins_row, shm_env->nb_pins_col, shm_env->nb_leds);
  for(int i=0; i<shm_env->nb_leds; i++){
      printf("led[%d]->(pin%d, pin%d)\n", i, shm_env->leds[i][0], shm_env->leds[i][1]);
  }
  printf("nb_buttons=%d\n", shm_env->nb_buttons);
  for(int i=0; i<shm_env->nb_buttons; i++){
      printf("button[%d]->(%s, pin%d)\n", i, shm_env->buttons[i].label, shm_env->buttons[i].pins);
  }
}

struct shared_use_st* create_shm(key_t id){
    struct shared_use_st* shmdata;
    int shmid = shmget(id, sizeof(struct shared_use_st), 0666|IPC_CREAT);
    if(shmid < 0){
      perror("shmget fail"); exit(1);
    }
    void * shm = shmat(shmid, 0, 0);
    if(shm < (void *)0){
      perror("shmat failed"); exit(1);
    }
    shmdata=(struct shared_use_st*)shm;
    shmdata->shmid = shmid;
    shmdata->written = 0;
    pthread_mutexattr_init(&shmdata->mute_atr);
    pthread_mutexattr_setpshared(&shmdata->mute_atr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shmdata->mute, &shmdata->mute_atr);

    pthread_condattr_init(&shmdata->cond_atr);
    pthread_condattr_setpshared(&shmdata->cond_atr, PTHREAD_PROCESS_SHARED);
    pthread_cond_init(&shmdata->cond_r, &shmdata->cond_atr);
    pthread_cond_init(&shmdata->cond_w, &shmdata->cond_atr);
    
    printf("id=%d, memory attached at %p\n",shmid, shmdata);
    return shmdata;
}

void free_shm(struct shared_use_st* shmdata){
  int shmid=shmdata->shmid;
  if(shmdt(shmdata) == -1){
		perror("shmdt failed"); exit(1);
	}
	if(shmctl(shmid, IPC_RMID, 0) == -1){
		perror("shmctl(IPC_RMID) failed"); exit(1);
	}
}