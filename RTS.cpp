// 
// Created by Spencer Keith on 11/18/21.
//

#include <iostream>
#include <vector>

#include "RTS.h"
#include "Process.h"
#include "Statistics.h"

//#define PER_CLOCK_OUT

using namespace std;

RTS::RTS(vector<Process*> **processes_vector, bool hard_RTS, Statistics *stat_tracker) {
	
	if( stat_tracker == nullptr ){
		this->do_stats = false;
	}else{
		this->do_stats = true;
		this->stats = stat_tracker;
	}
	
    this->processes_vector = *processes_vector;
    this->hard_RTS = hard_RTS;

}

void RTS::start() {

    cout << "Starting simulation:" << endl;

    current_process = this->processes_vector->front();
    clock = current_process->arrival;

    pop_heap( processes_vector->begin(), processes_vector->end(), compare_process_arrival );
    processes_vector->pop_back();

    //cout << "Process " << to_string( current_process->process_ID ) << " has been submitted" << endl;


    cycle();

}

void RTS::cycle() {

    Process *temp_process;
    auto *processes_queue = new vector<Process*>;
	
	int procs_completed = 0;
	int procs_failed = 0;
	
	#ifdef PER_CLOCK_OUT
	cout << "Beginning Process " << current_process->process_ID << " at C" << clock << endl;
	#endif
	
	if( do_stats ){
		stats->add_stat(current_process->process_ID,"processstart",(long int) clock);
	}

    do {

        //iterate the progress of the current process by one if
        //it's arrival is less than or equal to the clock
        if(current_process != nullptr && current_process->arrival <= clock) {
            current_process->progress++;
			if( do_stats ){
				stats->add_stat(current_process->process_ID,"cputime",(long int) 1);
			}
        }

        //iterate the clock by 1 unit
        clock++;

        //if the RTS is of hard type and the clock minus the arrival of the current process is greater than
        //the deadline of the current process, the deadline has expired and the process is deleted
        if(hard_RTS && current_process != nullptr && clock - current_process->arrival > current_process->deadline) {
            cout << "Process " << current_process->process_ID << " was not completed before its deadline - ABORT" << endl;
            delete current_process;
            current_process = nullptr;
        }

        //if the progress of the current process is equal to
        //the burst of the current process, delete the current
        //process and set the variable to a nullptr
        if(current_process != nullptr && current_process->progress == current_process->burst) {
			
			#ifdef PER_CLOCK_OUT
            cout << "Completed Process " << current_process->process_ID << " at C" << clock;
			#endif
			
            if(clock - current_process->arrival > current_process->deadline) {
                #ifdef PER_CLOCK_OUT
				cout << "... after the deadline(" << current_process->deadline << ") expired - LATE!" << endl;
				#endif
				procs_failed++;
            } else {
				#ifdef PER_CLOCK_OUT
                cout << "... before the deadline(" << current_process->deadline << ") expired - ON TIME!" << endl;
				#endif
				procs_completed++;
			}

			if( do_stats ){
				stats->add_stat(current_process->process_ID,"processend",(long int) clock);
			}
			
            delete current_process;
            current_process = nullptr;
        }

        //while the arrival time of the front process of the processes_vector is equal
        //to the clock, move the process to the process_queue in order of deadline
        while(!processes_vector->empty() && processes_vector->front()->arrival == clock) {	
		
		
            //pop the process from the processes_vector
			temp_process = processes_vector->front();
            pop_heap(processes_vector->begin(), processes_vector->end(), compare_process_arrival);
            processes_vector->pop_back();
            //push the process to the processes_queue
			processes_queue->push_back(temp_process);
            push_heap(processes_queue->begin(), processes_queue->end(), compare_process_deadline);
			
			if( do_stats ){
					stats->add_stat(temp_process->process_ID,"processstart",(long int) clock);
				}
        }

        //if the processes_queue is not empty and the front process of the processes_queue
        //has a deadline less than the deadline of the current process or the current process is a nullptr,
        //set the current process to the front process of the processes_queue, pop the process from the processes_queue,
        //and push the former current process to the processes_queue
		if( !processes_queue->empty() && ( current_process == nullptr || compare_process_deadline(processes_queue->front(), current_process) ) ) {	
			
			// The if branch here was entered if current_process was null, so setting temp_process to current_process and then pushing temp_process to 
			//the queue meant trying to dereference a null pointer in that case. Also, pop_back() wascalled on processes_vector instead of processes_queue.
			// temp_process is now checked for null status before it is pushed to the queue.
			
            temp_process = current_process;
            current_process = processes_queue->front();
            pop_heap(processes_queue->begin(), processes_queue->end(), compare_process_deadline);
            processes_queue->pop_back();		
			if( temp_process != nullptr ){
				processes_queue->push_back(temp_process);
				push_heap(processes_queue->begin(), processes_queue->end(), compare_process_deadline);
			}    
        }


        //begin or resume process, according to its progress value
        if(current_process != nullptr) {
            if(current_process->progress == 0) { 
                #ifdef PER_CLOCK_OUT
				cout << "Beginning Process " << current_process->process_ID << " at C" << clock << endl;
				#endif
            } else {
				#ifdef PER_CLOCK_OUT
				cout << "Resuming Process " << current_process->process_ID << " at C" << clock << endl;
				#endif
			}
        }

    } while(current_process != nullptr || !processes_vector->empty() || !processes_queue->empty());

	cout << to_string( procs_completed ) << " processes completed." << endl;
	cout << to_string( procs_failed ) << " processes failed." << endl;

}