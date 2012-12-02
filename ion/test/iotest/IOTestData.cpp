#include "IOTestData.h"
#include "core/Debug.h"

TestSerialisable::TestSerialisable()
{
	mTestInt = 0;
	mTestFloat = 0.0f;
	mTestIntV2 = 0;
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
		&& mTestIntV2 == rhs.mTestIntV2
		&& mTestFloat == rhs.mTestFloat
		&& mTestString1.compare(rhs.mTestString1) == 0
		&& mTestString2.compare(rhs.mTestString2) == 0
		&& mTestSubClass == rhs.mTestSubClass;
}


bool TestSerialisable::Test(TestSerialisable& rhs, u32 version)
{
	bool passed = false;

	if(version == sVersion)
	{
		passed = (*this == rhs);
	}
	else if(version == 1)
	{
		passed = mTestInt == rhs.mTestInt
			&& mTestFloat == rhs.mTestFloat
			&& mTestString1.compare(rhs.mTestString1) == 0
			&& mTestString2.compare(rhs.mTestString2) == 0
			&& mTestSubClass == rhs.mTestSubClass;

		for(unsigned int i = 0; i < mTestIntArray.size(); i++)
		{
			if(mTestIntArray[i] != rhs.mTestIntArray[i])
			{
				passed = false;
			}
		}

		for(unsigned int i = 0; i < mTestFloatArray.size(); i++)
		{
			if(mTestFloatArray[i] != rhs.mTestFloatArray[i])
			{
				passed = false;
			}
		}
	}
	else
	{
		ion::debug::Error("Bad version");
	}

	return passed;
}

void TestSerialisable::Serialise(ion::serialise::Archive& archive)
{
	archive.Serialise(mTestInt);
	archive.Serialise(mTestFloat);
	
	archive.Serialise(mTestIntArray);
	archive.Serialise(mTestFloatArray);

	archive.Serialise(mTestString1);
	archive.Serialise(mTestString2);

	if(archive.GetVersion() >= 2)
	{
		archive.Serialise(mTestIntV2);
	}

	archive.Serialise(mTestSubClass);
}

TestSerialisable::SubClass::SubClass()
{
	mTestInt = 0;
	mTestFloat = 0.0f;
	mTestIntV2 = 0;
}

bool TestSerialisable::SubClass::operator == (TestSerialisable::SubClass& rhs)
{
	return mTestInt == rhs.mTestInt
		&& mTestIntV2 == rhs.mTestIntV2
		&& mTestFloat == rhs.mTestFloat
		&& mTestString.compare(rhs.mTestString) == 0;
}

bool TestSerialisable::SubClass::Test(TestSerialisable::SubClass& rhs, u32 version)
{
	bool passed = false;

	if(version == TestSerialisable::sVersion)
	{
		passed = (*this == rhs);
	}
	else if(version == 1)
	{
		passed = mTestInt == rhs.mTestInt
			&& mTestFloat == rhs.mTestFloat
			&& mTestString.compare(rhs.mTestString) == 0;
	}
	else
	{
		ion::debug::Error("Bad version");
	}

	return passed;
}

void TestSerialisable::SubClass::Serialise(ion::serialise::Archive& archive)
{
	archive.Serialise(mTestInt);
	archive.Serialise(mTestFloat);
	archive.Serialise(mTestString);

	if(archive.GetVersion() >= 2)
	{
		archive.Serialise(mTestIntV2);
	}
}