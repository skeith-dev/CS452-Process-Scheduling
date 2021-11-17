//
// Created by Spencer Keith on 11/3/21.
// --

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

#include "MFQS.h"
#include "RTS.h"
#include "Client.h"




using namespace std;


interface_input::interface_input(){
	
	msg_menu.reserve( msg_total );

}

// Get a line of input from the user.
int interface_input::get_line_int( int &flow_control, bool &print_menu ){
	int input_int = 0;
	
	if( print_menu ){
		for( int i = 0; i < msg_ind_switch; i++ ){
			cout << msg_menu[i] << endl;
		}
	
		cout << "   -" << endl;
		if( flow_control != -1 ){
			cout << "   b : Re-enter the previous parameter" << endl;	
		}
		cout << "   q : Exit the program" << endl;
	}

	print_menu = false;
	while( input_int == 0 ){
		input_str.clear();
		while( input_str.empty() ){
			cout << "->";
			getline( cin, input_str );
		}
	
		if( input_str.compare( "b" ) == 0 && flow_control != -1 ){
			flow_control = 1;
			print_menu = true;
			return 0;
		}else 
		if( input_str.compare( "q" ) == 0  ){
			flow_control = 2;
			return 0;
		}
	
		try{
			input_int = stoi( input_str );
			return input_int;
		}catch( ... ){
			cout << "Command not recognized." << endl;
			input_int = 0;
		}
	}
	
	return 0;
}



int main() {
	
	interface_input interface;

	int flow_control = -1;
	
	bool get_input = true;
	bool print_menu = true;
	
	int next_input = NEXT_INPUT_SCHEDULE;
	
	
	int sch_alg;
	int time_quantum;
	int IO_point;
	int queue_count;
	int age_point;
	int readmode;
	
	vector<Process*> *vect_new_procs;

	while( get_input ){

		// Get the scheduling algorithm to be simulated.
		while( next_input == NEXT_INPUT_SCHEDULE ){
			
			//Debugcout << "{schedule}" << endl; 
			
			interface.msg_menu[0] = "Select the scheduling algorithm to simulate:";
			interface.msg_menu[1] = "   1 : Multi-Feedback Queue";
			interface.msg_menu[2] = "   2 : Real-Time";
			interface.msg_ind_switch = 3;
			
			sch_alg = interface.get_line_int( flow_control, print_menu );
			
			if( flow_control == 2 ){
				return 0;
			}
			
			if( sch_alg == 1 ){
				next_input = NEXT_INPUT_MFQS_QUEUES;
			}else
			if( sch_alg == 2 ){
				next_input = NEXT_INPUT_TIME;
			}else{
				cout << "Command not recognized." << endl;
				continue;
			}
			
			cout << endl;
			print_menu = true;
			flow_control = 0;
		}
		
		// If MFQS, get the number of queues.
		while( next_input == NEXT_INPUT_MFQS_QUEUES ){
			
			//Debugcout << "{queues}" << endl; 
			
			interface.msg_menu[0] = "Enter the number of queues for the MFQS simulation:";
			interface.msg_ind_switch = 1;
		
			queue_count = interface.get_line_int( flow_control, print_menu );
			
			if( flow_control == 1 ){
				next_input = NEXT_INPUT_SCHEDULE;
				flow_control = -1;
				break;
			}
			if( flow_control == 2 ){
				return 0;
			}
			
			if( queue_count < 1 ){
				cout << "The number of queues must be greater than or equal to 1." << endl;
			}else{
				cout << endl;
				print_menu = true;	
				if( queue_count > 1 ){
					next_input = NEXT_INPUT_MFQS_AGE;
				}else{
					next_input = NEXT_INPUT_TIME;
				}
			}
		
		}
		
		// If MFQS and more than 1 queue, get the lowest-priority age-up threshold.
		while( next_input == NEXT_INPUT_MFQS_AGE && queue_count > 1 ){
			
			//Debugcout << "{age}" << endl; 
			
			interface.msg_menu[0] = "Enter the time in the lowest queue after which a process should age up:";
			interface.msg_ind_switch = 1;
			
			age_point = interface.get_line_int( flow_control, print_menu );
			
			if( flow_control == 1 ){
				next_input = NEXT_INPUT_MFQS_QUEUES;
				flow_control = 0;
				break;
			}
			if( flow_control == 2 ){
				return 0;
			} 
			
			if( age_point < 1 ){
				cout << "The specified age must be greater than or equal to 1." << endl;
			}else{
				cout << endl;
				print_menu = true;
				next_input = NEXT_INPUT_TIME;
			}
			
		}			
		
		// Get the time quantum to be used.
		while( next_input == NEXT_INPUT_TIME ){
			
			//Debugcout << "{time quantum}" << endl; 
			
			interface.msg_menu[0] = "Enter the time quantum to be used:";
			interface.msg_ind_switch = 1;
			
			time_quantum = interface.get_line_int( flow_control, print_menu );
			
			if( flow_control == 1 ){
				if( sch_alg == 1 ){
					if( queue_count > 1 ){
						next_input = NEXT_INPUT_MFQS_AGE;	
					}else{
						next_input = NEXT_INPUT_MFQS_QUEUES;
					}
					flow_control = 0;
				}else{
					next_input = NEXT_INPUT_SCHEDULE;
					flow_control = -1;
				}
				break;
			}
			if( flow_control == 2 ){
				return 0;
			}
			
			if( time_quantum < 1 ){
				cout << "The time quantum must be greater than or equal to 1." << endl;
			}else{
				cout << endl;
				print_menu = true;
				next_input = NEXT_INPUT_IO;
			}
			
		}
		
		// Get the point in the time quantum at which I/O should be performed.		
		while( next_input == NEXT_INPUT_IO ){
			
			//Debugcout << "{io point}" << endl; 
			
			interface.msg_menu[0] = "Enter the point in the time quantum at which I/O should be performed, or 0 if I/O should not be performed:";
			interface.msg_ind_switch = 1;
			
			IO_point = interface.get_line_int( flow_control, print_menu );
			
			if( flow_control == 1 ){
				next_input = NEXT_INPUT_TIME;
				flow_control = 0;
				break;
			}
			if( flow_control == 2 ){
				return 0;
			}
		
			if( IO_point > time_quantum || IO_point < 0 ){
				cout << "The I/O point must be between 1 and the chosen time qauntum, or 0 if I/O should not be performed." << endl;
			}else{
				cout << endl;
				print_menu = true;
				next_input = NEXT_INPUT_READMODE;

			}
		}


		// Get the input mode (read from file or user input).
		while( next_input == NEXT_INPUT_READMODE ){

			interface.msg_menu[0] = "Select the method for gathering process information:";
			interface.msg_menu[1] = "   1 : Read from file";
			interface.msg_menu[2] = "   2 : Enter process information via console";
			interface.msg_ind_switch = 2;
			
			readmode = interface.get_line_int( flow_control, print_menu );
			
			if( flow_control == 1 ){
				next_input = NEXT_INPUT_IO;
				flow_control = 0;
				break;
			}
			if( flow_control == 2 ){
				return 0;
			}
			
			//vector<Process*> *vect_new_procs;
			if( readmode == 1 ){
				vect_new_procs = read_proc_file( (IO_point != 0) );
				if( vect_new_procs == nullptr ){
					print_menu = true;
					continue;
				}
			}else{
				cout << "Command not recognized." << endl;
				continue;
			}
			
		
			next_input = 0;
			get_input = false; 
			
			// Debug return 0; 
		}
	
		

	}

	// Build and execute the simulation.
	if( sch_alg == 1 ){
		MFQS mfqs( queue_count, time_quantum, IO_point, age_point, vect_new_procs );
		mfqs.start();
	}
	

    return 0;
 
}

vector<Process*>* read_proc_file( bool do_io ){
	bool need_file = true;
	int proc_info_arr[PROCESS_PARAM_COUNT];
	size_t pos_begin;
	size_t pos_end;
	int arr_ind;
	string fname;
	string proc_info;
	vector<Process*> *vect_new_procs;
	
	cout << "Enter the name of the file to be read: " << endl;
	cout << "   b : Return to input select" << endl;
	
	
	while( need_file ){
		
		cout << "->";
		getline( cin, fname );
		
		if( fname.compare( "b" ) == 0 ){
			return nullptr;
		}

		ifstream proc_file( fname );
		
		// If file was opened successfully...
		if( proc_file.is_open() ){
			
			vect_new_procs = new vector<Process*>;
			vect_new_procs->reserve( QUEUE_CAP_INIT );
			
			// While there are lines to be read...
			while( getline( proc_file, proc_info ) ){
				
				pos_begin = proc_info.find_first_of( "1234567890" ); 
				if( proc_info.find( "-" ) != string::npos || pos_begin == string::npos ){
					continue;
				}

				for( int i = 0; i < 5; i++ ){
					pos_end = proc_info.find( "\t", pos_begin );

					proc_info_arr[i] = stoi( proc_info.substr( pos_begin, pos_end - pos_begin ) );
					
					pos_begin = proc_info.find_first_of( "1234567890", pos_end );
				}
				
				proc_info_arr[5] = stoi( proc_info.substr( pos_begin ) );

				// Construct Process objects.
				Process *proc = new Process( proc_info_arr[0] );
				proc->Burst = proc_info_arr[1];
				proc->Arrival = proc_info_arr[2];
				proc->Priority = proc_info_arr[3];
				proc->Deadline = proc_info_arr[4];
				proc->IO = proc_info_arr[5];
				if( proc->IO == 0 || !do_io ){
					proc->IO_Done = -1;
				}else{
					proc->IO_Done = 0;
				}

				insert_sorted( vect_new_procs, proc, SORT_ARRIVAL_TIME );
			}
			
			// If reading stopped because of an error...
			if( !proc_file.eof() ){
				cout << "Error reading file." << endl;
				delete vect_new_procs;
				break;
			}
			
			need_file = false;
		}else{
			// Could not open file.
			cout << "Failed to open file." << endl;
		}

	}
	

	
	//Begin Debug
	/*	
	Process *p;
	int s = vect_new_procs->size();
	for( int i = 0; i < s; i++ ){
		
		p = vect_new_procs->front();
		pop_heap( vect_new_procs->begin(), vect_new_procs->end(), comp_proc );
		vect_new_procs->pop_back();
		
		cout << "Process " << i << ":" << endl;
		cout << "  P_ID: " << p->P_ID << endl;
		cout << "  Burst: " << p->Burst << endl;
		cout << "  Arrival: " << p->Arrival << endl;
		cout << "  Priority: " << p->Priority << endl;
		cout << "  Deadline: " << p->Deadline << endl;
		cout << "  IO: " << p->IO << endl;

	}*/
	
	
	cout << "Input parsed." << endl;
	
	//End Debug
	
	
	
	
	return vect_new_procs;
}

vector<Process*>* read_proc_console(){

	return nullptr;
}




int multi_feedback_queue(  ){
	
	
	//	Create vector for new processes and reserve capacity.
	//vector<Process*> *vect_new_procs = new vector<Process*>;
	//vect_new_procs->reserve( QUEUE_CAP_INIT );
	

	
	
	
	//Begin Test Data
	/*
	int q_count = 3;
	int time_quantum = 2;
	
	Process *p1 = new Process( 1 );
	vect_new_procs->push_back( p1 );
	push_heap( vect_new_procs->begin(), vect_new_procs->end() );
	
	Process *p2 = new Process( 2 );
	vect_new_procs->push_back( p2 );
	push_heap( vect_new_procs->begin(), vect_new_procs->end() );
	
	Process *p3 = new Process( 3 );
	vect_new_procs->push_back( p3 );
	push_heap( vect_new_procs->begin(), vect_new_procs->end() );
	
	Process *p4 = new Process( 4 );
	vect_new_procs->push_back( p4 );
	push_heap( vect_new_procs->begin(), vect_new_procs->end() );
	
	Process *p5 = new Process( 5 );
	vect_new_procs->push_back( p5 );
	push_heap( vect_new_procs->begin(), vect_new_procs->end() );
	
	Process *p6 = new Process( 6 );
	vect_new_procs->push_back( p6 );
	push_heap( vect_new_procs->begin(), vect_new_procs->end() );
	
	Process *p7 = new Process( 7 );
	vect_new_procs->push_back( p7 );
	push_heap( vect_new_procs->begin(), vect_new_procs->end() );
	
	Process *p8 = new Process( 8 );
	vect_new_procs->push_back( p8 );
	push_heap( vect_new_procs->begin(), vect_new_procs->end() );
	
	Process *p9 = new Process( 9 );
	vect_new_procs->push_back( p9 );
	push_heap( vect_new_procs->begin(), vect_new_procs->end() );
	
	Process *p10 = new Process( 10 );
	vect_new_procs->push_back( p10 );
	push_heap( vect_new_procs->begin(), vect_new_procs->end() );
	*/
	//End Test Data
	
	
	
	//MFQS *mfqs = new MFQS( q_count, time_quantum, vect_new_procs );
	

	return 0;
}


int real_time( ){


	return 0;
}

