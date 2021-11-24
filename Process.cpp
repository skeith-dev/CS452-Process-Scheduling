

#include "Process.h"

using namespace std;

Process::Process() = default;

Process::Process(int processID, int arrival, int burst, int deadline) {

    this->process_ID = processID;
    this->arrival = arrival;
    this->burst = burst;
    this->deadline = deadline;
    this->progress = -1;
	
	
	this->priority = 0;
	this->IO = 0;
	this->age = 0;
	this->qlevel = 0;
	this->io_done = 0;
	

}

Process::Process( int processID, int burst, int arrival, int priority, int deadline, int IO ){
	
	this->process_ID = processID;
	this->burst = burst;	
	this->arrival = arrival;
	this->priority = priority;
	this->deadline = deadline;
	this->IO = IO;
	
	this->progress = 0;
	this->age = 0;
	this->qlevel = 0;
	this->io_done = 0;

}


int insert_sorted_1( vector<Process*> *proc_vect, Process *proc, int flag ){
	proc_vect->push_back( proc );
	if( flag == SORT_ARRIVAL_TIME ){
		push_heap( proc_vect->begin(), proc_vect->end(), comp_proc_arrival );
	}

	return 0;
}


void insert_sorted( vector<Process*> *process_vector, Process *process, int flag ) {
    process_vector->push_back( process );
    if( flag == SORT_ARRIVAL_TIME ) {
        push_heap( process_vector->begin(), process_vector->end(), compare_process_arrival );
    }
    if (flag == SORT_DEADLINE_TIME ) {
        push_heap( process_vector->begin(), process_vector->end(), compare_process_deadline );
    }
}



bool comp_proc_arrival( Process *proc_1, Process *proc_2 ){
	return (proc_1->arrival > proc_2->arrival || 
			(proc_1->arrival == proc_2-> arrival && proc_1->priority > proc_2->priority) );
}

bool comp_proc_io_wait( Process *proc_1, Process *proc_2 ){
	return (proc_1->io_done > proc_2->io_done ||
			(proc_1->io_done == proc_2->io_done && proc_1->priority > proc_2->priority) );

}

bool comp_proc_age( Process *proc_1, Process *proc_2 ){
	return (proc_1->age > proc_2->age ||
			(proc_1->age == proc_2->age && proc_1->priority > proc_2->priority) );
}


bool compare_process_arrival( Process *proc_1, Process *proc_2 ) {
    return ( proc_1->arrival > proc_2->arrival || 
			( proc_1->arrival == proc_2->arrival && proc_1->deadline > proc_2->deadline ) );
}

bool compare_process_deadline( Process *proc_1, Process *proc_2 ) {
    return ( proc_1->deadline > proc_2->deadline );
}