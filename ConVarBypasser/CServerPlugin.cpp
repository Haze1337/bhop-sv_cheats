#include "CServerPlugin.h"

extern void Main(CreateInterfaceFn ef, CreateInterfaceFn sf);

bool CServerPlugin::Load(CreateInterfaceFn ef, CreateInterfaceFn sf)
{
    Main(ef, sf);
    return true;
}

void CServerPlugin::Unload()
{
    
}

void CServerPlugin::Pause()
{
    
}

void CServerPlugin::UnPause()
{
    
}

const char* CServerPlugin::GetPluginDescription()
{
    return "sv_cheats bypass";
}

void CServerPlugin::LevelInit(const char* map_name)
{
    
}

void CServerPlugin::ServerActivate(edict_t* edict_list, int edict_count, int max_clients)
{

}

void CServerPlugin::GameFrame(bool simulating)
{

}

void CServerPlugin::LevelShutdown()
{
    
}

void CServerPlugin::ClientActive(edict_t* ent)
{
    
}

void CServerPlugin::ClientDisconnect(edict_t* ent)
{
    
}

void CServerPlugin::ClientPutInServer(edict_t* ent, const char* player_name)
{
    
}

void CServerPlugin::SetCommandClient(int index)
{
    
}

void CServerPlugin::ClientSettingsChanged(edict_t* edict)
{
    
}

PLUGIN_RESULT CServerPlugin::ClientConnect(bool* allow_connect, edict_t* ent, const char* name, const char* address, char* reject, int max_reject_len)
{
    return PLUGIN_RESULT::PLUGIN_CONTINUE;
}

PLUGIN_RESULT CServerPlugin::ClientCommand(edict_t* ent, const CCommand& args)
{
    return PLUGIN_RESULT::PLUGIN_CONTINUE;
}

PLUGIN_RESULT CServerPlugin::NetworkIDValidated(const char* name, const char* network_id)
{
    return PLUGIN_RESULT::PLUGIN_CONTINUE;
}

void CServerPlugin::OnQueryCvarValueFinished(int cookie, edict_t* ent, EQueryCvarValueStatus status, const char* name, const char* value)
{
    
}

void CServerPlugin::OnEdictAllocated(edict_t* edict)
{
    
}

void CServerPlugin::OnEdictFreed(const edict_t* edict)
{
    
}