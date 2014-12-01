set(QEJDBSERVER_SOURCES
    main.cpp
    qejdbdatabaseservice.cpp
    rcpsocketlistener.cpp
    serverconfiguration.cpp
    serverlistener.cpp
)

find_package(Qt5Core)
find_package(Qt5Network)

add_executable ( qejdbserver ${QEJDBSERVER_SOURCES} )

target_include_directories (qejdbserver PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/3rdparty/qtrcp2/include
        ${PROJECT_SOURCE_DIR}/3rdparty/qtrcp2/lib
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/qbson
        ${PROJECT_SOURCE_DIR}/3rdparty/qtargparser
)

target_link_libraries(qejdbserver Qt5::Core Qt5::Network z qbson qejdb qtrpc2)
