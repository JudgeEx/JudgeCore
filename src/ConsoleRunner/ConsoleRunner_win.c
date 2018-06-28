#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <Shlwapi.h>
#include <Psapi.h>
#include <stdio.h>
#include "make_output.h"
#include "config_loader.h"
DWORD ProcessError(DWORD dwErrorCode)
{
    LPTSTR strErrorMessage = NULL;
    bool bRet = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwErrorCode,
        LANG_USER_DEFAULT,
        strErrorMessage,
        0,
        NULL);
    if (bRet && strErrorMessage != NULL)
        fprintf(stderr, "Error [%x]: %s\n", dwErrorCode, strErrorMessage);
    else
        fprintf(stderr, "Error [%x]\n", dwErrorCode);
    LocalFree(strErrorMessage);
    return dwErrorCode;
}
LPCSTR ReadConfigFile(LPCTSTR strConfigPath)
{
    BOOL bRet = PathFileExists(strConfigPath);
    if (!bRet) return NULL;
    HANDLE hFile = CreateFile(
        strConfigPath,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (hFile == INVALID_HANDLE_VALUE) return NULL;
    LARGE_INTEGER liFileLength;
    bRet = GetFileSizeEx(hFile, &liFileLength);
    if (!bRet) return NULL;
    LPBYTE lpBuffer = malloc(sizeof(BYTE) * (liFileLength.LowPart + 1));
    memset(lpBuffer, 0, sizeof(BYTE) * liFileLength.LowPart + 1);
    bRet = ReadFile(hFile, lpBuffer, liFileLength.LowPart, 0, NULL);
    if (!bRet) return NULL;
    return (LPCSTR)lpBuffer;
}
DWORD ExecuteProcess(LPTSTR strCommand, LPSTARTUPINFO lpStartupInfo, LPPROCESS_LIMIT lpLimit, LPPROCESS_RESULT lpResult)
{
    PROCESS_INFORMATION ProcessInfo;
    PROCESS_MEMORY_COUNTERS ProcessMemoryCounters;
    LARGE_INTEGER StartingTime, EndingTime, Frequency;
    memset(&ProcessInfo, 0, sizeof(ProcessInfo));
    if (!CreateProcess(NULL, strCommand, NULL, NULL, FALSE, 0, NULL, NULL, lpStartupInfo, &ProcessInfo)) return ProcessError(GetLastError());
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&StartingTime);
    DWORD ret = WaitForSingleObject(ProcessInfo.hProcess, lpLimit->ExecutionTime + (lpLimit->ExecutionTime >> 2));
    QueryPerformanceCounter(&EndingTime);
    TerminateProcess(ProcessInfo.hProcess, -1);
    if (ret == WAIT_FAILED) ProcessError(GetLastError());
    GetExitCodeProcess(ProcessInfo.hProcess, &lpResult->ReturnCode);
    GetProcessMemoryInfo(ProcessInfo.hProcess, &ProcessMemoryCounters, sizeof(ProcessMemoryCounters));
    CloseHandle(ProcessInfo.hProcess);
    lpResult->ExecutionTime = (EndingTime.QuadPart - StartingTime.QuadPart) * 1000000 / Frequency.QuadPart;
    lpResult->PeakWorkingSetSize = ProcessMemoryCounters.PeakWorkingSetSize;
    return ret;
}
DWORD RunProgram(LPTSTR strCommand, const LPPROCESS_LIMIT lpLimit, LPPROCESS_RESULT lpResult)
{
    STARTUPINFO StartupInfo;
    memset(&StartupInfo, 0, sizeof(StartupInfo));
    StartupInfo.cb = sizeof(StartupInfo);
    return ExecuteProcess(strCommand, &StartupInfo, lpLimit, lpResult);
}
DWORD RunProgramWithRedirectedStdIO(LPTSTR strCommand, const LPPROCESS_LIMIT lpLimit, LPPROCESS_RESULT lpResult)
{
    STARTUPINFO StartupInfo;
    memset(&StartupInfo, 0, sizeof(StartupInfo));
    StartupInfo.cb = sizeof(StartupInfo);
    return ExecuteProcess(strCommand, &StartupInfo, lpLimit, lpResult);
}
int main(int argc, TCHAR *argv[])
{
    if (argc < 2) return fputs("A config file is required.", stderr), -1;
    LPCSTR strConfig = ReadConfigFile(argv[1]);
    if (strConfig == NULL) return ProcessError(GetLastError()), -1;
    LoadConfig(strConfig);
    struct PROCESS_LIMIT limit;
    struct PROCESS_RESULT result;
    limit.ExecutionTime = config.TimeLimit;
    DWORD ret = config.Redirect ? RunProgramWithRedirectedStdIO(config.CommandLine, &limit, &result) : RunProgram(config.CommandLine, &limit, &result);
    puts(GetOutput(&result));
    return 0;
}
