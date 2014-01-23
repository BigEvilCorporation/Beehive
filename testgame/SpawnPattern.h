#pragma once

#include <ion/maths/Vector.h>
#include <ion/maths/Matrix.h>

class SpawnPattern
{
public:
	SpawnPattern(float sceneCylinderRadius, float sceneCylinderHeight);
	virtual ion::Matrix4 GetSpawnTransform(float time, const ion::Vector2& offset) const = 0;

protected:
	float mSceneCylinderRadius;
	float mSceneCylinderHeight;
};

class SpawnLine : public SpawnPattern
{
public:
	SpawnLine(float sceneCylinderRadius, float sceneCylinderHeight, const ion::Vector2& line);
	virtual ion::Matrix4 GetSpawnTransform(float time, const ion::Vector2& offset) const;

protected:
	ion::Vector2 mLine;
};

/*
class SpawnCircle : public Spawnpattern
{
public:
	virtual ion::Matrix4 GetSpawnTransform(float time, const Vector2& offset) const;
};

class SpawnSquare : public SpawnPattern
{
public:
	virtual ion::Matrix4 GetSpawnTransform(float time, const Vector2& offset) const;
};

class SpawnSpline : public SpawnPattern
{
public:
	virtual ion::Matrix4 GetSpawnTransform(float time, const Vector2& offset) const;
};
*/