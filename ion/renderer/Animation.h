///////////////////////////////////////////////////
// File:		Animation.h
// Date:		7th April 2013
// Authors:		Matt Phillips
// Description:	Animation, tracks and keyframes
///////////////////////////////////////////////////

#pragma once

#include "maths/matrix.h"
#include "io/Archive.h"

#include <vector>

namespace ion
{
	namespace render
	{
		class Animation
		{
		public:
			enum AnimationState { Stopped, Playing };
			enum PlaybackBehaviour { PlayOnce, Loop };
			enum PlaybackDirection { Forwards, Backwards };

			Animation();
			virtual ~Animation();

			//Step animation
			void Update(float deltaTime);

			//Set/get length
			void SetLength(float length);
			float GetLength() const;

			//Set current frame
			void SetFrame(float frame);

			//Get current/prev frame
			float GetFrame() const;
			float GetPreviousFrame() const;

			//Reset animation to start/end
			void SetStart();
			void SetEnd();

			//Set/get animation state
			void SetState(AnimationState state);
			AnimationState GetState() const;

			//Set/get playback speed
			void SetPlaybackSpeed(float speed);
			float GetPlaybackSpeed() const;

			//Set/get playback behaviour
			void SetPlaybackBehaviour(PlaybackBehaviour behaviour);
			PlaybackBehaviour GetPlaybackBehaviour() const;

			//Set/get playback direction
			void SetPlaybackDirection(PlaybackDirection direction);
			PlaybackDirection GetPlaybackDirection() const;

			//Serialise
			void Serialise(io::Archive& archive);

		protected:
			//Apply new animation frame
			virtual void ApplyFrame(float frame) {}

		private:
			float mLength;
			float mCurrentFrame;
			float mPreviousFrame;
			float mPlaybackSpeed;

			AnimationState mState;
			PlaybackBehaviour mPlaybackBehaviour;
			PlaybackDirection mPlaybackDirection;
		};

		template <class T> class Keyframe
		{
		public:
			Keyframe();
			Keyframe(float time, const T& value);

			float GetTime() const;
			const T GetValue() const;

			//Serialise
			void Serialise(io::Archive& archive);

		private:
			float mTime;
			T mValue;
		};

		template <class T> class AnimationTrack
		{
		public:

			enum BlendMode { Snap, Linear };

			AnimationTrack();
			virtual ~AnimationTrack();

			//Add new keyframe
			void AddKeyframe(const Keyframe<T>& keyframe);

			//Get keyframe
			const Keyframe<T>& GetKeyframe(int index) const;

			//Get num keyframes
			int GetNumKeyframes() const;

			//Get blended value at time
			virtual const T GetValue(float time) const = 0;

			//Get time of last keyframe
			float GetLength() const;

			//Get nearest keyframes to time
			const Keyframe<T>* GetPrevKeyframe(float time) const;
			const Keyframe<T>* GetNextKeyframe(float time) const;

			//Set/get blend mode
			void SetBlendMode(BlendMode blendMode);
			BlendMode GetBlendMode() const { return mBlendMode };

			//Serialise
			void Serialise(io::Archive& archive);

		private:
			std::vector< Keyframe<T> > mKeyframes;
			BlendMode mBlendMode;
		};

		class AnimationTrackFloat : public AnimationTrack<float>
		{
		public:
			virtual const float GetValue(float time) const;
		};

		class AnimationTrackInt : public AnimationTrack<int>
		{
		public:
			virtual const int GetValue(float time) const;
		};

		class AnimationTrackTransform : public AnimationTrack<Matrix4>
		{
		public:
			virtual const Matrix4 GetValue(float time) const;
		};

		///////////////////////////////////////////////////
		//Inline functions
		///////////////////////////////////////////////////

		template <class T> Keyframe<T>::Keyframe()
		{
		}

		template <class T> Keyframe<T>::Keyframe(float time, const T& value)
		{
			mTime = time;
			mValue = value;
		}

		template <class T> float Keyframe<T>::GetTime() const
		{
			return mTime;
		}

		template <class T> const T Keyframe<T>::GetValue() const
		{
			return mValue;
		}

		template <class T> void Keyframe<T>::Serialise(io::Archive& archive)
		{
			archive.Serialise(mTime, "time");
			archive.Serialise(mValue, "value");
		}

		template <class T> AnimationTrack<T>::AnimationTrack()
		{
			mBlendMode = Linear;
		}

		template <class T> AnimationTrack<T>::~AnimationTrack()
		{
		}

		template <class T> void AnimationTrack<T>::AddKeyframe(const Keyframe<T>& keyframe)
		{
			mKeyframes.push_back(keyframe);
		}

		template <class T> const Keyframe<T>& AnimationTrack<T>::GetKeyframe(int index) const
		{
			debug::Assert(index < GetNumKeyframes(), "AnimationTrack::GetKeyframe() - Index out of range");
			return mKeyframes[index];
		}

		template <class T> int AnimationTrack<T>::GetNumKeyframes() const
		{
			return mKeyframes.size();
		}

		template <class T> float AnimationTrack<T>::GetLength() const
		{
			float lastTime = 0.0f;

			if(mKeyframes.size() > 0)
			{
				lastTime = mKeyframes[mKeyframes.size() - 1].GetTime();
			}

			return lastTime;
		}

		template <class T> const Keyframe<T>* AnimationTrack<T>::GetPrevKeyframe(float time) const
		{
			const Keyframe<T>* idealKeyframe = NULL;

			if(mKeyframes.size() > 0)
			{
				idealKeyframe = &mKeyframes[0];

				if(mKeyframes.size() > 1)
				{
					idealKeyframe = NULL;

					const Keyframe<T>* prevKeyframe = &mKeyframes[0];

					for(unsigned int i = 1; i < mKeyframes.size() && !idealKeyframe; i++)
					{
						if(time >= prevKeyframe->GetTime() && time <= mKeyframes[i].GetTime())
						{
							idealKeyframe = prevKeyframe;
						}

						prevKeyframe = &mKeyframes[i];
					}
				}
			}

			return idealKeyframe;
		}

		template <class T> const Keyframe<T>* AnimationTrack<T>::GetNextKeyframe(float time) const
		{
			const Keyframe<T>* idealKeyframe = NULL;

			if(mKeyframes.size() > 0)
			{
				idealKeyframe = &mKeyframes[0];

				if(mKeyframes.size() > 1)
				{
					idealKeyframe = NULL;
					const Keyframe<T>* prevKeyframe = &mKeyframes[0];

					for(unsigned int i = 1; i < mKeyframes.size() && !idealKeyframe; i++)
					{
						if(time >= prevKeyframe->GetTime() && time <= mKeyframes[i].GetTime())
						{
							idealKeyframe = &mKeyframes[i];
						}

						prevKeyframe = &mKeyframes[i];
					}
				}
			}

			return idealKeyframe;
		}

		template <class T> void AnimationTrack<T>::SetBlendMode(BlendMode blendMode)
		{
			mBlendMode = blendMode;
		}

		template <class T> void AnimationTrack<T>::Serialise(io::Archive& archive)
		{
			archive.Serialise(mKeyframes, "keyframes");
			archive.Serialise((int&)mBlendMode, "blendMode");
		}
	}
}