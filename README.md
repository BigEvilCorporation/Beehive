# Beehive
Beehive: A complete content tool for the SEGA Mega Drive (and soon to be more)

# Getting Started

Beehive (and all other ion::engine projects) use the JamPlus build system (see https://github.com/jamplus/jamplus).
JamPlus includes support for XCode and other IDEs, but I haven't tested building this particular project with it, nor are the dependencies for other platforms included here.

- Grab ion::engine from https://github.com/BigEvilCorporation/ion-engine
- Run ion_symlink.bat FROM AN ELEVATED COMMAND LINE (on Win10: right-click Start button, "Command Prompt (Admin)") to link up the engine
- Run mdstudio/GenerateWorkspace_VS20XX.bat to create and open a Visual Studio workspace

# Dependencies

All dependencies are now included with ion::engine, you shouldn't need to download/do anything special.

# Running

Copy all DLLs from the dependencies pack into root\mdstudio, and make sure to set root\mdstudio as the working directory before launching.

# SNES (and other platform) support

Initial support for SNES added. New projects prompt for tile width/height, I *think* I've fixed up all places
referencing tile width/height properly but no promises. I've tested that it at least imports new tiles and
creates/draws stamps correctly. Take a look at PlatformConfig.h and all references to PlatformConfig.

To get started quickly, implement Tile::Export() and Map::Export() for SNES.
