
#ifndef LAB2_PROCESS_H
#define LAB2_PROCESS_H

#include<algorithm>
#include<vector>
#include<iostream>
#include<string>

#define SORT_ARRIVAL_TIME 0 
#define SORT_IO_WAIT 1 
#define SORT_AGE 3
#define SORT_DEADLINE 2


using namespace std;

class Process{
public:
	
	int P_ID;	// ID of the process.
	int Burst;	// Total processor time needed by the process.
	int Progress;	// Current processor time used by the process.
	int Arrival;	// Arrival time, measured in clock cycles.
	int Priority;	// Process priority.
	int Deadline;	// Process deadline, in clock cycles.
	int IO;		// Time needed for I/O, in clock cycles.
	int Age;	// Age of the process in clock cycles; used when measuring time spent waiting.
	int QLevel;	// Specifies which queue a process is in.
	int IO_Done;	// Is initially 0 if the process has an I/O operation it has not performed, or -1 if it has no I/O. 
	int Sort_Type;
	
	//string P_ID_str;
	
	Process();
	Process( int p );
	
	bool operator < ( Process &other );
	
	
	
	
};

	int insert_sorted( vector<Process*> *proc_vect, Process *proc, int flag );

	bool comp_proc( Process *proc_1, Process *proc2 );
	
	bool comp_proc_arrival( Process *proc_1, Process *proc2 );
	bool comp_proc_io_wait( Process *proc_1, Process *proc2 );
	bool comp_proc_age( Process *proc_1, Process *proc2 );
	

#endif

/*
Outline of information retained by the Process class:
	The Burst, Arrival, and IO fields are filled in when the Process object in constructed and never altered, so 
these direcly reflect the corresponding fields in the input provided by the user.

	The IO_Done field is given its final value when the process is placed in the I/O-Wait queue. This final value is
the time that the process completed its I/O, so the time that I/O was initiated can be obtaiend by subtracting the IO value 
from the IO_Done value.
	
	Completion time can be gathered before the object is destroyed by checking the current clock value. Total turnaround 
time, then, is obtained with subtracting the Arrival value from the completion time, and total time spent waiting is obtained
by subtracting the Burst value from the turnaround time.


*/