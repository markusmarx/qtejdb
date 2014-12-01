set(QBSON_PUBLIC_HEADER 
	qbsonarray.h 
	qbsonobject.h
	qbsonoid.h 
	qbsonvalue.h
)

set(QBSON_SOURCES 
	../../3rdparty/ejdb/tcejdb/bson.c 
	../../3rdparty/ejdb/tcejdb/nxjson.c 
	../../3rdparty/ejdb/tcejdb/tcutil.c
	../../3rdparty/ejdb/tcejdb/md5.c 
	../../3rdparty/ejdb/tcejdb/myconf.c 
	../../3rdparty/ejdb/tcejdb/nix/platform.c
	../../3rdparty/ejdb/tcejdb/encoding.c 
	../../3rdparty/ejdb/tcejdb/utf8proc.c 
	../../3rdparty/ejdb/tcejdb/numbers.c
	qbsonarray.cpp 
	qbsonobject.cpp 
	qbsonvalue.cpp 
	qbsonoid.cpp
)

find_package(Qt5Core)

add_library ( qbson SHARED ${QBSON_SOURCES} )

target_include_directories (qbson PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ../../3rdparty/ejdb/tcejdb/)

target_link_libraries(qbson Qt5::Core)

set(CMAKE_C_FLAGS "-std=gnu99")

file(COPY ${QBSON_PUBLIC_HEADER} DESTINATION ${PROJECT_SOURCE_DIR}/include/qbson)
