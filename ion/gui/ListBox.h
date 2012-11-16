///////////////////////////////////////////////////
// File:		ListBox.h
// Date:		12th November 2011
// Authors:		Matt Phillips
// Description:	UI list box
///////////////////////////////////////////////////

#pragma once

#include "Widget.h"
#include "EventListener.h"
#include <CEGUI/CEGUI.h>

#include <string>
#include <list>

namespace ion
{
	namespace gui
	{
		//Forward declaration
		class Scheme;

		class ListBox : public Widget
		{
		public:
			static Event sOnItemSelectedEvent;

			class Item
			{
			public:
				Item(std::string text, int id);
				std::string GetText() const;
				int GetId() const;

			protected:
				CEGUI::ListboxTextItem mCEListItem;
				friend class ListBox;
			};

			class Params : public Widget::EventParams
			{
			public:
				Params(const std::list<Item*>& items, int numItems) : mItems(items) {}
				const std::list<Item*> mItems;
			};

			ListBox(Scheme& scheme);
			~ListBox();

			void AddItem(Item& item);
			void RemoveItem(Item& item);
			int GetSelectedItems(std::list<Item*>& itemList);

		private:
			bool OnItemSelected(const CEGUI::EventArgs& args);
		};
	}
}