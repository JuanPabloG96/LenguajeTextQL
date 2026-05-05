#pragma once

#include <string>
class Node {
 private:
  int id;
  inline static int currentId = 1;
  std::string tokenType;
  std::string token;
  int line;
  int column;

  Node* next;

 public:
  Node(std::string tokenType, std::string token, int line, int column);

  int getId() const;
  std::string getToken() const;
  std::string getTokenType() const;
  int getLine() const;
  int getColumn() const;

  Node* getNext() const;
  void setNext(Node* n);
};
