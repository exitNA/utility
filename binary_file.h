#ifndef BINARY_FILE_
#define BINARY_FILE_

#include <limits>
#include <fstream>
#include <string>
#include <exception>


class BinaryFile
{
    char*         _pbuf;        // buffer pointer
    size_t        _capacity;    // max size the buffer can hold
    size_t        _size;        // current buffer usad size
    std::ofstream _out;
    
public:
    explicit BinaryFile(size_t bufferSize = 4 * 1024)
        : _pbuf(NULL)
        , _size(0)
        , _capacity(0)
    {
        _pbuf = new char[bufferSize];
        _capacity = bufferSize;
    }

    BinaryFile(char const* const filePath, size_t bufferSize = 4 * 1024)
    {
        new (this)BinaryFile(bufferSize);
        open(filePath);
    }

    ~BinaryFile()
    {
        close();
    }

    void open(char const * const filePath)
    {
        _out.open(filePath, std::ios::binary);
        if (!_out.good())
        {
            std::string err = "fail to open buffer file: ";
            err += filePath;
            throw std::logic_error(err);
        }
    }

    void close()
    {
        if (_size != 0)
        {
            _out.write(_pbuf, _size); // write the rest
            _size = 0;
        }

        if (_pbuf != NULL)
        {
            delete[] _pbuf;
            _pbuf = NULL;
        }
        _capacity = 0;
        _out.close();
    }

    template<typename T>
    inline BinaryFile& write(T const * const psrc, size_t const size)
    {
        // check the rest buffer
        _check_buffer_size(size);

        // copy data to buffer
        memcpy(_pbuf + _size, psrc, size);
        _size += size;

        return *this;
    }

private:
    inline void _check_buffer_size(size_t const required_size)
    {
        if (_capacity - _size < required_size)
        {
            _out.write(_pbuf, _size);
            _size = 0;
        }
    }

private:
    BinaryFile(BinaryFile const& obj);
    BinaryFile& operator = (BinaryFile const& obj);
};

#endif
