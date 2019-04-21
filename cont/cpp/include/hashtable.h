#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "list.h"

template <typename T>
class Table : public Cont<T>{
    int hash(const char *src)
    {
        int hashval = 0;
        for (const char *p = src; *p != '\0'; ++p)
        {
            hashval = hashval * 31 + *p;
        }
        return hashval & ((1 << _bits) - 1);
    }
public:
    Table(int bits = 6) :
        _bits(bits)
    {
        tab = new struct _tab [1 << bits];
    }
    ~Table()
    {
        delete[] tab;
    }

    Cont<T> *set(T *data, const std::string &key)
    {
        int hashval = hash(key.c_str());
        tab[hashval].cont = tab[hashval].cont->set(data, key);
    }

    T *get(const std::string &key)
    {
        return tab[hash(key.c_str())].cont->get(key);
    }
private:
    struct _tab {
        _tab()
        {
            cont = new List<T>();
        }
        ~_tab()
        {
            delete cont;
        }
        Cont<T> *cont;
    } *tab;
    int _bits;
};

#endif // HASHTABLE_H_
