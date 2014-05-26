///////////////////////////////////////////////////
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
			mLength = 1.0f;
			mCurrentFrame = 0.0f;
			mPreviousFrame = 0.0f;
			mPlaybackSpeed = 1.0f;

			mState = Stopped;
			mPlaybackBehaviour = Loop;
			mPlaybackDirection = Forwards;
		}

		Animation::~Animation()
		{
		}

		void Animation::Update(float deltaTime)
		{
			if(mState == Playing)
			{
				//Cache previous frame
				mPreviousFrame = mCurrentFrame;

				if(mPlaybackDirection == Forwards)
				{
					//Advance animation forwards
					mCurrentFrame += deltaTime * mPlaybackSpeed;

					//If gone past the length
					if(mCurrentFrame > mLength)
					{
						if(mPlaybackBehaviour == Loop)
						{
							//Apply the final frame then loop
							ApplyFrame(mLength);

							//Loop round
							mCurrentFrame = maths::Fmod(mCurrentFrame, mLength);
						}
						else
						{
							//Clamp to end
							mCurrentFrame = mLength;

							//Finished
							SetState(Stopped);
						}
					}
				}
				else
				{
					//Advance animation backwards
					mCurrentFrame -= deltaTime * mPlaybackSpeed;

					//If gone past the start
					if(mCurrentFrame < 0.0f)
					{
						if(mPlaybackBehaviour == Loop)
						{
							//Apply the first frame then loop
							ApplyFrame(0.0f);

							//Loop round
							mCurrentFrame = mLength - maths::Fmod(mLength - mCurrentFrame, mLength);
						}
						else
						{
							//Clamp to start
							mCurrentFrame = 0.0f;

							//Finished
							SetState(Stopped);
						}
					}
				}

				//Apply new frame
				ApplyFrame(mCurrentFrame);
			}
		}

		void Animation::SetLength(float length)
		{
			mLength = length;
		}

		float Animation::GetLength() const
		{
			return mLength;
		}

		void Animation::SetFrame(float frame)
		{
			mPreviousFrame = mCurrentFrame;
			mCurrentFrame = frame;
		}

		float Animation::GetFrame() const
		{
			return mCurrentFrame;
		}

		float Animation::GetPreviousFrame() const
		{
			return mPreviousFrame;
		}

		void Animation::SetStart()
		{
			mCurrentFrame = 0.0f;
			mPreviousFrame = 0.0f;
			ApplyFrame(mCurrentFrame);
		}

		void Animation::SetEnd()
		{
			mCurrentFrame = mLength;
			mPreviousFrame = mLength;
			ApplyFrame(mCurrentFrame);
		}

		void Animation::SetState(AnimationState state)
		{
			mState = state;
		}

		Animation::AnimationState Animation::GetState() const
		{
			return mState;
		}

		void Animation::SetPlaybackSpeed(float speed)
		{
			mPlaybackSpeed = speed;
		}

		float Animation::GetPlaybackSpeed() const
		{
			return mPlaybackSpeed;
		}

		void Animation::SetPlaybackBehaviour(PlaybackBehaviour behaviour)
		{
			mPlaybackBehaviour = behaviour;
		}

		Animation::PlaybackBehaviour Animation::GetPlaybackBehaviour() const
		{
			return mPlaybackBehaviour;
		}

		void Animation::SetPlaybackDirection(PlaybackDirection direction)
		{
			mPlaybackDirection = direction;
		}

		Animation::PlaybackDirection Animation::GetPlaybackDirection() const
		{
			return mPlaybackDirection;
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

				if((timeB - timeA) >= 0.5f)
					intValue = keyframeB->GetValue();
				else
					intValue = keyframeA->GetValue();
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