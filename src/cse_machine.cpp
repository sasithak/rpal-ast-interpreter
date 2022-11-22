#include <iostream>
#include <iomanip>
#include "environment.h"
#include "operators.h"
#include "st.h"
#include "st_types.h"

using namespace std;

shared_ptr<STNode> apply(shared_ptr<UnaryOperator> unOp, shared_ptr<STNode> rand);
shared_ptr<STNode> apply(shared_ptr<BinaryOperator> binOp, shared_ptr<STNode> rand_l, shared_ptr<STNode> rand_r);
shared_ptr<STNode> apply(shared_ptr<Function> op, shared_ptr<STNode> rand);
void applyErr(shared_ptr<UnaryOperator> unOp, shared_ptr<STNode> rand);
void applyErr(shared_ptr<BinaryOperator> binOp, shared_ptr<STNode> rand_l, shared_ptr<STNode> rand_r);
shared_ptr<STNode> lookup(string name, shared_ptr<Environment> env);
void stackUflowErr();

void ST::runCSEMachine(vector<vector<shared_ptr<STNode>>> &controlStructures, ostream &out)
{
    vector<shared_ptr<STNode>> stack;
    vector<shared_ptr<STNode>> control;
    vector<shared_ptr<Environment>> envs;

    shared_ptr<Environment> e_0 = make_shared<Environment>();
    stack.push_back(e_0);
    control.push_back(e_0);
    envs.push_back(e_0);
    control.insert(control.end(), controlStructures[0].begin(), controlStructures[0].end());

    shared_ptr<Environment> currentEnvironment = e_0;
    while (true)
    {
        if (control.empty())
        {
            break;
        }

        out << setw(8) << "Control"
            << ": ";
        for (int i = 0; i < (int)control.size(); ++i)
        {
            auto node = control[i];
            out << (node->getType() == "String" ? ("'" + node->toString() + "'") : node->toString()) << (i == (int)control.size() - 1 ? "\n" : " ");
        }

        out << setw(8) << "Stack"
            << ": ";
        for (int i = stack.size() - 1; i >= 0; --i)
        {
            auto node = stack[i];
            out << (node->getType() == "String" ? ("'" + node->toString() + "'") : node->toString()) << (i == 0 ? "\n" : " ");
        }

        int controlSize = control.size();
        shared_ptr<STNode> next = control[controlSize - 1];
        control.pop_back();

        out << setw(8) << "Next"
            << ": " << (next->getType() == "String" ? ("'" + next->toString() + "'") : next->toString()) << "\n";

        // CSE Rule 1
        if (next->getType() == "Identifier")
        {
            string name = dynamic_pointer_cast<Identifier>(next)->getName();
            shared_ptr<STNode> value = lookup(name, currentEnvironment);

            if (value == nullptr)
            {
                cerr << "Error: Identifier " << name << " is not defined.\n";
                exit(EXIT_FAILURE);
            }
            else if (value->getType() == "Function")
            {
                value = dynamic_pointer_cast<Function>(value)->getCopy();
            }

            stack.push_back(value);
            out << setw(8) << "Rule"
                << ": " << 1 << "\n\n";
            continue;
        }

        // CSE Rule 2
        if (next->getType() == "Lambda")
        {
            shared_ptr<Lambda> l = dynamic_pointer_cast<Lambda>(next)->getCopy();
            l->setEnv(currentEnvironment->getIndex());
            stack.push_back(l);
            out << setw(8) << "Rule"
                << ": " << 2 << "\n\n";
            continue;
        }

        if (next->getType() == "Gamma")
        {
            if (stack.size() < 3)
            {
                stackUflowErr();
            }

            shared_ptr<STNode> rator = stack[stack.size() - 1];
            shared_ptr<STNode> rand = stack[stack.size() - 2];

            if (rator->getType() == "Environment" || rand->getType() == "Environment")
            {
                stackUflowErr();
            }

            stack.pop_back();
            stack.pop_back();

            // CSE Rule 3
            if (rator->getType() == "Function")
            {
                shared_ptr<STNode> result = apply(dynamic_pointer_cast<Function>(rator), rand);
                stack.push_back(result);
                out << setw(8) << "Rule"
                    << ": " << 3 << "\n\n";
                continue;
            }

            // CSE Rule 3 & CSE Rule 4 & CSE Rule 11
            if (rator->getType() == "Lambda")
            {
                shared_ptr<Lambda> l = dynamic_pointer_cast<Lambda>(rator);
                shared_ptr<Environment> newEnv = make_shared<Environment>();

                newEnv->setParent(envs[l->getEnv()]);
                out << setw(8) << "New Env"
                    << ": " << newEnv->getIndex() << "\n";

                envs.push_back(newEnv);

                int bindingCnt = l->getBindingCount();
                auto bindings = l->getBindings();

                if (bindingCnt > 1)
                {
                    // CSE Rule 11
                    if (l->isComma())
                    {
                        if (rand->getType() == "Tuple")
                        {

                            shared_ptr<Tuple> t = dynamic_pointer_cast<Tuple>(rand);
                            int order = t->getOrder();

                            if (order != bindingCnt)
                            {
                                cerr << "Error: Expected " << l->getBindingCount() << " arguments but got " << order << ".\n";
                                exit(EXIT_FAILURE);
                            }

                            out << setw(8) << "Bindings"
                                << ": ";
                            for (int i = 0; i < bindingCnt; ++i)
                            {
                                string name = dynamic_pointer_cast<Identifier>(bindings[i])->getName();
                                newEnv->addVariable(name, (*t)[i]);
                                out << "(" << name << " = " << (*t)[i]->toString() << ")" << (i == bindingCnt - 1 ? "\n" : ", ");
                            }

                            out << setw(8) << "Rule"
                                << ": " << 11 << "\n\n";
                        }
                        else
                        {
                            cerr << "Error: Expected " << l->getBindingCount() << " arguments but got 1.\n";
                            exit(EXIT_FAILURE);
                        }
                    }
                    else
                    {
                        // CSE Rule 3
                        shared_ptr<IncompleteLambda> il = make_shared<IncompleteLambda>(l);
                        il->addArgument(rand);
                        stack.push_back(il);

                        out << setw(8) << "Rule"
                            << ": " << 3 << "\n\n";
                        continue;
                    }
                }
                else
                {
                    // CSE Rule 4
                    out << setw(8) << "Bindings"
                        << ": ";
                    string name = dynamic_pointer_cast<Identifier>(bindings[0])->getName();
                    newEnv->addVariable(name, rand);
                    out << "(" << name << " = " << rand->toString() << ")\n";
                    out << setw(8) << "Rule"
                        << ": " << 4 << "\n\n";
                }

                currentEnvironment = newEnv;
                control.push_back(newEnv);
                stack.push_back(newEnv);
                vector<shared_ptr<STNode>> _delta = controlStructures[l->getIndex()];
                control.insert(control.end(), _delta.begin(), _delta.end());
                continue;
            }

            // CSE Rule 3 & CSE Rule 4
            if (rator->getType() == "IncompleteLambda")
            {
                shared_ptr<IncompleteLambda> il = dynamic_pointer_cast<IncompleteLambda>(rator);
                il->addArgument(rand);

                if (il->isComplete())
                {
                    // CSE Rule 4
                    shared_ptr<Environment> newEnv = make_shared<Environment>();
                    newEnv->setParent(envs[il->getEnv()]);
                    envs.push_back(newEnv);

                    out << setw(8) << "Bindings"
                        << ": ";

                    auto arguments = il->getArguments();
                    for (int i = 0; i < il->getBindingCount(); ++i)
                    {
                        string name = dynamic_pointer_cast<Identifier>(il->getBindings()[i])->getName();
                        newEnv->addVariable(name, arguments[i]);
                        out << "(" << name << " = " << arguments[i]->toString() << ")" << (i == il->getBindingCount() - 1 ? "\n" : ", ");
                    }

                    currentEnvironment = newEnv;
                    control.push_back(newEnv);
                    stack.push_back(newEnv);
                    vector<shared_ptr<STNode>> _delta = controlStructures[il->getIndex()];
                    control.insert(control.end(), _delta.begin(), _delta.end());

                    out << setw(8) << "Rule"
                        << ": " << 4 << "\n\n";
                }
                else
                {
                    // CSE Rule 3
                    out << setw(8) << "Rule"
                        << ": " << 3 << "\n\n";
                    stack.push_back(il);
                }
                continue;
            }

            // CSE Rule 10
            if (rator->getType() == "Tuple")
            {
                shared_ptr<Tuple> t = dynamic_pointer_cast<Tuple>(rator);

                if (rand->getType() != "Integer")
                {
                    cerr << "Error: Tuple index must be an integer.\n";
                    exit(EXIT_FAILURE);
                }

                int index = dynamic_pointer_cast<Integer>(rand)->getValue() - 1;
                shared_ptr<STNode> value = (*t)[index];
                if (value == nullptr)
                {
                    cerr << "Error: Tuple index out of range.\n";
                    exit(EXIT_FAILURE);
                }

                stack.push_back(value);
                out << setw(8) << "Rule"
                    << ": " << 10 << "\n\n";
                continue;
            }

            // CSE Rule 11
            if (rator->getType() == "YStar")
            {
                shared_ptr<YStar> y = dynamic_pointer_cast<YStar>(rator);

                if (rand->getType() != "Lambda")
                {
                    cerr << "Error: Recursion Error.\n";
                    exit(EXIT_FAILURE);
                }

                shared_ptr<Lambda> l = dynamic_pointer_cast<Lambda>(rand);
                shared_ptr<Eta> e = make_shared<Eta>(l);
                stack.push_back(e);
                out << setw(8) << "Rule"
                    << ": " << 11 << "\n\n";
                continue;
            }

            // CSE Rule 12
            if (rator->getType() == "Eta")
            {
                shared_ptr<Eta> e = dynamic_pointer_cast<Eta>(rator);
                shared_ptr<Lambda> l = e->getLambda();

                stack.push_back(rand);
                stack.push_back(e);
                stack.push_back(l);
                control.push_back(make_shared<Gamma>());
                control.push_back(make_shared<Gamma>());
                out << setw(8) << "Rule"
                    << ": " << 12 << "\n\n";
                continue;
            }

            cerr << "Error: Illegal Function Application.\n";
            exit(EXIT_FAILURE);
        }

        // CSE Rule 5
        if (next->getType() == "Environment")
        {
            if (stack.size() < 2)
            {
                stackUflowErr();
            }

            shared_ptr<STNode> v = stack[stack.size() - 1];
            shared_ptr<STNode> e = stack[stack.size() - 2];

            if (v->getType() == "Environment")
            {
                stackUflowErr();
            }

            if (e->getType() != "Environment")
            {
                cerr << "Error: Expected environment.\n";
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

            out << setw(8) << "Rule"
                << ": " << 5 << "\n\n";
            continue;
        }

        // CSE Rule 6
        if (next->getType() == "BinaryOperator")
        {
            if (stack.size() < 3)
            {
                stackUflowErr();
            }

            shared_ptr<BinaryOperator> binOp = dynamic_pointer_cast<BinaryOperator>(next);
            shared_ptr<STNode> rand_l = stack[stack.size() - 1];
            shared_ptr<STNode> rand_r = stack[stack.size() - 2];

            if (rand_l->getType() == "Environment" || rand_r->getType() == "Environment")
            {
                stackUflowErr();
            }

            stack.pop_back();
            stack.pop_back();

            shared_ptr<STNode> result = apply(binOp, rand_l, rand_r);
            stack.push_back(result);
            out << setw(8) << "Rule"
                << ": " << 6 << "\n\n";
            continue;
        }

        // CSE Rule 7
        if (next->getType() == "UnaryOperator")
        {
            if (stack.size() < 2)
            {
                stackUflowErr();
            }

            shared_ptr<UnaryOperator> unOp = dynamic_pointer_cast<UnaryOperator>(next);
            shared_ptr<STNode> rand = stack[stack.size() - 1];

            if (rand->getType() == "Environment")
            {
                stackUflowErr();
            }

            stack.pop_back();

            shared_ptr<STNode> result = apply(unOp, rand);
            stack.push_back(result);
            out << setw(8) << "Rule"
                << ": " << 7 << "\n\n";
            continue;
        }

        // CSE Rule 8
        if (next->getType() == "Beta")
        {
            if (stack.size() < 2)
            {
                stackUflowErr();
            }

            shared_ptr<STNode> v = stack[stack.size() - 1];

            if (v->getType() == "Environment")
            {
                stackUflowErr();
            }

            if (v->getType() != "TruthValue")
            {
                cerr << "Error: Expected truth value.\n";
                exit(EXIT_FAILURE);
            }

            shared_ptr<TruthValue> tv = dynamic_pointer_cast<TruthValue>(v);
            stack.pop_back();

            shared_ptr<Beta>
                beta = dynamic_pointer_cast<Beta>(next);
            if (control.size() < 3)
            {
                cerr << "Error: Control underflow.\n";
                exit(EXIT_FAILURE);
            }

            shared_ptr<STNode> _next_1 = control[control.size() - 1];
            shared_ptr<STNode> _next_2 = control[control.size() - 2];
            control.pop_back();
            control.pop_back();

            if (_next_1->getType() != "Delta" || _next_2->getType() != "Delta")
            {
                cerr << "Error: Expected delta.\n";
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
            out << setw(8) << "Rule"
                << ": " << 8 << "\n\n";
            continue;
        }

        // CSE Rule 9
        if (next->getType() == "Tau")
        {
            shared_ptr<Tau> tau = dynamic_pointer_cast<Tau>(next);
            int n = tau->getSize();

            if ((int)stack.size() <= n)
            {
                stackUflowErr();
            }

            shared_ptr<Tuple> tuple = make_shared<Tuple>();
            for (int i = 0; i < n; ++i)
            {
                auto elem = stack[stack.size() - 1];

                if (elem->getType() == "Environment")
                {
                    stackUflowErr();
                }

                tuple->push_back(elem);
                stack.pop_back();
            }

            stack.push_back(tuple);
            out << setw(8) << "Rule"
                << ": " << 9 << "\n\n";
            continue;
        }

        out << setw(8) << "Rule"
            << ": 1\n\n";

        if (next->getType() == "Tuple")
        {
            stack.push_back(dynamic_pointer_cast<Tuple>(next)->getCopy());
            continue;
        }

        stack.push_back(next);
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
            applyErr(unOp, rand);
            exit(EXIT_FAILURE);
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
            applyErr(unOp, rand);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        cerr << "Error: Unknown unary operator.\n";
        exit(EXIT_FAILURE);
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
            applyErr(binOp, rand_l, rand_r);
            exit(EXIT_FAILURE);
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
            applyErr(binOp, rand_l, rand_r);
            exit(EXIT_FAILURE);
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
            applyErr(binOp, rand_l, rand_r);
            exit(EXIT_FAILURE);
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
            applyErr(binOp, rand_l, rand_r);
            exit(EXIT_FAILURE);
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
            applyErr(binOp, rand_l, rand_r);
            exit(EXIT_FAILURE);
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
            applyErr(binOp, rand_l, rand_r);
            exit(EXIT_FAILURE);
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
            applyErr(binOp, rand_l, rand_r);
            exit(EXIT_FAILURE);
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
            applyErr(binOp, rand_l, rand_r);
            exit(EXIT_FAILURE);
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
            applyErr(binOp, rand_l, rand_r);
            exit(EXIT_FAILURE);
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
            applyErr(binOp, rand_l, rand_r);
            exit(EXIT_FAILURE);
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
            applyErr(binOp, rand_l, rand_r);
            exit(EXIT_FAILURE);
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
            applyErr(binOp, rand_l, rand_r);
            exit(EXIT_FAILURE);
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
            applyErr(binOp, rand_l, rand_r);
            exit(EXIT_FAILURE);
        }
    }
    else if (binOpStr == "ne")
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
            applyErr(binOp, rand_l, rand_r);
            exit(EXIT_FAILURE);
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
            applyErr(binOp, rand_l, rand_r);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        cerr << "Error: Unknown binary operator " << binOpStr << "\n";
        exit(EXIT_FAILURE);
    }
}

shared_ptr<STNode> apply(shared_ptr<Function> op, shared_ptr<STNode> rand)
{
    op->addArgument(rand);

    if (!op->isFull())
    {
        return op;
    }

    string opStr = op->toString();
    vector<shared_ptr<STNode>> rands = op->getArguments();

    if (opStr == "Print")
    {
        string randStr = rands[0]->toString();
        cout << randStr << "\n";
        return make_shared<Dummy>(randStr);
    }

    if (opStr == "Stern")
    {
        if (rands[0]->getType() == "String")
        {
            return dynamic_pointer_cast<String>(rands[0])->stern();
        }
        else
        {
            cerr << "Stern: Argument is not a string\n";
            exit(EXIT_FAILURE);
        }
    }

    if (opStr == "Stem")
    {
        if (rands[0]->getType() == "String")
        {
            return dynamic_pointer_cast<String>(rands[0])->stem();
        }
        else
        {
            cerr << "Stem: Argument is not a string\n";
            exit(EXIT_FAILURE);
        }
    }

    if (opStr == "Conc")
    {
        if (rands[0]->getType() == "String" && rands[1]->getType() == "String")
        {
            return (*dynamic_pointer_cast<String>(rands[0])) + dynamic_pointer_cast<String>(rands[1]);
        }
        else
        {
            cerr << "Conc: Arguments are not strings\n";
            exit(EXIT_FAILURE);
        }
    }

    if (opStr == "Order")
    {
        if (rands[0]->getType() == "Tuple")
        {
            return make_shared<Integer>(dynamic_pointer_cast<Tuple>(rands[0])->getOrder());
        }
        else
        {
            cerr << "Order: Argument is not a tuple\n";
            exit(EXIT_FAILURE);
        }
    }

    if (opStr == "Null")
    {
        if (rands[0]->getType() == "Tuple")
        {
            return make_shared<TruthValue>(dynamic_pointer_cast<Tuple>(rands[0])->getOrder() == 0);
        }
        else
        {
            cerr << "Null: Argument is not a tuple\n";
            exit(EXIT_FAILURE);
        }
    }

    if (opStr == "Isinteger")
    {
        if (rands[0]->getType() == "Integer")
        {
            return make_shared<TruthValue>(true);
        }
        else
        {
            return make_shared<TruthValue>(false);
        }
    }

    if (opStr == "Isstring")
    {
        if (rands[0]->getType() == "String")
        {
            return make_shared<TruthValue>(true);
        }
        else
        {
            return make_shared<TruthValue>(false);
        }
    }

    if (opStr == "Istruthvalue")
    {
        if (rands[0]->getType() == "TruthValue")
        {
            return make_shared<TruthValue>(true);
        }
        else
        {
            return make_shared<TruthValue>(false);
        }
    }

    if (opStr == "Isfunction")
    {
        string type = rands[0]->getType();
        if (type == "Function" || type == "UnaryOperator" || type == "BinaryOperator" || type == "Lambda")
        {
            return make_shared<TruthValue>(true);
        }
        else
        {
            return make_shared<TruthValue>(false);
        }
    }

    if (opStr == "Istuple")
    {
        if (rands[0]->getType() == "Tuple")
        {
            return make_shared<TruthValue>(true);
        }
        else
        {
            return make_shared<TruthValue>(false);
        }
    }

    if (opStr == "Isdummy")
    {
        if (rands[0]->getType() == "Dummy")
        {
            return make_shared<TruthValue>(true);
        }
        else
        {
            cerr << "Isempty: Argument is not a tuple\n";
            exit(EXIT_FAILURE);
        }
    }

    cerr << "Error: Unknown function " << opStr << "\n";
    exit(EXIT_FAILURE);
}

void applyErr(shared_ptr<UnaryOperator> unOp, shared_ptr<STNode> rand)
{
    cerr << "Error: Operator " << unOp->toString() << " is not defined for " << rand->getType() << "\n";
}

void applyErr(shared_ptr<BinaryOperator> binOp, shared_ptr<STNode> rand_l, shared_ptr<STNode> rand_r)
{
    cerr << "Error: Operator " << binOp->toString() << " is not defined for " << rand_l->getType() << " and " << rand_r->getType() << "\n";
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

void stackUflowErr()
{
    cerr << "Error: Stack underflow\n";
    exit(EXIT_FAILURE);
}
