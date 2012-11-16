///////////////////////////////////////////////////
// File:		FileBrowser.h
// Date:		22th November 2011
// Authors:		Matt Phillips
// Description:	File Browser common dialog
///////////////////////////////////////////////////

#pragma once

#include "../Window.h"
#include "../Button.h"
#include "../ListBox.h"
#include "../StaticText.h"
#include "../TextBox.h"
#include "../EventListener.h"

#include "../../core/FileDevice.h"

#include <string>

namespace ion
{
	namespace gui
	{
		class FileBrowser : public Window, EventListener
		{
		public:
			enum Result { Ok, Cancelled };

			static Event sBrowserClosedEvent;

			class Params : public Widget::EventParams
			{
			public:
				Params(Result result, std::string fullPath)
					: mResult(result),
					mFullPath(fullPath)
				{}
				
				const Result mResult;
				const std::string mFullPath;
			};

			FileBrowser(Scheme& scheme, std::string title, io::FileDevice& fileDevice, std::string directory, std::vector<std::string>* wildcards);
			virtual ~FileBrowser();

			virtual void OnEvent(const Event& event, const Event::Params& params);

		private:
			void SetDirectory(std::string directory);
			void ClearList();

			gui::ListBox mListBox;
			gui::TextBox mTextFilename;
			gui::Button mButtonOk;
			gui::Button mButtonCancel;

			io::FileDevice& mFileDevice;
			std::string mCurrentDirectory;
			std::string mFullPath;
			std::vector<ion::io::FileDevice::DirectoryItem> mCurrentDirectoryItems;
			std::vector<ListBox::Item*> mListBoxItems;
		};
	}
}