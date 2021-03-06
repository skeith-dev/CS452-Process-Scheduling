#ifndef LAB2_CLIENT_H
#define LAB2_CLIENT_H

#include <unistd.h>
#include <string>
#include <vector>
#include "Statistics.h"
	
#define QUEUE_CAP_INIT 32768


#define INPUT_EXIT "q"
#define INPUT_BACK "b"

#define NEXT_INPUT_SCHEDULE 1
#define NEXT_INPUT_TIME 2
#define NEXT_INPUT_IO 3 
#define NEXT_INPUT_READMODE 5

#define NEXT_INPUT_MFQS_AGE 4
#define NEXT_INPUT_MFQS_QUEUES 6

#define NEXT_INPUT_PROCESS 7
#define NEXT_INPUT_STATISTICS 8

#define PROCESS_PARAM_COUNT 6



using namespace std;

class interface_input {

	int msg_total = 4;

public:

	vector<string> msg_menu;
	string input_str;
	
	int msg_ind_switch;
	
	interface_input();
	int get_line_int( int &flow_control, bool &print_menu );
};

vector<Process*>* read_proc_file( bool do_io, int sch_alg );
//vector<Process*>* read_proc_console( vector<Process*> *proc_vect_ptr = nullptr, bool do_io = false );
vector<Process*>* read_proc_console( bool do_io, int sch_alg );

vector<Process*>* read_proc_file_for_test( string test_file, bool do_io, int sch_alg );

#endif