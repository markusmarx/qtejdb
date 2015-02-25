import QtQuick 2.3
import QtQuick.Window 2.1
import QEjdb 1.0 as QEjdb

Window {
    visible: true
    width: 360
    height: 360

    QEjdb.QEjdbClient {
        id: qejdbClient
        uri: 'file:testmodel_db2'

        Component.onCompleted: {
            connect();
            var json = save('testcollection',
                 {
                     'name':'test',
                     'date': new Date()
                 })
            console.log(json.id);
        }
    }



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
