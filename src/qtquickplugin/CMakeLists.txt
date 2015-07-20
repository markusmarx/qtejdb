set(QEJDBQUICK_QML_SOURCES
    qmldir
)

set(QEJDBQUICK_PUBLIC_HEADER
    qejdbplugin.h
    qejdbclient.h
    qbsonconverter.h
    collectionmodel.h
)

set(QEJDBQUICK_SOURCES
    qejdbplugin.cpp
    qejdbclient.cpp
    qbsonconverter.cpp
    collectionmodel.cpp
)

add_library ( qejdbplugin SHARED ${QEJDBQUICK_SOURCES} ${QEJDBQUICK_QML_SOURCES} )

qt5_use_modules( qejdbplugin Quick Qml)

target_link_libraries(qejdbplugin qtejdb_p Qt5::Qml Qt5::Quick)

target_include_directories (qejdbplugin PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ..
)

execute_process(
  COMMAND awk "{print $2}" ${CMAKE_CURRENT_SOURCE_DIR}/qmldir
  COMMAND head -n1
  COMMAND tr . /
  COMMAND tr -d \n
  OUTPUT_VARIABLE qmldir_output
)
set(qmldir ${CMAKE_BINARY_DIR}/plugins/${qmldir_output})
message(STATUS "qmldir: ${qmldir}")

add_custom_command(TARGET qejdbplugin POST_BUILD
  COMMAND mkdir -pv ${qmldir}
  COMMAND cp -uv ${CMAKE_CURRENT_SOURCE_DIR}/qmldir ${qmldir}/qmldir
  COMMAND cp -v ${LIBRARY_OUTPUT_PATH}/libqejdbplugin.so ${qmldir}/
)

foreach(file ${QEJDBQUICK_QML_SOURCES} )
  add_custom_command(TARGET qejdbplugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${file} ${qmldir}/${file}
    DEPENDS ${file})
  set(output_files ${output_files} ${qmldir}/${file})
endforeach()
