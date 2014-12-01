find_package(Qt5Core)
find_package(Qt5Test)

MACRO(add_qejdb_test testname)
  add_executable ( ${testname} ${testname}.cpp )
  target_link_libraries ( ${testname} Qt5::Core Qt5::Test qtrpc2 qejdb qbson pthread z)
  target_include_directories (${testname} PUBLIC ../../include)
ENDMACRO()

add_qejdb_test(tst_qejdbcollection)
add_qejdb_test(tst_qejdbdatabase)

add_test(tst_qejdbcollection tst_qejdbcollection)
add_test(tst_qejdbdatabase tst_qejdbdatabase)
