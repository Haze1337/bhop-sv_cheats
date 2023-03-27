#pragma once
#include "SDK.h"

//------------------------------------------//
// CreateInterface
//------------------------------------------//
extern ICvar* gI_EngineCvar;
//------------------------------------------//
// Initialize Interfaces
//------------------------------------------//

namespace Interfaces
{
	void Initialize(CreateInterfaceFn ef);
};