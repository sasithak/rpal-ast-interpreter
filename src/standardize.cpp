#include <iostream>
#include "ast.h"
#include "st.h"

using namespace std;

ST::ST(std::shared_ptr<AST> ast)
{
    root = nullptr;
}
