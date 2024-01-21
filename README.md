# A naive implementation of an ECS framework

How to build the demo Raylib game (you will need vcpkg and CMake):

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

## Performance tests

There is a performance test to see where are the hotspots of the ECS library. This test displays the time to both spawn entities and to run systems with them.
The test can also be used with VTune Profiler to check more detailed information about the library in runtime.

To run the tests, do the following:
* Build with RelWithDebInfo
```
cmake --build build-msvc --config RelWithDebInfo
```
* The test will be generated in `build-msvc\perf_test\RelWithDebInfo` and can be run with `perf_test.exe <#entities/3> <#iterations>`

* Example results from VTune, showing how the use of maps to manage entities and components together with a lack of parallelism impairs the performance:
```
Time to perform 300000 spawns: 204ms
Time to run 100 iterations: 25175ms
```

![image](https://github.com/g-lujan/cpp-ecs-raylib/assets/23530029/71cdbc37-9d5e-45c6-a503-f51775e1deaa)
![image](https://github.com/g-lujan/cpp-ecs-raylib/assets/23530029/89f234ee-4353-4155-870d-10e336eb0a76)

  
