#include "SDK.h"

extern "C" __declspec(dllexport)
void* CreateInterface(const char* name, InterfaceReturnStatus * rc) 
{
    void* p = (std::strcmp(name, "ISERVERPLUGINCALLBACKS003") == 0) ? &plugin : nullptr;

    if (rc)
    {
        *rc = (p ? InterfaceReturnStatus::OK : InterfaceReturnStatus::FAILED);
    }
    return p;
}

uintptr_t CreateConVar(const char* pName, const char* pDefaultValue, int flags = 0)
{
    auto pVar = (ConVar*)malloc(sizeof(ConVar));
    memset(pVar, 0, sizeof(ConVar));

#ifdef _WIN64
    using create_t = uintptr_t(__fastcall*)(void*, const char*, const char*, int);
    static auto CreateFn = reinterpret_cast<create_t>(g_Utils.FindPattern("client.dll", "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 83 EC 20 33 ED C6 41 10 00"));
#else
    using create_t = uintptr_t(__thiscall*)(void*, const char*, const char*, int);
    static auto CreateFn = reinterpret_cast<create_t>(g_Utils.FindPattern("client.dll", "55 8B EC D9 EE 56 6A 00 51 D9 14 24 6A 00 51 D9 1C 24 6A 00 8B F1"));
#endif

    return CreateFn(pVar, pName, pDefaultValue, flags);
}

void Main(CreateInterfaceFn ef, CreateInterfaceFn sf) 
{
    Interfaces::Initialize(ef);

    char* sNewName = new char[32];

    strcpy_s(sNewName, 32, "sv_cheats_");
    strcat_s(sNewName, 32, g_Utils.RandomStr(5));

    SpoofedConvar* sv_cheats = new SpoofedConvar("sv_cheats", sNewName);
    if (sv_cheats->IsSpoofed())
    {
        Color Green;
        Green.r = 0;
        Green.g = 255;
        Green.b = 0;
        Green.a = 255;

        gI_EngineCvar->ConsoleColorPrintf(Green, "sv_cheats renamed to: %s\n", sNewName);

        sv_cheats->SetInt(1);

        //SourceMod convar check bypass (hallo fuckit devs)
        CreateConVar("sv_cheats", "0", FCVAR_NOTIFY | FCVAR_REPLICATED);
    }
}