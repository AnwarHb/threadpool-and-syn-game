#ifndef _QUEUEL_H
#define _QUEUEL_H

#include "Headers.hpp"
#include "Semaphore.hpp"

// implement here - template // 
// Single Producer - Multiple Consumer queue
template <typename T>
class PCQueue{

public:
	// add constructor and destructor// 
	PCQueue(){
		tasksQueue = new std::queue<T>();
		// intialize condition,semaphore,flags//
		pthread_cond_init(&readers_cond,nullptr);
		this->sem = new Semaphore();
		this->locked_reader=false;
		this->locked_writer=false;
		//intialize mutexes // 
		pthread_mutexattr_t attrs;
		pthread_mutexattr_init(&attrs);
		pthread_mutexattr_settype(&attrs,PTHREAD_MUTEX_ERRORCHECK);
		pthread_mutex_init(&uncond_mutex,&attrs);			
		pthread_mutex_init(&cond_mutex,&attrs);	
	
		}
	~PCQueue(){
		delete sem;
		delete tasksQueue;
		pthread_mutex_destroy(&cond_mutex);
		pthread_mutex_destroy(&uncond_mutex);
		pthread_cond_destroy(&readers_cond);
		
		
		}
	// Allows for producer to enter with *minimal delay* and push items to back of the queue.
	// Hint for *minimal delay* - Allow the consumers to delay the producer as little as possible.  
	// Assumes single producer 
	void push(const T& item){
		pthread_mutex_lock(&cond_mutex);
		locked_writer = true;
		// unlock now//
		pthread_mutex_unlock(&cond_mutex);
		// pushing after locking the mutex//
		pthread_mutex_lock(&uncond_mutex);
		// push item into our queue 
		tasksQueue->push(item);
		// unlock mutex now//
		pthread_mutex_unlock(&uncond_mutex);
		// up the semaphore //
		sem->up();
		pthread_mutex_lock(&cond_mutex);
		// change locked reader // 
		locked_writer = false;
		// next thread , use broadcast // 
		pthread_cond_broadcast(&readers_cond);
		pthread_mutex_unlock(&cond_mutex);
	
		}		
	// Blocks while queue is empty. When queue holds items, allows for a single
	// thread to enter and remove an item from the front of the queue and return it. 
	// Assumes multiple consumers.
	T pop(){
		T result;
		// to prevent pop from empty queue use down from semaphore // 
		sem->down();
		// first lock the cond_mutex // 
		pthread_mutex_lock(&cond_mutex);
		while(locked_reader || locked_writer){
			pthread_cond_wait(&readers_cond,&cond_mutex);
		}
		// no writers - > make sure no readers // 
		locked_reader = true;
		// unlock now//
		pthread_mutex_unlock(&cond_mutex);
		// this is the critical section // 
		// lock the uncond mutex and get the first task in queue // 
		pthread_mutex_lock(&uncond_mutex);
		result=tasksQueue->front();
		//now pop // 
		tasksQueue->pop();
		// unlock mutex now//
		pthread_mutex_unlock(&uncond_mutex);
		pthread_mutex_lock(&cond_mutex);
		// change locked reader // 
		locked_reader = false;
		// next thread , use broadcast // 
		pthread_cond_broadcast(&readers_cond);
		pthread_mutex_unlock(&cond_mutex);
		// finally return result
		return result; // result is the returned value // 
	
		
		}
	



private:
	std::queue<T> *tasksQueue;
	pthread_cond_t readers_cond;
	pthread_mutex_t uncond_mutex;
	pthread_mutex_t cond_mutex;
	// added flags // 
	bool locked_writer; // inside lock there is a writer // 
	bool locked_reader; // inside lock there is a reader // 
	Semaphore *sem; // use to prevent acsses to empty queue //
	

};
// Recommendation: Use the implementation of the std::queue for this exercise
#endif
