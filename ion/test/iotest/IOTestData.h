#include "io/Archive.h"

#include <string>
#include <vector>
#include <list>
#include <map>

class TestSerialisable
{
public:

	static const u32 sVersion = 2;

	//A base subclass
	class SubClass
	{
	public:
		SubClass();

		bool operator == (SubClass& rhs);
		bool Test(TestSerialisable::SubClass& rhs, u32 version);

		virtual int GetTestInt() { return mTestInt; }

		int mTestInt;
		float mTestFloat;
		std::string mTestString;

		//Additional data for V2
		int mTestIntV2;

		void Serialise(ion::io::Archive& archive);
	};

	//A derived class
	class SubClassDerived : public SubClass
	{
	public:
		SubClassDerived();

		bool operator == (SubClassDerived& rhs);
		bool Test(TestSerialisable::SubClassDerived& rhs, u32 version);

		virtual int GetTestInt() { return mTestInt + 1; }

		void Serialise(ion::io::Archive& archive);

		int mTestInt2;
		int mTestInt3;
	};

	//A derived class to be strictly serialised as base (but reconstructed as derived)
	class SubClassDerivedSerialiseAsBase : public SubClass
	{
	public:

		SubClassDerivedSerialiseAsBase() { mDummyData = 1234; }

		//Not serialised
		int mDummyData;
	};

	TestSerialisable();

	bool operator == (TestSerialisable& rhs);
	bool Test(TestSerialisable& rhs, u32 version);

	int mTestInt;
	float mTestFloat;
	
	std::vector<int> mTestIntArray;
	std::vector<float> mTestFloatArray;
	std::list<int> mTestIntList;
	std::map<std::string, int> mTestIntMap;

	std::string mTestString1;
	std::string mTestString2;

	//Additional data for V2
	int mTestIntV2;

	//Block data
	int mTestIntBlock1;
	int mTestIntBlock2;
	int mTestIntBlock3;
	int mTestIntBlock4;
	int mTestIntBlock5;
	int mTestIntBlock6;

	SubClass mTestSubClass;

	SubClass* mTestSubClassPtrBase;
	SubClass* mTestSubClassPtrDerived1;
	SubClassDerived* mTestSubClassPtrDerived2;
	SubClass* mNullPtr;
	SubClassDerivedSerialiseAsBase* mTestSubClassPtrDerived3;

	void Serialise(ion::io::Archive& archive);
};