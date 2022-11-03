#ifndef ST_H
#define ST_H

#include <iostream>
#include <memory>
#include <vector>
#include "st_types.h"

class ST
{
public:
    ST(std::shared_ptr<STNode> root);
    void execute();
    void printST();
    friend std::ostream &operator<<(std::ostream &os, const ST &st);

private:
    std::shared_ptr<STNode> root;
    void preOrder(std::shared_ptr<STNode> node, int level, std::ostream &os) const;
    void preOrder(std::shared_ptr<STNode> node, std::vector<std::shared_ptr<STNode>> &controlStructure, std::vector<std::shared_ptr<Delta>> &deltas) const;
};

#endif // ST_H
