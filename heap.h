#ifndef HEAP_H
#define HEAP_H
/************************************************************************/
/* root is always at the head
/************************************************************************/
#include <iostream>
#include <algorithm>

template<typename T>
struct max_heap_cmp
{
    inline bool operator () (T const& parent, T const& child)
    {
        return (parent > child);
    }
};

template<typename T>
struct min_heap_cmp
{
    inline bool operator () (T const& parent, T const& child)
    {
        return (parent < child);
    }
};

template<typename T,
    typename Compare = min_heap_cmp<T> >
class Heap
{
    T*      _pdata;
    size_t  _capacity;
    size_t  _size;
    Compare _heapify_fun;
    bool    _is_sorted;

public:
    Heap(size_t length = 0)
        : _pdata(NULL)
        , _capacity(0)
        , _size(0)
        , _is_sorted(false)
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

    inline void insert(T const& val)
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

    void clear()
    {
        _size = 0;
    }

    void sort()
    {
//        if (_is_sorted)
//        {
//            return;
//        }
//        _is_sorted = true;

        size_t org_size = _size; // keep orginal size
        T val;
        while (_size > 0)
        {
            _size--;
            val = _pdata[_size]; // get last element
            _pdata[_size] = _pdata[0];
            bubble_down(0, val);
        }
        _size = org_size;
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

    T* data()
    {
        return _pdata;
    }

    size_t size() const
    {
        return _size;
    }

    inline void erase(size_t pos)
    {
        if (pos >= _size)   // range exceed
        {
            return;
        }

        T val = _pdata[--_size]; // get last element
        bubble_down(pos, val);
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
        while ((pos > 0) && _heapify_fun(val, _pdata[p = parent_pos(pos)]))
        {
            _pdata[pos] = _pdata[p];
            pos = p;
        }
        _pdata[pos] = val;
    }

    inline void bubble_down(size_t pos, T const& val) 
    {
        // test if val as a parent of the _pdata[pos] is ok
        if (_heapify_fun(val, _pdata[pos]))
        {
            return;
        }

        size_t l = 0;
        size_t r = 0;
        size_t p = pos;
        while (((l = lchild_pos(pos)) < _size) // it has left child
            && _heapify_fun(_pdata[l], val))
        {
            p = l;
            if (((r = rchild_pos(pos)) < _size) // it has right child
                && _heapify_fun(_pdata[r], _pdata[p]))
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