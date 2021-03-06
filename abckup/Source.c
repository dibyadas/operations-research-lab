// Name Gurmanddeep Singh Sidhu
// Roll 16MA20019


#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define SIZE 50
#define M 0

void swap(int, int, int);
int calrank(int, int);
void printMatrix(double **, int, int);
void getMatrix(double **, int, int);
// void getA(double **, double **, int, int,const char **, float *, float *, int);
double** augMat(double **, double **, int, int);
long long cmb(int, int);
void getMin(int, int);
void max(int, int);
void slve(int);

double d[SIZE] = { 0 };
double copied[SIZE][SIZE], ans[SIZE][SIZE], z[SIZE];

int main() {

	int n, ns, m;
	int i, ii, j, k, l, count, iter;
	int rank, rank_aug;
	int check, nnew;
	double min, minr, temp;
	printf("Enter the number of variables in the system(excluding surlplus variables):");
	scanf("%d", &n);
	nnew = n;
	ns = 0;
	/*printf("Enter the number of surplus variables:");
	scanf("%d", &ns);
	n=n+ns;*/
	printf("Enter the number of equations in the system:");
	scanf("%d", &m);
	nnew = n + m;

	const char **bfs, **nbfs, *ttemp;
	float *cbv, *cnbv, **cbvv, **cnbvv;
	nbfs = (const char **)malloc(2*n * sizeof(const char *));
	bfs = (const char **)malloc(m * sizeof(const char *));
	cnbv = (float *)malloc(n * sizeof(float *));
	cbv = (float *)malloc(10 * sizeof(float *));
	cbvv = (float **)malloc(10 * sizeof(float **));
	for (i = 0; i < 10; i++)
		cbvv[i] = (float *)malloc(n * sizeof(float *));

	cnbvv = (float **)malloc(n * sizeof(float **));
	for (i = 0; i < 10; i++)
		cnbvv[i] = (float *)malloc(n * sizeof(float *));

	const char ***bfss, ***nbfss;
	bfss = (const char ***)malloc(10 * sizeof(const char **));
	for (i = 0; i < 10; i++)
	{
		bfss[i] = (const char **)malloc(m * sizeof(const char *));
	}

	nbfss = (char ***)malloc(10 * sizeof(char **));
	for (i = 0; i < 10; i++)
	{
		nbfss[i] = (char **)malloc(2*n * sizeof(char *));
	}
	//printf("n,ns is:%d%d\n",n,ns);

	if ((n - ns) == 2)
	{
		nbfs[0] = "x"; nbfs[1] = "y";
		if (ns == 1)
			nbfs[2] = "sur1";
		else if (ns == 2)
		{
			nbfs[2] = "sur1";
			nbfs[3] = "sur2";
		}
		else if (ns == 3)
		{
			nbfs[2] = "sur1";
			nbfs[3] = "sur2";
			nbfs[4] = "sur3";
		}

	}
	else if (n == 3) { nbfs[0] = "x"; nbfs[1] = "y"; nbfs[2] = "z"; }
	else if (n == 4) { nbfs[0] = "x"; nbfs[1] = "y"; nbfs[2] = "z"; nbfs[3] = "w"; };

	if (m == 2) { bfs[0] = "s1"; bfs[1] = "s2"; }
	else if (m == 3) { bfs[0] = "s1"; bfs[1] = "s2"; bfs[2] = "s3"; }
	else if (m == 4) { bfs[0] = "s1"; bfs[1] = "s2"; bfs[2] = "s3"; bfs[3] = "s4"; }


	/*printf("The initial artificial and slack variables are:\n");
	for(i=0;i<m;i++)
		printf("%s	",bfs[i]);
	printf("\nThe inital surplus variables are:\n");
	for(i=n-ns;i<n;i++)
		printf("%s	",nbfs[i]);
	printf("\n");*/

	int nb = nnew - m - ns;

	double **A = (double **)malloc(m * sizeof(double *));
	for (i = 0; i < m; i++)
		A[i] = (double *)malloc(nnew * sizeof(double));

	double **B = (double **)malloc(m * sizeof(double *));
	for (i = 0; i < m; i++)
		B[i] = (double *)malloc(sizeof(double));

	getA(A, B, m, n, nbfs, cbv, cnbv, ns);

	/*for (i = 0; i < m; i++)
	{
		for (j = 0; j <= n+1; j++)
			printf("%lf	",A[i][j]);
		printf("\n");
	}*/




	



	double **C = (double **)malloc((m + 1) * sizeof(double *));
	for (i = 0; i < m + 1; i++)
		C[i] = (double *)malloc((2*n + 1) * sizeof(double));

	double **D = (double **)malloc((m + 1) * sizeof(double *));
	for (i = 0; i < m + 1; i++)
		D[i] = (double *)malloc((n + 1) * sizeof(double));

	double ***BB = (double ***)malloc(10 * sizeof(double **));
	for (i = 0; i < 10; i++)
	{
		BB[i] = (double **)malloc((m + 1) * sizeof(double *));
		for (j = 0; j < m + 1; j++)
			BB[i][j] = (double *)malloc((n + 1) * sizeof(double));
	}
	for (i = 0; i < m; i++)
		for (j = 0; j < n + 1; j++)
		{
			if (A[i][n + 1] == 1)
				C[i][j] = A[i][j];
			else
				C[i][j] = -A[i][j];
		}


	double **CC = (double **)malloc((m + 1) * sizeof(double *));
	for (i = 0; i < m + 1; i++)
		CC[i] = (double *)malloc((2*n + 1) * sizeof(double));



	printf("Enter the objective function please:\n");

	for (i = 0; i < n; i++)
	{
		printf("Coefficient of %s = ", nbfs[i]);
		scanf("%lf", &min);
		cnbv[i] = min;
		temp = 0;
		for (j = 0; j < m; j++)
		{
			temp = temp + (cbv[j] * C[j][i]);
		}
		C[m][i] = temp + min;
	}
	temp = 0;
	for (i = 0; i < m; i++)
		temp = temp + cbv[i] * C[i][n];

	char **tnbfs = ( char **)malloc(2 * n * sizeof( char *));

	C[m][n] = temp;
	j = 0;
	for (i = 0; i < n; i++)
	{
		printf("Is %s free.Enter 1 for Y and 0 for N?", nbfs[i]);
		scanf("%d", &count);
		if (count == 1)
		{	
			if (i == 0)
			{
				tnbfs[j] = "x1";
				tnbfs[j + 1] = "x2";
			}
			
			else if (i == 1)
			{
				tnbfs[j] = "y1";
				tnbfs[j + 1] = "y2";
			}
			else if (i == 2)
			{
				tnbfs[j] = "z1";
				tnbfs[j + 1] = "z2";
			}
			printf("%s split into %s and %s,where (%s,%s)>=0.\n", nbfs[i], tnbfs[j], tnbfs[j + 1], tnbfs[j], tnbfs[j + 1]);
			for (ii = 0; ii <= m; ii++)
			{
				CC[ii][j] = C[ii][i];
				CC[ii][j + 1] = -C[ii][i];
			}
			j = j + 2;
		}
		else
		{
			tnbfs[j] = nbfs[i];
			for (ii = 0; ii <= m; ii++)
			{
				CC[ii][j] = C[ii][i];
			}
			j++;
		}
	}
	for (ii = 0; ii <= m; ii++)
	{
		CC[ii][j] = C[ii][n];
	}
	
	for (i = 0; i < j; i++)
	{
		nbfs[i] = tnbfs[i];
	}
	

	for (i = 0; i <= m; i++)
	{
		for (ii = 0; ii <= j; ii++)
			C[i][ii] = CC[i][ii];
	}
	n = j;


	




	for (i = 0; i < m + 1; i++) {
		for (j = 0; j < n + 1; j++)
			BB[0][i][j] = C[i][j];
	}
	for (i = 0; i < m; i++)
		bfss[0][i] = bfs[i];
	for (i = 0; i < n; i++)
		nbfss[0][i] = nbfs[i];

	i = 0;
	printf("Initial table is:\n");
	for (j = 0; j < m + 1; j++)
	{
		for (ii = 0; ii < n + 1; ii++)
		{
			if (BB[i][j][ii] >= 0)
				printf("  %lf  ", BB[i][j][ii]);
			else
				printf(" %lf  ", BB[i][j][ii]);
		}
		if (j < m)
			printf("%s \n", bfss[i][j]);
		else
			printf("\n");
	}



	check = 1;

	iter = 1;

	printf("n is:%d\n", n);


	do
	{




		check = 1;
		count = 0;
		min = C[0][n];
		l = 0;
		for (i = 1; i < m; i++)
		{
			if (C[i][n] < min)
			{
				min = C[i][n];
				l = i;
			}
		}
		minr = -99999;

		for (i = 0; i < n; i++)
		{
			if (C[l][i] < 0)
			{
				temp = C[m][i] / C[l][i];
				count = 1;

				if ((temp > minr) && (temp < 0)) { minr = temp; k = i; }
			}

		}

		if (count == 0)
		{
			printf("Simplex is unbounded.\n");
			return 0;
		}


		ttemp = nbfs[k];
		nbfs[k] = bfs[l];
		bfs[l] = ttemp;

		printf("Pivot is: %f , for iteration number:%d\n", C[l][k], iter);

		D[l][k] = 1 / C[l][k];

		for (i = 0; i < n + 1; i++)
		{
			if (i != k)
				D[l][i] = C[l][i] * (1 / C[l][k]);
		}

		for (i = 0; i < m + 1; i++)
		{
			if (i != l)
				D[i][k] = C[i][k] * (-1 / C[l][k]);
		}

		for (i = 0; i < m + 1; i++)
		{
			for (j = 0; j < n + 1; j++)
			{
				if ((j != k) && (i != l))
				{
					if ((i < l) && (j < k))
					{
						D[i][j] = (C[i][j] * C[l][k] - C[l][j] * C[i][k]) / C[l][k];
					}
					if ((i > l) && (j < k))
					{
						D[i][j] = -(C[i][k] * C[l][j] - C[l][k] * C[i][j]) / C[l][k];
					}
					if ((i < l) && (j > k))
					{
						D[i][j] = -(C[l][j] * C[i][k] - C[l][k] * C[i][j]) / C[l][k];
					}

					if ((i > l) && (j > k))
					{
						D[i][j] = -(C[i][k] * C[l][j] - C[l][k] * C[i][j]) / C[l][k];
					}


				}
			}
		}
		printf("Objective function value at this iteration = %lf\n\n", -D[m][n]);




		check = 1;

		for (i = 0; i < m; i++)
		{
			if (D[i][n] <= 0)
			{
				check = -1;

			}
		}

		for (i = 0; i < m + 1; i++) {
			for (j = 0; j < n + 1; j++)
				C[i][j] = D[i][j];
		}



		for (i = 0; i < m + 1; i++) {
			for (j = 0; j < n + 1; j++)
				BB[iter][i][j] = D[i][j];
		}
		for (i = 0; i < m; i++)
			bfss[iter][i] = bfs[i];
		for (i = 0; i < n; i++)
			nbfss[iter][i] = nbfs[i];

		//for(i=0;i<n;i++)
			//cbvv[iter][i]=cbv




		iter++;


	} while (check != 1);

	char a;






	do {

		printf("\nEnter x to exit\n");
		printf("Enter a for the table for the ith iteraton:\n");
		printf("Enter b for geting Number of iterations needed:\n");
		//printf("Enter e for getting the jth non basic variable column of the ith iteration:\n");
		printf("Enter c for min ratios in the table for ith iteration (i=0 means the initial table):\n");

		printf("Enter d for optimal solution:\n");

		scanf(" %c", &a);
		switch (a) {


		case 'b':
		{
			printf("Number of iterations is : %d \n", iter - 1);
		}
		break;

		case 'c':
		{
			printf("Enter i:\n");
			scanf("%d", &i);

			printf("\nFeasible Min Ratios are:\n");
			min = BB[i][0][n];
			l = 0;
			for (j = 1; j < m; j++)
			{
				if (BB[i][j][n] < min)
				{
					min = BB[i][j][n];
					l = j;
				}
			}
			minr = -99999;
			for (ii = 0; ii < n; ii++)
			{
				if (BB[i][l][ii] < 0)
				{
					temp = BB[i][m][ii] / BB[i][l][ii];

					if (temp < 0) { printf("%lf ", temp); }
					if ((temp > minr) && (temp < 0)) { minr = temp; k = ii; }
				}

			}
			printf("\nPivot element is:%lf\n", BB[i][l][k]);

			break;
		}
		case 'e':
		{
			printf("Enter i:\n");
			scanf("%d", &i);
			printf("Enter j\n");
			scanf("%d", &j);
			printf("jth Non-basic variable of the ith iteration is:%s\n", nbfss[i][j]);
			printf("Column associated with the variable is:\n");
			for (k = 0; k <= m; k++)
			{
				printf("%lf\n", BB[i][k][j]);
			}


			break;
		}
		case 'a':
		{
			printf("Enter i:\n");
			scanf("%d", &i);
			for (j = 0; j < n; j++)
				printf("     %s    ", nbfss[i][j]);
			printf("\n");
			for (j = 0; j < m + 1; j++)
			{
				for (ii = 0; ii < n + 1; ii++)
				{
					if (BB[i][j][ii] >= 0)
						printf("  %lf  ", BB[i][j][ii]);
					else
						printf(" %lf  ", BB[i][j][ii]);
				}
				if (j < m)
					printf("%s \n", bfss[i][j]);
				else
					printf("\n");
			}
			printf("\n");


			break;
		}
		case 'd':
		{
			printf("Optimal solution is:\n");
			printf("Non Basic solutions are:\n");
			for (i = 0; i < n; i++)
			{
				printf("%s = 0 ", nbfs[i]);
			}
			printf("\n");

			printf("Basic solutions are:\n");
			for (i = 0; i < m; i++)
			{
				printf("%s = %lf ", bfs[i], D[i][n]);
			}

			printf("\n");

			printf(" z = %lf \n", -D[m][n]);


		}
		break;
		}
	} while (a != 'x');


	printf("Optimal solution is:\n");
	printf("Non Basic solutions are:\n");
	for (i = 0; i < n; i++)
	{
		printf("%s = 0 ", nbfs[i]);
	}
	printf("\n");

	printf("Basic solutions are:\n");
	for (i = 0; i < m; i++)
	{
		printf("%s = %lf ", bfs[i], D[i][n]);
	}

	printf("\n");

	printf(" z = %lf \n", D[m][n]);





	free(A);
	free(B);
	free(C);
	return 0;
}

double** augMat(double **A, double **b, int m, int n) {
	int i, j;
	double **result = (double **)malloc(m * sizeof(double *));
	for (i = 0; i < m; i++)
		result[i] = (double *)malloc((n + 1) * sizeof(double));
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++)
			result[i][j] = A[i][j];
	}
	for (i = 0; i < m; i++)
		result[i][n] = b[i][0];
	return result;
}

void swap(int r1, int r2, int cols) {
	int i;
	for (i = 0; i < cols; i++) {
		double temp = copied[r1][i];
		copied[r1][i] = copied[r2][i];
		copied[r2][i] = temp;
	}
}

int calrank(int R, int C) {
	int rank = C, row, col, i;
	for (row = 0; row < rank; row++) {
		if (copied[row][row]) {
			for (col = 0; col < R; col++) {
				if (col != row) {
					double mult = (double)copied[col][row] / copied[row][row];
					for (i = 0; i < rank; i++)
						copied[col][i] -= mult * copied[row][i];
				}
			}
		}
		else {
			bool red = true;
			for (i = row + 1; i < R; i++) {
				if (copied[i][row]) {
					swap(row, i, rank);
					red = false;
					break;
				}
			}
			if (red) {
				rank--;
				for (i = 0; i < R; i++)
					copied[i][row] = copied[i][rank];
			}
			row--;
		}
	}
	return rank;
}

void printMatrix(double **A, int rows, int cols) {
	int i, j;
	for (i = 0; i < rows; i++) {
		printf("\n");
		for (j = 0; j < cols; j++)
			printf("%lf ", A[i][j]);
	}
}

void getMatrix(double **A, int rows, int cols) {
	int i, j;
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++)
		{
			printf("Enter %dth row %dth column element:", i, j);
			scanf("%lf", &A[i][j]);
		}
	}
}

void getA(double **A, double **B, int rows, int cols, char **nbfs, float *cbv, float *cnbv, int ns) {
	int i, j, k;
	for (i = 0; i < rows; i++) {
		printf("Please enter the coefficients of equation number %d:\n", i + 1);
		for (j = 0; j <= cols + 1; j++)
		{
			if (j < cols) {

				printf("Enter coefficient of %s :", nbfs[j]);



				scanf("%lf", &A[i][j]);
			}
			else if (j == cols) {
				printf("Enter 1 for </<=,0 for = ,and -1 for >/>=:");
				scanf("%lf", &A[i][cols+1]) ;
				if (A[i][cols + 1] == 1)
					cbv[i] = 0;
				else
					cbv[i] = 0;

			}
			else if (j == cols + 1)
			{
				printf("Enter the RHS:");
				scanf("%lf", &A[i][cols]);
			}
			else { A[i][j] = 0; }
		}
	}
}

long long cmb(int c, int d) {
	if (d > c / 2) d = c - d;
	long long a = 1;
	int i;
	for (i = 1; i <= d; i++) {
		a *= c - d + i;
		a /= i;
	}
	return a;
}

void getMin(int solInd, int var) {
	double mnanswer = 999999.0, curr = 0;
	int k, i, l, cor = 0, ch;
	for (k = 0; k < solInd; k++) {
		ch = 1;
		curr = 0;
		for (l = 0; l < var; l++) {
			if (ans[k][l] < 0) {
				ch = 0;
				break;
			}
		}
		if (ch != 1)
			continue;
		for (l = 0; l < var; l++) {
			curr = curr + ans[k][l] * z[l];
		}
		if (mnanswer > curr)
		{
			cor = k;
			mnanswer = curr;
		}
	}
	if (ch == 0) { printf("No optimal solution obtained(NaN error).\n"); }
	else {
		printf("The minimum is %6.1f\n", mnanswer);
		printf("The optimum solution is:\n");
		for (i = 0; i < var; i++) printf("%6.1f ", ans[k][i]);
	}
}

void max(int solInd, int var) {
	double mxanswer = 0.0, curr = 0;
	int k, ch, cor = 0, l;
	for (k = 0; k < solInd; k++) {
		ch = 1;
		curr = 0;
		for (l = 0; l < var; l++) {
			if (ans[k][l] < 0) {
				ch = 0;
				break;
			}
		}
		if (ch != 1)
			continue;
		for (l = 0; l < var; l++) {
			curr = curr + ans[k][l] * z[l];
		}
		if (mxanswer < curr) {
			mxanswer = curr;
			cor = k;
		}
	}
	if (ch == 0) { printf("No optimal solution obtained(NaN error).\n"); }
	else {
		printf("The maximum is %7.1f\n", mxanswer);
		printf("Optimal solution is:\n");
		for (l = 0; l < var; l++) printf("%7.1f ", ans[cor][l]); printf("\n");
	}
}

void slve(int n) {
	double  c;
	int i, j, k;
	for (i = 0; i < 10; i++)
		d[i] = 0;
	for (k = 0; k < n - 1; k++)
		for (i = k; i < n - 1; i++) {
			c = (copied[i + 1][k] / copied[k][k]);
			for (j = 0; j <= n; j++)
				copied[i + 1][j] -= c * copied[k][j];
		}

	for (i = n - 1; i >= 0; i--) {
		c = 0;
		for (j = i; j <= n - 1; j++)
			c = c + copied[i][j] * d[j];
		d[i] = (copied[i][n] - c) / copied[i][i];
	}
}
