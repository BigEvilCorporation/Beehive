///////////////////////////////////////////////////
// File:		FileDevice.cpp
// Date:		20th November 2011
// Authors:		Matt Phillips
// Description:	Interface to a file storage system
///////////////////////////////////////////////////

#include "io/FileDevice.h"

//WIN32_LEAN_AND_MEAN defined in JamRules
#include <windows.h>

namespace ion
{
	namespace io
	{
		FileDevice::FileDevice(std::string label, std::string mountPoint, DeviceType deviceType, AccessType accessType)
			: mLabel(label),
			mMountPoint(mountPoint),
			mDeviceType(deviceType),
			mAccessType(accessType)
		{
			mCurrentDirectory = "/";
		}

		FileDevice::DeviceType FileDevice::GetDeviceType() const
		{
			return mDeviceType;
		}

		FileDevice::AccessType FileDevice::GetAccessType() const
		{
			return mAccessType;
		}

		const std::string& FileDevice::GetLabel() const
		{
			return mLabel;
		}

		const std::string& FileDevice::GetMountPoint() const
		{
			return mMountPoint;
		}

		void FileDevice::SetDirectory(std::string directory)
		{
			//If path begins with mount point, skip it
			if(directory.find(mMountPoint) == 0)
			{
				mCurrentDirectory = directory.substr(0, mMountPoint.size());
			}
			else
			{
				mCurrentDirectory = NormalisePath(directory);
			}
		}

		const std::string& FileDevice::GetDirectory() const
		{
			return mCurrentDirectory;
		}

		void FileDevice::ReadDirectory(std::string directory, std::vector<DirectoryItem>& directoryListing)
		{
			HANDLE searchHndl = INVALID_HANDLE_VALUE;
			bool fileFound = true;

			WIN32_FIND_DATA findData = {0};

			std::string path = mMountPoint;
			path += directory;
			path += "\\*.*";
			path = NormalisePath(path);

			for(searchHndl = FindFirstFile(path.c_str(), &findData);
				searchHndl != INVALID_HANDLE_VALUE && fileFound;
				fileFound = (FindNextFile(searchHndl, &findData) != 0))
			{
				DirectoryItem directoryItem;

				directoryItem.mFilename = findData.cFileName;
				directoryItem.mFileType = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? Directory : File;
				directoryItem.mAccessType = (findData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) ? ReadOnly : Writeable;

				directoryListing.push_back(directoryItem);
			}
		}

		std::string FileDevice::NormalisePath(std::string inPath)
		{
			std::string outPath;

			for(unsigned int in = 0; in < inPath.size(); in++)
			{
				if(inPath[in] == '/' || inPath[in] == '\\')
				{
					//Don't copy duplicate slashes
					if(outPath.size() == 0 || outPath[outPath.size() - 1] != '\\')
					{
						outPath += '\\';
					}
				}
				else
				{
					outPath += inPath[in];
				}
			}

			return outPath;
		}
	}
}