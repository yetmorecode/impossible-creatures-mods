#include "../SDKHelper.h"

#define ORIGINAL_DLL "tellurian.dll"
#define NAME_SUFFIX "Plus"

static HINSTANCE originalDll;
static GetDllInterface_t originalGetInterface;
static GetDllVersion_t originalGetVersion;
static DLLInterface* original;

class RenameMod : public DLLInterface {
    bool Initialize(const char* version) {
        return original->Initialize(version);
    }
    const wchar_t* GetName() {
        static wchar_t name[64];
        wcscpy(name, original->GetName());
        wcscat(name, L" " NAME_SUFFIX);
        return name;
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

static RenameMod* renamed;

BOOL APIENTRY DllMain(HMODULE h, DWORD  reason, LPVOID lpReserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        originalDll = LoadLibraryA(ORIGINAL_DLL);
        if (originalDll) {
            originalGetInterface = (GetDllInterface_t)GetProcAddress(originalDll, "GetDllInterface");
            originalGetVersion = (GetDllVersion_t)GetProcAddress(originalDll, "GetDllVersion");
            original = originalGetInterface();
            renamed = new RenameMod();
        }
        else {
            dbTracefAux("%s: Could not load %s", __FUNCTION__, ORIGINAL_DLL);
        }
        DisableThreadLibraryCalls(h);
        break;
    case DLL_PROCESS_DETACH:
        delete original;
    }
    return TRUE;
}

EXPORT_MOD_FUNCTIONS(renamed, originalGetVersion())
