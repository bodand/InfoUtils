# InfoUtils
Utilities used by Info* projects.

A set of utility function and classes used by Info* projects. First created when InfoConfig was in need of these utilities, and
bodand grew tired of ~~reimplementing them~~ copy-pasting from older projects. More utilities are to be awaited as every Info* project's 
utility directory gets lifted into this repo. 

## Documentation 
Documentation is currently pending, as we await the completion of InfoScriptor,
however, the code itself is well documented, in regards to the API at least.

## Utilities
Currently this is the list of utilities supported in InfoUtils.

 - nonnull: A pointer wrapper which refuses null
 - nullable: Explicitly specifies a pointer accepts null
 - lambda: A lambda(tbh functor, really) wrapper to allow the creation of recursive lambdas
