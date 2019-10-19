#include <stdlib.h>

int forwardElim(float **mat, int m);
  
// function to calculate the values of the unknowns 
void backSub(float **mat, int m, float *sol); 
  
// function to get matrix content 
void gaussianElimination(float **mat, int m, float *sol) 
{ 
    /* reduction into r.e.f. */
    int singular_flag = forwardElim(mat, m); 
  
    /* if matrix is singular */
    if (singular_flag != -1) 
    { 
        // printf("Singular Matrix.\n"); 
  
        /* if the RHS of equation corresponding to 
           zero row  is 0, * system has infinitely 
           many solutions, else inconsistent*/
        // if (mat[singular_flag][m]) 
        //     printf("Inconsistent System."); 
        // else
        //     printf("May have infinitely many "
        //            "solutions."); 
  
        return; 
    } 
  
    /* get solution to system and print it using 
       backward substitution */
    backSub(mat, m, sol); 
} 
  
// function for elemntary operation of swapping two rows 
void swap_row(float **mat, int i, int j, int m) 
{ 
    //printf("Swapped rows %d and %d\n", i, j); 
  
    for (int k=0; k<=m; k++) 
    { 
        float temp = mat[i][k]; 
        mat[i][k] = mat[j][k]; 
        mat[j][k] = temp; 
    } 
} 
  
// function to print matrix content at any stage 
void print(float **mat, int m) 
{ 
    for (int i=0; i<m; i++, printf("\n")) 
        for (int j=0; j<=m; j++) 
            printf("%lf ", mat[i][j]); 
  
    printf("\n"); 
} 
  
// function to reduce matrix to r.e.f. 
int forwardElim(float **mat, int m) 
{ 
    for (int k=0; k<m; k++) 
    { 
        // Initialize maximum value and index for pivot 
        int i_max = k; 
        int v_max = mat[i_max][k]; 
  
        /* find greater amplitude for pivot if any */
        for (int i = k+1; i < m; i++) 
            if (abs(mat[i][k]) > v_max) 
                v_max = mat[i][k], i_max = i; 
  
        /* if a prinicipal diagonal element  is zero, 
         * it denotes that matrix is singular, and 
         * will lead to a division-by-zero later. */
        if (!mat[k][i_max]) 
            return k; // Matrix is singular 
  
        /* Swap the greatest value row with current row */
        if (i_max != k) 
            swap_row(mat, k, i_max, m); 
  
  
        for (int i=k+1; i<m; i++) 
        { 
            /* factor f to set current row kth elemnt to 0, 
             * and subsequently remaining kth column to 0 */
            float f = mat[i][k]/mat[k][k]; 
  
            /* subtract fth multiple of corresponding kth 
               row element*/
            for (int j=k+1; j<=m; j++) 
                mat[i][j] -= mat[k][j]*f; 
  
            /* filling lower triangular matrix with zeros*/
            mat[i][k] = 0; 
        } 
  
        //print(mat);        //for matrix state 
    } 
    //print(mat);            //for matrix state 
    return -1; 
} 
  
// function to calculate the values of the unknowns 
void backSub(float **mat, int m, float *sol) 
{ 
    float x[m];  // An array to store solution 
  
    /* Start calculating from last equation up to the 
       first */
    for (int i = m-1; i >= 0; i--) 
    { 
        /* start with the RHS of the equation */
        x[i] = mat[i][m]; 
  
        /* Initialize j to i+1 since matrix is upper 
           triangular*/
        for (int j=i+1; j<m; j++) 
        { 
            /* subtract all the lhs values 
             * except the coefficient of the variable 
             * whose value is being calculated */
            x[i] -= mat[i][j]*x[j]; 
        } 
  
        /* divide the RHS by the coefficient of the 
           unknown being calculated */
        x[i] = x[i]/mat[i][i]; 
    } 
  
    // printf("\nSolution for the system:\n"); 
    for (int i=0; i<m; i++) {
        // printf("%lf\n", x[i]);
        sol[i] = x[i]; 
    }
} 