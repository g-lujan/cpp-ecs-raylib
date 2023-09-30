# A naive implementation of an ECS framework with a Demo game using Raylib

How to build (you will need vcpkg and CMake):

## Demo Project Setup

- Get the required dependencies with VCPKG, as below:

```
vcpkg install raylib
```

## Building with MSVC
### for Debug
```
cmake -B build-msvc -S . -G "Visual Studio 17 2022"
cmake --build build-msvc
```

### for Release (Dynamic linking)
```
cmake -B build-msvc -S . -G "Visual Studio 17 2022"
cmake --build build-msvc --config Release
```

The resultant folder "build-msvc" will contain the build and the .sln project for Visual Studio.