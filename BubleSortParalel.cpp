#include <mpi.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <fstream>

using namespace std;

void merge(int array[], int const left, int const mid, int const right)
{
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;

    auto* leftArray = new int[subArrayOne];
    auto* rightArray = new int[subArrayTwo];

    for (auto i = 0; i < subArrayOne; i++) {
        leftArray[i] = array[left + i];
    }
    for (auto j = 0; j < subArrayTwo; j++) {
        rightArray[j] = array[mid + 1 + j];
    }

    auto indexOfSubArrayOne = 0;
    auto indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left;

    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else {
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }

    while (indexOfSubArrayOne < subArrayOne) {
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }
    while (indexOfSubArrayTwo < subArrayTwo) {
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
    delete[] leftArray;
    delete[] rightArray;
}

void mergeSort(int array[], int const begin, int const end)
{
    if (begin >= end) return;

    auto mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);
    merge(array, begin, mid, end);
}

void printArray(int A[], int size)
{
    for (auto i = 0; i < size; i++) {
        cout << A[i] << " ";
    }
    cout << endl;
}

void printArrayWithSingleOccurrenceOfTheElement(int A[], int size)
{
    int prev = A[0];

    for (auto i = 1; i < size; i++) {
        if (A[i] != prev) {
           
            prev = A[i];
        }
    }
    cout << endl;
}

int main(int argc, char** argv)
{
    ofstream MyFile("filename.txt");
    srand(time(NULL));
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 500000;
    int* arr = new int[n];
    int* sortedArr = new int[n];
    if (rank == 0) {
       
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % 100000;
        }

        // write unsorted array to file
       
       
        for (int i = 0; i < n; i++) {
            MyFile << arr[i] << endl;
        }
       
    }

    auto startTime = chrono::high_resolution_clock::now();

    MPI_Scatter(arr, n/size, MPI_INT, sortedArr, n / size, MPI_INT, 0, MPI_COMM_WORLD);

    int start = rank * (n / size);
    int end = start + (n / size) - 1;

    if (rank == size - 1) {
        end = n - 1;
    }

    mergeSort(sortedArr, start, end);

    MPI_Gather(sortedArr, n/ size, MPI_INT, arr, n/ size, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank == 0)
    {
       

        mergeSort(arr, 0, n - 1);
        auto endTime = chrono::high_resolution_clock::now();

        // write sorted array to file
        MyFile << "sorted elemetns: \n";
        for (int i = 0; i < n; i++) {
            MyFile << arr[i] << endl;
        }
        MyFile.close();

        cout << "Time taken: " << chrono::duration_cast<chrono::microseconds>(endTime - startTime).count() << " microseconds." << endl;

        // print array with single occurrence of each element
        printArrayWithSingleOccurrenceOfTheElement(sortedArr, n);

        delete[] sortedArr;
    }

    delete[] arr;

    MPI_Finalize();
    return 0;
}