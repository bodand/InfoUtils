# Configuration

The project is not that configurable. This, I know is sad, however, a few things
are configurable.

## Unexpected

The `unexpected` for the `info::expected` utility, requires an API change between
compilers. To get around this a macro defined as `INFO_UNEXPECTED` is exported,
which always expands to the correct `unexpected` identifier.
If, for some reason, you do not wish to see this macro, you can define `INFO_USE_UNEXPECTED`
to a valid identifier, which you want the library to use instead.
If it is defined, the library will use it to declare `info::INFO_USE_UNEXPECTED`, 
and will not define `INFO_UNEXPECTED`.
