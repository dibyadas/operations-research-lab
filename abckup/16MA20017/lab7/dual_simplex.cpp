#include <iostream>
#include <stdlib.h>
#include <bits/stdc++.h>
#include "equation.h"
#include "gauss_elim.h"


using namespace std;


void dual_simplex(vector<EQN>, int, int, int);

struct Point
{
	int x;
	int y;
};

void bigm(vector<EQN> eqns, int num_eqns, int num_vars){
	int extra = 0;
	for(auto eqn: eqns){
		if (eqn.sign == 2 || eqn.sign == 3)
		{
			extra++;
		}
	}

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

	for (int i = 0; i < num_eqns; ++i)
	{
		EQN *eqn = &eqns[i];
		vector<float> old_coeffs = eqn->coeffs;
		vector<float> new_coeffs;
		for (float val : old_coeffs)
		{	
			new_coeffs.push_back(-val);
		}
		eqn->coeffs = new_coeffs;
		eqn->bias *= -1; 		
	}
	
	dual_simplex(eqns, num_eqns, num_vars, extra);

}

void dual_simplex(vector<EQN> eqns, int num_eqns, int orig_num_vars, int extra){

	int num_vars = orig_num_vars + extra;
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

	cout << "Enter optimizing function as ( = 0) :- ";
	EQN opt = EQN(orig_num_vars);
	// vector<float> mat_coeffs = opt.prep_gauss();
	vector<float> mat_coeffs = opt.coeffs;
	for (int i = 0; i < num_vars; ++i)
	{
		mat[num_eqns][i] = 	-mat_coeffs[i];
	}
	mat[num_eqns][num_vars] = 0;
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
					// cin >> 0
					cout << mat[i][j] << "      ";
				}
				cout << endl;
			}

		int trap_flag = 1;
		int min_row_index;
		int VALMAX = 99999;

		// cout << num_eqns << " " << num_vars << "\n"; 

		for (int i = 0; i < num_eqns; ++i)
		{	
			// float abs = mat[i][num_vars] > 0 ? mat[i][num_vars] : -mat[i][num_vars];
			if( mat[i][num_vars] < VALMAX && mat[i][num_vars] <= 0) {
				
				VALMAX = mat[i][num_vars];
				min_row_index = i;
			}
			trap_flag = ( mat[i][num_vars] < 0  ) ? 0 : trap_flag;
		}

		if (trap_flag == 1)	
		{
			cout << "Solved" << endl;
			break;
		}

		Point pivot;
		VALMAX = 99999;
		for (int i = 0; i < num_vars; ++i)
		{
		
			if(mat[min_row_index][i] == 0 || mat[min_row_index][i] == -0 || mat[num_eqns][i] == 0 || mat[num_eqns][i] == -0){
				continue;
			}
			float ratio =  mat[num_eqns][i] / mat[min_row_index][i];
			ratio = ratio > 0 ? ratio : -ratio;
			if(ratio < VALMAX && ratio >= 0 )
			{
				pivot.x = min_row_index;
				pivot.y = i;
			}
		}
		

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
			// cout << min_ratios[i];
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

 	bigm(eqns, num_eqns, num_vars);
 	cout << endl;
} 