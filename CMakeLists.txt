cmake_minimum_required(VERSION 2.8.11)
#cmake_policy(SET CMP0028 OLD)
project(qtejdb)

option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)
option(BUILD_TESTS "Build test cases" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

# Enable ExternalProject CMake module
include(ExternalProject)
include(CodeCoverage)
include(CheckIncludeFile)
include(CheckLibraryExists)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(LIBRARY_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/lib)

##
## set compiler flags. add converage flags for debug mode
##
SET(QTEJDB_CMAKE_CXX_FLAGS  "")
if (NOT WIN32)
    SET(QTEJDB_CMAKE_CXX_FLAGS  "-fPIC")
endif()
SET(QTEJDB_CMAKE_CXX_FLAGS_DEBUG  "${QTEJDB_CMAKE_CXX_FLAGS} -g -O0 --coverage -fprofile-arcs -ftest-coverage")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${QTEJDB_CMAKE_CXX_FLAGS_DEBUG}")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${QTEJDB_CMAKE_CXX_FLAGS}")

##
## end set compiler flags
##
############################
##
## setup dependency projects
##
if(WIN32)
    ## check pre library for project ejdb
    check_library_exists(pcre pre_compile "" HAVE_PCRE_LIB)

    ExternalProject_Add(
        pcre_lib
        URL http://downloads.sourceforge.net/project/pcre/pcre/8.37/pcre-8.37.zip
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/pcre
        # Disable install step
        INSTALL_COMMAND ""
        BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/pcre
    )
    add_library(pcre IMPORTED STATIC GLOBAL)
    add_library(pcreposix IMPORTED STATIC GLOBAL)

    ExternalProject_Get_Property(pcre_lib source_dir binary_dir)
    add_dependencies(pcre pcre_lib)
    set_target_properties(pcre PROPERTIES
        "IMPORTED_LOCATION" "${binary_dir}/libpcre.a"
    )
    set_target_properties(pcreposix PROPERTIES
        "IMPORTED_LOCATION" "${binary_dir}/libpcreposix.a"
    )
    include_directories(${source_dir})
    link_directories(${binary_dir})

    ## check zlib for project ejdb
    ExternalProject_Add(
        zlib_project
        URL http://downloads.sourceforge.net/project/libpng/zlib/1.2.8/zlib128.zip
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/zlib
        INSTALL_COMMAND ""
    )
    add_library(libzlib IMPORTED STATIC GLOBAL)
    add_dependencies(libzlib ejdb)
    ExternalProject_Get_Property(zlib_project source_dir binary_dir)
    include_directories(${source_dir})
    link_directories(${binary_dir})

    SET(CMAKE_INCLUDE_PATH ${source_dir} ${CMAKE_INCLUDE_PATH})
    SET(CMAKE_LIBRARY_PATH ${binary_dir} ${CMAKE_LIBRARY_PATH})
    set(ZLIB_LIB_PATH ${binary_dir})

#    find_package(Lzo)
#    if (NOT LZO_FOUND)
#        ExternalProject_Add(
#            lzo_project
#            URL http://www.oberhumer.com/opensource/lzo/download/lzo-2.09.tar.gz
#            PREFIX ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/lzo
#            INSTALL_COMMAND ""
#        )
#        add_library(liblzo IMPORTED STATIC GLOBAL)
#        add_dependencies(liblzo qtejdb)
#        ExternalProject_Get_Property(lzo_project source_dir binary_dir)
#        include_directories(${source_dir})
#        link_directories(${binary_dir})

#    endif(NOT LZO_FOUND)


endif(WIN32)
##
## end of dependency projects
##
#############################
##
## generate doc
##
add_custom_target(api-doc
    qdoc --outputdir ${CMAKE_SOURCE_DIR}/build/gh-pages/api-doc ${CMAKE_SOURCE_DIR}/doc/conf.qdocconf --log-progress
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/doc
)
##
## end generate doc
##
#############################
##
##
##

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
# Build Tests
#
if (BUILD_TESTS)
    #
    # ADD UNIT-TESTS
    #
    add_subdirectory(test/unit)
    add_subdirectory(test/unit_qml)

    SETUP_TARGET_FOR_COVERAGE(test_coverage test coverage "-R;.*QtEjdb.*")

    #
    # ADD DEMO AND MANUAL TESTS
    #

    add_subdirectory(test/manual/demo1)
    add_subdirectory(test/manual/demo2)
endif()
