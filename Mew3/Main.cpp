#include "stdafx.h"
#include <thread>
#include <ctime>
#include "CSGO.h"
#include "math.h"

CSGO myCS;
bool cheatEnabled = true;
D3DXVECTOR3 oldAngle, newAngle;
int shotsFired;

void rcs()
{
	while (1)
	{
		shotsFired = myCS.shotsFired();
		if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && shotsFired > 1 && myCS.rcs == 1 && myCS.aimbot == 0)
		{
			D3DXVECTOR3 m_ViewAngle = myCS.getLocalViewAngles();
			D3DXVECTOR3 m_PunchAngle = myCS.getLocalPunchAngles();

			m_ViewAngle.x += oldAngle.x;
			m_ViewAngle.y += oldAngle.y;
			// Add the old "viewpunch" so we get the "center" of the screen again
			newAngle.x = m_ViewAngle.x - m_PunchAngle.x * 2.0f;
			newAngle.y = m_ViewAngle.y - m_PunchAngle.y * 2.0f;
			// remove the new "viewpunch" from the viewangles
			myCS.setAngle(Math::D3DXVECTOR::ClampAngle(newAngle));

			oldAngle.x = m_PunchAngle.x * 2.0f;
			oldAngle.y = m_PunchAngle.y * 2.0f;
			// save the old "viewpunch"
		}
		else {
			oldAngle.x = 0;
			oldAngle.y = 0;
		}
		Sleep(1);
	}
}

void scctaimRCS()
{
	int flags = myCS.getFlags() & 0x1;

	DWORD tempEnemy;

	int i;

	while (1)
	{
		float aimbotTimer = 0.f;
		DWORD LocalPlayer = myCS.returnLocalPlayer();
		DWORD enemyinCH = myCS.returnPlayerInCrosshair(myCS.crosshairID(LocalPlayer));
		D3DXVECTOR3 localViewAngle = myCS.getLocalViewAngles();
		D3DXVECTOR3 localEyePos = myCS.getLocalEyePos();
		D3DXVECTOR3 m_PunchAngle = myCS.getLocalPunchAngles();

		int LocalTeam = myCS.returnTeam(myCS.returnLocalPlayer());
		int EnemyTeam = myCS.returnTeam(enemyinCH);
		bool entityValid = myCS.validateEntity(enemyinCH);
		bool entityDormant = myCS.playerDormant(enemyinCH);
		int entityHP = myCS.playerHP(enemyinCH);

		float YawRecoilReductionFactor = Math::D3DXVECTOR::GetRandomFloat(myCS.MinRandomYaw, myCS.MaxRandomYaw);
		float PitchRecoilReductionFactor = Math::D3DXVECTOR::GetRandomFloat(myCS.MinRandomPitch, myCS.MaxRandomPitch);

		//Idea: wenn crosshair vom aktuellen gegner abweicht, korrigiere und setze auf alten gegner. wenn winkel / fov zu groß von viewangle is, dann resette target

		D3DXVECTOR3 targetBone = myCS.getBone(myCS.boneToAim, enemyinCH);
		D3DXVECTOR3 perfectAim;

		float minDist = Math::D3DXVECTOR::magnitude(targetBone - localEyePos);

		if (GetAsyncKeyState(myCS.aimkey) != 0 &&
			GetForegroundWindow() == myCS.CSGOwindow &&
			myCS.aimbot == 1 && entityValid &&
			LocalTeam != EnemyTeam &&
			myCS.inAir(enemyinCH) == false &&
			myCS.inAir(LocalPlayer) == false &&
			minDist > myCS.minimalDist
			)
		{
			tempEnemy = enemyinCH;
			D3DXVECTOR3 CompEnemyPos = Math::D3DXVECTOR::VelocityComp(targetBone, myCS.getVecVelocity(enemyinCH), myCS.getVecVelocity(LocalPlayer), Math::D3DXVECTOR::get3ddist(myCS.getVecOrigin(LocalPlayer), targetBone));
			perfectAim = Math::D3DXVECTOR::ClampAngle(Math::D3DXVECTOR::CalcAngle(myCS.getVecOrigin(LocalPlayer), CompEnemyPos, myCS.getaimPunchAngle(LocalPlayer), myCS.getVecViewOffset(LocalPlayer), YawRecoilReductionFactor, PitchRecoilReductionFactor));
			if (myCS.rcs == 1)
			{
				perfectAim.x = perfectAim.x - m_PunchAngle.x;
				perfectAim.y = perfectAim.y - m_PunchAngle.y;
			}
			perfectAim = Math::D3DXVECTOR::ClampAngle(Math::D3DXVECTOR::SmoothAngle(localViewAngle, perfectAim, myCS.SmoothPercent));
			shotsFired = myCS.shotsFired();

			myCS.setAngle(perfectAim);
			Sleep(1);
			aimbotTimer += 1.f;
		}
		//Debug Info
		else if (GetAsyncKeyState(myCS.aimkey) != 0 && GetForegroundWindow() == myCS.CSGOwindow)
		{
			/*
			system("cls");
			std::cout << std::endl << "NEW INFO" << std::endl;
			//std::cout << GetForegroundWindow() << " " << myCS.CSGOwindow << std::endl;	//FUNKT
			//std::cout << "Spieler in Luft: " << myCS.inAir(enemyinCH) << std::endl;		 //FUNKT
			std::cout << "EnemyTeam: " << EnemyTeam << std::endl;
			std::cout << "Localteam: " << LocalTeam << std::endl;
			std::cout << "MinimalDistance: " << myCS.minimalDist << std::endl;
			std::cout << "Local in Air: " << myCS.inAir(enemyinCH) << std::endl;
			std::cout << "Entity in Air: " << myCS.inAir(LocalPlayer) << std::endl;


			if (entityValid)
			{
			std::cout << "Entity valid!" << std::endl;
			}
			else
			{
			std::cout << "Entity not valid!" << std::endl;
			std::cout << "Player Dormant: " << entityDormant << std::endl;
			std::cout << "Player HP: " << entityHP << std::endl;
			std::cout << "Localplayer: " << myCS.returnLocalPlayer() << std::endl;
			
			}
			*/
			aimbotTimer += 0.f;
		}
	}
}

int trgbot()
{
	while (true)
	{
		DWORD LocalPlayer = myCS.returnLocalPlayer();
		DWORD EnemyInCH = myCS.returnPlayerInCrosshair(myCS.crosshairID(LocalPlayer));

		int LocalTeam = myCS.returnTeam(myCS.returnLocalPlayer());
		int EnemyTeam = myCS.returnTeam(EnemyInCH);
		int CrossHairID = myCS.crosshairID(myCS.returnLocalPlayer());
		int EnemyHealth = myCS.playerHP(EnemyInCH);

		//Debugging Info
		/*
		system("cls");
		std::cout << "Localteam:\t" << LocalTeam << std::endl;
		std::cout << "Enemyteam:\t" << EnemyTeam << std::endl;
		std::cout << "Enemy HP:\t" << EnemyHealth << std::endl;
		*/

		if (GetAsyncKeyState(myCS.triggerkey) != 0 && GetForegroundWindow() == myCS.CSGOwindow  && myCS.triggerbot == 1)
		{
			if (LocalTeam != EnemyTeam && EnemyHealth > 0)
			{
				Sleep(myCS.triggerdelay);
				mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
				Sleep(10);
				mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);

			}
		}
		Sleep(5);
	}
	return 0;
}

void bunhop()
{
	while (1)
	{
		int jumpkey = GetAsyncKeyState(myCS.bhopkey) & 0x8000;
		int flags = myCS.getFlags() & 0x1;

		if (jumpkey && flags & (1 << 0) == 1 && GetForegroundWindow() == myCS.CSGOwindow && myCS.bunnyhop == 1)
		{
			//Alternative: writes to memory!!
			//myCS.CSGOMemory.write2<int>(myCS.ClientDLL + myCS.dwForceJump, 6);

			SendMessageA(myCS.CSGOwindow, WM_KEYDOWN, VK_SPACE, 0x390000);
			Sleep(1);
			SendMessageA(myCS.CSGOwindow, WM_KEYUP, VK_SPACE, 0x390000);
			Sleep(40);
		}
		Sleep(1);
	}
}

void checkDate(int checkDay)
{
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	if (now->tm_mday != checkDay) exit(1);
}

void listFeatures()
{
	std::cout << std::endl << "Features: " << std::endl;
	if (myCS.triggerbot == 1) std::cout << "TriggerBot \t active" << std::endl; else std::cout << "TriggerBot \t not active" << std::endl;
	if (myCS.bunnyhop == 1) std::cout << "Bunnyhop \t active" << std::endl; else std::cout << "Bunnyhop \t not active" << std::endl;
	if (myCS.aimbot == 1) std::cout << "AimAssist \t active" << std::endl; else std::cout << "AimAssist \t not active" << std::endl;
	if (myCS.rcs == 1) std::cout << "Anti Recoil \t active" << std::endl; else std::cout << "Anti Recoil \t not active" << std::endl;

}

void check()
{
	int keystate;
	while (1)
	{
		if (GetAsyncKeyState(myCS.togglekey) & 0x8000 != 0)
		{
			cheatEnabled = !cheatEnabled;
			if (cheatEnabled) std::cout << "\a";
			myCS.triggerbot = !myCS.triggerbot;
			myCS.aimbot = !myCS.aimbot;
			myCS.bunnyhop = !myCS.bunnyhop;
			myCS.rcs != myCS.rcs;
			//system("cls");
			//listFeatures();
		}
		Sleep(200);
	}
}

int main()
{
	//checkDate(25);
	myCS.init();
	listFeatures();

	std::thread t1(trgbot);
	std::thread t2(bunhop);
	std::thread t3(check);
	std::thread t4(scctaimRCS);
	std::thread t5(rcs);


	t1.join();
	t2.join();
	t3.join();
	t4.join();
	//t5.join();
	std::cin.get();
	return 0;
}
