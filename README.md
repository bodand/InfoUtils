# InfoUtils
Utilities used by Info* projects.

A set of utility function and classes used by Info* projects. First created when InfoConfig was in need of these utilities, and
bodand grew tired of ~~reimplementing them~~ copy-pasting from older projects. More utilities are awaited as every Info* project's 
utility directory gets lifted into this repo. 

## Documentation 
Documentation is currently pending, as we await the completion of InfoScriptor,
however, the code itself is well documented, regarding the API at least.

## Utilities
Currently, this is the list of utilities supported in InfoUtils.

Since version `1.0`:
 - `info::nonnull<T*>`: A pointer wrapper which refuses null
 - `info::nullable<T*>`: Explicitly specifies a pointer accepts null
 - `info::lambda<T>`: A lambda(tbh functor, really) wrapper to allow the creation of recursive lambdas

Since version `1.1`:
 - `info::expected<T, E>`: A exception handling type which says a `T` is expected, and if it cannot be created it is described why with an `E` object.

Since version `1.2`:
 - `info::fail<T>`: A type which returns false with the `value` member. Used to conditionally fail compilation.

## Macros
Some macros are implemented by InfoUtils as further utilities to accompany the
class-types described above. These are used internally by InfoUtils as well.

Since version `1.0`:
 - `INFO_NODISCARD(MSG)`: Applies the C++20 `[[nodiscard(MSG)]]` attribute if supported, or the C++17 `[[nodiscard]]` attribute if supported.

Since version `1.1`:
 - `INFO_LIKELY`: Applies the C++ `[[likely]]` attribute if supported.
 - `INFO_UNLIKELY`: Applies the C++ `[[unlikely]]` attribute if supported.

## License

The InfoUtils project is licensed under the `BSD 3-Clause` license. For more information
see the provided *LICENSE* file.<br />
The dependency [Boost][boost] is licensed under the `Boost Software License`. For more information
see the provided *LICENSE.boost* file, or the official [website][bsl].<br />
The test dependency [snowhouse][sh] is licensed under the `Boost Software License`. For more information
see the provided *LICENSE.boost* file, or the official [website][bsl].


[boost]: https://boost.org/
[sh]: https://github.com/banditcpp/snowhouse
[bsl]: https://www.boost.org/users/license.html
