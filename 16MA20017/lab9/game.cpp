#include <iostream>
#include <stdlib.h>
#include <bits/stdc++.h>

using namespace std;

float **alloc_2D(int nrows, int ncols){

	float **temp_mat = (float **)malloc((nrows)*sizeof(float *));
	for (int i = 0; i < nrows; ++i)
	{
		temp_mat[i] = (float *)malloc((ncols)*sizeof(float));
	}
	return  temp_mat;
}

struct Matrix
{
	float **mat;
	int nrows;
	int ncols;
};

struct Result
{
	int index;
	float value;
};

void display_menu(){
	cout << "Please enter your choice.\n";
	cout << "1. Solve the game \n";
	cout << "9. Exit \n";
}


Result find_rows_maxmin(Matrix mat){
	int max_row_index = 0; 
	float all_row_max = -99999;


	for (int i = 0; i < mat.nrows; ++i)
	{	
		int curr_row_min = 99999;
		for (int j = 0; j < mat.ncols; ++j)
		{
			if( mat.mat[i][j] <= curr_row_min )
				curr_row_min = mat.mat[i][j];
		}
		if(curr_row_min > all_row_max){
			all_row_max = curr_row_min;
			max_row_index = i;
		}
	}
	Result res = {max_row_index, all_row_max};
	return res; 
}
	
Result find_cols_minmax(Matrix mat){
	int min_col_index = 0; 
	float all_col_min = 99999;


	for (int j = 0; j < mat.ncols; ++j)
	{	
		int curr_col_max = -99999;
		for (int i = 0; i < mat.nrows; ++i)
		{
			if( mat.mat[i][j] >= curr_col_max )
				curr_col_max = mat.mat[i][j];
		}
		if(curr_col_max < all_col_min){
			all_col_min = curr_col_max;
			min_col_index = j;

		}
	}
	Result res = {min_col_index, all_col_min};
	return res;
}

void solve_game(){
	int na, nb;
	cout << "Enter the number of strategies of A :- ";
	cin >> na;
	cout << "Enter the number of strategies of B :- ";
	cin >> nb;

	float **payoff_mat = alloc_2D(na, nb);

	cout << "Enter the payoff matrix :- \n";
	for (int i = 0; i < na; ++i)
	{	
		printf("Enter the payoffs for strategies of A%d :- \n", i+1);
		for (int j = 0; j < nb; ++j)
		{
			printf("Enter the payoff for strategy B%d :- \n", j+1);
			cin >> payoff_mat[i][j];
		}
	}

	for (int i = 0; i < na; ++i)
	{	
		for (int j = 0; j < nb; ++j)
		{
			cout << payoff_mat[i][j] << "  ";
		}
		cout << "\n";
	}

	Matrix mat = {payoff_mat, na, nb};

	Result res_A = find_rows_maxmin(mat);
	int strategy_A = res_A.index;
	float optimal_A = res_A.value;
	Result res_B = find_cols_minmax(mat);
	int strategy_B = res_B.index;
	float optimal_B = res_B.value;

	cout << " Optimal game value for A:-  " << optimal_A << ", Optimal game value for B:- " << optimal_B << endl;
	if(optimal_A == optimal_B){
		printf("The game has a saddle point and A should play its strategy-%d and B should play its strategy-%d\n", strategy_A+1, strategy_B+1);
	}
	else{
		printf("Since there is no saddle point, we have to go for mixed strategies to get optimal game value\n");
	}


	for (int i = 0; i < na; ++i)
	{	
		printf("1 -1 ");
		for (int j = 0; j < nb; ++j)
		{
			printf("-%0.2f ",payoff_mat[i][j]);
		}
		printf("0\n");
	}


}


int main(int argc, char const *argv[])
{	
	int choice;
	do{
		display_menu();
		cin >> choice;
		if(choice == 1){
			solve_game();
		}
	}while(choice != 9);
	return 0;
}