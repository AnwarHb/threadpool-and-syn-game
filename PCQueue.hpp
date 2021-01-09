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

	PCQueue():tasksQueue(),mutex(),sem(),waiting_produsers(0){

		pthread_mutex_init(&mutex,nullptr);

	

		}

	/*~PCQueue(){

		delete sem;

		delete tasksQueue;

		pthread_mutex_destroy(&cond_mutex);

		pthread_mutex_destroy(&uncond_mutex);

		pthread_cond_destroy(&readers_cond);

		

		

		}*/

	// Allows for producer to enter with *minimal delay* and push items to back of the queue.

	// Hint for *minimal delay* - Allow the consumers to delay the producer as little as possible.  

	// Assumes single producer 

	void push(const T& item){

		waiting_produsers=1;

		pthread_mutex_lock(&mutex);

		tasksQueue.push(item);

		waiting_produsers=0;

		pthread_mutex_unlock(&mutex);

		sem.up();

	

		}		

	// Blocks while queue is empty. When queue holds items, allows for a single

	// thread to enter and remove an item from the front of the queue and return it. 

	// Assumes multiple consumers.

	T pop(){

		// give priority to producers // 

		while(waiting_produsers){ // 

				sched_yield(); // better performance // 

		}

		// to prevent pop from empty queue use down from semaphore // 

		sem.down();

		// first lock the mutex // 

		pthread_mutex_lock(&mutex);

		T result=tasksQueue.front();

		//now pop // 

		tasksQueue.pop();

		// unlock mutex now//

		pthread_mutex_unlock(&mutex);

		// finally return result

		return result; // result is the returned value // 

		

		}

	







private:

	std::queue<T> tasksQueue;	

	pthread_mutex_t mutex;

	Semaphore sem; // use to prevent acsses to empty queue //

	int waiting_produsers; // added (values 1,0)- if there is one produser awaiting should give priority//



};

// Recommendation: Use the implementation of the std::queue for this exercise

#endif


