find_package(Qt5Core)
find_package(Qt5Test)

add_executable ( tst_qbson tst_qbson.cpp )

target_link_libraries ( tst_qbson Qt5::Core Qt5::Test qbson pthread z)
target_include_directories (tst_qbson PUBLIC ../../include)

add_test(tst_qbson tst_qbson)
