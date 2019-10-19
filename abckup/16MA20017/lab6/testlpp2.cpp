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

void dprint(string s, int counter, int iter){
	if (counter == iter)
	{
		cout << s;
	}
}

void dprint(float s, int counter, int iter){
	if (counter == iter)
	{
		cout << s;
	}
}

void dprint(int s, int counter, int iter){
	if (counter == iter)
	{
		cout << s;
	}
}

int simplex(vector<EQN> eqns, int num_eqns, int num_vars, EQN opt, int iter){

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

	vector<float> mat_coeffs = opt.prep_gauss();
	for (int i = 0; i < num_vars; ++i)
	{
		mat[num_eqns][i] = 	-mat_coeffs[i];
	}
	mat[num_eqns][num_vars] = 0;

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

	
	int counter = 0;
	int flag = 0;
	// cout << "Initial Simplex Tableau" <<  endl;
	dprint("Initial Simplex Tableau", counter, iter);
	while(1){

		dprint("\n", counter, iter);

		for (int i = 0; i < num_vars + 1; ++i)
		{
			dprint(map_non_basic[i], counter, iter);
			dprint("   ", counter, iter);
		}
		dprint("\n", counter, iter);

		for (int i = 0; i < num_eqns + 1; ++i)
		{
			dprint(map_basic[i], counter, iter);
			dprint("   ", counter, iter);
		}
		
		dprint("\n", counter, iter);		

		dprint("New iteration \n", counter, iter);
		for (int i = 0; i < num_eqns + 1; ++i)
			{
				for (int j = 0; j < num_vars + 1; ++j)
				{
					dprint(mat[i][j], counter, iter);
					dprint("   ", counter, iter);
				}
				dprint("\n", counter, iter);
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
			dprint("Solved\n", counter, iter);
			break;
		}

		counter++;

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

		// string l = sprintf("\nPivot element is %f at :- (%d,%d)\n", , pivot.x,pivot.y );
		dprint("\nPivot element is ",counter, iter);
		dprint(mat[pivot.x][pivot.y],counter, iter);
		dprint(" at :- (",counter, iter);
		dprint(pivot.x,counter, iter);
		dprint(" , ",counter, iter);
		dprint(pivot.y,counter, iter);
		dprint(" ) ",counter, iter);
		dprint(" \n",counter, iter);
		// dprint(s, counter, iter);
		float pivot_elem = mat[pivot.x][pivot.y];

		auto temp = map_basic[pivot.x];
		map_basic[pivot.x] = map_non_basic[pivot.y];
		map_non_basic[pivot.y] = temp;

		dprint("\n Tableau with ratios :- ", counter, iter);

		for (int i = 0; i < num_eqns + 1; ++i)
		{
			for (int j = 0; j < num_vars + 1; ++j)
			{
				// cin >> 
				dprint(mat[i][j], counter, iter);
				dprint("   ", counter, iter);
			}
			dprint(min_ratios[i], counter, iter);
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
				// cin >> 
				dprint(new_mat[i][j], counter, iter);
				dprint("   ", counter, iter);
			}
			cout << endl;
		}

		mat = new_mat;

		if(counter >= 100){
			return counter;
		}
	}
	return counter;
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

	cout << "Enter optimizing function as ( = 0) :- ";
	EQN opt = EQN(num_vars);

	cout << endl;
	cout << "Enter 1 for listing all BFS \n";
	cout << "Enter 2 for checking number of iterations needed to solve \n";
	cout << "Enter 3 for list of Non-basic variables along with net evaluations in ith iteration \n";
	cout << "Enter 4 for list of Basic variables along with min ratios in ith iteration \n";
	cout << "Enter 5 for simplex table of ith iteration \n";
	cout << "Enter 6 for optimal solution \n";
	cout << "Enter 7 to exit \n";
	cout << "Input choice :- ";
	int choice;
	cin >> choice;

	while(choice != 7){
		if (choice == 1) {
			cout << "\n";
		} 
		else if (choice == 2) {
			auto count = simplex(eqns, num_eqns, num_vars, opt, 101);
			if(count < 100){
				cout << "Took " << count << " iterations to solve the system";
			} else {
				cout << "Cannot solve (Exceeding 100 iterations)";
			}
		}
		else if (choice == 3) {
			int iter;
			cout << "Enter iter number to show :- ";
			cin >> iter;
			auto count2 = simplex(eqns, num_eqns, num_vars, opt, iter);
		}
		cout << endl;
		cin >> choice;
	}
 	// simplex(eqns, num_eqns, num_vars, opt);
 	cout << endl;
 } 