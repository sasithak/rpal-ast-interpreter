#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <iostream>
#include <memory>
#include <unordered_map>
#include "st_types.h"

class Environment : public STNode
{
public:
    Environment();
    int getIndex() const;
    std::shared_ptr<STNode> getVariable(std::string key) const;
    bool addVariable(std::string key, std::shared_ptr<STNode> value);
    std::vector<std::shared_ptr<Environment>> getChildren() const;
    std::shared_ptr<Environment> getParent() const;
    std::string toString() const override;
    std::string getType() const override;

private:
    int index;
    std::unordered_map<std::string, std::shared_ptr<STNode>> bindings;
    std::shared_ptr<Environment> parent;
    static int nextIndex;
};

void getPrimitiveEnvironment(std::unordered_map<std::string, std::shared_ptr<STNode>> &primitiveEnvironment);

#endif