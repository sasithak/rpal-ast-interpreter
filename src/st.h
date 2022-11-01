#include <iostream>
#include <memory>
#include <vector>
#include "ast.h"
#include "st_types.h"

class ST
{
public:
    ST(std::shared_ptr<STNode> root);
    void execute();

private:
    std::shared_ptr<STNode> root;
};
