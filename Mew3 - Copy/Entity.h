#pragma once
#include "stdafx.h"

class Entity
{ 
public:
	Entity();
	Entity(DWORD ptr)
	{
		player = ptr;
	}
	~Entity();

	DWORD player;
	DWORD playerIndex;

	int getHealth();
	int getTeam();
};

