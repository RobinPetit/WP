#include <iostream>
using namespace std;


#include "Monster.hpp"

int main()
{
    vector<int> x = {0,1,2};
    
    Card Stan(10, "Très nul", x );
    Stan.print();                 //Card test and print
    std::cout << endl;
    
    x.push_back(10);
    
    Monster allan(10, "Roi", x , 10, 10);
    cout << endl;       // Monster test and print
    allan.print();
}
