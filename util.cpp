#include <iostream>
#include <cstdlib>
#include <vector>
#include <time.h>
#include <string>
#include <fstream>
using namespace std;

int randomInteger(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}


/* Generates a strictly column diagonally dominant square
   matrix of size NxN. The off diagonal elements are 
   random integers between -5 and 5. The diagonal elements
   are filled in a way to ensure diagonal dominance. 
*/
void generateMatrix(vector<vector<double>>& matrix, int N) {
    srand(time(0));
    for (int j=0; j<N; j++) {
        int sum = 0;
        for (int i=0; i<N; i++) {
            if (i == j) continue;
            int num = randomInteger(-5, 5);
            matrix[i][j] = (double)num;
            sum += abs(num);
        }
        matrix[j][j] = (double)randomInteger(sum+1, 2*sum);
    }
    return;
}


/* Code to calculate the LU decomposition sequentially
   More details about the algorithm in the report. 
*/
vector<vector<double>> sequential_LU(const vector<vector<double>>& matrix) {
    vector<vector<double>> A = matrix; // creates a copy of matrix
    int N = matrix.size();
    for (int k=0; k<N-1; k++) {
        for (int i=k+1; i<N; i++) {
            A[i][k] = A[i][k] / A[k][k];
        }
        for (int j=k+1; j<N; j++) {
            for (int i=k+1; i<N; i++) {
                A[i][j] = A[i][j] - A[i][k] * A[k][j];
            }
        }
    }
    return A;
}


/* printMatrix function to write the matrix into the file whose
   filename is given as input. 
*/
void printMatrix(const vector<vector<double>>& matrix, string filename) {
    fstream outfile;
    outfile.open(filename, ios::app);
    for (auto it1: matrix) {
        for (auto it2: it1) outfile << it2 << " ";
        outfile << endl;
    }
    outfile.close();
    return;
}


/* This function takes the decomposed matrix as input and 
   returns a unit lower triangular matrix retrieved from input. 
*/
vector<vector<double>> get_L_matrix(const vector<vector<double>>& matrix) {
    int N = matrix.size();
    vector<vector<double>> L(N, vector<double>(N, 0));
    for (int i=0; i<N; i++) {
        for (int j=0; j<i; j++) 
            L[i][j] = matrix[i][j];
        L[i][i] = 1;
    }
    return L;
}


/* This function takes the decomposed matrix as input and
   returns the upper triangular matrix retrieved from input. 
*/
vector<vector<double>> get_U_matrix(const vector<vector<double>>& matrix) {
    int N = matrix.size();
    vector<vector<double>> U(N, vector<double>(N, 0));
    for (int i=0; i<N; i++) {
        for (int j=i; j<N; j++) 
            U[i][j] = matrix[i][j];
    }
    return U;
}