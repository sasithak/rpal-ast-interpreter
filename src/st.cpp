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

ostream &operator<<(ostream &os, const ST &st)
{
    st.preOrderPrint(st.root, 0, os);
    return os;
}

void ST::printST()
{
    cout << *this;
}

void ST::preOrderPrint(shared_ptr<STNode> node, int level, ostream &os) const
{
    if (node == nullptr)
    {
        return;
    }

    for (int i = 0; i < level; ++i)
    {
        os << ".";
    }
    os << *node << endl;
    for (auto child : node->getChildren())
    {
        preOrderPrint(child, level + 1, os);
    }
}
