#include <iostream>
#include <stdlib.h>
#include <bits/stdc++.h>
#include "equation.h"
#include "get_combination.h"
#include "gauss_elim.h"

using namespace std;

struct Point
{
	int x;
	int y;
};

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

void simplex(vector<EQN> eqns, int num_eqns, int num_vars){

	int num_slack_vars = num_eqns;

	float **mat = (float **)malloc((num_eqns + 1)*sizeof(float *));
	for (int i = 0; i < num_eqns + 1; ++i)
	{
		mat[i] = (float *)malloc((num_vars  + 1)*sizeof(float));
	}

	

	for (int i = 0; i < num_eqns; ++i)
	{	
		vector<int> mat_coeffs = eqns[i].prep_gauss();
		for (int j = 0; j < num_vars; ++j)
		{	
			mat[i][j] = mat_coeffs[j];
		}
		mat[i][num_vars] = mat_coeffs[num_vars];
	}

	cout << "Enter optimizing function as ( = 0) :- ";
	EQN opt = EQN(num_vars);
	vector<int> mat_coeffs = opt.prep_gauss();
	for (int i = 0; i < num_vars; ++i)
	{
		mat[num_eqns][i] = 	-mat_coeffs[i];
	}
	mat[num_eqns][num_vars] = 0;
	cout << endl;

	int flag = 0;
	cout << "Initial Simplex Tableau" <<  endl;
	while(1){

		cout << "New iteration " <<  endl <<  endl <<  endl;
		for (int i = 0; i < num_eqns + 1; ++i)
			{
				for (int j = 0; j < num_vars + 1; ++j)
				{
					// cin >> 
					cout << mat[i][j] << "      ";
				}
				cout << endl;
			}

		int trap_flag = 0;
		int min_col_index;
		int VALMAX = 99999;
		for (int i = 0; i < num_vars; ++i)
		{
			if( mat[num_eqns][i] < VALMAX) {
				VALMAX = mat[num_eqns][i];
				min_col_index = i;
			}
			trap_flag = ( mat[num_eqns][i] <= 0  ) ? 1 : trap_flag;
		}

		if (trap_flag == 0)
		{
			cout << "Solved" << endl;
			break;
		}


		Point pivot;
		vector<float> min_ratios;
		VALMAX = 99999;
		for (int i = 0; i < num_eqns; ++i)
		{
			if ( mat[i][min_col_index] <= 0 )
				min_ratios.push_back(0);
			else {
				min_ratios.push_back( mat[i][num_vars]/mat[i][min_col_index] );
				if (mat[i][num_vars]/mat[i][min_col_index] < VALMAX){
					VALMAX = mat[i][num_vars]/mat[i][min_col_index];
					pivot.x = i;
					pivot.y = min_col_index;
				}
			}
		}
		min_ratios.push_back(0);

		printf("\nPivot element is %f at :- (%d,%d)\n", mat[pivot.x][pivot.y], pivot.x,pivot.y );
		float pivot_elem = mat[pivot.x][pivot.y];

		cout << endl << "Tableau with ratios :- " << endl;

		for (int i = 0; i < num_eqns + 1; ++i)
		{
			for (int j = 0; j < num_vars + 1; ++j)
			{
				// cin >> 
				cout << mat[i][j] << "     ";
			}
			cout << min_ratios[i];
			cout << endl;
		}

		cout << endl;
		
		// cout << "Pivot element is at :- (" << pivot.x << "," << pivot.y << " )"
		// cout << " x " << ;
		// cout << " y " << pivot.y;
		// cout << endl;

		

		float **new_mat = (float **)malloc((num_eqns + 1)*sizeof(float *));
		for (int i = 0; i < num_eqns + 1; ++i)
		{
			new_mat[i] = (float *)malloc((num_vars  + 1)*sizeof(float));
		}

		/// change the row elements
		for (int i = 0; i < num_vars + 1; ++i)
		{
			new_mat[pivot.x][i] =  mat[pivot.x][i]/pivot_elem; 
		}

		/// change the column elements
		for (int i = 0; i < num_eqns + 1; ++i)
		{
			new_mat[i][pivot.y] =  -mat[i][pivot.y]/pivot_elem; 
		}

		new_mat[pivot.x][pivot.y] = 1/pivot_elem;

		//change using the rectangle formula
		for (int i = 0; i < num_eqns + 1; ++i)
		{
			if ( i == pivot.x)
				continue;

			for (int j = 0; j < num_vars + 1; ++j)
			{
				if ( j == pivot.y )
					continue;
				
				float s = mat[i][j];
				float q = mat[i][pivot.y];
				float r = mat[pivot.x][j];
				new_mat[i][j] = (pivot_elem*s - q*r)/pivot_elem;

			}
		}	


		for (int i = 0; i < num_eqns + 1; ++i)
		{
			for (int j = 0; j < num_vars + 1; ++j)
			{
				// cin >> 
				cout << new_mat[i][j] << "     ";
			}
			cout << endl;
		}

		mat = new_mat;
	}

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

 	simplex(eqns, num_eqns, num_vars);

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