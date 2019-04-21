#ifndef STACK_H
#define STACK_H

#include "list.h"
#include <vector>
template<typename T>
class Stack
{
public:
    Stack() :
        head(nullptr)
    {
    }
    ~Stack()
    {
        delete head;
    }
    void push(T *data)
    {
        head = new struct stack_node(data, head);
    }
    T *pop()
    {
    }
private:
    struct stack_node
    {
        stack_node(T *data, struct stack_node *next = nullptr) :
            data(data), next(next)
        {
        }
        ~stack_node()
        {
            delete next;
            delete data;
        }
        T* data;
        stack_node *next;
    } *head;
};

#endif // STACK_H
