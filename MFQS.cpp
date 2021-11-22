//
// Created by Spencer Keith on 11/3/21.
//--
//Need to handle potential failure of 'new' calls.

#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>

//#define DEBUG 
//#define PER_CLOCK_OUT 


#include "MFQS.h"

using namespace std;


MFQS::MFQS(int qc, int tq, int iop, int ap, vector<Process*> *vnp){

	// Initialize the clock, the new-process queue, and 
	//user-defined variables.
	clock = 0;
	q_count = qc;
	time_quantum = tq;
	io_point = iop;
	age_point = ap;
	vect_new_procs = vnp;

	// Create the array used to hold the queue vector pointers.
	qv_arr = new vector<Process*>*[q_count+1];
	tq_arr = new int[q_count];
	
	
	// Create the queue vectors.
	for( int i = 0; i <= q_count; i++ ){
		qv_arr[i] = new vector<Process*>();
		qv_arr[i]->reserve( vect_new_procs->capacity() );	
		tq_arr[i] = time_quantum * pow( 2, i ); 
	}


}

// Set up the simulation and call the primary method.
int MFQS::start(){
	//Statistics:
	//This is where the first-arriving processes begin waiting. 
	//The front of vect_new_procs is always selected for execution. Additional process are placed in ready queue 0
	//if their arrival time is equal to the arrival time of the first process. Thus, the process *outside* the for loop
	//begins executing at its arrival time without waiting. Processes added *inside* the for loop begin waiting for execution
	//at their arrival time (note that the arrival time for all of these processes is equal to the current clock value, after
	//the clock variable is set.
	
	cout << "Starting simulation. " << endl;
	
	current_process = vect_new_procs->front();
	clock = current_process->Arrival;
	
	pop_heap( vect_new_procs->begin(), vect_new_procs->end(), comp_proc_arrival );
	vect_new_procs->pop_back();
	
#ifdef PER_CLOCK_OUT
	cout << "C" << to_string( clock ) << ": Process " << to_string(current_process->P_ID) << " has been submitted. \n";
#endif
	
	Process *temp;
	while( vect_new_procs->front()->Arrival == current_process->Arrival ){
		temp = vect_new_procs->front();
		pop_heap( vect_new_procs->begin(), vect_new_procs->end(), comp_proc_arrival );
		vect_new_procs->pop_back();
		
		qv_arr[0]->push_back( temp );
		
		#ifdef PER_CLOCK_OUT
		cout << "C" << to_string( clock ) << ": Process " << to_string(temp->P_ID) << " has been submitted. \n";
		#endif
	}

	cycle2();

	return 0;
}




// The primary method of MFQS. Unlike the initial attempt, each clock cycle within the step is processed individually after the step is calculated. This avoids 
//the need for space in which to retroactively generate and sort output and vastly improves memory efficiency (i.e. it can run now) at the cost of some runtime efficiency.
void MFQS::cycle2(){
	int step;	// The distance of each step in the simulation.
	int time_quant_by_queue; // The time quanta for a specific queue.
	int q_ind;	// Used when finding the next available process in a ready queue.
	int clock_step; // Used when iterating through the individual cycles of a step.
	//int procs_complete = 0;
	
	//bool IO_flag = false;
	bool req_flag = false;
	bool done_flag = false;
	
	int max_queue = q_count-1;
	
	int qcache_upper = 0; // Index of the ready queue the current process was taken from.
	int qcache_lower = 0; // Index of the ready queue the current process was added to, if applicable.

	Process *temp_proc; // A temporary Process pointer.
	vector<Process*> *temp_queue; // A temporary vector<Process*> pointer.

	//int loop_counter = 0;
	string input_str;
	//while( current_process != nullptr ){
	while( true ){
		
		/*loop_counter++;
		if( loop_counter > 25 ){
			cout << "Loop control exit." << endl;
			return;
		}*/

		#ifdef PER_CLOCK_OUT
		cout << "C" << to_string( clock ) << ": Process " << to_string( current_process->P_ID ) << " has begun executing. \n" ;
		#endif
		//Statistics:
		//This is where a process begins executing. As a process may be selected for execution from multiple places in the code, I recommend that
		//if waiting-execution transition time needs to be gathered outside of the process termination branch (where the Process object is deleted), 
		//it be gathered here and not where the waiting technically ended.
		
		// Calculate the step time.
		// If the difference between the current process's progress and total burst is less than the time quantum, the step is set to that difference,
		//minus one if the process has I/O to perform. Otherwise, the step is set to the time quantum of the current queue.
		
		time_quant_by_queue = tq_arr[current_process->QLevel];
		if( current_process->Progress + time_quant_by_queue < current_process->Burst ){
			req_flag = true;
			step = time_quant_by_queue;
		}else
		if( current_process->IO_Done != 0 ){
			step = current_process->Burst - current_process->Progress;
			done_flag = true;
		}else{
			step = (current_process->Burst - current_process->Progress)-1;
			//IO_flag = true;
		}

		// Increment the clock time and the current process's age by the step, set clock_step to begin iterating over
		//the step's individual cycles.
		clock_step = clock+1;
		clock += step;
		current_process->Progress += step;
		current_process->Age = clock;

		// Iterate through the step's individual cycles.
		while( clock_step <= clock ){

			// Check for new processes.
			// While the front process's Arrival is equal to the current clock step...
			while( vect_new_procs->size() != 0 && vect_new_procs->front()->Arrival == clock_step ){
				//Statistics:
				//This is where a process enters the system and begins waiting.

				qcache_upper = 0;

				temp_proc = vect_new_procs->front();
				temp_proc->Age = clock_step;
				
				#ifdef DEBUG
				cout << "Checking new processes..." << endl; // Debug
				#endif
			
				// Remove the process from vect_new_procs.
				pop_heap( vect_new_procs->begin(), vect_new_procs->end(), comp_proc_arrival );
				vect_new_procs->pop_back();
		
				// Add it to queue 0.
				temp_queue = qv_arr[0];
				temp_queue->push_back( temp_proc );
				push_heap( temp_queue->begin(), temp_queue->end(), comp_proc_age );
				
				#ifdef PER_CLOCK_OUT
				cout << "C" << to_string( clock_step ) << ": Process " << to_string( temp_proc->P_ID ) << " has been submitted. \n";
				#endif
			
			}
			
			// Check on processes waiting for I/O.
			// While the front process's I/O completion time is equal to the current clock step...
			temp_queue = qv_arr[q_count];
			while( temp_queue->size() != 0 && temp_queue->front()->IO_Done == clock_step ){
				//Statistics:
				//This is where a process finishes waiting for I/O. It must be noted that the process is still waiting at this time.


				#ifdef DEBUG
				cout << "Checking I/O..." << endl; // Debug
				#endif

				qcache_upper = 0;

				temp_proc = temp_queue->front();
				temp_proc->Age = clock_step;
			
				// Remove process from I/O-Wait queue.
				pop_heap( temp_queue->begin(), temp_queue->end(), comp_proc_io_wait );
				temp_queue->pop_back();
			
				// Add process to the first ready queue.
				temp_proc->QLevel = 0;
				temp_queue = qv_arr[0];
				temp_queue->push_back( temp_proc );
				push_heap( temp_queue->begin(), temp_queue->end(), comp_proc_age );
				
				#ifdef PER_CLOCK_OUT
				cout << "C" << to_string( clock_step ) << ": Process " << to_string( temp_proc->P_ID ) <<" has finished its I/O and joined queue 0. \n";
				#endif
				
				temp_queue = qv_arr[q_count];
			}
			
			// Check lowest-priority ready queue.
			// If the simulation has more than 1 queue...
			if( q_count != 1 ){
			
				// While the front process's Age plus the user-dfined age-up threshold are equal to the current clock step...
				temp_queue = qv_arr[max_queue];	
				while( temp_queue->size() != 0 && (temp_queue->front()->Age + age_point) == clock_step ){
					
					#ifdef DEBUG
					cout << "Checking lowest queue..." << endl; // Debug
					#endif

					qcache_upper = 0;
					
					temp_proc = temp_queue->front();
					temp_proc->Age = clock_step;

					// Remove process from lowest ready queue.
					pop_heap( temp_queue->begin(), temp_queue->end(), comp_proc_age );
					temp_queue->pop_back();
			
					// Add process to highest ready queue.
					temp_proc->QLevel = 0;
					temp_queue = qv_arr[0];
					temp_queue->push_back( temp_proc );
					push_heap( temp_queue->begin(), temp_queue->end(), comp_proc_age );
					
					#ifdef PER_CLOCK_OUT
					cout << "C" << to_string( clock_step ) << ": Process " << to_string( temp_proc->P_ID ) << " has been promoted from queue " << to_string( max_queue ) << " to queue 0. \n"; 
					#endif
					
					temp_queue = qv_arr[max_queue];
				}
			}
			
			clock_step++;

		}	


		
		if( req_flag ){
			//Statistics:
			//This is where an unfinished process is added to the appropriate ready queue (i.e., it is no longer executing and begins waiting again).
			
			
			#ifdef DEBUG
			cout << "Queuing current process..." << endl; // Debug
			#endif
			
			req_flag = false;
			
			// Adjust queue level of current process.
			if( current_process->QLevel != max_queue ){
				current_process->QLevel++;
				qcache_lower = current_process->QLevel;
			}

			//Add the current process to the next ready queue.
			temp_queue = qv_arr[current_process->QLevel];
			temp_queue->push_back( current_process );
			push_heap( temp_queue->begin(), temp_queue->end(), comp_proc_age );
			
			#ifdef PER_CLOCK_OUT
			cout << "C" << to_string( clock ) << ": Process " << to_string( current_process->P_ID ) << " has been moved to queue " << to_string( current_process->QLevel ) << ". \n";
			#endif
		
		}else 
		// If the current process is now complete...
		if( done_flag ){
			//Statistics:
			//This is where a process terminates.
			
			//Given the assumptions that a process is always executing or waiting, and a process executes no longer or shorter than its burst value, then...
			//Given the following assumptions:
				//A process is always either executing or waiting,
				//A process executes no longer or shorter than its burst value,
				//A process is not considered executing while waiting for I/O,
				//A process is waiting for execution if it is not waiting for I/O,
				//A process is only waiting or executing at or after its arrival time,
			//then...
			//Total waiting should be: clock - (current_process->Arrival + current_process->Burst)
			//Turnaround should be: clock - current_process->Arrival
			//The time a process begins waiting for I/O should be: current_process->IO_Done - current_process->IO
			//The time a process finishes waiting for I/O should be current_process->IO_Done
			
			
			#ifdef DEBUG
			cout << "Finishing current process..." << endl; // Debug
			#endif
			
			done_flag = false;

			#ifdef PER_CLOCK_OUT
			cout << "C" << to_string( clock ) << ": Process " << to_string( current_process->P_ID ) << " has completed. \n";
			#endif
			
			//++procs_complete;
			//cout << to_string( procs_complete ) << " processes complete. \n";
			
			// Destroy the Process object.
			//+...
			delete current_process;
			

		}else{
			// If the current process has I/O to perform and the current clock time is the universal per-process I/O point (that is, 
			//one less than the process's total burst)
			
			//Statistics:
			//This is where a process begins waitng for IO. 
			
			#ifdef DEBUG
			cout << "Moving current process to I/O queue..." << endl; // Debug
			#endif

			// Add process to I/O-Wait queue ( qv_arr[q_count] ), sorting on IO completion time.
			current_process->IO_Done = clock + current_process->IO;
			temp_queue = qv_arr[q_count];
			temp_queue->push_back( current_process );
			push_heap( temp_queue->begin(), temp_queue->end(), comp_proc_io_wait );
			
			//IO_flag = false;
			
			#ifdef PER_CLOCK_OUT
			cout << "C" << to_string( clock ) << ": Process " << to_string( current_process->P_ID ) << " is now waiting for I/O. \n";
			#endif

		}
		
		/*
		if( IO_flag ){
			
			#ifdef DEBUG
			cout << "Moving current process to I/O queue..." << endl; // Debug
			#endif
			
			// Add process to I/O-Wait queue ( qv_arr[q_count] ), sorting on IO completion time.
			current_process->IO_Done = clock + current_process->IO;
			temp_queue = qv_arr[q_count];
			temp_queue->push_back( current_process );
			push_heap( temp_queue->begin(), temp_queue->end(), comp_proc_io_wait );
			
			IO_flag = false;
			
			#ifdef PER_CLOCK_OUT
			cout << "C" << to_string( clock ) << ": Process " << to_string( current_process->P_ID ) << " is now waiting for I/O. \n";
			#endif
		
		}else 
		// If the current process is now complete...
		//if( current_process->Progress >= current_process->Burst ){
		if( current_process->Burst <= current_process->Progress ){
			
			#ifdef DEBUG
			cout << "Finishing current process..." << endl; // Debug
			#endif
			
			// Collect statistical information.
			//+...
			
			#ifdef PER_CLOCK_OUT
			cout << "C" << to_string( clock ) << ": Process " << to_string( current_process->P_ID ) << " has completed. \n";
			#endif
			
			//++procs_complete;
			//cout << to_string( procs_complete ) << " processes complete. \n";
			
			// Destroy the Process object.
			//+...
			delete current_process;

		}else{
		//Else, the process is unfinished and is to be returned to the appropriate ready queue.
			
			#ifdef DEBUG
			cout << "Queuing current process..." << endl; // Debug
			#endif
			
			// Adjust queue level of current process.
			if( current_process->QLevel != max_queue ){
				//current_process->QLevel++;
				++current_process->QLevel;
				qcache_lower = current_process->QLevel;
			}

			//Add the current process to the next ready queue.
			temp_queue = qv_arr[current_process->QLevel];
			temp_queue->push_back( current_process );
			push_heap( temp_queue->begin(), temp_queue->end(), comp_proc_age );
			
			#ifdef PER_CLOCK_OUT
			cout << "C" << to_string( clock ) << ": Process " << to_string( current_process->P_ID ) << " has been moved to queue " << to_string( current_process->QLevel ) << ". \n";
			#endif
			
		}
		*/
		
		
		// If the queue that the current process was taken from is not empty, select the front process of that queue for execution.
		//Statistics:
		//This is where the next ready process is selected for execution and is no longer waiting.
		if( qv_arr[qcache_upper]->size() != 0 ){
			temp_queue = qv_arr[qcache_upper];
			current_process = temp_queue->front();
			pop_heap( temp_queue->begin(), temp_queue->end(), comp_proc_age );
			temp_queue->pop_back();
			
		}else
		// Else, if the current process was placed in a ready queue, select the front of that queue for execution.
		if( qv_arr[qcache_lower]->size() != 0 ){
			qcache_upper = qcache_lower;
			temp_queue = qv_arr[qcache_lower];
			current_process = temp_queue->front();
			pop_heap( temp_queue->begin(), temp_queue->end(), comp_proc_age );
			temp_queue->pop_back();

		}else{
			// Else, there are no immediately ready processes.
			current_process =  nullptr;
			#ifdef DEBUG
			cout << "Preparing wait jump..." << endl; // Debug
			#endif
			
			// If there are no processes in the ready queues, the new process queue, or the I/O-Wait queue, then all processes
			//are finished and the simulation is complete.
			if( vect_new_procs->size() == 0 && qv_arr[q_count]->size() == 0 ){
				cout << "Simulation complete." << endl;
				
				//cout << to_string( procs_complete ) << endl;
				
				return;
			}
			
			// Check the new process queue and the I/O-Wait queue for processes and 
			//select the one that will be ready soonest.
			if( vect_new_procs->size() != 0){
				#ifdef DEBUG
				cout << " Set to new." << endl; // Debug
				#endif
				
				current_process = vect_new_procs->front();
			}
			if( qv_arr[q_count]->size() != 0 && (current_process == nullptr || current_process->Arrival > qv_arr[q_count]->front()->IO_Done) ){				
				#ifdef DEBUG
				cout << " Set to I/O." << endl; // Debug
				#endif
				
				current_process = qv_arr[q_count]->front();
			}

			// If the next-ready process is one that has not yet been submitted...
			if( current_process->Arrival > clock ){
				//Statistics:
				//A process selected here begins execution at its arrival time without any waiting.
				
				#ifdef DEBUG
				cout << "Selecting from new..." << endl; // Debug
				#endif
				
				// Set the clock to its arrival time and remove it from the new-process queue.
				clock = current_process->Arrival;
				current_process->Age = clock;
				
				pop_heap( vect_new_procs->begin(), vect_new_procs->end(), comp_proc_arrival );
				vect_new_procs->pop_back();
				
				#ifdef PER_CLOCK_OUT
				cout << "C" << to_string( clock ) << ": Process " << to_string( current_process->P_ID ) << " has been submitted. \n";
				#endif
			
			// Else, the next-ready process is currently waiting for I/O
			}else{
				//Statistics:
				//A process selected here finishes waiting for I/O and begins execution without additional waiting.
				
				#ifdef DEBUG
				cout << "Selecting from I/O..." << endl; // Debug
				#endif
				
				// Set the clock to its ready time and remove it from the I/O-Wait queue.
				clock = current_process->IO_Done;
				current_process->Age = clock;
				
				temp_queue = qv_arr[q_count];
				pop_heap( temp_queue->begin(), temp_queue->end(), comp_proc_io_wait );
				temp_queue->pop_back();
				
				#ifdef PER_CLOCK_OUT
				cout << "C" << to_string( clock ) << ": Process " << to_string( current_process->P_ID ) << " has finished its I/O and joined queue 0. \n";
				#endif
				
			}
			
			qcache_upper = 0;
			qcache_lower = 0;

		}
	
	}


}





