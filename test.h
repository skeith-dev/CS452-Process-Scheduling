#ifndef TEST_H
#define TEST_H


#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

class object{
	public:
	int val;
	int val2;
	
	vector<int> **v_arr;

	object();
	object( int v );
	
	bool operator < (object &other);

};

object::object( int v ){
	val = v;
}


bool object::operator < ( object &other ){
	
	return ( this->val < other.val || this->val2 > other.val2 );

}

int test_arr_asn();


int test_object_pass1( object obj );

#endif