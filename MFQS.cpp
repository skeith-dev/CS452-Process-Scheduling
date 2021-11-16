//
// Created by Spencer Keith on 11/3/21.
//--
//Need to handle potential failure of 'new' calls.

#include <iostream>
#include<vector>
#include<algorithm>

#include "MFQS.h"

using namespace std;

//q = number of queues
//t = time quantum, in loop cycles
//p = first process in the input list

//q_arr[q_count] = queue for I/O-suspended processes

//MFQS::MFQS(int q, int t, Process *p){
MFQS::MFQS(int q, int t, vector<Process*> *n){

	clock = 0;
	
	q_count = q;
	time_quantum = t;
	vect_new_procs = n;
	
	//next_arrival = p;
	
	//Process *array[q_count+1];
	//q_arr = array;
	
	//vector<Process> *qv_arr[q_count+1];
	
	qv_arr = new vector<Process*>*[q_count+1];
	
	for( int i = 0; i <= q_count; i++ ){
	
		qv_arr[i] = new vector<Process*>();
		qv_arr[i]->reserve( vect_new_procs->capacity() );
		
	}
	
	// Begin Test
	
	cout << "- vect_new_procs 1-3: " << endl;

	Process *p1 = vect_new_procs->front();
	cout << "	" << p1->P_ID << endl;
	pop_heap( vect_new_procs->begin(), vect_new_procs->end() );
	vect_new_procs->pop_back();
	
	Process *p2 = vect_new_procs->front();
	cout << "	" << p2->P_ID << endl;
	pop_heap( vect_new_procs->begin(), vect_new_procs->end() );
	vect_new_procs->pop_back();
	
	Process *p3 = vect_new_procs->front();
	cout << "	" << p3->P_ID << endl;
	pop_heap( vect_new_procs->begin(), vect_new_procs->end() );
	vect_new_procs->pop_back();
	
	qv_arr[0]->push_back( p3 );
	push_heap( qv_arr[0]->begin(), qv_arr[0]->end() );
	
	qv_arr[0]->push_back( p2 );
	push_heap( qv_arr[0]->begin(), qv_arr[0]->end() );
	
	qv_arr[0]->push_back( p1 );
	push_heap( qv_arr[0]->begin(), qv_arr[0]->end() );
	
	cout << "qv_arr[0]: " << qv_arr[0]->front()->P_ID << endl;
	
	
	//End Test
	
	
	
	
	//Remove the first process from the highest-level ready queue, set it to current_process, and call cycle()
	

}

void cycle(){
	
//while there are processes to complete...
//while( current_process != NULL ){
	
	// Current_process->Burst may be replaced with some other progress counter, so we can more easily preserve the Burst value for information purposes.
	// This outline assumes granularity on the level of the time quantum, rather than individual cycles
	
	/*
	
	// Sets the time for the next step in the simulation to be either I/O initiation, process completion, or the full amount of the time quantum.
	// IO should be excluded from this calculation if the user has indicated that no IO should be performed, or the process's IO parameter is 0.
	step = minimum( time quantum IO point, current_process->Burst, time_quantum * (2^queue level) );
	
	clock += step;
	current_process->Burst -= step;
	
	*/
	
	
	/*
	// A process will need to be moved if:
	//	next_arrival->Arrival <= clock
	//	time suspended for I/O exceeds time required for I/O
	//	time in lowest queue exceeds user-specified threshold
	//	It is the current process and has not yet completed
	// Process are moved in order of decreasing differential between current clock and time spent waiting (for 
	//example, if p1->age - clock = -1 and p2->age - clock = 0, p1 is moved and then p2). Ties are broken by 
	//priority, so the list of processes to be moved will need to be sorted by time differential and then priority.
	
	// Here, q_time is the clock value when the process was placed in its current queue
	
	Process proc = first in next_arrival list;
	while( proc->arrival < clock ){
		proc->QLevel = 0;
		add proc to q_move_list;
		proc = next proc;
	}
	
	proc = first in q_arr[q_count];
	while( clock - proc->q_time  >= proc->I/O ){
		proc->QLevel++;
		add proc to q_move_list;
		proc = next in q_arr[q_count];
	}
	
	proc = first in q_arr[q_count-1]
	while( clock - proc->q_time >= wait threshold ){
		proc->QLevel--;
		add proc to q_move_list;
		proc = next in q_arr[q_count];
	}
	
	if( step = current_process->I/O ){
		current_process->q_time = clock;
		move process to q_move_list;
	}else if( current_process->Burst == 0 ){
		process is complete - gather statistical information and destroy the Process object.
	}
	
	
	for( each process in q_move_list ){
		move each process to its appropriate queue;
	}
	
	current_process = first in the highest non-empty queue.
	
//}	
	*/
	
	
	
	
}


/*

queues : sort
-----------------
new arrivals: arrival time

ready queues: priority(?)

I/O: I/O time

Assumptions:
	Processes in the lowest-level ready queue will be naturally sorted by age. That is, if the first process in the queue is P_n, the second is P_n-1, and so on, then
P_n will have an age greater than or equal to P_n-1, P_n-1 will have an age greater than or equal to P_n-2, and so on.
	Processes may be inserted into positions that are not at the front or end of the relevant queue or list, but will only ever be removed from the front.





*/


