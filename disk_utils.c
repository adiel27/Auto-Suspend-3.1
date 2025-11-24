#include "disk_utils.h"
#include <windows.h>
#include <psapi.h>

typedef struct {
    DWORD pid;
    ULONGLONG lastRead;
    ULONGLONG lastWrite;
} DiskTrack;

static DiskTrack trackList[128];
static int trackCount = 0;

BOOL IsDiskActive(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!hProcess) return FALSE;

    IO_COUNTERS io;
    BOOL active = FALSE;

    if (GetProcessIoCounters(hProcess, &io)) {
        ULONGLONG totalRead = io.ReadTransferCount;
        ULONGLONG totalWrite = io.WriteTransferCount;

        // cari entry pid di trackList
        DiskTrack* entry = NULL;
        for (int i=0; i<trackCount; i++) {
            if (trackList[i].pid == pid) {
                entry = &trackList[i];
                break;
            }
        }
        if (!entry && trackCount < 128) {
            trackList[trackCount].pid = pid;
            trackList[trackCount].lastRead = totalRead;
            trackList[trackCount].lastWrite = totalWrite;
            entry = &trackList[trackCount];
            trackCount++;
        }

        if (entry) {
            // threshold: 512 KB per interval
            if ((totalRead > entry->lastRead + 512*1024) ||
                (totalWrite > entry->lastWrite + 512*1024)) {
                active = TRUE;
            }
            entry->lastRead = totalRead;
            entry->lastWrite = totalWrite;
        }
    }

    CloseHandle(hProcess);
    return active;
}

