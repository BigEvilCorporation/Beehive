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
			enum DeviceType { Fixed, Optical, Removable, Network };
			enum AccessType { ReadOnly, Writeable };
			enum FileType { File, Directory };

			struct DirectoryItem
			{
				//TODO: Add creation/modify time, etc
				std::string mFilename;
				FileType mFileType;
				AccessType mAccessType;
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

			const std::string mLabel;
			const std::string mMountPoint;
			const DeviceType mDeviceType;
			const AccessType mAccessType;

			std::string mCurrentDirectory;
		};
	}
}