#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <psapi.h>

int main() {
    DWORD pid = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 entry = { sizeof(PROCESSENTRY32) };

        if (Process32First(snapshot, &entry)) {
            do {
                if (_stricmp(entry.szExeFile, "StarRail.exe") == 0) {
                    pid = entry.th32ProcessID;
                    break;
                }
            } while (Process32Next(snapshot, &entry));
        }
    }

    if (pid == 0) {
        std::cout << "Honkai: StarRail is not running." << std::endl;
        return 1;
    }

    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (process == NULL) {
        std::cout << "Failed to open Honkai: StarRail." << std::endl;
        return 1;
    }

    while (true) {
        HMODULE module[1024];
        DWORD needed;
        if (EnumProcessModules(process, module, sizeof(module), &needed)) {
            PROCESS_MEMORY_COUNTERS pmc;
            if (GetProcessMemoryInfo(process, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
                DWORD fps = 0;
                DWORD_PTR baseAddr = (DWORD_PTR)module[0];
                DWORD_PTR unityplayerAddr = 0x1C4E000; // Should be the actual address.

                DWORD frameCountAddr = baseAddr + unityplayerAddr;

                ReadProcessMemory(process, (LPCVOID)frameCountAddr, &fps, sizeof(fps), NULL);

                std::cout << "StarRail.exe FPS: " << fps << std::endl;
            }
        }

        Sleep(1000); // FPS Change Check every second
    }

    CloseHandle(process);
    return 0;
}

#if defined(_WIN64)
#pragma comment(lib, "libstdc++-6")
#else
#pragma comment(lib, "libstdc++-6")
#endif
