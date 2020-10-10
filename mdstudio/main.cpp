///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
///////////////////////////////////////////////////////

#include <windows.h>
#include "BeehiveStudio.h"
#include "core/time/Time.h"

namespace ion
{
	int EntryPoint(int numargs, char** args)
	{
		SetDllDirectoryA("bin");

		Beehive app;

		if (app.Initialise())
		{
			float deltaTime = 0.0f;
			bool run = true;
			while (run)
			{
				u64 startTicks = ion::time::GetSystemTicks();

				if (run = app.Update(deltaTime))
				{
					app.Render();
				}

				u64 endTicks = ion::time::GetSystemTicks();
				deltaTime = (float)ion::time::TicksToSeconds(endTicks - startTicks);
			}

			app.Shutdown();
		}

		return 0;
	}
}