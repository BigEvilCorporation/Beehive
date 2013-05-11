#include "core/Application.h"
#include "core/FileSystem.h"
#include "core/Archive.h"
#include "core/Serialise.h"

#include "IOTestData.h"

class IOTest : public ion::framework::Application
{
public:
	IOTest();
	~IOTest();

	bool Initialise();
	void Shutdown();
	bool Update(float deltaTime);
	void Render();

	void TestBasicSerialisation();
	void TestVersionedSerialisation();

	ion::io::FileSystem* mFileSystem;
};
