

#include "Process.h"

using namespace std;


bool Process::operator < ( Process &other ){
	cout << "sort1" << endl;
	

	if( this->Sort_Type == SORT_ARRIVAL_TIME ){	
		//cout << "sort2" << endl;
		return (this->Arrival < other.Arrival || 
				this->Priority > other.Priority);
		
	}else
	if( this->Sort_Type == SORT_IO_WAIT ){
		return (this->Age < other.Age ||
				this->Priority > other.Priority);
		
	}else
	if( this->Sort_Type == SORT_DEADLINE ){
		return (this->Deadline < other.Deadline ||
				this->Priority > other.Priority);
	}else{
		cout << "Sort error" << endl;
		return false;
	}


}

Process::Process( int p ){
	P_ID = p;
}


int insert_sorted( vector<Process*> *proc_vect, Process *proc, int flag ){
	proc_vect->push_back( proc );
	if( flag == SORT_ARRIVAL_TIME ){
		push_heap( proc_vect->begin(), proc_vect->end(), comp_proc_arrival );
	}

	return 0;
}


bool comp_proc( Process *proc_1, Process *proc_2 ){

	if( proc_1->Sort_Type == SORT_ARRIVAL_TIME ){
		return (proc_1->Arrival > proc_2->Arrival || 
				(proc_1->Arrival == proc_2-> Arrival && proc_1->Priority > proc_2->Priority) );
	}else
	if( proc_1->Sort_Type == SORT_IO_WAIT ){
		return (proc_1->Age > proc_2->Age ||
				(proc_1->Age == proc_2->Age && proc_1->Priority > proc_2->Priority) );
	}else
	if( proc_1->Sort_Type == SORT_DEADLINE ){
		return (proc_1->Deadline > proc_2->Deadline ||
				(proc_1->Deadline == proc_2->Deadline && proc_1->Priority > proc_2->Priority) );
	}else{
		cout << "Sort error" << endl;
		return false;
	}
}

bool comp_proc_arrival( Process *proc_1, Process *proc_2 ){
	return (proc_1->Arrival > proc_2->Arrival || 
			(proc_1->Arrival == proc_2-> Arrival && proc_1->Priority > proc_2->Priority) );
}

bool comp_proc_io_wait( Process *proc_1, Process *proc_2 ){
	return (proc_1->IO_Done > proc_2->IO_Done ||
			(proc_1->IO_Done == proc_2->IO_Done && proc_1->Priority > proc_2->Priority) );

}

bool comp_proc_age( Process *proc_1, Process *proc_2 ){
	return (proc_1->Age > proc_2->Age ||
			(proc_1->Age == proc_2->Age && proc_1->Priority > proc_2->Priority) );
}




/*

bool comp_proc_io_wait( Process *proc_1, Process *proc2 ){
	return (proc_1->Age + proc_1->IO > proc_2->Age + proc_2->IO ||
			(proc_1->Age + proc_1->IO == proc_2->Age + proc_2->IO && proc_1->Priority > proc_2->Priority) );

}




bool comp_proc( Process *proc_1, Process *proc_2 ){
	//cout << "sort1" << endl;
	
	if( proc_1->Sort_Type == SORT_ARRIVAL_TIME ){
		
		cout << proc_1->Arrival << " : ";
		cout << proc_2->Arrival << endl;
		
		if( proc_1->Arrival < proc_2->Arrival || 
			proc_1->Priority > proc_2->Priority ){
			
			cout << " True" << endl;
		
		}else{
			cout << " False" << endl;
		}
		
		
		
		return (proc_1->Arrival < proc_2->Arrival || 
				proc_1->Priority > proc_2->Priority);
		
	}else
	if( proc_1->Sort_Type == SORT_IO_WAIT ){
		return (proc_1->Age < proc_2->Age ||
				proc_1->Priority > proc_2->Priority);
		
	}else
	if( proc_1->Sort_Type == SORT_DEADLINE ){
		return (proc_1->Deadline < proc_2->Deadline ||
				proc_1->Priority > proc_2->Priority);
	}else{
		cout << "Sort error" << endl;
		return false;
	}
}
*/
