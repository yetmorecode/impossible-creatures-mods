#include "../SDKHelper.h"

class AiMod : public WrappedDllInterface {
public:
	AiMod(DLLInterface* o) : WrappedDllInterface(o) {}

    const wchar_t* GetName() {
        static wchar_t name[64];
        wcscpy(name, original->GetName());
        wcscat(name, L" AiMod");
        return name;
    }
};

WRAP_DLL_INTERFACE("tellurian.dll", AiMod)