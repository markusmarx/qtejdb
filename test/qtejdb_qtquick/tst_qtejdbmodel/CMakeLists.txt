find_package(Qt5Core)
find_package(Qt5Test)

set(tst_qtejdbmodel_SRCS demo.cpp)
qt5_add_resources(tst_qtejdbmodel_QRC demo.qrc)


add_executable(tst_qtejdbmodel ${tst_qtejdbmodel_SRCS} ${tst_qtejdbmodel_QRC})
qt5_use_modules(tst_qtejdbmodel Qml Quick)
target_include_directories (tst_qejdb PUBLIC ../../include)

target_link_libraries(
    tst_qtejdbmodel
    Qt5::Qml
    Qt5::Quick
    qejdb
    qbson
    qtrpc2
    z
)
