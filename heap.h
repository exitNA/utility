/**
* \brief   a template heap class for get max/min n value from large scale dataset
* \note    root is always at the head
* \version 1.0
*/
#ifndef HEAP_H_
#define HEAP_H_

#include <iostream>
#include <algorithm>

/**
* \brief max heap template function
*/
template <typename T>
inline bool max_heap_cmp(T const& parent, T const& child)
{
    return (parent > child);
}

/**
* \brief min heap template function
*/
template <typename T>
inline bool min_heap_cmp(T const& parent, T const& child)
{
    return (parent < child);
}


/**
* \class Heap class
* \note 
*/
template <
    typename T
    , bool (*_heapify_fun)(T const& parent, T const& child) = min_heap_cmp
    >
class Heap
{
    T*      _pdata;
    size_t  _capacity;
    size_t  _size;

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

    inline void sort()
    {
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

    inline T& operator [] (size_t idx)
    {
        return _pdata[idx];
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
