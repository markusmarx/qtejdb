project(tst_qejdbmodel_qml)

add_custom_target("run" /usr/bin/qmlscene -I ${CMAKE_BINARY_DIR}/backend ${CMAKE_SOURCE_DIR}/app/saucybacon.qml
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
#add_custom_target(tst_qejdbmodel_qml SOURCES ModelTest.qml)
