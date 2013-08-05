#ifndef TIME_RULER_H_
#define TIME_RULER_H_

#include <windows.h>
#include <stdio.h>

class TimeRuler
{
public:
	TimeRuler()
	{
		m_start = GetTickCount();
	}

	~TimeRuler()
	{
		_show("[live time]");
	}

	void show()
	{
		_show("[time elapsed]");
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
};

#endif