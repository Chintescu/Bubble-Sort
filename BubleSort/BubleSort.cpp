#include<iostream>
#include <chrono>
#include <fstream>
#include <cstdlib> 
#include <ctime> 
#include <random>
using namespace std;
using namespace std::chrono;
#define SIZE 10000

void bubbleSort(uint32_t array[]);
int main()
{
    mt19937 gen(random_device{}());
    cout << "Dimension: " << SIZE;
    
    uint32_t a[SIZE];
    ofstream MyFile("filename.txt");
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < SIZE; i++) {
        a[i] = gen();
    }
    cout << endl;
    auto start = high_resolution_clock::now();
    bubbleSort(a);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    

    for (uint32_t i = 0; i < SIZE; i++) {
        MyFile <<i<<". "<< a[i] << "\n";
     
    }
    cout << "Time taken by function: "
        << duration.count() << " microseconds" << endl;
    MyFile << "Time taken by function: "
        << duration.count() << " microseconds" << endl;
    MyFile.close();
    return 0;
}
void bubbleSort(uint32_t array[]) {
 
    for (uint32_t i = 0; i < SIZE; i++) {

        uint32_t flag = 0;
        for (uint32_t j = 0; j < (SIZE-i-1) ; j++)
        {
            if (array[j] > array[j + 1]) {
                flag = true;
                uint32_t temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
        if (flag == 0) {
            break;
        }
       
    }

    
}