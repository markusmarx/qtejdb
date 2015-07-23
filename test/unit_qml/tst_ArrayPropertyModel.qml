import QtQuick 2.0
import QEjdb 1.0 as QEjdb
import QtTest 1.0

TestCase {
    id: testCase
    name: "QEjdbPlugin ArrayPropertyModel Test"
    property variant parentObject

    QEjdb.Client {
        id: qejdbClient
        uri: 'file:tst_qejdpluginarraymodel.db'
        autoCreateCollection: true
        Component.onCompleted: {
            connect()
            removeCollection('testcollection')
            removeCollection('testcollection2')
            createCollection('testcollection')
            createCollection('testcollection2')
            var json2 = {'name': 'test'}
            json2 = save('testcollection2', json2);
            var json = {
                array: [json2._id],
                name: 'test'
            };
            json = save('testcollection', json);
            modelValid.parentObject = json
        }
    }

    QEjdb.ArrayPropertyModel {
        id: modelInvalid
    }

    QEjdb.ArrayPropertyModel {
        id: modelValid
        collection: 'testcollection'
        propertyCollection: 'testcollection2'
        arrayProperty: 'array'
        client: qejdbClient
        parentObject: testCase.parentObject
    }

    ListView {
        id: listView
        model: modelValid
        delegate: Item {
                property string theName: name
                Text {
                    text: theName
                }
            }
        }

    function test_Insert() {
        var json = {name: 'name'}
        listView.currentIndex = 0
        compare(listView.currentItem.theName, 'test')
        modelValid.insert(0, json)
        compare(listView.count, 2)
    }

    function test_Remove() {
        var json = {name: 'name'}
        modelValid.remove(0)
        compare(listView.count, 1)
    }

    function test_Properties() {

        compare(modelValid.arrayProperty, 'array')
        compare(modelValid.propertyCollection, 'testcollection2')
        compare(modelValid.collection, 'testcollection')
        compare(modelValid.parentObject.name, 'test')

    }
}
