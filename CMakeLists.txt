cmake_minimum_required(VERSION 2.8)
#cmake_policy(SET CMP0028 OLD)
project(qtejdb)

option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")


# Enable ExternalProject CMake module
include(ExternalProject)
include(CodeCoverage)

SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage --coverage -fPIC")
#	 SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(LIBRARY_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/lib)
set(EJDB_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/3rdparty/ejdb/src/ejdb
                      ${CMAKE_SOURCE_DIR}/3rdparty/ejdb/src/bson
                      ${CMAKE_SOURCE_DIR}/3rdparty/ejdb/src/tcutil
                      ${PROJECT_BINARY_DIR}/3rdparty/ejdb/src/generated)

enable_testing()

MACRO(add_qejdb_test testname)
  add_executable ( ${testname} ${testname}.cpp )
  target_link_libraries ( ${testname}
    Qt5::Qml Qt5::Quick Qt5::Core Qt5::Test qtrpc2 qejdb qbson pthread z
  )
  target_include_directories (${testname} PUBLIC ../../include)
  add_test(${testname} ${testname})
ENDMACRO()

find_package(Qt5Core REQUIRED)
if(NOT Qt5Core_FOUND)
    message(FATAL_ERROR "Qt5Core module is required!")
endif()


find_package(Qt5Gui REQUIRED)
if(NOT Qt5Gui_FOUND)
    message(FATAL_ERROR "Qt5Gui module is required!")
endif()

find_package(Qt5Qml REQUIRED)
if(NOT Qt5Qml_FOUND)
    message(FATAL_ERROR "Qt5Qml module is required!")
endif()

find_package(Qt5Quick REQUIRED)
if(NOT Qt5Quick_FOUND)
    message(FATAL_ERROR "Qt5Quick module is required!")
endif()

find_package(Qt5Test REQUIRED)
if(NOT Qt5Test_FOUND)
    message(FATAL_ERROR "Qt5Test module is required!")
endif()

find_package(Qt5QuickTest REQUIRED)
if(NOT Qt5QuickTest_FOUND)
    message(FATAL_ERROR "Qt5QuickTest module is required!")
endif()

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Gui_INCLUDE_DIRS})
include_directories(${Qt5Quick_INCLUDE_DIRS})
include_directories(${Qt5Qml_INCLUDE_DIRS})

#
# ADD BUILD FOR QTEJDB LIBRARY
#
add_subdirectory(3rdparty/ejdb)
add_subdirectory(src)

#
# ADD BUILD FOR QTQUCKPLUGIN
#
add_subdirectory(src/qtquickplugin)

#
# ADD UNIT-TESTS
#
add_subdirectory(test/unit)
add_subdirectory(test/unit_qml)

#
# ADD DEMO AND MANUAL TESTS
#
add_subdirectory(test/manual/demo1)



