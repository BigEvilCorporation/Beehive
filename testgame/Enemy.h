#pragma once

#include "Ship.h"

class Enemy : public Ship
{
public:
	Enemy(float sceneCylinderRadius, float sceneCylinderHeight);
	virtual ~Enemy();

	virtual void Update(float deltaTime, const Ship& player);
};