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
    if (nextIndex == 1)
    {
        this->parent = nullptr;
        this->bindings = getPrimitiveEnvironment();
    }
}

int Environment::getIndex() const
{
    return index;
}

shared_ptr<STNode> Environment::getVariable(string key) const
{
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

vector<shared_ptr<Environment>> Environment::getChildren() const
{
    return children;
}

void Environment::addChild(shared_ptr<Environment> child)
{
    children.push_back(child);
}

shared_ptr<Environment> Environment::getParent() const
{
    return parent;
}

string Environment::toString() const
{
    return "e_" + to_string(index);
}

unordered_map<string, shared_ptr<STNode>> getPrimitiveEnvironment()
{
    unordered_map<string, shared_ptr<STNode>> primitiveEnvironment;
    primitiveEnvironment.insert({"Print", make_shared<Function>("Print", 1)});
    primitiveEnvironment.insert({"Stern", make_shared<Function>("Stern", 1)});
    primitiveEnvironment.insert({"Stem", make_shared<Function>("Stem", 1)});
    primitiveEnvironment.insert({"Conc", make_shared<Function>("Conc", 2)});
    primitiveEnvironment.insert({"Order", make_shared<Function>("Order", 1)});
    primitiveEnvironment.insert({"Null", make_shared<Function>("Null", 1)});

    return primitiveEnvironment;
}
