///////////////////////////////////////////////////
// File:		ListBox.cpp
// Date:		12th November 2011
// Authors:		Matt Phillips
// Description:	UI list box
///////////////////////////////////////////////////

#include "ListBox.h"

namespace ion
{
	namespace gui
	{
		Event ListBox::sOnItemSelectedEvent;

		ListBox::Item::Item(std::string text, int id)
			: mCEListItem(text, id, (void*)this, false, false)
		{
			//TODO: Find out why this isn't fetched from the skin
			mCEListItem.setTextColours(CEGUI::colour(0.0f, 0.0f, 0.0f, 1.0f));
			mCEListItem.setSelectionColours(CEGUI::colour(0.0f, 0.0f, 1.0f, 1.0f));
			// mCEListItem.setSelectionBrushImage("WindowsLook/Background");
		}

		std::string ListBox::Item::GetText() const
		{
			return std::string(mCEListItem.getText().c_str());
		}

		int ListBox::Item::GetId() const
		{
			return mCEListItem.getID();
		}

		ListBox::ListBox(Scheme& scheme)
			: Widget(scheme, Widget::ListBox)
		{
			mCEWidget->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&ListBox::OnItemSelected, this));
		}

		ListBox::~ListBox()
		{
		}

		void ListBox::AddItem(ListBox::Item& item)
		{
			((CEGUI::Listbox*)mCEWidget)->addItem(&item.mCEListItem);
		}

		void ListBox::RemoveItem(ListBox::Item& item)
		{
			((CEGUI::Listbox*)mCEWidget)->removeItem(&item.mCEListItem);
		}

		int ListBox::GetSelectedItems(std::list<ListBox::Item*>& itemList)
		{
			CEGUI::Listbox* ceListBox = (CEGUI::Listbox*)mCEWidget;

			for(CEGUI::ListboxItem* item = ceListBox->getFirstSelectedItem(); item; item = ceListBox->getNextSelected(item))
			{
				itemList.push_back((ListBox::Item*)item->getUserData());
			}

			return itemList.size();
		}

		bool ListBox::OnItemSelected(const CEGUI::EventArgs& args)
		{
			std::list<ListBox::Item*> items;
			int numItems = GetSelectedItems(items);
			Params params(items, numItems);
			params.mWidget = this;

			sOnItemSelectedEvent.Post(params);

			return true;
		}
	}
}