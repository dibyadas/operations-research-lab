#include <iostream>
#include <stdlib.h>
#include <bits/stdc++.h>
#include "equation.h"
#include "get_combination.h"
#include "gauss_elim.h"

using namespace std;

vector< vector<float> > get_combination(vector<EQN> eqns, int num_eqns, int num_vars){

	vector<vector<int> > comb;
	vector<int> v;
	for (int i = 0; i < num_eqns; ++i)
	{
		v.push_back(i);
	}

	combination(v, num_eqns, num_vars, comb);

	set< vector<float> > uni_sols;
	for (int i = 0; i < comb.size(); ++i)
	{
		float **mat = (float **)malloc(2*sizeof(float *));
		mat[0] = (float *)malloc(num_eqns*sizeof(float));
		mat[1] = (float *)malloc(num_eqns*sizeof(float));

		float *sol = new float[num_eqns];
		for(int j = 0; j < 2; j ++ ){

		  	vector<int> mat_coeffs = eqns[comb[i][j]].prep_gauss();
		  	for (int p = 0; p < mat_coeffs.size(); ++p)
		  	{
		  		mat[j][p] = mat_coeffs[p];
		  		// cout << mat[j][p] << " ";
		  	}
			// cout << endl;
		}

		vector<float> solution;
		
		gaussianElimination(mat, 2, sol);
		// cout << "Basic Solution :- ";
		for(int i = 0; i < num_vars; i ++){
			float temp = sol[i];
			if( temp <= 1e-10 ){
				temp = 0;
			}
			
			solution.push_back(temp);
			
		}
		uni_sols.insert(solution);

	}

	vector< vector<float> > feasible_solutions;

	for(auto solution: uni_sols){
		for(auto val2 : solution){
			cout << val2 << " ";
		}
		int feasib = 1;
		for(auto eqn : eqns){
			feasib = (int)eqn.check_feasible(solution);
			if(feasib == 0){
				cout << " -- Not a feasible solution" << endl;
				break;
			}
		}
		if(feasib != 0)
			feasible_solutions.push_back(solution);
		cout << endl;
	}

	return feasible_solutions;

}


int main(int argc, char const *argv[])
 {
 	vector<EQN> eqns;
 	int num_eqns;
 	int num_vars; 
 	cout << "Enter the number of eqns :- ";
 	cin >> num_eqns;
 	cout << "Enter the number of vars :- ";
 	cin >> num_vars;
 	for (int i = 0; i < num_eqns; ++i)
 	{
 		EQN e1 = EQN(num_vars);
 		eqns.push_back(e1);
 	}

 	vector< vector<float> > feasible = get_combination(eqns, num_eqns, num_vars);

 	for(auto i : feasible){
 		for(auto j : i){
 			cout << j << " ";
 		}
 		cout << endl;
 	}

 	cout << "Enter optimizing expression with  (= 0) as the type and bias:- ";
 	EQN e2 = EQN(num_vars);
 	cout << "Enter 1 for maximize and -1 for minimize :- ";
 	int control;
 	cin >> control;
 	int sol_index = -99999;
 	float minsum = 999999;
 	float maxsum = -999999;
 	for (int i = 0; i < feasible.size(); ++i)
 	{	
 		if(control == 1){
 			float temp = e2.compute(feasible[i]);
 			if(temp > maxsum){
 				maxsum = temp;
 				sol_index = i;
 			}

 		} else if(control == -1){
 			float temp = e2.compute(feasible[i]);
 			if(temp < minsum){
 				minsum = temp;
 				sol_index = i;
 			}
 		}
 		
 	}
 	cout << "The feasible solution for the system is :- " << endl;
 	if(sol_index == -99999){
 		cout << "No optimum solution";
 	} else{
 		auto feee = feasible[sol_index];
	 	for(auto val : feee ){
	 		cout << val << " ";
	 	}	
 	}
 	
 	cout << endl;
 } 