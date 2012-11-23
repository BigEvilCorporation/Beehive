#include "IOTest.h"
#include "core/Version.h"
#include "core/Debug.h"
#include "core/Thread.h"
#include "core/Time.h"
#include "core/BinaryFile.h"

#include <sstream>

IOTest::IOTest()
: ion::framework::Application("ion::engine I/O Test")
{
	mFileSystem = NULL;
	mKeyboard = NULL;
}

IOTest::~IOTest()
{
}

bool IOTest::Initialise()
{
	std::stringstream welcomeMsg;
	welcomeMsg << "ion::engine - build " << ion::sVersion.Major << "." << ion::sVersion.Minor << "." << ion::sVersion.Build;
	ion::debug::Log(welcomeMsg.str().c_str());

	//Create filesystem
	mFileSystem = new ion::io::FileSystem();

	//Create keyboard input handler
	mKeyboard = new ion::input::Keyboard();

	//Get default file device
	ion::io::FileDevice* defaultFileDevice = mFileSystem->GetDefaultFileDevice();

	//Get current directory
	std::string currentDirectory = defaultFileDevice->GetDirectory();

	//Read directory
	std::vector<ion::io::FileDevice::DirectoryItem> directoryItems;
	defaultFileDevice->ReadDirectory(currentDirectory, directoryItems);

	//Create source object
	TestSerialisable sourceObject;

	//Fill with known data
	sourceObject.mTestInt = 12345;
	sourceObject.mTestFloat = 2.0f;
	sourceObject.mTestString1 = "Hello ";
	sourceObject.mTestString2 = " world!";
	sourceObject.mTestIntArray.push_back(67890);
	sourceObject.mTestIntArray.push_back(98765);
	sourceObject.mTestFloatArray.push_back(3.14195f);
	sourceObject.mTestFloatArray.push_back(3.0f);
	sourceObject.mTestSubClass.mTestInt = 43210;
	sourceObject.mTestSubClass.mTestFloat = 4.0f;
	sourceObject.mTestSubClass.mTestString = "SubClass test string";

	const char* filename = "ion_serialise_test.bin";

	//Create and open a file stream for writing
	ion::io::File fileOut;
	if(!fileOut.Open(filename, ion::io::File::OpenWrite))
	{
		ion::debug::Error("Error opening file for writing");
	}

	//Create an archive for serialising out
	ion::serialise::Archive archiveOut(fileOut, ion::serialise::Archive::Out);

	//Serialise
	sourceObject.Serialise(archiveOut);

	//Close file
	fileOut.Close();

	//Create destination object
	TestSerialisable destinationObject;

	//Create and open a file stream for reading
	ion::io::File fileIn;
	if(!fileIn.Open(filename, ion::io::File::OpenRead))
	{
		ion::debug::Error("Error opening file for reading");
	}

	//Create an archive for serialising in
	ion::serialise::Archive archiveIn(fileIn, ion::serialise::Archive::In);

	//Serialise
	destinationObject.Serialise(archiveIn);

	//Close file
	fileIn.Close();

	//Test
	if(sourceObject == destinationObject)
	{
		ion::debug::Log("Source and destination objects match - serialisation test passed");
	}
	else
	{
		ion::debug::Error("Source and destination objects mismatch - serialisation test failed");
	}
	
	//No updating/rendering required, exit app
	return true;
}

void IOTest::Shutdown()
{
	if(mKeyboard)
		delete mKeyboard;

	if(mFileSystem)
		delete mFileSystem;
}

bool IOTest::Update(float deltaTime)
{
	return false;
}

void IOTest::Render()
{
}