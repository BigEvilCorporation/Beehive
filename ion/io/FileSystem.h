///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
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
			std::vector<FileDevice*> m_fileDevices;
			FileDevice* m_defaultFileDevice;
		};
	}
}