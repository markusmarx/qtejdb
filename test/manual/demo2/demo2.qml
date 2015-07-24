import QtQuick 2.0

import "logic.js" as Logic

Rectangle {
    width: 800;  height: 600

    id: main

    Rectangle {
        anchors.fill: parent
        id: board
        color: "#222222"

        ListView {
            interactive: false
            x: 20
            y: 20
            width: 40
            height: Math.max(childrenRect.height, 40)

            currentIndex: -1

            model: ListModel {
                ListElement { colorCode: "red"; name: "1" }
                ListElement { colorCode: "blue"; name: "2" }
                ListElement { colorCode: "green"; name: "3" }
            }
            delegate: ListItem {
            }
        }

        ListView {
            interactive: false
            x: 200
            y: 20
            width: 40
            height: Math.max(childrenRect.height, 40)

            currentIndex: -1

            model: ListModel {
                ListElement { colorCode: "red"; name: "4" }
                ListElement { colorCode: "blue"; name: "5" }
                ListElement { colorCode: "green"; name: "6" }
            }
            delegate: ListItem {
            }

        }

        MouseArea {
            id: moveArea

            property var holder: null
            parent: main
            anchors.fill: board
            onPressed: {
                Logic.handlePress(mouse, board, hidden, moveArea);
            }

            onPositionChanged: {
                Logic.positionChanged(mouse, board, hidden, moveArea);
            }

            onReleased: {
                Logic.released(hidden);
            }
        }
    }

    Rectangle {
        id: hidden
        visible: false
        rotation: 10

        property string name: ""

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: hidden.name
        }
    }
}
