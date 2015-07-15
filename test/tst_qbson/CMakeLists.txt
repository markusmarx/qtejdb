set(SOURCE qbsonvaluetest.cc qbsonoidtest.cc qbsontobsonrectest.cc
qbsonfrombsonrectest.cc qbsonarraytest.cc qbsonobjecttest.cc)

find_package(GTest REQUIRED)
include_directories(
    ${GTEST_INCLUDE_DIRS}
    ../../src
    ${EJDB_INCLUDE_PATH}
)
add_executable(qbsontest ${SOURCE})
target_link_libraries ( qbsontest Qt5::Core qtejdb)
target_link_libraries(qbsontest ${GTEST_BOTH_LIBRARIES})
add_test(AllTestsInFoo qbsontest)

SETUP_TARGET_FOR_COVERAGE(tst_qbson_coverage  qbsontest coverage)
