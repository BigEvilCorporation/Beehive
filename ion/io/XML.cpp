///////////////////////////////////////////////////
// File:		XML.cpp
// Date:		15th February 2014
// Authors:		Matt Phillips
// Description:	XML reader
///////////////////////////////////////////////////

#pragma once

#include "io/XML.h"
#include "core/string/string.h"

#include <stdlib.h>
#include <stdio.h>

namespace ion
{
	namespace io
	{
		XML::XML()
		{
		}

		XML::~XML()
		{
		}		

		bool XML::Load(const std::string& filename)
		{
			tinyxml2::XMLDocument document;
			if(document.LoadFile(filename.c_str()) == tinyxml2::XML_SUCCESS)
			{
				tinyxml2::XMLElement* root = document.FirstChildElement();
				if(root)
				{
					ParseTinyXmlElement(*root);
					return true;
				}
			}

			return false;
		}

		XML* XML::FindChild(const std::string& name) const
		{
			XML* child = NULL;

			std::string nameLower = string::ToLower(name);

			std::map<std::string, XML>::const_iterator it = mChildren.find(nameLower);
			if(it != mChildren.end())
			{
				child = (XML*)&it->second;
			}

			return child;
		}

		XML* XML::AddChild(const std::string& name)
		{
			std::string nameLower = string::ToLower(name);
			std::pair<std::map<std::string, XML>::iterator, bool> result = mChildren.insert(std::make_pair(nameLower, XML()));
			return &result.first->second;
		}

		bool XML::GetAttribute(const std::string& name, std::string& value) const
		{
			std::string nameLower = string::ToLower(name);
			std::map<std::string, std::string>::const_iterator it = mAttributes.find(nameLower);
			if(it != mAttributes.end())
			{
				value = it->second;
				return true;
			}

			return false;
		}

		bool XML::GetAttribute(const std::string& name, int& value) const
		{
			std::string nameLower = string::ToLower(name);
			std::map<std::string, std::string>::const_iterator it = mAttributes.find(nameLower);
			if(it != mAttributes.end())
			{
				value = atoi(it->second.c_str());
				return true;
			}

			return false;
		}

		bool XML::GetAttribute(const std::string& name, float& value) const
		{
			std::string nameLower = string::ToLower(name);
			std::map<std::string, std::string>::const_iterator it = mAttributes.find(nameLower);
			if(it != mAttributes.end())
			{
				value = (float)atof(it->second.c_str());
				return true;
			}

			return false;
		}

		bool XML::GetAttribute(const std::string& name, bool& value) const
		{
			std::string nameLower = string::ToLower(name);
			std::map<std::string, std::string>::const_iterator it = mAttributes.find(nameLower);
			if(it != mAttributes.end())
			{
				std::string valueLower = string::ToLower(it->second);
				value =  (valueLower == "1" || valueLower == "true") ? true : false;
				return true;
			}

			return false;
		}

		void XML::SetAttribute(const std::string& name, const std::string& value)
		{
			std::string nameLower = string::ToLower(name);
			mAttributes.insert(std::make_pair(nameLower, value));
		}

		void XML::SetAttribute(const std::string& name, int value)
		{
			std::string nameLower = string::ToLower(name);
			char text[128] = {0};
			mAttributes.insert(std::make_pair(nameLower, std::string(itoa(value, text, 10))));
		}

		void XML::SetAttribute(const std::string& name, float value)
		{
			std::string nameLower = string::ToLower(name);
			char text[128] = {0};
			sprintf_s(text, 128, "%f", value);
			mAttributes.insert(std::make_pair(nameLower, std::string(text)));
		}

		void XML::SetAttribute(const std::string& name, bool value)
		{
			std::string nameLower = string::ToLower(name);
			char text[4] = {0};
			mAttributes.insert(std::make_pair(nameLower, value ? "true" : "false"));
		}

		void XML::ParseTinyXmlElement(const tinyxml2::XMLElement& element)
		{
			//Parse this
			const char* text = element.GetText();
			if(text)
				mData = text;

			//Parse attributes
			for(const tinyxml2::XMLAttribute* attribute = element.FirstAttribute(); attribute; attribute = attribute->Next())
			{
				mAttributes.insert(std::make_pair(string::ToLower(attribute->Name()), std::string(attribute->Value())));
			}

			//Parse children
			for(const tinyxml2::XMLElement* childElement = element.FirstChildElement(); childElement; childElement = childElement->NextSiblingElement())
			{
				XML* childNode = AddChild(childElement->Name());
				childNode->ParseTinyXmlElement(*childElement);
			}
		}
	}
}