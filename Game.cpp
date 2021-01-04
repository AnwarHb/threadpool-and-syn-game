#include "Game.hpp"
#include "utils.hpp"
#include "taskThread.hpp"

static const char *colors[7] = {BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN};
// should add print board def to work // 
inline void print_board(const char* header) ;
/*--------------------------------------------------------------------------------
								
--------------------------------------------------------------------------------*/
Game::Game(game_params parmsForGame):m_gen_num(parmsForGame.n_gen),m_thread_num(parmsForGame.n_thread),m_thread_temp(parmsForGame.n_thread),
									 m_tile_hist(),m_gen_hist(), m_threadpool(),interactive_on(parmsForGame.interactive_on),
									 print_on(parmsForGame.print_on),game_mutex(),game_curr_board(),game_next_board(),game_file(parmsForGame.filename){}

void Game::run() {

	_init_game(); // Starts the threads and all other variables you need
	print_board("Initial Board");
	for (uint i = 0; i < m_gen_num; ++i) {
		auto gen_start = std::chrono::system_clock::now();
		_step(i); // Iterates a single generation 
		auto gen_end = std::chrono::system_clock::now();
		m_gen_hist.push_back((float)std::chrono::duration_cast<std::chrono::microseconds>(gen_end - gen_start).count());
		print_board(nullptr);
	} // generation loop
	print_board("Final Board");
	_destroy_game();
}

void Game::_init_game() {

	// Create game fields - Consider using utils:read_file, utils::split
	// Create & Start threads
	// Testing of your implementation will presume all threads are started here
	vector<string> exss = utils::read_lines(game_file);
	pthread_mutex_init(&game_mutex,nullptr);
	// iterate through exss // 
	for(auto &current:exss){
		vector<string> myLine= utils::split(current, ' ');
		vector<int> myLine_gg = vector<int>();
		for(uint j=0;j<myLine.size();j++)
			myLine_gg.push_back(atoi(&(myLine[j][0])));
		game_curr_board.push_back(myLine_gg);
		game_next_board.push_back(myLine_gg);
	}
	if(game_curr_board.size() <m_thread_temp){
		m_thread_num = game_curr_board.size();
	}else{
		m_thread_num = m_thread_temp;
	}
	int numOfRow = game_curr_board.size() / m_thread_temp;
	int sheret = game_curr_board.size()%m_thread_num;
	// create threads // 
	for(int j=0;j<(int) m_thread_num-1;j++){
		m_threadpool.push_back(new taskThread(j,numOfRow,numOfRow*j,&game_curr_board,&game_next_board,&m_tile_hist,&game_mutex));
	}
	m_threadpool.push_back(new taskThread(m_thread_num-1,numOfRow*(m_thread_num-1),numOfRow+sheret,&game_curr_board,&game_next_board,&m_tile_hist,&game_mutex));
	
}

void Game::_step(uint curr_gen) {
	// Push jobs to queue
	// Wait for the workers to finish calculating 
	// Swap pointers between current and next field 
	// NOTE: Threads must not be started here - doing so will lead to a heavy penalty in your grade 
	for (uint i = 0; i < m_thread_num; ++i) { 
        m_threadpool[i]->start();
    }
    for (uint i = 0; i < m_thread_num; ++i) {
        m_threadpool[i]->join();
    }
    // swap boards 
    game_curr_board=game_next_board;
    // for phase 2 // 
    for (uint i = 0; i < m_thread_num; ++i) { 
        m_threadpool[i]->start();
    }
    for (uint i = 0; i < m_thread_num; ++i) {
        m_threadpool[i]->join();
    }
    // final swap//
    game_curr_board=game_next_board;
    
}

void Game::_destroy_game(){
	// Destroys board and frees all threads and resources 
	// Not implemented in the Game's destructor for testing purposes. 
	// All threads must be joined here
	// go through thread pool and delete // 
	   for(auto & curr : m_threadpool) {
			delete curr;
    }

}
const vector<float> Game::tile_hist() const {
    return m_tile_hist;
}
uint Game::thread_num() const {
    return m_thread_num;
}
const vector<float> Game::gen_hist() const {
    return m_gen_hist;
}

/*--------------------------------------------------------------------------------
								
--------------------------------------------------------------------------------*/
inline void Game::print_board(const char* header) {

	if(print_on){ 

		// Clear the screen, to create a running animation 
		if(interactive_on)
			system("clear");

		// Print small header if needed
		if (header != nullptr)
			cout << "<------------" << header << "------------>" << endl;
		
		// TODO: Print the board //
		// define field width and hieght // 
		uint field_height = game_curr_board.size();
		uint field_width = game_curr_board[0].size();
		cout << u8"╔" << string(u8"═") * field_width << u8"╗" << endl;
		for (uint i = 0; i < field_height; ++i) {
			cout << u8"║";
			for (uint j = 0; j < field_width; ++j) {
                if (game_curr_board[i][j] > 0)
                    cout << colors[game_curr_board[i][j] % 7] << u8"█" << RESET;
                else
                    cout << u8"░";
			}
			cout << u8"║" << endl;
		}
		cout << u8"╚" << string(u8"═") * field_width << u8"╝" << endl;
		// Display for GEN_SLEEP_USEC micro-seconds on screen 
		if(interactive_on)
			usleep(GEN_SLEEP_USEC);
	}

}


/* Function sketch to use for printing the board. You will need to decide its placement and how exactly 
	to bring in the field's parameters. 

		cout << u8"╔" << string(u8"═") * field_width << u8"╗" << endl;
		for (uint i = 0; i < field_height ++i) {
			cout << u8"║";
			for (uint j = 0; j < field_width; ++j) {
                if (field[i][j] > 0)
                    cout << colors[field[i][j] % 7] << u8"█" << RESET;
                else
                    cout << u8"░";
			}
			cout << u8"║" << endl;
		}
		cout << u8"╚" << string(u8"═") * field_width << u8"╝" << endl;
*/ 



