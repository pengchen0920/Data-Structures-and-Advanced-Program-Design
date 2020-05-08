#include <iostream>
#include <string>
#include <algorithm>
#include <queue>

using namespace std;

const int ENGLISH_ALPHABET = 52;

// Prefix node
struct PrefixNode
{
    struct PrefixNode *children[ENGLISH_ALPHABET];
    
    // isLeaf is true if the node represents
    // end of a word
    bool isLeaf;
};

// Returns new Prefix node (initialized to NULLs)
PrefixNode* getNode(void)
{
    struct PrefixNode *pNode =  new PrefixNode;
    
    pNode->isLeaf = false;
    
    for (int i = 0; i < ENGLISH_ALPHABET; i++)
        pNode->children[i] = NULL;
    
    return pNode;
}

// If not present, inserts key into Prefix
// If the key is prefix of Prefix node, just
// marks leaf node
void insert(PrefixNode* root, string key)
{
    PrefixNode *pointer = root;
    
    for (int i = 0; i < key.length(); i++)
    {
        //
        int index;
        if (key[i] <= 'Z'){
            index = key[i]-'A';
        }
        else{
            index = key[i]-'a'+26;
        }
        if (!pointer->children[index])
            pointer->children[index] = getNode();
        
        pointer = pointer->children[index];
    }
    
    // mark last node as leaf
    pointer->isLeaf = true;
}

// Returns true if key presents in Prefix, else
// false
PrefixNode* search(PrefixNode* root, string key)
{
    PrefixNode* pointer = root;
    
    for (int i = 0; i < key.length(); i++)
    {
        int index;
        if (key[i] <= 'Z'){
            index = key[i]-'A';
        }
        else{
            index = key[i]-'a'+26;
        }
        pointer = pointer->children[index];
    }
    
    return pointer;
}

// function to check if current node is leaf node or not
bool isLeafNode(PrefixNode* root)
{
    return root->isLeaf;
}

// function to display the content of Trie
void display(PrefixNode* root, char str[], int level, string prefix, queue<string>& words)
{
    // If node is leaf node, it indiicates end
    // of string, so a null charcter is added
    // and string is displayed
    if (isLeafNode(root))
    {
        str[level] = '\0';
        words.push(prefix+str);
//        cout << prefix << str << endl;
    }
    
    
    for (int i = 0; i < ENGLISH_ALPHABET; i++)
    {
        // if NON NULL child is found
        // add parent key to str and
        // call the display function recursively
        // for child node
        if (root->children[i])
        {
            if(i<=25){
                str[level] = i + 'A';
            }
            else{
                str[level] = i + 'a' - 26;
            }
            display(root->children[i], str, level + 1, prefix, words);
        }
    }
}


int main()
{
    int m,n;
    cin >> m >> n;
    string* term = new string[m];
    string* prefix = new string[n];
    
    for(int i=0; i<m; i++){
        cin >> term[i];
    }
    for(int i=0; i<n; i++){
        cin >> prefix[i];
    }

    
//    for(int i=0; i<m; i++){
//        cout << term[i] << " ";
//    }
//    for(int i=0; i<n; i++){
//        cout << prefix[i] << " ";
//    }

    PrefixNode* root = getNode();
    // Construct Prefix
    for (int i = 0; i < m; i++)
        insert(root, term[i]);

    queue<string> words;
    for(int i=0; i<n; i++){
        char str[10000];
        display( search(root, prefix[i]), str, 0, prefix[i], words);
        while(!words.empty()){
            if(words.size()==1){
                cout << words.front();
                words.pop();
            }
            else{
                cout << words.front() << " ";
                words.pop();
            }
        }
        if(i < (n-1))
            cout << endl;
    }
    return 0;
} 
