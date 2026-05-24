#include "Node.hpp"

#include <string>

Node::Node(std::string tokenType, std::string token, int line, int column)
    : tokenType(tokenType), token(token), next(nullptr), line(line),
      column(column)
{
    id = currentId++;
}

int Node::getId() const
{
    return this->id;
}
std::string Node::getToken() const
{
    return this->token;
}
std::string Node::getTokenType() const
{
    return this->tokenType;
}

int Node::getLine() const
{
    return this->line;
}
int Node::getColumn() const
{
    return this->column;
}

Node* Node::getNext() const
{
    return this->next;
}
void Node::setNext(Node* n)
{
    this->next = n;
}
