#ifndef COLOR_PRINTF_H_
#define COLOR_PRINTF_H_

#include <stdarg.h>
#include <Windows.h>

struct ConsoleColor
{
    /// \brief This enum type represent all the color which could be used.
    enum Type
    {
        Default     = 9,
        DarkRed     = 0,   // the start element
        DarkGreen,
        DarkBlue,
        Red,
        Green,
        Blue,
        DarkCyan,
        DarkMagenta,
        DarkYellow,
        Gray,
        Cyan,
        Magenta,
        Yellow,
        White               // the end element
    };
};

namespace detail
{
    struct ColorAttribute
    {
        ConsoleColor::Type color;
        WORD  value;
    };


    // the "colorTable" should has the same sequence as "enum ConsoleColor"
    const ColorAttribute colorTable[] =
    {
        {ConsoleColor::DarkRed,     FOREGROUND_RED   },
        {ConsoleColor::DarkGreen,   FOREGROUND_GREEN },
        {ConsoleColor::DarkBlue,    FOREGROUND_BLUE  },
        {ConsoleColor::Red,         FOREGROUND_RED   | FOREGROUND_INTENSITY },
        {ConsoleColor::Green,       FOREGROUND_GREEN | FOREGROUND_INTENSITY },
        {ConsoleColor::Blue,        FOREGROUND_BLUE  | FOREGROUND_INTENSITY },
        {ConsoleColor::DarkCyan,    FOREGROUND_BLUE  | FOREGROUND_GREEN     },
        {ConsoleColor::DarkMagenta, FOREGROUND_BLUE  | FOREGROUND_RED       },
        {ConsoleColor::DarkYellow,  FOREGROUND_RED   | FOREGROUND_GREEN     },
        {ConsoleColor::Gray,        FOREGROUND_BLUE  | FOREGROUND_RED       | FOREGROUND_GREEN },
        {ConsoleColor::Cyan,        FOREGROUND_BLUE  | FOREGROUND_GREEN     | FOREGROUND_INTENSITY},
        {ConsoleColor::Magenta,     FOREGROUND_BLUE  | FOREGROUND_RED       | FOREGROUND_INTENSITY},
        {ConsoleColor::Yellow,      FOREGROUND_RED   | FOREGROUND_GREEN     | FOREGROUND_INTENSITY},
        {ConsoleColor::White,       FOREGROUND_BLUE  | FOREGROUND_RED       | FOREGROUND_GREEN | FOREGROUND_INTENSITY}
    };


    inline int vfprintf(ConsoleColor::Type color,
        FILE* pstream, const char* fmt, va_list args)
    {
        if ((ConsoleColor::DarkRed > color) || (ConsoleColor::White < color))
        {
            color = ConsoleColor::Default;
        }

        HANDLE hConsole = NULL;
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo = {0};
        WORD wColor = 0;
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // get current console text's color
        GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
        wColor = colorTable[color - ConsoleColor::DarkRed].value;

        // set console text color
        fflush(stdout);
        SetConsoleTextAttribute(hConsole, wColor);

        // print text
        int num = vfprintf(pstream, fmt, args);

        // restore console text color
        fflush(stdout);
        SetConsoleTextAttribute(hConsole, consoleInfo.wAttributes);

        return num;
    }
}


inline int fprintf(ConsoleColor::Type color, FILE* pstream, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    int num = detail::vfprintf(color, pstream, fmt, args);

    va_end(args);

    return num;
}


// print text with color
inline int printf(ConsoleColor::Type color, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    int num = detail::vfprintf(color, stdout, fmt, args);

    va_end(args);

    return num;
}


#endif
