set(QBSON_SOURCES 
	qbsonarray.cpp 
	qbsonobject.cpp 
	qbsonvalue.cpp 
	qbsonoid.cpp
        qbsontobsonrec.cpp
        qbsonfrombsonrec.cpp
)

find_package(Qt5Core)

add_library ( qbson SHARED ${QBSON_SOURCES} )

target_include_directories (qbson PUBLIC ${EJDB_INCLUDE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(qbson Qt5::Core z ejdb_p)

set(CMAKE_C_FLAGS "-std=gnu99")
