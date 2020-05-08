#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

template<class ItemType>
class StackInterface
{
    public :
    /** Sees whether this stack is empty.
     @return True if the stack is empty, or false if not. */
    virtual bool isEmpty() const = 0;
    
    /** Adds a new entry to the top of this stack.
     @post If the operation was successful, newEntry is at the top of the stack.
     @param newEntry The object to be added as a new entry.
     @return True if the addition is successful or false if not. */
    virtual bool push( const ItemType& newEntry) = 0;
    
    /** Removes the top of this stack.
     @post If the operation was successful, the top of the stack has been removed.
     @return True if the removal is successful or false if not. */
    virtual bool pop() = 0;
    
    /** Returns the top of this stack.
     @pre The stack is not empty.
     @post The top of the stack has been returned, and the stack is unchanged.
     @return The top of the stack. */
    virtual ItemType peek() const = 0;
};














template<class ItemType>
class Node
{
private:
    ItemType item;
    Node<ItemType>* next;
public:
    Node();
    Node(const ItemType& anItem);
    Node(const ItemType& anItem, Node<ItemType>* nextNodePtr);
    void setItem(const ItemType& anItem);
    void setNext(Node<ItemType>* nextNodePtr);
    ItemType getItem() const;
    Node<ItemType>* getNext() const;
};

template<class ItemType>
Node<ItemType>::Node() : next(nullptr)
{
}

template<class ItemType>
Node<ItemType>::Node(const ItemType& anItem) : item(anItem), next(nullptr)
{
}

template<class ItemType>
Node<ItemType>::Node(const ItemType& anItem, Node<ItemType>* nextNodePtr) :
item(anItem), next(nextNodePtr)
{
}

template<class ItemType>
void Node<ItemType>::setItem(const ItemType& anItem)
{
    item = anItem;
}

template<class ItemType>
void Node<ItemType>::setNext(Node<ItemType>* nextNodePtr)
{
    next = nextNodePtr;
}

template<class ItemType>
ItemType Node<ItemType>::getItem() const
{
    return item;
}

template<class ItemType>
Node<ItemType>* Node<ItemType>::getNext() const
{
    return next;
}












template<typename ItemType>
class LinkedStack : public StackInterface<ItemType>
{
private:
    Node<ItemType>* topPtr; // Pointer to first node in the chain;
    // this node contains the stack's top
public:
    // Constructors and destructor:
    LinkedStack(); // Default constructor
    LinkedStack(const LinkedStack<ItemType>& aStack); // Copy constructor
    virtual ~LinkedStack(); // Destructor
    // Stack operations:
    bool isEmpty() const;
    bool push( const ItemType& newItem);
    bool pop();
    ItemType peek() const;
    void print();
};

template<typename ItemType>
LinkedStack<ItemType>::LinkedStack() : topPtr(nullptr)
{
}

template<typename ItemType>
LinkedStack<ItemType>::LinkedStack(const LinkedStack<ItemType>& aStack)
{
    // Point to nodes in original chain
    Node<ItemType>* origChainPtr = aStack.topPtr; // typo in the textbook
    if(origChainPtr == nullptr)
    this->topPtr = nullptr; // Original bag is empty
    else
    {
        // Copy first node
        topPtr = new Node<ItemType>();
        topPtr->setItem(origChainPtr->getItem());
        Node<ItemType>* newChainPtr = topPtr;
        // Copy remaining nodes
        while(origChainPtr->getNext() != nullptr) // typo in the textbook
        {
            origChainPtr = origChainPtr->getNext();
            ItemType nextItem = origChainPtr->getItem();
            Node<ItemType>* newNodePtr = new Node<ItemType>(nextItem);
            newChainPtr->setNext(newNodePtr);
            newChainPtr = newChainPtr->getNext();
        }
        newChainPtr->setNext(nullptr); // Mark the bottom of stack
    }
}

template<typename ItemType>
LinkedStack<ItemType>::~LinkedStack()
{
    // Pop until stack is empty
    while(!isEmpty())
    pop();
}

template<typename ItemType>
bool LinkedStack<ItemType>::isEmpty() const
{
    return topPtr == nullptr;
}

template<typename ItemType>
bool LinkedStack<ItemType>::push(const ItemType& newItem)
{
    Node<ItemType>* newNodePtr = new Node<ItemType>(newItem, topPtr);
    topPtr = newNodePtr;
    return true;
}

template<typename ItemType>
bool LinkedStack<ItemType>::pop()
{
    bool result = false ;
    if(!isEmpty())
    {
        // Stack is not empty; delete top
        Node<ItemType>* nodeToDeletePtr = topPtr;
        topPtr = topPtr->getNext();
        delete nodeToDeletePtr;
        result = true ;
    }
    return result;
}

template<typename ItemType>
ItemType LinkedStack<ItemType>::peek() const
{
    if(!this->isEmpty()) // check precondition
    return topPtr->getItem();
    else
    throw logic_error("...");
}

//print from the bottom to top
template<typename ItemType>
void LinkedStack<ItemType>::print()
{
    LinkedStack<ItemType> reverse;
    while(this->isEmpty()==false){
        ItemType aItem = this->peek();
        this->pop();
        reverse.push(aItem);
    }
    while(reverse.isEmpty()==false){
        cout << reverse.peek();
        reverse.pop();
        if(reverse.isEmpty()==false)
        cout << ",";
    }
}




//寫一個string函式切token，回傳運算元（1234567890）或是運算子（+-*/）
//演算法再去判斷是運算元（轉成int）或是運算子（依然是string)



//erase掉已經回傳的部分
string input_token(string& input){
    string numbers = "0123456789";
    size_t number_pos = 0;
    string tokenString = "";
    //如果第一個是運算子（+-*/）直接回傳
    if( input[0]=='+' || input[0]=='-' || input[0]=='*' || input[0]=='/'){
        tokenString = input[0];
        input.erase(0,1);
        return tokenString;
    }
    else{
        for(int i = 0; i<=input.length(); i++){
            //如果不是運算子，則看到前面第幾個還是數字，切到那裡，然後回傳
            if(numbers.find(input[i]) == string::npos){
                number_pos = i;
                break;
            }
        }
        tokenString = input.substr(0,number_pos);
        input.erase(0,number_pos);
        return tokenString;
    }
}

class Operator{
public:
    string operator_mark;
    int precedence;
    Operator(string mark);
    Operator(int precedence);
    void operator=(const Operator& X);
};
Operator::Operator(string mark){
    this->operator_mark = mark;
    if(mark=="+" || mark=="-")
        this->precedence = 0;
    else
        this->precedence = 1;
}
Operator::Operator(int precedence){
    this->operator_mark = "";
    this->precedence = precedence;
}

void Operator::operator=(const Operator& X){
    this->operator_mark = X.operator_mark;
    this->precedence = X.precedence;
}

int main() {
    
    string input = "";
    cin >> input;
    //operand
    LinkedStack<int> stack1;
    //operator
    LinkedStack<Operator> stack2;
    
//    while(input.length()>0){
//        cout << token(input) << " ";
//    }
    
    string num = "0123456789";
    string token_inital = input_token(input);
    stack1.push(stoi(token_inital));
    cout << token_inital << endl;
    string operator_initail = input_token(input);
    stack2.push(Operator(operator_initail));
    
    //stage 1
    while(input.length()>0){
        string token = input_token(input);
        if(num.find(token[0]) != string::npos){
            stack1.push(stoi(token));
        }
        else{
            Operator A(token);
            
            
            //Operator::Operator(int precedence){
            Operator B(-1);
            try{
                B = stack2.peek();
            }
            catch(logic_error e){
            }
            while( B.precedence >= A.precedence ){
                Operator op = stack2.peek();
                stack2.pop();
                int num1 = stack1.peek();
                stack1.pop();
                int num2 = stack1.peek();
                stack1.pop();
                int result = 0;
                
                if(op.operator_mark == "+"){
                    result = num2 + num1;
                }
                else if (op.operator_mark == "-"){
                    result = num2 - num1;
                }
                else if (op.operator_mark == "*"){
                    result = num2 * num1;
                }
                else if (op.operator_mark == "/"){
                    result = num2 / num1;
                }
                
                stack1.push(result);
                
                try{
                    B = stack2.peek();
                }
                catch(logic_error e){
                    B.precedence = (-1);
                }
                
            }
            stack2.push(A);
            
            LinkedStack<int> stack1_print(stack1);
            stack1_print.print();
            cout << endl;
        }
    }
    
    while(stack2.isEmpty()==false){
        Operator op = stack2.peek();
        stack2.pop();
        int num1 = stack1.peek();
        stack1.pop();
        int num2 = stack1.peek();
        stack1.pop();
        int result = 0;
        
        if(op.operator_mark == "+"){
            result = num2 + num1;
        }
        else if (op.operator_mark == "-"){
            result = num2 - num1;
        }
        else if (op.operator_mark == "*"){
            result = num2 * num1;
        }
        else if (op.operator_mark == "/"){
            result = num2 / num1;
        }
        stack1.push(result);
    }
    cout << stack1.peek();
    stack1.pop();
    
    return 0;
}
