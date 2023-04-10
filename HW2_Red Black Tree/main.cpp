#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>

using namespace std;

class Node{
public:
    Node* parent = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;
    int key = 0;
    int size = 1;
    bool RED = true;    // RED = false if black node
};

class osTree{
public:
    Node* root = nullptr;
    vector<int> a;
    vector<array<int ,2>> inCommand;    // {command, key}   command : [I: 0 / D: 1 / S: 2 / R: 3]
    vector<int> outputSequence;

    void printIn(){
        for(auto itr = inCommand.begin(); itr!= inCommand.end(); itr++){
            cout<<(*itr)[0] << " " << (*itr)[1] << endl;
        }
    }

    void printInOrder(Node* thisNode){
        if(thisNode == nullptr) return;
        printInOrder(thisNode->left);
        cout << "key: " << thisNode->key << " RED:" << thisNode->RED << " size: " << thisNode->size << endl;
        printInOrder(thisNode->right);
    }

    void readInput(string inputPath){
        ifstream ifs(inputPath);
        string commandIn;
        int dataIn;
        while(ifs >> commandIn){
            ifs >> dataIn;
            
            int tempCom;
            if(commandIn == "I") tempCom = 0;
            else if(commandIn == "D") tempCom = 1;
            else if(commandIn == "S") tempCom = 2;
            else if(commandIn == "R") tempCom = 3;

            array<int, 2> tempIn = {tempCom, dataIn};
            inCommand.push_back(tempIn);
        }
        ifs.close();
    }
    
    void makeOutput(string outputPath){
        ofstream Output;
        Output.open(outputPath);
        for(auto itr = inCommand.begin(); itr != inCommand.end(); itr++){
            int tempCom = (*itr)[0];
            int tempKey = (*itr)[1];
            if(tempCom == 0) Output << "I " << tempKey << endl;
            else if(tempCom == 1) Output << "D " << tempKey << endl;
            else if(tempCom == 2) Output << "S " << tempKey << endl;
            else if(tempCom == 3) Output << "R " << tempKey << endl;
        }
        for(auto itr = outputSequence.begin(); itr != outputSequence.end(); itr++){
            Output << (*itr) << endl;
        }
        Output.close();
    }

    void buildTree(){
        for(auto itr = inCommand.begin(); itr != inCommand.end(); itr++){
            if((*itr)[0] == 0) {
                outputSequence.push_back(Insert((*itr)[1]));
                
            }
            else if((*itr)[0] == 1) {
                outputSequence.push_back(Delete((*itr)[1]));
            }
            else if((*itr)[0] == 2) outputSequence.push_back(Select((*itr)[1]));
            else if((*itr)[0] == 3) outputSequence.push_back(Rank((*itr)[1]));
        }
    }

    int Insert(int key){
        if(treeSearch(root, key) != nullptr) return 0;
        root = binaryInsert(root, key);
        insertFixUp(treeSearch(root, key));
        calcSize(root);
        
        return key;
    }
    
    int Delete(int key){
        if(treeSearch(root,key) == nullptr) return 0;

        if(treeSearch(root,key) == root && root->size == 1){
            root = nullptr;
            return key;
        }

        Node* delNode = treeSearch(root, key);
        rbDelete(delNode);
        // rebuildTree();
        calcSize(root);

        return key;
    }

    int Select(int idx){
        if(root == nullptr) return 0;
        if(root->size < idx) return 0;
        else return osSelect(root, idx);
    }

    int Rank(int key){
        Node* keyNode = treeSearch(root, key);
        if(keyNode == nullptr) return 0;
        else return osRank(keyNode);
    }


private:
    vector<int> tempKeys;

    int osRank(Node* thisNode){
        int nodeRank = 0;
        if(thisNode->left == nullptr) nodeRank = 1;
        else nodeRank = thisNode->left->size + 1;

        Node* tempNode = thisNode;
        while(tempNode != root){
            if(tempNode == tempNode->parent->right) {
                if(tempNode->parent->left == nullptr) nodeRank += 1;
                else nodeRank += tempNode->parent->left->size + 1;
            }
            tempNode = tempNode->parent;
        }
        return nodeRank;
    }

    int osSelect(Node* thisNode, int idx){
        int nodeRank = 0;
        if(thisNode->left == nullptr) nodeRank = 1;
        else nodeRank = thisNode->left->size + 1;

        if(idx == nodeRank) return thisNode->key;
        else if(idx < nodeRank) return osSelect(thisNode->left, idx);
        else return osSelect(thisNode->right, idx - nodeRank);
    }

    void rbDelete(Node* zNode){
        Node* yNode = zNode;
        bool yOriginalRED = yNode->RED;
        Node* xNode;
        if(zNode->left == nullptr){
            xNode = zNode->right;
            transPlant(zNode, zNode->right);
        }
        else if(zNode->right == nullptr){
            xNode = zNode->left;
            transPlant(zNode, zNode->left);
        }
        else{
            yNode = minNode(zNode->right);
            xNode = yNode->right;
            if(yNode->parent != zNode){
                transPlant(yNode, yNode->right);
                yNode->right = zNode->right;
                yNode->right->parent = yNode;
            }
            transPlant(zNode, yNode);
            yNode->left = zNode->left;
            yNode->left->parent = yNode;
            yNode->RED = zNode->RED;
        }
        if(!yOriginalRED && xNode != nullptr) deleteFixUp(xNode);
    }

    void rebuildTree(){
        makeTempKeys(root);
        root = nullptr;
        for(auto itr = tempKeys.begin(); itr != tempKeys.end(); itr++){
            if(treeSearch(root, (*itr)) == nullptr){
                root = binaryInsert(root, (*itr));
                insertFixUp(treeSearch(root, (*itr)));
                calcSize(root);
            }   
        }
    }

    void makeTempKeys(Node* thisNode){
        if(thisNode == nullptr) return;
        makeTempKeys(thisNode->left);
        tempKeys.push_back(thisNode->key);
        makeTempKeys(thisNode->right);
    }

    int calcSize(Node* thisNode){
        if(thisNode->left == nullptr && thisNode->right == nullptr){
            thisNode->size = 1;
            return thisNode->size;
        }
        else if(thisNode->left == nullptr){
            thisNode->size = calcSize(thisNode->right) + 1;
            return thisNode->size;
        }
        else if(thisNode->right == nullptr){
            thisNode->size = calcSize(thisNode->left) + 1;
            return thisNode->size;
        }
        else{
            thisNode->size = calcSize(thisNode->left) + calcSize(thisNode->right) + 1;
            return thisNode->size;
        }
    }

    void deleteFixUp(Node* thisNode){
        while(thisNode != root && !thisNode->RED){
            if(thisNode == thisNode->parent->left){
                Node* wNode = thisNode->parent->right;
                if(wNode->RED){
                    wNode->RED = false;
                    thisNode->parent->RED = true;
                    leftRotate(thisNode->parent);
                    wNode = thisNode->parent->right;
                }
                if(!wNode->left->RED && !wNode->right->RED){
                    wNode->RED = true;
                    thisNode = thisNode->parent;
                    continue;
                }
                else if(!wNode->right->RED){
                    wNode->left->RED = false;
                    wNode->RED = true;
                    rightRotate(wNode);
                    wNode = thisNode->right;
                }
                wNode->RED = thisNode->parent->RED;
                thisNode->parent->RED = false;
                wNode->right->RED = false;
                leftRotate(thisNode->parent);
                thisNode = root;
                continue;
            }
            else{
                Node* wNode = thisNode->parent->left;
                if(wNode->RED){
                    wNode->RED = false;
                    thisNode->parent->RED = true;
                    rightRotate(thisNode->parent);
                    wNode = thisNode->parent->left;
                }
                if(!wNode->right->RED && !wNode->left->RED){
                    wNode->RED = true;
                    thisNode = thisNode->parent;
                    continue;
                }
                else if(!wNode->left->RED){
                    wNode->right->RED = false;
                    wNode->RED = true;
                    leftRotate(wNode);
                    wNode = thisNode->left;
                }
                wNode->RED = thisNode->parent->RED;
                thisNode->parent->RED = false;
                wNode->left->RED = false;
                rightRotate(thisNode->parent);
                thisNode = root;
                continue;
            }
        }
        thisNode->RED = false;
    }

    Node* minNode(Node* thisNode){
        if(thisNode->left == nullptr) return thisNode;
        return minNode(thisNode->left);
    }

    void transPlant(Node* uNode, Node* vNode){
        if(uNode->parent == nullptr) root = vNode;
        else if(uNode == uNode->parent->left) uNode->parent->left = vNode;
        else uNode->parent->right = vNode;
        if(vNode != nullptr) {
            vNode->size = uNode->size;
            vNode->parent = uNode->parent;
        }
    }

    Node* treeSearch(Node* thisNode , int key){
        if(thisNode == nullptr) return nullptr;
        if(thisNode->key == key) return thisNode;
        if(key < thisNode->key) return treeSearch(thisNode->left, key);
        else return treeSearch(thisNode->right, key);
    }

    void insertFixUp(Node* thisNode){
        if(thisNode == root){
            root->RED = false;
            return;
        }
        while(thisNode->parent->RED){
            if(thisNode->parent == thisNode->parent->parent->left){
                Node* sNode = thisNode->parent->parent->right;
                if(sNode == nullptr){
                    if(thisNode == thisNode->parent->right){
                        leftRotate(thisNode->parent);
                        thisNode = thisNode->left;
                    }
                    thisNode->parent->RED = false;
                    thisNode->parent->parent->RED = true;
                    rightRotate(thisNode->parent->parent);
                    continue;
                }
                if(sNode->RED){
                    thisNode->parent->RED = false;
                    sNode->RED = false;
                    thisNode->parent->parent->RED = true;
                    thisNode = thisNode->parent->parent;
                    if(thisNode == root) break;
                    continue;
                }
                else if(thisNode == thisNode->parent->right){
                    thisNode = thisNode->parent;
                    leftRotate(thisNode);
                }
                thisNode->parent->RED = false;
                thisNode->parent->parent->RED = true;
                rightRotate(thisNode->parent->parent);
            }
            else{
                Node* sNode = thisNode->parent->parent->left;
                if(sNode == nullptr){
                    if(thisNode == thisNode->parent->left){
                        rightRotate(thisNode->parent);
                        thisNode = thisNode->right;
                    }
                    thisNode->parent->RED = false;
                    thisNode->parent->parent->RED = true;
                    leftRotate(thisNode->parent->parent);
                    continue;
                }
                if(sNode->RED){
                    thisNode->parent->RED = false;
                    sNode->RED = false;
                    thisNode->parent->parent->RED = true;
                    thisNode = thisNode->parent->parent;
                    if(thisNode == root) break;
                    continue;
                }
                else if(thisNode == thisNode->parent->left){
                    thisNode = thisNode->parent;
                    rightRotate(thisNode);
                    
                }
                thisNode->parent->RED = false;
                thisNode->parent->parent->RED = true;
                leftRotate(thisNode->parent->parent);
            }
        }
        root->RED = false;
    }

    void leftRotate(Node* thisNode){
        Node* yNode = thisNode->right;
        thisNode->right = yNode->left;
        if(yNode->left != nullptr) yNode->left->parent = thisNode;
        yNode->parent = thisNode->parent;

        if(thisNode->parent == nullptr) root = yNode;
        else if(thisNode == thisNode->parent->left) thisNode->parent->left = yNode;
        else thisNode->parent->right = yNode;

        yNode->left = thisNode;
        thisNode->parent = yNode;
    }

    void rightRotate(Node* thisNode){
        Node* yNode = thisNode->left;
        thisNode->left = yNode->right;
        if(yNode->right != nullptr) yNode->right->parent = thisNode;
        yNode->parent = thisNode->parent;

        if(thisNode->parent == nullptr) root = yNode;
        else if(thisNode == thisNode->parent->right) thisNode->parent->right = yNode;
        else thisNode->parent->left = yNode;

        yNode->right = thisNode;
        thisNode->parent = yNode;
    }

    Node* binaryInsert(Node* thisNode, int key){
        if(thisNode == nullptr){
            Node* newNode = new Node();
            newNode->key = key;
            return newNode;
        }
        if(key < thisNode->key){
            thisNode->size++;
            thisNode->left = binaryInsert(thisNode->left, key);
            thisNode->left->parent = thisNode;
            return thisNode;
        }
        else{
            thisNode->size++;
            thisNode->right = binaryInsert(thisNode->right, key);
            thisNode->right->parent = thisNode;
            return thisNode;
        }
    }

};

class Checker{
public:
    array<bool, 10000> checkArray = {false};
    vector<array<int ,2>> inCommand;    // {command, key}   command : [I: 0 / D: 1 / S: 2 / R: 3]
    vector<int> outputSequence;
    vector<int> checkSequence;

    void readInput(string inputPath){
        ifstream ifs(inputPath);
        string commandIn;
        int dataIn;
        while(ifs >> commandIn){
            ifs >> dataIn;
            
            int tempCom;
            if(commandIn == "I") tempCom = 0;
            else if(commandIn == "D") tempCom = 1;
            else if(commandIn == "S") tempCom = 2;
            else if(commandIn == "R") tempCom = 3;

            array<int, 2> tempIn = {tempCom, dataIn};
            inCommand.push_back(tempIn);
        }
        ifs.close();
    }

    void makeOutput(string outputPath){
        ofstream Output;
        Output.open(outputPath);
        if(Correct()) {
            Output << "Correct" << endl;
        }
        else Output << "Wrong" << endl;
        Output.close();
    }

    bool Correct(){
        processCheckSeq();
        if(outputSequence.size() != checkSequence.size()) return false;
        for(int i = 0; i < outputSequence.size(); i++){
            if(outputSequence[i] != checkSequence[i]) return false;
        }
        return true;
    }
private:
        void processCheckSeq(){
        for(auto itr = inCommand.begin(); itr != inCommand.end(); itr++){
            int tempCom = (*itr)[0];
            int tempKey = (*itr)[1];
            if(tempCom == 0) {
                if(!checkArray[tempKey]) {
                    checkArray[tempKey] = true;
                    checkSequence.push_back(tempKey);
                }
                else checkSequence.push_back(0);
            }
            else if(tempCom == 1) {
                if(checkArray[tempKey]) {
                    checkArray[tempKey] = false;
                    checkSequence.push_back(tempKey);
                }
                else checkSequence.push_back(0);
            }
            else if(tempCom == 2) {
                int count = 0;
                for(int i = 1; i < checkArray.size(); i++){
                    if(checkArray[i]){
                        count++;
                        if(count == tempKey){
                            checkSequence.push_back(i);
                            break;
                        }
                    }
                }
                if(count != tempKey) checkSequence.push_back(0);
            }
            else if(tempCom == 3) {
                int count = 0;
                if(!checkArray[tempKey]) checkSequence.push_back(0);
                else{
                    for(int i = 1; i <= tempKey; i++){
                        if(checkArray[i]) count++;
                    }
                    checkSequence.push_back(count);
                }
            }
        }
    }
    
};

int main(){
    osTree tree;
    Checker checker;
    tree.readInput("input.txt");

    tree.buildTree();

    tree.makeOutput("output.txt");
    
    checker.inCommand = tree.inCommand;
    checker.outputSequence = tree.outputSequence;
    checker.makeOutput("checker.txt");

    return 0;
}