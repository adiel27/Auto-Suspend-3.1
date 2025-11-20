#include "cpu_utils.h"
#include <psapi.h>
#include <stdio.h>

double GetCPUUsage(DWORD pid, double intervalSec) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!hProcess) return -1.0;

    FILETIME ftCreation, ftExit, ftKernel1, ftUser1, ftKernel2, ftUser2;
    ULARGE_INTEGER k1, u1, k2, u2;

    if (!GetProcessTimes(hProcess, &ftCreation, &ftExit, &ftKernel1, &ftUser1)) {
        CloseHandle(hProcess);
        return -1.0;
    }
    k1.LowPart = ftKernel1.dwLowDateTime; k1.HighPart = ftKernel1.dwHighDateTime;
    u1.LowPart = ftUser1.dwLowDateTime;   u1.HighPart = ftUser1.dwHighDateTime;

    Sleep((DWORD)(intervalSec * 1000));

    if (!GetProcessTimes(hProcess, &ftCreation, &ftExit, &ftKernel2, &ftUser2)) {
        CloseHandle(hProcess);
        return -1.0;
    }
    k2.LowPart = ftKernel2.dwLowDateTime; k2.HighPart = ftKernel2.dwHighDateTime;
    u2.LowPart = ftUser2.dwLowDateTime;   u2.HighPart = ftUser2.dwHighDateTime;

    CloseHandle(hProcess);

    double delta = (double)((k2.QuadPart - k1.QuadPart) + (u2.QuadPart - u1.QuadPart)) / 10000000.0;

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    int numCPU = sysInfo.dwNumberOfProcessors;

    double usage = (delta / (intervalSec * numCPU)) * 100.0;
    return usage;
}

