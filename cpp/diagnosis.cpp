#include "diagnosis.h"

#include <windows.h>
#include <RestartManager.h>
#include <iostream>

static void getProcName(std::vector<std::string> &appList, const UINT nProcInfo, RM_PROCESS_INFO const* pRgpi) {
    const UINT appBufLen = CCH_RM_MAX_APP_NAME * 2 + 1;
    char appName[appBufLen];

    for (UINT i = 0; i < nProcInfo; i++) {
        WideCharToMultiByte(CP_ACP, 0, pRgpi[i].strAppName, _TRUNCATE, appName, appBufLen, NULL, NULL);
        // check process creation time
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pRgpi[i].Process.dwProcessId);
        if (hProcess == NULL) {
            continue;
        }

        FILETIME ftCreate, ftExit, ftKernel, ftUser;
        if (GetProcessTimes(hProcess, &ftCreate, &ftExit, &ftKernel, &ftUser) &&
            CompareFileTime(&pRgpi[i].Process.ProcessStartTime, &ftCreate) == 0) {
            appList.push_back(appName);
        }
        CloseHandle(hProcess);
    }
}


size_t Diagnosis::whoUseMe(std::vector<std::string>& appList, std::string const& oneFilePath) {
    DWORD dwSession = 0;
    WCHAR szSessionKey[CCH_RM_SESSION_KEY + 1] = { 0 };

    UINT nProcInfoNeeded = 0;
    UINT nProcInfo = 0;
    RM_PROCESS_INFO* pRgpi = NULL;
    DWORD dwReason;

    DWORD dwError = 0;
    wchar_t* pwcs = NULL;
    char* pApp = NULL;
    try {
        dwError = RmStartSession(&dwSession, 0, szSessionKey);
        if (dwError != ERROR_SUCCESS) {
            throw std::runtime_error("fail to start restart manager.");
        }
        // convert char string to wchar string
        pwcs = new wchar_t[oneFilePath.size() + 1]();
        MultiByteToWideChar(CP_ACP, 0, oneFilePath.c_str(), oneFilePath.size(), pwcs, oneFilePath.size() + 1);

        LPCWSTR supervisedFiles[] = {pwcs};
        dwError = RmRegisterResources(dwSession, 1, supervisedFiles, 0, NULL, 0, NULL);
        if (dwError != ERROR_SUCCESS) {
            std::string msg("fail to register resources. system error code: ");
            msg += std::to_string(dwError) + ", refer to: http://msdn.microsoft.com/en-us/library/windows/desktop/aa373663(v=vs.85).aspx";
            throw std::runtime_error(msg);
        }

        // first try to get process list max 5
        nProcInfo = 5;
        pRgpi = new RM_PROCESS_INFO[nProcInfo];
        dwError = RmGetList(dwSession, &nProcInfoNeeded, &nProcInfo, pRgpi, &dwReason);

        // there are more than 5 process using the file, then get them all
        if (dwError == ERROR_MORE_DATA) {
            // get list of all the process info
            delete[] pRgpi;
            nProcInfo = nProcInfoNeeded;
            pRgpi = new RM_PROCESS_INFO[nProcInfo];
            dwError = RmGetList(dwSession, &nProcInfoNeeded, &nProcInfo, pRgpi, &dwReason);
        }

        if (dwError != ERROR_SUCCESS) {
            std::string msg("fail to get process list. system error code: ");
            msg += std::to_string(dwError) + ", refer to: http://msdn.microsoft.com/en-us/library/windows/desktop/aa373661(v=vs.85).aspx";
            throw std::runtime_error(msg);
        }
        getProcName(appList, nProcInfo, pRgpi);
        RmEndSession(dwSession);
    }
    catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
        RmEndSession(dwSession);
    }
    delete[] pRgpi;
    delete[] pwcs;
    delete[] pApp;

    return nProcInfoNeeded;
}
