#!/bin/bash

src/qtejdb_server/qejdbserver -n qtejdbtest -P testdb&
SERVERPID=`pidof src/qtejdb_server/qejdbserver`
ctest ARGS="-V"
#test/tst_qtejdbserver/tst_connection
kill $SERVERPID
