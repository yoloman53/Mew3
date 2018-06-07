#pragma once
#include <cmath>
#include <D3dx9math.h>
#include <d3dx9.h>
#include <random>
#include "CSGO.h"
#include <math.h>

#define M_PI		3.1415926535897f
#define M_RADPI		57.295779513082f

namespace Math
{
	struct D3DXVECTOR
	{	
		static float magnitude(D3DXVECTOR3 myvec)
		{
			return sqrt(pow(myvec.x, 2) + pow(myvec.y, 2) + pow(myvec.z, 2));
		}

		static float GetRandomFloat(float min, float max)
		{
			std::random_device Random;
			std::mt19937 RandomGenerator(Random());
			std::uniform_real<float> RandomDistribute(min, max);
			return RandomDistribute(RandomGenerator);
		}

		static D3DXVECTOR3 Normalize(D3DXVECTOR3 vector)
		{
			float length = magnitude(vector);
			if (length != 0)
			{
				vector.x = vector.x / length;
				vector.y = vector.y / length;
				vector.z = vector.z / length;
			}
			return vector;
		}

		static float get3ddist(D3DXVECTOR3 myCoords, D3DXVECTOR3 enemyCoords)
		{
			return sqrt(
				powf(enemyCoords.x - myCoords.x, 2.0) +
				powf(enemyCoords.y - myCoords.y, 2.0) +
				powf(enemyCoords.z - myCoords.z, 2.0));
		}

		static D3DXVECTOR3 ClampAngle(D3DXVECTOR3 &ViewAngle)
		{
			if (ViewAngle.x > 89.0f && ViewAngle.x <= 180.0f)
			{
				ViewAngle.x = 89.0f;
			}
			if (ViewAngle.x > 180.f)
			{
				ViewAngle.x -= 360.f;
			}
			if (ViewAngle.x < -89.0f)
			{
				ViewAngle.x = -89.0f;
			}
			if (ViewAngle.y > 180.f)
			{
				ViewAngle.y -= 360.f;
			}
			if (ViewAngle.y < -180.f)
			{
				ViewAngle.y += 360.f;
			}
			if (ViewAngle.z != 0.0f)
			{
				ViewAngle.z = 0.0f;
			}
			return ViewAngle;
		}

		static D3DXVECTOR3 VelocityComp(D3DXVECTOR3 EnemyPosition, D3DXVECTOR3 EnemyVelocity, D3DXVECTOR3 PlayerVelocity, float Distance)
		{
			EnemyPosition += EnemyVelocity / Distance;
			EnemyPosition -= PlayerVelocity / Distance;
			return EnemyPosition;
		}

		static D3DXVECTOR3 Curve(D3DXVECTOR3 src, D3DXVECTOR3 dst, float aimbotTimer) { //My View Angles, Aim Angles
			D3DXVECTOR3 delta = src - dst;
			ClampAngle(delta);

			float randValPt1 = 30.0f + GetRandomFloat(0.0f, 15.0f);
			float finalRandValPt1 = 2.f / randValPt1; // between 0.333 and 0.666 1.f
			D3DXVECTOR3 point1 = src + (delta * finalRandValPt1);
			ClampAngle(point1);

			float randValPt2 = 50.0f + GetRandomFloat(0.0f, 15.0f);
			float finalRandValPt2 = .2f / randValPt2; // between 0.018 and 0.025 .4
			D3DXVECTOR3 point2 = dst * (1.0f + finalRandValPt2);
			ClampAngle(point2);

			D3DXVECTOR3 CurvedAngle = CubicInterpolate(src, point1, point2, dst, aimbotTimer); // Bug here, at certain viewangles the cursor snaps in a random direction.
																							   //std::cout << CurvedAngle.y << "\n";
			return CurvedAngle;
		}

		// Credits: dude719
		// Performs a cubic bezier interpolation between four control points,
		// returning the value at the specified time (t ranges 0 to 1).
		// This template implementation can be used to interpolate XMVECTOR,
		// Vector, float, or any other types that define suitable * and + operators.
		template<typename T>
		static T CubicInterpolate(T const& p1, T const& p2, T const& p3, T const& p4, float t)
		{
			return p1 * (1 - t) * (1 - t) * (1 - t) +
				p2 * 3 * t * (1 - t) * (1 - t) +
				p3 * 3 * t * t * (1 - t) +
				p4 * t * t * t;
		}

		static D3DXVECTOR3 CalcAngle(D3DXVECTOR3 PlayerPosition, D3DXVECTOR3 EnemyPosition, D3DXVECTOR3 PunchAngle, D3DXVECTOR3 ViewOffset, float YawRecoilReductionFactor, float PitchRecoilReductionFactor)
		{
			D3DXVECTOR3 AimAngle;
			D3DXVECTOR3 Delta = { (PlayerPosition.x - EnemyPosition.x), (PlayerPosition.y - EnemyPosition.y), ((PlayerPosition.z + ViewOffset.z) - EnemyPosition.z) };
			float hyp = sqrt(Delta.x * Delta.x + Delta.y * Delta.y);
			AimAngle.x = atanf(Delta.z / hyp) * M_RADPI - PunchAngle.x * YawRecoilReductionFactor;
			AimAngle.y = atanf(Delta.y / Delta.x) * M_RADPI - PunchAngle.y * PitchRecoilReductionFactor;
			AimAngle.z = 0.0f;
			if (Delta.x >= 0.0)
			{
				AimAngle.y += 180.0f;
			}

			return AimAngle;
		}

		static D3DXVECTOR3 dVelocityComp(D3DXVECTOR3 EnemyPos, D3DXVECTOR3 EnemyVecVelocity, D3DXVECTOR3 PlayerVecVelocity, float dist)
		{
			EnemyPos.x += (EnemyVecVelocity.x) / dist;
			EnemyPos.x -= (PlayerVecVelocity.x) / dist;
			EnemyPos.y += (EnemyVecVelocity.y) / dist;
			EnemyPos.y -= (PlayerVecVelocity.y) / dist;
			EnemyPos.z += (EnemyVecVelocity.z) / dist;
			EnemyPos.z -= (PlayerVecVelocity.z) / dist;
			return EnemyPos;
		}

		static D3DXVECTOR3 SmoothAngle(D3DXVECTOR3 SrcAngles, D3DXVECTOR3 DestAngles, float SmoothAmount)
		{
			D3DXVECTOR3 SmoothedAngles;
			SmoothedAngles.x = DestAngles.x - SrcAngles.x;
			SmoothedAngles.y = DestAngles.y - SrcAngles.y;
			SmoothedAngles.z = 0.0f;
			SmoothedAngles = ClampAngle(SmoothedAngles);
			SmoothedAngles.x = SrcAngles.x + SmoothedAngles.x / 100.0f * SmoothAmount;
			SmoothedAngles.y = SrcAngles.y + SmoothedAngles.y / 100.0f * SmoothAmount;
			SmoothedAngles.z = 0.0f;
			SmoothedAngles = ClampAngle(SmoothedAngles);
			return SmoothedAngles;
		}
	};
}
