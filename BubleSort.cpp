#include<iostream>
#include <chrono>
#include <fstream>
#include <cstdlib> 
#include <ctime> 
#include <random>
#include "mpi.h"
using namespace std;
using namespace std::chrono;
#define NRPROC 10
#define SIZE 150000
#define LOCALSIZE SIZE/NRPROC
void numberGenerator(uint32_t  sir[], int dimensiune);
void bubbleSort(uint32_t  array[], int size);
int main(int argc, char* argv[])
{
    mt19937 gen(random_device{}());
    int rank;
    int nrproc;
 
    uint32_t  localArray[SIZE];
    uint32_t  a[SIZE];
    ofstream MyFile("filename.txt");

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nrproc);

    if (rank == 0) {
        numberGenerator(a, SIZE);
    }
    auto start = high_resolution_clock::now();
    
    MPI_Scatter(a, LOCALSIZE, MPI_INT, localArray, LOCALSIZE, MPI_INT, 0, MPI_COMM_WORLD);
    srand(static_cast<unsigned int>(time(nullptr)));
    auto localStart = high_resolution_clock::now();
 
    
    bubbleSort(localArray, LOCALSIZE);

    auto localStop = high_resolution_clock::now();
    auto localDuration = duration_cast<microseconds>(localStop - localStart);
    cout <<"Rank: "<<rank<< " Time taken : "
        << localDuration.count() << " microseconds" << endl;
    MPI_Gather(localArray, LOCALSIZE, MPI_INT, a, LOCALSIZE, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        bubbleSort(a, SIZE);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        for (int i = 0; i < SIZE; i++) {
            MyFile << i << ". " << a[i] << "\n";

        }
        cout << "Time taken by function: "
            << duration.count() << " microseconds" << endl;
    }
    
   

    MyFile.close();
    MPI_Finalize();
    exit(0);
}
void bubbleSort(uint32_t  array[], int size) {

    for (uint32_t  i = 0; i < size; i++) {

        uint32_t  flag = 0;
        for (uint32_t  j = 0; j < (size - i - 1); j++)
        {
            if (array[j] > array[j + 1]) {
                flag = true;
                uint32_t  temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
        if (flag == 0) {
            break;
        }

    }


}
void numberGenerator(uint32_t  sir[], int dimensiune) {
    mt19937 gen(random_device{}());
    srand(static_cast<unsigned int>(time(nullptr)));
    for (uint32_t  i = 0; i < dimensiune; i++) {
        sir[i] = gen();
    }
}
