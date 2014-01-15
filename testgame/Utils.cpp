#include "Utils.h"

namespace utils
{
	ion::Matrix4 CalculateCylinderTransform(float rotationY, float positionY, float distanceFromCentre)
	{
		//Rotate around Y
		ion::Quaternion quat;
		quat.FromAxis(rotationY, ion::Vector3(0.0f, 1.0f, 0.0f));
		ion::Matrix4 matrix = quat.ToMatrix();

		//Move to Y and Z
		matrix.SetTranslation(matrix.RotateVector(ion::Vector3(0.0f, positionY, distanceFromCentre)));

		return matrix;
	}
}

namespace temp
{
	ion::io::ResourceHandle<ion::render::Material> Materials::sDefault;

	void Materials::LoadTempMaterials(ion::io::ResourceManager& resourceManager)
	{
		sDefault = resourceManager.GetResource<ion::render::Material>("ship.ion.material");
	}
}