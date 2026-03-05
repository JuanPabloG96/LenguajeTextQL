#ifndef NODE_H
#define NODE_H

#include <string>
class Node {
private:
  int id;
  inline static int currentId = 1;
  std::string tokenType;
  std::string token;

  Node* next;

public:
  Node(std::string tokenType, std::string token);

  int getId() const;
  std::string getToken() const;
  std::string getTokenType() const;

  Node* getNext() const;
  void setNext(Node* n);
};

#endif
