#pragma once

#include <ion/maths/Vector.h>
#include <ion/maths/Matrix.h>
#include <ion/maths/Quaternion.h>

#include <ion/io/ResourceHandle.h>
#include <ion/io/ResourceManager.h>
#include <ion/renderer/Material.h>

namespace utils
{
	//Calculates a Y rotation around cylinder from a given X coordinate
	float XPositionToCylinderYRotation(float positionX, float distancefromCentre);

	//Calculates a pos/rot matrix to position an object around the outside of a cylinder, facing its centre
	ion::Matrix4 CalculateCylinderTransform(float rotationY, float positionY, float distanceFromCentre);
}

//Temp stuff to be revised
namespace temp
{
	class Materials
	{
	public:
		static void LoadTempMaterials(ion::io::ResourceManager& resourceManager);

		static ion::io::ResourceHandle<ion::render::Material> sDefault;
	};
}