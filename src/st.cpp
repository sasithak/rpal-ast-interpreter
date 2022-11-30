#include <iostream>
#include <iomanip>
#include <string>
#include "st.h"

using namespace std;

ST::ST(shared_ptr<STNode> root)
{
    this->root = root;
}

void ST::execute(bool printCS, bool printExe)
{
    vector<vector<shared_ptr<STNode>>> controlStructures; // stores the control structures
    vector<shared_ptr<Delta>> deltas;                     // stores the deltas

    shared_ptr<Delta> delta = make_shared<Delta>(0, root); // create the initial delta
    deltas.push_back(delta);

    int i = 0;
    while (i < (int)deltas.size())
    {
        // iterate through the deltas
        vector<shared_ptr<STNode>> controlStructure;

        // go in preorder through the subtree referenced by the delta and generate the control structures
        preOrder(deltas[i]->getChildren()[0], controlStructure, deltas);
        controlStructures.push_back(controlStructure);
        ++i;
    }

    if (printCS)
    {
        int width = 6 + to_string(controlStructures.size()).length();
        for (int i = 0; i < (int)controlStructures.size(); ++i)
        {
            cout << right << setw(width) << ("delta_" + to_string(i)) << left << ": ";
            int ith_size = controlStructures[i].size();
            for (int j = 0; j < (int)controlStructures[i].size(); ++j)
            {
                shared_ptr<STNode> node = controlStructures[i][j];
                cout << (node->getType() == "String" ? "'" + node->toString() + "'" : node->toString())
                     << (j == ith_size - 1 ? "\n" : " ");
            }
        }
        cout << "\n";
    }

    runCSEMachine(controlStructures, printExe); // run the CSE machine with the control structures
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

    os << node->toCompleteString() << "\n";

    if (node->getType() == "Arrow")
    {
        auto children = node->getChildren();
        preOrder(children[3], level + 1, os);
        preOrder(children[0]->getChildren()[0], level + 1, os);
        preOrder(children[1]->getChildren()[0], level + 1, os);
    }
    else if (node->getType() == "Lambda")
    {
        shared_ptr<Lambda> l = dynamic_pointer_cast<Lambda>(node);
        auto children = l->getChildren();
        auto bindings = l->getBindings();
        int bindingCount = l->getBindingCount();

        if (bindingCount == 1)
        {
            preOrder(bindings[0], level + 1, os);
        }
        else if (bindingCount > 1)
        {
            shared_ptr<STNode> c = make_shared<Comma>();
            for (int i = 0; i < bindingCount; ++i)
            {
                c->addChild(bindings[i]);
            }
            preOrder(c, level + 1, os);
        }

        for (auto child : children)
        {
            preOrder(child, level + 1, os);
        }
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
    if (nodeType != "Arrow") // skip the arrow node
    {
        if (nodeType == "Lambda")
        {
            dynamic_pointer_cast<Lambda>(node)->setIndex(deltas.size());
            shared_ptr<Delta> delta = make_shared<Delta>((int)deltas.size(), node->getChildren()[0]); // create a new delta for the right child of lambda
            deltas.push_back(delta);
            expandChildren = false; // skip traversing through the children of lambda; they will be traversed through the new delta
        }

        else if (nodeType == "Delta")
        {
            // delta_then or delta_else of arrow node
            dynamic_pointer_cast<Delta>(node)->setIndex(deltas.size());
            deltas.push_back(dynamic_pointer_cast<Delta>(node));
            expandChildren = false; // skip traversing through the children of delta; they will be traversed later
        }

        controlStructure.push_back(node); // add the node to the control structure
    }

    if (expandChildren)
    {
        for (auto child : node->getChildren())
        {
            preOrder(child, controlStructure, deltas);
        }
    }
}
