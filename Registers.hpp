#ifndef Registers_H
#define Registers_H

#include <set>
#include <string>
#include <vector>


using namespace std;

class Registers {
public:
    int count;
    vector<string> funcs_params_array_pts;

	Registers() : count(0) , funcs_params_array_pts() {};
	string get_curr_func_params_array_ptr();
	string allocateRegister();
	~Registers() = default; 
};

#endif