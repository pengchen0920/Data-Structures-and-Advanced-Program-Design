#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <stack>
#include <algorithm>
#include <list>
using namespace std;

class Node{
public:
    int index;
    string name;
    Node(int index, string name);
};
Node::Node(int i, string n):index(i),name(n){
}

class HashedEntry{
private:
    int index;
    string searchKey;//name of the class
    HashedEntry* nextPtr;
public:
    HashedEntry();
    HashedEntry(string newSearchKey, int newindex);
    int getindex() const;
    string getKey() const;
    void setNext(HashedEntry* nextEntryPtr);
    HashedEntry* getNext() const;
    void print();
};
HashedEntry::HashedEntry(string newSearchKey, int newindex ):searchKey(newSearchKey),index(newindex),nextPtr(nullptr){
}

int HashedEntry::getindex() const{
    return index;
}

string HashedEntry::getKey() const{
    return searchKey;
}

HashedEntry* HashedEntry::getNext() const{
    return nextPtr;
}

void HashedEntry::setNext(HashedEntry* nextEntryPtr){
    this->nextPtr = nextEntryPtr;
}

void HashedEntry::print(){
    cout << this->index << endl;
}



class HashedDictionary{
private:
    HashedEntry** hashTable;
    int indexCount;      // cout of dictionary entries
    int hashTableSize = 701;  // Table size
public:
    HashedDictionary();
    bool add(const string searchKey,const int newindex);
    int search(const string searchKey);//return index to build path
    int getHashIndex(string searchKey);
};

int HashedDictionary::getHashIndex(string searchKey){
    int seed = 131;
    unsigned long hash = 0;
    for(int i = 0; i < searchKey.length(); i++)
    {
        hash = (hash * seed) + searchKey[i];
    }
    hash %= this->hashTableSize;
    return static_cast<int>(hash);
}
HashedDictionary::HashedDictionary(){
    this->indexCount = 0;
    this->hashTable = new HashedEntry*[hashTableSize];
    for (int i=0;i<this->hashTableSize; i++){
        hashTable[i] = nullptr;
    }
}

bool HashedDictionary::add(const string searchKey,const int newindex)
{
    HashedEntry* entryToAddPtr = new HashedEntry(searchKey,newindex);
    int indexHashIndex = getHashIndex(searchKey);
    if(hashTable[indexHashIndex] == nullptr){
        hashTable[indexHashIndex] = entryToAddPtr;
    }
    else
    {
        entryToAddPtr->setNext(hashTable[indexHashIndex]);
        hashTable[indexHashIndex] = entryToAddPtr;
    }
    return true;
}

int HashedDictionary::search(const string searchKey)
{
    
    bool indexFound = false;
    // Compute the hashed index into the array
    int indexHashIndex = getHashIndex(searchKey);
    //    cout << indexHashIndex << "!!!";
    if (hashTable[indexHashIndex] != nullptr)
    {
        // Special case - first node is the target
        if (searchKey == hashTable[indexHashIndex]->getKey())
        {
            indexFound = true;
            return hashTable[indexHashIndex]->getindex();
        }
        else // Search the rest of the chain
        {
            HashedEntry* prevPtr = hashTable[indexHashIndex];
            HashedEntry* curPtr = prevPtr->getNext();
            while ((curPtr != nullptr) && !indexFound )
            {
                // Found index in chain so remove that node
                if (searchKey == curPtr->getKey())
                {
                    indexFound = true;
                    return curPtr->getindex();
                    
                }
                else // Look at next entry in chain
                {
                    prevPtr = curPtr;
                    curPtr = curPtr->getNext();
                } // end if
            } // end while
        } // end if
    } // end if
    return -1;
}



string getFirstClass(string line){
    if(line.find(",")==string::npos){
        return line;
    }
    else{
        size_t pos = 0;
        pos = line.find(",");
        return line.substr(0,pos);
    }
}

bool isRoot(bool** matrix, int n, int index){
    bool isRoot = false;
    int predecessor = 0;
    for(int i=0; i<n; i++){
        if(matrix[i][index]==true){
            predecessor++;
            break;
        }
    }
    if(predecessor==0){
        isRoot = true;
    }
    return isRoot;
}

void cleanPath(bool** matrix, int n, int index){
    for(int i=0; i<n; i++){
        matrix[index][i] = false;
    }
}

// Graph Implementation using STL
int main()
{
    
    HashedDictionary dic;

    int n;
    cin >> n;
    bool** matrix = new bool*[n];
    for(int i=0; i<n; i++){
        matrix[i] = new bool[n];
    }
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            matrix[i][j] = false;
        }
    }

    cin.ignore();
    vector<string> all;
    for(int i=0; i<n; i++){
        string line;
        getline(cin, line);
        all.push_back(line);
    }

    vector<Node> nodes;
    for(int i=0; i<n; i++){
        dic.add(getFirstClass(all[i]), i);
        nodes.push_back(Node(i, getFirstClass(all[i])));
//        cout << getFirstClass(all[i])  << endl;
    }
    
    
    //尋找path
    //如果沒有逗點就直接跳過
    //如果有逗點，就找第一個逗號到第二個逗號中間的，或是第一個逗號到最後的
    
    for(int i=0; i<n; i++){
        if(all[i].find(",")==string::npos){
            continue;
        }
        else{
            size_t pos = 0;
            size_t next = 0;
            while(next!=string::npos){
                pos = all[i].find(",",pos+1);
                next = all[i].find(",",pos+1);
                
                if (next!=string::npos){
//                    cout << dic.search(all[i].substr(pos+1,next-pos-1)) << endl;
                    matrix[dic.search(all[i].substr(pos+1,next-pos-1))][i] = true;
                }
                else{
//                    cout << dic.search(all[i].substr(pos+1,all[i].length()-1-pos)) << endl;
                    matrix[dic.search(all[i].substr(pos+1,all[i].length()-1-pos))][i] = true;
                }
            }
            
        }
    }
    
//    for(int i=0; i<n; i++){
//        for(int j=0; j<n; j++){
//            cout << matrix[i][j] << " ";
//        }
//        cout << endl;
//    }
    
    bool* visited = new bool[n];
    for(int i = 0; i<n; i++){
        visited[i] = false;
    }
    //按照輸入順序直接印node[i]：
    //如果是root，把node[i]設為visited，把從他的路經都取消
    //放進out裡面
    //重複做到每個node都被visited過
    vector<int> out;
    bool allVisited = true;
    
    do{
        allVisited = true;
        for(int i=0; i<n; i++){
            
            bool sthVisited = false;
            if(isRoot(matrix,n, i) && visited[i]==false){
                cleanPath(matrix, n, i);
                out.push_back(i);
                visited[i] = true;
                sthVisited = true;
            }
            
            for(int i=0; i<n; i++){
                if(visited[i]==false){
                    allVisited = false;
                    break;
                }
            }
            if(sthVisited)
                break;
        }
    }
    while(allVisited == false);
    
    
    for(int i=0; i<out.size(); i++){
        if(i<out.size()-1){
            cout << nodes[out[i]].name << endl;
        }
        else{
            cout << nodes[out[i]].name;
        }
    }
    
    return 0;
}
