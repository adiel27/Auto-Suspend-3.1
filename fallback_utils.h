#pragma once
#include <windows.h>

BOOL IsExcludedPermanent(const char* exeName);
BOOL IsExcludedDynamic(const char* exeName);
void AddToExcludedDynamic(const char* exeName);
BOOL IsProcessHung(DWORD pid);
