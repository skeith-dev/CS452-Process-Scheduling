//
// Created by Kevin Emery on 11/21/21.
//

#ifndef LAB2_STATISTICS_H
#define LAB2_STATISTICS_H


#include <cmath>
#include <vector>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
#include <iostream>
#include <list>

#include "Process.h"

using namespace std;

class statistic {
	public:
		long int program;
		string type;
		long int value;
		statistic(long int, string, long int);
		void setValues(long int, string, long int);
};

class Statistics {

	std::list<statistic> statistics_holder;

	public:
		Statistics();
		void get_stats();
		void add_stat(long int program, string type, long int value);

};
#endif