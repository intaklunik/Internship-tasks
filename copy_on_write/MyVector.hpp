#ifndef CODEREVIEWTASK_MYVECTOR_HPP
#define CODEREVIEWTASK_MYVECTOR_HPP

#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <stdexcept>

template <typename T>
class MyVector
{
using map_t = std::vector<std::pair<T, std::string>>;

public:
    MyVector() : m_objnames(std::make_shared<map_t>()) {}
    MyVector(const MyVector& other): m_objnames(other.m_objnames) {}
    ~MyVector() {}
    void push_back(const T& obj, const std::string& name)
    {
        copy();
        m_objnames->push_back(std::make_pair(obj, name));
    }

    std::pair<const T&, const std::string&> operator[](int index) const
    {
        return std::pair<const T&, const std::string&>(m_objnames->at(index).first, m_objnames->at(index).second);
    }

    std::pair<T&, std::string&> operator[](int index)
    {
        copy();
        return std::pair<T&, std::string&>(m_objnames->at(index).first, m_objnames->at(index).second);
    }


    const T& operator[](const std::string& name) const
    {
        auto is_even = [&name](const std::pair<T, std::string> &i){ return i.second == name;};
        auto iter = std::find_if(m_objnames->begin(), m_objnames->end(), is_even);

        if (iter == m_objnames->end())
            throw std::invalid_argument(name + " is not found in the MyVector");

        return iter->first;
    }


    T& operator[](const std::string& name)
    {
        copy();
        auto is_even = [&name](std::pair<T, std::string> &i){ return i.second == name;};
        auto iter = std::find_if(m_objnames->begin(), m_objnames->end(), is_even);

        if (iter == m_objnames->end())
            throw std::invalid_argument(name + " is not found in the MyVector");

        return iter->first;
    }

    typename map_t::const_iterator cbegin() const
    {
        return m_objnames->cbegin();
    }

    typename map_t::const_iterator cend() const
    {
        return m_objnames->cend();
    }

    typename map_t::iterator end()
    {
        copy();
        return m_objnames->end();
    }

    typename map_t::iterator begin()
    {
        copy();
        return m_objnames->begin();
    }

    bool empty() const
    {
        return size() == 0;
    }

    typename map_t::size_type size() const
    {
        return m_objnames->size();
    }

    void reserve (typename map_t::size_type n)
    {
        if (m_objnames->capacity() < n) {
            copy();
            m_objnames->reserve(n);
        }
    }

    void clear()
    {
        if(m_objnames.use_count() == 1) {
            m_objnames->clear();
        } else {
            auto tmp = std::make_shared<map_t>();
            m_objnames = tmp;
        }
    }

private:
    void copy()
    {
        if (m_objnames.use_count() == 1)
            return;
        auto tmp = std::make_shared<map_t>(*m_objnames);
        m_objnames = tmp;
    }

private:
    std::shared_ptr<map_t> m_objnames;
};

#endif //CODEREVIEWTASK_MYVECTOR_HPP

