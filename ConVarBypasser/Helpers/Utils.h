#pragma once
#include "../SDK.h"

#define INRANGE(x, a, b) (x >= a && x <= b) 
#define getBits(x) (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA): (INRANGE(x, '0', '9') ? x - '0': 0))
#define getByte(x) (getBits(x[0]) << 4 | getBits(x[1]))

class Utils
{
public:
    uintptr_t FindPattern(const char* szModule, const char* szSignature);
    char* RandomStr(int length);
};

extern Utils g_Utils;