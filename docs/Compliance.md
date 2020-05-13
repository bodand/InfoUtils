# Compiler compliance
aka problems with MSVC.

---

The code, as provided on the master-branch, is sure to compile under the newest
released GCC and Clang versions. For MSVC, it may or may not work, and possibly 
may need some tweaking, see [MSVC](#MSVC).

## MSVC

The project does not compile under MSVC prior to `v1.4.0 Izanami`, which brought
a macro configuration to allow compilation with MSVC.  
As a default, the project compiles with either GCC/Clang or MSVC, however,
as MSVC *requires* a change in the public API, code written for either will not compile
for the other. 
To get around this limitation the macro `INFO_UNEXPECTED` is defined. For more information
see the configuration's documentation's section under `Unexpected`. 
