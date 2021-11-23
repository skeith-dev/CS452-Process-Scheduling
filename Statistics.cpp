//
// Created by Kevin Emery on 11/21/21.
//

#include <iostream>
#include <bits/stdc++.h>
using namespace std;
#include <iostream>
#include <list>
#include "Statistics.h"
#include <map>

//#define DEBUG

statistic::statistic(int program, string type, int value){
	this->program = program;
	this->type = type;
	this->value = value;
}

Statistics::Statistics(){
	std::list<statistic> statistics_holder = std::list<statistic>();
}
	
void Statistics::add_stat(int program, string type, int value){
	statistic *stat = new statistic(program, type, value);
	statistic stat_obj = statistic(* stat);
	statistics_holder.push_back(stat_obj);
};
	
void Statistics::get_stats(){
	
	int running_total_waittime = 0;
	int running_total_turnaroundtime = 0;
	int running_total_processstart = 0;
	int running_total_processend = 0;
	
	std::map<int, int> start_tracker = {};
	std::map<int, int> end_tracker = {};
	std::map<int, int> cpu_tracker = {};
	std::map<int, int> io_tracker = {};
	
	for (std::list<statistic>::const_iterator it = statistics_holder.begin(); it != statistics_holder.end(); ++it){
		
		#ifdef DEBUG
		std::cout << it->program;
		std::cout << "\t";
		std::cout << it->type;
		std::cout << "\t";
		std::cout << it->value;
		std::cout << "\n";
		#endif
		
		if(it->type == "cputime"){
			cpu_tracker[it->program] += it->value;
		}
		if(it->type == "iotime"){
			io_tracker[it->program] += it->value;
		}
		else if(it->type == "processstart"){
			running_total_processstart += 1;
			start_tracker[it->program] = it->value;
		}
		else if(it->type == "processend"){
			running_total_processend += 1;
			end_tracker[it->program] = it->value;
		}
	}
	
	map<int, int>::iterator it;
	for (it = end_tracker.begin(); it != end_tracker.end(); it++){
		int k = it->first;
		int v = it->second;
		if(start_tracker.count(k) == 1){
			running_total_turnaroundtime += end_tracker[k]-start_tracker[k];
			running_total_waittime += (end_tracker[k]-start_tracker[k])-cpu_tracker[k];
		}
	}
	
	std::cout << "Total Processes Ran: ";
	std::cout << running_total_processend;
	std::cout << "\n";
	
	std::cout << "Total Processes Attempted/Seen: ";
	std::cout << running_total_processstart;
	std::cout << "\n";
	
	std::cout << "Total Wait Time: ";
	std::cout << running_total_waittime;
	std::cout << "\n";
	
	float avg_waittime = (float)running_total_waittime/(float)running_total_processend;
	std::cout << "Average Wait Time: ";
	std::cout << avg_waittime;
	std::cout << "\n";
	
	std::cout << "Total Turnaround Time: ";
	std::cout << running_total_turnaroundtime;
	std::cout << "\n";
	
	float avg_turnaroundtime = (float)running_total_turnaroundtime/(float)running_total_processend;
	std::cout << "Average Turnaround Time: ";
	std::cout << avg_turnaroundtime;
	std::cout << "\n";
};
