# Parallel LU Decompositon

This directory consists of the following files: 
 - main.cpp
 - util.cpp
 - input.txt
 - Report.pdf
 - readme.MD


<hr>


`input.txt` is a sample input file. This file contains the following: 

The first line contains two numbers (seperated by space)
 - `1` (to give input matrix) or `0` (to use randomly generated matrix)
 - `1` (to compare with sequential) or `0` (to skip the comparison)

The second line contains two numbers (seperated by space) 
 - `N` the size of matrix
 - `p` number of threads

If you have chosen to provide the input matrix, enter the values of the matrix from here. 


<hr>


The actual file to be executed is `main.cpp`. To execute this file, 
 - `g++ main.cpp -lpthread`
 - `./a.out`


<hr>


The output file named `output.txt` is created in the same directory. It contains the following information: 
 - `N` and `p` read from the input file
 - Total time taken for _Parallel_ computation
 - Total time taken for _Sequential_ execution (if set to 1)
 - Original matrix (input)
 - Resultant matrix of _Parallel_ computation
 - Resultant matrix of _Sequential_ computation (if applicable)

The time taken for _Sequential_ and _Parallel_ executions is printed on the terminal as well. 

<hr>
