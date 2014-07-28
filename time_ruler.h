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

    inline long long elapsed_ms()
    {
        QueryPerformanceCounter(&_end);

        LARGE_INTEGER elapsed;
        elapsed.QuadPart  = _end.QuadPart - _start.QuadPart;
        elapsed.QuadPart *= 1000; // s: *1, ms: *1000, microseconds: *1000000
        elapsed.QuadPart /= _freq.QuadPart;

        return elapsed.QuadPart;
    }

private:
    inline void _show(char const * const log)
    {
        long long elapsed = elapsed_ms();
        if (elapsed >= 1000 * 60)
        {
            elapsed /= 1000;
            printf("%s = %d min + %d s\n", log, elapsed / 60, elapsed % 60);
        }
        else if (elapsed >= 1000)
        {
            printf("%s = %d s + %d ms\n", log, elapsed / 1000, elapsed % 1000);
        }
        else
        {
            printf("%s = %d ms\n", log, elapsed);
        }
    }

private:
    Mode  _show_mode;
    LARGE_INTEGER _freq;
    LARGE_INTEGER _start;
    LARGE_INTEGER _end;
};

#endif