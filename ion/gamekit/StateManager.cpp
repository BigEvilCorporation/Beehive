///////////////////////////////////////////////////
// File:		StateManager.cpp
// Date:		16th February 2014
// Authors:		Matt Phillips
// Description:	Game state manager
///////////////////////////////////////////////////

#include "gamekit/StateManager.h"
#include "core/debug/Debug.h"

namespace ion
{
	namespace gamekit
	{
		State::State(StateManager& stateManager, io::ResourceManager& resourceManager)
			: mStateManager(stateManager)
			, mResourceManager(resourceManager)
		{
		}

		State::~State()
		{
		}

		StateManager::StateManager()
		{
		}

		StateManager::~StateManager()
		{
		}

		void StateManager::PushState(State& state)
		{
			if(mStateStack.size() > 0)
				mStateStack.back()->OnLeaveState();

			state.OnEnterState();
			mStateStack.push_back(&state);
		}

		void StateManager::SwapState(State& state)
		{
			if(mStateStack.size() > 0)
				PopState();

			state.OnEnterState();
			mStateStack.push_back(&state);
		}

		void StateManager::PopState()
		{
			debug::Assert(mStateStack.size() > 0, "StateManager::PopState() - State stack is empty");
			mStateStack.back()->OnLeaveState();
			mStateStack.pop_back();
		}

		void StateManager::Pause()
		{
			if(mStateStack.size() > 0)
				mStateStack.back()->OnPauseState();
		}

		void StateManager::Resume()
		{
			if(mStateStack.size() > 0)
				mStateStack.back()->OnResumeState();
		}

		void StateManager::Update(float deltaTime, input::Keyboard* keyboard, input::Mouse* mouse, input::Gamepad* gamepad)
		{
			if(mStateStack.size() > 0)
				mStateStack.back()->Update(deltaTime, keyboard, mouse, gamepad);
		}

		void StateManager::Render(render::Renderer& renderer, render::Camera& camera)
		{
			if(mStateStack.size() > 0)
				mStateStack.back()->Render(renderer, camera);
		}
	}
}