#include "SpawnPattern.h"
#include "Utils.h"

SpawnPattern::SpawnPattern(float sceneCylinderRadius, float sceneCylinderHeight)
{
	mSceneCylinderRadius = sceneCylinderRadius;
	mSceneCylinderHeight = sceneCylinderHeight;
}

SpawnLine::SpawnLine(float sceneCylinderRadius, float sceneCylinderHeight, const ion::Vector2& line)
	: SpawnPattern(sceneCylinderRadius, sceneCylinderHeight)
{
	mLine = line;
}

ion::Matrix4 SpawnLine::GetSpawnTransform(float time, const ion::Vector2& offset) const
{
	ion::Vector2 startPoint = offset;
	ion::Vector2 endPoint = startPoint + offset + mLine;
	ion::Vector2 position = startPoint.Lerp(endPoint, time);
	float rotationY = utils::XPositionToCylinderYRotation(position.x, mSceneCylinderRadius);
	return utils::CalculateCylinderTransform(rotationY, position.y, mSceneCylinderRadius);
}