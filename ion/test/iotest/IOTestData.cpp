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
	if(mTestIntArray.size() != rhs.mTestIntArray.size())
	{
		return false;
	}

	if(mTestIntList.size() != rhs.mTestIntList.size())
	{
		return false;
	}

	if(mTestIntMap.size() != rhs.mTestIntMap.size())
	{
		return false;
	}

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

	for(std::list<int>::iterator it = mTestIntList.begin(), end = mTestIntList.end(), rhsIt = rhs.mTestIntList.begin(), rhsEnd = rhs.mTestIntList.end(); it != end && rhsIt != rhsEnd; ++it, ++rhsIt)
	{
		if(*it != *rhsIt)
		{
			return false;
		}
	}

	for(std::map<std::string, int>::iterator it = mTestIntMap.begin(), end = mTestIntMap.end(), rhsIt = rhs.mTestIntMap.begin(), rhsEnd = rhs.mTestIntMap.end(); it != end && rhsIt != rhsEnd; ++it, ++rhsIt)
	{
		if(it->first.compare(rhsIt->first) != 0 || it->second != rhsIt->second)
		{
			return false;
		}
	}

	return mTestInt == rhs.mTestInt
		&& mTestIntV2 == rhs.mTestIntV2
		&& mTestFloat == rhs.mTestFloat
		&& mTestString1.compare(rhs.mTestString1) == 0
		&& mTestString2.compare(rhs.mTestString2) == 0
		&& mTestSubClass == rhs.mTestSubClass
		&& *mTestSubClassPtrBase == *rhs.mTestSubClassPtrBase
		&& *mTestSubClassPtrDerived1 == *rhs.mTestSubClassPtrDerived1
		&& *mTestSubClassPtrDerived2 == *rhs.mTestSubClassPtrDerived2
		&& mNullPtr == rhs.mNullPtr;
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
	//Register pointer types
	archive.RegisterPointerType<TestSerialisable::SubClass>();
	archive.RegisterPointerType<TestSerialisable::SubClassDerived>();

	archive.Serialise(mTestInt);
	archive.Serialise(mTestFloat);
	
	archive.Serialise(mTestIntArray);
	archive.Serialise(mTestFloatArray);
	archive.Serialise(mTestIntList);
	archive.Serialise(mTestIntMap);

	archive.Serialise(mTestString1);
	archive.Serialise(mTestString2);

	if(archive.GetVersion() >= 2)
	{
		archive.Serialise(mTestIntV2);
	}

	archive.Serialise(mTestSubClass);

	archive.Serialise(mTestSubClassPtrBase);
	archive.Serialise(mTestSubClassPtrDerived1);
	archive.Serialise(mTestSubClassPtrDerived2);
	archive.Serialise(mNullPtr);
}

TestSerialisable::SubClass::SubClass()
{
	mTestInt = 1;
	mTestFloat = 3.14195f;
	mTestIntV2 = 2;
}

bool TestSerialisable::SubClass::operator == (TestSerialisable::SubClass& rhs)
{
	return mTestInt == rhs.mTestInt
		&& mTestIntV2 == rhs.mTestIntV2
		&& mTestFloat == rhs.mTestFloat
		&& mTestString.compare(rhs.mTestString) == 0
		&& GetTestInt() == rhs.GetTestInt();
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

TestSerialisable::SubClassDerived::SubClassDerived()
{
	mTestInt2 = 2;
	mTestInt3 = 3;
}

bool TestSerialisable::SubClassDerived::operator == (TestSerialisable::SubClassDerived& rhs)
{
	return SubClass::operator ==((SubClass&)rhs) && mTestInt2 == rhs.mTestInt2 && mTestInt3 == rhs.mTestInt3;
}

bool TestSerialisable::SubClassDerived::Test(TestSerialisable::SubClassDerived& rhs, u32 version)
{
	return (*this == rhs);
}

void TestSerialisable::SubClassDerived::Serialise(ion::serialise::Archive& archive)
{
	//Serialise base class first
	SubClass::Serialise(archive);

	archive.Serialise(mTestInt2);
	archive.Serialise(mTestInt3);
}