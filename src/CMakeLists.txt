set(MODULES qbson qtejdb itemmodel)
set(ALL_SRC)
set(ALL_HDRS)
set(PUB_HDRS)

foreach(MODULE IN LISTS MODULES)
        file(GLOB MODULE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}/*.cpp)
        file(GLOB MODULE_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}/*.h)
        list(APPEND ALL_SRC ${MODULE_SRC})
        list(APPEND ALL_HDRS ${MODULE_HDRS})
        list(APPEND PUB_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}/${MODULE}.h)
endforeach(MODULE)

add_library(qtejdb_p STATIC ${ALL_SRC})
add_library(qtejdb SHARED ${ALL_SRC})

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
