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


    function test_CreateCollection() {
        qejdbClient.createCollection('newCollection')
    }

    function test_Uri() {
        compare('file:tst_qejdbmodel.db', qejdbClient.uri)
    }

    function test_SaveLoadRemove() {

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
        var json2 = qejdbClient.load('testcollection', json._id);
        json.object.street = 'Schnulze Boysen Str. 12';
        json.array.splice(1, 0, "5");
        qejdbClient.save('testcollection', json);
        json2 = qejdbClient.load('testcollection', json._id);
        compare(json._id, json2._id);
        qejdbClient.remove('testcollection', json._id);
        json2 = qejdbClient.load('testcollection', json._id);
        compare(null, json2)
    }

}
