#include "core/Archive.h"

#include <string>
#include <vector>
#include <list>
#include <map>

class TestSerialisable
{
public:

	static const u32 sVersion = 2;

	class SubClass
	{
	public:
		SubClass();

		bool operator == (SubClass& rhs);
		bool Test(TestSerialisable::SubClass& rhs, u32 version);

		int mTestInt;
		float mTestFloat;
		std::string mTestString;

		//Additional data for V2
		int mTestIntV2;

		void Serialise(ion::serialise::Archive& archive);
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

	SubClass mTestSubClass;

	void Serialise(ion::serialise::Archive& archive);
};