#include "Thread.hpp"
#include "taskThread.hpp"
#include "../Part1/PCQueue.hpp"
#include "task.hpp"

// this is our c'tor // 
taskThread::taskThread(uint idThread,pthread_mutex_t* mutex,vector<double>* hist,
	PCQueue<Task>* tasks,vector<vector<int>>* current,vector<vector<int>>* next,
	uint* stopPhase):
	Thread(idThread),m(mutex),histogram_of_tile(hist),
	tasksQueue(tasks),curr_table(current),nxt_table(next),
	phase_phinished(stopPhase){}
// thread workload // 
void taskThread::thread_workload(){
	while(1){
		// get the queue top // 
		Task curr_task=tasksQueue->pop();
		// get the start - use clock // 
		auto begin_point=std::chrono::system_clock::now();
		int task_num_of_lines = curr_task.get_number_of_lines();
		int task_start = curr_task.get_start_of_line();
		// first, phase 1 // 
		if(curr_task.get_phase() == 1){
		// start iterating - outter loop top to bottom //
		for(int i=task_start;i<task_start+task_num_of_lines;i++){
			// get bottom and top corners - check if we go out of the board // 
			int corner_top=((int) curr_table->size() >i+1) ? i+1 : (int) curr_table->size()-1;
			int corner_below = (i-1>0) ? i-1 :0;
			// inner loop - left to right // 
			for(int j=0;j<(int)(*curr_table)[i].size();j++){
				// get left and right - check out of borders//
				int right=j+1>(int)(*curr_table)[i].size()-1?(int)(*curr_table)[i].size()-1:j+1;//changed here to j+1
				int left=j-1<0?0:j-1;
				// define a histogram for colores and num of neighbours//
				int histogram_of_colors[8]={0,0,0,0,0,0,0,0}; // intial = 0
				int n_num=0; // for neighbours
				for(int r=corner_below;r<=corner_top;r++){
					for(int c=left;c<=right;c++){
						if(c==j && r==i)
							continue; // not a neighbour - our current cell in table // 
						else if((*curr_table)[r][c] !=0){
							if(++n_num<=3){ // change here to ++n_num //
								histogram_of_colors[((*curr_table)[r][c])]++; 	
							}
						}
					}
				}
				if(n_num==3 && (*curr_table)[i][j] == 0){
					// get the dominant species // 
					int maxSpecies=0;
					int valMax=0;
					for(int c=0;c<8;c++){
						if(c*histogram_of_colors[c]>valMax){
							valMax=histogram_of_colors[c]*c;
							maxSpecies=c;
						}
							(*nxt_table)[i][j] = maxSpecies;
						}
					}
					else if(n_num!=2 && n_num!=3 &&(*curr_table)[i][j]!=0){
						(*nxt_table)[i][j]=0;
					} else{
						(*nxt_table)[i][j]= (*curr_table)[i][j];
					}
				}
			}
		}
			// now lock the mutex and change finished_P1 counter //
			/* pthread_mutex_lock(m);
			(*finished_P1)--;
			// wait on cond //

			pthread_mutex_unlock(m);
			while((*finished_P1) !=0){
				sched_yield();
			}*/
			// ended phase 1 // 		
		else{
			// now for phase 2 // 
			for(int i=task_start;i<task_start+task_num_of_lines;i++){
				int corner_top=((int) nxt_table->size() >i+1) ? i+1 : (int) nxt_table->size()-1;
				int corner_below = (i-1>0) ? i-1 :0;
				for(int j=0;j<(int)(*nxt_table)[i].size();j++){
					// add checking for if value is 0//
					if((*nxt_table)[i][j] == 0){
						(*curr_table)[i][j]=0;
						// continue no need to go through inner loops // 
						continue;
					}
					int right=j+1>(int)(*curr_table)[i].size()-1?(int)(*curr_table)[i].size()-1:j+1;//changed here to j+1
					int left=j-1<0?0:j-1;	
					double n_num=0;
					double sum=0;
					for(int r=corner_below;r<=corner_top;r++){
						for(int c=left;c<=right;c++){
							if((*nxt_table)[r][c] !=0){
								n_num++;
								sum=sum+(*nxt_table)[r][c];
							}
						}
					}
					// now calculate round(average) //
					//note : due to phase 1, num of neighbours is not 0//
					(*curr_table)[i][j] = round(sum/n_num);
				}
			}
		}
			// now lock mutex and perform changes // 
			pthread_mutex_lock(m);
			auto finish= std::chrono::system_clock::now();
			(*phase_phinished)--;
			histogram_of_tile->push_back((double)std::chrono::duration_cast<std::chrono::microseconds>(finish-begin_point).count());
			//pthread_cond_broadcast(thread_cond);
		    // finally !! unlock // 
			pthread_mutex_unlock(m);
			if(curr_task.check_is_last()){
				return;
			}
		} 
 } // end function // 				

