#include "core/Archive.h"
#include "core/Serialise.h"

class TestSerialisable : public ion::serialise::Serialisable
{
public:

	class SubClass : public ion::serialise::Serialisable
	{
	public:
		SubClass();

		bool operator == (SubClass& rhs);

		int mTestInt;
		float mTestFloat;
		std::string mTestString;

		virtual void Serialise(ion::serialise::Archive& archive);
	};

	TestSerialisable();

	bool operator == (TestSerialisable& rhs);

	int mTestInt;
	float mTestFloat;
	
	std::vector<int> mTestIntArray;
	std::vector<float> mTestFloatArray;

	std::string mTestString1;
	std::string mTestString2;

	SubClass mTestSubClass;

	virtual void Serialise(ion::serialise::Archive& archive);
};