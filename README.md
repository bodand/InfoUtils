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

 - `info::nonnull<T*>`: A pointer wrapper which refuses null
 - `info::nullable<T*>`: Explicitly specifies a pointer accepts null
 - `info::lambda<T>`: A lambda(tbh functor, really) wrapper to allow the creation of recursive lambdas
 - `info::expected<T, E>`: A exception handling type which says a `T` is expected, and if it cannot be created it is described why with an `E` object.
 - `info::fail<T>`: A type which returns false with the `value` member. Used to conditionally fail compilation.
 - `info::functor<R(ArgsT...)>`: A function type which guarantees no reconstruction of the underlying functor.

## Macros
Some macros are implemented by InfoUtils as further utilities to accompany the
class-types described above. These are used internally by InfoUtils as well.

 - `INFO_NODISCARD(MSG)`: Applies the C++20 `[[nodiscard(MSG)]]` attribute if supported, or the C++17 `[[nodiscard]]` attribute if supported.
 - `INFO_NODISCARD_JUST`: Applies the C++17 `[[nodiscard]]` attribute if supported.
 - `INFO_LIKELY`: Applies the C++ `[[likely]]` attribute if supported.
 - `INFO_UNLIKELY`: Applies the C++ `[[unlikely]]` attribute if supported.
 - `INFO_CONSTEVAL`: Expands to `consteval` if supported, otherwise `constexpr`
 - `INFO_CONSTINIT`: Expands to `constinit` if supported, otherwise `constexpr`

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
