#ifndef TEST_H
#define TEST_H


#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

class object{
	public:
	int val;
	
	vector<int> **v_arr;

	object();
	object( int v );

};

object::object( int v ){
	val = v;
}


int test_arr_asn();


int test_object_pass1( object obj );

#endif