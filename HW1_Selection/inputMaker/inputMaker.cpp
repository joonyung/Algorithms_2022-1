#include <iostream>
#include <fstream>
#include <string>
#include <random>

#define N 50000  //Number of elements
#define MAX 99999999  //Range[-MAX : MAX]


using namespace std;



int main(){
    random_device rd;
    mt19937 mersenne(rd());
    uniform_int_distribution<> ran(-MAX, MAX);
    uniform_int_distribution<> idx(1, N);
    
    ofstream inputWrite;
    inputWrite.open("input.txt");

    inputWrite << N << endl;

    for(int i = 0; i < N; i++){
        inputWrite << ran(mersenne) << " ";
    }

    int target = idx(mersenne);
    inputWrite << "\n" << target;

    inputWrite.close();
    return 0;
}