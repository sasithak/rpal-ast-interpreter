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
            continue;
        }

        // CSE Rule 2
        if (next->getType() == "Lambda")
        {
            shared_ptr<Lambda> l = dynamic_pointer_cast<Lambda>(next);
            l->setEnv(currentEnvironment->getIndex());
            stack.push_back(l);
            continue;
        }

        if (next->getType() == "Gamma")
        {
            if (stack.size() < 3)
            {
                cout << "Error: Stack underflow." << endl;
                exit(EXIT_FAILURE);
            }

            shared_ptr<STNode> rator = stack[stack.size() - 1];
            shared_ptr<STNode> rand = stack[stack.size() - 2];
            stack.pop_back();
            stack.pop_back();

            /**
             * TODO: Complete Rule 3
             */
            // CSE Rule 3
            if (rator->getType() == "")
            {
                // apply(rator, rand);
                // continue;
            }

            // CSE Rule 4 & CSE Rule 11
            if (rator->getType() == "Lambda")
            {
                shared_ptr<Lambda> l = dynamic_pointer_cast<Lambda>(next);
                shared_ptr<Environment> newEnv = make_shared<Environment>();
                shared_ptr<Delta> d = make_shared<Delta>(l->getIndex(), l->getChildren()[0]);

                newEnv->setParent(currentEnvironment);

                int bindingCnt = l->getBindingCount();
                auto bindings = l->getBindings();
                if (bindingCnt > 1)
                {
                    if (rand->getType() == "Tuple")
                    {
                        shared_ptr<Tuple> t = dynamic_pointer_cast<Tuple>(rand);
                        for (int i = 0; i < bindingCnt; ++i)
                        {
                            string name = dynamic_pointer_cast<Identifier>(bindings[i])->getName();
                            newEnv->addVariable(name, (*t)[i]);
                        }
                    }
                    else
                    {
                        string name = dynamic_pointer_cast<Identifier>(bindings[0])->getName();
                        newEnv->addVariable(name, rand);

                        for (int i = 1; i < bindingCnt; ++i)
                        {
                            shared_ptr<STNode> _next = control[control.size() - 1];
                            string name = dynamic_pointer_cast<Identifier>(bindings[i])->getName();
                            if (_next->getType() == "Gamma")
                            {
                                control.pop_back();
                                newEnv->addVariable(name, rand);
                            }
                            else
                            {
                                cout << "Error: parameter '" << name << "' not found\n";
                                exit(EXIT_FAILURE);
                            }
                        }
                    }
                }
                else
                {
                    string name = dynamic_pointer_cast<Identifier>(bindings[0])->getName();
                    newEnv->addVariable(name, rand);
                }

                currentEnvironment = newEnv;
                control.push_back(newEnv);
                stack.push_back(newEnv);
                vector<shared_ptr<STNode>> _delta = controlStructures[l->getIndex()];
                control.insert(control.end(), _delta.begin(), _delta.end());
                continue;
            }
        }

        // CSE Rule 5
        if (next->getType() == "Environment")
        {
            if (stack.size() < 3)
            {
                cout << "Error: Stack underflow." << endl;
                exit(EXIT_FAILURE);
            }

            shared_ptr<STNode> v = stack[stack.size() - 1];
            shared_ptr<STNode> e = stack[stack.size() - 2];

            if (e->getType() != "Environment")
            {
                cout << "Error: Expected environment." << endl;
                exit(EXIT_FAILURE);
            }

            stack.pop_back();
            stack.pop_back();
            stack.push_back(v);

            std::shared_ptr<STNode> it;
            for (int i = stack.size() - 1; i >= 0; --i)
            {
                it = stack[i];
                if (it->getType() == "Environment")
                {
                    currentEnvironment = dynamic_pointer_cast<Environment>(it);
                    break;
                }
            }
        }

        // CSE Rule 6
        if (next->getType() == "BinaryOperator")
        {
            if (stack.size() < 3)
            {
                cout << "Error: Stack underflow." << endl;
                exit(EXIT_FAILURE);
            }

            shared_ptr<BinaryOperator> binOp = dynamic_pointer_cast<BinaryOperator>(next);
            shared_ptr<STNode> rand_l = stack[stack.size() - 1];
            shared_ptr<STNode> rand_r = stack[stack.size() - 2];
            stack.pop_back();
            stack.pop_back();

            shared_ptr<STNode> result = apply(binOp, rand_l, rand_r);
            stack.push_back(result);
            continue;
        }

        // CSE Rule 7
        if (next->getType() == "UnaryOperator")
        {
            if (stack.size() < 2)
            {
                cout << "Error: Stack underflow." << endl;
                exit(EXIT_FAILURE);
            }

            shared_ptr<UnaryOperator> unOp = dynamic_pointer_cast<UnaryOperator>(next);
            shared_ptr<STNode> rand = stack[stack.size() - 1];
            stack.pop_back();

            shared_ptr<STNode> result = apply(unOp, rand);
            stack.push_back(result);
            continue;
        }

        // CSE Rule 8
        if (next->getType() == "Beta")
        {
            if (stack.size() < 2)
            {
                cout << "Error: Stack underflow." << endl;
                exit(EXIT_FAILURE);
            }

            shared_ptr<STNode> v = stack[stack.size() - 1];
            if (v->getType() != "Truth Value")
            {
                cout << "Error: Expected truth value." << endl;
                exit(EXIT_FAILURE);
            }

            shared_ptr<TruthValue> tv = dynamic_pointer_cast<TruthValue>(v);

            shared_ptr<Beta> beta = dynamic_pointer_cast<Beta>(next);
            if (control.size() < 3)
            {
                cout << "Error: Control underflow." << endl;
                exit(EXIT_FAILURE);
            }

            shared_ptr<STNode> _next_1 = control[control.size() - 1];
            shared_ptr<STNode> _next_2 = control[control.size() - 2];
            control.pop_back();
            control.pop_back();

            if (_next_1->getType() != "Delta" || _next_2->getType() != "Delta")
            {
                cout << "Error: Expected delta." << endl;
                exit(EXIT_FAILURE);
            }

            shared_ptr<Delta> delta_else = dynamic_pointer_cast<Delta>(_next_1);
            shared_ptr<Delta> delta_then = dynamic_pointer_cast<Delta>(_next_2);

            int delta_index;
            if (tv->getValue())
            {
                delta_index = delta_then->getIndex();
            }
            else
            {
                delta_index = delta_else->getIndex();
            }

            control.insert(control.end(), controlStructures[delta_index].begin(), controlStructures[delta_index].end());
            continue;
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
