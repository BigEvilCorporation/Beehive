///////////////////////////////////////////////////
// File:		FileSystem.h
// Date:		20th November 2011
// Authors:		Matt Phillips
// Description:	File device management
///////////////////////////////////////////////////

#pragma once

#include "io/FileDevice.h"

#include <vector>

namespace ion
{
	namespace io
	{
		class FileSystem
		{
		public:

			FileSystem();
			virtual ~FileSystem();

			//Find file device
			FileDevice* FindFileDevice(io::FileDevice::DeviceType, io::FileDevice::AccessType accessType);
			FileDevice* FindFileDevice(std::string label);

			//Set default file device
			void SetDefaultFileDevice(FileDevice& device);
			FileDevice* GetDefaultFileDevice();

		private:
			void EnumerateDevices();
			std::vector<FileDevice*> mFileDevices;
			FileDevice* mDefaultFileDevice;
		};
	}
}