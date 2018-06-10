#ifndef STRING_UTILITY_H_
#define STRING_UTILITY_H_

#include <stdint.h>
#include <string>
#include <limits>
#include <algorithm>


namespace detail
{
    inline void str2num(uint32_t& ret, std::string const& str)
    {
        ret = 0;
        char c;
        size_t i = 0;

        c = str[i];
        while ((c >= '0') && (c <= '9'))
        {
            ret = (ret * 10) + (c - '0');
            c = str[++i];
        }
    }

    inline void str2num(int32_t& ret, std::string const& str)
    {
        ret = 0;
        bool neg = false;
        char c;
        size_t i = 0;

        if (str[i] == '-')
        {
            ++i;
            neg = true;
        }

        c = str[i];
        while (c >= '0' && c <= '9')
        {
            ret = (ret * 10) + (c - '0');
            c = str[++i];
        }

        if (neg)
        {
            ret = -ret;
        }
    }

    inline size_t num2str(char* pstr, uint32_t val)
    {
        size_t length = 0;
        for (; val != 0; val /= 10)
        {
            uint32_t c = val % 10;
            pstr[length++] = (char)(c + '0');
        }

        // exchange
        char* phead = pstr;
        char* ptail = pstr + length - 1;
        while (phead < ptail)
        {
            *phead ^= *ptail;
            *ptail ^= *phead;
            *phead ^= *ptail;
            ++phead;
            --ptail;
        }
        return length;
    }
}


template<typename T>
T str2num(std::string const& p)
{
    T val;
    detail::str2num(val, p);
    return val;
}


inline std::string num2str(uint32_t val)
{
    std::string res(std::numeric_limits<uint32_t>::digits10 + 2, 0);
    size_t length = detail::num2str(&res[0], val);
    res.resize(length);
    return res;
}


inline std::string num2str(int32_t val)
{
    std::string res(std::numeric_limits<int32_t>::digits10 + 2, 0);
    size_t length = 0;
    if (val < 0)
    {
        res[0] = '-';
        length = detail::num2str(&res[1], (uint32_t)(-val)) + 1;
    }
    else
    {
        length = detail::num2str(&res[0], (uint32_t)val);
    }
    res.resize(length);
    return res;
}


inline bool is_letter(std::string const& str)
{
    if (str.empty())
    {
        return false;
    }

    size_t length = str.size();
    for (size_t i = 0; i < length; ++i)
    {
        char c = str[i] | 0x20;
        if (c < 'a' || c > 'z')
        {
            return false;
        }
    }

    return true;
}


inline bool is_number(std::string const& str)
{
    if (str.empty())
    {
        return false;
    }

    size_t length = str.size();
    for (size_t i = 0; i < length; ++i)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            return false;
        }
    }
    return true;
}


inline bool is_symbol(std::string const& str)
{
    if (str.empty())
    {
        return false;
    }

    size_t length = str.size();
    for (size_t i = 0; i < length; ++i)
    {
        char c = str[i];
        if ((c < 0) ||
            (c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z'))
        {
            return false;
        }
    }
    return true;
}


inline char* truncate_line(size_t& length, char* const psrc)
{
    char* ptr = psrc;
    char c    = *ptr;
    while (c != '\n' && c != '\0')
    {
        ptr++;
        c = *ptr;
    }
    length = ptr - psrc;
    if (c == '\0') // string end
    {
        return NULL;
    }

    if ((ptr > psrc) && (*(ptr - 1) == '\r'))
    {
        length--;
        *(ptr - 1) = '\0';
    }
    else
    {
        *ptr = '\0';
    }
    return ptr + 1;
}

inline bool greater_integer_string(std::string const& smaller, std::string const& greater)
{
    uint32_t na = str2num<uint32_t>(smaller);
    uint32_t nb = str2num<uint32_t>(greater);
    return na < nb;
}


#endif
