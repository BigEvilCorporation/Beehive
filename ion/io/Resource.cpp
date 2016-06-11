///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		Resource.cpp
// Date:		22nd December 2013
// Authors:		Matt Phillips
// Description:	Resource management
///////////////////////////////////////////////////

#include "io/Resource.h"
#include "io/ResourceManager.h"
#include "core/thread/Atomic.h"

namespace ion
{
	namespace io
	{
		Resource::Resource(ResourceManager& resourceManager, const std::string& filename)
			: m_resourceManager(resourceManager)
		{
			m_filename = filename;
			m_isLoaded = false;
			m_resourceCount = 0;
		}

		Resource::~Resource()
		{

		}

		u32 Resource::GetResourceCount() const
		{
			return m_resourceCount;
		}

		void Resource::Reference()
		{
			if(!m_resourceCount)
			{
				m_resourceManager.RequestLoad(*this);
			}

			thread::atomic::Increment(m_resourceCount);
		}

		void Resource::Release()
		{
			thread::atomic::Decrement(m_resourceCount);

			if(!m_resourceCount)
			{
				m_resourceManager.RequestUnload(*this);
			}
		}

		const std::string& Resource::GetFilename() const
		{
			return m_filename;
		}
	}
}