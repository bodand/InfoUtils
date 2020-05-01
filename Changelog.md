# InfoUtils Changelog

The followings list every change ever to happen to the codebase on a behavioral level.
Each version has their own subheader with sub-subheaders defining what's been added, changed, or removed. 
Additionally, every version-level header may have a 'Developer notes' subheader defining notes
by the developer publishing that version addressed to other developers (or their older selves),
which are more implementation detail oriented.
Patch updates may not include the whole set of subheaders at the developer's discretion.  

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
