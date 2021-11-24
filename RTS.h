//
// Created by Spencer Keith on 11/18/21.
//
 
#ifndef RTS_RTS_H
#define RTS_RTS_H

#include <vector>

#include "Process.h"
#include "Statistics.h"


class RTS {

	bool do_stats;	
    bool hard_RTS;
    unsigned long clock{};
	Statistics *stats;
    vector<Process*> *processes_vector;
    Process *current_process{};

public:
    explicit RTS(vector<Process*> **processes_vector, bool hard_RTS, Statistics *stat_tracker);
    void start();
    void cycle();

};


#endif //RTS_RTS_H
