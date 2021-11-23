//
// Created by Spencer Keith on 11/3/21.
// --
  
//#include <jni_md.h>
//#include <jni.h>

// When merging RTS with the rest of the program, it may be best to convert the input functions from using return values to setting the 
//passed vectors. Otherwise, the entirety of the two programs could be called by a separate entry point.



//#define TEST
#define TEST_FILE "input_100"
//#define DEBUG_INPUT_ECHO

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
//#include <gtk.h>

#include "MFQS.h"
#include "RTS.h"
#include "Client.h"
#include "Process.h"
#include "Statistics.h"
//#include "sch_gui.h"

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
			
			#ifdef DEBUG_INPUT_ECHO
			cout << to_string( input_int );
			#endif
			
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
	int statsmode;
	Statistics *stat_tracker;
	
	vector<Process*> *vect_new_procs;

	#ifdef TEST
	queue_count = 3;
	time_quantum = 4;
	IO_point = 1;
	age_point = 40;
	vect_new_procs = read_proc_file_for_test( "input_100", true );
	stat_tracker = new Statistics();
	MFQS mfqs( queue_count, time_quantum, IO_point, age_point, vect_new_procs, stat_tracker );
	mfqs.start();
	stat_tracker->get_stats(); 
	return 0;
	#endif



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
				IO_point = 0;
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
				if( sch_alg == 1){
					next_input = NEXT_INPUT_IO;
				}else{
					next_input = NEXT_INPUT_READMODE;
				}
			}
			
		}
		
		// Get the point in the time quantum at which I/O should be performed.		
		while( next_input == NEXT_INPUT_IO ){
			
			//Debugcout << "{io point}" << endl; 
			
			interface.msg_menu[0] = "Choose whether or not processes should perform I/O operations, if they have any:";
			interface.msg_menu[1] = "   1 : Yes";
			interface.msg_menu[2] = "   2 : No";
			interface.msg_ind_switch = 3;
			
			IO_point = interface.get_line_int( flow_control, print_menu );
			
			if( flow_control == 1 ){
				next_input = NEXT_INPUT_TIME;
				flow_control = 0;
				break;
			}
			if( flow_control == 2 ){
				return 0;
			}
			
			if( IO_point == 2 ){
				IO_point = 0;
			}else
			if( IO_point != 1 ){
				cout << "Command not recognized." << endl;
				continue;
			}
			
			cout << endl;
			print_menu = true;
			next_input = NEXT_INPUT_READMODE;
			
			/*
			if( IO_point > time_quantum || IO_point < 0 ){
				cout << "The I/O point must be between 1 and the chosen time qauntum, or 0 if I/O should not be performed." << endl;
			}else{
				cout << endl;
				print_menu = true;
				next_input = NEXT_INPUT_READMODE;

			}
			*/
		}

		// Get the input mode (read from file or user input).
		while( next_input == NEXT_INPUT_READMODE ){

			interface.msg_menu[0] = "Select the method for gathering process information:";
			interface.msg_menu[1] = "   1 : Read from file";
			interface.msg_menu[2] = "   2 : Enter process information via console";
			interface.msg_ind_switch = 3;
			
			readmode = interface.get_line_int( flow_control, print_menu );
			
			if( flow_control == 1 ){
				if( sch_alg == 1 ){
					next_input = NEXT_INPUT_IO;
				}else{
					next_input = NEXT_INPUT_TIME;
				}
	
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
			}else
			if( readmode == 2 ){
				vect_new_procs = read_proc_console( nullptr, (IO_point != 0) );
				if( vect_new_procs == nullptr ){
					print_menu = true;
					continue;
				}else
				if( vect_new_procs->size() == 0 ){
					cout << "No process entered. Exiting..." << endl;
					return 0;
				}
			}else{
				cout << "Command not recognized." << endl;
				continue;
			}

			next_input = NEXT_INPUT_STATISTICS;
			
		}
	
		// Get the mode of statistics tracking.
		while( next_input == NEXT_INPUT_STATISTICS ){

			interface.msg_menu[0] = "Select the method for statistics gathering:";
			interface.msg_menu[1] = "   1 : Don't track statistics.";
			interface.msg_menu[2] = "   2 : Track statistics.";
			interface.msg_ind_switch = 3;
			
			print_menu = true;
			statsmode = interface.get_line_int( flow_control, print_menu );
			
			if( flow_control == 1 ){
				NEXT_INPUT_READMODE;
				flow_control = 0;
				break; 
			}
			if( flow_control == 2 ){
				return 0;
			}
			
			//vector<Process*> *vect_new_procs;
			if( statsmode == 1 ){
				stat_tracker = nullptr;
			}
			else if( statsmode == 2 ){
				stat_tracker = new Statistics();
			}else{
				cout << "Invalid input.\n";
				continue;
			}

			next_input = 0;
			get_input = false; 

		}
	
	
	}

	// Build and execute the simulation.
	if( sch_alg == 1 ){
		MFQS mfqs( queue_count, time_quantum, IO_point, age_point, vect_new_procs, stat_tracker );
		mfqs.start();
		
		if(stat_tracker != nullptr){
			stat_tracker->get_stats();
		}
	}else{
		//RTS
		//+...
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
	Process *proc;
	
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
				//Process *proc = new Process( proc_info_arr[0] );
				proc = new Process( proc_info_arr[0] );
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
	
	cout << "Input parsed." << endl;

	return vect_new_procs;
}

vector<Process*>* read_proc_console( vector<Process*> *proc_vect_ptr, bool do_io ){

	bool need_input = true;
	bool line_complete;
	int proc_info_arr[PROCESS_PARAM_COUNT];
	size_t pos_begin;
	size_t pos_end;
	int arr_ind;
	string proc_info;
	vector<Process*> *vect_new_procs;
	Process *proc;
	
	if( proc_vect_ptr != nullptr ){
		vect_new_procs = proc_vect_ptr;
	}else{
		vect_new_procs = new vector<Process*>;
		vect_new_procs->reserve( QUEUE_CAP_INIT );
	}
	
	cout << "Enter process parameters in the following format:" << endl;
	cout << " <ID> <Burst> <Arrival> <Priority> <Deadline> <IO>" << endl;
	cout << "Processes with invalid parameters will be discarded." << endl;
	cout << "   b : Return to input select" << endl;
	cout << "   f : End process input" << endl;
	
	while( need_input ){
		cout << "-> ";
		getline( cin, proc_info );
		if( proc_info.size() == 0 ){
			continue;
		}
		
		if( proc_info.compare( "b" ) == 0 ){
			delete vect_new_procs;
			return nullptr;
		}
		if( proc_info.compare( "f" ) == 0 ){
			return vect_new_procs;
		}
	
		pos_begin = proc_info.find_first_of( "1234567890" ); 
		if( proc_info.find( "-" ) != string::npos || pos_begin == string::npos ){
			continue;
		}

		for( int i = 0; i < 5; i++ ){
			pos_end = proc_info.find( " ", pos_begin );
			
			
			proc_info_arr[i] = stoi( proc_info.substr( pos_begin, pos_end - pos_begin ) );
			
			
			pos_begin = proc_info.find_first_of( "1234567890", pos_end );
		}

		proc_info_arr[5] = stoi( proc_info.substr( pos_begin ) );
		
		// Validate info.
		if( proc_info_arr[1] == 0 ){
			continue;
		}

		// Construct Process objects.
		//Process *proc = new Process( proc_info_arr[0] );
		proc = new Process( proc_info_arr[0] );
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
		
		#ifdef DEBUG_INPUT_ECHO
		cout << "Process parameters:" << endl;
		cout << " ID: " << to_string( proc->P_ID ) << endl;
		cout << " Burst: " << to_string( proc->Burst ) << endl;
		cout << " Arrival: " << to_string( proc->Arrival ) << endl;
		cout << " Priority: " << to_string( proc->Priority ) << endl;
		cout << " Deadline: " << to_string( proc->Deadline ) << endl;
		cout << " IO: " << to_string( proc->IO ) << endl;
		cout << endl;
		#endif
		
		insert_sorted( vect_new_procs, proc, SORT_ARRIVAL_TIME );

	}

	return nullptr;
}

vector<Process*>* read_proc_file_for_test( string test_file, bool do_io ){

	int proc_info_arr[PROCESS_PARAM_COUNT];
	size_t pos_begin;
	size_t pos_end;
	string proc_info;
	vector<Process*> *vect_new_procs;

	ifstream proc_file( test_file );
		
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
				return 0;
			}
			
		}else{
			// Could not open file.
			cout << "Failed to open file." << endl;
		}
	
	cout << "Input parsed." << endl;
	
	return vect_new_procs;
}

