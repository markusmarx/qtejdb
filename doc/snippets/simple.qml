//! [all]
import QtQuick 2.0
//! [import]
import QtEjdb 1.0
//! [import]

Rectangle {
    //! [client]
    QEjdbClient {
	id: client
        uri: 'file:test_db'
    }
    //! [client]
    //! [save]
    function saveBson() {
        client.createCollection('person')
        var json = client.save('person', {name: 'Edgar Marx', age: 5})
        console.log(json._id)
	var edgar = client.load('person', json._id)
    }
    //! [save]
}
//! [all]
