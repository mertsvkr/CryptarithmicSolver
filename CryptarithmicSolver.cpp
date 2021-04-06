#include "CryptarithmicSolver.hpp"
#include <fstream>
#include <sstream>

CryptarithmicSolver::CryptarithmicSolver(string *searchType, string *operand1, string *operand2, string *result, string *outputFilename)
{
    this->searchType = searchType;
    this->operand1 = operand1;
    this->operand2 = operand2;
    this->result = result;
    this->outputFilename = outputFilename;

    this->createLetterMap();
    this->createTree();
}

void CryptarithmicSolver::createLetterMap()
{
    string fileName(*(this->operand1) + " " + *(this->operand2) + " " + *(this->result) + ".txt");
    string line;
    ifstream infile(fileName);
    getline(infile, line);
    int i;
    for (i = 0; getline(infile, line); i++)
    {
        this->letterIndex[toupper(line[0])] = i;
        this->distinctLetters.push_back(toupper(line[0]));
    }
}

void CryptarithmicSolver::createTree()
{
    map<char, int> previousLetterValues;
    previousLetterValues = this->letterIndex;
    for (int i = 0; i < this->distinctLetters.size(); i++)
    {
        // at the begining there is no previous state of the letters so it is set -1
        previousLetterValues[distinctLetters[i]] = -1;
    }

    this->treeHead = new Node(previousLetterValues, this->letterIndex, this->distinctLetters);
    this->createChildrenNodes(this->treeHead, 0, previousLetterValues);
}

void CryptarithmicSolver::createChildrenNodes(Node *node, int currentLayer, map<char, int> previousLetterValues)
{
    if (currentLayer < this->distinctLetters.size())
    {
        for (int i = 0; i < 10; i++)
        { // create 10 different child matrix for the next layer of the current node, recursively.
            previousLetterValues[this->distinctLetters[currentLayer]] = i;
            node->children[i] = new Node(previousLetterValues, this->letterIndex, this->distinctLetters);
            this->createChildrenNodes(node->children[i], currentLayer + 1, previousLetterValues);
        }
    }
}

void CryptarithmicSolver::printTemp()
{
}

void CryptarithmicSolver::constraintChecker(Node *node)
{
    if (this->figureConflictErrorChecker(node))
    {
        node->constraintsHold = false;
        return;
    }
    if (this->leftmostZeroErrorChecker(node))
    {
        node->constraintsHold = false;
        return;
    }
    if (this->suitableNumbersErrorChecker(node))
    {
        node->constraintsHold = false;
        return;
    }
}

bool CryptarithmicSolver::suitableNumbersErrorChecker(Node *node)
{
    int maxNumberOfDigits = this->result->length(); // the longest number must be in the result of the addition.
    int carry = 0;
    int digitTotal = 0;
    int letterValue;
    for (int i = 0; i < maxNumberOfDigits; i++)
    {
        digitTotal = carry;
        if (this->operand1->length() - 1 >= i)
        {
            letterValue = node->valueOfTheLetter(this->letterIndex[((*operand1)[operand1->length() - 1 - i])]);
            if (letterValue == -1)
            {
                return false; // no error. because of the insufficient info about the value of the letter.
            }
            else
            {
                digitTotal += letterValue;
            }
        }
        if (this->operand2->length() - 1 >= i)
        {
            letterValue = node->valueOfTheLetter(this->letterIndex[((*operand2)[operand2->length() - 1 - i])]);
            if (letterValue == -1)
            {
                return false; // no error. because of the insufficient info about the value of the letter.
            }
            else
            {
                digitTotal += letterValue;
            }
        }
        letterValue = node->valueOfTheLetter(this->letterIndex[((*result)[result->length() - 1 - i])]);
        if (letterValue == -1)
        {
            return false; // no error. because of the insufficient info about the value of the letter.
        }
        else
        {
            if (letterValue != digitTotal % 10)
            {
                return true; // error in result
            }
            else
            {
                carry = digitTotal / 10;
            }
        }
    }
    //solution found:
    node->resultFound = true;
    return false; //no problem.
}

bool CryptarithmicSolver::leftmostZeroErrorChecker(Node *node)
{
    /*
        I assumed if the operand have more then 1 digit (operand > 10), the leftmost digit cannot be zero.
        but if the number's itself is 0, it is possible to be zero.
    */
    if (this->operand1->length() > 1)
    {
        if (node->matrix[this->letterIndex[(*(this->operand1))[0]]][0]) // checks the value of the first letter of the operand
        {
            return true; // there is error;
        }
    }
    if (this->operand2->length() > 1)
    {
        if (node->matrix[this->letterIndex[(*(this->operand2))[0]]][0])
        {
            return true; // there is error;
        }
    }
    if (this->result->length() > 1)
    {
        if (node->matrix[this->letterIndex[(*(this->result))[0]]][0])
        {
            return true; // there is error;
        }
    }
    return false; // no error
}

bool CryptarithmicSolver::figureConflictErrorChecker(Node *node)
{
    // checks columns; if there are more then one check in the column
    //it means there are numbers with same value
    int columnTotal;
    for (int i = 0; i < 10; i++)
    {
        columnTotal = 0; // make it zero for each column check;
        for (int j = 0; j < this->distinctLetters.size(); j++)
        {
            columnTotal += node->matrix[j][i];
            if (columnTotal == 2)
            {
                return true; // there is error
            }
        }
    }
    return false; // no error
}

void CryptarithmicSolver::BFS()
{
    int totalVisitedNodes = 0;
    MyList queue;
    MyListNode *temp;
    for (int i = 0; i < 10; i++) // firstly, add the first 10 node in the layer 1.
    {
        queue.enqueue(new MyListNode((this->treeHead->children)[i]));
    }
    this->treeHead->visited = true;
    totalVisitedNodes++;

    while (queue.head != NULL) // make the operation till the queue is empty.
    {
        temp = queue.pop();
        //        temp->value->print(distinctLetters.size());
        this->constraintChecker(temp->value);
        if (temp->value->constraintsHold) // if the constraints are hold, add its children to the queue
        {
            if (temp->value->resultFound)
            { // if this node has the solution
                temp->value->print((this->distinctLetters).size());
                return;
            }
            for (int i = 0; i < 10; i++)
            {
                queue.enqueue(new MyListNode((temp->value->children)[i]));
            }
        }
        temp->value->visited = true;
        totalVisitedNodes++;
    }
}

Node::Node(map<char, int> previousLetterValues, map<char, int> letterIndex, vector<char> distinctLetters)
{
    this->visited = false;
    this->resultFound = false;
    this->constraintsHold = true; // this will be checked  and changed by the further search operations.

    //set the children to NULL
    for (int i = 0; i < 10; i++)
    {
        this->children[i] = NULL;
    }

    //initialize the matrix with full of zeros
    this->matrix = new bool[distinctLetters.size()][10]();

    //set the state of the matrix with the previous letter values.
    for (int i = 0; i < distinctLetters.size(); i++)
    {
        if (previousLetterValues[distinctLetters[i]] != -1)
        {
            this->matrix[letterIndex[distinctLetters[i]]][previousLetterValues[distinctLetters[i]]] = true;
        }
    }
}

int Node::valueOfTheLetter(int letterIndex)
{
    for (int i = 0; i < 10; i++)
    {
        if (this->matrix[letterIndex][i])
        {
            return i;
        }
    }
    return -1;
}

void Node::print(int numberOfLetters)
{
    for (int i = 0; i < numberOfLetters; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            cout << this->matrix[i][j] << " ";
        }
        cout << endl;
    }
}

MyListNode::MyListNode(Node *node)
{
    this->value = node;
    this->next = NULL;
}

MyList::MyList()
{
    this->head = NULL;
    this->tail = NULL;
}

void MyList::enqueue(MyListNode *newNode)
{
    if (this->head == NULL)
    {
        this->head = newNode;
        this->tail = newNode;
    }
    else
    {
        this->tail->next = newNode;
        this->tail = this->tail->next;
    }
}

void MyList::push(MyListNode *newNode)
{
    if (this->head == NULL)
    {
        newNode->next = this->head;
        this->head = newNode;
        this->tail = this->head;
    }
    else
    {
        newNode->next = this->head;
        this->head = newNode;
    }
}

MyListNode *MyList::pop()
{
    MyListNode *temp = this->head;
    if (this->head == NULL)
    {
        return temp;
    }
    if (this->head->next == NULL)
    {
        this->head = NULL;
        this->tail = NULL;
        return temp;
    }
    else
    {
        this->head = this->head->next;
        return temp;
    }
}
