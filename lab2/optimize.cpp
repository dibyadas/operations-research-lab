#include <iostream>
#include <bits/stdc++.h>
#include "gauss_elim.h"
#include "equation.h"
#include "get_combination.h"

using namespace std;

int main(int argc, char const *argv[])
{
	// float mat[2][3];

	vector<vector<int> > sol;

	for (int i = 0; i < 3; ++i)
	{
		float **mat = (float **)malloc(2*sizeof(float *));
		mat[0] = (float *)malloc(3*sizeof(float));
		mat[1] = (float *)malloc(3*sizeof(float));

		mat[0][0] = 1;
		mat[0][1] = 2;

		mat[0][2] = 450;

		mat[1][0] = 1;
		mat[1][1] = 1;
		mat[1][2] = 250;

		// { {1,1,450}, {2,1,600} };
		float *sol = new float[2];
		gaussianElimination(mat, 2, sol);
		// cout << "sol :- " << sol[0] << endl;
		// cout << "sol :- " << sol[1] << endl;
		for(int i = 0; i < 2; i ++){
			cout << "sol :- " << sol[i] << endl;
		}
		return 0;	
	}

	
}