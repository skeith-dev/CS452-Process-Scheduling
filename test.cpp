#include "test.h"

using namespace std;



int main(){
	
	object obj_1( 1 );
	obj_1.val2 = 1;
	
	object obj_2( 1 );
	obj_2.val2 = 2;
	
	if( obj_1 < obj_2 ){
		cout << "True" << endl;
	}else{
		cout << "False" << endl;
	}
	
	
	
	/*
	object obj( 1 );
	test_object_pass1( obj );
	
	cout << "val: " << obj.val << endl;
	*/

	return 0;
}


/**/
int test_arr_asn(){
		


	//int *v_arr[3];
	
	//vector<int> *v_arr[3];
	
	object *obj = new object( 1 );
	
	//vector<int> *p_arr[3];
	obj->v_arr = new vector<int>*[3];
	
	//obj->v_arr = new vector<vector<int>*>;
	//obj->v_arr->reserve(3);

	
	
	for( int i = 0; i < 3; i++ ){
		//int ints[] = {i, i+1, i+2};
		//vector<int> v ( ints, ints+3 );
		
		//vector<int>* v = new vector<int>(2, i);
		//v_arr[i] = v;
		
		obj->v_arr[i] = new vector<int>;
		obj->v_arr[i]->reserve(5);
		
		obj->v_arr[i]->push_back(i);
		push_heap( obj->v_arr[i]->begin(), obj->v_arr[i]->end() );
		
		obj->v_arr[i]->push_back(i+1);
		push_heap( obj->v_arr[i]->begin(), obj->v_arr[i]->end() );
		
		obj->v_arr[i]->push_back(i+2);
		push_heap( obj->v_arr[i]->begin(), obj->v_arr[i]->end() );
		
		obj->v_arr[i]->push_back(i+3);
		push_heap( obj->v_arr[i]->begin(), obj->v_arr[i]->end() );
		
		obj->v_arr[i]->push_back(i+4);
		push_heap( obj->v_arr[i]->begin(), obj->v_arr[i]->end() );
		
		//cout << "i: " << i << endl;
		//cout << "v_arr[" << i << "]->front(): " << v_arr[i]->front() << endl;
		cout << "v_arr[" << i << "] heap max: " << obj->v_arr[i]->front() << endl;

		
		//cout << "lvi>f: " << v_arr[i]->front() << endl;
	}
	
	/*
	cout << v_arr[0] << endl;
	cout << v_arr[1] << endl;
	cout << v_arr[2] << endl;
	*/
	
	/*
	int ints[] = {1, 2, 3};
	vector<int> v( ints, ints+3 );
	v_arr[0] = &v;
	*/
	cout << "-----" << endl;
	//cout << "v_arr[0]: " << v_arr[0][0] << ", " <<  v_arr[0][1] << ", " v_arr[0][2] << endl;
	cout << "v_arr[0]->front(): " << obj->v_arr[0]->front() << endl;
	cout << "v_arr[1]->front(): " << obj->v_arr[1]->front() << endl;
	cout << "v_arr[2]->front(): " << obj->v_arr[2]->front() << endl;

	
	return 0;
}


int test_object_pass1( object obj ){
	obj.val = 2;
	std::cout << "val: " << obj.val << std::endl;
	return 0;
}

/*
int test_object_pass2( object *obj ){
}
*/