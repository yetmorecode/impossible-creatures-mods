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
*/

#define _CRT_SECURE_NO_WARNINGS 1

#include <windows.h>
#include "HelloMod.h"

class HelloCpu : public DLLCpuInterface {
    GameAI* CreateGameAI(CommandInterface* command) {
        return NULL;
    }
    void InitLuaAI(LuaConfig* lc) {}
    void ShutLuaAI(LuaConfig* lc) {}
};

class HelloGui : public DLLGuiInterface {
    void InitLuaGui(LuaConfig* lc) {}
    void ShutLuaGui(LuaConfig* lc) {}
    void OnEntityCreate(const Entity* e) {}
    void ChangePlayerArmy(unsigned long, const std::vector<long>&) {}
    EntityFilter* GetEntityFilter() { return NULL; }
    ModSimVis* GetModSimVis() { return NULL; }
    ModUIEvent* GetModUIEvent() { return NULL; }
    NISletInterface* GetNISletInterface() { return NULL; }
    void DoCommand(const EntityGroup& g) {}
    void DoCommand(const Vec3f* v, unsigned long n) {}
    bool ProcessInput(const Plat::InputEvent& ie) { return true; }
    const char* GetCursor(const Entity* mouseOverEntity) { return NULL; }
    
    void CreateHUD(
        const Player* localplayer, 
        RTSHud* hud, CommandInterface* command, 
        UIInterface* ui, 
        MessageInterface* message, 
        SelectionInterface* sel,
        CameraInterface* cam,
        SoundInterface* sound,
        FXInterface* fx
    ) {}
    void ShutdownHUD() {}
    void UpdateHUD(float elapsedSeconds) {}
    void UIPause(bool bPause) {}
    void Save(IFF& iff) {}
    void Load(IFF& iff) {}
    void ShowModOptions(void) {}
};

class HelloSim : public DLLSimInterface {
    bool InitLuaSim(LuaConfig* lc) { return true; }
    void ShutLuaSim(LuaConfig* lc) {}
    void InitTriggers() {}
    void SetDecalInterface(DecalInterface* decal) {}
    void SetTerrainOverlayInterface(TerrainOverlayInterface* overlay) {}
    void SetGhostInterface(GhostInterface* ghost) {}
    // TODO: need a world?
    World* CreateNewWorld(bool bMissionEd) { return NULL; }
    // TODO: need a player?
    Player* CreateNewPlayer() { return NULL; }
    // TODO: need entities?
    Entity* CreateNewEntity(void* buffer, unsigned long id, const ControllerBlueprint* cbp) { return NULL; }
    unsigned long MapPlayerToSimulation(size_t playerIndex) const { return 0; }
    void GetDataToken(std::vector< std::pair< unsigned long, const char* > >& crcArray) const {}
    bool IsPlayerAlly(unsigned long idPlayer1, unsigned long idPlayer2) const { return idPlayer1 == idPlayer2; }
    bool IsPlayerEnemy(unsigned long idPlayer1, unsigned long idPlayer2) const { return idPlayer1 != idPlayer2; }
    void SaveWorldStaticData(IFF& iff, const ImpassEditArray* impassEdit) {}
    void LoadWorldStaticData(IFF& iff) {}
    void LoadSPPersistentData(IFF&, SPPersistenceInterface*) {}
    void SaveSPPersistentData(IFF&, SPPersistenceInterface*) {}
    void NetworkKillPlayer(unsigned long idplayer, NetworkKillType type) {}
    void StatsGameAbort() {}
    void StatsZSSave() {}
    bool IsScenarioSuccess(unsigned long idPlayer) const { return true; }
    void OnTerrainModify(const Rect2f& rect, const ImpassEditArray* impassEdit) {}
    bool IsCellImpassible(int x, int z) { return false; }
    size_t GetPlayerCount(GameType gametype) { return 1; }
};

class HelloGame : public DLLGameInterface {
    HelloCpu *cpu;
    HelloGui *gui;
    HelloSim *sim;
public:
    HelloGame(SimEngineInterface* sim) {
        cpu = new HelloCpu();
        gui = new HelloGui();
        this->sim = new HelloSim();
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

    bool Initialize(const char* version) {
        strncpy(this->version, version, sizeof(this->version) - 1);
        dbTracefAux("(HelloMod) Thanks for loading Hello Creatures %s", version);
        setup = new HelloSetup();
        return true;
    }

    const wchar_t* GetName() { return L"Hello Creatures"; }
    void Shutdown() {}
    bool IsScenarioCompatible(const char* modname) const { return strcmp("IC", modname) == 0; }
    DLLGameInterface* GameCreate(SimEngineInterface* sim) { return new HelloGame(sim); }
    void GameDestroy(DLLGameInterface* p) { delete p; }
    DLLScoreInterface* ScoreCreate() { return new HelloScore(); }
    void ScoreDestroy(DLLScoreInterface* p) { delete p; }
    DLLSetupInterface* SetupCreate() { return setup; }
    void SetupDestroy(DLLSetupInterface* p) { delete setup;  setup = new HelloSetup(); }
    ZsProgress ZsPublish() { return ZSP_Done; }
    ZsProgress ZsUpdate() { return ZSP_Done; }
    ZsProgress ZsAbort() { return ZSP_Done; }
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