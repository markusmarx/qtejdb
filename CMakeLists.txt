cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0028 OLD)
project(qtejdb)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(LIBRARY_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/lib)

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


add_subdirectory(3rdparty/qtrcp2)

add_subdirectory(src/qbson)
add_subdirectory(test/tst_qbson)

add_subdirectory(src/qtejdb)
add_subdirectory(test/tst_qtejdb)

add_subdirectory(src/qtejdb_server)
add_subdirectory(test/tst_qtejdbserver)

add_subdirectory(src/qtejdb_model)
add_subdirectory(test/tst_qtejdbmodel)
add_subdirectory(test/tst_concurrent)

add_subdirectory(src/qtejdb_qtquick)
add_subdirectory(test/qtejdb_qtquick)
