///////////////////////////////////////////////////
// File:		Bezier.h
// Date:		2nd April 2016
// Authors:		Matt Phillips
// Description:	Bezier curves
///////////////////////////////////////////////////

#pragma once

#include <vector>
#include <maths/Vector.h>

namespace ion
{
	namespace gamekit
	{
		class BezierCurve
		{
		public:
			int AddPoint(const Vector2& position, const Vector2& controlA, const Vector2& controlB);
			void RemovePoint(int index);
			void SetPoint(int index, const Vector2& position, const Vector2& controlA, const Vector2& controlB);
			void GetPoint(int index, Vector2& position, Vector2& controlA, Vector2& controlB) const;
			int GetNumPoints() const;

			Vector2 GetPosition(float time) const;
			int GetPositions(std::vector<Vector2>& positions, float startTime, float endTime, int numPositions) const;

		private:
			Vector2 CalculatePosition(const Vector2 controlPoints[4], float time) const;
			std::vector<Vector2> m_controlPoints;
		};
	}
}