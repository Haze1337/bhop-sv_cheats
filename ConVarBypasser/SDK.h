#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <cstring>
#include <Windows.h>
#include <stdio.h>
#include <Psapi.h>
#include <time.h>
#include "Helpers/Utils.h"

using CreateInterfaceFn = void* (*)(const char*, int*);

enum class InterfaceReturnStatus {
	OK,
	FAILED
};

struct Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

#include "SDK/ICvar.h"
#include "SDK/IServerPlugin.h"
#include "CServerPlugin.h"
#include "SpoofedConvar.h"
#include "Interfaces.h"