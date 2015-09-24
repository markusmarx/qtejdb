import QtQuick 2.0
import QtEjdb 1.0 as QtEjdb
import QtTest 1.0

TestCase {
    name: "QEjdbPlugin CollectionModel Test"

    function cleanupTestCase() {
        qejdbClient.removeCollection('testcollection3')
    }

    QtEjdb.Client {
        id: qejdbClient
        uri: 'file:tst_qejdplugincollectionmodel.db'
        autoCreateCollection: true
        Component.onCompleted: {
            connect();
            createCollection('testcollection3')
        }
    }

    QtEjdb.CollectionModel {
        id: collectionModelValid
        client: qejdbClient
        query: {'name': {'$begin': 'test'}}
        hints: {'$orderby': {'name': 1}}
        collection: 'testcollection3'
    }

    QtEjdb.CollectionModel {
        id: collectionModelInValid
        client: qejdbClient
    }

    ListView {
        id: listView
        model: collectionModelValid
        delegate: Text {
            text:name
        }
    }

    function test_Append() {
        collectionModelValid.append({'name': 'test2'})
        compare(listView.count, 1)
        var json = collectionModelValid.get(0)
        compare(json.name, 'test2');
    }

    function test_InsertAndGetItem() {
        collectionModelValid.reset()
        collectionModelValid.insert(0, {'name': 'test'})
        compare(listView.count, 2)
        compare(collectionModelValid.count, 2)
        var json = collectionModelValid.get(0)
        compare(json.name, 'test');
    }

    function test_Properties() {
        collectionModelValid.reset()
        compare(collectionModelValid.collection, 'testcollection3')
        var query = collectionModelValid.query
        var hints = collectionModelValid.hints
        var client = collectionModelValid.client
        compare(query.name.$begin, 'test')
        compare(hints.$orderby.name, 1)
        compare(client.id, qejdbClient.id)
    }

    function test_Set() {
        collectionModelValid.reset()
        collectionModelValid.set(0, {'name': 'test1'})
        collectionModelValid.reset()
        compare(listView.count, 2)
        var json = collectionModelValid.get(0)
        compare(json.name, 'test1');
    }

    function test_SetProperty() {
        collectionModelValid.reset()
        compare(listView.count, 2)
        collectionModelValid.setProperty(0, 'name', 'test3');
        collectionModelValid.reset()
        var json = collectionModelValid.get(1)
        compare(json.name, 'test3');
    }

}
