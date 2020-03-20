#ifndef STD
#define STD

#include <bits/stdc++.h>
using namespace std;

namespace Template
{
    void check(const bool& expression, string message = "")
    {
        if (!expression)
        {
            cerr << message << "\n";
            exit(0);
        }
    }

}

#endif
