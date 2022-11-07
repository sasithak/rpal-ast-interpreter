#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <iostream>
#include <memory>
#include <unordered_map>
#include "st_types.h"

class Environment
{
public:
    Environment();
    int getIndex() const;
    std::shared_ptr<STNode> getVariable(std::shared_ptr<Identifier> key) const;
    bool addVariable(std::shared_ptr<Identifier> key, std::shared_ptr<STNode> value);
    std::vector<std::shared_ptr<Environment>> getChildren() const;
    void addChild(std::shared_ptr<Environment> child);
    std::shared_ptr<Environment> getParent() const;
    std::string toString() const;

private:
    int index;
    std::unordered_map<std::shared_ptr<Identifier>, std::shared_ptr<STNode>> bindings;
    std::vector<std::shared_ptr<Environment>> children;
    std::shared_ptr<Environment> parent;
    static int nextIndex;
};

#endif