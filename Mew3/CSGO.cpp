#pragma once
#include "CSGO.h"

bool CSGO::init()
{
	//make sure csgo is running
	while (!CSGOMemory.openProcess("csgo.exe")) 
	{ 
		std::cout << "Waiting for CS:GO to open ..." << std::endl;
		Sleep(1000);
	}
	std::cout << "\a";
	system("cls");
	//Sleep(15000);
	//Find Window with given Titel to return to the Window Handle
	//Needed to check the foreground window
	CSGOwindow = FindWindowA(NULL, "Counter-Strike: Global Offensive");

	if (!CSGO::readConfig()) return false;
	if (!CSGO::GetDLLs()) return false;
	return true;
}

bool CSGO::readConfig()
{
	std::cout << "Reading Config\t..." << std::endl;

	TCHAR path[MAX_PATH];
	GetCurrentDirectory(sizeof(path), path);

	LPCSTR config = _T("\\config.ini");
	LPCSTR path2 = _T(path);

	std::string combined = std::string(path2) + config;
	LPCSTR fullPath = combined.c_str();

	//check if config file exists and is readable
	std::ifstream cfgfile(fullPath);
	if (!cfgfile.good())
	{
		std::cout << "No config file found! Make sure the config.ini file is available at " << fullPath << std::endl;
		return false;
	}

	//features
	CSGO::triggerbot = GetPrivateProfileInt("features", "triggerbot", 0, fullPath);
	CSGO::bunnyhop = GetPrivateProfileInt("features", "bunnyhop", 0, fullPath);
	CSGO::aimbot = GetPrivateProfileInt("features", "aimbot", 0, fullPath);;
	CSGO::rcs = GetPrivateProfileInt("features", "antirecoil", 0, fullPath);

	//misc
	CSGO::triggerdelay = GetPrivateProfileInt("misc", "triggerdelay", 0, fullPath);
	CSGO::smoothaim = GetPrivateProfileInt("misc", "smoothaim", 0, fullPath);;
	CSGO::MaxRandomPitch = GetPrivateProfileInt("misc", "MaxRandomPitch", 0, fullPath);;;
	CSGO::MaxRandomYaw = GetPrivateProfileInt("misc", "MaxRandomYaw", 0, fullPath);;;
	CSGO::MinRandomPitch = GetPrivateProfileInt("misc", "MinRandomPitch", 0, fullPath);;;
	CSGO::MinRandomYaw = GetPrivateProfileInt("misc", "MinRandomYaw", 0, fullPath);;;
	CSGO::SmoothPercent = GetPrivateProfileInt("misc", "SmoothPercent", 0, fullPath);;;
	CSGO::StaticPitch = GetPrivateProfileInt("misc", "StaticPitch", 0, fullPath);;;
	CSGO::StaticYaw = GetPrivateProfileInt("misc", "StaticYaw", 0, fullPath);;;
	CSGO::boneToAim = GetPrivateProfileInt("misc", "boneToAim", 0, fullPath);;;
	CSGO::minimalDist = GetPrivateProfileInt("misc", "minimalDist", 0, fullPath);;;

	//keys
	CSGO::triggerkey = GetPrivateProfileInt("keys", "triggerkey", 0, fullPath);
	CSGO::bhopkey = GetPrivateProfileInt("keys", "bhopkey", 0, fullPath);
	CSGO::aimkey = GetPrivateProfileInt("keys", "aimkey", 0, fullPath);
	CSGO::togglekey = GetPrivateProfileInt("keys", "togglekey", 0, fullPath);

	//offsets
	//change after every update - necessary for the hack to work!
	dwLocalPlayer = GetPrivateProfileInt("offsets", "dwLocalPlayer", 0, fullPath);;
	dwEntityList = GetPrivateProfileInt("offsets", "dwEntityList", 0, fullPath);;
	m_iCrosshairId = GetPrivateProfileInt("offsets", "m_iCrosshairId", 0, fullPath);;
	m_vecOrigin = GetPrivateProfileInt("offsets", "m_vecOrigin", 0, fullPath);;
	m_vecViewOffset = GetPrivateProfileInt("offsets", "m_vecViewOffset", 0, fullPath);;
	m_dwBoneMatrix = GetPrivateProfileInt("offsets", "m_dwBoneMatrix", 0, fullPath);;
	dwClientState = GetPrivateProfileInt("offsets", "dwClientState", 0, fullPath);;
	dwClientState_ViewAngles = GetPrivateProfileInt("offsets", "dwClientState_ViewAngles", 0, fullPath);;
	m_bSpotted = GetPrivateProfileInt("offsets", "m_bSpotted", 0, fullPath);;
	m_bSpottedByMask = GetPrivateProfileInt("offsets", "m_bSpottedByMask", 0, fullPath);;
	m_aimPunchAngle = GetPrivateProfileInt("offsets", "m_aimPunchAngle", 0, fullPath);;
	m_aimPunchAngleVel = GetPrivateProfileInt("offsets", "m_aimPunchAngleVel", 0, fullPath);;
	m_iShotsFired = GetPrivateProfileInt("offsets", "m_iShotsFired", 0, fullPath);;
	dwForceAttack = GetPrivateProfileInt("offsets", "dwForceAttack", 0, fullPath);

	//usually don't change
	m_iTeamNum = GetPrivateProfileInt("offsets", "m_iTeamNum", 0, fullPath);;
	m_iHealth = GetPrivateProfileInt("offsets", "m_iHealth", 0, fullPath);;
	m_fFlags = GetPrivateProfileInt("offsets", "m_fFlags", 0, fullPath);;

	//not used - no need to change
	m_szCustomName = GetPrivateProfileInt("offsets", "m_szCustomName", 0, fullPath);;
	m_iCompetitiveRanking = GetPrivateProfileInt("offsets", "m_iCompetitiveRanking", 0, fullPath);;
	m_iCompetitiveWins = GetPrivateProfileInt("offsets", "m_iCompetitiveWins", 0, fullPath);;
	m_bIsDefusing = GetPrivateProfileInt("offsets", "m_bIsDefusing", 0, fullPath);;
	dwRadarBase = GetPrivateProfileInt("offsets", "dwRadarBase", 0, fullPath);;
	dwPlayerResource = GetPrivateProfileInt("offsets", "dwPlayerResource", 0, fullPath);;
	m_vecVelocity = GetPrivateProfileInt("offsets", "m_vecVelocity", 0, fullPath);;
	m_bHasDefuser = GetPrivateProfileInt("offsets", "m_bHasDefuser", 0, fullPath);;
	dwForceJump = GetPrivateProfileInt("offsets", "dwForceJump", 0, fullPath);;

	return true;
}

bool CSGO::GetDLLs()
{
	//Initialize and set process to CSGOMemory Object

	std::cout << "Reading Modules\t..." << std::endl;

	//Initialize Module & Size Pointers
	CSGOBase = CSGOMemory.Module("csgo.exe");
	CSGOBase_Size = CSGOMemory.ModuleSize("csgo.exe");
	ClientDLL = CSGOMemory.Module("client.dll");
	ClientDLL_Size = CSGOMemory.ModuleSize("client.dll");
	EngineDLL = CSGOMemory.Module("engine.dll");
	EngineDLL_Size = CSGOMemory.ModuleSize("engine.dll");

	std::cout << std::endl;
	std::cout << "CSGOBASE: \t" << CSGOBase << "\t" << CSGOBase_Size << std::endl;
	std::cout << "ClientDLL: \t" << ClientDLL << "\t" << ClientDLL_Size << std::endl;
	std::cout << "EngineDLL: \t" << EngineDLL << "\t" << EngineDLL_Size << std::endl;

	return true;
}

DWORD CSGO::returnPlayer(int i)
{
	return CSGOMemory.read<DWORD>(ClientDLL + dwEntityList + (i * 0x10)); //0x10 = EntityLoopDistance
}

DWORD CSGO::returnLocalPlayer()
{
	return CSGOMemory.read<DWORD>(ClientDLL + dwLocalPlayer);
}

DWORD CSGO::returnPlayerInCrosshair(DWORD CrossHairID)
{
	return CSGOMemory.read<DWORD>(ClientDLL + dwEntityList + +((CrossHairID - 1) * _ENTLOOPDISTANCE));
}

int CSGO::returnTeam(DWORD player)
{
	return CSGOMemory.read<int>(player + m_iTeamNum);
}

int CSGO::crosshairID(DWORD player)
{
	return CSGOMemory.read<int>(player + m_iCrosshairId);
}

int CSGO::playerHP(int player)
{
	return CSGOMemory.read<DWORD>(player + m_iHealth);
}

bool CSGO::playerDefuse(int player)
{
	return CSGOMemory.read<DWORD>(returnPlayer(player) + m_bIsDefusing);
}

std::string CSGO::playerRank(int player)
{
	int GameResources = CSGOMemory.read<int>(ClientDLL + dwPlayerResource);
	int currentRank = CSGOMemory.read<int>(GameResources + m_iCompetitiveRanking + (player * 4));
	std::cout << "GameResources: " << GameResources << std::endl;
	std::cout << "m_iCompetitiveRanking: " << m_iCompetitiveRanking << std::endl;
	std::cout << "player: " << player << std::endl;
	std::cout << "currentRank: " << currentRank << std::endl << std::endl;

	if (currentRank <= sizeof(ranks))
	{
		return ranks[currentRank];
	}
	else
	{
		return "Error";
	}
}

std::string CSGO::playerName(int player)
{
	int currentRadarBase = CSGOMemory.read<int>(ClientDLL + dwRadarBase);
	int radar = CSGOMemory.read<int>(currentRadarBase + 0x20);

	wchar_t firstChar;

	CSGOMemory.read((radar + (0x1EC * returnPlayer(player)) + 0x3C), &firstChar, sizeof(wchar_t));
	if (firstChar != pName[0])
	{
		CSGOMemory.read(radar + (0x1EC * returnPlayer(player)) + 0x3C, pName, sizeof(wchar_t) * 32);
	}
	char nameCh[32];
	char DefChar = ' ';
	WideCharToMultiByte(CP_ACP, 0, pName, -1, nameCh, 260, &DefChar, NULL);

	std::string ss(nameCh);

	return ss;
}

int CSGO::playerWins(int player)
{
	return CSGOMemory.read<DWORD>(player + m_iCompetitiveWins);
}

bool CSGO::playerHasKit(int player)
{
	return CSGOMemory.read<DWORD>(returnPlayer(player) + m_bHasDefuser);
}

D3DXVECTOR3 CSGO::getLocalEyePos()
{
	return CSGO::getVecOrigin(CSGO::returnLocalPlayer());;
}

D3DXVECTOR3 CSGO::getVecOrigin(DWORD entity)
{
	return CSGOMemory.read<D3DXVECTOR3>(entity + m_vecOrigin);
}

D3DXVECTOR3 CSGO::getVecViewOffset(DWORD entity)
{
	return CSGOMemory.read<D3DXVECTOR3>(entity + m_vecViewOffset);
}

uintptr_t CSGO::getBoneMatrix(DWORD player)
{
	return CSGOMemory.read<uintptr_t>(player + m_dwBoneMatrix);
}

D3DXVECTOR3 CSGO::getBone(int bone, DWORD player)
{
	DWORD dwBoneBase = getBoneMatrix(player);
	D3DXVECTOR3 customBone;

	customBone.x = CSGOMemory.read<float>(dwBoneBase + 0x30 * bone + 0x0C);
	customBone.y = CSGOMemory.read<float>(dwBoneBase + 0x30 * bone + 0x1C);
	customBone.z = CSGOMemory.read<float>(dwBoneBase + 0x30 * bone + 0x2C);
	return customBone;
}

D3DXVECTOR3 CSGO::getHeadBonePos(DWORD player)
{
	return CSGO::getBone(8, player);
}

D3DXVECTOR3 CSGO::getLocalViewAngles()
{
	return CSGOMemory.read<D3DXVECTOR3>(enginePointer() + dwClientState_ViewAngles);
}

D3DXVECTOR3 CSGO::getLocalPunchAngles()
{
	return CSGOMemory.read<D3DXVECTOR3>(returnLocalPlayer() + m_aimPunchAngle);;
}

float CSGO::get3DDistance(D3DXVECTOR3 PlayerPosition, D3DXVECTOR3 EnemyPosition)
{
	return sqrt(powf(EnemyPosition.x - PlayerPosition.x, 2.f) + powf(EnemyPosition.y - PlayerPosition.y, 2.f) + powf(EnemyPosition.z - PlayerPosition.z, 2.f));
}

bool CSGO::playerDormant(DWORD player)
{
	return CSGOMemory.read<bool>(player + m_bDormant);
}
//This method returns true if the entity is alive, not local, not dormant 
bool CSGO::validateEntity(DWORD player)
{
	if (!playerDormant(player) && /*playerHP(player) > 0 && */returnLocalPlayer() != player)
	{
		return true;
	}
	else return false;
}

bool CSGO::playerSpotted(DWORD player)
{
	return CSGOMemory.read<bool>(player + m_bSpotted);
}

//tbd
bool CSGO::isVisible(DWORD entity, int BoneID)
{
	if (BoneID < 0) return false;
	return true;
}

bool CSGO::inAir(DWORD entity)
{
	DWORD entityFlag = CSGOMemory.read<DWORD>(entity + m_fFlags);

	if ((entityFlag & FL_ONGROUND) || (entityFlag & FL_PARTIALGROUND))
	{
		return false;
	}
	else return true;
}

//USE CAREFULLY!!!!! (!>180 & !>89 OR BAN!)
void CSGO::setAngle(D3DXVECTOR3 customAngle)
{
	//CSGOMemory.write<D3DXVECTOR3>(enginePointer() + dwClientState_ViewAngles, customAngle);
	CSGOMemory.write2<D3DXVECTOR3>(enginePointer() + dwClientState_ViewAngles, customAngle);
}

void CSGO::shoot(bool attack)
{
	if (attack)
	{
		CSGOMemory.write2<int>(enginePointer() + dwForceAttack, 5);
	}
	else
	{
		CSGOMemory.write2<int>(enginePointer() + dwForceAttack, 4);

	}
}

DWORD CSGO::enginePointer()
{
	return CSGOMemory.read<DWORD>(CSGO::EngineDLL + dwClientState);
}

D3DXVECTOR3 CSGO::getaimPunchAngle(DWORD entityAddress)
{
	return CSGOMemory.read<D3DXVECTOR3>(entityAddress + m_aimPunchAngle);
}

D3DXVECTOR3 CSGO::getvecView(DWORD entityAddress)
{
	return CSGOMemory.read<D3DXVECTOR3>(entityAddress + m_vecViewOffset);
}

D3DXVECTOR3 CSGO::getVecVelocity(DWORD entityAddress)
{
	return CSGOMemory.read<D3DXVECTOR3>(entityAddress + m_vecVelocity);
}

int CSGO::shotsFired()
{
	return CSGOMemory.read<int>(returnLocalPlayer() + m_iShotsFired);
}


//unused -> mfFlags is a bitmasked value
int CSGO::getFlags()
{
	//257 - bottom
	//256 - air
	return CSGOMemory.read<int>(returnLocalPlayer() + m_fFlags);
}









