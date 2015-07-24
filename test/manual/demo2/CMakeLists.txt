project(demo2)

file(GLOB QML_JS_FILES *.qml *.js)

add_custom_target(demo2_QmlFiles ALL SOURCES ${QML_JS_FILES})
add_custom_target("demo2" APP_ID=com.mmx.demo1 /usr/bin/qmlscene -I ${CMAKE_BINARY_DIR}/plugins ${CMAKE_CURRENT_SOURCE_DIR}/demo2.qml
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

