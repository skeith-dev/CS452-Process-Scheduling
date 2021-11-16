#include <string>
#include <vector>

#ifndef LAB2_CLIENT_H
#define LAB2_CLIENT_H

#define QUEUE_CAP_INIT 4096

#define INPUT_EXIT "q"
#define INPUT_BACK "b"

#define NEXT_INPUT_SCHEDULE 1
#define NEXT_INPUT_TIME 2
#define NEXT_INPUT_IO 3 
#define NEXT_INPUT_MFQS_AGE 4
#define NEXT_INPUT_READMODE 5
#define NEXT_INPUT_MFQS_QUEUES 6
#define NEXT_INPUT_PROCESS 7

#define PROCESS_PARAM_COUNT 6

using namespace std;


class interface_input {

	int msg_total = 4;

public:

	vector<string> msg_menu;
	string input_str;
	int msg_ind_switch{};
	
	interface_input();
	int get_line_int( int &flow_control, bool &print_menu );

};


int multi_feedback_queue();
int real_time();

vector<Process*>* read_proc_file();
vector<Process*>* read_proc_console();


#endif