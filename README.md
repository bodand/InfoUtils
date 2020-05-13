# InfoUtils
Utilities used by Info* projects.

A set of utility function and classes used by Info* projects. First created when InfoConfig was in need of these utilities, and
bodand grew tired of ~~reimplementing them~~ copy-pasting from older projects. More utilities are awaited as every Info* project's 
utility directory gets lifted into this repo. 

## Documentation 
Documentation currently relies on GitHub's Markdown viewing capabilities, and
is found under the `docs` directory as markdown files for the general usage
of the library.
For class-level documentation, check the header files at the moment. There are 
a few features which may be undocumented like `info::functor`. This is work in progress.

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
The project uses [Catch2][c2] as a test dependency which is licensed under the `Boost Software License`.
For more information see the provided *LICENSE.boost* file.

[c2]: https://github.com/catchorg/Catch2
