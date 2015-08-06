
project(demo1)

file(GLOB QML_JS_FILES *.qml *.js)

add_custom_target(demo1_QmlFiles ALL SOURCES ${QML_JS_FILES})

if(WIN32)
    add_custom_target("demo1" qmlscene -I ${CMAKE_BINARY_DIR}/plugins ${CMAKE_CURRENT_SOURCE_DIR}/demo1.qml
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
else()
    add_custom_target("demo1" APP_ID=com.mmx.demo1 qmlscene -I ${CMAKE_BINARY_DIR}/plugins ${CMAKE_CURRENT_SOURCE_DIR}/demo1.qml
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif(WIN32)

