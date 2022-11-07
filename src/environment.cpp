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
}

int Environment::getIndex() const
{
    return index;
}

shared_ptr<STNode> Environment::getVariable(shared_ptr<Identifier> key) const
{
    return bindings.at(key);
}

bool Environment::addVariable(shared_ptr<Identifier> key, shared_ptr<STNode> value)
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
