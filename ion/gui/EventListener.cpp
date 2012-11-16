///////////////////////////////////////////////////
// File:		EventListener.cpp
// Date:		6th November 2011
// Authors:		Matt Phillips
// Description:	UI widget event listener
///////////////////////////////////////////////////

#include "EventListener.h"

namespace ion
{
	namespace gui
	{
		int Event::sNumEvents = 0;

		Event::Event()
			: mEventId(sNumEvents++)
		{

		}

		bool Event::operator == (Event& event) const
		{
			return GetId() == event.GetId();
		}

		int Event::GetId() const
		{
			return mEventId;
		}

		void Event::Subscribe(EventListener& eventListener)
		{
			mListeners.push_back(&eventListener);
		}

		void Event::Post(const Params& params)
		{
			for(std::list<EventListener*>::iterator it = mListeners.begin(), end = mListeners.end(); it != end; ++it)
			{
				(*it)->OnEvent(*this, params);
			}
		}
	}
}