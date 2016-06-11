///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
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
			: m_stateManager(stateManager)
			, m_resourceManager(resourceManager)
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
			if(m_stateStack.size() > 0)
				m_stateStack.back()->OnLeaveState();

			state.OnEnterState();
			m_stateStack.push_back(&state);
		}

		void StateManager::SwapState(State& state)
		{
			if(m_stateStack.size() > 0)
				PopState();

			state.OnEnterState();
			m_stateStack.push_back(&state);
		}

		void StateManager::PopState()
		{
			debug::Assert(m_stateStack.size() > 0, "StateManager::PopState() - State stack is empty");
			m_stateStack.back()->OnLeaveState();
			m_stateStack.pop_back();
		}

		void StateManager::Pause()
		{
			if(m_stateStack.size() > 0)
				m_stateStack.back()->OnPauseState();
		}

		void StateManager::Resume()
		{
			if(m_stateStack.size() > 0)
				m_stateStack.back()->OnResumeState();
		}

		void StateManager::Update(float deltaTime, input::Keyboard* keyboard, input::Mouse* mouse, input::Gamepad* gamepad)
		{
			if(m_stateStack.size() > 0)
				m_stateStack.back()->Update(deltaTime, keyboard, mouse, gamepad);
		}

		void StateManager::Render(render::Renderer& renderer, render::Camera& camera)
		{
			if(m_stateStack.size() > 0)
				m_stateStack.back()->Render(renderer, camera);
		}
	}
}