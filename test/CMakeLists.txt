find_package(GTest REQUIRED)

set(MODULES tst_qbson tst_qtejdb)
set(ALL_SRC)
set(ALL_HDRS)
set(PUB_HDRS)

foreach(MODULE IN LISTS MODULES)
        file(GLOB MODULE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}/*.cc)
        list(APPEND ALL_SRC ${MODULE_SRC})
endforeach(MODULE)

include_directories(
     ${GTEST_INCLUDE_DIRS}
     ../../src
     ${EJDB_INCLUDE_PATH}
)
add_executable(qtejdbtest ${ALL_SRC})
target_link_libraries (qtejdbtest Qt5::Core qtejdb ${GTEST_BOTH_LIBRARIES})
add_test(AllTests qtejdbtest)
SETUP_TARGET_FOR_COVERAGE(test_coverage qtejdbtest coverage)
