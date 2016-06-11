///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		Animation.cpp
// Date:		7th April 2013
// Authors:		Matt Phillips
// Description:	Animation, tracks and keyframes
///////////////////////////////////////////////////

#include "Animation.h"

namespace ion
{
	namespace render
	{
		Animation::Animation()
		{
			m_length = 1.0f;
			m_currentFrame = 0.0f;
			m_previousFrame = 0.0f;
			m_playbackSpeed = 1.0f;

			m_state = eStopped;
			m_playbackBehaviour = eLoop;
			m_playbackDirection = eForwards;
		}

		Animation::~Animation()
		{
		}

		void Animation::Update(float deltaTime)
		{
			if(m_state == ePlaying)
			{
				//Cache previous frame
				m_previousFrame = m_currentFrame;

				if(m_playbackDirection == eForwards)
				{
					//Advance animation forwards
					m_currentFrame += deltaTime * m_playbackSpeed;

					//If gone past the length
					if(m_currentFrame > m_length)
					{
						if(m_playbackBehaviour == eLoop)
						{
							//Apply the final frame then loop
							ApplyFrame(m_length);

							//Loop round
							m_currentFrame = maths::Fmod(m_currentFrame, m_length);
						}
						else
						{
							//Clamp to end
							m_currentFrame = m_length;

							//Finished
							SetState(eStopped);
						}
					}
				}
				else
				{
					//Advance animation backwards
					m_currentFrame -= deltaTime * m_playbackSpeed;

					//If gone past the start
					if(m_currentFrame < 0.0f)
					{
						if(m_playbackBehaviour == eLoop)
						{
							//Apply the first frame then loop
							ApplyFrame(0.0f);

							//Loop round
							m_currentFrame = m_length - maths::Fmod(m_length - m_currentFrame, m_length);
						}
						else
						{
							//Clamp to start
							m_currentFrame = 0.0f;

							//Finished
							SetState(eStopped);
						}
					}
				}

				//Apply new frame
				ApplyFrame(m_currentFrame);
			}
		}

		void Animation::SetLength(float length)
		{
			m_length = length;
		}

		float Animation::GetLength() const
		{
			return m_length;
		}

		void Animation::SetFrame(float frame)
		{
			m_previousFrame = m_currentFrame;
			m_currentFrame = frame;
		}

		float Animation::GetFrame() const
		{
			return m_currentFrame;
		}

		float Animation::GetPreviousFrame() const
		{
			return m_previousFrame;
		}

		void Animation::SetStart()
		{
			m_currentFrame = 0.0f;
			m_previousFrame = 0.0f;
			ApplyFrame(m_currentFrame);
		}

		void Animation::SetEnd()
		{
			m_currentFrame = m_length;
			m_previousFrame = m_length;
			ApplyFrame(m_currentFrame);
		}

		void Animation::SetState(AnimationState state)
		{
			m_state = state;
		}

		Animation::AnimationState Animation::GetState() const
		{
			return m_state;
		}

		void Animation::SetPlaybackSpeed(float speed)
		{
			m_playbackSpeed = speed;
		}

		float Animation::GetPlaybackSpeed() const
		{
			return m_playbackSpeed;
		}

		void Animation::SetPlaybackBehaviour(PlaybackBehaviour behaviour)
		{
			m_playbackBehaviour = behaviour;
		}

		Animation::PlaybackBehaviour Animation::GetPlaybackBehaviour() const
		{
			return m_playbackBehaviour;
		}

		void Animation::SetPlaybackDirection(PlaybackDirection direction)
		{
			m_playbackDirection = direction;
		}

		Animation::PlaybackDirection Animation::GetPlaybackDirection() const
		{
			return m_playbackDirection;
		}

		void Animation::Serialise(io::Archive& archive)
		{
			archive.Serialise(m_length, "length");
			archive.Serialise(m_currentFrame, "currentFrame");
			archive.Serialise(m_previousFrame, "previousFrame");
			archive.Serialise(m_playbackSpeed, "playbackSpeed");
			archive.Serialise((int&)m_state, "state");
			archive.Serialise((int&)m_playbackBehaviour, "playbackBehaviour");
			archive.Serialise((int&)m_playbackDirection, "playbackDirection");
		}

		const float AnimationTrackFloat::GetValue(float time) const
		{
			float floatValue = 0.0f;

			const Keyframe<float>* keyframeA = GetPrevKeyframe(time);
			const Keyframe<float>* keyframeB = GetNextKeyframe(time);

			if(keyframeA && keyframeB)
			{
				float timeA = keyframeA->GetTime();
				float timeB = keyframeB->GetTime();

				float lerpTime = maths::UnLerp(timeA, timeB, time);

				float valueA = keyframeA->GetValue();
				float valueB = keyframeB->GetValue();

				floatValue = valueA + (valueB - valueA) * lerpTime;
			}

			return floatValue;
		}

		const int AnimationTrackInt::GetValue(float time) const
		{
			int intValue = 0;

			const Keyframe<int>* keyframeA = GetPrevKeyframe(time);
			const Keyframe<int>* keyframeB = GetNextKeyframe(time);

 			if(keyframeA && keyframeB)
			{
				float timeA = keyframeA->GetTime();
				float timeB = keyframeB->GetTime();
				float lerpTime = maths::UnLerp(timeA, timeB, time);

				intValue = (int)maths::Round(maths::Lerp((float)keyframeA->GetValue(), (float)keyframeB->GetValue(), lerpTime));
			}

			return intValue;
		}

		const Matrix4 AnimationTrackTransform::GetValue(float time) const
		{
			Matrix4 resultMatrix;

			const Keyframe<Matrix4>* keyframeA = GetPrevKeyframe(time);
			const Keyframe<Matrix4>* keyframeB = GetNextKeyframe(time);

			if(keyframeA && keyframeB)
			{
				float timeA = keyframeA->GetTime();
				float timeB = keyframeB->GetTime();

				float lerpTime = maths::UnLerp(timeA, timeB, time);

				const Matrix4& matrixA = keyframeA->GetValue();
				const Matrix4& matrixB = keyframeB->GetValue();

				resultMatrix = matrixA.GetInterpolated(matrixB, lerpTime);
			}

			return resultMatrix;
		}
	}
}