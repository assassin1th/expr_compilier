#ifndef CONTAINERS_H_
#define CONTAINERS_H_

#include <string>

namespace Containers
{

template <typename T>
class Cont
{
public:
    Cont() {}
    virtual ~Cont() {}
    virtual T *get(const std::string &key) = 0;
    virtual Cont<T> *set(T *data, const std::string &key) = 0;
};

template <typename T>
class List : public Cont<T>
{
public:
    List() :
        m_head(nullptr), m_n_elem(0)
    {
    }
    virtual ~List()
    {
        delete m_head;
    }
    virtual T *get(const std::string &key)
    {
        struct node *tmp = m_head;
        for (; tmp != nullptr; tmp = tmp->next)
        {
            if (tmp->key == key)
            {
                --m_n_elem;
                return tmp->data;
            }
        }
        return nullptr;
    }
    virtual Cont<T> *set(T *data, const std::string &key)
    {
        if (m_n_elem == 0)
        {
            m_head = new struct node(data, key);
        } else {
            m_head = new struct node(data, key, m_head);
        }
        ++m_n_elem;
        return this;
    }
private:
    struct node
    {
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
    } *m_head;
    int m_n_elem;
};

template <typename T>
class Table : public Cont<T>
{
    int hash(const char *src) const
    {
        int hashval = 0;
        for (const char *p = src; *p != '\0'; ++p)
        {
            hashval = hashval * 31 + *p;
        }
        return hashval & ((1 << m_bits) - 1);
    }
public:
    Table(int bits = 6) :
        m_bits(bits)
    {
        m_tab = new struct tab [1 << bits];
    }
    ~Table()
    {
        delete[] m_tab;
    }

    Cont<T> *set(T *data, const std::string &key)
    {
        int hashval = hash(key.c_str());
        m_tab[hashval].m_cont = m_tab[hashval].m_cont->set(data, key);
    }

    T *get(const std::string &key)
    {
        return m_tab[hash(key.c_str())].m_cont->get(key);
    }
private:
    struct tab {
        tab()
        {
            m_cont = new List<T>();
        }
        ~tab()
        {
            delete m_cont;
        }
        Cont<T> *m_cont;
    } *m_tab;
    int m_bits;
};

}

#endif // CONTAINERS_H_
