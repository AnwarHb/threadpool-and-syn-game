#include "Semaphore.hpp" 
// define a value for locked mutex // 
#define ZEROSEM 0
// implement methods // 
Semaphore::Semaphore(): startingVal(0),currentVal(0){
	pthread_cond_init(&sem_Cond,nullptr);
	// define attribute // 
	pthread_mutexattr_t attrs;
	pthread_mutexattr_init(&attrs);
	// set type of attribute - ERRORCHECK //
	//  This type of mutex provides error checking.
	// A thread attempting to relock this mutex without first unlocking the mutex returns an error. //
	// A thread attempting to unlock a mutex that another thread has locked returns an error. //
	 //A thread attempting to unlock an unlocked mutex returns an error.//
	pthread_mutexattr_settype(&attrs,PTHREAD_MUTEX_ERRORCHECK);
	// now intialize mutex // 
	pthread_mutex_init(&sem_Mutex,&attrs);	
	}
// same but intialize with val instead of zero //
Semaphore::Semaphore(unsigned val) :startingVal(val),currentVal(val){
	pthread_cond_init(&sem_Cond,nullptr);
	pthread_mutexattr_t attrs;
	pthread_mutexattr_init(&attrs);
	pthread_mutexattr_settype(&attrs,PTHREAD_MUTEX_ERRORCHECK);
	pthread_mutex_init(&sem_Mutex,&attrs);	
	
	}
void Semaphore::down(){
	// lock mutex //
	pthread_mutex_lock(&sem_Mutex);
	// wait until we unlocked // 
	while(currentVal<=ZEROSEM){
			pthread_cond_wait(&sem_Cond,&sem_Mutex);
		}
	// dec the counter // 
	currentVal--;
	// finally, unlock//
	pthread_mutex_unlock(&sem_Mutex);	
	}
void Semaphore::up(){
	//first lock the mutex //
	pthread_mutex_lock(&sem_Mutex);
	currentVal++; // increse the current counter //
	// ready for signal // 
	pthread_cond_signal(&sem_Cond);
	// unlock the mutex in the end // 
	pthread_mutex_unlock(&sem_Mutex);

	}	
Semaphore::~Semaphore(){
	// destroy cond and then the mutex //
	pthread_cond_destroy(&sem_Cond);
	 pthread_mutex_destroy(&sem_Mutex);
}	
