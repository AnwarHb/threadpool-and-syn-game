#ifndef _TASKTHREAD_H
#define _TASKTHREAD_H

#include "../Part1/PCQueue.hpp"
#include "task.hpp"
#include "Thread.hpp"
#include "utils.hpp"

// inherit from thread // 
class taskThread:public Thread{
private:
	//int linesNum;//
	//int pointTostart;// will not work
	pthread_mutex_t *m; // mutex//
	vector<double>* histogram_of_tile;
	PCQueue<Task>* tasksQueue; // added 			
	vector<vector<int>>* curr_table;
	vector<vector<int>>* nxt_table;
	// instead of phase and diff time changed to  unsigned for cheking finish phase//
	uint* phase_phinished;

public:
	taskThread(uint idThread,pthread_mutex_t* mutex,vector<double>* hist,
	PCQueue<Task>* tasks,vector<vector<int>>* current,vector<vector<int>>* next,
	uint* stopPhase);          
	~taskThread()=default;
	void thread_workload() override;
	};
#endif
