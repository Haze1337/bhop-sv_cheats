#pragma once

#define Assert(_exp) ((void)0)

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class IConCommandBaseAccessor;
class ConCommand;
struct CVarDLLIdentifier_t;
class ICvarQuery;
class IConVar;
class CCommand;
struct Color;

//-----------------------------------------------------------------------------
// ConVar flags
//-----------------------------------------------------------------------------
// The default, no flags at all
#define FCVAR_NONE				0 

// Command to ConVars and ConCommands
// ConVar Systems
#define FCVAR_UNREGISTERED		(1<<0)	// If this is set, don't add to linked list, etc.
#define FCVAR_DEVELOPMENTONLY	(1<<1)	// Hidden in released products. Flag is removed automatically if ALLOW_DEVELOPMENT_CVARS is defined.
#define FCVAR_GAMEDLL			(1<<2)	// defined by the game DLL
#define FCVAR_CLIENTDLL			(1<<3)  // defined by the client DLL
#define FCVAR_HIDDEN			(1<<4)	// Hidden. Doesn't appear in find or autocomplete. Like DEVELOPMENTONLY, but can't be compiled out.

// ConVar only
#define FCVAR_PROTECTED			(1<<5)  // It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value
#define FCVAR_SPONLY			(1<<6)  // This cvar cannot be changed by clients connected to a multiplayer server.
#define	FCVAR_ARCHIVE			(1<<7)	// set to cause it to be saved to vars.rc
#define	FCVAR_NOTIFY			(1<<8)	// notifies players when changed
#define	FCVAR_USERINFO			(1<<9)	// changes the client's info string
#define FCVAR_CHEAT				(1<<14) // Only useable in singleplayer / debug / multiplayer & sv_cheats

#define FCVAR_PRINTABLEONLY		(1<<10)  // This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
#define FCVAR_UNLOGGED			(1<<11)  // If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
#define FCVAR_NEVER_AS_STRING	(1<<12)  // never try to print that cvar

// It's a ConVar that's shared between the client and the server.
// At signon, the values of all such ConVars are sent from the server to the client (skipped for local
//  client, of course )
// If a change is requested it must come from the console (i.e., no remote client changes)
// If a value is changed while a server is active, it's replicated to all connected clients
#define FCVAR_REPLICATED		(1<<13)	// server setting enforced on clients, TODO rename to FCAR_SERVER at some time
#define FCVAR_DEMO				(1<<16)  // record this cvar when starting a demo file
#define FCVAR_DONTRECORD		(1<<17)  // don't record these command in demofiles
#define FCVAR_RELOAD_MATERIALS	(1<<20)	// If this cvar changes, it forces a material reload
#define FCVAR_RELOAD_TEXTURES	(1<<21)	// If this cvar changes, if forces a texture reload

#define FCVAR_NOT_CONNECTED		(1<<22)	// cvar cannot be changed by a client that is connected to a server
#define FCVAR_MATERIAL_SYSTEM_THREAD (1<<23)	// Indicates this cvar is read from the material system thread
#define FCVAR_ARCHIVE_XBOX		(1<<24) // cvar written to config.cfg on the Xbox

#define FCVAR_ACCESSIBLE_FROM_THREADS	(1<<25)	// used as a debugging tool necessary to check material system thread convars

#define FCVAR_SERVER_CAN_EXECUTE	(1<<28)// the server is allowed to execute this command on clients via ClientCommand/NET_StringCmd/CBaseClientState::ProcessStringCmd.
#define FCVAR_SERVER_CANNOT_QUERY	(1<<29)// If this is set, then the server is not allowed to query this cvar's value (via IServerPluginHelpers::StartQueryCvarValue).
#define FCVAR_CLIENTCMD_CAN_EXECUTE	(1<<30)	// IVEngineClient::ClientCmd is allowed to execute this command. 
											// Note: IVEngineClient::ClientCmd_Unrestricted can run any client command.

// #define FCVAR_AVAILABLE			(1<<15)
// #define FCVAR_AVAILABLE			(1<<18)
// #define FCVAR_AVAILABLE			(1<<19)
// #define FCVAR_AVAILABLE			(1<<20)
// #define FCVAR_AVAILABLE			(1<<21)
// #define FCVAR_AVAILABLE			(1<<23)
// #define FCVAR_AVAILABLE			(1<<26)
// #define FCVAR_AVAILABLE			(1<<27)
// #define FCVAR_AVAILABLE			(1<<31)

#define FCVAR_MATERIAL_THREAD_MASK ( FCVAR_RELOAD_MATERIALS | FCVAR_RELOAD_TEXTURES | FCVAR_MATERIAL_SYSTEM_THREAD )	

//-----------------------------------------------------------------------------
// Called when a ConVar changes value
// NOTE: For FCVAR_NEVER_AS_STRING ConVars, pOldValue == NULL
//-----------------------------------------------------------------------------
typedef void (*FnChangeCallback_t)(IConVar* var, const char* pOldValue, float flOldValue);

class ConCommandBase
{
public:
	virtual						~ConCommandBase(void);
	virtual	bool				IsCommand(void) const;

	// Check flag
	virtual bool				IsFlagSet(int flag) const;

	// Set flag
	virtual void				AddFlags(int flags);

	// Return name of cvar
	virtual const char* GetName(void) const;

	// Return help text for cvar
	virtual const char* GetHelpText(void) const;
	virtual bool				IsRegistered(void) const;

	// Returns the DLL identifier
	virtual CVarDLLIdentifier_t	GetDLLIdentifier() const;
	virtual void				CreateBase(const char* pName, const char* pHelpString = 0, int flags = 0);

	// Used internally by OneTimeInit to initialize/shutdown
	virtual void				Init();
 
	// Next ConVar in chain
	// Prior to register, it points to the next convar in the DLL.
	// Once registered, though, m_pNext is reset to point to the next
	// convar in the global list
	ConCommandBase* m_pNext;

	// Has the cvar been added to the global list?
	bool						m_bRegistered;

	// Static data
	const char* m_pszName;
	const char* m_pszHelpString;

	// ConVar flags
	int							m_nFlags;

	// ConVars add themselves to this list for the executable. 
	// Then ConVar_Register runs through  all the console variables 
	// and registers them into a global list stored in vstdlib.dll
	static ConCommandBase* s_pConCommandBases;

	// ConVars in this executable use this 'global' to access values.
	static IConCommandBaseAccessor* s_pAccessor;
};

//-----------------------------------------------------------------------------
// Abstract interface for ConVars
//-----------------------------------------------------------------------------
class IConVar
{
public:
	// Value set
	virtual void SetValue(const char* pValue) = 0;
	virtual void SetValue(float flValue) = 0;
	virtual void SetValue(int nValue) = 0;

	// Return name of command
	virtual const char* GetName(void) const = 0;

	// Accessors.. not as efficient as using GetState()/GetInfo()
	// if you call these methods multiple times on the same IConVar
	virtual bool IsFlagSet(int nFlag) const = 0;
};



class ConVar : public ConCommandBase, public IConVar
{
public:
	ConVar* m_pParent;
	const char* m_pszDefaultValue;
	char* m_pszString;
	int m_StringLength;
	float m_fValue;
	int m_nValue;
	bool m_bHasMin;
	float m_fMinVal;
	bool m_bHasMax;
	float m_fMaxVal;
	FnChangeCallback_t	m_fnChangeCallback;

	virtual						~ConVar(void) = 0;

	virtual bool				IsFlagSet(int flag) const = 0;
	virtual const char* GetHelpText(void) const = 0;
	virtual bool				IsRegistered(void) const = 0;
	virtual const char* GetName(void) const = 0;
	virtual void				AddFlags(int flags) = 0;
	virtual	bool				IsCommand(void) const = 0;

	// Retrieve value
	const char* GetString(void) const { return m_pParent->m_pszString; }

	float GetFloat(void) const { return m_pParent->m_fValue; }

	int GetInt(void) const { return m_pParent->m_nValue; }

	bool GetBool() { return !!GetInt(); }

	// Any function that allocates/frees memory needs to be virtual or else you'll have crashes
	//  from alloc/free across dll/exe boundaries.

	// These just call into the IConCommandBaseAccessor to check flags and set the var (which ends up calling InternalSetValue).
	virtual void				SetValue(const char* value) = 0;
	virtual void				SetValue(float value) = 0;
	virtual void				SetValue(int value) = 0;

	// Reset to default value
	void						Revert(void) {
		ConVar* var = (ConVar*)m_pParent; var->SetValue(var->m_pszDefaultValue);
	};

	// True if it has a min/max setting
	bool						GetMin(float& minVal) const { minVal = m_pParent->m_fMinVal; return m_pParent->m_bHasMin; };
	bool						GetMax(float& maxVal) const { maxVal = m_pParent->m_fMaxVal; return m_pParent->m_bHasMax; };
	const char* GetDefault(void) const { return m_pParent->m_pszDefaultValue; };
	void						SetDefault(const char* pszDefault) { static const char* empty_string = ""; m_pszDefaultValue = pszDefault ? pszDefault : empty_string; Assert(m_pszDefaultValue); };

	// Called by CCvar when the value of a var is changing.
	virtual void				InternalSetValue(const char* value) = 0;
	// For CVARs marked FCVAR_NEVER_AS_STRING
	virtual void				InternalSetFloatValue(float fNewValue) = 0;
	virtual void				InternalSetIntValue(int nValue) = 0;

	virtual bool				ClampValue(float& value) = 0;
	virtual void				ChangeStringValue(const char* tempVal, float flOldValue) = 0;

	virtual void				Create(const char* pName, const char* pDefaultValue, int flags = 0,
										const char* pHelpString = 0, bool bMin = false, float fMin = 0.0,
										bool bMax = false, float fMax = false, FnChangeCallback_t callback = 0) = 0;

	// Used internally by OneTimeInit to initialize.
	virtual void				Init() = 0;
	int GetFlags() { return m_pParent->m_nFlags; }
};

//-----------------------------------------------------------------------------
// Used to display console messages
//-----------------------------------------------------------------------------
class IConsoleDisplayFunc
{
public:
	virtual void ColorPrint(const Color& clr, const char* pMessage) = 0;
	virtual void Print(const char* pMessage) = 0;
	virtual void DPrint(const char* pMessage) = 0;
};

enum InitReturnVal_t
{
	INIT_FAILED = 0,
	INIT_OK,

	INIT_LAST_VAL,
};

class IAppSystem
{
public:
	virtual bool Connect(CreateInterfaceFn factory) = 0;
	virtual void Disconnect() = 0;
	virtual void* QueryInterface(const char* pInterfaceName) = 0;
	virtual InitReturnVal_t Init() = 0;
	virtual void Shutdown() = 0;
};

//-----------------------------------------------------------------------------
// Purpose: DLL interface to ConVars/ConCommands
//-----------------------------------------------------------------------------
class ICvar : public IAppSystem
{
public:
	virtual CVarDLLIdentifier_t AllocateDLLIdentifier() = 0;
	virtual void RegisterConCommand(ConCommandBase* pCommandBase) = 0;
	virtual void UnregisterConCommand(ConCommandBase* pCommandBase) = 0;
	virtual void UnregisterConCommands(CVarDLLIdentifier_t id) = 0;
	virtual const char* GetCommandLineValue(const char* pVariableName) = 0;
	virtual ConCommandBase* FindCommandBase(const char* name) = 0;
	virtual const ConCommandBase* FindCommandBase(const char* name) const = 0;
	virtual ConVar* FindVar(const char* var_name) = 0;
	virtual const ConVar* FindVar(const char* var_name) const = 0;
	virtual ConCommand* FindCommand(const char* name) = 0;
	virtual const ConCommand* FindCommand(const char* name) const = 0;
	virtual ConCommandBase* GetCommands(void) = 0;
	virtual const ConVar* GetCommands(void) const = 0;
	virtual void InstallGlobalChangeCallback(FnChangeCallback_t callback) = 0;
	virtual void RemoveGlobalChangeCallback(FnChangeCallback_t callback) = 0;
	virtual void CallGlobalChangeCallbacks(ConVar* var, const char* pOldString, float flOldValue) = 0;
	virtual void InstallConsoleDisplayFunc(IConsoleDisplayFunc* pDisplayFunc) = 0;
	virtual void RemoveConsoleDisplayFunc(IConsoleDisplayFunc* pDisplayFunc) = 0;
	virtual void ConsoleColorPrintf(const Color& clr, const char* pFormat, ...) const = 0;
	virtual void ConsolePrintf(const char* pFormat, ...) const = 0;
	virtual void ConsoleDPrintf(const char* pFormat, ...) const = 0;
	virtual void RevertFlaggedConVars(int nFlag) = 0;

	// Method allowing the engine ICvarQuery interface to take over
	// A little hacky, owing to the fact the engine is loaded
	// well after ICVar, so we can't use the standard connect pattern
	virtual void InstallCVarQuery(ICvarQuery* pQuery) = 0;
	virtual bool IsMaterialThreadSetAllowed() const = 0;
	virtual void QueueMaterialThreadSetValue(ConVar* pConVar, const char* pValue) = 0;
	virtual void QueueMaterialThreadSetValue(ConVar* pConVar, int nValue) = 0;
	virtual void QueueMaterialThreadSetValue(ConVar* pConVar, float flValue) = 0;
	virtual bool HasQueuedMaterialThreadConVarSets() const = 0;
	virtual int	ProcessQueuedMaterialThreadConVarSets() = 0;
};