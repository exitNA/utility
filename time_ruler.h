#ifndef TIME_RULER_H_
#define TIME_RULER_H_

#include <windows.h>

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

    size_t elapsed_ms()
    {
        DWORD current = GetTickCount();
        if (current <= m_start)
        {
            return 0;
        }
        else
        {
            return current - m_start;
        }
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