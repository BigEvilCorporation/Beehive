///////////////////////////////////////////////////
// File:		Bezier.cpp
// Date:		2nd April 2016
// Authors:		Matt Phillips
// Description:	Bezier curves
///////////////////////////////////////////////////

#include "Bezier.h"
#include "ion/core/debug/Debug.h"

namespace ion
{
	namespace gamekit
	{
		int BezierCurve::AddPoint(const Vector2& position, const Vector2& control)
		{
			if((m_controlPoints.size() / 2) & 1)
			{
				//Odd number of points, add control point first
				m_controlPoints.push_back(position + control);
				m_controlPoints.push_back(position);
			}
			else
			{
				m_controlPoints.push_back(position);
				m_controlPoints.push_back(position + control);
			}

			return m_controlPoints.size() / 2;
		}

		void BezierCurve::RemovePoint(int index)
		{
			ion::debug::Assert(index < GetNumPoints(), "Out of range");
			m_controlPoints.erase(m_controlPoints.begin() + index, m_controlPoints.begin() + index + 1);
		}

		void BezierCurve::SetPoint(int index, const Vector2& position, const Vector2& control)
		{
			ion::debug::Assert(index < GetNumPoints(), "Out of range");

			if((index / 2) & 1)
			{
				//Odd number of points, add control point first
				m_controlPoints[(index * 2) + 1] = position;
				m_controlPoints[(index * 2) + 0] = position + control;
			}
			else
			{
				m_controlPoints[(index * 2) + 0] = position;
				m_controlPoints[(index * 2) + 1] = position + control;
			}
		}

		void BezierCurve::GetPoint(int index, Vector2& position, Vector2& control) const
		{
			ion::debug::Assert(index < GetNumPoints(), "Out of range");

			if((index / 2) & 1)
			{
				//Odd number of points, add control point first
				position = m_controlPoints[(index * 2) + 1];
				control = m_controlPoints[(index * 2) + 0] - position;
			}
			else
			{
				position = m_controlPoints[(index * 2) + 0];
				control = m_controlPoints[(index * 2) + 1] - position;
			}
		}

		int BezierCurve::GetNumPoints() const
		{
			return m_controlPoints.size() / 2;
		}

		Vector2 BezierCurve::GetPosition(float time) const
		{
			if(m_controlPoints.size() == 0)
			{
				return Vector2(0.0f, 0.0f);
			}
			else if(m_controlPoints.size() < 4)
			{
				return m_controlPoints[0];
			}
			else
			{
				ion::debug::Assert(time >= 0.0f && time <= 1.0f, "Out of range");

				//Last control point of prev curve overlaps first control point of next curve
				int curveIndex = maths::Floor(((m_controlPoints.size() / 3) - 1) * time);
				int pointIndex = curveIndex * 3;
				float segmentTime = maths::Fmod(time, 1.0f / (float)m_controlPoints.size() - 3);
				Vector2 controlPoints[4] = { m_controlPoints[pointIndex], m_controlPoints[pointIndex + 1], m_controlPoints[pointIndex + 2], m_controlPoints[pointIndex + 3] };
				return CalculatePosition(controlPoints, segmentTime);
			}
		}

		int BezierCurve::GetPositions(std::vector<Vector2>& positions, float startTime, float endTime, int numPositions) const
		{
			if(m_controlPoints.size() == 0)
			{
				positions.push_back(Vector2(0.0f, 0.0f));
			}
			else if(m_controlPoints.size() < 4)
			{
				positions.push_back(m_controlPoints[0]);
			}
			else
			{
				for(int i = 0; i < numPositions + 1; i++)
				{
					float time = startTime + ((endTime - startTime) * ((float)i / (float)numPositions));
					positions.push_back(GetPosition(time));
				}
			}
			
			return positions.size();
		}

		Vector2 BezierCurve::CalculatePosition(const Vector2 controlPoints[4], float time) const
		{
			const float timeSq = time * time;
			const float timeCb = timeSq * time;
			const float timeInv = 1.0f - time;
			const float timeInvSq = timeInv * timeInv;
			const float timeInvCb = timeInvSq * timeInv;

			Vector2 position = controlPoints[0] * timeInvCb;
			position += controlPoints[1] * (3.0f * timeInvSq * time);
			position += controlPoints[2] * (3.0f * timeInv * timeSq);
			position += controlPoints[3] * timeCb;

			return position;
		}
	}
}