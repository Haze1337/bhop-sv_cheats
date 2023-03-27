#pragma once
#include "SDK.h"

class SpoofedConvar 
{
public:
	SpoofedConvar(const char* szCVar, const char* newName);
	SpoofedConvar(ConVar* pCVar, const char* newName);

	~SpoofedConvar();

	bool		IsSpoofed();
	void		Spoof(const char* newName);

	void		SetFlags(int flags);
	int			GetFlags();

	void		SetInt(int iValue);
	void		SetFloat(float flValue);
	void		SetString(const char* szValue);

	int			GetInt();
	float		GetFloat();
	const char* GetString();

private:
	ConVar* m_pOriginalCVar = nullptr;
	ConVar* m_pDummyCVar = nullptr;

	char m_szDummyName[128];
	char m_szDummyValue[128];
	char m_szOriginalName[128];
	char m_szOriginalValue[128];
	int m_iOriginalFlags;
};