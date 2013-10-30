/**
progressbar style:
 23 % [==========                       ]
*/

#ifndef PROGRESS_BAR_H_
#define PROGRESS_BAR_H_

#include <stdio.h>
#include <string.h>
#include <vector>

#ifdef _WIN32
    #define snprintf _snprintf
#endif

class ProgressBar
{
    static const size_t HEAD_LENGTH = 6;

    size_t _prev_percent;       // previous percent
    size_t _total;              // total work number
    size_t _total_bar_length;   // console bar length
    std::vector<char> _buf;     // bar string buffer

    char   _finish_style;
    char   _unfinish_style;

public:
    ProgressBar(size_t range = 100, size_t bar_length = 80,
        char finish_style = '|', char unfinish_style = '-') :
        _prev_percent((size_t)-1),
        _total(0),
        _total_bar_length(0),
        _finish_style(finish_style),
        _unfinish_style(unfinish_style)
    {
        init(range, bar_length);
    }

    void init(size_t range, size_t bar_length = 80)
    {
        _prev_percent = (size_t)-1;
        _total = range > 1 ? range : 1;

        _buf.resize(HEAD_LENGTH + bar_length + 2); // 2 = "]\0"
        _total_bar_length = bar_length;

        printf("\ntotal: %Iu\n", _total);
    }

    void update(size_t num)
    {
        // compute current percentage
        size_t curr_percent = 100 * num / _total;
        if (curr_percent == _prev_percent)
        {
            return;
        }
        // record current percentage
        _prev_percent = curr_percent;

        // write the percentage
        char* pbar = _buf.data();
        int count = snprintf(pbar, HEAD_LENGTH + 1, "%3Iu%% [", curr_percent);
        pbar += count;

        // write the finish bar
        size_t bar_len = curr_percent * _total_bar_length / 100;
        memset(pbar, _finish_style, bar_len);
        pbar += bar_len;

        // write the unfinish bar
        bar_len = _total_bar_length - bar_len;
        memset(pbar, _unfinish_style, bar_len);
        pbar += bar_len;

        // write "]"
        pbar[0] = ']';
        pbar[1] = 0;

        printf("\r%s", _buf.data());
        fflush(stdout);
    }

    void finish()
    {
        update(_total);
        printf("\n");
    }
};

#endif