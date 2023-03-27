#include "Utils.h"
Utils g_Utils;

uintptr_t Utils::FindPattern(const char* szModule, const char* szSignature)
{
    MODULEINFO modInfo;
    GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &modInfo, sizeof(MODULEINFO));
    uintptr_t startAddress = reinterpret_cast<uintptr_t>(modInfo.lpBaseOfDll);
    uintptr_t endAddress = startAddress + modInfo.SizeOfImage;
    const char* pat = szSignature;
    uintptr_t firstMatch = 0;
    for (uintptr_t pCur = startAddress; pCur < endAddress; pCur++)
    {
        if (!*pat) return firstMatch;
        if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
        {
            if (!firstMatch) firstMatch = pCur;
            if (!pat[2]) return firstMatch;
            if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
            else pat += 2;
        }
        else
        {
            pat = szSignature;
            firstMatch = 0;
        }
    }

    return NULL;
}

char* Utils::RandomStr(int length)
{
    srand((unsigned)time(0));

    static const char Alphabets[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    char* sRet = new char[length + 1];

    int size = sizeof(Alphabets) - 1;

    for (int i = 0; i < length; i++)
    {
        sRet[i] = Alphabets[rand() % size];
        sRet[i + 1] = '\0';
    }
    return sRet;
}