#ifndef CRYPTARITHMIC_SOLVER_HPP
#define CRYPTARITHMIC_SOLVER_HPP
#include <iostream>
#include <map>
#include <vector>

using namespace std;

class Node // Node of the tree
{
public:
    bool visited;
    bool constraintsHold;
    bool resultFound;
    bool (*matrix)[10];
    Node(map<char, int> previousLetterValues, map<char, int> letterIndex, vector<char> distinctLetters);
    Node *children[10];
    void print(int numberOfLetters);
    int valueOfTheLetter(int letterIndex); // takes the row number of the letter and returns the value. if there is no, returns -1
};

class MyListNode // Node of the list
{
public:
    Node *value;
    MyListNode *next;
    MyListNode(Node *node);
};

class MyList
{
public:
    MyListNode *head;
    MyListNode *tail;

    void enqueue(MyListNode *newNode);
    void push(MyListNode *newNode);
    MyListNode *pop();
    MyList();
};

class CryptarithmicSolver
{
public:
    CryptarithmicSolver(string *searchType, string *operand1, string *operand2, string *result, string *outputFilename);
    vector<char> distinctLetters;
    vector<char> nonzeroLetters;
    Node *treeHead = NULL;
    void printTemp();
    void BFS();

private:
    string *searchType, *operand1, *operand2, *result, *outputFilename;
    map<char, int> letterIndex;
    void createLetterMap();
    void createTree();
    void createChildrenNodes(Node *, int currentLayer, map<char, int> previousLetterValues);
    void constraintChecker(Node *node);
    bool figureConflictErrorChecker(Node *node);  // if true, there is error
    bool leftmostZeroErrorChecker(Node *node);    // if true, there is error
    bool suitableNumbersErrorChecker(Node *node); // if true, there is error
};

#endif
