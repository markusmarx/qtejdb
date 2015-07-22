# Download and install GoogleMock
ExternalProject_Add(
    gmock
    URL https://googlemock.googlecode.com/files/gmock-1.7.0.zip
    #SVN_REPOSITORY http://googlemock.googlecode.com/svn/trunk
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/googlemock
    # Disable install step
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
)

# Create a libgmock target to be used as a dependency by test programs
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gmock_main)

# Set gmock properties
ExternalProject_Get_Property(gmock source_dir binary_dir)
set_target_properties(libgmock PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/libgmock_main.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
#    "INTERFACE_INCLUDE_DIRECTORIES" "${source_dir}/include"
)

# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/include")
include_directories("${source_dir}/gtest/include")

set(MODULES qbson qtejdb itemmodel qtquickplugin)
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
target_link_libraries (qtejdbtest Qt5::Core qejdbplugin libgmock)
add_test(QtEjdbUnitTest qtejdbtest)
