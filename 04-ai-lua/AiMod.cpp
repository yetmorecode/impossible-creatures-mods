#include "../SDKHelper.h"

static int lua_hello(lua_State* s) {
    //double d = lua_tonumber(s, 1);  /* get argument */
    lua_pushstring(s, "Hello Lua");  /* push result */
    return 1;  /* number of results */
}

class AiCpu : public WrappedCpuInterface {
public:
    AiCpu(DLLCpuInterface* o) : WrappedCpuInterface(o) {}
    
    void InitLuaAI(LuaConfig* lc) {
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
        return new AiCpu(original->GetCpuInterface());
    }
};

class AiMod : public WrappedDllInterface {
public:
	AiMod(DLLInterface* o) : WrappedDllInterface(o) {}

    const wchar_t* GetName() {
        static wchar_t name[64];
        wcscpy(name, original->GetName());
        wcscat(name, L" AiMod");
        return name;
    }

    DLLGameInterface* GameCreate(SimEngineInterface* sim) {
        return new AiGame(original->GameCreate(sim));
    }
};

WRAP_DLL_INTERFACE("tellurian.dll", AiMod)