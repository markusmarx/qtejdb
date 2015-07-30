set(QEJDBQUICK_QML_SOURCES
    qmldir
)

set(QEJDBQUICK_PUBLIC_HEADER
    qejdbplugin.h
    qejdbclient.h
    qbsonconverter.h
    collectionmodel.h
    basemodel.h
    arraypropertymodel.h
)

set(QEJDBQUICK_SOURCES
    qejdbplugin.cpp
    qejdbclient.cpp
    qbsonconverter.cpp
    collectionmodel.cpp
    basemodel.cpp
    arraypropertymodel.cpp
)

add_library ( qejdbplugin SHARED ${QEJDBQUICK_SOURCES} ${QEJDBQUICK_QML_SOURCES} )

qt5_use_modules( qejdbplugin Quick Qml)
set(qmldir ${CMAKE_BINARY_DIR}/plugins/QEjdb)
if(WIN32)
    target_link_libraries(qejdbplugin qtejdb Qt5::Qml Qt5::Quick)
    add_custom_command(TARGET qejdbplugin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/qmldir ${qmldir}/qmldir
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBRARY_OUTPUT_PATH}/libqejdbplugin${CMAKE_SHARED_LIBRARY_SUFFIX} ${qmldir}/qejdbplugin${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
else()
    target_link_libraries(qejdbplugin ejdb_p qtejdb_p Qt5::Qml Qt5::Quick)
    add_custom_command(TARGET qejdbplugin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/qmldir ${qmldir}/qmldir
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBRARY_OUTPUT_PATH}/libqejdbplugin${CMAKE_SHARED_LIBRARY_SUFFIX} ${qmldir}/
    )
endif()

target_include_directories (qejdbplugin PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ..
)

foreach(file ${QEJDBQUICK_QML_SOURCES} )
  add_custom_command(TARGET qejdbplugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${file} ${qmldir}/${file}
    DEPENDS ${file})
  set(output_files ${output_files} ${qmldir}/${file})
endforeach()
