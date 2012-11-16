///////////////////////////////////////////////////
// File:		EventListener.h
// Date:		6th November 2011
// Authors:		Matt Phillips
// Description:	UI widget event listener
///////////////////////////////////////////////////

#pragma once

#include <list>

namespace ion
{
	namespace gui
	{
		//Forward declaration
		class EventListener;

		class Event
		{
		public:
			//Event params bass class
			class Params {};

			Event();

			int GetId() const;
			void Subscribe(EventListener& eventListener);
			void Post(const Params& params);

			bool operator == (Event& event) const;

		private:
			std::list<EventListener*> mListeners;
			const int mEventId;
			static int sNumEvents;
		};

		class EventListener
		{
		public:
			virtual void OnEvent(const Event& event, const Event::Params& params) {}
		};
	}
}