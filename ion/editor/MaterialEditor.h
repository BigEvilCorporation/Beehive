///////////////////////////////////////////////////
// File:		MaterialEditor.h
// Date:		22nd September 2011
// Authors:		Matt Phillips
// Description:	Material editor dialog
///////////////////////////////////////////////////

#pragma once

#include "../GUI/Window.h"
#include "../GUI/Button.h"
#include "../GUI/ListBox.h"
#include "../GUI/Slider.h"

namespace ion
{
	namespace editor
	{
		class MaterialEditor
		{
		public:
			MaterialEditor();
			~MaterialEditor();

			void Show();
			void Hide();

		private:

		};
	}
}