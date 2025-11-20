#pragma once
#include <windows.h>

void MonitorProcesses();
BOOL SuspendProcess(DWORD pid);
BOOL UniversalResumeProcess(DWORD pid);
void TrimProcessMemory(DWORD pid);
BOOL IsUWPProcess(const char* exeName);
BOOL IsExcludedPermanent(const char* exeName);
BOOL IsExcludedDynamic(const char* exeName);
void AddToExcludedDynamic(const char* exeName);
BOOL IsProcessHung(DWORD pid);
