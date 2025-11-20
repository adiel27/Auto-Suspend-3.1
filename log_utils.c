#include "log_utils.h"
#include <stdio.h>

void LogAction(const char* action, const char* processName, DWORD pid) {
    FILE* f = fopen("service_log.txt", "a");
    if (f) {
        fprintf(f, "%s: %s (PID %lu)\n", action, processName, pid);
        fclose(f);
    }
}

