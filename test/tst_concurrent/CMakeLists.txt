find_package(Qt5Core)
find_package(Qt5Test)

add_qejdb_test(tst_concurrent)

add_test(tst_concurrent tst_concurrent)
