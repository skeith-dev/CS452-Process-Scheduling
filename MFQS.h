 //
// Created by Spencer Keith on 11/3/21.
//



#ifndef LAB2_MFQS_H
#define LAB2_MFQS_H

#include <cmath>
#include <vector>

#include "Process.h"
#include "Statistics.h"

using namespace std;



class MFQS {

	unsigned long clock;	// Tracks the total number of cycles that have passed in the simulation.
	int time_quantum;	// The time quantum used for process execution, in clock cycles.
	int io_point;	// The point in the time quantum at which to perform I/O.
	int age_point;	// The number of cycles a process spends in the lowest-priority queue before aging up.
	int step;	// The number of cycles between the current simulation step and the next. 
	int q_count;	// The number of process queues.
	int **q_arr;	// A pointer to an array that stores the process queues. Its total size is equal to q_count+1. q_arr[q_count] stores processes suspended for I/O.
	int *tq_arr;	// A pointer to an array that stores the pre-calculated time quanta for each queue. 
	int time_quantum_max;
	bool do_stats;
	Statistics *stats;
	Process *current_process;	// The process currently in the processor.

	vector<Process*> *vect_new_procs;
	vector<Process*> **qv_arr;

public:
    MFQS();
	MFQS( int qc, int tq, int iop, int ap, vector<Process*> *vnp, Statistics *stat_tracker);
	
	// Main method of the class.
	int start();
	
	//void cycle(); 
	void cycle2();

};

#endif //LAB2_MFQS_H
