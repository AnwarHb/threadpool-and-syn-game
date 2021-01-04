#ifndef __SEMAPHORE_H
#define __SEMAPHORE_H
#include "Headers.hpp"

// Synchronization Warm up 
class Semaphore {
public:
	Semaphore(); // Constructs a new semaphore with a counter of 0
	Semaphore(unsigned val); // Constructs a new semaphore with a counter of val


	void up(); // Mark: 1 Thread has left the critical section
	void down(); // Block untill counter >0, and mark - One thread has entered the critical section.
	// add destructor //
	~Semaphore();
private:
	pthread_mutex_t sem_Mutex;
	pthread_cond_t sem_Cond;
	// added values for the starting value of counter and current value of counter // 
	int startingVal;
	int currentVal;
	
};

#endif
