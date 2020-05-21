# InfoUtils Changelog

The followings list every change ever to happen to the codebase on a behavioral level.
Each version has their own subheader with sub-subheaders defining what's been added, changed, or removed. 
Additionally, every version-level header may have a 'Developer notes' subheader defining notes
by the developer publishing that version addressed to other developers (or their older selves),
which are more implementation detail oriented.
Patch updates may not include the whole set of subheaders at the developer's discretion.  

## VERSION 1.5.0 - Heimdallr

### Added:
 - `static_warning` to conditionally warn the user during compilation

### Changed:
 - `info::functor` can now can be move assigned

### Removed:
 - None

## VERSION 1.4.0 - Izanami

### Added:
 - Brief docs about general project usage and config
 - MSVC Compliance with a bit of a tweak: see `Compliance#MSVC` in `docs/`

### Changed:
 - `info::unexpected` for people who build for GCC/Clang **and** MSVC. This is not
 considered a breaking change, as before Izanami MSVC wouldn't compile, so there
 is no code to be broken by this. GCC/Clang only libraries will not experience any change. 
 - MSVC now has `/W4`, `/Wall`, and `/Wextra` disabled as those are really pedantic
   and warn for nigh each character you write
 - Drastic internal changes
 
### Removed:
 - None
 
### Developer notes:
 Changed the test framework from `Boost.Test` to `Catch2`. I wanted to actually make
 this change for a while, mostly because the way I used `Boost.Test` was horrendous 
 anyways, and kind of wanted to try `Catch2`, but didn't really do the 
 deed 'cause I's lazy. But the new C++ standard, C++20 deletes a stdlib function,
 which was used by `Boost.Test v1.71.0`, which I can get my hands on using the 
 `Boost-CMake` github project, therefore, it won't compile anymore in C++20 mode,
 therefore, I was driven in a corner to actually make the change, which this version
 contains. Should also decrease compile-times considerably, for the tests that is,
 and mostly incremental build times.   

## VERSION 1.3.1 - Daedalus-2

### Changed:
 - There was a bug which caused Daedalus to fail compilation. Detected by the InfoGrammatik project.

## VERSION 1.3.0 - Daedalus

### Added:
 - This really cool changelog
 - `info::functor<R(ArgsT...)>`: An owning function type which guarantees no reconstruction of the underlying functor.
 - `INFO_CONSTEVAL`: Expands to `consteval` if supported, otherwise `constexpr`.
 - `INFO_CONSTINIT`: Expands to `constinit` if supported, otherwise `constexpr`.
 - `INFO_NODISCARD_JUST`: Applies the C++17 `[[nodiscard]]` attribute if supported.

### Changed:
 - Adapted [semantic versioning](https://semver.org):
   - Backwards compatibility broken: *major* version change.
   - A new feature added: *minor* version change.
   - A backwards-compatible change to existing features, including bugfixes: *patch* version change.
   - Any version-change from *minor* and up gets a new 
   codename.
  
### Removed:
 - Build dependency on Boost.
 - Depending on Boost being installed. Now InfoUtils will install it as a subproject
 if (and only if) testing is enabled.

### Developer notes:
  Removed the warning `-Wmissing-braces` as GCC only needs that for C, but the Clang warning
  enables it for C++ in GCC which causes false (unnecessary?) warnings. 

## VERSION 1.2

### Added:
 - `info::fail<T>`: A type which returns false with the `value` member. Used to conditionally fail compilation.
 
### Changed:
 None

### Removed:
 None

## VERSION 1.1

### Added:
 - `info::expected<T, E>`: A exception handling type which says a `T` is expected, and if it cannot be created it is described why with an `E` object.
 - `INFO_LIKELY`: Applies the C++ `[[likely]]` attribute if supported.
 - `INFO_UNLIKELY`: Applies the C++ `[[unlikely]]` attribute if supported.
 
### Changed:
 None

### Removed:
 None

## VERSION 1.0

### Added: 
  - `info::nonnull<T*>`: A pointer wrapper which refuses null.
  - `info::nullable<T*>`: Explicitly specifies a pointer accepts null.
  - `info::lambda<T>`: A lambda(tbh functor, really) wrapper to allow the creation of recursive lambdas.
  - `INFO_NODISCARD(MSG)`: Applies the C++20 `[[nodiscard(MSG)]]` attribute if supported, or the C++17 `[[nodiscard]]` attribute if supported.
  
### Changed:
 N/A
  
### Removed:
 N/A
