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
Time to perform 300000 spawns: 10ms
Time to run 100 systems with 100000 entities: 310ms
```

![image](https://github.com/g-lujan/cpp-ecs-raylib/assets/23530029/fbba0070-34aa-448e-a9d8-f89aae0f3e69)
![image](https://github.com/g-lujan/cpp-ecs-raylib/assets/23530029/edd0449c-6f0f-4b1a-9b30-729388c820c9)

  
