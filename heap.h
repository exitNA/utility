#ifndef HEAP_H
#define HEAP_H
/************************************************************************/
/* root is always at the head
/* min heap
/************************************************************************/
#include <iostream>
#include <algorithm>

template<typename T>
struct max_heap_cmp
{
    inline bool operator () (T const& parent, T const& child)
    {
        return parent > child;
    }
};

template<typename T>
struct min_heap_cmp
{
    inline bool operator () (T const& parent, T const& child)
    {
        return parent < child;
    }
};
template<typename T, typename Compare = min_heap_cmp<T>>
class Heap
{
    T*     _pdata;
    size_t _capacity;
    size_t _size;
    Compare _cmp_fun;
public:
    Heap(size_t length = 0)
        : _pdata(NULL)
        , _capacity(0)
        , _size(0)
    {
        resize(length);
    }

    ~Heap()
    {
        if (_pdata != NULL)
        {
            delete [] _pdata;
            _pdata = NULL;
        }
        _capacity = 0;
        _size = 0;
    }

    void resize(size_t length)
    {
        if (length == _capacity)
        {
            return;
        }

        T* ptemp = new T[length];
        if (_size > length)
        {
            _size = length;
        }
        // copy data to new memory
        memcpy(ptemp, _pdata, _size * sizeof(T));

        delete [] _pdata;
        _pdata = ptemp;

        _capacity = length;
    }

    void insert(T const& val)
    {
        if (_size < _capacity) // not full
        {
            bubble_up(_size, val);
            _size++;
        }
        else // full
        {
            bubble_down(0, val);
        }
    }

    void sort()
    {
        std::sort(_pdata, _pdata + _size, _cmp_fun);
    }

    void print()
    {
        sort();
        size_t i = 0;
        std::cout << _pdata[i++];
        for (; i < _size; ++i)
        {
            std::cout << ", " << _pdata[i];
        }
        std::cout << std::endl;
    }

private:
    inline size_t parent_pos(size_t idx)
    {
        return ((idx - 1) >> 1);
    }

    inline size_t lchild_pos(size_t idx)
    {
        return ((idx << 1) + 1);
    }

    inline size_t rchild_pos(size_t idx)
    {
        return ((idx << 1) + 2);
    }

    inline void bubble_up(size_t pos, T const& val)
    {
        size_t p = 0;
        while ((pos > 0) && _cmp_fun(val, _pdata[p = parent_pos(pos)]))
        {
            _pdata[pos] = _pdata[p];
            pos = p;
        }
        _pdata[pos] = val;
    }

    inline void bubble_down(size_t pos, T const& val) 
    {
        if (_cmp_fun(_pdata[pos], val))
        {
            return;
        }

        size_t l = 0;
        size_t r = 0;
        size_t p = pos;
        while (((l = lchild_pos(pos)) < _size) // it has left child
            && _cmp_fun(_pdata[l], val))
        {
            p = l;
            if (((r = rchild_pos(pos)) < _size) // it has right child
                && _cmp_fun(_pdata[r], _pdata[p]))
            {
                p = r;
            }
            _pdata[pos] = _pdata[p];
            pos = p;
        }
        _pdata[pos] = val;
    }
};

#endif