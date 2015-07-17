set(MODULES qbson qtejdb itemmodel)
set(ALL_SRC)
set(ALL_HDRS)
set(PUB_HDRS)

list(APPEND ALL_HDRS helper.h qsignalmock.h)
list(APPEND ALL_SRC qsignalmock.cpp helper.cpp)

foreach(MODULE IN LISTS MODULES)
    file(GLOB MODULE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}/*.cc)
    list(APPEND ALL_SRC ${MODULE_SRC})
endforeach(MODULE)

include_directories(
    ../../src
    ${EJDB_INCLUDE_PATH}
    ${GTEST_INCLUDE_DIRS}
    ${GMOCK_INCLUDE_DIRS}
)

add_executable(qtejdbtest ${ALL_SRC})
target_link_libraries (qtejdbtest Qt5::Core qtejdb gmock_main libgmock)
add_test(AllTests qtejdbtest)
SETUP_TARGET_FOR_COVERAGE(test_coverage qtejdbtest coverage)
