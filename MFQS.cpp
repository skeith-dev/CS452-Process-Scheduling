//
// Created by Spencer Keith on 11/3/21.
//--
//Need to handle potential failure of 'new' calls.

#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>


#include "MFQS.h"

using namespace std;

//MFQS::MFQS() = default;

//q = number of queues
//t = time quantum, in loop cycles
//p = first process in the input list

//q_arr[q_count] = queue for I/O-suspended processes

//MFQS::MFQS(int q, int t, Process *p){
//MFQS::MFQS(int q, int t, vector<Process*> *n){
MFQS::MFQS(int qc, int tq, int iop, int ap, vector<Process*> *vnp){

	// Initialize the clock, the new-process queue, and 
	//user-defined variables.
	clock = 0;
	q_count = qc;
	time_quantum = tq;
	io_point = iop;
	age_point = ap;
	vect_new_procs = vnp;
	
	// Determine the size of the output string array
	time_quantum_max = (int) time_quantum * pow( 2, q_count-1 );
	
	//cout << "time_quantum_max: " << time_quantum_max << endl; // Debug

	// Create the array used to hold the queue vector pointers.
	qv_arr = new vector<Process*>*[q_count+1];
	tq_arr = new int[q_count];
	os_arr = new string[time_quantum_max];
	
	
	// Create the queue vectors.
	for( int i = 0; i <= q_count; i++ ){
		qv_arr[i] = new vector<Process*>();
		qv_arr[i]->reserve( vect_new_procs->capacity() );	
		tq_arr[i] = time_quantum * pow( 2, i ); 
	}

}

// Set up the simulation and call the primary method.
int MFQS::start(){
	cout << "Starting simulation. " << endl;
	current_process = vect_new_procs->front();
	pop_heap( vect_new_procs->begin(), vect_new_procs->end(), comp_proc );
	vect_new_procs->pop_back();

	current_process->Progress = 0;
	current_process->Age = 0;
	current_process->QLevel = 0;
	
	clock = current_process->Arrival;
	cout << "C" << to_string( clock ) << ": Process " << to_string( current_process->P_ID ) << " has been submitted." << endl;
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

	Process *temp_proc; // A temporary Process pointer.
	vector<Process*> *temp_queue; // A temporary vector<Process*> pointer.

	//int loop_counter = 0;
	while( current_process != nullptr ){
		/*
		loop_counter++;
		if( loop_counter > 25 ){
			return;
		}
		*/
		
		// Calculate the step time.
		cout << "C" << to_string( clock_step ) << ": Process " << to_string( current_process->P_ID ) << " has begun executing." << endl;
		
		// If the process has IO it has not performed and the user-defined 
		//time quantum I/O point is less than the process's Burst...
		if( current_process->IO_Done == 0 && io_point < current_process->Burst ){
			step = io_point;
			
		// Else, the step is the lesser of the queue-specific time quantum and the process's remaining burst.
		}else{		
			time_quant_by_queue = tq_arr[current_process->QLevel];
			if( (time_quant_by_queue + current_process->Progress) < current_process->Burst ){
				step = time_quant_by_queue;
			}else{
				step = current_process->Burst - current_process->Progress;
			}
		}
		
		clock_step = clock+1;
		clock += step;
		current_process->Progress += step;
		current_process->Age = clock;

		// Iterate through the step's individual cycles.
		while( clock_step <= clock ){

			// Check for new processes.
			// While the front process's Arrival is equal to the current clock step...
			while( vect_new_procs->size() != 0 && vect_new_procs->front()->Arrival == clock_step ){
				temp_proc = vect_new_procs->front();
				//cout << "Checking new processes..." << endl; // Debug
			
				// Remove the process from vect_new_procs.
				pop_heap( vect_new_procs->begin(), vect_new_procs->end(), comp_proc_arrival );
				vect_new_procs->pop_back();
			
				// Add it to queue 0.
				temp_proc->QLevel = 0;
				qv_arr[0]->push_back( temp_proc );
				
				cout << "C" << to_string( clock_step ) << ": Process " << to_string( temp_proc->P_ID ) << " has been submitted." << endl;
			}
			
			
			// Check on processes waiting for I/O.
			// While the front process's I/O completion time is equal to the current clock step...
			temp_queue = qv_arr[q_count];
			while( temp_queue->size() != 0 && temp_queue->front()->IO_Done == clock_step ){
				//cout << "Checking I/O..." << endl; // Debug
				temp_proc = temp_queue->front();
				temp_proc->Age = clock_step;
			
				// Remove process from I/O-Wait queue.
				pop_heap( temp_queue->begin(), temp_queue->end(), comp_proc_io_wait );
				temp_queue->pop_back();
			
				// Add process to the next ready queue.
				temp_queue = qv_arr[temp_proc->QLevel];
				temp_queue->push_back( temp_proc );
				push_heap( temp_queue->begin(), temp_queue->end(), comp_proc_age );

				cout << "C" << to_string( clock_step ) << ": Process " << to_string( temp_proc->P_ID ) <<" has finished its I/O and joined queue " << to_string( temp_proc->QLevel ) << "." << endl;
				
				temp_queue = qv_arr[q_count];
			}
			
			// Check lowest-priority ready queue.
			// If the simulation has more than 1 queue...
			if( 1 < q_count ){
			
				// While the front process's Age plus the user-dfined age-up threshold are equal to the current clock step...
				temp_queue = qv_arr[q_count-1];	
				while( temp_queue->size() != 0 && (temp_queue->front()->Age + age_point) == clock_step ){
					//cout << "Checking lowest queue..." << endl; // Debug
					temp_proc = temp_queue->front();
					temp_proc->Age = clock_step;
					temp_proc->QLevel--;
	
					// Remove process from lowest ready queue.
					pop_heap( temp_queue->begin(), temp_queue->end(), comp_proc_age );
					temp_queue->pop_back();
			
					// Add process to next-highest ready queue.
					temp_queue = qv_arr[temp_proc->QLevel];
					temp_queue->push_back( temp_proc );
					push_heap( temp_queue->begin(), temp_queue->end(), comp_proc_age );

					cout << "C" << to_string( clock_step ) << ": Process " << to_string( temp_proc->P_ID ) << " has been promoted from queue " << to_string( q_count-1 ) << " to queue " << to_string( q_count-2 ) << "." << endl;

					temp_queue = qv_arr[q_count-1];
				}
			}
			
			clock_step++;
		}			

		// Adjust queue level of current process.
		if( current_process->QLevel != q_count-1 ){
			current_process->QLevel++;
		}

		// If the current process has I/O to perform..
		if( current_process->IO_Done == 0 ){
			//cout << "Moving current process to I/O queue..." << endl; // Debug
			
			// Add process to I/O-Wait queue ( qv_arr[q_count] ), sorting on IO completion time.
			current_process->IO_Done = clock + current_process->IO;
			temp_queue = qv_arr[q_count];
			temp_queue->push_back( current_process );
			push_heap( temp_queue->begin(), temp_queue->end(), comp_proc_io_wait );

			cout << "C" << to_string( clock ) << ": Process " << to_string( current_process->P_ID ) << " is now waiting for I/O." << endl;
		
		}else 
		// If the current process is now complete...
		if( current_process->Progress >= current_process->Burst ){
			//cout << "Finishing current process..." << endl; // Debug
			// Collect statistical information.
			//+...

			// Destroy the Process object.
			//+...
			cout << "C" << to_string( clock ) << ": Process " << to_string( current_process->P_ID ) << " has completed." << endl;
			delete current_process;
		
		//Else, the process is unfinished and is to be returned to the appropriate ready queue.
		}else{
			//cout << "Queuing current process..." << endl; // Debug
			current_process->Age = clock;
			temp_queue = qv_arr[current_process->QLevel];
			temp_queue->push_back( current_process );
			push_heap( temp_queue->begin(), temp_queue->end(), comp_proc_age );

			cout << "C" << to_string( clock ) << ": Process " << to_string( current_process->P_ID ) << " has been moved to queue " << to_string( current_process->QLevel ) << "." << endl;
		}
		
		current_process =  nullptr;
		
		// Select the first process from the highest ready queue. 
		//cout << "Selecting next process..." << endl; // Debug
		q_ind = 0;
		while( qv_arr[q_ind]->size() == 0 && q_ind < q_count ){
			q_ind++;
		}
		
		// If q_ind is less than q_count, there is a process available in a ready queue...
		if( q_ind < q_count ){
			//cout << "Selecting from ready queue " << to_string( q_ind ) << "..." << endl; // Debug
			//cout << "Preparing ready process..." << endl; // Debug
			temp_queue = qv_arr[q_ind];
			
			current_process = temp_queue->front();
			
			pop_heap( temp_queue->begin(), temp_queue->end(), comp_proc_age );
			temp_queue->pop_back();

		// Else, there are no immediately ready processes. 
		
		}else{
			//cout << "Preparing wait jump..." << endl; // Debug
			
			// If there are no processes in the ready queues, the new process queue, or the I/O-Wait queue, then all processes
			//are finished and the simulation is complete.
			if( vect_new_procs->size() == 0 && qv_arr[q_count]->size() == 0 ){
				cout << "Simulation complete." << endl;
				return;
			}
			
			// Check the new process queue and the I/O-Wait queue for processes and 
			//select the one that will be ready soonest.
			if( vect_new_procs->size() != 0){
				current_process = vect_new_procs->front();
			}
			if( qv_arr[q_count]->size() != 0 && (current_process == nullptr || current_process->Arrival > qv_arr[q_count]->front()->IO_Done) ){
				current_process = qv_arr[q_count]->front();
			}
			
			// If the next-ready process is one that has not yet been submitted...
			if( current_process->Arrival > clock ){
				//cout << "Selecting from new..." << endl; // Debug
				
				// Set the clock to its arrival time and remove it from the new-process queue.
				clock = current_process->Arrival;
				pop_heap( vect_new_procs->begin(), vect_new_procs->end(), comp_proc_arrival );
				vect_new_procs->pop_back();

				cout << "C" << to_string( clock ) << ": Process " << to_string( current_process->P_ID ) << " has been submitted." << endl;
				
			// Else, the next-ready process is currently waiting for I/O
			}else{
				//cout << "Selecting from I/O..." << endl; // Debug
				
				// Set the clock to its ready time and remove it from the I/O-Wait queue.
				clock = current_process->IO_Done;
				temp_queue = qv_arr[q_count];
				pop_heap( temp_queue->begin(), temp_queue->end(), comp_proc_io_wait );
				temp_queue->pop_back();

				cout << "C" << to_string( clock ) << ": Process " << to_string( current_process->P_ID ) << " has finished its I/O and joined queue " << to_string( current_process->QLevel ) << "." << endl;
			}

		}
	
	}


}

// The original MFQS attempt. The output was retroactively generated and sorted after the 
//step was calculated and then appended to an array of strings, where the index was the step number at which the event was determined to have ocurred. However, 
//the string array quickly overran the program's alloted memory at higher time quanta, and thus higher step widths, so the attempt was abandoned. 
void MFQS::cycle(){
	int step;
	int time_quant_by_queue;
	int q_ind;

	Process *temp_proc;
	vector<Process*> *temp_queue;

	int loop_counter = 0;

	while( current_process != nullptr ){
		/*
		loop_counter++;
		if( loop_counter > 20 ){
			return;
		}
		*/
		
		// Calculate the step time.
		// Assumes that the user-specified time quantum IO point is universal, and IO is only performed once for each process.
		//Thus, if a process has IO, it is always performed during its first run as the currently executing process.
		
		if( current_process->IO_Done == 0 && io_point < current_process->Burst ){
			step = io_point;
		}else{
			
			time_quant_by_queue = tq_arr[current_process->QLevel];
			if( (time_quant_by_queue + current_process->Progress) < current_process->Burst ){
				step = time_quant_by_queue;
			}else{
				step = current_process->Burst - current_process->Progress;
			}
			//cout << "tqq: " << to_string( time_quant_by_queue ) << endl; // Debug
			//cout << " " << to_string( time_quant_by_queue ) << endl; // Debug
			
		}
		
		
		clock += step;
		current_process->Progress += step;
		current_process->Age = clock;
		
		swap( os_arr[0], os_arr[step-1] );
		
		os_arr[step-1].append( " Process " + to_string( current_process->P_ID ) + " has begun executing. \n" );

		// Check for new processes.
		while( vect_new_procs->size() != 0 && vect_new_procs->front()->Arrival <= clock ){
			temp_proc = vect_new_procs->front();
			//cout << " New process: " + to_string( temp_proc->P_ID ) + "\n";
			
			// Remove the process from vect_new_procs.
			pop_heap( vect_new_procs->begin(), vect_new_procs->end(), comp_proc_arrival );
			vect_new_procs->pop_back();
			
			// Add it to queue 0.
			temp_proc->QLevel = 0;
			qv_arr[0]->push_back( temp_proc );

			//Add output string to os_arr.
			os_arr[clock - temp_proc->Arrival].append( " Process " + to_string( temp_proc->P_ID ) + " has been submitted. \n" );
			
		}

		
		// Check on processes waiting for I/O.
		temp_queue = qv_arr[q_count];
		//while( temp_queue->size() != 0 && clock - temp_queue->front()->IO_Done >= 0 ){
		while( temp_queue->size() != 0 && temp_queue->front()->IO_Done <= clock ){
			cout << "Checking I/O..." << endl; // Debug
			temp_proc = temp_queue->front();
			temp_proc->Age = temp_proc->IO_Done;
			
			// Remove process from I/O-Wait queue.
			pop_heap( temp_queue->begin(), temp_queue->end(), comp_proc_io_wait );
			temp_queue->pop_back();
			
			// Add process to the next ready queue.
			temp_queue = qv_arr[temp_proc->QLevel];
			temp_queue->push_back( temp_proc );
			push_heap( temp_queue->begin(), temp_queue->end(), comp_proc_age );
			
			// Add output string to os_arr.
			os_arr[clock - (temp_proc->IO_Done)].append( " Process " + to_string( temp_proc->P_ID ) + " has finished its I/O and joined queue " + to_string( temp_proc->QLevel ) + ". \n" ); 
		}
		
		
		// Check on processes in lowest ready queue.
		if( 1 < q_count ){
			
			temp_queue = qv_arr[q_count-1];	
			//while( temp_queue->size() != 0 && clock - (temp_queue->front()->Age + age_point) >= 0 ){
			while( temp_queue->size() != 0 && (temp_queue->front()->Age + age_point) <= clock ){
				cout << "Checking lowest queue..." << endl; // Debug
				temp_proc = temp_queue->front();
				temp_proc->Age += age_point;
				temp_proc->QLevel--;
	
				// Remove process from lowest ready queue.
				pop_heap( temp_queue->begin(), temp_queue->end(), comp_proc_age );
				temp_queue->pop_back();
			
				// Add process to next-highest ready queue.
				temp_queue = qv_arr[temp_proc->QLevel];
				temp_queue->push_back( temp_proc );
				push_heap( temp_queue->begin(), temp_queue->end(), comp_proc_age );
			
				// Add output string to os_arr.
				os_arr[clock - (temp_proc->Age)].append(" Process " + to_string( temp_proc->P_ID ) + " has been promoted from queue " + to_string( q_count-1 ) + " to queue " + to_string( q_count-2 ) + ". \n" );
			
				temp_queue = qv_arr[q_count-1];
			}
		}


		// Adjust queue level of current process.
		if( current_process->QLevel != q_count-1 ){
			current_process->QLevel++;
		}

		if( current_process->IO_Done == 0 ){
			//cout << "Moving current process to I/O queue.." << endl; // Debug
			// Add process to I/O-Wait queue ( qv_arr[q_count] ), sorting on IO wait.
				current_process->IO_Done = clock + current_process->IO;
				
				temp_queue = qv_arr[q_count];
				temp_queue->push_back( current_process );
				push_heap( temp_queue->begin(), temp_queue->end(), comp_proc_io_wait );

				os_arr[0].append( " Process " + to_string( current_process->P_ID ) + " is now waiting for I/O. \n" );
		
		}else 
		if( current_process->Progress >= current_process->Burst ){
			//cout << "Finishing current process..." << endl; // Debug
			// Collect statistical information.
			//+...
			

			
			try{
				os_arr[0].append( " Process " + to_string( current_process->P_ID ) + " has completed. \n" );
			} catch( const length_error& le ){
				cout << "Length error." << endl;
				
				cout << " " << to_string( os_arr[0].size() );
				//cout << "-----\n" << os_arr[0] << endl;
				
				return;
			}catch( const bad_alloc& ba ){
				cout << "Bad alloc." << endl;
				cout << " " << to_string( os_arr[0].size() );
				return;
			}
			// Destroy the Process object.
			//+...
			delete current_process;
		
		}else{
			// Add current process to appropriate queue.
			//cout << "Moving current process..." << endl; // Debug
			temp_queue = qv_arr[current_process->QLevel];
			temp_queue->push_back( current_process );
			push_heap( temp_queue->begin(), temp_queue->end(), comp_proc_age );
				
			//cout << "Setting output(" + to_string( step ) + ")..." << endl; // Debug
			os_arr[0].append( " Process " + to_string( current_process->P_ID ) + " has been moved to queue " + to_string( current_process->QLevel ) + ". \n" );
		}
		
		// Print and clear output.
		//cout << "Printing output..." << endl; // Debug
		for( int i = time_quantum_max-1; i >= 0; i-- ){	
			if( os_arr[i].size() != 0 ){
				//cout << " Printing line..." << endl; // Debug
				//cout << "Clock " + to_string( clock-(i+1) ) + "\n" +  os_arr[i];
				os_arr[i].clear();
			}
		}
		
		current_process =  nullptr;
		// Select first process from highest ready queue. 
		//cout << "Selecting next process..." << endl; // Debug
		q_ind = 0;
		while( qv_arr[q_ind]->size() == 0 && q_ind < q_count ){
			q_ind++;
		}
		
		// If there is a process ready to execute...
		if( q_ind < q_count ){
			//cout << "Selecting from ready queue " << to_string( q_ind ) << "..." << endl; // Debug
			//cout << "Preparing ready process..." << endl; // Debug
			temp_queue = qv_arr[q_ind];
			
			current_process = temp_queue->front();
			
			pop_heap( temp_queue->begin(), temp_queue->end(), comp_proc_age );
			temp_queue->pop_back();
			
			//os_arr[0].append( "Process " + to_string( current_process->P_ID ) + " has begun executing. \n" );
		
		
		
		// Else, there are no immediately ready processes.
		}else{
			//cout << "Preparing wait jump..." << endl; // Debug
			if( vect_new_procs->size() == 0 && qv_arr[q_count]->size() == 0 ){
				//cout << "Simulation complete." << endl; // Debug
				return;
			}
			
			if( vect_new_procs->size() != 0){
				current_process = vect_new_procs->front();
			}
			
			if( qv_arr[q_count]->size() != 0 && (current_process == nullptr || current_process->Arrival > qv_arr[q_count]->front()->IO_Done) ){
				//cout << "Set c_p to I/O head..." << endl; // Debug
				current_process = qv_arr[q_count]->front();
			}
			
			// If the next-ready process is one that has not yet been submitted...
			if( current_process->Arrival > clock ){
				//cout << "Selecting from new..." << endl; // Debug
				clock = current_process->Arrival;
				pop_heap( vect_new_procs->begin(), vect_new_procs->end(), comp_proc_arrival );
				vect_new_procs->pop_back();
				
				//os_arr[clock - current_process->Arrival].append( "Process " + to_string( current_process->P_ID ) + " has been submitted. \n" );
				os_arr[0].append( " Process " + to_string( current_process->P_ID ) + " has been submitted. \n" );
				//cout << " Process " << to_string( current_process->P_ID ) << " has been submitted." << endl;
				
			// Else, the next-ready process is currently waiting for I/O
			}else{
				//cout << "Selecting from I/O..." << endl; // Debug
				clock = current_process->IO_Done;
				temp_queue = qv_arr[q_count];
				pop_heap( temp_queue->begin(), temp_queue->end(), comp_proc_io_wait );
				temp_queue->pop_back();
				
				//os_arr[clock - (current_process->IO_Done)].append( "Process " + to_string( current_process->P_ID ) + " has finished its I/O and joined queue " + to_string( current_process->QLevel ) + ". \n" );
				os_arr[0].append( " Process " + to_string( current_process->P_ID ) + " has finished its I/O and joined queue " + to_string( current_process->QLevel ) + ". \n" );
				//cout << " Process " << to_string( current_process->P_ID ) << " has finished its I/O and joined queue " << to_string( current_process->QLevel ) << ". " << endl; 
			}

		}
		//cout << endl;

	}

}








