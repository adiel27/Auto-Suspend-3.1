#include "process_utils.h"
#include "fallback_utils.h"
#include "window_utils.h"
#include "cpu_utils.h"
#include "log_utils.h"
#include "network_utils.h"
#include <tlhelp32.h>
#include <psapi.h>
#include <string.h>

// ==== Core Actions ====

// Suspend semua thread dalam proses
BOOL SuspendProcess(DWORD pid) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (snap == INVALID_HANDLE_VALUE) return FALSE;

    THREADENTRY32 te32 = { sizeof(te32) };
    if (Thread32First(snap, &te32)) {
        do {
            if (te32.th32OwnerProcessID == pid) {
                HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
                if (hThread) {
                    SuspendThread(hThread);
                    CloseHandle(hThread);
                }
            }
        } while (Thread32Next(snap, &te32));
    }
    CloseHandle(snap);
    return TRUE;
}

// Resume semua thread dalam proses
BOOL UniversalResumeProcess(DWORD pid) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (snap == INVALID_HANDLE_VALUE) return FALSE;

    THREADENTRY32 te32 = { sizeof(te32) };
    BOOL resumed = FALSE;
    if (Thread32First(snap, &te32)) {
        do {
            if (te32.th32OwnerProcessID == pid) {
                HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
                if (hThread) {
                    while (ResumeThread(hThread) > 0);
                    CloseHandle(hThread);
                    resumed = TRUE;
                }
            }
        } while (Thread32Next(snap, &te32));
    }
    CloseHandle(snap);
    return resumed;
}

// Trim working set RAM
void TrimProcessMemory(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA | PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (hProcess) {
        SetProcessWorkingSetSize(hProcess, -1, -1);
        CloseHandle(hProcess);
    }
}

// ==== UWP Check ====
BOOL IsUWPProcess(const char* exeName) {
    return (_stricmp(exeName, "ApplicationFrameHost.exe") == 0);
}

// ==== Main Monitor Loop ====
void MonitorProcesses() {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32 pe32 = { sizeof(pe32) };
    if (Process32First(snap, &pe32)) {
        do {
            // Filter awal
            if (IsExcludedPermanent(pe32.szExeFile)) continue;
            if (IsExcludedDynamic(pe32.szExeFile)) continue;
            if (IsUWPProcess(pe32.szExeFile)) continue;
            if (!HasVisibleWindow(pe32.th32ProcessID)) continue;

            // User activity check
            if (HasUserActivity(pe32.th32ProcessID)) {
                UniversalResumeProcess(pe32.th32ProcessID);
                LogAction("Resumed (user returned)", pe32.szExeFile, pe32.th32ProcessID);
                continue;
            }
            // Anti gagal download
            if (IsNetworkActive(pe32.th32ProcessID)) {
            LogAction("Skipped suspend (network active)", pe32.szExeFile, pe32.th32ProcessID);
            continue;
            }

            // CPU usage check
            double cpu = GetCPUUsage(pe32.th32ProcessID, 1.0);
            BOOL idle = cpu < 5.0;

            if (idle) {
                SuspendProcess(pe32.th32ProcessID);
                TrimProcessMemory(pe32.th32ProcessID);
                LogAction("Suspended + trimmed RAM", pe32.szExeFile, pe32.th32ProcessID);

                // Fallback check → resume sekali, kalau hung → excluded dinamis
                UniversalResumeProcess(pe32.th32ProcessID);
                if (IsProcessHung(pe32.th32ProcessID)) {
                    AddToExcludedDynamic(pe32.szExeFile);
                    LogAction("Process hung → added to dynamic excluded",
                              pe32.szExeFile, pe32.th32ProcessID);
                }
            } else {
                UniversalResumeProcess(pe32.th32ProcessID);
                LogAction("Resumed (CPU active)", pe32.szExeFile, pe32.th32ProcessID);
            }
        } while (Process32Next(snap, &pe32));
    }
    CloseHandle(snap);
}
