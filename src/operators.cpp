#include <iostream>
#include "operators.h"

using namespace std;

string unOps[] = {"not", "neg"};
string binOps[] = {"+", "-", "*", "/", "**", "aug", "or", "&", "gr", "ls", "ge", "le", "eq", "ne"};

bool isUnOp(string op)
{
    for (int i = 0; i < 2; ++i)
    {
        if (op == unOps[i])
        {
            return true;
        }
    }
    return false;
}

bool isBinOp(string op)
{
    for (int i = 0; i < 14; ++i)
    {
        if (op == binOps[i])
        {
            return true;
        }
    }
    return false;
}
