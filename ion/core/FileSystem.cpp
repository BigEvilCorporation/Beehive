///////////////////////////////////////////////////
// File:		FileSystem.cpp
// Date:		20th November 2011
// Authors:		Matt Phillips
// Description:	File device management
///////////////////////////////////////////////////

#include "FileSystem.h"

//WIN32_LEAN_AND_MEAN defined in JamRules
#include <windows.h>
#include <direct.h>

namespace ion
{
	namespace io
	{
		FileSystem::FileSystem()
		{
			mDefaultFileDevice = NULL;

			//Enumerate all file devices
			EnumerateDevices();
		}

		FileSystem::~FileSystem()
		{
			//Cleanup file devices
			for(unsigned int i = 0; i < mFileDevices.size(); i++)
			{
				delete mFileDevices[i];
			}
		}

		void FileSystem::EnumerateDevices()
		{
			static const int stringBufferLength = 2048;
			char stringBuffer[stringBufferLength] = {0};

			//Get current working directory
			_getcwd(stringBuffer, stringBufferLength);
			std::string workingDirectory = stringBuffer;

			//Get root path for working directory
			std::string workingRoot = workingDirectory.substr(0, 3);

			//Suppress "Drive not ready" error dialogue when getting attributes for floppy drives
			SetErrorMode(SEM_FAILCRITICALERRORS);

			//Enumerate volumes
			bool volumeFound = true;
			HANDLE volumeHndl = INVALID_HANDLE_VALUE;
			for(volumeHndl = FindFirstVolume(stringBuffer, stringBufferLength);
				volumeHndl != INVALID_HANDLE_VALUE && volumeFound;
				volumeFound = (FindNextVolume(volumeHndl, stringBuffer, stringBufferLength) != 0))
			{
				std::string volumeName = stringBuffer;

				//Get device type
				unsigned int winDriveType = GetDriveType(volumeName.c_str());
				FileDevice::DeviceType deviceType = FileDevice::Fixed;

				bool deviceSupported = true;

				switch(winDriveType)
				{
				case DRIVE_REMOVABLE:
					deviceType = FileDevice::Removable;
					break;

				case DRIVE_FIXED:
					deviceType = FileDevice::Fixed;
					break;

				case DRIVE_REMOTE:
					deviceType = FileDevice::Network;
					break;

				case DRIVE_CDROM:
					deviceType = FileDevice::Optical;
					break;

				default:
					//Unsupported device
					deviceSupported = false;
					break;
				}

				if(deviceSupported)
				{
					int returnLength = 0;

					//Get Windows root path name for device
					GetVolumePathNamesForVolumeName(volumeName.c_str(), stringBuffer, stringBufferLength, (PDWORD)&returnLength);
					std::string rootPath = stringBuffer;

					if(returnLength > 1)
					{
						//Check if path is writeable
						DWORD folderAttributes = GetFileAttributes((LPCSTR)rootPath.c_str());

						FileDevice::AccessType accessType = (folderAttributes & FILE_ATTRIBUTE_READONLY) ? FileDevice::ReadOnly : FileDevice::Writeable;

						//Create device
						FileDevice* device = new FileDevice(rootPath, rootPath, deviceType, accessType);

						//Add device
						mFileDevices.push_back(device);

						//Check if the current working directory is on this volume
						if(!_stricmp(rootPath.c_str(), workingRoot.c_str()))
						{
							//Set as default device
							SetDefaultFileDevice(*device);

							//Set current directory
							std::string directory = workingDirectory.substr(rootPath.size(), workingDirectory.size() - rootPath.size());
							device->SetDirectory(directory);
						}
					}
				}
			}

			//Finished volume search
			FindVolumeClose(volumeHndl);

			//Restore default error mode
			SetErrorMode(0);
		}

		FileDevice* FileSystem::FindFileDevice(io::FileDevice::DeviceType, io::FileDevice::AccessType accessType)
		{
			return NULL;
		}

		FileDevice* FileSystem::FindFileDevice(std::string label)
		{
			return NULL;
		}

		void FileSystem::SetDefaultFileDevice(FileDevice& device)
		{
			mDefaultFileDevice = &device;
		}

		FileDevice* FileSystem::GetDefaultFileDevice()
		{
			return mDefaultFileDevice;
		}
	}
}