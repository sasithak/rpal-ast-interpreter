#include <iostream>
#include <vector>
#include "ast.h"
#include "operators.h"
#include "st.h"

using namespace std;

void bind_lambda(shared_ptr<Lambda> l, shared_ptr<STNode> b, shared_ptr<STNode> p)
{
    if (b->toString() == ",")
    {
        shared_ptr<Identifier> temp = make_shared<Identifier>("Temp");
        l->addBinding(temp);
        auto children = b->getChildren();
        int childrenCnt = children.size();

        weak_ptr<Lambda> w_l = l;
        for (int i = childrenCnt - 1; i >= 0; --i)
        {
            shared_ptr<Gamma> g_1 = make_shared<Gamma>();
            shared_ptr<Gamma> g_2 = make_shared<Gamma>();
            shared_ptr<Lambda> l_1 = make_shared<Lambda>();

            g_2->addChild(temp);
            g_2->addChild(make_shared<Integer>(i + 1));
            l_1->addBinding(dynamic_pointer_cast<Identifier>(children[i]));
            w_l.lock()->addChild(g_1);
            w_l = l_1;
        }
        w_l.lock()->addChild(p);
    }
    else
    {
        l->addBinding(dynamic_pointer_cast<Identifier>(b));
        l->addChild(p);
    }
}

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
    else if (this->value == "<nil>")
    {
        return make_shared<Tuple>();
    }

    // Custom standardizations as in CSE Machine rules

    if (isUnOp(this->value))
    {
        return make_shared<UnaryOperator>(this->value, children[0]);
    }

    if (isBinOp(this->value))
    {
        return make_shared<BinaryOperator>(this->value, children[0], children[1]);
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

    if (this->value == "function_form")
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

    if (this->value == "lambda")
    {
        int childrenCnt = children.size();
        auto e = children[childrenCnt - 1];
        shared_ptr<Lambda> l = make_shared<Lambda>();
        l->addChild(e);
        for (int i = 0; i < childrenCnt - 1; ++i)
        {
            l->addBinding(dynamic_pointer_cast<Identifier>(children[i]));
        }
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
            auto child_children = child->getChildren();
            comma->addChild(child_children[0]);
            e_s.push_back(child_children[1]);
        }

        shared_ptr<Tau> t = make_shared<Tau>(e_s);

        eq->addChild(comma);
        eq->addChild(t);
        return eq;
    }

    if (this->value == "@")
    {
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
        shared_ptr<Gamma> g = make_shared<Gamma>();
        g->addChild(children[0]);
        g->addChild(children[1]);
        return g;
    }

    return nullptr;
}
