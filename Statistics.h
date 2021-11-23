//
// Created by Kevin Emery on 11/21/21.
//

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
		int program;
		string type;
		int value;
		statistic(int, string, int);
		void setValues(int, string, int);
};

class Statistics {

	std::list<statistic> statistics_holder;

	public:
		Statistics();
		void get_stats();
		void add_stat(int program, string type, int value);

};
