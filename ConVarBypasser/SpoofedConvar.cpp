#include "SpoofedConvar.h"

SpoofedConvar::SpoofedConvar(const char* szCVar, const char* newName)
{
	m_pOriginalCVar = gI_EngineCvar->FindVar(szCVar);
	Spoof(newName);
}

SpoofedConvar::SpoofedConvar(ConVar* pCVar, const char* newName)
{
	m_pOriginalCVar = pCVar;
	Spoof(newName);
}

SpoofedConvar::~SpoofedConvar() 
{
	if (IsSpoofed()) 
	{
		DWORD dwOld;

		//Restore flags
		SetFlags(m_iOriginalFlags);
		//Restore value
		SetString(m_szOriginalValue);
		//Restore the name
		VirtualProtect((LPVOID)m_pOriginalCVar->m_pszName, 128, PAGE_READWRITE, &dwOld);
		strcpy((char*)m_pOriginalCVar->m_pszName, m_szOriginalName);
		VirtualProtect((LPVOID)m_pOriginalCVar->m_pszName, 128, dwOld, &dwOld);

		//Unregister dummy cvar
		gI_EngineCvar->UnregisterConCommand(m_pDummyCVar);
		free(m_pDummyCVar);
		m_pDummyCVar = nullptr;
	}
}

bool SpoofedConvar::IsSpoofed() 
{
	return m_pDummyCVar != nullptr;
}

void SpoofedConvar::Spoof(const char* newName)
{
	if (!IsSpoofed() && m_pOriginalCVar) 
	{
		//Save old name value and flags so we can restore the cvar lates if needed
		m_iOriginalFlags = m_pOriginalCVar->m_nFlags;
		strcpy(m_szOriginalName, m_pOriginalCVar->m_pszName);
		strcpy(m_szOriginalValue, m_pOriginalCVar->GetDefault());


		sprintf_s(m_szDummyName, 128, "%s", newName);

		//Create the dummy cvar
		m_pDummyCVar = (ConVar*)malloc(sizeof(ConVar));
		if (!m_pDummyCVar)
		{
			return;
		}

		memcpy(m_pDummyCVar, m_pOriginalCVar, sizeof(ConVar));

		m_pDummyCVar->m_pNext = nullptr;
		//Register it
		gI_EngineCvar->RegisterConCommand(m_pDummyCVar);

		//Fix "write access violation" bullshit
		DWORD dwOld;
		VirtualProtect((LPVOID)m_pOriginalCVar->m_pszName, 128, PAGE_READWRITE, &dwOld);
		//Rename the cvar
		strcpy((char*)m_pOriginalCVar->m_pszName, m_szDummyName);
		VirtualProtect((LPVOID)m_pOriginalCVar->m_pszName, 128, dwOld, &dwOld);

		SetFlags(FCVAR_NONE);
	}
}

void SpoofedConvar::SetFlags(int flags) 
{
	if (IsSpoofed()) 
	{
		m_pOriginalCVar->m_nFlags = flags;
	}
}

int SpoofedConvar::GetFlags() 
{
	if (IsSpoofed()) 
	{
		return m_pOriginalCVar->m_nFlags;
	}

    return 0;
}

void SpoofedConvar::SetInt(int iValue) 
{
	if (IsSpoofed()) 
	{
		m_pOriginalCVar->SetValue(iValue);
	}
}

void SpoofedConvar::SetFloat(float flValue) 
{
	if (IsSpoofed()) 
	{
		m_pOriginalCVar->SetValue(flValue);
	}
}

void SpoofedConvar::SetString(const char* szValue) 
{
	if (IsSpoofed()) 
	{
		m_pOriginalCVar->SetValue(szValue);
	}
}

int SpoofedConvar::GetInt() 
{
	if (IsSpoofed()) 
	{
		return m_pOriginalCVar->GetInt();
	}

    return 0;
}

float SpoofedConvar::GetFloat() 
{
	if (IsSpoofed()) 
	{
		return m_pOriginalCVar->GetFloat();
	}

    return 0.f;
}

const char* SpoofedConvar::GetString() 
{
	if (IsSpoofed()) 
	{
		return m_pOriginalCVar->GetString();
	}

    return "";
}