#include <iostream>
#include <stdlib.h>
#include <vector>
#include "equation.h"

using namespace std;

void get_combination(vector<EQN> eqns){ 

}

// find_basic_solution(){

// }

int main(int argc, char const *argv[])
 {
 	vector<EQN> eqns;
 	int num_eqns;
 	int num_vars; 
 	cout << "Enter the number of eqns :- ";
 	cin >> num_eqns;
 	for (int i = 0; i < num_eqns; ++i)
 	{
 		EQN e1 = EQN();
 		e1 = e1.init_eqn();
 		eqns.push_back(e1);
 	}

 } 