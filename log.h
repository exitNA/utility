#ifndef LOG_H
#define LOG_H

#ifndef _WIN32
    #error "Logger only works on windows platform"
#endif

#include <string>
#include <windows.h>
#include "color_print.h"
#include "file_system.h"

////////////////////////////////////////////////////////////////////////////////
// use mode switch
// if define LOGGER_AS_DLL, it will used as dll mode, otherwise for normal use
////////////////////////////////////////////////////////////////////////////////

#ifdef LOGGER_AS_DLL
    #ifdef LOGGER_EXPORTS
        #define LOGGER_API __declspec(dllexport)
    #else
        #define LOGGER_API __declspec(dllimport)
    #endif
#else
    #define LOGGER_API
#endif
////////////////////////////////////////////////////////////////////////////////

#define MAX_LOG_BUFFER_SIZE 4096
#define MAX_ONE_LOG_LENGTH  1024
#define LOG_ROOT_DIRECTORY  "log\\"


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
    Logger::instance().write(one_log_buf, writen_len);                      \
}

#define log_cyan(fmt, ...)    _log_print(ConsoleColor::Cyan,   info,    fmt, __VA_ARGS__)
#define log_white(fmt, ...)   _log_print(ConsoleColor::White,  info,    fmt, __VA_ARGS__)
#define log_info(fmt, ...)    _log_print(ConsoleColor::Green,  info,    fmt, __VA_ARGS__)
#define log_warning(fmt, ...) _log_print(ConsoleColor::Yellow, warning, fmt, __VA_ARGS__)
#define log_error(fmt, ...)   _log_print(ConsoleColor::Red,    error,   fmt, __VA_ARGS__)


class Logger
{
private:
    FILE*   _pfile;
    size_t  _used_length;
    char    _file_buffer[MAX_LOG_BUFFER_SIZE];
    char    _line_buf[MAX_ONE_LOG_LENGTH];

public:
    LOGGER_API static Logger& instance()
    {
        static Logger single;
        return single;
    }

    LOGGER_API inline void write(char const* pstr, size_t length)
    {
        if (length + _used_length >= MAX_LOG_BUFFER_SIZE)
        {
            fwrite(_file_buffer, 1, _used_length, _pfile);
            fflush(_pfile);
            _used_length = 0;
        }
        memcpy(_file_buffer + _used_length, pstr, length);
        _used_length += length;
    }

    LOGGER_API void write(char const* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        int writen_len = _snprintf(_line_buf, MAX_ONE_LOG_LENGTH, fmt, args);
        va_end(args);
        write(_line_buf, writen_len);
    }

private:
    LOGGER_API Logger() :
        _pfile(NULL),
        _used_length(0)
    {
        create_file();
    }

    LOGGER_API ~Logger()
    {
        if (_pfile != NULL)
        {
            fwrite(_file_buffer, 1, _used_length, _pfile);
            fclose(_pfile);
            _pfile = NULL;
        }
    }

    LOGGER_API Logger(Logger const& val);

    LOGGER_API Logger& operator = (Logger const& val);

    void create_file()
    {
        // log file root directory
        size_t path_len = GetCurrentDirectoryA(0, NULL);
        std::string dir(path_len, '*');
        GetCurrentDirectoryA(path_len, &dir[0]);
        dir[path_len - 1] = '\\';
        dir += LOG_ROOT_DIRECTORY;

        // get current local time
        SYSTEMTIME lt;
        GetLocalTime(&lt);

        char path_name[MAX_PATH];   // path buffer

        // sub log directory name
        _snprintf(path_name, MAX_PATH - 1, "%04d-%02d-%02d\\", lt.wYear, lt.wMonth, lt.wDay);
        dir += path_name;
        os::mkdirs(dir.c_str());

        // log file name
        _snprintf(path_name, MAX_PATH - 1, "log-%02d-%02d-%02d.txt", lt.wHour, lt.wMinute, lt.wSecond);
        dir += path_name;

        _pfile = fopen(dir.c_str(), "wb");
        if (_pfile == NULL)
        {
            throw std::runtime_error("fail to open log file.");
        }
    }
};

#endif