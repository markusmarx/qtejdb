import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0
import QEjdb 1.0 as QEjdb

Window {

    width: 800
    height: 600

    QEjdb.Client {
        id: client
        uri: "file:demo1.db"
        connectionName: "test"
        Component.onCompleted: {
            connect()
            createCollection('testcollection')
        }
        Component.onDestruction: {
            disconnect();
        }
    }

    DemoModel {
        id: model
        client: client
    }

    ColumnLayout {

        anchors.fill: parent
        spacing: 2
        TextField {
            anchors.left: parent.left
            anchors.right: parent.right
            text: "hallo"
            Keys.onReturnPressed: {
                model.saveItem(text)
                text = ''
            }
        }

        ListView {
            id: listView
            anchors.left: parent.left
            anchors.right: parent.right
            height: 500
            model: model
            delegate: Text {text: name}
        }
    }
}
