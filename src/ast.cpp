#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include "ast.h"

using namespace std;

/**
 * @brief Return the level of a token from the number of preceding dots
 * @param str The token to check
 * @return The level of the token
 */
int getLevel(string str)
{
    int level = 0;
    for (int i = 0; i < (int)str.length(); i++)
    {
        if (str[i] == '.')
        {
            level++;
        }
        else
        {
            break;
        }
    }
    return level;
}

/**
 * @brief Get the type of a token
 * @param str The token to check
 * @param level The level of the token
 * @return A pair of type and value
 */
pair<Type, string> getType(string str, int level)
{
    str = str.substr(level, str.length() - level);
    if (str.rfind("<ID:", level - 1) == 0)
    {
        string identifier = str.substr(4, str.length() - 5);
        return make_pair(IDENTIFIER, identifier);
    }
    else if (str.rfind("<INT:", level - 1) == 0)
    {
        string integer = str.substr(5, str.length() - 6);
        return make_pair(INTEGER, integer);
    }
    else if (str.rfind("<STR:", level - 1) == 0)
    {
        string _string = str.substr(6, str.length() - 8);
        return make_pair(STRING, _string);
    }
    else
    {
        return make_pair(OTHER, str);
    }
}

bool ASTNode::isLeaf() const
{
    return children.size() == 0;
}

void ASTNode::addChild(shared_ptr<ASTNode> child)
{
    children.push_back(child);
}

ostream &operator<<(ostream &os, const ASTNode &node)
{
    if (node.type == IDENTIFIER)
    {
        os << "<ID:" << node.value << ">";
    }
    else if (node.type == INTEGER)
    {
        os << "<INT:" << node.value << ">";
    }
    else if (node.type == STRING)
    {
        os << "<STR:'" << node.value << "'>";
    }
    else
    {
        os << node.value;
    }
    return os;
}

shared_ptr<AST> AST::createAST(vector<string> tokens)
{
    shared_ptr<AST> ast = make_shared<AST>();

    if (ast == nullptr)
    {
        cout << "Error: Could not create AST\n";
        return nullptr;
    }

    int currentLevel = 0;
    weak_ptr<ASTNode> currentParent = ast->root;
    for (int i = 0; i < (int)tokens.size(); ++i)
    {
        shared_ptr<ASTNode> node = make_shared<ASTNode>();
        node->parent = currentParent;

        if (node == nullptr)
        {
            cout << "Error: Could not create ASTNode\n";
            return nullptr;
        }

        string token = tokens[i];
        int level = getLevel(token);
        auto type = getType(token, level);
        node->type = type.first;
        node->value = type.second;

        if (i == 0)
        {
            // Root node
            node->parent = node;
            ast->root = node;
            currentParent = ast->root;
            ++ast->size;
            continue;
        }

        if (level == currentLevel + 1)
        {
            // A child of the previous node
            currentLevel = level;
            if (ast->size > 1)
                currentParent = currentParent.lock()->children.back(); // Get the previous node from its parent
        }
        else if (level < currentLevel)
        {
            // A sibling of a previous node
            for (int j = 0; j < currentLevel - level; j++)
            {
                // traverse up the tree to the correct level
                currentParent = currentParent.lock()->parent;
            }
            currentLevel = level;
        }

        // Add the node to the tree
        node->parent = currentParent;
        currentParent.lock()->addChild(node);
        ++ast->size;
    }

    return ast;
}

AST::AST()
{
    root = nullptr;
    size = 0;
}

bool AST::isEmpty() const
{
    return size == 0;
}

void AST::preOrder(shared_ptr<ASTNode> node, int level, ostream &os) const
{
    if (node == nullptr)
    {
        return;
    }

    for (int i = 0; i < level; i++)
    {
        os << ".";
    }

    os << *node << "\n";

    for (int i = 0; i < (int)node->children.size(); i++)
    {
        preOrder(node->children[i], level + 1, os);
    }
}

ostream &operator<<(ostream &os, const AST &ast)
{
    if (ast.root == nullptr)
    {
        os << "Error: AST is empty\n";
        return os;
    }

    ast.preOrder(ast.root, 0, os);
    return os;
}

void AST::printAST() const
{
    cout << *this;
}
