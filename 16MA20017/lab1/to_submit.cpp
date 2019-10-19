#include <iostream>
#include <vector>
#include "equation.h"
#include <algorithm>
#include <cstdio>
#include "gauss_elim.h"
#include "get_combination.h"
#include <fstream>

using namespace std;

void read_equation(int m, int n, vector<vector<int> > &eq){
	cout << "Enter the equations :-  " << endl;

	for (int i = 0; i < m; ++i){
		cout << "Enter eq " << i + 1 << " coeff :: "<< endl;
		vector<int> vec;
		int tmp;
		for (int j = 0; j < n; ++j){
			cin >> tmp;
			vec.push_back(tmp);
		}
		cout << "Enter the signe for eq " << i + 1 << " (0 for <= and 1 for >=):: " << endl;
		cin >> tmp; 
		vector<int> vec_tmp(m, 0);
		vec_tmp[i] = 1;
		vec.insert(vec.end(), vec_tmp.begin(), vec_tmp.end());          // so that coeff of slack or surplus is **1**

		eq[m + i] = vector<int>(1, 1);

		if(tmp == 0){
			eq[m + n + i] = vector<int>(1, 1);       // to show +ve
		}
		else if(tmp == 1){
			eq[m + n + i] = vector<int>(1, 0);       // to show -ve
		}

		cout << "Enter intercept (c) for eq :: " << i + 1 << endl;
		cin >> tmp;
		vec.push_back(tmp);
		eq[i] = vec;
	}
}


void calculateBasicSol(int m, int n, vector<vector<int> > eq){
	int take = n - m;
	vector<int> arr(m + n);
	for (int i = 0; i < arr.size(); ++i){
		arr[i] = i+1;
	}
	vector<vector<int> > comb;
	combination(arr, m + n, m, comb);


	float *intercept = new float[m];
	for(int i = 0; i < m; i++){
		intercept[i] = eq[i][n + m];
	}

	
	cout << "The basic solutions are :- " << endl;
	for (int i = 0; i < comb.size(); ++i){

		float **mat = new float*[m];
		for (int i = 0; i < m; ++i){
			mat[i] = new float[m + 1];
		}

		int *var = new int[m];

		int tmp = 0;

		for (int j = 0, tmp = 0; j < comb[i].size(); ++j, ++tmp){
			var[tmp] = comb[i][j] - 1;

			for (int k = 0; k < m; ++k){
				mat[k][tmp] = eq[k][comb[i][j] - 1];
			}
		}

		for (int j = 0; j < m; ++j){
			mat[j][m] = intercept[j];
		}


		float *initial_values = new float[m];
		for (int i = 0; i < m; ++i){
			initial_values[i] = 0;
		}

		float *sol = new float[m];
		
		gaussianElimination(mat, m, sol);

		if( )
		cout<< "(";
		for (int x = 0; x < m; ++x){
			cout << " " << sol[x] << ", ";
		}
		cout << ")" << endl;
	}

}


int main(int argc, char const *argv[])
{
	int num_vars, num_eqns;
	cout << "Enter the number of eqns :- ";
	cin >> num_eqns;
	cout << "Enter the number of variables :- ";
	cin >> num_vars;

	vector<vector<int> > eq(2 * num_eqns + num_vars);  
	
	read_equation(num_eqns, num_vars, eq);
	calculateBasicSol(num_eqns, num_vars, eq);


	return 0;
}
	
