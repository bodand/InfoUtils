# Usage

The library is designed to be used as a subproject. 
This can be achieved either manually, or like the cool people do it, requesting
CMake to do it for us. The latter is described in this document.

## FetchContent

CMake has a nice feature called ExternalProject_Add which allows the addition
of external project, however, it has a fatal flaw: it runs at build-time and is 
a bit funky to use. 
To fix that, they added the `FetchContent` module, which allows the fetching,
configuring and depending on external projects, during configuration-time. 
Just what we need. Make sure your CMake version allows you to `include(FetchContent)`.
If not, get a new CMake version.

## Declaring the dependency

In a file which includes the `FetchContent` module, you can use the functions defined
in that module. First of which is `FetchContent_Declare`. This can be used to declare
a dependency and then tell CMake how to actually download it. For this project Git is used.
I advise creating a new CMake file like `Dependencies.cmake` to shove your dependencies
inside. For an example see this project.

The following shows how to declare InfoUtils as a dependency.
```cmake
include(FetchContent)

FetchContent_Declare(
    InfoUtils # Arbitrary project name, this is advised
    GIT_REPOSITORY https://github.com/isbodand/InfoUtils.git
    GIT_TAG v1.4.0 # Or whichever version you fancy
)
```

## Getting the dependency

After declaring the dependency, you can tell CMake to get it for you. This can be 
done the easy way, or the manually configured way. Most projects will work with the 
easy way, including this one. For more information see the official [docs][cmake-fc].
(Beware, light theme.)

To get the dependency the easy way the following code is used:
```cmake
FetchContent_MakeAvailable(InfoUtils)
```
Now, CMake will download and configure the dependency project and will do the 
equivalent of `add_subdirectory`. 

## Using the dependency

Now there is only one thing to do, use the library. This is really easy, just
have to tell CMake to "link" the library to your target. This is easily achievable
by a quick call to `target_link_libraries` like so, where `proj` is your target:

```cmake
target_link_libraries(proj
                      PRIVATE info::utils) # or whichever access modifier you need
```

Most Info libraries export an alias target equivalent to their C++ namespace.

[cmake-fc]: https://cmake.org/cmake/help/latest/module/FetchContent.html#populating-the-content
