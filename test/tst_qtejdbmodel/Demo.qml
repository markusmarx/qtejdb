import QtQuick 2.3
import QtQuick.Window 2.1

Window {
    visible: true
    width: 360
    height: 360

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }
    }

    ListView {
        anchors.fill: parent
        model: qejdbmodel
        delegate: Text {
            text: name1 + " " + name2
        }
    }
}
