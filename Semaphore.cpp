#include "Semaphore.hpp" 
// define a value for locked mutex // 
#define ZEROSEM 0
// implement methods // 
Semaphore::Semaphore(): thread_min_watinig(0),max_cores(0),new_id(0){
	pthread_mutex_init(&sem_Mutex,nullptr);	
	pthread_cond_init(&sem_Cond,nullptr);
	}
// same but intialize with val instead of zero //
Semaphore::Semaphore(unsigned val) :thread_min_watinig(0), max_cores((unsigned long)val),new_id(0){
	pthread_mutex_init(&sem_Mutex,nullptr);	
	pthread_cond_init(&sem_Cond,nullptr);
		
	}
void Semaphore::down(){
	// lock mutex //
	pthread_mutex_lock(&sem_Mutex);
	unsigned long sem_id = new_id++;
	// wait until we unlocked // 
	while(sem_id > max_cores+thread_min_watinig || max_cores==ZEROSEM){
			pthread_cond_wait(&sem_Cond,&sem_Mutex);
		}
	// dec the counter // 
	max_cores--;
	thread_min_watinig++; // added // 
	// finally, unlock//
	pthread_mutex_unlock(&sem_Mutex);	
	}
void Semaphore::up(){
	//first lock the mutex //
	pthread_mutex_lock(&sem_Mutex);
	max_cores++; // increse the current counter //
	// ready for signal - use broadcast instead // 
	pthread_cond_broadcast(&sem_Cond);
	// unlock the mutex in the end // 
	pthread_mutex_unlock(&sem_Mutex);

	}	
Semaphore::~Semaphore(){
	// destroy cond and then the mutex //
	pthread_cond_destroy(&sem_Cond);
	 pthread_mutex_destroy(&sem_Mutex);
}	
