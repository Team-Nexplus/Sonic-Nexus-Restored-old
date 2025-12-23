# Archival Notice
This repo is kept here for archival purposes

For the new repo, go to [RSDKv2.5](https://github.com/Team-Nexplus/RSDKv2.5)

# Sonic Nexus Restored
Sonic Nexus 08 decompilation modified for Sonic Nexus Restored

Without assets from the game, this decompilation will not run. You can download the game [here](https://info.sonicretro.org/Sonic_Nexus).

* Retro Engine by Christian Whitehead (A.K.A. The Taxman)
* RSDKv2 Decopilation by Rubberduckycooly, MegAmi, Lavesiime, LittlePlanetCD, and Leonx254

# Additional Tweaks
* Added buttons featured in v4 (X, Y, Z,  L, R,  Select)
* WASD controls by default hehe
* Extra life at 100 rings, not 99
* more junk to be added soon !11!!

# How to build
## Windows
* Clone the repo, then follow the instructions in the [depencencies readme for Windows](./dependencies/windows/dependencies.txt) to setup dependencies, then build via the visual studio solution.

## Mac
* Clone the repo, follow the instructions in the [depencencies readme for Mac](./dependencies/mac/dependencies.txt) to setup dependencies, then build via the Xcode project.

## Linux
* To setup your build enviroment and library dependecies, run the following commands:
  * Ubuntu (Mint, Pop!_OS, etc...): `sudo apt install build-essential git libsdl2-dev libvorbis-dev libogg-dev libtheora-dev`
  * Arch Linux: `sudo pacman -S base-devel git sdl2 libvorbis libogg libtheora`
* Clone the repo with the following command: `git clone https://github.com/EggBanana123/Sonic-Nexus-Restored`
* Go into the repo you just cloned with `cd Sonic-Nexus-Restored`.
* Then run `make CXXFLAGS=-O2 -j5`.
  * If your distro is using gcc 8.x.x, then add the argument `LIBS=-lstdc++fs`.
  * The `CXXFLAGS` option can be removed if you do not want optimizations.
  * -j switch is optional, but will make building faster by running it parallel on multiple cores (8 cores would be -j9.)

## Unofficial Branches
Follow the installation instructions in the readme of each branch.
* No unnoficial branches at the time of writing
  
Because these branches are unofficial, we can't provide support for them and they may not be up-to-date.

## Other Platforms
Currently the only supported platforms are the ones listed above, however the backend uses libogg, libvorbis, libtheora & SDL2 to power it, so the codebase is very multiplatform. If you're able to, you can clone this repo and port it to a platform not on the list.

# Contact:
Join the [Retro Engine Modding Discord Server](https://dc.railgun.works/retroengine) for any extra questions you may need to know about the decompilation or modding it.
