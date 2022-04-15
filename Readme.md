# Impossible Creatures Mod and SDK examples

This repository shows various simple to complex mods for Impossible Creatures using the Relic SDK (provided in the Steam version)

## Building with Visual Studio 2022 CE

Install Visual Studio 2022 CE and C++ develper tools and open this project.

To link the SDK headers you need to navigate to `HelloMod -> Properties ->
C/C++ -> General -> Additional Includes` and add the path to the `Include` folder
inside the SDK directory.

To link the SDK libs you need to navigate to `HelloMod -> Properties ->
Linker -> General -> Additional Library Directories` and add the path to the `Include` folder
inside the SDK directory.

Both are set to `C:\Program Files (x86)\Steam\steamapps\common\Impossible Creatures` by default.