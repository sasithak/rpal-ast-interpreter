#ifndef OPERATORS_H
#define OPERATORS_H

#include <iostream>

/**
 * @brief Identify whether a unary operator by name
 * @param op The name of the operator
 * @return true if the name refers to a unary operator, false otherwise
 */
bool isUnOp(std::string op);

/**
 * @brief Identify whether a binary operator by name
 * @param op The name of the operator
 * @return true if the name refers to a binary operator, false otherwise
 */
bool isBinOp(std::string op);

#endif // OPERATORS_H
