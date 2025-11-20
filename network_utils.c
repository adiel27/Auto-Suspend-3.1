#include "network_utils.h"
#include <iphlpapi.h>
#include <stdlib.h>
#pragma comment(lib, "iphlpapi.lib")

BOOL IsNetworkActive(DWORD pid) {
    PMIB_TCPTABLE_OWNER_PID tcpTable;
    DWORD size = 0;

    if (GetExtendedTcpTable(NULL, &size, FALSE, AF_INET,
                            TCP_TABLE_OWNER_PID_ALL, 0) == ERROR_INSUFFICIENT_BUFFER) {
        tcpTable = (PMIB_TCPTABLE_OWNER_PID)malloc(size);
        if (tcpTable &&
            GetExtendedTcpTable(tcpTable, &size, FALSE, AF_INET,
                                TCP_TABLE_OWNER_PID_ALL, 0) == NO_ERROR) {
            for (DWORD i = 0; i < tcpTable->dwNumEntries; i++) {
                if (tcpTable->table[i].dwOwningPid == pid &&
                    tcpTable->table[i].dwState == MIB_TCP_STATE_ESTAB) {
                    free(tcpTable);
                    return TRUE; // ada koneksi aktif
                }
            }
        }
        if (tcpTable) free(tcpTable);
    }
    return FALSE;
}

