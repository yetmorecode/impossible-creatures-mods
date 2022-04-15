#include "../SDKHelper.h"

static int __stdcall lua_hello(lua_State* s) {
    dbTracefAux("%s", __FUNCTION__);
    //double d = lua_tonumber(s, 1);  /* get argument */
    //lua_pushnumber(s, 1337.0);  /* push result */
    return 0;  /* number of results */
}

class AiCpu : public WrappedCpuInterface {
public:
    AiCpu(DLLCpuInterface* o) : WrappedCpuInterface(o) {}
    
    void InitLuaAI(LuaConfig* lc) {
        dbTracefAux("%s", __FUNCTION__);
        original->InitLuaAI(lc);
        lc->RegisterCFunc("hello", &lua_hello);
        dbTracefAux("%s: Added %s::hello()", __FUNCTION__, lc->GetName());
        // TODO: dump all globals in lc?
    }
};

class AiGame : public WrappedGameInterface {
public:
    AiGame(DLLGameInterface* o) : WrappedGameInterface(o) {}
    DLLCpuInterface* GetCpuInterface() {
        dbTracefAux("%s", __FUNCTION__);
        return new AiCpu(original->GetCpuInterface());
    }
};

class AiMod : public WrappedDllInterface {
public:
	AiMod(DLLInterface* o) : WrappedDllInterface(o) {}

    const wchar_t* GetName() {
        dbTracefAux("%s", __FUNCTION__);
        static wchar_t name[64];
        wcscpy(name, original->GetName());
        wcscat(name, L" AiMod");
        return name;
    }

    DLLGameInterface* GameCreate(SimEngineInterface* sim) {
        dbTracefAux("%s", __FUNCTION__);
        return new AiGame(original->GameCreate(sim));
    }
};

WRAP_DLL_INTERFACE("tellurian.dll", AiMod)