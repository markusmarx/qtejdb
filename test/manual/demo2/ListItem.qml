import QtQuick 2.0

Item {
    id: item

    width: parent.width
    height: 40

    state: ListView.isCurrentItem ? "selected" : "";

    Rectangle {
        id: container
        width: 40
        height: 40

        color: colorCode ? colorCode : ""

        Text {
            id: text
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: name ? name : ""
        }
    }

    states: [
        State {
            name: "selected";
            PropertyChanges {
                target: container
                color: "gray"
            }
            PropertyChanges {
                target: text
                text: ""
            }
        }
    ]
}
