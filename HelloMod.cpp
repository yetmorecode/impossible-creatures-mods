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
	int TotalDuration() const { return 13; }
	PlayerState PlayerFinalState(unsigned long idplayer) const { return PlayerState::PS_WON; }
	const wchar_t* PlayerName(unsigned long idplayer) const { return L"player name"; }
	size_t UnitsTypeCount(unsigned long idplayer) const { return 0; }
	long UnitsTypeAt(unsigned long idplayer, size_t index) const { return 0; }
	int UnitTotal(unsigned long idplayer, long ebpid) const { return 0; }
	size_t StatListCount() const { return 0; }
	void StatListAt(size_t idx, wchar_t* out, size_t outlen) const { wcsncpy(out, L"StatListAt", outlen); }
	void StatListTooltipAt(size_t idx, wchar_t* out, size_t outlen) const { wcsncpy(out, L"StatListTooltipAt", outlen); }
	TabType	StatListTabTypeAt(size_t idx) const { return TT_ByPlayer; }
	ValueType StatListTypeAt(size_t idx) const { return VT_Number; }
	DecoratorFunc StatListDecoratorFuncAt(size_t idx) const { return DF_None; }
	void StatListValue(size_t idx, unsigned long idplayer, int& out) const { out = 0; }
	void StatListValue(size_t idx, unsigned long idplayer, long ebpnetid, int& out) const { out = 0; }
	size_t ScoresTabsQty() const { return 0; }
	void ScoresTabName(size_t tabIndex, wchar_t* out, size_t outlen) const { wcsncpy(out, L"ScoresTabName", outlen); }
	bool ScoresTabColumn(size_t tabIndex, size_t columnIndex) const { return false; }
	void ScoresTabColumnName(size_t tabIndex, size_t columnIndex, wchar_t* out, size_t outlen) const { wcsncpy(out, L"ScoresTabColumnName", outlen); }
	void ScoresTabColumnTooltip(size_t tabIndex, size_t columnIndex, wchar_t* out, size_t outlen) const {	wcsncpy(out, L"ScoresTabColumnTooltip", outlen); }
	ValueType ScoresTabColumnType(size_t tabIndex, size_t columnIndex) const { return VT_Number; }
	DecoratorFunc ScoresTabColumnDecoratorFunc(size_t tabIndex, size_t columnIndex) const { return DF_None; }
	TabType ScoresTabType(size_t tabIndex) const { return TT_Custom; }
	void ScoresTabColumnValue(size_t tabIndex, size_t columnIndex, unsigned long arg0, int& out) const { out = 0; }
	void ScoresTabColumnValue(size_t tabIndex, size_t columnIndex, unsigned long arg0, long arg1, int& out) const { out = 0; }
	void ScoresTabColumnValue(size_t tabIndex, size_t columnIndex, unsigned long idplayer, wchar_t* out, size_t outlen) const { out[0] = L'\0'; }
};

class HelloSetup : public DLLSetupInterface {
    void SetGameType(DLLSetupInterface::GameType gt) {}
    void SetNetworkGame(bool bNetwork) {}
    void SetRandomSeed(unsigned long seed) {}
    void SetStatsScenario(const char* scenfile, const wchar_t* scenname) {}
    void SetStatsGameID(const GUID& id) {}
    unsigned long GetRandomSeed() const { return 1; }
    void PlayerSetCount(size_t n) {}
    void PlayerSetMax(size_t n) {}
    size_t PlayerGetCount() const { return 1; }
    size_t PlayerGetMax() const { return 1; }
    void PlayerSetToAI(size_t n) {}
    void PlayerSetToHuman(size_t n) {}
    void PlayerSetName(size_t n, const wchar_t* name) {}
    void PlayerSetPassport(size_t n, const wchar_t* passport) {}
    void PlayerSetRace(size_t n, size_t race) {}
    void PlayerSetTeam(size_t n, size_t t) {}
	void OptionsInit(GameType gt, bool bNetwork) {}	
	size_t OptionsCount() const { return 0; }
	unsigned long OptionID(size_t optionIndex) const { return 0; }
	void OptionName(wchar_t* out, size_t outlen, unsigned long optionID) const {}
	void OptionTooltip(wchar_t* out, size_t outlen, unsigned long optionID) const {}
	size_t OptionChoices(unsigned long optionID) const { return 0; }
	size_t OptionChoiceDef(unsigned long optionID) const { return 0; }
	bool OptionChoiceValid(unsigned long optionID, size_t choice) const { return true; }
    void OptionChoiceName(wchar_t* out, size_t outlen, unsigned long optionID, size_t choice) const {}
    bool OptionChoiceAISupport(unsigned long optionID, size_t choice) const { return false; }
    void OptionSet(unsigned long optionID, size_t val) {}
    size_t RaceGetCount() const { return 1; }
    const wchar_t* RaceGetName(size_t index) const { return L"Hello race"; }
    const wchar_t* RaceGetDesc(size_t index) const { return L"A simple but the only true race for sure"; }
    // TODO: image?
    const char* RaceGetImageFilename(size_t index) const { return "idk image filename?"; }
    bool TeamGetEnabled() const { return false; }
    // TODO: needs one option for game mode at least?
    unsigned long GetGameModeOptionID() const { return 1; }
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