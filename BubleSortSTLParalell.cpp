#include <iostream>
#include <vector>
#include <algorithm>
#include <execution>
#include <fstream>
#include <random>
#define SIZE 100000
using namespace std::chrono;
template <typename T>
void bubbleSortParallel(std::vector<T>& arr) {
    bool swapped = true;
    while (swapped) {
        swapped = false;

        // Parallel loop for comparing adjacent elements and swapping if necessary
        std::for_each(std::execution::par, arr.begin(), arr.end() - 1, [&](T& a) {
            auto next = std::next(&a);
            if (*next < a) {
                std::iter_swap(&a, next);
                swapped = true;
            }
            });
    }
}
template <typename T>
void numberGenerator(std::vector<T>& arr) {
    std::random_device rd;
    std::mt19937 gen(rd());
    int minNumber = 1;
    int maxNumber = 100000;

    // Create a uniform distribution for the range
   
    for (int i = 0; i < SIZE; i++) {
        std::uniform_int_distribution<int> dist(minNumber, maxNumber);
        arr[i] = dist(gen);
    }
    // Generate a random number
   
}
int main() {
    
    std::ofstream MyFile("filename.txt");


    std::vector<int> arr(SIZE);
    numberGenerator(arr);
    auto start = high_resolution_clock::now();
    bubbleSortParallel(arr);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    std::cout << "Sorted array: ";
    for (uint_fast32_t i = 0; i < SIZE; i++) {
        MyFile << i << ". " << arr[i] << "\n";
    }
   

    std::cout << "Time taken by function: "
        << duration.count() << " microseconds" << std::endl;

    MyFile.close();
    return 0;
}