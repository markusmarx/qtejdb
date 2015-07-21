# Qt/Qml Documentstore

[![Build Status](https://travis-ci.org/markusmarx/qtejdb.svg?branch=develop)](https://travis-ci.org/markusmarx/qtejdb)

QtEjdb is a simple to use document store. It's based on Qt and ejdb.

## Requirements

Dependencies:

  - Qt (>= 5)
  - ejdb (included)
  - boost
  - CMake (>= 2.8.11)

Test dependencies:

  - GoogleTest, GoogleMock, will automaticly downloaded while building

## Build and Test

```
  git clone --recursive https://github.com/markusmarx/qtejdb.git
  cd qtejdb
  mkdir build
  cd build
  cmake .. -DCMAKE_BUILD_TYPE=Release
  cmake --build .
```

The unit tests can be executed with ctest:

```
  ctest -V
```

## Examples

The demo can be executed with
```
  make demo1
```

## Licence

Distributed under [LGPL-3.0 license](http://opensource.org/licenses/LGPL-3.0).

Copyright (c) 2014-2015 [Markus Marx](mailto:markus.marx@marxenter.de) and contributors.
