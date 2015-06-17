#include "core/bootstrap/Application.h"
#include "io/FileSystem.h"
#include "io/Archive.h"
#include "io/Serialise.h"

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
