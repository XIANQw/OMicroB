/*************************************************************************/
/*                                                                       */
/*                                OCaPIC                                 */
/*                                                                       */
/*                             Benoit Vaugon                             */
/*                                                                       */
/*    This file is distributed under the terms of the CeCILL license.    */
/*    See file ../../LICENSE-en.                                         */
/*                                                                       */
/*************************************************************************/

#ifndef SHARED_H
#define SHARED_H

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


int  create_sem(int init);
void destroy_sem(int sem);
void *alloc_shm(size_t size);
void P(int sem);
void V(int sem);



#endif
