// Name: Benny Hui, Rishi Singh
// Student ID: 29738390, 17722430
// CWL: Bhui05, rishisin

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"
#define BUFSIZE 256

int main(void) {

	Engine* ep = NULL;
	mxArray* A = NULL, * B = NULL, * C = NULL; // Create variables for Matlab (A = matrix A, B = Matrix B, C = Result Matrix
	double matrixA[3][3] = { {1.0, 4.0, 7.0}, {2.0, 5.0, 8.0}, {3.0, 6.0, 9.0} }; // Sets the values of the matrix
	double matrixB[3][3] = { {2.0, 3.0, 4.0}, {2.0, 3.0, 4.0}, {2.0, 3.0, 4.0} }; // Sets the values of the matrix

	/* Starts a MATLAB process */
	if (!(ep = engOpen(NULL))) {
		fprintf(stderr, "\nCan't start MATLAB engine\n"); // Error checks
		system("pause");
		return 1;
	}

	A = mxCreateDoubleMatrix(3, 3, mxREAL); // Creates matrix into Matlab as a 3 by 3 matrix
	B = mxCreateDoubleMatrix(3, 3, mxREAL); // Creates matrix into Matlab as a 3 by 3 matrix


	memcpy((void*)mxGetPr(A), (void*)matrixA, 9 * sizeof(double)); // copies into memory the double array "matrixA" into the Matlab "A"
	memcpy((void*)mxGetPr(B), (void*)matrixB, 9 * sizeof(double)); // copies into memory the double array "matrixB" into the Matlab "B"

	if (engPutVariable(ep, "A", A)) { // Stores the values of the "A" array into the matlab "A" from memory
		fprintf(stderr, "\nCannot write A array to MATLAB \n"); // Error checks
		system("pause");
		exit(1); // Same as return 1;
	}

	if (engPutVariable(ep, "B", B)) { // Stores the values of the "B" array into the matlab "B" from memory
		fprintf(stderr, "\nCannot write B array to MATLAB \n"); // Error checks
		system("pause");
		exit(1); // Same as return 1;
	}

	if (engEvalString(ep, "Result = B * A;")) { // Prints command into matlab which gets the product from the two arrays A and B and stores into "Result"
		fprintf(stderr, "\nError calculating product matrix  \n"); // Error checks
		system("pause");
		exit(1);
	}

	if ((C = engGetVariable(ep, "Result")) == NULL) { // // Stores the matrix product into result
		fprintf(stderr, "\nFailed to retrieve product matrix\n"); // Error checks
		system("pause");
		exit(1);
	}
	else {
		size_t i = 0;
		size_t j = 0;
		printf("The first matrix was: \n");
		for (i = 0; i < 3; i++) { // Prints out the double matrix of "matrixA" by going through each row and printing out all the columns within the row
			for (j = 0; j < 3; j++) {
				printf("%lf ", matrixA[i][j]);
			}
			printf("\n"); // New line after all the columns within the row has been printed
		}
		printf("The second matrix was: \n");
		for (i = 0; i < 3; i++) { // Prints out the double matrix of "matrixB" by going through each row and printing out all the columns within the row
			for (j = 0; j < 3; j++) {
				printf("%lf ", matrixB[i][j]);
			}
			printf("\n"); // New line after all the columns within the row has been printed
		}
		printf("The matrix product is: \n");
		double* Data = mxGetPr(C); // Stores the results of the matrix product into data
		int count = 0;
		for (i = 0; i < 9; i++) { // Prints out the data of the product matrix by going through each value and when it 
			count++; // reaches the dimension size of 3, it creates a new line to print the next row from the product matrix
			printf("%lf ", Data[i]);
			if (count == 3) {
				printf("\n"); // Prints new line when condition is met
				count = 0; // resets count when condition is met
			}
		}

	}

	mxDestroyArray(C); // Removes "C" from Matlab
	mxDestroyArray(A); // Removes "A" from Matlab
	mxDestroyArray(B); // Removes "B" from Matlab
	C = NULL;
	A = NULL;
	B = NULL;

	if (engClose(ep)) { // Closes Matlab engine
		fprintf(stderr, "\nFailed to close MATLAB engine\n");
	}

	system("pause"); // So the terminal window remains open long enough for you to read it
	return 0; // Because main returns 0 for successful completion

}