#include <iostream>
#include "environment.h"
#include "operators.h"
#include "st.h"
#include "st_types.h"

using namespace std;

shared_ptr<STNode> apply(shared_ptr<UnaryOperator> unOp, shared_ptr<STNode> rand);
shared_ptr<STNode> apply(shared_ptr<BinaryOperator> binOp, shared_ptr<STNode> rand_l, shared_ptr<STNode> rand_r);
shared_ptr<STNode> lookup(string name, shared_ptr<Environment> env);

void ST::runCSEMachine(vector<vector<shared_ptr<STNode>>> &controlStructures)
{
    vector<shared_ptr<STNode>> stack;
    vector<shared_ptr<STNode>> control;

    shared_ptr<Environment> e_0 = make_shared<Environment>();
    stack.push_back(e_0);
    control.push_back(e_0);
    control.insert(control.end(), controlStructures[0].begin(), controlStructures[0].end());

    shared_ptr<Environment> currentEnvironment = e_0;
    while (true)
    {
        if (control.empty())
        {
            break;
        }

        int controlSize = control.size();
        shared_ptr<STNode> next = control[controlSize - 1];
        control.pop_back();

        // CSE Rule 1
        if (next->getType() == "Identifier")
        {
            string name = dynamic_pointer_cast<Identifier>(next)->getName();
            shared_ptr<STNode> value = lookup(name, currentEnvironment);

            if (value == nullptr)
            {
                cout << "Error: Identifier " << name << " is not defined." << endl;
                exit(EXIT_FAILURE);
            }

            stack.push_back(value);
        }
    }
}

shared_ptr<STNode> apply(shared_ptr<UnaryOperator> unOp, shared_ptr<STNode> rand)
{
    string unOpStr = unOp->toString();
    if (unOpStr == "not")
    {
        if (rand->getType() == "TruthValue")
        {
            return !(*dynamic_pointer_cast<TruthValue>(rand));
        }
        else
        {
            return nullptr;
        }
    }
    else if (unOpStr == "neg")
    {
        if (rand->getType() == "Integer")
        {
            return dynamic_pointer_cast<Integer>(rand)->negate();
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        return nullptr;
    }
}

shared_ptr<STNode> apply(shared_ptr<BinaryOperator> binOp, shared_ptr<STNode> rand_l, shared_ptr<STNode> rand_r)
{
    string binOpStr = binOp->toString();
    if (binOpStr == "+")
    {
        if (rand_l->getType() == "Integer" && rand_r->getType() == "Integer")
        {
            return *dynamic_pointer_cast<Integer>(rand_l) + dynamic_pointer_cast<Integer>(rand_r);
        }
        else
        {
            return nullptr;
        }
    }
    else if (binOpStr == "-")
    {
        if (rand_l->getType() == "Integer" && rand_r->getType() == "Integer")
        {
            return *dynamic_pointer_cast<Integer>(rand_l) - dynamic_pointer_cast<Integer>(rand_r);
        }
        else
        {
            return nullptr;
        }
    }
    else if (binOpStr == "*")
    {
        if (rand_l->getType() == "Integer" && rand_r->getType() == "Integer")
        {
            return *dynamic_pointer_cast<Integer>(rand_l) * dynamic_pointer_cast<Integer>(rand_r);
        }
        else
        {
            return nullptr;
        }
    }
    else if (binOpStr == "/")
    {
        if (rand_l->getType() == "Integer" && rand_r->getType() == "Integer")
        {
            return *dynamic_pointer_cast<Integer>(rand_l) / dynamic_pointer_cast<Integer>(rand_r);
        }
        else
        {
            return nullptr;
        }
    }
    else if (binOpStr == "**")
    {
        if (rand_l->getType() == "Integer" && rand_r->getType() == "Integer")
        {
            return *dynamic_pointer_cast<Integer>(rand_l) ^ dynamic_pointer_cast<Integer>(rand_r);
        }
        else
        {
            return nullptr;
        }
    }
    else if (binOpStr == "aug")
    {
        if (rand_l->getType() == "Tuple")
        {
            dynamic_pointer_cast<Tuple>(rand_l)->push_back(rand_r);
            return rand_l;
        }
        else
        {
            return nullptr;
        }
    }
    else if (binOpStr == "or")
    {
        if (rand_l->getType() == "TruthValue" && rand_r->getType() == "TruthValue")
        {
            return *dynamic_pointer_cast<TruthValue>(rand_l) || dynamic_pointer_cast<TruthValue>(rand_r);
        }
        else
        {
            return nullptr;
        }
    }
    else if (binOpStr == "&")
    {
        if (rand_l->getType() == "TruthValue" && rand_r->getType() == "TruthValue")
        {
            return *dynamic_pointer_cast<TruthValue>(rand_l) && dynamic_pointer_cast<TruthValue>(rand_r);
        }
        else
        {
            return nullptr;
        }
    }
    else if (binOpStr == "gr")
    {
        if (rand_l->getType() == "Integer" && rand_r->getType() == "Integer")
        {
            return *dynamic_pointer_cast<Integer>(rand_l) > dynamic_pointer_cast<Integer>(rand_r);
        }
        else
        {
            return nullptr;
        }
    }
    else if (binOpStr == "ls")
    {
        if (rand_l->getType() == "Integer" && rand_r->getType() == "Integer")
        {
            return *dynamic_pointer_cast<Integer>(rand_l) < dynamic_pointer_cast<Integer>(rand_r);
        }
        else
        {
            return nullptr;
        }
    }
    else if (binOpStr == "ge")
    {
        if (rand_l->getType() == "Integer" && rand_r->getType() == "Integer")
        {
            return *dynamic_pointer_cast<Integer>(rand_l) >= dynamic_pointer_cast<Integer>(rand_r);
        }
        else
        {
            return nullptr;
        }
    }
    else if (binOpStr == "le")
    {
        if (rand_l->getType() == "Integer" && rand_r->getType() == "Integer")
        {
            return *dynamic_pointer_cast<Integer>(rand_l) <= dynamic_pointer_cast<Integer>(rand_r);
        }
        else
        {
            return nullptr;
        }
    }
    else if (binOpStr == "eq")
    {
        if (rand_l->getType() == "Integer" && rand_r->getType() == "Integer")
        {
            return *dynamic_pointer_cast<Integer>(rand_l) == dynamic_pointer_cast<Integer>(rand_r);
        }
        else if (rand_l->getType() == "TruthValue" && rand_r->getType() == "TruthValue")
        {
            return *dynamic_pointer_cast<TruthValue>(rand_l) == dynamic_pointer_cast<TruthValue>(rand_r);
        }
        else if (rand_l->getType() == "String" && rand_r->getType() == "String")
        {
            return *dynamic_pointer_cast<String>(rand_l) == dynamic_pointer_cast<String>(rand_r);
        }
        else
        {
            return nullptr;
        }
    }
    else if (binOpStr == "neq")
    {
        if (rand_l->getType() == "Integer" && rand_r->getType() == "Integer")
        {
            return *dynamic_pointer_cast<Integer>(rand_l) != dynamic_pointer_cast<Integer>(rand_r);
        }
        else if (rand_l->getType() == "TruthValue" && rand_r->getType() == "TruthValue")
        {
            return *dynamic_pointer_cast<TruthValue>(rand_l) != dynamic_pointer_cast<TruthValue>(rand_r);
        }
        else if (rand_l->getType() == "String" && rand_r->getType() == "String")
        {
            return *dynamic_pointer_cast<String>(rand_l) != dynamic_pointer_cast<String>(rand_r);
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        return nullptr;
    }
}

shared_ptr<STNode> lookup(string name, shared_ptr<Environment> env)
{
    if (env == nullptr)
    {
        return nullptr;
    }

    shared_ptr<STNode> val = env->getVariable(name);
    if (val != nullptr)
    {
        return val;
    }

    return lookup(name, env->getParent());
}
