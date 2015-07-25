# Qt/Qml JSON database

[![Build Status](https://travis-ci.org/markusmarx/qtejdb.svg?branch=master)](https://travis-ci.org/markusmarx/qtejdb)
[![codecov.io](http://codecov.io/github/markusmarx/qtejdb/coverage.svg?branch=master)](http://codecov.io/github/markusmarx/qtejdb?branch=master)
<a href="https://scan.coverity.com/projects/5815">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/5815/badge.svg"/>
</a>
[![Gitter](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/markusmarx/qtejdb?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)

QtEjdb is an ejdb based embedded bson and json document store/ database. QtEjdb is completely build uppon Qt. Mein features are store and query documents in and from collections. QtEjdb serves a qtquickplugin to access documents from qml.

## Requirements

Dependencies:

  - Qt (>= 5)
  - ejdb (included)
  - CMake (>= 2.8.11)

Test dependencies:

  - GoogleTest, GoogleMock (automaticly downloaded while building)
  - boost (automaticly downloaded while building)

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
