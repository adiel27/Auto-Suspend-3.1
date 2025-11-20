#include <windows.h>
#include <stdio.h>
#include "process_utils.h"

int main() {
    printf("Process Monitor Test\n");
    printf("Program akan memantau proses dan suspend/resume sesuai CPU usage.\n");
    printf("Tekan Ctrl+C untuk berhenti.\n\n");

    while (1) {
        MonitorProcesses();
        Sleep(5000); // cek tiap 5 detik
    }

    return 0;
}
