#ifndef LOG_H
#define LOG_H

#if !defined(_WIN32) && !defined(_WIN64)
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
    int writen_len = _snprintf_s(one_log_buf, MAX_ONE_LOG_LENGTH, _TRUNCATE \
        , "\n[%s] %02d:%02d:%02d:%03d %s> " fmt                             \
        , level                                                             \
        , lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds                \
        , __FUNCTION__                                                      \
        , __VA_ARGS__);                                                     \
    printf(color, "%s", one_log_buf); fflush(stdout);                       \
    Logger::instance().write(one_log_buf, writen_len);                      \
}


#define log_cyan(fmt, ...)    _log_print(ConsoleColor::Cyan,   " info", fmt, __VA_ARGS__)
#define log_msg(fmt, ...)     _log_print(ConsoleColor::Gray,   " info", fmt, __VA_ARGS__)
#define log_info(fmt, ...)    _log_print(ConsoleColor::Green,  " info", fmt, __VA_ARGS__)
#define log_warning(fmt, ...) _log_print(ConsoleColor::Yellow, " warn", fmt, __VA_ARGS__)
#define log_error(fmt, ...)   _log_print(ConsoleColor::Red,    "error", fmt, __VA_ARGS__)


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
        int writen_len = _snprintf_s(_line_buf, MAX_ONE_LOG_LENGTH, _TRUNCATE, fmt, args);
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
//        size_t path_len = GetCurrentDirectoryA(0, NULL); // get direcotory name length
//        std::string dir(path_len, '*');
//        GetCurrentDirectoryA(path_len, &dir[0]);
//        dir[path_len - 1] = '\\';
//        dir += LOG_ROOT_DIRECTORY;
        char dir[MAX_PATH + 1] = {0};
        GetModuleFileNameA(NULL, dir, MAX_PATH);
        char* pExeName = strrchr(dir, '\\'); pExeName++;
        char* pEnd = strrchr(pExeName, '.'); *pEnd = 0;

        // get current runing directory
        std::string logPath(dir, pExeName);
        logPath += LOG_ROOT_DIRECTORY;

        // get current local time
        SYSTEMTIME lt;
        GetLocalTime(&lt);

        char pathName[MAX_PATH];   // path buffer
        // sub log directory name
        _snprintf_s(pathName, MAX_PATH, _TRUNCATE, "%04d-%02d-%02d\\", lt.wYear, lt.wMonth, lt.wDay);
        logPath += pathName;
        os::mkdirs(logPath.c_str());

        // log file name
        _snprintf_s(pathName, MAX_PATH, _TRUNCATE, "%s.log", pExeName);
        logPath += pathName;

        errno_t err = fopen_s(&_pfile, logPath.c_str(), "a");
        if (err != 0)
        {
            throw std::runtime_error("fail to open log file.");
        }
    }
};

#endif