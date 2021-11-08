

#include "Process.h"

using namespace std;

//Test Operator 
bool Process::operator < ( Process &other ){
	
	if( this->P_ID < other.P_ID ){
		return false;
	}
	
	return true;
}

Process::Process( int p ){
	P_ID = p;
}


int insert_sorted( vector<Process> *proc_vect, Process *proc, int flag ){
	proc->Sort_Type = flag;
	
	


	return 0;
}



