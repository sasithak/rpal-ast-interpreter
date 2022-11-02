#include <iostream>
#include <vector>
#include "ast.h"
#include "st.h"

using namespace std;

string binaryOperators[] = {"+", "-", "*", "/", "**", "aug", "or", "&", "gr", "ls", "ge", "le", "eq", "ne"};
string unaryOperators[] = {"not", "neg"};

shared_ptr<ST> AST::standardize() const
{
    return make_shared<ST>(postOrder(root, 0));
}

shared_ptr<STNode> AST::postOrder(shared_ptr<ASTNode> node, int level) const
{
    if (node == nullptr)
    {
        return nullptr;
    }

    vector<shared_ptr<STNode>> children;
    for (auto child : node->children)
    {
        auto childNode = postOrder(child, level + 1);

        if (childNode->toString() != "arrow")
        {
            children.push_back(childNode);
        }
        else
        {
            children.insert(children.end(), childNode->getChildren().begin(), childNode->getChildren().end());
        }
    }

    return node->standardize(children);
}

shared_ptr<STNode> ASTNode::standardize(vector<shared_ptr<STNode>> children) const
{
    if (this->type == IDENTIFIER)
    {
        return make_shared<Identifier>(this->value);
    }
    else if (this->type == INTEGER)
    {
        return make_shared<Integer>(this->value);
    }
    else if (this->type == STRING)
    {
        return make_shared<String>(this->value);
    }
    else if (this->value == "true")
    {
        return make_shared<TruthValue>(true);
    }
    else if (this->value == "false")
    {
        return make_shared<TruthValue>(false);
    }

    // Custom standardizations as in CSE Machine rules

    for (int i = 0; i < 2; i++)
    {
        if (this->value == unaryOperators[i])
        {
            return make_shared<UnaryOperator>(this->value, children[0]);
        }
    }

    for (int i = 0; i < 14; i++)
    {
        if (this->value == binaryOperators[i])
        {
            return make_shared<BinaryOperator>(this->value, children[0], children[1]);
        }
    }

    if (this->value == "->")
    {
        vector<shared_ptr<STNode>> newChildren;
        newChildren.push_back(make_shared<Delta>(children[1]));
        newChildren.push_back(make_shared<Delta>(children[2]));
        newChildren.push_back(make_shared<Beta>());
        newChildren.push_back(children[0]);
        return make_shared<Arrow>(newChildren);
    }

    if (this->value == "tau")
    {
        return make_shared<Tau>(children);
    }

    if (this->value == "fcn_form")
    {
        int childrenCnt = children.size();
        auto p = children[0];
        auto e = children[childrenCnt - 1];

        shared_ptr<Lambda> l = make_shared<Lambda>();
        l->addChild(e);
        for (int i = 1; i < childrenCnt - 1; ++i)
        {
            l->addBinding(dynamic_pointer_cast<Identifier>(children[i]));
        }

        shared_ptr<Equal> eq = make_shared<Equal>();
        eq->addChild(p);
        eq->addChild(l);
        return eq;
    }

    return nullptr;
}
