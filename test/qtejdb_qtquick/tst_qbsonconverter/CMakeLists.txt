add_executable ( tst_qbsonconverter tst_qbsonconverter.cpp
    ${PROJECT_SOURCE_DIR}/src/qtejdb_qtquick/qbsonconverter.cpp
)
target_link_libraries ( tst_qbsonconverter
    Qt5::Qml Qt5::Quick Qt5::Core Qt5::Test qtrpc2 qejdb qbson pthread z
)
target_include_directories (tst_qbsonconverter PUBLIC
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src/qtejdb_qtquick
)
add_test(tst_qbsonconverter tst_qbsonconverter)
