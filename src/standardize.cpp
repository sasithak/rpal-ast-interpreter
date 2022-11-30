#include <iostream>
#include <vector>
#include "ast.h"
#include "operators.h"
#include "st.h"

using namespace std;

/**
 * @brief Bind the variables of a lambda
 * @param l The lambda to bind
 * @param b The identifier or comma node to bind
 * @param v The value to bind
 */
void bind_lambda(shared_ptr<Lambda> l, shared_ptr<STNode> b, shared_ptr<STNode> p);

/**
 * @brief Bind the variables of a lambda
 * @param l The lambda to bind
 * @param toBind The vector of identifiers or comma nodes to bind; the last element is the value to bind
 * @param startIndex The index to start binding from
 */
void bind_lambda(shared_ptr<Lambda> l, vector<shared_ptr<STNode>> toBind, int startIndex);

void checkChildrenCount(string nodeStr, int expected, int given);

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
        // recursively standardize the children
        children.push_back(postOrder(child, level + 1));
    }

    return node->standardize(children); // standardize the node
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
    else if (this->value == "<true>")
    {
        return make_shared<TruthValue>(true);
    }
    else if (this->value == "<false>")
    {
        return make_shared<TruthValue>(false);
    }
    else if (this->value == "<nil>")
    {
        return make_shared<Tuple>();
    }

    // Custom standardizations as in CSE Machine rules

    if (isUnOp(this->value))
    {
        checkChildrenCount(this->value, 1, children.size());
        return make_shared<UnaryOperator>(this->value, children[0]);
    }

    if (isBinOp(this->value))
    {
        checkChildrenCount(this->value, 2, children.size());
        return make_shared<BinaryOperator>(this->value, children[0], children[1]);
    }

    if (this->value == "->")
    {
        checkChildrenCount("Arrow", 3, children.size());
        vector<shared_ptr<STNode>> newChildren;
        newChildren.push_back(make_shared<Delta>(children[1])); // delta_then
        newChildren.push_back(make_shared<Delta>(children[2])); // delta_else
        newChildren.push_back(make_shared<Beta>());
        newChildren.push_back(children[0]);
        return make_shared<Arrow>(newChildren);
    }

    if (this->value == "tau")
    {
        return make_shared<Tau>(children);
    }

    if (this->value == "function_form")
    {
        if ((int)children.size() < 3)
        {
            cerr << "Function form must have at least 3 children";
            exit(EXIT_FAILURE);
        }

        auto p = children[0];

        shared_ptr<Lambda> l = make_shared<Lambda>();
        bind_lambda(l, children, 1); // bind the identifiers and comma nodes to the lambda

        shared_ptr<Equal> eq = make_shared<Equal>();
        eq->addChild(p);
        eq->addChild(l);
        return eq;
    }

    if (this->value == "lambda")
    {
        if ((int)children.size() < 2)
        {
            cerr << "Lambda must have at least 2 children";
            exit(EXIT_FAILURE);
        }

        shared_ptr<Lambda> l = make_shared<Lambda>();
        bind_lambda(l, children, 0); // bind the identifiers and comma nodes to the lambda
        return l;
    }

    // Standardizations as in RPAL Semantics

    if (this->value == "and")
    {
        shared_ptr<Equal> eq = make_shared<Equal>();
        shared_ptr<Comma> comma = make_shared<Comma>();
        vector<shared_ptr<STNode>> e_s;

        for (auto child : children)
        {
            // iterate through each identifier and value pair
            if (child->getType() != "Equal")
            {
                cerr << "Error: Expected 'Equal' node while standardizing 'Rec' node\n";
                exit(EXIT_FAILURE);
            }

            auto child_children = child->getChildren();
            comma->addChild(child_children[0]); // add the identifier to the comma node
            e_s.push_back(child_children[1]);   // add the value to the tau node
        }

        shared_ptr<Tau> t = make_shared<Tau>(e_s); // create the tau node

        eq->addChild(comma);
        eq->addChild(t);
        return eq;
    }

    if (this->value == "@")
    {
        checkChildrenCount("@", 3, children.size());
        shared_ptr<Gamma> g_1 = make_shared<Gamma>();
        shared_ptr<Gamma> g_2 = make_shared<Gamma>();
        g_2->addChild(children[1]);
        g_2->addChild(children[0]);
        g_1->addChild(g_2);
        g_1->addChild(children[2]);
        return g_1;
    }

    if (this->value == "rec")
    {
        checkChildrenCount("Rec", 1, children.size());

        if (children[0]->getType() != "Equal")
        {
            cerr << "Error: Expected 'Equal' node while standardizing 'Rec' node\n";
            exit(EXIT_FAILURE);
        }

        auto child_children = children[0]->getChildren();
        auto x = child_children[0];
        auto e = child_children[1];

        shared_ptr<Equal> eq = make_shared<Equal>();
        shared_ptr<Gamma> g = make_shared<Gamma>();
        shared_ptr<Lambda> l = make_shared<Lambda>();
        shared_ptr<YStar> y = make_shared<YStar>();

        bind_lambda(l, x, e);
        g->addChild(y);
        g->addChild(l);
        eq->addChild(x);
        eq->addChild(g);

        return eq;
    }

    if (this->value == "within")
    {
        checkChildrenCount("Within", 2, children.size());

        if (children[0]->getType() != "Equal" || children[1]->getType() != "Equal")
        {
            cerr << "Error: Expected two 'Equal' nodes while standardizing 'Within' node\n";
            exit(EXIT_FAILURE);
        }

        auto children_1 = children[0]->getChildren();
        auto x_1 = children_1[0];
        auto e_1 = children_1[1];

        auto children_2 = children[1]->getChildren();
        auto x_2 = children_2[0];
        auto e_2 = children_2[1];

        shared_ptr<Equal> eq = make_shared<Equal>();
        shared_ptr<Gamma> g = make_shared<Gamma>();
        shared_ptr<Lambda> l = make_shared<Lambda>();

        bind_lambda(l, x_1, e_2);
        g->addChild(l);
        g->addChild(e_1);
        eq->addChild(x_2);
        eq->addChild(g);

        return eq;
    }

    if (this->value == "where")
    {
        checkChildrenCount("Where", 2, children.size());

        if (children[1]->getType() != "Equal")
        {
            cerr << "Error: Expected 'Equal' node while standardizing 'Where' node\n";
            exit(EXIT_FAILURE);
        }

        auto p = children[0];
        auto children_eq = children[1]->getChildren();
        auto x = children_eq[0];
        auto e = children_eq[1];

        shared_ptr<Gamma> g = make_shared<Gamma>();
        shared_ptr<Lambda> l = make_shared<Lambda>();

        bind_lambda(l, x, p);
        g->addChild(l);
        g->addChild(e);

        return g;
    }

    if (this->value == "let")
    {
        checkChildrenCount("Let", 2, children.size());

        if (children[0]->getType() != "Equal")
        {
            cerr << "Error: Expected 'Equal' node while standardizing 'Let' node\n";
            exit(EXIT_FAILURE);
        }

        auto children_eq = children[0]->getChildren();
        auto x = children_eq[0];
        auto e = children_eq[1];
        auto p = children[1];

        shared_ptr<Gamma> g = make_shared<Gamma>();
        shared_ptr<Lambda> l = make_shared<Lambda>();

        bind_lambda(l, x, p);
        g->addChild(l);
        g->addChild(e);

        return g;
    }

    if (this->value == "gamma")
    {
        checkChildrenCount("Gamma", 2, children.size());
        shared_ptr<Gamma> g = make_shared<Gamma>();
        g->addChild(children[0]);
        g->addChild(children[1]);
        return g;
    }

    if (this->value == ",")
    {
        shared_ptr<Comma> c = make_shared<Comma>();
        for (auto child : children)
        {
            c->addChild(child);
        }
        return c;
    }

    if (this->value == "=")
    {
        checkChildrenCount("Equal", 2, children.size());
        shared_ptr<Equal> eq = make_shared<Equal>();
        eq->addChild(children[0]);
        eq->addChild(children[1]);
        return eq;
    }

    cerr << "Unknown node type: " << this->value << "\n";
    exit(EXIT_FAILURE);
}

void bind_lambda(shared_ptr<Lambda> l, shared_ptr<STNode> b, shared_ptr<STNode> p)
{
    if (b->getType() == "Comma")
    {
        // bind all the identifiers in the comma node to the lambda node
        auto children = b->getChildren();
        for (auto child : children)
        {
            if (child->getType() != "Identifier")
            {
                cerr << "Error: Expected 'Identifier' node while binding lambda.\n";
                exit(EXIT_FAILURE);
            }
            l->addBinding(dynamic_pointer_cast<Identifier>(child));
        }
    }
    else if (b->getType() == "Identifier")
    {
        // bind the identifier to the lambda node
        l->addBinding(dynamic_pointer_cast<Identifier>(b));
    }
    else
    {
        cerr << "Error: Expected 'Comma' or 'Identifier' node while binding Lambda\n";
        exit(EXIT_FAILURE);
    }

    l->addChild(p);
}

void bind_lambda(shared_ptr<Lambda> l, vector<shared_ptr<STNode>> toBind, int startIndex)
{
    int toBindSize = toBind.size();

    if (startIndex == 1 && toBindSize == 3)
    {
        auto b = toBind[1];
        auto e = toBind[2];
        bind_lambda(l, b, e);
    }
    else if (startIndex == 0 && toBindSize == 2)
    {
        auto b = toBind[0];
        auto e = toBind[1];
        bind_lambda(l, b, e);
    }
    else
    {
        shared_ptr<Lambda> w = l;
        for (int i = startIndex; i < toBindSize - 1; ++i)
        {
            // iterate through all the identifiers and comma nodes create the tree of lambdas, binding those identifiers

            auto b = toBind[i];

            if (i == toBindSize - 2)
            {
                // bind the value to the last lambda
                auto p = toBind[toBindSize - 1];
                bind_lambda(w, b, p);
            }
            else
            {
                // if not the last identifier or comma node, create a new lambda node and bind the identifier(s) to it
                auto p = make_shared<Lambda>();
                bind_lambda(w, b, p);
                w = p; // set the current lambda node to the new lambda node to continue building the tree
            }
        }
    }
}

void checkChildrenCount(string nodeStr, int expected, int given)
{
    if (expected != given)
    {
        cerr << "Error: Expected " << expected << " children for node '" << nodeStr << "', got " << given << "\n";
        exit(EXIT_FAILURE);
    }
}
