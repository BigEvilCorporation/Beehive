///////////////////////////////////////////////////
// File:		Scheme.cpp
// Date:		22nd September 2011
// Authors:		Matt Phillips
// Description:	UI scheme
///////////////////////////////////////////////////

#include "Scheme.h"

namespace ion
{
	namespace gui
	{
		Scheme::Scheme(std::string schemeName)
		{
			mSchemeName = schemeName;
			schemeName += ".scheme";

			#if CEGUI_VERSION >= 8
			CEGUI::SchemeManager::getSingleton().createFromFile(schemeName.c_str());
			std::string mouseCursorName = mSchemeName + "/MouseArrow";
			CEGUI::System::getSingleton().setDefaultMouseCursor(mouseCursorName);
			#else
			CEGUI::SchemeManager::getSingleton().create(schemeName.c_str());
			CEGUI::System::getSingleton().setDefaultMouseCursor(mSchemeName, "MouseArrow");
			#endif
		}

		Scheme::~Scheme()
		{
		}

		const std::string& Scheme::GetSchemeName()
		{
			return mSchemeName;
		}
	}
}