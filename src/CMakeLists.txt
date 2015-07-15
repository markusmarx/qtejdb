set(MODULES qbson qtejdb)
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

add_library(qtejdb SHARED ${ALL_SRC})
target_include_directories (
    qtejdb PUBLIC
    ${EJDB_INCLUDE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/3rdparty/qtrcp2/include
    ${PROJECT_SOURCE_DIR}/3rdparty/qtrcp2/lib
)

target_link_libraries(qtejdb Qt5::Core z ejdb_p qtrpc2)
