#include <iostream>
#include <iomanip>
#include <string>
#include "st.h"

using namespace std;

ST::ST(shared_ptr<STNode> root)
{
    this->root = root;
}

void ST::execute(ostream &out)
{
    vector<vector<shared_ptr<STNode>>> controlStructures;
    vector<shared_ptr<Delta>> deltas;

    shared_ptr<Delta> delta = make_shared<Delta>(0, root);
    deltas.push_back(delta);

    int i = 0;
    while (i < (int)deltas.size())
    {
        vector<shared_ptr<STNode>> controlStructure;
        preOrder(deltas[i]->getChildren()[0], controlStructure, deltas);
        controlStructures.push_back(controlStructure);
        ++i;
    }

    int width = 6 + to_string(controlStructures.size()).length();
    for (int i = 0; i < (int)controlStructures.size(); ++i)
    {
        out << right << setw(width) << ("delta_" + to_string(i)) << left << ": ";
        for (int j = 0; j < (int)controlStructures[i].size(); ++j)
        {
            out << controlStructures[i][j]->toString() << (j == (int)controlStructures[i].size() - 1 ? "" : " ");
        }
        out << endl;
    }
    out << endl;

    runCSEMachine(controlStructures, out);
}

ostream &operator<<(ostream &os, const ST &st)
{
    st.preOrder(st.root, 0, os);
    return os;
}

void ST::printST()
{
    cout << *this;
}

void ST::preOrder(shared_ptr<STNode> node, int level, ostream &os) const
{
    if (node == nullptr)
    {
        return;
    }

    for (int i = 0; i < level; ++i)
    {
        os << ".";
    }

    if (node->getType() == "String")
    {
        os << "'" << *node << "'" << endl;
    }
    else
    {
        os << *node << endl;
    }

    if (node->getType() == "Arrow")
    {
        auto children = node->getChildren();
        preOrder(children[3], level + 1, os);
        preOrder(children[0]->getChildren()[0], level + 1, os);
        preOrder(children[1]->getChildren()[0], level + 1, os);
    }
    else
    {
        for (auto child : node->getChildren())
        {
            preOrder(child, level + 1, os);
        }
    }
}

void ST::preOrder(shared_ptr<STNode> node, vector<shared_ptr<STNode>> &controlStructure, vector<shared_ptr<Delta>> &deltas) const
{
    if (node == nullptr)
    {
        return;
    }

    bool expandChildren = true;
    string nodeType = node->getType();
    if (nodeType != "Arrow")
    {
        if (nodeType == "Lambda")
        {
            dynamic_pointer_cast<Lambda>(node)->setIndex(deltas.size());
            shared_ptr<Delta> delta = make_shared<Delta>((int)deltas.size(), node->getChildren()[0]);
            deltas.push_back(delta);
            expandChildren = false;
        }

        else if (nodeType == "Delta")
        {
            dynamic_pointer_cast<Delta>(node)->setIndex(deltas.size());
            deltas.push_back(dynamic_pointer_cast<Delta>(node));
            expandChildren = false;
        }

        controlStructure.push_back(node);
    }

    if (expandChildren)
    {
        for (auto child : node->getChildren())
        {
            preOrder(child, controlStructure, deltas);
        }
    }
}
