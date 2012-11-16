///////////////////////////////////////////////////
// File:		FileBrowser.cpp
// Date:		22th November 2011
// Authors:		Matt Phillips
// Description:	File Browser common dialog
///////////////////////////////////////////////////

#include "FileBrowser.h"

namespace ion
{
	namespace gui
	{
		Event FileBrowser::sBrowserClosedEvent;

		FileBrowser::FileBrowser(Scheme& scheme, std::string title, io::FileDevice& fileDevice, std::string directory, std::vector<std::string>* wildcards)
			: Window(title, scheme, true),
			mListBox(scheme),
			mTextFilename(scheme),
			mButtonOk("Ok", scheme),
			mButtonCancel("Cancel", scheme),
			mFileDevice(fileDevice)
		{
			//Set dimentions
			SetSize(Vector2(0.5f, 0.3f));
			SetPosition(Vector2(0.1f, 0.1f));
			mListBox.SetSize(Vector2(0.955f, 0.75f));
			mListBox.SetPosition(Vector2(0.025f, 0.025f));
			mTextFilename.SetSize(Vector2(0.45f, 0.18f));
			mTextFilename.SetPosition(Vector2(0.025f, 0.8f));
			mButtonOk.SetSize(Vector2(0.22f, 0.18f));
			mButtonOk.SetPosition(Vector2(0.5f, 0.8f));
			mButtonCancel.SetSize(Vector2(0.22f, 0.18f));
			mButtonCancel.SetPosition(Vector2(0.75f, 0.8f));

			//Add widgets to window
			AddChild(mListBox);
			AddChild(mTextFilename);
			AddChild(mButtonOk);
			AddChild(mButtonCancel);

			//Subscribe to events
			ListBox::sOnItemSelectedEvent.Subscribe(*this);
			Button::sOnPressedEvent.Subscribe(*this);

			//Populate
			SetDirectory(directory);
		}

		FileBrowser::~FileBrowser()
		{
			ClearList();
		}

		void FileBrowser::SetDirectory(std::string directory)
		{
			//Clear existing items from listbox
			ClearList();

			//Get directory listing
			mFileDevice.ReadDirectory(directory, mCurrentDirectoryItems);

			//Populate list box
			for(unsigned int i = 0; i < mCurrentDirectoryItems.size(); i++)
			{
				//Generate name
				std::stringstream name;

				if(mCurrentDirectoryItems[i].mFileType == ion::io::FileDevice::Directory)
					name << "<" << mCurrentDirectoryItems[i].mFilename << ">";
				else
					name << mCurrentDirectoryItems[i].mFilename;

				//Create new item
				ion::gui::ListBox::Item* item = new ion::gui::ListBox::Item(name.str(), i);

				//Push to local list for destruction later
				mListBoxItems.push_back(item);

				//Add to listbox
				mListBox.AddItem(*item);
			}

			//Store directory
			mCurrentDirectory = directory;
		}

		void FileBrowser::ClearList()
		{
			for(unsigned int i = 0; i < mListBoxItems.size(); i++)
			{
				mListBox.RemoveItem(*mListBoxItems[i]);
				delete mListBoxItems[i];
			}

			mListBoxItems.clear();
			mCurrentDirectoryItems.clear();
		}

		void FileBrowser::OnEvent(const Event& event, const Event::Params& params)
		{
			if(event == gui::ListBox::sOnItemSelectedEvent)
			{
				//Listbox event params
				gui::ListBox::Params& listboxParams = (gui::ListBox::Params&)params;

				if(listboxParams.mWidget == &mListBox)
				{
					//Get item index
					int itemIndex = (*listboxParams.mItems.begin())->GetId();

					//Get directory item
					io::FileDevice::DirectoryItem& directoryItem = mCurrentDirectoryItems[itemIndex];

					//If a directory
					if(directoryItem.mFileType == io::FileDevice::Directory)
					{
						//Ignore "."
						if(directoryItem.mFilename != ".")
						{
							std::string newDirectory;

							//If "..", back up one directory
							if(directoryItem.mFilename == "..")
							{
								//Find last slash
								for(int i = mCurrentDirectory.size() - 2; i > 0; i--)
								{
									if(mCurrentDirectory[i] == '\\' || mCurrentDirectory[i] == '/')
									{
										newDirectory = mCurrentDirectory.substr(i, mCurrentDirectory.size() - i);
										break;
									}
								}
							}
							else
							{
								//Real directory name, make full path
								newDirectory = mCurrentDirectory + "\\" + directoryItem.mFilename;	
							}

							//Set new directory, and repopulate list box
							SetDirectory(newDirectory);
						}
						
					}
					else
					{
						//Set new filename
						mFullPath = mCurrentDirectory + "\\" + directoryItem.mFilename;
						mTextFilename.SetText(mFullPath);
					}
				}
			}
			else if(event == gui::Button::sOnPressedEvent)
			{
				//Generic widget params
				gui::Widget::EventParams& widgetParams = (gui::Widget::EventParams&)params;

				if(widgetParams.mWidget == &mButtonOk && mFullPath.size() > 0)
				{
					//Ok pressed, and a filename has been chosen
					Params okParams(Ok, mFullPath);
					okParams.mWidget = this;
					sBrowserClosedEvent.Post(okParams);
				}
				else if(widgetParams.mWidget == &mButtonCancel)
				{
					//Cancel pressed, or no filename chosen
					Params cancelParams(Cancelled, std::string());
					cancelParams.mWidget = this;
					sBrowserClosedEvent.Post(cancelParams);
				}

				//Finished, hide window (but still up to the calling code to delete it)
				Hide();
			}
		}
	}
}