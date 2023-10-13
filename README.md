# README

This is a collection of my code snippets regarding OpenGL programming in C++.
Some of them are copied or modified from other sources, so if you find it against your copyright, please let me know.
I will refer the source as far as possible.

## How To Build

Call cmake with proper toolchain file of vcpkg, and pass proper triplet.
For example:

```sh
# in build directory
cmake -DCMAKE_TOOLCHAIN_FILE=C:/src/vcpkg/scripts/buildsystems/vcpkg.cmake ..
```

Or you can set these cache variables in `CMakePresets.json`.

Only tested under MSVC.

## Where to find exercise answer in this repo

I done some of the exercises. You can checkout corressponding branch to get code.
The naming scheme is as following: e_#_#_#, # stands for 3 three-bit code for the exercise.
