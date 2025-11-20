#pragma once
#include <windows.h>

HWND GetMainWindowHandle(DWORD pid);
BOOL HasVisibleWindow(DWORD pid);
BOOL HasUserActivity(DWORD pid);
