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

    /**
     * @brief Get the index of the environment
     * @return The index
     */
    int getIndex() const;

    /**
     * @brief Get the value of a variable
     * @param key The name of the identifier
     * @return The value bound to the identifier
     */
    std::shared_ptr<STNode> getVariable(std::string key) const;

    /**
     * @brief Add a variable to the environment
     * @param key The name of the identifier
     * @param value The value to bind to the identifier
     * @return True if the variable was added, false if it already exists
     */
    bool addVariable(std::string key, std::shared_ptr<STNode> value);

    /**
     * @brief Get the child environments
     * @return A vector containing child environments
     */
    std::vector<std::shared_ptr<Environment>> getChildren() const;

    /**
     * @brief Get the parent of the environment
     * @return The parent environment
     */
    std::shared_ptr<Environment> getParent() const;

    /**
     * @brief Set the parent of the environment
     * @param parent The parent environment
     */
    void setParent(std::shared_ptr<Environment> parent);

    std::string toString() const override;
    std::string getType() const override;

private:
    int index;
    std::unordered_map<std::string, std::shared_ptr<STNode>> bindings;
    std::shared_ptr<Environment> parent;
    static int nextIndex;
};

/**
 * @brief Setup the primitive environment
 * @param primitiveEnvironment A map to store the variables in
 */
void setupPrimitiveEnvironment(std::unordered_map<std::string, std::shared_ptr<STNode>> &primitiveEnvironment);

/**
 * @brief Get the bound value of a variable
 * @param name The name of the variable
 * @param env The environment to start search
 * @return nullptr if the variable is not found, otherwise the bound value
 */
std::shared_ptr<STNode> lookup(std::string name, std::shared_ptr<Environment> env);

#endif
