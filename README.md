<img src="https://github.com/ENDESGA/EDGITOR/blob/master/git/title.png" />
[pron. EDGE-itor]

- - - - - - -

[<img align="center" alt="Discord" src="https://img.shields.io/discord/732380484956586035?color=FF0040&label=chat&logo=discord&logoColor=FFFFFF">](https://discord.gg/hrtrV2x)
[<img align="center" alt="Downloads" src="https://img.shields.io/github/downloads/ENDESGA/EDGITOR/total?color=FF0040">](https://github.com/ENDESGA/EDGITOR)
[<img align="center" alt="Release" src="https://img.shields.io/github/v/release/ENDESGA/EDGITOR?include_prereleases?color=FF0040">](https://github.com/ENDESGA/EDGITOR/releases)
[<img align="center" alt="License" src="https://img.shields.io/github/license/ENDESGA/EDGITOR?color=FF0040">](https://github.com/ENDESGA/EDGITOR)
[<img align="center" alt="Size" src="https://img.shields.io/github/languages/code-size/ENDESGA/EDGITOR?color=FF0040">](https://github.com/ENDESGA/EDGITOR)

## minimal C++ pixelart editor
EDGITOR was designed to be extremely concise and clean from the ground up. It uses SDL2 as the core of its rendering system, and is designed to be as efficient as possible. EDGITOR can theoretically support colossal canvases (stress tests got close to 20Kx20K), but that isn't a focused feature since this is a pixelart editor. That being said, this could branch off to support AA and Photoshop-like features to bridge a gap between traditional painting and pixelart.

## FEATURE GOALS:
- Extremely fast and efficient workflow.
- A big focus on palette use and creation.
- Custom-everything, from brushes to effects.
- Tile system to allow easy tile creation.
- Simple per-frame animation system.
- Limitless layers, undos, canvas size.
- Clean and minimal UI.

### COMMUNITY DRIVEN
EDGITOR is now open source, and I'd love for this to grow via the community! Feel free to send pull requests, and add features to this so it can grow faster like never before!

Now you can use Cmake!

## BUILD INSTRUCTIONS

### Dependencies

EDGITOR requires the following libraries (instructions on how to install are provided below):

  * SDL2
  * SDL2_image
  * SDL2_ttf

##### Windows

On Windows you'll need to following dependencies for building EDGITOR:
  * Git
  * Visual Studio 2015 Update 3 or newer
  * Cmake
  
##### MacOS

On MacOS you'll need to following dependencies for building EDGITOR:
  * Xcode
  * Git
  * Cmake

##### Linux

On Linux you'll need to following dependencies for building EDGITOR:
  * GCC (or an equivalent Cmake support C++ compiler like clang)
  * Git
  * Cmake

#### Installing Dependencies

EDGITOR uses vcpkg for managing it's dependencies with optimal cross-platform support.

To get started you'll need to either manually install vcpkg and set the VCPKG_ROOT environment variable to your vcpkg root, or run the bootstrap.bat/sh file in the root of the repository.

##### Installing vcpkg automatically

The repository contains a bootstrap script in the root that can be used to automatically fetch the necessary tools and dependencies for the project.

What the boostrap script does:
  * Creates a `/boostrap` directory in the repo
  * Clones or pulls (updates) vcpkg in `/boostrap/vcpkg`
  * Runs the vcpkg boostrap script to compile or update vcpkg
  * Installs the necessary project dependencies

To run the bootstrap script, open a command line or Powershell to the repository directory and run:
```
./bootstrap.bat
```

This may take 10-15 minutes on the first run as it installs vcpkg and then builds the dependencies. 

> If you want to pull the latest version of vcpkg or any of the libraries, just re-run the bootstrap script and it'll update the vcpkg install.

##### Installing vcpkg manually

First you'll need to clone the vcpkg repository somewhere on your machine (I use `C:\bin\` for files like this - Keith).
```
git clone https://github.com/microsoft/vcpkg
``` 

Next you'll need to run vcpkg's bootstrap script which will build and initialize vcpkg.
```
.\vcpkg\bootstrap-vcpkg.bat
```

Now you should add the vcpkg directory to your system `PATH` so you can run the `vcpkg` command without needing to be in the vcpkg directory. You also need to set the `VCPKG_ROOT` system or user environment variable to the vcpkg installation directory.  

Now you can install all the dependencies with the dependencies script:

**Windows**
```
dependencies.bat
```

**Linux**
```
dependencies.sh
```

### Building

#### Windows

First you'll need to generate the Visual Studio project with cmake:
```
cmake -B build -G "Visual Studio 16 2019"
```

Now you can open the EDGITOR folder as a Visual Studio project and set `EDGITOR` as StartUp Project.

#### macOS

You can build the EDGITOR project using Xcode via cmake:
```
cmake -B build -G Xcode
```

#### Linux

```
cmake -B build .
cd build
make
```

