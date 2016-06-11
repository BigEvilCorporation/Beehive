///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
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
			: m_label(label),
			m_mountPoint(mountPoint),
			m_deviceType(deviceType),
			m_accessType(accessType)
		{
			m_currentDirectory = "/";
		}

		FileDevice::DeviceType FileDevice::GetDeviceType() const
		{
			return m_deviceType;
		}

		FileDevice::AccessType FileDevice::GetAccessType() const
		{
			return m_accessType;
		}

		const std::string& FileDevice::GetLabel() const
		{
			return m_label;
		}

		const std::string& FileDevice::GetMountPoint() const
		{
			return m_mountPoint;
		}

		void FileDevice::SetDirectory(std::string directory)
		{
			//If path begins with mount point, skip it
			if(directory.find(m_mountPoint) == 0)
			{
				m_currentDirectory = directory.substr(0, m_mountPoint.size());
			}
			else
			{
				m_currentDirectory = NormalisePath(directory);
			}
		}

		const std::string& FileDevice::GetDirectory() const
		{
			return m_currentDirectory;
		}

		void FileDevice::ReadDirectory(std::string directory, std::vector<DirectoryItem>& directoryListing)
		{
			HANDLE searchHndl = INVALID_HANDLE_VALUE;
			bool fileFound = true;

			WIN32_FIND_DATA findData = {0};

			std::string path = m_mountPoint;
			path += directory;
			path += "\\*.*";
			path = NormalisePath(path);

			for(searchHndl = FindFirstFile(path.c_str(), &findData);
				searchHndl != INVALID_HANDLE_VALUE && fileFound;
				fileFound = (FindNextFile(searchHndl, &findData) != 0))
			{
				DirectoryItem directoryItem;

				directoryItem.m_filename = findData.cFileName;
				directoryItem.m_fileType = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? eDirectory : eFile;
				directoryItem.m_accessType = (findData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) ? eReadOnly : eWriteable;

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