set(QEJDB_PUBLIC_HEADER
        qejdbitemmodel.h
)

set(QEJDB_SOURCES
    qejdbitemmodel.cpp
)

find_package(Qt5Core)

add_library ( qejdbmodel SHARED ${QEJDB_SOURCES} )

target_include_directories (qejdbmodel PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ../qbson
        ../qtejdb
)

target_link_libraries(qejdbmodel Qt5::Core)

file(COPY ${QEJDB_PUBLIC_HEADER} DESTINATION ${PROJECT_SOURCE_DIR}/include)
