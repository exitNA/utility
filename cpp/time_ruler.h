#ifndef TIME_RULER_H_
#define TIME_RULER_H_
#include <stdio.h>
#include <windows.h>

class TimeRuler
{
public:
    enum Mode {AutoShow, NoAutoShow};

public:
    explicit TimeRuler(Mode type = AutoShow) :
        _show_mode(type)
    {
        QueryPerformanceFrequency(&_freq);
        reset();
    }

    ~TimeRuler()
    {
        if (_show_mode == AutoShow)
        {
            _show("[live time]");
        }
    }

    inline void show()
    {
        _show("[time elapsed]");
    }

    inline void reset()
    {
        QueryPerformanceCounter(&_start);
    }
    
    inline double elapsed_ms()
    {
        QueryPerformanceCounter(&_end);
        double elapsed = (double)(_end.QuadPart - _start.QuadPart);
        elapsed *= 1000; // s: *1, ms: *1000, microseconds: *1000000
        elapsed /= _freq.QuadPart;
        return elapsed;
    }

private:
    inline void _show(char const * const log)
    {
        double elapsed = elapsed_ms();
        if (elapsed >= 1000 * 60)
        {
            elapsed /= 1000;
            printf("\n%s = %d min + %d s", log, (unsigned)(elapsed / 60), (unsigned)elapsed % 60);
        }
        else if (elapsed >= 1000)
        {
            printf("\n%s = %d s + %d ms", log, (unsigned)(elapsed / 1000), (unsigned)elapsed % 1000);
        }
        else
        {
            printf("\n%s = %.3f ms", log, elapsed);
        }
    }

private:
    Mode  _show_mode;
    LARGE_INTEGER _freq;
    LARGE_INTEGER _start;
    LARGE_INTEGER _end;
};

#endif