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

	//Run tests
	TestBasicSerialisation();
	TestVersionedSerialisation();
	
	//No updating/rendering required, exit app
	return true;
}

void IOTest::TestBasicSerialisation()
{
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
	ion::serialise::Archive archiveOut(fileOut, ion::serialise::Archive::Out, TestSerialisable::sVersion);

	//Serialise
	archiveOut.Serialise(sourceObject);

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
	ion::serialise::Archive archiveIn(fileIn, ion::serialise::Archive::In, TestSerialisable::sVersion);

	//Serialise
	archiveIn.Serialise(destinationObject);

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
}

void IOTest::TestVersionedSerialisation()
{
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

	const char* filenameV1 = "ion_serialise_test_v1.bin";
	const char* filenameV2 = "ion_serialise_test_v2.bin";

	//Create and open file streams for writing
	ion::io::File fileOutV1;
	ion::io::File fileOutV2;

	if(!fileOutV1.Open(filenameV1, ion::io::File::OpenWrite))
	{
		ion::debug::Error("Error opening file for writing");
	}

	if(!fileOutV2.Open(filenameV2, ion::io::File::OpenWrite))
	{
		ion::debug::Error("Error opening file for writing");
	}

	//Create archives for serialising out
	ion::serialise::Archive archiveOutV1(fileOutV1, ion::serialise::Archive::Out, 1);
	ion::serialise::Archive archiveOutV2(fileOutV2, ion::serialise::Archive::Out, 2);

	//Serialise
	sourceObject.Serialise(archiveOutV1);
	sourceObject.Serialise(archiveOutV2);

	//Close files
	fileOutV1.Close();
	fileOutV2.Close();

	//Create destination objects
	TestSerialisable destinationObjectV1;
	TestSerialisable destinationObjectV2;

	//Create and open file streams for reading
	ion::io::File fileInV1;
	ion::io::File fileInV2;

	if(!fileInV1.Open(filenameV1, ion::io::File::OpenRead))
	{
		ion::debug::Error("Error opening file for reading");
	}

	if(!fileInV2.Open(filenameV2, ion::io::File::OpenRead))
	{
		ion::debug::Error("Error opening file for reading");
	}

	//Create archives for serialising in
	ion::serialise::Archive archiveInV1(fileInV1, ion::serialise::Archive::In, 1);
	ion::serialise::Archive archiveInV2(fileInV2, ion::serialise::Archive::In, 2);

	//Serialise
	destinationObjectV1.Serialise(archiveInV1);
	destinationObjectV2.Serialise(archiveInV2);

	//Close files
	fileInV1.Close();
	fileInV2.Close();

	//Test
	if(sourceObject.Test(destinationObjectV1, 1))
	{
		ion::debug::Log("Source and destination objects match - serialisation test passed");
	}
	else
	{
		ion::debug::Error("Source and destination objects mismatch - serialisation test failed");
	}

	if(sourceObject.Test(destinationObjectV2, 2))
	{
		ion::debug::Log("Source and destination objects match - serialisation test passed");
	}
	else
	{
		ion::debug::Error("Source and destination objects mismatch - serialisation test failed");
	}
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