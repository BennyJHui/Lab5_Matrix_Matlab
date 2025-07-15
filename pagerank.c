// Name: Benny Hui, Rishi Singh
// Student ID: 29738390, 17722430
// CWL: Bhui05, rishisin

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"

// Create size for array and buffer
#define SIZE 1024

int main(void) {

    FILE* FP = NULL; // File pointer
    int count = 0;
    int binary;
    int x = 1;
    double number[SIZE];
    Engine* ep = NULL;
    mxArray* V = NULL;
    mxArray* R = NULL;

    FP = fopen("web.txt", "r"); // Opens text file

    while (fscanf(FP, "%d", &binary) == 1) { // Takes the value from the file one at a time 
        number[count] = binary; // Stores the value taken from file into array
        count++;
    }

    fclose(FP); // Close file 

    while (x * x != count) { // Gets the dimension of the matrix since we know it is a square, 
        x++; // we get dimension until the squared value equates to the total number of values in the file
    }

    double** matrix = (double**)malloc(x * sizeof(double*)); // Allocates memory for double matrix since it points 
    for (int i = 0; i < x; i++) { // from the matrix to the row and then pointing to the columns
        matrix[i] = (double*)malloc(x * sizeof(double)); // Allocates memory for each row dependent on the dimension of the matrix
    }

    fseek(FP, 0, 0); // Resets file pointer back to origin

    int shift = 0;
    for (int i = 0; i < x; i++) { // Stores all the values from the single matrix into the double matrix
        for (int j = 0; j < x; j++) {
            matrix[i][j] = number[shift];
            shift++;
        }
    }

    if (!(ep = engOpen(NULL))) { // Starts Matlab engine
        fprintf(stderr, "\nCan't start MATLAB engine\n");
        system("pause");
        return 1;
    }

    V = mxCreateDoubleMatrix(x, x, mxREAL); // Creates matrix x by x depending on matrix dimension

    for (int i = 0; i < x; i++) { // Gets the transpose matrix from the original double matrix 
        for (int j = 0; j < x; j++) { // Setting it into array V
            mxGetPr(V)[j*x+i] = matrix[i][j];
        }
    }

    if (engPutVariable(ep, "ConnectivityMatrix", V)) {
        fprintf(stderr, "\nCannot write V array to MATLAB \n"); // Error check
        system("pause");
        exit(1); // Same as return 1;
    }

    if (engEvalString(ep, "[rows, columns] = size(ConnectivityMatrix)")) { // Gets the dimensions of the matrix
        fprintf(stderr, "\nError [rows, columns]  \n"); // Error check
        system("pause");
        exit(1);
    }

    if (engEvalString(ep, "dimension = size(ConnectivityMatrix, 1)")) { // Gets the dimension size
        fprintf(stderr, "\nError dimension = size(ConnectivityMatrix, 1) \n"); // Error check
        system("pause");
        exit(1);
    }

    if (engEvalString(ep, "columnsums = sum(ConnectivityMatrix, 1)")) { // Sums up each column
        fprintf(stderr, "\nError columnsums = sum(ConnectivityMatrix, 1)  \n"); // Error check
        system("pause");
        exit(1);
    }

    if (engEvalString(ep, "p = 0.85;")) { // Probability factor
        fprintf(stderr, "\nError p = 0.85;  \n"); // Error check
        system("pause");
        exit(1);
    }

    if (engEvalString(ep, "zerocolumns = find(columnsums~=0)")) { // Finds indices of column which sum is not zero
        fprintf(stderr, "\nError zerocolumns = find(columnsums~=0)  \n"); // Error check
        system("pause");
        exit(1);
    }

    if (engEvalString(ep, "D = sparse( zerocolumns, zerocolumns, 1./columnsums(zerocolumns), dimension, dimension)")) { // Creates a sparse matrix
        fprintf(stderr, "\nError D = sparse  \n"); // Error check
        system("pause");
        exit(1);
    }

    if (engEvalString(ep, "StochasticMatrix = ConnectivityMatrix * D")) { // Create stochastic matrix
        fprintf(stderr, "\nError StochasticMatrix = ConnectivityMatrix * D  \n"); // Error check
        system("pause");
        exit(1);
    }

    if (engEvalString(ep, "[row, column] = find(columnsums==0)")) { // Find the zero column of original connectivity matrix
        fprintf(stderr, "\nError [row, column] = find(columnsums==0)  \n"); // Error check
        system("pause");
        exit(1);
    }

    if (engEvalString(ep, "StochasticMatrix(:, column) = 1./dimension")) { // Create Stochastic Matrix
        fprintf(stderr, "\nError StochasticMatrix(:, column) = 1./dimension  \n"); // Error check
        system("pause");
        exit(1);
    }

    if (engEvalString(ep, "Q = ones(dimension, dimension)")) { // Generates transition matrix
        fprintf(stderr, "\nError Q  \n"); // Error check
        system("pause");
        exit(1);
    }

    if (engEvalString(ep, "TransitionMatrix = p * StochasticMatrix + (1 - p) * (Q/dimension)")) { // Create transition matrix
        fprintf(stderr, "\nError TransitionMatrix \n"); // Error check
        system("pause");
        exit(1);
    }

    if (engEvalString(ep, "PageRank = ones(dimension, 1)")) {
        fprintf(stderr, "\nError PageRank = ones(dimension, 1)  \n"); // Error check
        system("pause");
        exit(1);
    }

    if (engEvalString(ep, "for i = 1:100; PageRank = TransitionMatrix * PageRank; end")) {
        fprintf(stderr, "\nError for loop  \n"); // Error check
        system("pause");
        exit(1);
    }

    if (engEvalString(ep, "PageRank = PageRank / sum(PageRank)")) { // Stores results into "PageRank"
        fprintf(stderr, "\nError calculating pagerank  \n"); // Error check
        system("pause");
        exit(1);
    }

    if ((R = engGetVariable(ep, "PageRank")) == NULL) { // Stores results into "R"
        fprintf(stderr, "\nFailed to retrieve pagerank\n"); // Error check
        system("pause");
        exit(1);
    } else {
        printf("NODE  RANK\n");
        printf("---   ----\n");
        size_t sizeofR = mxGetNumberOfElements(R); // Gets size of the result
        double* Data = mxGetPr(R); // Stores result into Data
        int j = 1;
        for (int i = 0; i < sizeofR; i++) { // Prints out each individual pagerank result along with the node of the pagerank
            printf("%d     %0.4lf ", j, Data[i]);
            printf("\n");
            j++;
        }
    }

    mxDestroyArray(V); // Removes "V" from Matlab
    mxDestroyArray(R); // Removes "R" from Matlab
    V = NULL;
    R = NULL;

    if (engClose(ep)) { // Closes Matlab engine
        fprintf(stderr, "\nFailed to close MATLAB engine\n");
    }

    system("pause"); // So the terminal window remains open long enough for you to read it
    return 0;
}