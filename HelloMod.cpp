/* 
* Minimal Impossible Creatures Mod using the SDK
* 
* A mod has to be build as a DLL exposing the following interface
* hierarchy to the game:
* 
* DLLInterface (HelloMod)
*   DLLGameInterface (HelloGame)
*     DLLCpuInterface (HelloCpu)
*     DLLGuiInterface (HelloGui)
*     DLLSimInterface (HelloSim)
*   DLLSetupInterface (HelloSetup)
*   DLLScoreInterface (HelloScore)
* 
* The goal of this mod is to provide a minimal implementation all of those
* and to be buildable on a modern 2022 toolchain (e.g. Visual Studio 2022 CE).
* 
* To link the SDK headers in VS2022 you need to navigate to "HelloMod -> Properties ->
* C/C++ -> General -> Additional Includes" and add the path to the "Include" folder
* inside the SDK directory.
*/
 
#include <windows.h>
#include "ModInterface/DllInterface.h"

class HelloCpu : public DLLCpuInterface {
    GameAI* CreateGameAI(CommandInterface* command) {
        return NULL;
    }
    void InitLuaAI(LuaConfig* lc) {}
    void ShutLuaAI(LuaConfig* lc) {}
};

class HelloGui : public DLLGuiInterface {

};

class HelloSim : public DLLSimInterface {

};

class HelloGame : public DLLGameInterface {
    HelloCpu *cpu;
    HelloGui *gui;
    HelloSim *sim;
public:
    HelloGame(SimEngineInterface* sim) {
        cpu = new HelloCpu();
        gui = new HelloGui();
        sim = new HelloSim();
    }
    DLLCpuInterface* GetCpuInterface() { return cpu; }
    DLLGuiInterface* GetGuiInterface() { return gui; }
    DLLSimInterface* GetSimInterface() { return sim; }
};

class HelloScore : public DLLScoreInterface {

};

class HelloSetup : public DLLSetupInterface {

};

class HelloMod : public DLLInterface {
    char version[16] = "\0";
    HelloSetup* setup;

    const wchar_t* GetName() { return L"Hello Creatures"; }

    bool Initialize(const char* version) {
        strncpy(this->version, version, sizeof(this->version) - 1);
        setup = new HelloSetup();
    }

    void Shutdown() {
        return;
    }

    bool IsScenarioCompatible(const char* modname) const {
        return strcmp("RDNMod", modname) == 0;
    }

    DLLGameInterface* GameCreate(SimEngineInterface* sim) {
        return new HelloGame(sim);
    }

    void GameDestroy(DLLGameInterface* p) {
        delete p;
        return;
    }

    DLLScoreInterface* ScoreCreate() {
        return new HelloScore();
    }

    void ScoreDestroy(DLLScoreInterface* p) {
        delete p;
        return;
    }

    DLLSetupInterface* SetupCreate() {
        return setup;
    }

    void SetupDestroy(DLLSetupInterface* p) {
        setup = new HelloSetup();
        return;
    }

    ZsProgress ZsPublish() {
        return ZSP_Done;
    }

    ZsProgress ZsUpdate() {
        return ZSP_Done;
    }

    ZsProgress ZsAbort() {
        return ZSP_Done;
    }
};

static HelloMod* mod = NULL;

BOOL APIENTRY DllMain(HMODULE h, DWORD  reason, LPVOID lpReserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(h);
        mod = new HelloMod();
        break;
    case DLL_PROCESS_DETACH:
        delete mod;
        mod = NULL;
        break;
    }
    return TRUE;
}

extern "C" {
    __declspec(dllexport) DLLInterface* __cdecl GetDllInterface() {
        return mod;
    }
    __declspec(dllexport) unsigned long __cdecl GetDllVersion() {
        return MODMAKE_VERSION(MajorVersion, MinorVersion);
    }
}