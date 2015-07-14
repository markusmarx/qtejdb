enable_testing()
find_package(Qt5Core)
find_package(Qt5Test)

set(tst_qejdbmodel_SRCS demo.cpp)
qt5_add_resources(tst_qejdbmodel_QRC demo.qrc)

add_executable(tst_qejdbmodel ${tst_qejdbmodel_SRCS} ${tst_qejdbmodel_QRC})
target_include_directories (tst_qejdb PUBLIC ../../include)


target_link_libraries(
    tst_qejdbmodel
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickTest
    qejdb
    qbson
    qtrpc2
    z
)

add_test(qejdbmodel tst_qejdbmodel)
