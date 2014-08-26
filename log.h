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


#define log_cyan(fmt, ...)    Logger::obj().log(ConsoleColor::Cyan,   " info", __FUNCTION__, fmt, __VA_ARGS__)
#define log_msg(fmt, ...)     Logger::obj().log(ConsoleColor::Gray,   "  msg", __FUNCTION__, fmt, __VA_ARGS__)
#define log_info(fmt, ...)    Logger::obj().log(ConsoleColor::Green,  " info", __FUNCTION__, fmt, __VA_ARGS__)
#define log_warning(fmt, ...) Logger::obj().log(ConsoleColor::Yellow, " warn", __FUNCTION__, fmt, __VA_ARGS__)
#define log_error(fmt, ...)   Logger::obj().log(ConsoleColor::Red,    "error", __FUNCTION__, fmt, __VA_ARGS__)


class Logger {
    SYSTEMTIME _today;
    FILE*      _pfile;
    size_t     _used_length;
    char       _file_buffer[MAX_LOG_BUFFER_SIZE];

public:
    LOGGER_API static Logger& obj() {
        static Logger single;
        return single;
    }

    LOGGER_API inline void log(
        ConsoleColor::Type color
        , char const* level
        , char const* fun_name
        , char const* fmt
        , ...) {
        // single log msg buffer
        char line_buf[MAX_ONE_LOG_LENGTH];

        // get current log time
        SYSTEMTIME now;
        GetLocalTime(&now);

        // write log header
        int writen_len = _snprintf_s(line_buf, MAX_ONE_LOG_LENGTH, _TRUNCATE
            , "\n[%s] %02d:%02d:%02d:%03d %s> ", level
            , now.wHour, now.wMinute, now.wSecond, now.wMilliseconds
            , fun_name);

        // write log content
        va_list args;
        va_start(args, fmt);
        writen_len += vsnprintf_s(line_buf + writen_len
            , MAX_ONE_LOG_LENGTH - writen_len, _TRUNCATE
            , fmt, args);
        va_end(args);

        // console print
        printf(color, "%s", line_buf); fflush(stdout);

        // write to log file
        write(now, line_buf, writen_len);
    }

private:
    LOGGER_API Logger()
        : _pfile(NULL)
        , _used_length(0) {
        open_new_file();
    }

    LOGGER_API ~Logger() {
        close_file();
    }

    LOGGER_API Logger(Logger const& val);

    LOGGER_API Logger& operator = (Logger const& val);

private:
    inline void write(SYSTEMTIME const& now, char const* pstr, size_t length) {
        if (is_new_day(now)) {
            open_new_file(&now);
        }

        if (length + _used_length >= MAX_LOG_BUFFER_SIZE) {
            flush();
        }

        memcpy(_file_buffer + _used_length, pstr, length);
        _used_length += length;
    }

    inline bool is_new_day(SYSTEMTIME const& date) {
        return _today.wDay != date.wDay;
    }

    inline void flush() {
        fwrite(_file_buffer, 1, _used_length, _pfile);
        fflush(_pfile);
        _used_length = 0;
    }

    inline void close_file() {
        if (_pfile != NULL) {
            flush(); // write yesterday's log to file
            fclose(_pfile);
            _pfile = NULL;
        }
    }

    void open_new_file(SYSTEMTIME const* now = NULL) {
        close_file();

        // get current runing directory
        char exePath[MAX_PATH + 1] = { 0 };
        GetModuleFileNameA(NULL, exePath, MAX_PATH);
        char* pExeName = strrchr(exePath, '\\') + 1;
        char* pEnd = strrchr(pExeName, '.');
        *pEnd = 0;
        std::string logPath(exePath, pExeName);
        logPath += LOG_ROOT_DIRECTORY;

        // get current local time
        if (now == NULL) {
            GetLocalTime(&_today);
        } else {
            memcpy(&_today, now, sizeof(SYSTEMTIME));
        }

        // log file directory
        char pathName[MAX_PATH + 1];
        _snprintf_s(pathName, MAX_PATH, _TRUNCATE, "%04d-%02d-%02d\\"
            , _today.wYear, _today.wMonth, _today.wDay);
        logPath += pathName;
        os::mkdirs(logPath.c_str());

        // log file name
        _snprintf_s(pathName, MAX_PATH, _TRUNCATE, "%s.log", pExeName);
        logPath += pathName;

        errno_t err = fopen_s(&_pfile, logPath.c_str(), "a");
        if (err != 0) {
            throw std::runtime_error("fail to open log file.");
        }
    }
};

#endif