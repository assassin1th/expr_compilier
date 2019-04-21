#ifndef LIST_H_
#define LIST_H_

#include <string>
#include <stdlib.h>
#include <iostream>

template <typename T>
class Cont {
public:
    Cont() {}
    virtual ~Cont() {}
    virtual T *get(const std::string &key) = 0;
    virtual Cont<T> *set(T *data, const std::string &key) = 0;
};

template <typename T>
class List : public Cont<T> {
public:
    List() :
        head(nullptr), n_elem(0)
    {
    }
    virtual ~List()
    {
        delete head;
    }
    virtual T *get(const std::string &key)
    {
        struct node *tmp = head;
        for (; tmp != nullptr; tmp = tmp->next)
        {
            if (tmp->key == key)
            {
                --n_elem;
                return tmp->data;
            }
        }
        return nullptr;
    }
    virtual Cont<T> *set(T *data, const std::string &key)
    {
        if (n_elem == 0)
        {
            head = new struct node(data, key);
        } else {
            head = new struct node(data, key, head);
        }
        ++n_elem;
        return this;
    }
private:
    struct node {
        node(T *data, const std::string &key, struct node *next = nullptr):
            data(data), key(key), next(next)
        {
        }
        ~node()
        {
            delete next;
            delete data;
        }
        T *data;
        std::string key;
        struct node *next;
    } *head;
    int n_elem;
};

#endif // LIST_H_
