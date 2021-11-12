
#ifndef LAB2_PROCESS_H
#define LAB2_PROCESS_H

#include<algorithm>
#include<vector>
#include<iostream>

#define SORT_ARRIVAL_TIME 0 
#define SORT_IO_WAIT 1 
#define SORT_DEADLINE 2

using namespace std;

class Process{
public:
	
	int P_ID;	// ID of the process.
	int Burst;	// Total processor time needed by the process.
	int Progress;	// Current processor time used by the process.
	int Arrival;	// Arrival time, measured in clock cycles.
	int Init_Time;	// Time placed in the initial ready queue, measured in clock cycles.
	int Priority;	// Process priority.
	int Deadline;	// Process deadline, in clock cycles.
	int IO;		// Time needed for I/O, in clock cycles.
	int Age;	// Age of the process in clock cycles; used when measuring time spent waiting.
	int QLevel;	// Specifies which queue a process is in.
	int Sort_Type;
	
	Process();
	Process( int p );
	
	bool operator < ( Process &other );
	
	
	
	
};

	int insert_sorted( vector<Process*> *proc_vect, Process *proc, int flag );
	//int insert_unsorted( vector<Process*> *proc_vect, Process *proc );
	
	bool comp_proc( Process *proc_1, Process *proc2 );
	
	
	

#endif