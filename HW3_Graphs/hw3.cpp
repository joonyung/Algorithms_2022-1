#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;

class AdjMat{
public:
    void readInput(string inputPath){
        ifstream ifs(inputPath);
        string line;        
        
        int row = 0;
        bool FIRST = true;

        while(getline(ifs, line)){
            istringstream iss(line);
            if(FIRST){
                FIRST = false;
                iss >> numVtx;
                vector<vector<int>> initMat(numVtx, vector<int>(numVtx, 0));
                mat = initMat;
                continue;
            }
            
            int numDest;
            iss >> numDest;
            if (numDest != 0){
                int dataIn;
                while(iss >> dataIn){
                    mat[row][dataIn - 1] = 1;
                }
            }            
            row++;
        }
        ifs.close();
    }

    void makeSCC(string outputPath){
        auto start = chrono::system_clock::now();
        DFS();
        makeMatReverse();
        reverseDFS();
        auto end = chrono::system_clock::now();
        auto durationTime = chrono::duration_cast<chrono::milliseconds>(end - start);
        
        for(int i = 0; i < SCC.size(); i++){
            sort(SCC[i].begin(), SCC[i].end());
        }

        vector<string> outString;
        for(int i = 0; i < SCC.size(); i++){
            string thisLine;
            for(int out = 0; out < SCC[i].size(); out++){
                if(out == 0) thisLine = to_string(SCC[i][out]);
                else thisLine = thisLine + " " + to_string(SCC[i][out]);
            }
            outString.push_back(thisLine);
        }
        sort(outString.begin(), outString.end());


        ofstream Output;
        Output.open(outputPath);
        for(int i = 0; i < outString.size(); i++){
            Output << outString[i] << endl;
        }
        Output << durationTime.count() << "ms";
        Output.close();
    }

    void DFS(){
        vector<bool> initVisited(numVtx, false);
        visited = initVisited;
        for(int i = 0; i < numVtx; i++){
            if(!visited[i]) aDFS(i);
        }

    }

    void aDFS(int vtx){
        visited[vtx] = true;

        for(int adj = 0; adj < numVtx; adj++){
            if(mat[vtx][adj] == 1 && !visited[adj]) aDFS(adj);
        }
        finStack.push_back(vtx);
    }

    void reverseDFS(){
        vector<bool> initRVisited(numVtx, false);
        rVisited = initRVisited;

        for(int i = numVtx - 1; i >= 0; i--){
            if(!rVisited[finStack[i]]) {
                SCC.push_back(vector<int>());
                aReverseDFS(finStack[i]);
            }
        }
    }

    void aReverseDFS(int vtx){
        rVisited[vtx] = true;
        SCC.back().push_back(vtx + 1);

        for(int adj = 0; adj < numVtx; adj++){
            if(matReverse[vtx][adj] == 1 && !rVisited[adj]) aReverseDFS(adj);
        }
    }

    void makeMatReverse(){
        vector<vector<int>> initRev(numVtx, vector<int>(numVtx, 0));
        matReverse = initRev;
        for(int i = 0; i < numVtx; i++){
            for(int j = 0; j < numVtx; j++){
                matReverse[i][j] = mat[j][i];
            }
        }
    }

    void testPrint(){
        cout << "[Mat] SCC size :" << SCC.size() << endl;
        for(auto itr = SCC.begin(); itr != SCC.end(); itr++){
            for(auto itr2 = (*itr).begin(); itr2 != (*itr).end(); itr2++){
                cout << (*itr2) << " ";
            }
            cout << "\n" << endl;
        }
        
    }
private:
    int numVtx;
    vector<vector<int>> mat;    //Adjacency Matrix
    vector<vector<int>> matReverse; //Transpose of Adjacency Matrix (reversed graph)
    vector<bool> visited;   //visited vertices in DFS
    vector<bool> rVisited;  //visited vertices in DFS of reverse graph
    vector<int> finStack;   //Stack by finished time of DFS
    vector<vector<int>> SCC;    //Strongly connected components
};

class listNode{
public:
    int outNode;
    listNode* nextNode = nullptr;
    listNode(int out){
        outNode = out;
    }
};

class AdjList{
public:
    void readInput(string inputPath){
        ifstream ifs(inputPath);
        string line;        
        bool FIRST = true;

        while(getline(ifs, line)){
            istringstream iss(line);
            if(FIRST){
                FIRST = false;
                iss >> numVtx;
                continue;
            }
            
            int numDest;
            iss >> numDest;
            bool HEAD = true;
            if (numDest != 0){
                int dataIn;
                while(iss >> dataIn){
                    if(HEAD){
                        HEAD = false;
                        headList.push_back(new listNode(dataIn));
                    }
                    else{
                        listNode* tempNode = new listNode(dataIn);
                        listNode* tempLast = headList.back();
                        while(tempLast->nextNode != nullptr) tempLast = tempLast->nextNode;
                        tempLast->nextNode = tempNode;
                    }
                }
            }
            else headList.push_back(nullptr);            
        }
        ifs.close();
    }

    void makeSCC(string outputPath){
        auto start = chrono::system_clock::now();
        makeReverseList();
        DFS();
        reverseDFS();
        auto end = chrono::system_clock::now();
        auto durationTime = chrono::duration_cast<chrono::milliseconds>(end - start);
        
        for(int i = 0; i < SCC.size(); i++){
            sort(SCC[i].begin(), SCC[i].end());
        }

        vector<string> outString;
        for(int i = 0; i < SCC.size(); i++){
            string thisLine;
            for(int out = 0; out < SCC[i].size(); out++){
                if(out == 0) thisLine = to_string(SCC[i][out]);
                else thisLine = thisLine + " " + to_string(SCC[i][out]);
            }
            outString.push_back(thisLine);
        }
        sort(outString.begin(), outString.end());


        ofstream Output;
        Output.open(outputPath);
        for(int i = 0; i < outString.size(); i++){
            Output << outString[i] << endl;
        }
        Output << durationTime.count() << "ms";
        Output.close();
    }

    void makeReverseList(){
        vector<listNode*> tempReverse(headList.size(), nullptr);
        reverseList = tempReverse;
        for(int i = 0; i < headList.size(); i++){
            listNode* tempLast = headList[i];

            while(tempLast != nullptr){
                int outN = tempLast->outNode - 1;
                listNode* newNode = new listNode(i + 1);
                if(reverseList[outN] == nullptr) {
                    reverseList[outN] = newNode;
                }
                else{
                    listNode* tempBack = reverseList[outN];
                    while(tempBack->nextNode != nullptr) tempBack = tempBack->nextNode;
                    tempBack->nextNode = newNode; 
                }
                tempLast = tempLast->nextNode;
            }
        }
    }

    void DFS(){
        vector<bool> initVisited(numVtx, false);
        visited = initVisited;

        for(int i = 0; i < numVtx; i++){
            if(!visited[i]) aDFS(i);
        }
    }

    void aDFS(int vtx){
        visited[vtx] = true;

        listNode* tempNode = headList[vtx];
        while(tempNode != nullptr){
            int adj = tempNode->outNode - 1;
            if(!visited[adj]) aDFS(adj);
            tempNode = tempNode->nextNode;
        }
        finStack.push_back(vtx);
    }

    void reverseDFS(){
        vector<bool> initRVisited(numVtx, false);
        rVisited = initRVisited;

        for(int i = numVtx - 1; i >= 0; i--){
            if(!rVisited[finStack[i]]) {
                SCC.push_back(vector<int>());
                aReverseDFS(finStack[i]);
            }
        }
    }

    void aReverseDFS(int vtx){
        rVisited[vtx] = true;
        SCC.back().push_back(vtx + 1);

        listNode* tempNode = reverseList[vtx];
        while(tempNode != nullptr){
            int adj = tempNode->outNode - 1;
            if(!rVisited[adj]) aReverseDFS(adj);
            tempNode = tempNode->nextNode;
        }
    }
    
    void testPrint(){
        cout << "[List] SCC size : " << SCC.size() << endl;
        for(auto itr = SCC.begin(); itr != SCC.end(); itr++){
            for(auto itr2 = (*itr).begin(); itr2 != (*itr).end(); itr2++){
                cout << (*itr2) << " ";
            }
            cout << "\n" << endl;
        }
    }

private:
    int numVtx;
    vector<listNode*> headList;     //List storing header of listNode pointer
    vector<listNode*> reverseList;      //Reversed graph
    vector<bool> visited;       //visited vertices in DFS
    vector<bool> rVisited;      //visited verticess in DFS of reversed graph
    vector<int> finStack;       //Stack by finished time of DFS
    vector<vector<int>> SCC;     //Strongly Connected Components
};

class AdjArray{
public:
    void readInput(string inputPath){
        ifstream ifs(inputPath);
        string line;        
        bool FIRST = true;

        while(getline(ifs, line)){
            istringstream iss(line);
            if(FIRST){
                FIRST = false;
                iss >> numVtx;
                continue;
            }         
            int numDest;
            iss >> numDest;
            Vtx.push_back(numDest);
            if (numDest != 0){
                int dataIn;
                while(iss >> dataIn){
                    Edge.push_back(dataIn);
                }
            }            
        }
        ifs.close();
        for(int i = 1; i < Vtx.size(); i++){
            Vtx[i] += Vtx[i - 1];
        }
    }

    void makeSCC(string outputPath){
        auto start = chrono::system_clock::now();
        makeReverse();
        DFS();
        reverseDFS();
        for(int i = 0; i < SCC.size(); i++){
            sort(SCC[i].begin(), SCC[i].end());
        }
        auto end = chrono::system_clock::now();
        auto durationTime = chrono::duration_cast<chrono::milliseconds>(end - start);
        
        for(int i = 0; i < SCC.size(); i++){
            sort(SCC[i].begin(), SCC[i].end());
        }

        vector<string> outString;
        for(int i = 0; i < SCC.size(); i++){
            string thisLine;
            for(int out = 0; out < SCC[i].size(); out++){
                if(out == 0) thisLine = to_string(SCC[i][out]);
                else thisLine = thisLine + " " + to_string(SCC[i][out]);
            }
            outString.push_back(thisLine);
        }
        sort(outString.begin(), outString.end());


        ofstream Output;
        Output.open(outputPath);
        for(int i = 0; i < outString.size(); i++){
            Output << outString[i] << endl;
        }
        Output << durationTime.count() << "ms";
        Output.close();
    }

    void makeReverse(){
        vector<int> tempRVtx(Vtx.size(), 0);
        vector<int> tempREdge(Edge.size(), 0);
        rVtx = tempRVtx;
        rEdge = tempREdge;
        
        for(int v = 0; v < Vtx.size(); v++){
            int startIdx;
            if(v == 0) startIdx = 0;
            else startIdx = Vtx[v - 1];
            int endIdx = Vtx[v];

            for(int e = startIdx; e < endIdx; e++){
                int out = Edge[e];
                rEdge.insert(rEdge.begin() + rVtx[out - 1], v + 1);

                for(int i = out - 1; i < rVtx.size(); i++){
                    rVtx[i]++;
                }
            }
        }
    }

    void DFS(){
        vector<bool> initVisited(numVtx, false);
        visited = initVisited;

        for(int i = 0; i < numVtx; i++){
            if(!visited[i]) aDFS(i);
        }
    }

    void aDFS(int vtx){
        visited[vtx] = true;

        int startIdx;
        if(vtx == 0) startIdx = 0;
        else startIdx = Vtx[vtx - 1];
        int endIdx = Vtx[vtx];

        for(int e = startIdx; e < endIdx; e++){
            int adj = Edge[e] - 1;
            if(!visited[adj]) aDFS(adj);
        }
        finStack.push_back(vtx);
    }

    void reverseDFS(){
        vector<bool> initRVisited(numVtx, false);
        rVisited = initRVisited;

        for(int i = numVtx - 1; i >= 0; i--){
            if(!rVisited[finStack[i]]) {
                SCC.push_back(vector<int>());
                aReverseDFS(finStack[i]);
            }
        }
    }

    void aReverseDFS(int vtx){
        rVisited[vtx] = true;
        SCC.back().push_back(vtx + 1);

        int startIdx;
        if(vtx == 0) startIdx = 0;
        else startIdx = rVtx[vtx - 1];
        int endIdx = rVtx[vtx];

        for(int e = startIdx; e < endIdx; e++){
            int adj = rEdge[e] - 1;
            if(!rVisited[adj]) aReverseDFS(adj);
        }
    }
    
    void testPrint(){
        cout << "[Array] SCC size : " << SCC.size() << endl;
        for(auto itr = SCC.begin(); itr != SCC.end(); itr++){
            for(auto itr2 = (*itr).begin(); itr2 != (*itr).end(); itr2++){
                cout << (*itr2) << " ";
            }
            cout << "\n" << endl;
        }
    }
private:
    int numVtx;
    vector<int> Vtx;        //array for storing end point of Edge for each vertex
    vector<int> Edge;       //array for stroring outgoing edge
    vector<int> rVtx;       //for reversed graph
    vector<int> rEdge;      //for reversed graph
    vector<bool> visited;       //visited vertices in DFS
    vector<bool> rVisited;      //visited vertices in DFS of reversed graph
    vector<int> finStack;       //Stack by finished time of DFS
    vector<vector<int>> SCC;    //Strongly Connected Components
};


int main(int argc, char **argv){
    string inputPath = argv[1];
    string outputPath = argv[2];
    string TYPE = argv[3];

    if(TYPE == "adj_mat"){
        AdjMat MAT;
        MAT.readInput(inputPath);
        MAT.makeSCC(outputPath);
    }
    else if(TYPE == "adj_list"){
        AdjList LIST;
        LIST.readInput(inputPath);
        LIST.makeSCC(outputPath);
    }
    else if(TYPE == "adj_arr"){
        AdjArray ARR;
        ARR.readInput(inputPath);
        ARR.makeSCC(outputPath);
    }
    else cout << "Wrong Argument" << endl;

    return 0;
}