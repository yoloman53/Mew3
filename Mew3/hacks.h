#pragma once
#include "math.h"
#include "CSGO.h"
#include "stdafx.h"

namespace hacks
{
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

			if (
				GetAsyncKeyState(myCS.aimkey) != 0 &&
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

				myCS.setAngle(perfectAim);
				Sleep(1);
				aimbotTimer += 1.f;
			}
			//wenn trigger aimbot aktiv ist
			else if (GetAsyncKeyState(myCS.aimkey) == 0 && GetAsyncKeyState(myCS.triggerkey) != 0 && myCS.legittrigger == 1)
			{
				tempEnemy = enemyinCH;
				D3DXVECTOR3 CompEnemyPos = Math::D3DXVECTOR::VelocityComp(targetBone, myCS.getVecVelocity(enemyinCH), myCS.getVecVelocity(LocalPlayer), Math::D3DXVECTOR::get3ddist(myCS.getVecOrigin(LocalPlayer), targetBone));
				perfectAim = Math::D3DXVECTOR::ClampAngle(Math::D3DXVECTOR::CalcAngle(myCS.getVecOrigin(LocalPlayer), CompEnemyPos, myCS.getaimPunchAngle(LocalPlayer), myCS.getVecViewOffset(LocalPlayer), YawRecoilReductionFactor, PitchRecoilReductionFactor));
				perfectAim = Math::D3DXVECTOR::ClampAngle(Math::D3DXVECTOR::SmoothAngle(localViewAngle, perfectAim, myCS.SmoothPercent));

				myCS.setAngle(perfectAim);
				Sleep(1);
				aimbotTimer += 1.f;
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


}