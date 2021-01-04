#include "Thread.hpp"
#include "taskThread.hpp"

// this is our c'tor // 
taskThread::taskThread(uint idThread,int numLines,int start,vector<vector<int>>* current,vector<vector<int>>* next,
	           vector<float>* hist, pthread_mutex_t* mutex):Thread(idThread),linesNum(numLines),pointTostart(start),curr_table(current),
	           nxt_table(next),histogram_of_tile(hist),m(mutex),phase(true),difTime(0){}
// thread workload // 
void taskThread::thread_workload(){
		// get the start - use clock // 
		auto begin_point=std::chrono::system_clock::now();
		// start phase 1
		if(phase==true){
			// change phase to false
			phase=false;
			// start iterating - outter loop top to bottom //
			for(int i=pointTostart;i<linesNum+pointTostart;i++){
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
								continue; // not a neighbour
							else if((*curr_table)[r][c] !=0){
								if(++n_num<=3)
									histogram_of_colors[((*curr_table)[r][c])]++; 	
							}
						}
					}
					if(n_num==3 && (*curr_table)[i][j] == 0){
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
					}
				}
			}
			//end of phase 1
			auto finish=std::chrono::system_clock::now();
			// now calculate difTime // 
			difTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(finish-begin_point).count();
			// apperntly this what works !! // 				
		}else{
			// now for phase 2 // 
			// change phase for true // 
			for(int i=pointTostart;i<linesNum+pointTostart;i++){
				int corner_top=((int) curr_table->size() >i+1) ? i+1 : (int) curr_table->size()-1;
				int corner_below = (i-1>0) ? i-1 :0;
				for(int j=0;j<(int)(*curr_table)[i].size();j++){
					int right=j+1>(int)(*curr_table)[i].size()-1?(int)(*curr_table)[i].size()-1:j+1;//changed here to j+1
					int left=j-1<0?0:j-1;	
					double n_num=0;
					double sum=0;
					for(int r=corner_below;r<=corner_top;r++){
						for(int c=left;c<=right;c++){
							if((*curr_table)[r][c] !=0){
								n_num++;
								sum=sum+(*curr_table)[r][c];
							}
						}
					}
					// now calculate round(average) //
					//note : due to phase 1, num of neighbours is not 0//
					(*nxt_table)[i][j] = round(sum/n_num);
				}
			}
			// now lock mutex and perform changes // 
				 pthread_mutex_lock(m);
				 auto finish= std::chrono::system_clock::now();
				 histogram_of_tile->push_back(difTime+(float)std::chrono::duration_cast<std::chrono::microseconds>(finish-begin_point).count());
				 // finally !! unlock // 
				  pthread_mutex_unlock(m);
			  } // end of phase 2 // 
 } // end function // 
					

