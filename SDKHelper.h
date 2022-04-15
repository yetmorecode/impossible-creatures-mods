#pragma once

#define _CRT_SECURE_NO_WARNINGS 1

#include <windows.h>
#include <math.h>
#include <vector>
#include "Debug/db.h"
#include "ModInterface/DllInterface.h"

typedef DLLInterface* (__cdecl *GetDllInterface_t)();
typedef unsigned long (__cdecl *GetDllVersion_t)();

#define EXPORT_DLL_FUNCTIONS(mod, major, minor) extern "C" { \
    __declspec(dllexport) DLLInterface* __cdecl GetDllInterface() { return mod; } \
    __declspec(dllexport) unsigned long __cdecl GetDllVersion() { return MODMAKE_VERSION(MajorVersion, MinorVersion); } \
}

#define EXPORT_MOD(C) static C* mod = NULL; \
BOOL APIENTRY DllMain(HMODULE h, DWORD  reason, LPVOID lpReserved) { \
    switch (reason) { \
    case DLL_PROCESS_ATTACH: DisableThreadLibraryCalls(h); mod = new C(); break; \
    case DLL_PROCESS_DETACH: delete mod; mod = NULL; break; } \
    return TRUE; \
} \
EXPORT_DLL_FUNCTIONS(mod, MajorVersion, MinorVersion)

