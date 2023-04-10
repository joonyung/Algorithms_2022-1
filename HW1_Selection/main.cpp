#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <chrono>

using namespace std;

class Selection{
public:
Selection() {
    //for random pivor in Randomized-select
    random_device rd;
    mersenne = mt19937(rd());
}
void readInput(string inputPath){
    ifstream ifs(inputPath);
    ifs >> inputSize;
    int dataIn;
    while(!ifs.eof()){
        ifs >> dataIn;
        inputData.push_back(dataIn);
    }
    ifs.close();

    targetIndex = inputData.back();
    inputData.pop_back();

    if(inputSize != inputData.size()) cout << "Error : Wrong Input!" << endl;
}
void processRandom(){
    auto start = chrono::system_clock::now();
    resultRandomized = randomizedSelect(inputData, 0, inputSize - 1, targetIndex);
    auto end = chrono::system_clock::now();
    auto durationTime = chrono::duration_cast<chrono::microseconds>(end - start);

    //print on console
    // cout << resultRandomized << endl;
    // cout << durationTime.count() << endl;
    
    ofstream outRandom;
    outRandom.open("random.txt");
    outRandom << resultRandomized << endl;
    outRandom << durationTime.count() << "us";
    outRandom.close();

    
}
void processDeter(){
    auto start = chrono::system_clock::now();
    resultLinear = linearSelect(inputData, 0, inputSize - 1, targetIndex);
    auto end = chrono::system_clock::now();
    auto durationTime = chrono::duration_cast<chrono::microseconds>(end - start);
    
    //print on console
    // cout << resultLinear << endl;
    // cout << durationTime.count() << endl;

    ofstream outDeter;
    outDeter.open("deter.txt");
    outDeter << resultRandomized << endl;
    outDeter << durationTime.count() << "us";
    outDeter.close();
}
int getInputSize(){
    return inputSize;
}
vector<int> getInputData(){
    return inputData;
}
int getTargetIndex(){
    return targetIndex;
}
int getResultRandomized(){
    return resultRandomized;
}
int getResultLinear(){
    return resultLinear;
}

private:
int inputSize;
vector<int> inputData;
int targetIndex;
int resultRandomized;
int resultLinear;
mt19937 mersenne;

int randomizedSelect(vector<int> A, int p, int r, int i){
    if(p == r) return A[r];
    int q = randomizedPartition(A, p, r);
    int k = q - p + 1;

    if(i == k) return A[q];
    else if(i < k) return randomizedSelect(A, p, q - 1, i);
    else return randomizedSelect(A, q + 1, r, i - k);
}
int randomizedPartition(vector<int>& A, int p, int r){
    int ran = getRandom(p, r);
    swapElements(A, ran, r);

    return partition(A, p, r);
}
int partition(vector<int>& A, int p, int r){
  int pivot = A[r];
  int i = p - 1;
  
  for(int j = p; j < r; j++){
      if(A[j] <= pivot){
          swapElements(A, ++i, j);
      }
  }
  swapElements(A, ++i, r);

  return i;
}
void swapElements(vector<int>& A, int i, int j){
    int temp = A[i];
    A[i] = A[j];
    A[j] = temp;
}
int getRandom(int start, int end){
    uniform_int_distribution<> ran(start, end);
    int ranInt = ran(mersenne);

    return ranInt;
}

int linearSelect(vector<int> A, int p, int r, int i){
    int groupSize = 5;
    int median = 0;
    
    if(r - p + 1 <= groupSize){
        insertionSort(A, p, r);
        return A[p + i - 1];
    }

    median = getMedian(A, p, r, groupSize);

    int q = linearPartition(A, p, r, median);
    int k = q - p + 1;

    if(i == k) return A[q];
    else if(i < k) return linearSelect(A, p, q - 1, i);
    else return linearSelect(A, q + 1, r, i - k);
}
int linearPartition(vector<int>& A, int p, int r, const int& median){
    int medianIdx = p;
    for(auto itr = A.begin() + p; itr != A.begin() + r + 1; itr++){
        if(*itr == median) break;
        medianIdx++;
    }
    swapElements(A, medianIdx, r);

    return partition(A, p, r);
}
void insertionSort(vector<int>& A, int p, int r){
    for(int i = p + 1; i <= r; i++){
        int temp = A[i];
        int j = i - 1;

        while(j >= p && A[j] > temp){
            A[j + 1] = A[j];
            j--;
        }
        A[j + 1] = temp;
    }
}
int getMedian(const vector<int>& A, int p, int r, int groupSize){
    vector<vector<int>> groups = divideGroup(A, p, r, groupSize);
    vector<int> medians;

    for(auto itr = groups.begin(); itr != groups.end(); itr++){
        insertionSort(*itr, 0, itr->size() - 1);
        int med = (*itr)[(itr->size() - 1)/2];
        medians.push_back(med);
    }

    insertionSort(medians, 0, medians.size() - 1);
    return medians[(medians.size() - 1) / 2];
}
vector<vector<int>> divideGroup(const vector<int>& A, int p, int r, const int& groupSize){
    vector<vector<int>> groups;

    groups.push_back(vector<int>());
    auto groupItr = groups.begin();
    int groupNum = 0;
    int count = 0;
    for(auto itr = A.begin() + p; itr != A.begin() + r + 1; itr++){
        if(count >= groupSize){
            groups.push_back(vector<int>());
            groupNum++;
            groupItr = groups.begin() + groupNum;
            count = 0;
        }
        groupItr->push_back(*itr);
        count++;
    }

    return groups;
}

};

class Checker{
public:
Checker(Selection result){
    //take the result of Selection as an input
    inputSize = result.getInputSize();
    inputData = result.getInputData();
    targetIndex = result.getTargetIndex();
    resultRandomized = result.getResultRandomized();
    resultLinear = result.getResultLinear();

    random = false;
    deter = false;
}
void check(){
    checkingRandomized();
    checkingLinear();

    ofstream outResult;
    outResult.open("result.txt");
    if(random) outResult << "Randomized-select : Correct" << endl;
    else outResult << "Randomized-select : Wrong" << endl;
    if(deter) outResult << "Deterministic-select : Correct" << endl;
    else outResult << "Deterministic-select : Wrong" << endl;
    outResult.close();
}
void checkingRandomized(){
    //count the number of smaller, same, and bigger one compared to the target, respectively.
    int duplicate = 0;
    int countSmaller = 0;
    int countBigger = 0;
    for(auto itr = inputData.begin(); itr != inputData.end(); itr++){
        int elem = *itr;
        if(elem < resultRandomized) countSmaller++;
        else if(elem == resultRandomized) duplicate++;
        else countBigger++;
    }

    if(targetIndex > countSmaller && targetIndex <= countSmaller + duplicate) random = true;
    // if(countSmaller + duplicate + countBigger != inputSize) cout << "something wrong in Checker" << endl; 
}
void checkingLinear(){
    int duplicate = 0;
    int countSmaller = 0;
    int countBigger = 0;
    for(auto itr = inputData.begin(); itr != inputData.end(); itr++){
        int elem = *itr;
        if(elem < resultLinear) countSmaller++;
        else if(elem == resultLinear) duplicate++;
        else countBigger++;
    }

    if(targetIndex > countSmaller && targetIndex <= countSmaller + duplicate) deter = true;
    // if(countSmaller + duplicate + countBigger != inputSize) cout << "something wrong in Checker" << endl; 
}

private:
int inputSize;
vector<int> inputData;
int targetIndex;
int resultRandomized;
int resultLinear;
bool random = false;
bool deter = false;
};


int main(){
    Selection sample;
    sample.readInput("input.txt");
    sample.processRandom();
    sample.processDeter();

    Checker checker(sample);
    checker.check();

    return 0;
}