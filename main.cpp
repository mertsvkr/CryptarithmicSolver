#include <iostream>
#include "CryptarithmicSolver.hpp"
using namespace std;

int main(int argc, char *argv[])
{
    string *searchType = new string(argv[1]);
    string *operand1 = new string(argv[2]);
    string *operand2 = new string(argv[3]);
    string *result = new string(argv[4]);
    string *outputFilename = new string(argv[5]);

    CryptarithmicSolver *cs = new CryptarithmicSolver(searchType, operand1, operand2, result, outputFilename);

    cs->BFS();

    cs->printTemp();
    delete searchType;
    delete operand1;
    delete operand2;
    delete result;
    delete outputFilename;
    delete cs;

    return 0;
}