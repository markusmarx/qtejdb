project(qejdbplugin)

set(LIBRARY_OUTPUT_PATH  ${PROJECT_BINARY_DIR})

set(QEJDBQUICK_PUBLIC_HEADER
    qejdbplugin.h
    qejdbclient.h
)

set(QEJDBQUICK_SOURCES
    qejdbplugin.cpp
    qejdbclient.cpp
)

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

add_library ( qejdbplugin SHARED ${QEJDBQUICK_SOURCES} )

qt5_use_modules( qejdbplugin Quick Qml)

target_link_libraries(qejdbplugin qejdb Qt5::Qml Qt5::Quick )

target_include_directories (qejdbplugin PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ../qbson
        ../qtejdb
)

execute_process(
  COMMAND awk "{print $2}" ${PROJECT_SOURCE_DIR}/qmldir
  COMMAND head -n1
  COMMAND tr . /
  COMMAND tr -d \n
  OUTPUT_VARIABLE qmldir_output
)
set(qmldir ../../plugins/${qmldir_output})
message(STATUS "qmldir: ${qmldir}")

add_custom_command(TARGET qejdbplugin POST_BUILD
  COMMAND mkdir -pv ${qmldir}
  COMMAND cp -uv ${PROJECT_SOURCE_DIR}/qmldir ${qmldir}/qmldir
  COMMAND mv -v *.so ${qmldir}/
)

