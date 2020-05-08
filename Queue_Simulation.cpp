#include <iostream>
#include <queue>
#include <cmath>
using namespace std;

class Guest{
public:
    Guest(double arr,double t);
    double arrive;
    double time;
    double start;
    double end;
};
Guest::Guest(double arr,double t){
    arrive = arr;
    time = t;
    start = 0;
    end = 0;
}
double rounding(double num, int index){
    bool isNegative = false; // whether is negative number or not
    
    if(num < 0) // if this number is negative, then convert to positive number
    {
        isNegative = true;
        num = -num;
    }
    
    if(index >= 0)
    {
        int multiplier;
        multiplier = pow(10, index);
        num = (int)(num * multiplier + 0.5) / (multiplier * 1.0);
    }
    
    if(isNegative) // if this number is negative, then convert to negative number
    {
        num = -num;
    }
    
    return num;
}
double waitingTime(queue<Guest*> line){
    double present = 0;
    double waiting = 0;
    
    while(!line.empty()){
        Guest* newGuest = line.front();
        if(newGuest->arrive >= present){
            present = (newGuest->arrive + newGuest->time);
            line.pop();
        }
        else{
            newGuest->start += present;
            newGuest->end = newGuest->start + newGuest->time;
            waiting += (newGuest->start - newGuest->arrive);
            present = newGuest->end;
            line.pop();
        }
    }
    
    
    return waiting;
}

int main ()
{
    int n;
    double lambda;
    double mu;
    cin >> n >> lambda >> mu;
    queue<Guest*> line;
    
    for(int i = 0; i<n; i++){
        double x1,x2;
        cin >> x1 >> x2;
        Guest* newGuest = new Guest(x1,x2);
        line.push(newGuest);
    }
    double preWaiting,realWaiting;
    preWaiting = lambda/(mu*(mu-lambda));
    realWaiting = waitingTime(line)/n;
    
    cout << rounding(((realWaiting-preWaiting)/preWaiting)*100,0);
    
}
