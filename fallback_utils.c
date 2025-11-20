#include "fallback_utils.h"
#include "window_utils.h"
#include <string.h>
#include <tlhelp32.h>

// ==== Dynamic Excluded List ====
typedef struct {
    char exeName[MAX_PATH];
    DWORD lastExcludedTick;
} DynamicExcluded;

static DynamicExcluded dynExcludedList[64];
static int dynExcludedCount = 0;

// ==== Excluded Permanen ====
BOOL IsExcludedPermanent(const char* exeName) {
    const char* excluded[] = {
        // Dev tools
        "codeblocks.exe","devenv.exe","msvc.exe",

        // Browser / explorer
        "explorer.exe",

        // Security
        "MsMpEng.exe","SecurityHealthService.exe",

        // GPU drivers
        "nvcontainer.exe","nvsphelper64.exe",
        "atieclxx.exe","atig6pxx.exe","igfxtray.exe",
        "amddvr.exe","amdow.exe",

        // Audio drivers
        "audiodg.exe","realtek.exe","rtkaudiodriver.exe",
        "nahimic.exe","wavesmaxxaudio.exe",

        // Network drivers / services
        "intelproset.exe","killercontrolcenter.exe",
        "rtwlane.exe","athrx.exe","bcmwltry.exe",

        // Printer / peripheral drivers
        "spoolsv.exe","hpservice.exe","epsonstatus.exe","canonservice.exe",

        // System services (critical)
        "services.exe","wininit.exe","winlogon.exe",
        "csrss.exe","smss.exe","lsass.exe","svchost.exe",
        "dwm.exe","system.exe", "AuSp3.exe", "AuSp3.1.exe"
    };

    for (int i=0; i<sizeof(excluded)/sizeof(excluded[0]); i++) {
        if (_stricmp(exeName, excluded[i]) == 0) return TRUE;
    }
    return FALSE;
}

// ==== Excluded Dinamis ====
BOOL IsExcludedDynamic(const char* exeName) {
    DWORD now = GetTickCount();
    for (int i=0; i<dynExcludedCount; i++) {
        if (_stricmp(exeName, dynExcludedList[i].exeName) == 0) {
            // expire setelah 30 menit
            if (now - dynExcludedList[i].lastExcludedTick > 1800000) {
                // hapus dari list
                for (int j=i; j<dynExcludedCount-1; j++)
                    dynExcludedList[j] = dynExcludedList[j+1];
                dynExcludedCount--;
                return FALSE;
            }
            return TRUE;
        }
    }
    return FALSE;
}

void AddToExcludedDynamic(const char* exeName) {
    if (dynExcludedCount < 64) {
        strcpy(dynExcludedList[dynExcludedCount].exeName, exeName);
        dynExcludedList[dynExcludedCount].lastExcludedTick = GetTickCount();
        dynExcludedCount++;
    }
}

// ==== Hung Detection ====
BOOL IsProcessHung(DWORD pid) {
    HWND hwnd = GetMainWindowHandle(pid);
    if (hwnd) {
        DWORD_PTR result;
        if (!SendMessageTimeout(hwnd, WM_NULL, 0, 0,
                                SMTO_ABORTIFHUNG, 2000, &result)) {
            return TRUE;
        }
    }
    return FALSE;
}
