#ifndef AST_H
#define AST_H

#include <iostream>
#include <memory>
#include <vector>

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

    bool isLeaf() const;
    void addChild(std::shared_ptr<ASTNode> child);
    friend std::ostream &operator<<(std::ostream &os, const ASTNode &node);
};

class AST
{
public:
    static std::shared_ptr<AST> createAST(std::vector<std::string> tokens);
    AST();
    bool isEmpty() const;
    void printAST() const;
    friend std::ostream &operator<<(std::ostream &os, const AST &ast);

private:
    void preOrder(std::shared_ptr<ASTNode> node, int level, std::ostream &os) const;

    std::shared_ptr<ASTNode> root;
    int size;
};

#endif // AST_H
