#include "IOTestData.h"

TestSerialisable::TestSerialisable()
{
	mTestInt = 0;
	mTestFloat = 0.0f;
}

bool TestSerialisable::operator == (TestSerialisable& rhs)
{
	for(unsigned int i = 0; i < mTestIntArray.size(); i++)
	{
		if(mTestIntArray[i] != rhs.mTestIntArray[i])
		{
			return false;
		}
	}

	for(unsigned int i = 0; i < mTestFloatArray.size(); i++)
	{
		if(mTestFloatArray[i] != rhs.mTestFloatArray[i])
		{
			return false;
		}
	}

	return mTestInt == rhs.mTestInt
		&& mTestFloat == rhs.mTestFloat
		&& mTestString1.compare(rhs.mTestString1) == 0
		&& mTestString2.compare(rhs.mTestString2) == 0
		&& mTestSubClass == rhs.mTestSubClass;
}

void TestSerialisable::Serialise(ion::serialise::Archive& archive)
{
	archive.Serialise(mTestInt);
	archive.Serialise(mTestFloat);
	
	archive.Serialise(mTestIntArray);
	archive.Serialise(mTestFloatArray);

	archive.Serialise(mTestString1);
	archive.Serialise(mTestString2);

	mTestSubClass.Serialise(archive);
}

TestSerialisable::SubClass::SubClass()
{
	mTestInt = 0;
	mTestFloat = 0.0f;
}

bool TestSerialisable::SubClass::operator == (TestSerialisable::SubClass& rhs)
{
	return mTestInt == rhs.mTestInt
		&& mTestFloat == rhs.mTestFloat
		&& mTestString.compare(rhs.mTestString) == 0;
}

void TestSerialisable::SubClass::Serialise(ion::serialise::Archive& archive)
{
	archive.Serialise(mTestInt);
	archive.Serialise(mTestFloat);
	archive.Serialise(mTestString);
}