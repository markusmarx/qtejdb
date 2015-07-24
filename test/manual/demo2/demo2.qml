import QtQuick 2.0
import QEjdb 1.0 as QEjdb
import "logic.js" as Logic

Rectangle {
    id: main

    width: 800;  height: 600

    property variant d1
    property variant d2

    QEjdb.Client {
        id: qtejdb
        uri: "file:demo2.db"
        Component.onCompleted: {
            connect()
            createCollection("testcollection")

            var json1 = load("testcollection", "55b2311259e6b64f00000000")
            var json2 = load("testcollection", "55b2311259e6b64f00000001")

            if (!json1) {
                json1 = {
                    _id: '55b2311259e6b64f00000000',
                    colors: [
                        {colorCode:'red', name: '1'},
                        {colorCode:'blue', name: '2'},
                        {colorCode:'green', name: '3'},
                    ]
                }

                json1 = save('testcollection', json1)
            }
            d1 = json1
            if (!json2) {
                json2 = {
                    _id: '55b2311259e6b64f00000001',
                    colors: [
                        {colorCode:'red', name: '1'},
                        {colorCode:'blue', name: '2'},
                        {colorCode:'green', name: '3'},
                    ]
                }
                json2 = save('testcollection', json2)
            }
            d2 = json2
        }
    }

    Component.onDestruction: {
    }

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

            model: QEjdb.ArrayPropertyModel {
                client: qtejdb
                collection: 'testcollection'
                arrayProperty: 'colors'
                parentObject: d1
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

            model: QEjdb.ArrayPropertyModel {
                client: qtejdb
                collection: 'testcollection'
                arrayProperty: 'colors'
                parentObject: d2
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
