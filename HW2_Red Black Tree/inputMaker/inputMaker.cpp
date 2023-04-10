#include <iostream>
#include <fstream>
#include <string>
#include <random>

#define N 100000  //Number of elements
#define MAX 100 

using namespace std;

int main(){
    random_device rd;
    mt19937 mersenne(rd());
    uniform_int_distribution<> ran(1, MAX);
    uniform_int_distribution<> idx(1, N);
    uniform_int_distribution<> command(0, 3);
    
    // for(int i = 0; i < 10; i++){
    ofstream Input;
    string fileName = "input.txt";
    Input.open(fileName);

    for(int i = 0; i < N; i++){
        int tempCom = command(mersenne);
        int tempKey = ran(mersenne);
        if(tempCom == 0) Input << "I " << tempKey << endl;
        // else if(tempCom == 1) Input << "D " << tempKey << endl;
        else if(tempCom == 2) Input << "S " << tempKey << endl;
        else if(tempCom == 3) Input << "R " << tempKey << endl;
    }

    Input.close();
    // }
    
    return 0;
}