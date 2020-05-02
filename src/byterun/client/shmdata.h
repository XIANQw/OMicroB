#ifndef SHMDATA_H
#define SHMDATA_H
#include <sys/shm.h>

#define TEXT_SZ 2048
 
struct shared_use_st{	
	int shmid;
	int written;
	char text[TEXT_SZ];
};

void p_lock();
void v_unlock();

struct shared_use_st* create_shm(key_t id);
void free_shm(struct shared_use_st* shmdata);

#endif