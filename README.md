# D3D Example

A (currently broken) simple interactive example of rendering a cube using Direct3D 11
implemented for debugging purposes.

## Overview

The source code is under the `code` directory. The main file is `win_main.cpp` which
includes setting up the Windows window. The Direct3D initialization work is in
`d3d.cpp`. The rendering happens in the UpdateAndRender function of `game.cpp`.


## To Build And Run

The general workflow is listed below, but see NOTES section for important notes.

1. navigate to project root
2. run `setup`
3. run `build`
4. run `debug`
5. hit F5 in Visual Studio to start program

## NOTES

- expects to compile with MSVC
- setup.bat attempts to run vcvarsall from Visual Studio 2019 Community Edition
- build.bat compiles all the things into a build directory above the root directory
- debug.bat runs devenv targeting the build directory
