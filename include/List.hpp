#pragma once

#include "Node.hpp"
#include <string>

class List {
private:
  Node *head;
  Node *current;

public:
  List();
  ~List();
  void add(std::string type, std::string token);
  void show() const;

  Node *getHead() const;
};
