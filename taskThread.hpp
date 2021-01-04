#ifndef _TASKTHREAD_H
#define _TASKTHREAD_H

#include "Thread.hpp"
#include "utils.hpp"

// inherit from thread // 
class taskThread:public Thread{
private:
	int linesNum;
	int pointTostart;
	vector<vector<int>>* curr_table;
	vector<vector<int>>* nxt_table;
	vector<float>* histogram_of_tile;
	pthread_mutex_t *m; // mutex//
	bool phase;
	float difTime;
public:
	taskThread(uint idThread,int numLines,int start,vector<vector<int>>* current,vector<vector<int>>* next,
	           vector<float>* hist, pthread_mutex_t* mutex);
	~taskThread()=default;
	void thread_workload() override;
	};
#endif
