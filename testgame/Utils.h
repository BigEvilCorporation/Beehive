#include <ion/maths/Vector.h>
#include <ion/maths/Matrix.h>
#include <ion/maths/Quaternion.h>

namespace utils
{
	//Calculates a pos/rot matrix to position an object around the outside of a cylinder, facing its centre
	ion::Matrix4 CalculateCylinderTransform(float rotationY, float positionY, float distanceFromCentre);
}