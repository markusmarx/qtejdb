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

    TextField {
        id:textF
        anchors.top: parent.top
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
        anchors.top: textF.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        model: model
        delegate: Text {text: name}
    }
}
