import QtQuick 2.3
import QEjdb 1.0 as QEjdb
import QtTest 1.0

TestCase {
    name: "QEjdb Test"

    QEjdb.QEjdbClient {
        id: qejdbClient
        uri: 'file:testmodel_db2'

        Component.onCompleted: {
            connect();

        }
    }

    function test_client() {
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
        console.log(json.id);
        console.log(json.date);
        console.log(json.name);

        var json2 = qejdbClient.load('testcollection', json.id);
        console.log(json2.id);
        console.log(json2.date);
        console.log(json2.name);
        console.log(json2.object.street);
        console.log(json2.array);
        json.object.street = 'Schnulze Boysen Str. 12';
        json.array.splice(1, 0, "5");
        qejdbClient.save('testcollection', json);
        json2 = qejdbClient.load('testcollection', json.id);
        console.log(json2.object.street);
        console.log(json2.array);
    }
}
