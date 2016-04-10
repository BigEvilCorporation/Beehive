///////////////////////////////////////////////////
// File:		Bezier.h
// Date:		2nd April 2016
// Authors:		Matt Phillips
// Description:	Bezier curves
///////////////////////////////////////////////////

#pragma once

#include <vector>
#include <maths/Vector.h>
#include <io/Archive.h>

namespace ion
{
	namespace gamekit
	{
		class BezierPath
		{
		public:
			int AddPoint(const Vector2& position, const Vector2& controlA, const Vector2& controlB);
			void RemovePoint(int index);
			void SetPoint(int index, const Vector2& position, const Vector2& controlA, const Vector2& controlB);
			void GetPoint(int index, Vector2& position, Vector2& controlA, Vector2& controlB) const;
			int GetNumPoints() const;
			int GetNumCurves() const;
			void GetBounds(Vector2& boundsMin, Vector2& boundsMax) const;

			Vector2 GetPosition(float time) const;
			int GetPositions(std::vector<Vector2>& positions, float startTime, float endTime, int numPositions) const;

			void Serialise(io::Archive& archive);

		private:
			Vector2 CalculatePosition(const Vector2 controlPoints[4], float time) const;
			void CalculateBounds();
			std::vector<Vector2> m_controlPoints;
			Vector2 m_boundsMin;
			Vector2 m_boundsMax;
		};
	}
}