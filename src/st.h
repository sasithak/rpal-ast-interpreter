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

private:
    std::shared_ptr<STNode> root;
};

#endif // ST_H
