#ifndef TEXT_FILE_H_
#define TEXT_FILE_H_

#include <limits>
#include <fstream>
#include <string>
#include <exception>


class TextFile
{
    char*  _pbuf;			// buffer pointer
    size_t _capacity; 		// max size the buffer can hold
    size_t _size;           // current buffer usad size
    std::ofstream _out;

public:
    explicit TextFile(size_t buffer_size = 4 * 1024)
        : _pbuf(NULL)
        , _size(0)
        , _capacity(0)
    {
        _pbuf = new char[buffer_size];
        _capacity = buffer_size;
    }

    ~TextFile()
    {
        close();
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

    TextFile& operator << (std::string const& str)
    {
        return write(str.c_str(), str.size());
    }

    TextFile& operator << (char const* pstr)
    {
        return write(pstr, strlen(pstr));
    }

    TextFile& operator << (const size_t val)
    {
        // calculate required memory
        // 能够完全表示的有效位个数 + 1(不能完全表示的个数) + 1(符号) + 1('\0')
        /*static */const int DIGITAL_WIDTH = std::numeric_limits<size_t>::digits10 + 3;
        _check_buffer_size(DIGITAL_WIDTH);

        // convert and write to memory
#ifdef _WIN32
        int written = _snprintf(_pbuf + _size, DIGITAL_WIDTH, "%Iu", val);
#elif defined __GNUC__
        int written = snprintf(_pbuf + _size, DIGITAL_WIDTH, "%zu", val);
#endif
        _size += written;

        return *this;
    }

    TextFile& operator << (const double val)
    {
        // calculate required memory
        /*static */const int DIGITAL_WIDTH = std::numeric_limits<double>::digits10 + 3;
        _check_buffer_size(DIGITAL_WIDTH);

        // convert and write to memory
#ifdef _WIN32
        int written = _snprintf(_pbuf + _size, DIGITAL_WIDTH, "%0.15f", val); // 15 precision
#elif defined __GNUC__
        int written = snprintf(_pbuf + _size, DIGITAL_WIDTH, "%0.15f", val); // 15 precision
#endif
        _size += written;

        return *this;
    }

    TextFile& operator << (const char c)
    {
        // check the rest buffer
        _check_buffer_size(1);

        // copy the string to the buffer
        _pbuf[_size] = c;
        _size += 1;

        return *this;
    }

    template<typename T>
    TextFile& write(T const * const psrc, size_t size)
    {
        // check the rest buffer
        _check_buffer_size(size);

        // copy the string to the buffer
        memcpy(_pbuf + _size, psrc, size);
        _size += size;

        return *this;
    }


private:
    inline void _check_buffer_size(size_t in_size)
    {
        if (_capacity - _size < in_size)
        {
            _out.write(_pbuf, _size);
            _size = 0;
        }
    }

    TextFile(TextFile const& obj);
    TextFile& operator = (TextFile const& obj);
};

#endif
