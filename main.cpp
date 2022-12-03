#include <iostream>
#include "fatBinaryTree.hpp"

using namespace std;

int main()
{

    FatBinaryTree fbt;
    
    for(int i = 0; i < 5; i++)
    {
        fbt.insert(i);
        fbt.pprint();
    }

    cout << fbt.find(0) << endl;

    cout << "\n\nremove\n\n";

    for(int i = 4; i >= 0; i--)
    {
        fbt.remove(i);
        fbt.pprint();
    }



    // for(int i = 0; i < 20; i++)
    // {
    //     fbt.insert(i);
    // }
    fbt.printBFS(4);
}