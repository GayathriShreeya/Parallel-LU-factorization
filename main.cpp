#include <iostream>
#include <atomic>
#include <vector>
#include <fstream>
#include <time.h>
#include <thread>
#include <string>
#include "util.cpp"
using namespace std;
using namespace std::chrono;


int N, p;
int par_time, seq_time;
bool doSequential;
vector<vector<double>> matrix;
atomic_int* numUpdates;

void readInput();
void manageFiles();
void writeOutput(const vector<vector<double>>& M, const vector<vector<double>>& seq_result);


void runner(int id) {
    vector<int> toCheck;
    for (int i=0; i<N; i++) {
        if (i % p == id) toCheck.push_back(i);
    }
    for (int k=0; k<N-1; k++) {
        // check if the k-th row is updated k times
        while (numUpdates[k].load() != k) {}
        for (auto i: toCheck) {
            if (i > k) {
                matrix[i][k] = matrix[i][k] / matrix[k][k];
                for (int j=k+1; j<N; j++) 
                    matrix[i][j] = matrix[i][j] - matrix[i][k] * matrix[k][j];
                numUpdates[i].fetch_add(1);
            }
        }
    }
    return;
}


int main() {
    readInput();
    cout << "> Input read:" << endl;
    cout << "    Size of matrix, N = " << N << endl;
    cout << "    Number of threads, p = " << p << endl;
    manageFiles();

    vector<vector<double>> M = matrix; // creating a copy
    vector<vector<double>> seq_result;
    numUpdates = (atomic_int*)malloc(sizeof(atomic_int)*N);
    for (int i=0; i<N; i++) numUpdates[i].store(0, std::memory_order_relaxed);
    
    cout << "> Starting parallel LU decomposition" << endl;
    auto par_begin = high_resolution_clock::now();
    vector<thread> threads;
    for (int i=0; i<p; i++) threads.push_back(thread(runner, i));
    cout << "> Created " << p << " threads" << endl;
    for (auto &thread: threads) thread.join();
    auto par_end = high_resolution_clock::now();
    par_time = (int)duration_cast<microseconds>(par_end-par_begin).count();
    cout << "> All threads terminated" << endl;
    cout << "> Parallel LU decomposition of matrix completed" << endl;

    if (doSequential)  {
        cout << "> Starting sequential LU decomposition" << endl;
        auto seq_begin = high_resolution_clock::now();
        seq_result = sequential_LU(M);
        auto seq_end = high_resolution_clock::now();
        seq_time = (int)duration_cast<microseconds>(seq_end-seq_begin).count();
        cout << "> Sequential LU decomposition of matrix completed" << endl;
    }

    cout << "------------------------" << endl;
    cout << "Time taken for parallel LU decomposition: " << par_time << " microseconds" << endl;
    if (doSequential) cout << "Time taken for sequential LU decomposition: " << seq_time << " microseconds" << endl;
    cout << "------------------------" << endl;

    cout << "> Writing the results to output file" << endl;
    writeOutput(M, seq_result);
    cout << "> Results to \'output.txt\' successfully" << endl;
    return 0;
}

void readInput() {
    ifstream input("input.txt");
    if (!input) {
        cout << "readInput(): Input file could not be opened." << endl;
        return;
    }
    int mat_input;
    input >> mat_input >> doSequential;
    input >> N >> p;
    matrix.resize(N, vector<double>(N, 0));
    if (mat_input) {
        for (int i=0; i<N; i++) {
            for (int j=0; j<N; j++) input >> matrix[i][j];
        }
    } else {
        generateMatrix(matrix, N);
    }
    return;
}

void manageFiles() {
    // clearing the output files
    ofstream output("output.txt");
    output.close();
    return;
}

void writeOutput(const vector<vector<double>>& M, const vector<vector<double>>& seq_result) {
    fstream output;
    output.open("output.txt", ios::app);
    time_t start = time(NULL);
    output << "Started writing file at " << ctime(&start) << endl;
    output << "Size of matrix, N = " << N << endl;
    output << "Number of threads, p = " << p << endl;
    output << "------------------------" << endl;
    output << "Time taken for parallel LU decomposition: " << par_time << " microseconds" << endl;
    if (doSequential) output << "Time taken for sequential LU decomposition: " << seq_time << " microseconds" << endl;
    output << "------------------------" << endl;
    output << "INPUT MATRIX: \n" << endl;
    printMatrix(M, "output.txt");
    output << "------------------------" << endl;
    output << "PARALLEL LU DECOMPOSITION RESULT: \n" << endl;
    printMatrix(matrix, "output.txt");
    output << "------------------------" << endl;
    if (doSequential) {
        output << "SEQUENTIAL LU DECOMPOSITION RESULT: \n" << endl;
        printMatrix(seq_result, "output.txt");
        output << "------------------------" << endl;
    }
    time_t end = time(NULL);
    output << "Finished writing file at " << ctime(&end) << endl;
    output.close();
    return;
}
