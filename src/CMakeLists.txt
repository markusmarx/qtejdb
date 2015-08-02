set(MODULES qbson qtejdb itemmodel)
set(ALL_SRC)
set(ALL_HDRS)
set(PUB_HDRS)

foreach(MODULE IN LISTS MODULES)
        file(GLOB MODULE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}/*.cpp)
        file(GLOB MODULE_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}/*.h)
        file(GLOB MODULE_PUB_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}/*[^_p].h)
        list(APPEND ALL_SRC ${MODULE_SRC})
        list(APPEND ALL_HDRS ${MODULE_HDRS})
        list(APPEND PUB_HDRS ${MODULE_PUB_HDRS})
endforeach(MODULE)

add_library(qtejdb_p STATIC ${ALL_SRC})
add_library(qtejdb SHARED ${ALL_SRC})

set_target_properties(qtejdb PROPERTIES
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER "${PUB_HDRS}")
set_target_properties(qtejdb_p PROPERTIES
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER "${PUB_HDRS}"
    OUTPUT_NAME qtejdb)

target_include_directories (
    qtejdb_p PUBLIC
    ${EJDB_INCLUDE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/3rdparty/qtrcp2/include
    ${PROJECT_SOURCE_DIR}/3rdparty/qtrcp2/lib
)
target_include_directories (
    qtejdb PUBLIC
    ${EJDB_INCLUDE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/3rdparty/qtrcp2/include
    ${PROJECT_SOURCE_DIR}/3rdparty/qtrcp2/lib
)


if (WIN32)
    target_link_libraries(qtejdb Qt5::Core ejdb)
    target_link_libraries(qtejdb_p Qt5::Core)
        add_custom_command(TARGET qtejdb POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${ZLIB_LIB_PATH}/libzlib.dll ${LIBRARY_OUTPUT_PATH}
        )
else()
    target_link_libraries(qtejdb Qt5::Core ejdb_p)
    target_link_libraries(qtejdb_p Qt5::Core ejdb_p)
endif()

install(TARGETS qtejdb
        EXPORT qtejdb-export
        FRAMEWORK DESTINATION ${FRAMEWORK_INSTALL_DIR} COMPONENT qtejdb
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT qtejdb
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT qtejdb
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT qtejdb
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME} COMPONENT qtejdb
)

install(TARGETS qtejdb_p
        EXPORT qtejdb-static-export
        FRAMEWORK DESTINATION ${FRAMEWORK_INSTALL_DIR} COMPONENT qtejdb
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT qtejdb
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT qtejdb
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT qtejdb
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME} COMPONENT qtejdb
)
