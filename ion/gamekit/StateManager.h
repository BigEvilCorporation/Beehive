///////////////////////////////////////////////////
// File:		StateManager.h
// Date:		16th February 2014
// Authors:		Matt Phillips
// Description:	Game state manager
///////////////////////////////////////////////////

#pragma once

#include <vector>

namespace ion
{
	namespace render
	{
		class Renderer;
		class Camera;
	}

	namespace io
	{
		class ResourceManager;
	}

	namespace gamekit
	{
		class StateManager;

		class State
		{
		public:
			State(StateManager& stateManager, io::ResourceManager& resourceManager);
			virtual ~State();

			virtual void OnEnterState() = 0;
			virtual void OnLeaveState() = 0;
			virtual void OnPauseState() = 0;
			virtual void OnResumeState() = 0;

			virtual void Update(float deltaTime) = 0;
			virtual void Render(render::Renderer& renderer, render::Camera& camera) = 0;

		protected:
			StateManager& mStateManager;
			io::ResourceManager& mResourceManager;
		};

		class StateManager
		{
		public:
			StateManager();
			~StateManager();

			void PushState(State& state);
			void SwapState(State& state);
			void PopState();

			void Pause();
			void Resume();

			void Update(float deltaTime);
			void Render(render::Renderer& renderer, render::Camera& camera);

		private:
			std::vector<State*> mStateStack;
		};
	}
}