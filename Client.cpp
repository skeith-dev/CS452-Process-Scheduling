//
// Created by Spencer Keith on 11/3/21.
// --

#include <iostream>
#include <stdio.h>
#include<vector>
#include<algorithm>

#include "MFQS.h"
#include "RTS.h"
#include "Client.h"



using namespace std;


int main() {

	multi_feedback_queue();
    

// Get and validate all user input. 
	// All must be non-negative
	// All must be entries corresponding to valid options

	//get scheduling mode (MFQS or RTS) from user
	// mode setup (contained in classes?)
	//MFQS *mfqs = new MFQS;
    //RTS *rts = new RTS;
	
	//get time quantum
	// must be greater than 0
	
	//get whether or not processes should do I/O
	// if I/O is desired, get time quantum I/O point
	//	time quantum I/O point must be less than or equal to the time quantum
	
	//get input mode (interactive or file) from user
	
	 
	
	//	prepare input from file, or loop until user has given all input
	//	preparation involves validating individual parameters
	//		MFQS: P_ID Burst  Arrival   Priority I/O
	//		RTS: P_ID Burst  Arrival   Priority  Deadline I/O
	//		All values must be non-negative. 	
	//		May need to validate Burst and P_ID.	
	//	Probably best to create a Process object for each line as it is parsed, and add it to a linked list.
	//	The list should be sorted by process arrival time. We can either sort the entire list after all of the input is parsed, or
	//we can add each Process to the list in sorted order as it is created. Not sure which one would ultimately be preferable.

	

    return 0;

}

int multi_feedback_queue( /*MFQS *mfqs*/ ){
	
	
	//	Create vector for new processes and reserve capacity.
	vector<Process*> *vect_new_procs = new vector<Process*>;
	vect_new_procs->reserve( 4096 );
	
	
	//Begin Test Data
	
	
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
	
	
	
	
	//End Test Data
	
	
	MFQS *mfqs = new MFQS( q_count, time_quantum, vect_new_procs );
	

	return 0;
}


int real_time(){


	return 0;
}
