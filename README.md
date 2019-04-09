# SelScript

Sel script (sscript) is a scripting language that is used in the
[selector](https://github.com/diegoarjz/selector) procedural modelling
framework.

The goal is to allow a familiar and fluid syntax to the specification of
multiple procedural modelling methodologies (e.g., shape grammars or
graph-based approaches) and related elements (e.g., geometries or textures).

## Building SelScript

To build SelScript you need to install the following dependencies first:

* [Boost](https://www.boost.org) since the parser is implemented with [Boost Spirit](https://www.boost.org/doc/libs/1_69_0/libs/spirit/doc/html/index.html).
* [Google Test](https://github.com/google/googletest) for the unit tests.

Then execute the following commands in the terminal:

```
$ git clone https://github.com/diegoarjz/selscript.git selscript
$ cd selscript
$ mkdir build
$ cd build
$ cmake ..
$ make && make install
```

This will build and install a libselscript.a library and a sscript executable.

## Examples

You can find examples in the [scripts](https://github.com/diegoarjz/selscript/tree/master/test/scripts) folder.
