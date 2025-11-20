#include "window_utils.h"
#include <windows.h>

// Context untuk cari main window
typedef struct {
    DWORD pid;
    HWND hwnd;
} HandleData;

// Callback cari main window
static BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam) {
    HandleData* data = (HandleData*)lParam;
    DWORD windowPid;
    GetWindowThreadProcessId(hwnd, &windowPid);

    if (windowPid == data->pid &&
        GetWindow(hwnd, GW_OWNER) == NULL &&
        IsWindowVisible(hwnd)) {
        data->hwnd = hwnd;
        return FALSE; // stop enumerasi
    }
    return TRUE;
}

// Ambil main window dari PID
HWND GetMainWindowHandle(DWORD pid) {
    HandleData data;
    data.pid = pid;
    data.hwnd = NULL;
    EnumWindows(EnumWindowsCallback, (LPARAM)&data);
    return data.hwnd;
}

// Context untuk cek visible window
typedef struct {
    DWORD pid;
    BOOL visible;
} WindowCheckContext;

// Callback cek visible window
static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    WindowCheckContext* ctx = (WindowCheckContext*)lParam;
    DWORD windowPid;
    GetWindowThreadProcessId(hwnd, &windowPid);

    if (windowPid == ctx->pid && IsWindowVisible(hwnd)) {
        ctx->visible = TRUE;
        return FALSE; // stop enumerasi
    }
    return TRUE;
}

// Cek apakah proses punya window visible
BOOL HasVisibleWindow(DWORD pid) {
    WindowCheckContext ctx;
    ctx.pid = pid;
    ctx.visible = FALSE;
    EnumWindows(EnumWindowsProc, (LPARAM)&ctx);
    return ctx.visible;
}

// Cek apakah user aktif di proses (foreground/kursor)
BOOL HasUserActivity(DWORD pid) {
    // cek foreground window
    HWND fg = GetForegroundWindow();
    if (fg) {
        DWORD fgPid;
        GetWindowThreadProcessId(fg, &fgPid);
        if (fgPid == pid) return TRUE;
    }

    // cek kursor
    POINT pt;
    if (GetCursorPos(&pt)) {
        HWND hwndAtCursor = WindowFromPoint(pt);
        if (hwndAtCursor) {
            DWORD cursorPid;
            GetWindowThreadProcessId(hwndAtCursor, &cursorPid);
            if (cursorPid == pid) return TRUE;
        }
    }

    return FALSE;
}
