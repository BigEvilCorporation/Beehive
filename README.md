# Beehive
Beehive: A complete content tool for the SEGA Mega Drive (and soon to be more)

# Getting Started

Beehive (and all other ion::engine projects) use the JamPlus build system (see https://github.com/jamplus/jamplus).
JamPlus includes support for XCode and other IDEs, but the .sh files need fetching from JamPlus source and I haven't
used it in a long time so mileage may vary.

- Grab ion::engine from https://github.com/BigEvilCorporation/ion-engine
- Run ion_symlink.bat AS ADMINISTRATOR to link up the engine
- Run mdstudio/GenerateWorkspace_VS20XX.bat to create and open a Visual Studio workspace

# Dependencies

You'll need:

- NVIDIA Cg 3.1
- wxWidgets 3.0.2

I've added binary dependencies for Win32, MSBuild v11 (VS2013) here:

 http://www.mediafire.com/download/yl22ar67eszy5r6/beehive_ion_dependencies.7z

# Running

Copy all DLLs from the dependencies pack into root\mdstudio, and make sure to set root\mdstudio as the working directory before launching.

# SNES (and other platform) support

Initial support for SNES added. New projects prompt for tile width/height, I *think* I've fixed up all places
referencing tile width/height properly but no promises. I've tested that it at least imports new tiles and
creates/draws stamps correctly. Take a look at PlatformConfig.h and all references to PlatformConfig.

To get started quickly, implement Tile::Export() and Map::Export() for SNES.
