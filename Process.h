
#ifndef LAB2_PROCESS_H
#define LAB2_PROCESS_H

#include<algorithm>
#include<vector>
#include<iostream>
#include<string>

#define SORT_ARRIVAL_TIME 0 
#define SORT_IO_WAIT 1 
#define SORT_AGE 3
#define SORT_DEADLINE_TIME 2

using namespace std;

class Process{

public:
	
	int process_ID;	// ID of the process.
	int burst;	// Total processor time needed by the process.	
	int arrival;	// Arrival time, measured in clock cycles.
	int priority;	// Process priority.
	int deadline;	// Process deadline, in clock cycles.
	int IO;		// Time needed for I/O, in clock cycles.
	
	int progress;	// Current processor time used by the process.
	int age;	// Age of the process in clock cycles; used when measuring time spent waiting.
	int qlevel;	// Specifies which queue a process is in.
	int io_done;	// Is initially 0 if the process has an I/O operation it has not performed, or -1 if it has no I/O. 

	Process();
	Process(int processID, int arrival, int burst, int deadline);
	Process( int processID, int burst, int arrival, int priority, int deadline, int IO );


};

int insert_sorted_1( vector<Process*> *proc_vect, Process *proc, int flag );
void insert_sorted( vector<Process*> *process_vector, Process *process, int flag );

bool comp_proc_arrival( Process *proc_1, Process *proc2 );
bool comp_proc_io_wait( Process *proc_1, Process *proc2 );
bool comp_proc_age( Process *proc_1, Process *proc2 );
	
bool compare_process_arrival( Process *proc_1, Process *proc_2 );
bool compare_process_deadline( Process *proc_1, Process *proc_2);

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