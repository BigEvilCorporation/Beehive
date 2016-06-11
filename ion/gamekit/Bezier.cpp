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
		int BezierPath::AddPoint(const Vector2& position, const Vector2& controlA, const Vector2& controlB)
		{
			m_controlPoints.push_back(position + controlA);
			m_controlPoints.push_back(position);
			m_controlPoints.push_back(position + controlB);
			CalculateBounds();
			return m_controlPoints.size() / 3;
		}

		void BezierPath::RemovePoint(int index)
		{
			ion::debug::Assert(index < GetNumPoints(), "eOut of range");
			m_controlPoints.erase(m_controlPoints.begin() + (index * 3), m_controlPoints.begin() + (index * 3) + 3);
			CalculateBounds();
		}

		void BezierPath::SetPoint(int index, const Vector2& position, const Vector2& controlA, const Vector2& controlB)
		{
			ion::debug::Assert(index < GetNumPoints(), "eOut of range");
			m_controlPoints[(index * 3) + 0] = position + controlA;
			m_controlPoints[(index * 3) + 1] = position;
			m_controlPoints[(index * 3) + 2] = position + controlB;
			CalculateBounds();
		}

		void BezierPath::GetPoint(int index, Vector2& position, Vector2& controlA, Vector2& controlB) const
		{
			ion::debug::Assert(index < GetNumPoints(), "eOut of range");
			controlA = m_controlPoints[(index * 3) + 0];
			position = m_controlPoints[(index * 3) + 1];
			controlB = m_controlPoints[(index * 3) + 2];
			controlA -= position;
			controlB -= position;
		}

		int BezierPath::GetNumCurves() const
		{
			int numControlPoints = m_controlPoints.size();
			return (numControlPoints > 3) ? ((numControlPoints - 3) / 3) : 0;
		}

		int BezierPath::GetNumPoints() const
		{
			return m_controlPoints.size() / 3;
		}

		void BezierPath::GetBounds(Vector2& boundsMin, Vector2& boundsMax) const
		{
			boundsMin = m_boundsMin;
			boundsMax = m_boundsMax;
		}

		Vector2 BezierPath::GetPosition(float time) const
		{
			if(m_controlPoints.size() == 0)
			{
				return Vector2(0.0f, 0.0f);
			}
			else if(m_controlPoints.size() < 6)
			{
				return m_controlPoints[0];
			}
			else
			{
				ion::debug::Assert(time >= 0.0f && time <= 1.0f, "eOut of range");

				//Get curve index (points in groups of 3, skipping first)
				int curveIndex = maths::Floor(((float)(m_controlPoints.size() - 3) / 3.0f) * time);

				//Skip first point control A (it's meanlingless)
				int pointIndex = (curveIndex * 3) + 1;

				//Calculate time within current curve
				float totalTime = (m_controlPoints.size() - 3) / 3.0f;
				float segmentTime = maths::Fmod(time * totalTime, 1.0f);

				//Prev point pos/controlB + next point controlA/pos
				Vector2 controlPoints[4] = { m_controlPoints[pointIndex], m_controlPoints[pointIndex + 1], m_controlPoints[pointIndex + 2], m_controlPoints[pointIndex + 3] };
				return CalculatePosition(controlPoints, segmentTime);
			}
		}

		int BezierPath::GetPositions(std::vector<Vector2>& positions, float startTime, float endTime, int numPositions) const
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
				for(int i = 0; i < numPositions; i++)
				{
					float time = startTime + ((endTime - startTime) * ((float)i / (float)numPositions));
					positions.push_back(GetPosition(time));
				}
			}
			
			return positions.size();
		}

		Vector2 BezierPath::CalculatePosition(const Vector2 controlPoints[4], float time) const
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

		void BezierPath::CalculateBounds()
		{
			m_boundsMin.x = maths::FLOAT_MAX;
			m_boundsMin.y = maths::FLOAT_MAX;
			m_boundsMax.x = maths::FLOAT_MIN;
			m_boundsMax.y = maths::FLOAT_MIN;

			for(int i = 0; i < GetNumPoints(); i++)
			{
				const Vector2& point = m_controlPoints[(i * 3) + 1];

				if(point.x > m_boundsMax.x)
					m_boundsMax.x = point.x;
				if(point.x < m_boundsMin.x)
					m_boundsMin.x = point.x;
				if(point.y > m_boundsMax.y)
					m_boundsMax.y = point.y;
				if(point.y < m_boundsMin.y)
					m_boundsMin.y = point.y;
			}
		}

		void BezierPath::Serialise(io::Archive& archive)
		{
			archive.Serialise(m_controlPoints, "controlPoints");
			archive.Serialise(m_boundsMin, "boundsMin");
			archive.Serialise(m_boundsMax, "boundsMax");
		}
	}
}