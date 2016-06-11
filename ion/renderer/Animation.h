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
			enum AnimationState { eStopped, ePlaying };
			enum PlaybackBehaviour { ePlayOnce, eLoop };
			enum PlaybackDirection { eForwards, eBackwards };

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
			float m_length;
			float m_currentFrame;
			float m_previousFrame;
			float m_playbackSpeed;

			AnimationState m_state;
			PlaybackBehaviour m_playbackBehaviour;
			PlaybackDirection m_playbackDirection;
		};

		template <class T> class Keyframe
		{
		public:
			Keyframe();
			Keyframe(float time, const T& value);

			float GetTime() const;
			const T GetValue() const;

			bool operator < (const Keyframe& rhs) const;

			//Serialise
			void Serialise(io::Archive& archive);

		private:
			float m_time;
			T m_value;
		};

		template <class T> class AnimationTrack
		{
		public:

			enum BlendMode { eSnap, eLinear };

			AnimationTrack();
			virtual ~AnimationTrack();

			//Add new keyframe
			void InsertKeyframe(const Keyframe<T>& keyframe);

			//Get keyframe
			const Keyframe<T>& GetKeyframe(int index) const;

			//Get num keyframes
			int GetNumKeyframes() const;

			//Clear all keyframes
			void Clear();

			//Get blended value at time
			virtual const T GetValue(float time) const = 0;

			//Get time of last keyframe
			float GetLength() const;

			//Get nearest keyframes to time
			const Keyframe<T>* GetPrevKeyframe(float time) const;
			const Keyframe<T>* GetNextKeyframe(float time) const;

			//Set/get blend mode
			void SetBlendMode(BlendMode blendMode);
			BlendMode GetBlendMode() const { return m_blendMode };

			//Serialise
			void Serialise(io::Archive& archive);

		private:
			std::vector< Keyframe<T> > m_keyframes;
			BlendMode m_blendMode;
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
			m_time = time;
			m_value = value;
		}

		template <class T> float Keyframe<T>::GetTime() const
		{
			return m_time;
		}

		template <class T> const T Keyframe<T>::GetValue() const
		{
			return m_value;
		}

		template <class T> bool Keyframe<T>::operator < (const Keyframe& rhs) const
		{
			return m_time < rhs.m_time;
		}

		template <class T> void Keyframe<T>::Serialise(io::Archive& archive)
		{
			archive.Serialise(m_time, "time");
			archive.Serialise(m_value, "value");
		}

		template <class T> AnimationTrack<T>::AnimationTrack()
		{
			m_blendMode = eLinear;
		}

		template <class T> AnimationTrack<T>::~AnimationTrack()
		{
		}

		template <class T> void AnimationTrack<T>::InsertKeyframe(const Keyframe<T>& keyframe)
		{
			m_keyframes.insert(std::upper_bound(m_keyframes.begin(), m_keyframes.end(), keyframe), keyframe);
		}

		template <class T> const Keyframe<T>& AnimationTrack<T>::GetKeyframe(int index) const
		{
			debug::Assert(index < GetNumKeyframes(), "AnimationTrack::GetKeyframe() - Index out of range");
			return m_keyframes[index];
		}

		template <class T> int AnimationTrack<T>::GetNumKeyframes() const
		{
			return m_keyframes.size();
		}

		template <class T> void AnimationTrack<T>::Clear()
		{
			m_keyframes.clear();
		}

		template <class T> float AnimationTrack<T>::GetLength() const
		{
			float lastTime = 0.0f;

			if(m_keyframes.size() > 0)
			{
				lastTime = m_keyframes.back().GetTime();
			}

			return lastTime;
		}

		template <class T> const Keyframe<T>* AnimationTrack<T>::GetPrevKeyframe(float time) const
		{
			const Keyframe<T>* idealKeyframe = NULL;

			if(m_keyframes.size() > 0)
			{
				idealKeyframe = &m_keyframes[0];

				if(m_keyframes.size() > 1)
				{
					if(time >= GetLength())
					{
						idealKeyframe = &m_keyframes.back();
					}
					else
					{
						idealKeyframe = NULL;

						const Keyframe<T>* prevKeyframe = &m_keyframes[0];

						for(unsigned int i = 1; i < m_keyframes.size() && !idealKeyframe; i++)
						{
							if(time >= prevKeyframe->GetTime() && time <= m_keyframes[i].GetTime())
							{
								idealKeyframe = prevKeyframe;
							}

							prevKeyframe = &m_keyframes[i];
						}
					}
				}
			}

			return idealKeyframe;
		}

		template <class T> const Keyframe<T>* AnimationTrack<T>::GetNextKeyframe(float time) const
		{
			const Keyframe<T>* idealKeyframe = NULL;

			if(m_keyframes.size() > 0)
			{
				idealKeyframe = &m_keyframes[0];

				if(m_keyframes.size() > 1)
				{
					if(time >= GetLength())
					{
						idealKeyframe = &m_keyframes.back();
					}
					else
					{
						idealKeyframe = NULL;
						const Keyframe<T>* prevKeyframe = &m_keyframes[0];

						for(unsigned int i = 1; i < m_keyframes.size() && !idealKeyframe; i++)
						{
							if(time >= prevKeyframe->GetTime() && time <= m_keyframes[i].GetTime())
							{
								idealKeyframe = &m_keyframes[i];
							}

							prevKeyframe = &m_keyframes[i];
						}
					}
				}
			}

			return idealKeyframe;
		}

		template <class T> void AnimationTrack<T>::SetBlendMode(BlendMode blendMode)
		{
			m_blendMode = blendMode;
		}

		template <class T> void AnimationTrack<T>::Serialise(io::Archive& archive)
		{
			archive.Serialise(m_keyframes, "keyframes");
			archive.Serialise((int&)m_blendMode, "blendMode");
		}
	}
}