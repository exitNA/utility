#ifndef HEAP_H
#define HEAP_H
/************************************************************************/
/* root is always at the head
/* min heap
/************************************************************************/

template<typename T>
class Heap
{
    T*     _pdata;
    size_t _capacity;
    size_t _size;

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
        }
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
            sort();
            _size = length;
        }
        memcpy(ptemp, _pdata, _size);
        delete [] _pdata;
        _pdata = ptemp;
        _capacity = length;
    }

    void insert(T const& val)
    {
        if (_size < _capacity) // not full
        {
            // store new value at the end for temporary
            size_t curr = _size++;
            size_t p = 0;
            // keep min heapify
            while ((curr > 0) && (val < _pdata[p = parent(curr)]))
            {
                // parent down
                _pdata[curr] = _pdata[p];
                curr = p;
            }
            _pdata[curr] = val;
        }
        else // full
        {
            keep_heapify(0, val);
        }
    }

    void sort()
    {
        std::sort(_pdata, _pdata + _size);
    }

    void print()
    {
        sort();
        for (size_t i = 0; i < _size; ++i)
        {
            std::cout << _pdata[i] << ", ";
        }
        std::cout << std::endl;
    }

private:
    inline size_t parent(size_t idx)
    {
        return ((idx - 1) >> 1);
    }

    inline size_t left_child(size_t idx)
    {
        return ((idx << 1) + 1);
    }

    inline size_t right_child(size_t idx)
    {
        return ((idx << 1) + 2);
    }

    inline void keep_heapify( size_t curr, T const& val ) 
    {
        if (val < _pdata[curr]) // smaller than root
        {
            return;
        }

        size_t l;
        size_t r;
        size_t smallest;
        while (((l = left_child(curr)) < _capacity) // it has left child
            && val > _pdata[l])
        {
            smallest = l;
            if (((r = right_child(curr)) < _capacity) // it has right child
                && (_pdata[l] > _pdata[r]))
            {
                smallest = r;
            }
            _pdata[curr] = _pdata[smallest];
            curr = smallest;
        }
        _pdata[curr] = val;
    }
};

#endif