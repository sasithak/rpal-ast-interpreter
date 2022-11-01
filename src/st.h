#include <iostream>
#include <memory>
#include <vector>
#include "ast.h"
#include "st_types.h"

class ST
{
public:
    ST(std::shared_ptr<AST> ast);
    void execute();

private:
    STNode *root;
};
