#pragma once

#include "Enemy.h"
#include "SpawnPattern.h"
#include <vector>

struct WaveParams
{
	//TODO: Serialise from XML

	enum EnemyType { Default };
	enum SpawnPattern { Line, Circle, Box };
	enum MovePattern { Static, JitterX, JitterY, CircleSpot, CircleScene, Follow };

	static const int maxDifficulty = 10;

	EnemyType enemytype;
	SpawnPattern spawnPattern;
	MovePattern movePattern;

	int minNumUnits;
	int maxNumUnits;

	int minHealth;
	int maxHealth;

	float minMoveSpeed;
	float maxMoveSpeed;

	ion::Vector2 spawnLine;
	ion::Vector2 spawnBox;
	float spawnCircleRadius;

	float moveJitterAmplitude;
	float moveCircleRadius;
	float moveFollowDistance;
};

class EnemyWave
{
public:
	EnemyWave(const WaveParams& waveParams, int difficulty, float sceneRadius, float sceneHeight);
	~EnemyWave();

	virtual bool Update(float deltaTime, const Ship& player);
	virtual void Render(ion::render::Renderer& renderer, ion::render::Camera& camera);

protected:
	WaveParams mWaveParams;
	int mDifficulty;

	std::vector<Enemy*> mEnemies;
};