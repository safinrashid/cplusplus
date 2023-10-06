// matrix-multiplication <Project2.cpp>
// ECE 312 Project 2 submission by
// Safin Rashid
// srr3288
// Slip days used: <0>
// Fall 2022

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "MatrixMultiply.h"

//included for debugging
void printMatrix(double a[], const uint32_t rows, const uint32_t cols);
void printMatrixPtr(double** a, const uint32_t rows, const uint32_t cols);
void memoryRelease(double** ptr, int length);

void multiplyMatrices(
        double a[],
        const uint32_t a_rows,
        const uint32_t a_cols, //= b_rows
        double b[],
        const uint32_t b_cols,
        double c[]) {
    //A_ij is stored in the location a[i*a_cols+j]

    int i = 0,j,l,
        m = a_rows, n = a_cols, k = b_cols;//c_rows = m, c_col = k n = b_rows
    double sum;

    //multiply into C:
    while(i < m){//each row in A
        j = 0;
        while(j < k){//each column in B
           l = 0;
           sum = 0; //init C to 0
            while(l < n){//"k loop"
                sum += a[i * n + l] * //traverse row elements to multiply/sum
                        b[l * k + j]; //traverse col
                c[i * k + j] = sum;
                l++;
            }
            j++;
        }
        i++;
    }
    //printf("\n");
    //printMatrix(c, m, k); //Debugging
    // https://en.wikipedia.org/wiki/Row-_and_column-major_order
}


double** multiplyMatricesPtr(
        double** a,
        const uint32_t a_rows,
        const uint32_t a_cols,
        double** b,
        const uint32_t b_cols) {

    //allocate product matrix in heap
    double** c = (double **) malloc(sizeof(double *) * a_rows);//allocate array of row ptrs
    for (int i = 0; i < a_rows; i++) {//for each row
        c[i] = (double *) malloc(sizeof(double) * b_cols);//allocate,declare num of cols
    }

    int i, j, l;
    double sum;
    //multiply into C:
    for(i=0; i < a_rows; i++){//each row in A
        for(j=0; j < b_cols; j++){//each column in B
            sum = 0; //inits would-be C[][] element to 0
            for(l=0; l < a_cols; l++){//iterate through A row elements

                sum += a[i][l] * b[l][j];
                c[i][j] = sum;

            }
        }
    }
    //printf("\n");
    //printMatrixPtr(c, a_rows, b_cols); //Debugging
    return c;
}

// https://en.wikipedia.org/wiki/Matrix_(mathematics)#Submatrix
double** createSubMatrix(
         double** a,            //pointer to matrix
         const uint32_t a_rows, //old number of ROW (M)
         const uint32_t a_cols, //old number of COL (N)
		 const uint32_t row_x,  //ROW to remove (0 index)
		 const uint32_t col_y) {//COL to remove (0 index)

    int a_rowsNew = a_rows - 1, a_colsNew = a_cols - 1, i_new, j_new;
    double** subMatrix = (double **) malloc(sizeof(double *) * a_rowsNew);//allocate array of row ptrs
    for (int i = 0; i < a_rowsNew; i++) {
        subMatrix[i] = (double *) malloc(sizeof(double) * a_colsNew);//per row, allocate num of cols
    }
/*
    printf("\n");
    printMatrixPtr(a, a_rows, a_cols); //Debugging
    printf("\n");
*/
    i_new=0;
    for(int i = 0; i < a_rows; i++){            //store row by row
        j_new=0;
        if(i != row_x) {                        //skips row to remove
            for (int j = 0; j < a_cols; j++) {  //store columns
                if(j != col_y){                 //skips col to remove
                    subMatrix[i_new][j_new] = a[i][j];
                    j_new++;
                }
            }
            i_new++;
        }
    }

/*
    printf("\n");
    printMatrixPtr(subMatrix, a_rowsNew, a_colsNew); //Debugging
    printf("\n");
*/

    return subMatrix;//return double** to new matrix in heap
}
