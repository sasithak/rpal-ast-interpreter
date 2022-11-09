#include <iostream>
#include <fstream>
#include <vector>
#include "ast.h"

using namespace std;

vector<string> getTokens(string filename);
string trim(const string &str);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "No input file specified" << endl;
        return 1;
    }

    if (argc > 2)
    {
        cerr << "Too many arguments" << endl;
        return 1;
    }

    string filename(argv[1]);
    vector<string> tokens = getTokens(filename);

    ofstream out("output.txt");

    shared_ptr<AST> ast = AST::createAST(tokens);
    out << *ast;
    out << endl;

    shared_ptr<ST> st = ast->standardize();
    out << *st;
    out << endl;

    st->execute(out);
    return 0;
}

vector<string> getTokens(string filename)
{
    vector<string> content;
    ifstream _file(filename);

    if (!_file)
    {
        cerr << "File not found: " + filename << endl;
        exit(EXIT_FAILURE);
    }

    for (string line; getline(_file, line);)
        content.push_back(trim(line));

    return content;
}

string trim(const string &str)
{
    const auto strBegin = str.find_first_not_of(" ");
    if (strBegin == string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(" ");
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}
