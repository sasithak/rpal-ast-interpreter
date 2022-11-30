#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "ast.h"

using namespace std;

/**
 * @brief Get the tokens from the input file
 * @param filename The name of the input file
 * @return A vector of tokens
 */
vector<string> getTokens(string filename);

/**
 * @brief Remove the trailing spaces from a string
 * @param str The string to remove the leading and trailing spaces from
 * @return The string without leading and trailing spaces
 */
string trim(const string &str);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "No input file specified\n";
        return 1;
    }

    if (argc > 6)
    {
        cerr << "Too many arguments\n";
        return 1;
    }

    bool printAST = false;
    bool printST = false;
    bool printCS = false;
    bool printExe = false;

    for (int i = 1; i < argc - 1; ++i)
    {
        string arg = argv[i];
        if (arg == "-ast")
        {
            printAST = true;
        }
        else if (arg == "-st")
        {
            printST = true;
        }
        else if (arg == "-cs")
        {
            printCS = true;
        }
        else if (arg == "-exe")
        {
            printExe = true;
        }
        else
        {
            cerr << "Invalid argument: " << arg << "\n";
            return 1;
        }
    }

    string filename(argv[argc - 1]);
    vector<string> tokens = getTokens(filename);

    shared_ptr<AST> ast = AST::createAST(tokens);
    if (printAST)
        cout << *ast << "\n";

    shared_ptr<ST> st = ast->standardize();
    if (printST)
        cout << *st << "\n";

    st->execute(printCS, printExe);
    return 0;
}

vector<string> getTokens(string filename)
{
    vector<string> content;
    ifstream _file(filename);

    if (!_file)
    {
        cerr << "File not found: " + filename << "\n";
        exit(EXIT_FAILURE);
    }

    // Read the file line by line
    for (string line; getline(_file, line);)
        content.push_back(trim(line));

    _file.close();
    return content;
}

string trim(const string &str)
{
    // Remove the leading spaces
    const auto strBegin = str.find_first_not_of(" ");
    if (strBegin == string::npos)
        return ""; // no content

    // Remove the trailing spaces
    const auto strEnd = str.find_last_not_of(" ");
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}
