#include <iostream>
#include <memory>
#include <unordered_map>
#include "environment.h"
#include "st_types.h"

using namespace std;

int Environment::nextIndex = 0;

Environment::Environment()
{
    this->index = nextIndex++;
    if (this->index == 0)
    {
        this->parent = nullptr;
        getPrimitiveEnvironment(this->bindings);
    }
}

int Environment::getIndex() const
{
    return index;
}

shared_ptr<STNode> Environment::getVariable(string key) const
{
    if (bindings.find(key) != bindings.end())
    {
        return nullptr;
    }

    return bindings.at(key);
}

bool Environment::addVariable(string key, shared_ptr<STNode> value)
{
    if (bindings.find(key) != bindings.end())
    {
        return false;
    }
    bindings[key] = value;
    return true;
}

shared_ptr<Environment> Environment::getParent() const
{
    return parent;
}

void Environment::setParent(shared_ptr<Environment> parent)
{
    this->parent = parent;
}

string Environment::toString() const
{
    return "e_" + to_string(index);
}

string Environment::getType() const
{
    return "Environment";
}

void getPrimitiveEnvironment(unordered_map<string, shared_ptr<STNode>> &primitiveEnvironment)
{
    primitiveEnvironment.insert({"Print", make_shared<Function>("Print", 1)});
    primitiveEnvironment.insert({"Stern", make_shared<Function>("Stern", 1)});
    primitiveEnvironment.insert({"Stem", make_shared<Function>("Stem", 1)});
    primitiveEnvironment.insert({"Conc", make_shared<Function>("Conc", 2)});
    primitiveEnvironment.insert({"Order", make_shared<Function>("Order", 1)});
    primitiveEnvironment.insert({"Null", make_shared<Function>("Null", 1)});
    primitiveEnvironment.insert({"Isinteger", make_shared<Function>("Isinteger", 1)});
    primitiveEnvironment.insert({"Isstring", make_shared<Function>("Isstring", 1)});
    primitiveEnvironment.insert({"Istruthvalue", make_shared<Function>("Istruthvalue", 1)});
    primitiveEnvironment.insert({"Isfunction", make_shared<Function>("Isfunction", 1)});
    primitiveEnvironment.insert({"Istuple", make_shared<Function>("Istuple", 1)});
    primitiveEnvironment.insert({"Isdummy", make_shared<Function>("Isdummy", 1)});
}
