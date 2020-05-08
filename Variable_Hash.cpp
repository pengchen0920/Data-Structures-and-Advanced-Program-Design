#include <iostream>
#include <string>
#include <functional>
using namespace std;

class HashedEntry{
private:
    string item;
    string searchKey;
    HashedEntry* nextPtr;
public:
    HashedEntry();
    HashedEntry(string newSearchKey, string newItem);
    string getItem() const;
    string getKey() const;
    void setNext(HashedEntry* nextEntryPtr);
    HashedEntry* getNext() const;
    void print();
};
HashedEntry::HashedEntry(string newSearchKey, string newItem ):searchKey(newSearchKey),item(newItem),nextPtr(nullptr){
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
    cout << this->item << endl;
}



class HashedDictionary{
private:
    HashedEntry** hashTable;
    int itemCount;      // cout of dictionary entries
    int hashTableSize = 701;  // Table size
public:
    HashedDictionary();
    bool add(const string searchKey,const string newItem);
    bool search(const string searchKey);
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
    this->itemCount = 0;
    this->hashTable = new HashedEntry*[hashTableSize];
    for (int i=0;i<this->hashTableSize; i++){
        hashTable[i] = nullptr;
    }
}

bool HashedDictionary::add(const string searchKey,const string newItem)
{
    HashedEntry* entryToAddPtr = new HashedEntry(searchKey,newItem);
    int itemHashIndex = getHashIndex(searchKey);
    if(hashTable[itemHashIndex] == nullptr){
        hashTable[itemHashIndex] = entryToAddPtr;
    }
    else
    {
        entryToAddPtr->setNext(hashTable[itemHashIndex]);
        hashTable[itemHashIndex] = entryToAddPtr;
    }
    return true;
}

bool HashedDictionary::search(const string searchKey)
{
    bool itemFound = false;
    
    // Compute the hashed index into the array
    int itemHashIndex = getHashIndex(searchKey);

    if (hashTable[itemHashIndex] != nullptr)
    {
        // Special case - first node is the target
        if (searchKey == hashTable[itemHashIndex]->getKey())
        {
            hashTable[itemHashIndex]->print();
            itemFound = true;
        }
        else // Search the rest of the chain
        {
            HashedEntry* prevPtr = hashTable[itemHashIndex];
            HashedEntry* curPtr = prevPtr->getNext();
            while ((curPtr != nullptr) && !itemFound )
            {
                // Found item in chain so remove that node
                if (searchKey == curPtr->getKey())
                {
                    curPtr->print();
                    itemFound = true;
                }
                else // Look at next entry in chain
                {
                    prevPtr = curPtr;
                    curPtr = curPtr->getNext();
                } // end if
            } // end while
        } // end if
    } // end if
    if(!itemFound){
        cout << "not defined" << endl;
    }
    return itemFound;
}

int main() {
    string line;
    HashedDictionary dic;
    while(getline(cin,line)){
        if (line.empty())
            break;
        if (line.find(";")!=string::npos){
            
            size_t pos = 0;
            size_t conti = 0;
            while(line.find(" ",conti)!=string::npos){
                pos = line.find(" ",conti);
                conti = line.find(" ",conti)+1;
            }
            string item = line.substr(0,pos);
            string searchKey = line.substr(pos+1,line.length()-pos-2);
            dic.add(searchKey, item);
        }
        else{
            dic.search(line);
        }
        
    }
}
