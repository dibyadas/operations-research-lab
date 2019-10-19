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

		  	vector<float> mat_coeffs = eqns[comb[i][j]].prep_gauss();
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



void simplex(vector<EQN> eqns, EQN opt, int num_eqns, int num_vars, vector<float> last_row){

	int num_slack_vars = num_eqns;

	float **mat = (float **)malloc((num_eqns + 1)*sizeof(float *));
	for (int i = 0; i < num_eqns + 1; ++i)
	{
		mat[i] = (float *)malloc((num_vars  + 1)*sizeof(float));
	}

	for (int i = 0; i < num_eqns; ++i)
	{	
		vector<float> mat_coeffs = eqns[i].prep_gauss();
		for (int j = 0; j < num_vars; ++j)
		{	
			mat[i][j] = mat_coeffs[j];
		}
		mat[i][num_vars] = mat_coeffs[num_vars];
	}

	vector<float> mat_coeffs = last_row;
	for (int i = 0; i < num_vars; ++i)
	{
		mat[num_eqns][i] = 	mat_coeffs[i];
	}
	mat[num_eqns][num_vars] = mat_coeffs[num_vars];
	cout << endl;

	map<int, string> map_basic;
	map<int, string> map_non_basic;

	for (int i = 0; i < num_vars; ++i)
	{
		map_non_basic[i] = "x" + to_string(i+1);
	}
	map_non_basic[num_vars] = "1";

	for (int i = 0; i < num_eqns; ++i)
	{
		map_basic[i] = "z" + to_string(i+1);
	}
	map_basic[num_eqns] = "z";

	
		
	int flag = 0;
	cout << "Initial Simplex Tableau" <<  endl;
	while(1){

		cout << endl;

		for (int i = 0; i < num_vars + 1; ++i)
		{
			cout << map_non_basic[i] << "   ";
		}
		cout << endl;

		for (int i = 0; i < num_eqns + 1; ++i)
		{
			cout << map_basic[i] << "   ";
		}
		cout << endl;

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
		// break;
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
		// cout << "trap_flag " << trap_flag << endl;
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

		auto temp = map_basic[pivot.x];
		map_basic[pivot.x] = map_non_basic[pivot.y];
		map_non_basic[pivot.y] = temp;

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
				cout << new_mat[i][j] << "     ";
			}
			cout << endl;
		}
		mat = new_mat;
	}

}

void bigm(vector<EQN> eqns, int num_eqns, int num_vars){
	int extra = 0;
	for(auto eqn: eqns){
		if (eqn.sign == 2 || eqn.sign == 3)
		{
			// eqn.num_vars++;
			extra++;
			// eqn.coeffs.push_back(-1);
		}
	}
	// cout << "extra " << extra << "\n";
	int flip_sign = num_vars;
	for(int i = 0; i < num_eqns; ++i){
		EQN *eqn = &eqns[i];
		eqn->num_vars += extra;
		for (int j = 0; j < extra; ++j)
		{
			eqn->coeffs.push_back(0);
		}
		if (eqn->sign == 2 || eqn->sign == 3)
		{
			eqn->coeffs[flip_sign] = -1;
			flip_sign++;
		}
	}
	cout << "Enter optimizing function as ( = 0) :- ";
	EQN opt = EQN(num_vars);
	opt.num_vars += extra;

	vector<float> opt_coeff = opt.prep_gauss();
	// for(auto v : opt_coeff){
	// 	cout << v << "  "; 
	// }
	int M = 1000;
	vector<float> M_coeff;
	for (int j = 0; j < num_vars + extra; ++j)
	{
		float sum = 0;
		for (int i = 0; i < num_eqns; ++i)
		{	
			sum += eqns[i].coeffs[j];
		}
		// cout << "sum " << sum;
		M_coeff.push_back( -opt_coeff[j] - M*sum);
	}

	float sum = 0;
	for (int i = 0; i < num_eqns; ++i)
	{	
		sum += eqns[i].prep_gauss()[num_vars + extra ];
	}
	M_coeff.push_back( -opt_coeff[num_vars + extra ] - M*sum);
	// cout << "sum " << sum;

	// for (int i = 0; i < num_vars + extra + 1; ++i)
	// {
	// 	opt.coeffs[i] = -M_coeff[i];
	// 	cout << opt.coeffs[i] << "  dd  "; 
	// }



	// opt.bias = M_coeff[num_vars + extra];

	cout << endl;
	for (int i = 0; i < num_vars + extra + 1; ++i)
	{
		cout << M_coeff[i] << "   ";
	}
	cout << endl;

	simplex(eqns, opt , num_eqns, num_vars + extra, M_coeff);

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

	

 	bigm(eqns, num_eqns, num_vars);

 	// vector< vector<float> > feasible = get_combination(eqns, num_eqns, num_vars);

 	// for(auto i : feasible){
 	// 	for(auto j : i){
 	// 		cout << j << " ";
 	// 	}
 	// 	cout << endl;
 	// }

 	// cout << "Enter optimizing expression with  (= 0) as the type and bias:- ";
 	// EQN e2 = EQN(num_vars);
 	// cout << "Enter 1 for maximize and -1 for minimize :- ";
 	// int control;
 	// cin >> control;
 	// int sol_index = -99999;
 	// float minsum = 999999;
 	// float maxsum = -999999;
 	// for (int i = 0; i < feasible.size(); ++i)
 	// {	
 	// 	if(control == 1){
 	// 		float temp = e2.compute(feasible[i]);
 	// 		if(temp > maxsum){
 	// 			maxsum = temp;
 	// 			sol_index = i;
 	// 		}

 	// 	} else if(control == -1){
 	// 		float temp = e2.compute(feasible[i]);
 	// 		if(temp < minsum){
 	// 			minsum = temp;
 	// 			sol_index = i;
 	// 		}
 	// 	}
 		
 	// }
 	// cout << "The feasible solution for the system is :- " << endl;
 	// if(sol_index == -99999){
 	// 	cout << "No optimum solution";
 	// } else{
 	// 	auto feee = feasible[sol_index];
	 // 	for(auto val : feee ){
	 // 		cout << val << " ";
	 // 	}	
 	// }
 	
 	cout << endl;
 } 