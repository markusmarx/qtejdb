import QtQuick 2.0
import QEjdb 1.0 as QEjdb
import QtTest 1.0

TestCase {
    name: "QEjdb Test"

    QEjdb.Client {
        id: qejdbClient
        uri: 'file:tst_qejdbmodel.db'
        autoCreateCollection: true
        Component.onCompleted: {
            connect();
        }
    }

    function init() {
        qejdbClient.createCollection('testcollection')

        for (var i = 0; i < 10; i++) {
            qejdbClient.save('testcollection',
                             {name: 'test', marker: i})
        }
    }

    function cleanup() {
        qejdbClient.removeCollection('testcollection')
    }

    function test_CreateCollection() {
        qejdbClient.createCollection('newCollection')

    }

    function test_Uri() {
        compare('file:tst_qejdbmodel.db', qejdbClient.uri)
    }

    function test_Query() {
        var jsonArray = qejdbClient.query('testcollection', {}, {})
        compare(jsonArray.length, 10)
        jsonArray = qejdbClient.query('testcollection', {marker:1}, {})
        compare(jsonArray.length, 1)
    }

    function test_QueryWithHint() {
        var jsonArray = qejdbClient.query('testcollection', {}, {$orderby: {marker: -1}})
        compare(jsonArray.length, 10);
        for (var i = 0; i < jsonArray.length; i++) {
            compare(jsonArray[i].marker, 9-i)
        }
    }

    function test_SaveLoadRemove() {
        // save json
        var json = qejdbClient.save('testcollection',
             {
                 'name':'test',
                 'date': new Date(),
                 'object': {
                                'street': 'Schulze Boysen Str. 10'
                            },
                 'array': ['1', 2, '3'],
                 'test' : 'test'
             })
        // load it by _id
        var json2 = qejdbClient.load('testcollection', json._id);
        json.object.street = 'Schnulze Boysen Str. 12';
        json.array.splice(1, 0, "5");
        qejdbClient.save('testcollection', json);
        json2 = qejdbClient.load('testcollection', json._id);
        compare(json._id, json2._id);
        // remove it
        qejdbClient.remove('testcollection', json._id);
        // should be null
        json2 = qejdbClient.load('testcollection', json._id);
        compare(null, json2)
    }



}
