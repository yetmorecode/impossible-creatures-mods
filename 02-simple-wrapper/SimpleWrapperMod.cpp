#include "../SDKHelper.h"

#define ORIGINAL_DLL "tellurian.dll"

static HINSTANCE originalDll;
static GetDllInterface_t originalGetInterface;
static GetDllVersion_t originalGetVersion;
static DLLInterface* mod = NULL;

BOOL APIENTRY DllMain(HMODULE h, DWORD  reason, LPVOID lpReserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH: 
        originalDll = LoadLibraryA(ORIGINAL_DLL);
        if (originalDll) {
            dbTracefAux("%s: Loaded %s", __FUNCTION__, ORIGINAL_DLL);
            originalGetInterface = (GetDllInterface_t)GetProcAddress(originalDll, "GetDllInterface");
            originalGetVersion = (GetDllVersion_t)GetProcAddress(originalDll, "GetDllVersion");
        }
        else {
            dbTracefAux("%s: Could not load %s", __FUNCTION__, ORIGINAL_DLL);
        }
        DisableThreadLibraryCalls(h); 
        break;
    case DLL_PROCESS_DETACH: 
        delete mod; 
    }
    return TRUE;
}

extern "C" {
    __declspec(dllexport) DLLInterface* __cdecl GetDllInterface() {
        // Get access to tellurian.dll DLLInterface
        mod = originalGetInterface();

        // Do something with (i.e. log its name and version for a start)
        long version = originalGetVersion();
        dbTracefAux("%s: Original mod: %s %d.%d", __FUNCTION__,
            mod->GetName(),
            MODGET_MAJORVERSION(version),
            MODGET_MINORVERSION(version)
        );

        // Do any kind of SDK assisted modding here
        // ...

        // Pass the DllInterface to the game
        return mod;
    }

    __declspec(dllexport) unsigned long __cdecl GetDllVersion() { 
        return originalGetVersion();
    }
}
