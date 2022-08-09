#ifndef FORWARDLIST_H
#define FORWARDLIST_H
#include <initializer_list>
#include <functional>

struct node_base {
    node_base *next = nullptr;
    virtual ~node_base() {}
};

template<class T>
struct node : public node_base{
    T value;
    node() : value() {}
    node(const T &val) : value(val) {}
};


template<typename T>
using SplitPredicate = std::function<bool(const T &value)>;

template<typename T>
class forward_list
{
    public:
    class iterator {
            public:
                iterator(node_base * const n) : pnode(n) {}
                T& operator*() { return ((node<T> *)pnode)->value; }
                T* operator->() {return &((node<T> *)pnode)->value; }
                iterator& operator++() { pnode = pnode->next; return *this; }
                iterator& operator++(int) {iterator tmp(pnode); pnode = pnode->next; return tmp; }
                bool operator!=(const iterator &it) {return pnode != it.pnode;}
                bool operator==(const iterator &it) {return pnode == it.pnode;}
            private:
                node_base *pnode;
    };
    class const_iterator {
            public:
                const_iterator(const node_base *n) : pnode(n) {}
                const T& operator*() const { return ((node<T> *)pnode)->value; }
                const T* operator->() const {return &((node<T> *)pnode)->value; }
                const_iterator& operator++() const { pnode = pnode->next; return *this; }
                const_iterator& operator++(int) const {const_iterator tmp(pnode); pnode = pnode->next; return tmp; }
                bool operator!=(const const_iterator &it) const {return pnode != it.pnode;}
                bool operator==(const const_iterator &it) const {return pnode == it.pnode;}
            private:
                node_base *pnode;
    };
        forward_list() {top.next = &tail;}
        forward_list(unsigned long n);
        forward_list(std::initializer_list<T> il);
        forward_list(const forward_list &list);
        forward_list (forward_list &&list) noexcept;
        forward_list & operator=(const forward_list &list);
        forward_list & operator=(forward_list &&list);
        ~forward_list() {clear();}
        iterator before_begin();
        iterator begin();
        iterator end();
        const_iterator cbefore_begin() const;
        const_iterator cbegin() const;
        const_iterator cend() const;
        void push_front (const T &val);
        void push_front(T &&val);
        void pop_front();
        void clear();
        size_t size() {return m_size; }
        forward_list split_when(SplitPredicate<T> condition);
    private:
        node_base top;
        node_base tail;
        size_t m_size = 0;
};



template<typename T>
forward_list<T> forward_list<T>::split_when(SplitPredicate<T> condition)
{
    bool isCondition = false;
    node_base *pOrig = top.next;
    node_base *newTail = pOrig;
    int newSize = 0;

    for(; pOrig != tail.next;newTail = pOrig, pOrig = pOrig->next, ++newSize) {
        if (isCondition = condition(((node<T> *)pOrig)->value))
            break;
    }
    if(!isCondition) {
        return forward_list<T>();
    }

    int tailSize = size() - newSize;
    m_size = newSize;
    forward_list<T> tail_list;
    tail_list.top.next = pOrig;
    newTail->next = tail.next;
    node_base *pTail = tail_list.top.next;
    while(--tailSize)
        pTail = pTail->next;
    pTail = &tail_list.tail;
    tail_list.m_size = tailSize;

    return tail_list;
}


template<typename T>
void forward_list<T>::clear()
{
    while(m_size) {
        pop_front();
    }
}

template<typename T>
forward_list<T> & forward_list<T>::operator=(forward_list &&list)
{
    if (this != &list) {
        clear();
        std::swap(top, list.top);
        std::swap(tail, list.tail);
        std::swap(m_size, list.m_size);
    }

    return *this;
}

template<typename T>
forward_list<T> & forward_list<T>::operator=(const forward_list &list)
{
    if (this != &list) {
        clear();
        node_base *p = &top;
        for(auto it = list.cbegin(); it != list.cend(); ++it) {
            p->next = new node<T>(*it);
            p = p->next;
        }
        p = &tail;
    }

    return *this;
}


template<typename T>
forward_list<T>::forward_list (forward_list &&list) noexcept
{
    if (this != &list) {
        std::swap(top, list.top);
        std::swap(tail, list.tail);
        std::swap(m_size, list.m_size);
    }
}

template<typename T>
forward_list<T>::forward_list(const forward_list &list) : m_size(list.m_size)
{
    if(this != &list){
        node_base *p = &top;
        for(auto it = list.cbegin(); it != list.cend(); ++it) {
            p->next = new node<T>(*it);
            p = p->next;
        }
        p = &tail;
    }
}

template<typename T>
forward_list<T>::forward_list(unsigned long n) : m_size(n)
{
    node_base *p = &top;
    for(; n--; p = p->next) {
         p->next = new node<T>();
    }
    p = &tail;
}

template<typename T>
forward_list<T>::forward_list(std::initializer_list<T> il) : m_size(il.size())
{
    node_base *p = &top;
    for(auto it = il.begin(); it != il.end(); ++it) {
        p->next = new node<T>(*it);
        p = p->next;
    }
    p = &tail;
}

template<typename T>
typename forward_list<T>::iterator forward_list<T>::before_begin()
 {
        return iterator(&top);
 }

 template<typename T>
 typename forward_list<T>::const_iterator forward_list<T>::cbefore_begin() const
 {
        return const_iterator(&top);
 }


template<typename T>
 typename forward_list<T>::iterator forward_list<T>::begin()
 {
        return iterator(top.next);
 }

template<typename T>
 typename forward_list<T>::const_iterator forward_list<T>::cbegin() const
 {
        return const_iterator(top.next);
 }

 template<typename T>
 typename forward_list<T>::iterator forward_list<T>::end()
 {
        return iterator(tail.next);
 }

 template<typename T>
 typename forward_list<T>::const_iterator forward_list<T>::cend() const
 {
        return const_iterator(tail.next);
 }


 template<typename T>
 void forward_list<T>::push_front(const T &val)
 {
    node<T> *el = new node<T>(val);
    el->next = top.next;
    top.next = el;
    ++m_size;
 }


template<typename T>
void forward_list<T>::push_front(T &&val)
{
    node<T> *el = new node<T>();
    el->value = std::move(val);
    el->next = top.next;
    top.next = el;
    ++m_size;
}


 template<typename T>
 void forward_list<T>::pop_front()
 {
    if(m_size != 0){
        --m_size;
        node_base *del_el = top.next;
        top.next = del_el->next;

        node<T> *del = (node<T>*)del_el;
        delete del;
    }
 }


#endif // FORWARDLIST_H
