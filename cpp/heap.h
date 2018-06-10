/**
* \brief   a template heap class for get max/min n value from large scale dataset
* \note    root is always at the head
* \author  mark
* \version 1.0
*/
#ifndef HEAP_H_
#define HEAP_H_

#include <string>

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

    size_t size() const
    {
        return _size;
    }

    size_t capacity() const
    {
        return _capacity;
    }

    inline T& operator [] (size_t idx)
    {
        return _pdata[idx];
    }

    void clear()
    {
        _size = 0;
    }

    void resize(size_t length)
    {
        if (length == _capacity)
        {
            return;
        }

        // create new space
        T* ptemp = new T[length];
        if (_size > length)
        {
            _size = length;
        }

        // copy data to new space
        for (size_t i = 0; i < _size; ++i)
        {
            ptemp[i] = _pdata[i];
        }

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
        else if (_heapify_fun(_pdata[0], val)) // full
        {
            bubble_down(0, val);
        }
    }

    inline void sort()
    {
        size_t org_size = _size; // keep original size
        T val;
        while (_size > 1)
        {
            _size--;
            val = _pdata[_size]; // get last element
            _pdata[_size] = _pdata[0];
            bubble_down(0, val);
        }
        _size = org_size;
    }

private:
    inline size_t parent_of(size_t idx)
    {
        return ((idx - 1) >> 1);
    }

    inline size_t left_child_of(size_t idx)
    {
        return ((idx << 1) + 1);
    }

    inline size_t right_child_of(size_t idx)
    {
        return ((idx << 1) + 2);
    }

    inline void bubble_up(size_t pos, T const& val)
    {
        size_t p = 0;
        while ((pos > 0) && _heapify_fun(val, _pdata[p = parent_of(pos)]))
        {
            _pdata[pos] = _pdata[p];
            pos = p;
        }
        _pdata[pos] = val;
    }

    inline void bubble_down(size_t pos, T const& val)
    {
        size_t child = 0;
        size_t left  = 0;
        size_t right = 0;

        while ((left = left_child_of(pos)) < _size) // left child position
        {
            // right child position
            right = left + 1;

            // get best child position
            child = right < _size
                ? (_heapify_fun(_pdata[right], _pdata[left]) ? right : left)
                : left;

            // check if child could be a father
            if (_heapify_fun(_pdata[child], val))
            {
                _pdata[pos] = _pdata[child];
                pos = child;
            }
            else
            {
                break;
            }
        }
        _pdata[pos] = val;
    }
};

#endif
