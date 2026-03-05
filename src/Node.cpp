#include "Node.hpp"
#include <string>

Node::Node(std::string tokenType, std::string token)
    : tokenType(tokenType), token(token), next(nullptr) {
  id = currentId++;
}

int Node::getId() const { return this->id; }
std::string Node::getToken() const { return this->token; }
std::string Node::getTokenType() const { return this->tokenType; }

Node* Node::getNext() const { return this->next; }
void Node::setNext(Node* n) { this->next = n; }
