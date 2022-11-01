#include <iostream>
#include "st.h"

using namespace std;

ST::ST(shared_ptr<STNode> root)
{
    this->root = root;
}

void ST::execute()
{
    cout << "Executing..." << endl;
}
