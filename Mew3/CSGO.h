#pragma once
#include "stdafx.h"
#include <map>
#include <memory>
#include <vector>
#include "Memory.h"
#include "LocalPlayer.h"
#include "math.h"

#define FL_ONGROUND (1 << 0)
#define FL_PARTIALGROUND (1 << 18)

class CSGO
{
public:
	bool		init();
	bool		readConfig();
	bool		GetDLLs();

	//config values:
	bool		triggerbot;
	bool		legittrigger;
	bool		bunnyhop;
	bool		aimbot;
	bool		rcs;
	int			triggerkey;
	int			bhopkey;
	int			aimkey;
	int			togglekey;
	int			triggerdelay;
	int			smoothaim;
	int			boneToAim;
	float		MaxRandomPitch;
	float		MaxRandomYaw;
	float		MinRandomPitch;
	float		MinRandomYaw;
	float		SmoothPercent;
	float		StaticPitch;
	float		StaticYaw;
	float		minimalDist;

	//misc
	float		m_PunchAmount = 2.f;

	//methods
	DWORD		returnPlayer(int);
	DWORD		returnLocalPlayer();
	DWORD		returnPlayerInCrosshair(DWORD);
	int			returnTeam(DWORD);
	int			crosshairID(DWORD);
	int			playerHP(int);
	bool		playerDefuse(int);
	std::string	playerRank(int);
	std::string	playerName(int);
	int			playerWins(int);
	int			getFlags();
	bool		playerHasKit(int);
	uintptr_t	getBoneMatrix(DWORD);
	D3DXVECTOR3	getLocalEyePos();
	D3DXVECTOR3	getVecOrigin(DWORD);
	D3DXVECTOR3	getVecViewOffset(DWORD);
	D3DXVECTOR3	getBone(int, DWORD);
	D3DXVECTOR3	getHeadBonePos(DWORD);
	D3DXVECTOR3	getLocalViewAngles();
	D3DXVECTOR3	getLocalPunchAngles();
	float		get3DDistance(D3DXVECTOR3, D3DXVECTOR3);
	bool		playerDormant(DWORD);
	bool		validateEntity(DWORD);
	bool		playerSpotted(DWORD);
	bool		isVisible(DWORD, int);
	bool		inAir(DWORD);
	void		setAngle(D3DXVECTOR3);
	void		shoot(bool);
	DWORD		enginePointer();
	D3DXVECTOR3	getaimPunchAngle(DWORD);
	D3DXVECTOR3	getvecView(DWORD);
	D3DXVECTOR3 getVecVelocity(DWORD);
	int			shotsFired();

	struct BoneMatrix
	{
		char pad[0x0C];
		float boneX;
		char pad1[0x0C];
		float boneY;
		char pad2[0x0C];
		float boneZ;
	};


	wchar_t pName[32];

	Memory		CSGOMemory;
	HWND		CSGOwindow;
	uintptr_t	CSGOBase;
	size_t		CSGOBase_Size;
	uintptr_t	ClientDLL;
	size_t		ClientDLL_Size;
	uintptr_t	EngineDLL;
	size_t		EngineDLL_Size;

	// Wed, 31 May 2017 13:31:26 +0200
	DWORD dwLocalPlayer = 0xAA8FDC;
	DWORD dwEntityList = 0x4A853C4;
	DWORD m_iCrosshairId = 0xB2B4;
	DWORD m_iTeamNum = 0xF0;
	DWORD m_iHealth = 0xFC;;
	DWORD m_szCustomName = 0x301C;
	DWORD m_iCompetitiveRanking = 0x1A44;
	DWORD m_iCompetitiveWins = 0x1B48;
	DWORD m_bIsDefusing = 0x38A4;
	DWORD dwRadarBase = 0x4EBA0BC;
	DWORD dwPlayerResource = 0x2EC5ADC;
	DWORD m_fFlags = 0x100;
	DWORD m_vecVelocity = 0x110;
	DWORD dwForceJump = 0x4F1C354;
	DWORD m_bHasDefuser = 0xB258;
	DWORD m_vecOrigin = 0x134;
	DWORD m_vecViewOffset = 0x104;
	DWORD m_dwBoneMatrix = 0x2698;
	DWORD m_bDormant = 0xE9;
	DWORD m_bSpotted = 0x939;
	DWORD m_bSpottedByMask = 0x97C;
	DWORD m_aimPunchAngle = 0x301C;
	DWORD m_aimPunchAngleVel = 0x3028;
	DWORD m_iShotsFired;
	DWORD m_viewPunchAngle;
	DWORD dwForceAttack;


	//neu:
	DWORD dwClientState = 0x59F21C;
	DWORD dwClientState_ViewAngles;

	//rework:
	const DWORD _ENTLOOPDISTANCE = 0x10;

	std::string ranks[19] = { "Unranked", "Silver I", "Silver II", "Silver III", "Silver IV", "Silver Elite",
								"Silver Elite Master", "Gold Nova I", "Gold Nova II", "Gold Nova III", "Gold Nova Elite",
								"Master Guardian I", "Master Guardian II", "Master Guardian Elite", "Distinguished Master Guardian",
								"Legendary Eagle", "Legendary Eagle Master", "Supreme Master First Class", "The Global Elite" };

	LocalPlayer	myPlayer;
};

