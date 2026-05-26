// Tiny symbolizer using DbgHelp - resolves EXE-relative RVAs to functions+source lines
// Build:  cl /EHsc /Fe:symbolize.exe symbolize.cpp dbghelp.lib
// Usage:  symbolize.exe <exe_path> <pdb_search_dir> <rva1> [rva2 ...]

#include <windows.h>
#include <dbghelp.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "dbghelp.lib")

int main(int argc, char** argv) {
    if (argc < 4) {
        printf("Usage: %s <exe_path> <pdb_search_dir> <rva_hex> [...]\n", argv[0]);
        return 1;
    }
    const char* exe = argv[1];
    const char* searchDir = argv[2];

    HANDLE proc = GetCurrentProcess();
    SymSetOptions(SYMOPT_UNDNAME | SYMOPT_LOAD_LINES | SYMOPT_DEBUG);
    if (!SymInitialize(proc, searchDir, FALSE)) {
        printf("SymInitialize failed: %lu\n", GetLastError());
        return 2;
    }

    const DWORD64 base = 0x10000000ULL;
    // Copy PDB next to EXE to ensure DbgHelp finds it (also try direct load)
    DWORD64 loaded = SymLoadModuleEx(proc, NULL, exe, NULL, base, 0, NULL, 0);
    if (!loaded) {
        printf("SymLoadModuleEx failed: %lu\n", GetLastError());
        return 3;
    }
    printf("Loaded module at base 0x%llX\n", loaded);

    IMAGEHLP_MODULE64 mi = {0};
    mi.SizeOfStruct = sizeof(mi);
    if (SymGetModuleInfo64(proc, loaded, &mi)) {
        printf("PDB: '%s'  SymType=%d (5=Deferred, 10=Pdb)\n", mi.LoadedPdbName, mi.SymType);
    }
    if (mi.SymType != SymPdb) {
        // Force resolution
        SymRefreshModuleList(proc);
        if (SymGetModuleInfo64(proc, loaded, &mi)) {
            printf("After refresh: PDB: '%s'  SymType=%d\n", mi.LoadedPdbName, mi.SymType);
        }
    }

    char buf[sizeof(SYMBOL_INFO) + 1024];
    PSYMBOL_INFO sym = (PSYMBOL_INFO)buf;
    sym->SizeOfStruct = sizeof(SYMBOL_INFO);
    sym->MaxNameLen = 1024;

    for (int i = 3; i < argc; i++) {
        DWORD64 rva = strtoull(argv[i], NULL, 16);
        DWORD64 addr = loaded + rva;
        DWORD64 disp = 0;
        printf("\n--- RVA 0x%llX (addr 0x%llX) ---\n", rva, addr);
        if (SymFromAddr(proc, addr, &disp, sym)) {
            printf("  %s + 0x%llX\n", sym->Name, disp);
        } else {
            printf("  SymFromAddr failed: %lu\n", GetLastError());
        }
        IMAGEHLP_LINE64 line = {0};
        line.SizeOfStruct = sizeof(line);
        DWORD ldisp = 0;
        if (SymGetLineFromAddr64(proc, addr, &ldisp, &line)) {
            printf("  %s:%lu (+0x%lX)\n", line.FileName, line.LineNumber, ldisp);
        } else {
            printf("  SymGetLineFromAddr64 failed: %lu\n", GetLastError());
        }
    }

    SymCleanup(proc);
    return 0;
}

