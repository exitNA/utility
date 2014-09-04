#ifndef LOG_H
#define LOG_H

#if !defined(_WIN32) && !defined(_WIN64)
    #error "Logger only works on windows platform"
#endif

#include <string>
#include "color_print.h"
#include "file_system.h"

#define MAX_ONE_LOG_LENGTH  (512)
#define LOG_ROOT_DIRECTORY  "log\\"

//////////////////////////////////////////////////////////////////////////
// interface
//////////////////////////////////////////////////////////////////////////
#define log_cyan(fmt, ...)    Logger::obj().log(ConsoleColor::Cyan,   " info", __FUNCTION__, fmt, __VA_ARGS__)
#define log_msg(fmt, ...)     Logger::obj().log(ConsoleColor::Gray,   "  msg", __FUNCTION__, fmt, __VA_ARGS__)
#define log_info(fmt, ...)    Logger::obj().log(ConsoleColor::Green,  " info", __FUNCTION__, fmt, __VA_ARGS__)
#define log_warning(fmt, ...) Logger::obj().log(ConsoleColor::Yellow, " warn", __FUNCTION__, fmt, __VA_ARGS__)
#define log_error(fmt, ...)   Logger::obj().log(ConsoleColor::Red,    "error", __FUNCTION__, fmt, __VA_ARGS__)

#define OPEN_LOG_DEBUG
#ifdef OPEN_LOG_DEBUG
#define log_debug(fmt, ...)   Logger::obj().log(ConsoleColor::DarkGreen, "debug", __FUNCTION__, fmt, __VA_ARGS__)
#else
#define log_debug(fmt, ...)
#endif
//////////////////////////////////////////////////////////////////////////

class Logger {
    SYSTEMTIME _today;
    FILE*      _pfile;
    size_t     _used_length;

public:
    static Logger& obj() {
        static Logger single;
        return single;
    }

    void log (
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
        int writen_len = _snprintf_s(line_buf
            , MAX_ONE_LOG_LENGTH
            , _TRUNCATE
            , "\n[%s] %02d:%02d:%02d:%03d %s> "
            , level
            , now.wHour, now.wMinute, now.wSecond, now.wMilliseconds
            , fun_name);

        // write log content
        va_list args;
        va_start(args, fmt);
        writen_len += vsnprintf_s(line_buf + writen_len
            , MAX_ONE_LOG_LENGTH - writen_len
            , _TRUNCATE
            , fmt, args);
        va_end(args);

        // console print
        printf(color, "%s", line_buf);
//        fflush(stdout);

        // write to log file
        write(now, line_buf, writen_len);
    }

private:
    Logger() : _pfile(NULL) , _used_length(0) {
        open_new_file();
    }

    ~Logger() {
        close_file();
    }

    Logger(Logger const& val);

    Logger& operator = (Logger const& val);

private:
    inline void write(SYSTEMTIME const& now, char const* str, size_t length) {
        if (is_new_day(now)) {
            open_new_file(&now);
        }
        fwrite(str, 1, length, _pfile);
        fflush(_pfile);
    }

    inline bool is_new_day(SYSTEMTIME const& date) {
        return _today.wDay != date.wDay;
    }

    inline void close_file() {
        if (_pfile != NULL) {
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