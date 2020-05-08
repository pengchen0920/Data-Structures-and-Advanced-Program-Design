#include <iostream>
#include <cassert>
#include <stdexcept>
#include <vector>
using namespace std;


class Node
{
private:
    int item;
    Node* next;
public:
    Node();
    Node(const int& anItem);
    Node(const int& anItem, Node* nextNodePtr);
    void setItem(const int& anItem);
    void setNext(Node* nextNodePtr);
    int getItem() const;
    Node* getNext() const;
};


Node::Node() : next(nullptr)
{
}


Node::Node(const int& anItem) : item(anItem), next(nullptr)
{
}


Node::Node(const int& anItem, Node* nextNodePtr) :
item(anItem), next(nextNodePtr)
{
}


void Node::setItem(const int& anItem)
{
    item = anItem;
}


void Node::setNext(Node* nextNodePtr)
{
    next = nextNodePtr;
}


int Node::getItem() const
{
    return item;
}


Node* Node::getNext() const
{
    return next;
}


class ListInterface
{
public:
    virtual bool isEmpty() const = 0;
    
    virtual int getLength() const = 0;
    
    virtual bool insert(int newPosition, const int& newEntry) = 0;
    
    /** Removes the entry at a given position from this list.
     @pre  None.
     @post  If 1 <= position <= getLength() and the removal is successful,
     the entry at the given position in the list is removed, other
     items are renumbered accordingly, and the returned value is true.
     @param position  The list position of the entry to remove.
     @return  True if removal is successful, or false if not. */
    virtual bool remove_byIndex(int position) = 0;
    
    /** Removes all entries from this list.
     @post  List contains no entries and the count of items is 0. */
    virtual void clear() = 0;
    
    /** Gets the entry at the given position in this list.
     @pre  1 <= position <= getLength().
     @post  The desired entry has been returned.
     @param position  The list position of the desired entry.
     @return  The entry at the given position. */
    virtual int getEntry(int position) const = 0;
    
    /** Replaces the entry at the given position in this list.
     @pre  1 <= position <= getLength().
     @post  The entry at the given position is newEntry.
     @param position  The list position of the entry to replace.
     @param newEntry  The replacement entry. */
//    virtual void setEntry(int position, const int& newEntry) = 0;
};







//把list加入name的variable，才方便去辨認
//做一個getList（byName）的function，回傳一個list

class list : public ListInterface
{
private:
    Node* headPtr; // Pointer to first node in the chain;
    // (contains the first entry in the list)
    int itemCount;           // Current count of list items
    
    // Locates a specified node in this linked list.
    // @pre  position is the "index" number of the desired node;
    //       position >= 0 and position <= itemCount-1.
    // @post  The node is found and a pointer to it is returned.
    // @param position(index)  The number of the node to locate.
    // @return  A pointer to the node at the given position(index).
    Node* getNodeAt(int position) const;
    
public:
    string name;
    list();
    list(string name);
    list(const list* anotherList);
    virtual ~list();
    
    /** Sees whether this list is empty.
     @return True if the list is empty; otherwise returns false. */
    bool isEmpty() const;
    
    /** Gets the current number of entries in this list.
     @return The integer number of entries currently in the list. */
    int getLength() const;
   
    /** Inserts an entry into this list at a given position(index).
     @pre  None.
     @post  If 0 <= position < getLength()  and the insertion is
     successful, newEntry is at the given position in the list,
     other entries are renumbered accordingly, and the returned
     value is true. And if position >= getLength(), it acts like append.
     @param newPosition  The list position(index) at which to insert newEntry.
     @param newEntry  The entry to insert into the list.
     @return  True if insertion is successful, or false if not. */
    bool insert(int newPosition, const int& newEntry);
    void append(const int& newEntry);
    
    void clear();
    
    /** @throw PrecondViolatedExcep if position < 1 or
     position > getLength(). */
    int getEntry(int position) const throw(logic_error);
    
    /** @return the index, which anEntry appears for the first time. If the entry didn't appear at all, return -1.*/
    int index(const int& anEntry);
    /** @return how many times anEntry appears in the list. If the entry didn't appear at all, return 0.*/
    int count(const int& anEntry);
    void extend(const list* anotherList);
    void reverse();
    void sort();
    void print();
    void swap(Node* a,Node* b);
    bool remove(const int& newEntry);
    bool remove_byIndex(int position);
    
//    void setEntry(int position, const int& newEntry) throw(logic_error);
};


list::list() : headPtr(nullptr), itemCount(0)
{
}  // end default constructor

list::list(string name):headPtr(nullptr), itemCount(0){
    this->name = name;
}

list::list(const list* anotherList){
    // Point to nodes in original chain
    this->itemCount = anotherList->itemCount;
    Node* origChainPtr = anotherList->headPtr;
    if(origChainPtr == nullptr)
        this->headPtr = nullptr; // Original bag is empty
    else
    {
        // Copy first node
        headPtr = new Node();
        headPtr->setItem(origChainPtr->getItem());
        Node* newChainPtr = headPtr;
        // Copy remaining nodes
        while(origChainPtr->getNext() != nullptr) // typo in the textbook
        {
            origChainPtr = origChainPtr->getNext();
            int nextItem = origChainPtr->getItem();
            Node* newNodePtr = new Node(nextItem);
            newChainPtr->setNext(newNodePtr);
            newChainPtr = newChainPtr->getNext();
        }
        newChainPtr->setNext(nullptr); // Mark the bottom of stack
    }
}


list::~list()
{
    clear();
}  // end destructor


bool list::isEmpty() const
{
    return itemCount == 0;
}  // end isEmpty


int list::getLength() const
{
    return itemCount;
}  // end getLength



bool list::insert(int newPosition, const int& newEntry)
{
    bool ableToInsert = (newPosition >= 0);
    if (ableToInsert)
    {
        // Create a new node containing the new entry
        Node* newNodePtr = new Node(newEntry);
        
        // Attach new node to chain
        if (newPosition == 0 || this->getLength()==0)
        {
            // Insert new node at beginning of chain
            newNodePtr->setNext(headPtr);
            headPtr = newNodePtr;
        }
        else
        {
            if(newPosition > this->getLength())
                newPosition = this->getLength();
            // Find node that will be before new node
            // Assume getNodeAt() returns the index of that node
            Node* prevPtr = getNodeAt(newPosition - 1);
            
            // Insert new node after node to which prevPtr points
            newNodePtr->setNext(prevPtr->getNext());
            prevPtr->setNext(newNodePtr);
        }  // end if
        
        itemCount++;  // Increase count of entries
    }  // end if
    
    return ableToInsert;
}  // end insert

void list::append(const int &newEntry){
    this->insert(this->getLength(), newEntry);
}


bool list::remove(const int &newEntry){
    bool removeable = (this->index(newEntry)==-1? false:true);
    if(removeable){
        this->remove_byIndex(this->index(newEntry));
    }
    return removeable;
}

bool list::remove_byIndex(int position)
{
    bool ableToRemove = (position >= 0) && (position < itemCount);
    if (ableToRemove)
    {
        Node* curPtr = nullptr;
        if (position == 0)
        {
            // Remove the first node in the chain
            curPtr = headPtr; // Save pointer to node
            headPtr = headPtr->getNext();
        }
        else
        {
            // Find node that is before the one to delete
            // getNodeAt() returns the index of that node
            Node* prevPtr = getNodeAt(position - 1);
            
            // Point to node to delete
            curPtr = prevPtr->getNext();
            
            // Disconnect indicated node from chain by connecting the
            // prior node with the one after
            prevPtr->setNext(curPtr->getNext());
        }  // end if
        
        // Return node to system
        curPtr->setNext(nullptr);
        delete curPtr;
        curPtr = nullptr;
        
        itemCount--;  // Decrease count of entries
    }  // end if
    
    return ableToRemove;
}  // end remove


void list::clear()
{
    while (!isEmpty())
        remove_byIndex(0);
}  // end clear

int list::index(const int &anEntry){
    
    for(int i = 0; i<this->getLength(); i++){
        if((this->getNodeAt(i))->getItem()==anEntry){
            return i;
        }
    }
    return (-1);
}

int list::count(const int &anEntry){
    int count = 0;
    for(int i = 0; i<this->getLength(); i++){
        if((this->getNodeAt(i))->getItem()==anEntry){
            count++;
        }
    }
    return count;
}

void list::extend(const list* anotherList){
    
    list another(anotherList);
    
    Node* last = nullptr;
    if(this->getLength()>=1){
        last = this->getNodeAt(this->getLength()-1);
        
        Node* origChainPtr = another.headPtr;
        if(origChainPtr == nullptr)
            last->setNext(nullptr); // The list you want to extend is empty
        else// The list you want to extend is "not" empty
        {
            // extend the first node
            Node* extend = new Node();
            extend->setItem(origChainPtr->getItem());
            //!!!!!!!!!!!!!!!!!!!
            last->setNext(extend);
            last = last->getNext();
            // extend the remaining nodes
            while(origChainPtr->getNext() != nullptr)
            {
                origChainPtr = origChainPtr->getNext();
                int nextItem = origChainPtr->getItem();
                Node* newNodePtr = new Node(nextItem);
                last->setNext(newNodePtr);
                last = last->getNext();
            }
            last->setNext(nullptr); // Mark the bottom of stack
        }
    }
    else if(this->getLength() == 0){
        Node* origChainPtr = another.headPtr;
        if(origChainPtr == nullptr){
            //do nothing
        }
//            this->headPtr->setNext(nullptr); // The list you want to extend is empty
            
        else// The list you want to extend is "not" empty
        {
            this->headPtr = new Node();
            this->headPtr->setItem(origChainPtr->getItem());
            Node* newChainPtr = headPtr;
            
            // Copy remaining nodes
            while(origChainPtr->getNext() != nullptr) // typo in the textbook
            {
                origChainPtr = origChainPtr->getNext();
                int nextItem = origChainPtr->getItem();
                Node* newNodePtr = new Node(nextItem);
                newChainPtr->setNext(newNodePtr);
                newChainPtr = newChainPtr->getNext();
            }
            newChainPtr->setNext(nullptr); // Mark the bottom of stack
        }
    }
    //把兩個list的數目加起來
    this->itemCount += another.itemCount;
    
}

//2019/04/27 reverse 先做一個要reverse的list的copy，把copy中的每個元素依照index順序，依序插入原本list的第0個位子(相當於插入原本list的0,2,4.....個)，在remove最後面(pos = length-1)的元素copy的length次。

void list::reverse(){
    int count = this->getLength();
    int pos = 0;
    while(this->getLength() < count*2){
        int insertEntry = this->getEntry(pos);
        this->insert(0, insertEntry);
        pos += 2;
    }
    for(int i = 0 ; i<count; i++){
        this->remove_byIndex(this->getLength()-1);
    }
}

/* Bubble sort the given linked list */
void list::sort()
{
    int swapped;
    Node* ptr1;
    Node* lptr = nullptr;
    
    /* Checking for empty list */
    if (this->headPtr == nullptr)
        return;
    
    do
    {
        swapped = 0;
        ptr1 = this->headPtr;
        
        while (ptr1->getNext() != lptr)
        {
            if (ptr1->getItem() > (ptr1->getNext())->getItem())
            {
                swap(ptr1, ptr1->getNext());
                swapped = 1;
            }
            ptr1 = ptr1->getNext();
        }
        lptr = ptr1;
    }
    while (swapped);
}


void list::swap(Node* a,Node* b)
{
    int temp = a->getItem();
    a->setItem(b->getItem());
    b->setItem(temp);
}

int list::getEntry(int position) const throw(logic_error)
{
    // Enforce precondition
    bool ableToGet = (position >= 0) && (position < itemCount);
    if (ableToGet)
    {
        Node* nodePtr = getNodeAt(position);
        return nodePtr->getItem();
    }
    else
    {
        string message = "getEntry() called with an empty list or ";
        message  = message + "invalid position.";
        throw(logic_error(message));
    }  // end if
}  // end getEntry


Node* list::getNodeAt(int position) const
{
    // Debugging check of precondition
    assert( (position >= 0) && (position < itemCount) );
    
    // Count from the beginning of the chain
    Node* curPtr = headPtr;
    for (int skip = 0; skip < position; skip++)
        curPtr = curPtr->getNext();
    
    return curPtr;
}
void list::print(){
    Node* temp = this->headPtr;
    cout << "[";
    while(temp != nullptr){
        if(temp->getNext() == nullptr)
            cout << temp->getItem();
        else
            cout << temp->getItem() << ",";
        temp = temp->getNext();
    }
    cout << "]" << endl;
}

//===========================================================================

class listArray{
public:
    listArray();
    vector<list*> List;
    list* getList(string name);
};
listArray::listArray(){
}
list* listArray::getList(string queryName){
    int found = 0;
    for(int i = 0; i<this->List.size() ;i++){
        if(this->List[i]->name == queryName){
            found = i;
            break;
        }
    }
    return this->List[found];
}

string findName_initial(string command){
    size_t name_pos = 0;
    name_pos = command.find(" ");
    string list_name = command.substr(0,name_pos);
    return list_name;
}

string findName(string command){
    size_t name_pos = 0;
    name_pos = command.find(".");
    string list_name = command.substr(0,name_pos);
    return list_name;
}

int findInt(string command){
    
    size_t num_pos1 = 0, num_pos2 = 0;
    num_pos1 = command.find("(");
    num_pos2 = command.find(")");
    int num = stoi(command.substr(num_pos1+1,num_pos2-num_pos1-1));
    
    return num;
}


int findInt_1_comma(string command){
    size_t num_pos1 = 0, num_pos2 = 0;
    num_pos1 = command.find("(");
    num_pos2 = command.find(",");
    int num1 = stoi(command.substr(num_pos1+1,num_pos2-num_pos1-1));
    return num1;
}

int findInt_2_comma(string command){
    size_t num_pos1 = 0, num_pos2 = 0;
    num_pos1 = command.find(",");
    num_pos2 = command.find(")");
    int num2 = stoi(command.substr(num_pos1+1,num_pos2-num_pos1-1));
    return num2;
}

string findList(string command){
    
    size_t num_pos1 = 0, num_pos2 = 0;
    num_pos1 = command.find("(");
    num_pos2 = command.find(")");
    string extend_list = command.substr(num_pos1+1,num_pos2-num_pos1-1);
    
    return extend_list;
}


int main() {
    
    listArray listArray;
    
    string command = "";
    while(getline(cin,command)){
        //================第一到第十的測資======================
        if(command.find("list()") != string::npos){
            string list_name = findName_initial(command);
            list* newList = new list(list_name);
            listArray.List.push_back(newList);
        }
        else if(command.find("append") != string::npos){
            string list_name = findName(command);
            int num = findInt(command);
            listArray.getList(list_name)->append(num);
        }
        else if (command.find("insert") != string::npos){
            string list_name = findName(command);
            int num1 = findInt_1_comma(command);
            int num2 = findInt_2_comma(command);
            listArray.getList(list_name)->insert(num1, num2);
        }
        else if (command.find("(") == string::npos){
            listArray.getList(command)->print();
        }
        //================第十到第十五的測資======================
        else if(command.find("remove") != string::npos){
            string list_name = findName(command);
            int num = findInt(command);
            listArray.getList(list_name)->remove(num);
        }
        //================第十六到第二十的測資======================
        else if(command.find("index") != string::npos){
            string list_name = findName(command);
            int num = findInt(command);
            cout << listArray.getList(list_name)->index(num) << endl;
        }
        else if(command.find("count") != string::npos){
            string list_name = findName(command);
            int num = findInt(command);
            cout << listArray.getList(list_name)->count(num) << endl;
        }
        //================第二十一到第二十五的測資======================
        else if(command.find("extend") != string::npos){
            string list_name = findName(command);
            string extend_list = findList(command);
            listArray.getList(list_name)->extend(listArray.getList(extend_list));
        }
        //================第二十六到第三十的測資========================
        else if(command.find("reverse") != string::npos){
            string list_name = findName(command);
            listArray.getList(list_name)->reverse();
        }
        else if(command.find("sort") != string::npos){
            string list_name = findName(command);
            listArray.getList(list_name)->sort();
        }
        
    }
    return 0;
}
//2019/04/25 append = insert in the postion(length+1)
//2019/04/27 insert 如果pos大於等於length，則直接將element放在list的最後面
//2019/04/27 extend 先做一個another list的copy constructor，然後讓alist最後一個node的next指向copy的anotherList的headPtr


