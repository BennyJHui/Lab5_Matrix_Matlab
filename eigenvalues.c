// Name: Benny Hui, Rishi Singh
// Student ID: 29738390, 17722430
// CWL: Bhui05, rishisin

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"
#define BUFSIZE 256

int main(void) {

    /* Variables */
    char buffer[BUFSIZE + 1];
    Engine* ep = NULL; // A pointer to a MATLAB engine object
    mxArray* testArray = NULL, * result = NULL; // mxArray is the fundamental type underlying MATLAB data
    double time[3][3] = { { 1.0, 2.0, 3.0 }, {4.0, 5.0, 6.0 }, {7.0, 8.0, 9.0 } }; // Our test 'matrix', a 2-D array

    /* Starts a MATLAB process */
    if (!(ep = engOpen(NULL))) { 
        fprintf(stderr, "\nCan't start MATLAB engine\n"); // Error check
        system("pause");
        return 1;
    }

    testArray = mxCreateDoubleMatrix(3, 3, mxREAL); // Creates a 3 by 3 array

    memcpy((void*)mxGetPr(testArray), (void*)time, 9 * sizeof(double)); // copies into memory the double array "time" into the matlab test array

    if (engPutVariable(ep, "testArray", testArray)) { // Stores the values of the "time" array into the matlab test array from memory
        fprintf(stderr, "\nCannot write test array to MATLAB \n"); // Error check
        system("pause");
        exit(1); // Same as return 1;
    }

    if (engEvalString(ep, "testArrayEigen = eig(testArray)")) { // Prints command into matlab which gets the eigenvalues of the test array
        fprintf(stderr, "\nError calculating eigenvalues  \n"); // Error check
        system("pause");
        exit(1);
    }

    printf("\nRetrieving eigenvector\n");
    if ((result = engGetVariable(ep, "testArrayEigen")) == NULL) { // Stores the eigenvalues into result
        fprintf(stderr, "\nFailed to retrieve eigenvalue vector\n"); // Error check
        system("pause");
        exit(1);
    }
    else {
        size_t sizeOfResult = mxGetNumberOfElements(result); // Retrieves size of result
        size_t i = 0;
        printf("The eigenvalues are:\n");
        for (i = 0; i < sizeOfResult; ++i) {
            printf("%f\n", *(mxGetPr(result) + i)); // Prints out all the eigenvalues
        }
    }

    if (engOutputBuffer(ep, buffer, BUFSIZE)) {
        fprintf(stderr, "\nCan't create buffer for MATLAB output\n"); // Error check
        system("pause");
        return 1;
    }
    buffer[BUFSIZE] = '\0';

    engEvalString(ep, "whos"); // whos is a handy MATLAB command that generates a list of all current variables
    printf("%s\n", buffer);

    mxDestroyArray(testArray); // Removes the test array from Matlab
    mxDestroyArray(result); // Removes the result from Matlab
    testArray = NULL;
    result = NULL;
    if (engClose(ep)) { // closes Matlab engine
        fprintf(stderr, "\nFailed to close MATLAB engine\n");
    }

    system("pause"); // So the terminal window remains open long enough for you to read it
    return 0; // Because main returns 0 for successful completion
}