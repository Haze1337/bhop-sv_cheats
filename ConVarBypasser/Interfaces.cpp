#include "Interfaces.h"

//------------------------------------------//
// CreateInterface
//------------------------------------------//
ICvar* gI_EngineCvar = nullptr;
//------------------------------------------//
// Initialize Interfaces
//------------------------------------------//
void Interfaces::Initialize(CreateInterfaceFn ef)
{
	//------------------------------------------//
	// Factories
	//------------------------------------------//
	gI_EngineCvar = reinterpret_cast<ICvar*>(ef("VEngineCvar004", nullptr));
}