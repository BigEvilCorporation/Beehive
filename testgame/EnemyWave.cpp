#include "EnemyWave.h"

EnemyWave::EnemyWave(const WaveParams& waveParams, int difficulty, float sceneRadius, float sceneHeight)
{
	mWaveParams = waveParams;
	mDifficulty = difficulty;

	float difficultyScale = ((1.0f / (float)WaveParams::maxDifficulty) * (float)difficulty);
	int numUnits = ion::maths::Round((float)(waveParams.maxNumUnits - waveParams.minNumUnits) * difficultyScale) + waveParams.minNumUnits;

	SpawnPattern* spawnPattern = NULL;

	switch(waveParams.spawnPattern)
	{
		case WaveParams::Line:
		{
			spawnPattern = new SpawnLine(sceneRadius, sceneHeight, waveParams.spawnLine);
			break;
		}

		default:
			break;
	}

	for(int i = 0; i < numUnits; i++)
	{
		float time = (1.0f / (float)numUnits) * (float)i;
		ion::Matrix4 spawnTransform = spawnPattern->GetSpawnTransform(time, ion::Vector2(0.0f, 0.0f));
		Enemy* enemy = new Enemy(sceneRadius, sceneHeight);
		enemy->SetTransform(spawnTransform);
		enemy->SetMaterial(temp::Materials::sDefault.Get());
		mEnemies.push_back(enemy);
	}
}

EnemyWave::~EnemyWave()
{
}

bool EnemyWave::Update(float deltaTime, const Ship& player)
{
	for(unsigned int i = 0; i < mEnemies.size(); i++)
	{
		mEnemies[i]->Update(deltaTime, player);
	}

	return mEnemies.size() > 0;
}

void EnemyWave::Render(ion::render::Renderer& renderer, ion::render::Camera& camera)
{
	for(unsigned int i = 0; i < mEnemies.size(); i++)
	{
		mEnemies[i]->Render(renderer, camera);
	}
}