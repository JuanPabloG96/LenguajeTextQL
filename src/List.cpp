#include "List.hpp"
#include <iostream>

List::List() : head(nullptr), current(nullptr)
{
}

List::~List()
{
    Node *cur = head;

    while (cur)
    {
        Node *tmp = cur;
        cur = cur->getNext();
        delete tmp;
    }
}

void List::add(std::string type, std::string token)
{

    Node *n = new Node(type, token);

    if (!head)
    {
        head = current = n;
    }
    else
    {
        current->setNext(n);
        current = n;
    }
}

void List::show() const
{
    Node *cur = head;

    while (cur)
    {
        std::cout << cur->getToken() << " (" << cur->getTokenType() << ")\n";

        cur = cur->getNext();
    }
}

Node *List::getHead() const
{
    return this->head;
}
