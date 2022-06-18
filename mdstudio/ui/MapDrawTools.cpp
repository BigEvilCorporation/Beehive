#include "MapDrawTools.h"

namespace drawtools
{
	void SanitiseBoxOrder(const ion::Vector2i& boxStart, const ion::Vector2i& boxEnd, ion::Vector2i& topLeft, ion::Vector2i& bottomRight)
	{
		topLeft.x = ion::maths::Min(boxStart.x, boxEnd.x);
		topLeft.y = ion::maths::Min(boxStart.y, boxEnd.y);
		bottomRight.x = ion::maths::Max(boxStart.x, boxEnd.x);
		bottomRight.y = ion::maths::Max(boxStart.y, boxEnd.y);
	}

	ion::Matrix4 CalcBoxDrawMatrix(const ion::Vector2i& boxStart, const ion::Vector2i& boxEnd, const ion::Vector2i& mapSizePx, float z)
	{
		const float x = ion::maths::Min(boxStart.x, boxEnd.x);
		const float y = ion::maths::Min(boxStart.y, boxEnd.y);
		const float y_inv = mapSizePx.y - y;
		const float width = (float)abs(boxEnd.x - boxStart.x);
		const float height = (float)abs(boxEnd.y - boxStart.y);
		const ion::Vector2 mapCentre(mapSizePx.x / 2.0f, mapSizePx.y / 2.0f);
		const ion::Vector2 boxCentre(width / 2.0f, height / 2.0f);

		ion::Vector3 boxScale(width, height, 0.0f);
		ion::Vector3 boxPos(x - mapCentre.x + boxCentre.x, y_inv - mapCentre.y - boxCentre.y, z);

		ion::Matrix4 boxMtx;
		boxMtx.SetTranslation(boxPos);
		boxMtx.SetScale(boxScale);

		return boxMtx;
	}
}