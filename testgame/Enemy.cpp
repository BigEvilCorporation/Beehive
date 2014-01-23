#include "Enemy.h"

Enemy::Enemy(float sceneCylinderRadius, float sceneCylinderHeight)
	: Ship(sceneCylinderRadius, sceneCylinderHeight)
{
}

Enemy::~Enemy()
{
}

void Enemy::Update(float deltaTime, const Ship& player)
{
}