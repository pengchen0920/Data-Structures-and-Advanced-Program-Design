#include<iostream>
#include<vector>
#include<iomanip>
#include<limits>
#include<string>
#include <algorithm>
using namespace std;
 
const int VERY_LARGE = numeric_limits<int>::max();//max of int:2147483647
 
 
class Path
{
private:
    vector<int> nodes;
    int capacity;
public:
    Path();
    Path(int aNode);
    Path(Path aShorterPath, int aNode, int capacity);
    vector<int> getNodes(){return nodes;};
    int getTail();
    int getCapacity();
    void print(vector<string> name);
//    void operator=(Path& aPath);
};
 
Path::Path() : capacity(VERY_LARGE)
{
}
 
Path::Path(int aNode) : capacity(VERY_LARGE)
{
    this->nodes.push_back(aNode);
}
 
Path::Path(Path aShorterPath, int aNode, int capacity)
{
    for(int i = 0; i < aShorterPath.nodes.size(); i++)
        this->nodes.push_back(aShorterPath.nodes[i]);
    this->nodes.push_back(aNode);
    this->capacity = capacity;
}
 
void Path::print(vector<string> name)
{
    if(this->nodes.size() == 0)
        cout << -1 << endl;
    else
    {
        for(int i = 0; i < this->nodes.size() - 1; i++)
            cout << name[this->nodes[i]] << ",";
        cout << name[this->nodes.back()] << ";";
        cout << this->capacity << endl;
    }
}
 
int Path::getTail()
{
    return this->nodes.back();
}
 
int Path::getCapacity()
{
    return this->capacity;
}
 
 
 
class Network
{
private:
    int n;
    int** resCap;
public:
    Network(int n);
    void adjustArcCap(int node1, int node2, int capacity);
    void print();
    vector<Path> getNextWithResCap(Path aPath,vector<string> name);
    Path getAnAugmentingPath(vector<string> name);
    ~Network();
    void swap(vector<Path>& longerPaths, int i, int j);
};
 
Network::Network(int n) : n(n)
{
    this->resCap = new int*[this->n];
    for(int i = 0; i < this->n; i++)
    {
        this->resCap[i] = new int[this->n];
        for(int j = 0; j < this->n; j++)
            this->resCap[i][j] = 0;
    }
}
 
void Network::adjustArcCap(int node1, int node2, int capacity)
{
    this->resCap[node1][node2] += capacity;
}
 
void Network::print()
{
    for(int i = 0; i < this->n; i++)
    {
        for(int j = 0; j < this->n; j++)
            cout << setw(2) << this->resCap[i][j] << " ";
        cout << endl;
    }
}
 
// input: a path of length L
// output: a vector of paths of length L + 1
// precondition: aPath is really a path in this network
void Network::swap(vector<Path>& longerPaths, int i, int j){
    Path temp = longerPaths[i];
    longerPaths[i] = longerPaths[j];
    longerPaths[j] = temp;
}
// put the "max flow" first
// if the flow equals, the put the smaller number in the queue first
vector<Path> Network::getNextWithResCap(Path aPath,vector<string> name)
{
    vector<Path> longerPaths;
    int tail = aPath.getTail();
     
    for(int i = 0; i < this->n; i++)
    {
        if(this->resCap[tail][i] > 0)
        {
            int oldCap = aPath.getCapacity();
            int newCap = oldCap;
            if(this->resCap[tail][i] < oldCap)
                newCap = this->resCap[tail][i];
            Path p(aPath, i, newCap);
            longerPaths.push_back(p);
        }
    }
    // sort the longerPaths by flow(from max to min)
    sort(longerPaths.begin(), longerPaths.end(),[](Path& one, Path& two){return one.getCapacity() > two.getCapacity();});
    //name[longerPaths[i].getTail()]
    //若運量平手，則將最終節點編號較小的先放入queue
    // if the flows equals, then we put smaller name(string.compare) first
     
    for(int i = 0; i<longerPaths.size(); i++){
        for(int j = i; j<longerPaths.size(); j++){
            if(longerPaths[i].getCapacity()==longerPaths[j].getCapacity() && i!=j){
                if(name[longerPaths[i].getTail()] > name[longerPaths[j].getTail()]){
                     
                    swap(longerPaths,i,j);
                }
            }
        }
    }
 
    return longerPaths;
     
}
 
// this function does not check whether a path visits a visited node
Path Network::getAnAugmentingPath(vector<string> name)
{
    // find an augmenting path through BFS
    vector<Path> pathQueue;
    Path start(0);
    pathQueue.push_back(start);
     
    Path anAugmentingPath;
    bool keepGoing = true;
    while(keepGoing && pathQueue.size() > 0)
    {
        Path cur = pathQueue.front();
        pathQueue.erase(pathQueue.begin());
        vector<Path> curPlusOne = this->getNextWithResCap(cur,name);
         
        for(int i = 0; i < curPlusOne.size(); i++)
        {
            bool tail_is_visted = false;
            // whether the tail is a visited node should be checked below
            for(int j=0; j<curPlusOne[i].getNodes().size()-1; j++){
                if(curPlusOne[i].getTail()==curPlusOne[i].getNodes()[j]){
                    tail_is_visted = true;
                }
            }
            if(tail_is_visted)
                continue;
            //      cout << "..." << curPlusOne[i].getTail() << endl;
            if(curPlusOne[i].getTail() == n - 1)
            {
                anAugmentingPath = curPlusOne[i];
                keepGoing = false;
                break;
            }
            else
                pathQueue.push_back(curPlusOne[i]);
        }
         
        //    cout << "===============\n";
        //    for(int i = 0; i < pathQueue.size(); i++)
        //      pathQueue[i].print();
    }
    return anAugmentingPath;
}
 
Network::~Network()
{
    for(int i = 0; i < this->n; i++)
        delete [] resCap[i];
    delete [] resCap;
}
 

int main()
{
    // initializing the original network
    int n = 0, m = 0;
    string line;
    cin >> line;
    string delimiter = ",";
    size_t pos = 0;
    //n
    pos = line.find(delimiter);
    n = stoi(line.substr(0, pos));
    line.erase(0, pos + delimiter.length());
    //m
    m = stoi(line);
    line.erase(0, line.length());
     
    //nodes name input
    vector<string> name;
    cin >> line;
    pos = 0;
    int name_cnt = 0;
    while(name_cnt < n){
        if(name_cnt<n-1){
            pos = line.find(delimiter);
            name.push_back(line.substr(0, pos));
            line.erase(0, pos + delimiter.length());
        }
        else{
            name.push_back(line);
            line.erase(0, line.length());
        }
        name_cnt++;
    }
     
    Network net(n);
     
     
    //edge input
    int input_cnt = 0;
    pos = 0;
    while(input_cnt < m){
        cin >> line;
        string u = "";
        string v = "";
        int capacity = 0;
        pos = line.find(delimiter);
        u = (line.substr(0, pos));
        line.erase(0, pos + delimiter.length());
        pos = line.find(delimiter);
        v = (line.substr(0, pos));
        line.erase(0, pos + delimiter.length());
        capacity = stoi(line);
        vector<string>::iterator it = find(name.begin(), name.end(), u);
        int u_index = static_cast<int>(distance(name.begin(), it));
        it = find(name.begin(), name.end(), v);
        int v_index = static_cast<int>(distance(name.begin(), it));
//        cout << "u_index:" << u_index << "\t" << "v_index:" << v_index << "\t" << "capacity:" << capacity << endl;
        net.adjustArcCap(u_index, v_index, capacity);
        input_cnt++;
    }
     
//    net.print();
     
    net.getAnAugmentingPath(name).print(name);
     
     
    return 0;
}