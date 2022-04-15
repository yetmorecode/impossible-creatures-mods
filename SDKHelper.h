#pragma once

#define _CRT_SECURE_NO_WARNINGS 1

#include <windows.h>
#include <math.h>
#include <vector>
#include <string>
#include "Debug/db.h"
#include "Memory/Memorylib.h"
#include "ModInterface/DllInterface.h"
#include "Lua/LuaConfig.h"

typedef DLLInterface* (__cdecl *GetDllInterface_t)();
typedef unsigned long (__cdecl *GetDllVersion_t)();

class WrappedDllInterface : public DLLInterface {
protected:
    DLLInterface* original;
public:
    WrappedDllInterface(DLLInterface* o) {
        original = o;
    }
    bool Initialize(const char* version) {
        return original->Initialize(version);
    }
    const wchar_t* GetName() {
        return original->GetName();
    }
    void Shutdown() {
        return original->Shutdown();
    }
    bool IsScenarioCompatible(const char* modname) const {
        return original->IsScenarioCompatible(modname);
    }
    DLLGameInterface* GameCreate(SimEngineInterface* sim) {
        return original->GameCreate(sim);
    }
    void GameDestroy(DLLGameInterface* p) {
        return original->GameDestroy(p);
    }
    DLLScoreInterface* ScoreCreate() {
        return original->ScoreCreate();
    }
    void ScoreDestroy(DLLScoreInterface* p) {
        return original->ScoreDestroy(p);
    }
    DLLSetupInterface* SetupCreate() {
        return original->SetupCreate();
    }
    void SetupDestroy(DLLSetupInterface* p) {
        return original->SetupDestroy(p);
    }
    ZsProgress ZsPublish() {
        return original->ZsPublish();
    }
    ZsProgress ZsUpdate() {
        return original->ZsUpdate();
    }
    ZsProgress ZsAbort() {
        return original->ZsAbort();
    }
};

class WrappedGameInterface : public DLLGameInterface {
protected:
    DLLGameInterface* original;
public:
    WrappedGameInterface(DLLGameInterface *o) {
        original = o;
    }
    DLLCpuInterface* GetCpuInterface() { 
        return original->GetCpuInterface();
    }
    DLLGuiInterface* GetGuiInterface() { 
        return original->GetGuiInterface();
    }
    DLLSimInterface* GetSimInterface() { 
        return original->GetSimInterface();
    }
};

class WrappedCpuInterface : public DLLCpuInterface {
protected:
    DLLCpuInterface* original;
public:
    WrappedCpuInterface(DLLCpuInterface* o) {
        original = o;
    }
    GameAI* CreateGameAI(CommandInterface* command) {
        return original->CreateGameAI(command);
    }
    void InitLuaAI(LuaConfig* lc) {
        return original->InitLuaAI(lc);
    }
    void ShutLuaAI(LuaConfig* lc) {
        return original->ShutLuaAI(lc);
    }
};

#define EXPORT_MOD_FUNCTIONS(mod, version) extern "C" { \
    __declspec(dllexport) DLLInterface* __cdecl GetDllInterface() { return mod; } \
    __declspec(dllexport) unsigned long __cdecl GetDllVersion() { return version; } \
}

#define EXPORT_MOD(C) static DLLInterface* mod = NULL; \
BOOL APIENTRY DllMain(HMODULE h, DWORD reason, LPVOID lpReserved) { \
    switch (reason) { \
    case DLL_PROCESS_ATTACH: DisableThreadLibraryCalls(h); mod = C; break; \
    case DLL_PROCESS_DETACH: delete mod; mod = NULL; break; } \
    return TRUE; \
} \
EXPORT_MOD_FUNCTIONS(mod, MODMAKE_VERSION(MajorVersion, MinorVersion))

#define WRAP_DLL_INTERFACE(DLL, C) static HINSTANCE originalDll; \
static GetDllInterface_t originalGetInterface; \
static GetDllVersion_t originalGetVersion; \
static DLLInterface* original; \
static WrappedDllInterface* wrapper; \
BOOL APIENTRY DllMain(HMODULE h, DWORD  reason, LPVOID lpReserved) { \
    switch (reason) { \
    case DLL_PROCESS_ATTACH: \
        originalDll = LoadLibraryA(DLL); if (originalDll) { \
            originalGetInterface = (GetDllInterface_t)GetProcAddress(originalDll, "GetDllInterface"); \
            originalGetVersion = (GetDllVersion_t)GetProcAddress(originalDll, "GetDllVersion"); \
            original = originalGetInterface(); \
            wrapper = new C(original); \
        } else { dbTracefAux("%s: Could not load %s", __FUNCTION__, DLL); } \
        DisableThreadLibraryCalls(h); break; \
    case DLL_PROCESS_DETACH: delete original; } \
    return TRUE; \
} \
EXPORT_MOD_FUNCTIONS(wrapper, originalGetVersion())





