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

    /**
     * @brief Generate the control structures and start the execution of the CSE Machine
     * @param printCS Whether to print the control structures
     * @param printExe Whether to print the execution of the CSE Machine
     */
    void execute(bool printCS, bool printExe);

    /**
     * @brief Print the ST to stdout
     */
    void printST();

    friend std::ostream &operator<<(std::ostream &os, const ST &st);

private:
    std::shared_ptr<STNode> root;

    /**
     * @brief Run the CSE machine according to the CSE Rules
     * @param controlStructures A 2D vector containing the control structures
     * @param printExe Whether to print the execution of the CSE Machine
     */
    void runCSEMachine(std::vector<std::vector<std::shared_ptr<STNode>>> &controlStructures, bool printExe);

    /**
     * @brief Traverse the ST in preorder and print the tree
     * @param node The node to print
     * @param level The depth of the node
     * @param os The output stream to print to
     */
    void preOrder(std::shared_ptr<STNode> node, int level, std::ostream &os) const;

    /**
     * @brief Traverse the ST in preorder and generate control structures
     * @param node The node to process
     * @param controlStructure A reference to a vector containing the control structures of current subtree
     * @param deltas A reference to a vector containing the identified delta nodes
     */
    void preOrder(std::shared_ptr<STNode> node, std::vector<std::shared_ptr<STNode>> &controlStructure, std::vector<std::shared_ptr<Delta>> &deltas) const;
};

#endif // ST_H
