#ifndef LOG_H
#define LOG_H

#include <windows.h>
#include <string>
#include "color_print.h"


#define MAX_LOG_BUFFER_SIZE 65536
#define MAX_ONE_LOG_LENGTH  1024


#define _log_print(color, level, fmt, ...)                                  \
{                                                                           \
    static char one_log_buf[MAX_ONE_LOG_LENGTH];                            \
    SYSTEMTIME lt;                                                          \
    GetLocalTime(&lt);                                                      \
    int writen_len = _snprintf(one_log_buf, MAX_ONE_LOG_LENGTH,             \
        "%02d:%02d:%02d:%03d-%s>["#level"] " fmt "\n",                      \
        lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds,                 \
        __FUNCTION__,                                                       \
        __VA_ARGS__);                                                       \
    printf(color, "%s", one_log_buf);                                       \
    Logger::instance.write(one_log_buf, writen_len);                        \
}

#define log_cyan(fmt, ...)    _log_print(ConsoleColor::Cyan,   info,    fmt, __VA_ARGS__)
#define log_white(fmt, ...)   _log_print(ConsoleColor::White,  info,    fmt, __VA_ARGS__)
#define log_info(fmt, ...)    _log_print(ConsoleColor::Green,  info,    fmt, __VA_ARGS__)
#define log_warning(fmt, ...) _log_print(ConsoleColor::Yellow, warning, fmt, __VA_ARGS__)
#define log_error(fmt, ...)   _log_print(ConsoleColor::Red,    error,   fmt, __VA_ARGS__)


class Logger
{
public:
    static Logger instance;

private:
    FILE*   _pfile;
    size_t  _used_length;
    char    _file_buffer[MAX_LOG_BUFFER_SIZE];
    char    _line_buf[MAX_ONE_LOG_LENGTH];

public:

    inline void write(char const* pstr, size_t length)
    {
        if (length + _used_length >= MAX_LOG_BUFFER_SIZE)
        {
            fwrite(_file_buffer, 1, _used_length, _pfile);
            //fflush(_pfile);
            _used_length = 0;
        }
        memcpy(_file_buffer + _used_length, pstr, length);
        _used_length += length;
    }

    void write(char const* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        int writen_len = _snprintf(_line_buf, MAX_ONE_LOG_LENGTH, fmt, args);
        va_end(args);

        write(_line_buf, writen_len);
    }

private:
    Logger() :
        _pfile(NULL),
        _used_length(0)
    {
        create_file();
    }

    ~Logger()
    {
        if (_pfile != NULL)
        {
            fwrite(_file_buffer, 1, _used_length, _pfile);
            fclose(_pfile);
            _pfile = NULL;
        }
    }

    void create_file()
    {
        // get current process directory and create log file path in the same
        // directory
        char const* pfile_name = "log.txt";

        size_t file_name_length = strlen(pfile_name);
        size_t path_buf_length = GetCurrentDirectoryA(0, NULL);

        std::string dir(path_buf_length + file_name_length, '*');

        GetCurrentDirectoryA(path_buf_length, &dir[0]);
        dir[path_buf_length - 1] = '\\';
        memcpy(&dir[path_buf_length], pfile_name, file_name_length);

        _pfile = fopen(dir.c_str(), "wb");
        if (_pfile == NULL)
        {
            throw "fail to open log file.";
        }
    }

};

// Loger Loger::instance;  // put it in a cpp file to instantiate the loger

#endif