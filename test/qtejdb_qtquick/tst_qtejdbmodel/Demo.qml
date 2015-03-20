import QtQuick 2.3
import QtQuick.Window 2.1
import QEjdb 1.0 as QEjdb

Window {
    visible: true
    width: 360
    height: 360


    /*QEjdb.QEjdbClient {
        id: qejdbClient
        uri: 'file:testmodel_db2'

        Component.onCompleted: {
            connect();
            var json = save('testcollection',
                 {
                     'name':'test',
                     'date': new Date(),
                     'object': {
                                    'street': 'Schulze Boysen Str. 10'
                                },
                     'array': ['1', 2, '3'],
                     'test' : 'test'
                 })
            console.log(json.id);
            console.log(json.date);
            console.log(json.name);

            var json2 = load('testcollection', json.id);
            console.log(json2.id);
            console.log(json2.date);
            console.log(json2.name);
            console.log(json2.object.street);
            console.log(json2.array);
            json.object.street = 'Schnulze Boysen Str. 12';
            json.array.splice(1, 0, "5");
            save('testcollection', json);
            json2 = load('testcollection', json.id);
            console.log(json2.object.street);
            console.log(json2.array);

        }
    }
*/


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
