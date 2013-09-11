#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_

#include <Windows.h>
#include <fstream>
#include <list>
#include <string>
#include <vector>
#include <stdexcept>

namespace os
{
    inline std::vector<std::string> lsdir(std::string const& dir)
    {
        std::string temp_dir(dir + "/*");
        WIN32_FIND_DATAA ffd;
        HANDLE hFind = FindFirstFileA(temp_dir.c_str(), &ffd);

        std::vector<std::string> file_list;
        if (INVALID_HANDLE_VALUE == hFind)
        {
            return file_list;
        }

        // found file
        do
        {
            if (strcmp(ffd.cFileName, ".") != 0 &&
                strcmp(ffd.cFileName, "..") != 0)
            {
                file_list.push_back(ffd.cFileName);
            }
        }
        while (FindNextFileA(hFind, &ffd) != FALSE);
        
        FindClose(hFind);

        return file_list;
    }


    inline bool mkdir(char const* pdir)
    {
        if (CreateDirectoryA(pdir, NULL) == FALSE)
        {
            DWORD error_code = GetLastError();
            if (ERROR_PATH_NOT_FOUND == error_code)
            {
                return false;
            }
        }
        return true;
    }


    inline bool mkdirs(char const* pdir)
    {
        size_t length = strlen(pdir);
        char ptmp[MAX_PATH];
        memcpy(ptmp, pdir, length);
        char c = ptmp[length - 1];
        if (c != '/' && c != '\\')
        {
            ptmp[length++] = '/';
        }
        ptmp[length] = '\0';

        // skip root directory
        char* p_sub_dir = strpbrk(ptmp, "\\/");
        if (p_sub_dir == NULL)
        {
            return false;
        }
        ++p_sub_dir;

        while ((p_sub_dir = strpbrk(p_sub_dir, "\\/")) != NULL)
        {
            *p_sub_dir = '\0';
            if (!mkdir(ptmp))
            {
                return false;
            }
            *p_sub_dir = '/';
            ++p_sub_dir;
        }

        return true;
    }


    inline bool rmdir(char const* pdir)
    {
        if (RemoveDirectoryA(pdir) == FALSE)
        {
            return false;
        }
        return true;
    }


    inline void read_file(std::vector<char>& buf, const char* file_path_name,
        std::ios::openmode mode = std::ios::binary)
    {
        std::ifstream in(file_path_name, mode);
        if (!in.good())
        {
            std::string err_info("fail to open file: ");
            err_info += file_path_name;
            throw std::invalid_argument(err_info);
        }

        std::streamoff src_size = in.seekg(0, std::ios::end).tellg();
        if (src_size == -1)
        {
            std::string err_info("fail to get file size: ");
            err_info += file_path_name;
            throw std::invalid_argument(err_info);
        }

        buf.resize((size_t)src_size + 1);
        in.seekg(0).read(&buf[0], src_size);
        buf[(size_t)src_size] = '\0';
    }
};

#endif