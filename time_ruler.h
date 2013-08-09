#ifndef TIME_RULER_H_
#define TIME_RULER_H_

#include <windows.h>
#include <stdio.h>
#include <stdint.h>

class TimeRuler
{
public:
    enum Mode {AutoShow, NoAutoShow};

public:
    explicit TimeRuler(Mode type = AutoShow) :
        m_show_mode(type)
    {
        m_start = GetTickCount();
    }

    ~TimeRuler()
    {
        if (m_show_mode == AutoShow)
        {
            _show("[live time]");
        }
    }

    void show()
    {
        _show("[time elapsed]");
    }

    uint64_t elapsed_ms()
    {
        return GetTickCount() - m_start;
    }

private:

    void _show(char const * const log)
    {
        DWORD stop = GetTickCount();
        m_elapsed = stop - m_start;

        if (stop <= m_start)
        {
            m_elapsed = 0;
        }

        if (m_elapsed > 1000)
        {
            printf("%s = %d s + %d ms\n", log, m_elapsed / 1000, m_elapsed % 1000);
        }
        else
        {
            printf("%s = %d ms\n", log, m_elapsed);
        }
    }

private:
    DWORD m_start;
    DWORD m_elapsed;
    Mode  m_show_mode;
};

#endif