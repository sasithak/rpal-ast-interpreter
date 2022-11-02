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
        children.push_back(postOrder(child, level + 1));
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

    if (this->value == "tau")
    {
        return make_shared<Tau>(children);
    }

    return nullptr;
}
