#pragma once
#include <windows.h>

// Hitung CPU usage dalam persen, dengan interval sampling (detik)
double GetCPUUsage(DWORD pid, double intervalSec);

