//
// Created by Spencer Keith on 11/3/21.
//

#ifndef LAB2_RTS_H
#define LAB2_RTS_H

#include <vector>
#include "Process.h"


class RTS {

    int clock;	// Tracks the total number of cycles that have passed in the simulation.
    int time_quantum;	// The time quantum used for process execution, in clock cycles.
    int queues_count;	// The number of process queues.
    int **q_arr;	// A pointer to an array that stores the process queues. Its total size is equal to q_count+1. q_arr[q_count] stores processes suspended for I/O.

    Process *next_arrival;	// The next process to arrive.
    Process *current_process;	// The process currently in the processor.
    Process *q_move_list;	// A list of processes that need to be moved to a queue, either from its current queue or from the processor.

public:
    RTS( int queues, int time_quantum, vector<Process*> *processes );

};


#endif //LAB2_RTS_H
