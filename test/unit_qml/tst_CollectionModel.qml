import QtQuick 2.0
import QEjdb 1.0 as QEjdb
import QtTest 1.0

TestCase {
    name: "QEjdbPlugin CollectionModel Test"

    QEjdb.Client {
        id: qejdbClient
        uri: 'file:tst_qejdplugincollectionmodel.db'
        autoCreateCollection: true
        Component.onCompleted: {
            connect();
            qejdbClient.createCollection('testcollection');
        }
    }

    QEjdb.CollectionModel {
        id: collectionModelValid
        client: qejdbClient
        query: {'name': 'test'}
        hints: {'$orderby': {'name': 1}}
        collection: 'testcollection'
    }

    QEjdb.CollectionModel {
        id: collectionModelInValid
    }

    ListView {
        id: listView
        model: collectionModelValid
    }

    function test_InsertItem() {
        collectionModelValid.insert({'name': 'test'}, 0)
    }

    function test_Properties() {
        compare(collectionModelValid.collection, 'testcollection')
        var query = collectionModelValid.query
        var hints = collectionModelValid.hints
        compare(query.name, 'test')
        compare(hints.$orderby.name, 1)

    }

}
