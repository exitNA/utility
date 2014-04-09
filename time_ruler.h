#ifndef TIME_RULER_H_
#define TIME_RULER_H_

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
        QueryPerformanceCounter(&_start);
    }

    ~TimeRuler()
    {
        if (_show_mode == AutoShow)
        {
            _show("[live time]");
        }
    }

    void show()
    {
        _show("[time elapsed]");
    }

	void reset()
	{
        QueryPerformanceCounter(&_start);
	}
	
    inline size_t elapsed_ms()
    {
        QueryPerformanceCounter(&_end);
		LARGE_INTEGER elapsed;
		elapsed.QuadPart = _end.QuadPart - _start.QuadPart;
		elapsed.QuadPart *= 1000; // s: *1, ms: *1000, microseconds: *1000000
		elapsed.QuadPart /= _freq.QuadPart;
		return (size_t)elapsed.QuadPart;
    }

private:
    inline void _show(char const * const log)
    {
        size_t elapsed = elapsed_ms();
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
    LARGE_INTEGER _freq;
    LARGE_INTEGER _start;
    LARGE_INTEGER _end;

    Mode  _show_mode;
};

#endif