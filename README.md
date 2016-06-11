# Beehive
Beehive: A complete content tool for the SEGA Mega Drive (and soon to be more)

///////////////////////////////////////////////////////
// BUILDING
///////////////////////////////////////////////////////

*** To create a workspace, run one of the GenerateWorkspace_VS20XX.bat files ***

Beehive (and all other ion::engine projects) use the JamPlus build system (see https://github.com/jamplus/jamplus).
JamPlus includes support for XCode and other IDEs, but the .sh files need fetching from JamPlus source and I haven't
used it in a long time so, mileage may vary.

///////////////////////////////////////////////////////
// DEPENDENCIES
///////////////////////////////////////////////////////

You'll need:

- NVIDIA Cg 2.1
- wxWidgets 3.5.1

to go in the ion/dependencies directory.

I've added binary dependencies for Win32, MSBuild v11 (VS2013) here:

- http://www.mediafire.com/download/yl22ar67eszy5r6/beehive_ion_dependencies.7z

This is only temporary and probably violates some license somewhere
so it's only provided for convenience until I figure out the configuration used to build them in the first place.

///////////////////////////////////////////////////////
// ALTERNATIVE PLATFORMS
///////////////////////////////////////////////////////

Initial support for SNES added. New projects prompt for tile width/height, I *think* I've fixed up all places
referencing tile width/height properly but no promises. I've tested that it at least imports new tiles and
creates/draws stamps correctly. Take a look at PlatformConfig.h and all references to PlatformConfig.

The first thing that will need fixing up for SNES is the Export() functions (particulary Tile and Map).
I know nothing of Nintendo systems so I haven't touched them yet!
