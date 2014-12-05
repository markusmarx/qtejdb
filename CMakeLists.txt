cmake_minimum_required(VERSION 2.8)

project(qtejdb)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(LIBRARY_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/lib)

enable_testing()


add_subdirectory(3rdparty/qtrcp2)

add_subdirectory(src/qbson)
add_subdirectory(test/tst_qbson)

add_subdirectory(src/qtejdb)
add_subdirectory(test/tst_qtejdb)

add_subdirectory(src/qtejdb_server)
add_subdirectory(test/tst_qtejdbserver)

add_subdirectory(src/qtejdb_model)
add_subdirectory(test/tst_qtejdbmodel)
