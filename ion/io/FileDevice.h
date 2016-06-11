///////////////////////////////////////////////////
// File:		FileDevice.h
// Date:		20th November 2011
// Authors:		Matt Phillips
// Description:	Interface to a file storage system
///////////////////////////////////////////////////

#pragma once

#include <vector>

namespace ion
{
	namespace io
	{
		//TODO: Pure virtual base class, for cross-platform and device support

		class FileDevice
		{
		public:
			enum DeviceType { eFixed, eOptical, eRemovable, eNetwork };
			enum AccessType { eReadOnly, eWriteable };
			enum FileType { eFile, eDirectory };

			struct DirectoryItem
			{
				//TODO: Add creation/modify time, etc
				std::string m_filename;
				FileType m_fileType;
				AccessType m_accessType;
			};

			FileDevice(std::string label, std::string mountPoint, DeviceType deviceType, AccessType accessType);

			DeviceType GetDeviceType() const;
			AccessType GetAccessType() const;
			const std::string& GetLabel() const;
			const std::string& GetMountPoint() const;

			void SetDirectory(std::string directory);
			const std::string& GetDirectory() const;

			void ReadDirectory(std::string directory, std::vector<DirectoryItem>& directoryListing);

		private:
			std::string NormalisePath(std::string path);

			const std::string m_label;
			const std::string m_mountPoint;
			const DeviceType m_deviceType;
			const AccessType m_accessType;

			std::string m_currentDirectory;
		};
	}
}