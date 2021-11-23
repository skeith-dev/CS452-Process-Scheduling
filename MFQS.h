 //
// Created by Spencer Keith on 11/3/21.
//

/*
--- 500k ---

Original
1: 3.956
2: 4.045
3: 3.917
4: 3.915
5: 4.024
A: 3.971

determine next process via caching queue indices
1: 2.578
2: 2.485
3: 2.489
4: 2.533
5: 2.537
A: 2.524

step calc reorder, end-of-step check reorder
1: 2.524
2: 2.522
3: 2.519
4: 2.504
5: 2.526
A: 2.519

++x -> x = x + 1
1: 2.562
2: 2.529
3: 2.505
4: 2.505
5: 2.543
A: 2.529

--- 1m ---
1: 5.639
2: 5.445
3: 5.364
4: 5.385
5: 5.476
6: 5.449
7: 5.738
8: 5.696
9: 5.686
10: 5.640
A: 5.552

*/

#ifndef LAB2_MFQS_H
#define LAB2_MFQS_H

#include <cmath>
#include <vector>

#include "Process.h"

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

	Process *current_process;	// The process currently in the processor.

	vector<Process*> *vect_new_procs;
	vector<Process*> **qv_arr;
	

public:
    MFQS();
	MFQS( int qc, int tq, int iop, int ap, vector<Process*> *vnp );
	
	// Main method of the class.
	int start();
	
	//void cycle(); 
	void cycle2();

};


#endif //LAB2_MFQS_H
