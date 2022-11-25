#ifndef AST_H
#define AST_H

#include <iostream>
#include <memory>
#include <vector>
#include "st.h"
#include "st_types.h"

enum Type
{
    IDENTIFIER,
    INTEGER,
    STRING,
    OTHER,
};

struct ASTNode
{
    Type type;
    std::string value;
    std::weak_ptr<ASTNode> parent;
    std::vector<std::shared_ptr<ASTNode>> children;

    /**
     * @brief Check if this node is a leaf node
     * @return true if this node is a leaf node, false otherwise
     */
    bool isLeaf() const;

    /**
     * @brief Add a child to this node
     * @param child The child to add
     */
    void addChild(std::shared_ptr<ASTNode> child);

    /**
     * @brief Standardize the node according to the semantics
     * @param children A vector containing the standardized children of this node
     * @return The standardized node
     */
    std::shared_ptr<STNode> standardize(std::vector<std::shared_ptr<STNode>> children) const;

    friend std::ostream &operator<<(std::ostream &os, const ASTNode &node);
};

class AST
{
public:
    /**
     * @brief Construct a new AST object
     * @param tokens A vector containing the tokens to parse
     * @return A new AST object
     */
    static std::shared_ptr<AST> createAST(std::vector<std::string> tokens);

    AST();

    /**
     * @brief Check whether the AST is empty
     * @return true if the AST is empty, false otherwise
     */
    bool isEmpty() const;

    /**
     * @brief Print the AST
     */
    void printAST() const;

    /**
     * @brief Standardize the AST according to the semantics
     * @return The standardized tree
     */
    std::shared_ptr<ST> standardize() const;

    friend std::ostream &operator<<(std::ostream &os, const AST &ast);

private:
    /**
     * @brief Traverse the AST in preorder and print the tree
     * @param node The node to print
     * @param level The depth of the node
     * @param os The output stream to print to
     */
    void preOrder(std::shared_ptr<ASTNode> node, int level, std::ostream &os) const;

    /**
     * @brief Standardize the AST in postorder
     * @param node The node to standardize
     * @param level The depth of the node
     * @return The standardized node
     */
    std::shared_ptr<STNode> postOrder(std::shared_ptr<ASTNode> node, int level) const;

    std::shared_ptr<ASTNode> root;
    int size;
};

#endif // AST_H
