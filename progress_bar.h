#ifndef PROGRESS_BAR_H_
#define PROGRESS_BAR_H_

#include <stdio.h>
#include <string>

class ProgressBar
{
    size_t _length;
//    size_t _resolution;
    size_t _total;
    size_t _curr;
    char   _finish_style;
    char   _unfinish_style;
    std::string _buf;

public:
    ProgressBar()
    {

    }

    void update(size_t num)
    {

    }

    void complete()
    {

    }
};
#endif