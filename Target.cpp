#include <iostream>
#include <windows.h>
using namespace std;

int main()
{
    int hp = 100;

    while(true)
    {
        hp = hp - 1;
        cout << hp << endl;
        Sleep(3000);
    }
return 0;
}

