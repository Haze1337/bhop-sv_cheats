#pragma once
#include "../SDK.h"

class edict_t;
class CCommand;
typedef int QueryCvarCookie_t;

typedef enum
{
	eQueryCvarValueStatus_ValueIntact = 0,	// It got the value fine.
	eQueryCvarValueStatus_CvarNotFound = 1,
	eQueryCvarValueStatus_NotACvar = 2,		// There's a ConCommand, but it's not a ConVar.
	eQueryCvarValueStatus_CvarProtected = 3	// The cvar was marked with FCVAR_SERVER_CAN_NOT_QUERY, so the server is not allowed to have its value.
} EQueryCvarValueStatus;

typedef enum
{
	PLUGIN_CONTINUE = 0, // keep going
	PLUGIN_OVERRIDE, // run the game dll function but use our return value instead
	PLUGIN_STOP, // don't run the game dll function at all
} PLUGIN_RESULT;


class IServerPluginCallbacks
{
public:
	// Initialize the plugin to run
	// Return false if there is an error during startup.
	virtual bool			Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) = 0;

	// Called when the plugin should be shutdown
	virtual void			Unload(void) = 0;

	// called when a plugins execution is stopped but the plugin is not unloaded
	virtual void			Pause(void) = 0;

	// called when a plugin should start executing again (sometime after a Pause() call)
	virtual void			UnPause(void) = 0;

	// Returns string describing current plugin.  e.g., Admin-Mod.  
	virtual const char* GetPluginDescription(void) = 0;

	// Called any time a new level is started (after GameInit() also on level transitions within a game)
	virtual void			LevelInit(char const* pMapName) = 0;

	// The server is about to activate
	virtual void			ServerActivate(edict_t* pEdictList, int edictCount, int clientMax) = 0;

	// The server should run physics/think on all edicts
	virtual void			GameFrame(bool simulating) = 0;

	// Called when a level is shutdown (including changing levels)
	virtual void			LevelShutdown(void) = 0;

	// Client is going active
	virtual void			ClientActive(edict_t* pEntity) = 0;

	// Client is disconnecting from server
	virtual void			ClientDisconnect(edict_t* pEntity) = 0;

	// Client is connected and should be put in the game
	virtual void			ClientPutInServer(edict_t* pEntity, char const* playername) = 0;

	// Sets the client index for the client who typed the command into their console
	virtual void			SetCommandClient(int index) = 0;

	// A player changed one/several replicated cvars (name etc)
	virtual void			ClientSettingsChanged(edict_t* pEdict) = 0;

	// Client is connecting to server ( set retVal to false to reject the connection )
	//	You can specify a rejection message by writing it into reject
	virtual PLUGIN_RESULT	ClientConnect(bool* bAllowConnect, edict_t* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen) = 0;

	// The client has typed a command at the console
	virtual PLUGIN_RESULT	ClientCommand(edict_t* pEntity, const CCommand& args) = 0;

	// A user has had their network id setup and validated 
	virtual PLUGIN_RESULT	NetworkIDValidated(const char* pszUserName, const char* pszNetworkID) = 0;

	// This is called when a query from IServerPluginHelpers::StartQueryCvarValue is finished.
	// iCookie is the value returned by IServerPluginHelpers::StartQueryCvarValue.
	// Added with version 2 of the interface.
	virtual void			OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t* pPlayerEntity, EQueryCvarValueStatus eStatus, const char* pCvarName, const char* pCvarValue) = 0;

	// added with version 3 of the interface.
	virtual void			OnEdictAllocated(edict_t* edict) = 0;
	virtual void			OnEdictFreed(const edict_t* edict) = 0;
};