#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <vector>
#include <algorithm>

#define N 10  //Number of vertices

using namespace std;

int main(){
    random_device rd;
    mt19937 mersenne(rd());
    uniform_int_distribution<> out(0, N - 1);

    bool DENSE = true;
    
    ofstream inputWrite;
    inputWrite.open("input.txt");

    inputWrite << N << endl;

    for(int node = 1; node <= N; node++){
        bool PASS = false;
        int numOut;
        while(!PASS){
            numOut = out(mersenne);
            if((DENSE && numOut >= N/2) || (!DENSE && numOut < N/2)) PASS = true;
        }
        inputWrite << numOut;
        vector<int> saveOut;

        while(saveOut.size() != numOut){
            int newOut = out(mersenne) + 1;
            if(newOut != node){
                auto itr = find(saveOut.begin(), saveOut.end(), newOut);
                if(itr == saveOut.end()) saveOut.push_back(newOut);
            }
        }
        sort(saveOut.begin(), saveOut.end());
        for(int i = 0; i < saveOut.size(); i++){
            inputWrite << " " << saveOut[i];
        }
        if(node != N) inputWrite << "\n";
    }

    inputWrite.close();
    return 0;
}