import QtQuick 2.0
import QEjdb 1.0 as QEjdb
import QtTest 1.0

TestCase {
    name: "QEjdbPlugin CollectionModel Test"

    function cleanupTestCase() {
        qejdbClient.removeCollection('testcollection3')
    }

    QEjdb.Client {
        id: qejdbClient
        uri: 'file:tst_qejdplugincollectionmodel.db'
        autoCreateCollection: true
        Component.onCompleted: {
            connect();
            createCollection('testcollection3')
        }
    }

    QEjdb.CollectionModel {
        id: collectionModelValid
        client: qejdbClient
        query: {'name': 'test'}
        hints: {'$orderby': {'name': 1}}
        collection: 'testcollection3'
    }

    QEjdb.CollectionModel {
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

    function test_InsertItem() {
        collectionModelValid.insert({'name': 'test'}, 0)
        compare(listView.count, 1)
    }

    function test_Properties() {
        compare(collectionModelValid.collection, 'testcollection3')
        var query = collectionModelValid.query
        var hints = collectionModelValid.hints
        compare(query.name, 'test')
        compare(hints.$orderby.name, 1)
    }
}
