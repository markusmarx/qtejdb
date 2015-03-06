set(QEJDB_PUBLIC_HEADER 
	qejdbdatabase.h 
	qejdbexception.h
	qejdbquery.h 
	qejdbresult.h
        qejdbcollection.h
        qejdbworker.h
        qejdbrpcworker.h
        qejdbitemmodel.h
        qbsonitemmodel_p.h
)

set(QEJDB_SOURCES
    ../../3rdparty/ejdb/tcejdb/ejdb.c
    ../../3rdparty/ejdb/tcejdb/ejdbutl.c
    ../../3rdparty/ejdb/tcejdb/tcadb.c
    ../../3rdparty/ejdb/tcejdb/tcbdb.c
    ../../3rdparty/ejdb/tcejdb/tcfdb.c
    ../../3rdparty/ejdb/tcejdb/tchdb.c
    ../../3rdparty/ejdb/tcejdb/tctdb.c
    qejdbcollection.cpp 
	qejdbdatabase.cpp 
	qejdbexception.cpp 
	qejdbfileworker.cpp
	qejdbquery.cpp
	qejdbresult.cpp
	qejdbrpcworker.cpp
	qejdbworker.cpp
        qejdbitemmodel.cpp
        qbsonitemmodel.cpp
)

find_package(Qt5Core)

add_library ( qejdb SHARED ${QEJDB_SOURCES} )

target_include_directories (qejdb PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR} 
        ${PROJECT_SOURCE_DIR}/3rdparty/ejdb/tcejdb/
	../qbson
        ${PROJECT_SOURCE_DIR}/3rdparty/qtrcp2/include
        ${PROJECT_SOURCE_DIR}/3rdparty/qtrcp2/lib
)

target_link_libraries(qejdb Qt5::Core qbson qtrpc2)

set(CMAKE_C_FLAGS "-std=gnu99")

file(COPY ${QEJDB_PUBLIC_HEADER} DESTINATION ${PROJECT_SOURCE_DIR}/include)
