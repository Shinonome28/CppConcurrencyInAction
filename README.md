# README

This is a collection of my code snippets regarding concurrncy programming in C++.
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
